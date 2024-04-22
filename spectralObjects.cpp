#include "spectralObjects.h"

void writeToFile(std::ofstream& file, int value, int size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}

void getSample() {
    sampleCount++;
    repositionInterval = pow(harmonics, (int)(2.0 * abs(progress - 0.5))) + 1;
    if ((int)sampleCount % repositionInterval == 0) {
        reposition = true;
    }
    else {
        reposition = false;
    }
    progress = sampleCount / samples;
    remainingProgress = 1.0 - progress;
    lfoPhaseAIncrement = pow(remainingProgress, 2.0) * fundamentalIncrement;
    lfoPhaseBIncrement = pow(remainingProgress, 0.5) * fundamentalIncrement;
    lfoPhaseA += lfoPhaseAIncrement;
    lfoPhaseB += lfoPhaseBIncrement;
    lfoPhaseA = fmod(lfoPhaseA, (float)wavetableSize);
    lfoPhaseB = fmod(lfoPhaseB, (float)wavetableSize);
    lfoA = wavetable.at((int)lfoPhaseA);
    lfoB = wavetable.at((int)lfoPhaseB);
    totalAmplitude = 0.0;
    for (int a = 0; a < harmonics; a++) {
        float partial = (float)(a + 1);
        phaseIncrements[a] = ((float)a * pow(progress, 2.0) * 4.0 + 1.0) * fundamentalIncrement;
        amplitudes[a] = (1.0 / partial) * fm(partial, lfoA);
        totalAmplitude += amplitudes[a];
        jitterDepths[a] = fm(partial, lfoB);
        if (reposition) {
            for (int b = 0; b < 2; b++) {
                float lastPosition = positions[a][b];
                float lastJitterPosition = jitterPositions[a][b];
                positions[a][b] = move(lastPosition, lfoA);
            }
        }
    }
    sample = { 0.0, 0.0, 0.0, 0.0 };
    for (int a = 0; a < harmonics; a++) {
        jitterPan = pan(jitterPositions[a], jitterDepths[a]);
        partialPan = pan(positions[a], amplitudes[a]);
        for (int b = 0; b < 4; b++) {
            phases[a][b] += phaseIncrements[a] * noise() * jitterDepths[a] * jitterPan[b] + phaseIncrements[a];
            phases[a][b] = fmod(phases[a][b], (float)wavetableSize);
            partialSample[b] = wavetable.at((int)(phases[a][b])) / totalAmplitude;
            sample[b] += partialSample[b] * partialPan[b];
        }
    }
    //std::cout << sample << std::endl;
}

float fm(float partial, float lfo) {
    return sin(partial * pow(lfo, 3.0) * M_PI * 2.0) * 0.5 + 0.5;
}

float noise() {
    return ((float)(rand() + 1.0) * 2 / (RAND_MAX + 1.0)) - 1;
}

float move(float last, float lfo) {
    return 1.0 - abs(last - 0.5) * (noise() * 0.5 * lfo);
}

std::array<float, 4> pan(std::array<float, 2> xy, float magnitude) {
    std::array<float, 4> channelMagnitudes = { magnitude, magnitude, magnitude, magnitude };
    channelMagnitudes[0] *= sqrt(sqrt(1.0 - xy[0]) * sqrt(1.0 - xy[1]));
    channelMagnitudes[1] *= sqrt(sqrt(xy[0]) * sqrt(1.0 - xy[1]));
    channelMagnitudes[2] *= sqrt(sqrt(1.0 - xy[0]) * sqrt(xy[1]));
    channelMagnitudes[3] *= sqrt(sqrt(xy[0]) * sqrt(xy[1]));
    return channelMagnitudes;
}

void render() {
    samples = length * (float)sampleRate;
    wavFile.open("Test.wav", std::ios::binary);
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
    fundamentalIncrement = (float)wavetableSize / ((float)(harmonics * 2) * cutoff * 4.0);
    for (int a = 0; a < wavetableSize; a++) {
        wavetable[a] = sin((float)a * 2.0 * M_PI / wavetableSize);
    }
    for (int a = 0; a < harmonics; a++) {
        jitterDepths[a] = 0.5;
        //length += (float)((a + 1) * harmonics) / (float)sampleRate;
    }
    length = 60.0;
    sampleCount = 0;
    render();
}