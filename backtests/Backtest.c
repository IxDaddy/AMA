#include "Backtest.h"

void CreateTrade(StockData *m, BacktestHistory *B, TradesData *T, int i, int type, double mise, double lev)
{
    T->isWinner = 0;
    T->number = B->numberTrades;
    T->entry = m->close[i];
    T->type = type;
    T->quantity = mise / m->close[i];
    T->leverage = lev;
    T->EntryDate = i;
    T->mise = mise;
    T->isActive = 1;
}

void AddTradeToBacktest(BacktestHistory *B, TradesData *T)
{
    int i = B->numberTrades + 1;
    B->resPNL += T->pnl;
    B->pnl[i] = T->pnl;
    B->type[i] = T->type;
    B->quantity[i] = T->quantity;
    B->chartBK[i] = B->chartBK[i - 1] + B->pnl[i];
    B->chartMise[i] = T->mise;
    B->type[i] = T->type;
    B->entry[i] = T->entry;
    B->exit[i] = T->exit;
    B->EntryDate[i] = T->EntryDate;
    B->ExitDate[i] = T->ExitDate;
    B->leverage[i] = T->leverage;
    B->variation[i] = T->variation;
    B->numberTrades++;
}

double CloseTrade(StockData *m, BacktestHistory *B, TradesData *T, int i)
{
    T->exit = m->close[i];

    if (T->type == 1)
    {
        T->variation = ((T->exit - T->entry) / T->entry) * 100 * T->leverage;
    }
    else
    {
        T->variation = ((T->entry - T->exit) / T->exit) * 100 * T->leverage;
    }

    T->pnl = T->variation * T->mise / 100;

    T->ExitDate = i;
    if ((T->exit - T->entry) > 0 && T->type == 1)
    {
        T->isWinner = 1;
    }
    if ((T->exit - T->entry) < 0 && T->type == -1)
    {
        T->isWinner = 1;
    }
    T->isActive = 0;
    AddTradeToBacktest(B, T);
    return T->pnl;
}

void ShowChartBacktest(BacktestHistory *B)
{
    //MODE 1 : ChartBK

    double *xs = malloc(sizeof(double) * B->numberTrades);
    for (int i = 0; i < B->numberTrades; i++)
    {
        xs[i] = i;
    }
    RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
    DrawScatterPlot(canvasReference, 600, 400, xs, B->numberTrades, B->chartBK, B->numberTrades);

    size_t length;
    double *pngdata = ConvertToPNG(&length, canvasReference->image);
    WriteToFile(pngdata, length, "BacktestChart.png");
    DeleteImage(canvasReference->image);
}

void BacktestVizualizer(BacktestSumUp *BS)
{
    printf("\nBACKTEST : \n");
    for (int i = 1; i < BS->BacktestHistory->numberTrades; i++)
    {
        printf("| Trade Number   : %d\n", i);
        printf("| Trade Type     : %d\n", BS->BacktestHistory->type[i]);
        printf("| Entry/Exit Day : %d -> %d\n", BS->BacktestHistory->EntryDate[i], BS->BacktestHistory->ExitDate[i]);
        printf("| Trade Length   : %d\n", BS->BacktestHistory->ExitDate[i] - BS->BacktestHistory->EntryDate[i]);
        printf("| Trade Quantity : %f\n", BS->BacktestHistory->quantity[i] * BS->BacktestHistory->type[i]);
        printf("| Trade Leverage : x%.f\n", BS->BacktestHistory->leverage[i]);
        printf("| Trade Mise     : %.2f $\n", BS->BacktestHistory->chartMise[i]);
        printf("| Trade Entry    : %f $\n", BS->BacktestHistory->entry[i]);
        printf("| Trade Exit     : %f $\n", BS->BacktestHistory->exit[i]);
        printf("| Trade PNL      : %.2f$\n", BS->BacktestHistory->pnl[i]);
        printf("| Trade Variation: %+.2f%%\n", BS->BacktestHistory->variation[i]);
        printf("| Actuel Bankroll: %+.2f$\n", BS->BacktestHistory->chartBK[i]);
        printf("--------------------------------\n");
    }
    printf("BankrollStart  : %+.0f$\n", BS->bankroll);
    printf("Mise          : %.0f$\n", BS->mise);
    printf("Number Trades : %d\n", BS->numberTrades);
    printf("Winning Trades: %d\n", BS->WinnerTrades);
    printf("Looser Trades : %d\n", BS->LooserTrades);
    printf("Winrate       : %.2f%%\n", BS->winrate);
    printf("PNL           : %+.2f$\n", BS->PNL);
    printf("Variation     : %+.2f%%\n", BS->variation);
    printf("Max Drawdown  : %.2f%%\n", BS->maxDrawdown);
    printf("Max Bankroll  : %.2f$\n", BS->maxBankroll);
    printf("Min Bankroll  : %.2f$\n", BS->minBankroll);
    printf("Max Loss      : %.2f$\n", BS->maxLoss);
    printf("Sum Loss      : %.2f$\n", BS->sumLoss);
    printf("Average Loss  : %.2f$\n", BS->averageLoss);
    printf("Max Profits   : %.2f$\n", BS->maxProfits);
    printf("Sum Profits   : %.2f$\n", BS->sumProfits);
    printf("Average Profit: %.2f$\n", BS->averageProfits);
    printf("Average Variat: %.2f%%\n", BS->averageVariation);
    printf("BankrollEnd   : %+.0f$\n", BS->endBankroll);
    printf("---------------------- \n");
    ShowChartBacktest(BS->BacktestHistory);
}

