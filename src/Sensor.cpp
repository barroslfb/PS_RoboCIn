#include "sensor.h"
#include "mbed.h"

Sensor::Sensor() : i2c(PB_7, PB_6) {} // Inicializa o I2C nos pinos SDA e SCL

// Função para sair do modo de suspensão:
void Sensor::setup() {
    char data[2];

    // Configuração para sair do modo de suspensão
    data[0] = PWR_MGMT_1;
    data[1] = 0x00;
    i2c.write(MPU6050_ADDR, data, 2);
}

// Função para ler o giroscópio:
int16_t Sensor::readGyroX() {
    char reg = GYRO_XOUT_H; // Registrador do eixo X
    char data[2]; // Buffer para armazenar os 2 bytes (MSB E LSB)

    i2c.write(MPU6050_ADDR, &reg, 1, true); // Seleciona o registrador
    i2c.read(MPU6050_ADDR, data, 2); // Lê os 2 bytes

    return (data[0] << 8) | data[1]; // Combina MSB e LSB
}

// Função para calcular o bias:
int16_t Sensor::calibrate() {
    int32_t sum = 0;

    // Coleta 100 amostras para calcular o bias
    for (int i = 0; i < 100; i++) {
        sum += readGyroX();
        ThisThread::sleep_for(10ms); // Intervalo entre as leituras
    }

    return sum / 100; // Calcula a média (bias)
}

// Função que corrige o bias:
int16_t Sensor::readCorrectedGyroX(int16_t bias) {
    return readGyroX() - bias; // Retorna o valor corrigdo
}
