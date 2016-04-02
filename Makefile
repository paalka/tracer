CC=cc
CFLAGS=-Wall -g -std=c99
LIBS=
SOURCES=dbug.c debugger.c breakpoint.c process.c
OBJECTS_FOLDER=obj
OBJECTS=$(patsubst %, $(OBJECTS_FOLDER)/%, $(SOURCES:.c=.o))
OUT_FOLDER=bin
TARGET=dbug
RM=rm -fr

all: $(OBJECTS) $(TARGET)

$(OBJECTS_FOLDER)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $(OUT_FOLDER)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(OBJECTS_FOLDER)/* $(OUT_FOLDER)/*
