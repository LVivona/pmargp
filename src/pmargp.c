#include "pmargp.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

static inline bool  is_help(const char *flag) {
    return strcmp(flag, "--help") == 0 || strcmp(flag, "-h") == 0;
}

pmargp_argument_t *get_argument(struct pmargp_parser_t* parser, const char *key) {
    for (int i = 0; i < parser->argc; i++) {
        if (strcmp(parser->args[i].key, key) == 0 || 
            (parser->args[i].short_key && strcmp(parser->args[i].short_key, key) == 0)) {
            return &parser->args[i];
        }
    }
    return NULL;
}


int get_argument_index(struct pmargp_parser_t* parser, const char *key) {
    for (int i = 0; i < parser->argc; i++) {
        if (strcmp(parser->args[i].key, key) == 0 ||
            (parser->args[i].short_key && strcmp(parser->args[i].short_key, key) == 0)) {
            return i;
        }
    }
    return -1;
}

bool add_argument(struct pmargp_parser_t* parser, const char* restrict short_key, const char* restrict key, 
                  pmargp_type_t type, void* value_ptr, char *description, bool required) {
    if (key == NULL || parser == NULL) return false;
    if (get_argument_index(parser, key) >= 0 || (short_key && get_argument_index(parser, short_key) >= 0)) return false;
    if (is_help(key)) return false;

    pmargp_argument_t *new_args = realloc(parser->args, (parser->argc + 1) * sizeof(pmargp_argument_t));
    if (new_args == NULL) return false;
    parser->args = new_args;

    pmargp_argument_t *arg = &parser->args[parser->argc];
    arg->key = strdup(key);
    arg->short_key = short_key ? strdup(short_key) : NULL;
    arg->description = description ? strdup(description) : NULL;
    arg->type = type;
    arg->required = required;
    arg->value_ptr = value_ptr;
    arg->allocated = false;

    parser->argc++;
    return true;
}

static bool help_info(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (is_help(argv[i])) return true;
    }
    return false;
}

static const char* type_to_string(pmargp_type_t type) {
    static const char *type_strings[] = {
        [PMARGP_FLOAT] = "float",
        [PMARGP_INT] = "int",
        [PMARGP_STRING] = "string",
        [PMARGP_CHAR] = "char",
        [PMARGP_BOOL] = "bool",
        [PMARGP_R_FILE] = "read file",
        [PMARGP_W_FILE] = "write file",
        [PMARGP_RW_FILE] = "read-write file",
        [PMARGP_B_R_FILE] = "binary read file",
        [PMARGP_B_W_FILE] = "binary write file",
        [PMARGP_B_RW_FILE] = "binary read-write file"
    };
    return (type >= 0 && type <= PMARGP_B_RW_FILE) ? type_strings[type] : "unknown";
}

static const char* type_to_token(pmargp_type_t type) {
    static const char *type_tokens[] = {
        [PMARGP_FLOAT] = "<float>",
        [PMARGP_INT] = "<integer>",
        [PMARGP_STRING] = "<string>",
        [PMARGP_CHAR] = "<char>",
        [PMARGP_BOOL] = "<bool>",
        [PMARGP_R_FILE] = "<read_file>",
        [PMARGP_W_FILE] = "<write_file>",
        [PMARGP_RW_FILE] = "<read_write_file>",
        [PMARGP_B_R_FILE] = "<binary_read_file>",
        [PMARGP_B_W_FILE] = "<binary_write_file>",
        [PMARGP_B_RW_FILE] = "<binary_read_write_file>"
    };
    return (type >= 0 && type <= PMARGP_B_RW_FILE) ? type_tokens[type] : "";
}

