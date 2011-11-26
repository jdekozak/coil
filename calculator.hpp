#ifndef __CALCULATOR_HPP__
#define __CALCULATOR_HPP__

#include <boost/math/special_functions/ellint_1.hpp>
#include <boost/math/special_functions/ellint_2.hpp>

#include <cmath>

const double mu0 = 4*M_PI*1e-7;

void magnetic_field(const double& R, const double& Z0, const double& I, const mesh_t& iMesh, field_t& ioField) {
  size_t index, element_number;
  element_number = iMesh.size();
#pragma omp parallel for
  for(index = 0; index < element_number; ++index) {
    const Point& point = iMesh.at(index);
    Vector& magnet = ioField.at(index);
    double rho, denominateur, denominateur2, kcarre, E, K, B0;
    rho = std::fabs(point.x);
    if(rho > 0) {
      denominateur  = (R+rho)*(R+rho)+(point.z-Z0)*(point.z-Z0);
      denominateur2 = (R-rho)*(R-rho)+(point.z-Z0)*(point.z-Z0);
      kcarre = 4*R*rho / denominateur;
      try {
	E = boost::math::ellint_2(kcarre);
	K = boost::math::ellint_1(kcarre);
	B0 = mu0*I/(2*M_PI*sqrt(denominateur));
	magnet.Bz += B0                  * (E*(R*R-rho*rho-(point.z-Z0)*(point.z-Z0))/denominateur2 + K);
	magnet.Bx += B0*(point.z-Z0)/rho * (E*(R*R+rho*rho+(point.z-Z0)*(point.z-Z0))/denominateur2 - K);
	if(point.x < 0) {
	  magnet.Bx += - magnet.Bx;
	}
      } catch(...) {
	//just ignore overflow errors
	//if the point to compute is on a current loop...
	//std::cout << point.x << "," << point.z << std::endl;	
      }
    } else {
      magnet.Bx += 0;
      magnet.Bz += mu0*I*R*R/(2
			       *sqrt(R*R+(point.z-Z0)*(point.z-Z0))
			       *sqrt(R*R+(point.z-Z0)*(point.z-Z0))
			       *sqrt(R*R+(point.z-Z0)*(point.z-Z0)));
    }
  }
}

#endif
