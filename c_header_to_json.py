#!/bin/env python3
#
# Copyright (c) 2025 Nathaniel Houghton <nathan@brainwerk.org>
#
# Permission to use, copy, modify, and distribute this software for
# any purpose with or without fee is hereby granted, provided that
# the above copyright notice and this permission notice appear in all
# copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
# WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
# AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
# DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
# OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
# TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
#

import sys
import pycparser
import pprint
import re

c_indent_level = 0
json_indent_level = 0

import sys

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def type_is_enum(item_type, items):
    eprint("enter type_is_enum, looking for {}".format(item_type))
    for item in items:
        eprint(item["name"])
        if item["type"] == item_type:
            eprint("found: {}".format(item))
            assert(0)
            return True

    return False

stdint_type_re = re.compile(r"(u?)int([0-9]+)_t")
type_to_fmt_str = {
    "int": "%d",
    "char": "%c",
    "signed": "%d",
    "unsigned": "%u",
    "unsigned long": "%lu",
    "unsigned long long": "%llu",
    "long long": "%lld",
    "long": "%ld",
    "_Bool": "%d",
}

def fmt_type_is_string(type_str):
    return type_str in ("char",)

# which_dim is which dimension is being queried (multi_dim[0][1][2][3] <-- the
# value shown here is which_dim)
def get_array_bounds_string(arr_info, which_dim):
    dim = arr_info[which_dim]

    if isinstance(dim, pycparser.c_ast.Constant):
        return "{}".format(dim.value)
    elif isinstance(dim, pycparser.c_ast.ID):
        return "{}".format(dim.name)
    elif isinstance(dim, pycparser.c_ast.BinaryOp):
        left = right = None
        if isinstance(dim.left, pycparser.c_ast.Constant):
            left = str(dim.left.value)
        elif isinstance(dim.left, pycparser.c_ast.ID):
            left = str(dim.left.name)

        if isinstance(dim.right, pycparser.c_ast.Constant):
            right = str(dim.right.value)
        elif isinstance(dim.right, pycparser.c_ast.ID):
            right = str(dim.right.name)

        assert(left is not None)
        assert(right is not None)

        # sanity check, just because I'm not sure what all is possible here
        assert(dim.op == '*' or dim.op == '+' or dim.op == '/' or dim.op == '-')

        return "({} {} {})".format(left, dim.op, right)
    else:
        assert(0)

