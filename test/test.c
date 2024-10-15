#include "../src/pmargp.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef bool (*TestFunction)();

char version[] = "0.1.0"; 

bool run_test(const char *test_name, TestFunction fn) {
    bool result = fn();
    printf("\r%s Complete... %s: %s\n", result ? "✅" : "❌",  test_name, result ? "PASSED" : "FAILED");
    return result;
}

bool run_test_group(const char *group_name, TestFunction *tests, const char **test_names, size_t count) {
    printf("\n=== Running Test Group: %s ===\n", group_name);
    bool result = true;
    for (size_t i = 0; i < count; ++i) {
        bool test_result = run_test(test_names[i], tests[i]);
        if (test_result == false){
            result = test_result;
        }
    }
    printf("=============================\n");
    return result;
}

bool test_basic_parsing() {
    struct parser_va parser;
    parser_start(&parser);


    int _int;
    float _float;
    char * _str;
    bool _bool;
    parser.add_argument(&parser, "-s", "--string", PMARGP_STRING, &_str, "String argument", false);
    parser.add_argument(&parser, "-i", "--int", PMARGP_INT, &_int, "Integer argument", false);
    parser.add_argument(&parser, "-f", "--float", PMARGP_FLOAT, &_float, "Float argument", false);
    parser.add_argument(&parser, "-b", "--bool", PMARGP_BOOL, &_bool, "Boolean argument", false);

    char *argv[] = {"program", "--string", "hello", "--int", "42", "--float", "3.14", "--bool"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = parser.parses(&parser, argc, argv);

    bool correct_output = _int == 42 && fabs(_float - 3.14) < 0.000001 && strncmp(_str, "hello", 6) == 0 && _bool;
    free_parser(&parser);
    return result && correct_output;
}

bool test_missing_required_arguments() {
    struct parser_va parser;
    parser_start(&parser);

    // Add required and optional arguments
    int i;
    char *s;
    parser.add_argument(&parser, "-r", "--required", PMARGP_INT, &i, "Required integer argument", true);
    parser.add_argument(&parser, "-o", "--optional", PMARGP_STRING, &s, "Optional string argument", false);

    // Simulate command-line input without the required argument
    char *argv[] = {"program", "--optional", "test"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = parser.parses(&parser, argc, argv) == PMARGP_ERR_ARG_MISSING;
    free_parser(&parser);
    // Expect false since a required argument is missing
    return result;
}


bool test_default_values() {

    struct parser_va parser;
    parser_start(&parser);

    int default_int = 10;
    parser.add_argument(&parser, "-i", "--int", PMARGP_INT, &default_int, "Integer argument with default", false);

    // Simulate command-line input without providing the argument
    char *argv[] = {"program"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = parser.parses(&parser, argc, argv);

    pmargp_argument_t *_int_arg = parser.get_argument(&parser, "-i");

    // Check if the default value is correctly set
    
    bool default_set = default_int == *((int *)_int_arg->value_ptr);
    free_parser(&parser);

    return result && default_set;
}



bool test_argument_overlap() {
    struct parser_va parser;
    parser_start(&parser);

    int default_num = 69;
    parser.add_argument(&parser, "-n", "--number", PMARGP_INT, &default_num, "Number argument", false);

    // Simulate command-line input with both short and long keys
    char *argv[] = {"program", "-n", "42", "--number", "100"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = parser.parses(&parser, argc, argv);

    int value = *((int *)parser.get_argument(&parser, "--number")->value_ptr);
    bool default_set = value == 42;

    free_parser(&parser);

    // The test should ideally fail as both short and long keys are provided, which is ambiguous
    return result && default_set;
}

bool test_multiple_strings() {
    struct parser_va parser;
    parser_start(&parser);

    char *arg1;
    char *arg2;
    parser.add_argument(&parser, "-a", "--arg1", PMARGP_STRING, &arg1, "First string argument", false);
    parser.add_argument(&parser, "-b", "--arg2", PMARGP_STRING, &arg2, "Second string argument", false);

    char *argv[] = {"program", "--arg1", "hello", "--arg2", "world"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = parser.parses(&parser, argc, argv);

    bool correct_output = strncmp(arg1, "hello", 6) == 0 && strncmp(arg2, "world", 6) == 0;
    free_parser(&parser);

    return result && correct_output;
}

bool test_boolean_flags() {
    struct parser_va parser;
    parser_start(&parser);

    bool b = false;
    parser.add_argument(&parser, "-f", "--flag", PMARGP_BOOL, &b, "Boolean flag", false);

    char *argv[] = {"program"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = parser.parses(&parser, argc, argv);

    free_parser(&parser);

    return result && !b; // Expect the flag to be not set
}


bool test_missing_optional_arguments() {
    struct parser_va parser;
    parser_start(&parser);
    char *o = "";
    parser.add_argument(&parser, "-o", "--optional", PMARGP_STRING, &o, "Optional argument", false);

    char *argv[] = {"program"};
    bool result = parser.parses(&parser, 1, argv);
    bool is_null = strlen(o) == 0;
    free_parser(&parser);
    return result && is_null; // Expect the optional argument to be null
}


bool test_invalid_argument() {
    struct parser_va parser;
    parser_start(&parser);
    // No arguments are added to the parser

    char *argv[] = {"program", "--invalid", "value"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = parser.parses(&parser, argc, argv);
    free_parser(&parser);

    return !result; // Expect the result to be false due to an invalid argument
}


bool test_unrecognized_short_key() {
    struct parser_va parser;
    parser_start(&parser);

    char *arg1;
    // Add some arguments
    parser.add_argument(&parser, "-a", "--arg", PMARGP_STRING, &arg1, "Test argument", true);

    char *argv[] = {"program", "-x", "value"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = parser.parses(&parser, argc, argv) == PMARGP_ERR_ARG_MISSING;
    free_parser(&parser);

    return result; // Expect the result to be false due to unrecognized short key
}


bool test_argument_aliases() {
    struct parser_va parser;
    parser_start(&parser);

    // Add an argument with an alias
    bool b;
    parser.add_argument(&parser, "-v", "--verbose", PMARGP_BOOL, &b, "Verbose output", false);

    char *argv[] = {"program", "-v"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = parser.parses(&parser, argc, argv);


    free_parser(&parser);

    return result && b;
}



int main(int argc, char *argv[]) {
    
    printf("1.Start program\n");
    struct parser_va args;
    parser_start(&args);
    printf("2.Create Parser\n");
    
    printf("3.Add description\n");

    bool name;
    bool all;
    if (args.add_argument(&args, "-a", "--all", PMARGP_BOOL, &all, "boolean type that runs all test(s)", false) == false) return EXIT_FAILURE;
    printf("4.Add argument 1\n");
    if (args.add_argument(&args, "-n", "--names", PMARGP_BOOL, &name, "name of all the test(s)", false) == false) return EXIT_FAILURE;
    printf("5.Add argument 2\n");

    if (args.parses(&args, argc, argv) == false) {
        printf("Failed to parse\n");
        free_parser(&args);
        return EXIT_FAILURE;
    }
    printf("6. parse argument\n");

    // Define test groups
    TestFunction basic_tests[] = {
        test_basic_parsing,
        test_default_values,
        test_multiple_strings,
    };
    const char *basic_test_names[] = {
        "test_basic_parsing",
        "test_default_values",
        "test_multiple_strings",
    };

    TestFunction error_handling_tests[] = {
        test_missing_required_arguments,
        test_invalid_argument,
        test_unrecognized_short_key,
    };
    const char *error_handling_test_names[] = {
        "test_missing_required_arguments",
        "test_invalid_argument",
        "test_unrecognized_short_key",
    };

    TestFunction boolean_tests[] = {
        test_boolean_flags,
        test_missing_optional_arguments,
        test_argument_aliases,
    };
    const char *boolean_test_names[] = {
        "test_boolean_flags",
        "test_missing_optional_arguments",
        "test_argument_aliases",
    };

    // Run tests based on input
    bool result = true;
    printf("7. run\n");
    if (all) {
        result &= run_test_group("Basic Tests", basic_tests, basic_test_names, sizeof(basic_tests) / sizeof(basic_tests[0]));
        result &= run_test_group("Error Handling Tests", error_handling_tests, error_handling_test_names, sizeof(error_handling_tests) / sizeof(error_handling_tests[0]));
        result &= run_test_group("Boolean Flag Tests", boolean_tests, boolean_test_names, sizeof(boolean_tests) / sizeof(boolean_tests[0]));
        
    } else if (name) {
        printf("Available Test Names:\n");
        for (int i = 0; i < (int)(sizeof(basic_test_names) / sizeof(basic_test_names[0])); ++i) {
            printf(" - %s\n", basic_test_names[i]);
        }
        for (int i = 0; i < (int)(sizeof(error_handling_test_names) / sizeof(error_handling_test_names[0])); ++i) {
            printf(" - %s\n", error_handling_test_names[i]);
        }
        for (int i = 0; i < (int)(sizeof(boolean_test_names) / sizeof(boolean_test_names[0])); ++i) {
            printf(" - %s\n", boolean_test_names[i]);
        }
    }


    free_parser(&args);
    printf("== END ==\n");
    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}