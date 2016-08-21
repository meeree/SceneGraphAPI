#ifndef __INCLUDED_SCENENODE_H__
#define __INCLUDED_SCENENODE_H__

#include <vector>
#include <stack>
#include <array>

class Object;
typedef std::array<std::array<double,4>,4> matType;
 
/* Scene Node class
 * Not intended to be used by the user; it is only designed
 * for inheritance reasons */

class SceneNode
{
public:
    virtual void retrieve ( std::stack<Object*> &objStack, std::stack<matType>& matStack ) = 0;             
    void addChild ( SceneNode* child ); 
protected:
    SceneNode () = default;
    SceneNode ( std::vector<SceneNode*> const& children_ );
    std::vector<SceneNode*> children;
};

/* Group Scene Node class
 * Pretty much an exact replica of the scene node class 
 * intended as to be used as the filler node in the 
 * scene graph */

class GroupSceneNode : public SceneNode 
{
public:
    GroupSceneNode ( std::vector<SceneNode*> const& children_ );
    void retrieve ( std::stack<Object*> &objStack, std::stack<matType>& matStack ) override;
};

/* Transformation Scene Node class
 * Designed to be inhertied by other classes;
 * handles transformation functions and the 
 * transformation matrix */

class TransSceneNode : public SceneNode
{
public:
    void transform ( matType const& concMat, bool left=true );
protected:
    static void leftMul ( matType& mat1, matType const& mat2 );
    static void rightMul ( matType& mat1, matType const& mat2 );

    TransSceneNode () = default;
    TransSceneNode ( std::vector<SceneNode*> const& children_, matType const& mat_ );
    matType mat;

};

/* Tranformation Group Scene Node class
 * Filler class like the other group scene node class
 * except it also has a transformation, allowing 
 * for the user to make mutliple instances of a scene
 * with different transforms using this node */

class TransGroupSceneNode : public TransSceneNode 
{
public:
    TransGroupSceneNode ( std::vector<SceneNode*> const& children_, matType const& mat_= {{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}} );
    void retrieve ( std::stack<Object*> &objStack, std::stack<matType>& matStack ) override;
};

/* Object Scene Node class
 * The scene node that will hold an object and handle 
 * what to do with this object when called by other
 * classes */

class ObjSceneNode : public TransSceneNode 
{
public:
    ObjSceneNode ( Object* obj_, std::vector<SceneNode*> const& children_ = {}, matType const& mat_ = {{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}} );
    ObjSceneNode ( Object* obj_, matType const& mat_ );
    void retrieve ( std::stack<Object*> &objStack, std::stack<matType>& matStack ) override;
private:
    Object* obj;
    /* this is a stack because we can have multiple paths to 
     * one object: leading to multiple sets of vertices */
    std::stack<std::vector<double>> relativeVerts;
};

#endif
