# The “kumy2seed” Program

The `kumy2seed` program converts `.muk1` files to the MiniSEED format.

## Usage

    $ kumy2seed [options] file.muk1

This converts `file.muk1` to MiniSEED.
The resulting files will be placed into a folder `file`.
Every channel will get a seperate set of `.seed` files.
The files will be named after their respective start time.

The output will be sliced into parts of 90 minutes up to one day, depending on the sample rate.

### Options

* `-n`, `--no-compression`: Do not compress the resulting `.seed` files.
  Use this option if your software does not support compressed MiniSEED files.

* `-h`, `--help`: Print a short help message.
