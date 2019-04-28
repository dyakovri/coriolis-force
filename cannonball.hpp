#ifndef CANNONBALL_HPP
#define CANNONBALL_HPP

#include "RungeKutta.hpp"
#include <cmath>
#include <vector>

class Cannonball : public RungeKutta
{
private:
    const double C = 0.47;  // Коэффициет формы тела
    const double g = 9.81;  // Коэффициет формы тела

    double S;           // Площадь ядра
    double M;           // Масса ядра
    double x, y, z;     // Координата ядра
    double Vx, Vy, Vz;  // Скорость ядра
    double p;           // Плотность среды
    double w;           // Угловая скорость вращения Земли

public:
    Cannonball(double M, double S,
               double x0, double y0, double z0,
               double Vx0, double Vy0, double Vz0,
               double p, double w);
    ~Cannonball();

    virtual std::vector<double> F(double t, std::vector<double> &Y);

    double Step(int);
    double get_x() { return Y[0]; }
    double get_y() { return Y[1]; }
    double get_z() { return Y[2]; }
    double get_path() { return std::sqrt(std::pow(Y[0], 2) + std::pow(Y[1],2)); }
    double get_Vx() { return Y[3]; }
    double get_Vy() { return Y[4]; }
    double get_Vz() { return Y[5]; }
};

#endif // CANNONBALL_HPP
