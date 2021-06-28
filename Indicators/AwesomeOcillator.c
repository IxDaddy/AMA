#include "AwesomeOcillator.h"

void AwesomeOscillator(StockData *m, double* res)
{
    double ma5[m->range];
    double ma34[m->range];
    MA(m, ma5, 5);
    MA(m, ma34, 34);
    for(size_t i = 0; i < m->size; i++)
    {
        res[i] = ma5[i] - ma34[i];
    }
}
