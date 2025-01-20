#ifndef FILTER_H
#define FILTER_H

#include "mbed.h"
#include <vector>

// Classe para aplicar filtro de média móvel
class Filter {
public:
    Filter();                       // Construtor
    float applyMovingAverage(int16_t newValue); // Aplica o filtro de média móvel

private:
    static constexpr size_t WINDOW_SIZE = 10; // Tamanho do buffer
    std::vector<int16_t> buffer;              // Buffer para armazenar leituras recentes
    size_t index;                             // Índice atual no buffer
    int sum;                                  // Soma das leituras no buffer
};

#endif
