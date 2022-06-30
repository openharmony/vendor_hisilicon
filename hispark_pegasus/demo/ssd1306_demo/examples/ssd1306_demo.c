﻿/*
 * Copyright (C) 2022 HiHope Open Source Organization .
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http:// www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 *
 * limitations under the License.
 */

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_pwm.h"
#include "wifiiot_adc.h"
#include "wifiiot_i2c.h"
#include "wifiiot_errno.h"
#include "wifiiot_watchdog.h"

#include "ssd1306.h"
#include "ssd1306_tests.h"

#define OLED_I2C_BAUDRATE (400*1000)
#define SIXTY_FOUR 64
#define FIVE_HUNDRED 500
#define SEVEN 7
#define ATTR.STACK_SIZE 10240
#define SIX 6
#define EIGHT 8
#define TWO 2
#define TEN 10
#define TWENTY 20
#define ONE_THOUSAND 1000
#define ONE_HUNDRED_TWENTY_EIGHT 8

const unsigned char headSize[] = { 64, 64 };
const unsigned char headData[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x80, 0x00,
0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xE0, 0x00,
0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xF0, 0x00,
0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xF0, 0x00,
0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xF0, 0x00,
0x00, 0x00, 0x00, 0xFF, 0x83, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE3, 0xF0, 0x18, 0x00,
0x00, 0x00, 0x01, 0xFF, 0xE3, 0xE0, 0x10, 0x00, 0x00, 0x00, 0x61, 0xFE, 0x03, 0xE1, 0xF8, 0x00,
0x00, 0xC1, 0xF1, 0xF8, 0x97, 0xE0, 0x18, 0x00, 0x00, 0x00, 0xB9, 0xFF, 0xFF, 0xF8, 0x88, 0x00,
0x00, 0x00, 0xFD, 0xFF, 0xFF, 0xFD, 0xF8, 0x00, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0xFE, 0x78, 0x00,
0x00, 0x00, 0xCD, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x40, 0xDF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00,
0x00, 0x40, 0xDF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x40, 0x7F, 0xFF, 0xFF, 0xFF, 0xF8, 0x00,
0x00, 0xA0, 0x7F, 0xFF, 0xFF, 0xFF, 0xF9, 0x00, 0x01, 0x20, 0x3F, 0xFF, 0xFC, 0x47, 0xF8, 0x00,
0x02, 0x10, 0x1F, 0xFF, 0xFF, 0x87, 0xF0, 0x00, 0x0C, 0x18, 0x0F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00,
0x00, 0x08, 0x07, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x0D, 0x07, 0xFF, 0xFC, 0x07, 0xF0, 0x00,
0x00, 0x05, 0x07, 0xFF, 0xC0, 0x01, 0xF0, 0x00, 0x00, 0x05, 0x07, 0xFF, 0xDF, 0xE1, 0xE0, 0x00,
0x02, 0x06, 0x07, 0xFF, 0xFF, 0x09, 0xE0, 0x00, 0x00, 0x02, 0x07, 0xFF, 0xFE, 0x1F, 0xC0, 0x00,
0x00, 0x02, 0x07, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x60, 0x3F, 0xFF, 0xFF, 0xFF, 0x80, 0x00,
0x00, 0x30, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x32, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00,
0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0x18, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00,
0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFC, 0xF0, 0x00, 0x00,
0x00, 0x07, 0xFF, 0xFF, 0xFE, 0xF0, 0x00, 0x01, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x07,
0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xF0, 0xE0, 0x3F, 0x00, 0x23, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
};

