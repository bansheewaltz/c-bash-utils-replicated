.PHONY: all build build_heavy build_light test leak leaks style \
		legacy_style clean rebuild re add_clang sign_code
CC := gcc
CFLAGS := -Wall -Wextra -Werror -g #-O2
STD := -std=c11
ASAN := -fsanitize=address #-fstack-protector-all -fsanitize=pointer-compare \
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
build_heavy:
	$(CC) $(CFLAGS) $(ASAN) $(STD) $(SRC) -o $(BIN)
build_light:
	$(CC) $(SRC) -o $(BIN)
test: build
	@$(CLEAR_SCREEN)
	@cp $(BIN) $(TESTS_DIR)
	@cd $(TESTS_DIR) && sh test.sh
	@rm -rf $(TESTS_DIR)/$(BIN)
leak: build
	clear
	leaks -atExit -- ./$(BIN) $(TESTS_DIR)/data_samples/* 2>/dev/null | grep -aE 'nodes|total|TOTAL|LEAK'
	@#leaks -atExit -- ./$(BIN) $(TESTS_DIR)/data_samples/* | grep -aE 'nodes|total|TOTAL|LEAK'
leaks: build
	@$(CLEAR_SCREEN)
	@cp $(BIN) $(TESTS_DIR)
	@cd $(TESTS_DIR) && sh test_leak.sh
	@rm -rf $(TESTS_DIR)/$(BIN)
style: build add_clang
	@$(CLEAR_SCREEN)
	clang-format -n *.[ch]
	clang-format -i *.[ch]
	cppcheck --enable=all --suppress=missingIncludeSystem *.[ch]
style_legacy:
	@clear
	@cp ../../materials/linters/*.{py,cfg} ../
	-echo; python3 ../cpplint.py --extensions=c *.[ch] 2>&1 1>/dev/null \
		| awk -F' {2}' '{printf "%-20s %s %s\n", $$1, $$2, $$3}' | sort -k3 | nl
	@rm -f ../*.{py,cfg}
clean:
	@rm -rf {$(BIN),*.{out,log,dSYM,ntesting},*tmp*}
	@rm -rf $(TESTS_DIR)/{$(BIN),*.{out,log,dSYM,ntesting},*tmp*}
	@rm -rf ../.clang-format
rebuild: re
re:
	@$(MAKE) clean
	@$(MAKE) build
add_clang:
	cp ../../materials/linters/.clang-format ../
sign_code: # macOS only
	/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" tmp.entitlements
	codesign -s - --entitlements tmp.entitlements -f $(BIN)
