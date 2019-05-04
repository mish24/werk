#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>

#include "Werk/Utility/NamedObject.hpp"
#include "ConsoleMessage.hpp"

namespace Werk {

	//console server that uses ipc queue to receive message
	class IpcConsoleServer : public NamedObject {

	private:
		boost::interprocess::message_queue _queue;

	public:
		static void remove(const std::string& name) {
			boost::interprocess::message_queue::remove(name.c_str());
		}

		IpcConsoleServer(const std::string& name, uint32_t maxMessages=1024) :
		NamedObject(name), _queue(boost::interprocess::create_only, name.c_str(), maxMessages, sizeof(ConsoleMessage)) {}

		~IpcConsoleServer() { remove(name()); }

		bool receive(uint32_t& sequenceNumber, std::string& message);
	};

}