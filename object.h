#ifndef __INCLUDED_OBJECT_H__
#define __INCLUDED_OBJECT_H__

class Object 
{
public:
    enum obj_type_t {
        CUBE
    };
    obj_type_t type;
protected: 
    Object ( obj_type_t const& type_ );
};

class Cube : public Object 
{
public:
    Cube ();
    /* incorporate transform function into object class for generalization */
    void transform ( double const (&transMat)[4][4] );
    double dynamVerts [3*3*2*6];
private:
    static double verts [3*3*2*6];
};

#endif
