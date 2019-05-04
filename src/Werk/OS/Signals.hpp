#pragma once

namespace Werk {

	//forward declaration
	class Action;

	/*
	sets handler for SIGSEGV and  SIGBUS
	*/
	bool setupSegfaultHandler();

	/*
	SETS UP A SIGNAL HANDLER FOR A GIVEN HANDLER THAT 
	EXECUTES A GIVEN ACTION
	*/
	bool setupSignalHandler(int signal, Action* action);
}