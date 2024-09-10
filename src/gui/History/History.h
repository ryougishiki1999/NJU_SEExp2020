#pragma once
#include<string>
#include<vector>
const int ATTR_NUM=4;

struct item {
	std::string in_addr;
    std::string out_addr;
    std::string time;
	bool is_godmode;
    item():in_addr(""),out_addr(""),time(""),is_godmode(false){}
};

class History {
	std::string file_name;
	std::vector<item> history_list;
    bool is_loaded;
public:
    explicit History(std::string file = "./history.txt") :file_name(file){
        is_loaded=false;
    };
    void load_history();
	void clear_history();
    bool delete_his_by_index(unsigned int i);
	void add_history(item his);
	void write_history();
    int size();
    bool empty();
    item get(int i);
};
