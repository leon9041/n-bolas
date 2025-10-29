/**
 * @file     main.cpp
 * @brief    implementacion de poo en gas ideal
 * @author   Angie Gomez, Leonardo Tovar
 * @date     29/10/25
 * @version  1.0
 * @license  owner
 */


#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <cstdlib>
#include <cmath>
#include "Caja.h"

#include <windows.h> //tíldes


int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::filesystem::create_directories("results");

    int N;
    std::cout << "Ingrese el número de partículas N: ";
    std::cin >> N;

    // Parámetros de simulación:
    double W = 1.0, H = 1.0, radio = 0.001 , vmax = 0.4, dt = 0.002, T = 100.0;
    unsigned int seed = 45;

    Caja caja(W, H);
    caja.inicializarAleatorio(N, radio, vmax, seed);

    std::ofstream out("results/salida.dat");
    out << std::fixed << std::setprecision(6);

    // Archivo para presión por bloques:
    std::ofstream presFile("results/presion.dat");
    presFile << "# tiempo  P_exp  P_teo\n";

    double t = 0.0;
    int steps = static_cast<int>(T / dt);

    // Acumuladores globales
    double suma_Pexp = 0.0;
    double suma_Pteo = 0.0;
    int contador_presiones = 0;
    long total_rebotes_global = 0;

    //Energía inicial
    double energia_inicial = caja.energiaTotal();

    // Bucle principal de simulación
    for (int s = 0; s < steps; ++s) {
        caja.guardarEstado(out, t);
        caja.pasoTemporal(dt);
        t += dt;

        // pasos para medir presión
        if (s % 1000 == 0) {
            double energia = caja.energiaTotal();

            // presión teórica ∝ nm⟨v²⟩ / L²
            double v2prom = (2 * energia) / (N * 1.0);  // masa=1
            double P_teo = (N * v2prom) / (W * H);

            // presión experimental ∝ impulso acumulado / (tiempo × área)
            double P_exp = caja.impulso_total / (1000 * dt * 2 * (W + H));

            presFile << t << " " << P_exp << " " << P_teo << "\n";
            std::cout << "Paso " << s
                      << "  P_exp = " << P_exp
                      << "  P_teo = " << P_teo << "\n";

            // acumular promedios globales
            suma_Pexp += P_exp;
            suma_Pteo += P_teo;
            contador_presiones++;

            //  rebotes totales
            total_rebotes_global += caja.contador_rebotes;

            // reiniciar acumuladores de bloque
            caja.contador_rebotes = 0;
            caja.impulso_total = 0.0;
        }

        // mensaje de avance
        if (s % 1000 == 0)
            std::cout << "Paso " << s << " / " << steps
                      << "  Energía total = " << caja.energiaTotal() << "\n";
    }

    out.close();
    presFile.close();

    // Energía final
    double energia_final = caja.energiaTotal();

    // Promedios finales de presión
    double P_exp_prom = suma_Pexp / contador_presiones;
    double P_teo_prom = suma_Pteo / contador_presiones;
    double error_rel = 100.0 * fabs(P_exp_prom - P_teo_prom) / P_teo_prom;

    // Resultados finales
    std::cout << "\nEnergía inicial = " << energia_inicial
              << "\nEnergía final   = " << energia_final << "\n";
    std::cout << "Número total de rebotes = " << total_rebotes_global << "\n";

    std::cout << "\nPresión experimental promedio = " << P_exp_prom
              << "\nPresión teórica promedio      = " << P_teo_prom
              << "\nError relativo (%)            = " << error_rel << "\n";

    std::cout << "\nSimulación completada. Archivos generados en carpeta 'results'\n";
    std::cout << "Generando gráficas y animación...\n";

    int ret = std::system("python scripts\\graficas_animacion.py");
    if (ret != 0) {
        std::cout << "No se pudo ejecutar el script de Python automáticamente.\n"
                     "Puedes hacerlo manualmente con: python scripts/graficas_animacion.py\n";
    }

    return 0;
}
