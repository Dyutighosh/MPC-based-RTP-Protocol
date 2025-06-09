# Makefile for compiling mpc_rtp

CXX = g++ -DNUM_CONSUMERS=1000000 -DNUM_SUPPLIERS=10
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = mpc_rtp
SRCS = main.cpp FloatingPoint.cpp MPCProtocol.cpp PricingProtocol.cpp Constants.cpp Utils.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
