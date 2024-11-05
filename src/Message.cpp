#include "Message.h"

Message::Message() : message("") {}

Message::Message(const string msg) : message(msg) {}

string Message::fixNumberString(int number, int size) {
    return string(size - to_string(number).length(), '0') + to_string(number);
}

string Message::addPadding(string message, int size) {
    return message + string(size - message.length(), '#');
}

void Message::setTCPFormat() {
    // Implementación para formato TCP si es necesario
}

void Message::setProtocolBegin(char protocol) {
    message = protocol;
}

char Message::getProtocolBegin() {
    return message[0];
}

void Message::addName(const string& name) {
    message += fixNumberString(name.length(), 4) + name;
}

string Message::getName() {
    int nameLength = stoi(message.substr(1, 4));
    return message.substr(5, nameLength);
}

void Message::addMessage(const string& letter) {
    message += fixNumberString(letter.length(), 5) + letter;
}

void Message::addBool(char value){
    message += value;
}

string Message::toString(){
    return message;
}

bool Message::isStructure(){
    return message[995] == 's';
}

string Message::getRawMessage() {
    // El mensaje completo se almacena en una variable miembro llamada 'message'
    size_t paddingStart = message.find('#');
    
    // Si se encuentra padding, devolvemos la parte del mensaje antes del padding
    if (paddingStart != std::string::npos) {
        return message.substr(0, paddingStart);
    }
    
    // Si no hay padding, devolvemos el mensaje completo sin el checksum
    return message.substr(0, 992);
}

bool Message::matchChecksum() {
    std::string rawMessage = getRawMessage();
    cout << rawMessage << endl;
    int expectedChecksum = std::stoi(message.substr(994, 3));
    cout << expectedChecksum << endl;
    int actualChecksum = std::accumulate(rawMessage.begin(), rawMessage.begin() + rawMessage.length(), 0);

    // Comparamos el checksum calculado con el esperado
    cout << actualChecksum % 1000 << endl;
    return actualChecksum % 1000 == expectedChecksum; // Nos aseguramos de que el checksum esté en un rango de 3 dígitos
}

int Message::calculateChecksum() {
    int checksum = 0;
    for (char c : message) {
        checksum += static_cast<unsigned char>(c);
    }
    // Asegurarse de que el checksum tenga exactamente tres dígitos
    return checksum % 1000;
}

void Message::setUDPFormat(int isEnd, int offset, int size) {
    int originalMsgSize = message.length();
    int checksum = calculateChecksum();
    message = addPadding(message, size - 14);
    char typeOfMessage = (offset ? 'd' : 's');
    message += to_string(isEnd) + fixNumberString(offset, 7) + fixNumberString(checksum, 3) + typeOfMessage;
    message += to_string(1000 - originalMsgSize - 15);
}

void Message::show() {
    cout << getRawMessage() << endl;
}
