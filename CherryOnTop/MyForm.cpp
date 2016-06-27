#include "MyForm.h"


//Create a pop-up menu for selecting the desired image,
//filters to only allow bmp images
//check if the image member of our picturebox1 is pointing to anything, 
//If it is then we need to free the memory it points to, (  delete )
//then set the pointer to null so it doesnt refer to the disposed of location (not necessary but good form)
//now we can set the image to point to a picture
//Lastly copy the elements from the bmap to the 2d-vector, using enum values from gridworldbase
void CherryOnTop::MyForm::loadMapButton_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	
	OpenFileDialog of;
	of.Title = "Select Map to Load";
	of.ShowDialog();
	of.Filter = "*.bmp | bmp";

	if (System::IO::File::Exists(of.FileName))
	{
		activeFile.FileName = of.FileName;//So we can reload the world with the same map in the load world func.
		LoadMapFromFileName(of.FileName);
		runButton->Enabled = true;
		mapLoaded = true;
	}//Endif file exists
	else
	{
		throw("MyForm.cpp::Load::file does not exist, How did you manage to select an non-existent file, stop trying to use computer.");
	}
}

//Is used when the Run button is clicked, mostly to help clean up the code and make the runButtonClicked function smaller 
inline void CherryOnTop::MyForm::RunSteps(PerformanceStats& tempStats, int& iteration, System::Diagnostics::Stopwatch^ sw)
{
	tempStats = world->StepAgent();

	++iteration;
	//Update the actionIterator
	actionCountDisplay->Text = iteration.ToString();
	actionCountDisplay->Refresh();

	//Update the timers
	timeCountDisplay->Text = sw->Elapsed.TotalSeconds.ToString();
	timeCountDisplay->Refresh();

	//Update the Charts with performance Stats		
	chartCumulativeReward->Series[((chartCumulativeReward->Series->Count) - 1).ToString()]->Points->AddY(tempStats.GetCumulativeReward());
	chartModelUse->Series[((chartModelUse->Series->Count) - 1).ToString()]->Points->AddY(tempStats.GetModelAccesses() + tempStats.GetModelUpdates());

}

//Button that will run the trials using the selected or default settings
void CherryOnTop::MyForm::runButton_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	runButton->Enabled = false;//So you don't end up mashing the button and queuing a bunch of runs
	if (polChanged || actChanged || worldChanged)//Create a new series in the charts so that they are displayed as a parallel line instead of a sequential
		ChartNewSeries();


	System::TimeSpan^ stopAtThisTime;
	int stopAtThisStep;
	polChanged = actChanged = worldChanged = false;

	if (doCompareTime)
		stopAtThisTime = gcnew System::TimeSpan(0, 0, timerStop->Value.ToInt32(timerStop->Value));
	if (doCompareSteps)
		stopAtThisStep = stepStop->Value.ToInt32(stepStop->Value);


	PerformanceStats tempStats;
	System::Diagnostics::Stopwatch^ sw = gcnew System::Diagnostics::Stopwatch();
	sw->Start();
	int iteration = 0;


	if (checkBox1->Checked) //Notice the duplicate code below, but instead of putting the comparison in the loops(reducing code)
	{					//We put the comparison outside as it only needs to be known once to display(faster iterations)

		if (doCompareSteps && doCompareTime)
			while (sw->ElapsedMilliseconds < stopAtThisTime->TotalMilliseconds && iteration < stopAtThisStep)
			{
				RunSteps(tempStats, iteration, sw);
				//Update the pictureBox
				VectorToBitmap(bmap, world->ShowState());
				pictureBox1->Image = bmap;
				pictureBox1->Invalidate();
				pictureBox1->Update();
				//Run Code
			}
		else if (doCompareSteps)
			while (iteration < stopAtThisStep)
			{
				RunSteps(tempStats, iteration, sw);
				//Update the pictureBox
				VectorToBitmap(bmap, world->ShowState());
				pictureBox1->Image = bmap;
				pictureBox1->Invalidate();
				pictureBox1->Update();
			}
		else if (doCompareTime)
		{
			while (sw->ElapsedMilliseconds < stopAtThisTime->TotalMilliseconds)
			{
				RunSteps(tempStats, iteration, sw);
				//Update the pictureBox
				VectorToBitmap(bmap, world->ShowState());
				pictureBox1->Image = bmap;
				pictureBox1->Invalidate();
				pictureBox1->Update();
			}
		}
	}

	else//WE are not displaying the individual steps.
	{
		if (doCompareSteps && doCompareTime)
			while (sw->ElapsedMilliseconds < stopAtThisTime->TotalMilliseconds && iteration < stopAtThisStep)
			{
				RunSteps(tempStats, iteration, sw);

			}
		else if (doCompareSteps)
			while (iteration < stopAtThisStep)
			{
				RunSteps(tempStats, iteration, sw);
			}
		else if (doCompareTime)
		{
			while (sw->ElapsedMilliseconds < stopAtThisTime->TotalMilliseconds)
			{
				RunSteps(tempStats, iteration, sw);
			}
		}
	}


	sw->Stop();//We are done running the trials.

	//Do the chart for steps to goal and update the map and other values
	chartStepsToGoal->Series[((chartStepsToGoal->Series->Count)-1).ToString()]->Points->Clear();
	for each  (double d in tempStats.GetStepsToGoal())
	{
		if (d <= 0)
			break;
		chartStepsToGoal->Series[((chartStepsToGoal->Series->Count) - 1).ToString()]->Points->Add(d);
	}

	cumulativeTime = cumulativeTime.Add(sw->Elapsed);
	cumulativeIterations += iteration;

	VectorToBitmap(bmap, world->ShowState());
	pictureBox1->Image = bmap;
	pictureBox1->Invalidate();
	pictureBox1->Update();

	cumTimeDisplay->Text = cumulativeTime.TotalSeconds.ToString();
	cumActionDisplay->Text = cumulativeIterations.ToString();

	cumTimeDisplay->Refresh();
	cumActionDisplay->Refresh();

	if (perfStats != nullptr)
		delete perfStats;
	perfStats = new PerformanceStats(tempStats); //Copy constructor, since we will lose tempStats, it lets us keep a persistent form of it. 
	runButton->Enabled = true;
}

