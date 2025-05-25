/*
 * Copyright (c) 2025 Nathaniel Houghton <nathan@brainwerk.org>
 *
 * Permission to use, copy, modify, and distribute this software for
 * any purpose with or without fee is hereby granted, provided that
 * the above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

#define INDENT_WIDTH 4

bool g_at_col0 = true;

int i_printf(uint32_t indent, const char *restrict format, ...)
{
    size_t len = strlen(format);
    size_t num_newlines = 0;

    for (size_t i = 0; i < len; ++i) {
        if (format[i] == '\n') {
            num_newlines++;
        }
    }

    char *new_fmt = (char *) format;
    bool allocated_new_fmt = false;

    if (num_newlines || g_at_col0) {
        new_fmt = malloc(len + 1 + (num_newlines + 1) * indent * INDENT_WIDTH);
        assert(new_fmt);
        allocated_new_fmt = true;

        size_t d = 0;
        for (size_t s = 0; s < len; ++s) {
            if (g_at_col0) {
                size_t rem_spaces = indent * INDENT_WIDTH;
                while (rem_spaces) {
                    new_fmt[d++] = ' ';
                    rem_spaces--;
                }
            }
            new_fmt[d++] = format[s];

            g_at_col0 = (format[s] == '\n');
        }
        new_fmt[d] = '\0';
    }

    va_list args;
    va_start(args, format);
    int r = vprintf(new_fmt, args);
    va_end(args);

    if (allocated_new_fmt) {
//        printf("old fmt string: \"%s\", new fmt string: \"%s\"\n",
//            format, new_fmt);

        free(new_fmt);
    }

    return r;
}
