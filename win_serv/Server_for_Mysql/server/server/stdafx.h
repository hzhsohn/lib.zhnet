// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once


#define ID_NC                           9998
#define IDI_MAIN_ICO                    3
#define IDC_POPMENU                     1001


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#include <stdio.h>
#include <tchar.h>

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#include <afxdisp.h>        // MFC �Զ�����

#include "GCSNetWork.h"

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#define msgprint(format,...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);\
									_tprintf(_T(format),__VA_ARGS__);\
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED )
#define warnprint(format,...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN  |FOREGROUND_RED);\
									_tprintf(_T(format),__VA_ARGS__);\
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED )
#define alterprint(format,...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);\
									_tprintf(_T(format),__VA_ARGS__);\
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED )

#define msgshow(format,...)		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);\
									GCH_TRACE(_T(""),_T(format),__VA_ARGS__);\
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED )
#define warnlog(format,...)		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN  |FOREGROUND_RED);\
									GCH_TRACE(_T(""),_T(format),__VA_ARGS__);\
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED )
#define setlog(format,...)		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);\
									GCH_TRACE(_T(""),_T(format),__VA_ARGS__);\
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED )
#define errlog(format,...)		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);\
									GCH_TRACE(_T(""),_T(format),__VA_ARGS__);\
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED )

#define input(str)				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);\
									_tscanf(_T("%s"),str);fflush(stdin);fflush(stdout);\
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED )

