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
float fm(float partial, float lfo);
float noise();
float move(float last, float lfo);
std::array<float, 4> pan(std::array<float, 2> xy, float magnitude);
void render();
float length;
float samples;
float sampleCount;
float progress;
float remainingProgress;
std::ofstream wavFile;
int preAudioP;
int sampleInt;
int postAudioP;
const int sampleRate = 48000;
const int byteDepth = 2;
const int channels = 4;
const int harmonics = 256;
const int wavetableSize = 92682;
const float cutoff = 4.0;
float fundamentalIncrement;
float lfoPhaseAIncrement;
float lfoPhaseA;
float lfoA;
float lfoPhaseBIncrement;
float lfoPhaseB;
float lfoB;
float totalAmplitude;
int repositionInterval;
bool reposition;
std::array<float, wavetableSize> wavetable;
std::array<float, harmonics> phaseIncrements;
std::array<std::array<float, 4>, harmonics> phases;
std::array<std::array<float, 2>, harmonics> jitterPositions;
std::array<std::array<float, 2>, harmonics> positions;
std::array<float, harmonics> jitterDepths;
std::array<float, harmonics> amplitudes;
std::array<float, 4> jitterPan;
std::array<float, 4> partialSample;
std::array<float, 4> partialPan;
std::array<float, 4> sample;
