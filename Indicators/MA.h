# ifndef MA_H_
# define MA_H_

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"

void MA(StockData *m, double* ma, size_t a);

void MA_universel(double* value, size_t len, double* ma, size_t a);

# endif