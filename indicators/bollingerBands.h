#ifndef BOLLINGERBAND_H
#define BOLLINGERBAND_H

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"
#include "ma.h"

void BollingerBands(StockData *m, double* upper_band, double* lower_band, double* basis, double mult, size_t period);

#endif
