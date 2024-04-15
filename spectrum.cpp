#include "spectrum.h"

spectrum::spectrum() {

}

spectrum::spectrum(float fundamental, int sampleRate) {
	float frequency = fundamental;
	harmonicCount = 0.0;
	while (frequency + ((harmonicCount + 1.0) * fundamental) <= (float)sampleRate) {
		harmonicCount++;
		frequency = fundamental * harmonicCount;
		centerFrequencies.push_back(frequency);
		std::array<sinOsc, 4> channelOscillators;
		for (int a = 0; a < 4; a++) {
			channelOscillators[a] = sinOsc(frequency, 0.0, 0.0, sampleRate);
		}
		oscillators.push_back(channelOscillators);
	}
	harmonics.resize(harmonicCount);
	spread.resize(harmonicCount);
	jitter.resize(harmonicCount);
}

std::vector <std::array<float, 4>> spectrum::renderSpectrum() {
	std::vector<std::array<float, 4>> spectrum;
	spectrum.clear();
	std::array<float, 4> sample = { 0.0, 0.0, 0.0, 0.0 };
	for (int b = 0; b < harmonicCount; b++) {
		for (int a = 0; a < 4; a++) {
			sample[a] += oscillators.at(b)[a].getSample();
		}
		spectrum.push_back(sample);
	}
	return spectrum;
}

std::vector<std::array<float, 4>> spectrum::renderBuffer(int samples) {
	std::vector<std::array<float, 4>> buffer;
	buffer.clear();
	for (int c = 0; c < samples; c++) {
		buffer.push_back(renderSample());
	}
	return buffer;
}

std::array<float, 4> spectrum::renderSample() {
	std::array<float, 4> sample = { 0.0, 0.0, 0.0, 0.0 };
	for (int b = 0; b < harmonicCount; b++) {
		for (int a = 0; a < 4; a++) {
			oscillators.at(b)[a].setAmp(harmonics.at(b)[0]);
			sample[a] += oscillators.at(b)[a].getSample();
		}
	}
	for (int a = 0; a < 4; a++) {
		sample[a] /= harmonicCount;
	}
	return sample;
}