#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <unistd.h>


using namespace std;

//Convierte las palabras en solo minusculas
string convertirMinusculas(string palabra) {
    string palabra_minus = palabra;
    transform(palabra_minus.begin(), palabra_minus.end(), palabra_minus.begin(), ::tolower);
    return palabra_minus;
}

//Elimina puntuacion de una palabra
string eliminarPuntuacion(string palabra) {
    palabra.erase(remove_if(palabra.begin(), palabra.end(), [](char c) {
        return ispunct(c);
    }), palabra.end());
    return palabra;
}

//Funcion que procesa los archivos y cuenta las palabras
void contarPalabras(string ruta_archivo, string pathOut) {
    ifstream archivo(ruta_archivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de entrada: " << ruta_archivo << endl;
        return;
    }


    //Map que almacena la cantidad de cada palabra
    map<string, int> contador_palabras;
    string linea, palabra;

    //Lectura de archivo
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        //Extrae cada palabra de la linea
        while (ss >> palabra) {
            palabra = eliminarPuntuacion(palabra);
            palabra = convertirMinusculas(palabra);
            if (!palabra.empty()) {
                contador_palabras[palabra]++;
            }
        }
    }
    archivo.close();

    //Creacion de archivo de salida con mismo nombre que el de entrada
    ofstream archivo_salida(pathOut);
    if (!archivo_salida.is_open()) {
        cerr << "No se pudo crear el archivo de salida: " << pathOut << endl;
        return;
    }

    //Escribe resultado en output
    for ( auto [palabra, cantidad] : contador_palabras) {
        archivo_salida << palabra << "; " << cantidad << endl;
    }
    //Imprime path del archivo y la cantidad de palabras distintas
    cout << "Archivo de salida: " << pathOut << endl;
    cout << "Cantidad de palabras distintas: " << contador_palabras.size() << endl;
    cout << endl;

    archivo_salida.close();
}

int procesar(string ext, string pathIn, string pathOut) {
    //Aplica funcion contar palabras en todos los archivos de la carpeta de entrada
    bool archivosProcesados = false;
    for (auto entry : filesystem::directory_iterator(pathIn)) {
        if (entry.path().extension() == ext) {
            archivosProcesados = true;
            string nombre_archivo = entry.path().filename().string();
            string pathArchivoSalida = pathOut + "/" + nombre_archivo;
            contarPalabras(entry.path().string(), pathArchivoSalida);
        }
    }

    if (!archivosProcesados) {
        cout << "No se encontraron archivos con la extensión " << ext << " en " << pathIn << endl;
    }

    return 0;
}

int main(){
    int opcion3;
    string pathIn;
    string pathOut;
    string extension;
    bool check1 = false, check2 = false, check3 = false;

    pid_t pid = getpid();
    cout << "\nPrograma contador de palabras" << endl;
    cout << "PID: " << pid << endl;
    
    do{

        cout << "seleccione la opción: " << endl;
        cout << "\n0) Salir" << endl;
        cout << "1) Extensión de archivos a procesar(ej:txt)" << endl;
        cout << "2) Path de carpeta a procesar(eje: /home/usuario/in)" << endl;
        cout << "3) Path de carpeta que contendrá la respuesta del proceso(eje:/home/usuario/out)" << endl;
        cout << "4) Procesar" << endl;
        cout << "Escriba aquí: ";

        cin >> opcion3;

        switch (opcion3) {
                case 0: {
                    cout << "Saliendo..." << endl;
                    break;
                }

                case 1: {
                    cout << "Opción 1: Extensión de archivos a procesar" << endl;
                    cin >> extension;
                    extension = '.' + extension;
                    cout << "Extension: " << extension << endl;
                    check1 = true;
                    break;
                }
                case 2: {
                    cout << "Opción 2: Path de carpeta a procesar" << endl;
                    cin >> pathIn;
                    while (!filesystem::exists(pathIn) || !filesystem::is_directory(pathIn)) {
                        cerr << "La carpeta de entrada no existe o no es un directorio: " << pathIn << endl;
                        cout << "Por favor ingrese una carpeta que existe: " << endl;
                        cin >> pathIn;
                    }
                    check2 = true;
                    break;
                }
                case 3: {
                    cout << "Opción 3: Path de carpeta que contendrá la respuesta del proceso" << endl;
                    cin >> pathOut;
                    while (!filesystem::exists(pathOut) || !filesystem::is_directory(pathOut)) {
                        cerr << "La carpeta de salida no existe o no es un directorio: " << pathOut << endl;
                        cout << "Por favor ingrese una carpeta que existe: " << endl;
                        cin >> pathOut;
                    }
                    check3 = true;
                    break;
                }
                case 4: {
                    cout << "Opción 4: Procesar" << endl;
                    if (check1 == true && check2 == true && check3 == true){
                        cout << "Procesando..." << endl;
                        procesar(extension, pathIn, pathOut);
                        return 0;
                    }
                    else{
                        cout << "Tienes que completar las opciones 1 2 y 3." << endl;
                    }
                    break;

                default:
                    cout<<endl << "\033[31m" << "Opción inválida. Intente de nuevo.!!!!"<<"\033[0m" << endl;
                
            }
        }
    }
    while(opcion3 != 0);
    return 0;
}

