C = g++
CFLAGS = -std=c++20 -Wall -Wextra -I. -fPIE
LDFLAGS = -pie

TARGET = game

SRCS = src/main.cpp src/tui.cpp src/card.cpp src/game.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(C) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(C) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
