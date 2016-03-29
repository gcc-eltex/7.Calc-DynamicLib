#include <stdio.h>
#include <stdarg.h>
#include "global/pluginfo.c"

struct pluginfo plug_info = {"Деление", 2};

double plug_func(int argc, ...){
	va_list vl;
	double oper_1, oper_2;
	//Вытаскиваем параметры
	va_start(vl, argc);
	oper_1 = va_arg(vl, double);
	oper_2 = va_arg(vl, double);
	va_end(vl);	
	return (double)oper_1 / oper_2;
}
