#include "Persona.h"
#include <cmath>
#include <cstdlib>   // rand()
#include <cstdio>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

extern ALLEGRO_FONT* defaultFont;

Persona::Persona(float x, float y, float direction)
    : Thing(x, y), direction(direction), PersonaDNA() {
        Color = PersonaDNA.getColor();
        rangeDetection = PersonaDNA.getRange();
        speed = PersonaDNA.getSpeed();
        ThingType = Thing::Type::Persona;
        food = 0;
        beingDetected = 0;
    }

Persona::Persona(float x, float y, float direction, float speed)
    : Thing(x, y), direction(direction), speed(speed) {
        int a = (rand() % 255);
        int b = (rand() % 255);
        int c = (rand() % 255);
        Color = al_map_rgb(a, b, c);
        rangeDetection = (rand() % 150);
        ThingType = Thing::Type::Persona;
        food = 0;
        beingDetected = 0;
    }

Persona::Persona(float x, float y, float direction, DNA64 kidDNA)
    : Thing(x, y), direction(direction), PersonaDNA(kidDNA.getBits()) {
        Color = PersonaDNA.getColor();
        rangeDetection = PersonaDNA.getRange();
        speed = PersonaDNA.getSpeed();
        ThingType = Thing::Type::Persona;
        food = 0;
        beingDetected = 0;
}

Persona::~Persona(){}

void Persona::set(float x, float y, float direction, float speed)
{
    // usa os sets da classe Thing
    set_position(x, y);    

    // seta direção e velocidade
    this->direction = direction;
    this->speed = speed;
}

void Persona::set_direction(float dir)
{
    direction = dir;
}

void Persona::set_speed(float s)
{
    speed = s;
}

float Persona::get_direction()
{
    return direction;
}

float Persona::get_speed()
{
    return speed;
}    

// ----- DRAW -----
void Persona::draw() {
    //update();
    float radius = 5.0f;

    float psize = radius * size_global;
    float px = Thing::hall_x0+((x-x0_global)*size_global);
    float py = Thing::hall_y0+((y-y0_global)*size_global);

    al_draw_filled_circle(
        px,
        py,
        psize,
        Color
    );
    
    //std::vector<Thing*> vet1 = detect_nearby();
    //lookAtClosest(vet1);
    //EatFoodAndDelete(vet1);
    drawDetectionCircle(beingDetected);
    drawFoodLabel(food);
}
   
    

void Persona::update()
{
    updateWalk();
    std::vector<Thing*> vet1 = detect_nearby();
    lookAtClosest(vet1);
    EatFoodAndDelete(vet1);
    beingDetected = vet1.size();
}

void Persona::updateWalk()
{
    // --- 1. Pequena variação aleatória ---
    // valor entre -0.05 e 0.05 radianos (~3 graus)
    float variation = ((rand() % 1000) / 1000.0f - 0.5f) * 0.1f;

    direction += variation;

    // --- 2. Mantém a direção entre 0 e 2π ---
    const float TWO_PI = 6.28318530718f;
    if (direction < 0) direction += TWO_PI;
    if (direction > TWO_PI) direction -= TWO_PI;

    // --- 3. Atualiza posição ---
    x += std::cos(direction) * speed;
    y += std::sin(direction) * speed;

    // --- 4. Opcional: evitar sair da tela (exemplo: 1920x1080) ---
    if (x < 0) { x = 0; direction += 3.14159f; }
    if (y < 0) { y = 0; direction += 3.14159f; }
    if (x > Thing::sizeX_Hall) { x = Thing::sizeX_Hall; direction += 3.14159f; }
    if (y > Thing::sizeY_Hall) { y = Thing::sizeY_Hall; direction += 3.14159f; }
}

