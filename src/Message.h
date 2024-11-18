#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <numeric>
using namespace std;

class Message {
public:
    // Constructor por defecto
    Message();

    // Constructor que acepta un string
    Message(const string msg);

    void setTCPFormat();
    void setProtocolBegin(char protocol);
    char getProtocolBegin();
    void addName(const string& name);
    void addBool(char value);
    string getName();
    void addMessage(const string& letter);
    int calculateChecksum();
    void setUDPFormat(int isEnd, int offset, int size);
    void show();
    string toString();
    bool isStructure();
    string getRawMessage();
    bool matchChecksum();

    void addDate();
    
private:
    
    string message;
    // Métodos auxiliares
    string fixNumberString(int number, int size);
    string addPadding(string message, int size);
};

#endif // MESSAGE_H
