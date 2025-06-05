CC = g++
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lssh

SRCS = src/main.cpp src/ssh_connection.cpp src/hardware_monitor.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = remote_monitor

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
