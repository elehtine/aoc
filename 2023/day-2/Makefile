appname = main

CXX = g++
CXXFLAGS = -Wall

SRCS = main.cpp


.PHONY: test
test: $(appname)
	./$(appname) < test_input.txt


.PHONY: run
run: $(appname)
	./$(appname)


$(appname): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(appname)

