# ifndef MOMENTUM_H_
# define MOMENTUM_H_

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"

void Momentum(StockData *m, double *momen, size_t a);

void Momentum_universel(double *l, size_t len, double *momen, size_t a);

# endif