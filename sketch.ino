#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- CT Sensors ----------
#define CT1 34
#define CT2 35
#define CT3 32

// ---------- Relays ----------
#define RELAY_LIGHT 26
#define RELAY_FAN   27
#define RELAY_AC    25

// ---------- LEDs ----------
#define LED_LIGHT 18
#define LED_FAN   19
#define LED_AC    33

// ---------- Buzzer ----------
#define BUZZER 23

// ---------- Away Mode Button ----------
#define AWAY_BUTTON 4

bool awayMode = false;

void setup() {
  Serial.begin(115200);

  // Relay outputs
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_AC, OUTPUT);

  // LED outputs
  pinMode(LED_LIGHT, OUTPUT);
  pinMode(LED_FAN, OUTPUT);
  pinMode(LED_AC, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);

  // Button
  pinMode(AWAY_BUTTON, INPUT_PULLUP);

  // Start everything ON
  digitalWrite(RELAY_LIGHT, HIGH);
  digitalWrite(RELAY_FAN, HIGH);
  digitalWrite(RELAY_AC, HIGH);

  digitalWrite(LED_LIGHT, HIGH);
  digitalWrite(LED_FAN, HIGH);
  digitalWrite(LED_AC, HIGH);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("CELSIO WATTSMART");
  display.println();
  display.println("System Starting...");
  display.display();

  delay(2000);
}

void loop() {

  // ---------- Away Mode Button ----------
  if (digitalRead(AWAY_BUTTON) == LOW) {

    awayMode = !awayMode;

    if (awayMode) {
      activateAwayMode();
    } else {
      deactivateAwayMode();
    }

    delay(500);
  }

  // ---------- Read CT Sensors ----------
  int current1 = analogRead(CT1);
  int current2 = analogRead(CT2);
  int current3 = analogRead(CT3);

  // ---------- OLED Display ----------
  display.clearDisplay();

  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("CELSIO WATTSMART");

  display.setCursor(0, 15);
  display.print("LIGHT : ");
  display.println(current1);

  display.setCursor(0, 27);
  display.print("FAN   : ");
  display.println(current2);

  display.setCursor(0, 39);
  display.print("AC    : ");
  display.println(current3);

  display.setCursor(0, 53);

  if (awayMode) {
    display.println("AWAY MODE: ON");
  } else {
    display.println("AWAY MODE: OFF");
  }

  display.display();

  // ---------- Serial Monitor ----------
  Serial.print("Light: ");
  Serial.print(current1);

  Serial.print(" | Fan: ");
  Serial.print(current2);

  Serial.print(" | AC: ");
  Serial.print(current3);

  Serial.print(" | Away Mode: ");

  if (awayMode) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }

  delay(500);
}


// =================================================
// AWAY MODE ON
// =================================================

void activateAwayMode() {

  // Turn selected appliances OFF
  digitalWrite(RELAY_LIGHT, LOW);
  digitalWrite(RELAY_FAN, LOW);
  digitalWrite(RELAY_AC, LOW);

  // Turn indicator LEDs OFF
  digitalWrite(LED_LIGHT, LOW);
  digitalWrite(LED_FAN, LOW);
  digitalWrite(LED_AC, LOW);

  // Short buzzer alert
  tone(BUZZER, 2000);
  delay(300);
  noTone(BUZZER);

  Serial.println("AWAY MODE ACTIVATED");
}


// =================================================
// AWAY MODE OFF
// =================================================

void deactivateAwayMode() {

  // Turn appliances ON
  digitalWrite(RELAY_LIGHT, HIGH);
  digitalWrite(RELAY_FAN, HIGH);
  digitalWrite(RELAY_AC, HIGH);

  // Turn indicator LEDs ON
  digitalWrite(LED_LIGHT, HIGH);
  digitalWrite(LED_FAN, HIGH);
  digitalWrite(LED_AC, HIGH);

  // Short buzzer alert
  tone(BUZZER, 1500);
  delay(300);
  noTone(BUZZER);

  Serial.println("AWAY MODE DEACTIVATED");
}
