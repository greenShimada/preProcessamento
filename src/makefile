
TARGET = exercicioWill

CXX = g++
CXXFLAGS = -Wall `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET) &

