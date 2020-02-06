#ifndef PENDULEINVERSE_CONFIGURATION_H
#define PENDULEINVERSE_CONFIGURATION_H

#include <cstdint>

namespace ip {
namespace configuration {
/**
 * \brief Fréquence d'actualisation de chaque tâche.
 */
namespace frequence {
static constexpr uint32_t control       = 100;
static constexpr uint32_t debug         = 100;
static constexpr uint32_t enableSoftPWM = 10000;
}  // namespace frequence

/**
 * \brief Affectation de pins.
 */
namespace pin {
// Encodeur
static constexpr uint8_t encoderA = 49;
static constexpr uint8_t encoderB = 51;
// Infrarouges
static constexpr uint8_t infrared1 = A3;
static constexpr uint8_t infrared2 = A2;
// Variateur
static constexpr uint8_t var11_Enable    = 52;
static constexpr uint8_t var12_Direction = 50;
static constexpr uint8_t var13_Speed     = DAC0;
}  // namespace pin

// Nombre de bits de l'ADC
static constexpr uint8_t nBits = 10;
// Limite pour activer le PWM sur le pin enable
static constexpr uint32_t lowSpeedThreshold = 10;

// Coefficients de l'interpolation polynomiale de la fonction distance.
static float distanceCoefsLeft[]  = {-373.15f, 3709.7f, -11435.f, 15705.f, -7933.9f};
static float distanceCoefsRight[] = {340.62f, -2231.6f, 6091.2f, -7586.2f, 3501.4f};

}  // namespace configuration
}  // namespace ip

#endif  // PENDULEINVERSE_CONFIGURATION_H