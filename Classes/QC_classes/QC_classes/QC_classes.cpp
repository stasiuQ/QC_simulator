#include "protocol.h"
#include "BB84.h"

using namespace std;

int main()
{
	buffer::init("random_data.txt");
	
	protocol* Alice = new BB84(20);
	protocol* Bob = new BB84(20);

	Alice->load_key();
	Alice->generate_basis();

	Bob->load_key();
	Bob->generate_basis();

	cout << Alice;
	cout << Bob;

	quantum_channel connection(Alice, 20);

	Bob->read_quantum(&connection);
	compare(Alice, Bob);
	
	buffer::close();

	return 0;
}
