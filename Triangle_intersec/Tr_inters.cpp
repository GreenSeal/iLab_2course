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

		Poligon_t<T> triangle(pnt_1, pnt_2, pnt_3, i);
		
		tr_vector.push_back(triangle);
	}
}

template <typename T> double Find_max_tr(const std::vector<Poligon_t<T>>& tr_vector, int num) {
	double max_dist = 0, buf_dist = 0;
	
	for(int i = 0; i < num; i++) {
		for(int j = 0; j < 3; j++) {
			Point_t<T> buf_pnt = tr_vector[i].pt_vector[j] - tr_vector[i].centre;
			buf_dist = sqrt(buf_pnt.x*buf_pnt.x + buf_pnt.y*buf_pnt.y + buf_pnt.z*buf_pnt.z);
			if (buf_dist > max_dist) max_dist = buf_dist;
		}
	}

	return max_dist;
}

template <typename T> bool cmp_tr (const Poligon_t<T>& a, const Poligon_t<T>& b) {

	if(a.centre.x < b.centre.x) return true;

	else return false;
}

int main() {

	double start = clock();
	int num, j_mem = 0;
	std::cin >> num;

	std::vector<Poligon_t<double>> tr_vector;

	//Insert triangles
	
	Insert_tr(tr_vector, num);

	double max_dist = Find_max_tr(tr_vector, num);

	std::sort(tr_vector.begin(), tr_vector.end(), [](auto x, auto y) {return cmp_tr(x, y);});

	for(int i = 0; i < num; i++) {

		//Build plane on main triagnle
		Plane_t<double> tr_plane(tr_vector[i].pt_vector[0], tr_vector[i].pt_vector[1], tr_vector[i].pt_vector[2]);

		for(int j = j_mem; j < num; j++) {

			if(j == i) continue;

			//double centre_dist_i = sqrt(tr_vector[i].centre.x*tr_vector[i].centre.x + tr_vector[i].centre.y*tr_vector[i].centre.y + tr_vector[i].centre.z*tr_vector[i].centre.z),
			//       centre_dist_j = sqrt(tr_vector[j].centre.x*tr_vector[j].centre.x + tr_vector[j].centre.y*tr_vector[j].centre.y + tr_vector[j].centre.z*tr_vector[j].centre.z);

			if(tr_vector[j].centre.x < tr_vector[i].centre.x - 2.0 * max_dist) {
				j_mem = j;
			}

			else{

			if(tr_vector[j].centre.x > tr_vector[i].centre.x + 2.0 * max_dist) {break;}

			else{

			Point_t<double> pnt_dist = tr_vector[j].centre - tr_vector[i].centre;
			double tr_dist = sqrt(pnt_dist.x*pnt_dist.x + pnt_dist.y*pnt_dist.y + pnt_dist.z*pnt_dist.z);

			if(tr_dist > 2.0 * max_dist) {}

			else {
			       
			
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
			}

		}
	}

	/*std::ofstream res;
	res.open("tests/res.txt");

	for(int i = 0; i < num; i++) {
		if(tr_vector[i].color == 1) res << tr_vector[i].idx << std::endl;
	}
	
	res.close();*/

	for(int i = 0; i < num; i++) {
		if(tr_vector[i].color == 1) std::cout << tr_vector[i].idx << std::endl;
	}
	

	double end = clock();
	double time = (end - start)/CLOCKS_PER_SEC;
	//std::cout << "time: " << time << std::endl;

	return 0;
}
