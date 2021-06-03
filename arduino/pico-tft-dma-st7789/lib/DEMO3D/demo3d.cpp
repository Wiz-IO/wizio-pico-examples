#include <stdio.h>
#include "pico/stdlib.h"
#include <Arduino.h>


#define SCR_WD 240
#define SCR_HT 240

#define WD_3D 240
#define HT_3D 240

#include <ST7789.h>
extern Arduino_ST7789 TFT;

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

unsigned int ms, msMin = 1000, msMax = 0, stats = 0, optim = 0;

void showStats()
{
    if (ms < msMin)
        msMin = ms;
    if (ms > msMax)
        msMax = ms;
    if (optim == 0)
    {
        snprintf(txt, 30, "%d ms     %d fps ", ms, 1000 / ms);
        TFT.setTextColor(YELLOW, BLACK);
        TFT.setCursor(0, SCR_HT - 28);
        TFT.print(txt);
        snprintf(txt, 30, "%d-%d ms  %d-%d fps   ", msMin, msMax, 1000 / msMax, 1000 / msMin);
        TFT.setTextColor(GREEN, BLACK);
        TFT.setCursor(0, SCR_HT - 18);
        TFT.print(txt);
        snprintf(txt, 30, "total/vis %d / %d   ", numPolys, numVisible);
        TFT.setTextColor(MAGENTA, BLACK);
        TFT.setCursor(0, SCR_HT - 8);
        TFT.print(txt);
    }
    else if (optim == 1)
    {
        optim = 2;
        snprintf(txt, 30, "00 ms     00 fps");
        TFT.setTextColor(YELLOW, BLACK);
        TFT.setCursor(0, SCR_HT - 28);
        TFT.print(txt);
        snprintf(txt, 30, "00-00 ms  00-00 fps");
        TFT.setTextColor(GREEN, BLACK);
        TFT.setCursor(0, SCR_HT - 18);
        TFT.print(txt);
        snprintf(txt, 30, "total/vis 000 / 000");
        TFT.setTextColor(MAGENTA, BLACK);
        TFT.setCursor(0, SCR_HT - 8);
        TFT.print(txt);
    }
    else
    {
        snprintf(txt, 30, "%2d", ms);
        TFT.setTextColor(YELLOW, BLACK);
        TFT.setCursor(0, SCR_HT - 28);
        TFT.print(txt);
        snprintf(txt, 30, "%2d", 1000 / ms);
        TFT.setCursor(10 * 6, SCR_HT - 28);
        TFT.print(txt);
        snprintf(txt, 30, "%2d-%2d", msMin, msMax);
        TFT.setTextColor(GREEN, BLACK);
        TFT.setCursor(0, SCR_HT - 18);
        TFT.print(txt);
        snprintf(txt, 30, "%2d-%2d", 1000 / msMax, 1000 / msMin);
        TFT.setCursor(10 * 6, SCR_HT - 18);
        TFT.print(txt);
        snprintf(txt, 30, "%3d", numPolys);
        TFT.setTextColor(MAGENTA, BLACK);
        TFT.setCursor(10 * 6, SCR_HT - 8);
        TFT.print(txt);
        snprintf(txt, 30, "%3d", numVisible);
        TFT.setCursor(16 * 6, SCR_HT - 8);
        TFT.print(txt);
    }
}

void demo_setup()
{
    TFT.fillScreen(BLACK);
    TFT.setTextSize(1);
    initStars();
}

void demo_loop()
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

    if (stats) showStats();

    static int T = 0;
    if (++T % 100 == 0)
    {
        if (++object > MAX_OBJ)
        {
            object = 0;
        }
    }
}
