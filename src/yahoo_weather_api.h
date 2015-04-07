#ifndef YAHOO_WEATHER_API_H
#define YAHOO_WEATHER_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

struct MemoryStruct {
	char *memory;
	size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

size_t getWOEID();

#endif