#include <iostream>
#include <vector>
#include <string>
#include <getopt.h>
#include "funciones.h"


using namespace std;

int main(int argc, char* argv[]){
    int opt;
    string mensaje;
    vector<string> mensajesplit;
    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
            case 'm':
                mensaje = optarg;
                break;
            default:
                cerr<<"\033[31m" << "Error en el paso de parametros." <<"\033[0m"<<endl;
        }
    }
    mensajesplit = split(mensaje, ',');
    mensajesplit.front().erase(0,1);
    mensajesplit.back().erase(mensajesplit.back().length() - 1,1);
    int coreid = atoi(mensajesplit[0].c_str());
    int id = atoi(mensajesplit[1].c_str());
    string operacion = mensajesplit[2];
    float n1 = atof(mensajesplit[3].c_str());
    float n2 = atof(mensajesplit[4].c_str());
    string execcore = "./core -c " +mensajesplit[0]+" -i "+mensajesplit[1]+ " -o "+mensajesplit[2]+ " -n "+mensajesplit[3] +" -m "+mensajesplit[4];

    int status = system(execcore.c_str());
    if(status == -1){
        cerr << "Falló la ejecución del programa hijo" << endl;
    }



}