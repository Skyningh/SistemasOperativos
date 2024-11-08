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
#include <deque>
#include "funciones.h"

using namespace std;

#define PORT 2020
#define MOTOR_PORT 2021  // Puerto para el motor de búsqueda

// Declaración de funciones
vector<tuple<int, int>> buscarPalabraCache(const string& palabra, map<string, vector<tuple<int, int>>>& cache);
void enviarAMotorBusqueda(const string& palabra, map<string, vector<tuple<int, int>>>& cache, deque<string>& ordenAcceso, int ncache, int new_socket);
string toString(const vector<tuple<int, int>>& vec);
vector<tuple<int, int>> parsearResultados(const string& respuesta);
void añadirCache(const string& palabra, const vector<tuple<int, int>>& resultados, map<string, vector<tuple<int, int>>>& cache, deque<string>& ordenAcceso, int ncache);

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

    // Leer tamaño máximo del caché
    char* memory_size = getenv("MEMORY_SIZE");
    int ncache = memory_size ? atoi(memory_size) : 10;

    // Inicializar el caché y el orden de acceso
    map<string, vector<tuple<int, int>>> cache;
    deque<string> ordenAcceso;

    bool seguir = true;
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
        } else {
            vector<tuple<int, int>> busqueda = buscarPalabraCache(mensaje, cache);
            if (!busqueda.empty()) {
                // Si está en caché, devolver la respuesta al cliente
                string resultadoStr = toString(busqueda);
                string respuesta = "Palabra encontrada en el caché: " + resultadoStr;
                send(new_socket, respuesta.c_str(), respuesta.length(), 0);
            } else {
                // Si no está en caché, enviar la palabra al motor de búsqueda
                enviarAMotorBusqueda(mensaje, cache, ordenAcceso, ncache, new_socket);
            }
        }
        close(new_socket);
    }

    close(server_fd);
    return 0;
}

// Función que envía la consulta al MOTOR DE BÚSQUEDA y añade al caché si hay resultados
void enviarAMotorBusqueda(const string& palabra, map<string, vector<tuple<int, int>>>& cache, deque<string>& ordenAcceso, int ncache, int new_socket) {
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

    // Convertir dirección IPv4
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

    // Esperar respuesta del motor de búsqueda
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    cout << "Respuesta del motor de búsqueda: " << buffer << endl;
    string respuesta(buffer);

    // Procesar la respuesta recibida
    if (respuesta != "NONE") {
        vector<tuple<int, int>> resultados = parsearResultados(respuesta);
        añadirCache(palabra, resultados, cache, ordenAcceso, ncache);

        // Enviar la respuesta del motor al buscador
        string resultadoStr = toString(resultados);
        string respuestaFinal = resultadoStr;
        send(new_socket, respuestaFinal.c_str(), respuestaFinal.length(), 0);
    } else {
        // Enviar "NONE" al buscador si no se encontraron resultados
        string respuestaFinal = "NONE";
        send(new_socket, respuestaFinal.c_str(), respuestaFinal.length(), 0);
    }

    close(sock);
}

// Función que convierte la respuesta en un vector de tuplas
vector<tuple<int, int>> parsearResultados(const string& respuesta) {
    vector<tuple<int, int>> resultados;
    istringstream ss(respuesta);
    string item;
    while (getline(ss, item, ' ')) {
        if (item.front() == '(' && item.back() == ')') {
            item = item.substr(1, item.size() - 2); // Quitar paréntesis
            int coma = item.find(',');
            int docId = stoi(item.substr(0, coma));
            int frecuencia = stoi(item.substr(coma + 1));
            resultados.push_back(make_tuple(docId, frecuencia));
        }
    }
    return resultados;
}

// Función que añade elementos al caché y respeta el límite de tamaño
void añadirCache(const string& palabra, const vector<tuple<int, int>>& resultados, 
                 map<string, vector<tuple<int, int>>>& cache, deque<string>& ordenAcceso, int ncache) {
    // Verificar si el caché ha alcanzado su tamaño máximo
    if (cache.size() >= ncache) {
        // Eliminar la entrada más antigua
        string claveAntigua = ordenAcceso.front();
        ordenAcceso.pop_front();
        cache.erase(claveAntigua);
        cout << "Eliminada entrada más antigua del caché: " << claveAntigua << endl;
    }

    // Añadir la nueva entrada al caché y registrar su orden de acceso
    cache[palabra] = resultados;
    ordenAcceso.push_back(palabra);

    cout << "Añadida palabra al caché: " << palabra << endl;
}

// Función que verifica si la palabra está en el caché
vector<tuple<int, int>> buscarPalabraCache(const string& palabra, map<string, vector<tuple<int, int>>>& cache) {
    auto it = cache.find(palabra);
    if (it != cache.end()) {
        cout << "Palabra encontrada en el CACHÉ" << endl;
        return it->second;
    } else {
        cout << "Palabra no encontrada en CACHÉ, llamando a MOTOR DE BÚSQUEDA" << endl;
        return {};
    }
}

// Función para convertir un vector de tuplas a string
string toString(const vector<tuple<int, int>>& vec) {
    stringstream ss;
    for(size_t i = 0; i < vec.size(); i++) {
        ss << "(" << get<0>(vec[i]) << "," << get<1>(vec[i]) << ")";
        if(i < vec.size() - 1) {
            ss << " "; // Separar cada tupla con un espacio
        }
    }
    return ss.str();
}
