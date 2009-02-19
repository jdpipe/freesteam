#include "steamtestpoint.h"

// Easy initialiser for values of known units

SteamTestPoint::SteamTestPoint(double t, double p, double v, double h, double u, double s, double cp, double w) {

	this->temp = t * Kelvin;
	this->pres = p * MPa;
	this->v = v * metre3 / kilogram;
	this->h = h * kJ_kg;
	this->u = u * kJ_kg;
	this->s = s * kJ_kgK;
	this->cp = cp * kJ_kgK;
	this->w = w * metre / second;

}
