#include "./signals/Signal.h"
#include <time.h>

#define STOCK BTC
#define STRATEGY Ma
#define QUANTITY 0.002
#define LEVERAGE 5

int main()
{
    // char* symbol = "BTC-USD";
    // char* interval = "15m";
    // char* range = "5d";
    int i = 0;
    Trade *T = TradeInit(STOCK,QUANTITY,LEVERAGE,STRATEGY);
    while (1)
    {
        // StockData *Mat = APItoTXTAIO(symbol, interval, range);
        // Signal *S = SignalChoose(Mat);
        // free(S);
        //system("./signal BTC-USD 1m 1d");

        if (time(NULL) % (60) == 40)
        {
            SignalMain(T ,"1m", "1d");
            i++;
            printf("sent : %d\n", i);
            sleep(2);
        }
    }
    TradeDelete(T);
    return 0;
}