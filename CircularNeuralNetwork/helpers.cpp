#include <stdlib.h>
#include <math.h>
#include "helpers.h"

float random(float min, float max) {
	return (rand() * (max - min) / RAND_MAX) + min;
}

float sigmoid(float num) {
	return (float)tanh(num);
}

float clamp(float value, float min, float max) {
	if (value > max) {
		return max;
	}
	if (value < min) {
		return min;
	}
	return value;
}