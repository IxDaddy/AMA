#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "StockData.h"


double char_To_Double(char* tmp)
{
	char* end;
	double res;
	res = strtod(tmp,&end);
	return res;
}


StockData *to_Struct(char* file)
{
    StockData *m = StockData_new(365);
    
	FILE *fp;
	fp = fopen(file,"r");
	if (fp == NULL)
	{
		printf("error fopen\n");
        return m;
	}
	//tailles de listes
	int i = 0;
	size_t l = 0;
	size_t i_open = 0;
	size_t i_high = 0;
	size_t i_low = 0;
	size_t i_close = 0;
	char tmp[20];


	//char a traiter
	char c = fgetc(fp);
	while(c != '\0')
	{
		if(c == ',')
		{
			tmp[l] = ';' ;
			double res = char_To_Double(tmp);
			switch(i)
			{
				case(0):
					m->open[i_open] = res;
					i_open ++;
					break;
				case(1):
					m->high[i_high] = res;
					i_high ++;
					break;
				case(2):
					m->low[i_low] = res;
					i_low ++;
					break;
				case(3):
					m->close[i_close] = res;
					i_close ++;
					break;
			}
			i++;
			if(i > 3)
				i = 0;
			l = 0;
			//memset(&tmp[0],20,0);
		}
		else
		{
			tmp[l] = c;
			l++;
		}
		c = fgetc(fp);
	}
	return m;
}
