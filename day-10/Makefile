appname = main

CXX = g++
CXXFLAGS = -Wall -g -std=c++20

SRCS = main.cpp


.PHONY: test
test: $(appname) | test_input.txt
	./$(appname) < test_input.txt

.PHONY: run
run: $(appname) | input.txt
	./$(appname) < input.txt

$(appname): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(appname) $(SRCS)

input.txt:
	touch input.txt

test_input.txt:
	touch test_input.txt

.PHONY: clean
clean:
	$(RM) $(appname) output.txt