/**
 * 汉字字模在线： https://www.23bei.com/tool-223.html
 * 数据排列：从左到右从上到下
 * 取模方式：横向8位左高位
**/
void TestDrawChinese1(void)
{
    const uint32_t W = 16, H = 16;
    uint8_t fonts[][32] = {
        {
            /*-- ID:0,字符:"你",ASCII编码:C4E3,对应字:宽x高=16x16,画布:宽W=16 高H=16,共32字节 */
            0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x23, 0xFC, 0x22, 0x04, 0x64, 0x08, 0xA8, 0x40, 0x20, 0x40,
            0x21, 0x50, 0x21, 0x48, 0x22, 0x4C, 0x24, 0x44, 0x20, 0x40, 0x20, 0x40, 0x21, 0x40, 0x20, 0x80,
        }, {
            /*-- ID:1,字符:"好",ASCII编码:BAC3,对应字:宽x高=16x16,画布:宽W=16 高H=16,共32字节 */
            0x10, 0x00, 0x11, 0xFC, 0x10, 0x04, 0x10, 0x08, 0xFC, 0x10, 0x24, 0x20, 0x24, 0x24, 0x27, 0xFE,
            0x24, 0x20, 0x44, 0x20, 0x28, 0x20, 0x10, 0x20, 0x28, 0x20, 0x44, 0x20, 0x84, 0xA0, 0x00, 0x40,
        }, {
            /*-- ID:2,字符:"鸿",ASCII编码:BAE8,对应字:宽x高=16x16,画布:宽W=16 高H=16,共32字节 */
            0x40, 0x20, 0x30, 0x48, 0x10, 0xFC, 0x02, 0x88, 0x9F, 0xA8, 0x64, 0x88, 0x24, 0xA8, 0x04, 0x90,
            0x14, 0x84, 0x14, 0xFE, 0xE7, 0x04, 0x3C, 0x24, 0x29, 0xF4, 0x20, 0x04, 0x20, 0x14, 0x20, 0x08,
        }, {
            /*-- ID:3,字符:"蒙",ASCII编码:C3C9,对应字:宽x高=16x16,画布:宽W=16 高H=16,共32字节 */
            0x04, 0x48, 0x7F, 0xFC, 0x04, 0x40, 0x7F, 0xFE, 0x40, 0x02, 0x8F, 0xE4, 0x00, 0x00, 0x7F, 0xFC,
            0x06, 0x10, 0x3B, 0x30, 0x05, 0xC0, 0x1A, 0xA0, 0x64, 0x90, 0x18, 0x8E, 0x62, 0x84, 0x01, 0x00
        }
    };

    ssd1306_Fill(Black);
    for (size_t i = 0; i < sizeof(fonts)/sizeof(fonts[0]); i++) {
        ssd1306_DrawRegion(i * W, 0, W, H, fonts[i], sizeof(fonts[0]), W);
    }

    ssd1306_SetCursor(SIXTY_FOUR, 0);
    ssd1306_DrawRegion(SIXTY_FOUR, 0, headSize[0], headSize[1], headData, sizeof(headData), headSize[0]);
    ssd1306_UpdateScreen();
}

