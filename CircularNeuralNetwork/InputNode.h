#pragma once

#include <vector>
#include <iostream>

class InputNode {
public:
	InputNode();

	InputNode(float _current_value, std::vector<float> _weights);

	friend std::ostream& operator<<(std::ostream& out_stream, const InputNode& node);

	friend std::istream& operator>>(std::istream& in_stream, InputNode& node);

	void setCurrentValue(float new_value);

	void randomize();

	float getWeightAt(int index);

	float getCurrentValue();

	void mutate(float scale);

private:
	float current_value;
	std::vector<float> weights;
};