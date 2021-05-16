#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>
#include<unordered_map>

std::vector<std::vector<int>> connect_list, connect_list_tr;
std::unordered_map<int, int> hash_table;

class dfs_t {

	// There are stored vertex indexes
	std::vector<bool> is_v_visited;
	
	std::vector<int> exit_time_order;

	std::vector<std::string> components;
	std::string cur_component;

	void dfs_cl(int v_idx) {
		is_v_visited.at(v_idx) = true;
		
		for (int i = 0; i < connect_list.at(v_idx).size(); ++i) {
			if(!is_v_visited.at(hash_table.at(connect_list.at(v_idx).at(i))))
				dfs_cl(hash_table.at(connect_list.at(v_idx).at(i)));
		}

		exit_time_order.push_back(connect_list.at(v_idx).at(0));
	}

	void dfs_clt(int v_idx) {
		is_v_visited.at(v_idx) = true;

		cur_component += std::to_string(connect_list_tr.at(v_idx).at(0)) + ", ";

		for(int i = 0; i < connect_list_tr.at(v_idx).size(); ++i) {
			if(!is_v_visited.at(hash_table.at(connect_list_tr.at(v_idx).at(i)))) {
				dfs_clt(hash_table.at(connect_list_tr.at(v_idx).at(i)));
			}
		}
	}
	
	void reboot_visited() {
		is_v_visited.assign(is_v_visited.size(), false);
	}

public:
	dfs_t() : is_v_visited(connect_list.size(), false), exit_time_order(), components(), cur_component() {}

	dfs_t(int v_num) : is_v_visited(v_num, false), exit_time_order(), components(), cur_component() {}

	void start_dfs_cl() {
		reboot_visited();
		
		for (int i = 0; i < connect_list.size(); ++i) {
			if(!is_v_visited.at(hash_table.at(connect_list.at(i).at(0))))
				dfs_cl(hash_table.at(connect_list.at(i).at(0)));
		}
	}

	void start_dfs_clt() {
		reboot_visited();

		for (int i = 0; i < exit_time_order.size(); ++i) {
			if(!is_v_visited.at(hash_table.at(exit_time_order.at(exit_time_order.size()-1-i)))) {
				dfs_clt(hash_table.at(exit_time_order.at(exit_time_order.size()-1-i)));
				cur_component.erase(cur_component.end()-2, cur_component.end());
				components.push_back(cur_component);
				cur_component = "";
			}
		}
	}

	void print_compinents() {
		for (int i = 0; i < components.size(); ++i) {
			std::cout << components.at(i) << std::endl;
		}
	}

};


void add_elem_to_ht(std::unordered_map<int, int>& hash_table, int elem) {
	if(hash_table.find(elem) == hash_table.end()) {
		std::vector<int> tmp_vec{elem};
		connect_list_tr.push_back({tmp_vec});
		hash_table[elem] = connect_list_tr.size() - 1;
	} 
}

bool cmp(const std::vector<int>& a, const std::vector<int>& b) {
	return a[0] < b[0];
}

int main (int argc, char** argv) {
	if(argc > 2) {
		std::cout << "Too many arguments given\n";
		std::exit(-1);
	}

	std::fstream input(argc == 2? argv[1] : "input.txt");
	if(!input.is_open()) {
		std::cout << "Couldn't open file!\n";
		std::exit(-1);
	}

	std::string token;
	
	while (!input.eof()) {
		input >> token;
		if (token.at(token.size() - 1) == ':') {

			token.pop_back();
			int cl_main_elem = std::stoi(token);
			std::vector<int> tmp_vec_cl{cl_main_elem};
			connect_list.push_back({tmp_vec_cl});

			add_elem_to_ht(hash_table, cl_main_elem);

			input >> token;
			for (; token != ";";) {
				int clt_main_elem = std::stoi(token);
				connect_list.at(connect_list.size() - 1).push_back(clt_main_elem);

				add_elem_to_ht(hash_table, clt_main_elem);
				
				connect_list_tr.at(hash_table.at(clt_main_elem)).push_back(cl_main_elem);

				input >> token;
			}
		}
	}

	std::sort(connect_list_tr.begin(), connect_list_tr.end(), cmp);
	for(int i = 0; i < connect_list_tr.size(); ++i) {
		hash_table[connect_list_tr[i][0]] = i;
	}

	dfs_t dfs{};
	dfs.start_dfs_cl();

	dfs.start_dfs_clt();

	dfs.print_compinents();

}