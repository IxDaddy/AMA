#include "EMA.h"

void EMA(StockData *m, double* ema, size_t a)
{
    for (size_t i = 0; i < a+1; i++)
	{
		ema[i] = m->close[i];
	}
    double al = 2/((double)a+1);
    double sum = 0;
    for (size_t i = 0; i < a; i++)
    {
        sum += m->close[i];
    }
    ema[a-1] = sum/(double)a;
    for (size_t i = a; i < m->range; i++)
    {
        ema[i] = ((m->close[i] - ema[i-1]) * al) + ema[i-1];
    }
}

void EMA_universel(double* value, size_t len, double* ema, size_t a)
{
    for (size_t i = 0; i < a; i++)
	{
		ema[i] = value[i];
	}
    double al = 2/((double)a+1);
    double sum = 0;
    for (size_t i = 0; i < a; i++)
    {
        sum += value[i];
    }
    ema[a-1] = sum/(double)a;
    for (size_t i = a; i < len; i++)
    {
        ema[i] = ((value[i] - ema[i-1]) * al) + ema[i-1];
    }
}