# ifndef DATADOWNLOADER_H_
# define DATADOWNLOADER_H_


struct StockData;

void bufferprinter(char *b, size_t len);

void APItoTXTAIO(char* symbol, char* interval, char* range);


# endif