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

//Hace un archivo map que tiene nombrelibro,id
void mapeoArchivos(){
    char* mapa_archivos_env = getenv("mapa_archivos");
    if(mapa_archivos_env == nullptr){
        cerr << "Error en la variable de entorno" << endl;
        return;
    }
    string mapa_archivos(mapa_archivos_env);
    string libros = "./libros";
    int id = 0;
    ofstream mapa(mapa_archivos);

    if (!mapa){
        cerr << "El archivo no se pudo abrir" << endl;
    }

    for (auto entrada : filesystem::directory_iterator(libros)){
        if(entrada.is_regular_file()){
            mapa << entrada.path().stem().string() << ", " << id << endl;
            id++;
        }
    }
    mapa.close();
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

int main(int argc, char* argv[]) {
    // Variables que se van a obtener de los argumentos de línea de comandos
    string stopWordsFile;
    int cantidad_threads = 0;
    string pathIn;
    string pathOut;
    string extension = ".txt";
    
    // Manejar los argumentos de línea de comandos usando getopt
    int opt;
    while ((opt = getopt(argc, argv, "s:t:i:o:")) != -1) {
        switch (opt) {
            case 's':
                stopWordsFile = optarg;  // Archivo de stop words
                break;
            case 't':
                cantidad_threads = atoi(optarg);  // Cantidad de hilos
                break;
            case 'i':
                pathIn = optarg;  // Directorio de entrada
                break;
            case 'o':
                pathOut = optarg;  // Directorio de salida
                break;
            default:
                cerr << "Uso: " << argv[0] << " -s <archivo_stopwords> -t <cantidad_hilos> -i <directorio_entrada> -o <directorio_salida>" << endl;
                return EXIT_FAILURE;
        }
    }

    if (stopWordsFile.empty() || cantidad_threads == 0 || pathIn.empty() || pathOut.empty()) {
        cerr << "Error: Faltan argumentos obligatorios." << endl;
        cerr << "Uso: " << argv[0] << " -s <archivo_stopwords> -t <cantidad_hilos> -i <directorio_entrada> -o <directorio_salida>" << endl;
        return EXIT_FAILURE;
    }

    pid_t pid = getpid();
    cout << "\nPrograma contador de palabras con hilos" << endl;
    cout << "PID: " << pid << endl;

    //Mapa archivos
    mapeoArchivos();
    
    // Cargar las stop words
    stopWords = cargarStopWords(stopWordsFile);
    if (stopWords.empty()) {
        cerr << "No se pudieron cargar las stop words. Saliendo..." << endl;
        return EXIT_FAILURE;
    }

    cout << "\033[32m" << "Stop words cargadas con éxito desde: " << stopWordsFile << "\033[0m" << endl;

    // Procesar archivos con hilos
    procesarConThreads(pathIn, pathOut, extension, cantidad_threads);

    return EXIT_SUCCESS;
}
