#ifndef __INCLUDED_OBJECT_H__
#define __INCLUDED_OBJECT_H__

#include <vector>
#include <array>

class Object 
{
public:
    enum class obj_type_t;
    obj_type_t type;
    Object () = delete;

    std::vector<double> const& getDynamVerts () const;
    void transform ( std::array<std::array<double,4>,4> const& mat );
protected: 
    Object ( obj_type_t const& type_, std::vector<double> const& verts_ );

    /* using vector here is pretty bad because it means I 
     * can change the size of it, unfortunately I have to
     * use it because I want a variable length array but I 
     * definitely do not want to use a template on Object */
    std::vector<double> const verts;
};

class Cube : public Object
{
public:
    Cube ();
    /* incorporate transform function into object class for generalization */
};

/* this should be in .cpp file */
enum class Object::obj_type_t 
{
    CUBE
};

#endif
