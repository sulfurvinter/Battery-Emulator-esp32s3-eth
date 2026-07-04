// 2026-06-04: Created by Claude — W5500 SPI Ethernet support for esp32s3_generic_330
// Uses the ESP32 Arduino ETH class (built into the framework) with W5500 over SPI.
// W5500 must be on a separate SPI bus from the CAN chips to avoid ISR-level SPI contention.
// Default: W5500 on SPI3_HOST (HSPI), CAN chips on SPI2_HOST (FSPI).

#ifdef HW_ESP32S3_GENERIC

#include "comm_ethernet.h"
#include <ETH.h>
#include "../../communication/nvm/comm_nvm.h"
#include "../utils/events.h"
#include "../utils/logging.h"

// ethernet_enabled is defined and owned by comm_nvm.cpp
bool ethernet_link_up = false;

static bool eth_init_done = false;

static void onEthEvent(arduino_event_id_t event, arduino_event_info_t info) {
  switch (event) {
    case ARDUINO_EVENT_ETH_CONNECTED:
      ethernet_link_up = true;
      logging.println("Ethernet: link up");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      logging.print("Ethernet: IP address: ");
      logging.println(ETH.localIP().toString());
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      ethernet_link_up = false;
      logging.println("Ethernet: link down");
      break;
    default:
      break;
  }
}

void init_ethernet() {
  if (!ethernet_enabled) {
    return;
  }
  if (generic_pin_w5500_cs == -1 || generic_pin_w5500_sck == -1 ||
      generic_pin_w5500_mosi == -1 || generic_pin_w5500_miso == -1) {
    logging.println("Ethernet: SPI pins not fully configured (need CS, SCK, MOSI, MISO) — skipping");
    return;
  }

  Network.onEvent(onEthEvent);

  bool ok = ETH.begin(
      ETH_PHY_W5500,
      1,                                    // phy_addr (arbitrary for SPI PHY)
      (int)generic_pin_w5500_cs,
      (int)generic_pin_w5500_int,           // -1 = polled mode
      (int)generic_pin_w5500_rst,           // -1 = no reset pin
      (spi_host_device_t)generic_w5500_bus, // SPI2_HOST=FSPI or SPI3_HOST=HSPI
      (int)generic_pin_w5500_sck,
      (int)generic_pin_w5500_miso,
      (int)generic_pin_w5500_mosi,
      20                                    // 20 MHz SPI clock
  );

  if (ok) {
    logging.println("Ethernet: W5500 initialized, waiting for link...");
    eth_init_done = true;
  } else {
    logging.println("Ethernet: W5500 init failed — check pins and SPI bus");
  }
}

void ethernet_monitor() {
  if (!ethernet_enabled || !eth_init_done) {
    return;
  }
  // Link state is handled by event callbacks; nothing to poll here.
  // Could add periodic link-check logging if needed.
}

#endif  // HW_ESP32S3_GENERIC
