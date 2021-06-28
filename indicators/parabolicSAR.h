# ifndef PARABOLICSAR_H_
# define PARABOLICSAR_H_

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "../StockData.h"

void ParabolicSAR(StockData *m, double *sar, double mult, size_t period);

# endif