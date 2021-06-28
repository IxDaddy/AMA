#ifndef EMA_H
#define EMA_H

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"

void EMA(StockData *m, double* ema, size_t a);

void EMA_universel(double* value, size_t len, double* ema, size_t a);

#endif