#pragma once
#ifndef _POLICYBASE_H
#define _POLICYBASE_H

#include "stdafx.h"
//#include <vector>

#if defined _EXPORT
#define  _declspec(export)
#else
#define  _declspec(import)
#endif

extern "C"
{
	//Base class for the policy derivatives such as e-Greedy or softmax
	class  PolicyBase abstract
	{
	public:

		PolicyBase()
		{}

		virtual ~PolicyBase()
		{}

		virtual std::vector<double> selectAction(std::vector<std::vector<double>>& availableActions, std::vector<double>& values, std::vector<double>& params) = 0;

	};
}
#endif
