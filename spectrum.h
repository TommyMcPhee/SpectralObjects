#pragma once
#include <stdio.h>
#include <vector>
#include <array>
#include "sinOsc.hpp"

class spectrum {
public:
	spectrum();
	spectrum(float fundamental, int sampleRate);
	std::vector<std::array<float, 4>> renderSpectrum();
	std::vector<std::array<float, 4>> renderBuffer(int samples);
	std::array<float, 4> renderSample();
	std::vector<std::array<float, 3>> harmonics;
	std::vector<std::array<float, 3>> spread;
	std::vector<std::array<float, 3>> jitter;
	float harmonicCount;

private:
	std::vector<float> centerFrequencies;
	std::vector<std::array<sinOsc, 4>> oscillators;
};