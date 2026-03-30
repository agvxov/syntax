//@BAKE gcc -fPIC -shared -o hello_extension.so $@
#include <stdio.h>

extern const char * STRING;

void hl_extension(void) {
    STRING = "\033[31m";
}
