.PHONY: all build_functional build_heavy build_light test leak leaks style \
		legacy_style clean rebuild re add_clang sign_code install uninstall

SRC := main.c parser.c processor.c error_outputs.c
PROGRAM := cat
PREFIX := s21_
EXE := ${PREFIX}${PROGRAM}
# compile options
CC := gcc
WFLAGS := -Wall -Wextra -Werror
DBG := -g
STD := -std=c11
OPT := -O2
ASAN := -fsanitize=address -fstack-protector-all -fsanitize=pointer-compare \
        -fsanitize=pointer-subtract -fsanitize=leak
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


all: build
build: add_clang build_functional 
build_functional: $(EXE)
$(EXE):
	$(CC) -o $(EXE) $(OS_MACRO) $(OPT) $(SRC)
build_debug: clean
	$(CC) -o $(EXE) $(DBG) $(OS_MACRO) $(SRC)
compile_check: clean
	$(CC) -o $(EXE) $(DBG) $(OS_MACRO) $(WFLAGS) $(STD) $(OPT) $(SRC)
test: build
	printf "\ec"
	@cd $(DIR_TESTS) && utility=$(PROGRAM) bash test.sh
leak: build # fast check
	clear
	@echo -e "\n"
	leaks -atExit -- ./$(EXE) $(DIR_TESTS)/test_files/* 2>/dev/null | grep -aE 'nodes|total|TOTAL|LEAK' | nl -b n
leaks: build
	clear
	@cp $(EXE) $(DIR_TESTS)
	@cd $(DIR_TESTS) && sh test_leak.sh
	@rm -rf $(DIR_TESTS)/$(EXE)
check_style: build add_clang
	clear
	clang-format -n *.[ch]
	clang-format -i *.[ch]
	@$(MAKE) check_headers_style
static_analysis:
	echo; cppcheck --enable=all --suppress=missingIncludeSystem *.[ch]
check_headers_style:
	bash $(DIR_SCRIPTS)/check_headers_style.sh
	@echo
clean:
	@rm -rf {$(EXE), logs,*.{out,log,dSYM,ntesting},*tmp*}
	@rm -rf $(DIR_TESTS)/{$(EXE),*.{out,log,dSYM,ntesting},*tmp*,logs}
	@rm -rf $(DIR_SRC)/.clang-format
rebuild: re
re:
	@$(MAKE) clean
	@$(MAKE) build


detect_os:
	@echo KERNEL=${KERNEL}
	@echo NAME=${NAME}
	@echo INTERNAL_NAME=${OS}
	@echo MACRO=${OS_MACRO}
add_clang:
	-cp $(DIR_PROJ_ROOT)/materials/linters/.clang-format $(DIR_SRC)
docker: clean
	bash $(DIR_SCRIPTS)/run_dockerfile_image.sh $(image) $(goal) $(DIR_DOCKERFILES)
docker_prune:
	yes | docker system prune -a
install: build
	sudo install -m 0755 $(EXE) /usr/local/bin
uninstall:
	sudo rm -f /usr/local/bin/$(EXE)
sign_code: # macOS only
	/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" tmp.entitlements
	codesign -s - --entitlements tmp.entitlements -f $(EXE)
