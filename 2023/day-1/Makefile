appname = main

CXX = g++
CXXFLAGS = -Wall

SRCS = main.cpp

.PHONY: run
run: $(appname)
	./$(appname)


.PHONY: test
test: $(appname)
	./$(appname) < test_input.txt


$(appname): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(appname)
