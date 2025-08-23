#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "karplus_synthsizer.h"
#include "audio_player.h"
#include "audio_writer.h"
#include "audio_logger.h"
#include "safe_input.h"

// функция для генерации уникального имени файла с датой и временем
std::string generateFilename() {
    auto t = std::time(nullptr);
    std::tm tm;

#ifdef _WIN32
    localtime_s(&tm, &t);   // Windows
#else
    localtime_r(&t, &tm);   // Linux/macOS
#endif

    std::ostringstream oss;
    oss << "output_"
        << std::put_time(&tm, "%Y%m%d_%H%M%S")
        << ".wav";
    return oss.str();
}

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        int sampleRate = readInt("Введите частоту дискретизации (например, 44100): ", 8000, 192000);
        double duration = readDouble("Введите длительность одной ноты (сек): ", 0.01, 10.0);
        int numFrequencies = readInt("Введите количество частот: ", 1, 100);

        std::vector<double> frequencies(numFrequencies);
        for (int i = 0; i < numFrequencies; i++) {
            frequencies[i] = readDouble("Частота " + std::to_string(i + 1) + ": ", 20.0, 20000.0);
        }

        std::string instrument = "guitar"; // можно потом расширить

        KarplusSynthesizer synth;
        std::vector<float> fullSound;

        // генерируем каждую ноту последовательно
        for (double freq : frequencies) {
            std::vector<double> singleFreq = { freq };
            std::vector<float> note = synth.synthesize(singleFreq, sampleRate, duration, instrument);
            fullSound.insert(fullSound.end(), note.begin(), note.end());
        }

        // float → int16_t для WAV
        std::vector<int16_t> intSamples(fullSound.size());
        for (size_t i = 0; i < fullSound.size(); i++) {
            float clamped = std::max(-1.0f, std::min(1.0f, fullSound[i]));
            intSamples[i] = static_cast<int16_t>(clamped * 32767);
        }

        std::string filename = generateFilename();
        AudioWriter::writeToFile(filename, intSamples, sampleRate);
        AudioLogger::info("Файл записан: " + filename);

        AudioPlayer player;
        player.play(fullSound, sampleRate);

        AudioLogger::info("Готово!");
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
