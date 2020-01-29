#ifndef PENDULEINVERSE_CONFIGURATION_H
#define PENDULEINVERSE_CONFIGURATION_H

#include <cstdint>

namespace ip {
namespace config {

// Pins de l'encodeur
static constexpr uint8_t pinEncA = 49;
static constexpr uint8_t pinEncB = 51;

// Pin du capteur gauche
static constexpr uint8_t pinLeftSensor = A3;
// Pin du capteur droite
static constexpr uint8_t pinRightSensor = A2;
// Nombre de bits de l'ADC
static constexpr uint8_t nBits = 10;

// Pin DAC0
static constexpr uint8_t pinSetValueSpeed   = DAC0;
static constexpr uint8_t pinVar11_Enable    = 48;
static constexpr uint8_t pinVar12_Direction = 50;

// Coefficients de l'interpolation polynomiale de la fonction distance.
static float distanceCoefsLeft[]  = {-373.15f, 3709.7f, -11435.f, 15705.f, -7933.9f};
static float distanceCoefsRight[] = {340.62f, -2231.6f, 6091.2f, -7586.2f, 3501.4f};

}  // namespace config
}  // namespace ip

#endif  // PENDULEINVERSE_CONFIGURATION_H