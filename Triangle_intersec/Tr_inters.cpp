#include"Tr_inters.h"
#include<fstream>
#include<ctime>

template <typename T> void Insert_tr(Space_t<T>& tr_space, int num) {
	tr_space.num = num;

	for(int i  = 0; i < num; i++) {
		
		Point_t<T> pnt_1, pnt_2, pnt_3;
		std::cin >> pnt_1.x >> pnt_1.y >> pnt_1.z 
			 >> pnt_2.x >> pnt_2.y >> pnt_2.z
			 >> pnt_3.x >> pnt_3.y >> pnt_3.z;

		Poligon_t<T> triangle(pnt_1, pnt_2, pnt_3, i);
		
		tr_space.tr_vector.push_back(triangle);
	}
}

int main() {

	double start = clock();
	int num;
	std::cin >> num;

	Space_t<double> tr_space;

	//Insert triangles
	
	Insert_tr(tr_space, num);

	tr_space.find_max_tr();

	tr_space.find_inters_tr();

	for(int i = 0; i < num; i++) {
		if(tr_space.tr_vector[i].color == 1) std::cout << tr_space.tr_vector[i].idx << std::endl;
	}
	

	double end = clock();
	double time = (end - start)/CLOCKS_PER_SEC;
	//std::cout << "time: " << time << std::endl;

	return 0;
}
