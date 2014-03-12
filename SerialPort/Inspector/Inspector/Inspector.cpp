// Inspector.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"
#include "time.h"
#include <conio.h>

HWND g_hBtnAddWatch=NULL,g_hBtnAddWatchByte=NULL,g_hBtnAddWatchWord=NULL,g_hBtnAddWatchDWord=NULL;
#define WATCH_DEFAULTREFRESHINTERVAL 20;
#define WATCHFLAG_DISABLED 1
#define WATCHFLAG_AUTOREFRESH 2
//const long c_defaultRefItv=100;
long g_refItv=100;
HWND g_hWnd=0;
HFONT g_hFont=NULL;
HWND g_hStaticRefItv=NULL;
HWND g_hBtnSetRefItv=NULL;
HANDLE g_hCom=INVALID_HANDLE_VALUE;
long g_baudRate=0;
TCHAR g_comName[16];
HWND g_hBtnReconnect=NULL;
HWND g_hStaticComName=NULL;
HWND g_hBtnSave=NULL;
HWND g_hBtnLoad=NULL;
HWND g_hBtnQuickSave=NULL;
HWND g_hBtnQuickLoad=NULL;
HWND g_hStaticFPS=NULL;
void Connect(TCHAR *name){
	{
		TCHAR t[32]=L"(未连接)";
		wcscat(t,name);
		SendMessage(g_hStaticComName,WM_SETTEXT,0,LPARAM(t));
	}
	if(g_hCom!=INVALID_HANDLE_VALUE){
		printf("当前连接未断开,不能重新连接\n");
		return;
	}
    g_hCom=CreateFile(name,
            GENERIC_READ|GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
    if(g_hCom==INVALID_HANDLE_VALUE){
		printf("Error:CreateFile(...)==INVALID_HANDLE_VALUE\n");
        return;
    }
    SetupComm(g_hCom,4096,4096);
	{
		DCB dcb;
		dcb.DCBlength=sizeof(dcb);
		//printf("SetBaudRate:");scanf("%ld",&dcb.BaudRate);
		dcb.BaudRate=g_baudRate;
		dcb.fBinary=true;
		dcb.fParity=false;
		dcb.fOutxCtsFlow=false;
		dcb.fOutxDsrFlow=false;
		dcb.fDtrControl=DTR_CONTROL_DISABLE;
		dcb.fDsrSensitivity=false;
		dcb.fTXContinueOnXoff=true;
		dcb.fOutX=false;
		dcb.fInX=false;
		dcb.fErrorChar=false;
		dcb.fNull=false;
		dcb.fRtsControl=RTS_CONTROL_DISABLE;
		dcb.fAbortOnError=false;
		dcb.fDummy2=0;
		dcb.wReserved=0;
		dcb.XonLim=1;
		dcb.XoffLim=0;
		dcb.ByteSize=8;
		dcb.Parity=NOPARITY;
		dcb.StopBits=ONESTOPBIT;
		dcb.XonChar='a';
		dcb.XoffChar='b';
		dcb.ErrorChar='e';
		dcb.EofChar='f';
		dcb.EvtChar='!';
		dcb.wReserved=0;
		if(SetCommState(g_hCom,&dcb)==0){
			printf("Error:SetCommState(...) 0x%X\n",GetLastError());
			CloseHandle(g_hCom);
			g_hCom=INVALID_HANDLE_VALUE;
		    return;
		}
	}
	{
		COMMTIMEOUTS timeout;
		timeout.ReadIntervalTimeout=0;
		timeout.ReadTotalTimeoutConstant=100;
		timeout.ReadTotalTimeoutMultiplier=100;
		timeout.WriteTotalTimeoutConstant=0;
		timeout.WriteTotalTimeoutMultiplier=0;
		if(SetCommTimeouts(g_hCom,&timeout)==0){
			printf("Error:SetCommTimeouts(...) 0x%X\n",GetLastError());
			CloseHandle(g_hCom);
			g_hCom=INVALID_HANDLE_VALUE;
		    return;
		}
	}	
	printf("连接中，按任意键取消...\n");
	while(!_kbhit()){
		BYTE handshake[2]={0,rand()&0xff};
		DWORD numBytesWritten=0;
		WriteFile(g_hCom,handshake,2,&numBytesWritten,NULL);
		if(numBytesWritten<2){
			printf("写串口失败\n");
			return;
		}
		DWORD numBytesRead=0;
		BYTE buf[2]={0,0};
		Sleep(200);
		ReadFile(g_hCom,buf,2,&numBytesRead,NULL);
		if(numBytesRead==2&&buf[0]==0&&buf[1]==(handshake[1]+17)&0xff){
			printf("连接成功\n");
			SendMessage(g_hStaticComName,WM_SETTEXT,0,LPARAM(name));
			return;
		}
	}
	_getch();
	printf("连接取消\n");
	CloseHandle(g_hCom);
	g_hCom=INVALID_HANDLE_VALUE;
	//
}
void Connect(){
	char t[16];
	printf("请输入串口名:");
	scanf("%s",t);
	mbstowcs(g_comName,t,16);
	Connect(g_comName);
}
void Disconnect(){	
	if(g_hCom!=INVALID_HANDLE_VALUE){
		CloseHandle(g_hCom);
		g_hCom=INVALID_HANDLE_VALUE;
	}
	{
		TCHAR t[32]=L"(未连接)";
		SendMessage(g_hStaticComName,WM_SETTEXT,0,LPARAM(t));
	}
}
#define FILL(delta) for(long i=wcslen(t)+delta;i<WIDTH;++i){ \
		**ppOut=L' '; \
		++*ppOut; \
	}
