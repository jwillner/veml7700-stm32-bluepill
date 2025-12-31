#include <Arduino.h>
#include <Wire.h>

// VEML7700 I2C address (typisch 0x10)
static const uint8_t VEML7700_ADDR = 0x10;

// Register
static const uint8_t REG_ALS_CONF_0 = 0x00;
static const uint8_t REG_ALS        = 0x04;
static const uint8_t REG_WHITE      = 0x05;

uint16_t i2cRead16(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return 0xFFFF; // repeated start
  Wire.requestFrom((int)addr, 2);
  if (Wire.available() < 2) return 0xFFFF;
  uint8_t lo = Wire.read();
  uint8_t hi = Wire.read();
  return (uint16_t)hi << 8 | lo; // little endian
}

bool i2cWrite16(uint8_t addr, uint8_t reg, uint16_t val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write((uint8_t)(val & 0xFF));         // lo
  Wire.write((uint8_t)((val >> 8) & 0xFF));  // hi
  return (Wire.endTransmission() == 0);
}

void i2cScan() {
  Serial1.println("\nI2C scan...");
  int found = 0;
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) {
      Serial1.print("  Found 0x");
      if (a < 16) Serial1.print('0');
      Serial1.println(a, HEX);
      found++;
    }
  }
  if (!found) Serial1.println("  No I2C devices found.");
}

// Default-Approx (Gain 1x, IT 100ms): ~0.0576 lux/count
float rawToLux_DefaultApprox(uint16_t raw) {
  return raw * 0.0576f;
}

void setup() {
  Serial1.begin(115200);
  delay(200);
  Serial1.println("BluePill + VEML7700 test");

  // I2C pins explizit (damit keine Überraschungen)
  Wire.setSDA(PB7);
  Wire.setSCL(PB6);
  Wire.begin();
  Wire.setClock(100000);

  i2cScan();

  // VEML7700 einschalten (Shutdown-Bit = 0)
  // Wir lassen Gain/IT erstmal default und setzen nur "power on"
  uint16_t conf = i2cRead16(VEML7700_ADDR, REG_ALS_CONF_0);
  Serial1.print("ALS_CONF_0 read = 0x");
  Serial1.println(conf, HEX);

  // Shutdown bit (ALS_SD) ist Bit0: 0 = on
  // Also: Bit0 löschen
  if (conf != 0xFFFF) {
    conf &= ~0x0001;
    if (i2cWrite16(VEML7700_ADDR, REG_ALS_CONF_0, conf)) {
      Serial1.println("VEML7700: power on OK");
    } else {
      Serial1.println("VEML7700: write conf failed");
    }
  } else {
    Serial1.println("VEML7700 not responding at 0x10 -> wiring/pullups/3.3V check");
  }
}

void loop() {
  uint16_t als_raw   = i2cRead16(VEML7700_ADDR, REG_ALS);
  uint16_t white_raw = i2cRead16(VEML7700_ADDR, REG_WHITE);

  if (als_raw == 0xFFFF) {
    Serial1.println("ALS read failed (0xFFFF)");
  } else {
    float lux = rawToLux_DefaultApprox(als_raw);
    Serial1.print("ALS raw=");
    Serial1.print(als_raw);
    Serial1.print("  lux~");
    Serial1.print(lux, 2);
    Serial1.print("  WHITE raw=");
    Serial1.println(white_raw);
  }

  delay(500);
}
