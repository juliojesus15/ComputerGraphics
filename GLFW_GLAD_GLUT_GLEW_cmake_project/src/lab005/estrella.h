#include <vector>
#include <iostream>
#include <math.h>       

#define PI 3.14159265

class Estrella: public Figura {
public:
    //std::vector<float> puntos;
    Estrella(float, float, float, float);
};

//Parametros (Radio de la estrealla, Vertice del centro de la estrella(x,y,z))
Estrella::Estrella(float radio, float x, float y, float z) {
    bool flag = true;
    float angulo = 30;
    std::vector<float> puntos = {};
    float tmp_x = (radio / 2) * cos(0 * PI / 180);
    float tmp_y = (radio / 2) * sin(0 * PI / 180);

    for (int i = 0; i < 10; i++) {
        puntos.insert(puntos.end(), { x, y, z });
        puntos.insert(puntos.end(), { tmp_x + x, tmp_y + y, z });

        tmp_x = flag ? radio * cos(angulo * PI / 180) : (radio / 2) * cos(angulo * PI / 180);
        tmp_y = flag ? radio * sin(angulo * PI / 180) : (radio / 2) * sin(angulo * PI / 180);

        puntos.insert(puntos.end(), { tmp_x + x, tmp_y + y, z });

        angulo = i < 5 ? angulo + 30 : angulo + 45;
        flag = !flag;
    }
    puntos.insert(puntos.end(), { x, y, z });
    this->puntos = puntos;
}
