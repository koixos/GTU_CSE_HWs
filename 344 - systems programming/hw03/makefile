CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lpthread
TARGET = hw03
SRC = 200104004039.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)