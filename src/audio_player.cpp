#include "audio_player.h"
#include "audio_logger.h"
#include "audio_errors.h"
#include <portaudio.h>
#include <algorithm>

// Вспомогательная структура для передачи данных в callback
namespace {
    struct CallbackData {
        const float* buffer; // Указатель на массив аудиосэмплов (float от -1.0f до 1.0f)
        size_t length;       // Общее количество сэмплов в буфере
        size_t index;        // Текущий индекс воспроизводимого сэмпла
    };
}

// Callback-функция, которую PortAudio вызывает при необходимости отдать новые сэмплы на вывод
static int pa_callback(const void* /*input*/,
    void* output,                           // Куда писать данные (аудиовыход)
    unsigned long frameCount,               // Сколько сэмплов за раз нужно отдать
    const PaStreamCallbackTimeInfo* /*time*/, // Информация о времени (не используем)
    PaStreamCallbackFlags /*statusFlags*/,    // Флаги состояния (не используем)
    void* userData) {                       // Пользовательские данные (наш CallbackData)

    auto* data = reinterpret_cast<CallbackData*>(userData); // Достаём наши данные
    float* out = reinterpret_cast<float*>(output);          // Буфер вывода (float)

    // Копируем нужное количество сэмплов
    for (unsigned long i = 0; i < frameCount; i++) {
        if (data->index < data->length) {
            *out++ = data->buffer[data->index++]; // Берём сэмпл из буфера и пишем в выход
        }
        else {
            *out++ = 0.0f; // Если данные закончились → пишем тишину
        }
    }

    // Возвращаем статус: если есть ещё данные → продолжаем, иначе завершаем
    return data->index < data->length ? paContinue : paComplete;
}

// Основная функция воспроизведения звука
void AudioPlayer::play(const std::vector<float>& buffer, int sampleRate) {
    AudioLogger::info("Playback started"); // Логируем начало воспроизведения

    Pa_Initialize(); // Инициализация PortAudio

    // Заполняем структуру с нашими аудиоданными
    CallbackData data{ buffer.data(), buffer.size(), 0 };
    PaStream* stream = nullptr; // Поток воспроизведения (будет создан ниже)

    // Открываем дефолтный аудиопоток:
    // - 0 входных каналов (не пишем с микрофона)
    // - 1 выходной канал (моно)
    // - paFloat32 → формат сэмплов float 32-бита
    // - sampleRate → частота дискретизации (например, 44100 Гц)
    // - 256 → размер блока кадров
    // - pa_callback → наша функция для подачи данных
    // - &data → передаём в callback указатель на наши данные
    Pa_OpenDefaultStream(&stream,
        0, 1, paFloat32,
        sampleRate,
        256,
        pa_callback,
        &data);

    // Проверяем, удалось ли открыть поток
    if (!stream) {
        throw AudioError("Failed to open PortAudio stream");
    }

    // Запускаем поток воспроизведения
    Pa_StartStream(stream);

    // Ждём, пока поток проигрывает данные
    while (Pa_IsStreamActive(stream) == 1) {
        Pa_Sleep(50); // Небольшая задержка (50 мс), чтобы не грузить процессор
    }

    // Останавливаем и закрываем поток, завершаем PortAudio
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    AudioLogger::info("Playback finished"); // Логируем завершение
}