class CWatch{
public:
	HWND m_hEdit;
	DWORD m_flags;
	short m_x,m_y;
//	long m_refreshInterval;
	WORD m_address,m_size;
	TCHAR m_name[32];
	BYTE  m_buf[1024];
	TCHAR m_text[512];
	long m_elementcount;
	void (*m_itprProc[32])(BYTE** ppIn,TCHAR** ppOut);
//	CWatch():m_refreshInterval(c_defaultRefItv){}
	static long s_nWatch;
	void Show(){
		long y=24*(this-g_watchList)+64;
		m_hEdit=CreateWindow(L"EDIT",      // predefined class 
                                    NULL,        // no window title 
                                    WS_CHILD | WS_VISIBLE |
                                    ES_LEFT,
                                    0, y, 800, 18,  // set size in WM_SIZE message 
                                    g_hWnd,        // parent window 
                                    0,   
                                    (HINSTANCE) GetWindowLong(g_hWnd, GWL_HINSTANCE), 
                                    NULL);       // pointer not needed 
		SendMessage(m_hEdit,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	}
	void Hide(){
		if(m_hEdit){
			DestroyWindow(m_hEdit);
			m_hEdit=NULL;
		}
	}
	void Refresh(){
		{
			BYTE b[5]={1};
			*(WORD*)(b+1)=m_address;
			*(WORD*)(b+3)=m_size;
			DWORD t=100000;
			WriteFile(g_hCom,b,5,&t,NULL);
			ReadFile(g_hCom,m_buf,m_size,&t,NULL);
			if(t<m_size){
				TCHAR t[32]=L"(连接中断)";
				SendMessage(g_hStaticComName,WM_SETTEXT,0,LPARAM(t));
			}
		}
		BYTE *in=m_buf;
		TCHAR *out=m_text;
		for(long i=wcslen(m_name);i<32;++i){
			*out=L' ';
			++out;
		}
		wcscpy(out,m_name);
		out+=wcslen(m_name);
		
		TCHAR t[16];
		_itow(m_address, t, 16);
		for(long i=wcslen(t)+2;i<16;++i){
			*out=L' ';
			++out;
		}
		wcscpy(out,L"0x");
		out+=2;
		wcscpy(out,t);
		out+=wcslen(t);

		for(long i=0;i<m_elementcount;++i)
			(m_itprProc[i])(&in,&out);
		*out=0;
		SendMessage( m_hEdit, WM_SETTEXT, 0,(LPARAM) m_text);
	}
}g_watchList[256];
long CWatch::s_nWatch=0;
#define WIDTH 16
void InterpretChar(BYTE **ppIn,TCHAR **ppOut){
	TCHAR t[WIDTH+2];
	_itow(**ppIn,t,10);
	FILL(0);
	wcscpy(*ppOut,t);
	*ppOut+=wcslen(t);
	_itow(*(signed char*)*ppIn,t,10);
	FILL(0);
	wcscpy(*ppOut,t);
	*ppOut+=wcslen(t);
	_itow(**ppIn,t,16);
	FILL(2);
	wcscpy(*ppOut,L"0x");
	*ppOut+=2;
	wcscpy(*ppOut,t);
	*ppOut+=wcslen(t);
	++*ppIn;
}
void InterpretWord(BYTE **ppIn,TCHAR **ppOut){
	TCHAR t[WIDTH+2];
	WORD a=((**ppIn)<<8)+*(*ppIn+1);
	_itow(a,t,10);
	FILL(0);
	wcscpy(*ppOut, t);
	*ppOut+=wcslen(t);
	_itow((signed short)a,t,10);
	FILL(0);
	wcscpy(*ppOut, t);
	*ppOut+=wcslen(t);
	_itow(a,t,16);
	FILL(2);
	wcscpy(*ppOut,L"0x");
	*ppOut+=2;
	wcscpy(*ppOut,t);
	*ppOut+=wcslen(t);
	*ppIn+=2;
}
void InterpretDWord(BYTE **ppIn,TCHAR **ppOut){
	TCHAR t[WIDTH+2];
	_itow(*(DWORD*)*ppIn,t,10);
	FILL(0);
	wcscpy(*ppOut, t);
	*ppOut+=wcslen(t);
	_itow(*(signed long*)*ppIn,t,10);
	FILL(0);
	wcscpy(*ppOut, t);
	*ppOut+=wcslen(t);
	_itow(*(DWORD*)*ppIn,t,16);
	FILL(2);
	wcscpy(*ppOut,L"0x");
	*ppOut+=2;
	wcscpy(*ppOut,t);
	*ppOut+=wcslen(t);
	*ppIn+=2;
}
#define WRITE(a,f) fwrite(&a,sizeof(a),1,f)
#define WRITESTRING(a,f) { \
	long len=wcslen(a); \
	WRITE(len,f); \
	fwrite(a,sizeof(TCHAR),len,f); \
}
#define READ(a,f) fread(&a,sizeof(a),1,f)
#define READSTRING(a,f) { \
	long len; \
	READ(len,f); \
	fread(a,sizeof(TCHAR),len,f); \
	a[len]=0; \
}
void Save(TCHAR *name){
	FILE *f=_wfopen(name,L"wb");
	if(!f){
		printf("打开文件失败\n");
		return;
	}
	WRITE(CWatch::s_nWatch,f);
	for(long i=0;i<CWatch::s_nWatch;++i){
		WRITE(g_watchList[i].m_flags,f);
		WRITE(g_watchList[i].m_size,f);
		WRITE(g_watchList[i].m_address,f);
		WRITESTRING(g_watchList[i].m_name,f);
		WRITE(g_watchList[i].m_elementcount,f);
		fwrite(g_watchList[i].m_itprProc,sizeof(void *),g_watchList[i].m_elementcount,f);
	}
	fclose(f);
	printf("保存成功\n");
}
void Load(TCHAR *name){
	FILE *f=_wfopen(name,L"rb");
	if(!f){
		printf("打开文件失败\n");
		return;
	}
	READ(CWatch::s_nWatch,f);
	for(long i=0;i<CWatch::s_nWatch;++i){
		READ(g_watchList[i].m_flags,f);
		READ(g_watchList[i].m_size,f);
		READ(g_watchList[i].m_address,f);
		READSTRING(g_watchList[i].m_name,f);
		READ(g_watchList[i].m_elementcount,f);
		fread(g_watchList[i].m_itprProc,sizeof(void *),g_watchList[i].m_elementcount,f);
	}
	fclose(f);
	printf("读取成功\n");
}
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
			exit(0);
            return 0;
		case WM_COMMAND:
