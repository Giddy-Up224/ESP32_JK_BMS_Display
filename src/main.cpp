/** A sketch for reading the JSON data from the JKBMS sketch that's hosting
  * a webserver. Is very basic as of now. I need to add a tabview and tables
  * for displaying the data.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LVGL_CYD.h>

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

class BMSData {
  public:
    float pack_voltage;
    float battery_power;
    float charge_current;
    int percent_remain;
    float capacity_remain;
    float nominal_capacity;
    float cycle_count;
    float cycle_capacity;
    String uptime;
    float battery_t1;
    float battery_t2;
    float mos_temp;
    std::vector<float> cell_voltages;
    float average_cell_voltage;
    float delta_cell_voltage;
    std::vector<float> wire_resist;
    bool charge;
    bool discharge;
    bool balance;
    int balancing_action;
    float balance_curr;
    int cell_count;
    float total_battery_capacity;
    float balance_trigger_voltage;
    float balance_starting_voltage;
    float max_charge_current;
    float max_discharge_current;
    float max_balance_current;
    float cell_undervoltage_protection;
    float cell_undervoltage_recovery;
    float cell_overvoltage_protection;
    float cell_overvoltage_recovery;
    float power_off_voltage;
    float charge_overcurrent_protection_delay;
    float charge_overcurrent_protection_recovery_time;
    float discharge_overcurrent_protection_delay;
    float discharge_overcurrent_protection_recovery_time;
    float short_circuit_protection_recovery_time;
    float charge_overtemperature_protection;
    float charge_overtemperature_protection_recovery;
    float discharge_overtemperature_protection;
    float discharge_overtemperature_protection_recovery;
    float charge_undertemperature_protection;
    float charge_undertemperature_protection_recovery;
    float power_tube_overtemperature_protection;
    float power_tube_overtemperature_protection_recovery;
    float short_circuit_protection_delay;

  bool fetchBMSData(const char* url) {
    HTTPClient http;
    http.begin(url); // Sender ESP32 IP
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      DynamicJsonDocument doc(4096);
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        // Now you can access the JSON fields
        JsonObject bms = doc["c8:47:80:23:4f:95"]; // Use your MAC address
        pack_voltage = bms["battery_voltage"];
        battery_power = bms["battery_power"];
        charge_current = bms["charge_current"];
        percent_remain = bms["percent_remain"];
        capacity_remain = bms["capacity_remain"];
        nominal_capacity = bms["nominal_capacity"];
        cycle_count = bms["cycle_count"];
        cycle_capacity = bms["cycle_capacity"];
        uptime = bms["uptime"].as<String>();
        battery_t1 = bms["battery_t1"];
        battery_t2 = bms["battery_t2"];
        mos_temp = bms["mos_temp"];
        // Parse cell_voltages as JsonArray
        cell_voltages.clear();
        JsonArray cv = bms["cell_voltages"];
        for (float v : cv) cell_voltages.push_back(v);
        average_cell_voltage = bms["average_cell_voltage"];
        delta_cell_voltage = bms["delta_cell_voltage"];
        // Parse wire resistances as JsonArray
        wire_resist.clear();
        JsonArray wr = bms["wire_resist"];
        for (float r : wr) wire_resist.push_back(r);
        charge = bms["charge"];
        discharge = bms["discharge"];
        balance = bms["balance"];
        balancing_action = bms["balancing_action"];
        balance_curr = bms["balance_curr"];
        cell_count = bms["cell_count"];
        total_battery_capacity = bms["total_battery_capacity"];
        balance_trigger_voltage = bms["balance_trigger_voltage"];
        balance_starting_voltage = bms["balance_starting_voltage"];
        max_charge_current = bms["max_charge_current"];
        max_discharge_current = bms["max_discharge_current"];
        max_balance_current = bms["max_balance_current"];
        cell_undervoltage_protection = bms["cell_undervoltage_protection"];
        cell_undervoltage_recovery = bms["cell_undervoltage_recovery"];
        cell_overvoltage_protection = bms["cell_overvoltage_protection"];
        cell_overvoltage_recovery = bms["cell_overvoltage_recovery"];
        power_off_voltage = bms["power_off_voltage"];
        charge_overcurrent_protection_delay = bms["charge_overcurrent_protection_delay"];
        charge_overcurrent_protection_recovery_time = bms["charge_overcurrent_protection_recovery_time"];
        discharge_overcurrent_protection_delay = bms["discharge_overcurrent_protection_delay"];
        discharge_overcurrent_protection_recovery_time = bms["discharge_overcurrent_protection_recovery_time"];
        short_circuit_protection_recovery_time = bms["short_circuit_protection_recovery_time"];
        charge_overtemperature_protection = bms["charge_overtemperature_protection"];
        charge_overtemperature_protection_recovery = bms["charge_overtemperature_protection_recovery"];
        discharge_overtemperature_protection = bms["discharge_overtemperature_protection"];
        discharge_overtemperature_protection_recovery = bms["discharge_overtemperature_protection_recovery"];
        charge_undertemperature_protection = bms["charge_undertemperature_protection"];
        charge_undertemperature_protection_recovery = bms["charge_undertemperature_protection_recovery"];
        power_tube_overtemperature_protection = bms["power_tube_overtemperature_protection"];
        power_tube_overtemperature_protection_recovery = bms["power_tube_overtemperature_protection_recovery"];
        short_circuit_protection_delay = bms["short_circuit_protection_delay"];
        http.end();
        return true;
      }
      http.end();
      return false;
    }return false;
  }
};


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
