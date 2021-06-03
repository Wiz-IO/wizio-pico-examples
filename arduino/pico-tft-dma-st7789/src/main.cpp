#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <ST7789.h>
Arduino_ST7789 TFT = Arduino_ST7789();

#include "logo.h"
extern void demo_setup();
extern void demo_loop();

void setup()
{
  pinMode(LED, OUTPUT);

  TFT.init(TFT_WIDTH, TFT_HEIGHT);
  TFT.fillScreen(BLUE);
  TFT.setTextSize(4);
  TFT.setTextColor(WHITE, BLUE);
  TFT.setCursor(60, 70);
  TFT.print("WizIO");
  TFT.setCursor(80, 130);
  TFT.print("2021");
  delay(1000);

  TFT.fillScreen(RED);
  for (int i = 0; i < 240; i += 20)
    TFT.drawFastVLine(i, 0, 240, WHITE);
  for (int i = 0; i < 240; i += 20)
    TFT.drawFastHLine(0, i, 240, WHITE);
  delay(1000);

  for (int i = 0; i < 100000; i += 1)
    TFT.drawPixel(random(0, 240), random(0, 240), WHITE);
  TFT.drawImage(0, 0, 240, 240, logo);
  delay(1000);

  demo_setup();
}

void loop()
{
#if 0  
  TFT.drawImage(0, 0, 240, 240, logo);
  digitalWrite(LED, 1);
  delay(200);

  TFT.fillScreen(RED);
  digitalWrite(LED, 0);
  delay(200);

  TFT.fillScreen(GREEN);
  digitalWrite(LED, 1);
  delay(200);

  TFT.fillScreen(BLUE);
  digitalWrite(LED, 0);
  delay(200);

#else
  demo_loop();
#endif
}