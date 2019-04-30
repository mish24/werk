//this func can be called from main or anywhere, hence 
//move it to cpp
#pragma once

#include <thread>
#include <time.h>
#include <vector>

#include "Werk/Utility/Action.hpp" 
#include "Werk/OS/Time.hpp"

namespace Werk {

	class BackgroundThread {

	public:
		BackgroundThread(uint64_t frequencyNs=100ul * 1000 * 1000) : 
		_frequencyNs(frequencyNs) {
			_thread = std::thread(&BackgroundThread::backgroundThread, this);
		}

		~BackgroundThread() { stop(); }

		uint64_t frequencyNs() const { return _frequencyNs; }
		void setFrequencyNs(long frequencyNs) { _frequencyNs = frequencyNs; }
		//to be called from main mostly
		std::vector<Action*>& tasks() { return _tasks; }
		const std::vector<Action*>& tasks() const { return _tasks; }
		void addTask(Action* task) { _tasks.push_back(task);}
		const Clock& backgroundClock() const { return _backgroundClock; }

		void stop() {
			if(_running) {
				_running = false;
				_thread.join();
			}
		}

	private:
		timespec _delay;
		Clock _backgroundClock;
		std::vector<Action*> _tasks;
		volatile uint64_t _frequencyNs;
		volatile bool _running = true;
		std::thread _thread;

		void backgroundThread();
	};
}