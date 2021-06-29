# ifndef SUPERTREND_H_
# define SUPERTREND_H_

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"

void SuperTrend(StockData *m, double* hausse, double* baisse, double* atr_mat, double* supertrend, double* final_up, double* final_down);

# endif
