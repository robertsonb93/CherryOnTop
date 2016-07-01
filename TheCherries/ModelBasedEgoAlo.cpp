#include "stdafx.h"
#include "ModelBasedEgoAlo.h"


ModelBasedEgoAlo::ModelBasedEgoAlo(const vector<vector<double>>& AvailActions, const vector<double>& StartState, const int EgoSize)
	:egoSize(EgoSize), availableActions(AvailActions), startState(StartState), aloLearner(AvailActions, StartState),
	rewardPredictionModel(ModelBasedLearning(AvailActions, StartState, defQ, 0, maxUps))
{
	//Properly size the aloFeaturePrediction vector, we are creating a model for each feature of the alo state space. Plus one of the reward prediction
	//Notice the gamma set to 0
	for (int i = egoSize; i < startState.size(); i++)
		aloFeaturePredictionModels.push_back(ModelBasedLearning(AvailActions, StartState, defQ, 0, maxUps));
	visitedStates.resize(AvailActions.size());
	

};

ModelBasedEgoAlo::ModelBasedEgoAlo(const vector<vector<double>>& AvailActions, const vector<double>& StartState, const int EgoSize, double DefQ, double gam, int MaxUps)
	: egoSize(EgoSize), availableActions(AvailActions), startState(StartState), aloLearner(AvailActions, StartState, DefQ, gam, maxUps),
	rewardPredictionModel(ModelBasedLearning(AvailActions,StartState,defQ,0,maxUps)) , gamma(gam), defQ(DefQ), maxUps(MaxUps)
{
	//Properly size the aloFeaturePrediction vector
	//Notice the gamma set to 0
	for (int i = egoSize; i < startState.size(); i++)
		aloFeaturePredictionModels.push_back(ModelBasedLearning(AvailActions, StartState, defQ, 0, maxUps));
	visitedStates.resize(AvailActions.size());
};

ModelBasedEgoAlo::~ModelBasedEgoAlo()
{
}

//Predict the Next ALOCENTRIC State, essentially parses what the ModelBasedEgoAlo class has for ego size
// uses the EgoSize to parse what the alocentric state is, then continues to give the aloCentric to the 
// aloLearner and get what it predicts the nextState to be 
ModelBasedEgoAlo::stateType ModelBasedEgoAlo::PredictNextState(stateType state, actionType action)
{
	if (state.size() < egoSize)
		throw("Invalid state Handed to ModelBasedEgoAlo::PredictNextState, State.size() < NumEgoFeatures, ");
	stateType aloState;
	for (int i = egoSize; i < state.size(); i++)
	{
		aloState[i-egoSize] = state[i];
	}
	return aloLearner.PredictNextState(aloState, action);
}

//Predict the Next ALOCENTRIC State, essentially parses what the ModelBasedEgoAlo class has for ego size
// uses the EgoSize to parse what the alocentric state is, then continues to give the aloCentric to the 
// aloLearner and get what it predicts the next States are
map<ModelBasedEgoAlo::stateType, double> ModelBasedEgoAlo::PredictNextStates(stateType state, actionType action)
{
	if (state.size() < egoSize)
		throw("Invalid state Handed to ModelBasedEgoAlo::PredictNextStates, State.size() < NumEgoFeatures, ");
	stateType aloState;
	for (int i = egoSize; i < state.size(); i++)
	{
		aloState[i - egoSize] = state[i];
	}
	return aloLearner.PredictNextStates(state, action);
}

//Predict the next reward from the alocentric learner, will use what the ModelBasedEgoAlo knows the egosize to be
//and parse the state into the alocentric and egocentric information.
//Then hands the alocentric state to the alocentric learner and returns what it predicts the next reward to be.
double ModelBasedEgoAlo::PredictReward(stateType state, actionType action, stateType newState)
{
	if (state.size() < egoSize)
		throw("Invalid state Handed to ModelBasedEgoAlo::PredictNextState, State.size() < NumEgoFeatures, ");
	if(newState.size() < egoSize)
		throw("Invalid state Handed to ModelBasedEgoAlo::PredictNextState, newState.size() < NumEgoFeatures, ");

	stateType aloState, newAloState;
	for (int i = egoSize; i < state.size(); i++)
	{
		aloState[i - egoSize] = state[i];
		newAloState[i - egoSize] = newState[i];
	}
	return aloLearner.PredictReward(aloState, action, newAloState);
}


