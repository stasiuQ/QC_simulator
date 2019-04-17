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

	cout << *dynamic_cast<BB84*>(Alice);
	cout << *dynamic_cast<BB84*>(Bob);

	quantum_channel connection(Alice, 20);

	Bob->read_quantum(&connection);
	compare(dynamic_cast<BB84*>(Alice), dynamic_cast<BB84*>(Bob));

	cout << *dynamic_cast<BB84*>(Alice);
	cout << *dynamic_cast<BB84*>(Bob);
	
	delete Alice;
	delete Bob;

	buffer::close();

	return 0;
}
