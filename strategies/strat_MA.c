#include "strat_MA.h"

int StartMASimple(double lowMA, double highMA)
{
    if (lowMA > highMA)
    {
        return BUY;
    }
    return SELL;
}