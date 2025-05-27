#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <MPU9250_WE.h>

// Define I2C address for MPU9250
#define MPU9250_ADDR 0x68

// Define a custom I2C bus for MPU9250 (on A4 and A5)
TwoWire MPUWire = TwoWire();

// Create MPU9250 object using custom wire
MPU9250_WE myMPU9250 = MPU9250_WE(&MPUWire, MPU9250_ADDR);

void setup() {
  Serial.begin(115200);

  // Start the secondary bus for MPU9250 on A4/A5
  MPUWire.begin();

	delay(500);

  // Init MPU9250
  if (!myMPU9250.init()) {
    Serial.println("MPU9250 does not respond");
  } else {
    Serial.println("MPU9250 is connected");
  }

  if (!myMPU9250.initMagnetometer()) {
    Serial.println("Magnetometer does not respond");
  } else {
    Serial.println("Magnetometer is connected");
  }

  Serial.println("Position your MPU9250 flat - calibrating...");
  delay(500);
  myMPU9250.autoOffsets();
  Serial.println("Done!");

  myMPU9250.enableGyrDLPF();
  myMPU9250.setGyrDLPF(MPU9250_DLPF_6);
  myMPU9250.setSampleRateDivider(5);
  myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);
  myMPU9250.enableAccDLPF(true);
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);
  myMPU9250.setMagOpMode(AK8963_CONT_MODE_100HZ);

  delay(5000);
}

void loop() {
  // MPU9250 Data
  xyzFloat gValue = myMPU9250.getGValues();
  xyzFloat gyr = myMPU9250.getGyrValues();
  xyzFloat magValue = myMPU9250.getMagValues();
  float tempMPU = myMPU9250.getTemperature();
  float resultantG = myMPU9250.getResultantG(gValue);

  Serial.println("MPU9250 Acceleration in g (x,y,z):");
  Serial.print(gValue.x); Serial.print("   ");
  Serial.print(gValue.y); Serial.print("   ");
  Serial.println(gValue.z);
  Serial.print("Resultant g: "); Serial.println(resultantG);

  Serial.println("Gyroscope data in degrees/s: ");
  Serial.print(gyr.x); Serial.print("   ");
  Serial.print(gyr.y); Serial.print("   ");
  Serial.println(gyr.z);

  Serial.println("Magnetometer Data in µTesla: ");
  Serial.print(magValue.x); Serial.print("   ");
  Serial.print(magValue.y); Serial.print("   ");
  Serial.println(magValue.z);

  Serial.print("MPU Temperature in °C: ");
  Serial.println(tempMPU);

  Serial.println("********************************************");

  delay(1000);
}
