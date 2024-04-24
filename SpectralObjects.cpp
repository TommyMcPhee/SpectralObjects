#include "spectralObjects.h"

void writeToFile(std::ofstream& file, int value, int size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}

void getSample() {
    sampleCount++;
    progress = sampleCount / samples;
    remainingProgress = 1.0 - progress;
    lfoPhaseAIncrement = pow(remainingProgress, 0.5) * fundamentalIncrement;
    lfoPhaseBIncrement = pow(remainingProgress, 2.0) * fundamentalIncrement;
    lfoPhaseA += lfoPhaseAIncrement;
    lfoPhaseB += lfoPhaseBIncrement;
    lfoPhaseA = fmod(lfoPhaseA, (float)wavetableSize);
    lfoPhaseB = fmod(lfoPhaseB, (float)wavetableSize);
    lfoA = wavetable.at((int)lfoPhaseA);
    lfoB = wavetable.at((int)lfoPhaseB);
    totalAmplitude = 0.0;
    for (int a = 0; a < harmonics; a++) {
        phaseIncrements[a] = ((float)a * pow(progress, 2.0) * 4.0 + 1.0) * fundamentalIncrement;
        float partial = (float)(a + 1);
        float ampCurve = pow(1.0 / partial, progress);
        float jitterCurve = 1.0 - ampCurve;
        float freqA = lfoA * remainingProgress;
        float freqB = lfoB * progress;
        amplitudes[a] = ampCurve * fm(partial, lfoA * remainingProgress);
        amplitudePositions[a] = fm(partial, ampCurve * (1.0 - freqA));
        jitter[a] = fm(partial, lfoB * progress);
        jitterPositions[a] = fm(partial, jitterCurve * (1.0 - freqB));
        totalAmplitude += amplitudes[a];
    }
    sample = { 0.0, 0.0 };
    for (int a = 0; a < harmonics; a++) {
        jitterPan = pan(jitterPositions[a], jitter[a]);
        partialPan = pan(amplitudePositions[a], amplitudes[a]);
        for (int b = 0; b < 2; b++) {
            phases[a][b] += phaseIncrements[a] * noise() * jitter[a] * jitterPan[b] + phaseIncrements[a];
            phases[a][b] = fmod(phases[a][b], (float)wavetableSize);
            partialSample[b] = wavetable.at((int)(phases[a][b])) / totalAmplitude;
            sample[b] += partialSample[b] * partialPan[b];
        }
    }
}

float fm(float partial, float lfo) {
    return sin(partial * pow(lfo, 3.0) * M_PI * 2.0) * 0.5 + 0.5;
}

float noise() {
    return ((float)(rand() + 1.0) * 2 / (RAND_MAX + 1.0)) - 1;
}

std::array<float, 2> pan(float distribution, float magnitude) {
    std::array<float, 2> channelMagnitudes = { magnitude, magnitude };
    channelMagnitudes[0] *= sqrt(1.0 - distribution);
    channelMagnitudes[1] *= sqrt(distribution);
    return channelMagnitudes;
}

void render() {
    samples = length * (float)sampleRate;
    wavFile.open("StudyforSinusoidalSpectraTest.wav", std::ios::binary);
    wavFile << "RIFF";
    wavFile << "----";
    wavFile << "WAVE";
    wavFile << "fmt ";
    writeToFile(wavFile, byteDepth * 8, 4);
    writeToFile(wavFile, 1, 2);
    writeToFile(wavFile, channels, 2);
    writeToFile(wavFile, sampleRate, 4);
    writeToFile(wavFile, sampleRate * byteDepth, 4);
    writeToFile(wavFile, byteDepth, 2);
    writeToFile(wavFile, byteDepth * 8, 2);
    wavFile << "data";
    wavFile << "----";
    preAudioP = wavFile.tellp();
    auto maxSampleInt = pow(2, byteDepth * 8 - 1) - 1;
    for (int c = 0; c < samples; c++) {
        getSample();
        for (int a = 0; a < 2; a++) {
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
    fundamentalIncrement = (float)wavetableSize / ((float)(harmonics * 2) * cutoff * 4.0);
    for (int a = 0; a < wavetableSize; a++) {
        wavetable[a] = sin((float)a * 2.0 * M_PI / wavetableSize);
    }
    for (int a = 0; a < harmonics; a++) {
        jitter[a] = 0.0;
        amplitudePositions[a] = 0.5;
        jitterPositions[a] = 0.5;
        length += (float)((a + 1) * harmonics) / (float)sampleRate;
    }
    sampleCount = 0;
    render();
}