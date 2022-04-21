#include <vector>
#include <iostream>
#include <math.h>

class Vitral {
public:
    std::vector<float> backgroung;
    std::vector<unsigned int> idx_margen, idx_background;

    std::vector<float> tallo;
    std::vector<unsigned int> idx_tallo_margen, idx_tallo_background;

    std::vector<float> petalo, petalo_izq, petalo_der;
    std::vector<unsigned int> idx_petalo, idx_petalo_izq, idx_petalo_der;

    std::vector<float> hoja_izq, hoja_der;
    std::vector<unsigned int> idx_hoja_izq, idx_hoja_der;

    float altura, ancho;
    float x, y, z;

    Vitral(float altura, float ancho, float x, float y, float z);

    void generar_background();

    void generar_tallo(float x, float y, float ancho_tallo);

    void generar_petalo_izquierda(float, float);
    void generar_petalo_derecha(float, float);
    void generar_petalo(float, float);

    void generar_hoja_izquierda(float, float);
    void generar_hoja_derecha(float, float);

private:
    std::vector<float> concatenar_vectores(std::vector<float> a, std::vector<float> b);
    std::vector<unsigned int> generar_idx(std::vector<float> a, std::vector<float> b, bool flag);
    std::vector<float> generar_elipse(float h, float k, float a, float b, bool eje_x, bool eje_y);
};

/* 
    * Ingresar por parametro el margen del marco(altura, ancho)
    * Ingresar el centro del marco(x,y,z)
*/
Vitral::Vitral(float altura, float ancho, float x, float y, float z) {
    std::cout << ":D" << std::endl;
    this->altura = altura;
    this->ancho = ancho;
    this->x = x;
    this->y = y;
    this->z = z;

    //BG
    generar_background();
    
    //Tallo
    generar_tallo(0.0f, -0.1f, 0.09f);

    //Flor
    generar_petalo(0.0f, 0.00f);
    generar_petalo_izquierda(0.0f, 0.8f); 
    generar_petalo_derecha(0.5f, -0.1f);
    
    //Ramas
    generar_hoja_izquierda(-0.05f, 0.3f);
    generar_hoja_derecha(0.7f,-0.8f);
}

// Genera un bg en base a la altura y ancho ingresados por parametro en el constructor
void Vitral::generar_background() { 
    float tmp_x = ancho / 2;
    float tmp_y = altura / 2;
   
    this->backgroung.insert(this->backgroung.end(), { x - tmp_x,  y + tmp_y, 0.0f });
    this->backgroung.insert(this->backgroung.end(), { x + tmp_x,  y + tmp_y, 0.0f });
    this->backgroung.insert(this->backgroung.end(), { x + tmp_x,  y - tmp_y, 0.0f });
    this->backgroung.insert(this->backgroung.end(), { x - tmp_x,  y - tmp_y, 0.0f });

    this->idx_margen.insert(this->idx_margen.end(), { 0,1,2,3,0});
    this->idx_background.insert(this->idx_background.end(), { 0,1,3,1,2,3 });
}

// Se indica la altura a la que empezara el tallo, el ultimo parametro indica el ancho del tallo (0.08 para cada lado)
void Vitral::generar_tallo(float x_inicio, float y_inicio, float ancho) {
    float tmp_x = ancho / 2;
    float tmp_y = (altura / 2) + y_inicio;

    this->tallo.insert(this->tallo.end(), { x_inicio - tmp_x,  y_inicio, 0.0f });
    this->tallo.insert(this->tallo.end(), { x_inicio + tmp_x,  y_inicio, 0.0f });
    this->tallo.insert(this->tallo.end(), { x_inicio + tmp_x,  y_inicio - tmp_y, 0.0f });
    this->tallo.insert(this->tallo.end(), { x_inicio - tmp_x,  y_inicio - tmp_y, 0.0f });

    this->idx_tallo_margen.insert(this->idx_tallo_margen.end(), { 0,1,2,3,0 });
    this->idx_tallo_background.insert(this->idx_tallo_background.end(), { 0,1,3,1,2,3 });
}

//Petalo central
void Vitral::generar_petalo(float h, float k) {
    float a = (altura / 2) - k;
    float b = 0.2f;
    std::vector<float> petalo_lado_izquierdo = generar_elipse(h, k, a, b, false, true);
    std::vector<float> petalo_lado_derecho = generar_elipse(h, k, a, b, true, true);
    this->petalo = concatenar_vectores(petalo_lado_izquierdo, petalo_lado_derecho);
    this->idx_petalo = generar_idx(petalo_lado_izquierdo, petalo_lado_derecho, false);
}

