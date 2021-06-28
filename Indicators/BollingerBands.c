#include "BollingerBands.h"

double ecart_type(size_t size ,double* li)
{
	double average = 0;
	for (size_t i = 0; i < size; ++i)
	{
		average += li[i];
	}
	average = average/size;
	double val;
	double res;
	for (size_t i = 0; i < size; ++i)
	{
		res = average-li[i];
		val += res*res;
	}
	val = val/size;
	val = sqrt(val);
	return val;
}


void standard_deviation(double* SD, size_t periode, size_t size, double* close_price, double* li)
{
	for (size_t i = 0; i < periode; ++i)
	{
		li[i] = close_price[i];
	}
	size_t test = 0;
	for (size_t i = periode; i < size; ++i)
	{
		SD[i] = ecart_type(periode, li);
		li[test] = close_price[i];
		if (test + 1 == periode)
			test = 0;
		else
			test++;
	}
}

// trois types medium 20, short 10, long 50
void BollingerBands(StockData *m, double* upper_band, double* lower_band, double* basis, size_t period)
{
	double SD[m->range];
	double li[period];
    double v;
    if (period == 50)
    {
        v = 2.5;
    }
    else if (period == 20)
    {
        v = 2;
    }
    else
    {
        v = 1.5;
    }
    
	standard_deviation(SD,period,m->range, m->close,li);
	// recuperer MA
	for (size_t i = 0; i < period; i++)
	{
		upper_band[i] = m->close[i];
		lower_band[i] = m->close[i];
	}
    MA_universel(m->close, m->range, basis,period);
	for (size_t i = period; i < m->range; i++)
	{
		upper_band[i] = basis[i] + v*SD[i];
		lower_band[i] = basis[i] - v*SD[i];
	}
}