std::vector<Thing*> Persona::detect_nearby() {
    std::vector<Thing*> result;

    float range2 = rangeDetection * rangeDetection;  // comparar com distância ao quadrado

    for (Thing* t : all) {
        if(t->ThingType == Thing::Type::Thing){
            if (t == this) continue;  // não detectar a si mesmo

            float d2 = distance2(this->x, this->y, t->get_x(), t->get_y());
        
             if (d2 <= range2) {
                result.push_back(t);
            }
        }
    }

    return result;
}

void Persona::drawDetectionCircle(int count)
{
    float psize = rangeDetection * size_global;
    float px = Thing::hall_x0+((x-x0_global)*size_global);
    float py = Thing::hall_y0+((y-y0_global)*size_global);

    // Cor do círculo (amarelo)
    ALLEGRO_COLOR color = al_map_rgb(255, 255, 0);

    // Desenha o círculo de detecção
    al_draw_circle(px, py, psize, color, 1.5f);

    // Desenhar o número (count) no centro
    // Assumindo que você já carregou alguma fonte globalmente
    extern ALLEGRO_FONT* defaultFont;

    // Texto preto para boa leitura
    ALLEGRO_COLOR textColor = al_map_rgb(0, 0, 0);

    char buf[32];
    snprintf(buf, 32, "%d", count);

    al_draw_text(defaultFont, textColor, px, py, ALLEGRO_ALIGN_CENTRE, buf);
}

void Persona::drawFoodLabel(int totalfood)
{
    float px = Thing::hall_x0+((x-x0_global)*size_global);
    float py = Thing::hall_y0+((y-y0_global)*size_global);

    if (!defaultFont) return;

    // Texto que vamos desenhar
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", totalfood);

    // Posição do texto (acima do centro da persona)
    float tx = px - 10;   // deslocar um pouco pro lado
    float ty = py - 40;   // acima do círculo

    // Medir o tamanho do texto
    int w = al_get_text_width(defaultFont, buffer);
    int h = al_get_font_line_height(defaultFont);

    // Desenhar retângulo vermelho de fundo
    al_draw_filled_rectangle(
        tx - 4,
        ty - 2,
        tx + w + 4,
        ty + h + 2,
        al_map_rgb(255, 0, 0)   // vermelho
    );

    // Desenhar o texto amarelo
    al_draw_text(
        defaultFont,
        al_map_rgb(255, 255, 0),   // amarelo
        tx,
        ty,
        0,
        buffer
    );
}

void Persona::EatFood(std::vector<Thing*>& origem)
{
    for (Thing* t : origem) {
        // verifica se já está no destino
        if (std::find(foodVector.begin(), foodVector.end(), t) == foodVector.end()) {
            foodVector.push_back(t);   // só adiciona se NÃO existe
        }
    }
}

void Persona::EatFoodAndDelete(std::vector<Thing*>& origem)
{
    for (Thing* t : origem) {
        float distance = distance2(x, y, t->get_x(), t->get_y());

        if(distance < 10) {
            removeThing(t, all);
            food++;
            ListToDelete.push_back(t);
        }
    }
}

void Persona::removeThing(Thing* t, std::vector<Thing*>& vet)
{
    if (!t) return;

    // Procurar no vetor
    auto it = std::find(vet.begin(), vet.end(), t);
    if (it != vet.end()) {
        vet.erase(it);   // remove do vetor
    }
}

void Persona::lookAtClosest(const std::vector<Thing*>& things)
{
    if (things.empty()) return;  // nada a fazer

    Thing* closest = nullptr;
    float closestDist2 = std::numeric_limits<float>::max();

    for (Thing* t : things)
    {
        if (!t) continue; // segurança

        float dx = t->get_x() - this->x;
        float dy = t->get_y() - this->y;
        float dist2 = dx*dx + dy*dy;

        if (dist2 < closestDist2) {
            closestDist2 = dist2;
            closest = t;
        }
    }

    if (!closest) return;

    // calcula o ângulo em radianos
    float dx = closest->get_x() - this->x;
    float dy = closest->get_y() - this->y;

    this->direction = std::atan2(dy, dx);
}