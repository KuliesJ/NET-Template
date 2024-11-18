#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include "Message.h"
using namespace std;

class UDPServer {
public:
    int sockfd;
    int port;
    map<string, pair<string, int>> clientsByName;  // Mapa de nombre a IP y puerto
    map<int, string> clientsByPort;                // Mapa de puerto a nombre

    UDPServer(int port) : port(port) {
        // Crear el socket UDP
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            cerr << "Error al crear el socket" << endl;
            exit(EXIT_FAILURE);
        }

        // Configurar la dirección del servidor
        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        // Enlazar el socket con la dirección y puerto
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
        sockaddr_in clientAddr;
        socklen_t len = sizeof(clientAddr);

        while (true) {
            // Recibir mensaje del cliente
            ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&clientAddr, &len);
            if (n > 0) {
                buffer[n] = '\0';  // Asegurarse de que el buffer esté terminado en nulo
                cout << "Mensaje recibido: " << buffer << endl;

                handleClientMessage(string(buffer), clientAddr);
            } else {
                cerr << "Error al recibir mensaje" << endl;
            }
        }
    }

private:
    void handleClientMessage(const string& msg, sockaddr_in& clientAddr) {
        Message mensajito(msg);
        if (mensajito.matchChecksum()) {
            cout << "Matched checksum\n";

            if (msg[997] == 's') {
                cout << "Is structure\n";
                char typeOfMessage = msg[0];
                switch (typeOfMessage) {
                    case 'n': // Iniciar sesión
                        procedureN(msg, clientAddr);
                        break;
                    case 'm':
                        procedureM(msg, clientAddr);
                        break;
                    case 'b':  // Cerrar sesión
                        procedureB(msg, clientAddr);
                        break;
                    case 'o':  // Cerrar sesión
                        procedureO(msg, clientAddr);
                        break;

                    default:
                        cout << "Algo inesperado ocurrió" << endl;
                        break;
                }
            }
        }
    }

    void procedureN(string msg, sockaddr_in clientAddr) {
        int nameLength = stoi(msg.substr(1, 4));
        string name = msg.substr(1 + 4, nameLength);
        cout << "Nombre recibido: " << name << endl;

        Message response;
        response.setProtocolBegin('N');

        // Extraemos la IP y el puerto del cliente
        string ip = inet_ntoa(clientAddr.sin_addr);
        int port = ntohs(clientAddr.sin_port);

        if (clientsByName.find(name) == clientsByName.end()) {
            // Guardamos en el mapa clientsByName (nombre -> (IP, puerto))
            clientsByName[name] = make_pair(ip, port);
            // También guardamos en el mapa clientsByPort (puerto -> nombre)
            clientsByPort[port] = name;
            response.addBool('1');  // Éxito
        } else {
            response.addBool('0');  // Fallo
        }

        response.setUDPFormat(1, 0, 1000);

        // Enviar respuesta al cliente
        string responseStr = response.toString();
        sendto(sockfd, responseStr.c_str(), responseStr.size(), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
    }

    void procedureM(string msg, sockaddr_in clientAddr) {
        cout << "Message: " << msg << endl;
        
        // Extraemos el puerto del sockaddr_in
        int port = ntohs(clientAddr.sin_port);
        if (clientsByPort.find(port) != clientsByPort.end()) {
            cout << "Message from: " << clientsByPort[port] << endl;
        } else {
            cout << "Cliente no encontrado para el puerto: " << port << endl;
        }
    }


    void procedureB(string msg, sockaddr_in clientAddr) {
        cout << "Broadcast" << endl;
    }

    void procedureO(string msg, sockaddr_in clientAddr) {
        bool found = false;
        int port = ntohs(clientAddr.sin_port);

        // Buscar en el mapa de puerto a nombre
        auto it = clientsByPort.find(port);
        if (it != clientsByPort.end()) {
            // Si encontramos el puerto, borramos tanto del mapa clientsByName como del mapa clientsByPort
            cout << "Cerrando sesión para el cliente: " << it->second << endl;
            clientsByName.erase(it->second);   // Borrar del mapa de nombre a IP+puerto
            clientsByPort.erase(it);           // Borrar del mapa de puerto a nombre
            found = true;
        }

        if (found) {
            cout << "Cliente eliminado exitosamente." << endl;
        } else {
            cout << "Error: cliente no encontrado en el mapa." << endl;
        }
    }
};

int main() {
    UDPServer server(8080);  // Puerto que debe coincidir con el cliente
    server.start();
    return 0;
}
