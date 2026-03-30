// @BAKE gcc -o $* $@ -std=c23 -ldl -rdynamic -Wall -Wpedantic -ggdb
#include "library/slurp.h"
#include <dlfcn.h>
#include <unistd.h>
#define SYNTAX_DEFINITION_MAX 32
#define SYNTAX_IMPLEMENTATION
#include "syntax.h"

const char * RST          = "\033[0m";
const char * RED          = "\033[31m";
const char * GREEN        = "\033[32m";
const char * YELLOW       = "\033[33m";
const char * BLUE         = "\033[34m";
const char * MAGENTA      = "\033[35m";
const char * CYAN         = "\033[36m";
const char * WHITE        = "\033[37m";
const char * GRAY         = "\033[90m";
const char * BOLD_RED     = "\033[1;31m";
const char * BOLD_GREEN   = "\033[1;32m";
const char * BOLD_YELLOW  = "\033[1;33m";
const char * BOLD_BLUE    = "\033[1;34m";
const char * BOLD_MAGENTA = "\033[1;35m";
const char * BOLD_CYAN    = "\033[1;36m";
const char * BOLD_WHITE   = "\033[1;37m";
const char * BOLD_GRAY    = "\033[1;90m";

const char * COMMENT = "\033[90m";
const char * TYPE    = "\033[36m";
const char * STRING  = "\033[32m";
const char * SPECIAL = "\033[33m";
const char * CONST   = "\033[34m";
const char * KEYWORD = "\033[1;34m";

const char * filename = nullptr;
const char * libname  = nullptr;
const char * srcname  = nullptr;

const char my_source_code[] = {
    #embed "hl.c"
    , '\0'
};

typedef void (*syntax_fn)(void);
syntax_fn syntax_function = nullptr;
syntax_fn hl_extension = nullptr;

// ---

const char * digits = "0123456789";

void syntax_ada(void) {
    static const char * const ada_types[] = {
        "string",
        "character",
        "integer",
        "natural",
        "float",
        "boolean",
        "positive",
        "negative",
        "duration",
        NULL,
    };
    static const char * const ada_errors[] = {
        "constraint_error",
        "program_error",
        "storage_error",
        "tasking_error",
        "numeric_error",
        NULL,
    };
    static const char * const ada_keywords[] = {
        "abort",
        "else",
        "new",
        "return",
        "abs",
        "elsif",
        "not",
        "reverse",
        "abstract",
        "end",
        "null",
        "accept",
        "entry",
        "select",
        "access",
        "of",
        "separate",
        "aliased",
        "exit",
        "or",
        "some",
        "all",
        "others",
        "subtype",
        "and",
        "for",
        "out",
        "array",
        "function",
        "at",
        "tagged",
        "generic",
        "package",
        "task",
        "begin",
        "goto",
        "pragma",
        "body",
        "private",
        "then",
        "type",
        "case",
        "in",
        "constant",
        "until",
        "is",
        "raise",
        "use",
        "if",
        "declare",
        "range",
        "delay",
        "limited",
        "record",
        "when",
        "delta",
        "loop",
        "rem",
        "while",
        "digits",
        "renames",
        "with",
        "do",
        "mod",
        "requeue",
        "xor",
        "procedure",
        "protected",
        "interface",
        "synchronized",
        "exception",
        "overriding",
        "terminate",
        NULL,
    };
    syntax_init();

    syntax_define_chars(digits, CONST, RST);
    syntax_define_chars(".,:;<=>+-*/&|@", SPECIAL, RST);
    syntax_define_chars("()[]", SPECIAL, RST);
    syntax_define_keywords(ada_types, TYPE, RST);
    syntax_define_keywords(ada_errors, RED, RST);
    syntax_define_keywords(ada_keywords, KEYWORD, RST);
    syntax_define_region("\"", "\"", "", STRING, RST);
    syntax_define_region(" '", "'", "", STRING, RST);
    syntax_define_region("--", "", "\n", COMMENT, RST);
}

