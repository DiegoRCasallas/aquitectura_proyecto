#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configuración de pines para botones
#define P1_UP 12
#define P1_DOWN 13
#define P2_UP 14
#define P2_DOWN 27

// Variables del juego
int paddle1_y = 24;           // Posición inicial del jugador 1
int paddle2_y = 24;           // Posición inicial del jugador 2
int ball_x = 64, ball_y = 32; // Posición inicial de la pelota
int ball_dx = 1, ball_dy = 1; // Dirección de la pelota
const int paddle_height = 16;
const int paddle_width = 3;
const int ball_size = 2;
int score1 = 0, score2 = 0;
// int velocidad_juego = 100;

// variables master
const char *ssid = "mired";
const char *password = "abc123";

WiFiServer server(80);
// Configuración inicial
void setup()
{
  // configuracion servidor maestro
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("Iniciando servidor...");
  server.begin();

  // Configurar botones como entradas
  pinMode(P1_UP, INPUT_PULLUP);
  pinMode(P1_DOWN, INPUT_PULLUP);
  pinMode(P2_UP, INPUT_PULLUP);
  pinMode(P2_DOWN, INPUT_PULLUP);

  // Inicializar pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1)
      ;
  }
  display.clearDisplay();
  display.display();
}

// Dibujar el juego en la pantalla
void drawGame()
{
  display.clearDisplay();

  // Dibujar las paletas
  display.fillRect(0, paddle1_y, paddle_width, paddle_height, SSD1306_WHITE);                           // Paleta jugador 1
  display.fillRect(SCREEN_WIDTH - paddle_width, paddle2_y, paddle_width, paddle_height, SSD1306_WHITE); // Paleta jugador 2

  // Dibujar la pelota
  display.fillRect(ball_x, ball_y, ball_size, ball_size, SSD1306_WHITE);

  // Dibujar puntuación
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 0);
  display.print("P1: ");
  display.print(score1);
  display.setCursor(80, 0);
  display.print("P2: ");
  display.print(score2);

  display.display();
}

// Actualizar la posición de las paletas
void updatePaddles()
{
  // Jugador 1
  if (!digitalRead(P1_UP) && paddle1_y > 0)
    paddle1_y -= 2;
  if (!digitalRead(P1_DOWN) && paddle1_y < SCREEN_HEIGHT - paddle_height)
    paddle1_y += 2;

  // Jugador 2
  if (!digitalRead(P2_UP) && paddle2_y > 0)
    paddle2_y -= 2;
  if (!digitalRead(P2_DOWN) && paddle2_y < SCREEN_HEIGHT - paddle_height)
    paddle2_y += 2;
}

// Actualizar la posición de la pelota
void updateBall()
{
  ball_x += ball_dx;
  ball_y += ball_dy;

  // Rebote en los bordes superior e inferior
  if (ball_y <= 0 || ball_y >= SCREEN_HEIGHT - ball_size)
  {
    ball_dy = -ball_dy;
  }

  // Rebote en las paletas
  if (ball_x <= paddle_width && ball_y >= paddle1_y && ball_y <= paddle1_y + paddle_height)
  {
    ball_dx = -ball_dx;
  }
  if (ball_x >= SCREEN_WIDTH - paddle_width - ball_size && ball_y >= paddle2_y && ball_y <= paddle2_y + paddle_height)
  {
    ball_dx = -ball_dx;
  }

  // Salida de la pelota: puntaje
  if (ball_x <= 0)
  {
    score2++;
    resetBall();
    //velocidad_juego /= 2;
  }
  else if (ball_x >= SCREEN_WIDTH - ball_size)
  {
    score1++;
    resetBall();
    //velocidad_juego /= 2;
  }
}

// Resetear la posición de la pelota
void resetBall()
{
  ball_x = SCREEN_WIDTH / 2;
  ball_y = SCREEN_HEIGHT / 2;
  ball_dx = ball_dx > 0 ? 1 : -1;
  ball_dy = ball_dy > 0 ? 1 : -1;
}

// Bucle principal
void loop()
{
  // ejecucion servidor maestro
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("Cliente conectado");
    while (client.connected())
    {
      if (client.available())
      {
        String data = client.readStringUntil('\n');
        Serial.println("Recibido: " + data);
        client.println("Respuesta del Master");
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");

    // ejecucion juego
    updatePaddles();
    updateBall();
    drawGame();
    delay(5); // Control de velocidad del juego
  }
}
