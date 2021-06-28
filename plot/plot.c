#include <math.h>
#include "plot.h"
#include "pbPlots.h"
#include "supportLib.h"
#include "../indicators/ma.h"
#include "../indicators/rsi.h"
#include "../indicators/stochastic.h"
#include "../indicators/awesomeOcillator.h"
#include "../indicators/ema.h"
#include "../indicators/atr.h"
#include "../indicators/bollingerBands.h"
#include "../indicators/macd.h"
#include "../indicators/ichimoku.h"
#include "../indicators/WilliamR.h"
#include "../indicators/momentum.h"
#include "../indicators/offset.h"
#include "../indicators/Supertrend.h"
#include "../indicators/Roc.h"
#include "../indicators/parabolicSAR.h"

void new_image(int width, int height, ScatterPlotSeries **s, size_t nb_plots, char* filename)
{
	RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
	ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = width;
	settings->height = height;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	settings->title = L"";
	settings->titleLength = wcslen(settings->title);
	settings->xLabel = L"";
	settings->xLabelLength = wcslen(settings->xLabel);
	settings->yLabel = L"";
	settings->yLabelLength = wcslen(settings->yLabel);
	settings->scatterPlotSeries = s;
	settings->scatterPlotSeriesLength = nb_plots;

	DrawScatterPlotFromSettings(imageReference, settings);

	size_t length;
	double *pngdata = ConvertToPNG(&length, imageReference->image);
	WriteToFile(pngdata, length, filename);
	DeleteImage(imageReference->image);
}

ScatterPlotSeries* new_curve(double* x, double* y, StockData* mat, RGBA* color)
{
	ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
	series->xs = x;
	series->xsLength = mat->range;
	series->ys = y;
	series->ysLength = mat->range;
	series->linearInterpolation = true;
	series->lineType = L"solid";
	series->lineTypeLength = wcslen(series->lineType);
	series->lineThickness = 1;
	series->color = color;

	return series;
}

void max_val(StockData* mat, double* max, double* min)
{
	*max = 0;
	*min = INFINITY;

	for(size_t i = 0; i < mat->range; i++)
	{
		if(mat->close[i] > *max)
			*max = mat->close[i];
		if(mat->close[i] < *min)
			*min = mat->close[i];
	}
}

void list_set(double* mm, size_t size, double val)
{
	for(size_t i = 0; i < size; i++)
	{
		mm[i] = val;
	}
}