//The dropdown box for the action-value, clicking it and changing will cause this function to be called
//Furthermore this will start a new series in the charts
void CherryOnTop::MyForm::avTypeBox_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e)
{	
	world->AddAgent(getPolicyBox(), getAVBox());//The news' from these functions will be acquired and cleaned up by agent etc..
	resetCounters();
	actChanged = true;
}


//The dropdown box for the policy, clicking it and changing will cause this function to be called
//Furthermore this will start a new series in the charts
void CherryOnTop::MyForm::PolicyBox_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e)
{
	world->AddAgent(getPolicyBox(), getAVBox());	
	resetCounters();
	polChanged = true;

}

//Used for the Form Item labelled World, it is a dropdown box that has multiple strings inside, one of which can be selected
//Have to create/give a new mapbmp to the gridworlds when they are lost before.
void CherryOnTop::MyForm::worldBox_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e)
{
	if (world != nullptr)
		delete world;

	switch (worldBox->SelectedIndex)
	{
	case (int)worldEnum::TraditionalMaze_4:
	{
		world = new TraditionalMaze();
		break;
	}
	case (int)worldEnum::TraditionalMaze_8:
	{
		world = new TraditionalMaze(8);
		break;
	}
	default:
	{
		world = new _DefaultWorld();
	}
	}

	if (System::IO::File::Exists(activeFile.FileName))
		LoadMapFromFileName(activeFile.FileName);
	
	world->AddAgent(getPolicyBox(), getAVBox());//Put this here, incase the world is swapped, but we want to keep the settings
	avTypeBox->Enabled = true;
	PolicyBox->Enabled = true;
	resetCounters();
	worldChanged = true;
}

void CherryOnTop::MyForm::numericUpDown1_ValueChanged(System::Object ^ sender, System::EventArgs ^ e)
{
	return System::Void();
}

