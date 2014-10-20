#pragma once

/* Positions of the values in a BCD encoded time. */
#define BCD_HOUR   0
#define BCD_MINUTE 1
#define BCD_SECOND 2
#define BCD_DAY    3
#define BCD_MONTH  4
#define BCD_YEAR   5

/* Converts a two digit BCD char into an integer. */
#define bcd_int(c) (((c) & 15) + (((c) >> 4) & 15) * 10)

/* Checks, if a char is a valid two digit BCD char. */
#define is_bcd(c) ((c) % 16 < 10 && (c) / 16 < 10)

/* Parses a zero terminated string into a BCD encoded time.
 * Supports the following formats:
 *   dd mm yy hh mm ss
 *   dd.mm.yy hh:mm:ss
 *   yy-mm-dd hh:mm:ss */
extern int bcd_parse(const char *input, char *bcd);

/* Checks if the BCD encoded time is a valid time. Returns 1 if the supplied
 * time is valid, 0 otherwise. */
extern int bcd_valid(const char *bcd);

/* Returns the number of seconds from t1 to t2. t1 and t2 are BCD encoded times. */
extern long long bcd_diff(const char *t1, const char *t2);

/* Returns the day of the week for the given BCD encoded time.
 * Sunday is 1, Monday is 2, ... Saturday is 7. */
extern int bcd_weekday(const char *bcd);

/* Returns the days since 1.1.2000. */
extern long bcd_mjd(const char *bcd);
