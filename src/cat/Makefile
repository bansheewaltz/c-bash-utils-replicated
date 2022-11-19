.PHONY: all build build_check test leak style clean rebuild re add_clang
CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -O3
STD := -std=c11
ASAN := -fsanitize=address -fstack-protector-all -fsanitize=pointer-compare \
        -fsanitize=pointer-subtract# -fsanitize=leak
SRC := main.c parser.c processing.c
BIN := s21_cat
TESTS_DIR := ./test_cat
# aliases
CLEAR_SCREEN := tput reset


all: build
build: $(BIN)
$(BIN):
	$(CC) $(CFLAGS) $(STD) $(SRC) -o $(BIN)
build_check:
	$(CC) $(CFLAGS) $(ASAN) $(STD) $(SRC) -o $(BIN)
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
style: build
	@$(CLEAR_SCREEN)
	@$(MAKE) clang
	clang-format -n *.[ch]
	clang-format -i *.[ch]
	cppcheck --enable=all --suppress=missingIncludeSystem *.[ch]
clean:
	@rm -rf $(BIN)
	@rm -rf *.{dSYM,out,ntesting,log,entitlements}
	@rm -rf $(TESTS_DIR)/{$(BIN),*.{log,dSYM}}
	@rm -rf ../.clang-format
rebuild: re
re:
	@$(MAKE) clean
	@$(MAKE) build
add_clang:
	cp ../../materials/linters/.clang-format ../
