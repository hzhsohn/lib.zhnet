#pragma once

#include <iostream>
#include <map>
#include <windows.h>

using namespace std;

struct TzhUser{
	HANDLE h;
	string my_username;
};

typedef map<HANDLE,TzhUser>  DEQUE_USER;
typedef DEQUE_USER::iterator  DEQUE_USER_IT;

class Userlist
{
	DEQUE_USER _lstUser;

public:
	Userlist(void);
	~Userlist(void);

	void addUser(HANDLE h,TzhUser u);
	TzhUser* findUser(HANDLE h);
	TzhUser* findUserByName(const char* username);
	void removeUser(HANDLE h);
};

