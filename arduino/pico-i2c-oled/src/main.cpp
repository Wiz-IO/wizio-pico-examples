#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SLAVE_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void oled_setup()
{
  printf("%s\n", __func__);
  //Wire.pins(4, 5);
  display.begin(SSD1306_SWITCHCAPVCC, SLAVE_ADDRESS);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("  Arduino");
  display.setTextColor(BLACK, WHITE);
  display.println(" Pi Pico  ");
  display.println("  RP2040  ");
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.println();
  display.println("      WizIO 2021");
  display.display();
}

void setup()
{
  delay(1000);                // just wait terminal to start
  Serial.begin(115200, true); // retarget stdio for printf
  printf("\n\nArdiuno Raspberrypi PI Pico 2021 Georgi Angelov\n");
  pinMode(LED, OUTPUT);
  oled_setup();
  printf("setup\n");
}

void loop()
{
  static int led = 0;
  digitalWrite(LED, led);
  led ^= 1;
  delay(500);
}