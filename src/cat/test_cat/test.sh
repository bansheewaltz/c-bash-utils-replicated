#!/bin/bash
### script for testing all combinations of test_options ###

# option set presets
cat_macOS="-b -e -n -s -t -v"
cat_GNU="-b -e -n -s -t -v -E -T --number --squeeze-blank --number-nonblank"

##### USER DEFINED PARAMETERS
utility="cat"
options_set=$cat_GNU
exec 2>/dev/null # *Turn on to suppress stderror output during the testing*
### data sample settings
test_dir="./data_samples/"
test_file="test_case_cat.txt"
#####

# variables
success_c=0
fail_c=0
total_c=0
s21_utility="s21_$utility"
log="test_$utility.log"
s21_log="test_$s21_utility.log"
# colours
RED=$(tput setaf 1)
GRN=$(tput setaf 2)
REG=$(tput sgr0)
# automatic formatting parameters
options_array=($options_set)
number_of_options="${#options_array[@]}"
number_of_combinations=$((1 << $number_of_options))
fw="${#number_of_combinations}" # field width for number columns
options_len="${#options_set}"
testname_len="${#test_file}"
total_len=$((fw * 3 + 2 + 1 + 7 + 1 + options_len + 1 + testname_len))

combine_and_pass() {
    local limit=$((1 << $#))
    local -a args=("$@")
    for ((value = 0; value < limit; value++)); do
        local -a parts=()
        for ((i = 0; i < $#; i++)); do
            (((1 << i) & value)) && parts+=("${args[i]}")
        done
        testing "${parts[@]}"
    done
}

testing() {
    local test_options=$@
    local test_path=${test_dir}${test_file}
    local test_entry="$test_options $test_path"

    ./$s21_utility $test_entry >$s21_log
    $utility $test_entry >$log

    local diff_red="$(diff -s $s21_log $log)"
    ((total_c++))
    if [ "$diff_red" == "Files $s21_log and $log are identical" ]; then
        ((success_c++))
        printf "${RED}%0${fw}d${REG}/${GRN}%0${fw}d${REG}/%0${fw}d ${GRN}%7s${REG} %-${options_len}s %s\n" \
            $fail_c $success_c $total_c "SUCCESS" "$test_options" "$test_file"
    else
        ((fail_c++))
        printf "${RED}%0${fw}d${REG}/${GRN}%0${fw}d${REG}/%0${fw}d ${RED}%-7s${REG} %-${options_len}s %s\n" \
            $fail_c $success_c $total_c "FAILURE" "$test_options" "$test_file"
    fi
}

print_result() {
    printf "%${total_len}s\n" | tr " " "-"
    if [ $fail_c ]; then
        printf "${RED}%*d/${REG}%*d\n" $((2 * fw + 1)) $fail_c $((fw)) $total_c

    else
        printf "${GRN}%*d/${REG}%*d\n" $((2 * fw + 1)) $success_c $((fw)) $total_c
    fi
    printf "%${total_len}s\n" | tr " " "="
    printf "%*s\n" $(((total_len + 9 - 2) / 2)) "ALL GOOD"
}

combine_and_pass $options_set
print_result
rm $s21_log $log
