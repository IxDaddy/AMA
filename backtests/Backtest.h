#ifndef BACKTEST_H_
#define BACKTEST_H_

#include "BacktestStructs.h"

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

//BACKTEST STUFF
#define BUY 1
#define SELL -1
#define HOLD 0
#include <math.h>
#include ".././plot/plot.h"
#include ".././plot/pbPlots.h"
#include ".././plot/supportLib.h"

void CreateTrade(StockData *m, BacktestHistory *B, TradesData *T, int i, int type, double mise, double lev);

void AddTradeToBacktest(BacktestHistory *B, TradesData *T);

double CloseTrade(StockData *m, BacktestHistory *B, TradesData *T, int i);

void ShowChartBacktest(BacktestHistory *B);

void BacktestVizualizer(BacktestSumUp *BS);

double AutoTPSL(TradesData *T, BacktestHistory *B, StockData *m, int i, BacktestOptions *Option);

double TrailingStop(TradesData *T, StockData *m, BacktestHistory *B, int i, double SL, double *TS);

double TradeActualProfit(StockData *m, TradesData *T, int i);

double OptionsUpdater(StockData *m, BacktestHistory *B, TradesData *T, BacktestOptions *Option, int i, double *lastWon, double *mise, double *TS);

double BacktestMA(StockData *m, BacktestHistory *B, BacktestOptions *Option, double fastP, double slowP);

double BacktestRSI(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period, double oversell, double overbuy);

double BacktestMOM(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period);

double BacktestMACD(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period);

double BacktestBB(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t mult, size_t period);

double BacktestStochastic(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period);

double BacktestWilliamR(StockData *m, BacktestHistory *B, BacktestOptions *Option, double overbought, double oversold, size_t period);

double BacktestSuperTrend(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period);

BacktestSumUp *Backtest(StockData *m, BacktestOptions *Option, int strat);

#endif