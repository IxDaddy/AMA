# ifndef STOCHASTIC_H_
# define STOCHASTIC_H_

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"
#include "MA.h"

void Stochastic(StockData *m, double* K, double* D, size_t smooth, size_t a);

# endif