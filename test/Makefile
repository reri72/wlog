CC = gcc
LDFLAGS = -lwlog -lpthread
TARGET = test
HEADERDIR = ../include
SRCS = test.c

all: $(TARGET)

$(TARGET): 
	$(CC) -o $(TARGET) $(SRCS) -I$(HEADERDIR) -L../ $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -f test.o