void syntax_c(void) {
    static const char * const c_type[] = {
        "void",
        "bool",
        "char",
        "int",
        "float",
        "double",
        "size_t",
        "FILE",
        "char8_t",
        "char16_t",
        "char32_t",
        "wchar_t",
        NULL,
    };
    static const char * const c_group[] = {
        "enum",
        "struct",
        "typedef",
        "union",
        NULL,
    };
    static const char * const c_value[] = {
        "true",
        "false",
        "TRUE",
        "FALSE",
        "nullptr",
        "NULL",
        NULL,
    };
    static const char * const c_specifier[] = {
        "extern",
        "static",
        "inline",
        "const",
        "auto",
        "signed",
        "unsigned",
        "short",
        "long",
        NULL,
    };
    static const char * const c_control[] = {
        "break",
        "continue",
        "goto",
        "return",
        NULL,
    };
    static const char * const c_jump[] = {
        "if",
        "else",
        "while",
        "do",
        "for",
        "default",
        "case",
        "error",
        "end",
        "done",
        NULL,
    };
    static const char * const c_special[] = {
        "sizeof",
        "alignas",
        "alignof",
        "restrict",
        "static_assert",
        "thread_local",
        "typeof",
        "typeof_unqual",
        "volatile",
        "asm",
        "fortran",
        NULL,
    };

    syntax_define_chars(digits, CONST, RST);
    syntax_define_chars("+-*/%=&|^~!<>?:", SPECIAL, RST);
    syntax_define_chars(".;()[]{}", SPECIAL, RST);
    syntax_define_keywords(c_type, TYPE, RST);
    syntax_define_keywords(c_group, GREEN, RST);
    syntax_define_keywords(c_value, CONST, RST);
    syntax_define_keywords(c_specifier, GREEN, RST);
    syntax_define_keywords(c_control, YELLOW, RST);
    syntax_define_keywords(c_jump, YELLOW, RST);
    syntax_define_keywords(c_special, SPECIAL, RST);
    syntax_define_region("\"", "\"", "\\", STRING, RST);
    syntax_define_region("'", "'", "\\", STRING, RST);
    syntax_define_region("#", "\n", "", GRAY, RST);
    syntax_define_region("//", "\n", "\"", COMMENT, RST);
    syntax_define_region("/*", "*/", "", COMMENT, RST);
}

void syntax_cpp(void) {
    static const char * const cpp_logic2[] = {
        "and",
        "and_eq",
        "bitand",
        "bitor",
        "not",
        "not_eq",
        "or",
        "or_eq",
        "xor",
        "xor_eq ",
        NULL,
    };
    static const char * const cpp_group2[] = {
        "template",
        "class",
        "decltype",
        "typeid",
        "typename",
        NULL,
    };
    static const char * const cpp_exception[] = {
        "try",
        "catch",
        "noexcept",
        "throw",
        NULL,
    };
    static const char * const cpp_atomic[] = {
        "atomic_cancel",
        "atomic_commit",
        "atomic_noexcept",
        NULL,
    };
    static const char * const cpp_cast[] = {
        "const_cast",
        "dynamic_cast",
        "reinterpret_cast",
        "static_cast",
        NULL,
    };
    static const char * const cpp_constant[] = {
        "consteval",
        "constexpr",
        "constinit",
        NULL,
    };
    static const char * const cpp_coroutine[] = {
        "co_await",
        "co_return",
        "co_yield",
        NULL,
    };
    static const char * const cpp_allocation[] = {
        "new",
        "delete",
        NULL,
    };
    static const char * const cpp_contract[] = {
        "compl",
        "pre",
        "post",
        "contract_assert",
        "transaction_safe",
        "transaction_safe_dynamic",
        NULL,
    };
    static const char * const cpp_protection[] = {
        "friend",
        "private",
        "protected",
        "public",
        NULL,
    };
    static const char * const cpp_extra_special[] = {
        "trivially_relocatable_if_eligible",
        "replaceable_if_eligible",
        NULL,
    };
    static const char * const cpp_bullshit1[] = {
        "using",
        "namespace",
        "import",
        "export",
        "module",
        "requires",
        NULL,
    };
    static const char * const cpp_bullshit2[] = {
        "this",
        "final",
        "virtual",
        "override",
        "concept",
        "explicit",
        "mutable",
        "operator",
        "reflexpr",
        "synchronized",
        NULL,
    };

    syntax_c();

    syntax_define_keywords(cpp_logic2, RED, RST);
    syntax_define_keywords(cpp_group2, GREEN, RST);
    syntax_define_keywords(cpp_exception, YELLOW, RST);
    syntax_define_keywords(cpp_atomic, GREEN, RST);
    syntax_define_keywords(cpp_cast, MAGENTA, RST);
    syntax_define_keywords(cpp_constant, BLUE, RST);
    syntax_define_keywords(cpp_coroutine, CYAN, RST);
    syntax_define_keywords(cpp_allocation, KEYWORD, RST);
    syntax_define_keywords(cpp_contract, SPECIAL, RST);
    syntax_define_keywords(cpp_protection, GREEN, RST);
    syntax_define_keywords(cpp_extra_special, SPECIAL, RST);
    syntax_define_keywords(cpp_bullshit1, MAGENTA, RST);
    syntax_define_keywords(cpp_bullshit2, RED, RST);
}

