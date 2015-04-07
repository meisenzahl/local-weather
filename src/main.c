#include <stdio.h>

#include "yahoo_weather_api.h"

int main(void)
{
    size_t woeid = getWOEID();
    if (woeid) {
        struct Weather weather = getWeather(woeid);

        printf("temperature: %zd°%s\n", weather.temp, weather.units.temperature);
    }

    return 0;
}