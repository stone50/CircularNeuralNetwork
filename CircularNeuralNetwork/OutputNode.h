#pragma once

#include <vector>
#include <iostream>

class OutputNode {
public:
	OutputNode();

	OutputNode(float _current_value, float _bias);

	friend std::ostream& operator<<(std::ostream& out_stream, const OutputNode& node);

	friend std::istream& operator>>(std::istream& in_stream, OutputNode& node);

	void calcCurrentValue();

	void randomize();

	void addInput(float input);

	float getCurrentValue();

	void mutate(float scale);

private:
	float current_value;
	std::vector<float> inputs;
	float bias;
};