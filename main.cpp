#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <sstream>
#include "map"
#include "Points.h"
std::map<int,Point> vmap;
int i=0;
std::vector<Point> vertices;
std::vector<std::vector<unsigned int>> faces;

float minx = INFINITY;
float miny = INFINITY;
float minz = INFINITY;
float maxx = -INFINITY;
float maxy = -INFINITY;
float maxz = -INFINITY;
void importOBJ(const char *file_in) {
    std::ifstream infile(file_in);
    std::string line= "";
    while (std::getline(infile, line)) {
        std::stringstream str_str;
        str_str.clear();
        str_str.str(line);
        std::string prefix = "";
        str_str >> prefix;
        if (prefix == "v") {
            i++;
            float x, y, z;
            str_str >> x >> y >> z;
            Point vertex={x,y,z};
            vmap[i]=vertex;
            vertices.push_back(vertex);
            if (x<minx){minx = x;}
            if (y<miny){miny = y;}
            if (z<minz){minz = z;}
            if (x>maxx){maxx = x;}
            if (y>maxy){maxy = y;}
            if (z>maxz){maxz = z;}
        }
        else if (prefix == "f") {
            unsigned int v1,v2,v3;
            str_str >> v1 >> v2 >> v3;
            std::vector<unsigned int> temp = {v1,v2,v3};
            faces.push_back(temp);
        }
        }
        std::cout << vertices.size() << std::endl;
        std::cout << faces.size() << std::endl;
}
void exportCityJSON(const char *file_out) {
    std::ofstream outfile(file_out);
    outfile <<"{\n";
    outfile <<"  \"type\": \"CityJSON\",\n";
    outfile <<"  \"version\": \"1.0\",\n";
    outfile <<"  \"CityObjects\": {\n";
    outfile << "       \"MyTerrain\": {\n";
    outfile << "          \"type\": \"TINRelief\",\n";
    outfile << "          \"geographicalExtent\": [ "<<minx<<", "<<miny<<", "<<minz<<", "<<maxx<<", "<<maxy<<", "<<maxz<<" ],\n";
    outfile << "          \"geometry\": [{\n";
    outfile << "              \"type\": \"CompositeSurface\",\n";
    outfile << "              \"lod\": 1,\n";
    outfile << "              \"boundaries\": [\n";
    std::string delimeter="";
    for (auto const &t:faces) {
        outfile << delimeter << "[[";
        outfile << t[0] - 1 << ", " << t[1] - 1 << ", " << t[2] - 1 << "]]";
        delimeter = ", ";
    }
    outfile<<"\n";
    outfile<<"          ]\n";
    outfile<<"      }]\n";
    outfile<<"      }\n";
    outfile<<"    },"<<"\n";
    outfile << "\"vertices\": [\n";
    std::string d="";
    for (auto const &v:vertices){
        outfile<<"      "<<d<<"[ "<<v.x<<", "<<v.y<<", "<<v.z<<"]\n";
        d=", ";
    }
    //close
    outfile<<"   ]\n";
    outfile<<"}\n";
    outfile.close();
}
int main(int argc, const char * argv[]) {
    const char *file_in = "C:\\Users\\User\\Desktop\\Msc_Geomatics\\Courses\\3rd_semester\\GEO1004\\hw03\\wetransfer-51df86\\OBJ_EXPORT.obj";
    const char *file_out = "C:\\Users\\User\\Desktop\\Msc_Geomatics\\Courses\\3rd_semester\\GEO1004\\hw03\\wetransfer-51df86\\Tin.json";
    importOBJ(file_in);
    exportCityJSON(file_out);
    return 0;
}