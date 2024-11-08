#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <sstream>
#include "funciones.h"

using namespace std;

#define PORT 2021

// Cargar el índice invertido desde un archivo
map<string, vector<tuple<int, int>>> cargarIndice(const string& rutaArchivo) {
    map<string, vector<tuple<int, int>>> indice;
    ifstream archivo(rutaArchivo);
    string linea;

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string palabra;
        getline(ss, palabra, ';');  // Obtener la palabra clave
        vector<tuple<int, int>> documentos;
        string entrada;

        while (getline(ss, entrada, ';')) {
            entrada = entrada.substr(1, entrada.size() - 2);  // Eliminar paréntesis
            int coma = entrada.find(',');
            int docId = stoi(entrada.substr(0, coma));
            int frecuencia = stoi(entrada.substr(coma + 1));
            documentos.push_back(make_tuple(docId, frecuencia));
        }
        indice[palabra] = documentos;
    }
    return indice;
}

// Calcular puntajes combinados para los términos de búsqueda
map<int, int> calcularPuntajes(const vector<string>& palabras, const map<string, vector<tuple<int, int>>>& indice) {
    map<int, int> puntajes;
    for (const auto& palabra : palabras) {
        auto it = indice.find(palabra);
        if (it != indice.end()) {
            for (const auto& [docId, frecuencia] : it->second) {
                puntajes[docId] += frecuencia;  // Sumar frecuencia al puntaje del documento
            }
        }
    }
    return puntajes;
}

// Procesar la consulta, ordenar y limitar los resultados
vector<tuple<int, int>> procesarConsulta(const string& consulta, const map<string, vector<tuple<int, int>>>& indice) {
    istringstream ss(consulta);
    vector<string> palabras;
    string palabra;
    while (ss >> palabra) {
        palabras.push_back(palabra);
    }


    map<int, int> puntajes = calcularPuntajes(palabras, indice);
    vector<tuple<int, int>> resultados;
    for (const auto& [docId, puntaje] : puntajes) {
        resultados.push_back(make_tuple(docId, puntaje));
    }

    // Ordenar por puntaje (descendente) y limitar a TOPK
    sort(resultados.begin(), resultados.end(), [](const auto& a, const auto& b) {
        return get<1>(a) > get<1>(b) || (get<1>(a) == get<1>(b) && get<0>(a) < get<0>(b));
    });

    char* top = getenv("TOPK");
    int TOPK = atoi(top);

    if (resultados.size() > TOPK) {
        resultados.resize(TOPK);  // Limitar a TOPK
    }

    return resultados;
}

// Iniciar el servidor y procesar consultas
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Hubo un error creando el socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        perror("Hubo un error en setsockopt para SO_REUSEADDR");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Hubo un error en el bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Hubo un error en el listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    cout << "Servidor de MOTOR DE BÚSQUEDA corriendo en el puerto: " << PORT << endl;
    
    // Cargar el índice invertido al iniciar
    char* envPath = getenv("inverted_index");
    if (!envPath) {
        cerr << "Error: La variable de entorno 'inverted_index' no está definida." << endl;
        return EXIT_FAILURE;
    }
    string rutaIndice(envPath);
    map<string, vector<tuple<int, int>>> indice = cargarIndice(rutaIndice);

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("Hubo un error en el accept");
            continue;
        }

        // Leer consulta del CACHE
        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, 1024);
        string consulta(buffer);

        if (consulta == "SALIR AHORA") {
            cout << "Cerrando servidor de MOTOR DE BÚSQUEDA" << endl;
            break;
        }

        cout << "Consulta recibida: " << consulta << endl;

        // Procesar la consulta y obtener resultados

        vector<tuple<int, int>> resultados = procesarConsulta(consulta, indice);

        string respuesta;
        if (resultados.empty()) {
            respuesta = "NONE";  // Enviar "NONE" si no hay resultados
        } else {
            // Convertir los resultados a una cadena para enviar al CACHE
            stringstream ss;
            for (const auto& [docId, puntaje] : resultados) {
                ss << "(" << docId << "," << puntaje << ") ";
            }
            respuesta = ss.str();
        }
        // Enviar resultados al CACHE
        send(new_socket, respuesta.c_str(), respuesta.size(), 0);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
