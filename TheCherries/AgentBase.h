#pragma once
#ifndef _AGENTBASE_H
#define _AGENTBASE_H
//Will function as the base class for the agent system.
//At the time of writing, the rational was that the agent system will need to be the communicator between the learning 
//algorithms and the world. 
//the derived agents are thought to be able to split into 2 groups, single agent class or multi-agent class
#include "stdafx.h"
#include "PerformanceStats.h"
#include "StateTransition.h"
#include "ActionValue.h"
#include "PolicyBase.h"


#if defined _EXPORT
#define  _declspec(export)
#else
#define  _declspec(import)
#endif

	//Base class for agents for polymorphism
	class  AgentBase abstract
	{
	public:
		 AgentBase() {};
		 virtual ~AgentBase() = 0 { delete actionValue; actionValue = nullptr; };

		 virtual std::vector<double> SelectAction() = 0;
		 virtual std::vector<double> SelectAction(vector<double>& params) = 0;//For use with policies that have parameters
		 virtual void LogEvent(StateTransition transition) = 0;
		 virtual PerformanceStats GetStats() = 0;
		 virtual void SetStats(PerformanceStats& PS) = 0;
		 virtual void SetState(vector<double>& inputState) = 0;
		 virtual vector<double> GetState() = 0;

		//Members//
	protected:
		//Having these members will force derived classes to have these members, but not tell them what to do with them.
		ActionValue* actionValue = nullptr;
		std::vector<std::vector<double>> possibleActions;



	};

#endif