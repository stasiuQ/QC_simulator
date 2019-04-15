#include "protocol.h"

using namespace std;

int main()
{
	buffer::init("random_data.txt");
	
	for (int i = 0; i < 50; i++) {
		cout << buffer::randomize() << endl;
	}

	buffer::close();

	return 0;
}
