#ifndef _DATE_H_
#define _DATE_H_

#ifdef PAPAL_CALENDAR
#define LAST_JULIAN_DAY_NUMBER  2299160L   // The last day of the Julian calendar (Pope Gregory XIII's decree)
#else
#define LAST_JULIAN_DAY_NUMBER  2361221L   // The last day of the Julian calendar (British/American usage)
#endif

#define GRG_DAYS_PER_400_YEARS 146097L
#define JUL_DAYS_PER_400_YEARS 146100L
#define GRG_FUDGE 31L
#define JUL_FUDGE  1L
#define GMT_DAYS 68569L
#define JUL_OFFSET 38L

struct datetime
{
    int YY, MM, DD, hh, mm;
};

void datetime_print(struct datetime *dt);
void datetime_from_astronomical_jdn(struct datetime *dt, double jdna);

#endif
