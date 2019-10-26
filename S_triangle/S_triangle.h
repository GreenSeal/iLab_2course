#include<iostream>
#include<list>
#include<assert.h>
#include<cmath>
#include<vector>
#include<algorithm>


template <typename T> struct Point_t {

        T x, y;

        Point_t(T X = 0, T Y = 0) : x(X), y(Y) {};
};

template <typename T> struct Line_t {

        T A, B, C;

        Line_t(T a = 0, T b = 0, T c = 0) : A(a), B(b), C(c) {};
};

template <typename T> struct Poligon_t {

        std::vector<Point_t<T>> pt_list;

        Poligon_t(Point_t<T> pnt_1, Point_t<T> pnt_2, Point_t<T> pnt_3) {
                pt_list.push_back(pnt_1);
                pt_list.push_back(pnt_2);
                pt_list.push_back(pnt_3);
        };

        //Переводим полигон в центр
        int go_to_centre();

	//Переводим полигон из декартовых в полярные координаты
        int polar(); 

        //Переводим полигон из полярных в декартовы координаты
        int decart(); 

	//Находим с какой стороны от line лежит треугольник
        int ident_side(Line_t<T> line);

	//Площадь треугольника в декартовых координатах
	double s_decart();
};


template <typename T> int Poligon_t<T>::go_to_centre() {
        T x_cntr, y_cntr, sum_x = 0, sum_y = 0;
        int count = 0;
        typename std::vector<Point_t<T>>::iterator it = pt_list.begin();
        while(it != pt_list.end()) {
                sum_x += it -> x;
                sum_y += it -> y;
                it++;
                count++;
        }

        x_cntr = sum_x/count;
        y_cntr = sum_y/count;
        it = pt_list.begin();

        while(it != pt_list.end()) {
                it -> x = (it -> x) - x_cntr;
                it -> y = (it -> y) - y_cntr;
                it++;
        }
        return 0;
}

template <typename T> int Poligon_t<T>::polar() {
        typename std::vector<Point_t<T>>::iterator it = pt_list.begin();
        T x, y;
        while(it != pt_list.end()) {
                x = it -> x;
                y = it -> y;
                it -> x = atan2(y, x);
                it -> y = sqrt(x*x + y*y);
                it++;
        }
        return 0;
}

template <typename T> int Poligon_t<T>::decart() {
	typename std::vector<Point_t<T>>::iterator it = pt_list.begin();
        T x, y;
        while(it != pt_list.end()) {
                x = it -> x;
                y = it -> y;
                it -> x = y*cos(x);
                it -> y = y*sin(x);
                it++;
        }
        
	return 0;
}

template <typename T> int Poligon_t<T>::ident_side(Line_t<T> line) {
	
        typename std::vector<Point_t<T>>::iterator it = pt_list.begin();
        Point_t<T> pt;
        double const tol = 0.000001;
	
	while(true){
                pt = *it;
                if((line.A*pt.x + line.B*pt.y + line.C - tol) > 0.0) return 1;

                if((line.A*pt.x + line.B*pt.y + line.C + tol) < 0.0) return -1;

                it++;
        	assert(it != pt_list.end());
	}

} 


template <typename T> int Check_side(Line_t<T> line, Point_t<T> pt, int right_side) {
	double const tol = 0.00001;
	int side;
	if((line.A*pt.x + line.B*pt.y + line.C - tol) > 0) side = 1;
	
	else {
		if((line.A*pt.x + line.B*pt.y + line.C + tol) < 0) side = -1;
		
		else side = 0;
	}

	if(side == 0) return 0;
	
	if(right_side == side) return 1;
	
	else return -1;
}

//Добавляем точки пересечения
template <typename T> int Inters_triangle(Poligon_t<T>& triangle, Line_t<T> line, Poligon_t<T>& base_triangle) {

	auto it_1 = triangle.pt_list.begin(), it_2 = ++(triangle.pt_list.begin());
	Point_t<T> pt_1, pt_2, pt_inters;
	Line_t<T> side;
	double const tol = 0.000001;
	
	if((it_1 == triangle.pt_list.end()) || (it_2 == triangle.pt_list.end())) return 0;	
//Переписать с учетом сравнения для double - done
	while(true) {
		pt_1 = *it_1;
		pt_2 = *it_2;
		side.A = pt_1.y - pt_2.y;
		side.B = pt_2.x - pt_1.x;
		side.C = pt_1.x*pt_2.y - pt_2.x*pt_1.y;

		//Если прямые параллельны или совпадают, ничего не делаем
		if (((side.A*line.B - side.B*line.A) < tol) && ((side.B*line.A - side.A* line.B) < tol)); 

		else {
			pt_inters.x = -(side.C*line.B - line.C*side.B)/(side.A*line.B - line.A*side.B);
			pt_inters.y = -(side.A*line.C - line.A*side.C)/(side.A*line.B - line.A*side.B);
			
			//Если точка пересечениялежит между точками стороны, вставляем ее в полигон
			if((Check_side<T>(line, pt_1, 1) != Check_side<T>(line, pt_2, 1)) && (Check_side<T>(line, pt_1, 1) != 0) && (Check_side<T>(line, pt_2, 1) != 0)) {
				base_triangle.pt_list.push_back(pt_inters);
			}
		}
		
		it_1++;
		it_2++;

		if(it_2 == triangle.pt_list.end()) it_2 = triangle.pt_list.begin();
		if(it_2 == ++(triangle.pt_list.begin())) break;
			
	}
	
	return 0;
}

