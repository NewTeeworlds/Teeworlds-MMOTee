#include "math.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <random>

std::random_device RandomDevice;
std::mt19937_64 RandomEngine(RandomDevice());

float random_float() {
	std::uniform_real_distribution<float> Distribution(0.0f, 1.0f);
	return Distribution(RandomEngine);
}

bool random_prob(float f) {
	std::uniform_real_distribution<float> Distribution(0.0f, 1.0f);
	return (Distribution(RandomEngine) <= f);
}

int random_int(int Min, int Max) {
	std::uniform_int_distribution<int> Distribution(Min, Max);
	return Distribution(RandomEngine);
}

int random_distribution(double *pProb, double *pProb2) {
	std::discrete_distribution<int> Distribution(pProb, pProb2);
	return Distribution(RandomEngine);
}
