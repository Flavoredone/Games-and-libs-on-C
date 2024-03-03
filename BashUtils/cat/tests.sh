#!/bin/bash

file=(1 2 3 4 5)

flags=(b e n s t v -number -squeeze-blank -number-nonblank)
dflags=(be bn bs bt bv en es et ev ns nt nv st sv tv ben ens nst benstv)

clang="clang.log"
cerror="cerror.log"

s21="s21.log"
s21error="s21error.log"

cnt=0
all=0

# Set the color variable
green='\033[0;32m'
yellow='\033[0;33m'
blue='\033[0;34m'
clear='\033[0m'

clear

echo " " > cat.log
echo " " > fail.log

################################################################

echo -e "----------------${blue} Test-1 ${clear}----------------------"

for ((i=0; i<${#flags[@]}; i++)); do
    echo "!!!  test flag:  ${flags[$i]} " >> cat.log

    for ((j=0; j<${#file[@]}; j++)); do

            let "all++"

            echo "-${flags[$i]} tests/${file[$j]}" >> cat.log
            
            cur="${flags[i]}"
            
            if [ "$cur" == "-number" ]
            then
                cat -n tests/${file[$j]} > ${clang} 2> ${cerror}
                ./s21_cat -n tests/${file[$j]} > ${s21} 2> ${s21error}
            elif [ "$cur" == "-squeeze-blank" ]
            then
                cat -s tests/${file[$j]} > ${clang} 2> ${cerror}
                ./s21_cat -s tests/${file[$j]} > ${s21} 2> ${s21error}
            elif [ "$cur" == "-number-nonblank" ]
            then
                cat -b tests/${file[$j]} > ${clang} 2> ${cerror}
                ./s21_cat -b tests/${file[$j]} > ${s21} 2> ${s21error}
            else
            cat -${flags[$i]} tests/${file[$j]} > ${clang} 2> ${cerror}
            ./s21_cat -${flags[$i]} tests/${file[$j]} > ${s21} 2> ${s21error}
            difference="$(diff -s "${clang}" "${s21}")"
            diferror="$(diff -s "${cerror}" "${s21error}")"
            fi

            if [ "$difference" == "Files ${clang} and ${s21} are identical" ] &&
                [ "$diferror" == "Files ${cerror} and ${s21error} are identical" ]

            then 
                let "cnt++"
            else 
                echo "${flags[$i]} tests/${file[$j]}" >> fail.log
            fi
    done
done

echo -e "Successfull:${yellow} $cnt ${clear} from ${green} $all  ${clear} tests"
echo "----------------------------------------------"

################################################################

echo -e "----------------${blue} Test-2 ${clear}----------------------"

all=0
cnt=0

for ((i=0; i<${#dflags[@]}; i++)); do
    echo "!!!  test flag:  ${dflags[$i]} " >> cat.log
    for ((j=0; j<${#file[@]}; j++)); do

            let "all++"

            echo "-${dflags[$i]} tests/${file[$j]}" >> cat.log

            cat -${dflags[$i]} tests/${file[$j]} > ${clang} 2> ${cerror}
            ./s21_cat -${dflags[$i]} tests/${file[$j]} > ${s21} 2> ${s21error}
            difference="$(diff -s "${clang}" "${s21}")"
            diferror="$(diff -s "${cerror}" "${s21error}")"
            
            if [ "$difference" == "Files ${clang} and ${s21} are identical" ] &&
                [ "$diferror" == "Files ${cerror} and ${s21error} are identical" ]

            then 
                let "cnt++"
            else 
                echo "${flags[$i]} tests/${file[$j]}" >> fail.log
            fi
    done
done

echo -e "Successfull:${yellow} $cnt ${clear} from ${green} $all  ${clear} tests"
echo "----------------------------------------------"

################################################################

echo -e "----------------${blue} Test-3 ${clear}----------------------"

for ((i=0; i<${#flags[@]}; i++)); do
    echo "!!!  test flag:  ${flags[$i]} " >> cat.log

            let "all++"
            echo "-${flags[$i]} tests/${file[0]} tests/${file[1]}" >> cat.log
            cur="${flags[i]}"
            if [ "$cur" == "-number" ]
            then
                cat -n tests/${file[0]} tests/${file[1]} > ${clang} 2> ${cerror}
                ./s21_cat -n tests/${file[0]} tests/${file[1]} > ${s21} 2> ${s21error}
            elif [ "$cur" == "-squeeze-blank" ]
            then
                cat -s tests/${file[0]} tests/${file[1]} > ${clang} 2> ${cerror}
                ./s21_cat -s tests/${file[0]} tests/${file[1]} > ${s21} 2> ${s21error}
            elif [ "$cur" == "-number-nonblank" ]
            then
                cat -b tests/${file[0]} tests/${file[1]} > ${clang} 2> ${cerror}
                ./s21_cat -b tests/${file[0]} tests/${file[1]} > ${s21} 2> ${s21error}
            else
            cat -${flags[$i]} tests/${file[0]} tests/${file[1]} > ${clang} 2> ${cerror}
            ./s21_cat -${flags[$i]} tests/${file[0]} tests/${file[1]} > ${s21} 2> ${s21error}
            
            difference="$(diff -s "${clang}" "${s21}")"
            diferror="$(diff -s "${cerror}" "${s21error}")"
            fi

            if [ "$difference" == "Files ${clang} and ${s21} are identical" ] &&
                [ "$diferror" == "Files ${cerror} and ${s21error} are identical" ]

            then 
                let "cnt++"
            else 
                echo "${flags[$i]} tests/${file[$j]}" >> fail.log
            fi
done

echo -e "Successfull:${yellow} $cnt ${clear} from ${green} $all  ${clear} tests"
echo "----------------------------------------------"

################################################################

rm -rf *.log