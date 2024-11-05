#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <unordered_map>
#include "Message.h"
using namespace std;

class UDPServer {
public:
    int sockfd;
    int port;
    unordered_map<string, sockaddr_in> clients; // Mapa para almacenar nombres y direcciones de clientes

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
        char typeOfMessage = msg[0];
        switch (typeOfMessage) {
            case 'n': // Iniciar sesión
                procedureN(msg, clientAddr);
                break;
            case 'b':  // Cerrar sesión
                procedureB(msg, clientAddr);
                break;
            case 'o':  // Cerrar sesión
                procedureO(msg, clientAddr);
                break;

            default:
                cout << "Something unexpected happened" << endl;
                //snprintf(response, sizeof(response), "Comando no reconocido");
                break;
        }

        // Enviar respuesta al cliente
        //sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
    }

    void procedureN(string msg, sockaddr_in clientAddr) {
        int nameLength = stoi(msg.substr(1, 4));
        string name = msg.substr(1 + 4, nameLength);
        cout << "Nombre recibido: " << name << endl;

        Message response;
        response.setProtocolBegin('N');

        // Verificar si el nombre ya existe en el mapa
        if (clients.find(name) == clients.end()) {
            // Si no existe, agregarlo
            clients[name] = clientAddr;
            response.addBool('1');  // Éxito
        } else {
            // Si ya existe, indicar fallo
            response.addBool('0');  // Fallo
        }

        response.setUDPFormat(1,0,1000);

        // Enviar respuesta al cliente
        string responseStr = response.toString();
        sendto(sockfd, responseStr.c_str(), responseStr.size(), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
    }

    void procedureB(string msg, sockaddr_in clientAddr){
        cout << "Broadcast" << endl;
    }

    void procedureO(string msg, sockaddr_in clientAddr) {
        bool found = false;

        // Buscar en el mapa de clientes la dirección `clientAddr`
        for (auto it = clients.begin(); it != clients.end(); ++it) {
            if (it->second.sin_addr.s_addr == clientAddr.sin_addr.s_addr &&
                it->second.sin_port == clientAddr.sin_port) {
                // Si encontramos una coincidencia, eliminamos la entrada
                cout << "Cerrando sesión para el cliente: " << it->first << endl;
                clients.erase(it);
                found = true;
                break;
            }
        }
        /*
        // Crear y configurar la respuesta para el cliente
        Message response;
        response.setProtocolBegin('O');
        response.addBool(found ? '1' : '0');  // '1' para éxito, '0' para fallo
        response.setUDPFormat(1, 0, 1000);

        // Enviar respuesta al cliente
        string responseStr = response.toString();
        //sendto(sockfd, responseStr.c_str(), responseStr.size(), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
        */
        // Mostrar el resultado de la operación en el servidor
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
