CPP = g++
CFLAGS = -O3

SRCDIR = src
OBJDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CPP) -c $< -o $@

all: build

build: main.cpp $(OBJS)
	$(CPP) $(CFLAGS) main.cpp -o main $(OBJS)

clean:
	rm ./obj/*.o
	rm ./logs/*.log