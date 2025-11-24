#pragma once
#include "Thing.h"
#include "DNA64.h"

class Persona : public Thing {
public:
    float direction;  // em radianos
    
    DNA64 PersonaDNA;
    float speed;
    float rangeDetection;

    int food;
    int beingDetected;

    Persona(float x , float y, float direction, float speed);
    Persona(float x, float y, float direction);
    Persona(float x, float y, float direction, DNA64 kidDNA);

    virtual ~Persona();

    static void removeThing(Thing* t, std::vector<Thing*>& vet);

    // Set completo
    void set(float x, float y, float direction, float speed);

    // Se quiser também sets individuais:
    void set_direction(float dir);
    void set_speed(float s);
    float get_direction();
    float get_speed();

    void update() override;
    void updateWalk();
    void EatFood(std::vector<Thing*>& origem);
    void EatFoodAndDelete(std::vector<Thing*>& origem);
    void lookAtClosest(const std::vector<Thing*>& things);

    ALLEGRO_COLOR Color;

    // sobrescreve draw
    void draw() override;
    void drawDetectionCircle(int count);
    void drawFoodLabel(int totalfood);
    std::vector<Thing*> detect_nearby();
    std::vector<Thing*> foodVector;
};
