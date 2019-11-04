#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "date.h"
#include "meeus.h"

enum season { SPRING, SUMMER, AUTUMN, WINTER };

int parse_arguments(int argc, char **argv, int *year, enum season *season)
{
    if(argc != 3 || strlen(argv[1]) > 7) return 1;

    *year = atoi(argv[2]);
         if(strcmp(argv[1], "spring") == 0){ *season = SPRING; }
    else if(strcmp(argv[1], "summer") == 0){ *season = SUMMER; }
    else if(strcmp(argv[1], "autumn") == 0){ *season = AUTUMN; }
    else if(strcmp(argv[1], "winter") == 0){ *season = WINTER; }
    else{ return 1; }
    return 0;
}

int main(int argc, char **argv)
{
    struct datetime dt;
    int year;
    enum season season;

    if(parse_arguments(argc, argv, &year, &season))
        return EXIT_FAILURE;

    switch(season){
    case SPRING: datetime_from_astronomical_jdn(&dt, spring_equinox(year)); break;
    case SUMMER: datetime_from_astronomical_jdn(&dt, summer_solstice(year)); break;
    case AUTUMN: datetime_from_astronomical_jdn(&dt, autumn_equinox(year)); break;
    case WINTER: datetime_from_astronomical_jdn(&dt, winter_solstice(year)); break;
    }

    datetime_print(&dt);
    return EXIT_SUCCESS;
}