void syntax_fasm(void) {
    // XXX
    // in asm every instruction is technically a keyword
    // there are thousands and they change as technology advances
    // it only makes sense to threat any bareword special
}


void syntax_fortran(void) {
    static const char * const fortran_keywords[] = {
        "allocatable",
        "allocate",
        "associate",
        "backspace",
        "block",
        "call",
        "case",
        "common",
        "contains",
        "cycle",
        "data",
        "deallocate",
        "d0",
        "do",
        "else",
        "elseif",
        "end",
        "enddo",
        "endfile",
        "endif",
        "entry",
        "equivalence",
        "exit",
        "external",
        "forall",
        "format",
        "function",
        "goto",
        "if",
        "implicit",
        "inquire",
        "intent",
        "intrinsic",
        "module",
        "namelist",
        "none",
        "nullify",
        "only",
        "open",
        "optional",
        "parameter",
        "pointer",
        "print",
        "private",
        "program",
        "public",
        "read",
        "recursive",
        "return",
        "rewind",
        "save",
        "select",
        "sequence",
        "stop",
        "subroutine",
        "target",
        "then",
        "to",
        "type",
        "use",
        "where",
        "write",
        NULL
    };
    static const char * const fortran_types[] = {
        "character",
        "integer",
        "boolean",
        "real",
        "complex",
        NULL
    };
    static const char * const fortran_commons[] = {
        ".and.",
        ".or.",
        ".not.",
        ".true.",
        ".false.",
        "in",
        "out",
        "len",
        "dimension",
        "modulo",
        "advance",
        NULL
    };

    syntax_define_chars("()[]", SPECIAL, RST);
    syntax_define_chars(",:<=>+-*/&", SPECIAL, RST);
    syntax_define_keywords(fortran_keywords, KEYWORD, RST);
    syntax_define_keywords(fortran_types, TYPE, RST);
    syntax_define_keywords(fortran_commons, KEYWORD, RST);
    syntax_define_region("!", "\n", "", COMMENT, RST);
    syntax_define_region("'", "'", "\\", STRING, RST);
    syntax_define_region("\"", "\"", "\\", STRING, RST);
}


