#include "spectralObjects.h"

void writeToFile(std::ofstream& file, int value, int size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}

float getSample() {
    sampleCount++;
    return spectrumA.renderSample()[0];
    //return noise();
}

float noise() {
    return ((float)(rand() + 1.0) * 2 / (RAND_MAX + 1.0)) - 1;
}

void render() {
    samples = length * (float)sampleRate;
    wavFile.open("Test.wav", std::ios::binary);
    // Header chunk
    wavFile << "RIFF";
    wavFile << "----";
    wavFile << "WAVE";
    // Format chunk
    wavFile << "fmt ";
    writeToFile(wavFile, byteDepth * 8, 4); // Size
    writeToFile(wavFile, 1, 2); // Compression Code (PCM)
    writeToFile(wavFile, channels, 2); // Channels
    writeToFile(wavFile, sampleRate, 4); // Sample Rate
    writeToFile(wavFile, sampleRate * byteDepth, 4); // Byte Rate
    writeToFile(wavFile, byteDepth, 2); // Block Align
    writeToFile(wavFile, byteDepth * 8, 2); // Bit Depth
    // Data chunk
    wavFile << "data";
    wavFile << "----";
    preAudioP = wavFile.tellp();
    auto maxSampleInt = pow(2, byteDepth * 8 - 1) - 1;
    for (int a = 0; a < samples; a++) {
        sample = getSample();
        sampleInt = static_cast<int>(sample * maxSampleInt);
        wavFile.write(reinterpret_cast<char*> (&sampleInt), byteDepth);
    }
    postAudioP = wavFile.tellp();
    wavFile.seekp(preAudioP - 4);
    writeToFile(wavFile, postAudioP - preAudioP, 4);
    wavFile.seekp(4, std::ios::beg);
    writeToFile(wavFile, postAudioP - 8, 4);
    wavFile.close();
}

int main()
{
    spectrumA = spectrum(10.0, sampleRate);
    for (int a = 0; a < spectrumA.harmonics.size(); a++) {
        spectrumA.harmonics.at(a) = { 1.0, 0.0, 0.0 };
    }
    length = 3.0;
    sampleCount = 0;
    render();
}