#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
extern Adafruit_SSD1306 display;

void blink(int ms)
{
    if (0 == gpio_get_dir(25))
    {
        gpio_init(25);
        gpio_set_dir(25, GPIO_OUT);
    }
    gpio_put(25, 1);
    sleep_ms(ms);
    gpio_put(25, 0);
    sleep_ms(ms);
}

extern char basic_rom[];
uint8_t basic_mem[0x10000];

static uint8_t key = 0x80;
static char keyString[] = "10 home\r20 print \"Pi Pico \"\rrun\r";
static uint32_t keyCount = 0;
static uint8_t last = 0x00;

const char *ascii = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ !\"#$%&'()*+,-./0123456789:;<=>?";

extern "C" void cpu6502_reset(void);
extern "C" int cpu6502_run(void);
extern "C" uint32_t cpu6502_pc;

extern "C" void cpu6502_dump(uint32_t pc, uint32_t a, uint32_t x, uint32_t y, uint32_t sp, uint32_t sr) {}

extern "C" uint8_t cpu6502_load(uint16_t addr)
{
    uint32_t fp;
    uint8_t result = 0;
    //__asm__("movs %0, r12" : "=r"(fp));
    if (0x0400 <= addr && addr <= 0x7FF)
    {
        // Fake text VRAM.
        result = basic_mem[addr];
        result = 0xff;
    }
    else if ((addr >= 0x300 && addr <= 0x3FF) || (0x0900 <= addr && addr <= 0xFFF))
    {
        result = last; // Fake memory impl to make it run on low memory chip.
    }
    else if ((addr >= 0x1000) && (addr < 0xBFFF))
    {
        return 0xFF; // Bus error due to minimum memory installation.
    }
#if 0
    else if ((addr >= 0xC000) && (addr < 0xCFFF))
    {
        if (0xC000 == addr) // I/O emulation.
        {
            keyCount++;
            if (keyCount < 0x800)
            {
                result = key;
            }
            else if (keyCount & 0xFF)
            {
                result = key;
            }
            else
            {
                uint32_t n = (keyCount - 0x800) >> 8;
                if (n < sizeof(keyString))
                    key = keyString[n] | 0x80;
                result = key;
            }
        }
        else if (0xC010 == addr)
        {
            key &= 0x7F;
            result = 0;
        }
    }
#endif
    else
    {
        result = basic_mem[addr];
    }
    //__asm__("movs r12, %0" ::"r"(fp));
    return result;
}

extern "C" void cpu6502_store(uint16_t addr, uint8_t data)
{
    uint32_t fp;
    if (addr >= 0x400 && addr <= 0x7FF)
    {
        basic_mem[addr] = data;
        int y = (addr - 0x400) >> 7;
        int x = addr & 0x7F;
        if (x >= 80)
        {
            x -= 80;
            y += 16;
        }
        else if (x >= 40)
        {
            x -= 40;
            y += 8;
        }

        //printf("(%2d,%2d)@%04x %c( %02x )\n", x, y, addr, ascii[data & 0x3f], data);
        //if (ascii[data & 0x3f]) uart_putc(uart0, ascii[data & 0x3f]);

        /*  

            Need display decoder, 
            
            This display is only for test

        */

        x %= 15;
        y %= 20;
        display.setCursor(x * 5, y * 5);
        display.print(ascii[data & 0x3f]);
    }
    else if ((addr >= 0x300 && addr <= 0x03FF) || (addr >= 0x900 && addr <= 0xFFF))
    {
        last = data; // Fake memory impl to make it run on low memory chip.
    }
    else if ((addr >= 0x1000) && (addr < 0xBFFF))
    {
        return;
    }
    else if ((addr >= 0xC000) && (addr < 0xCFFF))
    {
        if (0xC000 == addr) // I/O emulation.
            key = data;
    }
    else
    {
        basic_mem[addr] = data; // Installed memory.
    }
}

void apple(void)
{
    memset(basic_mem, 0, 0x10000);
    memcpy(&basic_mem[0xD000], basic_rom, 0x3000); // 12k
    cpu6502_reset();
    int res = cpu6502_run();
    printf("[APPLE] QUIT %04x\n", res);
}
