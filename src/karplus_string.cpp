#include "karplus_string.h"
#include <random>

// Конструктор "виртуальной струны"
// frequency  — частота колебаний (Гц)
// sampleRate — частота дискретизации (Гц)
KarplusString::KarplusString(double frequency, int sampleRate) {
    // Длина буфера — количество отсчётов на один период колебания
    N = static_cast<int>(sampleRate / frequency);
    if (N < 2) N = 2; // защита от слишком больших частот

    // Инициализируем буфер нулями
    buffer.resize(N, 0.0f);

    // Текущий индекс в буфере
    index = 0;
}

// "Дёрнуть струну" (инициализировать шумом)
// В буфер записываются случайные значения от -0.5 до +0.5,
// которые имитируют начальное колебание струны.
void KarplusString::pluck() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(-0.5f, 0.5f);

    for (int i = 0; i < N; i++) {
        buffer[i] = dist(rng);
    }
}

// Генерация следующего сэмпла (основной алгоритм Карплуса–Стронга)
// Берём среднее двух соседних значений, умножаем на коэффициент затухания,
// обновляем буфер и возвращаем новое значение.
float KarplusString::generate() {
    // Берём текущее значение и следующее
    float first = buffer[index];
    float next = buffer[(index + 1) % N];

    // Новое значение = среднее двух соседей * коэффициент затухания
    float newSample = 0.996f * 0.5f * (first + next);

    // Записываем обратно в буфер
    buffer[index] = newSample;

    // Сдвигаем индекс по кольцу
    index = (index + 1) % N;

    // Возвращаем сгенерированный сэмпл
    return newSample;
}
