PROG_1   = main
PROG_2   = main_optimized

CC = gcc -std=c11 -g -O3 -mavx2 -march=native
OBJS_MAIN = main.o \
matrix.o \
lin_system.o \
interpol.o \
utils.o

OBJS_MAIN_OP = main_optimized.o \
matrix.o \
lin_system.o \
interpol.o \
utils.o

CFLAGS = -DLIKWID_PERFMON -I$(LIKWID_INCLUDE)
LFLAGS = -L$(LIKWID_LIB) -lm -llikwid

.PHONY: all debug clean limpa purge faxina

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

all: $(PROG_1) $(PROG_2)

debug:         CFLAGS += -DDEBUG
debug:         $(PROG_1) $(PROG_2)

$(PROG_1):  $(PROG_1).o

$(PROG_1): $(OBJS_MAIN) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(PROG_2):  $(PROG_2).o

$(PROG_2): $(OBJS_MAIN_OP) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean limpa:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge faxina:   clean
	@echo "Faxina ...."
	@rm -f  $(PROG_1) $(PROG_2) *.o core a.out
	@rm -f *.png marker.out *.log
