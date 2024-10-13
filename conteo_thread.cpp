#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

//Función para leer las "stop words" desde un archivo y guardarlas en un diccionario
unordered_set<string> cargarStopWords(string stopWordsFile) {
    unordered_set<string> stopWords; //creamos el diccionario
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

// Función para procesar el archivo de texto, eliminando las "stop words"
void borrarStopWords(string inputFile, string outputFile, unordered_set<string> stopWords) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    string line;

    if (!inFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de entrada." << endl;
        return;
    }

    if (!outFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de salida." << endl;
        return;
    }

    while (getline(inFile, line)) {
        stringstream ss(line);
        string palabra;

        // Filtramos las palabras, eliminando las stop words
        while (ss >> palabra) {
            if (stopWords.find(palabra) == stopWords.end()) { // Si no es una stop word
                outFile << palabra << " "; // Escribimos la palabra en el archivo de salida
            }
        }
        outFile << endl; // Nueva línea en el archivo de salida
    }

    inFile.close();
    outFile.close();
}

int main() {
    // Define tus archivos de entrada y salida
    string stopWordsFile = "stop_word.txt"; // Archivo que contiene las stop words
    string inputFile = "librostxt/harrypotter.txt";          // Archivo de entrada
    string outputFile = "librostxt/output.txt";        // Archivo de salida

    // Cargar las stop words
    unordered_set<string> stopWords = cargarStopWords(stopWordsFile);

    // Procesar el archivo y eliminar las stop words
    borrarStopWords(inputFile, outputFile, stopWords);

    cout << "Proceso completado. Archivo procesado guardado como: " << outputFile << endl;

    return 0;
}