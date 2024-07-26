#include "TimerFunctor.h"

TimerFunctor::TimerFunctor(float delay) : delay_(delay), elapsedTime_(0.0f) {
    // El reloj se inicializa en su declaración
}

bool TimerFunctor::operator()() {
    elapsedTime_ = clock_.getElapsedTime().asSeconds();
    if (elapsedTime_ >= delay_) {
        clock_.restart(); // Reinicia el reloj
        return true; // Tiempo de espera alcanzado
    }
    return false; // Tiempo de espera no alcanzado
}
