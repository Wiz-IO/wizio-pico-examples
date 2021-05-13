#include <stdio.h>
#include "pico/stdlib.h"
#include <VFS.h>
#include <fcntl.h>

char buf[256];

int main(void)
{
    stdio_init_all();
    printf("\n\n[APP] SD Card test\n");
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    vfs_init(); // see vfs_config.h

    memset(buf, 0, sizeof(buf));
    
    int res, fd = open("0:/test.txt", O_RDONLY);
    printf("File FD = %d\n", fd);
    if (fd > 0)
    {
        res = read(fd, buf, sizeof(buf));
        printf("\n(%d bytes) %s\n", res, buf);
        close(fd);
    }
    else
        perror("Error: ");

    while (true)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(500);
    }
}
