#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "util.h"
#include "test1_input.h"
#include "out1.c"

int main(int argc, char **argv)
{
    struct test t = { .c = 'x', .anon_internal_b = 'q', .nested_struct_name_0 = { .internal_struct_b = 'r' }, .nested_struct_name_1 = { .internal_named_struct_b = 'm' } };

    i_printf(0, "{\n");
    dump_json_struct_test(1, &t);
    i_printf(0, "\n}\n");
}
