#include "strat_MACD.h"

int StratMACDSimple(double MACD, double signal)
{
    if (MACD < 0 && signal < 0)
    {
        if (MACD >= signal)
        {
            return BUY;
        }
    }
    if (MACD > 0 && signal > 0)
    {
        if (MACD <= signal)
        {
            return SELL;
        }
    }
    return HOLD;
}