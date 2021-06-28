#include "BacktestStructs.h"

TradesData *TradesDataInit()
{
    TradesData *new = malloc(sizeof(TradesData));
    if (new == NULL)
        err(1, "Out of memory");
    new->isActive = 0;
    return new;
}

void TradesData_destroy(TradesData *data)
{
    free(data);
}

BacktestOptions *BacktestOptionsInit()
{
    BacktestOptions *new = malloc(sizeof(BacktestOptions));

    new->StartBankroll = 10000;
    new->Mise = 100;
    new->Leverage = 1;
    new->TakeProfit = 0;
    new->StopLoss = 0;
    new->TrailingStop = 0;
    new->Martingale = 0;
    new->Isolated = 0;
    new->Cross = 0;
    new->MiseRelative = 0;
    new->BestOpti = 0;
    new->Visualize = 0;
    return new;
}

BacktestHistory *BacktestInit(BacktestOptions *Option, size_t max)
{
    BacktestHistory *new = malloc(sizeof(BacktestHistory));
    if (new == NULL)
        err(1, "Out of memory");
    new->numberTrades = 0;
    new->resPNL = 0;

    //OPTIONS
    new->bankroll = Option->StartBankroll;
    new->lev = Option->Leverage;
    new->mise = Option->Mise;

    //HISTORY OF TRADES
    new->chartBK = malloc(sizeof(double) * max + 1);
    new->chartBK[0] = new->bankroll;
    new->chartMise = malloc(sizeof(double) * max);
    new->chartMise[0] = 0;
    new->type = malloc(sizeof(int) * max);
    new->type[0] = 0;
    new->quantity = malloc(sizeof(double) * max);
    new->quantity[0] = 0;
    new->entry = malloc(sizeof(double) * max);
    new->entry[0] = 0;
    new->exit = malloc(sizeof(double) * max);
    new->exit[0] = 0;
    new->pnl = malloc(sizeof(double) * max);
    new->pnl[0] = 0;
    new->variation = malloc(sizeof(double) * max);
    new->variation[0] = 0;
    new->leverage = malloc(sizeof(double) * max);
    new->leverage[0] = 0;
    new->EntryDate = malloc(sizeof(int) * max);
    new->EntryDate[0] = 0;
    new->ExitDate = malloc(sizeof(int) * max);
    new->ExitDate[0] = 0;
    return new;
}

void BacktestDestroy(BacktestHistory *B)
{
    free(B->chartBK);
    free(B->chartMise);
    free(B->type);
    free(B->quantity);
    free(B->entry);
    free(B->exit);
    free(B->pnl);
    free(B->variation);
    free(B->leverage);
    free(B->EntryDate);
    free(B->ExitDate);
    free(B->BacktestOption);
    free(B);
}

double max_drawdown(BacktestHistory *B)
{
    double mdd = 0;
    double peak = B->chartBK[0];
    for (int i = 0; i < B->numberTrades; i++)
    {
        if (B->chartBK[i] > peak)
        {
            peak = B->chartBK[i];
        }
        if ((peak - B->chartBK[i]) / peak  > mdd)
        {
            mdd = (peak - B->chartBK[i]);
        }
    }
    return mdd;
}

BacktestSumUp *BacktestSumUpInit(BacktestHistory *B)
{
    BacktestSumUp *new = malloc(sizeof(BacktestSumUp));
    if (new == NULL)
        err(1, "Out of memory");
    new->BacktestHistory = B;
    new->bankroll = B->bankroll;
    new->mise = B->mise;
    new->maxLoss = 0;
    new->maxProfits = 0;
    new->maxBankroll = B->chartBK[0];
    new->minBankroll = B->chartBK[0];
    new->numberTrades = B->numberTrades;
    new->sumLoss = 0;
    new->sumProfits = 0;
    new->WinnerTrades = 0;
    new->LooserTrades = 0;
    new->averageVariation = 0;
    new->PNL = 0;
    new->maxDrawdown = max_drawdown(B);
    for (int i = 1; i < B->numberTrades; i++)
    {
        new->WinnerTrades += (B->pnl[i] >= 0);
        new->LooserTrades += (B->pnl[i] < 0);
        new->PNL += B->pnl[i];
        if (B->pnl[i] < new->maxLoss)
            new->maxLoss = B->pnl[i];

        if (B->pnl[i] > new->maxProfits)
            new->maxProfits = B->pnl[i];

        if (B->chartBK[i] < new->minBankroll)
            new->minBankroll = B->chartBK[i];

        if (B->chartBK[i] > new->maxBankroll)
            new->maxBankroll = B->chartBK[i];

        if (B->pnl[i] >= 0)
        {
            new->sumProfits += B->pnl[i];
        }
        else
        {
            new->sumLoss += B->pnl[i];
        }
        new->averageVariation += B->variation[i];
    }
    new->variation = (B->resPNL / B->bankroll) * 100;
    new->winrate = ((double)new->WinnerTrades / (double)new->numberTrades) * 100;
    new->averageLoss = new->sumLoss / new->LooserTrades;
    new->averageProfits = new->sumProfits / new->WinnerTrades;
    new->averageVariation /= B->numberTrades;
    new->endBankroll = B->bankroll + B->resPNL;
    return new;
}