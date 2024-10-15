/******************************************************************************************
 * 
 * Project
 *                              ▗▄▄▖ ▗▖  ▗▖ ▗▄▖ ▗▄▄▖  ▗▄▄▖▗▄▄▖ 
 *                              ▐▌ ▐▌▐▛▚▞▜▌▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌
 *                              ▐▛▀▘ ▐▌  ▐▌▐▛▀▜▌▐▛▀▚▖▐▌▝▜▌▐▛▀▘ 
 *                              ▐▌   ▐▌  ▐▌▐▌ ▐▌▐▌ ▐▌▝▚▄▞▘▐▌   

 * 
 * @name Poor man's argument parser (pmargp)
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
 ******************************************************************************************/

#ifndef PMARGP_H
#define PMARGP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Library version
 */
#define PMARGP_VERSION "0.1.0"

/**
 * @brief Flags for argument properties
 */
#define PMARGP_FLAG_REQUIRED 0x01  // Argument is required
#define PMARGP_FLAG_OPTIONAL 0x00  // Argument is optional


/**
 * @brief Error codes
 */
#define PMARGP_SUCCESS 0x01
#define PMARGP_ERR_FILE_OPEN 0x02
#define PMARGP_ERR_UNKNOWN_TYPE 0x03
#define PMARGP_ERR_ARG_MISSING 0x04

/**
 * @brief Enumeration of supported argument types.
 */
typedef enum {
    PMARGP_FLOAT,    ///< Floating-point number
    PMARGP_INT,      ///< Integer
    PMARGP_STRING,   ///< String
    PMARGP_CHAR,     ///< Single character
    PMARGP_BOOL,     ///< Boolean
    PMARGP_R_FILE,   ///< Read-only file
    PMARGP_W_FILE,   ///< Write-only file
    PMARGP_RW_FILE,  ///< Read-write file
    PMARGP_B_R_FILE, ///< Binary read-only file
    PMARGP_B_W_FILE, ///< Binary write-only file
    PMARGP_B_RW_FILE ///< Binary read-write file
} pmargp_type_t;


/**
 * @brief Structure representing a command-line argument.
 */
typedef struct pmargp_argument_t
{
    char *key;         ///< Long form of the argument (e.g., "--output")
    char *short_key;   ///< Short form of the argument (e.g., "-o")
    char *description; ///< Description of the argument
    void *value_ptr;   ///< Pointer to the parsed value
    pmargp_type_t type;   ///< Type of the argument
    bool required;     ///< Whether the argument is required
    bool allocated;    ///< Indicates if memory was dynamically allocated for this argument
} pmargp_argument_t;


/**
 * @brief Structure representing the argument parser.
 */
typedef struct pmargp_parser_t pmargp_parser_t;

struct pmargp_parser_t
{
    const char *name;        ///< Name of the program
    const char *description; ///< Description of the program
    int argc;                ///< Number of arguments
    pmargp_argument_t *args;        ///< Array of arguments

    /**
     * @brief Get an argument by its key.
     * @param parser Pointer to the parser structure.
     * @param key Key of the argument to find.
     * @return Pointer to the found pmargp_argument_t, or NULL if not found.
     */
    pmargp_argument_t *(*get_argument)(struct pmargp_parser_t *parser, const char *key);

    /**
     * @brief Get the index of an argument by its key.
     * @param parser Pointer to the parser structure.
     * @param key Key of the argument to find.
     * @return Index of the argument, or -1 if not found.
     */
    int (*get_argument_index)(struct pmargp_parser_t *parser, const char *key);

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
    bool (*add_argument)(struct pmargp_parser_t *parser, const char *short_key, const char *key,
                         pmargp_type_t type, void *value_ptr, char *description, bool required);

    /**
     * @brief Parse command-line arguments.
     * @param parser Pointer to the parser structure.
     * @param argc Number of command-line arguments.
     * @param argv Array of command-line argument strings.
     * @return true if parsing was successful, false otherwise.
     */
    int (*parses)(struct pmargp_parser_t *parser, int argc, char *argv[]);

};

/**
 * @brief Initialize the parser structure.
 * @param parser Pointer to the parser structure to initialize.
 */
void parser_start(struct pmargp_parser_t *parser);

/**
 * @brief Free resources allocated by the parser.
 * @param parser Pointer to the parser structure to free.
 */
void free_parser(struct pmargp_parser_t *parser);

#ifdef __cplusplus
}
#endif

#endif // ARGP_H
