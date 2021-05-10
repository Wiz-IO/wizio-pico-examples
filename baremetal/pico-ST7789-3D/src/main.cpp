#include <stdio.h>
#include "pico/stdlib.h"
#include <Arduino.h>

#include <SPI.h>
SPIClass SPI(spi0);

#define TFT_DC 12
#define TFT_RST 13
#define TFT_CS -1
#define TFT_MOSI 19
#define TFT_MISO 16
#define TFT_SCLK 18

#define SCR_WD 240
#define SCR_HT 240
#define WD_3D 240
#define HT_3D 240

#include <Arduino_ST7789_Fast.h>
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

char txt[30];
#define MAX_OBJ 12
int bgMode = 3;
int object = 6;
int bfCull = 1;
int orient = 0;
int polyMode = 0;

#include "pat2.h"
#include "pat7.h"
#include "pat8.h"
#include "gfx3d.h"
#include "r.h"

unsigned int ms, msMin = 1000, msMax = 0, stats = 1, optim = 0;

void showStats()
{
    if (ms < msMin)
        msMin = ms;
    if (ms > msMax)
        msMax = ms;
    if (optim == 0)
    {
        snprintf(txt, 30, "%d ms     %d fps ", ms, 1000 / ms);
        lcd.setTextColor(YELLOW, BLACK);
        lcd.setCursor(0, SCR_HT - 28);
        lcd.print(txt);
        snprintf(txt, 30, "%d-%d ms  %d-%d fps   ", msMin, msMax, 1000 / msMax, 1000 / msMin);
        lcd.setTextColor(GREEN, BLACK);
        lcd.setCursor(0, SCR_HT - 18);
        lcd.print(txt);
        snprintf(txt, 30, "total/vis %d / %d   ", numPolys, numVisible);
        lcd.setTextColor(MAGENTA, BLACK);
        lcd.setCursor(0, SCR_HT - 8);
        lcd.print(txt);
    }
    else if (optim == 1)
    {
        optim = 2;
        snprintf(txt, 30, "00 ms     00 fps");
        lcd.setTextColor(YELLOW, BLACK);
        lcd.setCursor(0, SCR_HT - 28);
        lcd.print(txt);
        snprintf(txt, 30, "00-00 ms  00-00 fps");
        lcd.setTextColor(GREEN, BLACK);
        lcd.setCursor(0, SCR_HT - 18);
        lcd.print(txt);
        snprintf(txt, 30, "total/vis 000 / 000");
        lcd.setTextColor(MAGENTA, BLACK);
        lcd.setCursor(0, SCR_HT - 8);
        lcd.print(txt);
    }
    else
    {
        snprintf(txt, 30, "%2d", ms);
        lcd.setTextColor(YELLOW, BLACK);
        lcd.setCursor(0, SCR_HT - 28);
        lcd.print(txt);
        snprintf(txt, 30, "%2d", 1000 / ms);
        lcd.setCursor(10 * 6, SCR_HT - 28);
        lcd.print(txt);
        snprintf(txt, 30, "%2d-%2d", msMin, msMax);
        lcd.setTextColor(GREEN, BLACK);
        lcd.setCursor(0, SCR_HT - 18);
        lcd.print(txt);
        snprintf(txt, 30, "%2d-%2d", 1000 / msMax, 1000 / msMin);
        lcd.setCursor(10 * 6, SCR_HT - 18);
        lcd.print(txt);
        snprintf(txt, 30, "%3d", numPolys);
        lcd.setTextColor(MAGENTA, BLACK);
        lcd.setCursor(10 * 6, SCR_HT - 8);
        lcd.print(txt);
        snprintf(txt, 30, "%3d", numVisible);
        lcd.setCursor(16 * 6, SCR_HT - 8);
        lcd.print(txt);
    }
}

