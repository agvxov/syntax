// @BAKE cc -o $*.out $@ -lcriterion -std=c23 -Wall -Wpedantic -ggdb
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>
#include <stdint.h>

#define SYNTAX_IMPLEMENTATION 
#include "syntax.h"

/* ANSI color sequences. */
#define ANSI_RED    "\x1b[31m"
#define ANSI_GREEN  "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE   "\x1b[34m"
#define ANSI_MAG    "\x1b[35m"
#define ANSI_CYAN   "\x1b[36m"
#define ANSI_RST    "\x1b[0m"

/* IRC color/control sequences. */
#define IRC_RED  "\x03" "04"
#define IRC_BLUE "\x03" "12"
#define IRC_RST  "\x03"

[[maybe_unused]] static const char * numbers = "0123456789";

[[maybe_unused]] static const char * kw_c[]    = {"int", "break", "char", "if", NULL};
[[maybe_unused]] static const char * kw_py[]   = {"def", "return", "None", "print", NULL};
[[maybe_unused]] static const char * kw_perl[] = {"my", "sub", "say", NULL};
[[maybe_unused]] static const char * kw_m4[]   = {"define", "ifdef", "dnl", NULL};
[[maybe_unused]] static const char * kw_tcl[]  = {"proc", "set", "puts", NULL};

static
void configure_common_syntax(void) {
    cr_assert_eq(syntax_init(), 0);

    cr_assert_eq(syntax_define_chars(numbers, ANSI_CYAN, ANSI_RST), 0);

    cr_assert_eq(syntax_define_keywords(kw_c, ANSI_RED, ANSI_RST), 0);
    cr_assert_eq(syntax_define_keywords(kw_py, ANSI_GREEN, ANSI_RST), 0);
    cr_assert_eq(syntax_define_keywords(kw_perl, ANSI_BLUE, ANSI_RST), 0);
    cr_assert_eq(syntax_define_keywords(kw_m4, ANSI_MAG, ANSI_RST), 0);
    cr_assert_eq(syntax_define_keywords(kw_tcl, IRC_RED, IRC_RST), 0);

    cr_assert_eq(syntax_define_region("\"", "\"", "\\", ANSI_YELLOW, ANSI_RST), 0);
    cr_assert_eq(syntax_define_region("'", "'", "\\", ANSI_CYAN, ANSI_RST), 0);
    cr_assert_eq(syntax_define_region("`", "'", NULL, ANSI_GREEN, ANSI_RST), 0);
}

static
void expect_highlight(
  const char * source,
  size_t dst_size,
  const char * expected
) {
    unsigned char buf[256];
    memset(buf, 0xCC, sizeof(buf));

    syntax_highlight_string((char *)buf, source, dst_size);
    cr_assert_str_eq((char *)buf, expected);

    for (size_t i = dst_size; i < sizeof(buf); ++i) {
        cr_assert_eq(buf[i], 0xCC);
    }
}

