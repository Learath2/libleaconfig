#include "config.h"
#include "stdio.h"

int main(void)
{
    config_t conf = config_init();
    int i = 0;
    const char *s;
    config_add_entry(conf, "test");
    config_entry_set_int(conf, "test", 155);
    config_entry_get_int(conf, "test", &i);
    printf("test=%d\n", i);
}