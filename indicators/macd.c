#include "macd.h"

void MACD(StockData *m, double* macd, double* signal, double* histogram)
{
    double ema12[m->range];
    double ema26[m->range];
    EMA(m,ema12,12);
    EMA(m,ema26,26);
    for (size_t i = 26; i < m->range; i++)
    {
        macd[i] = ema12[i] - ema26[i];
    }
    double emaMACD[m->range];
    EMA_universel(macd, m->range,emaMACD,9);
    for (size_t i = 0; i < m->range; i++)
    {
        signal[i] = emaMACD[i];
        histogram[i] = macd[i] - signal[i];
    }
}