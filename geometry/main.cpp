#include <cmath>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/function.hpp>

//HEIGHT AND INTENSITY TAGS
struct constant_tag;
struct nef_tag;
struct linear_decrease_tag;
//INTENSITY
template<typename PARAMETER=constant_tag>
struct get_intensity {
  double operator()(const double&) const {
    return 1;
  }
};
template<>
struct get_intensity<linear_decrease_tag> {
  double operator()(const double& radius) const {
    return 1-0.01*radius;
  }
};
//INTENSITY INTERFACE
typedef boost::function<double(const double &)> intensity_t;

//HEIGHT
template<typename PARAMETER=constant_tag>
struct get_height {
  double operator()(const double&) const {
    return 1;
  }
};
template<>
struct get_height<nef_tag> {
  double operator()(const double& radius) const {
    return 1-std::exp(-1/radius/radius);
  }
};
//HEIGHT INTERFACE
typedef boost::function<double(const double &)> height_t;

//PROFILE TAGS
struct wall_confinement_tag;
struct regular_coil_tag;
//PROFILE
template<typename PARAMETER=regular_coil_tag>
struct get_profile {
  void operator()(const double& radius, const size_t&, const height_t& height, const intensity_t& intensity) const {
    double z = height(radius);
    double i = intensity(radius);
    std::cout << radius << ",0.," << z << "," << i << std::endl;
    std::cout << radius << ",0.," << -z << "," << i << std::endl;  
  }
};
template<>
struct get_profile<wall_confinement_tag> {
  void operator()(const double& radius, const size_t& index, const height_t& height, const intensity_t& intensity) const {
    double z = height(radius);
    double i = intensity(radius);
    if(index % 2) {
      std::cout << radius << ",0.," << z << "," << i << std::endl;
      std::cout << radius << ",0.," << -z << "," << i << std::endl;
    } else {
      std::cout << radius << ",0.," << z << "," << -i << std::endl;
      std::cout << radius << ",0.," << -z << "," << -i << std::endl;
    }
  }
};
//PROFILE INTERFACE
typedef boost::function<void(const double&, const size_t&, const height_t&, const intensity_t&)> profile_t;

//EXTREMITY TAGS
struct close_extremity_tag;
struct open_extremity_tag;
//EXTREMITY
template<typename PARAMETER=close_extremity_tag, typename PROFILE=regular_coil_tag>
struct get_extremity {
  void operator()(const double& radius, const size_t&, const intensity_t& intensity) const {
    double i = intensity(radius);
    std::cout << radius << ",0.,0," << i << std::endl;
  }
};
template<>
struct get_extremity<close_extremity_tag, wall_confinement_tag> {
  void operator()(const double& radius, const size_t& index, const intensity_t& intensity) const {
    double i = intensity(radius);
    if(index % 2) {
      std::cout << radius << ",0.,0," << i << std::endl;
    } else {
      std::cout << radius << ",0.,0," << -i << std::endl;
    }
  }
};
template<typename PROFILE>
struct get_extremity<open_extremity_tag, PROFILE> {
  void operator()(const double&, const size_t&, const intensity_t&) const {
    //empty
  }
};
//EXTREMITY INTERFACE
typedef boost::function<void(const double&, const size_t&, const intensity_t&)> extremity_t;

//APPLICATION
struct generate_geometry {
  generate_geometry(const size_t& maximum_number_loops, const double& maximum_radius) :
    _maximum_number_loops(maximum_number_loops),
    _step(maximum_radius/maximum_number_loops)
  {}
  void operator()(const profile_t& profile, const extremity_t& extremity, const height_t& height, const intensity_t& intensity) const {
    size_t index;
    double radius;
    std::cout << "R,y,Z,I" << std::endl;
    for(index = 1, radius = _step; index < _maximum_number_loops; ++index, radius+=_step) {
      profile(radius, index, height, intensity);
    }
    extremity(radius, index, intensity);
  }
  size_t _maximum_number_loops;
  double _step;
};

//OS PLUGIN ;)
int main(int argc, char* argv[]) {
  size_t maximum_number_loops;
  double maximum_radius;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("radius", boost::program_options::value<double>(&maximum_radius)->default_value(6.), "maximum radius")
    ("loops", boost::program_options::value<size_t>(&maximum_number_loops)->default_value(13), "number of current loops")
    ("nef_profile", "use the nef profile for height")
    ("constant_intensity", "use the same intensity in all loops")
    ("wall_confinement", "alternate sign of intensity between loops")
    ("open_extremity", "no loop at z=0")
    ;

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  generate_geometry application(maximum_number_loops, maximum_radius);

  height_t Z;
  intensity_t I;
  profile_t P;
  extremity_t E;

  if (vm.count("nef_profile")) {
    Z = get_height<nef_tag>();
  } else {
    Z = get_height<constant_tag>();
  }
  if (vm.count("constant_intensity")) {
    I = get_intensity<constant_tag>();
  } else {
    I = get_intensity<linear_decrease_tag>();
  }
  if (vm.count("wall_confinement")) {
    P = get_profile<wall_confinement_tag>();
    if (vm.count("open_extremity")) {
      E = get_extremity<open_extremity_tag, wall_confinement_tag>();
    } else {
      E = get_extremity<close_extremity_tag, wall_confinement_tag>();
    }
  } else {
    P = get_profile<regular_coil_tag>();
    if (vm.count("open_extremity")) {
      E = get_extremity<open_extremity_tag, regular_coil_tag>();
    } else {
      E = get_extremity<close_extremity_tag, regular_coil_tag>();
    }
  }

  application(P, E, Z, I);

  return 0;
}
