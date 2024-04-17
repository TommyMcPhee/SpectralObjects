#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <array>

void writeToFile(std::ofstream& file, int value, int size);
float getSample();
float noise();
void render();
float length;
int sampleCount;
std::ofstream wavFile;
int preAudioP;
int sampleInt;
int postAudioP;
const int sampleRate = 48000;
const int byteDepth = 2;
const int channels = 4;
const int harmonics = 480;
int wavetableSize;
float fundamentalIncrement;
std::vector<float> wavetable;
std::array<float, harmonics> phaseIncrements;
std::array<float, harmonics> phases;
std::array<float, harmonics> partialSamples;
std::array<float, harmonics> amplitudes;
std::array<float, harmonics> jitterDepths;
std::array<float, harmonics> jitterDeltas;
std::array<float, harmonics> currentOffsets;
std::array<std::array<float, 2>, harmonics> positions;
float sample;
float samples;
//std::array<float, 4> sample;


