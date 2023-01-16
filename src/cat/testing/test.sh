#!/bin/bash
### script for automated testing of cli utilities ###

# option set presets
cat_stripped="-b -e -n -t -v" # for Linux Alpine and lets assume that for other no-names too
cat_macOS_regular="-b -e -n -s -t -v"
cat_macOS_inscript_compatible="-b -e -n -s -t -v --number --squeeze-blank --number-nonblank"
cat_GNU="-b -e -n -s -t -v -E -T --number --squeeze-blank --number-nonblank"

##### USER DEFINED GLOBAL TESTING PARAMETERS
# utility="cat" # is passed by shell variable
prefix="s21_"
target_utility="../${prefix}${utility}"
options_set=$cat_stripped
test_file="test_case_cat.txt" # multiple files could be set
dynamic_analyzer="valgrind"   # will be overwritten based on OS type
### optional macOS only parameters
opt_substitute_nonlegit_options="false"  # substitute some of the non-compatible macOS options
opt_cleanup_nonlegit_options_log="false" # cleanup the log file with failed cases by deleting the non-legit macOS options
### paths
testing_dir="./test_files"
logs_dir="./logs"
failed_log="failed_cases.log"
succeeded_log="succeeded_cases.log"
### messages
summary_appeal="DUDE..."
summary_success_msg="${summary_appeal} ЦЕ ДУЖЕ ДОБРЕ"
summary_failure_msg="${summary_appeal} КРАЩЕ Б ТОБІ ЦЕ ПОЛАГОДИТИ"
summary_delim=" | "
summary_log_msg="you can check the failed cases in the \"$failed_log\" file"
#####

# variables
c_success=0
c_failure=0
c_processed=0
log="test_$utility.tmp"
target_log="test_$prefix$utility.tmp"
# colours
RED="\033[38;5;203m"
GRN="\033[38;5;106m"
ORN="\033[38;5;179m"
REG="\033[0m"

apply_automatic_formatting_calculations() {
  options_array=($options_set)
  number_of_options="${#options_array[@]}"
  number_of_combinations=$((1 << $number_of_options))
  fw="${#number_of_combinations}" # field width for number columns
  options_len="${#options_set}"
  testname_len="${#test_file}"
  summray_log_msg_len="${#summary_log_msg}"
  delim_len="${#summary_delim}"
  result_record_len=$((fw * 3 + 2 + 1 + 7 + 1 + options_len + 1 + testname_len))
  total_len=$result_record_len
  record_shift=0
  if [ $summray_log_msg_len -ge $total_len ]; then
    total_len=$((summray_log_msg_len + 6))
    record_shift=$(((total_len - result_record_len) / 2))
  fi
}

apply_automatic_OS_based_settings() {
  if [[ "$OSTYPE" == "darwin"* ]]; then
    opt_substitute_nonlegit_options="true"
    options_set=$cat_macOS_inscript_compatible
    opt_cleanup_nonlegit_options_log="true"
    dynamic_analyzer="leaks"
  elif [ -f /etc/os-release ]; then
    . /etc/os-release
    if [[ "$NAME" == "Ubuntu" ]]; then
      options_set=$cat_GNU
    fi
  fi
}

clean_nonlegit_log() {
  sed -i '' '/-E/d' $logs_dir/$failed_log &>/dev/null
  sed -i '' '/-T/d' $logs_dir/$failed_log &>/dev/null
}

