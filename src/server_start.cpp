#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <unordered_map>

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
        char response[1024];
        char typeOfMessage = msg[0];
        int nameLength = stoi(msg.substr(1, 4));
        string name = msg.substr(5, nameLength);

        switch (typeOfMessage) {
            case 'n':  // Registrar nombre de usuario
                cout << "Registrando nombre de usuario: " << name << endl;
                // Almacenar el c clientAddr clientAddrliente en el mapa
                clients[name] = clientAddr; 
                snprintf(response, sizeof(response), "Nombre de usuario %s registrado correctamente", name.c_str());
                cout << &clientAddr << endl;
                break;
            
            case 'o':  // Cerrar sesión
                cout << "Cerrando sesión para el usuario: " << name << endl;
                clients.erase(name); // Eliminar el cliente del mapa
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
    UDPServer server(8080);  // Puerto que debe coincidir con el cliente
    server.start();
    return 0;
}
