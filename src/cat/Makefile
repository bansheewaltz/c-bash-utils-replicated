.PHONY: all build_functional build_heavy build_light test leak leaks style \
		legacy_style clean rebuild re add_clang sign_code install uninstall
CC := gcc
CFLAGS := -Wall -Wextra -Werror
DBG := -g
STD := -std=c11
OPT := -O2
ASAN := -fsanitize=address# -fstack-protector-all -fsanitize=pointer-compare \
        -fsanitize=pointer-subtract# -fsanitize=leak
SRC := main.c parser.c processor.c
BIN := s21_cat
TESTS_DIR := ./testing
# aliases
CLEAR_SCREEN := clear #tput reset


all: build
build: build_functional
build_functional: add_clang $(BIN)
$(BIN):
	$(CC) -o $(BIN) $(OPT) $(SRC) 
build_check:
	$(CC) -o $(BIN) $(CFLAGS) $(STD) $(DBG) $(OPT) $(SRC)
test: build
	@$(CLEAR_SCREEN)
	@cp $(BIN) $(TESTS_DIR)
	@cd $(TESTS_DIR) && bash test.sh
	@rm -rf $(TESTS_DIR)/$(BIN)
leak: build
	clear
	echo; leaks -atExit -- ./$(BIN) $(TESTS_DIR)/data_samples/* 2>/dev/null | grep -aE 'nodes|total|TOTAL|LEAK'
	@#leaks -atExit -- ./$(BIN) $(TESTS_DIR)/data_samples/* | grep -aE 'nodes|total|TOTAL|LEAK'
leaks: build
	@$(CLEAR_SCREEN)
	@cp $(BIN) $(TESTS_DIR)
	@cd $(TESTS_DIR) && sh test_leak.sh
	@rm -rf $(TESTS_DIR)/$(BIN)
alpine: clean
	bash $(TESTS_DIR)/alpine.sh
valgrind: clean
	bash $(TESTS_DIR)/valgrind.sh
style: build add_clang
	@$(CLEAR_SCREEN)
	clang-format -n *.[ch]
	clang-format -i *.[ch]
	cppcheck --enable=all --suppress=missingIncludeSystem *.[ch]
check_headers:
	@clear
	@cp ../../materials/linters/*.{py,cfg} ../
	-echo; python3 ../cpplint.py --extensions=c *.[ch] 2>&1 1>/dev/null \
		| awk -F' {2}' '{printf "%-20s %s %s\n", $$1, $$2, $$3}' | sort -k3 | nl
	@rm -f ../*.{py,cfg}
clean:
	@rm -rf {$(BIN),*.{out,log,dSYM,ntesting},*tmp*}
	@rm -rf $(TESTS_DIR)/{$(BIN),*.{out,log,dSYM,ntesting},*tmp*,logs}
	@rm -rf ../.clang-format
rebuild: re
re:
	@$(MAKE) clean
	@$(MAKE) build
add_clang:
	-cp ../../materials/linters/.clang-format ../
sign_code: # macOS only
	/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" tmp.entitlements
	codesign -s - --entitlements tmp.entitlements -f $(BIN)
install: build
	install -m 0755 $(BIN) /usr/local/bin
uninstall:
	rm -f /usr/local/bin/$(BIN)
