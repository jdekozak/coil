#include <cmath>
#include <iostream>

#include <boost/program_options.hpp>

double get_intensity(const double& radius) {
  return 1;//-std::exp(-1/radius/radius);
}

double get_height(const double& radius) {
  return 1-std::exp(-1/radius/radius);
}

int main(int argc, char* argv[]) {
  size_t maximum_number_loops;
  double maximum_radius;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("radius", boost::program_options::value<double>(&maximum_radius)->default_value(6.), "maximum radius")
    ("loops", boost::program_options::value<size_t>(&maximum_number_loops)->default_value(13), "number of current loops")
    ;

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  double step = maximum_radius / maximum_number_loops;
  size_t index;
  double radius;
  double intensity;
  double height;
  std::cout << "R,y,Z,I" << std::endl;
  for(index = 1, radius = step; index < maximum_number_loops; ++index, radius+=step) {
    height = get_height(radius);
    intensity = get_intensity(radius);
    if(index % 2) {
      std::cout << radius << ",0.," << height << "," << intensity << std::endl;
      std::cout << radius << ",0.," << -height << "," << intensity << std::endl;
    } else {
      std::cout << radius << ",0.," << height << "," << intensity << std::endl;
      std::cout << radius << ",0.," << -height << "," << intensity << std::endl;
    }
  }
  if(index % 2) {
    std::cout << radius << ",0.,0," << intensity << std::endl;
  } else {
    std::cout << radius << ",0.,0," << intensity << std::endl;
  }

  return 0;
}