//Сравниваем точки по углу, т.е. сортируем их по часовой стрелке
template <typename T> bool cmp(Point_t<T> a, Point_t<T> b) {
	double tol = 0.000001;
	if(((a.y + tol) < b.y) && ((a.y - tol) < b.y)) return true;
	
	else return false;
}

//Обрезаем треугольник по стороне
template <typename T> int Clip_line(Poligon_t<T>& triangle_base, Line_t<T> line, int side) {
	auto it_1 = triangle_base.pt_list.begin(), it_2 = ++(triangle_base.pt_list.begin());
	Point_t<T> pt_1;
	Line_t<T> line_base;
	double const tol = 0.000001;
	Poligon_t<T> copy_triangle = triangle_base;
	//Треугольник-база используется здесь как полигон пересечения, треугольник который там лежал сохраняется в copy_triangle	
	//Выкидываем точки, которые лежат не с той стороны
	
	
	if((it_1 == triangle_base.pt_list.end()) || (it_2 == triangle_base.pt_list.end())) return 0;	
	
	while(true) {
		pt_1 = *it_1;
			
		if(Check_side<T>(line, pt_1, side) == -1) {
				
			if(it_2 != triangle_base.pt_list.end()) {
				triangle_base.pt_list.erase(it_1);
			}

			else {
				triangle_base.pt_list.erase(it_1);
				break;
			}
		}

		else {
				
			if(it_2 == triangle_base.pt_list.end()) break;
			it_1++;
			it_2++;
		}	
	}

	Inters_triangle(copy_triangle, line, triangle_base);

	return 0;
}

//Обрезаем один треугольник по другому, чтобы получить список точек многоугольника(пересечения треугольников)
template <typename T> int Clip_poligon(Poligon_t<T>& triangle_base, Poligon_t<T>& triangle_clip) {
	Line_t<T> line;
	Point_t<T> pt_1, pt_2;
	auto it_1 = triangle_clip.pt_list.begin(), it_2 = ++(triangle_clip.pt_list.begin());
		
	while(true) {
		Point_t<T> pt_1 = *it_1;
		Point_t<T> pt_2 = *it_2;
		
		assert((pt_1.x != pt_2.x) || (pt_1.y != pt_2.y));
		
		line.A = pt_1.y - pt_2.y;
		line.B = pt_2.x - pt_1.x;
		line.C = pt_1.x*pt_2.y - pt_2.x*pt_1.y;
		
		Clip_line<T>(triangle_base, line, triangle_clip.ident_side(line));
			
		it_1++;
		it_2++;
		
		if(it_2 == triangle_clip.pt_list.end()) it_2 = triangle_clip.pt_list.begin();
		if(it_2 == ++(triangle_clip.pt_list.begin())) break;		
	}

	if(triangle_base.pt_list.begin() == triangle_base.pt_list.end()) return 0;

	triangle_base.go_to_centre();
	
	triangle_base.polar();

	std::sort(triangle_base.pt_list.begin(), --triangle_base.pt_list.end(), [](auto x, auto y) { return cmp<T>(x, y);});

	triangle_base.decart();	
	//Проверить заполнение полигона по часовой стрелке - done
	
	return 0;
}

template <typename T> double Poligon_t<T>::s_decart() {
	double S = 0.0;
	auto it_1 = pt_list.begin(), it_2 = ++(pt_list.begin());
	if(it_1 == pt_list.end() || it_2 == pt_list.end()) return S;
	while(it_2 != pt_list.end()) {
		S = S + (it_1 -> x)*(it_2 -> y) - (it_2 -> x)*(it_1 -> y);
		it_1++;
		it_2++;
	}
	
	it_1 = pt_list.begin();
	it_2 = (--pt_list.end());
	S = (S + (it_2 -> x)*(it_1 -> y) - (it_1 -> x)*(it_2 -> y)) / 2.0;
	if(S < 0) S = -S;
	return S;
}

/*std::vector<Point_t<double>>::iterator it = triangle_1.pt_list.begin();
while(it != triangle_1.pt_list.end()) {
	std::cout << it -> x << ";" << it -> y << "///";
	it++;
}
	
std::cout << std::endl;*/
