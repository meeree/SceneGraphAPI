#ifndef __INCLUDED_SCENENODE_H__
#define __INCLUDED_SCENENODE_H__

#include <vector>
#include <stack>

class Object;
 
/* Scene Node class
 * Not intended to be used by the user; it is only designed
 * for inheritance reasons */

class SceneNode
{
public:
    virtual void retrieve ( std::stack<Object*> &objStack, double (&totalTransMat)[4][4] ) = 0;             
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
    void retrieve ( std::stack<Object*> &objStack, double (&totalTransMat)[4][4] ) override;
};

/* Transformation Scene Node class
 * Designed to be inhertied by other classes;
 * handles transformation functions and the 
 * transformation matrix */

class TransSceneNode : public SceneNode
{
public:
    void transform ( double const (&concTransMat)[4][4] );
protected:
    static void transMul ( double (&transMat1)[4][4], double const (&transMat2)[4][4] );

    TransSceneNode () = default;
    TransSceneNode ( std::vector<SceneNode*> const& children_, double const (&transMat_)[4][4] );
    double transMat [4][4];
};

/* Tranformation Group Scene Node class
 * Filler class like the other group scene node class
 * except it also has a transformation, allowing 
 * for the user to make mutliple instances of a scene
 * with different transforms using this node */

class TransGroupSceneNode : public TransSceneNode 
{
public:
    TransGroupSceneNode ( std::vector<SceneNode*> const& children_, double const (&transMat_)[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}} );
    void retrieve ( std::stack<Object*> &objStack, double (&totalTransMat)[4][4] ) override;
};

/* Object Scene Node class
 * The scene node that will hold an object and handle 
 * what to do with this object when called by other
 * classes */

class ObjSceneNode : public TransSceneNode 
{
public:
    ObjSceneNode ( Object* obj_, std::vector<SceneNode*> const& children_={}, double const(&transMat_)[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}} );
    ObjSceneNode ( Object* obj_, double const(&transMat_)[4][4] );
    void retrieve ( std::stack<Object*> &objStack, double (&totalTransMat)[4][4] ) override;
private:
    Object* obj;
};

#endif
