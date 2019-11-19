#include<iostream>
#include<assert.h>
#include<cmath>
#include<vector>
#include<algorithm>

double const tol = 0.000001;

template <typename T> struct Point_t final {

        T x, y, z;

        explicit Point_t(T X = 0, T Y = 0, T Z = 0) : x(X), y(Y), z(Z) {};
};


template <typename T> Point_t<T> operator+(const Point_t<T>& lft_pnt, const Point_t<T>& rgh_pnt) {
	Point_t<T> buf{lft_pnt};
	
	buf.x += rgh_pnt.x;
	buf.y += rgh_pnt.y;
	buf.z += rgh_pnt.z;
	
	return buf;
}

template <typename T> Point_t<T> operator-(const Point_t<T>& lft_pnt, const Point_t<T>& rgh_pnt) {
	Point_t<T> buf{lft_pnt};
	
	buf.x -= rgh_pnt.x;
	buf.y -= rgh_pnt.y;
	buf.z -= rgh_pnt.z;
	
	return buf;
}

template <typename T> struct Line_t final {

        T a_x, a_y, a_z, x, y, z;

        explicit Line_t(T A_x = 0, T A_y = 0, T A_z = 0, T X = 0, T Y = 0, T Z = 0) : a_x(A_x), a_y(A_y), a_z(A_z), x(X), y(Y), z(Z) {};

	Line_t(Point_t<T> pnt_1, Point_t<T> pnt_2) : x(pnt_1.x), y(pnt_1.y), z(pnt_1.z) {
		double vec_mod = sqrt((pnt_2.x-pnt_1.x)*(pnt_2.x-pnt_1.x) + (pnt_2.y-pnt_1.y)*(pnt_2.y-pnt_1.y) + (pnt_2.z-pnt_1.z)*(pnt_2.z-pnt_1.z));
		a_x = (pnt_2.x - pnt_1.x)/vec_mod;
		a_y = (pnt_2.y - pnt_1.y)/vec_mod;
		a_z = (pnt_2.z - pnt_1.z)/vec_mod;
	};
};

template <typename T> struct Plane_t final {
	T A, B, C, D;

	explicit Plane_t (T a = 0, T b = 0, T c = 0, T d = 0) : A(a), B(b), C(c), D(d) {};
	
	Plane_t(Point_t<T> pnt_1, Point_t<T> pnt_2, Point_t<T> pnt_3) {
		
		A = (pnt_2.y-pnt_1.y)*(pnt_3.z-pnt_1.z) - (pnt_3.y-pnt_1.y)*(pnt_2.z-pnt_1.z),
		B = (pnt_2.z-pnt_1.z)*(pnt_3.x-pnt_1.x) - (pnt_2.x-pnt_1.x)*(pnt_3.z-pnt_1.z),
		C = (pnt_2.x-pnt_1.x)*(pnt_3.y-pnt_1.y) - (pnt_2.y-pnt_1.y)*(pnt_3.x-pnt_1.x),
		D = pnt_1.x*((pnt_3.y-pnt_1.y)*(pnt_2.z-pnt_1.z) - (pnt_2.y-pnt_1.y)*(pnt_3.z-pnt_1.z)) + 
		    pnt_1.y*((pnt_2.x-pnt_1.x)*(pnt_3.z-pnt_1.z) - (pnt_2.z-pnt_1.z)*(pnt_3.x-pnt_1.x)) + 
		    pnt_1.z*((pnt_2.y-pnt_1.y)*(pnt_3.x-pnt_1.x) - (pnt_2.x-pnt_1.x)*(pnt_3.y-pnt_1.y));
	};

	//Check which side of the line the point lie
	int check_side(Point_t<T> pnt, int right_side);

	//Find distanse btw plane and point
	double dist_point(Point_t<T> pnt);
	
	//Find angle btw plane and line
	double angle_line(Line_t<T> line);

	//Find points of intersec of plane and line
	Point_t<T> pnt_inters_line(Line_t<T> line);

	//Find pnt which lies alone in her side
	//Point_t<T> find_lone_pnt(Poligon_t<T> tr);
};


template <typename T> int Plane_t<T>::check_side(Point_t<T> pt, int right_side) {
	int side;
	if((A*pt.x + B*pt.y + C*pt.z + D - tol) > 0) side = 1;
	
	else {
		if((A*pt.x + B*pt.y + C*pt.z + D + tol) < 0) side = -1;
		
		else side = 0;
	}

	if(side == 0) return 0;
	
	if(right_side == side) return 1;
	
	else return -1;
}

template <typename T> double Plane_t<T>::dist_point(Point_t<T> pnt) {
	double dist;
	dist = (A*pnt.x + B*pnt.y + C*pnt.z + D)/sqrt(A*A + B*B + C*C);
	if(dist < 0) dist = -dist;
	return dist;
}

template <typename T> double Plane_t<T>::angle_line(Line_t<T> line) {
	double sin_angle;
	sin_angle = (line.a_x*A + line.a_y*B + line.a_z*C)/(sqrt(A*A + B*B + C*C) * sqrt(line.a_x*line.a_x + line.a_y*line.a_y + line.a_z*line.a_z));
	if(sin_angle < 0) sin_angle = -sin_angle;
	return sin_angle;
}


