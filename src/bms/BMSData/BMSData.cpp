#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "BMSData.h"

bool BMSData::fetchBMSData(const char* url) {
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