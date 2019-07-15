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

quantum_channel::quantum_channel(const quantum_channel & channel)
{
	this->key_size = channel.key_size;
	this->state_key = channel.state_key;
	this->state_base = channel.state_base;
	this->QBER_est = channel.QBER_est;
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
		rand_iterator = static_cast<unsigned int>(buffer::rand_1()*(key_size));
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

void quantum_channel::error_estimation(protocol * sender, protocol * receiver,float comp_percent) //we can estimate error level with accuracy modulated with the key percent to compare
{
	long long permutation_num = buffer::rand_int(8);
	int key_size_cut= static_cast<int>(sender->actual_key_size*comp_percent / 100);
	vector<bool> send_per_key(sender->key.begin(), sender->key.end());
	vector<bool> rec_per_key(receiver->key.begin(), receiver->key.end());
	vector<bool> send_cut_key(sender->key.end() - key_size_cut, sender->key.end());
	vector<bool> rec_cut_key(receiver->key.end() - key_size_cut, receiver->key.end());
	vector<int> permutation;
	for (int i = 0; i <sender->actual_key_size; i++) {
		permutation.push_back(i);
	}
	for (int i = 0; i < permutation_num; i++) {
		next_permutation(permutation.begin(), permutation.end());
	}// we can choose which permutation we want exactly as it is transported through classical channel

	for (int i = 0; i <sender->actual_key_size; i++){//permutation is for homogeneous distribution
		send_per_key[i] = sender->key[permutation[i]];
		rec_per_key[i] = receiver->key[permutation[i]];
	}

	//Now we must cut take xor of both, we'll do it on cut keys, so we don't create now memory reservations
	send_per_key.erase(send_per_key.begin(), send_per_key.end() - key_size_cut);
	rec_per_key.erase(rec_per_key.begin(), rec_per_key.end() - key_size_cut);// to make them the same length
	int errors=0;//we don't need to know their places yet
	for (int i = 0; i < key_size_cut; i++){
		//send_cut_key[i] = (send_cut_key[i] + send_per_key[i])%2 ;
		//rec_cut_key[i] = (rec_cut_key[i] + rec_per_key[i])%2;
		if (send_cut_key[i] != rec_cut_key[i]) {
			errors++;
		}
	}
	this->QBER_est = static_cast<double>(errors)/(static_cast<double>(key_size_cut));
	//I don't know about state key and base, shall we remove it as well?

	sender->actual_key_size -= key_size_cut;
	sender->key.erase(sender->key.end() - key_size_cut, sender->key.end());

	receiver->actual_key_size -= key_size_cut;
	receiver->key.erase(receiver->key.end() - key_size_cut, receiver->key.end());//cuts off all sent components
	
}

void quantum_channel::Cascade(protocol * sender, protocol * receiver,float alpha,int steps)
{

	for (int j = 0; j < steps; j++)
	{
		long long permutation_num = buffer::rand_int(8);
		if (QBER_est == 0) {
			return;
		}
		double QBER_esti = QBER_est;
		vector<int> permutation;
		for (int i = 0; i < sender->actual_key_size; i++) {
			permutation.push_back(i);
		}

		int r1 = static_cast<int>(alpha / QBER_esti);//block length
		if (r1 >= sender->actual_key_size || sender->actual_key_size<10) {
			return;
		}
		while (sender->actual_key_size%r1 != 0) {//to make key able to be divide
			sender->actual_key_size -=1;
			sender->key.pop_back();


			receiver->actual_key_size -= 1;
			receiver->key.pop_back();


			permutation.pop_back();
		}
			//I don't know about state key and base, shall we remove it as well?


		// don't know if I shall remove that
		for (int i = 0; i < permutation_num; i++) {
			next_permutation(permutation.begin(), permutation.end());
		}// we can permutate however we want, but this is our model
		vector<bool> temp1= sender->key;
		vector<bool> temp2= receiver->key;// asign only at the beginning
		for (int i = 0; i < sender->actual_key_size; i++)
		{

			temp1[i] = sender->key[permutation[i]];

			temp2[i] = receiver->key[permutation[i]];

		}
		sender->key = temp1;
		receiver->key = temp2;
		//state key?????

		vector<bool> par_check;
		bool par_sen = 0;
		bool par_rec = 0;
		int temp = 0;
		for (int i = 0; i < sender->actual_key_size; i++) {

			if (receiver->key[i] == true)
			{
				par_rec = (par_rec + 1) % 2;
			}
			if (sender->key[i] == true)
			{
				par_sen = (par_sen + 1) % 2;
			}

			if (i%r1 == r1 - 1)//end of block
			{
				if (par_rec != par_sen) {
					par_check.insert(par_check.end(),1); //if it's ok it gives us 0, otherwise 1
				}
				else {
					par_check.insert(par_check.end(), 0);
				}

				temp++;
				par_sen = 0;
				par_rec = 0;
			}

		}

		for (int i = 0; i < temp; i++) {
			if (par_check[i] == 1) {
				bool is_changed = 0;
				vector<bool> changed(sender->actual_key_size);
				bin_search(sender->key, receiver->key, i*r1,i*r1,r1,changed,is_changed);//negate which gives error
				vector<bool> temp_changed(changed.begin(), changed.end());;
				for (int j = 0; j < sender->actual_key_size; j++)
				{
					temp_changed[i] = changed[permutation[j]];
				}
				changed = temp_changed;//permutate changed table
			}

		}
		QBER_esti = QBER_esti / 2;// We don't look upon if there's another error near the one taken before, go to another step then
	}
}

void quantum_channel::privacy_amp(protocol * sender, protocol * receiver, int treshhold)
{
	
	//easy to do, Toeplitz matrices
	while (sender->actual_key_size % 3 != 0)
	{

		sender->actual_key_size -=1;
		sender->key.pop_back();


		receiver->actual_key_size -= 1;
		receiver->key.pop_back();
		
	}
	vector<bool> temp(sender->key.begin() + static_cast<int>(sender->actual_key_size / 3), sender->key.end());

	vector< vector<bool>> matrix(sender->actual_key_size / 3,vector<bool>(sender->actual_key_size / 3));
	for (int col = 0; col < sender->actual_key_size / 3; col++) {
		matrix[0][col] = temp[col];
	}

	for (int row = 1; row < sender->actual_key_size / 3;row++) {
		for (int i = 1; i <= row; i++) {
			matrix[row].pop_back();
			matrix[row].insert(matrix[row].begin(),temp[sender->actual_key_size / 3 - 1 + i]);
		}

	}
	vector<bool> raw_sen(sender->actual_key_size / 3);
	vector<bool> raw_rec(sender->actual_key_size / 3);
	vector<bool> sen_key(sender->key.begin(), sender->key.begin() + (sender->actual_key_size / 3));
	vector<bool> rec_key(receiver->key.begin(), receiver->key.begin() + (sender->actual_key_size / 3));
	for (int i = 0; i < sender->actual_key_size / 3; i++) {
		raw_sen[i] = 0;
		raw_rec[i] = 0;
		for (int j = 0; j < sender->actual_key_size/3; j++)
		{
			raw_sen[i] = (raw_sen[i] + matrix[i][j] * sen_key[j])%2;
			raw_rec[i] = (raw_rec[i] + matrix[i][j] * rec_key[j])%2;
		}
	}
	receiver->key = raw_rec;
	receiver->actual_key_size = raw_rec.size();
	sender->key = raw_sen;
	sender->actual_key_size = raw_sen.size();
}

quantum_channel & quantum_channel::operator=(const quantum_channel & channel)
{
	this->key_size = channel.key_size;
	this->state_key = channel.state_key;
	this->state_base = channel.state_base;
	this->QBER_est = channel.QBER_est;

	return *this;
}
