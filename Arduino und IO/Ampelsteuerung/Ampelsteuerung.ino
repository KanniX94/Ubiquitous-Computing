#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define melodyPin 2

// Die Zahl am Ende steht für den digitalen Pin, an dem die Bauteile (LEDs, Lautsprecher, etc.) angeschlossen sind
// Mit #define sagst du dem Programm:"Überall wo btn1 steht, ersetze es durch 11"
// Somit steht für dem Computer intern überall wo btn1 steht eine 11
// Sonst wüsste der Computer nicht, was er mit digitalWrite(btn1,

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define btn1 11
#define btn2 12

#define ton 2

#define greens 3
#define greenb 5
#define reds 4
#define redb 7
#define orangeb 6

char tmp = 'b';

enum State {
  Gr,  // Auto grün Mensch rot
  Or,  // Auto orange Mensch rot
  Rr1, // Auto rot Mensch rot beim ersten Mal
  Rg,  // Auto rot Mensch grün
  Rr2, // Auto rot Mensch rot beim zweiten Mal
  ROr, // Auto rot orange Mensch rot
};

// Zustand wird am Anfang auf Gr gesetzt, weil Autos grün haben und Menschen rot
// Der Zustand wird in der Variable stat gespeichert und ist ab jetzt vom Typ State
// Somit kann er nun alle in State definierten Zustände annehmen
State stat = Gr;

// Zählt bis zum Ende der Laufzeiten hoch
// und wird dann zurück auf 0 gesetzt
int wait = 0;

// Wenn debug auf true, dann kann man die Schaltzustände auf einem seperaten Display ausgeben
bool debug = false;

// Zähler für individuelle Abläufe der Laufzeiten
int wOr = 20;
int wRr1 = 20;
int wRg = 50;
int wRr2 = 20;
int wROr = 20;

// ton1 dient als Schalter, um den Ton an-/auszuschalten
bool ton1 = false;

void setup() {
  Serial.begin(9600);
  //Serial.begin(14400);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();

  // Funktion, um den initialen Schaltzustand der Ampel festzulegen
  // und um den Pinmodus der digitalen Ausgänge zu definieren
  initalize();

  pinMode(2, OUTPUT);//buzzer
  pinMode(13, OUTPUT);//led indicator when singing a note
}

