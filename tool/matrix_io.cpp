#include "matrix_io.hpp"
#include<fstream>
#include<sstream>
#include<string>
using namespace std;
void square_matrix_input(const char * filename,
		  MatrixXd & res) throw (Exc) {
 
  ifstream myfile (filename);
  string line;
  stringstream sstr;
  long size;
  double d;
  if (!myfile.is_open())
    EXC_PRINT_2("Unable to open file", filename);
  
  getline(myfile,line);
  sstr<<line;
  size = 0;
  while(!sstr.eof()) {
    sstr>>d;
    size++;
  };
  
  MatrixXd m(size,size);
  sstr.clear();
  sstr<<line;
  int j = 0;
  int i = 0;
  while(!sstr.eof()) 
    sstr>>m(i,j++);
 
  while ( myfile.good() )
    {
      i++;
      j=0;
      getline (myfile,line);
      if (line.find_first_not_of(' ') == std::string::npos)
	break;
      sstr.clear();
      sstr<<line;
      while(!sstr.eof())
	{
	  sstr>>d;
	  if ((i==size)||(j==size))
	    EXC_PRINT_2("matrix incorrect in file:", filename);
	  m(i,j++)=d;
	};
    }
  myfile.close();
  res = m;
  return;
}



void column_vector_input(const char * filename,
			 VectorXd & res) throw (Exc) {
  ifstream myfile (filename);
  string line;
  double d;
  stringstream sstr;
  if (!myfile.is_open())
    EXC_PRINT_2("unable to open file", filename);

  int numLines=0, i = 0;
  while (std::getline(myfile, line))
    if (line.find_first_not_of(' ') == std::string::npos)
      break;
    else
      ++numLines;
  
  myfile.clear();
  myfile.seekg(0,myfile.beg);
 
  VectorXd v(numLines);
  while ( (myfile.good())&(i<numLines))
    {
      getline (myfile,line);
      sstr.clear();
      sstr<<line;
      sstr>>d;
      v(i++)= d;
    };

  myfile.close();
  res = v;
  return;
}
    




