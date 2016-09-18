#include "Userlist.h"


Userlist::Userlist(void)
{

}

Userlist::~Userlist(void)
{

}

void Userlist::addUser(HANDLE h,TzhUser u)
{
	TzhUser*p=findUserByName(u.my_username.c_str());
	if(NULL==p)
		_lstUser.insert(make_pair(h,u));
}

TzhUser* Userlist::findUser(HANDLE h)
{
	DEQUE_USER_IT it=_lstUser.find(h);
	if(it!=_lstUser.end())
		return &it->second;
	else
		return NULL;
}

TzhUser* Userlist::findUserByName(const char* username)
{
	DEQUE_USER_IT it; 
	for(it=_lstUser.begin(); it!=_lstUser.end();it++ ) 
	{ 
		if(0==strcmp(it->second.my_username.c_str(),username)){ 
			return &it->second;
		}
	}
	return NULL;
}

void Userlist::removeUser(HANDLE h)
{
	DEQUE_USER_IT it=_lstUser.find(h);
	if(it!=_lstUser.end())
	{
		_lstUser.erase(it);
	}
}