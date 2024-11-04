#include <string>
using namespace std;

string fixNumberString(int number, int size){
    return string(size - to_string(number).length(), '0') + to_string(number);
}

string addPadding(string message, int size){
    return message + string(size - message.length(), '#');
}

class Message{
public:

    Message(): message(""){};

    string message;
    void setTCPFormat(){

    }

    void setProtocolBegin(char protocol){
        message = protocol;
    }

    void addName(string name){
        message += fixNumberString(name.length(), 4) + name;
    }

    void addMessage(string letter){
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

    void setUDPFormat(int isEnd, int offset, int size){
        int originalMsgSize = message.length();
        int checksum = calculateChecksum();
        message = addPadding(message, size - 14);
        char typeOfMessage = offset ? 'd' : 's';
        message += to_string(isEnd) + fixNumberString(offset, 7) + fixNumberString(checksum, 3) + typeOfMessage;
        message += to_string(1000 - originalMsgSize - 15);
    }

    void show(){
        cout << message << endl;
    }

};