double ModelBasedEgoAlo::Update(const StateTransition & transition)
{
	steps++;
	auto oldState= transition.getOldState();
	auto newState = transition.getNewState();

		const auto egoOldBegin = oldState.begin();
		const auto egoNewBegin = newState.begin();
		const auto aloOldBegin = egoOldBegin + egoSize;
		const auto aloNewBegin = egoNewBegin + egoSize;

	//Firstly we will get the respective ego and alo state from the transition which contains the state(ego&alo)
	//Will use iterators to use a copy constructor for each of the different states.
	stateType egoOldState(egoOldBegin, aloOldBegin);//The copy goes up to but not including the last iterator. 
	stateType egoNewState(egoNewBegin, aloNewBegin);
	stateType alloOldState(aloOldBegin,oldState.end());
	stateType alloNewState(aloNewBegin, newState.end());

	updatePredictionModels(egoOldState,transition.getAction(),alloOldState,alloNewState,transition.getReward());
	aloLearner.Update(StateTransition(alloOldState, alloNewState, transition.getAction(), transition.getReward()));
	
	//TODO: Can we make this faster? Consider looking into some iterators for look ups we do multiple times
	for (int i = 0; i < availableActions.size(); i++)
	{
		auto visitedStateIter = visitedStates[i].second.emplace(alloNewState, 0).first;
		visitedStateIter->second++; //Regardless of if we have seen before or not, we will increment, so we mark 1, or if not emplaced then whatever was in place.

		if (steps >= minStepsRequired && visitedStateIter->second <= updateTerminationStepCount && egoSize)//Seen enough for worthy update, but not to much for it to be not needed
		{			
			auto currAct = availableActions[i];
			double predReward(rewardPredictionModel.Value(egoNewState, currAct));
			stateType predictedAlo(alloNewState);
			for (int j = 0; j < aloFeaturePredictionModels.size(); j++)
				predictedAlo[j] += aloFeaturePredictionModels[j].Value(egoNewState, currAct);//Get all the predicted changes for each alocentric feature
			
			aloLearner.Update(StateTransition(alloNewState, predictedAlo,currAct, predReward));//TODO: consider modifying the stats of aloLearner, and remove these updates.
		}
	}
	return 0.0;
}

double ModelBasedEgoAlo::Value(const vector<double>& aloState, const vector<double>& action)
{
	return aloLearner.Value(aloState, action);
}

vector<double> ModelBasedEgoAlo::Value(const vector<double>& aloState, const vector<vector<double>>& actions)
{
	return aloLearner.Value(aloState, actions);
}

void ModelBasedEgoAlo::ResetAllocentric()
{
	aloLearner = ModelBasedLearning(availableActions, startState, defQ, gamma, maxUps);
}

PerformanceStats & ModelBasedEgoAlo::GetStats()
{
	return aloLearner.GetStats();
}

void ModelBasedEgoAlo::SetStats(PerformanceStats & PS)
{
	aloLearner.SetStats(PS);
}

void ModelBasedEgoAlo::ResetStats()
{
	aloLearner.ResetStats();
}


//Given the Parameters, will run an update on each of aloFeaturePrediction models so that they may give accurate predictions on how alocentric features with change
//We will give the each learner its own feature from the alo, and it can reference the egoState, to start learning  what the relationship between egoState
//And the change in Alostate feature will be.
inline void ModelBasedEgoAlo::updatePredictionModels(const stateType& oldEgo, const actionType& act, const stateType newAlo, const stateType& oldAlo, const double rew)
{
	if (egoSize)
	{
		stateType dummyNewState = { -1 };
		for (int i = 0; i < aloFeaturePredictionModels.size(); i++)
		{
			//It doesn't need to learn of an S' as there is no continuity on the ego side of things, so we give a dummy state. 
			//The reward is what it is predicting, but reward is actually the feature change.
			aloFeaturePredictionModels[i].Update(StateTransition(oldEgo, dummyNewState, act, (newAlo[i] - oldAlo[i])));
		}
		rewardPredictionModel.Update(StateTransition(oldEgo, dummyNewState, act, rew));
	}


}

