// @BAKE gcc -o main.out main.c -std=c23 -ggdb
#include <stdio.h>
#include "syntax.h"

const char * keywords[] = {
    "bool", "char",
    "short", "int", "long",
    "float", "double",
    "static",
    NULL,
};

signed main(void) {
    char buffer[1024];

    syntax_init();
    syntax_define_keywords(keywords, "\033[32m", "\033[0m");
    syntax_define_range("\"", "\"", "\\", "\033[33m", "\033[0m");
    syntax_highlight_string(
        buffer,
        "static char * s = \"test \\\"t test\";\n",
        1024
    );
    syntax_deinit();

    puts(buffer);

    return 0;
}
