#include "karplus_synthsizer.h"
#include <vector>
#include <cmath>
#include <random>
#include <string>

// Основная функция синтеза звука по алгоритму Карплуса–Стронга
// frequencies — список частот (Гц)
// sampleRate  — частота дискретизации (Гц)
// duration    — длительность сигнала (секунды)
// instrument  — название инструмента (пока не используется, но можно расширить)
std::vector<float> KarplusSynthesizer::synthesize(
    const std::vector<double>& frequencies,
    int sampleRate,
    double duration,
    const std::string& instrument)
{
    // Общее количество сэмплов на заданную длительность
    size_t totalSamples = static_cast<size_t>(duration * sampleRate);

    // Буфер для финального результата (обнулён)
    std::vector<float> result(totalSamples, 0.0f);

    // Генератор случайных чисел для инициализации "струны"
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    // Для каждой частоты генерируем сигнал отдельно
    for (double freq : frequencies) {
        if (freq <= 0.0) continue; // защита от некорректных значений

        // Длина периода в сэмплах
        size_t period = static_cast<size_t>(sampleRate / freq);
        if (period < 2) continue; // слишком высокая частота → пропускаем

        // Буфер для струны (один период), инициализируем шумом
        std::vector<float> buffer(period);
        for (auto& s : buffer) {
            s = dist(rng);
        }

        // Буфер для текущей ноты
        std::vector<float> note(totalSamples, 0.0f);

        // Генерация сэмплов по алгоритму Карплуса–Стронга
        for (size_t i = 0; i < totalSamples; i++) {
            // Текущее значение — берём из буфера
            float value = buffer[i % period];
            note[i] = value;

            // Обновляем буфер:
            // новое значение = среднее текущего и следующего
            buffer[i % period] = 0.5f * (buffer[i % period] + buffer[(i + 1) % period]);
        }

        // Смешиваем текущую ноту в общий результат
        // Делим на количество частот, чтобы избежать клиппинга (перегруза)
        for (size_t i = 0; i < totalSamples; i++) {
            result[i] += note[i] / static_cast<float>(frequencies.size());
        }
    }

    // Возвращаем итоговый сигнал
    return result;
}
