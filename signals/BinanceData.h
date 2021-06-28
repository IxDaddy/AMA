#include <stdio.h>
#include <json-c/json.h>
#include <math.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include "Signal.h"

double BinancePrice(int Stock);