#define		MSGPROC_COMMONLINES printf("请输入变量名(32个字符以下): ");\
								{ \
									char t[32]; \
									scanf("%s",t); \
									mbstowcs(g_watchList[CWatch::s_nWatch].m_name,t,30); \
								} \
								printf("请输入地址: ");\
								scanf("%i",&g_watchList[CWatch::s_nWatch].m_address);

			if((HWND)lParam==g_hBtnAddWatch){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					printf("添加一个查看...\n");
					MSGPROC_COMMONLINES;
					g_watchList[CWatch::s_nWatch].Show();
					++CWatch::s_nWatch;
					ShowWindow(g_hWnd,SW_SHOW);
				}
				//SendMessage(g_hBtnAddWatch,WM_PAINT,0,0);
			}else if((HWND)lParam==g_hBtnAddWatchByte){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					printf("添加一个字节...\n");
					MSGPROC_COMMONLINES;
//					g_watchList[CWatch::s_nWatch].m_buf=new BYTE;
					g_watchList[CWatch::s_nWatch].m_elementcount=1;
					g_watchList[CWatch::s_nWatch].m_size=1;
					g_watchList[CWatch::s_nWatch].m_itprProc[0]=InterpretChar;
					g_watchList[CWatch::s_nWatch].Show();
					++CWatch::s_nWatch;
					ShowWindow(g_hWnd,SW_SHOW);
				}
			}else if((HWND)lParam==g_hBtnAddWatchWord){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					printf("添加一个单字...\n");
					MSGPROC_COMMONLINES;
//					g_watchList[CWatch::s_nWatch].m_buf=(BYTE*)new WORD;
					g_watchList[CWatch::s_nWatch].m_elementcount=1;
					g_watchList[CWatch::s_nWatch].m_size=2;
					g_watchList[CWatch::s_nWatch].m_itprProc[0]=InterpretWord;
					g_watchList[CWatch::s_nWatch].Show();
					++CWatch::s_nWatch;
					ShowWindow(g_hWnd,SW_SHOW);
				}
			}else if((HWND)lParam==g_hBtnAddWatchDWord){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					printf("添加一个双字...\n");
					MSGPROC_COMMONLINES;
//					g_watchList[CWatch::s_nWatch].m_buf=(BYTE*)new DWORD;
					g_watchList[CWatch::s_nWatch].m_elementcount=1;
					g_watchList[CWatch::s_nWatch].m_size=4;
					g_watchList[CWatch::s_nWatch].m_itprProc[0]=InterpretDWord;
					g_watchList[CWatch::s_nWatch].Show();
					++CWatch::s_nWatch;
					ShowWindow(g_hWnd,SW_SHOW);
				}
			}else if((HWND)lParam==g_hBtnSetRefItv){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					printf("请输入刷新间隔(ms)");
					scanf("%i",&g_refItv);
					TCHAR t[16];
					SendMessage(g_hStaticRefItv,WM_SETTEXT,0,(LPARAM)_itow(g_refItv,t,10));
					ShowWindow(g_hWnd,SW_SHOW);
				}
			}else if((HWND)lParam==g_hBtnReconnect){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					Disconnect();
					Connect();
					ShowWindow(g_hWnd,SW_SHOW);
				}
			}else if((HWND)lParam==g_hBtnSave){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					printf("请输入文件名: ");
					char m[128];
					scanf("%s",m);
					TCHAR w[128];
					mbstowcs(w,m,128);
					Save(w);
					ShowWindow(g_hWnd,SW_SHOW);
				}
			}else if((HWND)lParam==g_hBtnLoad){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					printf("请输入文件名: ");
					char m[128];
					scanf("%s",m);
					TCHAR w[128];
					mbstowcs(w,m,128);
					for(long i=0;i<CWatch::s_nWatch;++i){
						g_watchList[i].Hide();
					}
					Load(w);
					for(long i=0;i<CWatch::s_nWatch;++i){
						g_watchList[i].Show();
					}
					ShowWindow(g_hWnd,SW_SHOW);
				}
			}else if((HWND)lParam==g_hBtnQuickSave){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					Save(L".QuickSave");
					ShowWindow(g_hWnd,SW_SHOW);
				}
			}else if((HWND)lParam==g_hBtnQuickLoad){
				if(wParam>>16==BN_CLICKED){
					ShowWindow(g_hWnd,SW_HIDE);
					for(long i=0;i<CWatch::s_nWatch;++i){
						g_watchList[i].Hide();
					}
					Load(L".QuickSave");
					for(long i=0;i<CWatch::s_nWatch;++i){
						g_watchList[i].Show();
					}
					ShowWindow(g_hWnd,SW_SHOW);
				}
			}
			return 0;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}

