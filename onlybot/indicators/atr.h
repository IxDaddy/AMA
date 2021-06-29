# ifndef ATR_H
# define ATR_H

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"
#include "ma.h"

void atr(StockData *chart, double *atr, size_t a);

# endif