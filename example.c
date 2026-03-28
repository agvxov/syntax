// @BAKE gcc -o $*.out $@ -std=c99 -Wall -Wpedantic -ggdb
#include <stdio.h>
#include "syntax.h"

/* ANSI color sequences. */
#define ANSI_RED    "\x1b[31m"
#define ANSI_GREEN  "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE   "\x1b[34m"
#define ANSI_MAG    "\x1b[35m"
#define ANSI_CYAN   "\x1b[36m"
#define ANSI_RST    "\x1b[0m"

static const char * digits = "0123456789";

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
    syntax_define_chars(digits, ANSI_CYAN, ANSI_RST);
    syntax_define_keywords(keywords, ANSI_RED, ANSI_RST);

    syntax_highlight_string(buffer, "int x = 01010;", sizeof(buffer));

    syntax_deinit();

    puts(buffer);

    return 0;
}
