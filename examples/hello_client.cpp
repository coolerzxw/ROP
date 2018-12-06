#include "client.h"

int main(int argc, char** argv) {

	rop::client client;
	auto session = client.connect(argv[1], std::atoi(argv[2]));
	auto response = session.call("GET", "greeting", "hello, world");

	return 0;

}
