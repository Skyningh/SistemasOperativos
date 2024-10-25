#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "funciones.h"
#include "funciones.cpp"

using namespace std;

int main(int argc, char* argv[]){
    int opt;
    float resultado;
    string mensaje;
    vector<string> mensajesplit;
    cout << "ALO" << endl;
    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
            case 'm':
                mensaje = optarg;
                break;
            default:
                cerr<<"\033[31m" << "Error en el paso de parametros." <<"\033[0m"<<endl;
                return EXIT_FAILURE;
        }
    }
    mensajesplit = split(mensaje, ',');
    cout << "ALO2" << endl;
    if(mensajesplit.size() < 5){
        cerr << "mensaje incompleto" << endl;
    }
    for(int i=0;i < mensajesplit.size(); i++){
        cout << mensajesplit[i] << ",";
    }
    cout << "ALO3" << endl;
    int coreid = atoi(mensajesplit[0].c_str());
    cout << "ALO4" << endl;
    int id = atoi(mensajesplit[1].c_str());
    string operacion = mensajesplit[2];
    float n1 = atof(mensajesplit[3].c_str());
    float n2 = atof(mensajesplit[4].c_str());
    cout << "ALO5" << endl;
    

    string execcore = "./core -c " +mensajesplit[0]+" -i "+mensajesplit[1]+ " -o "+mensajesplit[2]+ " -n "+mensajesplit[3] +" -m "+mensajesplit[4];
    cout << "Ejecutando comando: " << execcore << endl;
    ofstream file("complementos/cores/core"+to_string(coreid)+".txt");
    if(!file.is_open()){
                cerr << "El archivo complementos/cores/core"+to_string(coreid)+".txt"+" no se pudo abrir" << endl;
                return EXIT_FAILURE;
            }
    file << "0";
    file.close();

    int status = system(execcore.c_str());
    if(status == -1){
        cerr << "Falló la ejecución del programa hijo" << endl;
    }

    string pathResultadoCore = "complementos/cores/results/resultado"+to_string(coreid)+"_"+to_string(id)+".txt";

    ifstream coreresultado(pathResultadoCore);
    while (!coreresultado.is_open()) {
        this_thread::sleep_for(chrono::milliseconds(50));
        coreresultado.open(pathResultadoCore);
    }

    coreresultado >> resultado;
    coreresultado.close();

    remove(pathResultadoCore.c_str());

    ofstream resultados("complementos/resultados.txt", ios::app);
    if(!resultados.is_open()){
        cerr << "El archivo de resultados no se pudo abrir" << endl;
        return EXIT_FAILURE;
    }
    resultados << "(" << coreid << ":" << id << ";" << operacion << ";" << n1 << "," << n2 << ") => " << resultado << endl;
    resultados.close();

    return 0;
}