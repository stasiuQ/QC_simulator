#include "channels.h"
#include "protocol.h"
#include "functions.h"
using namespace std;

quantum_channel::quantum_channel(protocol * person)
{
	this->key_size = person->key_size;
	this->QBER_est = 0;
	if (person->is_BB84) {
		this->state_key = person->key;
		this->state_base = person->base;
	}
	else if (person->is_B92) {
		this->state_base = person->key;
		for (int i = 0; i < this->key_size; i++) {
			this->state_key.push_back(0);
		}
	}
}

quantum_channel::~quantum_channel()
{
}

void quantum_channel::make_noise(double max_noise)    // max_noise is a maximal percentage of changed bits
{
	if (max_noise < 0 || max_noise > 100)
		throw "Inappropriate max noise parameter";
	unsigned int rand_iterator;
	bool temp;
	for (int i = 0; i < static_cast<int>(max_noise*key_size/100); i++) {
		rand_iterator = static_cast<unsigned int>((static_cast<double>(rand())/ RAND_MAX)*(key_size+1));
		if (rand_iterator == key_size)
			rand_iterator--;
		temp = buffer::randomize();
		if (temp == true) {
			state_base[rand_iterator] = (state_base[rand_iterator] + 1) % 2;
			temp = buffer::randomize();
			state_key[rand_iterator] = temp;
		}
	}
}

void quantum_channel::error_estimation(protocol * sender, protocol * receiver,float comp_percent,int permutation_num) //we can estimate error level with accuracy modulated with the key percent to compare
{
	
	int key_size_cut= static_cast<int>(key_size*comp_percent / 100);
	vector<bool> send_per_key(sender->temp_key.end() - key_size_cut, sender->temp_key.end());
	vector<bool> rec_per_key(receiver->temp_key.end() - key_size_cut, receiver->temp_key.end());
	vector<bool> send_cut_key(sender->temp_key.end() - key_size_cut, sender->temp_key.end());
	vector<bool> rec_cut_key(receiver->temp_key.end() - key_size_cut, receiver->temp_key.end());
	vector<int> permutation;
	for (int i = 0; i < key_size-key_size_cut; i++) {
		permutation[i] = i;
	}
	for (int i = 0; i < permutation_num, i++) {
		next_permutation(permutation.begin(), permutation.end());
	}// we can choose which permutation we want exactly as it is transported through classical channel

	for (int i = key_size_cut; i < key_size; i++){//permutation is for homogeneous distribution
		send_per_key[i] = send_cut_key[permutation[i]];
		rec_per_key[i] = rec_cut_key[permutation[i]];
	}
	//Now we must cut take xor of both, we'll do it on cut keys, so we don't create now memory reservations

	int errors=0;//we don't need to know their places yet
	for (int i = key_size_cut; i < key_size; i++){
		send_cut_key[i] = (send_cut_key[i] + send_per_key[i]) % 2;
		rec_cut_key[i] = (rec_cut_key[i] + rec_per_key[i]) % 2;
		if (send_cut_key[i] != rec_cut_key[i]) {
			errors++;
		}
	}
	this->QBER_est = errors/(key_size_cut);
	this->key_size = this->key_size - key_size_cut;
	this->state_key.erase(this->state_key.end() - key_size_cut, this->state_key.end());
	this->state_base.erase(this->state_base.end() - key_size_cut, this->state_base.end());
	//I don't know about state key and base, shall we remove it as well?

	sender->key_size = this->key_size;
	sender->key.erase(sender->key.end() - key_size_cut, sender->key.end());
	sender->temp_key.erase(sender->temp_key.end() - key_size_cut, sender->temp_key.end());
	sender->base.erase(sender->base.end() - key_size_cut, sender->base.end());

	receiver->key_size = this->key_size;
	receiver->key.erase(receiver->key.end() - key_size_cut, receiver->key.end());
	receiver->temp_key.erase(receiver->temp_key.end() - key_size_cut, receiver->temp_key.end());//cuts off all sent components
	receiver->base.erase(receiver->base.end() - key_size_cut, receiver->base.end());
	
}

