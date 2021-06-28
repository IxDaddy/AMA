#include <stdlib.h>
#include <err.h>

#include "StockData.h"

StockData* StockData_new(size_t nb_timestamp)
{
	StockData *new = malloc(sizeof(StockData));
	if(new == NULL)
		err(1, "Out of memory");
	new->range = nb_timestamp;
	new->symbol = NULL;
	new->StrInterval = NULL;
	new->StrRange = NULL;
	
	new->timestamp = malloc(sizeof(int) * nb_timestamp);
	if(new->timestamp == NULL)
		err(1, "Out of memory");
	
	new->high = malloc(sizeof(double) * nb_timestamp);
	if(new->high == NULL)
		err(1, "Out of memory");

	new->low = malloc(sizeof(double) * nb_timestamp);
	if(new->low == NULL)
		err(1, "Out of memory");

	new->open = malloc(sizeof(double) * nb_timestamp);
	if(new->open == NULL)
		err(1, "Out of memory");

	new->close = malloc(sizeof(double) * nb_timestamp);
	if(new->close == NULL)
		err(1, "Out of memory");

	new->volume = malloc(sizeof(double) * nb_timestamp);
	if(new->volume == NULL)
		err(1, "Out of memory");

	return new;
}

void StockData_destroy(StockData* data)
{
	if(data->symbol != NULL)
		free(data->symbol);

	free(data->timestamp);
	free(data->high);
	free(data->low);
	free(data->open);
	free(data->close);
	free(data->volume);
	free(data);
}
