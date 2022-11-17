CC=gcc

CFLAGS= -Wall -Wextra -Werror
LIBRARIES= -lregex
COMMON_SOURCES= options.c file.c command.c
COMMON_PATH= ../common/
CAT_SOURCES= main.c $(addprefix $(COMMON_PATH), $(COMMON_SOURCES))
EXECUTABLE=s21_grep.exe

all: clean build run

rebuild: clean build run

clean:
	del /s /q ..\*.exe

build: $(EXECUTABLE)

run:
	./$(EXECUTABLE) -eivcln

$(EXECUTABLE):
	$(CC) $(LDFLAGS) $(CAT_SOURCES) $(LIBRARIES) -o $@

