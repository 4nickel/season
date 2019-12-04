#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "date.h"
#include "meeus.h"

const char *argp_program_version = "0.1";
const char *argp_program_bug_address = "<unused@domain>";
static char args_doc[] = "SEASON YEAR";
static char doc[] = "Calculate the date and time of solstices and equinoctes like Jean Meeus.";

struct arguments
{
    char *args[2];
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    switch (key){
        case ARGP_KEY_ARG:
            if(state->arg_num >= 2)
                argp_usage(state);
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if(state->arg_num < 2)
                argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp_option options[] = {
    { 0 }
};
static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

enum season {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
};

static const enum season SEASONS[] = {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
};

static const int YEAR_LIMIT = 20000;
static const char *SEASON_ALL = "all";
static const char *SEASON_NAMES[] = {
    [SPRING] = "spring",
    [SUMMER] = "summer",
    [AUTUMN] = "autumn",
    [WINTER] = "winter",
};

static int parse_command(struct arguments *a, int *year, const enum season **season, int *count)
{
         if(strcmp(a->args[0], SEASON_ALL)       == 0){ *season = SEASONS;     *count = 4; }
    else if(strcmp(a->args[0], SEASON_NAMES[0])  == 0){ *season = &SEASONS[0]; *count = 1; }
    else if(strcmp(a->args[0], SEASON_NAMES[1])  == 0){ *season = &SEASONS[1]; *count = 1; }
    else if(strcmp(a->args[0], SEASON_NAMES[2])  == 0){ *season = &SEASONS[2]; *count = 1; }
    else if(strcmp(a->args[0], SEASON_NAMES[3])  == 0){ *season = &SEASONS[3]; *count = 1; }
    else {
        return 1;
    }

    *year = atoi(a->args[1]);
    if(*year > YEAR_LIMIT || *year < -YEAR_LIMIT) {
        return 1;
    }
    return 0;
}

void print_season_datetime(enum season season, int year)
{
    struct datetime dt;
    switch(season){
    case SPRING: datetime_from_astronomical_jdn(&dt, spring_equinox(year)); break;
    case SUMMER: datetime_from_astronomical_jdn(&dt, summer_solstice(year)); break;
    case AUTUMN: datetime_from_astronomical_jdn(&dt, autumn_equinox(year)); break;
    case WINTER: datetime_from_astronomical_jdn(&dt, winter_solstice(year)); break;
    }
    datetime_print(&dt);
}

int main(int argc, char **argv)
{
    struct arguments arguments = { .args = { }, };
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    int year;
    const enum season *seasons;
    int count;
    if(parse_command(&arguments, &year, &seasons, &count))
        return EXIT_FAILURE;

    for(int i=0; i<count; i++){
        print_season_datetime(seasons[i], year);
    }

    return EXIT_SUCCESS;
}
