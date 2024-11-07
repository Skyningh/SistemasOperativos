#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

#define PORT 2020

int main (){
    ifstream archivo("/home/rudy/2024/SO/SistemasOperativos/complementos/mapa.txt");
    if (!archivo.is_open()){
        cerr << "El archivo no se pudo abrir" << endl;
        return EXIT_FAILURE;
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    string mensaje;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        cerr << "Hubo un error al crear el socket" << endl;
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        cerr << "Direccion mala" << endl;
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        cerr << "Fallo la conexion" << endl;
        close(sock);
        return -1;
    }

    cout << "Escriba el mensaje, si quiere terminar el programa, escriba 'SALIR AHORA'" << endl;
    cin >> mensaje;

    send(sock, mensaje.c_str(), mensaje.size(), 0);

    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    cout << "Respuesta desde el servidor: " << buffer << endl;

    close(sock);





    string palabra;
    cout << "Ingrese una palabra a buscar: ";
    cin >> palabra;

    //Aquí definimos la estructura en la que almacenaremos las ocurrencias de palabras accedidas recientemente. 
    //Es una map donde la key es la palabra (un string) y el value, una lista de tuplas, que son los libros y su cantidad de ocurrencias
    map <string, vector<tuple <int, int>>> cache;

    //Verificamos si es que la palabra buscada está en el caché.
    auto it = cache.find(palabra);
    if (it != cache.end()) {
        //cout << "Palabra: "<< it->first << " encontrada en el caché, con ocurrencia en: " << it->second << endl;
        cout << "Encontrado en el cache" << endl;
    } else {
        cout << "Clave no encontrada, llamando a MOTOR DE BÚSQUEDA" << std::endl;
    }

    archivo.close();

    //Comienza el proceso de búsqueda de la palabra
    return 0;
}