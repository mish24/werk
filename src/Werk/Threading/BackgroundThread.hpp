//this func can be called from main or anywhere, hence 
//move it to cpp
#pragma once

#include <thread>
#include <time.h>
#include <vector>

#include "BackgroundTask.hpp"

namespace Werk {

	class BackgroundThread {

	public:
		BackgroundThread(long frequencyNs=100ul * 1000 * 1000) : 
		_frequencyNs(frequencyNs) {
			_thread = std::thread(&BackgroundThread::backgroundThread, this);
		}

		~BackgroundThread() { stop(); }

		void setFrequencyNs(long frequencyNs) { _frequencyNs = frequencyNs; }
		//to be called from main mostly
		void addTask(BackgroundTask* task) { _tasks.push_back(task);}

		void stop() {
			if(_running) {
				_running = false;
				_thread.join();
			}
		}

	private:
		timespec _delay;
		std::vector<BackgroundTask*> _tasks;
		volatile uint64_t _frequencyNs;
		volatile bool _running = true;
		std::thread _thread;

		void backgroundThread();
	};
}