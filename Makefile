BIN=wget

CXXFLAGS=-std=c++11 -W -Wall -fdiagnostics-color
LDFLAGS=

LIBS=-lboost_system -lpthread -lboost_filesystem -lssl -lcrypto -lz -lboost_iostreams
COMP=g++
CXX=$(COMP)
LD=$(COMP)

SRCS=$(wildcard *.cpp)
#SRCS=Exceptions.cpp tools.cpp Log.cpp BasicClient.cpp SSLClient.cpp Client.cpp HttpClient.cpp HttpDownloaderQueue.cpp HTMLTagParser.cpp HTMLTag.cpp HttpDownloader.cpp main.cpp 
#SRCS=tools.cpp Exceptions.cpp Log.cpp Client.cpp HttpClient.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)


$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $+ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MD -o $@ $<

.PHONY: clean

clean:
	$(RM) -r *~ $(OBJS) $(DEPS) $(BIN) doc/html/* doc/latex/*

.PHONY: cleanall

cleanall : clean
	$(RM) $(BIN)

-include $(DEPS)

