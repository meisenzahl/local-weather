#include <stdio.h>

#include "yahoo_weather_api.h"

int main(void)
{
    size_t woeid = getWOEID();
    /*printf("getWOEID(): %zd\n", woeid);*/
    if (woeid) {
        struct Weather weather = getWeather(woeid);

        printf("temperature: %zd°%s\n", weather.temp, weather.units.temperature);
    }
    else {
    	printf("Couldn't get woeid\n");
    }

    return 0;
}
