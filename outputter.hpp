#ifndef __OUTPUTTER_HPP__
#define __OUTPUTTER_HPP__

#include <fstream>

#include <boost/spirit/include/karma.hpp>

#include <boost/fusion/include/std_pair.hpp> 

template<typename Iterator>
struct vector_field_generator : boost::spirit::karma::grammar<Iterator, std::pair<Point, Vector>()>
{
  vector_field_generator() : vector_field_generator::base_type(start)
  {
    //x,y,z,Bx,By,Bz
    point = boost::spirit::karma::double_
      << ",0.0," << boost::spirit::karma::double_
      ;
    field = boost::spirit::karma::double_
      << ",0.0," << boost::spirit::karma::double_
      ;
    start = point
      << ',' << field;
  }
  boost::spirit::karma::rule<Iterator, std::pair<Point, Vector>()> start;
  boost::spirit::karma::rule<Iterator, Point()> point;
  boost::spirit::karma::rule<Iterator, Vector()> field;
};
typedef std::back_insert_iterator<std::string> iterator_t;
typedef vector_field_generator<iterator_t> string_vector_field_generator;

struct writer {
  writer(const std::string& filename) : _filename(filename) {}
  void operator()(const mesh_t& Mesh, const field_t& Field) const {
    std::ofstream output(_filename.c_str());

    mesh_t::const_iterator start = Mesh.begin();
    mesh_t::const_iterator finish = Mesh.end();
    mesh_t::const_iterator point;

    field_t::const_iterator debut = Field.begin();
    field_t::const_iterator fin = Field.end();
    field_t::const_iterator magnet;

    output << "x,y,z,Bx,By,Bz" << std::endl;
    string_vector_field_generator aGenerator;
    for(point = start, magnet = debut; point != finish; ++point, ++magnet) {
      std::string newLine;
      iterator_t iter(newLine);
      boost::spirit::karma::generate_delimited(iter, aGenerator, boost::spirit::ascii::space, std::pair<Point, Vector>(*point, *magnet));
      output << newLine << std::endl;
    }
    output.close();
  }
  std::string _filename;
};

#endif
