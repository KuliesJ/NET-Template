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
        message += protocol;
    }

    void addName(string name){
        message += fixNumberString(name.length(), 4) + name;
    }

    void addMessage(string letter){
        message += fixNumberString(letter.length(), 5) + letter;
    }

    void setUDPFormat(int isEnd, int offset, int checksum, int size){
        int originalMsgSize = message.length();
        
        message = addPadding(message, size - 14);
        message += to_string(isEnd) + fixNumberString(offset, 7) + fixNumberString(checksum, 3);
        message += to_string(1000 - originalMsgSize - 14);
    }

};