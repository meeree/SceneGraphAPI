#include "object.h"

Object::Object ( Object::obj_type_t const& type_, std::vector<double> const& verts_ ) 
    : type { type_ }, verts { verts_ }, dynamVerts { verts_ }
{
}

std::vector<double> const& Object::getDynamVerts () const
{
    return dynamVerts;
}

void Object::transform ( std::array<std::array<double,4>,4> const& mat ) 
{
    for ( int i = 0; i < 3*3*2*6; i += 3 ) 
    {
        dynamVerts[i] = mat[0][0]*verts[i]+mat[0][1]*verts[i+1]+mat[0][2]*verts[i+2]+mat[0][3];
        dynamVerts[i+1] = mat[1][0]*verts[i]+mat[1][1]*verts[i+1]+mat[1][2]*verts[i+2]+mat[1][3];
        dynamVerts[i+2] = mat[2][0]*verts[i]+mat[2][1]*verts[i+1]+mat[2][2]*verts[i+2]+mat[2][3];
    }
}

Cube::Cube () 
    : Object { Object::obj_type_t::CUBE, 
    {
        -0.5,-0.5,-0.5,
        -0.5,-0.5, 0.5,
        -0.5, 0.5, 0.5,
        0.5, 0.5,-0.5,
        -0.5,-0.5,-0.5,
        -0.5, 0.5,-0.5,
        0.5,-0.5, 0.5,
        -0.5,-0.5,-0.5,
        0.5,-0.5,-0.5,
        0.5, 0.5,-0.5,
        0.5,-0.5,-0.5,
        -0.5,-0.5,-0.5,
        -0.5,-0.5,-0.5,
        -0.5, 0.5, 0.5,
        -0.5, 0.5,-0.5,
        0.5,-0.5, 0.5,
        -0.5,-0.5, 0.5,
        -0.5,-0.5,-0.5,
        -0.5, 0.5, 0.5,
        -0.5,-0.5, 0.5,
        0.5,-0.5, 0.5,
        0.5, 0.5, 0.5,
        0.5,-0.5,-0.5,
        0.5, 0.5,-0.5,
        0.5,-0.5,-0.5,
        0.5, 0.5, 0.5,
        0.5,-0.5, 0.5,
        0.5, 0.5, 0.5,
        0.5, 0.5,-0.5,
        -0.5, 0.5,-0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5,-0.5,
        -0.5, 0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
        0.5,-0.5, 0.5
    }}
{
}
