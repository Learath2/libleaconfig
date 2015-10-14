#include "config.h"
#include "stdio.h"

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
    config_add_entry(conf, "kek");
    config_entry_set_string(conf, "kek", "toast");
    config_entry_get_string(conf, "kek", &s);
    printf("kek=%s\n", s);
    config_remove_entry(conf, "test");
    config_add_entry(conf, "test");
    config_entry_set_double(conf, "test", 0.1f);
    config_entry_get_double(conf, "test", &d);
    printf("test=%f\n", d);
    config_remove_entry(conf, "test");
    config_remove_entry(conf, "kek");
    config_set_filename(conf, "testconf.conf");
    config_read_file(conf);
    config_entry_get_int(conf, "ur_an_fagit", &i);
    printf("ur_an_fagit=%d", i);
    config_dispose(conf);
}