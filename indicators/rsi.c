#include "rsi.h"

void RSI(StockData *m, double* rsi, size_t a)
{
    double hausse[a];
    double baisse[a];
    for (size_t i = 1; i < a; i++)
    {
        double res = m->close[i-1] - m->close[i];
        if(res < 0)
        {
            hausse[i] = -res;
            baisse[i] = 0;
        }
        else
        {
            baisse[i] = res;
            hausse[i] = 0;
        }
    }
    double avg_hausse;
    double avg_baisse;
    for (size_t i = 0; i < a; i++)
    {
        avg_hausse += hausse[i];
        avg_baisse += baisse[i];
    }
    avg_hausse = avg_hausse/a;
    avg_baisse = avg_baisse/a;
    size_t index = 0;
    for (size_t i = a; i < m->range; i++)
    {
        rsi[i] = 100-(100/(1+avg_hausse/avg_baisse));
        double res = m->close[i-1] - m->close[i];
        if(res < 0)
        {
            hausse[index] = -res;
            baisse[index] = 0;
        }
        else
        {
            baisse[index] = res;
            hausse[index] = 0;
        }
        if (index + 1 == a)
        {
            index = 0;
        }
        else
        {
            index++;
        }
        for (size_t i = 0; i < a; i++)
        {
            avg_hausse += hausse[i];
            avg_baisse += baisse[i];
        }
        avg_hausse = avg_hausse/a;
        avg_baisse = avg_baisse/a;
    }
    
}