#include "mbed.h"

int main() {
    setupSensor(); // Inicializa o sensor
    while (true) {
        int16_t gyroX = readGyroX(); // Variável com o valor da velocidade angular bruta
        printf("Giro X: %d\n", gyroX);
        ThisThread::sleep_for(100ms); // Aguarde 100ms
    }
}