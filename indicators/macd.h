#ifndef MACD_H
#define MACD_H

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"
#include "ema.h"

void MACD(StockData *m, double* macd, double* signal, double* histogram);

#endif