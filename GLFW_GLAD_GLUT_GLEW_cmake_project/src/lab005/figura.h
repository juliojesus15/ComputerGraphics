#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtx/io.hpp> // To print
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class Figura {
public:
    std::vector<float> puntos;

    Figura();
    void aplicar_traslacion(float x, float y, bool eje);
    void aplicar_rotacion(float angulo);
    void aplicar_scala(bool escala);
    void axis_x(bool flag);
    void axis_y(bool);

private:
    bool verificando_limites(std::vector<float>, bool);    
};

Figura::Figura() {
    
}

void Figura::aplicar_traslacion(float x, float y, bool eje) {
    std::vector<float> tmp_puntos = this->puntos;
    glm::mat4 matriz_traslacion = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    for (float i = 0; i < tmp_puntos.size(); i += 3) {
        glm::vec4 vertice = glm::vec4(tmp_puntos[i], tmp_puntos[i + 1], 0.0f, 1.0f);
        glm::vec4 traslacion = matriz_traslacion * vertice;
        tmp_puntos[i] = traslacion.x;
        tmp_puntos[i + 1] = traslacion.y;
    }

    this->puntos = verificando_limites(tmp_puntos, eje) ? tmp_puntos : this->puntos;
}

void Figura::aplicar_rotacion(float angulo) {
    glm::mat4 matriz_rotacion = glm::rotate(glm::mat4(1.0f), glm::radians(angulo), glm::vec3(0.0f, 0.0f, 1.0f));
    
    for (float i = 0; i < this->puntos.size(); i += 3) {
        glm::vec4 vertice = glm::vec4(puntos[i], puntos[i + 1], 0.0f, 1.0f);
        glm::vec4 traslacion = matriz_rotacion * vertice;
        puntos[i] = traslacion.x;
        puntos[i + 1] = traslacion.y;
    }
}

// El booleano scala incara True, si el objeto aumenta su tamaño y False si se quiere reducir
void Figura::aplicar_scala(bool escala) {
    float k = escala ? 2.0f : 0.5f;
    glm::mat4 matriz_escala = glm::scale(glm::mat4(1.0f), glm::vec3(k, k, 0.0f));

    for (float i = 0; i < this->puntos.size(); i += 3) {
        glm::vec4 vertice = glm::vec4(puntos[i], puntos[i + 1], 0.0f, 1.0f);
        glm::vec4 escala = matriz_escala * vertice;
        std::cout << escala << std::endl;
        puntos[i] = escala.x;
        puntos[i + 1] = escala.y;
    }

}

bool Figura::verificando_limites(std::vector<float> tmp_puntos, bool flag) {
    int dir = flag ? 1 : 0;
    for (int i = 0; i < tmp_puntos.size(); i += 3) {
        if ((tmp_puntos[i + dir] > 1.0f) || (tmp_puntos[i + dir] < -1.0f)) return false;
    }
    return true;
}