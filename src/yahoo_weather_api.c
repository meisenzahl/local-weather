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
    char woeid[10];
    size_t WOEID = -1;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);                               /* will be grown as needed by the realloc above */
    chunk.size = 0;                                         /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl, CURLOPT_URL, URL);

    /* send all data to this function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);

    /* get it */
    res = curl_easy_perform(curl);

    /* check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    else {
    	if ((long) chunk.size < 100000) {
    		return 0;
    	}
        printf("%lu bytes retrieved\n", (long) chunk.size);

        strcpy(chunk.memory, strstr(chunk.memory, "woeId:"));
        strcpy(chunk.memory, strstr(chunk.memory, " "));
        int length = strcspn(chunk.memory, ",") - 1;

        int i;
        for (i = 0; i < length; i++) {
            woeid[i] = chunk.memory[i + 1];
        }
        woeid[length + 1] = '\0';

        WOEID = atoi(woeid);

        printf("%s\n", woeid);
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl);

    /* always free up allocated memory */
    free(chunk.memory);

    /* cleanup globally */
    curl_global_cleanup();

    return WOEID;
}