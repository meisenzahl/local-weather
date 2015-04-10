#include <stdio.h>
#include <string.h>

#include "yahoo_weather_api.h"

int main(int argc, char *argv[])
{
    if (argc == 2) {
        if (strcmp(argv[1], "metric") == 0) {
            size_t woeid = getWOEID();

            if (woeid) {
                struct Weather weather = getWeather(woeid);

                printf("temperature: %zd°%s\n", weather.temp, weather.units.temperature);
            }
            else {
                printf("Couldn't get woeid\n");
            }
        }
        else if (strcmp(argv[1], "us") == 0) {
            /* TODO */
        }
    }
    
    

    return 0;
}
