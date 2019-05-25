#include "cannonball.hpp"

Cannonball::Cannonball(double M, double S,
           double x0, double y0, double z0,
           double Vx0, double Vy0, double Vz0,
           double p, double w, double latitude) : RungeKutta (6)
{
    this->M = M;
    this->S = S;

    this->w = w;

    this->p = p;

    this->x = x0;
    this->y = y0;
    this->z = z0;
    this->Vx = Vx0;
    this->Vy = Vy0;
    this->Vz = Vz0;

    std::vector<double> Y0(6);
    Y0[0] = x0;
    Y0[1] = y0;
    Y0[2] = z0;
    Y0[3] = Vx0;
    Y0[4] = Vy0;
    Y0[5] = Vz0;
    SetInit(0, Y0);
}

std::vector<double> Cannonball::F(double time, std::vector<double> &coordinates) {
    // x'=Vx - FY[0] = Y[3]
    // y'=Vy - FY[1] = Y[4]
    // z'=Vz - FY[2] = Y[5]
    // Vx'=... FY[3] =
    // Vy'=... FY[4] =
    // Vy'=... FY[5] =

    Vx = Y[3];
    Vy = Y[4];
    Vz = Y[5];
    double V = sqrt(Vx*Vx + Vy*Vy + Vz*Vz);

    FY[0] = Y[3];
    FY[1] = Y[4];
    FY[2] = Y[5];
    FY[3] = 2*M*w*Vy*sin(latitude) - 2*M*w*Vz*cos(latitude) - 0.5*C*p*S*V*Vx;
    FY[4] =       - 2*M*w*Vx*sin(latitude) - 0.5*C*p*S*V*Vy;
    FY[5] = - M*g + 2*M*w*Vx*cos(latitude) - 0.5*C*p*S*V*Vz;
    return FY;
}

double Cannonball::Step(int interval) {
    NextStep(((double)interval) / 1000.0);
    return t;
}

Cannonball::~Cannonball()
{
}
