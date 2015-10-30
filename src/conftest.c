/* conftest.c - (C) 2015, Emir Marincic
 * libleaconfig - config library
 * See README for contact-, COPYING for license information. */

#include <stdio.h>

#include <libleaconfig/leaconfig.h>

int main(void)
{
    config_t conf = config_init();
    config_add_entry(conf, "test");
    config_entry_set_int(conf, "test", 155);
    printf("test=%d\n", config_entry_get_int(conf, "test"));
    config_add_entry(conf, "kek");
    config_entry_set_string(conf, "kek", "toast");
    printf("kek=%s\n", config_entry_get_string(conf, "kek"));
    config_remove_entry(conf, "test");
    config_add_entry(conf, "test");
    config_entry_set_double(conf, "test", 0.1f);
    printf("test=%f\n", config_entry_get_double(conf, "test"));
    config_write_file(conf, "tmp.conf");
    config_remove_entry(conf, "test");
    config_remove_entry(conf, "kek");
    config_set_filename(conf, "testconf.conf");
    config_read_file(conf, NULL);
    printf("ur_an_fagit=%d\n", config_entry_get_int(conf, "ur_an_fagit"));
    printf("potatoes_are_nice=%s\n", config_entry_get_string(conf, "potatoes_are_nice"));
    config_clear(conf);
    config_set_filename(conf, "tmp.conf");
    config_read_file(conf, NULL);
    printf("test=%f\n", config_entry_get_double(conf, "test"));
    printf("kek=%s\n", config_entry_get_string(conf, "kek"));
    config_dispose(conf);
}
