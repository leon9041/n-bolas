Simulación de Gas Ideal en 2D

Este programa realiza una simulación del movimiento de partículas dentro de una caja cuadrada, representando el comportamiento de un gas ideal.
A partir de los choques de las partículas entre sí y con las paredes, el programa calcula la presión ejercida y la compara con la presión teórica esperada.
Características: 
Integración temporal mediante el método Velocity Verlet.
Colisiones elásticas entre partículas y con las paredes.
Cálculo de la presión experimental y teórica.
Conservación de la energía cinética total.
Generación de gráficas y animaciones con Python (matplotlib).

Estructura: s
include/
    Bola.h
    Caja.h
src/
    main.cpp
    Bola.cpp
    Caja.cpp
scripts/
    graficas_animacion.py
results/
    salida.dat
    presion.dat
README.txt

Ejecución: 

Compilar el programa en C++:
g++ src/*.cpp -Iinclude -o simulacion

Ejecutar la simulación:./simulacion

   El programa pedirá el número de partículas y generará los resultados en la carpeta "results/".

Generar gráficas y animaciones:
python scripts/graficas_animacion.py
