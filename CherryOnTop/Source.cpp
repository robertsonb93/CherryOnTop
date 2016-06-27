
#include <Windows.h>
#include <windows.system.h>
#include "MyForm.h"

[System::STAThread]
int main()
{

	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	System::Windows::Forms::Application::Run(gcnew CherryOnTop::MyForm());
	return 0;
}