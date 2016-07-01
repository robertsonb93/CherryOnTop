#pragma once
#ifndef MODELBASEDEGOALO_H
#define MODELBASEDEGOALO_H

#include "stdafx.h"
#include "QLearning.h"
#include "ModelBasedLearning.h"
#include "ModelBasedBase.h"

class ModelBasedEgoAlo :
	public ModelBasedBase
{
private:

	typedef vector<double>  stateType;
	typedef vector<double> actionType;
	typedef unordered_map<stateType, map<actionType, map<stateType, pair<double, double>>>>  SASTable;
	typedef unordered_map<stateType, map<actionType, double>> QTableType;

public:
	

	ModelBasedEgoAlo(const vector<vector<double>>& AvailActions, const vector<double>& StartState,const int NumEgoFeatures);
	ModelBasedEgoAlo(const vector<vector<double>>& AvailActions, const vector<double>& StartState, const int NumEgoFeatures, double DefQ, double gam, int maxUps);
	~ModelBasedEgoAlo();


	
	stateType PredictNextState(stateType state, actionType action);
	map<stateType, double> PredictNextStates(stateType state, actionType action);
	double PredictReward(stateType state, actionType action, stateType newState);

	//Brunt of the Learner, Used for getting actions and making updates to/from the learners
	double Update(const StateTransition & transition);
	double Value(const vector<double>& state, const vector<double>& action);
	vector<double> Value(const vector<double>& state, const vector<vector<double>>& actions);
	void ResetAllocentric();

	//Get/Set the Performance Stats
	PerformanceStats& GetStats();
	void SetStats(PerformanceStats& PS);
	void ResetStats();

private:

	void updatePredictionModels(const stateType& oldEgo,const actionType& act,const stateType newAlo,const stateType& oldAlo,const double rew);

	//****MEMBERS****//
	ModelBasedLearning aloLearner,rewardPredictionModel;
	vector<ModelBasedLearning> aloFeaturePredictionModels; //This is the Ego part of the learner, each model uses .value to return its prediction on what change in feature is

	//Kept for re-setting the model-Based(alocentric) learner
	vector<vector<double>> availableActions;
	vector<double> startState; 
	
	vector< pair<actionType, unordered_map<stateType, int>>> visitedStates; //Keep track #-times an action has led to an alo-state. 
	PerformanceStats perfStats;

	int updateTerminationStepCount = 1;
	const int egoSize;
	double gamma = 0.9, defQ = 10, maxUps = 120;
	int steps = 0,minStepsRequired = 10;//How many steps this class has seen taken, (number of times that update has been called)



	

};

#endif