template <typename T> Point_t<T> Plane_t<T>::pnt_inters_line(Line_t<T> line) {
	double sin_a = angle_line(line);
	Point_t<T> line_pnt{line.x, line.y, line.z};
	double dist = dist_point(line_pnt);
	double lengh = dist/sin_a;
	Point_t<T> diff{line.a_x*lengh, line.a_y*lengh, line.a_z*lengh};
	Point_t<T> pnt_inters{line_pnt.x + diff.x, line_pnt.y + diff.y, line.z + diff.z};
	return pnt_inters;
}

/*template <typename T> Point_t<T> Plane_t<T>::find_lone_pnt(Poligon_t<T> tr) {
	
	int res_1 = plane.check_side(tr.pt_vector[0], 1),
	    res_2 = plane.check_side(tr.pt_vector[1], 1),
	    res_3 = plane.check_side(tr.pt_vector[2], 1);

	if(res_1 == res_2) return tr.pt_vector[2];
	
	else {
		if(res_1 == res_3) return tr.pt_vector[1];
		
		else return tr.pt_vector[0];
	}
}*/

template <typename T> struct Poligon_t final {

        std::vector<Point_t<T>> pt_vector;
	Point_t<T> centre;
	int color, idx;

	explicit Poligon_t(int col = 0, int Idx = 0) : pt_vector{}, color(col), centre{0, 0, 0}, idx(Idx) {};

        Poligon_t(Point_t<T> pnt_1, Point_t<T> pnt_2, Point_t<T> pnt_3, int Idx = 0, int col = 0) : pt_vector{pnt_1, pnt_2, pnt_3}, color(col), idx(Idx) {
		T sum_x = 0, sum_y = 0, sum_z = 0;
		int count = 0;
		auto it = pt_vector.begin();
		while(it != pt_vector.end()) {
			sum_x += it -> x;
			sum_y += it -> y;
			sum_z += it -> z;
			it++;
			count++;
		}

		centre.x = sum_x/count;
		centre.y = sum_y/count;
		centre.z = sum_z/count;
	};

	//Find where take place other poligon from line
        int ident_side(Plane_t<T> plane) const;

	Plane_t<T> get_plane() const;
};

template <typename T> int Poligon_t<T>::ident_side(Plane_t<T> plane) const {
	
        auto it = pt_vector.begin();
        Point_t<T> pt;
	
	while(true) {
                pt = *it;
                if((plane.A*pt.x + plane.B*pt.y + plane.C*pt.z + plane.D - tol) > 0.0) return 1;

                if((plane.A*pt.x + plane.B*pt.y + plane.C*pt.z + plane.D + tol) < 0.0) return -1;

                it++;
        	assert(it != pt_vector.end());
	}

}

template <typename T> Plane_t<T> Poligon_t<T>::get_plane() const {
	Plane_t<T> plane{pt_vector[0], pt_vector[1], pt_vector[2]};
	return plane;
}

template <typename T> struct Space_t final {
	
	std::vector<Poligon_t<T>> tr_vector;
	int num;
	double max_dist;

	void find_max_tr();
	Point_t<T> find_lone_pnt(Plane_t<T> plane, Poligon_t<T> tr);
	int is_near(int i, int j, int& j_mem);
	int is_mb_inters(Plane_t<T> tr_plane, int j);
	bool cmp_tr(const Poligon_t<T>& a, const Poligon_t<T>& b);
	void check_inters(Plane_t<T> tr_plane, int i, int j);
	void find_inters_tr();
		
};

template <typename T> void Space_t<T>::find_max_tr() {
	double max_dist_buf = 0, buf_dist = 0;

	for(int i = 0; i < num; i++) {
		for(int j = 0; j < 3; j++) {
			Point_t<T> buf_pnt = tr_vector[i].pt_vector[j] - tr_vector[i].centre;
			buf_dist = sqrt(buf_pnt.x*buf_pnt.x + buf_pnt.y*buf_pnt.y + buf_pnt.z*buf_pnt.z);
			if(buf_dist > max_dist_buf) max_dist_buf = buf_dist;
		}
	}
	max_dist = max_dist_buf;
}

template <typename T> bool Space_t<T>::cmp_tr(const Poligon_t<T>& a, const Poligon_t<T>& b) {
	return(a.centre.x < b.centre.x);
}

template <typename T> Point_t<T> Space_t<T>::find_lone_pnt(Plane_t<T> plane, Poligon_t<T> tr) {
	
	int res_1 = plane.check_side(tr.pt_vector[0], 1),
	    res_2 = plane.check_side(tr.pt_vector[1], 1),
	    res_3 = plane.check_side(tr.pt_vector[2], 1);

	if(res_1 == res_2) return tr.pt_vector[2];
	
	else {
		if(res_1 == res_3) return tr.pt_vector[1];
		
		else return tr.pt_vector[0];
	}
}

