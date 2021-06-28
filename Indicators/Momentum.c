#include "Momentum.h"

void Momentum(StockData *m, double *momen, size_t a)
{
    for (size_t i = 0; i < a; i++)
    {
        momen[i] = 0;
    }
    for (size_t i = a; i < m->range; i++)
    {
        momen[i] =  m->close[i] - m->close[i-a];
    } 
}