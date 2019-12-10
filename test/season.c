#include <cheat.h>

#include "date.h"
#include "meeus.h"

CHEAT_DECLARE(
    const int YEAR = 2019;
    struct datetime SPRING = { 2019,  3, 20, 21, 57 };
    struct datetime SUMMER = { 2019,  6, 21, 15, 48 };
    struct datetime AUTUMN = { 2019,  9, 23,  7, 34 };
    struct datetime WINTER = { 2019, 12, 22,  4,  7 };
)

CHEAT_TEST(check_spring,
    struct datetime dt;
    datetime_from_astronomical_jdn(&dt, spring_equinox(YEAR));
    datetime_print(&dt);
    cheat_assert(datetime_equals(&dt, &SPRING));
)
CHEAT_TEST(check_summer,
    struct datetime dt;
    datetime_from_astronomical_jdn(&dt, summer_solstice(YEAR));
    datetime_print(&dt);
    cheat_assert(datetime_equals(&dt, &SUMMER));
)
CHEAT_TEST(check_autumn,
    struct datetime dt;
    datetime_from_astronomical_jdn(&dt, autumn_equinox(YEAR));
    datetime_print(&dt);
    cheat_assert(datetime_equals(&dt, &AUTUMN));
)
CHEAT_TEST(check_winter,
    struct datetime dt;
    datetime_from_astronomical_jdn(&dt, winter_solstice(YEAR));
    datetime_print(&dt);
    cheat_assert(datetime_equals(&dt, &WINTER));
)
