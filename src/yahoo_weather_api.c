#include "yahoo_weather_api.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        /* out of memory */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

size_t getWOEID()
{
    CURL *curl;
    CURLcode res;

    const char URL[] = "https://weather.yahoo.com";
    char woeid[32];
    size_t WOEID = 0;

    struct MemoryStruct chunk;

    /* will be grown as needed by the realloc above */
    chunk.memory = malloc(1);
    /* no data at this point */
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl, CURLOPT_URL, URL);

    /* send all data to this function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);

    /* set user-agent*/
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it */
    res = curl_easy_perform(curl);

    /* check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    else {
        char *tmp;

        tmp = strstr(chunk.memory, "woeId:");
        if (tmp) {
            strcpy(chunk.memory, tmp);
            tmp = strstr(chunk.memory, " ");

            if (tmp) {
                strcpy(chunk.memory, tmp);
                size_t length = strcspn(chunk.memory, ",") -1;

                size_t i;
                for (i = 0; i < length; i++) {
                    woeid[i] = chunk.memory[i + 1];
                }
                woeid[length + 1] = '\0';

                WOEID = atoi(woeid);
            }
        }
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl);

    /* always free up allocated memory */
    if (chunk.memory)
        free(chunk.memory); 

    /* cleanup globally */
    curl_global_cleanup();

    return WOEID;
}

struct Weather getWeather(size_t woeid)
{
    struct Weather weather;

    weather.woeid = woeid;

    char help[10];
    sprintf(help, "%zd", woeid);

    char URL[200] = "https://query.yahooapis.com/v1/public/yql?q=SELECT%20*%20FROM%20weather.forecast%20WHERE%20woeid%3D";
    strcat(URL, help);
    strcat(URL, "%20AND%20u%3D'c'&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys");

    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;

