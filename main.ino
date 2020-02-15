//gesture control
#include <Keyboard.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>


#define MPU6050_ACCEL_OFFSET_X -773
#define MPU6050_ACCEL_OFFSET_Y -700
#define MPU6050_ACCEL_OFFSET_Z 1660
#define MPU6050_GYRO_OFFSET_X  -24
#define MPU6050_GYRO_OFFSET_Y  -62
#define MPU6050_GYRO_OFFSET_Z  -6
  
MPU6050 mpu;
//Endereco I2C do MPU6050
const int MPU=0x68;  


int up_right = 9;
int up_left = 11;
int down_right = 10;
int down_left = 12;
int portaBotao = 5;
int led = 6;
int portaBotaoOnOff = 7;
int ledPosicaoNeutra = 8;

bool ok = false;



int AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup()
{
  
  Keyboard.begin();
  Serial.begin(9600);
  pinMode(portaBotao, INPUT);
  pinMode(up_right, INPUT);
  pinMode(up_left, INPUT);
  pinMode(down_right, INPUT);
  pinMode(down_left, INPUT);
  pinMode(led, OUTPUT);
  pinMode(ledPosicaoNeutra, OUTPUT);
  pinMode(portaBotaoOnOff, INPUT);
  
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

}

void loop()
{
    if(digitalRead(portaBotaoOnOff) == HIGH){
      ok = !ok;
      Serial.print("INICIANDO! \n");
      delay(100);
      
    }
    
    if(ok){
      pegarDimensoes();
      mapearBotoes();
      ok = true;
    }
    else
      Serial.print("APERTE O BOTO PARA INICIAR \n");
    
    
    
}

void pegarDimensoes(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true);  
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  
}

void mapearBotoes(){
    Serial.println();
    
    if(AcX < 14000){
        Serial.print(" Front ");
        
        Keyboard.press(KEY_UP_ARROW);

        digitalWrite(led, HIGH);
        
    }
    if(AcX > 25000){
        Serial.print(" Back ");
        
        Keyboard.press(KEY_DOWN_ARROW);
        
    }
    if(AcY  >30000){
        Serial.print(" Right ");
        
        Keyboard.press(KEY_RIGHT_ARROW);
        
    }
    if(AcY < 22000){
        Serial.print(" Left ");
        
        Keyboard.press(KEY_LEFT_ARROW);
        
        
    }
    
    if(digitalRead(portaBotao) == HIGH){
        Serial.print(" Click ");
        
        Keyboard.press('z');
        
    }
    
    if(!(AcX < 14000 || AcX > 25000 || AcY  >30000 || AcY < 22000)){
        digitalWrite(ledPosicaoNeutra, HIGH);
    } else{
        digitalWrite(ledPosicaoNeutra, LOW);  
    }
    
    delay(100);
    Keyboard.releaseAll();
    Keyboard.end();
    digitalWrite(led, LOW);
}
