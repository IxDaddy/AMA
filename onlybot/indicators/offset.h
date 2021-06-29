#ifndef OFFSET_H
#define OFFSET_H

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../StockData.h"

void Offset(double *l, size_t len, int k);

int *best_trade(StockData *data, int period);

void Show(int *best, size_t len, double* close);

int* ZigZag(StockData *sourceStock, double dPourcentage);

int *Zigzag2(StockData *m);

#endif