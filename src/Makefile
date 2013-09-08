OS:=$(shell uname -s)

CC=gcc
CFLAGS=-c -Wall 
LFLAGS  =-L"../iniparser3.0b/" 
common_libs=
linux_libs=-liniparser
qnx_libs=
INCLUDES=-I"../iniparser3.0b/src" -I"include/"
SOURCES=main.c send_command.c agc_display.c agc_save_ini.c agc_lockout_test.c\
tx_port_open.c agc_results.c basbox_results.c basbox_display.c pm_results.c pm_display.c
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



