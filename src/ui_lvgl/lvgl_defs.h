// This is the file to store global lvgl declarations

#pragma once
#include <LVGL_CYD.h>
#include <stdint.h>

/* Takes position of USB connector relative to screen. These are synonyms
   as defined in LVGL_CYD.h:

      #define USB_DOWN  LV_DISPLAY_ROTATION_0
      #define USB_RIGHT LV_DISPLAY_ROTATION_90
      #define USB_UP    LV_DISPLAY_ROTATION_180
      #define USB_LEFT  LV_DISPLAY_ROTATION_270
*/ 
#define SCREEN_ORIENTATION USB_RIGHT

// Definie SOC arc min/max
constexpr uint8_t LDR_ARC_MIN = 0;
constexpr uint8_t LDR_ARC_MAX = 100;

// Define arc colors (in hex) for SOC states
// SOC arc colors (using lv_color_make for portability)
const lv_color_t SOC_UPPER_COLOR    = lv_color_make(0x0A, 0x70, 0x05);
const lv_color_t SOC_HIGH_COLOR     = lv_color_make(0x08, 0x9C, 0x84);
const lv_color_t SOC_MID_COLOR      = lv_color_make(0x82, 0x9C, 0x49);
const lv_color_t SOC_LOW_COLOR      = lv_color_make(0x9C, 0x73, 0x3F);
const lv_color_t SOC_CRITICAL_COLOR = lv_color_make(0x9C, 0x30, 0x30);