#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Standard I2C address for SSD1306

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin Definitions
const int LDR_PIN = 34; // LDR analog output connected to GPIO 34

void setup() {
  Serial.begin(115200);

  // Initialize I2C communication (ESP32 default SDA: GPIO 21, SCL: GPIO 22)
  Wire.begin(21, 22);

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Initial display clear and setup
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ESP32 LDR Monitor");
  display.display();
  delay(1000);
}

void loop() {
  // Read raw 12-bit analog light value from LDR (0 to 4095)
  int ldrValue = analogRead(LDR_PIN);

  // Clear display buffer for new frame
  display.clearDisplay();

  // Draw Header
  display.setTextSize(1);
  display.setCursor(10, 5);
  display.println("LIGHT INTENSITY");
  display.drawFastHLine(0, 18, 128, SSD1306_WHITE);

  // Draw Live Reading
  display.setTextSize(2);
  display.setCursor(25, 30);
  display.print(ldrValue);

  // Draw Progress Bar outline based on LDR value
  int barWidth = map(ldrValue, 0, 4095, 0, 120);
  display.drawRect(4, 54, 120, 8, SSD1306_WHITE);
  display.fillRect(4, 54, barWidth, 8, SSD1306_WHITE);

  // Render to physical screen
  display.display();

  delay(200); // Smooth screen update interval
}
