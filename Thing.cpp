#include "Thing.h"
#include <allegro5/allegro_primitives.h>

float Thing::size_global = 20.0f;
float Thing::x0_global   = 0.0f;
float Thing::y0_global   = 0.0f;
float Thing::x_global    = 0.0f;
float Thing::y_global    = 0.0f;
float Thing::sizeX_Hall = 1200.0f;
float Thing::sizeY_Hall = 1080.0f;
float Thing::hall_x0 = 0.0f;
float Thing::hall_y0 = 0.0f;
std::vector<Thing*> Thing::all;
std::vector<Thing*> Thing::ListToDelete;

void Thing::cleanAllUnused(){
    for(int i=0;i<static_cast<int>(ListToDelete.size());i++){
        delete ListToDelete[i];
        ListToDelete[i] = nullptr;
    }
    ListToDelete.clear();
}

Thing::Thing (){
    float px = float(rand() % int(Thing::sizeX_Hall));
    float py = float(rand() % int(Thing::sizeY_Hall));
    Thing(px, py);
}

Thing::Thing(float x, float y) : x(x), y(y) {
    all.push_back(this);
    ThingType = Thing::Type::Thing;
}

Thing::~Thing (){}

// ----- SETTERS -----
void Thing::set_position(float px, float py) {
    x = px;
    y = py;
}

void Thing::set_position(int px, int py) {
    x = static_cast<float>(px);
    y = static_cast<float>(py);
}

// ----- GETTERS -----
void Thing::get_position(float &px, float &py){
    px = x;
    py = y;
}

void Thing::get_position(int &px, int &py){
    px = static_cast<int>(x);
    py = static_cast<int>(y);
}

float Thing::get_x(){
    return x;
}

float Thing::get_y(){
    return y;
}

void Thing::update() {}

// ----- DRAW -----
void Thing::draw() {
        float size = 5;

        float psize = size * size_global;
        float px = Thing::hall_x0+((x-x0_global)*size_global);
        float py = Thing::hall_y0+((y-y0_global)*size_global);

        float x1 = px;
        float y1 = py - psize ;

        float x2 = px - psize;
        float y2 = py + psize;

        float x3 = px + psize;
        float y3 = py + psize;

        al_draw_filled_triangle(
            x1, y1,
            x2, y2,
            x3, y3,
            al_map_rgb(255, 255, 255)  // branco
        );
}


