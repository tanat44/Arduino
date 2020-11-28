#include<Wire.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int oX, oY, oZ, oGX, oGY, oGZ;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void readImu() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)


}

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

  oX = 0;
  oY = 0;
  oZ = 0;
  oGX = 0;
  oGY = 0;
  oGZ = 0;

  delay(1000);

  // Calibrate ground level
  for (int i=0; i < 200; ++i) {
    readImu();
    oX = (( (float) oX * i ) + AcX ) / (i+1);
    oY = (( (float) oY * i ) + AcY ) / (i+1);
    oZ = (( (float) oZ * i ) + AcZ ) / (i+1);
    oGX = (( (float) oGX * i ) + GyX ) / (i+1);
    oGY = (( (float) oGY * i ) + GyY ) / (i+1);
    oGZ = (( (float) oGZ * i ) + GyZ ) / (i+1);
  }
}




void loop(){

  readImu();

  GyX = (GyX - oGX) / 10;
  GyY = (GyY - oGY) / 10;
  GyZ = (GyZ - oGZ) / 10;

  // current a vector
  float x = AcX;
  float y = AcY;
  float z = AcZ;
  float absV = sqrt( x*x + y*y + z*z );
  x = x/absV;
  y = y/absV;
  z = z/absV;

  // original a vector
  float xo = oX;
  float yo = oY;
  float zo = oZ;
  float absVo = sqrt( xo*xo + yo*yo + zo*zo );
  xo = xo/absVo;
  yo = yo/absVo;
  zo = zo/absVo;
  
  double angle = acos(x*xo + y*yo + z*zo) * 180 / 3.141;

  Serial.print("ANGLE = "); Serial.println(angle);
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(333);
}
