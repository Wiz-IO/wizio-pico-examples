#ifndef _HAL_TFT_CFG_H_
#define _HAL_TFT_CFG_H_
#ifdef __cplusplus
extern "C"
{
#endif

#define TFT_WIDTH   240
#define TFT_HEIGHT  240

#define TFT_MOSI    15
#define TFT_SCLK    14
#define TFT_MISO    -1 /* not used*/
#define TFT_CS      -1 /* not used*/
#define TFT_DC      12
#define TFT_RST     13

#define TFT_SPI spi1

#define TFT_SPI_BRG 31250000u /* is max for pico default */

#define TFT_SPI_FORMAT(BITS) TFT_SPI, BITS, SPI_CPOL_1, SPI_CPHA_0, SPI_MSB_FIRST

#ifdef __cplusplus
}
#endif
#endif // _HAL_TFT_CFG_H_