//-------------------------------------------
//-------------------------------------------
//  ___    _
// | __|__| |__ _ ___ ___ __ __ _ ___ ___ ___
// | _|/ _` / _` / -_)___/ _/ _` (_-</ -_|_-<
// |___\__,_\__, \___|   \__\__,_/__/\___/__/
//          |___/
//-------------------------------------------
//-------------------------------------------
Test(syntax_highlight, empty_source) {
    configure_common_syntax();
    expect_highlight("", 32, "");
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, zero_destination_size_is_noop) {
    configure_common_syntax();

    char guard[] = "keep";
    syntax_highlight_string(guard, "int", 0);
    cr_assert_str_eq(guard, "keep");

    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, c_keyword_prefix_is_not_a_match) {
    configure_common_syntax();
    expect_highlight("integer", 64, "integer");
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, c_keyword_postfix_is_not_a_match) {
    configure_common_syntax();
    expect_highlight("elsif", 64, "elsif");
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, truncated_output_is_terminated_and_has_no_overflow) {
    configure_common_syntax();

    unsigned char buf[16];
    memset(buf, 0xCC, sizeof(buf));
    syntax_highlight_string((char *)buf, "int x;", 8);

    cr_assert_eq(buf[0], '\0');

    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, exact_fit_boundary) {
    configure_common_syntax();

    const char *expected = ANSI_RED "if" ANSI_RST;
    char buf[sizeof(ANSI_RED) + sizeof("if") + sizeof(ANSI_RST) + 1];
    memset(buf, 0xCC, sizeof(buf));

    syntax_highlight_string(buf, "if", sizeof(buf));
    cr_assert_str_eq(buf, expected);

    cr_assert_eq(syntax_deinit(), 0);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//  _  _                    _                            _   _
// | \| |___ _ _ _ __  __ _| |___ ___ _ __  ___ _ _ __ _| |_(_)___ _ _
// | .` / _ \ '_| '  \/ _` | |___/ _ \ '_ \/ -_) '_/ _` |  _| / _ \ ' |
// |_|\_\___/_| |_|_|_\__,_|_|   \___/ .__/\___|_| \__,_|\__|_\___/_||_|
//                                   |_|
//----------------------------------------------------------------------
//----------------------------------------------------------------------
Test(syntax_highlight, c_char_simple) {
    configure_common_syntax();
    expect_highlight("0", 64, ANSI_CYAN "0" ANSI_RST);
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, c_char_multi) {
    configure_common_syntax();
    expect_highlight("8697", 64, ANSI_CYAN "8697" ANSI_RST);
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, c_keyword_simple) {
    configure_common_syntax();
    expect_highlight("int x;", 64, ANSI_RED "int" ANSI_RST " x;");
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, c_mixed_tokens) {
    configure_common_syntax();
    expect_highlight(
        "if(x)break 0;",
        64,
        ANSI_RED "if" ANSI_RST "(" "x" ")" ANSI_RED "break" ANSI_RST " " ANSI_CYAN "0" ANSI_RST ";"
    );
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, c_string_with_escape) {
    configure_common_syntax();
    expect_highlight(
        "char*s=\"a\\\"b\";",
        128,
        ANSI_RED "char" ANSI_RST "*s=" ANSI_YELLOW "\"a\\\"b\"" ANSI_RST ";"
    );
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, unterminated_string_is_safe) {
    configure_common_syntax();
    expect_highlight("\"abc", 64, ANSI_YELLOW "\"abc" ANSI_RST);
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, python_def_and_return) {
    configure_common_syntax();
    expect_highlight(
        "def f():return None",
        128,
        ANSI_GREEN "def" ANSI_RST " f():"
        ANSI_GREEN "return" ANSI_RST " " ANSI_GREEN "None" ANSI_RST
    );
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, python_single_quoted_string) {
    configure_common_syntax();
    expect_highlight("x='y'", 64, "x=" ANSI_CYAN "'y'" ANSI_RST);
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, perl_short_statement) {
    configure_common_syntax();
    expect_highlight("my $x=1;", 64, ANSI_BLUE "my" ANSI_RST " $x=" ANSI_CYAN "1" ANSI_RST ";");
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, perl_print_with_string) {
    configure_common_syntax();
    expect_highlight(
        "say \"ok\";",
        64,
        ANSI_BLUE "say" ANSI_RST " " ANSI_YELLOW "\"ok\"" ANSI_RST ";"
    );
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, m4_define_with_backtick_quote) {
    configure_common_syntax();
    expect_highlight(
        "define(`x',1)",
        64,
        ANSI_MAG "define" ANSI_RST "(" ANSI_GREEN "`x'" ANSI_RST "," ANSI_CYAN "1" ANSI_RST ")"
    );
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, tcl_proc_uses_irc_sequences) {
    configure_common_syntax();
    expect_highlight(
        "proc p {} {puts hi}",
        128,
        IRC_RED "proc" IRC_RST " p {} {" IRC_RED "puts" IRC_RST " hi}"
    );
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, unknown_punctuation_is_preserved) {
    configure_common_syntax();
    expect_highlight("()[]{};", 32, "()[]{};");
    cr_assert_eq(syntax_deinit(), 0);
}

Test(syntax_highlight, adjacent_keywords_and_string) {
    configure_common_syntax();
    expect_highlight(
        "if\"x\"return",
        64,
        ANSI_RED "if" ANSI_RST ANSI_YELLOW "\"x\"" ANSI_RST ANSI_GREEN "return" ANSI_RST
    );
    cr_assert_eq(syntax_deinit(), 0);
}