void quantum_channel::Cascade(protocol * sender, protocol * receiver,float alpha,int steps)
{
	vector<int> permutation;
	for (int i = 0; i < key_size; i++) {
		permutation[i] = i;
	}
	for (int j = 0; j < steps; j++)
	{
		int r1 = static_cast<int>(alpha / QBER_est);//block length
		if (r1 > key_size) return;
		while (key_size%r1 != 0) {//to make key able to be divided
			this->key_size = this->key_size - 1;
			this->state_key.erase(this->state_key.end());
			this->state_base.erase(this->state_base.end());
			sender->key_size = this->key_size;
			sender->key.erase(sender->key.end());
			sender->temp_key.erase(sender->temp_key.end());
			sender->base.erase(sender->base.end());

			receiver->key_size = this->key_size;
			receiver->key.erase(receiver->key.end());
			receiver->temp_key.erase(receiver->temp_key.end());
			receiver->base.erase(receiver->base.end());
		}
			//I don't know about state key and base, shall we remove it as well?


		// don't know if I shall remove that
		for (int i = 0; i < j; i++) {
			next_permutation(permutation.begin(), permutation.end());
		}// we can permutate however we want, but this is our model
		vector<bool> temp1;
		vector<bool> temp2;
		for (int i = 0; i < key_size; i++)
		{

			temp1[i] = sender->key[permutation[i]];

			temp2[i] = receiver->temp_key[permutation[i]];

		}
		sender->key = temp1;
		sender->temp_key = temp1;
		receiver->key = temp2;
		receiver->temp_key = temp2;
		//state key?????


		vector<bool> par_check;
		bool par_sen = 0;
		bool par_rec = 0;
		int temp = 0;
		for (int i = 0; i < key_size; i++) {
			if (i%r1 == r1 - 1)
			{
				par_check[i] = ((par_sen + par_rec) % 2); //if it's ok it gives us 0, otherwise 1

				temp++;
				par_sen = 0;
				par_rec = 0;

			}
			if (receiver->temp_key[i] == true)
			{
				par_rec = (par_rec + 1) % 2;
			}
			if (sender->temp_key[i] == true)
			{
				par_sen = (par_sen + 1) % 2;
			}
		}

		for (int i = 0; i < temp; i++) {
			vector<bool> temp_send(sender->temp_key.begin() + i * r1, sender->temp_key.begin() + (i + 1)*r1-1);
			vector<bool> temp_rec(receiver->temp_key.begin() + i * r1, receiver->temp_key.begin() + (i + 1)*r1-1);
			if (par_check[i] == 1) {
				bin_search(temp_send, temp_rec, r1);//negate which gives error
			}
		}
		QBER_est = QBER_est / 2;// We don't look upon if there's another error near the one taken before, go to another step then
	}
}

void quantum_channel::privacy_amp(protocol * sender, protocol * receiver, int treshhold)
{
	vector<bool> temp(sender->temp_key.begin() + static_cast<int>(key_size / 3), sender->temp_key.end());
	//easy to do, Toeplitz matrices
	while (key_size % 3 != 0)
	{
		this->key_size = this->key_size - 1;
		this->state_key.erase(this->state_key.end());
		this->state_base.erase(this->state_base.end());
		sender->key_size = this->key_size;
		sender->key.erase(sender->key.end());
		sender->temp_key.erase(sender->temp_key.end());
		sender->base.erase(sender->base.end());

		receiver->key_size = this->key_size;
		receiver->key.erase(receiver->key.end());
		receiver->temp_key.erase(receiver->temp_key.end());
		receiver->base.erase(receiver->base.end());
	}
	

	vector< vector<bool>> matrix;
	for (int col = 0; col < key_size / 3; col++) {
		matrix[1][col] = temp[col];
	}

	for (int row = 1; row < key_size / 3;row++) {
		matrix[row].push_back(temp[key_size / 3-1+row]);
	}
	vector<bool> raw;
	for (int i = 0; i < key_size / 3; i++) {
		raw[i] = 0;
		for (int j = 0; j < key_size; j++)
		{
			raw[i] = raw[i] + matrix[i][j] * temp[j];
		}
	}
	this->state_key = raw;
	this->key_size = key_size / 3;
	receiver->temp_key = raw;
	receiver->key_size = key_size;
	sender->temp_key = raw;
	sender->key_size = key_size;
}
