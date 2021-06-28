#ifndef	PLOT_H
#define	PLOT_H

#define TEMP_GRAPH_FILENAME "temp_graph.png"
#define TEMP_INDIC_FILENAME "temp_indic.png"

#include "../StockData.h"

void BuildChart(StockData *mat, int* ids, char** indicators_datas, int nb_indicators);

#endif