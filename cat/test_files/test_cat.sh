#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_RES=""
OPTIONS="b e n s t v E T benstvET vtn"
RED=`tput setaf 1`
GREEN=`tput setaf 2`
BASE=`tput setaf 7`
 
declare -a files=(    
    "VAR test_files/test_1_cat.txt"
    "VAR test_files/test_2_cat.txt"
    "VAR test_files/test_3_cat.txt"
    "VAR test_files/test_4_cat.txt"
    "VAR test_files/test_5_cat.txt"
    "VAR test_files/test_case_cat.txt"
    "VAR test_files/test_1_cat.txt tests/test_2_cat.txt"
)

declare -a extra=(
     "test_files/test_case_cat.txt test_files/test_1_cat.txt -n"
     "test_files/test_case_cat.txt test_files/test_1_cat.txt -e"
     "test_files/test_case_cat.txt test_files/test_1_cat.txt -b"
     "test_files/test_case_cat.txt test_files/test_1_cat.txt -s"
     "test_files/test_case_cat.txt test_files/test_1_cat.txt -t"
     "test_files/test_case_cat.txt test_files/test_1_cat.txt -v"

    "test_files/test_case_cat.txt -b"
    "-vts test_files/test_case_cat.txt"
    
    "test_files/test_1_cat.txt"
    "-b -e -n -s -t -v test_files/test_1_cat.txt"
    "-t test_files/test_3_cat.txt"
    "-ET test_files/test_4_cat.txt"
    "-nb test_files/test_1_cat.txt"
    "-s -n -e test_files/test_4_cat.txt"
    "-n test_files/test_1_cat.txt"
    "-ne tetest_filessts/test_1_cat.txt"
    "-b test_files/test_1_cat.txt test_files/test_3_cat.txt"
    "-b test_files/test_1_cat.txt test_files/test_2_cat.txt"
    "-v test_files/test_5_cat.txt"
    "no_file.txt"
    "--number-nonblank test_files/test_5_cat.txt"
    "-d tests/test_5_cat.txt"
    "test_files/test_1_cat.txt"
    "-b -e -n -s -t -v test_files/test_1_cat.txt"
    "-b test_files/test_1_cat.txt nofile.txt"
    "-t test_files/test_3_cat.txt"
    "-n test_files/test_2_cat.txt"
    "no_file.txt"
    "-n -b test_files/test_1_cat.txt"
    "-s -n -e test_files/test_4_cat.txt"
    "test_files/test_1_cat.txt -n"
    "-n test_files/test_1_cat.txt"
    "-n test_files/test_1_cat.txt test_files/test_2_cat.txt"
    "-v test_files/test_5_cat.txt"
    "-- test_files/test_5_cat.txt"
        "-T test_files/test_1_cat.txt"
    "-E test_files/test_1_cat.txt"
    "-vT test_files/test_3_cat.txt"
    "--number test_files/test_2_cat.txt"
    "--squeeze-blank test_files/test_1_cat.txt"
    "--number-nonblank test_files/test_4_cat.txt"
    "test_files/test_1_cat.txt --number --number"
    "-bnvste test_files/test_6_cat.txt"
    "-n -v test_files/test_case_cat.txt test_files/test_1_cat.txt"
)


tests=(
    "VAR test_files/test_case_cat.txt"
    "VAR test_files/test_case_cat.txt test_files/test_1_cat.txt"
)

flags=(
    "b"
    "e"
    "n"
    "s"
    "t"
    "v"
)

manual=(
    "-s test_files/test_1_cat.txt"
    "-b -e -n -s -t -v test_files/test_1_cat.txt"
    "-b test_files/test_1_cat.txt nofile.txt"
    "-t test_files/test_3_cat.txt"
    "-n test_files/test_2_cat.txt"
    "no_file.txt"
    "-n -b test_files/test_1_cat.txt"
    "-s -n -e test_files/test_4_cat.txt"
    "test_files/test_1_cat.txt -n"
    "-n test_files/test_1_cat.txt"
    "-n test_files/test_1_cat.txt test_files/test_2_cat.txt"
    "-v test_files/test_5_cat.txt"
    "-- test_files/test_5_cat.txt"
)

gnu=(
    "-T test_files/test_1_cat.txt"
    "-E test_files/test_1_cat.txt"
    "-vT test_files/test_3_cat.txt"
    "--number test_files/test_2_cat.txt"
    "--squeeze-blank test_files/test_1_cat.txt"
    "--number-nonblank test_files/test_4_cat.txt"
    "test_files/test_1_cat.txt --number --number"
    "-bnvste test_files/test_6_cat.txt"
    " -n test_files/test_case_cat.txt test_files/test_1_cat.txt"
)

testing() {
    t=$(echo $@ | sed "s/VAR/$var/") #echo "-n test_files/test_case_cat.txt test_files/test_1_cat.txt" | sed "s/VAR/$var/"
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    SHA1=`cat test_s21_cat.log | sha256sum`
    SHA2=`cat test_sys_cat.log | sha256sum`
    #echo $SHA1
    #echo $SHA2
    DIFF_RES=`diff test_s21_cat.log test_sys_cat.log -q`

    # if [ -z "$DIFF_RES" ] && 
    if [[ $SHA1 == $SHA2 ]]
    then
      (( SUCCESS++ ))
        RESULT="SUCCESS"
        echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} cat $t"

    else
      (( FAIL++ ))
        RESULT="FAIL"
        echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${RED}${RESULT}${BASE} cat $t"

    fi
    # echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} cat $t"
    rm test_s21_cat.log test_sys_cat.log
}

# специфические тесты
for i in "${extra[@]}"
do
    # var=""
    testing "$i"
done

# 1 параметр
for var1 in $OPTIONS
do
    for i in "${files[@]}"
    do
        var="-$var1"
        testing "$i"
    done
done

# 2 параметра
for var1 in $OPTIONS
do
    for var2 in $OPTIONS
    do
        if [ $var1 != $var2 ]
        then
            for i in "${files[@]}"
            do
                var="-$var1 -$var2"
                testing "$i"
            done
        fi
    done
done

# echo "MANUAL TESTS"
for i in "${manual[@]}"
do
    var="-"
    testing "$i"
done

# echo "1 PARAMETER"
for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing "$i"
    done
done

# echo "2 PARAMETERS"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing "$i"
            done
        fi
    done
done

# echo "3 PARAMETERS"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing "$i"
                done
            fi
        done
    done
done

# 4 param
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            for var4 in "${flags[@]}"
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        testing "$i"
                    done
                fi
            done
        done
    done
done

# 2 сдвоенных параметра
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing "$i"
            done
        fi
    done
done

# 3 строенных параметра
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing "$i"
                done
            fi
        done
    done
done

for i in "${gnu[@]}"
do
    var="-"
    testing "$i"
done

echo "${RED}FAIL: ${FAIL}${BASE}"
echo "${GREEN}SUCCESS: ${SUCCESS}${BASE}"