#ifndef CAJA_H
#define CAJA_H

#include <vector>
#include <fstream>
#include "Bola.h"

class Caja {
public:
    double W, H;
    std::vector<Bola> bolas;

    int contador_rebotes = 0;
    double impulso_total = 0.0;

    Caja(double W_, double H_);

    void inicializarAleatorio(int N, double radio, double vmax, unsigned int seed);
    void pasoTemporal(double dt);
    void resolverChoquesPares();
    void guardarEstado(std::ofstream& out, double t) const;
    double energiaTotal() const;
};

#endif
