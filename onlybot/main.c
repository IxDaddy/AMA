#include "./signals/Signal.h"
#include <time.h>
#include <float.h>

#define STOCK BTC
#define STRATEGY Ma
#define QUANTITY 0.002
#define LEVERAGE 5

typedef struct OHLC
{
    double o;
    double h;
    double l;
    double c;
}OHLC;

OHLC *OHLCInit()
{
    OHLC *new = malloc(sizeof(OHLC));
    new->o = 0;
    new->h = 0;
    new->l = 0;
    new->c = 0;
    return new;
}

void OHLCReset(OHLC *Data, double currentPrice)
{
    Data->o = currentPrice;
    Data->h = currentPrice;
    Data->l = currentPrice;
    Data->c = 0;
}

void OHLCUpdate(OHLC *Data, double currentPrice)
{
    if (currentPrice > Data->h)
    {
        Data->h = currentPrice;
    }
    else if (currentPrice < Data->l)
    {
        Data->l = currentPrice;
    }
}

void UpdateStockDataPerTick(StockData *m, OHLC *Data)
{
    for (size_t i = 1; i < m->range; i++)
    {
        m->open[i - 1] = m->open[i];
        m->high[i - 1] = m->high[i];
        m->low[i - 1] = m->low[i];
        m->close[i - 1] = m->close[i];
    }
    m->open[m->range - 1] = Data->o;
    m->high[m->range - 1] = Data->h;
    m->low[m->range - 1] = Data->l;
    m->close[m->range - 1] = Data->c;
}

int main()
{
    //TRADE
    Trade *T = TradeInit(STOCK, QUANTITY, LEVERAGE, STRATEGY);
    StockData *Mat;

    //FIRST TRAITEMENT
    while (1)
    {
        if (time(NULL) % 60 == 1)
        {
            Mat = APItoTXTAIO("BTC-USD","1m","1d");
            SignalMain(T,Mat);
            break;
        }
    }

    //TRAITEMENT PEREEN

    //PRIX
    double price;
    OHLC *TickData = OHLCInit();

    //TIME
    time_t t;
    time_t t1 = time(NULL);
    time_t *t2 = malloc(sizeof(time_t));

    while (1)
    {
        t = time(t2);
        if (t1 < t)
        {
            price = BinancePrice(BTC);
            localtime(t2);

            //STUFF
            if (t % 60 == 1)
            {
                TickData->c = price;
                UpdateStockDataPerTick(Mat,TickData);
                SignalMain(T,Mat);
                OHLCReset(TickData,price);
            }
            else
            {
                OHLCUpdate(TickData,price);
            }
            t1++;
        }
    }
    TradeDelete(T);
    StockData_destroy(Mat);
    return 0;
}