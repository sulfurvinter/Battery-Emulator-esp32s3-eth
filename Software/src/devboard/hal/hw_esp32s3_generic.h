// 2026-06-04: Ported by Claude to v10.12.0 (originally created 2026-05-22) — generic ESP32-S3 HAL with web-configurable GPIO pins
// Supports CAN native, RS485, MCP2515, MCP2517FD (x2) add-on, contactor/BMS power GPIO, W5500 Ethernet
// All pin assignments are configured at runtime via the web settings interface

#ifndef __HW_ESP32S3_GENERIC_H__
#define __HW_ESP32S3_GENERIC_H__

#include "hal.h"
#include "../../communication/nvm/comm_nvm.h"

class Esp32S3GenericHal : public Esp32Hal {
 public:
  const char* name() { return "ESP32-S3 Generic (web-configurable pins)"; }

  // CAN native
  virtual gpio_num_t CAN_TX_PIN() { return (gpio_num_t)generic_pin_can_tx; }
  virtual gpio_num_t CAN_RX_PIN() { return (gpio_num_t)generic_pin_can_rx; }
  virtual gpio_num_t CAN_SE_PIN() { return GPIO_NUM_NC; }

  // RS485
  virtual gpio_num_t RS485_TX_PIN() { return (gpio_num_t)generic_pin_rs485_tx; }
  virtual gpio_num_t RS485_RX_PIN() { return (gpio_num_t)generic_pin_rs485_rx; }
  virtual gpio_num_t RS485_DE_PIN() { return (gpio_num_t)generic_pin_rs485_de; }
  virtual gpio_num_t RS485_EN_PIN() { return GPIO_NUM_NC; }
  virtual gpio_num_t RS485_SE_PIN() { return GPIO_NUM_NC; }

  // MCP2515 (classic CAN add-on)
  virtual gpio_num_t MCP2515_SCK()  { return (gpio_num_t)generic_pin_mcp_sck; }
  virtual gpio_num_t MCP2515_MOSI() { return (gpio_num_t)generic_pin_mcp_mosi; }
  virtual gpio_num_t MCP2515_MISO() { return (gpio_num_t)generic_pin_mcp_miso; }
  virtual gpio_num_t MCP2515_CS()   { return (gpio_num_t)generic_pin_mcp_cs; }
  virtual gpio_num_t MCP2515_INT()  { return (gpio_num_t)generic_pin_mcp_int; }

  // MCP2517FD / MCP2518FD (CAN FD add-on #1)
  virtual gpio_num_t MCP2517_SCK() { return (gpio_num_t)generic_pin_mcpfd_sck; }
  virtual gpio_num_t MCP2517_SDI() { return (gpio_num_t)generic_pin_mcpfd_sdi; }
  virtual gpio_num_t MCP2517_SDO() { return (gpio_num_t)generic_pin_mcpfd_sdo; }
  virtual gpio_num_t MCP2517_CS()  { return (gpio_num_t)generic_pin_mcpfd_cs; }
  virtual gpio_num_t MCP2517_INT() { return (gpio_num_t)generic_pin_mcpfd_int; }

  // MCP2517FD / MCP2518FD (CAN FD add-on #2)
  virtual gpio_num_t MCP2517_SCK2() { return (gpio_num_t)generic_pin_mcpfd2_sck; }
  virtual gpio_num_t MCP2517_SDI2() { return (gpio_num_t)generic_pin_mcpfd2_sdi; }
  virtual gpio_num_t MCP2517_SDO2() { return (gpio_num_t)generic_pin_mcpfd2_sdo; }
  virtual gpio_num_t MCP2517_CS2()  { return (gpio_num_t)generic_pin_mcpfd2_cs; }
  virtual gpio_num_t MCP2517_INT2() { return (gpio_num_t)generic_pin_mcpfd2_int; }

  // NeoPixel LED
  virtual gpio_num_t LED_PIN() { return (gpio_num_t)generic_pin_led; }

  // I2C OLED display (SSD1306)
  virtual gpio_num_t DISPLAY_SDA_PIN() { return (gpio_num_t)generic_pin_display_sda; }
  virtual gpio_num_t DISPLAY_SCL_PIN() { return (gpio_num_t)generic_pin_display_scl; }

  // W5500 Ethernet (SPI bus set separately via generic_w5500_bus NVS key)
  // W5500 pins are NOT routed through the HAL pin-alloc system — ETH class manages them directly.

  // Contactor / relay control
  virtual gpio_num_t POSITIVE_CONTACTOR_PIN() { return (gpio_num_t)generic_pin_pos_cnt; }
  virtual gpio_num_t NEGATIVE_CONTACTOR_PIN() { return (gpio_num_t)generic_pin_neg_cnt; }
  virtual gpio_num_t PRECHARGE_PIN()           { return (gpio_num_t)generic_pin_prech; }
  virtual gpio_num_t BMS_POWER()               { return (gpio_num_t)generic_pin_bms_pwr; }

  // All interfaces available; active ones depend on pin assignment
  std::vector<comm_interface> available_interfaces() {
    return {comm_interface::CanNative, comm_interface::RS485, comm_interface::Modbus,
            comm_interface::CanAddonMcp2515, comm_interface::CanFdAddonMcp2518,
            comm_interface::CanFdAddonMcp2518_2};
  }

  virtual const char* name_for_comm_interface(comm_interface comm) {
    switch (comm) {
      case comm_interface::CanNative:           return "CAN (Native)";
      case comm_interface::CanFdNative:         return "";
      case comm_interface::CanAddonMcp2515:     return "CAN (MCP2515 add-on)";
      case comm_interface::CanFdAddonMcp2518:   return "CAN FD #1 (MCP2517/18 add-on)";
      case comm_interface::CanFdAddonMcp2518_2: return "CAN FD #2 (MCP2517/18 add-on)";
      case comm_interface::Modbus:              return "Modbus";
      case comm_interface::RS485:               return "RS485";
      case comm_interface::Highest:             return "";
    }
    return Esp32Hal::name_for_comm_interface(comm);
  }
};

#define HalClass Esp32S3GenericHal

/* ----- Error checks below, don't change (can't be moved to separate file) ----- */
#ifndef HW_CONFIGURED
#define HW_CONFIGURED
#else
#error Multiple HW defined! Please select a single HW
#endif

#endif
