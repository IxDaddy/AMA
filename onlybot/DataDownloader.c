#include "DataDownloader.h"

#define URL_BASE "https://yahoo-finance-low-latency.p.rapidapi.com/v8/finance/chart/"
#define FIRST_ARG ""
#define SECOND_ARG "?interval="
#define THIRD_ARG "&range="
#define URL_BASE_LEN sizeof URL_BASE + sizeof FIRST_ARG + sizeof SECOND_ARG + sizeof THIRD_ARG
#define TEMPFILENAME "temp.json"
#define API_KEY "x-rapidapi-key: a7328e6b95msh41fe7df15076f62p197cf3jsn7775fc0f2a96"
//#define API_KEY "x-rapidapi-key: 1e9e9b10cdmsh555c137211fe9c0p167525jsn235f39828587"
//#define API_KEY "x-rapidapi-key: 807f54bcd4mshd4c952586ce1757p10f841jsnd98fc9a61737"
#define API_DOMAIN "x-rapidapi-host: yahoo-finance-low-latency.p.rapidapi.com"

void download_data(const char *symbole, const char *interval, const char *range)
{
    CURL *curl;
    CURLcode res = CURLE_OK;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl)
    {
        char *link = malloc(URL_BASE_LEN + strlen(symbole) + strlen(interval) + strlen(range) + 1);
        if (link == NULL)
            err(1, "Out of memory");
        sprintf(link, URL_BASE FIRST_ARG "%s" SECOND_ARG "%s" THIRD_ARG "%s",
                symbole, interval, range);

        //printf("\n%s\n", link);

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, link);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, API_KEY);
        headers = curl_slist_append(headers, API_DOMAIN);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        FILE *file = fopen(TEMPFILENAME, "w");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_perform(curl);
        /*curl_easy_perform( curl );*/
        fclose(file);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);

        free(link);
    }
    curl_global_cleanup();
}

void NullRemover(StockData *Mat)
{
    for (size_t i = 2; i < Mat->range; i++)
    {
        if (Mat->high[i] == 0.0000000)
        {
            Mat->high[i] = (Mat->high[i - 1] + Mat->high[i - 2]) / 2;
        }
        if (Mat->low[i] == 0.0000000)
        {
            Mat->low[i] = (Mat->low[i - 1] + Mat->low[i - 2]) / 2;
        }
        if (Mat->open[i] == 0.0000000)
        {
            Mat->open[i] = (Mat->open[i - 1] + Mat->open[i - 2]) / 2;
        }
        if (Mat->close[i] == 0.0000000)
        {
            Mat->close[i] = (Mat->close[i - 1] + Mat->close[i - 2]) / 2;
        }
    }
}

int ErrorCheck(char *buffer)
{
    for (size_t i = 0; buffer[i]; i++)
    {
        if (buffer[i] == 'r' && buffer[i + 2] == 's' && buffer[i + 4] == 'l' && buffer[i + 7] == ':' && buffer[i + 8] == 'n' && buffer[i + 9] == 'u' && buffer[i + 10] == 'l')
        {
            return 1;
        }
    }
    return 0;
}

void StockDataToFile(StockData *m)
{
    FILE *filePtr;
    filePtr = fopen("backtest_temp", "w");
    for (size_t i = 0; i < m->range; i++)
    {
        fprintf(filePtr, "%lf,", m->open[i]);
        fprintf(filePtr, "%lf,", m->high[i]);
        fprintf(filePtr, "%lf,", m->low[i]);
        fprintf(filePtr, "%lf\n", m->close[i]);
    }
    fclose(filePtr);
}

StockData *APItoTXTAIO(const char *symbole, const char *interval, const char *range)
{
    download_data(symbole, interval, range);
    FILE *fp;

    fp = fopen(TEMPFILENAME, "r");
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char *buffer = malloc(size);
    if (buffer == NULL)
        err(1, "Out of memory");
    char *buffer2 = malloc(size);
    if (buffer2 == NULL)
        err(1, "Out of memory");
    char *buffer3 = malloc(size);
    if (buffer3 == NULL)
        err(1, "Out of memory");

    fread(buffer, size, 1, fp);
    fclose(fp);
    remove(TEMPFILENAME);

    int error = ErrorCheck(buffer);
    if (error == 1)
    {
        err(0, "Veuillez selectionner d'autre range et interval");
    }

    //PARSING

    size_t start_quote;
    size_t end_quote;

    size_t start_time;
    size_t end_time;

    //Ecriture du 2eme buffer contentant | {"high":[....], "close":...}
    for (size_t i = 0; buffer[i]; i++)
    {
        if (buffer[i] == 'q' && buffer[i + 1] == 'u' && buffer[i + 2] == 'o' && buffer[i + 3] == 't' && buffer[i + 4] == 'e')
        {
            start_quote = i + 8;
            for (; buffer[i] < '}'; i++)
            {
            }
            end_quote = i + 1;
            break;
        }
    }

    for (size_t i = start_quote; i < end_quote; i++)
    {
        buffer2[i - start_quote] = buffer[i];
    }

    //Ecriture du 3eme buffer contentant | "timestamp" : [....]
    for (size_t i = 0; buffer[i]; i++)
    {
        if (buffer[i] == 't' && buffer[i + 1] == 'i' && buffer[i + 2] == 'm' && buffer[i + 3] == 'e' && buffer[i + 4] == 's' && buffer[i + 5] == 't' && buffer[i + 6] == 'a')
        {
            start_time = i - 2;
        }
        if (buffer[i] == 'i' && buffer[i + 1] == 'n' && buffer[i + 2] == 'd' && buffer[i + 3] == 'i' && buffer[i + 4] == 'c' && buffer[i + 5] == 'a' && buffer[i + 6] == 't')
        {
            end_time = i - 2;
            break;
        }
    }

    for (size_t i = start_time; i < end_time; i++)
    {
        buffer3[i - start_time] = buffer[i];
        if (buffer3[i - start_time] == ']')
        {
            buffer3[0] = '{';
            buffer3[i - start_time + 1] = '}';
        }
    }

    struct json_object *high;
    struct json_object *low;
    struct json_object *open;
    struct json_object *close;
    struct json_object *volume;
    struct json_object *time;

    struct json_object *parsed_new = json_tokener_parse(buffer2);
    struct json_object *parsed_new2 = json_tokener_parse(buffer3);

    json_object_object_get_ex(parsed_new2, "timestamp", &time);
    json_object_object_get_ex(parsed_new, "high", &high);
    json_object_object_get_ex(parsed_new, "low", &low);
    json_object_object_get_ex(parsed_new, "open", &open);
    json_object_object_get_ex(parsed_new, "close", &close);
    json_object_object_get_ex(parsed_new, "volume", &volume);

    size_t n_times = json_object_array_length(high);

    StockData *Mat = StockData_new(n_times);

    for (size_t i = 0; i < n_times; i++)
    {
        Mat->timestamp[i] = json_object_get_int(json_object_array_get_idx(time, i));
        Mat->high[i] = json_object_get_double(json_object_array_get_idx(high, i));
        Mat->low[i] = json_object_get_double(json_object_array_get_idx(low, i));
        Mat->open[i] = json_object_get_double(json_object_array_get_idx(open, i));
        Mat->close[i] = json_object_get_double(json_object_array_get_idx(close, i));
        Mat->volume[i] = json_object_get_double(json_object_array_get_idx(volume, i));
    }
    NullRemover(Mat);
    free(buffer);
    free(buffer2);
    free(buffer3);

    return Mat;
}