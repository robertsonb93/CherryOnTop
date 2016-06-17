#include "stdafx.h"
#include "EpsilonGreedy.h"



EpsilonGreedy::EpsilonGreedy()
{
	srand(static_cast<unsigned int> (time(NULL)));
}

EpsilonGreedy::EpsilonGreedy(double defE)
{
	defaultE = defE;
	srand ( static_cast<unsigned int> (time(NULL)) );

}


EpsilonGreedy::~EpsilonGreedy()
{
}

//Has a random chance of randomly selecting an action, 
//Otherwise selects the optimal action
std::vector<double> EpsilonGreedy::selectAction(std::vector<std::vector<double>>& availableActions, std::vector<double>& values, std::vector<double>& params)
{
	double randVal = (rand() % 100); // put the value between 0-1
	
	if (randVal < defaultE)//If the value is below epsilon, then use the optimal action
	{
		vector<double> bestAction = availableActions[0];
		double expectedReward = values[0];
		for (unsigned int i = 0; i < values.size(); i++)
		{
			if (values[i] > expectedReward)
			{
				expectedReward = values[i];
				bestAction = availableActions[i];
			}
		}
		return bestAction;
	}
	else //Randomly select a value
	{
		randVal = (rand() % availableActions.size());//Create a random index value
		if (randVal < 0 || randVal >= availableActions.size())
			throw logic_error::exception("Not actually Logic_Error but IDC, PolicyBase::EpsilonGreedy::SelectAction::randVal is not being modified properly");

		return availableActions[(unsigned int)randVal];
	}
}

//returns True if the value was changed to the specific parameter
//Return false if the value given was not accepted, sets epsilon to 0.8
bool EpsilonGreedy::setEpsilon(double newEps)
{
	if (newEps < 1 && newEps >= 0)
	{
		defaultE = newEps;
		return true;
	}

	defaultE = 0.8;
	return false;
}
