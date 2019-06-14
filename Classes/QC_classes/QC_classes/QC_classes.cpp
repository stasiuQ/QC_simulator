#include "protocol.h"
#include "BB84.h"
#include "B92.h"

using namespace std;

int main()
{
	buffer::init();
	srand(time(NULL));

	int size = 100;
	protocol* Alice = new B92(size,45);
	protocol* Bob = new B92(size,45);
	//protocol* Eve = new BB84(size);


	Alice->load_key();
	Alice->generate_basis();

	Bob->load_key();
	Bob->generate_basis();

	//Eve->load_key();
	//Eve->generate_basis();

	cout << "Alice" << endl << *dynamic_cast<BB84*>(Alice);
	cout << "Bob" << endl << *dynamic_cast<BB84*>(Bob);
	//cout << "Eve" << endl << *dynamic_cast<BB84*>(Eve);

	quantum_channel connection(Alice);
	connection.make_noise(20);

	//Eve->spy_quantum(&connection);
	Bob->read_quantum(&connection);

	Alice->compare(Bob);

	//Eve->spy_classic(Alice);
	Alice->key_reduction();
	Bob->key_reduction();

	cout << "Alice" << endl << *dynamic_cast<BB84*>(Alice);
	cout << "Bob" << endl << *dynamic_cast<BB84*>(Bob);
	//cout << "Eve" << endl << *dynamic_cast<BB84*>(Eve);
	
	delete Alice;
	delete Bob;
	//delete Eve;

	buffer::close();

	return 0;
}
