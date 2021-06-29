CC = gcc
CFLAGS =  -Wall -Wextra -Werror -g

LDFLAGS = -lcurl -ljson-c -lm

SRC = DataDownloader.c signals/Signal.c signals/BinanceData.c \
	StockData.c indicators/rsi.c indicators/stochastic.c indicators/ma.c \
	indicators/awesomeOcillator.c indicators/ema.c indicators/atr.c \
	indicators/macd.c indicators/ichimoku.c indicators/parabolicSAR.c \
	indicators/WilliamR.c indicators/momentum.c indicators/offset.c \
	indicators/Supertrend.c indicators/Roc.c indicators/offset.c indicators/bollingerBands.c
	
OBJ = $(SRC:.c=.o)
DEPS = $(SRC:.c=.h)

OUT	= main
RELEASE = ama

all: debug

debug: $(OBJ) main.c
	$(CC) $^ -o $(OUT) $(LDFLAGS) $(CFLAGS)

release: $(SRC) $(DEPS) main.c
	$(CC) $^ -o $(RELEASE) $(LDFLAGS)

%.o: %.c %.h
	$(CC) -o $@ -c $< $(LDFLAGS) $(CFLAGS)

clean:
	rm -f $(OBJ) $(OUT) $(RELEASE) bot
