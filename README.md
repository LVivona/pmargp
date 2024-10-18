# Poor Man's Argument Parser (pmargp)

<p align="center">
  <img src="/.github/assets/header.png">
</p>




![Build Status](https://github.com/LVivona/pmargp/actions/workflows/c-cpp.yml/badge.svg)

## Overview

**Poor Man's Argument Parser** (`pmargp`) is a minimalist, lightweight C library designed for effortless command-line argument parsing. It supports various argument types including integers, floats, strings, and file modes. `pmargp` is built to introduce minimal overhead, making it an excellent choice for small-to-medium-sized C applications that need flexible argument parsing without the complexity of large libraries.

## Features

- **Supports multiple argument types**: Handles integers, floats, strings, characters, and files (with read, write, and binary modes).
- **Short and long arguments**: Supports short form (`-o`) and long form (`--output`) argument types.
- **Required and optional arguments**: Specify mandatory arguments easily.
- **Automated memory management**: Automatically manages memory for dynamically parsed arguments.

## Project Structure

```bash
pmargp/
├── bin/               # Compiled binaries and object files
│   ├── pmargp.o
│   ├── example_program
├── lib/               # Compiled binaries and object files
│   └── libpmargp.a
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

- **C Compiler**: Ensure you have a C compiler (e.g., GCC).
- **Make**: A `Makefile` is provided to simplify the build process.

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

   This command compiles the following binaries in the `bin/` directory:
   - `example_program`: Demonstrates how to use the argument parser.
   - `pmargp.o`: The compiled object file of the `pmargp` library.

3. Clean the build artifacts:

    ```bash
    make clean
    ```

### Usage

To see the argument parser in action, run the `example_program`:

```bash
./bin/example_program --name Luca --output file.txt -v 3.14
```

Refer to the `example/example.c` file for a detailed usage example.

### Example Code

Below is a simplified example that shows how to configure and parse arguments using **Poor Man's Argument Parser**:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pmargp.h"

int main(int argc, char *argv[]) {
    struct pmargp_parser_t parser;
    parser_start(&parser);

    // Set program details
    parser.name = "example_program";
    parser.description = "A simple program demonstrating argument parsing.";

    // Declare arguments
    char *name;
    int count = 1;
    float value;
    bool quiet = false;
    FILE *output = stdout;
    char character;

    // Add arguments to the parser
    parser.add_argument(&parser, "-n", "--name", PMARGP_STRING, &name, "Your name", true);
    parser.add_argument(&parser, "-c", "--count", PMARGP_INT, &count, "Number of greetings", false);
    parser.add_argument(&parser, "-v", "--value", PMARGP_FLOAT, &value, "A floating-point value", false);
    parser.add_argument(&parser, "-q", "--quiet", PMARGP_BOOL, &quiet, "Run in quiet mode", false);
    parser.add_argument(&parser, "-o", "--output", PMARGP_W_FILE, &output, "Output file", false);
    parser.add_argument(&parser, "-r", "--character", PMARGP_CHAR, &character, "Random character", false);

    // Parse the arguments
    int error;
    if ((error = parser.parses(&parser, argc, argv)) != 1) {
        fprintf(stderr, "Error code %d parsing arguments\n", error);
        free_parser(&parser);
        return 1;
    }

    // Output and argument handling logic
    if (!quiet) {
        for (int i = 0; i < count; i++) {
            fprintf(output, "Hello, %s! The value is %f %c\n", name, value, character);
        }
    }

    // Cleanup
    if (output != stdout) {
        fclose(output);
    }

    free_parser(&parser);

    return 0;
}
```

You can get help on available arguments by running ``my_program -h``

```
A simple program demonstrating argument parsing.

Usage: example_program [OPTIONS]

Options:
  --name         -n  <string>       Your name (Required)
  --count        -c  <integer>      Number of greetings (Default: 1)
  --value        -v  <float>        A floating-point value (Default: 0.00)
  --quiet        -q  <bool>         Run in quiet mode (Default: false)
  --output       -o  <write_file>   Output file
  --character    -r  <char>         Random character
```

## Testing

Unit tests are available in the `test/` directory. To run the tests:

```bash
make test
```

This will execute the test suite, ensuring that the argument parser works as expected and validating different argument types and edge cases.
if you wish to add your own test you can follow the test I provided, and extend it by just adding the function pointer, and its corresponding 
names to the run_group similar to the others. 
## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

This version refines the layout, introduces consistent styling, and improves clarity, maintaining a professional tone throughout.
