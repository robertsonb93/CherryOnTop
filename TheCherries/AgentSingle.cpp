#include "stdafx.h"
#include "AgentSingle.h"



AgentSingle::AgentSingle()
{

}

//Note that the actionvalue* will be deleted by the base case.
//Note that the interpretor* will also be deleted by the base case.
AgentSingle::~AgentSingle()
{
	if(policy != nullptr)
		delete policy;
	policy = nullptr;
}

//Function will ask for the Q-Values from the current state, and possibleActions.
//Afterwards will give the values to the Policy to get a Single action.
//After receiving the Single action, the agent will give its state and action to the interpretor to perform the action in the world.
vector<double> AgentSingle::SelectAction()
{
	vector<double> values = actionValue->Value(state, possibleActions);
	//Use the policy to select an action from the values
	vector<double> empty;
	auto act = policy->selectAction(possibleActions, values, empty);

	this->LogEvent(interpretor->DoAction(act, state, actionValue->GetStats()));


	return act;

}

//function will give a the values alongside the possible actions, 
//the selected policy will then choose and return the "best" action
//Notice it is an actual value of action, not an index or iterator
//This is function differs from the one above for the sake of Policies that use parameters.
vector<double> AgentSingle::SelectAction(vector<double>& params)
{
	//get a QValue for every action
	vector<double> values = actionValue->Value(state, possibleActions);
	//Use the policy to select an action from the values
	auto act = policy->selectAction(possibleActions, values, params);
	this->LogEvent(interpretor->DoAction(act, state, actionValue->GetStats()));

	return act;
}

void AgentSingle::SetState(vector<double>& inputState)
{
	state = inputState;
}

vector<double> AgentSingle::GetState()
{
	return state;
}

//Function will perform the required update on the learner based off of the 
//given transition object
void AgentSingle::LogEvent(StateTransition transition)
{
	state = transition.getNewState();
	actionValue->Update(transition);
}

void AgentSingle::setActionValue(ActionValue* newAv)
{
	if (actionValue != nullptr)
	{
		delete actionValue;
	}
	actionValue = newAv;
}

void AgentSingle::setPolicy(PolicyBase* pol)
{
	if (policy != nullptr)
	{
		delete policy;
	}
	policy = pol;
}

 WorldBase* AgentSingle::setInterpretor(InterpretorBase* Intrptr)
{
	if (interpretor != nullptr)
	{
		delete interpretor;
	}
	possibleActions = Intrptr->GetAvailableActions();
	state = Intrptr->GetStartState();
	interpretor = Intrptr;
	return Intrptr->GetWorld();//So the interface can look at the world
}

PerformanceStats AgentSingle::GetStats()
{
	return actionValue->GetStats();
}

//Will take the given PerformanceStats object and copy it to the actionValue.PerformanceStats
void AgentSingle::SetStats(PerformanceStats& PS)
{
	actionValue->SetStats(PS);
}
