#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float Celcius = 0;
// DS18S20 Temperaturchip i/o

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

char str[512];
int count = 0;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  sensors.begin();

  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.clearDisplay();
}

void loop() {
  /*
    if (Serial.available() > 0) {
    str[count++] = (char) Serial.read();
    if (str[count - 2] == '\\' && str[count - 1] == 'n') {
      display.clearDisplay();
      display.setCursor(0, 0);
      for (int i = 0; i < count - 2; i++) {
        display.print(str[i]);
      }
      count = 0;
    }
    }
  */
  for (int i = 0; i <= 255; i++) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println((char) i);
    display.display();
    delay(100);
  }
  /*display.clearDisplay();
    display.setCursor(0, 0);
    sensors.requestTemperatures();
    Celcius = sensors.getTempCByIndex(0);
    display.print(" C  ");
    display.print(Celcius);
    delay(1000);
  */

}
