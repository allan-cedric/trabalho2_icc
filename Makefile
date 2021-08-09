PROG   = main

CC = gcc -std=c11 -g -O3 -mavx2 -march=native
OBJS = main.o \
matrix.o \
lin_system.o \
interpol.o \
utils.o

CFLAGS = -DLIKWID_PERFMON -I$(LIKWID_INCLUDE)
LFLAGS = -L$(LIKWID_LIB) -lm -llikwid

.PHONY: all debug clean limpa purge faxina

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

all: $(PROG)

debug:         CFLAGS += -DDEBUG
debug:         $(PROG)

$(PROG):  $(PROG).o

$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean limpa:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge faxina:   clean
	@echo "Faxina ...."
	@rm -f  $(PROG) *.o core a.out
	@rm -f *.png marker.out *.log