    /* will be grown as needed by the realloc above */
    chunk.memory = malloc(1);
    /* no data at this point */
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl, CURLOPT_URL, URL);

    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);

    /* set user-agent*/
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it! */
    res = curl_easy_perform(curl);

    /* check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    else {
        json_object *jobj = json_tokener_parse(chunk.memory);
    if (jobj == NULL) {
        perror("ERROR parsing string to json");
    }
    jobj = json_object_object_get(jobj, "query");
    jobj = json_object_object_get(jobj, "results");
    jobj = json_object_object_get(jobj, "channel");

    /* get city */
    json_object *j = json_object_object_get(jobj, "location");
    j = json_object_object_get(j, "city");
    strcpy(weather.city, json_object_get_string(j));

    /* get country */
    j = json_object_object_get(jobj, "location");
    j = json_object_object_get(j, "country");
    strcpy(weather.country, json_object_get_string(j));

    /* get region */
    j = json_object_object_get(jobj, "location");
    j = json_object_object_get(j, "region");
    strcpy(weather.region, json_object_get_string(j));

    /* get units.distance */
    j = json_object_object_get(jobj, "units");
    j = json_object_object_get(j, "distance");
    strcpy(weather.units.distance, json_object_get_string(j));

    /* get units.pressure */
    j = json_object_object_get(jobj, "units");
    j = json_object_object_get(j, "pressure");
    strcpy(weather.units.pressure, json_object_get_string(j));

    /* get units.speed */
    j = json_object_object_get(jobj, "units");
    j = json_object_object_get(j, "speed");
    strcpy(weather.units.speed, json_object_get_string(j));

    /* get units.temperature */
    j = json_object_object_get(jobj, "units");
    j = json_object_object_get(j, "temperature");
    strcpy(weather.units.temperature, json_object_get_string(j));

    /* get wind.chill */
    j = json_object_object_get(jobj, "wind");
    j = json_object_object_get(j, "chill");
    weather.wind.chill = json_object_get_int(j);

    /* get wind.direction */
    j = json_object_object_get(jobj, "wind");
    j = json_object_object_get(j, "direction");
    weather.wind.direction = json_object_get_int(j);

    /* get wind.speed */
    j = json_object_object_get(jobj, "wind");
    j = json_object_object_get(j, "speed");
    weather.wind.speed = json_object_get_double(j);

    /* get atmosphere.humidity */
    j = json_object_object_get(jobj, "atmosphere");
    j = json_object_object_get(j, "humidity");
    weather.atmosphere.humidity = json_object_get_int(j);

    /* get atmosphere.pressure */
    j = json_object_object_get(jobj, "atmosphere");
    j = json_object_object_get(j, "pressure");
    weather.atmosphere.pressure = json_object_get_double(j);

    /* get atmosphere.rising */
    j = json_object_object_get(jobj, "atmosphere");
    j = json_object_object_get(j, "rising");
    weather.atmosphere.rising = json_object_get_int(j);

    /* get atmosphere.visibility */
    j = json_object_object_get(jobj, "atmosphere");
    j = json_object_object_get(j, "visibility");
    weather.atmosphere.visibility = json_object_get_int(j);

    char *ptr;
    /* get astronomy.sunrise */
    j = json_object_object_get(jobj, "astronomy");
    j = json_object_object_get(j, "sunrise");
    chunk.memory = (char *) json_object_get_string(j);

    ptr = strchr(chunk.memory, ':');
    char help[3];
    size_t i;
    if (ptr) {
        size_t index = ptr - chunk.memory;

        for (i = 0; i < index; i++) {
            help[i] = chunk.memory[i];
        }

        weather.astronomy.sunrise_hour = atoi(help);
        help[1] = '\0';

        for (i = 0; i < 2; i++) {
            help[i] = chunk.memory[i + index + 1];
        }
        weather.astronomy.sunrise_minute = atoi(help);
        help[1] = '\0';

        if (strstr(chunk.memory, "pm")) {
            weather.astronomy.sunrise_hour += 12;
        }
    }

    /* get astronomy.sunset */
    j = json_object_object_get(jobj, "astronomy");
    j = json_object_object_get(j, "sunset");
    chunk.memory = (char *) json_object_get_string(j);

    ptr = strchr(chunk.memory, ':');
    if (ptr) {
        size_t index = ptr - chunk.memory;

        for (i = 0; i < index; i++) {
            help[i] = chunk.memory[i];
        }

        weather.astronomy.sunset_hour = atoi(help);
        help[1] = '\0';

        for (i = 0; i < 2; i++) {
            help[i] = chunk.memory[i + index + 1];
        }
        weather.astronomy.sunset_minute = atoi(help);
        help[1] = '\0';

        if (strstr(chunk.memory, "pm")) {
            weather.astronomy.sunset_hour += 12;
        }
    }

    /* get code */
    jobj = json_object_object_get(jobj, "item");
    j = json_object_object_get(jobj, "condition");
    j = json_object_object_get(j, "code");
    weather.code = json_object_get_int(j);

    /* get temp */
    j = json_object_object_get(jobj, "condition");
    j = json_object_object_get(j, "temp");
    weather.temp = json_object_get_int(j);

    /* get forecast */
    jobj = json_object_object_get(jobj, "forecast");

    size_t length = json_object_array_length(jobj);

    for (i = 0; i < length; i++) {
        /* get code */
        j = json_object_array_get_idx(jobj, i);
        j = json_object_object_get(j, "code");
        weather.temperature[i].code = json_object_get_int(j);

        /* get date */
        j = json_object_array_get_idx(jobj, i);
        j = json_object_object_get(j, "date");
        strcpy(weather.temperature[i].date, json_object_get_string(j));

        /* get day */
        j = json_object_array_get_idx(jobj, i);
        j = json_object_object_get(j, "day");
        strcpy(weather.temperature[i].day, json_object_get_string(j));

        /* get high */
        j = json_object_array_get_idx(jobj, i);
        j = json_object_object_get(j, "high");
        weather.temperature[i].high = json_object_get_int(j);

        /* get low */
        j = json_object_array_get_idx(jobj, i);
        j = json_object_object_get(j, "low");
        weather.temperature[i].low = json_object_get_int(j);
    }
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl);

    if (chunk.memory)
        free(chunk.memory);

    /* cleanup globally */
    curl_global_cleanup();

    return weather;
}