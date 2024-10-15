#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/pmargp.h"

int main(int argc, char *argv[]) {
    struct parser_va parser;
    parser_start(&parser);

    // Set program name and description
    parser.name = "example_program";
    parser.description = "A simple program to demonstrate the use of the argument parser.";

    // Add arguments
    char *name;
    int count = 1;
    float value;
    bool quiet = false;
    FILE *output = stdout;
    char character;

    parser.add_argument(&parser, "-n", "--name", PMARGP_STRING, &name, "Your name", true);
    parser.add_argument(&parser, "-c", "--count", PMARGP_INT, &count, "Number of greetings", false);
    parser.add_argument(&parser, "-v", "--value", PMARGP_FLOAT, &value, "A floating-point value", false);
    parser.add_argument(&parser, "-q", "--quiet", PMARGP_BOOL, &quiet, "Run in quiet mode", false);
    parser.add_argument(&parser, "-o", "--output", PMARGP_W_FILE, &output, "Output file", false);
    parser.add_argument(&parser, "-r", "--character", PMARGP_CHAR, &character, "random character", false);


    // Parse arguments
    if (!parser.parses(&parser, argc, argv)) {
        fprintf(stderr, "Error parsing arguments\n");
        free_parser(&parser);
        return 1;
    }

    // Retrieve and use the parsed values
    pmargp_argument_t *name_arg = parser.get_argument(&parser, "-n");
    pmargp_argument_t *count_arg = parser.get_argument(&parser, "-c");
    pmargp_argument_t *value_arg = parser.get_argument(&parser, "--value");
    pmargp_argument_t *quiet_arg = parser.get_argument(&parser, "--quiet");
    pmargp_argument_t *output_arg = parser.get_argument(&parser, "--output");
    pmargp_argument_t *char_arg = parser.get_argument(&parser, "--character");

    printf("Address of name:   %p\n", (void*)&name);
    printf("Address of count:  %p\n", (void*)&count);
    printf("Address of value:  %p\n", (void*)&value);
    printf("Address of quiet:  %p\n", (void*)&quiet);
    printf("Address of output: %p\n", (void*)&output);
    printf("Address of character: %p\n", (void*)&character);

    assert(&name == name_arg->value_ptr);
    assert(&count == count_arg->value_ptr);
    assert(&value == value_arg->value_ptr);
    assert(&quiet == quiet_arg->value_ptr);
    assert(&output == output_arg->value_ptr);
    assert(&character == char_arg->value_ptr);

    printf("quite: %d\n", quiet);
    printf("count: %d\n", count);

    // Use the parsed values
    if (!quiet) {
        for (int i = 0; i < count; i++) {
            fprintf(output, "Hello, %s! The value is %f %c\n", name, value, character);
        }
    }

    // Clean up
    if (output != stdout) {
        fclose(output);
    }

    free_parser(&parser);

    return 0;
}