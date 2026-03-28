// @BAKE gcc -o $*.out $@ -std=c23 -Wall -Wpedantic -ggdb
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

static const char * kw_c[]    = {"auto", "int", "char", "if", NULL};

signed main(void) {
    syntax_init();
    syntax_define_keywords(kw_c, ANSI_RED, ANSI_RST);

    unsigned char buf[32];
    //syntax_highlight_string((char *)buf, "int x;", 8);
    syntax_highlight_string((char *)buf, "01010", 32);

    syntax_deinit();

    puts(buf);

    return 0;
}
