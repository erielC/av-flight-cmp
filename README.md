# av-flight-cmp

SEDS Avionics Team Flight Computer 24'-25' Season
<br/>

This repository is the Vinson main flight computer code for the arduino nano that will be guiding the rocket, the other two repositories linked below are the test STM chip under development and the rover computer (ESP-32). 
<ul>
  <li><a href="" target="_blank"></a></li>
</ul>
<br/>
<h3>Sensor Libraries Directory:</h3>
<ul>
  <li><a href="https://github.com/sparkfun/SparkFun_MMC5983MA_Magnetometer_Arduino_Library/tree/main" target="_blank">MMC5983 Magnetometer</a></li>
  <li><a href="https://github.com/sosandroid/FRAM_MB85RC_I2C" target="_blank">MB85RC FRAM</a></li>
  <li><a href="https://github.com/libdriver/bmp390?tab=readme-ov-file#Install" target="_blank">BMP390 Barometer</a></li>
  <li><a href="https://github.com/stm32duino/ASM330LHH/tree/main" target="_blank">ASM330 Main IMU</a></li>
  <li><a href="https://github.com/boschsensortec/BMI323_SensorAPI" target="_blank">BMI323 Secondary IMU</a></li>
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
<h4>10.21.24</h4>
<br/>
Creation of the github repository for testing git commands, version control source code.
<br/>
Objectives:
<ul>
  <li>Understand how arduino/sensor libraries work</li>
  <li>Be able to store them locally on computer to work with no internet and be able to boot up on any computer</li>
</ul>
<br/>
<h4>10.26.24</h4>
<br/>
Completed: 
<ul>
  <li>Was able to store them locally on computer with no internet and able to boot them up on any computer as long as it has VSCode extension platformIO</li>
  <li>Found all the libraries for the sensors that are on custom PCB</li>
</ul>
<br>
Objectives:
<ul>
  <li>Use a budget camera capble of recording/taking photos</li>
  <li>Familiarize with the code of camera and how to store the data on SD card</li>
</ul>
