OBJS	= DataDownloader.o main.o
SOURCE	= DataDownloader.c main.c
HEADER	= DataDownloader.h
OUT	= main
CC	 = gcc

FLAGS	 = -g -c 
LFLAGS	 = -ljson-c -lcurl

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

DataDownloader.o: DataDownloader.c
	$(CC) $(FLAGS) DataDownloader.c -std=c99

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99

clean:
	rm -f $(OBJS) $(OUT)