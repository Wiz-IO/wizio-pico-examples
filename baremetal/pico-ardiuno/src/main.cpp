/*
     how to: https://github.com/Wiz-IO/wizio-pico/wiki/BAREMETAL#arduino-in-pico-sdk
*/

#include <Arduino.h> 

int main(void)
{
    Serial.begin(115200, true); // true - retarget stdio printf()
    pinMode(25, OUTPUT);
    while (true)
    {
        Serial.print("LO");
        Serial.println("OP");
        Serial.printf("MILLIS = %u\n", millis());
        printf("PRINTF %f\n", 22.0 / 7);

        static int led = 0;
        digitalWrite(25, led);
        led ^= 1;
        delay(1000);
    }
}
