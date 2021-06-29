#ifndef STOCKDATA_H
#define STOCKDATA_H

#include <stddef.h>

typedef struct StockData
{
    size_t range;
    char* StrInterval;
    char* StrRange;
    char* symbol;
    int* timestamp;
    double* high;
    double* low;
    double* open;
    double* close;
    double* volume;
} StockData;

StockData* StockData_new(size_t nb_timestamp);

void StockData_destroy(StockData* data);

#endif