void TestDrawChinese2(void)
{
    const uint32_t W = 12, H = 12, S = 16;
    uint8_t fonts[][24] = {
        {
            /*-- ID:0,字符:"你",ASCII编码:C4E3,对应字:宽x高=12x12,画布:宽W=16 高H=12,共24字节 */
            0x12, 0x00, 0x12, 0x00, 0x27, 0xF0, 0x24, 0x20, 0x69, 0x40, 0xA1, 0x00, 0x25, 0x40, 0x25, 0x20,
            0x29, 0x10, 0x31, 0x10, 0x25, 0x00, 0x22, 0x00,
        }, {
            /*-- ID:1,字符:"好",ASCII编码:BAC3,对应字:宽x高=12x12,画布:宽W=16 高H=12,共24字节 */
            0x20, 0x00, 0x27, 0xE0, 0x20, 0x40, 0xF8, 0x80, 0x48, 0x80, 0x48, 0xA0, 0x57, 0xF0, 0x50, 0x80,
            0x30, 0x80, 0x28, 0x80, 0x4A, 0x80, 0x81, 0x00,
        }, {
            /*-- ID:2,字符:"鸿",ASCII编码:BAE8,对应字:宽x高=12x12,画布:宽W=16 高H=12,共24字节 */
            0x00, 0x40, 0x80, 0x80, 0x5D, 0xE0, 0x09, 0x20, 0xC9, 0xA0, 0x09, 0x60, 0x29, 0x00, 0xCD, 0xF0,
            0x58, 0x10, 0x43, 0xD0, 0x40, 0x10, 0x40, 0x60,
        }, {
            /*-- ID:3,字符:"蒙",ASCII编码:C3C9,对应字:宽x高=12x12,画布:宽W=16 高H=12,共24字节 */
            0x09, 0x00, 0x7F, 0xE0, 0x09, 0x00, 0x7F, 0xF0, 0x80, 0x10, 0x7F, 0xE0, 0x0C, 0x40, 0x32, 0x80,
            0xC7, 0x00, 0x0A, 0x80, 0x32, 0x70, 0xC6, 0x20
        }
    };

    ssd1306_Fill(Black);
    for (size_t i = 0; i < sizeof(fonts)/sizeof(fonts[0]); i++) {
        ssd1306_DrawRegion(i * H, 0, W, H, fonts[i], sizeof(fonts[0]), S);
    }

    ssd1306_SetCursor(SIXTY_FOUR, 0);
    ssd1306_DrawRegion(SIXTY_FOUR, 0, headSize[0], headSize[1], headData, sizeof(headData), headSize[0]);

    ssd1306_SetCursor(0, SIXTY_FOUR - EIGHT);
    ssd1306_DrawString("gitee.com", Font_6x8, White);

    ssd1306_SetCursor(0, SIXTY_FOUR - EIGHT*TWO);
    ssd1306_DrawString("@swxu", Font_6x8, White);

    ssd1306_UpdateScreen();
}

void TestShowChars(FontDef font, uint8_t w, uint8_t h)
{
    ssd1306_Fill(Black);

    uint8_t x = 0, y = 0;
    for (uint8_t c = 1; c < ONE_HUNDRED_TWENTY_EIGHT; c++) {
        if (isprint(c)) {
            ssd1306_SetCursor(x, y);
            ssd1306_DrawChar((char) c, font, White);
            x += w;
            if (x >= SSD1306_WIDTH) {
                x = 0;
                y += h;
            }
        }
    }

    ssd1306_UpdateScreen();
}

void Ssd1306TestTask(int* arg)
{
    (void) arg;
    GpioInit();
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_13, WIFI_IOT_IO_FUNC_GPIO_13_I2C0_SDA);
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_14, WIFI_IOT_IO_FUNC_GPIO_14_I2C0_SCL);
    I2cInit(WIFI_IOT_I2C_IDX_0, OLED_I2C_BAUDRATE);

    WatchDogDisable();

    usleep(TWENTY*ONE_THOUSAND);
    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_DrawString("Hello HarmonyOS!", Font_7x10, White);

    uint32_t start = HAL_GetTick();
    ssd1306_UpdateScreen();
    uint32_t end = HAL_GetTick();
    printf("ssd1306_UpdateScreen time cost: %d ms.\r\n", end - start);

    while (1) {
        TestDrawChinese1();
        osDelay(FIVE_HUNDRED);

        TestDrawChinese2();
        osDelay(FIVE_HUNDRED);

        TestShowChars(Font_6x8, SIX, EIGHT);
        osDelay(FIVE_HUNDRED);

        TestShowChars(Font_7x10, SEVEN, TEN);
        osDelay(FIVE_HUNDRED);

        ssd1306_TestAll();
    }
}

void Ssd1306TestDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "Ssd1306Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = ATTR.STACK_SIZE;
    attr.priority = osPriorityNormal;

    if (osThreadNew(Ssd1306TestTask, NULL, &attr) == NULL) {
        printf("[Ssd1306TestDemo] Failed to create Ssd1306TestTask!\n");
    }
}
APP_FEATURE_INIT(Ssd1306TestDemo);