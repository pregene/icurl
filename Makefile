CC=g++
CFLAGS=
LIBS=-lcurl
EXEC=icurl
SRC= 	src/main.cpp \
			src/interpark.cpp \
			src/curlget.cpp \
			src/curlpost.cpp

OS := $(shell uname -s)

ifeq ($(OS), Darwin)
# CFLAGS := $(CFLAGS) -I/opt/homebrew/Cellar/openssl@1.1/1.1.1w/include
else
#CFLAGS := $(CFLAGS) `pkg-config openssl --cflags`
endif

LIBS=-lpthread -lcurl
ifeq ($(OS), Darwin)
#LIBS := $(LIBS) -lssl -lcrypto -L/opt/homebrew/Cellar/openssl@1.1/1.1.1w/lib
else
#LIBS := $(LIBS) `pkg-config openssl --libs`
endif

all:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(EXEC)

clean:
	rm -f $(EXEC)
