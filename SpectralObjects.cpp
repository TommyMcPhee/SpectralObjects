#include "spectralObjects.h"

void writeToFile(std::ofstream& file, int value, int size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}

void getSample() {
    //sampleCount++;
    monoSample = 0.0;
    sample = { 0.0, 0.0, 0.0, 0.0 };
    totalAmplitude = 0.0;
    for (int a = 0; a < harmonics; a++) {
        totalAmplitude += amplitudes[a];
    }
    for (int a = 0; a < harmonics; a++) {
        phases[a] += noise() * jitterDepths[a] * phaseIncrements[a] + phaseIncrements[a];
        phases[a] = fmod(phases[a], (float)wavetableSize);
        partialSample = wavetable.at((int)(phases[a])) * amplitudes[a] / totalAmplitude;
        monoSample += partialSample;
        sample[0] += partialSample * sqrt(sqrt(1.0 - positions[a][0]) * sqrt(1.0 - positions[a][1]));
        sample[1] += partialSample * sqrt(sqrt(positions[a][0]) * sqrt(1.0 - positions[a][1]));
        sample[2] += partialSample * sqrt(sqrt(1.0 - positions[a][0]) * sqrt(positions[a][1]));
        sample[3] += partialSample * sqrt(sqrt(positions[a][0]) * sqrt(positions[a][1]));
    }
    //std::cout << sample << std::endl;
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
    for (int b = 0; b < samples; b++) {
        getSample();
        for (int a = 0; a < 4; a++) {
            sampleInt = static_cast<int>(sample[a] * maxSampleInt);
            wavFile.write(reinterpret_cast<char*> (&sampleInt), byteDepth);
        }
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
    fundamentalIncrement = (float)wavetableSize / (float)(harmonics * 2);
    for (int a = 0; a < wavetableSize; a++) {
        wavetable[a] = sin((float)a * 2.0 * M_PI / wavetableSize);
    }
    for (int a = 0; a < harmonics; a++) {
        phaseIncrements[a] = (float)(a + 1) * fundamentalIncrement;
        amplitudes[a] = 1.0 / (float)(a + 1);
        jitterDepths[a] = 0.5;
    }
    length = 3.0;
    sampleCount = 0;
    render();
}