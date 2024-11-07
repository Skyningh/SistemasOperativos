#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define PORT 2020

int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
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

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("Hubo un error en el bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Hubo un error en el listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    cout << "Servidor corriendo y esperando conexiones port: " << PORT << endl;

    bool seguir = true;

    while (seguir){
        if((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
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
            string respuesta = "Mensaje recibido: " + mensaje;
            send(new_socket, respuesta.c_str(), respuesta.size(), 0);
        }
        close(new_socket);
    }

    close(server_fd);

    return 0;
}