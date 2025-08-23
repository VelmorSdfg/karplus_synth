#include "audio_writer.h"
#include <fstream>
#include <stdexcept>
#include <cstdint>

// Чтобы структура точно соответствовала формату WAV (без паддинга)
#pragma pack(push, 1)

// Структура заголовка WAV файла (стандартный формат PCM 16-bit mono)
struct WAVHeader {
    char riff[4] = { 'R','I','F','F' };  // Сигнатура "RIFF" (формат контейнера)
    uint32_t fileSize;                   // Общий размер файла - 8 байт
    char wave[4] = { 'W','A','V','E' };  // Сигнатура "WAVE" (формат аудио)

    // Подсекция fmt
    char fmtChunkId[4] = { 'f','m','t',' ' }; // Подсекция "fmt "
    uint32_t fmtChunkSize = 16;               // Размер секции fmt (16 для PCM)
    uint16_t audioFormat = 1;                 // Формат аудио (1 = PCM)
    uint16_t numChannels = 1;                 // Кол-во каналов (1 = моно, 2 = стерео)
    uint32_t sampleRate;                      // Частота дискретизации (Гц)
    uint32_t byteRate;                        // Количество байт в секунду (sampleRate * blockAlign)
    uint16_t blockAlign;                      // Кол-во байт на один "сэмпл всех каналов"
    uint16_t bitsPerSample = 16;              // Бит на сэмпл (16 бит = int16_t)

    // Подсекция data
    char dataChunkId[4] = { 'd','a','t','a' }; // Секция "data"
    uint32_t dataChunkSize;                    // Размер блока данных (в байтах)
};

#pragma pack(pop)

// Запись в WAV-файл массива сэмплов (16-бит PCM)
void AudioWriter::writeToFile(const std::string& filename,
    const std::vector<int16_t>& samples,
    int sampleRate)
{
    // Заполняем заголовок WAV
    WAVHeader header;
    header.sampleRate = sampleRate;
    header.numChannels = 1;
    header.bitsPerSample = 16;
    header.blockAlign = header.numChannels * header.bitsPerSample / 8;
    header.byteRate = sampleRate * header.blockAlign;
    header.dataChunkSize = static_cast<uint32_t>(samples.size() * sizeof(int16_t));
    header.fileSize = 36 + header.dataChunkSize;

    // Открываем файл на запись в бинарном режиме
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }

    // Сначала пишем заголовок
    file.write(reinterpret_cast<const char*>(&header), sizeof(WAVHeader));

    // Затем — сами аудиоданные (сэмплы)
    file.write(reinterpret_cast<const char*>(samples.data()), header.dataChunkSize);
}
