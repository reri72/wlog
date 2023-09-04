CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lpthread
TARGET = libwlog.a
SRCSDIR = src
TESTDIR = test
AR = ar src
SHARED = -shared
PIC = -fPIC
HEADERDIR = include

SRCS = $(wildcard $(SRCSDIR)/*.c)
OBJS = $(SRCS:.c=.o)

liba: $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -I $(HEADERDIR) $(LDFLAGS)

$(TARGET): $(OBJS)
	$(AR) $(TARGET) $(OBJS)

clean:
	rm -f $(SRCSDIR)/*.o
	rm -f $(TARGET)
