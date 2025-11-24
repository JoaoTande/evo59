#ifndef DNA64_H
#define DNA64_H

#include <cstdint>
#include <allegro5/allegro.h>

enum class BehaviorType : uint8_t {
    TypeA = 0,
    TypeB = 1,
    TypeC = 2,
    TypeD = 3
};

class DNA64 {
public:
    DNA64();
    explicit DNA64(uint64_t bitsValue);

    // Criação inicial da população
    static DNA64 randomDNA();

    // Genética
    DNA64 mutate(float mutationRate) const;
    DNA64 crossover(const DNA64& other) const;

    // Getters normais
    float getSpeed() const;
    float getRange() const;
    BehaviorType getBehavior() const;
    ALLEGRO_COLOR getColor() const;

    float getDetectionProb() const;
    float getPerceptionProb() const;
    float getDrasticChangeProb() const;

    uint64_t getBits() const { return bits; }

private:
    uint64_t bits;

    // Helpers de codificação
    static uint64_t encodeFloat(float value, float maxValue, int bitCount);
    static float decodeFloat(uint64_t raw, float maxValue, int bitCount);

    static uint64_t encodeProb(float p, int bitCount);
    static float decodeProb(uint64_t raw, int bitCount);
};

#endif