int _tmain(int argc, _TCHAR* argv[])
{
	{
		TCHAR cf[]=L"AutoWatch.txt";
		FILE *f=NULL;
		_wfopen_s(&f,cf,L"r");
		if(f){
			printf("已找到配置文件 %S , 读取配置中...\n",cf);
			TCHAR cmd[64];
			while(fwscanf(f,L"%s",cmd)!=EOF){
				if(!_wcsicmp(L"baudRate",cmd)){
					fwscanf(f,L"%ld",&g_baudRate);
				}
			}
			fclose(f);
			printf("配置读取完成\n");
			printf("Baud rate = %ld\n",g_baudRate);
		}else{
			printf("未找到配置文件 %S , 请手工输入配置...\n",cf);
			printf("Baud rate = ");
			scanf("%ld",&g_baudRate);
			printf("是否保存配置到 %S ?Y/N>",cf);
			char t[32];
			scanf("%s",t);
			if(t[0]=='Y'||t[0]=='y'){
				printf("已建立配置文件 %S , 保存配置中...\n",cf);
				_wfopen_s(&f,cf,L"w");
				fwprintf(f,L"baudRate %ld\n",g_baudRate);
				printf("保存完毕\n");
			}
		}
	}
//	Disconnect();
	// Register the window class.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC|CS_HREDRAW|CS_VREDRAW, MsgProc, 0L, 0L, 
	GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_APPWORKSPACE), NULL,
	L"RS232 AutoWatch", NULL };

	RegisterClassEx( &wc );

	// Create the application's window.
	HWND hWnd = CreateWindow( L"RS232 AutoWatch", L"RS232 AutoWatch", 
	WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
	GetDesktopWindow(), NULL, wc.hInstance, NULL );
	g_hWnd=hWnd;
	g_hFont=CreateFont(12,0,0,0,0,0,0,0,DEFAULT_CHARSET ,0,0,0,FIXED_PITCH,L"宋体");
	SendMessage(hWnd,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	g_hBtnAddWatch = CreateWindow(L"BUTTON",L"添加查看",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,0,0,96,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	g_hBtnAddWatchByte  = CreateWindow(L"BUTTON",L"添加字节",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,96,0,96,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	g_hBtnAddWatchWord  = CreateWindow(L"BUTTON",L"添加单字",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,2*96,0,96,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	g_hBtnAddWatchDWord = CreateWindow(L"BUTTON",L"添加双字",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,3*96,0,96,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	g_hStaticRefItv = CreateWindow(L"STATIC",L"100",WS_VISIBLE | WS_CHILD | SS_RIGHT,80,24,48,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	SendMessage(CreateWindow(L"STATIC",L"刷新间隔(ms)",WS_VISIBLE | WS_CHILD | SS_CENTER,0,24,80,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL),WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	g_hBtnSetRefItv = CreateWindow(L"BUTTON",L"重设",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,128,24,48,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	SendMessage(g_hBtnSetRefItv,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	SendMessage(g_hStaticRefItv,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	SendMessage(g_hBtnAddWatch,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	SendMessage(g_hBtnAddWatchByte,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	SendMessage(g_hBtnAddWatchWord,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	SendMessage(g_hBtnAddWatchDWord,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	g_hStaticComName = CreateWindow(L"STATIC",L"",WS_VISIBLE | WS_CHILD | SS_RIGHT,128+48,24,64,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	SendMessage(g_hStaticComName,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	g_hBtnReconnect = CreateWindow(L"BUTTON",L"重新连接",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,128+48+64,24,64,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	SendMessage(g_hBtnReconnect,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);

	g_hBtnSave = CreateWindow(L"BUTTON",L"保存查看",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,128+48+64+64,24,64,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	g_hBtnLoad = CreateWindow(L"BUTTON",L"读取查看",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,128+48+64+64*2,24,64,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	g_hBtnQuickSave = CreateWindow(L"BUTTON",L"快速保存",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,128+48+64+64*3,24,64,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	g_hBtnQuickLoad = CreateWindow(L"BUTTON",L"快速读取",WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_CENTER | BS_TEXT | BS_VCENTER ,128+48+64+64*4,24,64,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);

	SendMessage(g_hBtnSave,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	SendMessage(g_hBtnLoad,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	SendMessage(g_hBtnQuickSave,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	SendMessage(g_hBtnQuickLoad,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);

	g_hStaticFPS = CreateWindow(L"STATIC",L"",WS_VISIBLE | WS_CHILD | SS_RIGHT,128+48+64*6,24,64,24,hWnd,NULL,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
	SendMessage(g_hStaticFPS,WM_SETFONT,(WPARAM)g_hFont,(LPARAM)TRUE);
	Connect();

    {
        // Show the window
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

        // Enter the message loop
        MSG msg;
		while(true){
			if( PeekMessage( &msg, NULL, 0, 0 ,PM_REMOVE) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			static long last=0;
			long t=(clock()-last);
			if(t>g_refItv){
				last=clock();
				for(long i=0;i<CWatch::s_nWatch;++i){
					g_watchList[i].Refresh();
				}
				int a,b;
				TCHAR w[32];
				char m[32];
				_gcvt(1000.0/t,4,m);
				mbstowcs(w,m,32);
				SendMessage(g_hStaticFPS,WM_SETTEXT,0,(LPARAM)w);
				
			}
		}
    }

    UnregisterClass( L"RS232 AutoWatch", wc.hInstance );
	return 0;
}

