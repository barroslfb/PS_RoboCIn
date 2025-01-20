#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Sensor.h"
#include "Filter.h"

#define FRAME_STOP 3500 // Frame onde o robô está parado
#define PI 3.14159265359

using namespace std;

// Função para converter graus/s para rad/s
float degreesToRadians(float degrees) {
    return degrees * (PI / 180.0);
}

// Função principal para testar o log
int main() {
    // Instâncias das classes
    Sensor gyro;
    Filter filter;

    // Abrir o arquivo do log
    ifstream file("/mnt/data/filtered_robot_log.csv");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo de log!" << endl;
        return -1;
    }

    // Variáveis para processar os dados
    vector<float> angularSpeeds; // Velocidades angulares (graus/s)
    vector<float> angles;        // Ângulos acumulados (rad)
    string line;

    // Ler o arquivo e carregar os dados
    while (getline(file, line)) {
        angularSpeeds.push_back(stof(line)); // Converte cada linha para float
    }
    file.close();

    // Calibração: calcular o bias com os primeiros frames
    float bias = 0.0;
    for (int i = 0; i < FRAME_STOP; i++) {
        bias += angularSpeeds[i];
    }
    bias /= FRAME_STOP; // Média do bias
    cout << "Bias calculado: " << bias << " graus/s" << endl;

    // Processar os dados
    float angle = 0.0; // Ângulo inicial
    float deltaTime = 0.01; // Intervalo entre frames (ex.: 100 Hz -> 0.01s)

    for (size_t i = 0; i < angularSpeeds.size(); i++) {
        // Corrigir o bias e converter para rad/s
        float correctedSpeed = degreesToRadians(angularSpeeds[i] - bias);

        // Aplicar filtragem
        float filteredSpeed = filter.applyMovingAverage(correctedSpeed);

        // Calcular o ângulo acumulado
        angle += filteredSpeed * deltaTime;

        // Ajustar o ângulo para o intervalo [-π, π]
        if (angle > PI) angle -= 2 * PI;
        if (angle < -PI) angle += 2 * PI;

        // Armazenar o ângulo
        angles.push_back(angle);

        // Exibir alguns resultados (opcional)
        if (i % 500 == 0) { // Exibe a cada 500 frames
            cout << "Frame " << i << ": Velocidade Filtrada = " << filteredSpeed
                 << " rad/s, Ângulo = " << angle << " rad" << endl;
        }
    }

    // Salvar os resultados processados em um novo arquivo
    ofstream output("processed_log.csv");
    if (!output.is_open()) {
        cerr << "Erro ao salvar o arquivo processado!" << endl;
        return -1;
    }
    for (size_t i = 0; i < angles.size(); i++) {
        output << angles[i] << endl;
    }
    output.close();

    cout << "Processamento concluído! Resultados salvos em 'processed_log.csv'." << endl;
    return 0;
}
