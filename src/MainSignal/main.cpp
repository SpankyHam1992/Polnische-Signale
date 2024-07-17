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

constexpr int ID_s = 42;
constexpr int BlinkTime = 500;
constexpr int WaitTime = 500;
constexpr int lamps[] = {9, 8, 7, 6, 5, 4, 3};

SignalState currentState = SignalState::S1;
unsigned long stopWatchBlink = 0;
bool On = true;

void SignalLamps(bool states[]);
void SetSignalState(SignalState state);
void receiveEvent(int howMany);

void setup()
{
  Wire.begin(ID_s);
  for (int pin : lamps)
  {
    pinMode(pin, OUTPUT);
  }
  bool initialState[7] = {0};
  SignalLamps(initialState);
  Wire.onReceive(receiveEvent);
}

void loop()
{
  if (millis() - stopWatchBlink >= BlinkTime)
  {
    stopWatchBlink = millis();
    SetSignalState(currentState);
  }
}

void receiveEvent(int howMany)
{
  if (howMany >= sizeof(int))
  {
    int receivedValue;
    Wire.readBytes((char *)&receivedValue, sizeof(receivedValue));
    if (receivedValue >= static_cast<int>(SignalState::S1) && receivedValue <= static_cast<int>(SignalState::MS2))
    {
      currentState = static_cast<SignalState>(receivedValue);
    }
  }
}

void SignalLamps(bool states[])
{
  for (int i = 0; i < 7; ++i)
  {
    digitalWrite(lamps[i], states[i]);
  }
}

void SetSignalState(SignalState state)
{
  bool states[7] = {0};

  switch (state)
  {
  case SignalState::S1:
    states[2] = true; // RedLamp
    break;
  case SignalState::S2:
    states[0] = true; // GreenLamp
    break;
  case SignalState::S3:
    states[0] = On; // GreenLamp flashing
    On = !On;
    break;
  case SignalState::S4:
    states[1] = On; // Yellow1Lamp flashing
    On = !On;
    break;
  case SignalState::S5:
    states[1] = true; // Yellow1Lamp
    break;
  case SignalState::S6:
    states[0] = true; // GreenLamp
    states[3] = true; // Yellow2Lamp
    states[5] = true; // GreenStrip
    break;
  case SignalState::S7:
    states[0] = On;   // GreenLamp flashing
    states[3] = true; // Yellow2Lamp
    states[5] = true; // GreenStrip
    On = !On;
    break;
  case SignalState::S8:
    states[1] = On;   // Yellow1Lamp flashing
    states[3] = true; // Yellow2Lamp
    states[5] = true; // GreenStrip
    On = !On;
    break;
  case SignalState::S9:
    states[1] = true; // Yellow1Lamp
    states[3] = true; // Yellow2Lamp
    states[5] = true; // GreenStrip
    break;
  case SignalState::S10:
    states[0] = true; // GreenLamp
    states[3] = true; // Yellow2Lamp
    break;
  case SignalState::S10A:
    states[0] = true; // GreenLamp
    states[3] = true; // Yellow2Lamp
    states[6] = true; // YellowStrip
    break;
  case SignalState::S11:
    states[0] = On;   // GreenLamp flashing
    states[3] = true; // Yellow2Lamp
    On = !On;
    break;
  case SignalState::S11A:
    states[0] = On;   // GreenLamp flashing
    states[3] = true; // Yellow2Lamp
    states[6] = true; // YellowStrip
    On = !On;
    break;
  case SignalState::S12:
    states[1] = On;   // Yellow1Lamp flashing
    states[3] = true; // Yellow2Lamp
    On = !On;
    break;
  case SignalState::S12A:
    states[1] = On;   // Yellow1Lamp flashing
    states[3] = true; // Yellow2Lamp
    states[6] = true; // YellowStrip
    On = !On;
    break;
  case SignalState::S13:
    states[1] = true; // Yellow1Lamp
    states[3] = true; // Yellow2Lamp
    break;
  case SignalState::S13A:
    states[1] = true; // Yellow1Lamp
    states[3] = true; // Yellow2Lamp
    states[6] = true; // YellowStrip
    break;
  case SignalState::SZ:
    states[4] = On; // WhiteLamp flashing
    On = !On;
    break;
  case SignalState::MS2:
    states[4] = true; // WhiteLamp
    break;
  default:
    states[2] = On; // RedLamp
    On = !On;
    break;
  }

  SignalLamps(states);
}
