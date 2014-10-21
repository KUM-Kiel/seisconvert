#include "bcd.h"

#define set_check(x, i) if ((x) == -1) (x) = (i); else return -1
#define leapyear(year) ((year) % 400 == 0 || ((year) % 4 == 0 && (year) % 100 != 0))
#define longmonth(month) ((((month) >> 3) ^ (month)) & 1)
int bcd_parse(const char *input, char *bcd)
{
  int day = -1, month = -1, year = -1;
  int hour = -1, minute = -1, second = -1;
  int got_digits = 0, i = 0;
  int state = 0;
  if (!input || !bcd) return -1;
  while (1) {
    if ('0' <= *input && *input <= '9') {
      got_digits += 1;
      i = i * 10 + (*input - '0');
    } else if (*input == ' ' || *input == 0) {
      if (got_digits) {
        switch (state) {
          case 0: set_check(day, i); break;
          case 1: set_check(month, i); break;
          case 2: set_check(year, i); break;
          case 3: set_check(hour, i); break;
          case 4: set_check(minute, i); break;
          case 5: set_check(second, i); break;
          case 7: set_check(minute, i); set_check(second, 0); state = 5; break;
          case 8: set_check(month, 1); set_check(day, i);
            while (month < 12 && (day > 31 || (day > 30 && !longmonth(month)) ||
                (month == 2 && (day > 29 || (day > 28 && !leapyear(year)))))) {
              day -= month == 2 ? (28 + leapyear(year)) : (30 + longmonth(month));
              month += 1;
            }
            state = 2;
            break;
          case 9: set_check(day, i); state = 2; break;
          default: return -1;
        }
        state = (state + 1) % 6;
      }
      if (*input == 0) break;
      got_digits = 0;
      i = 0;
    } else if (*input == '.' && got_digits) {
      if (state == 0 || state == 3) {
        set_check(day, i);
        state = 6;
      } else if (state == 6) {
        set_check(month, i);
        state = 2;
      } else return -1;
      got_digits = 0;
      i = 0;
    } else if (*input == ':' && got_digits) {
      if (state == 0 || state == 3) {
        set_check(hour, i);
        state = 7;
      } else if (state == 7) {
        set_check(minute, i);
        state = 5;
      } else return -1;
      got_digits = 0;
      i = 0;
    } else if (*input == '-' && got_digits) {
      if (state == 0 || state == 3) {
        set_check(year, i);
        state = 8;
      } else if (state == 8) {
        set_check(month, i);
        state = 9;
      } else return -1;
      got_digits = 0;
      i = 0;
    } else {
      return -1;
    }
    if (got_digits > 4) return -1;
    ++input;
  }
  if (year < 100) year += 2000;
  if (second == -1) second = 0;
  /* Validate hour. */
  if (0 <= hour && hour <= 23)
    bcd[BCD_HOUR] = int_bcd(hour);
  else return -1;
  /* Validate minute. */
  if (0 <= minute && minute <= 59)
    bcd[BCD_MINUTE] = int_bcd(minute);
  else return -1;
  /* Validate second. */
  /* XXX: Leapseconds occur only on 31.3., 30.6., 30.9. and 31.12. */
  if (0 <= second && (second <= 59 || (hour == 23 && minute == 59 && second <= 60)))
    bcd[BCD_SECOND] = int_bcd(second);
  else return -1;
  /* Validate day. */
  if (1 <= day && (day <= 28 || (leapyear(year) && day <= 29) ||
    (month != 2 && day <= 30) || (longmonth(month) && day <= 31)))
    bcd[BCD_DAY] = int_bcd(day);
  else return -1;
  /* Validate month. */
  if (1 <= month && month <= 12)
    bcd[BCD_MONTH] = int_bcd(month);
  else return -1;
  /* Validate year. */
  if (2000 <= year && year <= 2099)
    bcd[BCD_YEAR] = int_bcd(year - 2000);
  else return -1;
  return 0;
}
