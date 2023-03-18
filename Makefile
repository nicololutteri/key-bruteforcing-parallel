PROJECT := build/key_finder
SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)
LD := gcc
CFLAGS := -I include/ -c
CC := gcc
ex: EX=thread

all: $(PROJECT)

$(PROJECT): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(PROJECT) -lpthread

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) $(EX) $< -o $@

install:
	mkdir -p bin
	cp $(PROJECT) bin/

help:
	@echo all: Compila tutti i file
	@echo thread: Compila la versione con i thread
	@echo install: Installa l'applicazione
	@echo clean: Cancella tutto tranne i file sorgenti
	@echo doc: Genera la documentazione

threads: EX := -D THREAD=1
threads: all
	
doc:
	doxygen doxygen.cfg

clean:
	rm $(OBJ) $(PROJECT)
	
.PHONY: all clean install help doc threads
