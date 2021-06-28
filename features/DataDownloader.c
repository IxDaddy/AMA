#include<stdio.h>
#include<json-c/json.h>
#include<math.h>
#include<stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <err.h>

struct StockData
{
    int start;
    size_t range;
    char* symbol;
    int timestamp[10000];
    double h[10000];
};

void bufferprinter(char *b, size_t len)
{
    printf("\n");
    for (size_t i = 0; b[i]; i++)
    {
        printf("%c",b[i]);
    }
}

// main: main.o 
// 	$(CC) -g -o $@ $< $(LDFLAGS)

// main.o: main.c
// 	gcc -g -c $< $(LDFLAGS)

struct StockData* APItoTXTAIO(char* symbole, char* intervale, char* range){
    CURL *curl;
    CURLcode res = CURLE_OK;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    char filenameT[50] = "_temp_";
    strcat(filenameT, symbole);
    strcat(filenameT, "_");
    strcat(filenameT, intervale);
    strcat(filenameT, "_");
    strcat(filenameT, range);
    strcat(filenameT, ".json");

    if(curl) 
    {
        char link[99];
        sprintf(link,"https://apidojo-yahoo-finance-v1.p.rapidapi.com"
            "/stock/v2/get-chart?interval=%s&symbol=%s&range=%s",
            intervale, symbole, range);
        
        // printf("%s",link);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        // curl_easy_setopt(curl, CURLOPT_URL, link);
        curl_easy_setopt(curl, CURLOPT_URL, "https://yahoo-finance-low-latency.p.rapidapi.com/v8/finance/spark?symbols=BTC-USD&range=1y&interval=1d");

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "x-rapidapi-key: a7328e6b95msh41fe7df15076f62p197cf3jsn7775fc0f2a96");
        // headers = curl_slist_append(headers, "x-rapidapi-host: apidojo-yahoo-finance-v1.p.rapidapi.com");
        headers = curl_slist_append(headers, "x-rapidapi-host: yahoo-finance-low-latency.p.rapidapi.com");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        FILE* file = fopen(filenameT, "w");
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, file);
        curl_easy_perform( curl );
        /*curl_easy_perform( curl );*/
        fclose(file);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    FILE *fp;
    FILE *f;
	char buffer[1000000] = {'\0'};
    char buffer2[100000] = {'\0'};
    struct json_object *closes;
    struct json_object *close;
    struct json_object *times;
    struct json_object *time;

    printf("\n1\n");
    
	fp = fopen(filenameT,"r");
	fread(buffer, 100000, 1, fp);
	fclose(fp);


    size_t place_start_high;
    size_t place_end_high;

    size_t place_start_time;
    size_t place_end_time;
    int a = 0;
    for(size_t i = 0;buffer[i];i++) 
    {
        if (buffer[i] == '{')
        {
            a++;
            if (a == 2)
            {
                place_start_high = i;
                for (; buffer[i] < '}'; i++){}
                place_end_high = i+1;
                break;
            }
        }
	}
    for(size_t i = place_start_high; i < place_end_high;i++) 
    {
        buffer2[i-place_start_high] = buffer[i];
	}
    
    printf("\n1\n");
    //bufferprinter(buffer2,10000);

    struct json_object *parsed_new = json_tokener_parse(buffer2);

    json_object_object_get_ex(parsed_new, "timestamp", &times);

    json_object_object_get_ex(parsed_new, "close", &closes);
    size_t n_closes = json_object_array_length(times);

    struct StockData *Mat = malloc(sizeof(struct StockData));
    /*{
        start;
        range;
        char* symbol;
        int timestamp[10000];
        double h[10000];
    };
*/
    printf("\n1\n");
    Mat->range = n_closes;
    for (size_t i = 0; i < n_closes; i++)
    {
        Mat->timestamp[i] = json_object_get_int(json_object_array_get_idx(times, i));
        Mat->h[i] = json_object_get_double(json_object_array_get_idx(closes, i));
    }
    return Mat;
}


double* MA(double a, struct StockData m)
{
    double *ma = malloc(sizeof(double)*m.range+1);
    for (size_t i = 0; i < a; i++)
    {
        ma[i] = 0;
    }
    for (int i = m.range; i > a; i--)
    {
        double sum = 0;
        for (int j = 0; j < a; j++)
        {
            sum += m.h[j];
        }
        ma[i] = sum/a;
    }
    return ma;
}