double AutoTPSL(TradesData *T, BacktestHistory *B, StockData *m, int i, BacktestOptions *Option)
{
    double isWin = 0;
    double TP = Option->TakeProfit;
    double SL = Option->StopLoss;
    if (T->isActive == 0)
    {
        return 0;
    }
    if (TP == 0 && SL == 0)
    {
        return 0;
    }
    if (((m->close[i] / T->entry) - 1) * T->type * T->leverage >= TP && TP != 0)
    {
        isWin = CloseTrade(m, B, T, i);
    }
    else if (((m->close[i] / T->entry) - 1) * T->type * T->leverage <= -SL && SL != 0)
    {
        isWin = CloseTrade(m, B, T, i);
    }
    return isWin;
}

double TrailingStop(TradesData *T, StockData *m, BacktestHistory *B, int i, double SL, double *TS)
{
    double price = m->close[i];
    double isWin = 0;
    if (*TS == 0)
    {
        *TS = (price + T->type * price * SL);
    }
    if (T->type == 1)
    {
        if ((price + price * SL) > *TS)
        {
            *TS = (price + price * SL);
        }
        if (price < *TS && T->isActive == 1)
        {
            isWin = CloseTrade(m, B, T, i);
            *TS = 0;
        }
    }
    else
    {
        if ((price - price * SL) < *TS)
        {
            *TS = (price - price * SL);
        }
        if (price > *TS && T->isActive == 1)
        {
            isWin = CloseTrade(m, B, T, i);
            *TS = 0;
        }
    }

    return isWin;
}

double TradeActualProfit(StockData *m, TradesData *T, int i)
{
    if (T->type == 1)
    {
        T->variation = ((m->close[i] - T->entry) / T->entry) * 100 * T->leverage;
    }
    else
    {
        T->variation = ((T->entry - m->close[i]) / m->close[i]) * 100 * T->leverage;
    }
    return T->variation * T->mise / 100;
}

double OptionsUpdater(StockData *m, BacktestHistory *B, TradesData *T, BacktestOptions *Option, int i, double *lastWon, double *mise, double *TS)
{
    if (Option->MiseRelative && B->numberTrades < 2)
    {
        *mise *= 1 + (Option->Mise / Option->StartBankroll);
    }
    if (Option->TakeProfit > 0 || Option->StopLoss > 0)
    {
        *lastWon = AutoTPSL(T, B, m, i, Option);
    }
    else if (Option->TrailingStop > 0)
    {
        *lastWon = TrailingStop(T, m, B, i, Option->TrailingStop, TS);
    }
    if (Option->Martingale > 0 && B->ExitDate[B->numberTrades] + 1 == (int)i)
    {
        if (B->pnl[B->numberTrades] < 0)
        {
            *mise *= Option->Martingale;
        }
        else
        {
            *mise = Option->Mise;
        }
    }
    if (T->isActive && (Option->Cross || Option->Isolated))
    {
        if (B->chartBK[B->numberTrades] + TradeActualProfit(m, T, i) <= 0)
        {
            //printf("lol,%d %f %f\n", B->numberTrades, B->chartBK[B->numberTrades], TradeActualProfit(m, T, i));
            return 1;
        }
        if (Option->Isolated && (T->mise + TradeActualProfit(m, T, i) <= 0))
        {
            CloseTrade(m, B, T, i);
        }
    }
    return 0;
}

