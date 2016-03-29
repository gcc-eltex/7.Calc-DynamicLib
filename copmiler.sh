#!/bin/bash
#Компилируем все исходники до обьектных модулей
for file in $(find ./src -maxdepth 1 -type f -name "*.c");
do
	# Определяем имя файла без расширения
	filename=$(basename $file)
    name=${filename%.*}
    #Компилируем до позиционно независимого кода
	gcc -fPIC $file -c
	#Собираем в библиотеку
    gcc -shared  -o lib$name.so $name.o
    #
    rm $name.o -f
    mv lib$name.so ./plugins/
done
#Компилируем саму программу и запускаем
gcc main.c -o main -ldl
exit 0