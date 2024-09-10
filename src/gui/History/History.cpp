#include <iostream>
using namespace std;
#include "History.h"
#include<fstream>

/*****************************************************************************
This is source coode of History module.
History is called by HistoryDialog to provide history records and CURD of history.txt fils.
This file contains the initialization of all the members of History
and the implementation of all of its functions
******************************************************************************/

//load history from file
void  History::load_history() 
{
	ifstream fin;
	fin.open(file_name,ios::in);
	if (!fin) 
	{
		// if file does not exist,create a nenw file
		fin.close();
		ofstream fout(file_name);
		fout.close();
		fin.open(file_name,ios::in);
	}
	while (!fin.eof())
	{
		item temp;
		fin >> temp.in_addr >> temp.out_addr >>temp.time>> temp.is_godmode;
		history_list.push_back(temp);
	}
	history_list.pop_back();
	fin.close();
    is_loaded=true;
}

void History::clear_history()
{
    if(!is_loaded)return ;
	history_list.clear();
	ofstream fout(file_name);
	fout.close();
}

void History::add_history(item temp)
{
	ofstream fout(file_name,ios::app);
	fout << temp.in_addr<<" " << temp.out_addr <<" "
		<<temp.time<<" "<<temp.is_godmode<<endl;
	fout.close();
}

//just delete in the history list,not update file content
//the write_history function is always called after a series of this operation
bool History::delete_his_by_index(unsigned int i)
{
    if(!is_loaded)return false;
    if (i >=history_list.size())return false;
	history_list.erase(history_list.begin() + i);
    write_history();
	return true;
}

//I advise you to use this function 
//only after a series of call of delete_by_index
void History::write_history()
{
     if(!is_loaded)return ;
	ofstream fout(file_name);
    for (unsigned int i = 0;i < history_list.size();i++) 
	{
		fout << history_list[i].in_addr<<" " << history_list[i].out_addr<<" " 
			<<history_list[i].time<<" "<< history_list[i].is_godmode << endl;
	}
	fout.close();
}


int History::size()
{
     if(!is_loaded)return 0;
    return this->history_list.size();
}

bool History::empty()
{
    if(!is_loaded)return true;
    return this->history_list.empty();
}
item History::get(int i)
{
    item temp;
    if(!is_loaded)return temp;
    return this->history_list[i];
}


