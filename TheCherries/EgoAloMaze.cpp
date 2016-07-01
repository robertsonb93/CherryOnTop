#include "stdafx.h"
#include "EgoAloMaze.h"


//If numberOfActions is not 4 or 8 then it will default to 4
//4 represents the the 4 polar directions left,up,right,down
//8 represents the polars and the corners between, clockwise in representation
EgoAloMaze::EgoAloMaze(int numActions )
	: numActions(numActions)
{
	
		if (numActions != 4 && numActions != 8)
			numActions = 4;
		SetActions();
		int egoSize = (2 * viewRange + 1) * (2 * viewRange + 1);
		startState.assign(egoSize, 0);
		startState.push_back(1);
		startState.push_back(1);
}


EgoAloMaze::~EgoAloMaze()
{
}


//Need to show that states have been visited, and where the agent is currently located
//The agent location will be marked as a agentLoc in the map,
GridWorldBase::mazeType EgoAloMaze::ShowState()
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

		for each (vector<double> agentLoc in currLocations)
			outMap[(int)agentLoc[0]][(int)agentLoc[1]] = agentLocation;
	}

	return outMap;
}



//Sets up the MapBMP inherited from GridworldBase
//The Form will have to provide each position, and the value for it(from enumerated types)
//Will have to make sure the Agent loads the start state from startState after this point.
void EgoAloMaze::Load(mazeType inMap, vector<double> start)
{
	mapBMP = inMap;
	startState = GetEgo(start[0], start[1]);
	for each(auto x in start)
		startState.push_back(x);
	
	mapBMP[(int)start[0]][(int)start[1]] = openSpace;//Notice marking the starting position as white with the map that is kept by TradMaze

	currLocations.push_back(vector<double> {start[0], start[1]});//We are pushing the the starting location as the agents currlocation

	visitedStates.clear();

}


// If it is decided that any of the goals should be replaced
//Then this function will change all the current rewards for this world.
void EgoAloMaze::ChangeRwrds(double newOpen, double newWall, double newLava, double newGoal)
{
	openSpaceRwrd = newOpen;
	wallRwrd = newWall;
	lavaRwrd = newLava;
	goalRwrd = newGoal;
}

int EgoAloMaze::GetMap(int x, int y)
{
	return (int)mapBMP[x][y];
}

GridWorldBase::mazeType EgoAloMaze::GetMap()
{
	return mapBMP;
}

//Ego Alo learners are organized such that The Ego Comes first.
//The state is from a transition object, thus contains both ego an alo information

StateTransition EgoAloMaze::PerformStep(const vector<double>& Action, const vector<double>& state, PerformanceStats & perfStats)
{
	int numEgoStates = ((2 * viewRange) + 1) * ((2 * viewRange) + 1);
	if (state.size() < numEgoStates)
		numEgoStates = 0;
	vector<double> Alostate(state.begin() + numEgoStates, state.end());//This gives the alocentric state, based on number of states in 


	SeenState(Alostate);//Will auto-check and add a state if it has not yet been seen

	int newY, newX;

	vector<double> newState;
	double reward = 0;
	bool absorbingStateReached = false;

	//Get the type of the new location, i.e, wall, lava, blank, goal...
	stateTypes newStateType = (stateTypes)(int)mapBMP[(int)Alostate[0] + (int)Action[0]][(int)Alostate[1] + (int)Action[1]];

	switch (newStateType)
	{
	case openSpace: //Open space
	{
		newY = Alostate[0] + Action[0];
		newX = Alostate[1] + Action[1];
		vector<double> egoVect = GetEgo(newY, newX);
		newState.assign(egoVect.begin(),egoVect.end());
		//Now assign the Alocentric
		newState.push_back(newY);
		newState.push_back(newX);
		reward = openSpaceRwrd;
		break;
	}
	case wall:
	{
		vector<double> egoVect = GetEgo(Alostate[0], Alostate[1]);
		newState.assign(egoVect.begin(), egoVect.end());
		for each (auto x in Alostate)
		newState.push_back(x) ; // the state does not change
		reward = wallRwrd;
		break;
	}
	case lava:
	{
		newY = Alostate[0] + Action[0];
		newX = Alostate[1] + Action[1];
		vector<double> egoVect = GetEgo(newY, newX);
		newState.assign(egoVect.begin(), egoVect.end());

		newState.push_back(newY);
		newState.push_back(newX);
		reward = lavaRwrd;
		break;
	}
	case goal:
	{
		vector<double> egoVect = GetEgo(startState[0], startState[1]);
		newState.assign(egoVect.begin(), egoVect.end());
		for each(auto x in startState)
			newState.push_back(x);
		reward = goalRwrd;
		absorbingStateReached = true;
		break;
	}
	default: throw invalid_argument("unhandled/invalid enumerated stateType in TheCherries::EgoAloMaze::PerformStep");
	}//End Switch statement.


	 //Update where we currently see the agent(s)
	bool newStateSeen = true;

	for (int i = 0; i < currLocations.size(); i++)
	{
		if (currLocations[i] == Alostate)
		{
			currLocations[i][0] = newState[0];
			currLocations[i][1] = newState[1];
			newStateSeen = false;
		}
	}

	if (newStateSeen)
		currLocations.push_back(newState);//We are seeing a new agent, possibly one that was overlapped onto another. 
	
	//Lastly we need the oldEgo state to put it into the transition.
	vector<double> oldstate = GetEgo(Alostate[0], Alostate[1]);
	oldstate.push_back(Alostate[0]);
	oldstate.push_back(Alostate[1]);

	perfStats.TallyStepsToGoal(reward == goalRwrd);//Update the performance Stats
	return StateTransition(oldstate, newState, Action, reward, absorbingStateReached);//Return a new transition
}

vector<vector<double>> EgoAloMaze::GetVisitedStates()
{
	return visitedStates;
}

vector<vector<double>> EgoAloMaze::GetAvailableActions()
{
	return availableActions;

}

//Needs to return the start state, but with the appropriate EgoInformation as well
vector<double> EgoAloMaze::GetStartState()
{
	int egoSize = (2 * viewRange + 1)*(2 * viewRange + 1);
	vector<double> ret(egoSize, 0);
	ret.push_back(startState[0]);
	ret.push_back(startState[1]);

	return ret;
}

inline vector<double> EgoAloMaze::GetEgo(int y, int x)
{
	//xS is (x Shift)
	vector<double> ego;
	if (mapBMP.size()) {
		
		for (int yS = -viewRange; yS <= viewRange; yS++)
		{
			for (int xS = -viewRange; xS <= viewRange; xS++)
			{
				ego.push_back(mapBMP[y + yS][x + xS]);
			}
		}
	}

		return ego;
	
}

//Sets up the proper actions for the agent to navigate the gridworld
void EgoAloMaze::SetActions()
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
		availableActions[1] = { -1,-1 };//Left UP,
		availableActions[2] = { -1, 0 };//Up
		availableActions[3] = { -1,1 };//Right UP,
		availableActions[4] = { 0, 1 };//Right
		availableActions[5] = { 1,1 };//Right Down
		availableActions[6] = { 1, 0 };//Down
		availableActions[7] = { 1,-1 };//Left Down,
	}
}

//Checks if a state has been previously visited by the agent. 
//If the agent has not previously seen, will push the state onto the 
//vector of visited states
void EgoAloMaze::SeenState(vector<double> state)
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

