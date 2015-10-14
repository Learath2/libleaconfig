#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "config.h"

#define CHUNK_SIZE 32
#define MAX_LINE 1000

struct config
{
    char *filename;
    size_t length;
    struct config_entry **data;
};

struct config_entry
{
    char *name;
    config_entry_type_t type;
    union {
        int ival;
        double fval;
        char *sval;
    } data;
};

static struct config_entry *config_lookup_entry(config_t d, const char* name, size_t *index);
static void remove_tl_whitespaces(char *s);
static config_entry_type_t config_determine_value_type(char *s);
static config_error_t config_entry_create(struct config_entry **e, const char *name);
static config_error_t config_entry_delete(struct config_entry *e);

config_t config_init(void)
{
    struct config *d = malloc(sizeof *d);
    d->length = 0;
    d->data = malloc(sizeof *d->data * CHUNK_SIZE);
    return d;
}

config_error_t config_clear(config_t d)
{
    for(int i = 0; i < d->length; i++)
        config_entry_delete(d->data[i]);
    return CONFIG_SUCCESS;
}

config_error_t config_dispose(config_t d)
{
    config_clear(d);
    free(d->data);
    free(d->filename);
    free(d);
    return CONFIG_SUCCESS;
}

config_error_t config_set_filename(config_t d, const char *name)
{
    d->filename = malloc(sizeof *d->filename * strlen(name) + 1);
    if(!d->filename)
        return CONFIG_ERROR_MALLOC;
    strcpy(d->filename, name);
    return CONFIG_SUCCESS;
}

config_error_t config_read_file(config_t d) //NOTDONE
{
    char line[MAX_LINE];
    char *lhs, *rhs, *p;
    FILE *file = fopen(d->filename, "r");
    while(fgets(line, MAX_LINE, file) && !feof(file))
    {
        if(!strchr(line, '\n') || line[0] == '#' || (line[0] == '/' && line[1] == '/')) //Comment or line too long
            continue;

        //Split the line into lhs and rhs
        lhs = line;
        rhs = strchr(line, ':');
        if(!rhs[1]) //Colon at EOL
            continue;
        (rhs++)[0] = '\0';

        //Remove trailing and leading spaces from lhs and rhs
        remove_tl_whitespaces(lhs);
        remove_tl_whitespaces(rhs);
    }
}

config_error_t config_add_entry(config_t d, const char *name)
{
    if( d->length > 0 && d->length % CHUNK_SIZE == 0){
        struct config_entry **new = realloc(d->data, (d->length + CHUNK_SIZE) * sizeof *d->data);
        if(!new)
            return CONFIG_ERROR_MALLOC;
        d->data = new;
    }
    struct config_entry *e;
    config_error_t err = config_entry_create(&e, name);
    if(err)
        return err;
    d->data[d->length++] = e;
    return CONFIG_SUCCESS;
}

config_error_t config_remove_entry(config_t d, const char *name)
{
    size_t index = 0;
    struct config_entry *e = config_lookup_entry(d, name, &index);
    if(!e)
        return CONFIG_ERROR_NEXISTS;

    config_entry_delete(e);
    memmove(d->data[index], d->data[index + 1], sizeof *d->data * (d->length - index - 1));
    d->length--;
    return CONFIG_SUCCESS;
}

config_error_t config_entry_set_int(config_t d, const char *name, int i)
{
    struct config_entry *e = config_lookup_entry(d, name, NULL);
    if(!e)
        return CONFIG_ERROR_NEXISTS;
    if(!e->type)
        e->type = CONFIG_TYPE_INT;
    else if(e->type != CONFIG_TYPE_INT)
        return CONFIG_ERROR_MISMATCH;

    e->data.ival = i;
    return CONFIG_SUCCESS;
}

config_error_t config_entry_set_double(config_t d, const char *name, double f)
{
    struct config_entry *e = config_lookup_entry(d, name, NULL);
    if(!e)
        return CONFIG_ERROR_NEXISTS;
    if(!e->type)
        e->type = CONFIG_TYPE_FLOAT;
    else if(e->type != CONFIG_TYPE_FLOAT)
        return CONFIG_ERROR_MISMATCH;

    e->data.fval = f;
    return CONFIG_SUCCESS;
}

config_error_t config_entry_set_bool(config_t d, const char *name, int i)
{
    struct config_entry *e = config_lookup_entry(d, name, NULL);
    if(!e)
        return CONFIG_ERROR_NEXISTS;
    if(!e->type)
        e->type = CONFIG_TYPE_BOOL;
    else if(e->type != CONFIG_TYPE_BOOL)
        return CONFIG_ERROR_MISMATCH;

    e->data.ival = i;
    return CONFIG_SUCCESS;
}

