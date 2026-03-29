# Syntax

> Simple header-only library for syntax highlighting

### Overview
To work with Syntax, you have to define a syntax first.
There are 3 concepts:
1. Keywords: strings made of keyword characters (redefinable) separated by non-keyword characters
2. Regions: a starter string and a terminating string between which no further matching should be done
3. Chars: special characters should be highlighted regardless of word boundaries

They are matched in the exact order as listed above.

When one of them is matched, they will be surrounded by the corresponding `hl_start` and `hl_end`,
which you are expected to set to your preferred ANSI escapes, IRC color codes, HTML spans, or similar.

> [!NOTE]
> Technical details and compiletime nuisances can be found at the top of `syntax.h`.

### Synopsis
```c
extern int syntax_init(void);
extern int syntax_deinit(void);
extern int syntax_define_chars(const char * chars, const char * hl_start, const char * hl_end);
extern int syntax_define_keywords(const char * * keywords, const char * hl_start, const char * hl_end);
extern int syntax_define_region(const char * start, const char * end, const char * escape, const char * hl_start, const char * hl_end);
extern size_t syntax_max_memory_requirement(size_t input_len);
extern void syntax_highlight_string(char * const destination, const char * const source, const size_t destination_size);
```

### Compatibility
Syntax is C99 header-only library with no dependencies,
which never allocates heap memory.

### HL
This repository also contains the source code for the executable `hl`,
which stands for "highlight" and behaves similar to `cat`,
but with syntax highlighting.
On surface level, it is similar to `bat`, however while `bat` excels at everyday usage,
`hl` is better customizable and is ideal for creating colored screenshots
(possibly using `xeen`).

The `library/` folder only exists to accommodate the dependencies of `hl`.

### History
In 2023, a collection of IRC chuds agreed to program a group project together.
They collected simple ideas that they all found agreeable,
then a robot drew one at random, and landed on the keyword "highlighting".
Which was puzzling, because that was **not** one of the topics they elected.
As it turned out, the mutt in charge of managing the ideas-lot
sneaked in random words "such that the list would look bigger and better".
However, the machinespirit's will is holy and work began shortly.
Said work could be best described as "spastic".
Communication turned into mumbling,
we smoke the toenails and hair of the wise man,
the agents of oblivion descended on the sane,
we played death games in the rain,
they whispered mutiny and we cut their throats while they slept,
dreaming and digesting shards of turtle shells.
Those who lived were cursed.
One man was destined to only ever write Makefiles,
the other to never quite finish a project.
As for myself, I am forced to death-spiral with subcomponents
of the things I actually want to make,
until the day of my inevitable stack overflow,
when I will surely burst like a human jellyfish.
Still, I am more fortunate than the others,
because atleast I can ship -for example this library-
during my short and forsaken existence.

It is also notable that the Serbian man who will
never ever finish anything under any circumstance,
has produced a demo called `xyntax` under a few hours,
which is the conceptual basis of Syntax.
3 years of death-spiraling later I am ready to recognize
that his methodology covers 90% of use-cases
with 10% of the complexity of alternatives
while still being fast enough.
