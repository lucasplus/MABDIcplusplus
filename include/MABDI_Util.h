#ifndef MABDI_UTIL_H
#define MABDI_UTIL_H

#include <string>
#include <vector>

// a quicker way to initialize a vtkSmartPointer
#define UTIL_VTK_INIT( myClass, myObj ) \
  vtkSmartPointer<myClass> myObj = vtkSmartPointer<myClass>::New()

namespace MABDI_Util
{
	extern const std::string StlEnvironmentDir;
	extern const std::string LogConfigFile;

  extern const std::vector<double> BackgroundRGB;
};

const std::string MABDI_Util::StlEnvironmentDir = "C:/Users/Lucas/Code/MABDI/util/stl/environment/";
const std::string MABDI_Util::LogConfigFile = "C:/Users/Lucas/Code/MABDI/util/LogConfig.conf";

double tmp[] = { 238, 232, 170 };
const std::vector<double> MABDI_Util::BackgroundRGB( tmp, tmp + sizeof(tmp)/sizeof(tmp[0]) );

#endif // MABDI_UTIL_H
