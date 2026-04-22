# Build rule for OJ: produce executable named 'code'
CXX ?= g++
CXXFLAGS ?= -O2 -std=gnu++17 -static -s
LDFLAGS ?=
SRC := main.cpp

all: code

code: $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f code
