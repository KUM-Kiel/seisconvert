# The “kumy2wav” Program

The `kumy2wav` program converts `.muk1` files to WAV.

## Usage

    $ kumy2wav file.muk1

This converts `file.muk1` to WAV.
The resulting files will be placed into a folder `file`.
The files will be named after their respective start time.

The output will be sliced into parts of 90 minutes up to one day, depending on the sample rate.
