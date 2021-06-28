#include "parabolicSAR.h"

void ParabolicSAR(StockData *m, double *sar, double mult, size_t period)
{
    for (size_t i = 0; i <= period; i++)
    {
        sar[i] = m->close[i];
    }
    double maxPeriod = 0;
    for (size_t i = period+1; i < m->range; i++)
    {
        maxPeriod = 0;
        for (size_t j = i-period; j <= i; j++)
        {
            if (m->close[i] > maxPeriod)
            {
                maxPeriod = m->close[i];
            }
        }
        sar[i] = sar[i-1] + mult*(maxPeriod - sar[i-1]);
    }
}