#pragma once
#include <allegro5/allegro.h>
#include <vector>
#include <algorithm>

class Thing {
protected:
    float x;
    float y;

public:
    enum class Type {
        Thing,
        Persona
    };

    Type ThingType;

    static std::vector<Thing*> ListToDelete;
    static void cleanAllUnused();

    static std::vector<Thing*> all;
    
    Thing ();
    Thing(float x, float y);
    virtual ~Thing();
        
    // propriedades globais
    static float size_global;
    static float x0_global;
    static float y0_global;
    static float x_global;
    static float y_global;
    static float sizeX_Hall;
    static float sizeY_Hall;
    static float hall_x0;
    static float hall_y0;

    // setters
    void set_position(float px, float py);
    void set_position(int px, int py);

    // getters
    void get_position(float &px, float &py);
    void get_position(int &px, int &py);

    float get_x();
    float get_y();

    virtual void update();
    // draw base
    virtual void draw();

    inline float distance2(float x1, float y1, float x2, float y2) {
        float dx = x2 - x1;
        float dy = y2 - y1;
        return dx*dx + dy*dy;   // distância ao quadrado (mais rápido)
    }
};