combine_and_pass() {
  local limit=$((1 << $#))
  local -a args=("$@")
  for ((value = 0; value < limit; value++)); do
    local -a parts=()
    for ((i = 0; i < $#; i++)); do
      (((1 << i) & value)) && parts+=("${args[i]}")
    done
    automated_testing "${parts[@]}"
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
  printf "%*s${RED}%0${fw}d${REG}/${GRN}%0${fw}d${REG}/%0${fw}d ${color}%7s${REG} %-${options_len}s %s\n" \
    $record_shift "" $c_failure $c_success $c_processed $1 "$test_options" $2
}

valgrind_testing() {
  local test_options="$@"
  local test_path=$testing_dir/$test_file
  local test_entry="$test_options $test_path"
  valgrind_args="valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose"
  $valgrind_args ../$target_utility $test_entry

}

automated_testing() {
  local test_options="$@"
  local test_path=${testing_dir}/${test_file}
  local test_entry="$test_options $test_path"

  $target_utility $test_entry >$logs_dir/$target_log
  if_opt_then_substitue_nonlegit_options
  $utility $sys_test_entry >$logs_dir/$log

  local diff_res="$(diff -s $logs_dir/$target_log $logs_dir/$log)"
  ((c_processed++))
  if [ "$diff_res" == "Files $logs_dir/$target_log and $logs_dir/$log are identical" ]; then
    ((c_success++))
    print_record "SUCCESS" $test_file
    printf "%s %s\n" "$test_options" "$test_file" >>$logs_dir/$succeeded_log
  else
    ((c_failure++))
    print_record "FAILURE" $test_file
    printf "%s %s\n" "$test_options" "$test_file" >>$logs_dir/$failed_log
  fi
}

print_summary() {
  printf "%${total_len}s\n" | tr " " "-"
  printf "${ORN}RESULTS FOR THE SET OF OPTIONS: $options_set${REG}\n"
  printf "%${total_len}s\n" | tr " " "-"
  if [ "$c_failure" -gt 0 ]; then
    local msg_len=${#summary_failure_msg}
    local output_len=$((msg_len + fw * 2 + 3 + 1))
    printf "${RED}%*d${REG}/%*d | %s\n" \
      $(((total_len - output_len) / 2 + fw)) $c_failure $fw $c_processed "$summary_failure_msg"
    printf "${ORN}%*s${REG}\n\n" \
      $(((total_len - summray_log_msg_len) / 2 + summray_log_msg_len)) "$summary_log_msg"

  else
    local msg_len="${#summary_success_msg}"
    local output_len=$((msg_len + fw * 2 + 3 + 1))
    printf "${GRN}%*d${REG}/%*d | %s\n\n" \
      $(((total_len - output_len) / 2 + fw)) $c_success $((fw)) $c_processed "$summary_success_msg"
  fi
}

if_opt_then_cleanup_nonlegit() {
  if [ "$opt_cleanup_nonlegit_options_log" == "true" ]; then
    clean_nonlegit_log
  fi
}

clear_screen() {
  printf "\ec"
}

prepare_log_dir() {
  rm $logs_dir/{$failed_log,$succeeded_log} &>/dev/null
  mkdir $logs_dir &>/dev/null
}

clear_temp_files() {
  if_opt_then_cleanup_nonlegit
  rm $logs_dir/{$target_log,$log} &>/dev/null
}

if_opt_then_substitue_nonlegit_options() {
  if [ "$opt_substitute_nonlegit_options" == "true" ]; then
    substitute_nonvalid_options
  else
    sys_test_entry=$test_entry
  fi
}

handwritten_entries_test() {
  local test_file=$testing_dir/handwritten_entries.txt
  local c_processed=1
  local c_success=0
  local c_failure=0

  printf "%${total_len}s\n" | tr " " "-"
  printf "${ORN}TESTING WITH THE HADWRITTEN ENTRIES:${REG}\n"
  printf "%${total_len}s\n" | tr " " "-"
  while read test_entry; do
    $target_utility $test_entry >$logs_dir/$target_log
    if_opt_then_substitue_nonlegit_options
    $utility $sys_test_entry >$logs_dir/$log

    local diff_res="$(diff -s $logs_dir/$target_log $logs_dir/$log)"
    test_options=$test_entry
    ((c_processed++))
    if [ "$diff_res" == "Files $logs_dir/$target_log and $logs_dir/$log are identical" ]; then
      ((c_success++))
      print_record "SUCCESS"
    else
      ((c_failure++))
      print_record "FAILURE"
      printf "%s %s\n" "$test_options" "$test_file" >>$logs_dir/$failed_log
    fi
  done <$test_file
}

### THE MAIN SCRIPT
clear_screen
apply_automatic_OS_based_settings
# options_set=$cat_stripped
apply_automatic_formatting_calculations
prepare_log_dir

combine_and_pass $options_set
print_summary
# exec 2>/dev/null # *Turn on/off by (un)commenting to suppress stderror output*
handwritten_entries_test

clear_temp_files
###
