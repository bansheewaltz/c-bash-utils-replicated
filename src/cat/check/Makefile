.PHONY: build test leak clean re
CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -g
SRC := main.c parser.c processing.c utils.c
BIN := s21_cat
TESTS_DIR := ./test_cat
# aliases
CLEAR_SCREEN := tput reset


all: build
build: $(BIN)
$(BIN):
	$(CC) $(CFLAGS) $(ASAN) $(SRC) -o $(BIN)
test: build
	@$(CLEAR_SCREEN)
	@cp $(BIN) $(TESTS_DIR)
	@cd $(TESTS_DIR) && sh test.sh
	@rm -rf $(TESTS_DIR)/$(BIN)
leak: build
	@$(CLEAR_SCREEN)
	@cp $(BIN) $(TESTS_DIR)
	@cd $(TESTS_DIR) && sh test_leak.sh
	@rm -rf $(TESTS_DIR)/$(BIN)
clean:
	@rm -rf $(BIN)
	@rm -rf *.{dSYM,out,ntesting,log}
	@rm -rf $(TESTS_DIR)/{$(BIN),*.{log,dSYM}}
re: clean all
