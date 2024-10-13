#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_set>
#include <unistd.h>
#include "funciones.h"

using namespace std;

mutex mtx; // Mutex para la exclusión mutua
mutex cout_mutex; // Mutex para la salida en consola
condition_variable cv; // Variable de condición para la sincronización
queue<string> work_queue; // Cola de trabajo para las rutas de archivos
bool terminado = false; // Bandera para indicar que no hay más tareas
unordered_set<string> stopWords; // Contiene las stop words

// Convierte las palabras a minúsculas
string convertirMinusculas(string palabra) {
    string palabra_minus = palabra;
    transform(palabra_minus.begin(), palabra_minus.end(), palabra_minus.begin(), ::tolower);
    return palabra_minus;
}

// Elimina caracteres no alfabéticos de una palabra
string eliminarCaracteres(string palabra) {
    palabra.erase(remove_if(palabra.begin(), palabra.end(), [](char c) {
        return !isalpha(c);
    }), palabra.end());
    return palabra;
}

// Función para cargar las stop words desde un archivo
unordered_set<string> cargarStopWords(const string& stopWordsFile) {
    unordered_set<string> stopWords;
    ifstream file(stopWordsFile);
    string palabra;

    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de stop words." << endl;
        return stopWords;
    }

    while (file >> palabra) {
        stopWords.insert(palabra);
    }

    file.close();
    return stopWords;
}

// Función para eliminar las stop words de un archivo
string borrarStopWords(const string& inputFile) {
    ifstream inFile(inputFile);
    stringstream outContent;
    string line;

    if (!inFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de entrada." << endl;
        return "";
    }

    while (getline(inFile, line)) {
        stringstream ss(line);
        string palabra;

        // Filtrar palabras eliminando las stop words
        while (ss >> palabra) {
            palabra = eliminarCaracteres(palabra);
            palabra = convertirMinusculas(palabra);

            if (!palabra.empty() && stopWords.find(palabra) == stopWords.end()) {
                outContent << palabra << " ";
            }
        }
        outContent << endl;
    }

    inFile.close();
    cout << "La limpieza de stop words se ha llevado a cabo correctamente para el archivo: " << inputFile << endl << endl;
    return outContent.str();
}

// Función que cuenta las palabras
void contarPalabras(const string& ruta_archivo, const string& pathOut, int thread_id) {
    // Paso 1: Borrar las stop words del archivo
    string contenidoFiltrado = borrarStopWords(ruta_archivo);
    if (contenidoFiltrado.empty()) {
        cerr << "Error: No se pudo procesar el archivo " << ruta_archivo << endl;
        return;
    }

    // Paso 2: Contar palabras en el contenido filtrado
    map<string, int> contador_palabras;
    stringstream ss(contenidoFiltrado);
    string palabra;

    while (ss >> palabra) {
        if (!palabra.empty()) {
            contador_palabras[palabra]++;
        }
    }

    // Paso 3: Guardar el conteo en el archivo de salida
    {
        lock_guard<mutex> lock(mtx);
        ofstream archivo_salida(pathOut);
        if (!archivo_salida.is_open()) {
            cerr << "No se pudo crear el archivo de salida: " << pathOut << endl;
            return;
        }

        for (auto& [palabra, cantidad] : contador_palabras) {
            archivo_salida << palabra << " ; " << cantidad << endl;
        }
    }

    // Mostrar estadísticas
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Thread " << thread_id << ": Libro " << ruta_archivo << " procesado con éxito!" << endl;
        cout << "Archivo de salida: " << pathOut << endl;
        cout << "Cantidad de palabras distintas: " << contador_palabras.size() << endl << endl;
    }
}

