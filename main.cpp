#include <iostream>
#include <vector>

#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

struct Point {
  double x;
  double z;
};

struct Vector {
  double Bx;
  double Bz;
};

struct Loop {
  double R;
  double Z;
  double I;
};

typedef std::vector<Point> mesh_t;
typedef std::vector<Vector> field_t;
typedef std::vector<Loop> loop_t;

BOOST_FUSION_ADAPT_STRUCT(
			  Point,
			  (double, x)
			  (double, z)
)
BOOST_FUSION_ADAPT_STRUCT(
			  Loop,
			  (double, R)
			  (double, Z)
			  (double, I)
)
BOOST_FUSION_ADAPT_STRUCT(
			  Vector,
			  (double, Bx)
			  (double, Bz)
)

#include "calculator.hpp"
#include "reader.hpp"
#include "outputter.hpp"

struct compute_field {
  typedef void result_type;
  void operator()(const Loop& loop, const mesh_t& iMesh, field_t& ioField) const {
    magnetic_field(loop.R, loop.Z, loop.I, iMesh, ioField);
  }
};

int main(int argc, char* argv[]) {
  std::string mesh_filename, geometry_filename, field_filename;

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("mesh", boost::program_options::value<std::string>(&mesh_filename)->default_value("mesh.csv"), "name of the file containing the mesh.")
    ("geometry", boost::program_options::value<std::string>(&geometry_filename)->default_value("loops.csv"), "name of the file containing the geometry of current loops")
    ("field", boost::program_options::value<std::string>(&field_filename)->default_value("field.csv"), "name of the output file with computation of field")
    ;

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  mesh_t aMesh;
  field_t aField;
  loop_t aGeometry;

  readLoop(geometry_filename, aGeometry);
  readMesh(mesh_filename, aMesh);

  size_t elements_number = aMesh.size();
  Vector null_vector = {0,0};
  for(size_t index; index < elements_number; ++index) {
    aField.push_back(null_vector);
  }

  std::for_each(aGeometry.begin(),aGeometry.end(),boost::bind(compute_field(),::_1,boost::cref(aMesh),boost::ref(aField)) );

  writeResult(field_filename, aMesh, aField);

  return 0;
}
