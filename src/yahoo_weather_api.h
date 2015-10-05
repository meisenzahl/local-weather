#ifndef YAHOO_WEATHER_API_H
#define YAHOO_WEATHER_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <json/json.h>

char *conditions[] = {
    "tornado",
    "tropical storm",
    "hurricane",
    "severe thunderstorms",
    "thunderstorms",
    "mixed rain and snow",
    "mixed rain and sleet",
    "mixed snow and sleet",
    "freezing drizzle",
    "drizzle",
    "freezing rain",
    "showers",
    "showers",
    "snow flurries",
    "light snow showers",
    "blowing snow",
    "snow",
    "hail",
    "sleet",
    "dust",
    "foggy",
    "haze",
    "smoky",
    "blustery",
    "windy",
    "cold",
    "cloudy",
    "mostly cloudy (night)",
    "mostly cloudy (day)",
    "partly cloudy (night)",
    "partly cloudy (day)",
    "clear (night)",
    "sunny",
    "fair (night)",
    "fair (day)",
    "mixed rain and hail",
    "hot",
    "isolated thunderstorms",
    "scattered thunderstorms",
    "scattered thunderstorms",
    "scattered showers",
    "heavy snow",
    "scattered snow showers",
    "heavy snow",
    "partly cloudy",
    "thundershowers",
    "snow showers",
    "isolated thundershowers",
    "not available"
};

struct Units {
    char distance[10];
    char pressure[10];
    char speed[10];
    char temperature[10];
};

struct Wind {
    size_t chill;
    size_t direction;
    float speed;
};

struct Atmosphere {
    size_t humidity;
    float pressure;
    size_t rising;
    size_t visibility;
};

struct Astronomy {
    size_t sunrise_hour;
    size_t sunrise_minute;
    size_t sunset_hour;
    size_t sunset_minute;
};

struct Temperature {
    size_t code;
    char date[20];
    char day[10];
    size_t high;
    size_t low;
};

struct Weather {
    size_t woeid;

    char city[100];
    char country[100];
    char region[100];

    size_t code;
    size_t temp;

    struct Units units;
    struct Wind wind;
    struct Atmosphere atmosphere;
    struct Astronomy astronomy;
    struct Temperature temperature[5];
};

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

size_t getWOEID();

struct Weather getWeather(size_t woeid, char* unit);

void printForecast(size_t woeid, char* unit);

#endif
