// 2026-06-04: Created by Claude — W5500 SPI Ethernet support for esp32s3_generic_330
#ifndef COMM_ETHERNET_H
#define COMM_ETHERNET_H

#ifdef HW_ESP32S3_GENERIC

#include <stdbool.h>

extern bool ethernet_enabled;
extern bool ethernet_link_up;

void init_ethernet();
void ethernet_monitor();

#endif  // HW_ESP32S3_GENERIC
#endif  // COMM_ETHERNET_H
