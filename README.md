# xyntax

## xyntax -- Header-only library for syntax definition, highlighting and tokenization.

Install: `$ sudo sh install.sh`

Include: `#include <xolatile/xyntax.h>`

This library depends on:
- xtandard: <https://gitlab.com/xolatile/xtandard>

About this library:
- Primary focus of this library is for syntax highlighting, hence the name, but it can do more.
- Important note: Regular expressions are more robust, this is simple solution for simple problems.
- If you want to do parsing, counting, tokenization, you can use return value of 'syntax_define'.
- Everything related to my libraries is clean of all warning options on Clang, GCC and Valgrind.
