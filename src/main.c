#include <stdio.h>
#include <string.h>

#include "yahoo_weather_api.h"

void usage();

int main(int argc, char *argv[])
{
    if (argc == 2) {
        if ((strcmp(argv[1], "--metric") == 0) || (strcmp(argv[1], "-c") == 0)) {
            size_t woeid = getWOEID();

            if (woeid) {
                struct Weather weather = getWeather(woeid, "c");

                printf("temperature: %zd°%s\n", weather.temp, weather.units.temperature);
            }
            else {
                printf("Couldn't get woeid\n");
            }
        }
        else if ((strcmp(argv[1], "--us") == 0) || (strcmp(argv[1], "-f") == 0)) {
            size_t woeid = getWOEID();

            if (woeid) {
                struct Weather weather = getWeather(woeid, "f");

                printf("temperature: %zd°%s\n", weather.temp, weather.units.temperature);
            }
            else {
                printf("Couldn't get woeid\n");
            }
        }
        else if ((strcmp(argv[1], "city") == 0)) {
            size_t woeid = getWOEID();

            if (woeid) {
                struct Weather weather = getWeather(woeid, "c");

                printf("city: %s\n", weather.city);
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
    
    

    return 0;
}

void usage()
{
    printf("Usage: local-weather [OPTION]\n");
    printf("       -c, --metric    show units in metric\n");
    printf("       -f, --us        show units in us format\n");
}
