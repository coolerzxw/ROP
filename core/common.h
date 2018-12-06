#pragma once

#include <stdint.h>

#include <boost/asio.hpp>

namespace rop {

	struct request_header {

		uint16_t len_op;
		uint16_t len_res;
		uint32_t len_detail;

		void network_to_host() {
			len_op = ntohs(len_op);
			len_res = ntohs(len_res);
			len_detail = ntohl(len_detail);
		}

		void host_to_network() {
			len_op = htons(len_op);
			len_res = htons(len_res);
			len_detail = htonl(len_detail);
		}

	};

	static_assert(sizeof(request_header) == 8, "request header should be 8-byte long");

	struct response_header {

		uint32_t error_code;
		uint32_t len_response;

		void network_to_host() {
			error_code = ntohs(error_code);
			len_response = ntohs(len_response);
		}

		void host_to_network() {
			error_code = htons(error_code);
			len_response = htons(len_response);
		}

	};

	static_assert(sizeof(response_header) == 8, "response header should be 8-byte long");

}
