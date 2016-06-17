#pragma once
#ifndef _WORLDBASE_H
#define _WORLDBASE_H

//Will be the abstract Base class for the worlds, Abstract base class is used as a type of interface
//Declaring a function as:
//  virtual func() = 0; 
//Makes an abstract base class of WorldBase, and furthermore, forces derived classed to define func(), and is
//now called a pure virtual function
#include "stdafx.h"
//#include <string>
//
#include "PerformanceStats.h"
#include "AgentBase.h"


	using namespace std;
	class  WorldBase abstract
	{
	public:

		WorldBase() {};
		virtual ~WorldBase() = 0 { delete agent; };
		virtual PerformanceStats StepAgent(string userAction) = 0;
		virtual void AddAgent(PolicyBase* pol, ActionValue* AV) = 0;
		//virtual void Load(string filename) = 0;


		//Members
	protected:
		AgentBase* agent = nullptr; //Every world has to have an agent

	};


#endif