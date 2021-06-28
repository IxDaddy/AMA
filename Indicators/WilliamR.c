#include "WilliamR.h"

void WilliamR(StockData *m, double* wil, size_t a)
{
    for (size_t i = 0; i < a; i++)
    {
        wil[i] = 0;
    }
    for (size_t i = a; i < m->range; i++)
    {
        double H = m->high[i-a];
        double L = m->low[i-a];
        for (size_t j = i-a; j < i+1; j++)
        {
            if (m->high[j] > H)
            {
                H = m->high[j];
            }
            if (m->low[j] < L)
            {
                L = m->low[j];
            }
        }
        wil[i] = 100*((m->close[i]-H)/(H-L));
    }
}