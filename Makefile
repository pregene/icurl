CC=g++
CFLAGS=
LIBS=-lcurl
EXEC=icurl
SRC= 	src/main.cpp \
			src/project.cpp \
			src/curl_pool.cpp

OS := $(shell uname -s)

ifeq ($(OS), Darwin)
#CFLAGS := $(CFLAGS) -Wc++17-extensions
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
	mkdir -p temp
	mkdir -p data
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(EXEC)

clean:
	rm -f $(EXEC)
	rm -f data/*.txt
	rm -f temp/*.res
	rm -f temp/*.utf8
