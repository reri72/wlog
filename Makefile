CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lpthread
TARGETA = libwlog.a
TARGETS = libwlog.so
SRCSDIR = src
TESTDIR = test
AR = ar src
SHARED = -shared
PIC = -fPIC
HEADERDIR = include

SRCS = $(wildcard $(SRCSDIR)/*.c)
OBJS = $(SRCS:.c=.o)

liba: $(TARGETA)
libs: $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -I $(HEADERDIR) $(LDFLAGS)

$(TARGETA): $(OBJS)
	$(AR) $(TARGETA) $(OBJS)

$(TARGETS):
	$(CC) $(OBJS) $(SHARED) -o $(TARGETS)

clean:
	rm -f $(SRCSDIR)/*.o
	rm -f $(TARGETA)
	rm -f $(TARGETS)
