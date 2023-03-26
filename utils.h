#include <stdarg.h>

inline int min(int nArguments, ...) {
    va_list args;
    va_start(args, nArguments);

    int result = va_arg(args, int); 

    int temp;
    for(int i = 1; i < nArguments; i++) {

	temp = va_arg(args, int);
	if(temp < result) {
	    result = temp; 
	}
    }

    va_end(args);

    return result;
}