//Will take the given bitmap and find the start location of the agent (from a black pixel)
//Will also return a vector representation of the map, marked with the enum colors from gridworldbase
vector<vector<double>> CherryOnTop::MyForm::BitmapToVector(System::Drawing::Bitmap^ bmapPtr, vector<double>& start)
{
	vector<vector<double>> outMap;
	size_t height = bmapPtr->Height;
	size_t width = bmapPtr->Width;

	outMap.resize(height);//faster to pre allocate then to keep growing and re-allocating the vector

	for (size_t h = 0; h < height; h++)
	{
		outMap[h].resize(width);
		for (size_t w = 0; w < width; w++)
		{
			Color gottenPixl = bmapPtr->GetPixel(w, h);//notice getpixl wants x,y but x is width, y is height
			bool hasRed = gottenPixl.R > 0;
			bool hasGreen = gottenPixl.G > 0;
			bool hasBlue = gottenPixl.B > 0;
			if (hasRed && hasGreen && hasBlue)//Its White
				outMap[h][w] = GridWorldBase::White;
			else if (!hasRed && !hasGreen && !hasBlue)//Its the agent
			{
				outMap[h][w] = GridWorldBase::White;
				start.push_back(h);
				start.push_back(w);
			}
			else if (!hasRed && !hasGreen && hasBlue)//Its the wall
				outMap[h][w] = GridWorldBase::Blue;
			else if (!hasRed && hasGreen && !hasBlue)//Its the goal
				outMap[h][w] = GridWorldBase::Green;
			else if (hasRed && !hasGreen && !hasBlue)//Its Lava
				outMap[h][w] = GridWorldBase::Red;
			else throw("MyForm.cpp::Load::invalid Color choice");
			//Further colors such as Purple(red,blue) Yellow(green,red) etc.. 
			//can be further defined here if there behaviour become defined
		}
	}
	return outMap;
}

//Will take the given vector<vector<double>> then, based on what the vector has different from the Bitmap 
//Will modify the bitmap but not all the properties
//Currently does not have a progressive shading effect, perhaps working on a seperate structure to keep track
//of the number of visits a state has had for that effect,
//Note that vecMap tends to come from Showstate in gridworlds
//Note that not all of these cases need to be reachable
void CherryOnTop::MyForm::VectorToBitmap(System::Drawing::Bitmap^ bmpPtr,const vector<vector<double>>& vecMap)
{
	for (unsigned int y = 0; y < vecMap.size(); y++)
		for (unsigned int x = 0; x < vecMap[y].size(); x++)
		{
			switch ((GridWorldBase::stateColors)(int)vecMap[y][x])
			{
			case GridWorldBase::Red: bmpPtr->SetPixel(x, y, Color::Red); break;//The thinger is red 
			case GridWorldBase::White: bmpPtr->SetPixel(x, y, Color::White); break;//The thinger is white
			case GridWorldBase::Blue: bmpPtr->SetPixel(x, y, Color::Blue); break;//The Thinger is blue
			case GridWorldBase::Green: bmpPtr->SetPixel(x, y, Color::Lime); break;//The thinger is green
			case GridWorldBase::Black: bmpPtr->SetPixel(x, y, Color::Black); break;//Its the agent 
			case GridWorldBase::LessWhite: bmpPtr->SetPixel(x, y, Color::Gray); break;//Seen this white statebefore
			case GridWorldBase::LessRed: bmpPtr->SetPixel(x, y, Color::DarkRed); break;
			case GridWorldBase::LessBlue: bmpPtr->SetPixel(x, y, Color::SlateBlue); break;
			case GridWorldBase::LessGreen: bmpPtr->SetPixel(x, y, Color::Green); break;
			case GridWorldBase::LessBlack: bmpPtr->SetPixel(x, y, Color::SlateGray); break;
			default: {
				bmpPtr->SetPixel(x,y,Color::Purple);//Ya did something silly didnt you
				}
			}
		}
}

//This is the paint event handler function, it is called when ever the paint event is called
//This includes whenever the picturebox.refresh or update methods are called
//Its purpose is to fit any size of image within the frame of the picture box
void CherryOnTop::MyForm::pictureBox1Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
	Graphics^ g = e->Graphics;
	g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
	
	if (pictureBox1->Image != nullptr)
	g->DrawImage(
		pictureBox1->Image,
		System::Drawing::Rectangle(0, 0, pictureBox1->Width, pictureBox1->Height),		// destination rectangle 
		0,
		0,           // upper-left corner of source rectangle
		pictureBox1->Image->Width,       // width of source rectangle
		pictureBox1->Image->Height,      // height of source rectangle
		GraphicsUnit::Pixel);

}
//Will be used to look after cleaning up ptrs for the picturebox and bmap
//Will also look at taking care of converting the bitmap given, and turning it into a vec<vec<dble>> 
//So that the world can understand it.
void CherryOnTop::MyForm::LoadMapFromFileName(System::String^ ofd)
{

	if (world == nullptr)
		world = new _DefaultWorld();
	if (pictureBox1->Image != nullptr)
		delete pictureBox1->Image;
	if (bmap != nullptr)
		delete bmap;

	bmap = nullptr;
	pictureBox1->Image = nullptr; //Prevents a memory leak from the old image being left on stack if something wrong happens in Fromfile

	bmap = gcnew Bitmap(ofd);
	pictureBox1->Image = bmap;

	pictureBox1->Invalidate();
	pictureBox1->Update();

	//Load up the bitmap into Image, then interpret the information into a vector to give the the world to load
	vector<double> start;//Starting location, found when a black is seen
	vector<vector<double>> inMap = BitmapToVector(bmap, start);
	world->Load(inMap, start);
	
}

