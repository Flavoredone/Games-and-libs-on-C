#!/bin/bash

file=(1 2 3 4 5)

pattern=("only" "This", "fckingtest", "'w.th'", "'^[A-Z]'")

flags=(-e -i -v -c -l -n -h -s -f -o);

clang="clang.log"
clangerror="clangerror.log"

s21="s21.log"
s21error="s21error.log"

difference=""
diferror=""

cnt=0
all=0

# Set the color variable
green='\033[0;32m'
yellow='\033[0;33m'
blue='\033[0;34m'
clear='\033[0m'

clear

################################################################

echo -e "----------------${blue} Test-1 ${clear}----------------------"

echo " " > grep.log
echo "" > fail.log

for ((i=0; i<${#flags[@]}; i++)); do
    echo "!!! test flag:  ${flags[$i]}   with pattern:   ${pattern[0]}" >> grep.log
    for ((j=0; j<${#file[@]}; j++)); do
        for ((k=0; k<${#pattern[@]}; k++)); do

            let "all++"

            echo "${flags[$i]} ${pattern[$k]} tests/${file[$j]}" >> grep.log 2> /dev/null
            
            grep ${flags[$i]} ${pattern[$k]} tests/${file[$j]} > ${clang} 2> ${clangerror}
            
            ./s21_grep ${flags[$i]} ${pattern[$k]} tests/${file[$j]} > ${s21} 2> ${s21error}
            
            difference="$(diff -s "${clang}" "${s21}")"
            
            diferror="$(diff -s "${clangerror}" "${s21error}")"
            
            if [ "$difference" == "Files ${clang} and ${s21} are identical" ] && 
                [ "$diferror" == "Files ${clangerror} and ${s21error} are identical" ]
            then 
                let "cnt++"
            else 
                echo "${flags[$i]} ${pattern[$k]} tests/${file[$j]}" >> fail.log 2> /dev/null

            fi
        done
    done
done

echo -e "Successfull:${yellow} $cnt ${clear} from ${green} $all  ${clear} tests"
echo "----------------------------------------------"

################################################################

echo -e "----------------${blue} Test-2 ${clear}----------------------"

all=0
cnt=0

for ((j=0; j<${#file[@]}; j++)); do
    echo "!!!  Without flag,  with pattern:   ${pattern[1]}" >> grep.log
    
    let "all++"
    
    echo " ${pattern[1]} tests/${file[$j]}" >> grep.log 2> /dev/null
    
    grep  ${pattern[1]} tests/${file[$j]} > ${clang} 2> ${clangerror}
    
    ./s21_grep  ${pattern[1]} tests/${file[$j]} > ${s21} 2> ${s21error}
    
    difference="$(diff -s "${clang}" "${s21}")"
    
    diferror="$(diff -s "${clangerror}" "${s21error}")"
    
    if [ "$difference" == "Files ${clang} and ${s21} are identical" ] && 
        [ "$diferror" == "Files ${clangerror} and ${s21error} are identical" ]
    then 
        let "cnt++"
    else 
        echo " ${pattern[1]} tests/${file[$j]} " >> fail.log 2> /dev/null
    fi
done

echo -e "Successfull:${yellow} $cnt ${clear} from ${green} $all  ${clear} tests"
echo "----------------------------------------------"

################################################################

echo -e "----------------${blue} Test-3 ${clear}----------------------"

all=0
cnt=0

for ((i=0; i<${#flags[@]}; i++)); do 
    for ((j=0; j<${#file[@]}; j++)); do
        
        echo "!!! Patterns: ${pattern[0]} ${pattern[1]}, flag: $flag{$i}, in file:   ${file[$i]}" >> grep.log
        
        let "all++"
        
        echo "${pattern[0]} ${pattern[1]} ${flag[$i]} tests/${file[$j]}" >> grep.log 2> /dev/null
        
        grep ${pattern[0]} ${pattern[1]} ${flag[$i]} tests/${file[$j]} > ${clang} 2> ${clangerror}
        
        ./s21_grep ${pattern[0]} ${pattern[1]} tests/${file[$j]} > ${s21} 2> ${s21error}
        
        difference="$(diff -s "${clang}" "${s21}")"
        
        diferror="$(diff -s "${clangerror}" "${s21error}")"
        
        if [ "$difference" == "Files ${clang} and ${s21} are identical" ] && 
            [ "$diferror" == "Files ${clangerror} and ${s21error} are identical" ]
        then 
            let "cnt++"
        else 
            echo "${pattern[0]} ${pattern[1]} ${flag[$i]} tests/${file[$j]}" >> fail.log 2> /dev/null
        fi
    done
done

echo -e "Successfull:${yellow} $cnt ${clear} from ${green} $all  ${clear} tests"
echo "----------------------------------------------"

################################################################

echo -e "----------------${blue} Test-4 ${clear}----------------------"

all=0
cnt=0

for ((j=0; j<${#file[@]}; j++)); do
    
    echo "  Without flag,  with pattern:   ${pattern[1]}" >> grep.log
    
    let "all++"
    
    echo "${pattern[0]} ${pattern[1]} tests/${file[$j]}" >> grep.log 2> /dev/null
    
    grep -e ${pattern[0]} -e ${pattern[1]} tests/${file[$j]} > ${clang} 2> ${clangerror}
    
    ./s21_grep -e ${pattern[0]} -e ${pattern[1]} tests/${file[$j]} > ${s21} 2> ${s21error}
    
    difference="$(diff -s "${clang}" "${s21}")"
    
    diferror="$(diff -s "${clangerror}" "${s21error}")"
    
    if [ "$difference" == "Files ${clang} and ${s21} are identical" ] && 
        [ "$diferror" == "Files ${clangerror} and ${s21error} are identical" ]
    then 
        let "cnt++"
    else 
        echo " -e ${pattern[0]} -e ${pattern[1]} tests/${file[$j]}" >> fail.log 2> /dev/null
    fi
done

echo -e "Successfull:${yellow} $cnt ${clear} from ${green} $all  ${clear} tests"
echo "----------------------------------------------"

################################################################

rm -rf *.log