#include "BinanceData.h"

struct string
{
    char *ptr;
    size_t len;
};

void init_string(struct string *s)
{
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL)
    {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL)
    {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}

double BinancePrice(int Stock)
{
    CURL *curl;
    curl = curl_easy_init();
    double prix = 0;
    if (curl)
    {
        struct string s;
        init_string(&s);
        char *link = malloc(100);
        if (link == NULL)
            err(1, "Out of memory");
        sprintf(link, "https://api.binance.com/api/v3/ticker/price?symbol=%s", CryptoStr(Stock, BINANCE));
        curl_easy_setopt(curl, CURLOPT_URL, link);
        free(link);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);

        //PARSE
        struct json_object *parsed_new = json_tokener_parse(s.ptr);
        struct json_object *price;
        json_object_object_get_ex(parsed_new, "price", &price);
        prix = json_object_get_double(price);
        free(s.ptr);
    }
    return prix;
}

// double BinancePrice(int stock)
// {
//     //CURL
//     CURL *curl;
//     CURLcode res = CURLE_OK;

//     curl_global_init(CURL_GLOBAL_ALL);
//     curl = curl_easy_init();
//     char *text = malloc(100);
//     double prix = 0;
//     if (curl)
//     {
//         char *link = malloc(100);
//         if (link == NULL)
//             err(1, "Out of memory");
//         sprintf(link, "https://api.binance.com/api/v3/ticker/price?symbol=%s", CryptoStr(stock, BINANCE));

//         printf("\n%s\n", link);

//         curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
//         curl_easy_setopt(curl, CURLOPT_URL, link);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &text);

//         // FILE *file = fopen("file", "w");
//         // curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
//         res = curl_easy_perform(curl);
//         /*curl_easy_perform( curl );*/
//         //fclose(file);
//         /* Check for errors */
//         if (res != CURLE_OK)
//             fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

//         printf("lol\n");

//         //PARSE
//         struct json_object *parsed_new = json_tokener_parse(text);
//         struct json_object *price;
//         json_object_object_get_ex(parsed_new, "price", &price);
//         prix = json_object_get_double(price);
//         free(price);

//         /* always cleanup */
//         curl_easy_cleanup(curl);
//         free(link);
//     }
//     free(text);
//     curl_global_cleanup();
//     return prix;
// }