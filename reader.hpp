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
template<typename STRUCTURE>
struct parser {
  typedef void type;
};
template<>
struct parser<mesh_t> {
  typedef string_point_parser type;
};
template<>
struct parser<loop_t> {
  typedef string_loop_parser type;
};


struct reader {
  reader(const std::string& filename) : _filename(filename) {}

  template <typename STRUCTURE>
  void operator()(STRUCTURE& data) const {
    std::ifstream input(_filename.c_str());
    std::string aLine;
    typename parser<STRUCTURE>::type aParser;
    while(std::getline(input, aLine)) {
      (*this)(aLine, aParser, data);
    }
  }

  template<typename STRUCTURE, typename PARSER>
  void operator()(const std::string& line, const PARSER& parser, STRUCTURE& data) const {
    typename STRUCTURE::value_type element;
    const_iterator_t iter = line.begin();
    const_iterator_t end = line.end();
    if(!line.empty()) {
      bool aResult = boost::spirit::qi::phrase_parse(iter
						     ,end
						     ,parser
						     ,boost::spirit::ascii::space
						     ,element);
      if(aResult && iter == end) {
	data.push_back(element);
      } else {
	std::cerr << "Error parsing at " << line << std::endl;
      } 
    }
  }

  std::string _filename;
};
#endif
