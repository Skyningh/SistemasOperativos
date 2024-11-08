#include <iostream>
#include <string>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

#define PORT 2020

int main (){
    ifstream archivo("complementos/mapa.txt");
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

    cout << "Escriba la palabra a buscar, si quiere terminar el programa, escriba 'SALIR AHORA'" << endl;
    getline(cin, mensaje);
    
    cout<< "Enviando a cache..."<<endl;

    //Envía el mensaje a caché
    send(sock, mensaje.c_str(), mensaje.size(), 0);

    //Recibe la respuesta de caché
    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    cout << "Respuesta desde el servidor: " << buffer << endl;

    close(sock);

    archivo.close();

    //Comienza el proceso de búsqueda de la palabra
    return 0;
}