//BACKTEST PATTERN
//Init BacktestParam : no changes
//Init Option        : no changes
//Init Indicator     : changes
double BacktestMA(StockData *m, BacktestHistory *B, BacktestOptions *Option, double fastP, double slowP)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    //Indicators
    double *fast = malloc(sizeof(double) * m->range);
    double *slow = malloc(sizeof(double) * m->range);
    MA(m, fast, fastP);
    MA(m, slow, slowP);
    size_t start = slowP + 1;

    //Start Backtest
    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }

        //SIGNALS
        //Custom Strategy
        if (fast[i - 1] <= slow[i - 1] && fast[i] > slow[i])
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (fast[i - 1] >= slow[i - 1] && fast[i] < slow[i])
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestRSI(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period, double oversell, double overbuy)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    double *rsi = malloc(sizeof(double) * m->range);
    RSI(m, rsi, period);
    size_t start = period + 1;

    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }

        //SIGNALS
        if (rsi[i - 1] < oversell && rsi[i] >= oversell)
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (rsi[i - 1] > overbuy && rsi[i] <= overbuy)
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
        if (rsi[i - 1] > oversell && rsi[i] <= oversell)
        {
            if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
            }
        }
        else if (rsi[i - 1] < overbuy && rsi[i] >= overbuy)
        {
            if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestMOM(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    double *mom0 = malloc(sizeof(double) * m->range);
    Momentum(m, mom0, period);
    size_t start = period + 1;

    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        //SIGNALS
        if (mom0[i - 1] > 0 && mom0[i] <= 0)
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (mom0[i - 1] < 0 && mom0[i] >= 0)
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestMACD(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    double *macd = malloc(sizeof(double) * m->range);
    double *signal = malloc(sizeof(double) * m->range);
    double *histo = malloc(sizeof(double) * m->range);
    MACD(m, macd, signal, histo);
    size_t start = period + 1;

    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        //SIGNALS
        if (histo[i - 1] < 0 && histo[i] >= 0)
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (histo[i - 1] > 0 && histo[i] <= 0)
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestBB(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t mult, size_t period)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    double *upper = malloc(sizeof(double) * m->range);
    double *lower = malloc(sizeof(double) * m->range);
    double *basis = malloc(sizeof(double) * m->range);
    BollingerBands(m, upper, lower, basis, mult, period);
    size_t start = period + 1;

    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        //SIGNALS
        if (m->close[i - 1] < lower[i - 1] && m->close[i] >= lower[i])
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (m->close[i - 1] > upper[i - 1] && m->close[i] <= upper[i])
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
        if (m->close[i - 1] > lower[i - 1] && m->close[i] <= lower[i])
        {
            if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
            }
        }
        else if (m->close[i - 1] < upper[i - 1] && m->close[i] >= upper[i])
        {
            if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestStochastic(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    double *K = malloc(sizeof(double) * m->range);
    double *D = malloc(sizeof(double) * m->range);
    Stochastic(m, K, D, 3, period);
    size_t start = period + 1;

    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        //SIGNALS
        if (K[i - 1] < D[i - 1] && K[i] >= D[i])
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (K[i - 1] > D[i] && K[i] <= D[i])
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestWilliamR(StockData *m, BacktestHistory *B, BacktestOptions *Option, double overbought, double oversold, size_t period)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    double *w = malloc(sizeof(double) * m->range);
    WilliamR(m, w, period);
    size_t start = period + 1;

    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        //SIGNALS
        if (w[i - 1] > oversold && w[i] <= oversold)
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (w[i - 1] < overbought && w[i] >= overbought)
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
        if (w[i - 1] < oversold && w[i] >= oversold)
        {
            if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
            }
        }
        else if (w[i - 1] > overbought && w[i] <= overbought)
        {
            if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestSuperTrend(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    double *m1 = malloc(sizeof(double) * m->range);
    double *m2 = malloc(sizeof(double) * m->range);
    double *m3 = malloc(sizeof(double) * m->range);
    double *ST = malloc(sizeof(double) * m->range);
    double *m5 = malloc(sizeof(double) * m->range);
    double *m6 = malloc(sizeof(double) * m->range);
    SuperTrend(m, m1, m2, m3, ST, m5, m6);
    size_t start = period + 1;

    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        //SIGNALS
        if (ST[i - 1] > m->close[i - 1] && ST[i] <= m->close[i])
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (ST[i - 1] < m->close[i - 1] && ST[i] >= m->close[i])
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestMAoffset(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period, int offset)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    //Indicators
    double *m1 = malloc(sizeof(double) * m->range);
    double *m2 = malloc(sizeof(double) * m->range);
    MA(m, m1, period);
    Offset(m1, m->range, offset);
    MA(m, m2, period);
    Offset(m2, m->range, -offset);
    size_t start = period + 1 + offset;

    //Start Backtest
    for (size_t i = start + 1; i < m->range - offset; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }

        //SIGNALS
        //Custom Strategy
        if (m1[i - 1] > m2[i - 1] && m1[i] <= m2[i])
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i + offset, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i + offset);
                CreateTrade(m, B, T, i + offset, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i + offset, 1, *mise, *lev);
            }
        }
        else if (m1[i - 1] < m2[i - 1] && m1[i] >= m2[i])
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i + offset, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i + offset);
                CreateTrade(m, B, T, i + offset, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i + offset, -1, *mise, *lev);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    return B->resPNL;
}

