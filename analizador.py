import matplotlib.pyplot as plt
import os

def leer_datos(archivo):
    datos = {}
    with open(archivo, 'r') as file:
        for linea in file:
            thread, tiempo = map(float, linea.split())
            if thread not in datos:
                datos[thread] = []
            datos[thread].append(tiempo)
    return datos

# Función para generar el gráfico
def graficar(datos, repeticiones, grafico_path):
    threads = sorted(datos.keys())  # Ordenar los threads por número
    for i in range(repeticiones):
        tiempos = [datos[thread][i] for thread in threads]
        plt.plot(threads, tiempos, label=f"Repetición {i+1}")

    # Configurar el gráfico con título, ejes y leyenda
    plt.title('Tiempo de ejecución vs. Número de Threads')
    plt.xlabel('Número de Threads')
    plt.ylabel('Tiempo de Ejecución (segundos)')
    plt.legend()
    
    # Guardar el gráfico en la ruta especificada
    plt.savefig(grafico_path)

if __name__ == "__main__":
    #Leer las variables de entorno
    repeticiones = int(os.environ.get('REPETICIONES'))
    grafico_path = os.environ.get('GRAFICO')

    # Leer los datos desde datos.txt
    datos = leer_datos('datos.txt')

    # Generar el gráfico
    graficar(datos, repeticiones, grafico_path)
