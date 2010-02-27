#include "Includes.h"

void quit(char * title, char * message, ...) {
	if(message != NULL) {
		char buffer[256];
		va_list parameters;
		va_start(parameters, message);
		vsprintf_s(buffer, 256, message, parameters);
		va_end(parameters);
		MessageBoxA(NULL, buffer, title, MB_OK);
	}
}

void prompt(char * message, ...) {
	if(message != NULL) {
		char buffer[256];
		va_list parameters;
		va_start(parameters, message);
		vsprintf_s(buffer, 256, message, parameters);
		va_end(parameters);
		MessageBoxA(NULL, buffer, "Message", MB_OK);
	}
}
