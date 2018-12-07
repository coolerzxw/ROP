#include "server.h"

int main(int argc, char** argv) {

	rop::server server(
		argv[1], std::atoi(argv[2]), 0,
		[](const std::string& operation, const std::string& resource, const std::string& detail, std::string& session_data)->std::string{
			if (operation == "AUTH") {
				auto& username = resource;
				auto& password = detail;
				session_data = username;
				return "hello, " + username + "!";
			} else if (operation == "GET" && resource == "username") {
				auto& username = session_data;
				return "hello again, " + username + "!";
			} else {
				return "unsupported method!";
			}
		}
	);

	return 0;

}
