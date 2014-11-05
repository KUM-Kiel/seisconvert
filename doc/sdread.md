# The “sdread” Program

The `sdread` program can be used to read the contents of an SD card, which has been formatted with KUM’s internal format.

The programm will create a `.muk1` file, that may be converted to various output formats.
Additionally a log file will be created, which contains some Metadata as well as events that occured during the recording.

## Usage

    $ sdread /dev/sdx

Instead of `/dev/sdx` you should supply the device file for the SD card, which will probably be `/dev/sdb` if you have only one hard disk installed in your system.
If you are running Mac OS X it will probably be `/dev/disk1`.
An image created with `dd` will work equally well.

If the program aborts with a ‘permission denied’ error, you do not have the required permission to open the card.
In this case you should run the program as root (preferably via `sudo`) or add yourself to the required system group.

## Output

The program creates a `.muk1` file in which all the data is stored.
This file is the starting point for further processing.
When a `.muk1` file is created, it’s name will be printed.

Alongside a new log file is started every time a new `.muk1` file is created.
It has the same name as the `.muk1` file except of it’s extension, which will be `.log.txt`.

Additionally three CSV files will be created, containing the battery voltage, the temperature and the humidity.
They will have the same name as the `.muk1` file, but they will have the extensions `.voltage.csv`, `.temperature.csv` and `.humidity.csv`.

A progress bar will be displayed, which shows the estimated overall progress, as reading a large SD card might take a very long time.
