#!/bin/bash
### script for functional_testing all combinations of test_options ###

# option set presets
cat_macOS="-b -e -n -s -t -v"
cat_GNU="-b -e -n -s -t -v -E -T --number --squeeze-blank --number-nonblank"

##### USER DEFINED TESTING PARAMETERS
utility="cat"
options_set=$cat_GNU
test_file="test_case_cat.txt" # multiple files could be set
# exec 2>/dev/null               # *Turn on/off by (un)commenting to suppress stderror output during the functional_testing*
opt_substitute_options="false" # substitute some of non-compatible macOS options
opt_cleanup_nonlegit="false"   # cleanup log file with failed cases by deleting non-legit macOS options
opt_valgrind="true"
### paths
testing_dir="./data_samples"
logs_dir="./logs"
failed_log="failed_cases.log"
succeeded_log="succeeded_cases.log"
#####

# automatic settings applying
if [[ "$OSTYPE" == "darwin"* ]]; then
  opt_substitute_options="true"
  opt_cleanup_nonlegit="true"
  opt_valgrind="false"
fi
# variables
success_c=0
fail_c=0
total_c=0
s21_utility="s21_$utility"
log="test_$utility.tmp"
s21_log="test_$s21_utility.tmp"
# colours
# RED=$(tput setaf 1)
# GRN=$(tput setaf 2)
# REG=$(tput sgr0)
### portable colours
RED="\033[38;5;203m"
GRN="\033[38;5;106m"
ORN="\033[38;5;179m"
REG="\033[0m"
# automatic formatting parameters
options_array=($options_set)
number_of_options="${#options_array[@]}"
number_of_combinations=$((1 << $number_of_options))
fw="${#number_of_combinations}" # field width for number columns
options_len="${#options_set}"
testname_len="${#test_file}"
total_len=$((fw * 3 + 2 + 1 + 7 + 1 + options_len + 1 + testname_len))

clean_nonlegit_log() {
  sed -i '' '/-E/d' $logs_dir/$failed_log
  sed -i '' '/-T/d' $logs_dir/$failed_log
}

combine_and_pass() {
  local limit=$((1 << $#))
  local -a args=("$@")
  for ((value = 0; value < limit; value++)); do
    local -a parts=()
    for ((i = 0; i < $#; i++)); do
      (((1 << i) & value)) && parts+=("${args[i]}")
    done
    # if [ "$opt_valgrind" == "true" ]; then
    #   valgrind_testing "${parts[@]}"
    # fi
    functional_testing "${parts[@]}"
  done
}

substitute_nonvalid_options() {
  # string manipulation to get around non-legit options for macOS system cat
  sys_test_entry=${test_entry//--squeeze-blank/-s}
  sys_test_entry=${sys_test_entry//--number-nonblank/-b}
  sys_test_entry=${sys_test_entry//--number/-n}
  # won't resolve the problem but will affect some cases
  sys_test_entry=${sys_test_entry//-E/-e}
  sys_test_entry=${sys_test_entry//-T/-t}
}

print_record() {
  local color
  if [ "$1" == "FAILURE" ]; then
    color=$RED
  else
    color=$GRN
  fi
  printf "${RED}%0${fw}d${REG}/${GRN}%0${fw}d${REG}/%0${fw}d ${color}%7s${REG} %-${options_len}s %s\n" \
    $fail_c $success_c $total_c $1 "$test_options" "$test_file"
}

valgrind_testing() {
  local test_options="$@"
  local test_path=${testing_dir}/${test_file}
  local test_entry="$test_options $test_path"
  valgrind_args="valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose"
  $valgrind_args ./$s21_utility $test_entry

}

functional_testing() {
  local test_options="$@"
  local test_path=${testing_dir}/${test_file}
  local test_entry="$test_options $test_path"

  ./$s21_utility $test_entry >$logs_dir/$s21_log
  if [ "$opt_substitute_options" == "true" ]; then
    substitute_nonvalid_options
  else
    sys_test_entry=$test_entry
  fi
  $utility $sys_test_entry >$logs_dir/$log

  local diff_res="$(diff -s $logs_dir/$s21_log $logs_dir/$log)"
  ((total_c++))
  if [ "$diff_res" == "Files $logs_dir/$s21_log and $logs_dir/$log are identical" ]; then
    ((success_c++))
    print_record "SUCCESS"
    printf "%s %s\n" "$test_options" "$test_file" >>$logs_dir/$succeeded_log
  else
    ((fail_c++))
    print_record "FAILURE"
    printf "%s %s\n" "$test_options" "$test_file" >>$logs_dir/$failed_log
  fi
}

print_result() {
  local appeal="DUDE..."
  local success_msg="${appeal} ЦЕ ДУЖЕ ДОБРЕ"
  local failure_msg="${appeal} КРАЩЕ Б ТОБІ ЦЕ ПОЛАГОДИТИ"
  local delim=" | "
  local delim_len="${#delim}"

  printf "%${total_len}s\n" | tr " " "-"
  if [ "$fail_c" -gt 0 ]; then
    local msg_len="${#failure_msg}"
    local output_len=$((msg_len + fw * 2 + 3 + 1))
    printf "${RED}%*d${REG}/%*d | %s\n" \
      $(((total_len - output_len) / 2 + fw)) $fail_c $((fw)) $total_c "$failure_msg"
    local log_msg="you can check the failed cases in the \"$failed_log\" file"
    local log_msg_len="${#log_msg}"
    printf "${ORN}%*s${REG}\n" \
      $(((total_len - log_msg_len) / 2 + log_msg_len)) "$log_msg"
    echo

  else
    local msg_len="${#success_msg}"
    local output_len=$((msg_len + fw * 2 + 3 + 1))
    printf "${GRN}%*d${REG}/%*d | %s\n" \
      $(((total_len - output_len) / 2 + fw)) $success_c $((fw)) $total_c "$success_msg"
    echo
  fi
}

clear_screen() {
  printf "\ec"
}

### main sript
clear_screen
rm $logs_dir/{$failed_log,$succeeded_log}
mkdir $logs_dir
combine_and_pass $options_set
print_result
if [ "$opt_cleanup_nonlegit" == "true" ]; then
  clean_nonlegit_log
fi
rm $logs_dir/{$s21_log,$log}
###