// Función que lee los archivos de un directorio con una extensión específica
vector<string> lee_archivos(const string& ruta_input, const string& file_type) {
    vector<string> files;
    for (const auto& entry : filesystem::directory_iterator(ruta_input)) {
        if (entry.path().extension() == file_type) {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

// Función que maneja la creación de hilos y la asignación de tareas
int procesarConThreads(const string& ruta_input, const string& ruta_output, const string& file_type, int num_threads) {
    vector<string> files = lee_archivos(ruta_input, file_type);
    vector<thread> threads;

    // Agregar los archivos a la cola de trabajo
    for (const string& file : files) {
        work_queue.push(file);
    }

    // Crear hilos
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, i]() {
            while (true) {
                string file;
                {
                    lock_guard<mutex> lock(mtx);
                    if (!work_queue.empty()) {
                        file = work_queue.front();
                        work_queue.pop();
                        cout_mutex.lock();
                        cout << "Thread " << i << ": Desocupado y procesando el archivo " << file << endl;
                        cout_mutex.unlock();
                    } else {
                        cout_mutex.lock();
                        cout << "Thread " << i << ": Trabajo terminado." << endl;
                        cout_mutex.unlock();
                        break;  // No hay más archivos para procesar.
                    }
                }
                // Procesar el archivo
                contarPalabras(file, ruta_output + "/" + filesystem::path(file).filename().string(), i);
            }
        });
    }

    // Unirse a todos los hilos
    for (auto& thread : threads) {
        thread.join();
    }
    cout<<"\033[32m"<<"Conteo finalizado con éxito!! "<<"\033[0m"<<endl<<endl;
    return EXIT_SUCCESS;
}

int main() {
    envLoad();
    char* stopWordsFile = getenv("stop_word");
    char* cantidad_threads1 = getenv("cantidad_thread");
    int cantidad_threads = atoi(cantidad_threads1);

    int opcion3;
    string pathIn;
    string pathOut;
    string extension;
    bool check1 = false, check2 = false, check3 = false;

    pid_t pid = getpid();
    cout << "\nPrograma contador de palabras con hilos" << endl;
    cout << "PID: " << pid << endl;
    
    // Cargar las stop words
    stopWords = cargarStopWords(stopWordsFile);
    if (stopWords.empty()) {
        cerr << "No se pudieron cargar las stop words. Saliendo..." << endl;
        return EXIT_FAILURE;
    }

    do { 
        cout << "#########################################"<<endl<<endl;
        cout << "Seleccione la opción: " << endl;
        cout << "\n0) Salir" << endl;
        cout << "1) Extensión de archivos a procesar (ej: txt)" << endl;
        cout << "2) Path de carpeta a procesar (ej: /home/usuario/in)" << endl;
        cout << "3) Path de carpeta de salida (ej: /home/usuario/out)" << endl;
        cout << "4) Procesar con " << cantidad_threads << " threads" <<endl<<endl;
        cout << "#########################################"<<endl<<endl;

        cout << "Escriba aquí: ";

        cin >> opcion3;

        switch (opcion3) {
            case 0: {
                system("clear");
                cout << "Saliendo..." << endl;
                break;
            }

            case 1: {
                system("clear");
                cout << "Opción 1: Extensión de archivos a procesar: ";
                cin >> extension;
                extension = '.' + extension;
                cout << "Extensión seleccionada: " << extension << endl<<endl;
                check1 = true;
                break;
            }
            case 2: {
                cout << "Opción 2: Path de carpeta a procesar: ";
                cin >> pathIn;
                while (!filesystem::exists(pathIn) || !filesystem::is_directory(pathIn)) {
                    cerr << "\033[31m" << "La carpeta de entrada no existe o no es un directorio: " << "\033[0m" << pathIn << endl;
                    cout << "Por favor ingrese una carpeta que exista: ";
                    cin >> pathIn;
                }
                check2 = true;
                break;
            }
            case 3: {
                cout << "Opción 3: Path de carpeta de salida: ";
                cin >> pathOut;
                while (!filesystem::exists(pathOut) || !filesystem::is_directory(pathOut)) {
                    cerr << "\033[31m" << "La carpeta de salida no existe o no es un directorio: " << "\033[0m" << pathOut << endl;
                    cout << "Por favor ingrese una carpeta que exista: ";
                    cin >> pathOut;
                }
                check3 = true;
                break;
            }
            case 4: {
                system("clear");
                cout << "Opción 4: Procesar" << endl<<endl;
                if (!check1 || !check2 || !check3) {
                    cerr << "\033[31m" << "Error, debe completar las opciones 1), 2), y 3) antes de procesar.\n" << "\033[0m";
                    break;
                } else {
                    cout <<"\033[32m"<< "Iniciando procesamiento..." <<"\033[0m"<< endl;
                    procesarConThreads(pathIn, pathOut, extension, cantidad_threads);
                }
                break;
            }
            default: {
                cerr << "Opción inválida." << endl;
                break;
            }
        }
    } while (opcion3 != 0);

    return EXIT_SUCCESS;
}
