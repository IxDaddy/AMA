#include "MA.h"

void MA(StockData *m, double* ma, size_t a)
{
	for (size_t i = 0; i < a; i++)
	{
		ma[i] = m->close[i];
	}
	for (size_t i = a; i < m->range; i++)
	{
		double sum = 0;
		for (size_t j = i - a; j < i+1; j++)
		{
			sum += m->close[j];
		}
		ma[i] = sum/(double)a;
	}
}

void MA_universel(double* value, size_t len, double* ma, size_t a)
{
	for (size_t i = 0; i < a; i++)
	{
		ma[i] = value[i];
	}
	for (size_t i = a; i < len; i++)
	{
		double sum = 0;
		for (size_t j = i - a; j < i+1; j++)
		{
			sum += value[j];
		}
		ma[i] = sum/(double)a;
	}
}
