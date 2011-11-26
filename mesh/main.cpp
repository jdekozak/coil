#include <cmath>
#include <iostream>

#include <boost/program_options.hpp>

int main(int argc, char* argv[]) {
  double maximum_x;
  double maximum_z;
  size_t x_divisions;
  size_t z_divisions;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("xmax", boost::program_options::value<double>(&maximum_x)->default_value(6.), "maximum x")
    ("xdiv", boost::program_options::value<size_t>(&x_divisions)->default_value(100), "x divisions")
    ("zmax", boost::program_options::value<double>(&maximum_z)->default_value(1.), "maximum z")
    ("zdiv", boost::program_options::value<size_t>(&z_divisions)->default_value(100), "z divisions")
    ;

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  double x_step = maximum_x / x_divisions;
  double z_step = maximum_z / z_divisions;
  size_t x_index, z_index;
  double x, z;
  for(x_index = 0, x = 0; x_index < x_divisions; ++x_index, x+=x_step) {
    std::cout << x << ",0" << std::endl;
    for(z_index = 1, z = z_step; z_index < z_divisions; ++z_index, z+=z_step) {
      std::cout << x << "," << z << std::endl;
      std::cout << x << "," << -z << std::endl;
    }
  }
  return 0;
}
