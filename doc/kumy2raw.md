# The “kumy2raw” Program

The `kumy2raw` program converts `.muk1` files to raw sample data.

## Usage

    $ kumy2raw file.muk1

This converts `file.muk1` to raw sample data.
The resulting files will be placed into a folder `file`.
Every channel will get a seperate set of `.raw` files.
The files will be named after their respective start time.

The files contain the raw 32 bit sample data as big endian encoded integers.

The output will be sliced into parts of 90 minutes up to one day, depending on the sample rate.
