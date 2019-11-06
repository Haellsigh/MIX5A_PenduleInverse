#ifndef PENDULE_CONFIGURATION_H
#define PENDULE_CONFIGURATION_H


// Pin du capteur gauche
constexpr uint8_t pinLeftSensor = A2;
// Pin du capteur droite
constexpr uint8_t pinRightSensor = A3;
// Nombre de bits
constexpr uint8_t nBits = 10;

// Coefficients de l'interpolation, de a0 vers a6 pour les deux capteurs
float coefsLeftSensor[] = {55.175f, -1150.f, 9029.8f, -31659.f, 56620.f, -49194.f, 16460.f};
float coefsRightSensor[] = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f};

#endif // PENDULE_CONFIGURATION_H