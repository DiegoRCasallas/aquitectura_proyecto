// This example code is in the Public Domain (or CC0 licensed, at your option.)
// By Evandro Copercini - 2018
//
// This example creates a bridge between Serial and Classical Bluetooth (SPP)
// and also demonstrate that SerialBT have the same functionalities of a normal Serial
// Note: Pairing is authenticated automatically by this device

#include "BluetoothSerial.h"

String device_name = "ESP32-BT-Slave";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
}
int direction = 0;  // Contador para la dirección (positivo para arriba, negativo para abajo)
unsigned long pressStartTime = 0;  // El tiempo en que se presionó la tecla
unsigned long pressDuration = 0;   // Duración total de la presión
bool keyPressed = false;           // Estado de la tecla (si está presionada o no)

void loop() {
  // Verificar si hay datos disponibles en el puerto serial Bluetooth
  if (SerialBT.available()) {
    String commands = SerialBT.readStringUntil('\n');  // Leer hasta el siguiente salto de línea

    // Procesar cada comando individualmente
    for (int i = 0; i < commands.length(); i += 2) {
      String command = commands.substring(i, i + 2);
      processCommand(command);
    }
  }

  // Si la tecla está presionada, contar el tiempo
  if (keyPressed) {
    pressDuration = millis() - pressStartTime;  // Calcular el tiempo que ha pasado desde que se presionó
  }

  // Si la tecla ha sido liberada, procesar el comando completo
  if (!keyPressed && pressDuration > 0) {
    Serial.println("Key released. Duration: " + String(pressDuration) + " ms");
    pressDuration = 0;  // Resetear la duración para la próxima vez
  }

  delay(20);  // Retraso para estabilizar las lecturas
}

void processCommand(String command) {
  if (command == "F0" || command == "F") {
    // Comando para ir hacia arriba, incrementamos la dirección
    direction += 1;
    Serial.println("F command detected. Moving UP. Direction: " + String(direction));
  }
  else if (command == "T0" || command == "T") {
    // Comando para ir hacia arriba, incrementamos la dirección
    direction += 1;
    Serial.println("T command detected. Moving UP. Direction: " + String(direction));
  }
  else if (command == "X0" || command == "X" || command == "B0" || command == "B") {
    // Comando para ir hacia abajo, decrementamos la dirección
    direction -= 1;
    Serial.println(command + " command detected. Moving DOWN. Direction: " + String(direction));
  }
  else {
    Serial.println("Unknown command: " + command);
  }
}