
#include <pcl/io/ply_io.h> // loadPLYFile() 

#include <pcl/visualization/pcl_visualizer.h> // PCLVisualizer

#include <iostream>
#include <string>


int main(int argc, char *argv[])
{
  // initialize a mesh and load a ply file to it
  pcl::PolygonMesh tableMesh;
  pcl::PolygonMesh rightCupMesh;
  pcl::PolygonMesh leftCupMesh;
  pcl::PolygonMesh floorMesh;

  // read file in
  pcl::io::loadPLYFile( "utilities/plyFiles/table_2.ply", tableMesh );
  pcl::io::loadPLYFile( "utilities/plyFiles/right_cup.ply", rightCupMesh );
  pcl::io::loadPLYFile( "utilities/plyFiles/left_cup.ply", leftCupMesh );
  pcl::io::loadPLYFile( "utilities/plyFiles/floor.ply", floorMesh );

  // start visualizer 
  pcl::visualization::PCLVisualizer myPCLViewer("hai");
  myPCLViewer.addPolygonMesh( tableMesh, "hai1" );
  myPCLViewer.addPolygonMesh( rightCupMesh, "hai2" );
  myPCLViewer.addPolygonMesh( leftCupMesh, "hai3" );
  myPCLViewer.addPolygonMesh( floorMesh, "hai4" );
  myPCLViewer.setBackgroundColor( 1.0000f,0.9725f, 0.8627f );
  myPCLViewer.setShapeRenderingProperties(
    pcl::visualization::PCL_VISUALIZER_COLOR, 
    1.0, 1.0, 1.0, "hai2");
  std::cout << tableMesh.header;

  myPCLViewer.spin();
}
