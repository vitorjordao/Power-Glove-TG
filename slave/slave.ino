#include <Wire.h>
#include <Keyboard.h>
#include <SoftwareSerial.h>

int rxdPort = 11;
int txdPort = 12;
SoftwareSerial mySerial =  SoftwareSerial(rxdPort, txdPort);

bool configured = false;

int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int command = 0;

void setup()
{

    Keyboard.begin();
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    Serial.println("Serial do Arduino iniciada...");

    mySerial.begin(9600);
    mySerial.println("Slave > Hello, world?");

    configured = true;
}

void loop()
{

    if (!configured)
    {
        setup();
    }

    recive();
    mapButtons();
}

void recive()
{

    if(mySerial.available() > 0)
    { 
        command = mySerial.read();
    }
//    Serial.println(mySerial.available());
}

void mapButtons()
{

    if (command == 1)
    {
        Serial.println(" Front | RIGHT ");
        Keyboard.press(KEY_UP_ARROW);
        Keyboard.press(KEY_RIGHT_ARROW);
    }
    else if (command == 2)
    {
        Serial.println(" Front | LEFT ");
        Keyboard.press(KEY_UP_ARROW);
        Keyboard.press(KEY_LEFT_ARROW);
    }
    else if (command == 3)
    {
        Serial.println(" Front ");
        Keyboard.press(KEY_UP_ARROW);
    }
    else if (command == 4)
    {
        Serial.println(" Back | Right ");
        Keyboard.press(KEY_DOWN_ARROW);
        Keyboard.press(KEY_RIGHT_ARROW);
    }
    else if (command == 5)
    {
        Serial.println(" Back | LEFT ");
        Keyboard.press(KEY_DOWN_ARROW);
        Keyboard.press(KEY_LEFT_ARROW);
    }
    else if (command == 6)
    {
        Serial.println(" Back ");
        Keyboard.press(KEY_DOWN_ARROW);
    }
    else if (command == 7)
    {
        Serial.println(" LEFT ");
        Keyboard.press(KEY_LEFT_ARROW);
    }
    else if (command == 8)
    {
        Serial.println(" Right ");
        Keyboard.press(KEY_RIGHT_ARROW);
    }

    delay(100);
    Keyboard.releaseAll();
    Keyboard.end();
}
