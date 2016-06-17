#pragma once
#ifndef _AGENTSINGLE_H
#define _AGENTSINGLE_H

#include "stdafx.h"
#include "AgentBase.h"
#include "QLearning.h"
#include "PolicyHeaders.h"

	

typedef OptimalPolicy _DefaultPolicyType;
typedef QLearning _DefaultAVType;

	//Derived class for creating a single Agent in the world
	//
	class  AgentSingle : public AgentBase
	{
	public:
		 AgentSingle();
		 AgentSingle(const vector<double>& State, PolicyBase *pol, ActionValue *AV,const vector<vector<double>>& possActions);
		 ~AgentSingle();

		 vector<double> SelectAction();
		 vector<double> SelectAction(vector<double>& params);

		 void SetState(vector<double>& inputState);
		 vector<double> GetState();

		 void LogEvent(StateTransition transition);
		 PerformanceStats GetStats();
		 void SetStats(PerformanceStats& PS);

	private:
		vector<double> state;//Where is the agent currently located
		PolicyBase* policy = nullptr;


		//Notice, since maps are in terms of h*w which is y*x so the moves are also in y*x, thus we have, left, up, right, down;
		//Understand that the folliwng are owned by the base class Agent.cpp
		//ActionValue* actionValue;
		//std::vector<vector<double>> possibleActions;
	};

#endif