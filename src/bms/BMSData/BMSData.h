#ifndef BMSDATA_H
#define BMSDATA_H
#include <Arduino.h>


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

  bool fetchBMSData(const char* url);
};

#endif // BMSDATA_H