#!/bin/env python3

import sys
import pycparser
import pprint
import re

def type_is_enum(item_type, items):
    print("enter type_is_enum, looking for {}".format(item_type))
    for item in items:
        print(item["name"])
        if item["type"] == item_type:
            print("found: {}".format(item))
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
}

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

def generate_c_json_for_children(item, info):
    num_children = len(item["children"])
    for c_idx, c in enumerate(item["children"]):
        if c_idx + 1 < num_children:
            line_end = ","
        else:
            line_end = ""

        # TODO: need to print the name before the array contents

        # handle array case
        array_depth = 0
        array_prefix = ""
        array_suffix = ""
        if "array_len" in c:
            array_len = c["array_len"]
            array_depth = len(array_len)
#            print(array_len)
            if len(array_len) > 1:
                array_prefix = "    "

                array_suffix = ""
                for idx in range(len(array_len)):
                    var_name = "a{}".format(idx)
                    array_suffix += "[{}]".format(var_name)

                    dim_str = get_array_bounds_string(array_len, idx)
                    print("    for (int {0} = 0; {0} < {1}; ++{0}) {{".format(var_name, dim_str))
            elif array_len[0] is None:
                print("    // skipped variable length array named {} of type {}".format(c["name"], c["type"]))
                continue
            else:
                dim_str = get_array_bounds_string(array_len, 0)
                array_prefix = "    "
                array_suffix = "[i]"
                print("    for (int i = 0; i < {}; ++i) {{".format(dim_str))

        printf_var_str = None
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
                # anonymous struct, just print them here without a sub-function
                generate_c_json_for_children(c, info)
            else:
                # sub-struct has associated type, call function to print it
                print(r'    {}printf("    \"{}\": \n");'.format(array_prefix, c["name"]))
                print(r'    {}dump_json_struct_{}(&s->{}{});'.format(array_prefix, c["type"].split("struct ")[1], c["name"], array_suffix))
        elif c["type"].startswith("enum "):
            print(r'    {}printf("    \"{}\": \"%s\"{}\n", enum_{}_to_str({}));'.format(array_prefix, c["name"], line_end, c["type"].split("enum ")[1], c["name"]))
        else:
            printf_var_str = type_to_fmt_str.get(c["type"])
            if printf_var_str is None:
                print("error: unknown type: {}".format(c["type"]))
                assert(0)

        if printf_var_str:
            print(r'    {}printf("    \"{}\": {}{}\n", s->{}{});'.format(array_prefix, c["name"], printf_var_str, line_end, c["name"], array_suffix))

        for i in range(array_depth):
            print("    }")

def generate_c_cases(item, info):
    for name, numeric in item["values"]:
        print(r'    case {}:'.format(name))
        print(r'        return "{}";'.format(name))

    print(r'    default:')
    print(r'        return "unknown";')

def generate_c_json_prints(info):
    for item in info:
        if item["type"].startswith("struct "):
#            print("NATE_DBG: {}".format(item))
            print(r"void dump_json_struct_{}({} *s)".format(item["type"].split("struct ")[1], item["type"]))
            print(r"{")
            print(r'    printf("{\n");')
            generate_c_json_for_children(item, info)
            print(r'    printf("}\n");')
            print(r"}")
        elif item["type"].startswith("enum "):
            print(r"const char *enum_{}_to_str({} e)".format(item["type"].split("enum ")[1], item["type"]))
            print(r"{")
            print(r'    switch (e) {')
            generate_c_cases(item, info)
            print(r'    }')
            print(r"}")

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

    print("typedecl-begin")
    if isinstance(x.type, pycparser.c_ast.IdentifierType):
        print("typedecl-identifier-begin")
        print(dir(x))
        print(dir(x.type))
        child = {
            "type": " ".join(x.type.names),
        }
        print("typedecl-identifier-end")
    elif isinstance(x.type, pycparser.c_ast.Enum):
        print("typedecl-enum-begin")
        print(dir(x))
        print(dir(x.type))
        child = {
            "type": "enum " + x.type.name,
        }
        print("typedecl-enum-end")
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

    print("begin")
    print(dir(ast))
    for x in ast.decls:
        print("decl-begin")
        print(x)
        child = None
        if isinstance(x, pycparser.c_ast.Decl):
            child = gen_decl(x)
        else:
            assert(0)

        if child:
            r["children"].append(child)
        print("decl-end")
    print("end")

    return r

def gen_array_decl(x):
    s = None

    orig = x

    dimensions = [x.dim]

    while isinstance(x.type, pycparser.c_ast.ArrayDecl):
        dimensions.append(x.type.dim)
        x = x.type

    if isinstance(x.type, pycparser.c_ast.TypeDecl):
        print("begin typedecl")
        s = gen_type_decl(x.type)
        if s is not None:
            s["array_len"] = dimensions
        print(s)
        print("end typedecl")
    else:
        print("don't know how to handle array decl: {}".format(x))
        assert(0)

    return s

def gen_decl(x):
    s = None

    if isinstance(x.type, pycparser.c_ast.Struct):
        print("begin struct")
        print(x.type)
        s = gen_struct(x.type)
        print("end struct")
    elif isinstance(x.type, pycparser.c_ast.Enum):
        print("begin enum")
        print(x.type)
        s = gen_enum(x.type)
        print(s)
        print("end enum")
    elif isinstance(x.type, pycparser.c_ast.FuncDecl):
        print("skipping function declaration: {}".format(x))
    elif isinstance(x.type, pycparser.c_ast.TypeDecl):
        print("begin typedecl")
        s = gen_type_decl(x.type)
        print(s)
        print("end typedecl")
    elif isinstance(x.type, pycparser.c_ast.ArrayDecl):
        print("begin arraydecl")
        s = gen_array_decl(x.type)
        print(s)
        print("end arraydecl")
    else:
        print("don't know how to handle decl: {}".format(x))
        assert(0)

    if s is not None:
        s["name"] = x.name

    return s

def main():
    ast = pycparser.parse_file(sys.argv[1])

    result = []

    for x in ast:
        print("begin loop")
        print(x)
        if isinstance(x, pycparser.c_ast.FuncDef):
            continue
        elif isinstance(x, pycparser.c_ast.Decl):
            s = gen_decl(x)
            if s is not None:
                result.append(s)
        elif isinstance(x, pycparser.c_ast.Typedef):
            print("skipping typedef {}".format(x.name))
        else:
            print("don't know how to handle: {}".format(x))
            assert(0)
        print("end loop")

    pp = pprint.PrettyPrinter()
    pp.pprint(result)

#    generate_c_json_func_decl_prints(result)
    generate_c_json_prints(result)

if __name__ == '__main__':
    main()
