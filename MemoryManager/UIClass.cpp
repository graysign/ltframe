#include "stdafx.h"

#include "UIClass.h"
#include <Psapi.h>
UIClass* UIClassC=0;
DEFINEFUNCTION DragWindowFun(LTExecState* es)
{
	UIClassC->DragWindow();
	return JsUndefined();
}

DEFINEFUNCTION QuitAppFun(LTExecState* es)
{
	UIClassC->QuitApp();
	return JsUndefined();
}
DEFINEFUNCTION MinWindowFun(LTExecState* es)
{
	UIClassC->MinWindow();
	return JsUndefined();
}
DEFINEFUNCTION JobStartFun(LTExecState* es)
{
	UIClassC->JobStart();
	return JsUndefined();
}
DEFINEFUNCTION AutoGCFun(LTExecState* es)
{
	bool t = JsValue2Boolean(es,GetJsParameter(es,0));
	UIClassC->IsAutoGC(t);
	return JsUndefined();
}

DEFINEFUNCTION CleanMemoryFun(LTExecState* es)
{
	UIClassC->CleanMemory();
	return JsUndefined();
}

UIClass::UIClass()
{	
	HMODULE hModule = ::GetModuleHandle(0);
	applicationpath  = (wchar_t*)malloc((MAX_PATH+1)*sizeof(wchar_t)); 
	GetModuleFileName(NULL, applicationpath, MAX_PATH);
	(wcsrchr(applicationpath, '\\'))[1] = 0;
	int ww = 700,wh=500;
	OleInitialize(0);


	ltskinview= CreateLTFrameInstance(this,L"LTFrame-MemoryManager",NULL,WS_POPUP | WS_VISIBLE,0,0,ww,wh,0);
	UIClassC =this;
	ltskinview->BindUserFunction("DragWindow",DragWindowFun,0);
	ltskinview->BindUserFunction("QuitApp",QuitAppFun,0);
	ltskinview->BindUserFunction("MinWindow",MinWindowFun,0);
	ltskinview->BindUserFunction("JobStart",JobStartFun,0);
	ltskinview->BindUserFunction("AutoGC",AutoGCFun,1);
	ltskinview->BindUserFunction("CleanMemory",CleanMemoryFun,0);
	SetWindowText(ltskinview->windowHandle(),L"LTFrame-MemoryManager");	
	
	DWORD dwProcessId;
	GetWindowThreadProcessId(ltskinview->windowHandle(), &dwProcessId);
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, dwProcessId);

	wstring path = wstring(applicationpath)+L"./template/MemoryManager.html";
	ltskinview->loadFile(path.c_str());
	ltskinview->EnableDragFrameChangeSize(false);
	ltskinview->MessageLoop();
}


UIClass::~UIClass(void)
{
	OleUninitialize();
	free(applicationpath);
	delete ltskinview;
}
void UIClass::DragWindow()
{
	ReleaseCapture();
	POINT DownPoint;
	GetCursorPos(&DownPoint);
	SendMessage(ltskinview->windowHandle(),WM_NCLBUTTONDOWN,HTCAPTION,MAKEWPARAM(DownPoint.x,DownPoint.y));
}
void UIClass::IsAutoGC(bool t)
{
	ltskinview->IsAutoGC(t);
}
void UIClass::CleanMemory()
{
	ltskinview->CleanMemory();
}
void UIClass::JobStart()
{
	::SetTimer(ltskinview->windowHandle(),8888888,1,NULL); 
}
void UIClass::QuitApp()
{
	ltskinview->QuitApp();
}
void UIClass::MinWindow()
{
	::ShowWindow(ltskinview->windowHandle(),SW_MINIMIZE);	
}
LRESULT UIClass::HandleUserMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    switch(uMsg)
	{
		case WM_TIMER:
		{
		    switch (wParam)
			{
			   case 8888888:
				   {
						PROCESS_MEMORY_COUNTERS pmc;
						GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
						int fCurUse = int(pmc.WorkingSetSize / (1024*1024));
						char *str =new char[100];
						ZeroMemory(str,100);
						_itoa(fCurUse,str,10);
						string js = string("showmemory(")+str+")";
						ltskinview->RunJavaScript(js.c_str());
						delete [] str;
				   }
			   break ;
			}
		}
		break;
	}
	return ltskinview->HandleMessage(hwnd,uMsg,wParam,lParam);
}


LPCWSTR UIClass::GetWindowClassName()
{
	return L"MemoryManager";
}
