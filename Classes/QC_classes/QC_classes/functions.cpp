#include "functions.h"

void bin_search(vector<bool>& sender, vector<bool>& receiver,int size)
{
	if (size == 1)
	{
		!receiver[0];
		return;
	}
	bool par_rec = 0;
	bool par_sen = 0;
	for (int i = 0; i < size; i++) {
		par_rec = (par_rec + receiver[i]) % 2;
		par_sen = (par_sen + sender[i]) % 2;
	}
	if (par_rec != par_sen) {
		int size_left = static_cast<int>(floor(size / 2));
		int size_right = size - size_left;
		vector<bool> left_sen(sender.begin(), sender.begin() + size_left-1);
		vector<bool> left_rec(receiver.begin(), receiver.begin() + size_left-1);
		vector<bool> right_sen(sender.begin()+size_left, sender.begin() + size_right-1);
		vector<bool>  right_rec(receiver.begin()+size_left, receiver.begin() + size_right-1);
		bin_search(left_sen, left_rec, size_left);
		bin_search(right_sen, right_rec, size_right);
	}
}