def generate_c_json_for_children(item, info, var_path, print_braces=True, always_print_comma=False):
    global c_indent_level, json_indent_level

    if print_braces:
        # print the struct tag, unless it is anonymous, in which case we print out the name
        print_name = item["type"].split("struct ")[1]
        if print_name == "":
            print_name = item["name"]
        print(r'{}i_printf(indent_level + {}, "\"{}\": {{\n");'.format("    " * c_indent_level, json_indent_level, print_name))
        json_indent_level += 1

    num_children = len(item["children"])
    for c_idx, c in enumerate(item["children"]):
        final_item = (c_idx + 1 >= num_children)

        if always_print_comma or not final_item:
            line_end = ","
        else:
            line_end = ""

        # handle array case
        array_depth = 0
        array_suffix = ""
        if "array_len" in c:
            array_len = c["array_len"]
            array_depth = len(array_len)
            if len(array_len) > 1:
                array_suffix = ""
                print(r'{}i_printf(indent_level + {}, "\"{}\": ");'.format("    " * c_indent_level, json_indent_level, c["name"]))
                for idx in range(array_depth):
                    var_name = "a{}".format(idx)
                    array_suffix += "[{}]".format(var_name)

                    dim_str = get_array_bounds_string(array_len, idx)
                    print(r'{}i_printf(indent_level + {}, "[");'.format("    " * c_indent_level, json_indent_level))
                    print("{0}for (int {1} = 0; {1} < {2}; ++{1}) {{".format("    " * c_indent_level, var_name, dim_str))
                    c_indent_level += 1
                    print(r'{}if ({} != 0) {{'.format("    " * c_indent_level, var_name))
                    c_indent_level += 1
                    if idx + 1 == array_depth:
                        print(r'{}i_printf(indent_level + {}, ", ");'.format("    " * c_indent_level, json_indent_level))
                    else:
                        print(r'{}i_printf(indent_level + {}, ",\n");'.format("    " * c_indent_level, json_indent_level))
                    c_indent_level -= 1
                    print(r'{}}}'.format("    " * c_indent_level))
            elif array_len[0] is None:
                print("{}// skipped variable length array named {} of type {}".format("    " * c_indent_level, c["name"], c["type"]))
                continue
            else:
                dim_str = get_array_bounds_string(array_len, 0)
                var_name = "i"
                array_suffix = "[{}]".format(var_name)
                print(r'{}i_printf(indent_level + {}, "\"{}\": [");'.format("    " * c_indent_level, json_indent_level, c["name"]))
                #json_indent_level += 1
                print("{}for (int i = 0; i < {}; ++i) {{".format("    " * c_indent_level, dim_str))
                c_indent_level += 1
                print(r'{}if ({} != 0) {{'.format("    " * c_indent_level, var_name))
                c_indent_level += 1
                print(r'{}i_printf(indent_level + {}, ", ");'.format("    " * c_indent_level, json_indent_level))
                c_indent_level -= 1
                print(r'{}}}'.format("    " * c_indent_level))

        printf_var_str = None
        printf_var_is_string = True
        m = stdint_type_re.match(c["type"])
        if m:
            if m.group(1) == "":
                var_sgn = "i"
            elif m.group(1) == "u":
                var_sgn = "u"
            var_sz = m.group(2)
            printf_var_str = '%" PRI' + var_sgn + var_sz + ' "'
        elif c["type"].startswith("struct "):
            if c["name"] is None:
                if c["type"] == "struct ":
                    # anonymous struct (may or may not be tagged)
                    generate_c_json_for_children(c, info, var_path, print_braces=False, always_print_comma=not final_item)
                else:
                    # definition of a struct, but one is not declared
                    print("{}// skipped definition without declaration (type: {})".format("    " * c_indent_level, c["type"]))
            elif c["type"] == "struct ":
                # not-anonymous but untagged struct since it is not tagged we
                # can't create a function to call, but we can print it out with
                # the name prefix.
                generate_c_json_for_children(c, info, var_path + c["name"] + ".")
                print(r'{}i_printf(indent_level + {}, "{}\n");'.format("    " * c_indent_level, json_indent_level, line_end))
            else:
                # sub-struct has associated type, call function to print it
                print(r'{}dump_json_struct_{}(indent_level + {}, &{}{}{});'.format("    " * c_indent_level, c["type"].split("struct ")[1], json_indent_level, var_path, c["name"], array_suffix))
                print(r'{}i_printf(indent_level + {}, "{}\n");'.format("    " * c_indent_level, json_indent_level, line_end))
        elif c["type"].startswith("enum "):
            print(r'{}i_printf(indent_level + {}, "\"{}\": \"%s\"{}\n", enum_{}_to_str({}{}));'.format(
                "    " * c_indent_level, json_indent_level, c["name"], line_end, c["type"].split("enum ")[1], var_path, c["name"]))
        else:
            printf_var_str = type_to_fmt_str.get(c["type"])
            if printf_var_str is None:
                eprint("error: unknown type: {}".format(c["type"]))
                assert(0)
            if fmt_type_is_string(c["type"]):
                printf_var_str = r'\"' + printf_var_str + r'\"'

        if printf_var_str:
            if array_depth:
                print(r'{}i_printf(indent_level + {}, "{}", {}{}{});'.format("    " * c_indent_level, json_indent_level, printf_var_str, var_path, c["name"], array_suffix))
            else:
                print(r'{}i_printf(indent_level + {}, "\"{}\": {}{}\n", {}{}{});'.format("    " * c_indent_level, json_indent_level, c["name"], printf_var_str, line_end, var_path, c["name"], array_suffix))

        for i in range(array_depth):
            assert(c_indent_level > 0)
            c_indent_level -= 1
            # end of for loop
            print("{}}}".format("    " * c_indent_level))
            if i + 1 < array_depth:
                special_line_end = ""
            else:
                special_line_end = line_end + r'\n'
            print(r'{}i_printf(indent_level + {}, "]{}");'.format("    " * c_indent_level, json_indent_level, special_line_end))

    if print_braces:
        assert(json_indent_level > 0)
        json_indent_level -= 1
        print(r'{}i_printf(indent_level + {}, "}}");'.format("    " * c_indent_level, json_indent_level))

def generate_c_cases(item, info):
    for name, numeric in item["values"]:
        print(r'    case {}:'.format(name))
        print(r'        return "{}";'.format(name))

    print(r'    default:')
    print(r'        return "unknown";')

def get_structs_to_generate(info, discovered_structs):
    structs_to_gen = []

    for item in info:
        if item["type"].startswith("struct "):
            # Can only generate functions tagged structs
            if item["type"] == "struct ":
                continue

            # previously discovered type
            if item["type"] in discovered_structs:
                continue

            discovered_structs.add(item["type"])
            children_structs = get_structs_to_generate(item["children"], discovered_structs)
            structs_to_gen.extend(children_structs)
            structs_to_gen.append(item)

    return structs_to_gen

def get_enums_to_generate(info):
    enums_to_gen = []

    for item in info:
        if not item["type"].startswith("enum "):
            continue

        enums_to_gen.append(item)

    return enums_to_gen

