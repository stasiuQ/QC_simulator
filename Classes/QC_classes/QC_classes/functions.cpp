#include "functions.h"

void bin_search(vector<bool> &sender, vector<bool> &receiver,int begin_sen, int begin_rec,int size)
{

	if (size == 1 && sender[begin_sen]!=receiver[begin_rec])
	{
		receiver[begin_rec] = sender[begin_sen];
		return;
	}
	else if (size == 1)
	{
		return;
	}
	

	bool par_rec = 0;
	bool par_sen = 0;
	for (int i = begin_sen; i < begin_sen + size; i++) {
		par_rec = (par_rec + receiver[i]) % 2;
		par_sen = (par_sen + sender[i]) % 2;
	}
	if (par_rec != par_sen) {
		int size_left = static_cast<int>(floor(size / 2));
		int size_right = size - size_left;
		bin_search(sender, receiver,begin_sen,begin_rec,size_left);
		bin_search(sender, receiver, begin_sen+size_left, begin_rec+size_left, size_right);
	}
}
