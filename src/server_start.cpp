#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

class UDPServer {
public:
    int sockfd;
    int port;

    UDPServer(int port) : port(port) {
        // Crear el socket UDP
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            cerr << "Error al crear el socket" << endl;
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
            cerr << "Error al enlazar el socket" << endl;
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        cout << "Servidor UDP escuchando en el puerto " << port << endl;
    }

    ~UDPServer() {
        close(sockfd);
    }

    void start() {
        char buffer[1024];
        struct sockaddr_in clientAddr;
        socklen_t len = sizeof(clientAddr);

        while (true) {
            ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&clientAddr, &len);
            if (n > 0) {
                buffer[n] = '\0';
                cout << "Mensaje recibido: " << buffer << endl;

                handleClientMessage(string(buffer), clientAddr);
            } else {
                cerr << "Error al recibir mensaje" << endl;
            }
        }
    }

private:
    void handleClientMessage(const string& msg, struct sockaddr_in& clientAddr) {
        char response[1024];

        char typeOfMessage = msg[0];
        int nameLength = stoi(msg.substr(1, 4));
        string name = msg.substr(5, nameLength);
        string padding = msg.substr(5 + nameLength, 500);
        string additionalData = msg.substr(5 + nameLength + padding.length());
        switch (typeOfMessage) {
            case 'n':  // Registrar nombre de usuario
                cout << "Registrando nombre de usuario: " << name << endl;
                snprintf(response, sizeof(response), "Nombre de usuario %s registrado correctamente", name.c_str());
                break;
            
            case 'o':  // Cerrar sesión
                cout << "Cerrando sesión para el usuario: " << name << endl;
                snprintf(response, sizeof(response), "Sesión cerrada correctamente para %s", name.c_str());
                break;

            default:
                snprintf(response, sizeof(response), "Comando no reconocido");
                break;
        }

        // Enviar respuesta al cliente
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
    }
};

int main() {
    UDPServer server(8080);
    server.start();
    return 0;
}
