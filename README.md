# [Trabajo 1
## Descripcion
Este programa es una ipequeña nterfaz que realiza diversas funciones desde los parámetros ingresados en consola.

## Requisitos
- Un ordenador con Linux, macOS o Windows
- El compilador g++ instalado
- Lirerias standard C++


## Instalacion / Compilacion
No se debe instalar nada. Para compilar, sólo se debe escribir "make" en el terminal en la carpeta donde están los archivos.

## Ejecucion
Después de compilar, se debe ejecutar como ./trabajo1 -u usuario -p contraseña -t texto entre comillas -v vector de la forma 1,2,3,4 entre comillas -n un número entero.

| Argumento | Descripcion |
| -------- | ----------- |
| u | Especifica el usuario |
| p | Especifica la contraseña del usuario |
| t | Un texto |
| v | Un vector de números enteros |
| n | Un número entero |

## Funciones
string palindromo(string a); 
Verifica si un texto es palíndromo 

int vocales(string a);
Verifica el número de vocales de un texto

int letras(string a);
Verifica el número de letras de un texto 

int sum(vector<int> vec);
Retorna la suma de los números de un vector

float prom(vector<int> vec);
Retorna el promedio de un vector

vector<int> procesaVector(string a);
Procesa el vector ingresado, convirtiéndolo en un vector 

int subMenu();
Despliega un subMenu para volver atrás

tuple<string, string, char> leerEnv();
Lee el .env

int verificacion(string username, string password, string frase, vector<int> vec, int num);
Verifica cada error que puedan tener los parámetros ingresados