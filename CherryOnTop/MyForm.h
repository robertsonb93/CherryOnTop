#pragma once

//Learners
#include "../TheCherries/QLearning.h"
#include "../TheCherries/ModelBasedLearning.h"

//Auxiliaries
#include "../TheCherries/AgentSingle.h"
#include "../TheCherries/PerformanceStats.h"
#include "../TheCherries/StateTransition.h"

//Policy
#include "../TheCherries/EpsilonGreedy.h"
#include "../TheCherries/OptimalPolicy.h"
#include "../TheCherries/SoftMaxPolicy.h"

//Worlds
#include "../TheCherries/TraditionalMaze.h"


typedef TraditionalMaze _DefaultWorld;


//The function and member declarations are at the bottom below all the windows form initilization
namespace CherryOnTop {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			stopAtThisTime = gcnew TimeSpan(99, 99, 99);
			pictureBox1->Image = nullptr;
			pictureBox1->Paint += gcnew PaintEventHandler(this, &MyForm::pictureBox1Paint);
			resetCounters();

			avTypeBox->DataSource = Enum::GetNames(actionValuesEnum::typeid); //This will populate the actionvalue list
			PolicyBox->DataSource = Enum::GetNames(policyEnum::typeid);
			worldBox->DataSource = Enum::GetNames(worldEnum::typeid);

