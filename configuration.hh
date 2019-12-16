#ifndef PENDULEINVERSE_CONFIGURATION_H
#define PENDULEINVERSE_CONFIGURATION_H

#include <cstdint>

namespace ip {
namespace config {

// Pins de l'encodeur
constexpr uint8_t pinEncA = 49;
constexpr uint8_t pinEncB = 51;

// Pin du capteur gauche
constexpr uint8_t pinLeftSensor = A0;
// Pin du capteur droite
constexpr uint8_t pinRightSensor = A1;
// Nombre de bits de l'ADC
constexpr uint8_t nBits = 10;

// Coefficients de l'interpolation polynomiale de la fonction distance.
float distanceCoefs[] = {55.175f, -1150.f,  9029.8f, -31659.f,
                         56620.f, -49194.f, 16460.f};

}  // namespace config
}  // namespace ip

#endif  // PENDULEINVERSE_CONFIGURATION_H