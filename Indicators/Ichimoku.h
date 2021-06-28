# ifndef ICHIMOKU_H_
# define ICHIMOKU_H_

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"

void Ichimoku(StockData *m, double* TS, double* KS, double* CS, double* SSA, double* SSB);

# endif