			PolicyBox->Enabled = false;//Turns the buttons off, will turn them on when a world is chosen.
			avTypeBox->Enabled = false;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  runButton;
	protected:

	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::ComboBox^  avTypeBox;
	private: System::Windows::Forms::ComboBox^  PolicyBox;
	private: System::Windows::Forms::ComboBox^  worldBox;
	private: System::Windows::Forms::Button^  loadMapButton;
	private: System::Windows::Forms::TextBox^  actionCountDisplay;
	private: System::Windows::Forms::TextBox^  timeCountDisplay;
	private: System::Windows::Forms::TextBox^  cumActionDisplay;
	private: System::Windows::Forms::TextBox^  cumTimeDisplay;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chartCumulativeReward;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chartModelUse;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chartStepsToGoal;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;




	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea3 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->runButton = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->avTypeBox = (gcnew System::Windows::Forms::ComboBox());
			this->PolicyBox = (gcnew System::Windows::Forms::ComboBox());
			this->worldBox = (gcnew System::Windows::Forms::ComboBox());
			this->loadMapButton = (gcnew System::Windows::Forms::Button());
			this->actionCountDisplay = (gcnew System::Windows::Forms::TextBox());
			this->timeCountDisplay = (gcnew System::Windows::Forms::TextBox());
			this->cumActionDisplay = (gcnew System::Windows::Forms::TextBox());
			this->cumTimeDisplay = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->chartCumulativeReward = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chartModelUse = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chartStepsToGoal = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartCumulativeReward))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartModelUse))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartStepsToGoal))->BeginInit();
			this->SuspendLayout();
			// 
			// runButton
			// 
			this->runButton->Enabled = false;
			this->runButton->Location = System::Drawing::Point(506, 143);
			this->runButton->Name = L"runButton";
			this->runButton->Size = System::Drawing::Size(64, 26);
			this->runButton->TabIndex = 0;
			this->runButton->Text = L"Run";
			this->runButton->UseVisualStyleBackColor = true;
			this->runButton->Click += gcnew System::EventHandler(this, &MyForm::runButton_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(12, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(200, 200);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(426, 15);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(74, 20);
			this->numericUpDown1->TabIndex = 2;
			this->numericUpDown1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->numericUpDown1->ThousandsSeparator = true;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2000, 0, 0, 0 });
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &MyForm::numericUpDown1_ValueChanged);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(287, 18);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(133, 17);
			this->checkBox1->TabIndex = 3;
			this->checkBox1->Text = L"Display While Running";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// avTypeBox
			// 
			this->avTypeBox->FormattingEnabled = true;
			this->avTypeBox->ImeMode = System::Windows::Forms::ImeMode::On;
			this->avTypeBox->Location = System::Drawing::Point(287, 42);
			this->avTypeBox->Name = L"avTypeBox";
			this->avTypeBox->Size = System::Drawing::Size(121, 21);
			this->avTypeBox->Sorted = true;
			this->avTypeBox->TabIndex = 4;
			this->avTypeBox->Text = L"ActionValue";
			this->avTypeBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::avTypeBox_SelectedIndexChanged);
			// 
			// PolicyBox
			// 
			this->PolicyBox->FormattingEnabled = true;
			this->PolicyBox->Location = System::Drawing::Point(287, 69);
			this->PolicyBox->Name = L"PolicyBox";
			this->PolicyBox->Size = System::Drawing::Size(121, 21);
			this->PolicyBox->TabIndex = 5;
			this->PolicyBox->Text = L"Policy";
			this->PolicyBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::PolicyBox_SelectedIndexChanged);
			// 
			// worldBox
			// 
			this->worldBox->FormattingEnabled = true;
			this->worldBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"TraditionalMaze-4", L"TraditionalMaze-8" });
			this->worldBox->Location = System::Drawing::Point(287, 96);
			this->worldBox->Name = L"worldBox";
			this->worldBox->Size = System::Drawing::Size(121, 21);
			this->worldBox->TabIndex = 6;
			this->worldBox->Text = L"World";
			this->worldBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::worldBox_SelectedIndexChanged);
			// 
			// loadMapButton
			// 
			this->loadMapButton->Location = System::Drawing::Point(506, 10);
			this->loadMapButton->Name = L"loadMapButton";
			this->loadMapButton->Size = System::Drawing::Size(64, 26);
			this->loadMapButton->TabIndex = 7;
			this->loadMapButton->Text = L"LoadMap";
			this->loadMapButton->UseVisualStyleBackColor = true;
			this->loadMapButton->Click += gcnew System::EventHandler(this, &MyForm::loadMapButton_Click);
			// 
			// actionCountDisplay
			// 
			this->actionCountDisplay->Location = System::Drawing::Point(287, 143);
			this->actionCountDisplay->Name = L"actionCountDisplay";
			this->actionCountDisplay->Size = System::Drawing::Size(74, 20);
			this->actionCountDisplay->TabIndex = 8;
			// 
			// timeCountDisplay
			// 
			this->timeCountDisplay->Location = System::Drawing::Point(287, 169);
			this->timeCountDisplay->Name = L"timeCountDisplay";
			this->timeCountDisplay->Size = System::Drawing::Size(74, 20);
			this->timeCountDisplay->TabIndex = 9;
			// 
			// cumActionDisplay
			// 
			this->cumActionDisplay->Location = System::Drawing::Point(377, 143);
			this->cumActionDisplay->Name = L"cumActionDisplay";
			this->cumActionDisplay->Size = System::Drawing::Size(74, 20);
			this->cumActionDisplay->TabIndex = 10;
			// 
			// cumTimeDisplay
			// 
			this->cumTimeDisplay->Location = System::Drawing::Point(377, 169);
			this->cumTimeDisplay->Name = L"cumTimeDisplay";
			this->cumTimeDisplay->Size = System::Drawing::Size(74, 20);
			this->cumTimeDisplay->TabIndex = 11;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(218, 146);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(68, 13);
			this->label1->TabIndex = 12;
			this->label1->Text = L"Action Count";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(212, 172);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(74, 13);
			this->label2->TabIndex = 13;
			this->label2->Text = L"Time Count(S)";
			// 
			// chartCumulativeReward
			// 
			this->chartCumulativeReward->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea1->Name = L"ChartArea1";
			this->chartCumulativeReward->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chartCumulativeReward->Legends->Add(legend1);
			this->chartCumulativeReward->Location = System::Drawing::Point(14, 234);
			this->chartCumulativeReward->Name = L"chartCumulativeReward";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series1->Legend = L"Legend1";
			series1->Name = L"0";
			this->chartCumulativeReward->Series->Add(series1);
			this->chartCumulativeReward->Size = System::Drawing::Size(555, 150);
			this->chartCumulativeReward->TabIndex = 14;
			this->chartCumulativeReward->Text = L"Cumulative Reward";
			// 
			// chartModelUse
			// 
			chartArea2->Name = L"ChartArea1";
			this->chartModelUse->ChartAreas->Add(chartArea2);
			legend2->Name = L"Legend1";
			this->chartModelUse->Legends->Add(legend2);
			this->chartModelUse->Location = System::Drawing::Point(15, 390);
			this->chartModelUse->Name = L"chartModelUse";
			series2->ChartArea = L"ChartArea1";
			series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series2->Legend = L"Legend1";
			series2->Name = L"0";
			this->chartModelUse->Series->Add(series2);
			this->chartModelUse->Size = System::Drawing::Size(555, 150);
			this->chartModelUse->TabIndex = 15;
			this->chartModelUse->Text = L"chart2";
			// 
			// chartStepsToGoal
			// 
			chartArea3->Name = L"ChartArea1";
			this->chartStepsToGoal->ChartAreas->Add(chartArea3);
			legend3->Name = L"Legend1";
			this->chartStepsToGoal->Legends->Add(legend3);
			this->chartStepsToGoal->Location = System::Drawing::Point(12, 546);
			this->chartStepsToGoal->Name = L"chartStepsToGoal";
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series3->Legend = L"Legend1";
			series3->Name = L"0";
			this->chartStepsToGoal->Series->Add(series3);
			this->chartStepsToGoal->Size = System::Drawing::Size(555, 150);
			this->chartStepsToGoal->TabIndex = 16;
			this->chartStepsToGoal->Text = L"chart3";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(238, 371);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(34, 13);
			this->label3->TabIndex = 17;
			this->label3->Text = L"Steps";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(0, 298);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(44, 13);
			this->label4->TabIndex = 18;
			this->label4->Text = L"Reward";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(0, 455);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(36, 26);
			this->label5->TabIndex = 19;
			this->label5->Text = L"Model\r\nUse";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(0, 592);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(34, 39);
			this->label6->TabIndex = 20;
			this->label6->Text = L"Steps\r\nTo\r\nGoal";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(208, 683);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(78, 13);
			this->label7->TabIndex = 21;
			this->label7->Text = L"GoalsReached";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(584, 702);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->chartStepsToGoal);
			this->Controls->Add(this->chartModelUse);
			this->Controls->Add(this->chartCumulativeReward);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->cumTimeDisplay);
			this->Controls->Add(this->cumActionDisplay);
			this->Controls->Add(this->timeCountDisplay);
			this->Controls->Add(this->actionCountDisplay);
			this->Controls->Add(this->loadMapButton);
			this->Controls->Add(this->worldBox);
			this->Controls->Add(this->PolicyBox);
			this->Controls->Add(this->avTypeBox);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->runButton);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartCumulativeReward))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartModelUse))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartStepsToGoal))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}


	private:
		//Runtime Members ARE HERE//
		GridWorldBase* world = new _DefaultWorld(); //Can point to any type of world. In the form though, we only want Gridworld

		Bitmap^ bmap = nullptr; //Will be used for holding onto a modifiable bitmap of the displayed image, since the picbox->image, cant modify.
		System::TimeSpan^ stopAtThisTime;
		System::TimeSpan cumulativeTime;
		int cumulativeIterations;
		bool doCompareTime = false;
		bool mapLoaded = false;

		//Used for Determining if a new chart is needed at run time.
		bool polChanged = false;
		bool actChanged = false;
		bool worldChanged = false;

		OpenFileDialog activeFile;
		PerformanceStats* perfStats = nullptr;

		enum class actionValuesEnum { ActionValue, ModelBased, QLearning };
		enum class policyEnum {Policy, EpsilonGreedy, Optimal, Softmax };
		enum class worldEnum {World, TraditionalMaze_4, TraditionalMaze_8 };
		
	
	//RUNTIME FUNCTIONS ARE DOWN HERE//

		void loadMapButton_Click(System::Object^  sender, System::EventArgs^  e);
		void runButton_Click(System::Object^  sender, System::EventArgs^  e);
		//void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		void avTypeBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
		void PolicyBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
		void worldBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
		void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e);

		vector<vector<double>> BitmapToVector(Bitmap^ bmapPtr, vector<double>& start);
		void VectorToBitmap(Bitmap^ bmpPtr, const vector<vector<double>>& vecMap);
		void pictureBox1Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
		void LoadMapFromFileName(System::String^ ofd);
		void resetCounters();
		void ChartNewSeries();
		ActionValue* getAVBox();
		PolicyBase* getPolicyBox();


};
}
#pragma endregion