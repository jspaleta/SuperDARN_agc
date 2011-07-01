OS:=$(shell uname -s)

CC=gcc
CFLAGS=-c -Wall 
LFLAGS  = 
common_libs=
linux_libs=
qnx_libs=
INCLUDES=-I"include/"
SOURCES=main.c send_command.c agc_results.c basbox_results.c pm_results.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=agc_monitor

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
ifeq ($(OS),Linux)
	$(CC) -o $@ $(OBJECTS) $(LFLAGS) $(common_libs) $(linux_libs) 
endif
ifeq ($(OS),QNX)
	$(CC) -o $@ $(OBJECTS) $(LFLAGS) $(common_libs) $(qnx_libs) 
endif
#	cp $(EXECUTABLE) ../../../bin/

.c.o:
	$(CC) $(CFLAGS) $< -o $@ $(LFAGS) $(INCLUDES)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)  



