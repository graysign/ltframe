#include "stdafx.h"

#include "UIClass.h"
#include <Psapi.h>
UIClass* UIClassC=0;

UIClass::UIClass()
{	
	HMODULE hModule = ::GetModuleHandle(0);
	applicationpath  = (wchar_t*)malloc((MAX_PATH+1)*sizeof(wchar_t)); 
	GetModuleFileName(NULL, applicationpath, MAX_PATH);
	(wcsrchr(applicationpath, '\\'))[1] = 0;
	int ww = 864,wh=662;
	OleInitialize(0);


	ltskinview= CreateLTFrameInstance(this,L"LTFrame-Canvas",NULL,WS_POPUP | WS_VISIBLE,100,50,ww,wh,0);
	UIClassC =this;
	SetWindowText(ltskinview->windowHandle(),L"LTFrame-Canvas");	
	ltskinview->SetViewTransparent(true);
	ltskinview->SetAllowKeyDownAutoScroll(false);
	wstring path = wstring(applicationpath)+L"./template/ipad/ipad.html";
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

	return ltskinview->HandleMessage(hwnd,uMsg,wParam,lParam);

}
LPCWSTR UIClass::GetWindowClassName()
{
	return L"Canvas";
}
