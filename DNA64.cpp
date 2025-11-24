#include "DNA64.h"
#include <random>
#include <algorithm>

static std::mt19937_64 rng(std::random_device{}());

// =======================================================
//                  CONSTRUTORES
// =======================================================
DNA64::DNA64() {
    bits = rng();  // 64 bits totalmente aleatórios
}
DNA64::DNA64(uint64_t bitsValue) : bits(bitsValue) {}


// =======================================================
//       FUNÇÃO PARA CRIAR DNA INICIAL ALEATÓRIO
// =======================================================
DNA64 DNA64::randomDNA()
{
    uint64_t newBits = 0;
    int shift = 0;

    // speed → 10 bits (0–5)
    float speed = std::uniform_real_distribution<float>(0, 5)(rng);
    newBits |= encodeFloat(speed, 5.0f, 10) << shift;

    shift += 10;

    // range → 12 bits (0–200)
    float range = std::uniform_real_distribution<float>(0, 200)(rng);
    newBits |= encodeFloat(range, 200.0f, 12) << shift;

    shift += 12;

    // behavior → 2 bits
    auto behavior = static_cast<BehaviorType>(
        std::uniform_int_distribution<int>(0,3)(rng)
    );
    newBits |= (uint64_t(behavior) << shift);

    shift += 2;

    // detection prob (0–1) → 10 bits
    float detect = std::uniform_real_distribution<float>(0,1)(rng);
    newBits |= encodeProb(detect, 10) << shift;

    shift += 10;

    // perception prob (0–1) → 10 bits
    float percept = std::uniform_real_distribution<float>(0,1)(rng);
    newBits |= encodeProb(percept, 10) << shift;

    shift += 10;

    // drastic prob (0–0.05) → 10 bits
    float drastic = std::uniform_real_distribution<float>(0,0.05f)(rng);
    newBits |= encodeFloat(drastic, 0.05f, 10) << shift;

    return DNA64(newBits);
}


// =======================================================
//                   FUNÇÕES GENÉTICAS
// =======================================================
DNA64 DNA64::mutate(float mutationRate) const
{
    uint64_t newBits = bits;

    std::bernoulli_distribution mutateDist(mutationRate);

    for (int i = 0; i < 64; i++) {
        if (mutateDist(rng)) {
            newBits ^= (1ULL << i);  // flip do bit
        }
    }

    return DNA64(newBits);
}


// Multi-point crossover
DNA64 DNA64::crossover(const DNA64& other) const
{
    int nCuts = std::uniform_int_distribution<int>(2, 5)(rng);

    uint64_t mask = 0;
    bool toggle = false;
    int lastCut = 0;

    for (int i = 0; i < nCuts; i++) {
        int cut = std::uniform_int_distribution<int>(lastCut, 63)(rng);
        for (int b = lastCut; b < cut; b++) {
            if (toggle) mask |= (1ULL << b);
        }
        toggle = !toggle;
        lastCut = cut;
    }

    uint64_t childBits = (bits & ~mask) | (other.bits & mask);
    return DNA64(childBits);
}


// =======================================================
//                    GETTERS
// =======================================================
float DNA64::getSpeed() const
{
    uint64_t raw = bits & ((1ULL << 10) - 1);
    return decodeFloat(raw, 5.0f, 10);
}

float DNA64::getRange() const
{
    uint64_t raw = (bits >> 10) & ((1ULL << 12) - 1);
    return decodeFloat(raw, 200.0f, 12);
}

BehaviorType DNA64::getBehavior() const
{
    uint64_t raw = (bits >> 22) & 0b11;
    return BehaviorType(raw);
}

float DNA64::getDetectionProb() const
{
    uint64_t raw = (bits >> 24) & ((1ULL << 10) - 1);
    return decodeProb(raw, 10);
}

float DNA64::getPerceptionProb() const
{
    uint64_t raw = (bits >> 34) & ((1ULL << 10) - 1);
    return decodeProb(raw, 10);
}

float DNA64::getDrasticChangeProb() const
{
    uint64_t raw = (bits >> 44) & ((1ULL << 10) - 1);
    return decodeFloat(raw, 0.05f, 10);
}


// Cores derivadas de speed e range (para similaridade genética → cores próximas)
ALLEGRO_COLOR DNA64::getColor() const
{
    float s = getSpeed() / 5.0f;      // 0–1
    float r = getRange() / 200.0f;    // 0–1

    float b = 1.0f - (s + r) * 0.5f;

    return al_map_rgb_f(s, r, b);
}


// =======================================================
//                FUNÇÕES DE CODIFICAÇÃO
// =======================================================
uint64_t DNA64::encodeFloat(float value, float maxValue, int bitCount)
{
    float n = value / maxValue;
    n = std::clamp(n, 0.0f, 1.0f);

    uint64_t maxInt = (1ULL << bitCount) - 1;
    return uint64_t(n * maxInt + 0.5f);
}

float DNA64::decodeFloat(uint64_t raw, float maxValue, int bitCount)
{
    float maxInt = float((1ULL << bitCount) - 1);
    return (raw / maxInt) * maxValue;
}

uint64_t DNA64::encodeProb(float p, int bitCount)
{
    p = std::clamp(p, 0.0f, 1.0f);

    uint64_t maxInt = (1ULL << bitCount) - 1;
    return uint64_t(p * maxInt + 0.5f);
}

float DNA64::decodeProb(uint64_t raw, int bitCount)
{
    float maxInt = float((1ULL << bitCount) - 1);
    return raw / maxInt;
}
