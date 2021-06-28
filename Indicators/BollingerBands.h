# ifndef BOLLINGERBAND_H_
# define BOLLINGERBAND_H_

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"
#include "MA.h"

void BollingerBands(StockData *m, double* upper_band, double* lower_band);

# endif
