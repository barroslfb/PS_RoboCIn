#ifndef SENSOR_H
#define SENSOR_H

#include "mbed.h"

// Classe para gerenciar o sensor MPU-6050
class Sensor {
public:
    Sensor();                // Construtor
    void setup();            // Inicializa o sensor
    int16_t calibrate();     // Realiza a calibração para determinar o bias
    int16_t readGyroX();     // Lê o valor bruto do giroscópio no eixo X
    int16_t readCorrectedGyroX(int16_t bias); // Retorna o valor corrigido com o bias

private:
    static constexpr uint8_t MPU6050_ADDR = 0x68 << 1; // Endereço I2C do sensor
    static constexpr uint8_t PWR_MGMT_1 = 0x6B;        // Registrador de energia
    static constexpr uint8_t GYRO_XOUT_H = 0x43;       // Registrador do giroscópio

    I2C i2c; // Objeto para comunicação I2C
};

#endif
