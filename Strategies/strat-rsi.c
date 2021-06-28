#include "strat-rsi.h"

int StratRsiSimple(double rsi, int oversold, int overbought)
{
    if ((int) rsi > overbought)
    {
        return SELL;
    }
    if ((int) rsi < oversold)
    {
        return BUY;
    }
    return HOLD;
}