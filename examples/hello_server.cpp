#include "server.h"

int main(int argc, char** argv) {

	rop::server server(argv[1], std::atoi(argv[2]), 0, [](const std::string& operation, const std::string& resource, const std::string& detail){
		return "hello, world";
	});

	return 0;

}
