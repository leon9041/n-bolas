
/**
 * @file     caja.cpp
 * @brief    implementacion de poo en gas ideal
 * @author   Angie Gomez, Leonardo Tovar
 * @date     29/10/25
 * @version  1.0
 * @license  owner
 */



#include "Caja.h"
#include <random>
#include <iostream>
#include <cmath>

Caja::Caja(double W_, double H_) : W(W_), H(H_) {}

// ---------------- Inicializar partículas aleatoriamente ----------------
void Caja::inicializarAleatorio(int N, double radio, double vmax, unsigned int seed) {
    bolas.clear();
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> ux(radio, W - radio);
    std::uniform_real_distribution<double> uy(radio, H - radio);
    std::uniform_real_distribution<double> uv(-vmax, vmax);

    int id = 0;
    while (id < N) {
        double x = ux(rng);
        double y = uy(rng);
        Bola nueva(id, x, y, uv(rng), uv(rng), radio, 1.0);

        bool ok = true;
        for (auto& b : bolas) {
            if (nueva.estaSolapadaCon(b)) { ok = false; break; }
        }

        if (ok) {
            bolas.push_back(nueva);
            id++;
        }
    }
}

// ---------------- Un paso de simulación ----------------
void Caja::pasoTemporal(double dt) {
    Vec2 acc(0.0, 0.0);

    //Paso medio de velocidad y actualización de posición
    for (auto& b : bolas) b.vel_half_step(acc, dt);
    for (auto& b : bolas) b.pos_full_step(dt);

    //Contado de rebotes con paredes y acumulación de impulso
    for (auto& b : bolas) {
        double imp = 0.0;
        if (b.reboteParedConImpulso(W, H, imp)) {
            contador_rebotes++;
            impulso_total += imp;
        }
    }

    // Resolver colisiones entre bolas
    resolverChoquesPares();

    // Completar paso de velocidad (Velocity-Verlet)
    for (auto& b : bolas) b.vel_full_step(acc, dt);
}

// ---------------- Revisar colisiones entre todas las bolas ----------------
void Caja::resolverChoquesPares() {
    size_t N = bolas.size();
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            if (bolas[i].estaSolapadaCon(bolas[j])) {
                bolas[i].resolverColision(bolas[j]);
            }
        }
    }
}

// ---------------- Guardar datos ----------------
void Caja::guardarEstado(std::ofstream& out, double t) const {
    out << t;
    for (auto& b : bolas) {
        out << " " << b.pos.x << " " << b.pos.y << " " << b.vel.x << " " << b.vel.y;
    }
    out << "\n";
}

// ---------------- Calcular energía total ----------------
double Caja::energiaTotal() const {
    double E = 0;
    for (auto& b : bolas) E += b.energiaCin();
    return E;
}
