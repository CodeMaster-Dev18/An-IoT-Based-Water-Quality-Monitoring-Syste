#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define ONE_WIRE_BUS 4  // GPIO4 (D2 on NodeMCU)
#define PH_PIN A0       // Analog pin for pH sensor
#define TURBIDITY_PIN A1 // Analog pin for turbidity sensor

// Initialize OneWire and DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Initialize the LCD (I2C address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  sensors.begin();
  lcd.init();           // Initialize the LCD
  lcd.backlight();      // Turn on the backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water Quality");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read pH value
  int phRaw = analogRead(PH_PIN);
  float phVoltage = phRaw * (5.0 / 1023.0);
  float phValue = 7 + ((2.5 - phVoltage) / 0.18); // Calibration formula

  // Read turbidity value
  int turbidityRaw = analogRead(TURBIDITY_PIN);
  float turbidity = turbidityRaw; // Convert as needed

  // Read temperature
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  if (temperature == -127.0) {
    Serial.println("Temperature sensor error!");
    temperature = 0;
  }

  // Display readings on Serial Monitor
  Serial.print("pH: ");
  Serial.print(phValue, 2);
  Serial.print(", Turbidity: ");
  Serial.print(turbidity, 2);
  Serial.print(", Temperature: ");
  Serial.println(temperature, 2);

  // Display readings on LCD
  lcd.setCursor(0, 0);
  lcd.print("pH:");
  lcd.print(phValue, 1);
  lcd.print(" Turb:");
  lcd.print(turbidity, 0);
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(temperature, 1);
  lcd.print((char)223); // Degree symbol
  lcd.print("C");

  delay(2000); // Wait for 2 seconds before next reading
}
