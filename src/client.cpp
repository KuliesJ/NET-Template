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

class Cliente {
public:
    Cliente(const string &ip, int puerto, Protocolo protocolo);
    ~Cliente();
    
    void iniciar();
    void enviarDatos(const string &mensaje);
    void recibirDatos();
    void leerEntrada();
    string extractTCP();
    string extractUDP();

private:
    int socketCliente;
    string ip;
    int puerto;
    Protocolo protocolo;
    struct sockaddr_in servidorAddr;
    
    atomic<bool> continuarRecibiendo;
    thread hiloRecibir;

    void configurarSocket();
    void conectarTCP();
    void manejarConexionUDP();
};

Cliente::Cliente(const string &ip, int puerto, Protocolo protocolo)
    : ip(ip), puerto(puerto), protocolo(protocolo), socketCliente(-1), continuarRecibiendo(true) {
    memset(&servidorAddr, 0, sizeof(servidorAddr));
}

Cliente::~Cliente() {
    if (socketCliente != -1) {
        continuarRecibiendo = false;
        hiloRecibir.join();
        close(socketCliente);
    }
}

void Cliente::configurarSocket() {
    socketCliente = socket(AF_INET, (protocolo == Protocolo::TCP) ? SOCK_STREAM : SOCK_DGRAM, 0);
    if (socketCliente < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_port = htons(puerto);
    if (inet_pton(AF_INET, ip.c_str(), &servidorAddr.sin_addr) <= 0) {
        perror("Dirección IP inválida");
        exit(EXIT_FAILURE);
    }

    if (protocolo == Protocolo::TCP) {
        conectarTCP();
    }
}

void Cliente::conectarTCP() {
    if (connect(socketCliente, (struct sockaddr *)&servidorAddr, sizeof(servidorAddr)) < 0) {
        perror("Error al conectar con el servidor");
        exit(EXIT_FAILURE);
    }
    cout << "Conectado al servidor en " << ip << ":" << puerto << " (TCP)" << endl;
}

void Cliente::iniciar() {
    configurarSocket();
    hiloRecibir = thread(&Cliente::recibirDatos, this);

    leerEntrada();
}

void Cliente::enviarDatos(const string &mensaje) {
    if (protocolo == Protocolo::TCP) {
        if (send(socketCliente, mensaje.c_str(), mensaje.size(), 0) < 0) {
            perror("Error al enviar mensaje por TCP");
        } else {
            cout << "Mensaje enviado (TCP): " << mensaje << endl;
        }
    } else {
        if (sendto(socketCliente, mensaje.c_str(), mensaje.size(), 0, (struct sockaddr *)&servidorAddr, sizeof(servidorAddr)) < 0) {
            perror("Error al enviar mensaje por UDP");
        } else {
            cout << "Mensaje enviado (UDP): " << mensaje << endl;
        }
    }
}

string Cliente::extractTCP(){
    return "TCP Example";
}

string Cliente::extractUDP(){
    return "UDP Example";
}

void Cliente::recibirDatos() {
    char buffer[1024];
    while (continuarRecibiendo) {
        string juice;
        if (protocolo == Protocolo::TCP) {
            
            // Leer de uno en uno segun protocolo y agregar información a un string
            int bytesRecibidos = recv(socketCliente, buffer, 1, 0);
            if (bytesRecibidos > 0) {
                buffer[bytesRecibidos] = '\0';
                juice = extractTCP();
            }
        } else {
            
            socklen_t len = sizeof(servidorAddr);
            int bytesRecibidos = recvfrom(socketCliente, buffer, sizeof(buffer), 0, (struct sockaddr *)&servidorAddr, &len);
            if (bytesRecibidos > 0) {
                buffer[bytesRecibidos] = '\0';
                juice = extractUDP();
            }
        }
        cout << "Procesar mensaje natural " << juice << endl;
    }
}

void Cliente::leerEntrada() {
    string mensaje;
    while (true) {
        cout << "Escribe un mensaje (o 'salir' para terminar): ";
        getline(cin, mensaje);
        
        if (mensaje == "salir") {
            continuarRecibiendo = false;
            break;
        }

        enviarDatos(mensaje);
    }
}

int main() {
    string ipServidor = "127.0.0.1"; 
    int puerto = 8080;

    Cliente cliente(ipServidor, puerto, Protocolo::UDP);

    cliente.iniciar();

    return 0;
}