template <typename T> int Space_t<T>::is_near(int i, int j, int& j_mem) {
	if(j == i) return 0;
	if(tr_vector[j].centre.x < tr_vector[i].centre.x - 2.0 * max_dist) {
		j_mem = j;
		return 0;
	}

	if(tr_vector[j].centre.x > tr_vector[i].centre.x + 2.0 * max_dist) return -1;
	Point_t<T> pnt_dist= tr_vector[j].centre - tr_vector[i].centre;
	double tr_dist = sqrt(pnt_dist.x*pnt_dist.x + pnt_dist.y*pnt_dist.y + pnt_dist.z*pnt_dist.z);

	if(tr_dist > 2.0 * max_dist) return 0;

	return 1;
}

template <typename T> int Space_t<T>::is_mb_inters(Plane_t<T> tr_plane, int j) {
	if((tr_plane.check_side(tr_vector[j].pt_vector[0], 1) == tr_plane.check_side(tr_vector[j].pt_vector[1], 1)) 
	&& (tr_plane.check_side(tr_vector[j].pt_vector[1], 1) == tr_plane.check_side(tr_vector[j].pt_vector[2], 1))) return 0;

	return 1;
}

template <typename T> void Space_t<T>::check_inters(Plane_t<T> tr_plane, int i, int j) {
	if(is_mb_inters(tr_plane, j) == 1) {
		
		//Find one point which lies alone on her side
		Point_t<T> lonely_pnt = find_lone_pnt(tr_plane, tr_vector[j]);
				
		std::vector<Line_t<T>> sides;

		//Find sides which intersec main plane
		for(int k = 0; k < 3; k++) {
			Line_t<T> side(lonely_pnt, tr_vector[j].pt_vector[k]);

			if(side.a_x == side.a_x || side.a_y == side.a_y || side.a_z == side.a_z) sides.push_back(side);
		}

		//Find points which are intersec sides and main plane
		Point_t<T> pnt_segm_1, pnt_segm_2;
		pnt_segm_1 = tr_plane.pnt_inters_line(sides[0]);
		pnt_segm_2 = tr_plane.pnt_inters_line(sides[1]);

		//Vector of normal to main plane
		Point_t<T> norm_vec{tr_plane.A, tr_plane.B, tr_plane.C};

		Point_t<T> pnt_dop_1{}, pnt_dop_2{}, pnt_dop_3{}, pnt_dop_segm{};
		pnt_dop_1 = tr_vector[i].pt_vector[0] + norm_vec;
		pnt_dop_2 = tr_vector[i].pt_vector[1] + norm_vec;
		pnt_dop_3 = tr_vector[i].pt_vector[2] + norm_vec;
		pnt_dop_segm = pnt_segm_1 + norm_vec;

		//Build planes perpendicular sides and segment of intersec
		Plane_t<T> side_plane_1{tr_vector[i].pt_vector[0], tr_vector[i].pt_vector[1], pnt_dop_1};
		Plane_t<T> side_plane_2{tr_vector[i].pt_vector[1], tr_vector[i].pt_vector[2], pnt_dop_2};
		Plane_t<T> side_plane_3{tr_vector[i].pt_vector[2], tr_vector[i].pt_vector[0], pnt_dop_3};
		Plane_t<T> segm_plane{pnt_segm_1, pnt_segm_2, pnt_dop_segm};


		std::vector<Plane_t<double>> side_planes = {side_plane_1, side_plane_2, side_plane_3};

		//For more convenient sort out of points
		std::vector<Point_t<double>> pnt_vector = {tr_vector[i].pt_vector[0], tr_vector[i].pt_vector[1], tr_vector[i].pt_vector[2], tr_vector[i].pt_vector[0]};

		for(int k = 0; k < 3; k++) {
			if(side_planes[k].check_side(pnt_segm_1, 1) != side_planes[k].check_side(pnt_segm_2, 1) 
			&& segm_plane.check_side(pnt_vector[k], 1) != segm_plane.check_side(pnt_vector[k+1], 1)) {
						
				//std::cout << i << " inters with " << j << std::endl;
				tr_vector[i].color = 1;
				tr_vector[j].color = 1;
			}
		}
	}
}

template <typename T> void Space_t<T>::find_inters_tr() {
	int j_mem = 0;
	find_max_tr();
	std::sort(tr_vector.begin(), tr_vector.end(), [this](auto x, auto y) {return cmp_tr(x, y);});

	for(int i = 0; i < num; i++) {
		Plane_t<T> tr_plane = tr_vector[i].get_plane();

		for(int j = j_mem; j < num; j++) {
			if(is_near(i, j, j_mem) == -1) break;

			else {
				if(is_near(i, j, j_mem) == 1) {	
					check_inters(tr_plane, i, j);
				}

				else {}
			}
		}
	}
}

/*std::vector<Point_t<double>>::iterator it = triangle_1.pt_list.begin();
while(it != triangle_1.pt_list.end()) {
	std::cout << it -> x << ";" << it -> y << "///";
	it++;
}
	
std::cout << std::endl;*/
