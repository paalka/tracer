CC=cc
CFLAGS=-Wall -g -std=c99
LIBS=

SOURCES=debugger.c breakpoint.c process.c prompt.c
SOURCE_FOLDER=src

OBJECTS_FOLDER=obj
OBJECTS=$(patsubst %, $(OBJECTS_FOLDER)/%, $(SOURCES:.c=.o))

OUT_FOLDER=bin

TARGET=dbug
RM=rm -fr

all: $(OBJECTS) $(TARGET)

$(OBJECTS_FOLDER)/%.o: $(SOURCE_FOLDER)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $(OUT_FOLDER)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(OBJECTS_FOLDER)/* $(OUT_FOLDER)/*
