.PHONY: build build_functional debug build_debug check build_compile_check \
		test test1 leak leaks style static linter verter clean rebuild re  \
		os_info add_clang docker docker_prune install uninstall sign_code  \
		gitignore

SRC := s21_grep.c
PROGRAM := grep
PREFIX := s21_
EXE := ${PREFIX}${PROGRAM}
# compile options
CC := gcc
WFLAGS := -Wall -Wextra -Werror
DEBUG_INFO := -g
D_DEBUG := -D DEBUG
# D_GNU_SRC := -D _GNU_SOURCE
STD := -std=c11
OPT := -O2
DFLAGS := -fsanitize=address -fno-omit-frame-pointer -fstack-protector-all \
		  -fsanitize=pointer-compare -fsanitize=pointer-subtract \
		  -fsanitize=leak -fsanitize=undefined -fsanitize=bounds-strict
# paths
DIR_PROJ_ROOT := ../..
DIR_SRC := ..
DIR_TESTS := ./testing
DIR_DOCKERFILES := $(DIR_PROJ_ROOT)/materials/build/custom_dockerfiles
DIR_SCRIPTS := $(DIR_PROJ_ROOT)/materials/build/custom_scripts
# environment-related
SHELL := /bin/bash
KERNEL := $(shell uname -s)
$(eval $(shell grep '^NAME' /etc/os-release 2>/dev/null)) # defines NAME(linux distro) shell varable
ifeq ($(KERNEL),Darwin)
	OS := MACOS
else
	ifeq ($(NAME),"Ubuntu")
		OS := UBUNTU
	else
		OS := ALPINE
	endif
endif
OS_MACRO := -D __${OS}__
ifndef VERBOSE
MAKEFLAGS += --no-print-directory
endif


all: build
build:
	$(MAKE) clean
	$(MAKE) add_clang
	$(MAKE) build_functional
build_functional: $(EXE)
$(EXE):
	$(CC) -o $(EXE) $(OS_MACRO) $(OPT) $(SRC)
debug:
	$(MAKE) clean
	$(MAKE) add_clang
	$(MAKE) build_debug
build_debug:
	$(CC) -o $(EXE) $(DEBUG_INFO) $(D_DEBUG) $(OS_MACRO) $(SRC)
check:
	$(MAKE) clean
	$(MAKE) add_clang
	$(MAKE) build_compile_check
build_compile_check: # separate task because of sanitizers don't work on ARM
	$(CC) -o $(EXE) $(DEBUG_INFO) $(OS_MACRO) $(WFLAGS) $(STD) $(SRC)
	@echo
asan:
	$(CC) -o $(EXE) $(DEBUG_INFO) $(OS_MACRO) $(WFLAGS) $(STD) $(DFLAGS) $(SRC)
	cp Makefile s21* ./test_grep
	@cd test_grep && bash test_func_grep.sh
	rm ./test_grep/s21*
test: build
	printf "\ec"
	@cd $(DIR_TESTS) && utility=$(PROGRAM) bash test.sh
test1: build
	printf "\ec"
	cp Makefile s21* ./test_grep
	@cd test_grep && time bash test_func_grep.sh
	rm ./test_grep/s21*
complex:
	printf "\ec"
	$(MAKE) style
	$(MAKE) static
	$(MAKE) check
	$(MAKE) asan
	$(MAKE) verter
valgrind: check
	{ \
	if [ -z "$$args" ]; then \
    	args="-e ^print s21_grep.c -v -l -f test_ptrn_grep.txt"; \
	fi; \
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXE) $$args; \
	}
leak: build # fast check
	leaks -atExit -- ./$(EXE) $(DIR_TESTS)/test_files/* 2>/dev/null | grep -aE 'nodes|total|TOTAL|LEAK' | nl -b n
leaks: build
	@cp $(EXE) $(DIR_TESTS)
	@cd $(DIR_TESTS) && sh test_leak.sh
	@rm -rf $(DIR_TESTS)/$(EXE)
style: add_clang
	clang-format -n *.[ch]
	clang-format -i *.[ch]
	@echo
	$(MAKE) linter
	@echo
static:
	cppcheck --enable=all --suppress=missingIncludeSystem *.[ch]
	@echo
linter:
	bash $(DIR_SCRIPTS)/run_linter.sh
verter:
	$(MAKE) clean
	$(MAKE) -C ../cat/ clean
	cd $(DIR_PROJ_ROOT)/materials/build/ && bash run.sh
	@echo
clean:
	@rm -rf {$(EXE),logs,*.{out,log,dSYM,ntesting},*tmp*}
	@rm -rf $(DIR_TESTS)/{$(EXE),*.{out,log,dSYM,ntesting},*tmp*,logs}
	@rm -rf $(DIR_SRC)/.clang-format
rebuild: re
re:
	$(MAKE) clean
	$(MAKE) build


os_info:
	@echo KERNEL=${KERNEL}
	@echo NAME=${NAME}
	@echo INTERNAL_NAME=${OS}
	@echo MACRO=${OS_MACRO}
gitignore:
	@echo -e '\
	*\n\
	!*.[ch]\n\
	!Makefiie' > .gitignore
	@echo -e ".gitignore:\n"
	@cat .gitignore
add_clang:
	-cp $(DIR_PROJ_ROOT)/materials/linters/.clang-format $(DIR_SRC)
docker: clean
	utility=$(PROGRAM) bash $(DIR_SCRIPTS)/run_dockerfile_image.sh $(image) $(goal)
docker_prune:
	yes | docker system prune -a
install: build
	sudo install -m 0755 $(EXE) /usr/local/bin
uninstall:
	sudo rm -f /usr/local/bin/$(EXE)
sign_code: # macOS only
	/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" tmp.entitlements
	codesign -s - --entitlements tmp.entitlements -f $(EXE)