config_error_t config_entry_set_string(config_t d, const char *name, const char *s)
{
    struct config_entry *e = config_lookup_entry(d, name, NULL);
    if(!e)
        return CONFIG_ERROR_NEXISTS;
    switch(e->type){
        case CONFIG_TYPE_NONE:
            e->type = CONFIG_TYPE_STRING;
        case CONFIG_TYPE_STRING:
            free(e->data.sval);
            e->data.sval = NULL;
            e->data.sval = malloc(strlen(s) + 1);
            if(!e->data.sval)
                return CONFIG_ERROR_MALLOC;
            strcpy(e->data.sval, s);
            break;
        default:
            return CONFIG_ERROR_MISMATCH;
    }
    return CONFIG_SUCCESS;
}

config_error_t config_entry_get_int(config_t d, const char *name, int *i)
{
    struct config_entry *e = config_lookup_entry(d, name, NULL);
    if(!e)
        return CONFIG_ERROR_NEXISTS;
    if(e->type != CONFIG_TYPE_INT)
        return CONFIG_ERROR_MISMATCH;
    *i = e->data.ival;
    return CONFIG_SUCCESS;
}

config_error_t config_entry_get_double(config_t d, const char *name, double *f)
{
    struct config_entry *e = config_lookup_entry(d, name, NULL);
    if(!e)
        return CONFIG_ERROR_NEXISTS;
    if(e->type != CONFIG_TYPE_FLOAT)
        return CONFIG_ERROR_MISMATCH;
    *f = e->data.fval;
    return CONFIG_SUCCESS;
}

config_error_t config_entry_get_bool(config_t d, const char *name, int *i)
{
    struct config_entry *e = config_lookup_entry(d, name, NULL);
    if(!e)
        return CONFIG_ERROR_NEXISTS;
    if(e->type != CONFIG_TYPE_BOOL)
        return CONFIG_ERROR_MISMATCH;
    *i = e->data.ival;
    return CONFIG_SUCCESS;
}

config_error_t config_entry_get_string(config_t d, const char *name, const char **s)
{
    struct config_entry *e = config_lookup_entry(d, name, NULL);
    if(!e)
        return CONFIG_ERROR_NEXISTS;
    if(e->type != CONFIG_TYPE_STRING)
        return CONFIG_ERROR_MISMATCH;
    *s = e->data.sval;
    return CONFIG_SUCCESS;
}

static config_error_t config_entry_delete(struct config_entry *e)
{
    if(e->type == CONFIG_TYPE_STRING)
        free(e->data.sval);
    free(e->name);
    free(e);
    return CONFIG_SUCCESS;
}

static struct config_entry *config_lookup_entry(config_t d, const char* name, size_t *index)
{
    for(size_t i = 0; i < d->length; i++)
    {
        if(!strcmp(d->data[i]->name, name)){
            if(index)
                *index = i;
            return d->data[i];
        }
    }
    return NULL;
}

static void remove_tl_whitespaces(char *s)
{
    char *p;
    while(isspace((int)*s)) s++;
    p = s + strlen(s) - 1;
    while(isspace((int)*p)) p--;
    p[1] = '\0';
}

static config_entry_type_t config_determine_value_type(char *s)
{
    char *end;
    long i;
    double d;
    if(s[0] == '"' && s[strlen(s)] == '"')
        return CONFIG_TYPE_STRING;
    else if((i = strtol(s, &end, 0)) && end == '\0' && (i <= INT_MAX && i >= INT_MIN))
        return CONFIG_TYPE_INT;
    else if((d = strtod(s, &end)) && end =='\0')
        return CONFIG_TYPE_FLOAT;
    else if(strstr(s, "true") || strstr(s, "false"))
        return CONFIG_TYPE_BOOL;
    else
        return CONFIG_TYPE_NAN;
}

static config_error_t config_entry_create(struct config_entry **e, const char *name)
{
    struct config_entry *entry = malloc(sizeof *entry);
    if(!entry)
        goto exit;

    memset(entry, 0, sizeof *entry);

    entry->name = malloc(sizeof *entry->name * strlen(name) + 1);
    if(!entry->name)
        goto exit; 
    strcpy(entry->name, name);
    entry->type = CONFIG_TYPE_NONE;
    *e = entry;

    return CONFIG_SUCCESS;

exit:
    free(entry);
    return CONFIG_ERROR_MALLOC;
}
