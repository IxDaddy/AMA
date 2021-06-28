#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "DataDownloader.h"



int main(int argc, char **argv)
{
    APItoTXTAIO(argv[1], argv[2], argv[3]);
    return 0;
}



/*
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, "https://yahoo-finance-low-latency.p.rapidapi.com/v8/finance/spark?symbols=BTC-USD&range=1y&interval=1d");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "x-rapidapi-key: a7328e6b95msh41fe7df15076f62p197cf3jsn7775fc0f2a96");
    headers = curl_slist_append(headers, "x-rapidapi-host: yahoo-finance-low-latency.p.rapidapi.com");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode ret = curl_easy_perform(curl);*/
    



