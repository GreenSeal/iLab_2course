#include"Tr_inters.h"
#include<fstream>
#include<ctime>


template <typename T> Point_t<T> Find_lone_pnt(Plane_t<T> plane, Poligon_t<T> tr) {
	int res_1 = Check_side(plane, tr.pt_vector[0], 1),
	    res_2 = Check_side(plane, tr.pt_vector[1], 1),
	    res_3 = Check_side(plane, tr.pt_vector[2], 1);

	if(res_1 == res_2) return tr.pt_vector[2];
	
	else {
		if(res_1 == res_3) return tr.pt_vector[1];
		
		else return tr.pt_vector[0];
	}
	
}

template <typename T> void Insert_tr(std::vector<Poligon_t<T>>& tr_vector, int num) {
	
	for(int i  = 0; i < num; i++) {
		
		Point_t<T> pnt_1, pnt_2, pnt_3;
		std::cin >> pnt_1.x >> pnt_1.y >> pnt_1.z 
			 >> pnt_2.x >> pnt_2.y >> pnt_2.z
			 >> pnt_3.x >> pnt_3.y >> pnt_3.z;

		Poligon_t<T> triangle(pnt_1, pnt_2, pnt_3);
		
		tr_vector.push_back(triangle);
	}
}

int main() {

	double start = clock();
	int num;
	std::cin >> num;

	std::vector<Poligon_t<double>> tr_vector;

	//Insert triangles
	
	Insert_tr(tr_vector, num);

	for(int i = 0; i < num; i++) {

		//Build plane on main triagnle
		Plane_t<double> tr_plane(tr_vector[i].pt_vector[0], tr_vector[i].pt_vector[1], tr_vector[i].pt_vector[2]);

		for(int j = 0; j < num; j++) {

			if(j == i) continue;

			//If point lies on different side of main plane, then triangles don't intersec
			if(Check_side(tr_plane, tr_vector[j].pt_vector[0], 1) == Check_side(tr_plane, tr_vector[j].pt_vector[1], 1) 
			&& Check_side(tr_plane, tr_vector[j].pt_vector[1], 1) == Check_side(tr_plane, tr_vector[j].pt_vector[2], 1)) {}

			//Else they may be intersec
			else {
				//Find one point which lies alone on her side
				Point_t<double> lonely_pnt = Find_lone_pnt(tr_plane, tr_vector[j]);
				
				std::vector<Line_t<double>> sides;

				//Find sides which intersec main plane
				for(int k = 0; k < 3; k++) {
					Line_t<double> side(lonely_pnt, tr_vector[j].pt_vector[k]);

					if(side.a_x == side.a_x || side.a_y == side.a_y || side.a_z == side.a_z) sides.push_back(side);
				}

				//Find points which are intersec sides and main plane
				Point_t<double> pnt_segm_1, pnt_segm_2;
				pnt_segm_1 = tr_plane.pnt_inters_line(sides[0]);
				pnt_segm_2 = tr_plane.pnt_inters_line(sides[1]);

				//Vector of normal to main plane
				Point_t<double> norm_vec{tr_plane.A, tr_plane.B, tr_plane.C};

				Point_t<double> pnt_dop_1{}, pnt_dop_2{}, pnt_dop_3{}, pnt_dop_segm{};
				pnt_dop_1 = tr_vector[i].pt_vector[0] + norm_vec;
				pnt_dop_2 = tr_vector[i].pt_vector[1] + norm_vec;
				pnt_dop_3 = tr_vector[i].pt_vector[2] + norm_vec;
				pnt_dop_segm = pnt_segm_1 + norm_vec;

				//Build planes perpendicular sides and segment of intersec
				Plane_t<double> side_plane_1{tr_vector[i].pt_vector[0], tr_vector[i].pt_vector[1], pnt_dop_1},
						side_plane_2{tr_vector[i].pt_vector[1], tr_vector[i].pt_vector[2], pnt_dop_2},
						side_plane_3{tr_vector[i].pt_vector[2], tr_vector[i].pt_vector[0], pnt_dop_3},
						segm_plane{pnt_segm_1, pnt_segm_2, pnt_dop_segm};


				std::vector<Plane_t<double>> side_planes = {side_plane_1, side_plane_2, side_plane_3};

				//For more convenient sort out of points
				std::vector<Point_t<double>> pnt_vector = {tr_vector[i].pt_vector[0], tr_vector[i].pt_vector[1], tr_vector[i].pt_vector[2], tr_vector[i].pt_vector[0]};

				for(int k = 0; k < 3; k++) {
					if(Check_side(side_planes[k], pnt_segm_1, 1) != Check_side(side_planes[k], pnt_segm_2, 1) 
					&& Check_side(segm_plane, pnt_vector[k], 1) != Check_side(segm_plane, pnt_vector[k+1], 1)) {
						
						//std::cout << i << " inters with " << j << std::endl;
						tr_vector[i].color = 1;
						tr_vector[j].color = 1;
					}
				}
			}

		}
	}

	std::ofstream res;
	res.open("tests/res.txt");

	for(int i = 0; i < num; i++) {
		if(tr_vector[i].color == 1) res << i << std::endl;
	}
	
	res.close();
	

	double end = clock();
	double time = (end - start)/CLOCKS_PER_SEC;
	std::cout << "time: " << time << std::endl;

	return 0;
}
