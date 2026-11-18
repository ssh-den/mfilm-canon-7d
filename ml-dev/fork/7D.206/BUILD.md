# Building M-Film for EOS 7D.206

Build the 7D card files locally from a clean checkout of this M-Film fork.
You also need a local clone of
[magiclantern_simplified_7d](https://github.com/ssh-den/magiclantern_simplified_7d)
and the `M-Film-tables.zip` asset from the
[upstream v0.2.3 release](https://github.com/HNGSeries/mfilm-canon-film-simulation/releases/tag/v0.2.3).

Install Arm GNU Toolchain 14.2.rel1, GNU Make, a host C compiler, Python 3
with NumPy, docutils (`rst2html`) and setuptools, and `zip`. On macOS, put
GNU Make, GNU coreutils and GNU sed ahead of the system tools in `PATH`.

```sh
export ARM_BINPATH=/path/to/arm-gnu-toolchain/bin
python3 ml-dev/build_7d206.py ../magiclantern_simplified_7d /path/to/M-Film-tables.zip \
    /path/to/M-Film-camera-7D.206.zip -j 8
```

The output path must be new. If you place the input ZIP inside the checkout,
use the ignored `ml-dev/out/` directory and pass its path to the command.

The builder checks out the pinned ML revision in a temporary clone and
applies the patches in this directory. It builds `autoexec.bin`, `7D_206.sym`
and `filmos.mo` together and checks the module's imports and ARM relocations.
It validates the 68 colour tables against the film catalog and develops the
shelf previews from the upstream sample. The source revisions and checksums
are in the output ZIP.

See [the card instructions](CARD-README.txt) for the camera test. Canon EOS
7D firmware 2.0.6 and a working bootable CF card are required.
