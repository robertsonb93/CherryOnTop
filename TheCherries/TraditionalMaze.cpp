
#include "stdafx.h"
#include "TraditionalMaze.h"
#include "AgentSingle.h"


//If numberOfActions is not 4 or 8 then it will default to 4
//4 represents the the 4 polar directions left,up,right,down
//8 represents the polars and the corners between, clockwise in representation
TraditionalMaze::TraditionalMaze(int numberOfActions )
	: numActions(numberOfActions)
{
	if (numActions != 4 && numActions != 8)
		numActions = 4;
	SetActions();
	startState.push_back(1);
	startState.push_back(1);

	if(agent != nullptr)
	delete agent;

	agent = new _DefaultAgentType(startState,nullptr,nullptr,availableActions);
	

	


	//Consider not setting the policy, actionvalue, agent in the constructor.

}

TraditionalMaze::~TraditionalMaze()
{
}

//Performs a step by the agent, 
//By-product, will also add any newly discovered states to the 
//Vector of previously vistedStates
//Can take an input param of a string in form x,y (no spaces)
//Which will define a custom move for the agent 

PerformanceStats TraditionalMaze::StepAgent(string userAction)
{
	vector<double> state = agent->GetState();
	SeenState(state);//Will auto-check and add a state if it has not yet been seen

	vector<double> action;
	if (userAction == "")
		action = agent->SelectAction();
	else 
	{
		//This world is only capable of having two commands y move and x move
		unsigned int pos = userAction.find(',');//This marks the end of the y command and the start x command
		if (pos == string::npos )//npos is a constant in string, this means the userAction is not formatted proper
			action = agent->SelectAction();
		else//Parse the string into action y and action x
		{
			//stod is string to double
			action.push_back(stod(userAction.substr(0, pos))); //grabs the first number, stops at the comma
			action.push_back(stod(userAction.substr(pos + 1)));//Will read the number after the comma untill the end
		}
	}
	vector<double> newState;
	double reward = 0;
	bool absorbingStateReached = false;

	//Get the type of the new location, ie, wall lava blank goal...
	 stateTypes newStateType = (stateTypes)(int)mapBMP[(int)state[0] + (int)action[0]][(int)state[1] + (int)action[1]];

	 //stateTypes is an enumeration 
	switch (newStateType)
	{
		case openSpace: //Open space
		{
			newState.push_back(state[0] + action[0]);//Push the new y and then the new x
			newState.push_back(state[1] + action[1]);
			reward = openSpaceRwrd;
			break;
		}
		case wall:
		{
			newState = state; // the state does not change
			reward = wallRwrd;
			break;
		}
		case lava: 
		{
			newState.push_back(state[0] + action[0]);
			newState.push_back(state[1] + action[1]);
			reward = lavaRwrd;
			break;
		}
		case goal:
		{
			newState = startState;
			reward = goalRwrd;
			absorbingStateReached = true;
			break;
		}
		default: //There was an issue with the map reading, the map has been made wrong
		{
			//Consider debug code in here
			throw invalid_argument("unhandled/invalid enumerated stateType in TheCherries::TradMaze::StepAgent");
		}
	}//End Switch statement.
	////consider one line on the set sats instead of copy to a new perfstats object
	PerformanceStats tempPS(agent->GetStats());
	tempPS.TallyStepsToGoal(reward == goalRwrd);
	agent->SetStats(tempPS);

	StateTransition st(state, newState, action, reward, absorbingStateReached);
		agent->LogEvent(st);

	return agent->GetStats();
}

//The Agent is not to be touched outside of TradMaze's control, thus no return
//pol and AV must be defined by the top level e.g Form, then given to this function
//Will create a new instance of agent and point towards it
//Will delete the old agent if there is one
void TraditionalMaze::AddAgent(PolicyBase* pol,ActionValue* AV)
{
	if(agent != nullptr)
	delete agent;

	//Start state is defined by the load function, and available actions is defined by the constructor
	agent = new AgentSingle(startState, pol, AV, availableActions);
}

//Need to show that states have been visited, and where the agent is currently located
//The agent location will be makred as a agentLoc in the map,
//
GridWorldBase::mazeType TraditionalMaze::ShowState()
{
	mazeType outMap = mapBMP;

	//Loop will mark visited areas in the map
	for each (vector<double> state in visitedStates)
	{
		int y = (int)state[0];
		int x = (int)state[1];
		switch ((int)outMap[y][x])
		{
		case openSpace: outMap[y][x] = visitedOpenSpace; break;
		case lava: outMap[y][x] = visitedLava; break;
		case goal: outMap[y][x] = visitedGoal; break;
		}
	}
	
	//Mark the location of the agent
	vector<double> agentState = agent->GetState();
	outMap[(int)agentState[0]][(int)agentState[1]] = agentLocation;//Note that agentState is of doubles, we have double -> int

	return outMap;
}


//Sets up the MapBMP inherited from GridworldBase
//The Form will have to provide each position, and the value for it(from enumerated types)
void TraditionalMaze::Load(mazeType inMap, vector<double> start)
{
	mapBMP = inMap;
	//int width = inMap.size();
	//int height = inMap[0].size();
	startState = start;
	mapBMP[(int)start[0]][(int)start[1]] = openSpace;
	//Notice marking the starting position as white with the map that is kept by TradMaze

	visitedStates.clear();

	agent->SetState(startState);
}

//If it is decided that any of the goals should be replaced
//Then this function will change all the current rewards for this world.
void TraditionalMaze::ChangeRwrds(double newOpen, double newWall, double newLava, double newGoal)
{
	openSpaceRwrd = newOpen;
	wallRwrd = newWall;
	lavaRwrd = newLava;
	goalRwrd = newGoal;
}

int TraditionalMaze::GetMap(int x, int y)
{
	return (int)mapBMP[x][y];
}

//Function that defines what the available actions will be, if the constructor has numactions set to anything
//besides 8, it will default to 4
void TraditionalMaze::SetActions()
{
	if (numActions == 4)
	{
		availableActions.resize(4);
		availableActions[0] = { 0, -1 };//Left
		availableActions[1] = { -1, 0 };//Up
		availableActions[2] = { 0, 1 };//Right
		availableActions[3] = { 1, 0 };//Down
	}
	if (numActions == 8)
	{
		availableActions.resize(8);

		availableActions[0] = { 0, -1 };//Left
		availableActions[1]=  { -1,-1 };//Left UP,
		availableActions[2] = { -1, 0 };//Up
		availableActions[3] = { -1,1 };//Right UP,
		availableActions[4] = { 0, 1 };//Right
		availableActions[5] = { 1,1 };//Right Down
		availableActions[6] = { 1, 0 };//Down
		availableActions[7] = { 1,-1 };//Left Down,
	}
}

vector<vector<double>> TraditionalMaze::GetMap()
{
	return mapBMP;
}

vector<vector<double>> TraditionalMaze::GetVisitedStates()
{
	return visitedStates;
}

vector<double> TraditionalMaze::GetStartState()
{
	return startState;
}

//Checks if a state has been previously visted by the agent. 
//If the agent has not previously seen, will push the state onto the 
//vector of visitedstates
void TraditionalMaze::SeenState(vector<double> state)
{
	bool seenBefore = false;
	for each  (vector<double> seenState in visitedStates)
	{
		if (seenState == state)
		{
			seenBefore = true;
			break;
		}
	}
	if (!seenBefore)
	{
		visitedStates.push_back(state);
	}
}

vector<vector<double>> TraditionalMaze::GetAvailableActions()
{
	return availableActions;
}