static void help(struct pmargp_parser_t *parser) {
    printf("\n%s\n", parser->name ? parser->name : "Program Name");
    printf("%s\n\n", parser->description ? parser->description : "No description provided.");
    printf("Usage: %s [OPTIONS]\n\n", parser->name ? parser->name : "program");
    printf("Options:\n");

    int max_key_length = 0, max_short_key_length = 0;
    for (int i = 0; i < parser->argc; i++) {
        int key_length = strlen(parser->args[i].key);
        int short_key_length = parser->args[i].short_key ? strlen(parser->args[i].short_key) : 0;
        if (key_length > max_key_length) max_key_length = key_length;
        if (short_key_length > max_short_key_length) max_short_key_length = short_key_length;
    }

    for (int i = 0; i < parser->argc; i++) {
        const pmargp_argument_t *arg = &parser->args[i];
        printf("  %-*s  %-*s%-15s%s",
               max_key_length + 2, arg->key,
               max_short_key_length + 2, arg->short_key ? arg->short_key : "",
               type_to_token(arg->type),
               arg->description ? arg->description : "No description");
        printf(" (Type: %s)", type_to_string(arg->type));
        if (arg->required) printf(" [Required]");
        if (arg->value_ptr) {
            switch (arg->type) {
                case PMARGP_INT: printf("[Default: %d]", *(int*)arg->value_ptr); break;
                case PMARGP_FLOAT: printf("[Default: %.2f]", *(float*)arg->value_ptr); break;
                case PMARGP_BOOL: printf("[Default: %s]", *(bool*)arg->value_ptr ? "true" : "false"); break;
                case PMARGP_STRING: printf("[Default: %s]", strlen(*(char**)arg->value_ptr) > 0 ?  *(char**)arg->value_ptr : "None"  ); break;
                case PMARGP_CHAR: printf("[Default: %s]", (char *)arg->value_ptr ); break;
                default: break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

static const char *get_file_mode(pmargp_type_t type) {
    switch (type) {
        case PMARGP_R_FILE: return "r";
        case PMARGP_W_FILE: return "w";
        case PMARGP_RW_FILE: return "r+";
        case PMARGP_B_R_FILE: return "rb";
        case PMARGP_B_W_FILE: return "wb";
        case PMARGP_B_RW_FILE: return "rb+";
        default:
            fprintf(stderr, "Error: Unknown file mode\n");
            exit(EXIT_FAILURE);
    }
}

int parses(struct pmargp_parser_t* parser, int argc, char* argv[]) {
    if (parser->argc == 0) return PMARGP_ERR_NO_ARGUMENTS;
    if (help_info(argc, argv)) {
        help(parser);
        exit(EXIT_SUCCESS);
    }

    for (int i = 1; i < argc; i++) {
        int idx = get_argument_index(parser, argv[i]);
        if (idx == -1) continue;

        pmargp_argument_t *arg = &parser->args[idx];
        if (arg->allocated) continue;

        if (arg->type == PMARGP_BOOL) {
            *(bool*)arg->value_ptr = true;
            arg->allocated = true;
        } else if (i + 1 < argc) {
            switch (arg->type) {
                errno = 0; // error catch
                case PMARGP_CHAR:
                    *(char*)arg->value_ptr = *argv[++i];  
                    break;
                case PMARGP_STRING:
                    *(char**)arg->value_ptr = strdup(argv[++i]);
                    break;
                case PMARGP_FLOAT: 
                    *(float*)arg->value_ptr = atof(argv[++i]);
                    break;
                case PMARGP_INT: 
                    *(int*)arg->value_ptr = atoi(argv[++i]);
                    break;
                case PMARGP_R_FILE:
                case PMARGP_W_FILE:
                case PMARGP_RW_FILE:
                case PMARGP_B_R_FILE:
                case PMARGP_B_W_FILE:
                case PMARGP_B_RW_FILE: {
                    const char *mode = get_file_mode(arg->type);
                    FILE *file = fopen(argv[++i], mode);
                    if (file) {
                        *(FILE**)arg->value_ptr = file;
                    } else {
                        fprintf(stderr, "Error opening file: %s\n", argv[i]);
                        return PMARGP_ERR_FILE_OPEN;
                    }
                    break;
                }
                default:
                    fprintf(stderr, "Unknown argument type for %s\n", arg->key);
                    return PMARGP_ERR_UNKNOWN_TYPE;
            }
            arg->allocated = true;
            if(errno == ERANGE){
                return PMARGP_ERR_OVERFLOW;
            }
        }
    }


    for (int j = 0; j < parser->argc; j++) {
        pmargp_argument_t *arg = &parser->args[j];
        if (arg->required && !arg->allocated) {
            return PMARGP_ERR_ARG_MISSING;
        }
    }

    return PMARGP_SUCCESS;
}

void parser_start(struct pmargp_parser_t *parser) {
    if (parser) {
        parser->argc = 0;
        parser->args = NULL;
        parser->name = NULL;
        parser->description = NULL;
        parser->add_argument = add_argument;
        parser->parses = parses;
        parser->get_argument = get_argument;
        parser->get_argument_index = get_argument_index;
    }
}

void free_parser(struct pmargp_parser_t *parser) {
    if (!parser) return;

    for (int j = 0; j < parser->argc; j++) {
        pmargp_argument_t *arg = &parser->args[j];
        free(arg->key);
        free(arg->short_key);
        free(arg->description);
    }
    free(parser->args);
    parser->args = NULL;
    parser->argc = 0;
}