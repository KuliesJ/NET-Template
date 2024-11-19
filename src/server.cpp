#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <atomic>

using namespace std;

enum class Protocolo {
    TCP,
    UDP
};

class Servidor {
public:
    Servidor(int puerto, Protocolo protocolo);
    ~Servidor();
    
    void iniciar();
    void esperarConexion();
    void recibirDatos();
    void enviarDatos(const string &mensaje);
    
private:
    int puerto;
    Protocolo protocolo;
    int socketServidor;
    
    struct sockaddr_in servidorAddr, clienteAddr;
    
    void configurarSocket();
    void configurarTCP();
    void configurarUDP();
    void manejarConexionTCP();
    void manejarConexionUDP();
};

Servidor::Servidor(int puerto, Protocolo protocolo)
    : puerto(puerto), protocolo(protocolo), socketServidor(-1) {
    memset(&servidorAddr, 0, sizeof(servidorAddr));
    memset(&clienteAddr, 0, sizeof(clienteAddr));
}

Servidor::~Servidor() {
    if (socketServidor != -1) {
        close(socketServidor);
    }
}

void Servidor::configurarSocket() {
    socketServidor = socket(AF_INET, (protocolo == Protocolo::TCP) ? SOCK_STREAM : SOCK_DGRAM, 0);
    if (socketServidor < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = INADDR_ANY;
    servidorAddr.sin_port = htons(puerto);
}

void Servidor::configurarTCP() {
    if (bind(socketServidor, (struct sockaddr *)&servidorAddr, sizeof(servidorAddr)) < 0) {
        perror("Error al hacer bind");
        exit(EXIT_FAILURE);
    }
    if (listen(socketServidor, 5) < 0) {
        perror("Error al escuchar");
        exit(EXIT_FAILURE);
    }
    cout << "Servidor TCP escuchando en el puerto " << puerto << endl;
}

void Servidor::configurarUDP() {
    if (bind(socketServidor, (struct sockaddr *)&servidorAddr, sizeof(servidorAddr)) < 0) {
        perror("Error al hacer bind");
        exit(EXIT_FAILURE);
    }
    cout << "Servidor UDP escuchando en el puerto " << puerto << endl;
}

void Servidor::iniciar() {
    configurarSocket();
    
    if (protocolo == Protocolo::TCP) {
        configurarTCP();
    } else {
        configurarUDP();
    }
}

void Servidor::esperarConexion() {
    if (protocolo == Protocolo::TCP) {
        while (true)
            manejarConexionTCP();
    } else {
        while (true)
            manejarConexionUDP();
    }
}

void Servidor::manejarConexionTCP() {
    socklen_t len = sizeof(clienteAddr);
    int socketCliente = accept(socketServidor, (struct sockaddr *)&clienteAddr, &len);
    if (socketCliente < 0) {
        perror("Error al aceptar la conexión");
        return;
    }
    cout << "Cliente conectado desde " << inet_ntoa(clienteAddr.sin_addr) << endl;

    char buffer[1024];
    int bytesRecibidos = read(socketCliente, buffer, sizeof(buffer));
    if (bytesRecibidos > 0) {
        buffer[bytesRecibidos] = '\0';
        cout << "Mensaje recibido: " << buffer << endl;
    }

    close(socketCliente);
}

void Servidor::manejarConexionUDP() {
    socklen_t len = sizeof(clienteAddr);
    char buffer[1024];
    
    int bytesRecibidos = recvfrom(socketServidor, buffer, sizeof(buffer), 0, (struct sockaddr *)&clienteAddr, &len);
    if (bytesRecibidos > 0) {
        buffer[bytesRecibidos] = '\0';
        cout << "Mensaje recibido desde " << inet_ntoa(clienteAddr.sin_addr) << ": " << buffer << endl;
    }
}

void Servidor::recibirDatos() {
    esperarConexion();
}

void Servidor::enviarDatos(const string &mensaje) {
    if (protocolo == Protocolo::TCP) {
    } else {
        sendto(socketServidor, mensaje.c_str(), mensaje.size(), 0, (struct sockaddr *)&clienteAddr, sizeof(clienteAddr));
        cout << "Mensaje enviado: " << mensaje << endl;
    }
}

int main() {
    Servidor servidor(8080, Protocolo::UDP);
    servidor.iniciar();
    servidor.recibirDatos();
    return 0;
}
