#include <TinyWireM.h>
#include <tinyNeoPixel_static.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define PIN_LEDS PB1
#define PIN_BTN  PB3
#define NUM_LEDS 25
#define MMC_ADDR 0x30

// Static RAM-Buffer for leds (saves on dynamic memory)
byte pixelBuffer[NUM_LEDS * 3];
tinyNeoPixel pixels = tinyNeoPixel(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800, pixelBuffer);

const uint8_t compass_ring[8] = {14, 4, 2, 0, 10, 20, 22, 24};

// Interrupt routine for button (PCBINT0 covers all the interrupt pins)
ISR(PCINT0_vect) {
  // Empty interrupt, wakes up the ATtiny from sleep-mode
}

// Overrides atan2 without using float numbers to save on flash
uint8_t getSector(int32_t x, int32_t y) {
  int32_t ax = (x < 0) ? -x : x;
  int32_t ay = (y < 0) ? -y : y;

  // The relation 5/12 is tan of (22.5 degrees) which is the limit inbetween the sectors
  if (12 * ay < 5 * ax) {
    return (x > 0) ? 0 : 4; // East (0) or West (4)
  } else if (5 * ay > 12 * ax) {
    return (y > 0) ? 2 : 6; // North (2) or South (6)
  } else {
    if (x > 0 && y > 0) return 1; // North East (1)
    if (x < 0 && y > 0) return 3; // North West (3)
    if (x < 0 && y < 0) return 5; // South West (5)
    return 7;                     // South East (7)
  }
}

void goToSleep() {
  // Turn off all the leds
  pixels.clear();
  pixels.show();

  // Disable unneeded pin reading functions
  ADCSRA &= ~(1 << ADEN);

  // Allow interrupt on button press
  GIMSK |= (1 << PCIE);    // Allow interrupt to be listened to
  PCMSK |= (1 << PCINT3);  // Which pin is interrupt pin

  // Go to deepest sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // Go into sleep mode until woken
  sleep_cpu();

  // --------------------------------------------------
  // Pause here until interrupt button has been pressed
  // --------------------------------------------------

  // Wake up when button has been pressed
  sleep_disable();

  // Deactivate button during duration
  GIMSK &= ~(1 << PCIE);
  PCMSK &= ~(1 << PCINT3);
  
  // Reactivate pin reading functions
  ADCSRA |= (1 << ADEN);

  // Small delay for debouncing on the button
  delay(200);
}

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP); // Activate interrupt for button
  pinMode(PIN_LEDS, OUTPUT); // Overrides pixels.begin()

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
    uint8_t sector = getSector(x, y);

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