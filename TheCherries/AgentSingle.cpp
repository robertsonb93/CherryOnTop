#include "stdafx.h"
#include "AgentSingle.h"


//THis constructor is not safe/complete, will need a definition of possActions and of state to work.
AgentSingle::AgentSingle()
{
	if (policy != nullptr)
		delete policy;
	if (actionValue != nullptr)
		delete actionValue;

	policy = new _DefaultPolicyType();
	//actionValue = new _DefaultAVType();

}

//Must be given a state, to mark where the agent currently is, or its starting state
//Pol can be a nullptr, if so it will default
//Av can be a nullptr, if so it will default with the possActions
//possActions is required for constructing the AV
AgentSingle::AgentSingle(const vector<double>& State, PolicyBase *pol, ActionValue *AV,const vector<vector<double>>& possActions)
	: state(State)
{
	possibleActions = possActions;
	if (pol != nullptr)
		policy = pol; //pol and policy will point to the same instance of PolicyBase now
	else policy = new _DefaultPolicyType();
	if(AV != nullptr)
	actionValue = AV; //actionValue and AV should be pointing at the same instance of the ActionValue
						//If the ActionValue gets deleted or lost, then actionValue will be lost
	else actionValue = new _DefaultAVType(possActions);
}

//Note that the actionvalue* will be deleted by the base case.
AgentSingle::~AgentSingle()
{
	if(policy != nullptr)
		delete policy;
	policy = nullptr;
}

//function will give a the values alongside the possible actions, 
//the selected policy will then choose and return the "best" action
//Notice it is an actual value of action, not an index or iterator
vector<double> AgentSingle::SelectAction()
{
	//get a QValue for every action

	vector<double> values = actionValue->Value(state, possibleActions);
	//Use the policy to select an action from the values
	vector<double> emptyParam;

	return policy->selectAction(possibleActions, values,emptyParam);
}

//function will give a the values alongside the possible actions, 
//the selected policy will then choose and return the "best" action
//Notice it is an actual value of action, not an index or iterator
vector<double> AgentSingle::SelectAction(vector<double>& params)
{
	//get a QValue for every action
	vector<double> values = actionValue->Value(state, possibleActions);
	//Use the policy to select an action from the values
	return policy->selectAction(possibleActions, values, params);
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

PerformanceStats AgentSingle::GetStats()
{
	
	return actionValue->GetStats();
}

//Will take the given PerformanceStats object and copy it to the actionValue.PerformanceStats
void AgentSingle::SetStats(PerformanceStats& PS)
{
	actionValue->SetStats(PS);
}
