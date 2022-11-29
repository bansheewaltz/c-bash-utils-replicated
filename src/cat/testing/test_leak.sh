#!/bin/bash
success=0
fail=0
counter=0
diff_red=""
path="./data_samples/"

# colours
RED=$(tput setaf 1)
GRN=$(tput setaf 2)
REG=$(tput sgr0)

declare -a tests=(
    "VAR ${path}test_case_cat.txt"
)

declare -a extra=(
    "-s ${path}test_1_cat.txt"
    "-b -e -n -s -t -v ${path}test_1_cat.txt"
    "-t ${path}test_3_cat.txt"
    "-n ${path}test_2_cat.txt"
    "no_file.txt"
    "-n -b ${path}test_1_cat.txt"
    "-s -n -e ${path}test_4_cat.txt"
    "${path}test_1_cat.txt -n"
    "-n ${path}test_1_cat.txt"
    "-n ${path}test_1_cat.txt ${path}test_2_cat.txt"
    "-v ${path}test_5_cat.txt"
)

# exec 2>/dev/null
testing() {
    t=$(echo $@ | sed "s/VAR/$var/")
    leaks -quiet -atExit -- ./s21_cat $t >test_s21_cat.log
    leak=$(grep -A100000 leaks test_s21_cat.log)
    ((counter++))
    if [[ $leak == *"0 leaks for 0 total leaked bytes"* ]]; then
        ((success++))
        printf "${RED}%3d${REG}/${GRN}%3d${REG}/%3d ${GRN}%7s${REG} cat $t\n" $fail $success $counter "SUCCESS"
    else
        ((fail++))
        printf "${RED}%3d${REG}/${GRN}%3d${REG}/%3d ${RED}%7s${REG} cat $t\n" $fail $success $counter "FAIL"
    fi
}

# специфические тесты
for i in "${extra[@]}"; do
    var="-"
    testing $i
done

# 1 параметр
for var1 in b e n s t v; do
    for i in "${tests[@]}"; do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in b e n s t v; do
    for var2 in b e n s t v; do
        if [ $var1 != $var2 ]; then
            for i in "${tests[@]}"; do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in b e n s t v; do
    for var2 in b e n s t v; do
        for var3 in b e n s t v; do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]; then
                for i in "${tests[@]}"; do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 4 параметра
for var1 in b e n s t v; do
    for var2 in b e n s t v; do
        for var3 in b e n s t v; do
            for var4 in b e n s t v; do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] &&
                    [ $var1 != $var3 ] && [ $var1 != $var4 ] &&
                    [ $var2 != $var4 ] && [ $var3 != $var4 ]; then
                    for i in "${tests[@]}"; do
                        var="-$var1 -$var2 -$var3 -$var4"
                        testing $i
                    done
                fi
            done
        done
    done
done

echo "${RED}FAIL: $fail ${REG}"
echo "${GRN}SUCCESS: $success ${REG}"
echo "ALL: $counter"
rm test_s21_cat.log
