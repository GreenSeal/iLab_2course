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

	double dist_point(Point_t<T> pnt) {
		double dist;
		dist = (A*pnt.x + B*pnt.y + C*pnt.z + D)/sqrt(A*A + B*B + C*C);
		if(dist < 0) dist = -dist;
		return dist;
	};

	double angle_line(Line_t<T> line) {
		double sin_angle;
		sin_angle = (line.a_x*A + line.a_y*B + line.a_z*C)/(sqrt(A*A + B*B + C*C) * sqrt(line.a_x*line.a_x + line.a_y*line.a_y + line.a_z*line.a_z));
		if(sin_angle < 0) sin_angle = -sin_angle;
		return sin_angle;
	};

	Point_t<T> pnt_inters_line(Line_t<T> line) {
		double sin_a = angle_line(line);
		Point_t<T> line_pnt{line.x, line.y, line.z};
		double dist = dist_point(line_pnt);
		double lengh = dist/sin_a;
		Point_t<T> diff{line.a_x*lengh, line.a_y*lengh, line.a_z*lengh};
		Point_t<T> pnt_inters{line_pnt.x + diff.x, line_pnt.y + diff.y, line.z + diff.z};
		return pnt_inters;
	};
};

template <typename T> struct Poligon_t final {

        std::vector<Point_t<T>> pt_vector;
	Point_t<T> centre;
	int color;

	explicit Poligon_t(int col = 0) : pt_vector{}, color(col), centre{0, 0, 0} {};

        Poligon_t(Point_t<T> pnt_1, Point_t<T> pnt_2, Point_t<T> pnt_3, int col = 0) : pt_vector{pnt_1, pnt_2, pnt_3}, color(col) {
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
};

//Are you sure that += doesn't change rgh_pnt
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


//Check which side of the line the point lies
template <typename T> int Check_side(Plane_t<T> plane, Point_t<T> pt, int right_side) {
	int side;
	if((plane.A*pt.x + plane.B*pt.y + plane.C*pt.z + plane.D - tol) > 0) side = 1;
	
	else {
		if((plane.A*pt.x + plane.B*pt.y + plane.C*pt.z + plane.D + tol) < 0) side = -1;
		
		else side = 0;
	}

	if(side == 0) return 0;
	
	if(right_side == side) return 1;
	
	else return -1;
}

/*std::vector<Point_t<double>>::iterator it = triangle_1.pt_list.begin();
while(it != triangle_1.pt_list.end()) {
	std::cout << it -> x << ";" << it -> y << "///";
	it++;
}
	
std::cout << std::endl;*/
