BIN=wget

CXXFLAGS=-std=c++11 -O0 -g -W -Wall -fdiagnostics-color
LDFLAGS=

LIBS=-lboost_system -lpthread -lboost_filesystem -lssl -lcrypto -lz -lboost_iostreams
COMP=g++
CXX=$(COMP) $(CXXFLAGS)
LD=$(COMP)

SRCS=$(wildcard *.cpp)
#SRCS=Exceptions.cpp tools.cpp Log.cpp BasicClient.cpp SSLClient.cpp Client.cpp HttpClient.cpp HttpDownloaderQueue.cpp HTMLTagParser.cpp HTMLTag.cpp HttpDownloader.cpp main.cpp 
#SRCS=tools.cpp Exceptions.cpp Log.cpp Client.cpp HttpClient.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)


$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $+ $(LIBS)

%.o: %.cpp
	$(CXX) -o $@ -c $<

%.d: %.cpp
	$(CXX) -MM -MD -o $@ $<

.PHONY: clean

clean:
	$(RM) *~ $(OBJS) $(DEPS) $(BIN) *.html

.PHONY: cleanall

cleanall : clean
	$(RM) $(BIN)

-include $(DEPS)

