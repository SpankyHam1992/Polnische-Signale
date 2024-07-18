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

constexpr int ID_s = 37;
constexpr int BlinkTime = 500;
constexpr int WaitTime = 500;
constexpr int lamps[] = {9, 8};

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
    bool initialState[2] = {0};
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
    if (howMany >= static_cast<int>(sizeof(int)))
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
    for (int i = 0; i < 2; ++i)
    {
        digitalWrite(lamps[i], states[i]);
    }
}

void SetSignalState(SignalState state)
{
    bool states[2] = {0};
    switch (state)
    {
    case SignalState::S1:
    case SignalState::SZ:
    case SignalState::MS2:
        states[1] = true; // Yellow1Lamp
        break;
    case SignalState::S2:
    case SignalState::S3:
    case SignalState::S4:
    case SignalState::S5:
        states[0] = true; // GreenLamp
        break;
    case SignalState::S6:
    case SignalState::S7:
    case SignalState::S8:
    case SignalState::S9:
        states[0] = On; // GreenLamp flashing
        On = !On;
        break;
    case SignalState::S10:
    case SignalState::S10A:
    case SignalState::S11:
    case SignalState::S11A:
    case SignalState::S12:
    case SignalState::S12A:
    case SignalState::S13:
    case SignalState::S13A:
        states[1] = On; // Yellow1Lamp flashing
        On = !On;
        break;
    default:
        states[0] = On; // GreenLamp flashing
        states[1] = On; // Yellow1Lamp flashing
        On = !On;
        break;
    }
    SignalLamps(states);
}
