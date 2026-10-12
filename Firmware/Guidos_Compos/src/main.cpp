#include <TinyWireM.h>
#include <tinyNeoPixel.h>
#include <math.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define PIN_LEDS PB1
#define PIN_BTN  PB3
#define NUM_LEDS 25
#define MMC_ADDR 0x30

tinyNeoPixel pixels = tinyNeoPixel(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

const uint8_t compass_ring[8] = {2, 4, 14, 24, 22, 20, 10, 0};

// Interruptroutine for button (PCBINT0 covers all the interrupt pins)
ISR(PCINT0_vect) {
  // ATtiny continues after sleep_cpu()
}

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP); // Activate interrupt for button

  pixels.begin();
  pixels.setBrightness(15);
  pixels.clear();
  pixels.show();

  TinyWireM.begin();
  
  // Send SET-Signal to reset the magnet on start
  TinyWireM.beginTransmission(MMC_ADDR);
  TinyWireM.write(0x09);
  TinyWireM.write(0x08);
  TinyWireM.endTransmission();
  delay(10);
}

void goToSleep() {
  // Turn off all the leds
  pixels.clear();
  pixels.show();

  ADCSRA &= ~(1 << ADEN);

  // Allow interrupt on button press
  GIMSK |= (1 << PCIE);    // Aktivera Pin Change Interrupt
  PCMSK |= (1 << PCINT3);  // Välj PB3 som avbrottskälla

  // Go to deepest sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // Go into sleep mode until woken
  sleep_cpu();

  // Wake up when button has been pressed
  sleep_disable();

  // Deactivate button during duration
  GIMSK &= ~(1 << PCIE);
  PCMSK &= ~(1 << PCINT3);

  ADCSRA |= (1 << ADEN);

  // Small delay for debouncing on the button
  delay(200);
}

void loop() {
  unsigned long startTime = millis();

  // Run compass measurements during 5 seconds (5000ms)
  while (millis() - startTime < 5000) {
    // Trigger a reading
    TinyWireM.beginTransmission(MMC_ADDR);
    TinyWireM.write(0x09);
    TinyWireM.write(0x01);
    TinyWireM.endTransmission();
    delay(10);

    // Read data
    TinyWireM.beginTransmission(MMC_ADDR);
    TinyWireM.write(0x00);
    TinyWireM.endTransmission();

    // Call for data
    TinyWireM.requestFrom(MMC_ADDR, (uint8_t)4);
    uint8_t raw[4];
    for (uint8_t i = 0; i < 4; i++) {
      raw[i] = TinyWireM.read();
    }

    int32_t x = ((int32_t)((raw[0] << 8) | raw[1])) - 32768;
    int32_t y = ((int32_t)((raw[2] << 8) | raw[3])) - 32768;

    // Calculate heading
    float heading = atan2((float)y, (float)x) * 180.0 / M_PI;
    if (heading < 0) heading += 360.0;

    int sector = ((int)(heading + 22.5) / 45) % 8;

    // Update led matrix
    pixels.clear();
    pixels.setPixelColor(12, pixels.Color(0, 0, 20)); // MidPoint

    if (sector == 0) {
      pixels.setPixelColor(compass_ring[sector], pixels.Color(255, 0, 0)); // Red for north
    } else {
      pixels.setPixelColor(compass_ring[sector], pixels.Color(0, 255, 0)); // Grean for other
    }

    pixels.show();
    delay(100);
  }

  // Go to sleep after 5 second delay
  goToSleep();
}