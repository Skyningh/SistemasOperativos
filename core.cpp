#include <iostream>
#include <vector>
#include <getopt.h>
#include <fstream>
#include "funciones.h"
using namespace std;

float suma(float n1,float n2){
    return n1+n2;
}

float resta(float n1,float n2){
    return n1-n2;
}

float multiplicacion(float n1,float n2){
    return n1*n2;
}

float division(float n1,float n2){
    if(n2 == 0.0){
        cerr << "No se puede dividir por 0" << endl;
        return -1;
    } else{
        return n1/n2;
    }
}


int main(int argc, char* argv[]){

    int opt;
    string coreid;
    string id;
    string operacion;
    float n1;
    float n2;
    vector<string> mensajesplit;
    while ((opt = getopt(argc, argv, "c:i:o:n:m:")) != -1) {
        switch (opt) {
            case 'c':
                coreid = optarg;
                break;
            case 'i':
                id = optarg;
                break;
            case 'o':
                operacion = optarg;
                break;
            case 'n':
                n1 = atof(optarg);
                break;
            case 'm':
                n2 = atof(optarg);
                break;
            default:
                cerr<<"\033[31m" << "Error en el paso de parametros." <<"\033[0m"<<endl;
        }
    }

    ofstream file("complementos/cores/core"+coreid+".txt");
    if(!file.is_open()){
                cerr << "El archivo complementos/cores/core"+coreid+".txt"+" no se pudo abrir" << endl;
                return EXIT_FAILURE;
            }
    file << "0";
    file.close();
    

    if(operacion == "suma"){
        return suma(n1,n2);
    } else if(operacion == "resta"){
        return resta(n1,n2);
    } else if(operacion == "multiplicacion"){
        return multiplicacion(n1,n2);
    } else if(operacion == "division"){
        return division(n1,n2);
    } else{
        cerr << "La operacion no existe" << endl;
    }

    return 0;
}