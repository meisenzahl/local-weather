#include <stdio.h>
#include <string.h>

#include "yahoo_weather_api.h"

void usage();

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        if ((strcmp(argv[1], "--metric") == 0) || (strcmp(argv[1], "-c") == 0)) {
            size_t woeid = getWOEID();

            if (woeid) {
                printForecast(woeid, "c");
            }
            else {
                printf("Couldn't get woeid\n");
            }
        }
        else if ((strcmp(argv[1], "--us") == 0) || (strcmp(argv[1], "-f") == 0)) {
            size_t woeid = getWOEID();

            if (woeid) {
                printForecast(woeid, "f");
            }
            else {
                printf("Couldn't get woeid\n");
            }
        }
        else if (((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--woeid") == 0)) && argv[2] != NULL) {
            size_t woeid = (size_t) atoi(argv[2]);

            if (woeid) {
                printForecast(woeid, "c");
            }
            else {
                printf("Couldn't get woeid\n");
            }
        }
        else {
            usage();
        }
    }
    else {
        usage();
    }
    
    /* suppress unused variable warning*/
    conditions[0] = NULL;

    return 0;
}

void usage()
{
    printf("Usage: local-weather [OPTION]\n");
    printf("       -c, --metric           show units in metric\n");
    printf("       -f, --us               show units in us format\n");
    printf("       -w, --woeid <woeid>    use this woeid\n");
}
