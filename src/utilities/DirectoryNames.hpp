#ifndef DIRECTORYNAMES_H
#define DIRECTORYNAMES_H

#include <string>

class DirectoryNames
{
  static const std::string PlyDirString;
  
public:
  static std::string getPlyDir() { return PlyDirString; }
	
};

const std::string DirectoryNames::PlyDirString = "C:/Users/Lucas/Code/testPCL/src/util/plyFiles/";

#endif // DIRECTORYNAMES_H

