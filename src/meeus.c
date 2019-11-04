#include "meeus.h"

/*
 * The "magic" numbers below are found in the book "Astronomical Algorithms"
 * by Jean Meeus and can verified there, alongside an explanation of the underlying
 * mathematics.
 */

double meeus_scale(double year)
{ return (year-2000.0)/1000.0; }

double meeus_winter_solstice(double m)
{ return 2451900.05952 + 365242.74049*m - 0.06223*m*m - 0.00823*m*m*m + 0.00032*m*m*m*m; }

double meeus_summer_solstice(double m)
{ return 2451716.56767 + 365241.62603*m + 0.00325*m*m + 0.00888*m*m*m - 0.00030*m*m*m*m; }

double meeus_spring_equinox(double m)
{ return 2451623.80984 + 365242.37404*m + 0.05169*m*m - 0.00411*m*m*m - 0.00057*m*m*m*m; }

double meeus_autumn_equinox(double m)
{ return 2451810.21715 + 365242.01767*m - 0.11575*m*m + 0.00337*m*m*m + 0.00078*m*m*m*m; }

double winter_solstice(double year)
{ return meeus_winter_solstice(meeus_scale(year)); }

double summer_solstice(double year)
{ return meeus_summer_solstice(meeus_scale(year)); }

double spring_equinox(double year)
{ return meeus_spring_equinox(meeus_scale(year)); }

double autumn_equinox(double year)
{ return meeus_autumn_equinox(meeus_scale(year)); }
