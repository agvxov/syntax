//                   _
// __  ___   _ _ __ | |_ __ ___  __
// \ \/ / | | | '_ \| __/ _` \ \/ /
//  >  <| |_| | | | | || (_| |>  <
// /_/\_\\__, |_| |_|\__\__,_/_/\_\
//       |___/
//
// Copyright (c) 1997 - Ognjen 'xolatile' Milan Robovic
//
// xolatile@proton.me - xyntax - Tiny, unsafe and somewhat insane unity header for generic syntax definition.
//
// This program is free software, free as in freedom and as in free beer, you can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version if you wish...
//
// This program is distributed in the hope that it will be useful, but it is probably not, and without any warranty, without even the implied
// warranty of merchantability or fitness for a particular purpose, because it is pointless. Please see the GNU (Geenoo) General Public License
// for more details, if you dare, it is a lot of text that nobody wants to read...

static uint     syntax_count   = 0;
static bool     syntax_active  = false;
static bool   * syntax_enrange = null;
static bool   * syntax_derange = null;
static char * * syntax_begin   = null;
static char * * syntax_end     = null;
static char   * syntax_escape  = null;
static int    * syntax_colour  = null;
static int    * syntax_effect  = null;

static void syntax_memory (void) {
	if (syntax_active == false) {
		return;
	}

	for (uint offset = 0; offset < syntax_count; ++offset) {
		syntax_begin [offset] = deallocate (syntax_begin [offset]);
		syntax_end   [offset] = deallocate (syntax_end   [offset]);
	}

	syntax_enrange = deallocate (syntax_enrange);
	syntax_derange = deallocate (syntax_derange);
	syntax_begin   = deallocate (syntax_begin);
	syntax_end     = deallocate (syntax_end);
	syntax_escape  = deallocate (syntax_escape);
	syntax_colour  = deallocate (syntax_colour);
	syntax_effect  = deallocate (syntax_effect);

	syntax_active = false;
	syntax_count  = 0;
}

static uint syntax_define (bool enrange, bool derange, const char * begin, const char * end, char escape, int colour, int effect) {
	if (syntax_active == false) {
		syntax_active = true;

		clean_up (syntax_memory);
	}

	fatal_failure (begin == null, "syntax_define: Begin string is null pointer.");
	fatal_failure (end   == null, "syntax_define: End string is null pointer.");

	++syntax_count;

	syntax_enrange = reallocate (syntax_enrange, syntax_count * sizeof (* syntax_enrange));
	syntax_derange = reallocate (syntax_derange, syntax_count * sizeof (* syntax_derange));
	syntax_begin   = reallocate (syntax_begin,   syntax_count * sizeof (* syntax_begin));
	syntax_end     = reallocate (syntax_end,     syntax_count * sizeof (* syntax_end));
	syntax_escape  = reallocate (syntax_escape,  syntax_count * sizeof (* syntax_escape));
	syntax_colour  = reallocate (syntax_colour,  syntax_count * sizeof (* syntax_colour));
	syntax_effect  = reallocate (syntax_effect,  syntax_count * sizeof (* syntax_effect));

	syntax_begin [syntax_count - 1] = allocate ((string_length (begin) + 1) * sizeof (* * syntax_begin));
	syntax_end   [syntax_count - 1] = allocate ((string_length (end)   + 1) * sizeof (* * syntax_end));

	syntax_enrange [syntax_count - 1] = enrange;
	syntax_derange [syntax_count - 1] = derange;
	syntax_escape  [syntax_count - 1] = escape;
	syntax_colour  [syntax_count - 1] = colour;
	syntax_effect  [syntax_count - 1] = effect;

	string_copy (syntax_begin [syntax_count - 1], begin);
	string_copy (syntax_end   [syntax_count - 1], end);

	return (syntax_count - 1);
}

static uint syntax_select (const char * string, uint * length) {
	uint offset = 0;
	uint subset = 0;
	uint select = 0;

	for (select = offset = 0; select != syntax_count; ++select) {
		if (! syntax_enrange [select]) {
			if (! syntax_derange [select]) {
				if (string_compare_limit (string, syntax_begin [select], string_length (syntax_begin [select]))) {
					break;
				}
			} else {
				if ((string_compare_limit    (string, syntax_begin [select], string_length (syntax_begin [select])))
				&&  (character_compare_array (string [offset + string_length (syntax_begin [select])], syntax_end [select]))) {
					break;
				}
			}
		} else {
			for (subset = 0; subset != string_length (syntax_begin [select]); ++subset) {
				if (string [offset] == syntax_begin [select] [subset]) {
					goto selected;
				}
			}
		}
	}

	selected:

	if (select >= syntax_count) {
		* length = 1;

		return (syntax_count);
	}

	for (offset = 1; string [offset - 1] != '\0'; ++offset) {
		if (string [offset] == syntax_escape [select]) {
			++offset;
			continue;
		}

		if (syntax_derange [select]) {
			subset = 0;
			if (string_length (syntax_end [select]) == 0) {
				break;
			} do {
				if (string [offset] == syntax_end [select] [subset]) {
					* length = offset;
					goto finished;
				}
			} while (++subset != string_length (syntax_end [select]));
		} else {
			if (string_length (syntax_end [select]) != 0) {
				if (string_compare_limit (& string [offset], syntax_end [select], string_length (syntax_end [select]))) {
					* length = offset + string_length (syntax_end [select]);
					return (select);
				}
			} else {
				* length = 1;
				return (select);
			}
		}
	}

	finished:

	return (select);
}
