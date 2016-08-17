#include "object.h"

Object::Object ( Object::obj_type_t const& type_ ) 
    : type { type_ }
{
}

Cube::Cube () 
    : Object { Object::obj_type_t::CUBE }
{
}

double Cube::verts [3*3*2*6]
{
    -0.5,-0.5,0.5, -0.5,0.5,0.5, 0.5,-0.5,0.5,
    0.5,0.5,0.5, 0.5,-0.5,0.5, -0.5,0.5,0.5,

    -0.5,-0.5,-0.5, -0.5,0.5,-0.5, 0.5,-0.5,-0.5,
    0.5,0.5,-0.5, -0.5,0.5,-0.5, 0.5,-0.5,-0.5,

    -0.5,0.5,0.5, -0.5,0.5,-0.5, 0.5,0.5,0.5,
    0.5,0.5,-0.5, 0.5,0.5,0.5, -0.5,0.5,-0.5,

    -0.5,-0.5,0.5, -0.5,-0.5,-0.5, 0.5,-0.5,0.5,
    0.5,-0.5,-0.5, 0.5,-0.5,0.5, -0.5,-0.5,-0.5,

    0.5,-0.5,0.5, 0.5,0.5,0.5, 0.5,-0.5,-0.5,
    0.5,-0.5,-0.5, 0.5,-0.5,-0.5, 0.5,0.5,0.5,

    -0.5,-0.5,0.5, -0.5,0.5,0.5, -0.5,-0.5,-0.5,
    -0.5,-0.5,-0.5, -0.5,-0.5,-0.5, -0.5,0.5,0.5
};

void Cube::transform ( double const (&transMat)[4][4] ) 
{
    for ( int i = 0; i < 3*3*2*6; i += 3 ) 
    {
        dynamVerts[i] = transMat[0][0]*dynamVerts[i]+transMat[0][1]*dynamVerts[i+1]+transMat[0][2]*dynamVerts[i+2]+transMat[0][3];
        dynamVerts[i+1] = transMat[1][0]*dynamVerts[i]+transMat[1][1]*dynamVerts[i+1]+transMat[1][2]*dynamVerts[i+2]+transMat[1][3];
        dynamVerts[i+2] = transMat[2][0]*dynamVerts[i]+transMat[2][1]*dynamVerts[i+1]+transMat[2][2]*dynamVerts[i+2]+transMat[2][3];
    }
}