//Will be looking after reseting the counters, any new counters that are added would be insterted in here,
//Then this function can be called whenever a reset is desired (new agent, new world etc..)
void  CherryOnTop::MyForm::resetCounters()
{
	cumActionDisplay->Text = "0";
	cumTimeDisplay->Text = "0";
	cumulativeIterations = 0;
	cumulativeTime = (cumulativeTime.Subtract(cumulativeTime));//I dont know if this is the best way to zero this out.
	cumTimeDisplay->Refresh();
	cumActionDisplay->Refresh();
}



//I put this here in shame, because I had some shitty logic with Ptrs, and missed a crucial piece
//Will just update with what the current actionvalue selection is.
//Then return a new ptr, which will be an input for the addAgent function.
//Further Values will need to be added to this list.
//For the dropdown menu labelled actionvalue, this is where you would add new Actionvalue selections.
ActionValue* CherryOnTop::MyForm::getAVBox()
{
	switch (avTypeBox->SelectedIndex)
	{
	case (int)actionValuesEnum::QLearning:
	{
		return new QLearning(world->GetAvailableActions());
	}
	case (int)actionValuesEnum::ModelBased:
	{
		return new ModelBasedLearning(world->GetAvailableActions(),world->GetStartState());
	}
	default: return  nullptr; //Nullptr, causes default choices from the agent. (Probably Q-Learning)
	}
}


//I put this here in shame, because I had some shitty logic with Ptrs, and missed a crucial piece
//Will just update with what the current policy selection is.
//Then return a new ptr, which will be an input for the addAgent function.
//Further Values will need to be added to this list.
//For the dropdown menu labelled Policy, this is where you would add new policy selections.
PolicyBase*  CherryOnTop::MyForm::getPolicyBox()
{
	switch (PolicyBox->SelectedIndex)
	{
	case(int)policyEnum::EpsilonGreedy:
	{
		return new EpsilonGreedy();
		break;
	}
	case(int)policyEnum::Optimal:
	{
		return new OptimalPolicy();
		break;
	}
	case(int)policyEnum::Softmax:
	{
		return new SoftmaxPolicy();
		break;
	}
	default: return nullptr; //Nullptr, causes default choices from the agent. (Probably Optimal)
	}
}



//function will reset the charts to have a new series started when a factor of the agent is changed such as the action value, the world, or the policy
//This function can be modified if a different style of charting is desired.
void  CherryOnTop::MyForm::ChartNewSeries()
{
	String^ cumCount = (chartCumulativeReward->Series->Count).ToString();
	chartCumulativeReward->Series->Add(cumCount);
	chartCumulativeReward->Series[cumCount]->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;

	String^ modUseCount = (chartModelUse->Series->Count).ToString();
	chartModelUse->Series->Add(cumCount);
	chartModelUse->Series[modUseCount]->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;

	String^ stepsCount = (chartStepsToGoal->Series->Count).ToString();
	chartStepsToGoal->Series->Add(stepsCount);
	chartStepsToGoal->Series[stepsCount]->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;

}

//This Combo Box is used to determine what the stopping condition is for the test whether it be steps, the timer or both.
void CherryOnTop::MyForm::stepTimerComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{

	if (stepTimerComboBox->SelectedItem == "Use Steps")
	{
		doCompareTime = false;
		doCompareSteps = true;
	}
	else if (stepTimerComboBox->SelectedItem == "Use Timer")
	{
		doCompareSteps = false;
		doCompareTime = true;
	}
	else //If not either above then we are using both
	{
		doCompareSteps = true;
		doCompareTime = true;
	}

	//For clarity while using we will disable the numeric up-down of whatever we aren't using, and enable what we are
	stepStop->Enabled = doCompareSteps;
	timerStop->Enabled = doCompareTime;
}