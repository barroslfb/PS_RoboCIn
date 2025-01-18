#include "mbed.h"

#define WINDOW_SIZE 10 // Número de leituras consideradas
#define CALIBRATION_SAMPLES 100
#define MPU6050_ADDR 0x68 << 1 // Esse é o endereço I2C do sensor MPU6050
#define PWR_MGMT_1 0x6B        // Registrador de gerenciamento de energia
#define GYRO_XOUT_H 0x43 // Endereço do registrador do eixo X

I2C i2c(PB_7, PB_6); // Pinos para SDA e SCL para o microcontrolador STM32
                     // PB_7 => Serial Data Line para troca de dados
                     // PB_6 => Serial Clock Line para sincronização do barramento

int16_t buffer[WINDOW_SIZE] = {0}; // Buffer circular para armazenar leituras
int index = 0; // Índice atual no buffer
int sum = 0; // Soma das leituras no buffer

int16_t calibrateGyroX();
float movingAverage(int16_t newValue);
int16_t readCorrectedGyroX();
int16_t readGyroX();
void setupSensor();

int16_t bias = 0; // Armazena o bias calculado

int main() {
    setupSensor(); // Inicializa o sensor
    bias = calibrateGyroX(); // Calibra o sensor

    while (true) {
        int16_t correctedValue = readCorrectedGyroX(); // Leitura corrigida
        float filteredValue = movingAverage(correctedValue); // Filtragem
        printf("Valor filtrado: %f\n", filteredValue);
        ThisThread::sleep_for(100ms);
    }
}

// Função que filtra os valores:
float movingAverage(int newValue) {
    sum -= buffer[index]; // Essa ação remove o valor mais antigo do buffer
    buffer[index] = newValue; // Adiciona o novo valor ao buffer
    sum += newValue; // Atualiza a soma com o novo valor
    index = (index + 1) % WINDOW_SIZE; // Avançã no buffer circular
    
    return sum / (float)WINDOW_SIZE; // Retorna a média dos valores
}

// Função que identifica o bias
int16_t calibrateGyroX() {
    int32_t sum = 0;
    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        sum += readGyroX(); // Essa função lê o valor bruto do giroscópio no eixo X
        ThisThread::sleep_for(10ms); // Aguarda 10ms entre as leituras
    }
    return sum / CALIBRATION_SAMPLES; // Retorna o bias (desvio constante)
}

// Função que corrige os desvios
int16_t readCorrectedGyroX() {
    return readGyroX() - bias;
}

// Função para realizar a leitura dos dados do eixo X:
int16_t readGyroX() {
    char reg = GYRO_XOUT_H; // Armazena o endereço do registrador
    char data[2];           // Buffer para armazenar os 2 bytes de MSB e LSB

    i2c.write(MPU6050_ADDR, &reg, 1, true); // Envia o endereço do registrador
    i2c.read(MPU6050_ADDR, data, 2);        // Lê os 2 bytes do registrador

    return (data[0] << 8) | data[1]; // Combinar MSB E LSB, que resulta na velocidade angular bruta do eixo X
}

// Função para inicializar o sensor MPU6050:
void setupSensor() {
    char data[2];
    data[0] = PWR_MGMT_1; // Registrador de energia
    data[1] = 0x00;      // Sair do modo de suspensão
    i2c.write(MPU6050_ADDR, data, 2); // Enviar comando
}