void syntax_go(void) {
    static const char * const go_keywords[] = {
        "break",
        "default",
        "func",
        "interface",
        "select",
        "case",
        "defer",
        "go",
        "struct",
        "else",
        "goto",
        "package",
        "switch",
        "const",
        "var",
        "for",
        "fallthrough",
        "if",
        "range",
        "type",
        "continue",
        "import",
        "return",
        NULL
    };
    static const char * const go_types[] = {
        "map",
        "uint",
        "int",
        "uintptr",
        "uint8",
        "uint16",
        "uint32",
        "uint64",
        "int8",
        "int16",
        "int32",
        "int64",
        "float32",
        "float64",
        "complex64",
        "complex128",
        "byte",
        "rune",
        "string",
        "chan",
        "boolean",
        NULL
    };
    static const char * const go_commons[] = {
        "true",
        "false",
        "nil",
        "err",
        NULL
    };

    syntax_define_chars("()[]{}", SPECIAL, RST);
    syntax_define_chars(".,:;<=>+*-/%!&~^?|", SPECIAL, RST);
    syntax_define_keywords(go_keywords, KEYWORD, RST);
    syntax_define_keywords(go_types, TYPE, RST);
    syntax_define_keywords(go_commons, SPECIAL, RST);
    syntax_define_region("\"", "\"", "\\", STRING, RST);
    syntax_define_region("'", "'", "\\", STRING, RST);
    syntax_define_region("`", "`", "", STRING, RST);
    syntax_define_region("//", "\"", "\n", COMMENT, RST);
    syntax_define_region("/*", "", "*/", COMMENT, RST);
}

void syntax_haskell(void) {
    static const char * const haskell_keywords[] = {
        "case",
        "class",
        "data",
        "deriving",
        "do",
        "else",
        "if",
        "import",
        "in",
        "infix",
        "infixl",
        "infixr",
        "instance",
        "let",
        "of",
        "module",
        "newtype",
        "then",
        "type",
        "where",
        NULL
    };
    static const char * const haskell_types[] = {
        "Int",
        "Integer",
        "String",
        "Char",
        "Float",
        "Boolean"
    };

    syntax_define_chars("()[]{}", SPECIAL, RST);
    syntax_define_chars(".,:;<=>+*-/%!@#$&~^?|", SPECIAL, RST);
    syntax_define_keywords(haskell_keywords, KEYWORD, RST);
    syntax_define_keywords(haskell_types, TYPE, RST);
    syntax_define_region("--", "\n", "", COMMENT, RST);
    syntax_define_region("{-", "-}", "", COMMENT, RST);
    syntax_define_region("'", "'", "\\", STRING, RST);
    syntax_define_region("\"", "\"", "\\", STRING, RST);
}

void syntax_holy_c(void) {
    static const char * const holy_c_special[] = {
        "class",
        "public",
        "no_warn",
        "start",
        "end",
        "throw",
        "catch",
        "_extern",
        "_intern",
        "_import",
        "lastclass",
        "offset",
        "defined",
        "lock",
        "interrupt",
        "reg",
        "noreg",
        "argpop",
        "noargpop",
        "haserrcode",
        NULL
    };
    static const char * const holy_c_type[] = {
        "U8",
        "U16",
        "U32",
        "U64",
        "I8",
        "I16",
        "I32",
        "I64",
        "U0",
        "I0",
        "F64",
        "Bool",
        NULL
    };

    syntax_c();

    syntax_define_keywords(holy_c_special, SPECIAL, RST);
    syntax_define_keywords(holy_c_type, TYPE, RST);
}

void syntax_lua(void) {
    static const char * const lua_keywords[] = {
        "and",
        "break",
        "do",
        "else",
        "elseif",
        "end",
        "false",
        "for",
        "function",
        "if",
        "in",
        "local",
        "nil",
        "not",
        "or",
        "until",
        "repeat",
        "return",
        "then",
        "true",
        "while",
         NULL
    };
    static const char * const lua_types[] = {
        "booleanean",
        "number",
        "string",
        "userdata",
        "function",
        "thread",
        "table"
    };
    static const char * const lua_commons[] = {
        "require",
        "print",
        "seek",
        "dofile",
        "loadfile",
        "assert",
        "rawset",
        "rawget",
        "setfenv",
        "pairs",
        "ipairs",
        "tonumber",
        "tostring",
        "foreach",
        "setn",
        "getn",
        "insert",
        "concat",
        "sort",
        "remove",
        "abs",
        "ceil",
        "floor",
        "log10",
        "rad",
        "sqrt",
        "acos",
        "cos",
        "fmod",
        "max",
        "random",
        "tan",
        "asin",
        "cosh",
        "frexp",
        "min",
        "randomseed",
        "tanh",
        "atan",
        "deg",
        "ldexp",
        "modf",
        "sin",
        "atan2",
        "sinh",
        "exp",
        "log",
        "pow",
        "open",
        "close",
        "read",
        "write",
        "input",
        "output",
        "format",
        "lines",
        "upper",
        "lower",
        "find",
        "gfind",
        "match",
        "gmatch",
        "sub",
        "gsub",
        "len",
        "rep",
        "char",
        "dump",
        "reverse",
        "byte"
    };

    syntax_define_chars(digits, CONST, RST);
    syntax_define_chars("()[]{}", SPECIAL, RST);
    syntax_define_chars(".,:;<=>+-*/%~^#|", SPECIAL, RST);
    syntax_define_keywords(lua_keywords, KEYWORD, RST);
    syntax_define_keywords(lua_types, TYPE, RST);
    syntax_define_keywords(lua_commons, KEYWORD, RST);
    syntax_define_region("--", "\n", "", COMMENT, RST);
    syntax_define_region("--[[", "]]", "", COMMENT, RST);
    syntax_define_region("'", "'", "\\", STRING, RST);
    syntax_define_region("\"", "\"", "\\", STRING, RST);
}