double BacktestIchimoku(StockData *m, BacktestHistory *B, BacktestOptions *Option)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    //Indicators
    double *Tenkan = malloc(sizeof(double) * m->range);
    double *Kijun = malloc(sizeof(double) * m->range);
    double *Chikou = malloc(sizeof(double) * m->range);
    double *SSA = malloc(sizeof(double) * m->range);
    double *SSB = malloc(sizeof(double) * m->range);
    Ichimoku(m, Tenkan, Kijun, Chikou, SSA, SSB);
    size_t start = 52 + 1;

    double strength = 1;
    //Start Backtest
    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        strength++;
        //SIGNALS
        //Custom Strategy
        if (m->close[i] > SSA[i] && m->close[i] > SSB[i] && Tenkan[i] > Kijun[i] && Chikou[i] > m->close[i])
        {
            if (Tenkan[i] > SSA[i] && Tenkan[i] > SSB[i] && Kijun[i] > SSA[i] && Kijun[i] > SSB[i])
            {
                strength = 3;
            }
            else if (Tenkan[i] < SSA[i] && Tenkan[i] < SSB[i] && Kijun[i] < SSA[i] && Kijun[i] < SSB[i])
            {
                strength = 1;
            }
            else
            {
                strength = 2;
            }
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        if (m->close[i] < SSA[i] && m->close[i] < SSB[i] && Tenkan[i] < Kijun[i] && Chikou[i] < m->close[i])
        {
            if (Tenkan[i] > SSA[i] && Tenkan[i] > SSB[i] && Kijun[i] > SSA[i] && Kijun[i] > SSB[i])
            {
                strength = 3;
            }
            else if (Tenkan[i] < SSA[i] && Tenkan[i] < SSB[i] && Kijun[i] < SSA[i] && Kijun[i] < SSB[i])
            {
                strength = 1;
            }
            else
            {
                strength = 2;
            }
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    return B->resPNL;
}

double BacktestROC(StockData *m, BacktestHistory *B, BacktestOptions *Option, size_t period)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    double *roc = malloc(sizeof(double) * m->range);
    Roc(m, roc, period);
    size_t start = period + 1;

    for (size_t i = start + 1; i < m->range; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        //SIGNALS
        if (roc[i - 1] < 0 && roc[i] >= 0)
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (roc[i - 1] > 0 && roc[i] <= 0)
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestZigzag(StockData *m, BacktestHistory *B, BacktestOptions *Option, double percent)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    int first = 0;
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    int *zigzag = ZigZag(m, percent);
    size_t start = 0;

    for (size_t i = start; i < m->range - 1; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        //SIGNALS
        if (zigzag[i + 1] > 0)
        {
            //Classic Buy
            if (first == 0)
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == -1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == 1))
            {
                CreateTrade(m, B, T, i, 1, *mise, *lev);
            }
        }
        else if (zigzag[i + 1] < 0)
        {
            //Classic Sell
            if (first == 0)
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
                first++;
            }
            else if (T->isActive && T->type == 1)
            {
                *lastWon = CloseTrade(m, B, T, i);
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
            else if (!(T->isActive && T->type == -1))
            {
                CreateTrade(m, B, T, i, -1, *mise, *lev);
            }
        }
    }
    if (T->isActive == 1)
    {
        CloseTrade(m, B, T, m->range - 1);
    }
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestHODL(StockData *m, BacktestHistory *B, BacktestOptions *Option)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    CreateTrade(m, B, T, 0, 1, Option->Mise, Option->Leverage);
    CloseTrade(m, B, T, m->range - 1);
    TradesData_destroy(T);
    return B->resPNL;
}

