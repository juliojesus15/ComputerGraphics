#include <vector>
#include <iostream>
#include <math.h>       

#define PI 3.14159265

class Pizza: public Figura {
public:
    //std::vector<float> puntos;
    Pizza(float, float, float, float, float);
};

//Parametros (Radio de la pizza, num. de cortes, Vertice del centro de la pizza(x,y,z))
Pizza::Pizza(float radio, float slice, float x, float y, float z) {
    int tam_slice = 360 / slice;
    float angulo = tam_slice;

    std::vector<float> puntos = {};

    float tmp_x = radio * cos(0 * PI / 180);
    float tmp_y = radio * sin(0 * PI / 180);

    for (int i = 0; i < slice; i++) {
        puntos.insert(puntos.end(), { x, y, z });
        puntos.insert(puntos.end(), { tmp_x + x, tmp_y + y, z });

        tmp_x = radio * cos(angulo * PI / 180);
        tmp_y = radio * sin(angulo * PI / 180);

        puntos.insert(puntos.end(), { tmp_x + x, tmp_y + y, z });

        angulo += tam_slice;
    }
    puntos.insert(puntos.end(), { x, y, z });
    this->puntos = puntos;
}

class Cuadrado : public Figura {
public:
    //std::vector<float> puntos;
    Cuadrado(float tam, float x, float y, float z);
};

Cuadrado::Cuadrado(float tam, float x, float y, float z) {
    std::vector<float> puntos = {};
    puntos.insert(puntos.end(), { x, y, z });
    puntos.insert(puntos.end(), { x + tam, y, z });
    puntos.insert(puntos.end(), { x + tam, y - tam, z });
    puntos.insert(puntos.end(), { x, y, z });
    puntos.insert(puntos.end(), { x + tam, y - tam, z });
    puntos.insert(puntos.end(), { x, y - tam, z });
    puntos.insert(puntos.end(), { x, y, z });
    this->puntos = puntos;
}

class Rombo : public Figura {
public:
    //std::vector<float> puntos;
    Rombo(float tam, float x, float y, float z);
};

Rombo::Rombo(float tam, float x, float y, float z) {
    std::vector<float> puntos = {};
    puntos.insert(puntos.end(), { x, y, z });
    puntos.insert(puntos.end(), { x + tam/2.0f, y - tam, z });
    puntos.insert(puntos.end(), { x, y-(tam*2.0f), z});
    puntos.insert(puntos.end(), { x, y, z });
    puntos.insert(puntos.end(), { x, y - (tam * 2.0f), z });
    puntos.insert(puntos.end(), { x - tam/2.0f, y - tam, z });
    puntos.insert(puntos.end(), { x, y, z });
    this->puntos = puntos;
}