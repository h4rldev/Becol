VERSION := 1.0.0
LIB_VERSION := 1.0.0
NAME := becol

TARGET = bin/$(NAME)
LIB_TARGET = bin/lib$(NAME).so
WIN_TARGET = bin/$(NAME).exe
WIN_LIB_TARGET = bin/lib$(NAME).dll
LIBS = -lm
CC ?= gcc
CFLAGS =-Wall -DCOMMIT="\"$(shell git describe --always --dirty)\"" -DVERSION="\"$(VERSION)\"" -DSOURCE="\"$(shell git remote get-url $(shell git remote))\"" -Werror -Wextra -pedantic
LIB_CFLAGS = -fPIC -DVERSION="\"$(LIB_VERSION)\""

OS := $(shell uname)

ifeq ($(OS),Darwin)
LIB_INCLUDE_FLAGS := -Lbin -l$(NAME)
else
LIB_INCLUDE_FLAGS := -L. -l:$(LIB_TARGET)
endif

.PHONY: default all clean

default: release
release: $(TARGET)
debug: CFLAGS += -g -fsanitize=address
debug: LIB_CFLAGS += -g -fsanitize=address -static-libasan
debug: $(TARGET)
all: default

windows: CC = x86_64-w64-mingw32-gcc
windows: TARGET = $(WIN_TARGET)
windows: LIB_TARGET = $(WIN_LIB_TARGET)
windows: $(WIN_TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
LIB_OBJECTS = $(patsubst %.c, %.o, $(shell find src/lib$(NAME) -name "*.c"))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(WIN_TARGET) $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS) $(LIB_TARGET)
	mkdir -p bin
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@ $(LIB_INCLUDE_FLAGS) $(CFLAGS)

$(LIB_TARGET): CFLAGS += $(LIB_CFLAGS)
$(LIB_TARGET): $(LIB_OBJECTS)
	mkdir -p bin
	$(CC) $(LIB_OBJECTS) -Wall -Werror -shared $(LIBS) -o $@ $(CFLAGS)

$(WIN_TARGET): $(OBJECTS) $(WIN_LIB_TARGET)
	mkdir -p bin
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@ $(LIB_INCLUDE_FLAGS)

$(WIN_LIB_TARGET): CFLAGS += $(LIB_CFLAGS)
$(WIN_LIB_TARGET): $(LIB_OBJECTS)
	mkdir -p bin
	$(CC) $(LIB_OBJECTS) -Wall -Werror -shared $(LIBS) -o $@

clean:
	-find . -name *.o -delete
	-rm -rf bin/*

run: $(TARGET)
	$(TARGET)

bear:
	bear -- make all