void syntax_python(void) {
    static const char * const python_keywords[] = {
        "and",
        "as",
        "assert",
        "break",
        "class",
        "continue",
        "def",
        "del",
        "elif",
        "else",
        "except",
        "False",
        "finally",
        "for",
        "from",
        "global",
        "if",
        "import",
        "in",
        "is",
        "labmda",
        "None",
        "nonlocal",
        "not",
        "or",
        "pass",
        "raise",
        "return",
        "True",
        "try",
        "while",
        "with",
        "yield",
        "async",
        "await",
        NULL
    };
    static const char * const python_builtins[] = {
        "abs",
        "aiter",
        "all",
        "anext",
        "any",
        "ascii",
        "bin",
        "bool",
        "breakpoint",
        "bytearray",
        "bytes",
        "callable",
        "chr",
        "classmethod",
        "compile",
        "complex",
        "copyright",
        "credits",
        "delattr",
        "dict",
        "dir",
        "divmod",
        "enumerate",
        "eval",
        "exec",
        "exit",
        "filter",
        "float",
        "format",
        "frozenset",
        "getattr",
        "globals",
        "hasattr",
        "hash",
        "help",
        "hex",
        "id",
        "input",
        "int",
        "isinstance",
        "issubclass",
        "iter",
        "len",
        "license",
        "list",
        "locals",
        "map",
        "max",
        "memoryview",
        "min",
        "next",
        "object",
        "oct",
        "open",
        "ord",
        "pow",
        "print",
        "property",
        "quit",
        "range",
        "repr",
        "reversed",
        "round",
        "set",
        "setattr",
        "slice",
        "sorted",
        "staticmethod",
        "str",
        "sum",
        "super",
        "tuple",
        "type",
        "vars",
        "zip",
        NULL
    };

    syntax_define_chars(".,:;<=>+*-/%!&~^?|", SPECIAL, RST);
    syntax_define_keywords(python_keywords, KEYWORD, RST);
    syntax_define_keywords(python_builtins, KEYWORD, RST);
    syntax_define_region("#", "\n", "", COMMENT, RST);
    syntax_define_region("\"", "\"", "\\", STRING, RST);
    syntax_define_region("\'", "\'", "\\", STRING, RST);
    syntax_define_region("'''", "'''", "", STRING, RST);
}

void syntax_valgrind(void) {
    static const char * const valgrind_title[] = {
        "HEAP",
        "LEAK",
        "ERROR",
        "SUMMARY",
        NULL
    };

    syntax_define_chars(digits, CONST, RST);
    syntax_define_chars(".,:;<=>+-*/%!&~^?|@#$", SPECIAL, RST);
    syntax_define_keywords(valgrind_title, KEYWORD, RST);
    syntax_define_region("==", "==", "", BOLD_RED, RST);
    syntax_define_region("\"", "\"", "", STRING, RST);
    syntax_define_region("(", ")", "", MAGENTA, RST);
    syntax_define_region("0x", ":", "", SPECIAL, RST);
}

