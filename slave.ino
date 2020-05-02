#include <Wire.h>

bool configured = false;

int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup()
{

    Keyboard.begin();
    Serial.begin(9600);

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
    if (Serial.available() > 0)
    {
        int state[7] = Serial.read();
        AcX = state[0];
        AcY = state[1];
        AcZ = state[2];
        Tmp = state[3];
        GyX = state[4];
        GyY = state[5];
        GyZ = state[6];
    }
}

void mapButtons()
{
    Serial.println();

    if (AcX < 14000)
    {
        Serial.print(" Front ");

        Keyboard.press(KEY_UP_ARROW);

        digitalWrite(led, HIGH);
    }
    if (AcX > 25000)
    {
        Serial.print(" Back ");

        Keyboard.press(KEY_DOWN_ARROW);
    }
    if (AcY > 30000)
    {
        Serial.print(" Right ");

        Keyboard.press(KEY_RIGHT_ARROW);
    }
    if (AcY < 22000)
    {
        Serial.print(" Left ");

        Keyboard.press(KEY_LEFT_ARROW);
    }

    delay(100);
    Keyboard.releaseAll();
    Keyboard.end();
}
