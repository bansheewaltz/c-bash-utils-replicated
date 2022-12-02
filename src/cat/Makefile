.PHONY: all build_functional build_heavy build_light test leak leaks style \
		legacy_style clean rebuild re add_clang sign_code install uninstall

SRC := main.c parser.c processor.c
BIN := s21_cat
# compile options
CC := gcc
CFLAGS := -Wall -Wextra -Werror
DBG := -g
STD := -std=c11
OPT := -O2
ASAN := -fsanitize=address# -fstack-protector-all -fsanitize=pointer-compare \
        -fsanitize=pointer-subtract# -fsanitize=leak
# paths
DIR_PROJ_ROOT := ../..
DIR_SRC := ..
DIR_TESTS := ./testing
DIR_DOCKERFILES := $(DIR_PROJ_ROOT)/materials/build/custom_dockerfiles
DIR_SCRIPTS := $(DIR_PROJ_ROOT)/materials/build/custom_scripts


all: build
build: build_functional add_clang
build_functional: $(BIN)
$(BIN):
	$(CC) -o $(BIN) $(OPT) $(SRC) 
build_debug:
	$(CC) -o $(BIN) $(DBG) $(SRC)
compile_check:
	$(CC) -o $(BIN) $(DBG) $(CFLAGS) $(STD) $(OPT) $(SRC)
test: build
	clear
	@cp $(BIN) $(DIR_TESTS)
	@cd $(DIR_TESTS) && bash test.sh
	@rm -rf $(DIR_TESTS)/$(BIN)
leak: build
	clear
	echo; leaks -atExit -- ./$(BIN) $(DIR_TESTS)/data_samples/* 2>/dev/null | grep -aE 'nodes|total|TOTAL|LEAK'
leaks: build
	clear
	@cp $(BIN) $(DIR_TESTS)
	@cd $(DIR_TESTS) && sh test_leak.sh
	@rm -rf $(DIR_TESTS)/$(BIN)
check_style: build add_clang
	clear
	clang-format -n *.[ch]
	clang-format -i *.[ch]
	@$(MAKE) check_headers
static_analyzer:
	echo; cppcheck --enable=all --suppress=missingIncludeSystem *.[ch]
check_headers:
	bash $(DIR_SCRIPTS)/check_headers_style.sh
	@echo
clean:
	@rm -rf {$(BIN),*.{out,log,dSYM,ntesting},*tmp*}
	@rm -rf $(DIR_TESTS)/{$(BIN),*.{out,log,dSYM,ntesting},*tmp*,logs}
	@rm -rf $(DIR_SRC)/.clang-format
rebuild: re
re:
	@$(MAKE) clean
	@$(MAKE) build


add_clang:
	-cp $(DIR_PROJ_ROOT)/materials/linters/.clang-format $(DIR_SRC)
docker: clean
	bash $(DIR_SCRIPTS)/run_dockerfile_image.sh $(image) $(DIR_DOCKERFILES)
docker_prune:
	yes | docker system prune -a
install: build
	install -m 0755 $(BIN) /usr/local/bin
uninstall:
	rm -f /usr/local/bin/$(BIN)
sign_code: # macOS only
	/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" tmp.entitlements
	codesign -s - --entitlements tmp.entitlements -f $(BIN)
