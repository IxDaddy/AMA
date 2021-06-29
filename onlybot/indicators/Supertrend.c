#include "Supertrend.h"
#include "atr.h"

void SuperTrend(StockData *m, double* hausse, double* baisse, double* atr_mat, double* supertrend, double* final_up, double* final_down)
{
	atr(m,atr_mat,10);
	for(size_t i = 10; i < m->range; ++i)
	{
		hausse[i] = (m->high[i] + m->low[i])/2 + 3*atr_mat[i];
		baisse[i] = (m->high[i] + m->low[i])/2 - 3*atr_mat[i];
	}
	final_down[10] = m->close[10];
	final_up[10] = m->close[10];
	for(size_t i = 11; i < m->range; i++)
	{
		if (hausse[i] < final_up[i-1] || m->close[i-1] > final_up[i-1])
		{
			final_up[i] = hausse[i];
		}
		else
		{
			final_up[i] = final_up[i-1];
		}
		if (baisse[i] > final_down[i-1] || m->close[i-1] < final_down[i-1])
		{
			final_down[i] = baisse[i];
		}
		else
		{
			final_down[i] = final_down[i-1];
		}	
	}
	supertrend[10] = final_up[10];
	for(size_t i = 11; i < m->range; ++i)
	{
		if(supertrend[i-1] == final_up[i-1] && m->close[i] <= final_up[i])
		{
			supertrend[i] = final_up[i];
		}
		else if(supertrend[i-1] == final_up[i-1] && m->close[i] > final_up[i])
		{
			supertrend[i] = final_down[i];
		}
		else if(supertrend[i-1] == final_down[i-1] && m->close[i] >= final_down[i])
		{
			supertrend[i] = final_down[i];
		}
		else if(supertrend[i-1] == final_down[i-1] && m->close[i] < final_down[i])
		{
			supertrend[i] = final_up[i];
		}
	}
	for(size_t i = 0; i < 10; ++i)
	{
		hausse[i] = m->close[i];
		baisse[i] = m->close[i];
		supertrend[i] = m->close[i];
		final_up[i] = m->close[i];
		final_down[i] = m->close[i];
	}
}
