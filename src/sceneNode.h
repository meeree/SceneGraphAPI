#ifndef __INCLUDED_SCENENODE_H__
#define __INCLUDED_SCENENODE_H__

#include <vector>
#include <stack>
#include <array>
#include <map>

#include "quadMat.h"

class Object;
typedef std::stack<std::pair<Object*,std::vector<double>>> objPosType;

struct SceneContext 
{
    std::stack<std::pair<Object*,std::vector<double>>> objPosStack;
    std::stack<QuadMat> matStack;
};
 
/* Scene Node class
 * Not intended to be used by the user; it is only designed
 * for inheritance reasons */

class SceneNode
{
public:
    void addChild ( SceneNode* child ); 
    virtual void retrieve ( objPosType& objPosStack, std::stack<QuadMat>& matStack );             
protected:
    SceneNode () = default;
    SceneNode ( std::vector<SceneNode*> const& children_ );
    std::vector<SceneNode*> children;
    virtual void notify ();
};

/* Group Scene Node class
 * Pretty much an exact replica of the scene node class 
 * intended as to be used as the filler node in the 
 * scene graph */

class GroupSceneNode : public SceneNode 
{
public:
    GroupSceneNode ( std::vector<SceneNode*> const& children_ );
};

/* Transformation Scene Node class
 * Designed to be inhertied by other classes;
 * handles transformation functions and the 
 * transformation matrix */

class TransSceneNode : public SceneNode
{
public:
    void transform ( QuadMat const& concMat, bool left=true );
protected:
    TransSceneNode () = default;
    TransSceneNode ( std::vector<SceneNode*> const& children_, QuadMat const& transMat_ );
    QuadMat transMat;
};

/* Transformation Group Scene Node class
 * Filler class like the other group scene node class
 * except it also has a transformation, allowing 
 * for the user to make multiple instances of a scene
 * with different transforms using this node */

class TransGroupSceneNode : public TransSceneNode 
{
public:
    TransGroupSceneNode ( std::vector<SceneNode*> const& children_, QuadMat const& transMat_= {{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}} );
    void retrieve ( objPosType& objPosStack, std::stack<QuadMat>& matStack ) override;
};

/* Object Scene Node class
 * The scene node that will hold an object and handle 
 * what to do with this object when called by other
 * classes */

class ObjSceneNode : public TransSceneNode 
{
public:
    ObjSceneNode ( Object* obj_, std::vector<SceneNode*> const& children_ = {}, QuadMat const& transMat_ = {{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}} );
    ObjSceneNode ( Object* obj_, QuadMat const& transMat_ );
    void retrieve ( objPosType& objPosStack, std::stack<QuadMat>& matStack ) override;
private:
    Object* obj;
    /* this will hold the verts of potentially more than one
     * object, as different paths can lead to different instances */
    std::vector<double> relativeVerts;
    void notify () override;
	unsigned vertHeadCnt;
};

#endif
