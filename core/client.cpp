#include <stdio.h>
#include <stdlib.h>

#include <stdexcept>

#include "client.h"

namespace rop {

	client::session::session(boost::asio::ip::tcp::socket& peer):
		peer_(std::move(peer))
	{

	}

	std::pair<uint32_t, std::string> client::session::call(const std::string& operation, const std::string& resource, const std::string& detail) {
		std::array<boost::asio::const_buffer, 4> buffers;
		request_header req_header;
		req_header.len_op = operation.size();
		req_header.len_res = resource.size();
		req_header.len_detail = detail.size();
		req_header.host_to_network();
		buffers[0] = boost::asio::buffer(&req_header, sizeof(req_header));
		buffers[1] = boost::asio::buffer(operation);
		buffers[2] = boost::asio::buffer(resource);
		buffers[3] = boost::asio::buffer(detail);
		boost::system::error_code error;
		boost::asio::write(peer_, buffers, error);
		if (error) {
			throw std::runtime_error("request write error: " + error.message());
		}
		response_header res_header;
		boost::asio::read(peer_, boost::asio::buffer(&res_header, sizeof(res_header)), error);
		if (error) {
			throw std::runtime_error("response header read error: " + error.message());
		}
		res_header.network_to_host();
		std::string result;
		result.resize(res_header.len_result);
		boost::asio::read(peer_, boost::asio::buffer(result), error);
		if (error) {
			throw std::runtime_error("response body read error: " + error.message());
		}
		return std::make_pair(res_header.error_code, std::move(result));
	}

	client::client():
		io_service_()
	{

	}

	client::session client::connect(const std::string& host, uint16_t port)
	{
		boost::asio::ip::tcp::socket peer(io_service_);
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
		peer.connect(endpoint);
		return client::session(peer);
	}

}
