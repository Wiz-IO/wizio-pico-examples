#include <Arduino.h>
#define SCR_WD 240
#define SCR_HT 320
#include <SPI.h>
SPIClass SPI(spi1);
#define TFT_SCLK 10
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_CS 13
#define TFT_RST 14
#define TFT_DC 15
#include <Adafruit_GFX.h>
#include <ILI9341.h>
ILI9341 lcd = ILI9341(TFT_DC, TFT_RST, TFT_CS);

#include <rp.h>
#include <bl.h>

void rainbow()
{
    for (int i = 0; i < SCR_HT; i += 4)
    {
        uint8_t r, g, b;
        lcd.rgbWheel(i * 512L / 240, &r, &g, &b);
        lcd.fillRect(0, i, SCR_WD, 4, RGBto565(r, g, b));
    }
}

void setup()
{
    Serial.begin(115200, true);
    printf("\n[APP] ILI9341 2021 Georgi Angelov\n");
    SPI.setPins(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
    lcd.init();
    rainbow();
    delay(1000);
    lcd.fillScreen(BLACK);
    lcd.drawImage(0, 0, 240, 240, (uint16_t *)rp);
    lcd.setTextColor(YELLOW, BLACK);
    lcd.setCursor(40, 250);
    lcd.println("WizIO 2021 Georgi Angelov");
    delay(1000);
    int t = millis();
    for (int i = 0; i < 24; i++)
        lcd.drawImage(0, 0, 240, 320, (uint16_t *)ballon); // ~23 frames per sec
    printf("Elapsed = %d\n", millis() - t);
    delay(1000);
}

void loop()
{
}