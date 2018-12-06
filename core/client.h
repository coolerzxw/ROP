#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include <boost/asio.hpp>

#include "common.h"

namespace rop {

	class client {

		boost::asio::io_service io_service_;

	public:

		class session {

			friend class client;

			boost::asio::ip::tcp::socket peer_;

			session(boost::asio::ip::tcp::socket& peer);

		public:

			std::pair<uint32_t, std::string> call(const std::string& operation, const std::string& resource, const std::string& detail);

		};

		client();

		session connect(const std::string& host, uint16_t port);

	};

}
