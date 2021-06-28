#include "atr.h"

double abss(double a)
 {
    if(a < 0)
    { 
    	a = -a;
    }	
    return a;
} 

double max(double a, double b, double c)
{
    if (a > c && a > b)
    {
        return a;
    }
    if (b > a && b > c)
    {
        return b;
    }
    return c;
}

void atr(StockData *chart, double *atr, size_t a)
{
	double tr[chart->range];
	for (size_t i = 0; i < chart->range; ++i)
	{
		double min_1 = chart->high[i] - chart->low[i];
		double min_2 = chart->close[i] - chart->high[i];
		double min_3 = chart->close[i] - chart->low[i];

		min_1 = abss(min_1);
		min_2 = abss(min_2);
		min_3 = abss(min_3);
		double max_gap = max(min_1, min_2, min_3);
		tr[i] = max_gap;
	}
	MA_universel(tr, chart->range,atr, a);
}
