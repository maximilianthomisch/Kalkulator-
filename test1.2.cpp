/*
    Grober Programmablauf Aufgabe 1.2 Phase 2:
    Stellt eine Kommunikation mit einem Arduino über eine serielle Schnittstelle her, 
    um rechnierische Operationen zu berechnen (+,-,*, /)
    Er nimmt Benutzereingaben für 2 Werte entgegen, 
    sendet diese an den Arduino, empfängt das Ergebnis und speichert die Kommunikation in einer Textdatei.

    Boost Asio-Bibliothek für Netzwerkommunikation.

    Funktionen:
    - communicateWithArduino: Ermöglicht die Kommunikation mit dem Arduino über einen seriellen Anschluss.
    Er sendet einen Ausdruck und empfängt das Ergebnis.

    Hauptprogramm:
    - Öffnet eine Protokolldatei für Kommunikationsdetails.
    - Nimmt wiederholt Benutzereingaben für Ausdrücke entgegen, kommuniziert mit dem Arduino, 
    protokolliert die Kommunikationsdetails und setzt den Prozess optional fort.
    - Protokolliert gesendete und empfangene Daten in einer Logdatei.
    - Protokolliert Berechnungsdetails in einer separaten Datei.
    - Schließt die Protokolldatei nach Abschluss.
    
*/
 // Bibliotheken einbinden 
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>

using namespace boost::asio;

// Kommunikation mit dem Arduino über eine serielle Schnittstelle
std::string communicateWithArduino(const std::string& expression, const std::string& port);

int main() {
    // Öffnet eine Protokolldatei für die Kommunikations 
    std::ofstream logFile("communication_log.txt");

    // Überprüft, ob das Protokoll erfolgreich geöffnet wurde, ggf. wird eine Fehlermeldung gebracht 
    if (!logFile.is_open()) {
        std::cerr << "Fehler beim Öffnen der Log-Datei!" << std::endl;
        return 1;
    }

    // Hauptschleife. Hier werden die Werte für die Berechnung eingegeben. Variablen werden angelegt. 
    while (true) {
        double Wert1, Wert2;
        char operation;

        // Benutzereingaben für Werte abfragen
        std::cout << "Wert 1: ";
        std::cin >> Wert1;

        std::cout << "Operation (+, -, *, /): ";
        std::cin >> operation;

        std::cout << "Wert 2: ";
        std::cin >> Wert2;

        // Auf ungültige Eingabe prüfen
        if (std::cin.fail()) {
            std::cerr << "Ungültige Eingabe!" << std::endl;
            break;
        }

        // Erstellt den arithmetischen Ausdruck
        std::ostringstream expressionStream;
        expressionStream << Wert1 << " " << operation << " " << Wert2;
        std::string expression = expressionStream.str();

        // Protokolliert den gesendeten Ausdruck
        logFile << "Gesendet: " << expression << std::endl;

        // Kommuniziert mit dem Arduino und empfängt das Ergebnis. COM Port Arduino /dev/cu.usbmodem1401
        std::string receivedData = communicateWithArduino(expression, "/dev/cu.usbmodem1401");

        // Überprüft auf Kommunikationsfehler
        if (receivedData.empty()) {
            std::cerr << "Fehler bei der Kommunikation mit dem Arduino!" << std::endl;
            break;
        }

        // Protokolliert die empfangenen Daten
        logFile << "Empfangen: " << receivedData << std::endl;

        // Zeigt das Ergebnis dem Benutzer an
        std::cout << "Ergebnis: " << receivedData << std::endl;

        // Öffnet eine Datei zum Protokollieren von Berechnungsdetails
        std::ofstream calculationFile("berechnungen.txt", std::ios_base::app);
        if (calculationFile.is_open()) {
            // Protokolliert Ausdruck und Ergebnis
            calculationFile << "Ausdruck: " << expression << "\n";
            calculationFile << "Berechnungen: " << receivedData << "\n";
            calculationFile << "---------------------------\n";
            calculationFile.close();
        } else {
            std::cerr << "Fehler beim Öffnen der Berechnungen-Datei!" << std::endl;
        }

        // Fragt den Benutzer, ob er fortsetzen möchte
        std::string continueInput;
        std::cout << "Weitermachen? (ja/nein): ";
        std::cin >> continueInput;

        // Schleife wird verlassen, wenn der Benutzer nicht fortsetzen möchte
        if (continueInput != "ja") {
            break;
        }
    }

    // Schließt die Protokolldatei
    logFile.close();
    return 0;
}

// Funktion zur Kommunikation mit dem Arduino
std::string communicateWithArduino(const std::string& expression, const std::string& port) {
    io_service io;
    serial_port serial(io, port);

    // Einstellungen seriellen Anschluss
    serial.set_option(serial_port_base::baud_rate(9600));
    serial.set_option(serial_port_base::character_size(8));
    serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
    serial.set_option(serial_port_base::parity(serial_port_base::parity::none));

    // Sendet den Ausdruck an den Arduino
    write(serial, buffer(expression + '\n'));

    // Zeit, für die Verarbeitung vom Arduino 
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Liest die Antwort vom Arduino
    std::string receivedData;
    read_until(serial, dynamic_buffer(receivedData), '\n');

    // Gibt die empfangenen Daten zurück
    return receivedData;
}
