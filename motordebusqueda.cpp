#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <tuple>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>
#include <vector>
#include "funciones.cpp"
#include "funciones.h"

using namespace std;

#define PORT 2021

map<string, vector<tuple<int, int>>> cargarii(string ii) {
    map<string, vector<tuple<int, int>>> index;
    ifstream archivo(ii);
    string linea;

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string palabra;
        // Lee cada palabra
        getline(ss, palabra, ';'); 

        vector<tuple<int, int>> docs;
        string entrada;
        while (getline(ss, entrada, ';')) {
            // Quitar parentesis
            entrada = entrada.substr(1, entrada.size() - 2);
            int comma_pos = entrada.find(',');

            int id = stoi(entrada.substr(0, comma_pos));
            int freq = stoi(entrada.substr(comma_pos + 1));

            docs.push_back(make_tuple(id, freq));
        }

        index[palabra] = docs;
    }
    return index;
}

map<int, int> get_combined_scores(vector<string> words, map<string, vector<tuple<int, int>>> index) {
    map<int, int> puntajes;

    for (auto word : words) {
        auto iterador = index.find(word);
        if (iterador != index.end()) {
            for (auto doc : iterador->second) {
                int id = get<0>(doc);
                int freq = get<1>(doc);
                puntajes[id] += freq;
            }
        }
    }

    return puntajes;
}

// Esta función envía los resultados de búsqueda al servidor de caché
void enviarAlCache(const vector<tuple<int, int>>& resultados) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creando socket para enviar al servidor de caché" << endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT); // Puerto del servidor de caché

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cout << "Dirección inválida / no soportada" << endl;
        close(sock);
        return;
    }

    // Conectar al servidor de caché
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Conexión al servidor de caché fallida" << endl;
        close(sock);
        return;
    }

    // Convertir los resultados a una cadena para enviarlos
    stringstream ss;
    for (const auto& resultado : resultados) {
        ss << "(" << get<0>(resultado) << "," << get<1>(resultado) << ") ";
    }
    string respuesta = ss.str();

    // Enviar los resultados al servidor de caché
    send(sock, respuesta.c_str(), respuesta.size(), 0);
    close(sock);
}

vector<tuple<int, int>> search_and_rank(string query, map<string, vector<tuple<int, int>>> index) {
    istringstream ss(query);
    string word;
    vector<string> words;

    while (ss >> word) {
        words.push_back(word);
    }

    auto scores = get_combined_scores(words, index);

    vector<tuple<int, int>> results;
    for (auto entry : scores) {
        results.push_back(make_tuple(entry.first, entry.second));
    }

    sort(results.begin(), results.end(), [](auto a, auto b) {
        return get<1>(a) > get<1>(b) || (get<1>(a) == get<1>(b) && get<0>(a) < get<0>(b));
    });

    // Enviar los resultados al servidor de caché
    enviarAlCache(results);

    return results;
}

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

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) != 0) {
        perror("Hubo un error en setsockopt para SO_REUSEPORT");
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

    cout << "Servidor corriendo y esperando conexiones en el puerto: " << PORT << endl;

    bool seguir = true;

    while (seguir){
            if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("Hubo un error en el accept");
            continue;
        }

        char buffer[1024] = {0};
        int valorleido = read(new_socket, buffer, 1024);
        string mensaje(buffer);

        cout << "Mensaje: " << mensaje << endl;

        if (mensaje == "SALIR AHORA"){
            cout << "Cerrando servidor" << endl;
            seguir = false;
        }
        else{
            char* iienv = getenv("inverted_index");
            string ii(iienv);
            int TOPK = 20;

            string respuesta = "Mensaje recibido: " + mensaje;
            send(new_socket, respuesta.c_str(), respuesta.size(), 0);
            auto index = cargarii(ii);

            auto resultados = search_and_rank(mensaje, index);


        }
        close(new_socket);
    }

    close(server_fd);

    return 0;
}