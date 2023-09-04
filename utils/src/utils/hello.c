#include <utils/hello.h>
#include <commons/string.h>

void say_hello(char* who) {
	char* string = string_new();
	string_append(&string, "Hello %s!!\n");
    printf(string, who);
}

int suma(int valor1, int valor2) {
	return valor1 + valor2;
}
