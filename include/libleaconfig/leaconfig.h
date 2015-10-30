/* leaconfig.h - (C) 2015, Emir Marincic
 * libleaconfig - config library
 * See README for contact-, COPYING for license information. */

#ifndef LIBLEACONFIG_LEACONFIG_H
#define LIBLEACONFIG_LEACONFIG_H 1

typedef enum
{
    CONFIG_TYPE_NAN = -1,
    CONFIG_TYPE_NONE = 0,
    CONFIG_TYPE_INT ,
    CONFIG_TYPE_FLOAT, 
    CONFIG_TYPE_BOOL,
    CONFIG_TYPE_STRING,
    CONFIG_TYPE_GROUP,
    CONFIG_TYPE_ARRAY
} config_entry_type_t;

typedef enum
{
    CONFIG_SUCCESS = 0,
    CONFIG_ERROR_EXISTS,
    CONFIG_ERROR_NEXISTS,
    CONFIG_ERROR_MISMATCH,
    CONFIG_ERROR_IO,
    CONFIG_ERROR_MALLOC
} config_error_t;

typedef struct config *config_t;

#define CFG(NAME, TYPE) config_entry_get_##TYPE(NULL,NAME)
#define CFGSET(NAME, VALUE, TYPE) config_entry_set_##TYPE(NULL, NAME, VALUE)

config_t config_init(void);
config_error_t config_clear(config_t d);
config_error_t config_dispose(config_t d);
config_error_t config_set_config(config_t d);
config_error_t config_set_filename(config_t d, const char *filename);
config_error_t config_read_file(config_t d, const char *filename);
config_error_t config_write_file(config_t d, const char *filename);
config_error_t config_add_entry(config_t d, const char *name);
config_error_t config_remove_entry(config_t d, const char *name);
config_error_t config_entry_set_int(config_t d, const char *name, int i);
config_error_t config_entry_set_double(config_t d, const char *name, double f);
config_error_t config_entry_set_bool(config_t d, const char *name, int i);
config_error_t config_entry_set_string(config_t d, const char *name, const char *s);
int config_entry_get_int(config_t d, const char *name);
double config_entry_get_double(config_t d, const char *name);
int config_entry_get_bool(config_t d, const char *name);
const char *config_entry_get_string(config_t d, const char *name);

#endif /* LIBLEACONFIG_LEACONFIG_H */
