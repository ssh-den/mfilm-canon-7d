#!/usr/bin/env python3
"""Build the 7D.206 card files from the pinned Magic Lantern commit."""

import argparse
import hashlib
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import zipfile


HERE = Path(__file__).resolve().parent
PORT = HERE / 'fork/7D.206'
ML_REVISION = '6ada3b49a5c4a0d01a1850771d4579b8ea4d9580'


def run(*args, **kwargs):
    subprocess.run(args, check=True, **kwargs)


def read(*args):
    return subprocess.check_output(args, text=True).strip()


def verify_module(module, symbols, tool):
    exports = {line.split()[1] for line in symbols.read_text().splitlines()}
    imports = {line.split()[-1] for line in read(tool('nm'), '-u', module).splitlines()}
    definitions = {line.split()[-1] for line in read(tool('nm'), '-g', '--defined-only', module).splitlines()}
    if imports - exports or definitions & exports:
        raise ValueError(f'Module symbols missing: {sorted(imports - exports)}. '
                         f'Conflicting definitions: {sorted(definitions & exports)}')

    sections = read(tool('readelf'), '-SW', module)
    if '.ARM.exidx' in sections or '.ARM.extab' in sections:
        raise ValueError('Unwind sections in filmos.mo')
    allowed = {'R_ARM_ABS32', 'R_ARM_CALL', 'R_ARM_JUMP24', 'R_ARM_V4BX'}
    for line in read(tool('readelf'), '-rW', module).splitlines():
        fields = line.split()
        if len(fields) < 3 or not fields[2].startswith('R_ARM_'):
            continue
        kind = fields[2]
        if kind not in allowed or (kind in {'R_ARM_CALL', 'R_ARM_JUMP24'} and fields[4] in imports):
            raise ValueError(f'Unsupported module relocation: {line}')
    if not {'__module_info_filmos', '__module_config_filmos', 'filmos_overlays'} <= definitions:
        raise ValueError('Missing module entry points')
    print(f'Checked {len(imports)} module imports and all ARM relocations', flush=True)


def build_core(args, work, version):
    ml = work / 'ml'
    run('git', 'clone', '--quiet', '--no-hardlinks', '--no-checkout', args.ml.resolve(), ml)
    run('git', '-C', ml, 'checkout', '--quiet', '--detach', ML_REVISION)
    patches = sorted(PORT.glob('*.patch'))
    run('git', '-C', ml, 'apply', '--check', '--whitespace=error', *patches)
    run('git', '-C', ml, 'apply', *patches)

    def tool(name):
        return args.arm_binpath / f'arm-none-eabi-{name}'

    make = [args.make, f'-j{args.jobs}', f'ARM_BINPATH={args.arm_binpath}',
            f'READELF={tool("readelf")}']
    if mac_stat := shutil.which('gstat'):
        make.append(f'STAT_CMD={mac_stat} -c "%n: %s bytes"')
    platform = ml / 'platform/7D.206'
    run(*make, '-C', platform)

    module = ml / 'modules/filmos'
    module.mkdir()
    for name in ('Makefile', 'README.rst', 'filmroll.c', 'filmos_mod.h', 'compat.h'):
        shutil.copy2(PORT / name, module)
    shutil.copy2(HERE / 'fork/src/filmroll.h', module)
    for source in (HERE / 'filmos').glob('fos*.[ch]'):
        shutil.copy2(source, module)
    (module / 'fos_version.h').write_text(f'#define MFILM_VERSION "{version}-7D206"\n')
    run(*make, '-C', module)

    card = work / 'card'
    with zipfile.ZipFile(platform / 'build/magiclantern.zip') as archive:
        archive.extractall(card, [name for name in archive.namelist()
                                 if name == 'autoexec.bin' or name.startswith('ML/')])
    binaries = card / 'ML/modules'
    filmos = module / 'build/filmos.mo'
    verify_module(filmos, binaries / '7D_206.sym', tool)
    shutil.copy2(filmos, binaries)
    settings = card / 'ML/SETTINGS'
    settings.mkdir(exist_ok=True)
    (settings / 'filmos.en').touch()
    shutil.copy2(ml / 'COPYING', card / 'COPYING-ML')
    return card


def make_zip(card, output):
    files = sorted(path for path in card.rglob('*') if path.is_file())
    forbidden = {'.fir', '.cr2', '.log', '.lck', '.cfg', '.zip'}
    for path in files:
        if path.suffix.lower() in forbidden:
            raise ValueError(f'Unexpected file in card files: {path.relative_to(card)}')
        if path.suffix.lower() == '.mo' and b'Build user\0' in path.read_bytes():
            raise ValueError(f'Build host metadata in {path.relative_to(card)}')
    sums = ''.join(f'{hashlib.sha256(path.read_bytes()).hexdigest()}  {path.relative_to(card).as_posix()}\n'
                   for path in files)
    with output.open('xb') as result, zipfile.ZipFile(result, 'w', zipfile.ZIP_DEFLATED) as archive:
        for path in files:
            archive.write(path, path.relative_to(card).as_posix())
        archive.writestr('SHA256SUMS', sums)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('ml', type=Path, help='local checkout of ssh-den/magiclantern_simplified_7d')
    parser.add_argument('tables', type=Path, help='upstream v0.2.3 M-Film-tables.zip')
    parser.add_argument('output', type=Path, help='new output ZIP')
    parser.add_argument('--make', default='make', help='GNU make executable')
    parser.add_argument('--arm-binpath', type=Path, default=os.environ.get('ARM_BINPATH', '/usr/bin'))
    parser.add_argument('--jobs', '-j', type=int, default=1)
    args = parser.parse_args()
    args.arm_binpath = args.arm_binpath.resolve()
    if args.output.exists():
        parser.error('output already exists')
    if read('git', '-C', HERE, 'status', '--porcelain'):
        parser.error('build from a clean M-Film checkout')

    from assets_7d206 import build_assets

    commit = read('git', '-C', HERE, 'rev-parse', 'HEAD')
    version = (HERE.parent / 'VERSION').read_text().strip()
    with tempfile.TemporaryDirectory(prefix='mfilm-7d206-') as directory:
        work = Path(directory)
        card = build_core(args, work, version)
        build_assets(args.tables.resolve(), card, work)
        shutil.copy2(HERE.parent / 'LICENSE', card / 'COPYING-MFILM')
        shutil.copy2(HERE.parent / 'THIRD_PARTY.md', card)
        shutil.copy2(HERE / 'filmos/fonts/OFL.txt', card / 'Barlow-OFL.txt')
        shutil.copy2(PORT / 'CARD-README.txt', card / 'M-FILM-README.txt')
        (card / 'SOURCE.txt').write_text(
            f'M-Film https://github.com/ssh-den/mfilm-canon-7d/tree/{commit}\n'
            f'Magic Lantern https://github.com/ssh-den/magiclantern_simplified_7d/tree/{ML_REVISION}\n'
            'Apply ml-dev/fork/7D.206/*.patch from the M-Film source when building Magic Lantern.\n')
        make_zip(card, args.output)
    print(args.output.resolve())


if __name__ == '__main__':
    sys.exit(main())
