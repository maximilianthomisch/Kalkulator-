#include <Arduino.h>

void setup() {
  Serial.begin(9600); // Baudrate einstellen
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Nachricht von PC lesen
    int Ergebnis = evaluateExpression(input);

    // Ergebnis an PC senden
    Serial.print("Ergebnis: ");
    Serial.println(Ergebnis);
  }
}

int evaluateExpression(String expression) {
  // Hier sollte die Funktion für die Berechnung der Ausdrücke implementiert werden
  // Die Eingabe, + 4 3, sollte zu dem Ergebnis 7 führen. 
 
  // Vorübergehend wird hier eine einfache Addition durchgeführt
  int Wert1, Wert2;
  char operation;
  sscanf(expression.c_str(), "%c %d %d", &operation, &Wert1, &Wert2);

  switch (operation) {
    case '+':
      return Wert1 + Wert2;
    case '-':
      return Wert1 - Wert2;
    case '*':
      return Wert1 * Wert2;
    case '/':
      return Wert1 / Wert2;
    default:
      return 0; // Fehlerhaftes Format
  }
}

