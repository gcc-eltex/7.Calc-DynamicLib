#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>

#include "src/global/pluginfo.c"
#define MAX_PLUG 255

void printrez(double rez){
	printf("Rezult: %lf\n", rez);
	printf("\nEnter any character to continue...");
	scanf("%*s");
}

void getoperand(double *op_1, double *op_2){
	printf("Enter first operand: ");
	scanf("%lf", op_1);
	printf("Enter second operand: ");
	scanf("%lf", op_2);
	scanf("%*[^\n]");
}

int main()
{
	DIR *dir_d;								//Дескриптор директории
	void *lib_handle;						//Указатель на библиотеку
	char lib_path[255];
    struct dirent *file_inf;				//Структура для вытягивания информации о файле
	struct pluginfo *plug_inf[MAX_PLUG];	//Массив структур с информацией о плагинах
	double (*plug_fun[MAX_PLUG])(int, ...);	//Массив указателей на функции плагинов
	unsigned int plug_count;				//Количество найденых плагинов

    dir_d = opendir("./plugins");
    if (!dir_d) 
    {
        perror("diropen");
        return -1;
    }
    file_inf = readdir(dir_d);
    for(plug_count = 0; file_inf != NULL; file_inf = readdir(dir_d)) 
	{
		if(!strstr(file_inf->d_name, ".so")) continue;	// Если в названии нет .so, то пропускаем
    	strcpy(lib_path, "./plugins/");					//Формируем относительный путь к библиотеке
    	strcat(lib_path, file_inf->d_name);				
        lib_handle = dlopen(lib_path, RTLD_NOW);
        plug_inf[plug_count] = (struct pluginfo *)dlsym(lib_handle, "plug_info");	//Вытащили информацию о плагине
        plug_fun[plug_count] = dlsym(lib_handle, "plug_func");						//Вытащили функцию плагина
        plug_count++;
        
    }
    closedir(dir_d);
    
    int option = 0;
    double op_1, op_2;

    while(option != plug_count)	//Меню
    {
		write(1,"\E[H\E[2J",7);
		for(int i = 0; i< plug_count; i++)
			printf("%d] %s\n", i, plug_inf[i]->name);
		printf("%d] Exit\nYour selection: ", plug_count);
		scanf("%d", &option);
		//Если ввели некорректные данные или выходим
		scanf("%*[^\n]");
		if(option >= plug_count)
			continue;

		getoperand(&op_1, &op_2);
		printrez(plug_fun[option](2, op_1, op_2));
	}
    
}