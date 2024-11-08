#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <tuple>
#include <sstream>
#include <cstring>

using namespace std;

#define PORT 2020
#define MOTOR_PORT 2021  // Puerto para el motor de búsqueda

// Declaración de funciones
vector<tuple<int, int>> buscarPalabraCache(string palabra, map<string, vector<tuple<int, int>>>& cache);
void enviarAMotorBusqueda(const string& palabra);
string toString(vector<tuple<int, int>> vec);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Hubo un error creando el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar opciones del socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        perror("Hubo un error en setsockopt para SO_REUSEADDR");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) != 0) {
        perror("Hubo un error en setsockopt para SO_REUSEPORT");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Configurar dirección
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Hacer bind del socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Hubo un error en el bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones
    if (listen(server_fd, 3) < 0) {
        perror("Hubo un error en el listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    cout << "Servidor corriendo y esperando conexiones en puerto: " << PORT << endl;

    bool seguir = true;
    map<string, vector<tuple<int, int>>> cache;
    
    // Inicializar el caché con algunos datos de ejemplo
    cache["potito"].push_back({1, 2});

    while (seguir) {
        // Aceptar nueva conexión
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Hubo un error en el accept");
            continue;
        }

        char buffer[1024] = {0};
        int valorleido = read(new_socket, buffer, 1024);
        string mensaje(buffer);

        cout << "Mensaje recibido: " << mensaje << endl;

        if (mensaje == "SALIR AHORA") {
            cout << "Cerrando servidor" << endl;
            seguir = false;
        }
        else {
            vector<tuple<int, int>> busqueda = buscarPalabraCache(mensaje, cache);
            if (!busqueda.empty()) {
                string resultadoStr = toString(busqueda);
                string respuesta = "Palabra encontrada en el caché: " + resultadoStr;
                send(new_socket, respuesta.c_str(), respuesta.length(), 0);
            } else {
                // Si no está en caché, enviar al motor de búsqueda
                enviarAMotorBusqueda(mensaje);
                string respuesta = "Palabra no encontrada en caché, enviada al motor de búsqueda";
                send(new_socket, respuesta.c_str(), respuesta.length(), 0);
            }
        }
        close(new_socket);
    }

    close(server_fd);
    return 0;
}

void enviarAMotorBusqueda(const string& palabra) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creando socket para motor de búsqueda" << endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(MOTOR_PORT);
    memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    // Convertir IPv4 o IPv6 de texto a binario
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cout << "Dirección inválida / no soportada" << endl;
        close(sock);
        return;
    }

    // Conectar al motor de búsqueda
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Conexión al motor de búsqueda fallida" << endl;
        close(sock);
        return;
    }

    // Enviar la palabra
    send(sock, palabra.c_str(), palabra.length(), 0);
    
    // Esperar respuesta (opcional)
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    cout << "Respuesta del motor de búsqueda: " << buffer << endl;
    close(sock);
}

vector<tuple<int, int>> buscarPalabraCache(string palabra, map<string, vector<tuple<int, int>>>& cache) {
    auto it = cache.find(palabra);
    if (it != cache.end()) {
        cout << "Palabra encontrada en el cache" << endl;
        return it->second;
    } else {
        cout << "Palabra no encontrada, llamando a MOTOR DE BÚSQUEDA" << endl;
        return {};
    }
}

string toString(vector<tuple<int, int>> vec) {
    stringstream ss;
    ss << "[";
    for(size_t i = 0; i < vec.size(); i++) {
        ss << "(" << get<0>(vec[i]) << "," << get<1>(vec[i]) << ")";
        if(i < vec.size() - 1) {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}