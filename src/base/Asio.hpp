//
// NeptuneVM
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <boost/asio/as_tuple.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/detail/timer_scheduler.hpp>
#include <boost/asio/basic_waitable_timer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/system_executor.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/deferred.hpp>

namespace neptunevm {

	namespace net = boost::asio;
	using tcp = net::ip::tcp;
	using unix_stream = net::local::stream_protocol;

	using error_code = boost::system::error_code;

	/**
	 * The type for executors to follow.
	 * This is the basic executor type.
	 */
	using BaseExecutorType = net::io_context::executor_type;

	/**
	 * The (real) type executors follow.
	 */
	using ExecutorType = net::strand<BaseExecutorType>; // ideally should use some default, but can't nest as_tuple_t doing it this way.

	/**
	 * CompletionToken for allowing usage of coroutines w/o exception handling.
	 * 
	 * \see Boost.Asio `CompletionToken`
	 */
	constexpr inline auto use_tuple_awaitable = net::as_tuple(net::use_awaitable_t<ExecutorType> {});
	
	/**
	 * net::deferred can be used with asio awaitables, and avoids creation of new coroutine frames.
	 *
	 * \see Boost.Asio `CompletionToken`
	 */
	constexpr inline auto use_tuple_deferred = net::as_tuple(net::deferred);

	/**
	 * Awaitable type (configured for the current executor)
	 */
	template <class T>
	using Awaitable = net::awaitable<T, ExecutorType>;

	template <typename Protocol>
	using SocketType = net::basic_stream_socket<Protocol, ExecutorType>;

	using SteadyTimerType = net::basic_waitable_timer<std::chrono::steady_clock, net::wait_traits<std::chrono::steady_clock>, ExecutorType>;

	template <typename Protocol>
	using AcceptorType = net::basic_socket_acceptor<Protocol, ExecutorType>;

} // namespace neptunevm
