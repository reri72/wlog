CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread
TARGET= testprint
SRCSDIR = src
AR = ar rcs
HEADERDIR = include
SRCS = $(wildcard $(SRCSDIR)/*.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
lib: $(MAKELIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -I $(HEADERDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(SRCSDIR)/*.o
	rm -f $(TARGET)
