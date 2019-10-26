#include<iostream>
#include"S_triangle.h"

int main() {
        double x_pt_1, y_pt_1, x_pt_2, y_pt_2, x_pt_3, y_pt_3;
        double x_pt_4, y_pt_4, x_pt_5, y_pt_5, x_pt_6, y_pt_6;
        double S = 0.0;

        std::cin >> x_pt_1 >> y_pt_1 >> x_pt_2 >> y_pt_2 >> x_pt_3 >> y_pt_3;
        std::cin >> x_pt_4 >> y_pt_4 >> x_pt_5 >> y_pt_5 >> x_pt_6 >> y_pt_6;

        Point_t<double> pt_1(x_pt_1, y_pt_1), pt_2(x_pt_2, y_pt_2), pt_3(x_pt_3, y_pt_3),
                        pt_4(x_pt_4, y_pt_4), pt_5(x_pt_5, y_pt_5), pt_6(x_pt_6, y_pt_6);

        Poligon_t<double> triangle_1(pt_1, pt_2, pt_3), triangle_2(pt_4, pt_5, pt_6);

        Clip_poligon<double>(triangle_1, triangle_2);;

        /*std::vector<Point_t<double>>::iterator it = triangle_1.pt_list.begin();
        while(it != triangle_1.pt_list.end()) {
                std::cout << it -> x << ";" << it -> y << "///";
                it++;
        }
        
        std::cout << std::endl;*/

        S = triangle_1.s_decart();

        std::cout << S << std::endl;

        return 0;
}