void BuildChart(StockData *mat, int* ids, char** indicators_datas, int nb_indicators)
{
	double x[mat->range];
	double* y = malloc(sizeof(double) * mat->range);
	double* y2 = malloc(sizeof(double) * mat->range);

	//log the chart TEST
	int isLog = 0;

	for (size_t i = 0; i < mat->range; i++)
	{
		x[i] = i;
		y[i] = mat->close[i];
		y2[i] = 100;
	}
	if (isLog == 1)
	{
		for (size_t i = 0; i < mat->range; i++)
		{
			x[i] = i;
			y[i] = log10(mat->close[i]);
			y2[i] = 100;
		}
	}
	else
	{
		for (size_t i = 0; i < mat->range; i++)
		{
			x[i] = i;
			y[i] = mat->close[i];
			y2[i] = 100;
		}
	}
	
	
	y2[mat->range-1] = 0;

	size_t nb_plots_graph = 1;
	size_t nb_plots_indic = 1;
	for(int i = 0; i < nb_indicators; i++)
	{
		if(ids[i] == 1 || ids[i] == 5)
			nb_plots_graph++;
		else if(ids[i] == 7)
			nb_plots_graph += 2;
		else if(ids[i] == 9)
			nb_plots_graph += 5;
		else if(ids[i] == 2 || ids[i] == 4 || ids[i] == 6 || ids[i] == 10 || ids[i] == 11)
			nb_plots_indic++;
		else if(ids[i] == 3)
			nb_plots_indic += 2;
		else if(ids[i] == 8)
			nb_plots_indic += 3;
	}

	ScatterPlotSeries **s1 = malloc(sizeof(ScatterPlotSeries*) * nb_plots_graph);
	ScatterPlotSeries **s2 = malloc(sizeof(ScatterPlotSeries*) * nb_plots_indic);
	size_t i1 = 0;
	size_t i2 = 0;
	s1[i1++] = new_curve(x, y, mat, CreateRGBColor(0, 0, 0));
	s2[i2++] = new_curve(x, y2, mat, CreateRGBColor(1, 1, 1));

	for(int j = 0; j < nb_indicators; j++)
	{
		if(ids[j] == 1)
		{
			
			double* mm = malloc(sizeof(double) * mat->range);

			long unsigned int st_val = atoi(indicators_datas[j]);
			MA(mat, mm, st_val);
			s1[i1++] = new_curve(x, mm, mat, CreateRGBColor(1, 0, 0));
			
			// //TEST SUPERTREND
			// //long unsigned int st_val = atoi(indicators_datas[j]);
			// double* m1 = malloc(sizeof(double) * mat->range);
			// double* m2 = malloc(sizeof(double) * mat->range);
			// double* m3 = malloc(sizeof(double) * mat->range);
			// double* m4 = malloc(sizeof(double) * mat->range);
			// double* m5 = malloc(sizeof(double) * mat->range);
			// double* m6 = malloc(sizeof(double) * mat->range);
			// SuperTrend(mat,m1,m2,m3,m4,m5,m6);
			// //s1[i1++] = new_curve(x, mm1, mat, CreateRGBColor(1, 0, 0));
			// s1[i1++] = new_curve(x, m4, mat, CreateRGBColor(0, 1, 0));
		}
		else if(ids[j] == 2)
		{
			double* mm = malloc(sizeof(double) * mat->range);

			long int param = strtol(indicators_datas[j], NULL, 10);
			RSI(mat, mm, param);
			s2[i2++] = new_curve(x, mm, mat, CreateRGBColor(0, 0, 1));
		}
		else if(ids[j] == 3)
		{
			double* mm1 = malloc(sizeof(double) * mat->range);
			double* mm2 = malloc(sizeof(double) * mat->range);
			char* txt;
			long int param1 = strtol(indicators_datas[j], &txt, 10);
			long int param2 = strtol(txt, NULL, 10);

			Stochastic(mat, mm1, mm2, param1, param2);

			s2[i2++] = new_curve(x, mm1, mat, CreateRGBColor(1, 0, 0));
			s2[i2++] = new_curve(x, mm2, mat, CreateRGBColor(0, 1, 0));
		}
		else if(ids[j] == 4)
		{
			double* mm = malloc(sizeof(double) * mat->range);
			AwesomeOscillator(mat, mm);
			s2[i2++] = new_curve(x, mm, mat, CreateRGBColor(0, 1, 0));
		}
		else if(ids[j] == 5)
		{
			double* mm = malloc(sizeof(double) * mat->range);

			long unsigned int st_val = atoi(indicators_datas[j]);
			EMA(mat, mm, st_val);
			s1[i1++] = new_curve(x, mm, mat, CreateRGBColor(1, 0, 0));
		}
		else if(ids[j] == 6)
		{
			double* mm = malloc(sizeof(double) * mat->range);

			long unsigned int param = atoi(indicators_datas[j]);
			atr(mat, mm, param);
			s2[i2++] = new_curve(x, mm, mat, CreateRGBColor(0, 0, 1));
		}
		else if(ids[j] == 7)
		{
			double* mm1 = malloc(sizeof(double) * mat->range);
			double* mm2 = malloc(sizeof(double) * mat->range);
			double* mm3 = malloc(sizeof(double) * mat->range);

			long unsigned int param = atoi(indicators_datas[j]);

			BollingerBands(mat, mm1, mm2, mm3, 2, param);

			s1[i1++] = new_curve(x, mm1, mat, CreateRGBColor(1, 0, 0));
			s1[i1++] = new_curve(x, mm2, mat, CreateRGBColor(0, 1, 0));
			s1[i1++] = new_curve(x, mm3, mat, CreateRGBColor(0, 0, 1));
		}
		else if(ids[j] == 8)
		{
			double* mm1 = malloc(sizeof(double) * mat->range);
			double* mm2 = malloc(sizeof(double) * mat->range);
			double* mm3 = malloc(sizeof(double) * mat->range);
			
			MACD(mat, mm1, mm2, mm3);

			s2[i2++] = new_curve(x, mm1, mat, CreateRGBColor(1, 0, 0));
			s2[i2++] = new_curve(x, mm2, mat, CreateRGBColor(0, 1, 0));
			s2[i2++] = new_curve(x, mm3, mat, CreateRGBColor(0, 0, 1));
		}
		else if(ids[j] == 9)
		{
			double* mm1 = malloc(sizeof(double) * mat->range);
			double* mm2 = malloc(sizeof(double) * mat->range);
			double* mm3 = malloc(sizeof(double) * mat->range);
			double* mm4 = malloc(sizeof(double) * mat->range);
			double* mm5 = malloc(sizeof(double) * mat->range);

			Ichimoku(mat, mm1, mm2, mm3, mm4, mm5);

			s1[i1++] = new_curve(x, mm1, mat, CreateRGBColor(1, 0, 0));
			s1[i1++] = new_curve(x, mm2, mat, CreateRGBColor(0, 1, 0));
			s1[i1++] = new_curve(x, mm3, mat, CreateRGBColor(0, 0, 1));
			s1[i1++] = new_curve(x, mm4, mat, CreateRGBColor(1, 1, 0));
			s1[i1++] = new_curve(x, mm5, mat, CreateRGBColor(0, 1, 1));
		}
		else if(ids[j] == 10)
		{
			double* mm = malloc(sizeof(double) * mat->range);

			long unsigned int param = atoi(indicators_datas[j]);

			WilliamR(mat, mm, param);

			s2[i2++] = new_curve(x, mm, mat, CreateRGBColor(1, 0, 0));
		}
		else if(ids[j] == 11)
		{
			double* mm = malloc(sizeof(double) * mat->range);
			
			long unsigned int param = atoi(indicators_datas[j]);

			Momentum(mat, mm, param);

			s2[i2++] = new_curve(x, mm, mat, CreateRGBColor(1, 0, 0));
		}
	}

	new_image(1200, 600, s1, nb_plots_graph, TEMP_GRAPH_FILENAME);
	if(nb_plots_indic > 1)
		new_image(1200, 200, s2, nb_plots_indic, TEMP_INDIC_FILENAME);
}
