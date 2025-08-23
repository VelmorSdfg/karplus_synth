#include "audio_writer.h"
#include <fstream>
#include <stdexcept>
#include <cstdint>

// ����� ��������� ����� ��������������� ������� WAV (��� ��������)
#pragma pack(push, 1)

// ��������� ��������� WAV ����� (����������� ������ PCM 16-bit mono)
struct WAVHeader {
    char riff[4] = { 'R','I','F','F' };  // ��������� "RIFF" (������ ����������)
    uint32_t fileSize;                   // ����� ������ ����� - 8 ����
    char wave[4] = { 'W','A','V','E' };  // ��������� "WAVE" (������ �����)

    // ��������� fmt
    char fmtChunkId[4] = { 'f','m','t',' ' }; // ��������� "fmt "
    uint32_t fmtChunkSize = 16;               // ������ ������ fmt (16 ��� PCM)
    uint16_t audioFormat = 1;                 // ������ ����� (1 = PCM)
    uint16_t numChannels = 1;                 // ���-�� ������� (1 = ����, 2 = ������)
    uint32_t sampleRate;                      // ������� ������������� (��)
    uint32_t byteRate;                        // ���������� ���� � ������� (sampleRate * blockAlign)
    uint16_t blockAlign;                      // ���-�� ���� �� ���� "����� ���� �������"
    uint16_t bitsPerSample = 16;              // ��� �� ����� (16 ��� = int16_t)

    // ��������� data
    char dataChunkId[4] = { 'd','a','t','a' }; // ������ "data"
    uint32_t dataChunkSize;                    // ������ ����� ������ (� ������)
};

#pragma pack(pop)

// ������ � WAV-���� ������� ������� (16-��� PCM)
void AudioWriter::writeToFile(const std::string& filename,
    const std::vector<int16_t>& samples,
    int sampleRate)
{
    // ��������� ��������� WAV
    WAVHeader header;
    header.sampleRate = sampleRate;
    header.numChannels = 1;
    header.bitsPerSample = 16;
    header.blockAlign = header.numChannels * header.bitsPerSample / 8;
    header.byteRate = sampleRate * header.blockAlign;
    header.dataChunkSize = static_cast<uint32_t>(samples.size() * sizeof(int16_t));
    header.fileSize = 36 + header.dataChunkSize;

    // ��������� ���� �� ������ � �������� ������
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("�� ������� ������� ���� ��� ������: " + filename);
    }

    // ������� ����� ���������
    file.write(reinterpret_cast<const char*>(&header), sizeof(WAVHeader));

    // ����� � ���� ����������� (������)
    file.write(reinterpret_cast<const char*>(samples.data()), header.dataChunkSize);
}
