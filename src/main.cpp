/** A sketch for reading the JSON data from the JKBMS sketch that's hosting
  * a webserver. Is very basic as of now. I need to add a tabview and tables
  * for displaying the data.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <LVGL_CYD.h>

#include "bms/BMSData/BMSData.h"

#include "ui_lvgl/lvgl_ui.h"
#include "ui_lvgl/lvgl_defs.h"
#include "ui_lvgl/lvgl_objects.h"

// Enable or disable debugging output
#define DEBUG_ENABLED true

// Debugging macro
#if DEBUG_ENABLED
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#define DEBUG_PRINTF(...)
#endif

// Forward declarations:
void main_screen();
lv_obj_t* new_screen();

// LDR pin
constexpr uint8_t LDR_PIN = 34;

// RGB LED Pins
constexpr uint8_t CYD_LED_RED   = 4;
constexpr uint8_t CYD_LED_GREEN = 16;
constexpr uint8_t CYD_LED_BLUE  = 17;
constexpr uint8_t CYD_BACKLIGHT = 21;

// WiFi credentials
const char* ssid = "JK_BMS";
const char* password = "87654321";

// Timer vars
unsigned long previousMillis = 0;
const long interval = 2000;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // set LED states
  pinMode(CYD_BACKLIGHT, OUTPUT);
  pinMode(CYD_LED_RED, OUTPUT);
  pinMode(CYD_LED_GREEN, OUTPUT);
  pinMode(CYD_LED_BLUE, OUTPUT);
  // Set HIGH to turn them all off (inverted logic for CYD)
  digitalWrite(CYD_LED_RED, HIGH);
  digitalWrite(CYD_LED_GREEN, HIGH);
  digitalWrite(CYD_LED_BLUE, HIGH);
  digitalWrite(CYD_BACKLIGHT, LOW);
  
  // Start LVGL
  LVGL_CYD::begin(SCREEN_ORIENTATION);

  setup_layers();

  // Function to draw the GUI
  main_screen();
}

void loop() {
  lv_task_handler();  // let the GUI do its work

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    BMSData bms;
    if(bms.fetchBMSData("http://192.168.4.1/data")) {
      DEBUG_PRINTF("Pack Voltage: %f\n", bms.pack_voltage);
        lv_label_set_text_fmt(soc_label, "%.2f", bms.pack_voltage);
    }
    DEBUG_PRINTLN("Polling for data!");
  }
}
