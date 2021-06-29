# ifndef DATADOWNLOADER_H_
# define DATADOWNLOADER_H_

#include <stddef.h>
#include "StockData.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <json-c/json.h>
#include <math.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <err.h>

void bufferprinter(char *b, size_t len);

StockData* APItoTXTAIO(const char* symbol, const char* interval, const char* range);

# endif