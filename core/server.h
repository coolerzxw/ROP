#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string>
#include <vector>
#include <thread>
#include <memory>

#include <boost/asio.hpp>

#include "common.h"

namespace rop {

	std::string noop(const std::string& operation, const std::string& resource, const std::string& detail);

	class server {

		class session {

			friend class server;

			server& server_;
			boost::asio::ip::tcp::socket peer_;
			request_header header_;
			std::array<boost::asio::mutable_buffer, 3> buffers_;
			std::string operation_;
			std::string resource_;
			std::string detail_;

			void do_handle();

			session(server& server, boost::asio::ip::tcp::socket& peer);

		};

		boost::asio::io_service io_service_;
		boost::asio::signal_set signals_;
		boost::asio::ip::tcp::endpoint endpoint_;
		boost::asio::ip::tcp::acceptor acceptor_;
		std::vector<std::thread> io_threads_;
		std::function<std::string(const std::string&, const std::string&, const std::string&)> process_;

		void do_accept();

	public:

		server(const std::string& host, uint16_t port, uint32_t num_threads = 0, const std::function<std::string(const std::string&, const std::string&, const std::string&)>& process = noop);

		~server();

		void reset_handler(const std::function<std::string(const std::string&, const std::string&, const std::string&)>& process);

	};

}
