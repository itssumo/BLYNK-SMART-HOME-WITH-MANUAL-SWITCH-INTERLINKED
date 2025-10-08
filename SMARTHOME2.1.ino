/***************  SMART HOME – ESP8266 (4 Relays + PWM Fan)  *****************
 * Local wall switches + Blynk virtual buttons stay in sync either way.
 * Works offline (local switches) and online (Blynk). Debounced inputs.
 * Board: NodeMCU/ESP-12E (ESP8266)
 *****************************************************************************/

// ---- Replace with your own (DO NOT commit real secrets to GitHub) ----
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "SMART HOME"
#define BLYNK_AUTH_TOKEN    "YOUR_BLYNK_AUTH_TOKEN"

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
// ---------------------------------------------------------------------

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Ticker.h>

//////////////////// Hardware mapping ////////////////////
// Relays (active-LOW)
const uint8_t RELAY_PINS[4]   = { D0, D2, D3, D4 };  // k1..k4
// Wall switches (to GND, use INPUT_PULLUP)
const uint8_t SWITCH_PINS[4]  = { D5, D6, D7, D8 };  // sw1..sw4
// PWM pin for DC fan
const uint8_t FAN_PWM_PIN     = D1;                  // speedPin

// Blynk virtual pins for the 4 relays + fan slider [0..100]
const uint8_t VPIN_RELAY[4]   = { V0, V1, V2, V3 };
const uint8_t VPIN_FAN        = V4;

//////////////////// Logic setup ////////////////////
// Active-LOW relay logic made explicit
const uint8_t RELAY_ON  = LOW;
const uint8_t RELAY_OFF = HIGH;

// State containers (false=OFF, true=ON)
bool relayState[4]      = { false, false, false, false };
bool lastSwitchLevel[4] = { HIGH, HIGH, HIGH, HIGH }; // remember last raw level
bool debouncedLevel[4]  = { HIGH, HIGH, HIGH, HIGH };
unsigned long lastChangeMs[4] = {0,0,0,0};

int fanDuty = 0;  // 0..255

// Debounce parameters
const unsigned long DEBOUNCE_MS = 40;

BlynkTimer timer;   // for periodic tasks

//////////////////// Helpers ////////////////////
void applyRelay(uint8_t idx, bool on, bool notifyBlynk) {
  relayState[idx] = on;
  digitalWrite(RELAY_PINS[idx], on ? RELAY_ON : RELAY_OFF);

  if (notifyBlynk && Blynk.connected()) {
    // Blynk button widget expects 1 for ON, 0 for OFF
    Blynk.virtualWrite(VPIN_RELAY[idx], on ? 1 : 0);
  }
}

void applyFan(int duty /*0..255*/, bool /*notifyBlynk not needed here*/) {
  fanDuty = constrain(duty, 0, 255);
  analogWrite(FAN_PWM_PIN, fanDuty);
}

//////////////////// Blynk callbacks ////////////////////
BLYNK_CONNECTED() {
  // pull server states to device on reconnect
  Blynk.syncAll();
}

// Each handler sets the corresponding relay
BLYNK_WRITE(V0) { applyRelay(0, param.asInt() == 1, false); }
BLYNK_WRITE(V1) { applyRelay(1, param.asInt() == 1, false); }
BLYNK_WRITE(V2) { applyRelay(2, param.asInt() == 1, false); }
BLYNK_WRITE(V3) { applyRelay(3, param.asInt() == 1, false); }

// Fan slider 0..100 → 0..255 PWM
BLYNK_WRITE(V4) {
  int percent = constrain(param.asInt(), 0, 100);
  int duty = map(percent, 0, 100, 0, 255);
  applyFan(duty, false);
}

//////////////////// Switch scanning (debounced, edge-detect) ////////////////////
void scanSwitches() {
  unsigned long now = millis();

  for (uint8_t i = 0; i < 4; i++) {
    int raw = digitalRead(SWITCH_PINS[i]); // HIGH = idle, LOW = pressed (active-low wiring)

    if (raw != lastSwitchLevel[i]) {
      lastSwitchLevel[i] = raw;
      lastChangeMs[i] = now;  // start debounce window
    }

    // stable long enough?
    if ((now - lastChangeMs[i]) >= DEBOUNCE_MS && raw != debouncedLevel[i]) {
      debouncedLevel[i] = raw;

      // On a valid edge, toggle relay: pressing the switch (LOW) toggles
      if (debouncedLevel[i] == LOW) {
        bool newState = !relayState[i];
        // Toggle came from local hardware → update Blynk so the app matches
        applyRelay(i, newState, /*notifyBlynk=*/true);
      }
    }
  }
}

//////////////////// Setup & loop ////////////////////
void setup() {
  Serial.begin(115200);
  delay(100);

  // Pins
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], RELAY_OFF); // start OFF
    pinMode(SWITCH_PINS[i], INPUT_PULLUP);  // active-low buttons/switches
  }

  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWriteRange(255); // so 0..255 maps 1:1
  applyFan(0, false);

  // WiFi + Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  // Poll switches ~50 Hz
  timer.setInterval(20L, scanSwitches);

  // If you want to periodically push states to app (helps keep widgets in sync)
  timer.setInterval(3000L, [](){
    if (Blynk.connected()) {
      for (uint8_t i = 0; i < 4; i++) {
        Blynk.virtualWrite(VPIN_RELAY[i], relayState[i] ? 1 : 0);
      }
      // (Optional) Also reflect the fan slider if you keep a separate % state
      // Blynk.virtualWrite(VPIN_FAN, map(fanDuty, 0, 255, 0, 100));
    }
  });
}

void loop() {
  // Online or offline, local switches always work
  Blynk.run();
  timer.run();
}
