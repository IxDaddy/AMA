# ifndef MACD_H_
# define MACD_H_

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"
#include "EMA.h"

void MACD(StockData *m, double* macd, double* signal, double* histogram);

# endif