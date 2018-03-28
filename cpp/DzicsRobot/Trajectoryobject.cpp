#include "Trajectoryobject.h"
#include <iostream>
#include <fstream>
using namespace std;
TrajectoryObject::TrajectoryObject(QObject *parent) :
    QObject(parent)
{
}

TrajectoryObject::~TrajectoryObject()
{
}

void TrajectoryObject::serialize(bsplines nurbs, char fileName[256])
{
    ofstream arc(fileName, ios::binary|ios::binary);
    arc.write(reinterpret_cast<char *>(&nurbs.degree), sizeof(int));
    arc.write(reinterpret_cast<char *>(&nurbs.knots), nurbs.knotsLen*sizeof(double));
    arc.write(reinterpret_cast<char *>(&nurbs.knotsLen), sizeof(int));
    arc.write(reinterpret_cast<char *>(&nurbs.multipLen), sizeof(int));
    arc.write(reinterpret_cast<char *>(&nurbs.multiplicities), nurbs.multipLen*sizeof(int));
    arc.write(reinterpret_cast<char *>(&nurbs.periodic), sizeof(bool));
    arc.write(reinterpret_cast<char *>(&nurbs.poles), nurbs.polesLen*sizeof(double));
    arc.write(reinterpret_cast<char *>(&nurbs.polesLen), sizeof(int));
    arc.write(reinterpret_cast<char *>(&nurbs.rational), sizeof(bool));
    arc.write(reinterpret_cast<char *>(&nurbs.weightLen), sizeof(int));
    arc.write(reinterpret_cast<char *>(&nurbs.weights), nurbs.weightLen*sizeof(double));
}

void TrajectoryObject::deserialize(bsplines &nurbs, char fileName[256])
{
    ifstream archive(fileName);
    archive.read(reinterpret_cast<char *>(&nurbs.degree),sizeof(int));
    archive.read(reinterpret_cast<char *>(&nurbs.degree), sizeof(int));
    archive.read(reinterpret_cast<char *>(&nurbs.knots), nurbs.knotsLen*sizeof(double));
    archive.read(reinterpret_cast<char *>(&nurbs.knotsLen), sizeof(int));
    archive.read(reinterpret_cast<char *>(&nurbs.multipLen), sizeof(int));
    archive.read(reinterpret_cast<char *>(&nurbs.multiplicities), nurbs.multipLen*sizeof(int));
    archive.read(reinterpret_cast<char *>(&nurbs.periodic), sizeof(bool));
    archive.read(reinterpret_cast<char *>(&nurbs.poles), nurbs.polesLen*sizeof(double));
    archive.read(reinterpret_cast<char *>(&nurbs.polesLen), sizeof(int));
    archive.read(reinterpret_cast<char *>(&nurbs.rational), sizeof(bool));
    archive.read(reinterpret_cast<char *>(&nurbs.weightLen), sizeof(int));
    archive.read(reinterpret_cast<char *>(&nurbs.weights), nurbs.weightLen*sizeof(double));
}

TrajectoryDetail ::TrajectoryDetail(QObject *parent) :
    QObject(parent)
{


}
TrajectoryDetail::~TrajectoryDetail()
{

}
