#ifndef __READER_HPP__
#define __READER_HPP__

#include <fstream>

#include <boost/spirit/include/qi.hpp>

template<typename Iterator>
struct point_parser : boost::spirit::qi::grammar<Iterator, Point(), boost::spirit::ascii::space_type>
{
  point_parser() : point_parser::base_type(start)
  {
    //x,z
    start %= boost::spirit::qi::double_ 
      >> ',' >>  boost::spirit::qi::double_
      ;
  }
  boost::spirit::qi::rule<Iterator, Point(), boost::spirit::ascii::space_type> start;
};

template<typename Iterator>
struct loop_parser : boost::spirit::qi::grammar<Iterator, Loop(), boost::spirit::ascii::space_type>
{
  loop_parser() : loop_parser::base_type(start)
  {
    //R,y,Z,I
    start %= boost::spirit::qi::double_
      >> ',' >>  boost::spirit::qi::omit[boost::spirit::qi::double_]
      >> ',' >>  boost::spirit::qi::double_
      >> ',' >>  boost::spirit::qi::double_
      ;
  }
  boost::spirit::qi::rule<Iterator, Loop(), boost::spirit::ascii::space_type> start;
};

typedef std::string::const_iterator const_iterator_t;
typedef point_parser<const_iterator_t> string_point_parser;
typedef loop_parser<const_iterator_t> string_loop_parser;

void readMesh(const std::string& filename, mesh_t& oMesh) {
  std::ifstream input(filename.c_str());
  std::string aLine;
  string_point_parser aParser;
  while(std::getline(input, aLine)) {
    Point point;
    const_iterator_t iter = aLine.begin();
    const_iterator_t end = aLine.end();
    if(!aLine.empty()) {
      bool aResult = boost::spirit::qi::phrase_parse(iter
						     ,end
						     ,aParser
						     ,boost::spirit::ascii::space
						     ,point);
      if(aResult && iter == end) {
	oMesh.push_back(point);
      } else {
	std::cerr << "Error parsing mesh at " << aLine << std::endl;
      }
    }
  }
}

void readLoop(const std::string& filename, loop_t& oLoop) {
  std::ifstream input(filename.c_str());
  std::string aLine;
  string_loop_parser aParser;
  while(std::getline(input, aLine)) {
    Loop loop;
    const_iterator_t iter = aLine.begin();
    const_iterator_t end = aLine.end();
    if(!aLine.empty()) {
      bool aResult = boost::spirit::qi::phrase_parse(iter
						     ,end
						     ,aParser
						     ,boost::spirit::ascii::space
						     ,loop);
      if(aResult && iter == end) {
	oLoop.push_back(loop);
      } else {
	std::cerr << "Error parsing loops at " << aLine << std::endl;
      }
    }
  }
}
#endif
