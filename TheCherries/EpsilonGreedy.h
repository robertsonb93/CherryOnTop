#pragma once
#ifndef _EPSILONGREEDY_H
#define _EPSILONGREEDY_H

#include "stdafx.h"
#include "PolicyBase.h"
//#include <random>
//#include <time.h>

#if defined _EXPORT
#define  _declspec(export)
#else
#define  _declspec(import)
#endif

extern "C"
{
	//E-Greedy Policy class derived from PolicyBase
	//Uses a default Epsilon of 0.8 unless otherwise set in the constructor or setEpsilon
	using namespace std;
	class  EpsilonGreedy : public PolicyBase
	{
	public:
		EpsilonGreedy();
		EpsilonGreedy(double defE);
		~EpsilonGreedy();

	    std::vector<double> selectAction(std::vector<std::vector<double>>& availableActions, std::vector<double>& values, std::vector<double>& params);
		bool setEpsilon(double newEps);

		//Members//
	private:
		double defaultE = 0.8;
	};
}
#endif