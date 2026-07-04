#ifndef _COMM_NVM_H_
#define _COMM_NVM_H_

#include <Preferences.h>
#include <WString.h>
#include <limits>
#include "../../datalayer/datalayer.h"
#include "../../devboard/utils/events.h"
#include "../../devboard/utils/logging.h"
#include "../../devboard/wifi/wifi.h"

/**
 * @brief Initialization of setting storage
 *
 * @param[in] void
 *
 * @return void
 */
void init_stored_settings();

/**
 * @brief Store settings of equipment stop button
 *
 * @param[in] void
 *
 * @return void
 */
void store_settings_equipment_stop();

/**
 * @brief Store settings
 *
 * @param[in] void
 *
 * @return void
 */
void store_settings();

// Wraps the Preferences object begin/end calls, so that the scope of this object
// runs them automatically (via constructor/destructor).
class BatteryEmulatorSettingsStore {
 public:
  BatteryEmulatorSettingsStore(bool readOnly = false) {
    if (!settings.begin("batterySettings", readOnly)) {
      set_event(EVENT_PERSISTENT_SAVE_INFO, 0);
    }
  }

  ~BatteryEmulatorSettingsStore() { settings.end(); }

  void clearAll() {
    settings.clear();
    settingsUpdated = true;
  }

  int32_t getInt(const char* name, int32_t defaultValue) {
    return settings.isKey(name) ? settings.getInt(name, defaultValue) : defaultValue;
  }

  void saveInt(const char* name, int32_t value) {
    auto oldValue = getInt(name, std::numeric_limits<int32_t>::max());
    if (value != oldValue) {
      settings.putInt(name, value);
      settingsUpdated = true;
    }
  }

  uint32_t getUInt(const char* name, uint32_t defaultValue) {
    return settings.isKey(name) ? settings.getUInt(name, defaultValue) : defaultValue;
  }

  void saveUInt(const char* name, uint32_t value) {
    auto oldValue = getUInt(name, std::numeric_limits<uint32_t>::max());
    settings.putUInt(name, value);
    settingsUpdated = settingsUpdated || value != oldValue;
  }

  bool settingExists(const char* name) { return settings.isKey(name); }

  bool getBool(const char* name, bool defaultValue = false) {
    return settings.isKey(name) ? settings.getBool(name, defaultValue) : defaultValue;
  }

  void saveBool(const char* name, bool value) {
    auto oldValue = getBool(name, false);
    settings.putBool(name, value);
    settingsUpdated = settingsUpdated || value != oldValue;
  }

  String getString(const char* name) { return getString(name, ""); }

  String getString(const char* name, const char* defaultValue) {
    return settings.isKey(name) ? settings.getString(name, defaultValue) : String(defaultValue);
  }

  void saveString(const char* name, const char* value) {
    auto oldValue = getString(name, "");
    settings.putString(name, value);
    settingsUpdated = settingsUpdated || String(value) != oldValue;
  }

  bool were_settings_updated() const { return settingsUpdated; }

 private:
  Preferences settings;

  // To track if settings were updated
  bool settingsUpdated = false;
};

// 2026-07-04: Added by Claude — opt-in GitHub version check (default off)
extern bool update_check_enabled;

// 2026-06-04: Added by Claude — web-configurable GPIO pin globals for ESP32-S3 generic board
#ifdef HW_ESP32S3_GENERIC
extern int32_t generic_pin_can_tx;
extern int32_t generic_pin_can_rx;
extern int32_t generic_pin_rs485_tx;
extern int32_t generic_pin_rs485_rx;
extern int32_t generic_pin_rs485_de;
extern int32_t generic_pin_mcp_sck;
extern int32_t generic_pin_mcp_mosi;
extern int32_t generic_pin_mcp_miso;
extern int32_t generic_pin_mcp_cs;
extern int32_t generic_pin_mcp_int;
extern int32_t generic_pin_mcpfd_sck;
extern int32_t generic_pin_mcpfd_sdi;
extern int32_t generic_pin_mcpfd_sdo;
extern int32_t generic_pin_mcpfd_cs;
extern int32_t generic_pin_mcpfd_int;
extern int32_t generic_pin_mcpfd2_sck;
extern int32_t generic_pin_mcpfd2_sdi;
extern int32_t generic_pin_mcpfd2_sdo;
extern int32_t generic_pin_mcpfd2_cs;
extern int32_t generic_pin_mcpfd2_int;
extern int32_t generic_pin_pos_cnt;
extern int32_t generic_pin_neg_cnt;
extern int32_t generic_pin_prech;
extern int32_t generic_pin_bms_pwr;
extern int32_t generic_pin_led;
extern int32_t generic_pin_display_sda;
extern int32_t generic_pin_display_scl;
// W5500 Ethernet pins and bus selector
extern int32_t generic_pin_w5500_sck;
extern int32_t generic_pin_w5500_mosi;
extern int32_t generic_pin_w5500_miso;
extern int32_t generic_pin_w5500_cs;
extern int32_t generic_pin_w5500_int;
extern int32_t generic_pin_w5500_rst;
extern int32_t generic_w5500_bus;  // 1=SPI2_HOST(FSPI), 2=SPI3_HOST(HSPI), default HSPI
extern bool    ethernet_enabled;
#endif

#endif
