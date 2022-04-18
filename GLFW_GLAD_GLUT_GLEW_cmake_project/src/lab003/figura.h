class Figura {
public:
    std::vector<float> puntos;

    Figura();
    void axis_x(bool flag);
    void axis_y(bool);

private:
    bool verificando_limites(std::vector<float>, bool);    
};

Figura::Figura() {
    
}


void Figura::axis_x(bool flag) {
    std::vector<float> tmp_puntos = this->puntos;
    for (int i = 0; i < tmp_puntos.size(); i += 3) {
        tmp_puntos[i] = flag ? tmp_puntos[i] + 0.1f : tmp_puntos[i] - 0.1f;
    }
    this->puntos = verificando_limites(tmp_puntos, false) ? tmp_puntos : this->puntos;
}

void Figura::axis_y(bool flag) {
    std::vector<float> tmp_puntos = this->puntos;
    for (int i = 0; i < tmp_puntos.size(); i += 3) {
        tmp_puntos[i + 1] = flag ? tmp_puntos[i + 1] + 0.1f : tmp_puntos[i + 1] - 0.1f;
    }
    this->puntos = verificando_limites(tmp_puntos, true) ? tmp_puntos : this->puntos;
}

bool Figura::verificando_limites(std::vector<float> tmp_puntos, bool flag) {
    int dir = flag ? 1 : 0;
    for (int i = 0; i < tmp_puntos.size(); i += 3) {
        if ((tmp_puntos[i + dir] > 1.0f) || (tmp_puntos[i + dir] < -1.0f)) return false;
    }
    return true;
}