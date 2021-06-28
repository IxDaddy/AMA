# ifndef DATADOWNLOADER_H_
# define DATADOWNLOADER_H_

#include <stddef.h>
#include "StockData.h"

void bufferprinter(char *b, size_t len);

StockData* APItoTXTAIO(const char* symbol, const char* interval, const char* range);

# endif