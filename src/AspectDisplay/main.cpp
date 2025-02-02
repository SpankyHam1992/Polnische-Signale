#include <Arduino.h>
#include <Wire.h>
#include <SevSeg.h>

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

constexpr int DispTime = 500;
constexpr int ID_s = 53;
constexpr byte numDigits = 4;
constexpr byte digitPins[] = {2, 3, 4, 5};
constexpr byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
constexpr bool resistorsOnSegments = true;
constexpr byte hardwareConfig = COMMON_CATHODE;

SignalState currentState = SignalState::S1;
unsigned long stopWatchDisp = 0;

void receiveEvent(int howMany);
void SetSignalState(SignalState aspect);
SevSeg sevseg;

void setup()
{
  Wire.begin(ID_s);
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
  Wire.onReceive(receiveEvent);
}

void loop()
{
  sevseg.refreshDisplay();

  if (millis() - stopWatchDisp >= DispTime)
  {
    stopWatchDisp = millis();
    SetSignalState(currentState);
  }
}

void receiveEvent(int howMany)
{
  // Überprüfen, ob genügend Bytes empfangen wurden
  if (howMany >= static_cast<int>(sizeof(int)))
  {
    int receivedValue;
    Wire.readBytes((char *)&receivedValue, sizeof(receivedValue));

    // Validierung der empfangenen Daten
    if (receivedValue >= static_cast<int>(SignalState::S1) && receivedValue <= static_cast<int>(SignalState::INVALID))
    {
      currentState = static_cast<SignalState>(receivedValue);
    }
  }
}

void SetSignalState(SignalState aspect)
{
  const char *displayStr = "0000";
  switch (aspect)
  {
  case SignalState::S1:
    displayStr = "S_1_";
    break;
  case SignalState::S2:
    displayStr = "S_2_";
    break;
  case SignalState::S3:
    displayStr = "S_3_";
    break;
  case SignalState::S4:
    displayStr = "S_4_";
    break;
  case SignalState::S5:
    displayStr = "S_5_";
    break;
  case SignalState::S6:
    displayStr = "S_6_";
    break;
  case SignalState::S7:
    displayStr = "S_7_";
    break;
  case SignalState::S8:
    displayStr = "S_8_";
    break;
  case SignalState::S9:
    displayStr = "S_9_";
    break;
  case SignalState::S10:
    displayStr = "S10_";
    break;
  case SignalState::S10A:
    displayStr = "S10A";
    break;
  case SignalState::S11:
    displayStr = "S11_";
    break;
  case SignalState::S11A:
    displayStr = "S11A";
    break;
  case SignalState::S12:
    displayStr = "S12_";
    break;
  case SignalState::S12A:
    displayStr = "S12A";
    break;
  case SignalState::S13:
    displayStr = "S13_";
    break;
  case SignalState::S13A:
    displayStr = "S13A";
    break;
  case SignalState::SZ:
    displayStr = "----";
    break;
  case SignalState::MS2:
    displayStr = "____";
    break;
  default:
    break;
  }
  sevseg.setChars(displayStr);
}
