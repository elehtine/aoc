appname = main

CXX = g++
CXXFLAGS = -Wall -g -std=c++20

SRCS = main.cpp readwrite.cpp

.PHONY: small
small: $(appname)
	./$(appname) < input.txt

.PHONY: test
test: $(appname)
	./$(appname) < test_input.txt

$(appname): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(appname) $(SRCS)

.PHONY: clean
clean:
	$(RM) $(appname) output.txt

