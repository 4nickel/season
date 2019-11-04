#include <stdio.h>
#include <math.h>
#include "date.h"

/* Format and print a struct datetime. */
void datetime_print(struct datetime *dt)
{
    printf("%04d-%02d-%02d@%02d:%02d\n", dt->YY, dt->MM, dt->DD, dt->hh, dt->mm);
}

/* Choose the correct conversion factors and clock-time. */
void calculate_factors_and_clock(double jdna, long *d4c, long *d4k, long *days, double *clock)
{
    double jdnc = jdna+0.5; // astronomical to chronological
    int jdn = floor(jdnc);
    *clock = jdnc-jdn;
    if(jdn <= LAST_JULIAN_DAY_NUMBER){
        *days = jdn + GMT_DAYS + JUL_OFFSET;
        *d4c = JUL_DAYS_PER_400_YEARS;
        *d4k = JUL_DAYS_PER_400_YEARS*10L + JUL_FUDGE;
    }else{
        *days = jdn + GMT_DAYS;
        *d4c = GRG_DAYS_PER_400_YEARS;
        *d4k = GRG_DAYS_PER_400_YEARS*10L + GRG_FUDGE;
    }
}

/* Convert julian day number to struct datetime. */
void convert_jdn_to_ymd(long days, long d4c, long d4k, long *y, long *m, long *d)
{
    long century = 4L*days / d4c;
    days = days - (d4c*century + 3L)/4L;
    *y = 4000L*(days+JUL_FUDGE) / d4k;
    days = days - 1461L*(*y) / 4L + GRG_FUDGE;
    *m = 80L*days / 2447L;
    *d = days - 2447L*(*m)/80L;
    days = (*m)/11L;
    *m = (*m)+2L - 12L*days;
    *y = 100L*(century-49L)+(*y)+days;
}

void datetime_set(struct datetime *dt, int yy, int mm, int dd, double clock)
{
    double hh = clock*24.0;
    dt->YY = yy > 0 ? yy : yy-1; // no year 0
    dt->MM = mm;
    dt->DD = dd;
    dt->hh = hh;
    dt->mm = ((hh - dt->hh)*60.0);
}

void datetime_from_astronomical_jdn(struct datetime *dt, double jdn)
{
    long d4c, d4k, days, mm, dd, yy;
    double clock;
    calculate_factors_and_clock(jdn, &d4c, &d4k, &days, &clock);
    convert_jdn_to_ymd(days, d4c, d4k, &yy, &mm, &dd);
    datetime_set(dt, yy, mm, dd, clock);
}
