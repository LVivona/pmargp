# Poor Man's Argument Parser (argp)


![example workflow](https://github.com/LVivona/pmargp/actions/workflows/c-cpp.yml/badge.svg)

## Overview

**Poor Man's Argument Parser** (`argp`) is a simple, lightweight C library for handling command-line arguments. It allows easy parsing of various argument types such as integers, floats, strings, and files. Designed with minimal overhead, it is ideal for small-to-medium-sized C applications that require flexible command-line argument parsing without the need for large dependencies.

## Features

- **Multiple argument types**: Supports integers, floats, strings, characters, and various file types (read, write, binary modes).
- **Short and long arguments**: Handle both short `-o` and long `--output` argument forms.
- **Required and optional arguments**: Enforce required arguments with ease.
- **Memory management**: Automatically handles memory allocation for dynamically parsed arguments.

## Project Structure

```bash
pmargp/
├── bin/               # Compiled binaries and object files
│   ├── pmargp.o
│   ├── example_program
├── example/           # Example usage of the argument parser
│   └── example.c
├── src/               # Source files for the argument parser
│   ├── pmargp.c
│   └── pmargp.h
├── test/              # Unit tests for the argument parser
│   └── test.c
├── Makefile           # Build script
└── README.md          # Project documentation
```

## Getting Started

### Prerequisites

- **C Compiler**: You will need a C compiler such as GCC.
- **Make**: To simplify the build process, a `Makefile` is provided.

### Building the Project

1. Clone the repository:

    ```bash
    git clone https://github.com/LVivona/pmargp.git
    cd pmargp
    ```

2. Build the example and test programs:

    ```bash
    make all
    ```

   This will create the binaries in the `bin/` directory:
   - `example_program`: Example usage of the argument parser.
   - `pmargp.o` object file of pmargp

3. Clean the build artifacts:

    ```bash
    make clean
    ```

### Usage

You can see how to use the argument parser by running the `example_program`:

```bash
./bin/example_program  --name Luca --output file.txt -v 3.14
```

To see the full example, check the `example/example.c` file.

### Example Code

Here’s a simple example showing how to add and parse arguments with the **Poor Man's Argument Parser**:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pmargp.h"

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

    parser.add_argument(&parser, "-n", "--name", STRING, &name, "Your name", true);
    parser.add_argument(&parser, "-c", "--count", INT, &count, "Number of greetings", false);
    parser.add_argument(&parser, "-v", "--value", FLOAT, &value, "A floating-point value", false);
    parser.add_argument(&parser, "-q", "--quiet", BOOL, &quiet, "Run in quiet mode", false);
    parser.add_argument(&parser, "-o", "--output", W_FILE, &output, "Output file", false);
    parser.add_argument(&parser, "-r", "--character", CHAR, &character, "random character", false);


    // Parse arguments
    if (!parser.parses(&parser, argc, argv)) {
        fprintf(stderr, "Error parsing arguments\n");
        free_parser(&parser);
        return 1;
    }

    // Retrieve and use the parsed values
    Argument_t *name_arg = parser.get_argument(&parser, "-n");
    Argument_t *count_arg = parser.get_argument(&parser, "-c");
    Argument_t *value_arg = parser.get_argument(&parser, "--value");
    Argument_t *quiet_arg = parser.get_argument(&parser, "--quiet");
    Argument_t *output_arg = parser.get_argument(&parser, "--output");
    Argument_t *char_arg = parser.get_argument(&parser, "--character");

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
```

```
>>./bin/example_program -h                                       

example_program
A simple program to demonstrate the use of the argument parser.

Usage: example_program [OPTIONS]

Options:
  --name         -n  <string>       Your name (Type: string) [Required][Default: None]
  --count        -c  <integer>      Number of greetings (Type: int)[Default: 1]
  --value        -v  <float>        A floating-point value (Type: float)[Default: 0.00]
  --quiet        -q  <bool>         Run in quiet mode (Type: bool)[Default: false]
  --output       -o  <write_file>   Output file (Type: write file)
  --character    -r  <char>         random character (Type: char)[Default: ]
```

### Tests

Unit tests are provided in the `test/` directory. To run them, simply build and execute:

```bash
make test
```

The test program will validate the functionality of the argument parser, ensuring proper handling of various argument types and cases.

```
1.Start program
2.Create Parser
3.Add description
4.Add argument 1
5.Add argument 2
6. parse argument

=== Running Test Group: Basic Tests ===
✅ Complete... test_basic_parsing: PASSED
✅ Complete... test_default_values: PASSED
✅ Complete... test_multiple_strings: PASSED
=============================

=== Running Test Group: Error Handling Tests ===
Required argument missing: --required
✅ Complete... test_missing_required_arguments: PASSED
✅ Complete... test_invalid_argument: PASSED
Required argument missing: --arg
✅ Complete... test_unrecognized_short_key: PASSED
=============================

=== Running Test Group: Boolean Flag Tests ===
✅ Complete... test_boolean_flags: PASSED
✅ Complete... test_missing_optional_arguments: PASSED
✅ Complete... test_argument_aliases: PASSED
=============================
== END ==
Test passed. Removing test executable.
```

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
