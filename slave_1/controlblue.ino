
#include "BluetoothSerial.h"

#define FORWARD 'F'
#define BACKWARD 'B'
#define LEFT 'L'
#define RIGHT 'R'
#define CIRCLE 'C'
#define CROSS 'X'
#define TRIANGLE 'T'
#define SQUARE 'S'
#define START 'A'
#define PAUSE 'P'

String device_name = "slave_1";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
const int ledPin = 2;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  SerialBT.begin(device_name); // Bluetooth device name
  // SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
}

char comando;

void loop()
{
  if (Serial.available())
  {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available())
  {
    char comando = SerialBT.read();
    Serial.write(comando);
    ejecutarFuncion(comando);
  }
  delay(20);
}
void encenderLed()
{
  digitalWrite(ledPin, HIGH);
  delay(1000);
}
void apagarLed()
{
  digitalWrite(ledPin, LOW); // Apaga el LED
  delay(1000);
}

void ejecutarFuncion(char comando)
{
  switch (comando)
  {
  case FORWARD:
    // Perform action for moving forward
    break;
  case BACKWARD:
    // Perform action for moving backward
    break;
  case LEFT:
    // Perform action for turning left
    break;
  case RIGHT:
    // Perform action for turning right
    break;
  case CIRCLE:
    // Perform action for circle
    break;
  case CROSS:
    encenderLed();
    break;
  case TRIANGLE:
    apagarLed();
    break;
  case SQUARE:
    // Perform action for retrieving and sending status information
    break;
  case START:
    // Perform action for starting a process or operation
    break;
  case PAUSE:
    // Perform action for pausing a process or operation
    break;
  default:
    // Invalid command received
    break;
  }
}