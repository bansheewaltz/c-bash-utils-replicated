CC=gcc

CFLAGS= -Wall -Wextra -Werror
ifeq ($(OS),Windows_NT)
	LIBRARIES= -lregex
endif
ifeq (uname -s, Darwin)
	LIBRARIES=
endif
COMMON_SOURCES= options.c file.c command.c
COMMON_PATH= ../common/
CAT_SOURCES= main.c $(addprefix $(COMMON_PATH), $(COMMON_SOURCES))
EXECUTABLE=s21_grep.o

all: clean build run

rebuild: clean build run

clean:
	rm -rf *.o

build: $(EXECUTABLE)

run:
	./$(EXECUTABLE) -e [a-z]

$(EXECUTABLE):
	$(CC) $(LDFLAGS) $(CAT_SOURCES) $(LIBRARIES) -o $@

