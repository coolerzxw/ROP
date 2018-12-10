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

			/**
			 * \param    operation    The requested operation.
			 * \param    resource     The requested resource.
			 * \param    detail       The detail of the request.
			 *
			 * \return                The error code (0 on success) and the response result.
			 */
			std::pair<uint32_t, std::string> call(const std::string& operation, const std::string& resource, const std::string& detail);

		};

		client();

		/**
		 * \param    host    The remote host to connect.
		 * \param    port    The remote port to connect.
		 */
		session connect(const std::string& host, uint16_t port);

	};

}
