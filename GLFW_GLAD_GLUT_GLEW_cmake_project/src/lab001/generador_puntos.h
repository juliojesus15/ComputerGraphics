#include <vector>
#include <iostream>
#include <math.h>       

#define PI 3.14159265

class Estrella {
public:
    std::vector<float> GenerarEstrella(float radio, float x, float y, float z) {
        bool flag = false;
        float angulo = 0;
        std::vector<float> puntos = {};        
        for (int i = 0; i < 11; i++) {
            float tmp_x = flag ? radio * cos(angulo * PI / 180) : (radio / 2) * cos(angulo * PI / 180);
            float tmp_y = flag ? radio * sin(angulo * PI / 180) : (radio / 2) * sin(angulo * PI / 180);
            puntos.insert(puntos.end(), { tmp_x + x, tmp_y + y, z });
            angulo = i<6 ? angulo + 30: angulo + 45;
            flag = !flag;
        }
        return puntos;
    }
};


class Triangulo {
public:
    std::vector<float> GenerarTriangulo(float tam, int angulo, float x, float y, float z) {
        std::vector<float> puntos = {};
        bool flag = true;
        for (int i = 0; i < 4; i++) {
            float tmp_x = tam * cos(angulo * PI / 180);
            float tmp_y = tam * sin(angulo * PI / 180);
            puntos.insert(puntos.end(), { tmp_x + x, tmp_y + y, z });
            angulo = flag ? (angulo + 90) % 360 : (angulo + 180) % 360;
            flag = !flag;
        }
        return puntos;
    }
};


class Pizza {
public:
    std::vector<int> indices;
    std::vector<float> puntos;

    std::vector<float> GenerarPizza(float radio, float slice, float x, float y, float z) {
        int tam_slice = 360 / slice;
        std::vector<float> puntos = {};
        float tmp_x = radio * cos(0 * PI / 180);
        float tmp_y = radio * sin(0 * PI / 180);
        float angulo = tam_slice;

        for (int i = 0; i < slice; i++) {
            puntos.insert(puntos.end(), { x, y, z });
            puntos.insert(puntos.end(), { tmp_x + x, tmp_y + y, z });
            tmp_x = radio * cos(angulo * PI / 180);
            tmp_y = radio * sin(angulo * PI / 180);
            puntos.insert(puntos.end(), { tmp_x + x, tmp_y + y, z });
            puntos.insert(puntos.end(), { x, y, z });
            angulo += tam_slice;
        }
        return puntos;
    }

    std::vector<float> GenerarIndicesPizza(float radio, int slice, float x, float y, float z) {
        int tam_slice = 360 / slice;
        std::vector<float> puntos = { x, y, z };
        std::vector<int> indices = {};
        float angulo = 0;        
        for (int i = 0; i < slice; i++) {            
            float tmp_x = (radio * cos(angulo * PI / 180)) + x;
            float tmp_y = (radio * sin(angulo * PI / 180)) + y;
            puntos.insert(puntos.end(), { tmp_x, tmp_y, z });        
            angulo += tam_slice;
        }
        
        for (int i = 1; i < slice+1; i++) {
            indices.insert(indices.end(), { 0, i, (i % slice) + 1 });
        }

        this->indices = indices;
        this->puntos = puntos;        
        return puntos;
    }
};