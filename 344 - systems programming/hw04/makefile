CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lpthread
TARGET = LogAnalyzer
SRCS = 200104004039.c buffer.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)