void loop() {
  // Status von Button 1 wird zwischengespeichert
  int btn1pressed = digitalRead(btn1);
  int btn2pressed = digitalRead(btn2);

  if (btn2pressed == 1) {
    //sing the tunes
    sing(1);
    sing(1);
    sing(2);
  }


  // Verschiede Ampelschaltzustände werden abgearbeitet
  switch  (stat) {

    // Auto grün, Mensch rot
    case (Gr):

      // Diese if-Abfrage dient dazu, dass die Schaltstellung nur einmal ausgeführt wird
      // Einmal reicht, da der Wert gehalten wird
      if (wait == 0) {
        digitalWrite(redb, LOW);
        digitalWrite(orangeb, LOW);
        digitalWrite(greenb, HIGH);

        digitalWrite(reds, HIGH);
        digitalWrite(greens, LOW);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos gruen, \nMenschen rot");

        // wait wird auf 1 (alles außer 0) gesetzt, damit diese if-Bedingung nicht mehr erfüllt ist
        // und die digitalen Pins nicht kontinuierlich gesetzt werden
        wait = 1;
      }

      if (debug == true && wait == 1) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos gruen, \nMenschen rot");
        display.println(wait);
      }

      if (Serial.available() > 0) {
        tmp = (char) Serial.read();
        if (tmp == 'a') {
          stat = Or;

          // wait wird für die if-Bedingung im nächsten Zustand zurück auf 0 gesetzt
          wait = 0;
        }
      }


      // Bleibe in dem Zustand, bis Button 1 gedrückt wird
      if (btn1pressed == 1) {

        // Gehe in den nächsten Zustand über (Auto orange, Mensch rot)
        stat = Or;

        // wait wird für die if-Bedingung im nächsten Zustand zurück auf 0 gesetzt
        wait = 0;
      }
      break;

    // Auto orange, Mensch rot
    case (Or):

      // Diese if-Abfrage dient dazu, dass die Schaltstellung nur einmal ausgeführt wird
      // Einmal reicht, da der Wert gehalten wird
      if (wait == 0) {
        digitalWrite(redb, LOW);
        digitalWrite(orangeb, HIGH);
        digitalWrite(greenb, LOW);

        digitalWrite(reds, HIGH);
        digitalWrite(greens, LOW);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos orange, \nMenschen rot");
      }

      if (debug == true && wait == 0) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos orange, \nMenschen rot");
        display.println(wait);
      }

      // wait wird um 1 hoch gezählt
      wait++;

      // Wenn wait = wOr entspricht (zB. 500), dann wurde 500 ms gewartet
      // Dauer kann man bei der Initialisierung von wOr anpassen (Quellcode ab Zeile 32)
      if (wait == wOr) {
        // Gehe in den nächsten Zustand über (Auto rot, Mensch rot)
        stat = Rr1;
        // wait wird für die if-Bedingung im nächsten Zustand zurück auf 0 gesetzt
        wait = 0;
      }
      break;

    // Auto rot, Mensch rot
    case (Rr1):

      if (wait == 0) {
        digitalWrite(redb, HIGH);
        digitalWrite(orangeb, LOW);
        digitalWrite(greenb, LOW);

        digitalWrite(reds, HIGH);
        digitalWrite(greens, LOW);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos rot, \nMenschen rot");
      }

      if (debug == true  && wait == 0) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos rot, \nMenschen rot");
        display.println(wait);
      }

      wait++;

      if (wait == wRr1) {
        stat = Rg;
        wait = 0;
      }
      break;

    // Auto rot, Mensch grün
    case Rg:

      if (wait == 0) {
        digitalWrite(redb, HIGH);
        digitalWrite(orangeb, LOW);
        digitalWrite(greenb, LOW);

        digitalWrite(reds, LOW);
        digitalWrite(greens, HIGH);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos rot, \nMenschen gruen");
      }

      // Hier Sound abspielen...
      // % heisst Modulo (Geteilt durch mit Rest, wobei der Rest interessant ist)
      // Wenn wait durch 500 geteilt wurde, muss der Restwert 0 betragen
      // So erreichen wir, dass alle 500 ms ein Ton für 500 ms abgespielt wird (500 ms = 2 Hz = 2 Mal pro Sekunde)
      if (wait % 5 == 0) {

        // Wenn momentan kein Ton abgespielt wird, spiele Ton ab
        // ton1 dient als Hilfe, um Ton als Schalter ein-/ und auszuschalten
        if (ton1 == false) {
          digitalWrite(13, HIGH);
          tone(ton, 880, 500); // Spiele Ton mit 880 Hz ab
          ton1 = true; // ton1 wird auf true gesetzt, weil ab jetzt Ton abgespielt wird
        } else { // Wenn Ton momentan abgespielt wird, schalte Ton aus
          noTone(ton);
          digitalWrite(13, LOW);
          ton1 = false;
        }
      }

      if (debug == true  && wait == 0) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos rot, \nMenschen gruen");
        display.println(wait);
      }

      wait++;

      if (wait == wRg) {
        noTone(ton);
        stat = Rr2;
        wait = 0;
      }
      break;

    // Auto rot, Mensch rot
    case Rr2:

      if (wait == 0) {
        digitalWrite(redb, HIGH);
        digitalWrite(orangeb, LOW);
        digitalWrite(greenb, LOW);

        digitalWrite(reds, HIGH);
        digitalWrite(greens, LOW);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos rot, \nMenschen rot");
      }

      if (debug == true  && wait == 0) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos rot, \nMenschen rot");
        display.println(wait);
      }

      wait++;

      if (wait == wRr2) {
        stat = ROr;
        wait = 0;
      }
      break;

    // Auto rot und orange, Mensch rot
    case ROr:

      if (wait == 0) {
        digitalWrite(redb, HIGH);
        digitalWrite(orangeb, HIGH);
        digitalWrite(greenb, LOW);

        digitalWrite(reds, HIGH);
        digitalWrite(greens, LOW);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos rot orange, \nMenschen rot");
      }

      if (debug == true  && wait == 0) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Autos rot orange, \nMenschen rot");
        display.println(wait);
      }

      wait++;

      if (wait == wROr) {
        stat = Gr;
        wait = 0;
      }
      break;

    // Falls unser Status einen anderen Zustand bekommt, als zuvor vorgegeben
    // Sollte hier nicht zutreffen, da beim letzten Zustand wieder in den ersten Zustand gewechselt wird (Gr)
    default:
      break; // Beende die Switchanweisung und führe mit dem weiteren Code fort
  }
  display.display();
  delay(1);
}

int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
//Underworld melody
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
//Underwolrd tempo
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

int song = 0;

void sing(int s) {
  // iterate over the notes of the melody:
  song = s;
  if (song == 2) {
    Serial.println("'Underworld Theme'");
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / underworld_tempo[thisNote];

      buzz(melodyPin, underworld_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }

  } else {

    Serial.println("'Mario Theme'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];

      buzz(melodyPin, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }
  }
}

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);

}


void initalize() {
  // pinMode setzt den Pinmodus der digitalen Ausgänge
  // OUTPUT bedeutet ein Signal soll gesendet werden (zB. an eine LED oder einen Transistor)
  // INPUT bedeutet ein Signal wird erwartet (zB. durch einen Button oder Sensor)
  pinMode(ton, OUTPUT);
  pinMode(greens, OUTPUT);
  pinMode(greenb, OUTPUT);
  pinMode(reds, OUTPUT);
  pinMode(redb, OUTPUT);
  pinMode(orangeb, OUTPUT);

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);

  // Mit digitalWrite legt man ein Signal auf einen Ausgang
  // HIGH bedeutet gebe ein Signal auf die Leitung (einschalten)
  // LOW bedeutet beende das senden des Signals (ausschalten)
  digitalWrite(redb, LOW);
  digitalWrite(orangeb, LOW);
  digitalWrite(greenb, HIGH);
  digitalWrite(reds, HIGH);
  digitalWrite(greens, LOW);
}

