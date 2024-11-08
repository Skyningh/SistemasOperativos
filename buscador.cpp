#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <sstream>

using namespace std;

#define PORT 2020

map<string, int> cargarMapaArchivos(const string& rutaArchivo) {
    map<string, int> mapaArchivos;
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de mapa de archivos." << endl;
        exit(EXIT_FAILURE);
    }

    string linea;
    while (getline(archivo, linea)) {
        // Ignorar líneas vacías
        if (linea.empty()) continue;
        
        // Encontrar la posición de la coma
        size_t pos = linea.find(',');
        if (pos != string::npos) {
            string nombreArchivo = linea.substr(0, pos);
            // Eliminar espacios en blanco después de la coma
            string idStr = linea.substr(pos + 1);
            idStr.erase(0, idStr.find_first_not_of(" \t"));
            
            try {
                int id = stoi(idStr);
                mapaArchivos[nombreArchivo] = id;
            } catch (const exception& e) {
                cerr << "Error al convertir ID para " << nombreArchivo << ": " << e.what() << endl;
            }
        }
    }
    
    archivo.close();
    return mapaArchivos;
}

int main() {
    char* mapa_archivos = getenv("mapa_archivos");
    if (!mapa_archivos) {
        cerr << "Error: Variable de entorno 'mapa_archivos' no definida" << endl;
        return EXIT_FAILURE;
    }

    map<string, int> mapaArchivos = cargarMapaArchivos(mapa_archivos);
    
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Error al crear el socket" << endl;
        return EXIT_FAILURE;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Error de conexión" << endl;
        close(sock);
        return EXIT_FAILURE;
    }

    cout << "\nEscriba la palabra a buscar (SALIR AHORA para terminar): ";
    string mensaje;
    getline(cin, mensaje);
    
    cout << "Enviando consulta: '" << mensaje << "'" << endl;
    send(sock, mensaje.c_str(), mensaje.length() + 1, 0);

    char buffer[1024] = {0};
    int valread = read(sock, buffer, sizeof(buffer) - 1);
    
    if (valread < 0) {
        cerr << "Error al leer la respuesta del servidor" << endl;
        close(sock);
        return EXIT_FAILURE;
    }
    
    string respuesta(buffer);
    cout << "\nRespuesta del servidor: ";

    istringstream ss(respuesta);
    string resultado;
    bool encontroResultados = false;
    
    while (ss >> resultado) {        
        if (resultado.front() == '(' && resultado.back() == ')') {
            // Quitar paréntesis
            resultado = resultado.substr(1, resultado.size() - 2);
            
            // Encontrar la posición de la coma
            size_t comaPos = resultado.find(',');
            if (comaPos == string::npos) {
                cerr << "Formato inválido en resultado: " << resultado << endl;
                continue;
            }
            
            try {
                // Extraer ID y frecuencia
                string idStr = resultado.substr(0, comaPos);
                string frecStr = resultado.substr(comaPos + 1);
                
                // Limpiar espacios
                idStr.erase(0, idStr.find_first_not_of(" \t"));
                idStr.erase(idStr.find_last_not_of(" \t") + 1);
                frecStr.erase(0, frecStr.find_first_not_of(" \t"));
                frecStr.erase(frecStr.find_last_not_of(" \t") + 1);
                
                int idArchivo = stoi(idStr);
                int frecuencia = stoi(frecStr);
                
                // Buscar en el mapa
                bool encontrado = false;
                for (const auto& [nombreArchivo, id] : mapaArchivos) {
                    if (id == idArchivo) {
                             cout<< "(" << nombreArchivo << "," << frecuencia << ") ";
                             
                        encontrado = true;
                        encontroResultados = true;
                        break;
                    }
                }
                
                if (!encontrado) {
                    cout << "No se encontró archivo para ID: " << idArchivo << endl;
                }
                
            } catch (const exception& e) {
                cerr << "Error procesando resultado '" << resultado << "': " << e.what() << endl;
            }
        }
    }
    cout<<endl;
    if (!encontroResultados) {
        cout << "No se encontraron coincidencias en ningún archivo." << endl;
    }

    close(sock);
    return 0;
}