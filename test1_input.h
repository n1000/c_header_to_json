#include <stdint.h>

struct test {
    int a;
    int b;
    char c;
    uint32_t x;
    uint8_t q;
    int64_t q64;
    unsigned long ultest;
//    char *char_ptr;
//    int *int_ptr;
    // this struct has no tag and no name (anonymous, untagged)
    struct {
        int anon_internal_a;
        char anon_internal_b;
    };
    // this struct has a tag but no name (anonymous, tagged)
/*
    struct internal_struct {
        int internal_struct_a;
        char internal_struct_b;
    };
*/
    // this struct has no tag but a name (not anonymous, untagged)
    struct {
        int internal_struct_a;
        char internal_struct_b;
    } nested_struct_name_0;
    // this struct has a tag and name
/*
    struct internal_struct_with_name {
        int internal_named_struct_a;
        char internal_named_struct_b;
    } nested_struct_name_1;
*/
};

struct other_struct {
    uint32_t x;
    uint8_t q;
};
