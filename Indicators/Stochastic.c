#include "Stochastic.h"

void Stochastic(StockData *m, double* K, double* D, size_t smooth, size_t a)
{
    //init
    for (size_t i = 0; i < a+3; i++)
    {
        K[i] = 0;
    }
    for (size_t i = 0; i < a+3; i++)
    {
        D[i] = 0;
    }
    
    //K
    for (size_t i = a; i < m->range; i++)
    {
        double H = m->close[i];
        double B = m->close[i];
        double price;
        for (size_t j = i-a; j < i+1; j++)
        {
            price = m->close[j];
            if (price > H)
            {
                H = price;
            }
            if (price < B)
            {
                B = price;
            }
        }
        D[i] = 100*((m->close[i] - B)/(H-B));
    }
    //smooth K
    MA_universel(D,m->range,K,smooth);`

    //D
    MA_universel(K,m->range,D,3);
}