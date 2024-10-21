#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pmargp.h"

int main(int argc, char *argv[]) {
    struct pmargp_parser_t parser;
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
    FILE *fp = NULL;
    char character;
    int error;
    /**
     * "class" based
     */
    // check if argument is valid argument
    if((error = parser.add_argument(&parser, "-n", NULL, PMARGP_STRING, &name, "Your name", true)) != PMARGP_SUCCESS){
        // this code will never reach here since this argument is valid
        fprintf(stderr, "Error code %d parsing arguments\n", error);
        return EXIT_FAILURE;
    }
    parser.add_argument(&parser, "-c", "--count", PMARGP_INT, &count, "Number of greetings", false);
    parser.add_argument(&parser, "-v", "--value", PMARGP_FLOAT, &value, "A floating-point value", false);

    
    /**
     * "functional" based
     */
    add_argument(&parser, NULL, "--quiet", PMARGP_BOOL, &quiet, "Run in quiet mode", false);
    add_argument(&parser, "-o", "--output", PMARGP_W_FILE, &output, "Output file", false);
    add_argument(&parser, "-f", NULL, PMARGP_R_FILE, &fp, "input file", true);
    add_argument(&parser, "-r", "--character", PMARGP_CHAR, &character, "random character", false);

    // Parse arguments
    if ((error = parser.parses(&parser, argc, argv)) != PMARGP_SUCCESS) {
        fprintf(stderr, "Error code %d parsing arguments\n", error);
        free_parser(&parser);
        return EXIT_FAILURE;
    }

    /* 
     * Retrieve and use the parsed values ptr
     * but this isn't not needed since where pointing to 
     * stack variables we can just use those.
     */
    pmargp_argument_t *name_arg = parser.get_argument(&parser, "-n");
    pmargp_argument_t *count_arg = parser.get_argument(&parser, "-c");
    pmargp_argument_t *value_arg = parser.get_argument(&parser, "--value");
    pmargp_argument_t *quiet_arg = parser.get_argument(&parser, "--quiet");
    pmargp_argument_t *output_arg = parser.get_argument(&parser, "--output");
    pmargp_argument_t *char_arg = parser.get_argument(&parser, "--character");
    pmargp_argument_t *file_arg = parser.get_argument(&parser, "-f");


    /**
     * since this is allocated within the stack it's easy fast access memory
     * then if we where to allocate this on the heap.
     *  ----------------
     * |     stack      |
     * |----------------|
     * |  0xffffff168   |
     * |----------------|
     * |  0xffffff164   |
     * |----------------|
     * |      ...       |
     *  ----------------
     */
    printf("Address of name:   %p\n", (void*)&name);
    printf("Address of count:  %p\n", (void*)&count);
    printf("Address of value:  %p\n", (void*)&value);
    printf("Address of quiet:  %p\n", (void*)&quiet);
    printf("Address of output: %p\n", (void*)&output);
    printf("Address of character: %p\n", (void*)&character);
    printf("Address of file pointer: %p\n", (void*)&fp);

    /**
     * this just assures that the value ptr is pointing to the stack
     * argument.
     */
    assert(&name == name_arg->value_ptr);
    assert(&count == count_arg->value_ptr);
    assert(&value == value_arg->value_ptr);
    assert(&quiet == quiet_arg->value_ptr);
    assert(&output == output_arg->value_ptr);
    assert(&character == char_arg->value_ptr);
    assert(&fp == file_arg->value_ptr);
    

    printf("quite: %d\n", quiet);
    printf("count: %d\n", count);

    // Use the parsed values
    if (!quiet) {
        if (-1 == (long int)count){
             fprintf(output, "I can't print that much\n");
        } else {
            for (int i = 0; i < count; i++) {
                fprintf(output, "Hello, %s! The value is %f %c\n", name, value, character);
            }
        }
    }

    if (fp != NULL){
        printf("Bee Movie\n\n");
        char * line;
        size_t len;
        int i = 0;
        while ((line = fgetln(fp, &len)) && i < 10){
            fwrite(line, len, 1, output);
            i++;
        }
            
    }


    // Clean up
    if (output != stdout) {
        fclose(output);
    }

    if (fp != NULL){
        fclose(fp);
    }

    free_parser(&parser);

    return 0;
}