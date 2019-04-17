#include "protocol.h"
#include "BB84.h"

using namespace std;

int main()
{
	buffer::init("random_data.txt");
	
	int size = 15;
	protocol* Alice = new BB84(size);
	protocol* Bob = new BB84(size);
	protocol* Eve = new BB84(size);


	Alice->load_key();
	Alice->generate_basis();

	Bob->load_key();
	Bob->generate_basis();

	Eve->load_key();
	Eve->generate_basis();

	cout << "Alice" << endl << *dynamic_cast<BB84*>(Alice);
	cout << "Bob" << endl << *dynamic_cast<BB84*>(Bob);
	cout << "Eve" << endl << *dynamic_cast<BB84*>(Eve);

	quantum_channel connection(Alice);

	Eve->spy_quantum(&connection);
	Bob->read_quantum(&connection);

	Alice->compare(Bob);

	Eve->spy_classic(Alice);
	Alice->key_reduction();
	Bob->key_reduction();

	cout << "Alice" << endl << *dynamic_cast<BB84*>(Alice);
	cout << "Bob" << endl << *dynamic_cast<BB84*>(Bob);
	cout << "Eve" << endl << *dynamic_cast<BB84*>(Eve);
	
	delete Alice;
	delete Bob;
	delete Eve;

	buffer::close();

	return 0;
}
