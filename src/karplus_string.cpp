#include "Karplus_String.h"
#include <random>

// ����������� "����������� ������"
// frequency  � ������� ��������� (��)
// sampleRate � ������� ������������� (��)
KarplusString::KarplusString(double frequency, int sampleRate) {
    // ����� ������ � ���������� �������� �� ���� ������ ���������
    N = static_cast<int>(sampleRate / frequency);
    if (N < 2) N = 2; // ������ �� ������� ������� ������

    // �������������� ����� ������
    buffer.resize(N, 0.0f);

    // ������� ������ � ������
    index = 0;
}

// "ĸ����� ������" (���������������� �����)
// � ����� ������������ ��������� �������� �� -0.5 �� +0.5,
// ������� ��������� ��������� ��������� ������.
void KarplusString::pluck() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(-0.5f, 0.5f);

    for (int i = 0; i < N; i++) {
        buffer[i] = dist(rng);
    }
}

// ��������� ���������� ������ (�������� �������� ����������������)
// ���� ������� ���� �������� ��������, �������� �� ����������� ���������,
// ��������� ����� � ���������� ����� ��������.
float KarplusString::generate() {
    // ���� ������� �������� � ���������
    float first = buffer[index];
    float next = buffer[(index + 1) % N];

    // ����� �������� = ������� ���� ������� * ����������� ���������
    float newSample = 0.996f * 0.5f * (first + next);

    // ���������� ������� � �����
    buffer[index] = newSample;

    // �������� ������ �� ������
    index = (index + 1) % N;

    // ���������� ��������������� �����
    return newSample;
}
