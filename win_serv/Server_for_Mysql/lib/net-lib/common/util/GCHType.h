#pragma once

#include <list>
#include <map>
#include <vector>
#include <queue>
using namespace std;

typedef vector<INT_PTR>					GCH_Vector_Point;
typedef GCH_Vector_Point::iterator		GCH_Vector_Point_Ite;

typedef list<INT_PTR>					GCH_Lst_Point;
typedef GCH_Lst_Point::iterator			GCH_Lst_Point_Ite;

typedef map <DWORD, INT_PTR>			GCH_Map_Point;
typedef GCH_Map_Point::iterator			GCH_Map_Point_Ite;

typedef map <DWORD, int>				GCH_Map_Int;
typedef GCH_Map_Int::iterator			GCH_Map_Int_Ite;


#define   G9BMC_COINFO_CFG   "cfg\\coinfo.xml"
#define   PACKET_MAX_LENGTH			(64*1024)

struct G9ST_Pack_Resend
{
	char cbuf[PACKET_MAX_LENGTH];
	int  nLen;
	WORD wCmd;
	int  nPreTime;
	int  nTimes;
	WORD wNumber;
	int  nSeverType;
};



typedef list<G9ST_Pack_Resend>				G9Lst_Pack_Resend;

typedef G9Lst_Pack_Resend::iterator			G9Lst_Pack_Resend_Ite;




//”Œœ∑¿‡–Õ
enum G9EM_GAMESTYLE
{
	G9EM_WAR3_1V1=1,
	G9EM_WAR3_2V2,
	G9EM_WAR3_Dota,
	G9EM_WAR3_3C,
	G9EM_WAR3_3Country,
	G9EM_Starcraft_1V1,
	G9EM_Starcraft_2V2,
	G9EM_CS,

};
enum G9EM_SHOW_MSG
{
	G9EM_ADDSYSTEM_MSG,
	G9EM_ADD_MSG,
	G9EM_GAMEEVENT_MSG,
	G9EM_GAMERESULT_MSG,
	G9EM_ADDMSGTIP_MSG,
	G9EM_VIPTIP_MSG,

};