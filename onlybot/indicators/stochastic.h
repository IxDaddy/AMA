#ifndef STOCHASTIC_H
#define STOCHASTIC_H

#include <stddef.h>
#include "../StockData.h"

void Stochastic(StockData *m, double* K, double* D, size_t smooth, size_t a);

#endif
