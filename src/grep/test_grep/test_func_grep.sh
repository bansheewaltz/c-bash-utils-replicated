#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

RED="\033[38;5;203m"
GRN="\033[38;5;106m"
REG="\033[0m"
tests_n=2166

rm {Makefile,main.c,typedefs.h,*.log,s21_grep} 2>/dev/null
cp ../{Makefile,main.c,typedefs.h,s21_grep} ./

if [[ "$mode" == "time" ]]; then
  echo "testing $version:"
fi

if [[ "$version" == "s21_grep" ]]; then
  version=./$version
fi

declare -a tests=(
  "s test_0_grep.txt VAR"
  "for main.c typedefs.h Makefile VAR"
  "for main.c VAR"
  "-e for -e ^int main.c typedefs.h Makefile VAR"
  "-e for -e ^int main.c VAR"
  "-e regex -e ^print main.c VAR -f test_ptrn_grep.txt"
  "-e while -e void main.c Makefile VAR -f test_ptrn_grep.txt"
)

declare -a extra=(
  "-n for test_1_grep.txt test_2_grep.txt"
  "-n for test_1_grep.txt"
  "-n -e ^\} test_1_grep.txt"
  "-c -e /\ test_1_grep.txt"
  "-ce ^int test_1_grep.txt test_2_grep.txt"
  "-e ^int test_1_grep.txt"
  "-nivh = test_1_grep.txt test_2_grep.txt"
  "-e"
  "-ie INT test_5_grep.txt"
  "-echar test_1_grep.txt test_2_grep.txt"
  "-ne = -e out test_5_grep.txt"
  "-iv int test_5_grep.txt"
  "-in int test_5_grep.txt"
  "-c -l aboba test_1_grep.txt test_5_grep.txt"
  "-v test_1_grep.txt -e ank"
  "-noe ) test_5_grep.txt"
  "-l for test_1_grep.txt test_2_grep.txt"
  "-o -e int test_4_grep.txt"
  "-e = -e out test_5_grep.txt"
  "-noe ing -e as -e the -e not -e is test_6_grep.txt"
  "-e ing -e as -e the -e not -e is test_6_grep.txt"
  "-c -e . test_1_grep.txt -e '.'"
  "-l for no_file.txt test_2_grep.txt"
  "-f test_3_grep.txt test_5_grep.txt"
)

function progress_bar {
  let _progress=(${1} * 100 / ${2} * 100)/100
  let _done=(${_progress} * 4)/10
  let _left=40-$_done

  _fill=$(printf "%${_done}s")
  _empty=$(printf "%${_left}s")

  printf "\rtest_number: %4d/$tests_n [${_fill// /#}${_empty// /-}] ${_progress}%%" $COUNTER
}

testing() {
  t=$(echo $@ | sed "s/VAR/$var/")
  if [[ "$mode" == "time" ]]; then
    $version $t &>/dev/null
    ((COUNTER++))
    progress_bar $COUNTER $tests_n
  else
    ./s21_grep $t >test_s21_grep.log
    grep $t >test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    ((COUNTER++))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]; then
      ((SUCCESS++))
      printf "${RED}$FAIL${REG}/${GRN}$SUCCESS${REG}/$COUNTER \033[32msuccess${REG} grep $t\n"
    else
      ((FAIL++))
      printf "${RED}$FAIL${REG}/${GRN}$SUCCESS${REG}/$COUNTER \033[31mfail${REG} grep $t\n"
    fi
  fi
}

# специфические тесты
for i in "${extra[@]}"; do
  var="-"
  testing $i
done

# 1 параметр
for var1 in v c l n h o; do
  for i in "${tests[@]}"; do
    var="-$var1"
    testing $i
  done
done

# 2 параметра
for var1 in v c l n h o; do
  for var2 in v c l n h o; do
    if [ $var1 != $var2 ]; then
      for i in "${tests[@]}"; do
        var="-$var1 -$var2"
        testing $i
      done
    fi
  done
done

# 3 параметра
for var1 in v c l n h o; do
  for var2 in v c l n h o; do
    for var3 in v c l n h o; do
      if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]; then
        for i in "${tests[@]}"; do
          var="-$var1 -$var2 -$var3"
          testing $i
        done
      fi
    done
  done
done

# 2 сдвоенных параметра
for var1 in v c l n h o; do
  for var2 in v c l n h o; do
    if [ $var1 != $var2 ]; then
      for i in "${tests[@]}"; do
        var="-$var1$var2"
        testing $i
      done
    fi
  done
done

# 3 строенных параметра
for var1 in v c l n h o; do
  for var2 in v c l n h o; do
    for var3 in v c l n h o; do
      if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]; then
        for i in "${tests[@]}"; do
          var="-$var1$var2$var3"
          testing $i
        done
      fi
    done
  done
done

if [[ "$mode" != "time" ]]; then
  printf "\033[31mFAIL: $FAIL${REG}\n"
  printf "\033[32mSUCCESS: $SUCCESS${REG}\n"
  echo "ALL: $COUNTER"
fi

rm {Makefile,main.c,typedefs.h,*.log,s21_grep} 2>/dev/null || true
