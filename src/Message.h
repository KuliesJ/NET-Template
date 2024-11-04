#include <iostream>
#include <string>
using namespace std;

string fixNumberString(int number, int size) {
    return string(size - to_string(number).length(), '0') + to_string(number);
}

string addPadding(string message, int size) {
    return message + string(size - message.length(), '#');
}

class Message {
public:
    // Constructor por defecto
    Message() : message("") {}

    // Constructor que acepta un string
    Message(const string msg) : message(msg) {}

    string message;

    void setTCPFormat() {
        // Implementación para formato TCP si es necesario
    }

    void setProtocolBegin(char protocol) {
        message = protocol;
    }

    char getProtocolBegin() {
        return message[0];
    }

    void addName(const string& name) {
        message += fixNumberString(name.length(), 4) + name;
    }

    string getName() {
        int nameLength = stoi(message.substr(1, 4));  // Obtener la longitud del nombre
        return message.substr(5, nameLength);           // Extraer el nombre
    }

    void addMessage(const string& letter) {
        message += fixNumberString(letter.length(), 5) + letter;
    }

    int calculateChecksum() {
        int checksum = 0;
        for (char c : message) {
            checksum += static_cast<unsigned char>(c);
        }
        // Asegurarse de que el checksum tenga exactamente tres dígitos
        return checksum % 1000;
    }

    void setUDPFormat(int isEnd, int offset, int size) {
        int originalMsgSize = message.length();
        int checksum = calculateChecksum();
        message = addPadding(message, size - 14);
        char typeOfMessage = (offset ? 'd' : 's');
        message += to_string(isEnd) + fixNumberString(offset, 7) + fixNumberString(checksum, 3) + typeOfMessage;
        message += to_string(1000 - originalMsgSize - 15);
    }

    void show() {
        cout << message << endl;
    }
};
