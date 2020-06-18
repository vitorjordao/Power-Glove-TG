#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>
#include <SoftwareSerial.h>

int rxdPort = 0;
int txdPort = 1;
SoftwareSerial mySerial =  SoftwareSerial(rxdPort, txdPort);

#define MPU6050_ACCEL_OFFSET_X -773
#define MPU6050_ACCEL_OFFSET_Y -700
#define MPU6050_ACCEL_OFFSET_Z 1660
#define MPU6050_GYRO_OFFSET_X -24
#define MPU6050_GYRO_OFFSET_Y -62
#define MPU6050_GYRO_OFFSET_Z -6

MPU6050 mpu;
//Endereco I2C do MPU6050
const int MPU = 0x68;

//int upRight = 9;
//int upLeft = 11;
//int downRight = 10;
//int downLeft = 12;

bool configured = false;

int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup()
{

    Serial.begin(9600);
    // pinMode(upRight, INPUT);
    // pinMode(upLeft, INPUT);
    // pinMode(downRight, INPUT);
    // pinMode(downLeft, INPUT);
    mySerial.begin(9600);
    mySerial.println("Master > Hello, world?");

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
    int command = 0;

    if (AcX < 14000 && AcY > 30000)
    {
        Serial.println(" Front | RIGHT ");

        command = 1;
    }
    else if (AcX < 14000 && AcY < 22000)
    {
        Serial.println(" Front | LEFT ");

        command = 2;
    }
    else if (AcX < 14000)
    {
        Serial.println(" Front ");

        command = 3;
    }
    else if (AcX > 25000 && AcY > 30000)
    {
        Serial.println(" Back | Right ");

        command = 4;
    }
    else if (AcX > 25000 && AcY < 22000)
    {
        Serial.println(" Back | LEFT ");

        command = 5;
    }
    else if (AcX > 25000)
    {
        Serial.println(" Back ");

        command = 6;
    }
    else if (AcY < 22000)
    {
        Serial.println(" LEFT ");

        command = 7;
    }
    else if (AcY > 30000)
    {
        Serial.println(" Right ");

        command = 8;
    }
    

    mySerial.write(command);
    delay(50);
}