//Petalo izquierdo
void Vitral::generar_petalo_izquierda(float h, float k) {
    float a = altura/2 + 0.1f;
    float b = 0.5f;  
    std::vector<float> petalo_lado_izquierdo = generar_elipse(h, k, a, b, false, false);
    std::vector<float> petalo_lado_derecho = generar_elipse(h - b, k - a, a, b, true, true);    
    this->petalo_izq = concatenar_vectores(petalo_lado_izquierdo, petalo_lado_derecho);
    this->idx_petalo_izq = generar_idx(petalo_lado_izquierdo, petalo_lado_derecho, true);
}

//petalo derecho
void Vitral::generar_petalo_derecha(float h, float k) {
    float a = altura / 2 + 0.1f;
    float b = 0.5f;
    std::vector<float> petalo_lado_izquierdo = generar_elipse(h, k, a, b, false, true);
    std::vector<float> petalo_lado_derecho = generar_elipse(h-b, k+a, a, b, true, false);
    this->petalo_der = concatenar_vectores(petalo_lado_izquierdo, petalo_lado_derecho);
    this->idx_petalo_der = generar_idx(petalo_lado_izquierdo, petalo_lado_derecho, true);
}

//hoja izquierda
void Vitral::generar_hoja_izquierda(float h, float k) {
    float a = (altura / 2) + k;
    float b = (ancho / 2) - 0.05f;
    std::vector<float> hoja_lado_izquierdo = generar_elipse(h, k, a, b, false, false);
    std::vector<float> hoja_lado_derecho = generar_elipse(h - b, k - a, a, b, true, true);
    this->hoja_izq = concatenar_vectores(hoja_lado_izquierdo, hoja_lado_derecho);
    this->idx_hoja_izq = generar_idx(hoja_lado_izquierdo, hoja_lado_derecho, true);
}

//hoja derecha
void Vitral::generar_hoja_derecha(float h, float k) {
    float a = (altura / 2) + 0.3f;
    float b = (ancho / 2) - 0.05f;
    std::vector<float> hoja_lado_izquierdo = generar_elipse(h, k, a, b, false, true);
    std::vector<float> hoja_lado_derecho = generar_elipse(h-b, k+a, a, b, true, false);
    this->hoja_der = concatenar_vectores(hoja_lado_izquierdo, hoja_lado_derecho);
    this->idx_hoja_der = generar_idx(hoja_lado_izquierdo, hoja_lado_derecho, true);       
}

/*
    * h,k representan el centro de la elipse  
    * a,b representan el ancho y altura desde el centro
    * lado_x indica si tomaremos el lado + ó - de la elipse respecto al eje x 
    * lado_y indica si tomaremos el lado + ó - de la elipse respecto al eje y
*/
std::vector<float> Vitral::generar_elipse(float h, float k, float a, float b, bool lado_x, bool lado_y) {
    std::vector<float> puntos;
    float idx = lado_y ? 1 : -1;
    float rango_a = lado_x ? h : h - b;
    float rango_b = lado_x ? h + b : h;
    for (float x = rango_a; x <= rango_b; x += 0.0001f) {
        float y = idx * sqrt(pow(a, 2) - pow(a, 2) * pow((x - h), 2) / pow(b, 2)) + k;
        puntos.insert(puntos.end(), { x, y, 0.0f });
    }
    return puntos;
}

// Concatena los vectores a y b mediante el uso de iteradores
std::vector<float> Vitral::concatenar_vectores(std::vector<float> a, std::vector<float> b) {
    std::vector<float> tmp;
    tmp.insert(tmp.end(), a.begin(), a.end());
    tmp.insert(tmp.end(), b.begin(), b.end());
    return tmp;    
}

/* 
  * Empareja cada vertice que se encuentra en el vector a, con uno del vector b.
  * El booleano Flag, indica si el sentido(Horizontal - / Vertical +) del emparejamiento.
 */
std::vector<unsigned int> Vitral::generar_idx(std::vector<float> a, std::vector<float> b, bool flag) {
    std::vector<unsigned int> tmp;
    unsigned int j = flag ? b.size() / 3: ((b.size() / 3) * 2) - 1;
    unsigned int k = flag ? 1 : -1;
    for (unsigned int i = 0; i < a.size() / 3; i++) {
        tmp.insert(tmp.end(), { i, j });
        j = j + k;
    }    
    return tmp;
}