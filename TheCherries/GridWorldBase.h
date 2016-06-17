#pragma once
#ifndef _GRIDWORLDBASE_H
#define  _GRIDWORLDBASE_H


#include "stdafx.h"
#include "WorldBase.h"
#include "AgentSingle.h"
//#include <easyBMP.h>
//#include <easyBMP_BMP.h>


//Base class for any worlds that will use a gridstyle map, often maze navigation.
//The world in these classes will have to have a bitmap that represents the map


	class  GridWorldBase abstract : public WorldBase
	{
	public:
		typedef vector<vector<double>> mazeType;

		enum stateColors{ White,Blue,Red,Green,Black,LessWhite,LessBlue,LessRed,LessGreen,LessBlack};
		 GridWorldBase()
		{}
		 virtual ~GridWorldBase()
		{}

		 virtual mazeType ShowState() = 0;

		 virtual int GetMap(int x, int y) =0;

		 virtual int GetHeight() { return mapBMP[0].size(); };
		 virtual int GetWidth() { return mapBMP.size(); };
		 
		 virtual void Load(mazeType inMap, vector<double> start) = 0;
		 virtual void AddAgent(PolicyBase* pol, ActionValue* AV) = 0;
		 virtual PerformanceStats StepAgent(string userAction = "") = 0;

		virtual vector<vector<double>> GetVisitedStates() =0;
		virtual vector<vector<double>> GetAvailableActions()=0;
		virtual vector<double> GetStartState() =0;

	protected:
		//BMP mapBMP;
		mazeType mapBMP;

	};
#endif

