#include "client.h"

int main(int argc, char** argv) {

	rop::client client;
	auto session = client.connect(argv[1], std::atoi(argv[2]));
	auto result1 = session.call("AUTH", "guest", "guest").second;
	printf("%s\n", result1.c_str());
	auto result2 = session.call("GET", "username", "").second;
	printf("%s\n", result2.c_str());
	auto result3 = session.call("PUT", "username", "").second;
	printf("%s\n", result3.c_str());

	return 0;

}
