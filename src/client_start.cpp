#include <iostream>
#include <string>
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "Message.h"
//UDP CLIENT
class UDPClient {
public:
    int sockfd;
    int port;
    string host;
    string nickname;
    UDPClient(const string& host, int port) : host(host), port(port) {
        // Crear el socket UDP
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            cerr << "Error al crear el socket" << endl;
            exit(EXIT_FAILURE);
        }

        // Configurar la dirección del servidor
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
            cerr << "Dirección no válida o no soportada" << endl;
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Iniciar el hilo para leer mensajes
        thread(&UDPClient::receiveMessages, this).detach();
    }

    ~UDPClient() {
        close(sockfd);
    }

    void start(){
        char procedure;
        while(true){
            displayMenu();
            cin >> procedure;
            processInput(procedure);
        }
    }

private:
    void displayMenu() {
        cout << "Seleccione una opción:\n";
        cout << "n: Registrar nombre de usuario\n";
        cout << "m: Enviar mensaje\n";
        cout << "o: Salir\n";
        cout << "Ingrese su elección: ";
    }
    struct sockaddr_in serverAddr;

    void processInput(char procedure){
        switch (procedure)
        {
        case 'n':
            procedureN();
            break;
        case 'o':
            procedureO();
            break;
        default:
            cout << procedure << " is not a implemented procedure\n";
            break;
        }
    }
    
    void procedureN(){
        Message msg;
        msg.setProtocolBegin('n');
        cout << "Insert desired nickname: ";
        cin >> nickname;
        msg.addName(nickname);
        msg.setUDPFormat(1,0,1000);
        sendMessage(msg);
    }

    void procedureO(){
        Message msg;
        msg.setProtocolBegin('o');
        cout << "You requested to logout\n";
        msg.setUDPFormat(1,0,1000);
        sendMessage(msg);
    }
    
    void sendMessage(Message msg) {
        string mess = msg.message;
        msg.show();
        //sendto(sockfd, mess.c_str(), mess.size(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    }

    void receiveMessages() {
        char buffer[1024];
        while (true) {
            socklen_t len = sizeof(serverAddr);
            ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&serverAddr, &len);
            if (n > 0) {
                buffer[n] = '\0';
                cout << "Mensaje recibido: " << buffer << endl;
            } else {
                cerr << "Error al recibir mensaje" << endl;
                break;
            }
        }
    }


};

int main() {
    UDPClient client("127.0.0.1", 8080);
    client.start();
    return 0;
}