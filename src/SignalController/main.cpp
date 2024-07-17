#include <Arduino.h>
#include <Wire.h>

enum class SignalState : int
{
  S1 = 1,
  S2,
  S3,
  S4,
  S5,
  S6,
  S7,
  S8,
  S9,
  S10,
  S10A,
  S11,
  S11A,
  S12,
  S12A,
  S13,
  S13A,
  SZ,
  MS2,
  INVALID
};

SignalState currentState = SignalState::S1;
const int ID_s1 = 42;
const int ID_s2 = 53;
const int ButtonPlus = 2;
const int ButtonMinus = 3;
const int CheckTime = 100;
const int SendTime = 2000;
unsigned long stopWatchSend = 0;
unsigned long stopWatchCheck = 0;
bool buttonPressed = false;

void sendState(int receiverID);

void setup()
{
  Wire.begin();
  pinMode(ButtonPlus, INPUT_PULLUP);
  pinMode(ButtonMinus, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - stopWatchCheck >= CheckTime)
  {
    stopWatchCheck = currentMillis;

    bool buttonMinusState = digitalRead(ButtonMinus) == HIGH;
    bool buttonPlusState = digitalRead(ButtonPlus) == HIGH;

    if (buttonMinusState && static_cast<int>(currentState) > static_cast<int>(SignalState::S1) && !buttonPlusState && !buttonPressed)
    {
      buttonPressed = true;
      currentState = static_cast<SignalState>(static_cast<int>(currentState) - 1);
    }

    if (buttonPlusState && static_cast<int>(currentState) < static_cast<int>(SignalState::MS2) && !buttonMinusState && !buttonPressed)
    {
      buttonPressed = true;
      currentState = static_cast<SignalState>(static_cast<int>(currentState) + 1);
    }

    if (!buttonMinusState && !buttonPlusState && buttonPressed)
    {
      buttonPressed = false;
    }
  }

  if (currentMillis - stopWatchSend >= SendTime)
  {
    stopWatchSend = currentMillis;
    sendState(ID_s1);
    sendState(ID_s2);
  }
}

void sendState(int receiverID)
{
  Wire.beginTransmission(receiverID);
  int state = static_cast<int>(currentState);
  Wire.write(reinterpret_cast<byte *>(&state), sizeof(state));
  byte error = Wire.endTransmission();

  if (error != 0)
  {
    Serial.print("Error sending to ID ");
    Serial.print(receiverID);
    Serial.print(": ");
    Serial.println(error);
  }
}
