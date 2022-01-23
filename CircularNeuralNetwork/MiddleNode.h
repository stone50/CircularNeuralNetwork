#pragma once

#include <vector>
#include <iostream>

class MiddleNode {
public:
	MiddleNode();

	MiddleNode(float _current_value, std::vector<float> _weights, float _bias);

	friend std::ostream& operator<<(std::ostream& out_stream, const MiddleNode& node);

	friend std::istream& operator>>(std::istream& in_stream, MiddleNode& node);

	void calcCurrentValue();

	void randomize();

	void addInput(float input);

	float getWeightAt(int index);

	float getCurrentValue();

	void mutate(float scale);

private:
	float current_value;
	std::vector<float> inputs;
	std::vector<float> weights;
	float bias;
};