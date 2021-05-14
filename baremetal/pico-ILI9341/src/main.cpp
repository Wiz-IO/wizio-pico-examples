#include <stdio.h>
#include "pico/stdlib.h"
#include <Arduino.h>
#define SCR_WD 240
#define SCR_HT 320
#include <SPI.h>
SPIClass SPI(spi1);
#define TFT_SCLK 10
#define TFT_MOSI 11 // TX
#define TFT_MISO 12 // RX
#define TFT_CS 13
#define TFT_RST 14
#define TFT_DC 15

#include <Adafruit_GFX.h>
#include <ILI9341_Fast.h>
ILI9341 lcd = ILI9341(TFT_DC, TFT_RST, TFT_CS);
#include <rp.h>

void setup(void)
{
    SPI.setPins(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
    lcd.init();
    lcd.fillScreen(BLACK);

#if 0
    lcd.setTextColor(YELLOW, BLACK);
    lcd.setCursor(40, 40);
    lcd.println("WizIO 2021 Georgi Angelov");
    lcd.setTextColor(WHITE, BLACK);
    int xt = (SCR_WD - 11 * 6) / 2, yt = (SCR_HT - 8) / 2;
    lcd.setCursor(xt, yt);
    lcd.println("HELLO WORLD");
    lcd.drawRect(xt - 10, yt - 10, 11 * 6 + 20, 8 + 20, GREEN);
#endif

    lcd.drawImageF(0, 0, 240, 240, rp);
    lcd.setTextColor(YELLOW, BLACK);
    lcd.setCursor(40, 250);
    lcd.println("WizIO 2021 Georgi Angelov");    
}

int main(void)
{
    stdio_init_all();
    printf("\n\nHello World\n");
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    setup();
    while (true)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(100);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(100);
    }
}
