#include "Signal.h"

Signal *SignalInit()
{
    Signal *new = malloc(sizeof(Signal));

    new->ma = 0;
    new->ema = 0;
    new->rsi = 0;
    new->macd = 0;

    new->TakeProfit = 0;
    new->StopLoss = 0;
    return new;
}

void CleanStr(char *str, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        str[i] = ' ';
    }
}

char *CryptoStr(int stock, int a)
{
    char *str = malloc(sizeof(char) * 10);
    CleanStr(str, 10);
    switch (stock * a)
    {
    case 1:
        sprintf(str, "BTCUSDT");
        break;

    case -1:
        sprintf(str, "BTC-USD");
        break;

    case 2:
        sprintf(str, "ETHUSDT");
        break;

    case -2:
        sprintf(str, "ETH-USD");
        break;

    case 3:
        sprintf(str, "BNBUSDT");
        break;

    case -3:
        sprintf(str, "BNB-USD");
        break;

    default:
        break;
    }
    return str;
}

Trade *TradeInit(int Stock, float quantity, int leverage, int Strategy)
{
    Trade *new = malloc(sizeof(Trade));
    new->Stock = Stock;
    new->YahooStr = CryptoStr(Stock, YAHOO);
    new->BinanceStr = CryptoStr(Stock, BINANCE);
    new->Strategy = Strategy;
    new->Quantity = quantity;
    new->Leverage = leverage;
    new->Status = NO;
    new->price = 0;
    new->TradeNumber = 0;
    return new;
}

void TradeDelete(Trade *T)
{
    free(T->YahooStr);
    free(T->BinanceStr);
    free(T);
}

void SignalMA(StockData *m, Signal *S, size_t fastP, size_t slowP, int TradeStatus)
{
    size_t i = m->range - 1;

    double *fast = malloc(sizeof(double) * m->range);
    double *slow = malloc(sizeof(double) * m->range);
    MA(m, fast, fastP);
    MA(m, slow, slowP);

    if (slow[i - 1] > fast[i - 1] && slow[i] <= fast[i])
    {
        switch (TradeStatus)
        {
        case NO:
            S->ma = BUY;
            break;
        case LONG:
            S->ma = HOLD;
            break;
        case SHORT:
            S->ma = CLOSE_AND_BUY;
            break;
        default:
            break;
        }
    }
    else if (slow[i - 1] < fast[i - 1] && slow[i] >= fast[i])
    {
        switch (TradeStatus)
        {
        case NO:
            S->ma = SELL;
            break;
        case LONG:
            S->ma = CLOSE_AND_SELL;
            break;
        case SHORT:
            S->ma = HOLD;
            break;
        default:
            break;
        }
    }
    else
    {
        S->ma = HOLD;
    }
    free(slow);
    free(fast);
}

void SignalRSI(StockData *m, Signal *S, size_t period, size_t overbuy, size_t oversell)
{
    size_t i = m->range - 1;

    double *rsi = malloc(sizeof(double) * m->range);
    RSI(m, rsi, period);

    if (rsi[i - 1] < oversell && rsi[i] >= oversell)
    {
        S->rsi = BUY;
    }
    else if (rsi[i - 1] > overbuy && rsi[i] <= overbuy)
    {
        S->rsi = SELL;
    }
    else
    {
        S->rsi = HOLD;
    }
    if (rsi[i - 1] > oversell && rsi[i] <= oversell)
    {
        S->rsi = CLOSE_SHORT;
    }
    else if (rsi[i - 1] < overbuy && rsi[i] >= overbuy)
    {
        S->rsi = CLOSE_LONG;
    }
    free(rsi);
}

Signal *SignalChoose(StockData *m, Trade *T)
{
    Signal *S = SignalInit();
    SignalMA(m, S, 12, 21,T->Status);
    return S;
}

void Tweet(Trade *T, int Signal)
{
    char *cmd = malloc(2*sizeof(char)*(strlen("python3 post_tweet.py ") + strlen(T->BinanceStr) + sizeof(T->Quantity) + sizeof(T->Status))+200);
    if (cmd == NULL)
        err(1, "Out of memory");

    switch (Signal)
    {
    case BUY:
        sprintf(cmd, "python3 post_tweet.py \"Trade Ouvert :\nBUY \n%s @ %.2lf$ \"", T->BinanceStr, T->price);
        break;
    case SELL:
        sprintf(cmd, "python3 post_tweet.py \"Trade Ouvert :\nSELL \n%s @ %.2lf$ \"", T->BinanceStr, T->price);
        break;
    case CLOSE_LONG:
        sprintf(cmd, "python3 post_tweet.py \"Trade Fermé :\nCLOSE LONG \n%s @ %.2lf$\"",T->BinanceStr, T->price);
        break;
    case CLOSE_SHORT:
        sprintf(cmd, "python3 post_tweet.py \"Trade Fermé :\nCLOSE SHORT \n%s @ %.2lf$\"",T->BinanceStr, T->price);
        break;
    case CLOSE_AND_BUY:
        sprintf(cmd, "python3 post_tweet.py \"Trade Inversé :\nSHORT TO LONG \n%s @ %.2lf$ \"", T->BinanceStr, T->price);
        break;
    case CLOSE_AND_SELL:
        sprintf(cmd, "python3 post_tweet.py \"Trade Inversé :\nLONG TO SHORT \n%s @ %.2lf$ \"", T->BinanceStr, T->price);
        break;
    default:
        break;
    }
    //printf("%s\n", command);
    system(cmd);
    free(cmd);
}

int TradeUpdater(Trade *T, int Signal)
{
    switch (Signal)
    {
    case BUY:
        T->Status = LONG;
        break;
    case SELL:
        T->Status = SHORT;
        break;
    case CLOSE_LONG:
        T->Status = NO;
        break;
    case CLOSE_SHORT:
        T->Status = NO;
        break;
    case CLOSE_AND_BUY:
        T->Status = LONG;
        return 2;
    case CLOSE_AND_SELL:
        T->Status = SHORT;
        return 2;
    default:
        break;
    }
    return 1;
}

void TradeOrder(Trade *T, int Signal)
{
    char *command = malloc(strlen("python3 BinanceAPI.py") + strlen(T->BinanceStr) + sizeof(T->Quantity) + sizeof(T->Status) + 1);
    if (command == NULL)
        err(1, "Out of memory");

    sprintf(command, "python3 BinanceAPI.py %s %.3f %d %d", T->BinanceStr, T->Quantity*TradeUpdater(T, Signal), Signal, T->Leverage);
    system(command);
    T->price = BinancePrice(T->Stock);
    //free(command);
    Tweet(T,Signal);
}

void SignalAction(Signal *S, Trade *T)
{
    if (S->ma != HOLD)
    {
        TradeOrder(T, S->ma);
    }
    free(S);
}

void SignalMain(Trade *T, StockData *Mat)
{
    Signal *S = SignalChoose(Mat,T);
    SignalAction(S, T);
}