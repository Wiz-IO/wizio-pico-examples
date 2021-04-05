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
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  //display.println("Arduino");
  display.display();
}

void apple(void);

void setup()
{
  Serial.begin(115200, true); // retarget stdio for printf
  printf("\n\nAPPLE II Raspberrypi PI Pico 2021 Georgi Angelov\n");
  pinMode(LED, OUTPUT);
  oled_setup();
  multicore_launch_core1(apple);
  printf("setup\n");
}

void loop()
{
  static int led = 0;
  digitalWrite(LED, led);
  led ^= 1;
  delay(1);
  display.display();
}