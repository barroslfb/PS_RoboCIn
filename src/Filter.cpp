#include "filter.h"
#include "mbed.h"

Filter::Filter() : buffer(WINDOW_SIZE, 0), index(0), sum(0) {}

// Função que filtra os valores:
float Filter::applyMovingAverage(int16_t newValue) {
    sum -= buffer[index]; // Essa ação remove o valor mais antigo do buffer
    buffer[index] = newValue; // Adiciona o novo valor ao buffer
    sum += newValue; // Atualiza a soma com o novo valor
    index = (index + 1) % WINDOW_SIZE; // Avançã no buffer circular
    
    return sum / (float)WINDOW_SIZE; // Retorna a média dos valores
}