# libbcd

The BCD library contains some useful functions for the handling of binary coded decimal dates and times.

## Definitions

A (two digit) BCD char is an 8 bit number, which encodes a number between 0 and 99 inclusive.
The least significant four bits encode the ones place as a number between 0 and 9 inclusive, the most significant four bits encode the tens place as a number between 0 and 9 inclusive.

A BCD encoded time is an array of six BCD chars, which encode day, month, year, hour minute and second of a specific time.

The positions of the individual fields in the array are specified by the constants in `bcd.h`.

    #define BCD_HOUR   0
    #define BCD_MINUTE 1
    #define BCD_SECOND 2
    #define BCD_DAY    3
    #define BCD_MONTH  4
    #define BCD_YEAR   5

Beware, that the year is encoded as number between 0 and 99 inclusive, so you have to add 2000 to it’s value.
This limits possible dates to within January 1st 2000 and December 31st 2099.

## Functions

### bcd_int(c)

A macro which converts a two digit BCD char into an integer.
Beware, that `c` is evaluated twice, so constructs like `bcd_int(c++)` are undefined.

### int_bcd(i)

A macro which converts an integer into a two digit BCD char.
Beware, that `c` is evaluated twice, so constructs like `int_bcd(i++)` are undefined.

### is_bcd(c)

A macro which checks, if a char is a valid two digit BCD char.
Beware, that `c` is evaluated twice, so constructs like `is_bcd(c++)` are undefined.

### int bcd_parse(const char *input, char *bcd)

Parses date and time in text form from the zero terminated `input` and stores the result as BCD encoded time into `bcd`.

Several formats are supported:

* `dd mm yyyy hh mm ss`

  Six numbers seperated by spaces.
  In this case the date comes first starting with the day.
  If the seconds are zero, they may be omitted.

* `hh:mm:ss`

  The time may be specified with colons instead of spaces.
  Again if the seconds are omitted, zero will be assumed.

* `dd.mm.yyyy`

  The date may be specified with dots instead spaces.
  Beware, that the first number is the day, not the month.

* `yyyy-mm-dd`

  The date may be specified in ISO format with dashes instead of spaces beginning with the year.

* `yyyy-ddd`

  The date may be specified as Julian day.
  January 1st has number 1, February 3rd has number 35, etc.
  Day numbers after February vary, depending on the year beeing a leapyear.
  So December 31st is either day 366 in a leapyear or day 365 otherwise.

* Date and time may be swapped, if at least one of them uses seperators apart from spaces.

* Leading zeros are allowed but not required in any format.

* The year may be given with either two or four digits.

If the string denotes valid date and time, the function returns zero.
If there was an error while parsing, `-1` is returned.

### int bcd_valid(const char *bcd)

Checks if the BCD encoded time is a valid time.
Returns `1` if the supplied time is valid, `0` otherwise.

### long long bcd_diff(const char *t1, const char *t2)

Returns the number of seconds from `t1` to `t2`.
`t1` and `t2` are BCD encoded times.
If `t2` is before `t1` the result will be negative.

### int bcd_weekday(const char *bcd)

Returns the day of the week for the given BCD encoded time.
Sunday is 1, Monday is 2 etc., Saturday is 7.

### long bcd_mjd(const char *bcd)

Returns the days since January 1st 2000.
