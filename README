# Poor Man's Argument Parser (argp)

## Overview

**Poor Man's Argument Parser** (`argp`) is a simple, lightweight C library for handling command-line arguments. It allows easy parsing of various argument types such as integers, floats, strings, and files. Designed with minimal overhead, it is ideal for small-to-medium-sized C applications that require flexible command-line argument parsing without the need for large dependencies.

## Features

- **Multiple argument types**: Supports integers, floats, strings, characters, and various file types (read, write, binary modes).
- **Short and long arguments**: Handle both short `-o` and long `--output` argument forms.
- **Required and optional arguments**: Enforce required arguments with ease.
- **Memory management**: Automatically handles memory allocation for dynamically parsed arguments.

## Project Structure

```bash
argp/
├── bin/               # Compiled binaries and object files
│   ├── argp.o
│   ├── example_program
├── example/           # Example usage of the argument parser
│   └── example.c
├── src/               # Source files for the argument parser
│   ├── argp.c
│   └── argp.h
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
    git clone https://github.com/yourusername/argp.git
    cd argp
    ```

2. Build the example and test programs:

    ```bash
    make
    ```

   This will create the binaries in the `bin/` directory:
   - `example_program`: Example usage of the argument parser.
   - `test_program`: Test cases for the argument parser.

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
#include "argp.h"

int main(int argc, char *argv[]) {
    parser_va parser;
    parser_start(&parser);

    int verbose = 0;
    char *output_file = NULL;

    // Add arguments
    parser.add_argument(&parser, "-v", "--verbose", BOOL, &verbose, "Enable verbose mode", false);
    parser.add_argument(&parser, "-o", "--output", STRING, &output_file, "Output file", true);

    // Parse arguments
    if (!parser.parses(&parser, argc, argv)) {
        printf("Error parsing arguments\n");
        return 1;
    }

    printf("Verbose: %d\n", verbose);
    if (output_file) {
        printf("Output file: %s\n", output_file);
    }

    free_parser(&parser);
    return 0;
}
```

### Tests

Unit tests are provided in the `test/` directory. To run them, simply build and execute:

```bash
make test
```

The test program will validate the functionality of the argument parser, ensuring proper handling of various argument types and cases.

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
