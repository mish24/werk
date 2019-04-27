#pragma once 

namespace Werk {

	class BackgroundTask {

	public:
		BackgroundTask(const std::string& name) : _name(name) {}
		const std::string& taskName() const { return _name; }
		virtual void executeTask() = 0;
	private:
		std::string _name;
	};

}