// ---

#define switchs(s) do { const char *__ss = (s); if (0) {
#define cases(x)   } else if (strcmp(__ss, (x)) == 0) {
#define defaults   } else {
#define endswitchs } } while (0)

void usage(void) {
    fputs(
        "hl [options] <file> : cat highlighted code\n"
        " --ada                   : use Ada syntax\n"
        " --c                     : use C syntax\n"
        " --cpp                   : use C++ syntax\n"
        " --fasm                  : use Fasm syntax\n"
        " --fortran               : use Fortran syntax\n"
        " --holy-c                : use Holy C syntax\n"
        " --lua                   : use Lua syntax\n"
        " --python                : use Python syntax\n"
        " --valgrind              : use Valgrind syntax\n"
        " --load file             : load dynamic library and call hl_extension\n"
        " --load-and-compile file : attempt to compile dynamic library and ^^^ load it;\n"
        "                            it will fail outside of a UNIX like system\n"
        "                            using a GCC like compiler\n"
        " --dump                  : print own source code and exit\n"
        , stderr
    );
}

void handle_arguments(int argc, char * * argv) {
    syntax_function = syntax_c;

    for (int i = 1; i < argc; i++) {
        switchs(argv[i])
            cases("--help") {
                usage();
                exit(0);
            }
            cases("--ada")      syntax_function = syntax_ada;      break;
            cases("--c")        syntax_function = syntax_c;        break;
            cases("--cpp")      syntax_function = syntax_cpp;      break;
            cases("--fasm")     syntax_function = syntax_fasm;     break;
            cases("--fortran")  syntax_function = syntax_fortran;  break;
            cases("--holy-c")   syntax_function = syntax_holy_c;   break;
            cases("--lua")      syntax_function = syntax_lua;      break;
            cases("--python")   syntax_function = syntax_python;   break;
            cases("--valgrind") syntax_function = syntax_valgrind; break;
            cases("--load") {
                if (i + 1 >= argc) {
                    fprintf(stderr, "Missing argument.\n");
                    exit(3);
                }

                libname = argv[i+1];
                ++i;
                break;
            }
            cases("--compile-and-load") {
                if (i + 1 >= argc) {
                    fprintf(stderr, "Missing argument.\n");
                    exit(3);
                }

                srcname = argv[i+1];
                libname = "./extension.so";
                ++i;
                break;
            }
            cases("--dump") {
                puts(my_source_code);
                exit(0);
            }
            defaults {
                if (argv[i][0] == '-') {
                    fprintf(stderr, "Unrecognized option '%s'.\n", argv[i]);
                    exit(2);
                }

                filename = argv[i];
                return;
            }
        endswitchs;
    }

    if (isatty(STDIN_FILENO)) {
        fputs("Missing required argument.\n", stderr);
        usage();
        exit(1);
    } else {
        filename = "/dev/stdin";
    }
}

void load_extension(void) {
    if (srcname) {
        char * command_buffer;
        asprintf(&command_buffer,
            "cc -fPIC -shared -o extension.so %s",
            srcname
        );
        int err = system(command_buffer);
        free(command_buffer);

        if (err) {
            fputs("Compile-step failed.\n", stderr);
            exit(5);
        }
    }

    if (!libname) {
        return;
    }

    void * handle = dlopen(libname, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Error of dlopen: %s.\n", dlerror());
        exit(4);
    }

    dlerror(); // clear existing errors

    hl_extension = (syntax_fn)dlsym(handle, "hl_extension");

    char * err = dlerror();
    if (err) {
        fprintf(stderr, "Error of dlsym: %s.\n", err);
        exit(4);
    }

    hl_extension();
}

signed main(int argc, char * argv[]) {
    handle_arguments(argc, argv);

    auto s = slurp(filename);

    load_extension();

    syntax_init();
    syntax_function();
    auto n = syntax_max_memory_requirement(strlen(s));
    auto buffer = malloc(sizeof(char) * n);
    syntax_highlight_string(buffer, s, n);
    syntax_deinit();

    puts(buffer);

    return 0;
}
