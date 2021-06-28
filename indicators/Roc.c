#include "Roc.h"

// soit 12 soit 25 pour a c'est les périodes
void Roc(StockData *m, double *roc, size_t a)
{
    for (size_t i = 0; i < a; i++)
    {
        roc[i] = 0;
    }
    for (size_t i = a; i < m->range; i++)
    {
        roc[i] =  ((m->close[i]/m->close[i-a])-1)*100;
    } 
}