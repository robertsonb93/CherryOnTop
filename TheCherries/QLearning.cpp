#include "stdafx.h"
#include "QLearning.h"



//constructor
 QLearning::QLearning(const vector<vector<double>> AA)
{
	availableActions = AA;
	alpha = 0.9;
	gamma = 0.9;
	defaultQ = 0.1;
};


 QLearning::QLearning(const vector<vector<double>> AA, const double alph, const double gam, const double defQ)
{
	alpha = alph;
	gamma = gam;
	defaultQ = defQ;
	availableActions = AA;
}


//destructor
 QLearning::~QLearning()//No Pointers Created
{}

//Returns a single value associated with the value of the given Action and state
 double QLearning::Value(const vector<double>& state, const vector<double>& action) 
 {
	 ensureExists(state);
	 return table.at(state).at(action);

 };


 //Takes the state vector and a vector of actions
 //returns a vector of values associated with each action at the state vector
vector<double> QLearning::Value(const vector<double>& state, const vector<vector<double>>& action)
{
	vector<double> response;
	response.resize(action.size()); //initilizing the vector to size
	ensureExists(state);

	map<vector<double>, double> actionMap = table.at(state);

	for (unsigned int i = 0; i < response.size(); i++)
		response[i] = actionMap[action[i]];//At the state, look up the action to get the Q-value

	return response;
};


//Updates the transition from an old state to a new state in the q-tables.
//the action,oldstate,newstate,reward are all in the transition object, accessed by get functions
//
double QLearning::Update(const StateTransition& transition)
{
	double cumRwrd = stats.GetCumulativeReward();
	stats.SetCumulativeReward(cumRwrd + transition.getReward());

	ensureExists(transition.getOldState());
	ensureExists(transition.getNewState());

	double q_s_a = Value(transition.getOldState(), transition.getAction());

	//table is the key'd by states with a map of actions(vector<double>) key'd each with a value, 
	map<vector<double>, double> actionMap = table.at(transition.getNewState());//at() safer due to throw if key DNE

	double maxNewQ,tempQ;
	maxNewQ = tempQ = actionMap[availableActions[0]];//Init to the first Q value.
	
													 //Find the maxQ value
	for each(vector<double> d in availableActions)
	{

		tempQ =	actionMap[d];
		if (tempQ > maxNewQ)
			maxNewQ = tempQ;
		
	}
	
	table[transition.getOldState()][transition.getAction()] = q_s_a + alpha * (transition.getReward() + gamma * maxNewQ - q_s_a);
	double newVal = table[transition.getOldState()][transition.getAction()];
	
	return abs(newVal - q_s_a);
};

PerformanceStats QLearning::GetStats() 
{
	return stats;
}

//Will take the given PerformanceStats and set the actionvalue stats to be the same
void QLearning::SetStats(PerformanceStats & PS)
{
	stats = PS;
};


//Given a state, will try table.at(state), this will throw an exception if the actionMap is non-existent
//On exception, will populate an actionMap and put it into the table.at(state)
//Else does nothing and continues
//CALL THIS FUNCTION BEFORE ACCESSING THE actionMap
void QLearning::ensureExists(const vector<double>& state)
{
	try
	{
		table.at(state);
	}
	catch (const out_of_range&)
	{
		map<vector<double>, double> actionMap;
		for each (vector<double> d in availableActions)
		{
			actionMap[d] = defaultQ;
		}
		table[state] = actionMap;
	}
};