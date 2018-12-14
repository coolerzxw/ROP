#include <stdio.h>
#include <stdlib.h>

#include <stdexcept>

#include "server.h"

namespace rop {

	std::string noop(const std::string& operation, const std::string& resource, const std::string& detail, std::string& session_data) {
		return "";
	}

	void server::session::do_handle() {
		boost::asio::async_read(peer_, boost::asio::buffer(&req_header_, sizeof(req_header_)), [&](const boost::system::error_code& error, size_t length){
			if (error) {
				if (error != boost::asio::error::eof) {
					fprintf(stderr, "request header read error: %s\n", error.message().c_str());
				}
				delete this;
			} else {
				req_header_.network_to_host();
				operation_.resize(req_header_.len_op);
				resource_.resize(req_header_.len_res);
				detail_.resize(req_header_.len_detail);
				req_buffers_[0] = boost::asio::buffer(operation_);
				req_buffers_[1] = boost::asio::buffer(resource_);
				req_buffers_[2] = boost::asio::buffer(detail_);
				boost::asio::async_read(peer_, req_buffers_, [&](const boost::system::error_code& error, size_t length){
					if (error) {
						fprintf(stderr, "request body read error: %s\n", error.message().c_str());
						delete this;
					} else {
						try {
							result_ = server_.process_(operation_, resource_, detail_, data_);
							res_header_.error_code = 0;
						} catch (std::exception& e) {
							result_ = e.what();
							res_header_.error_code = 1;
						}
						res_header_.len_result = result_.size();
						res_header_.host_to_network();
						res_buffers_[0] = boost::asio::buffer(&res_header_, sizeof(res_header_));
						res_buffers_[1] = boost::asio::buffer(result_);
						boost::asio::async_write(peer_, res_buffers_, [&](const boost::system::error_code& error, size_t length){
							if (error) {
								fprintf(stderr, "response write error: %s\n", error.message().c_str());
								delete this;
							} else {
								do_handle();
							}
						});
					}
				});
			}
		});
	}

	server::session::session(server& server, boost::asio::ip::tcp::socket& peer):
		server_(server),
		peer_(std::move(peer)),
		req_buffers_(),
		req_header_({0, 0, 0}),
		operation_(),
		resource_(),
		detail_(),
		data_(),
		res_buffers_(),
		res_header_({0, 0}),
		result_()
	{
		do_handle();
	}

	void server::do_accept() {
		acceptor_.async_accept([&](const boost::system::error_code& error, boost::asio::ip::tcp::socket peer){
			if (error) {
				fprintf(stderr, "accept error: %s\n", error.message().c_str());
			} else {
				new session(*this, peer);
			}
			do_accept();
		});
	}

	server::server(
		const std::string& host, uint16_t port, uint32_t num_threads,
		const std::function<std::string(const std::string&, const std::string&, const std::string&, std::string&)>& process
	):
		io_service_(),
		signals_(io_service_, SIGINT),
		endpoint_(boost::asio::ip::address::from_string(host), port),
		acceptor_(io_service_, endpoint_),
		io_threads_(),
		process_(process)
	{
		signals_.async_wait([&](const boost::system::error_code& error, int signum){
			if (error) {
				fprintf(stderr, "signal error: %s\n", error.message().c_str());
				return;
			}
			if (signum == SIGINT) {
				io_service_.stop();
			}
		});
		if (num_threads == 0) num_threads = std::thread::hardware_concurrency();
		for (uint32_t thread_id=0;thread_id<num_threads;thread_id++) {
			io_threads_.emplace_back([&,thread_id](){
				io_service_.run();
				// fprintf(stderr, "I/O thread [%u] exits\n", thread_id);
			});
		}
		do_accept();
	}

	server::~server() {
		for (auto& io_thread : io_threads_) {
			io_thread.join();
		}
	}

}
