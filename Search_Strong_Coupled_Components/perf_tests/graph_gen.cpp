#include<assert.h>
#include<vector>
#include<fstream>
#include<string>

int main() {
	int node_num = 4;
	std::vector<std::vector<int>> graph(4);

	for(int file_num = 0; node_num < 1000000; node_num*=2, ++file_num) {
		std::ofstream output{};
		if(file_num < 10) {
			std::ofstream output1{"0" + std::to_string(file_num) + ".dat"};
			output = std::move(output1);
		} else {
			std::ofstream output1{std::to_string(file_num) + ".dat"};
			output = std::move(output1);
		}
		for(int i = 1; i < node_num; ++i) {
			output << std::to_string(i) + ": ";
			for(int j = i; j < rand()%node_num; ++j) {
				output << std::to_string(j + 1) + " ";
			}
			output << "; ";
		}
		
	}	

}