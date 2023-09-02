CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread
TARGET= libwlog.a
SRCSDIR = src
TESTDIR = test
AR = ar src
SHARED = -shared
HEADERDIR = include

SRCS = $(wildcard $(SRCSDIR)/*.c)
OBJS = $(SRCS:.c=.o)


lib: $(MAKELIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -I $(HEADERDIR) $(LDFLAGS)

lib: $(OBJS)
	$(AR) $(TARGET) $(OBJS)

clean:
	rm -f $(SRCSDIR)/*.o
	rm -f $(TARGET)
