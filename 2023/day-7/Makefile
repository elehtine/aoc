appname = main

CXX = g++
CXXFLAGS = -Wall -g -std=c++20

SRCS = main.cpp

.PHONY: test
test: $(appname)
	./$(appname) < test_input.txt

.PHONY: small
small: $(appname)
	./$(appname) < input.txt

$(appname): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(appname) $(SRCS)

.PHONY: clean
clean:
	$(RM) $(appname) user_output.txt

