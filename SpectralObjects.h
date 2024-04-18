#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <array>

void writeToFile(std::ofstream& file, int value, int size);
void getSample();
float noise();
void render();
float length;
float samples;
int sampleCount;
std::ofstream wavFile;
int preAudioP;
int sampleInt;
int postAudioP;
const int sampleRate = 48000;
const int byteDepth = 2;
const int channels = 4;
const int harmonics = 480;
const int wavetableSize = 92682;
float fundamentalIncrement;
float totalAmplitude;
std::array<float, wavetableSize> wavetable;
std::array<float, harmonics> phaseIncrements;
std::array<float, harmonics> phases;
std::array<float, harmonics> amplitudes;
std::array<float, harmonics> jitterDepths;
std::array<std::array<float, 2>, harmonics> positions;
float partialSample;
float monoSample;
float monoSign;
std::array<float, 4> sample;

