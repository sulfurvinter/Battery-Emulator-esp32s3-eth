# Battery-Emulator ⚡🔋 — esp32s3_generic_330 Fork

> ⚠️ **WARNING: PROTOTYPE / VIBE-CODED SOFTWARE — USE AT YOUR OWN RISK** ⚠️
>
> This firmware is experimental, AI-assisted prototype code. It has not been formally reviewed,
> audited, or certified for safety. It is provided **as-is with no warranty of any kind**.
> You are solely responsible for any damage to property, equipment, or persons resulting from
> its use. **Do not use this in safety-critical or production environments.**

> **This is an independent fork based on [dalathegreat/Battery-Emulator](https://github.com/dalathegreat/Battery-Emulator) v10.12.0.**
> See [Changes from upstream](#changes-from-upstream) below for what has been added.

---

## Changes from upstream

This fork adds a new hardware target `esp32s3_generic_330` for generic ESP32-S3 development boards where no specific pinout is assumed. All GPIO pins are assigned at runtime through the web settings interface — nothing is hardcoded in firmware.

### New PlatformIO environment

`esp32s3_generic_330` — builds for any ESP32-S3 board with 16MB flash, USB CDC serial.

Build:
```bash
platformio run -e esp32s3_generic_330
```

Output files:
- `.pio/build/esp32s3_generic_330/firmware.factory.bin` — **use this for first-time / full flash** (includes bootloader + partition table)
- `.pio/build/esp32s3_generic_330/firmware.bin` — **use this for OTA updates only**

### Web-configurable GPIO pins

After flashing, connect to WiFi AP `BatteryEmulator` (password `123456789`) and open `http://192.168.4.1`. Go to **Settings → GPIO Pin Configuration** to assign pins before selecting a battery or inverter type. All pins default to -1 (not connected). Settings are stored in NVS and survive reboots.

| Interface | Configurable pins |
|-----------|------------------|
| CAN Native (TWAI) | TX, RX |
| RS485 | TX, RX, DE |
| MCP2515 CAN add-on (SPI) | SCK, MOSI, MISO, CS, INT |
| MCP2517FD / MCP2518FD CAN-FD add-on (SPI) | SCK, SDI, SDO, CS, INT |
| Positive contactor | GPIO pin |
| Negative contactor | GPIO pin |
| Precharge relay | GPIO pin |
| BMS power control | GPIO pin |
| NeoPixel status LED | GPIO pin |
| SSD1306 OLED display (I2C, 128x64) | SDA, SCL |

The OLED display is auto-detected at boot — if the configured pins are unset or the display is absent, it fails silently.

### Files changed from upstream

| File | Change |
|------|--------|
| `platformio.ini` | Added `esp32s3_generic_330` environment |
| `Software/Software.cpp` | Added `HW_ESP32S3_GENERIC` to hardware guard and USB CDC serial timeout |
| `Software/src/devboard/hal/hal.cpp` | Added `HW_ESP32S3_GENERIC` HAL instantiation |
| `Software/src/devboard/hal/hw_esp32s3_generic.h` | **New file** — HAL class with all pins reading from NVS globals |
| `Software/src/communication/nvm/comm_nvm.h` | Added 22 `extern int32_t generic_pin_*` declarations |
| `Software/src/communication/nvm/comm_nvm.cpp` | Added pin globals (default -1) and NVS reads in `init_stored_settings()` |
| `Software/src/devboard/webserver/settings_html.cpp` | Added GPIO dropdown UI, processor cases, `LED_PIN_SETTING` and `PIN_CONFIG_SETTING` macros |
| `Software/src/devboard/webserver/webserver.cpp` | Added `intSettingNames[]` and `saveInt` loop for signed pin values |

### Serial output

USB CDC on `/dev/ttyACM0` at 115200 baud. No separate UART adapter needed.

### OTA update

```bash
python3 espota.py -i 192.168.4.1 -f .pio/build/esp32s3_generic_330/firmware.bin
```

Or upload `firmware.bin` via browser at `http://192.168.4.1/update`.

---

![GitHub release](https://img.shields.io/github/v/release/sulfurvinter/Battery-Emulator-esp32s3-eth?color=%23008000)
![GitHub Repo stars](https://img.shields.io/github/stars/sulfurvinter/Battery-Emulator-esp32s3-eth?style=flat&color=%23128512)
![Static Badge](https://img.shields.io/badge/made-with_love-blue?color=%23008000)

## What is Battery Emulator?

Many manufacturers sell home battery systems to enable homeowners to store power collected from the grid, or renewable sources, to use at times when electricity demand is higher. However almost all of these home battery systems charge a high cost for every kilowatt hour (kWh) of capacity you buy.

At the same time, EV manufacturers have been putting high capacity battery packs into their cars, with no firm plan for what should happen to those batteries if the car is damaged in a crash, or reaches the end of its life as a vehicle.

**Battery Emulator** enables EV battery packs to be repurposed for stationary storage. It acts as a translation layer between the EV battery and the home inverter. This makes it extremely cheap and easy to use large EV batteries in a true plug'n'play fashion!

> [!CAUTION]
> Working with high voltage is dangerous. Always follow local laws and regulations regarding high voltage work. If you are unsure about the rules in your country, consult a licensed electrician for more information.


## Quickstart guide 📜
- Pick a [supported inverter](https://github.com/dalathegreat/Battery-Emulator/wiki#supported-inverters-list) (solar panels optional) :sun_with_face: 
- Pick a [supported battery](https://github.com/dalathegreat/Battery-Emulator/wiki#supported-batteries-list) :battery: 
- Order the Battery-Emulator [compatible hardware](https://github.com/dalathegreat/Battery-Emulator/wiki#where-do-i-get-the-hardware-needed) :robot: 
- Follow the [installation guidelines](https://github.com/dalathegreat/Battery-Emulator/wiki/Installation-guidelines) section for how to install and commission your battery properly :notebook: 

## Installation basics 🪛
1. Connect your Battery Emulator hardware to your EV battery
2. Connect your Battery Emulator hardware to your inverter
3. Wire up high voltage cable between the inverter and the battery
4. Add a low voltage power supply to your Battery Emulator hardware
5. Configure any additional requirements to allow Battery Emulator to switch on your EV battery (also referred to as 'closing contactors')
6. Enjoy a big cheap grid connected battery!

For examples showing wiring, see each battery type's own Wiki page. For instance the [Nissan LEAF page](https://github.com/dalathegreat/Battery-Emulator/wiki/Battery:-Nissan-LEAF---e%E2%80%90NV200)

## How to install the software 💻

### Which binary do I need?

| File | When to use |
|------|-------------|
| `BE_vX.X.X_ESP32S3Generic.factory.bin` | **First-time flash** — full image including bootloader and partition table. Use this on a blank or previously flashed board. |
| `BE_vX.X.X_ESP32S3Generic.ota.bin` | **Update only** — smaller app image, uploaded over-the-air. Requires firmware already running on the board. |

Download both from the [latest release](https://github.com/sulfurvinter/Battery-Emulator-esp32s3-eth/releases/latest).

---

### Option A — Flash via browser (easiest) 🌐

Use [ESPConnect](https://thelastoutpostworkshop.github.io/ESPConnect/) — a browser-based flashing tool that works without installing any software (requires Chrome or Edge with Web Serial support).

1. Connect the ESP32-S3 to your PC via USB
2. Open [ESPConnect](https://thelastoutpostworkshop.github.io/ESPConnect/) in Chrome or Edge
3. Click **Connect**, select your COM port
4. Upload `BE_vX.X.X_ESP32S3Generic.factory.bin` at offset `0x0`
5. Click **Flash** and wait for it to complete

---

### Option B — Flash via esptool (command line) 🖥️

Requires Python and [esptool](https://github.com/espressif/esptool) (`pip install esptool`).

```bash
esptool --chip esp32s3 --port /dev/ttyUSB0 --baud 921600 write_flash 0x0 BE_vX.X.X_ESP32S3Generic.factory.bin
```

On Windows replace `/dev/ttyUSB0` with the correct COM port (e.g. `COM3`).

---

### Option C — OTA update (over the air) 📡

Once the firmware is running, you can update without a USB cable:

1. Open the device web UI at `http://<device-ip>` (or `http://192.168.4.1` if using the WiFi AP)
2. Click **Perform OTA update**
3. Upload `BE_vX.X.X_ESP32S3Generic.ota.bin`

> **Note:** Always use the `.factory.bin` for the first flash. Using the OTA binary for a first-time flash will result in a non-booting board.

---

### After flashing

1. Connect to WiFi AP `BatteryEmulator` (password: `123456789`)
2. Open `http://192.168.4.1` → **Settings → GPIO Pin Configuration** and assign your pins
3. Select your battery and inverter type
4. Connect your battery and inverter and you are done! 🔋⚡

## Dependencies 📖
This code uses the following excellent libraries: 
- [adafruit/Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) LGPL-3.0 license
- [ayushsharma82/ElegantOTA](https://github.com/ayushsharma82/ElegantOTA) AGPL-3.0 license 
- [bblanchon/ArduinoJson](https://github.com/bblanchon/ArduinoJson) MIT-License
- [eModbus/eModbus](https://github.com/eModbus/eModbus) MIT-License
- [ESP32Async/AsyncTCP](https://github.com/ESP32Async/AsyncTCP) LGPL-3.0 license
- [ESP32Async/ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer) LGPL-3.0 license
- [pierremolinaro/acan-esp32](https://github.com/pierremolinaro/acan-esp32) MIT-License
- [pierremolinaro/acan2517FD](https://github.com/pierremolinaro/acan2517FD) MIT-License

It is also based on the information found in the following excellent repositories/websites:
- https://gitlab.com/pelle8/inverter_resources //new url
- https://github.com/burra/byd_battery
- https://github.com/flodorn/TeslaBMSV2
- https://github.com/SunshadeCorp/can-service
- https://github.com/openvehicles/Open-Vehicle-Monitoring-System-3
- https://github.com/dalathegreat/leaf_can_bus_messages
- https://github.com/rand12345/solax_can_bus
- https://github.com/Tom-evnut/BMWI3BMS/ SMA-CAN
- https://github.com/FozzieUK/FoxESS-Canbus-Protocol FoxESS-CAN
- https://github.com/maciek16c/hyundai-santa-fe-phev-battery
- https://github.com/ljames28/Renault-Zoe-PH2-ZE50-Canbus-LBC-Information
- Renault Zoe CAN Matrix https://docs.google.com/spreadsheets/u/0/d/1Qnk-yzzcPiMArO-QDzO4a8ptAS2Sa4HhVu441zBzlpM/edit?pli=1#gid=0
- Pylon hacking https://www.eevblog.com/forum/programming/pylontech-sc0500-protocol-hacking/

## Like the upstream project? 💖
This fork is based on dalathegreat's Battery-Emulator. If you find it useful, consider supporting the original author on Patreon:

<a href="https://www.patreon.com/dala">
	<img src="https://c5.patreon.com/external/logo/become_a_patron_button@2x.png" width="160">
</a>
