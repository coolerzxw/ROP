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

	std::string noop(const std::string& operation, const std::string& resource, const std::string& detail, std::string& session_data);

	class server {

		class session {

			friend class server;

			server& server_;
			boost::asio::ip::tcp::socket peer_;
			std::array<boost::asio::mutable_buffer, 3> req_buffers_;
			request_header req_header_;
			std::string operation_;
			std::string resource_;
			std::string detail_;
			std::string data_;
			std::array<boost::asio::const_buffer, 2> res_buffers_;
			response_header res_header_;
			std::string result_;

			void do_handle();

			session(server& server, boost::asio::ip::tcp::socket& peer);

		};

		boost::asio::io_service io_service_;
		boost::asio::signal_set signals_;
		boost::asio::ip::tcp::endpoint endpoint_;
		boost::asio::ip::tcp::acceptor acceptor_;
		std::vector<std::thread> io_threads_;
		std::function<std::string(const std::string&, const std::string&, const std::string&, std::string&)> process_;

		void do_accept();

	public:

		/**
		 * \param    host           The address to bind.
		 * \param    port           The port to bind.
		 * \param    num_threads    The number of threads to use.
		 * \param    process        The request handler. The first three parameters
		 * correspond to the operation, resource, and detail of the desired request.
		 * The last parameter is the mutable session data which can be used to maintain
		 * session-local states. Exceptions can be thrown within the handler, and the
		 * response result is specified through the return value.
		 *
		 * \return                  The response result.
		 */
		server(
			const std::string& host,
			uint16_t port,
			uint32_t num_threads = 0,
			const std::function<std::string(const std::string&, const std::string&, const std::string&, std::string&)>& process = noop
		);

		~server();

	};

}
