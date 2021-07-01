#ifndef SIGNAL_H_
#define SIGNAL_H_

#include ".././indicators/ma.h"
#include ".././indicators/ema.h"
#include ".././indicators/rsi.h"
#include ".././indicators/macd.h"
#include ".././indicators/momentum.h"
#include ".././indicators/bollingerBands.h"
#include ".././indicators/stochastic.h"
#include ".././indicators/WilliamR.h"
#include ".././indicators/Supertrend.h"
#include ".././indicators/offset.h"
#include ".././indicators/Roc.h"
#include ".././indicators/ichimoku.h"


#include <math.h>

#include <stddef.h>
#include "../StockData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "../DataDownloader.h"
#include "BinanceData.h"
#include <fcntl.h>
#include <unistd.h>

//SIMPLE STRATEGY
#define Ma 1
#define Ema 2
#define Rsi 3
#define Madc 4
#define Supertrend 5
#define Williamr 6
#define Stochastic 7

//TRADE STATUS
#define NO 0
#define LONG 1
#define SHORT -1

//SIGNALS
#define HOLD 0
#define BUY 1
#define SELL -1
#define CLOSE_SHORT 2
#define CLOSE_LONG -2
#define CLOSE_AND_BUY 3
#define CLOSE_AND_SELL -3


//STOCK
#define BTC 1
#define ETH 2
#define BNB 3
#define ADA 4
#define DOGE 5
#define XRP 6
#define DOT 7
#define UNI 8
#define LTC 9
#define SOL 10
#define LINK 11
#define DASH 12

//API
#define BINANCE 1
#define YAHOO -1

typedef struct Signal
{
    //INDICATORS
    int ma;
    int ema;
    int rsi;
    int macd;
    int supertrend;
    int stochastics;

    //OPTIONS
    int TakeProfit;
    int StopLoss;
} Signal;

typedef struct Trade
{
    int Stock;
    char* YahooStr;
    char* BinanceStr;
    int Strategy;
    float Quantity;
    int Leverage;
    int Status;
    double price;
    int TradeNumber;
} Trade;

Trade *TradeInit(int Stock, float quantity, int leverage, int Strategy);

void TradeDelete(Trade *T);

char *CryptoStr(int stock, int a);

// void SignalMA(StockData *m, Signal *S, size_t fastP, size_t slowP, int TradeStatus);

// void SignalRSI(StockData *m, Signal *S, size_t period, size_t overbuy, size_t oversell);

// Signal *SignalChoose(StockData *m);

// void SignalVisual(Signal *S);

void SignalMain(Trade *T, StockData *Mat);


#endif