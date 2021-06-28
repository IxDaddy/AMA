CC = gcc
CFLAGS =  -Wall -Wextra -Werror -g
LIB_DIRS = -Llibs -Iinclude -Iinclude/gtk-3.0 \
	-Iinclude/at-spi-2.0 -Iinclude/gio-unix-2.0 -Iinclude/cairo \
	-Iinclude/pango-1.0 -Iinclude/fribidi -Iinclude/harfbuzz \
	-Iinclude/pixman-1 -Iinclude/uuid -Iinclude/freetype2 \
	-Iinclude/libpng16 -Iinclude/gdk-pixbuf-2.0 -Iinclude/libmount \
	-Iinclude/blkid -Iinclude/glib-2.0 -Iinclude/glib-2.0/include
LDFLAGS = $(LIB_DIRS) -export-dynamic -lm -lgtk-3 -lgdk-3 -lpangocairo-1.0 \
	-lpango-1.0 -lharfbuzz -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 \
	-lgio-2.0 -lgobject-2.0 -lglib-2.0 -lcurl -ljson-c -pthread

SRC = backtests/BacktestStructs.c to_struct.c DataDownloader.c Glade/ui.c plot/plot.c plot/pbPlots.c plot/supportLib.c \
	StockData.c indicators/rsi.c indicators/stochastic.c indicators/ma.c \
	indicators/awesomeOcillator.c indicators/ema.c indicators/atr.c \
	indicators/bollingerBands.c indicators/macd.c indicators/ichimoku.c \
	indicators/WilliamR.c indicators/momentum.c strategies/strat_rsi.c \
	strategies/strat_MACD.c strategies/strat_MA.c indicators/Supertrend.c \
	indicators/Roc.c indicators/offset.c indicators/parabolicSAR.c signals/Signal.c signals/BinanceData.c

OBJ = $(SRC:.c=.o)
DEPS = $(SRC:.c=.h)

OUT	= main
RELEASE = ama

all: debug

backtest: $(OBJ) backtests/Backtest.c
	$(CC) $^ -o backtest $(LDFLAGS) $(CFLAGS)

signal: $(OBJ) signals/Signal.c
	$(CC) $^ -o signal $(LDFLAGS) $(CFLAGS)

bot: $(OBJ) signals/bot.c
	$(CC) $^ -o bot $(LDFLAGS) $(CFLAGS)

debug: $(OBJ) main.c
	$(CC) $^ -o $(OUT) $(LDFLAGS) $(CFLAGS)

release: $(SRC) $(DEPS) main.c
	$(CC) $^ -o $(RELEASE) $(LDFLAGS)

%.o: %.c %.h
	$(CC) -o $@ -c $< $(LDFLAGS) $(CFLAGS)

clean:
	rm -f $(OBJ) $(OUT) $(RELEASE) backtest signal bot
