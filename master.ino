#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

#define MPU6050_ACCEL_OFFSET_X -773
#define MPU6050_ACCEL_OFFSET_Y -700
#define MPU6050_ACCEL_OFFSET_Z 1660
#define MPU6050_GYRO_OFFSET_X -24
#define MPU6050_GYRO_OFFSET_Y -62
#define MPU6050_GYRO_OFFSET_Z -6

MPU6050 mpu;
//Endereco I2C do MPU6050
const int MPU = 0x68;

int upRight = 9;
int upLeft = 11;
int downRight = 10;
int downLeft = 12;

bool configured = false;

int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup()
{

    Serial.begin(9600);
    pinMode(upRight, INPUT);
    pinMode(upLeft, INPUT);
    pinMode(downRight, INPUT);
    pinMode(downLeft, INPUT);

    Wire.begin();
    mpu.initialize();
    mpu.setXAccelOffset(MPU6050_ACCEL_OFFSET_X);
    mpu.setYAccelOffset(MPU6050_ACCEL_OFFSET_Y);
    mpu.setZAccelOffset(MPU6050_ACCEL_OFFSET_Z);
    mpu.setXGyroOffset(MPU6050_GYRO_OFFSET_X);
    mpu.setYGyroOffset(MPU6050_GYRO_OFFSET_Y);
    mpu.setZGyroOffset(MPU6050_GYRO_OFFSET_Z);

    Wire.beginTransmission(MPU);
    Wire.write(0x6B);

    //Inicializa o MPU-6050
    Wire.write(0);
    Wire.endTransmission(true);

    configured = true;
}

void loop()
{

    if (!configured)
    {
        setup();
    }

    getDimensions();
    send();
}

void getDimensions()
{
    Wire.beginTransmission(MPU);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    //Solicita os dados do sensor
    Wire.requestFrom(MPU, 14, true);
    //Armazena o valor dos sensores nas variaveis correspondentes
    AcX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read() << 8 | Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void send()
{
    if (Serial.available() > 0)
    {
        int foo[7] = {AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ};
        Serial.write(foo);
    }
}