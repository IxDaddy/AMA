#ifndef BACKTESTSTRUCT_H_
#define BACKTESTSTRUCT_H_

#include <stddef.h>
#include "../StockData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "../DataDownloader.h"
#include <fcntl.h>
#include <unistd.h>
#include "../to_struct.h"


typedef struct TradesData
{
    int number;
    int isWinner;
    int type;
    int isActive;
    int EntryDate;
    int ExitDate;
    double mise;
    double quantity;
    double entry;
    double exit;
    double pnl;
    double variation;
    double leverage;
} TradesData;

typedef struct BacktestOptions
{
    //START INFOS
    double StartBankroll;
    double Mise;
    double Leverage;

    //OPTIONS
    double TakeProfit;
    double StopLoss;

    double TrailingStop;

    double Martingale;

    int MiseRelative;

    int Isolated;
    int Cross;

    int BestOpti;

    int Visualize;
} BacktestOptions;

typedef struct BacktestHistory
{
    int numberTrades;
    double bankroll;
    double resPNL;
    double lev;
    double mise;

    double *chartBK;
    double *chartMise;
    int *type;
    double *quantity;
    double *entry;
    double *exit;
    double *pnl;
    double *variation;
    double *leverage;
    int *EntryDate;
    int *ExitDate;

    BacktestOptions *BacktestOption;
} BacktestHistory;

typedef struct BacktestSumUp
{
    double bankroll; //
    double mise;
    int numberTrades; //
    int WinnerTrades; //
    int LooserTrades; //
    double winrate;   //
    double PNL;       //
    double variation;
    double maxDrawdown;
    double maxBankroll; //
    double minBankroll; //
    double maxLoss;     //
    double sumLoss;     //
    double averageLoss;      //
    double maxProfits;       //
    double sumProfits;       //
    double averageProfits;   //
    double averageVariation; //
    double endBankroll;
    BacktestHistory *BacktestHistory;
} BacktestSumUp;

TradesData *TradesDataInit();

void TradesData_destroy(TradesData *data);

BacktestOptions *BacktestOptionsInit();

BacktestHistory *BacktestInit(BacktestOptions *Option, size_t max);

void BacktestDestroy(BacktestHistory *B);

BacktestSumUp *BacktestSumUpInit(BacktestHistory *B);

#endif