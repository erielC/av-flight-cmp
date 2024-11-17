# av-flight-cmp

SEDS Avionics Team Flight Computer 24'-25' Season
<br/>

This repository is the Vinson main flight computer code for the arduino nano that will be guiding the rocket, the other two repositories linked below are the test STM chip under development and the rover computer (ESP-32). 
<ul>
  <li><a href="https://github.com/erielC/av-rover-cmp/tree/main" target="_blank">Rover Computer Repo</a></li>
</ul>
<br/>
<h3>Sensor Libraries Directory:</h3>
<ul>
  <h4>MMC5983 Magnetometer</h4>
  <li><a href="https://github.com/sparkfun/SparkFun_MMC5983MA_Magnetometer_Arduino_Library/tree/main" target="_blank"> Magnetometer Library</a></li>
  <li><a href="https://www.memsic.com/Public/Uploads/uploadfile/files/20220119/MMC5983MADatasheetRevA.pdf">Magnetometer Datasheet</a></li>
  <h4>MB85RC FRAM</h4>
  <li><a href="https://github.com/sosandroid/FRAM_MB85RC_I2C" target="_blank"> FRAM Library</a></li>
  <h4>BMP390 Barometer</h4>
  <li><a href="https://github.com/adafruit/Adafruit_BMP3XX" target="_blank"> Barometer Library</a></li>
  <li><a href="https://cdn-learn.adafruit.com/assets/assets/000/096/781/original/bst-bmp390-fl000.pdf?1604526926">BMP390 Datasheet #1 (Adafruit & Bosch)</a></li>
  <li><a href="https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp390-ds002.pdf">BMP390 Datasheet #2 (Bosch)</a></li>
  <h4>ASM330LHH Main IMU</h4>
  <li><a href="https://github.com/stm32duino/ASM330LHH/tree/main" target="_blank">ASM330 Main IMU</a></li>
  <li><a href="https://www.st.com/resource/en/datasheet/asm330lhh.pdf">Main IMU (ASM330) Datasheet</a></li>
  <li><a href="https://github.com/user-attachments/assets/08a4da38-90d2-46dd-91a4-b4015cfabe34">ASM330 Wiring Diagram</a></li>
  <h4>BMI323 Backup IMU</h4>
  <li><a href="https://github.com/boschsensortec/BMI323_SensorAPI" target="_blank">BMI323 IMU Library</a></li>
  <li><a href="https://github.com/adafruit/Adafruit_HTU21DF_Library/tree/master" target="_blank">HTU21D Humidity & Temperature Sensor</a></li>
</ul>
  The sparkfun Humidity sensor library can be used if the adafruit one proves to be too tedious so link will remain here.
  <br/>
  https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library
<br/>

<h3>GUIDELINES for Setting up platform.IO</h3>
<ul>
  <li>Make sure that there is only ONE .cpp file in your source file when you compile because if you have more .cpp and they contain the setup() & loop() functions then it will get confused and return an error.</li>
  <li>If files need to be stored and your intention is not to compile them place it in the archive folder as compiler only compiles src folder</li>
  <li>Unless your testing the code DO NOT place anything inside the test folder</li>
  <li>Any header files that you have written place them into the include folder as platformIO determines that's where the header files go</li>
</ul>
<h4>Sensor I2C Addresses:</h4>
<ul>
  <li>ASM330: 0x6A</li>
  <li>Magnetometer: 0x30</li>
  <li>BMP280: 0x76</li>
</ul>
