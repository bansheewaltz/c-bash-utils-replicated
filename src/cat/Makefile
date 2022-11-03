CC=gcc

CFLAGS= -Wall -Wextra -Werror
COMMON_SOURCES= regex.c file.c
COMMON_PATH= ../common/
CAT_SOURCES= main.c $(addprefix $(COMMON_PATH), $(COMMON_SOURCES))

all: build

rebuild: clean build

build: s21_cat.o

s21_cat.o:
	$(CC) $(LDFLAGS) $(CAT_SOURCES) -o $@

clean:
	rm -rf *.o
