#pragma once
#include <stdio.h>
#include <fstream>
#include <cmath>
#include "spectrum.h"

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
const int harmonics = 4800;
float sample;
float samples;

spectrum spectrumA;