void setup()
{
    SPI.setPins(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
    lcd.init();
    lcd.fillScreen(BLACK);
    lcd.setTextColor(YELLOW, BLACK);
    initStars();
    lcd.fillRect(0, 0, 240, 240, RED);
    delay(200);
    lcd.fillRect(0, 0, 240, 240, GREEN);
    delay(200);
    lcd.fillRect(0, 0, 240, 240, BLUE);
    delay(200);
    lcd.drawImage(0, 0, 240, 240, (uint16_t *)rp);
    delay(3000);
}

void loop()
{

    polyMode = 0;
    orient = 0;
    bfCull = 1;
    lightShade = 0;
    switch (object)
    {
    case 0:
        numVerts = numVertsCubeQ;
        verts = (int16_t *)vertsCubeQ;
        numPolys = numQuadsCubeQ;
        polys = (uint8_t *)quadsCubeQ;
        polyColors = (uint16_t *)colsCubeQ;
        break;
    case 1:
        numVerts = numVertsCubeQ;
        verts = (int16_t *)vertsCubeQ;
        numPolys = numQuadsCubeQ;
        polys = (uint8_t *)quadsCubeQ;
        lightShade = 44000;
        break;
    case 2:
        numVerts = numVertsCross;
        verts = (int16_t *)vertsCross;
        numPolys = numQuadsCross;
        polys = (uint8_t *)quadsCross;
        polyColors = (uint16_t *)colsCross;
        break;
    case 3:
        numVerts = numVertsCross;
        verts = (int16_t *)vertsCross;
        numPolys = numQuadsCross;
        polys = (uint8_t *)quadsCross;
        lightShade = 14000;
        break;
    case 4:
        numVerts = numVerts3;
        verts = (int16_t *)verts3;
        numPolys = numQuads3;
        polys = (uint8_t *)quads3;
        polyColors = (uint16_t *)cols3;
        break;
    case 5:
        numVerts = numVerts3;
        verts = (int16_t *)verts3;
        numPolys = numQuads3;
        polys = (uint8_t *)quads3;
        lightShade = 20000;
        break;
    case 6:
        numVerts = numVertsCubes;
        verts = (int16_t *)vertsCubes;
        numPolys = numQuadsCubes;
        polys = (uint8_t *)quadsCubes;
        polyColors = (uint16_t *)colsCubes;
        bfCull = 0;
        break;
    case 7:
        numVerts = numVertsCubes;
        verts = (int16_t *)vertsCubes;
        numPolys = numQuadsCubes;
        polys = (uint8_t *)quadsCubes;
        bfCull = 1;
        lightShade = 14000;
        break;
    case 8:
        numVerts = numVertsCone;
        verts = (int16_t *)vertsCone;
        numPolys = numTrisCone;
        polys = (uint8_t *)trisCone;
        polyColors = (uint16_t *)colsCone;
        bfCull = 1;
        orient = 1;
        polyMode = 1;
        break;
    case 9:
        numVerts = numVertsSphere;
        verts = (int16_t *)vertsSphere;
        numPolys = numTrisSphere;
        polys = (uint8_t *)trisSphere;
        //polyColors = (uint16_t*)colsSphere;
        lightShade = 58000;
        bfCull = 1;
        orient = 1;
        polyMode = 1;
        break;
    case 10:
        numVerts = numVertsTorus;
        verts = (int16_t *)vertsTorus;
        numPolys = numTrisTorus;
        polys = (uint8_t *)trisTorus;
        polyColors = (uint16_t *)colsTorus;
        bfCull = 1;
        orient = 1;
        polyMode = 1;
        break;
    case 11:
        numVerts = numVertsTorus;
        verts = (int16_t *)vertsTorus;
        numPolys = numTrisTorus;
        polys = (uint8_t *)trisTorus;
        lightShade = 20000;
        bfCull = 1;
        orient = 1;
        polyMode = 1;
        break;
    case 12:
        numVerts = numVertsMonkey;
        verts = (int16_t *)vertsMonkey;
        numPolys = numTrisMonkey;
        polys = (uint8_t *)trisMonkey;
        polyColors = (uint16_t *)colsMonkey;
        lightShade = 20000;
        bfCull = 1;
        orient = 1;
        polyMode = 1;
        break;
    }

    ms = millis();
    render3D(polyMode);
    ms = millis() - ms;

    //if (stats) showStats();

    static int T = 0;
    if (++T % 100 == 0)
    {
        if (++object > MAX_OBJ)
        {
            object = 0;
        }
    }
}

int main(void)
{
    stdio_init_all();
    printf("\n\nHello World\n");
    setup();
    while (true)
    {
        loop();
    }
}
