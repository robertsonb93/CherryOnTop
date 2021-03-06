#pragma once
#ifndef _QLEARNING_H
#define _QLEARNING_H

#include "stdafx.h"
#include "ModelFreeBase.h"

//Class for the QLearning, derived class from ModelFreeBase which is Derived from ActionValue
//
	using namespace std;
	class QLearning : public ModelFreeBase
	{
	public:
		 QLearning(const vector<vector<double>> AA);
		 QLearning(const vector<vector<double>> AA, const double alph, const double gam, const double defQ);
		 ~QLearning();

		 double Value(const vector<double>& state, const vector<double>& action);
		 vector<double> Value(const vector<double>& state, const vector<vector<double>>& actions);
		 double Update(const StateTransition & transition);
		 PerformanceStats& GetStats();
		 void SetStats(PerformanceStats& PS);


		//Members//
	private:
		double alpha;
		double gamma;
		double defaultQ;
		//First element is the statevector, this returns a map that given an action returns the q-value
		unordered_map<vector<double>, map<vector<double>, double>> table;
		vector<vector<double>> availableActions;
		PerformanceStats stats;
		map<vector<double>, double> defaultMap;



	};

#endif