def generate_c_json_prints(info):
    global c_indent_level, json_indent_level

    discovered_structs = set()
    structs_to_process = get_structs_to_generate(info, discovered_structs)
    enums_to_process = get_enums_to_generate(info)

    # TODO: every function needs to know whether they are the first item in the
    # current level or not.
    for item in enums_to_process:
        enum_name = item["type"].split("enum ")[1]
        print(r"const char *enum_{}_to_str({} e)".format(enum_name, item["type"]))
        print(r"{")
        print(r'    switch (e) {')
        generate_c_cases(item, info)
        print(r'    }')
        print(r"}")

    for item in structs_to_process:
        struct_name = item["type"].split("struct ")[1]
        print(r"{}void dump_json_struct_{}(uint32_t indent_level, {} *s)".format("    " * c_indent_level, struct_name, item["type"]))
        print(r"{}{{".format("    " * c_indent_level))
        c_indent_level += 1
        generate_c_json_for_children(item, info, "s->")
        c_indent_level -= 1
        print(r"{}}}".format("    " * c_indent_level))

def gen_enum(ast):
    r = {
        "type": "enum {}".format(ast.name),
    }

    values = []
    next_value = 0

    for x in ast.values:
        value = x.value
        if value is None:
            value = next_value
            next_value += 1
        elif isinstance(value, pycparser.c_ast.Constant):
            value = value.value
        else:
            assert(0)

        values.append((x.name, value))

    r["values"] = values

    return r

def gen_type_decl(x):
    child = None

    eprint("typedecl-begin")
    if isinstance(x.type, pycparser.c_ast.IdentifierType):
        eprint("typedecl-identifier-begin")
        eprint(dir(x))
        eprint(dir(x.type))
        child = {
            "type": " ".join(x.type.names),
        }
        eprint("typedecl-identifier-end")
    elif isinstance(x.type, pycparser.c_ast.Enum):
        eprint("typedecl-enum-begin")
        eprint(dir(x))
        eprint(dir(x.type))
        child = {
            "type": "enum " + x.type.name,
        }
        eprint("typedecl-enum-end")
    elif isinstance(x.type, pycparser.c_ast.Struct):
        child = gen_struct(x.type)
    else:
        assert(0)

    return child

def gen_struct(ast):
    r = {
        "type": "struct {}".format(ast.name),
        "children": []
    }

    # special case there the name is None for anonymous structs
    if ast.name is None:
        r["type"] = "struct "

    if ast.decls is None:
        return r

    eprint("begin")
    eprint(dir(ast))
    for x in ast.decls:
        eprint("decl-begin")
        eprint(x)
        child = None
        if isinstance(x, pycparser.c_ast.Decl):
            child = gen_decl(x)
        else:
            assert(0)

        if child:
            r["children"].append(child)
        eprint("decl-end")
    eprint("end")

    return r

def gen_array_decl(x):
    s = None

    orig = x

    dimensions = [x.dim]

    while isinstance(x.type, pycparser.c_ast.ArrayDecl):
        dimensions.append(x.type.dim)
        x = x.type

    if isinstance(x.type, pycparser.c_ast.TypeDecl):
        eprint("begin typedecl")
        s = gen_type_decl(x.type)
        if s is not None:
            s["array_len"] = dimensions
        eprint(s)
        eprint("end typedecl")
    else:
        eprint("don't know how to handle array decl: {}".format(x))
        assert(0)

    return s

def gen_decl(x):
    s = None

    if isinstance(x.type, pycparser.c_ast.Struct):
        eprint("begin struct")
        eprint(x.type)
        s = gen_struct(x.type)
        eprint("end struct")
    elif isinstance(x.type, pycparser.c_ast.Enum):
        eprint("begin enum")
        eprint(x.type)
        s = gen_enum(x.type)
        eprint(s)
        eprint("end enum")
    elif isinstance(x.type, pycparser.c_ast.FuncDecl):
        eprint("skipping function declaration: {}".format(x))
    elif isinstance(x.type, pycparser.c_ast.TypeDecl):
        eprint("begin typedecl")
        s = gen_type_decl(x.type)
        eprint(s)
        eprint("end typedecl")
    elif isinstance(x.type, pycparser.c_ast.ArrayDecl):
        eprint("begin arraydecl")
        s = gen_array_decl(x.type)
        eprint(s)
        eprint("end arraydecl")
    else:
        eprint("don't know how to handle decl: {}".format(x))
        assert(0)

    if s is not None:
        s["name"] = x.name

    return s

def main():
    ast = pycparser.parse_file(sys.argv[1])

    result = []

    for x in ast:
        eprint("begin loop")
        eprint(x)
        if isinstance(x, pycparser.c_ast.FuncDef):
            continue
        elif isinstance(x, pycparser.c_ast.Decl):
            s = gen_decl(x)
            if s is not None:
                result.append(s)
        elif isinstance(x, pycparser.c_ast.Typedef):
            eprint("skipping typedef {}".format(x.name))
        else:
            eprint("don't know how to handle: {}".format(x))
            assert(0)
        eprint("end loop")

    pp = pprint.PrettyPrinter(stream=sys.stderr)
    pp.pprint(result)

    generate_c_json_prints(result)

if __name__ == '__main__':
    main()
