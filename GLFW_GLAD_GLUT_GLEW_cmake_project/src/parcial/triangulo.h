#include <vector>
#include <iostream>
#include <math.h>       

#include "figura.h"

#define PI 3.14159265

class Triangulo: public Figura {
public:
    //std::vector<float> puntos;
    Triangulo(float tam, int angulo, float x, float y, float z);
};

//Parametros (tamaño del triangulo, Angulo de inclinacion, Vertice del centro del triangulo(x,y,z))
Triangulo::Triangulo(float tam, int angulo, float x, float y, float z) {    
    std::vector<float> puntos = {};
    bool flag = true;

    for (int i = 0; i < 4; i++) {
        float tmp_x = tam * cos(angulo * PI / 180);
        float tmp_y = tam * sin(angulo * PI / 180);

        puntos.insert(puntos.end(), { tmp_x + x, tmp_y + y, z });

        angulo = flag ? (angulo + 90) % 360 : (angulo + 180) % 360;
        flag = !flag;
    }
    this->puntos = puntos;
}

