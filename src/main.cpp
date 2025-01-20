#include "mbed.h"
#include "Sensor.h"
#include "Filter.h"

Sensor gyro; // Gerencia o sensor
Filter filter; // Aplica o filtro de média móvel

int main() {
    // Inicializar o sensor
    printf("Inicializando o sensor...\n");
    gyro.setup(); // Configura o sensor para operação

    // Calibrar o giroscópio
    printf("Calibrando o sensor...\n");
    int16_t bias = gyro.calibrate(); // Determina o bias
    printf("Calibração concluída. Bias: %d\n", bias);

    // Loop principal
    while (true) {
        // Lê o valor corrigido do giroscópio
        int16_t correctedValue = gyro.readCorrectedGyroX(bias);

        // Aplica o filtro de média móvel
        float filteredValue = filter.applyMovingAverage(correctedValue);

        // Exibe o valor filtrado no terminal
        printf("Velocidade Angular Filtrada: %.2f rad/s\n", filteredValue);

        // Aguardar 100ms para garantir uma frequência fixa de leitura (10 Hz)
        ThisThread::sleep_for(100ms);
    }
}