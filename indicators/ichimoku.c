#include "ichimoku.h"

void Ichimoku(StockData *m, double* TS, double* KS, double* CS, double* SSA, double* SSB)
{
    size_t begin = 52;
    for (size_t i = 0; i < begin; i++)
    {
        TS[i] = 0;
        KS[i] = 0;
        CS[i] = 0;
        SSA[i] = 0;
        SSB[i] = 0;
    }

    //Tenkan-Sen
    int H9 = m->high[begin];
    int L9 = m->low[begin];

    for (size_t i = begin; i < m->range; i++)
    {
        for (size_t j = i-9; j < i; j++)
        {
            if (m->high[j] > H9)
            {
                H9 = m->high[i];
            }
            if (m->low[j] < L9)
            {
                L9 = m->low[i];
            }
        }
        TS[i] = (H9 + L9)/2;
    }

    //Kijun-Sen
    int H26 = m->high[begin];
    int L26 = m->low[begin];

    for (size_t i = begin; i < m->range; i++)
    {
        for (size_t j = i-26; j < i; j++)
        {
            if (m->high[j] > H26)
            {
                H26 = m->high[i];
            }
            if (m->low[j] < L26)
            {
                L26 = m->low[i];
            }
        }
        KS[i] = (H26 + L26)/2;
    }
    
    //Chikou Span
    for (size_t i = begin; i+26 < m->range; i++)
    {
        CS[i] = m->close[i+26];
    }

    //Senku Span A
    for (size_t i = begin; i+26 < m->range; i++)
    {
        SSA[i+26] = (KS[i] + TS[i])/2;
    }

    //Senku Span B
    int H52 = m->high[begin];
    int L52 = m->low[begin];

    for (size_t i = begin; i < m->range; i++)
    {
        for (size_t j = i-52; j < i; j++)
        {
            if (m->high[j] > H52)
            {
                H52 = m->high[i];
            }
            if (m->low[j] < 52)
            {
                L52 = m->low[i];
            }
        }
        SSB[i] = (H52 + L52)/2;
    }
}