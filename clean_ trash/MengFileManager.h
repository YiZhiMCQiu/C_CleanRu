#include<iostream>
#include<algorithm>
#include<filesystem>
#include"system_clean.h"
using namespace std;

class MScreen {
public:
	MScreen(MThreadPool* tP,map<string,string>& target_list);
	void startClean();
	void showMenu();
	void showDetails();
	

	MFileClean* file_clean = nullptr;
};



