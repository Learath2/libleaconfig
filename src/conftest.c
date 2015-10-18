/* conftest.c - (C) 2015, Emir Marincic
 * libleaconfig - config library
 * See README for contact-, COPYING for license information. */

#include <stdio.h>

#include <libleaconfig/leaconfig.h>

int main(void)
{
    config_t conf = config_init();
    int i = 0;
    double d = 0;
    const char *s;
    config_add_entry(conf, "test");
    config_entry_set_int(conf, "test", 155);
    config_entry_get_int(conf, "test", &i);
    printf("test=%d\n", i);
    i = 0;
    config_add_entry(conf, "kek");
    config_entry_set_string(conf, "kek", "toast");
    config_entry_get_string(conf, "kek", &s);
    printf("kek=%s\n", s);
    s = NULL;
    config_remove_entry(conf, "test");
    config_add_entry(conf, "test");
    config_entry_set_double(conf, "test", 0.1f);
    config_entry_get_double(conf, "test", &d);
    printf("test=%f\n", d);
    config_remove_entry(conf, "test");
    config_remove_entry(conf, "kek");
    config_set_filename(conf, "testconf.conf");
    config_read_file(conf, NULL);
    config_entry_get_int(conf, "ur_an_fagit", &i);
    printf("ur_an_fagit=%d\n", i);
    config_entry_get_string(conf, "potatoes_are_nice", &s);
    printf("potatoes_are_nice=%s\n", s);
    config_dispose(conf);
}
