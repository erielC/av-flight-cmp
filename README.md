# av-flight-cmp

SEDS Avionics Team Flight Computer 2024-2025 Season

This repository is the Vinson main flight computer code for the Arduino Nano that will be guiding the rocket. The other repository linked below is the rover computer (ESP-32).

- [Rover Computer Repo](https://github.com/erielC/av-rover-cmp/tree/main)

---

### Sensor Libraries Directory:

#### MMC5983 Magnetometer

- [Magnetometer Library](https://github.com/sparkfun/SparkFun_MMC5983MA_Magnetometer_Arduino_Library/tree/main)
- [Magnetometer Datasheet](https://www.memsic.com/Public/Uploads/uploadfile/files/20220119/MMC5983MADatasheetRevA.pdf)

#### MB85RC FRAM

- [FRAM Library](https://github.com/sosandroid/FRAM_MB85RC_I2C)

#### BMP390 Barometer

- [Barometer Library](https://github.com/adafruit/Adafruit_BMP3XX)
- [BMP390 Datasheet #1 (Adafruit & Bosch)](https://cdn-learn.adafruit.com/assets/assets/000/096/781/original/bst-bmp390-fl000.pdf?1604526926)
- [BMP390 Datasheet #2 (Bosch)](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp390-ds002.pdf)

#### ASM330LHH Main IMU

- [ASM330 Main IMU](https://github.com/stm32duino/ASM330LHH/tree/main)
- [Main IMU (ASM330) Datasheet](https://www.st.com/resource/en/datasheet/asm330lhh.pdf)
- [ASM330 Wiring Diagram](https://github.com/user-attachments/assets/08a4da38-90d2-46dd-91a4-b4015cfabe34)

#### BMI323 Backup IMU

- [BMI323 IMU Library](https://github.com/boschsensortec/BMI323_SensorAPI)
- [HTU21D Humidity & Temperature Sensor](https://github.com/adafruit/Adafruit_HTU21DF_Library/tree/master)

_The SparkFun Humidity sensor library can be used if the Adafruit one proves too tedious, so the link is provided here:  
[https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library](https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library)_

---

### Sensor I2C Addresses:

- ASM330: `0x6A`
- Magnetometer: `0x30`
- BMP390: `0x76`

---

### GUIDELINES for Setting up Platform.IO

- **Single Main File:** Ensure there is only **ONE** `.cpp` file that contains `setup()` and `loop()` in your `src` directory. Multiple files with these functions will cause compilation errors.
- **Archive Storage:** If files are not intended for compilation, place them in the `archive` folder since only the `src` folder is compiled.
- **Test Folder Usage:** Unless you are actively testing, **do not** place any files in the `test` folder as it may cause errors.
- **Include Folder:** Any custom header files should be placed in the `include` folder, which is recognized by PlatformIO as the location for header files.
- **Corresponding File (.cpp) for Custom Header Files** for the `.cpp` file that corresponds to the header file in the `include` folder make sure that it is placed in the `src` folder so that platformIO can recognize it.

# Commenting Standards

This project uses C, C++, and Arduino code for embedded systems, and we adhere to a consistent commenting style to keep the codebase clear, maintainable, and easy to navigate.

## 1. Function-Level Comments

- **Format:** Use Doxygen-style comments (`/** ... */`) before each function.
- **Content:**
  - Briefly describe the function’s purpose, parameters, and return values.
  - Use `@brief` to summarize the function at a high level.
  - Use `@param` and `@return` tags to document function inputs/outputs.
  - For any known issues, planned work, or bugs, use `TODO(#issueNumber)` or `FIXME(#issueNumber)`.
  - Use `NOTE(name):` to highlight non-actionable observations or temporary conditions.

**Example:**

```c
/**
 * @brief Fetches data from a sensor and processes the result.
 *
 * Reads from the specified sensor pin, applies filtering, and returns the
 * processed value. This function is non-blocking.
 *
 * @param sensorPin The Arduino analog pin number where the sensor is connected.
 * @return The filtered sensor reading as an integer.
 *
 * NOTE(alice): Currently using a simple moving average filter. See #45 for a discussion on implementing a Kalman filter.
 * TODO(#101): Integrate a calibration routine to improve accuracy.
 * FIXME(#102): Handle sensor saturation conditions more gracefully.
 */
int readAndProcessSensor(int sensorPin) {
    // ...
}
```

## 2. Single-Level Comments (Above or Inline)

- **Format:** Use Doxygen-style comments (`//`) before each comment.
- **Content:**
  - Briefly describe the purpose of the code
  - (Case-by-case basis) Tag anyone who will work on this code

```c
/// This is a single-line Doxygen comment.
/// It documents myOtherVariable.
int myOtherVariable;
```

```c
int myVariable // Place the comment here
```

## 3. TODO/FIXME Comments

Use `TODO(#issueNumber)`or `FIXME(#issueNumber)` to mark pending tasks, enhancements, or additional steps you plan to implement.
**Conditions:**

- If going to use in a function multi-line comment you can place it inside as noted in #1 Function-Level Comments
- If going to be single line then mark it as the following below:

```c
int calculateChecksum(uint8_t data[], size_t length) {
    int checksum = 0;
    for (size_t i = 0; i < length; i++) {
        checksum += data[i]; // TODO(#789): Handle overflow for large data arrays.
    }
    return checksum;
}
```

or

```c
bool connectToWiFi(const char* ssid, const char* password) {
    bool success = WiFi.begin(ssid, password); // FIXME(#456): Implement retry logic for failed connections.
    return success;
}
```

- If needed for a multi-line `TODO/FIXME` comment then use the syntax as following to not clutter the code and hinder readbility:

```c
/**
 * FIXME(#456): Implement retry logic for failed connections.
 */
bool connectToWiFi(const char* ssid, const char* password) {
    bool success = WiFi.begin(ssid, password);
    return success;
}

```
