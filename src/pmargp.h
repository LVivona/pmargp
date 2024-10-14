/**
 * @name Poor man's argument parser (argp)
 * @author Luca Vivona
 * 
 * MIT License
 * Copyright (c) 2024 Luca Vince Vivona
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef ARGP_H
#define ARGP_H

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Enumeration of supported argument types.
 */
typedef enum
{
    FLOAT,      ///< Floating-point number
    INT,        ///< Integer
    STRING,     ///< String
    CHAR,       ///< Single character
    BOOL,       ///< Boolean
    R_FILE,     ///< Read-only file
    W_FILE,     ///< Write-only file
    RW_FILE,    ///< Read-write file
    B_R_FILE,   ///< Binary read-only file
    B_W_FILE,   ///< Binary write-only file
    B_RW_FILE   ///< Binary read-write file
} typeName_t;

/**
 * @brief Structure representing a command-line argument.
 */
typedef struct Argument_t
{
    char *key;         ///< Long form of the argument (e.g., "--output")
    char *short_key;   ///< Short form of the argument (e.g., "-o")
    char *description; ///< Description of the argument
    void *value_ptr;   ///< Pointer to the parsed value
    typeName_t type;   ///< Type of the argument
    bool required;     ///< Whether the argument is required
    bool allocated;    ///< Indicates if memory was dynamically allocated for this argument
} Argument_t;

/**
 * @brief Structure representing the argument parser.
 */
typedef struct parser_va
{
    const char *name;        ///< Name of the program
    const char *description; ///< Description of the program
    int argc;                ///< Number of arguments
    Argument_t *args;        ///< Array of arguments

    /**
     * @brief Get an argument by its key.
     * @param parser Pointer to the parser structure.
     * @param key Key of the argument to find.
     * @return Pointer to the found Argument_t, or NULL if not found.
     */
    Argument_t *(*get_argument)(struct parser_va *parser, const char *key);

    /**
     * @brief Get the index of an argument by its key.
     * @param parser Pointer to the parser structure.
     * @param key Key of the argument to find.
     * @return Index of the argument, or -1 if not found.
     */
    int (*get_argument_index)(struct parser_va *parser, const char *key);

    /**
     * @brief Add a new argument to the parser.
     * @param parser Pointer to the parser structure.
     * @param short_key Short form of the argument (can be NULL).
     * @param key Long form of the argument.
     * @param type Type of the argument.
     * @param value_ptr Pointer to store the parsed value.
     * @param description Description of the argument.
     * @param required Whether the argument is required.
     * @return true if the argument was added successfully, false otherwise.
     */
    bool (*add_argument)(struct parser_va *parser, const char *short_key, const char *key,
                         typeName_t type, void *value_ptr, char *description, bool required);

    /**
     * @brief Parse command-line arguments.
     * @param parser Pointer to the parser structure.
     * @param argc Number of command-line arguments.
     * @param argv Array of command-line argument strings.
     * @return true if parsing was successful, false otherwise.
     */
    bool (*parses)(struct parser_va *parser, int argc, char *argv[]);

} parser_va;

/**
 * @brief Initialize the parser structure.
 * @param parser Pointer to the parser structure to initialize.
 */
void parser_start(struct parser_va *parser);

/**
 * @brief Free resources allocated by the parser.
 * @param parser Pointer to the parser structure to free.
 */
void free_parser(struct parser_va *parser);

#endif // ARGP_H
