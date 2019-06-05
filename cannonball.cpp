#include "cannonball.hpp"

Cannonball::Cannonball(double M, double S,
           double x0, double y0, double z0,
           double V0, double alpha, double beta,
           double p, double w, double latitude) : RungeKutta (6)
{
    this->M = M;
    this->S = S;

    this->w = w;

    this->p = p;

    this->x = x0;
    this->y = y0;
    this->z = z0;
    this->Vx = fabs(V0)*cos(alpha*(M_PI/180))*sin(beta*(M_PI/180));
    this->Vy = fabs(V0)*cos(alpha*(M_PI/180))*cos(beta*(M_PI/180));
    this->Vz = fabs(V0)*sin(alpha*(M_PI/180));

    this->latitude = latitude*(M_PI/180);

    std::vector<double> Y0(6);
    Y0[0] = this->x;
    Y0[1] = this->y;
    Y0[2] = this->z;
    Y0[3] = this->Vx;
    Y0[4] = this->Vy;
    Y0[5] = this->Vz;
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
    FY[3] = (2*M*w*Vy*sin(latitude) - 2*M*w*Vz*cos(latitude) - 0.5*C*p*S*V*Vx)/M;
    FY[4] =  (     - 2*M*w*Vx*sin(latitude) - 0.5*C*p*S*V*Vy)/M;
    FY[5] = (- M*g + 2*M*w*Vx*cos(latitude) - 0.5*C*p*S*V*Vz)/M;
    return FY;
}

double Cannonball::Step(int interval) {
    NextStep(((double)interval) / 1000.0);
    return t;
}

Cannonball::~Cannonball()
{
}
