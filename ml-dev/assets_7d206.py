"""Assemble and check the colour tables, fonts and previews for 7D.206."""

import hashlib
import os
from pathlib import Path
import struct
import subprocess
import sys
import zipfile


HERE = Path(__file__).resolve().parent
FOS = HERE / 'filmos'
TABLES_SHA256 = '1ecb603dadb6c4f47b87c604a6709ca5550e0ad6a019cefc5b4b46fc0bf85fc2'


def build_assets(tables, card, work):
    if hashlib.sha256(tables.read_bytes()).hexdigest() != TABLES_SHA256:
        raise ValueError('Expected the v0.2.3 M-Film-tables.zip archive')

    sys.path.insert(0, str(FOS / 'tools'))
    import genfilms

    ml = card / 'ML'
    data = ml / 'data'
    darkroom = ml / 'DARKROOM'
    samples = ml / 'SAMPLES'
    for folder in (data, darkroom, samples):
        folder.mkdir(exist_ok=True)

    builtins = genfilms.builtin_keys()
    keys = builtins + [key for key in genfilms.FILMS if key not in builtins]
    if len(keys) != 68 or len(set(keys)) != 68:
        raise ValueError('Expected 68 unique film stocks')
    subprocess.run([sys.executable, FOS / 'tools/genfilms.py', data / 'FOSFILMS.BIN'], check=True)
    with zipfile.ZipFile(tables) as archive:
        index = archive.read('TABLES.txt')
        lines = index.decode().splitlines()
        rows = [line.split(maxsplit=2) for line in lines if line.startswith('LOOK')]
        entries = {key: (name, *description.rsplit(maxsplit=1)) for name, key, description in rows}
        if len(rows) != 68 or set(entries) != set(keys):
            raise ValueError('Film catalog and table archive have different stocks')
        release_index = [line for line in lines if line.startswith('#')]
        for i, key in enumerate(keys):
            name, source, digest = entries[key]
            table = archive.read(name)
            look = genfilms.look_hash(key, genfilms.look_bytes(genfilms.FILMS[key]))
            if (hashlib.sha256(table).hexdigest() != digest or len(table) != 8 + 33**3 * 3
                    or table[:8] != struct.pack('<4sI', b'FOL1', look)):
                raise ValueError(f'Film table does not match the catalog: {key}')
            target = f'LOOK{i:02}.LUT'
            (darkroom / target).write_bytes(table)
            release_index.append(f'{target}  {key}  {source}  {digest}')
        (card / 'TABLES.txt').write_text('\n'.join(release_index) + '\n')
        (card / 'THIRD_PARTY-TABLES.txt').write_bytes(archive.read('THIRD_PARTY.txt'))

    fontblob = work / 'fontblob'
    subprocess.run(['cc', '-O2', '-I', FOS, FOS / 'tools/fontblob.c', '-o', fontblob], check=True)
    subprocess.run([fontblob, data / 'FOSFONT.BIN'], check=True)

    devtest = work / 'devtest'
    sources = ('sim/devtest.c', 'sim/port_files.c', 'sim/films.c', 'fos_dev.c', 'fos_print.c',
               'fos_looks.c', 'fos_film.c', 'fos_gfx.c', 'fos_jpeg.c')
    subprocess.run(['cc', '-O2', '-std=gnu99', '-DFOS_DEV_TEST', '-I', FOS,
                    *(FOS / name for name in sources), '-lm', '-o', devtest], check=True)
    sample = work / 'SAMPLE.CR2'
    sample.symlink_to(HERE / 'films/sample/SAMPLE.CR2')
    env = dict(os.environ, FOS_FILMS='data/FOSFILMS.BIN', FOS_LUT_DIR='DARKROOM')
    for name in ('FOS_MEM_KB', 'FOS_SIZE', 'FOS_MIX', 'FOS_BARE'):
        env.pop(name, None)
    subprocess.run([devtest, '-S', '../../SAMPLE.CR2', 'SAMPLES'], cwd=ml, env=env, check=True)
    if len(list(samples.glob('F*.BMP'))) != len(keys):
        raise ValueError('Incomplete film previews')
    print('Verified 68 film tables and developed 68 previews', flush=True)
