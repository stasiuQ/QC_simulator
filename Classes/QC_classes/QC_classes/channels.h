#pragma once

#include <fstream>
#include <vector>

using namespace std;

class protocol;
class BB84;

class quantum_channel {
protected:
	int key_size;
	vector<bool> state_key;
	vector<bool> state_base;

public:
	double QBER_est;

	quantum_channel(protocol* person);
	quantum_channel(const quantum_channel& channel);  // copying constructor
	~quantum_channel();
	void make_noise(double max_noise);
	void error_estimation(protocol* sender, protocol* receiver, float comp_percent);
	void Cascade(protocol* sender, protocol* receiver,float alpha,int steps);
	void privacy_amp(protocol* sender, protocol* receiver, int treshhold);

	quantum_channel& operator = (const quantum_channel& channel);

	friend class protocol;
	friend class BB84;
	friend class B92;

};