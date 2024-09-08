#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int analogPin = A0;  // Analog input pin connected to A0
const float referenceResistance = 12000.0;  // Known reference resistor value in ohms (12k ohm)

void setup() {
  Serial.begin(9600);  // Initialize serial communication
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay();  // Clear the display
  display.setTextSize(1);  // Set text size
  display.setTextColor(SSD1306_WHITE);  // Set text color
  display.setCursor(0, 0);  // Set cursor position
}

void drawResistanceBar(float resistance) {
  int barLength = map(resistance, 0, 10000, 0, SCREEN_WIDTH - 10); // Map resistance to screen width
  display.drawRect(5, 35, SCREEN_WIDTH - 10, 8, SSD1306_WHITE); // Draw an empty bar above
  display.fillRect(5, 35, barLength, 8, SSD1306_WHITE); // Fill the bar based on resistance
  display.display(); // Update the display
}

void loop() {
  int sensorValue = analogRead(analogPin);  // Read analog input
  float voltage = sensorValue * (5.0 / 1023.0);  // Convert sensor value to voltage (assuming a 5V Arduino)
  float unknownResistance = (voltage * referenceResistance) / (5.0 - voltage);  // Calculate unknown resistance

  // Clear the display
  display.clearDisplay();

  // Draw the resistance bar graph above
  drawResistanceBar(unknownResistance);

  // Display resistance in standard format at the bottom
  display.setCursor(0, 20);  // Set cursor position for resistance value
  display.print("Resistance:");
  if (unknownResistance >= 1000000.0) {
    display.print(unknownResistance / 1000000.0, 2);  // Megaohms
    display.print(" Mohm");
  } else if (unknownResistance >= 1000.0) {
    display.print(unknownResistance / 1000.0, 2);  // Kilohms
    display.print(" Kohm");
  } else {
    display.print(unknownResistance, 2);  // Ohms
    display.print(" ohm");
  }

  display.display();  // Update the display

  // Print the resistance to the Serial Monitor for reference
  Serial.print("Unknown Resistance: ");
  Serial.print(unknownResistance, 2);  // Ohms
  Serial.println(" Ω");

  delay(1000);  // Delay for readability
}