double BacktestPerfect(StockData *m, BacktestHistory *B, BacktestOptions *Option)
{
    //Backtest Parameters
    TradesData *T = TradesDataInit();
    double *lev = malloc(sizeof(double));
    *lev = Option->Leverage;
    double *mise = malloc(sizeof(double));
    *mise = Option->Mise;
    double *lastWon = malloc(sizeof(int));
    *lastWon = 0;

    //Option
    double *TS = malloc(sizeof(double));
    *TS = 0;

    size_t start = 0;

    for (size_t i = start; i < m->range - 1; i++)
    {
        //OPTIONS
        if (OptionsUpdater(m, B, T, Option, i, lastWon, mise, TS))
        {
            return 0;
        }
        //SIGNALS
        if (m->close[i + 1] > m->close[i])
        {
            //Classic Buy
            CreateTrade(m, B, T, i, 1, *mise, *lev);
            CloseTrade(m, B, T, i + 1);
        }
        else if (m->close[i + 1] < m->close[i])
        {
            CreateTrade(m, B, T, i, -1, *mise, *lev);
            CloseTrade(m, B, T, i + 1);
        }
    }
    TradesData_destroy(T);
    return B->resPNL;
}

BacktestSumUp *Backtest(StockData *m, BacktestOptions *Option, int strat)
{
    BacktestHistory *B = BacktestInit(Option, m->range);
    switch (strat)
    {
    case 1:
        BacktestMA(m, B, Option, 50, 200);
        break;

    case 2:
        BacktestRSI(m, B, Option, 14, 40, 60);
        break;

    case 3:
        BacktestMOM(m, B, Option, 12);
        break;

    case 4:
        BacktestMACD(m, B, Option, 26);
        break;

    case 5:
        BacktestBB(m, B, Option, 1.5, 14);
        break;

    case 6:
        BacktestStochastic(m, B, Option, 26);
        break;

    case 7:
        BacktestWilliamR(m, B, Option, 20, 80, 26);
        break;

    case 8:
        BacktestSuperTrend(m, B, Option, 10);
        break;

    case 9:
        BacktestMAoffset(m, B, Option, 50, 10);
        break;

    case 10:
        BacktestIchimoku(m, B, Option);
        break;

    case 11:
        BacktestROC(m, B, Option, 9);
        break;

    case 12:
        BacktestZigzag(m, B, Option, 0.05);
        break;

    case 13:
        BacktestHODL(m, B, Option);
        break;

    case 14:
        BacktestPerfect(m, B, Option);
        break;

    default:
        break;
    }
    BacktestSumUp *BS = BacktestSumUpInit(B);
    BacktestVizualizer(BS);
    return BS;
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        err(1, "Pas assez d'argument\n format :\n./backtest <SYMBOL> <INTER> <RANGE> <STRAT> <BK> <MISE> <LEV> <MARTIN> <TP> <SL> <TS> <ISO> <CROSS> <MiseRela>");
    }

    BacktestOptions *Option = BacktestOptionsInit();
    for (int i = 5; i < argc; i++)
    {
        switch (i)
        {
        case 5:
            Option->StartBankroll = atoi(argv[i]);
            break;
        case 6:
            Option->Mise = atoi(argv[i]);
            break;
        case 7:
            Option->Leverage = atoi(argv[i]);
            break;
        case 8:
            Option->Martingale = atoi(argv[i]);
            break;
        case 9:
            Option->TakeProfit = atoi(argv[i]);
            break;
        case 10:
            Option->StopLoss = atoi(argv[i]);
            break;
        case 11:
            Option->TrailingStop = atoi(argv[i]);
            break;
        case 12:
            Option->Isolated = atoi(argv[i]);
            break;
        case 13:
            Option->Cross = atoi(argv[i]);
            break;
        case 14:
            Option->MiseRelative = atoi(argv[i]);
            break;
        default:
            break;
        }
    }
    StockData *Mat = APItoTXTAIO(argv[1], argv[2], argv[3]);
    //StockData *Mat = to_Struct("backtest_temp");
    Backtest(Mat, Option, atoi(argv[4]));
    return 0;
}