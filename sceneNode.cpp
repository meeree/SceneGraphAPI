#include "sceneNode.h"
#include "object.h"

void SceneNode::addChild ( SceneNode* child )
{
    this->children.push_back ( child );
}

SceneNode::SceneNode ( std::vector<SceneNode*> const& children_ )
    : children { children_ }
{
}

GroupSceneNode::GroupSceneNode ( std::vector<SceneNode*> const& children_ )
    : SceneNode { children_ } 
{
}

void GroupSceneNode::retrieve ( std::stack<Object*> &objStack, double (&totalTransMat)[4][4] ) 
{
    for ( auto const& child: this->children ) 
    {
        child->retrieve ( objStack, totalTransMat );
    }
}

void TransSceneNode::transform ( double const (&concTransMat)[4][4] )
{
    this->transMul ( this->transMat, concTransMat );
}

void TransSceneNode::transMul ( double (&transMat1)[4][4], double const (&transMat2)[4][4] )
{
    for ( int i = 0; i < 4; ++i ) 
    {
        for ( int j = 0; j < 4; ++j ) 
        {
            transMat1[i][j] = transMat2[i][0]*transMat1[0][j]+transMat2[i][1]*transMat1[1][j]+transMat2[i][2]*transMat1[2][j]+transMat2[i][3]*transMat1[3][j];
        }
    }
}

TransSceneNode::TransSceneNode ( std::vector<SceneNode*> const& children_, double const (&transMat_)[4][4] ) 
    : SceneNode { children_ } 
{
    for ( int i = 0; i < 4; ++i ) 
    {
        for ( int j = 0; j < 4; ++j ) 
        {
            this->transMat[i][j] = transMat_[i][j];
        }
    }
}

TransGroupSceneNode::TransGroupSceneNode ( std::vector<SceneNode*> const& children_, double const (&transMat_)[4][4] )
    : TransSceneNode { children_, transMat_ }
{
}

void TransGroupSceneNode::retrieve ( std::stack<Object*> &objStack, double (&totalTransMat)[4][4] )
{
    this->transMul ( totalTransMat, this->transMat );
    for ( auto const& child: this->children ) 
    {
        child->retrieve ( objStack, totalTransMat );
    }
}

ObjSceneNode::ObjSceneNode ( Object* obj_, std::vector<SceneNode*> const& children_, double const(&transMat_)[4][4] )
    : TransSceneNode { children_, transMat_ }, obj { obj_ } 
{
}


ObjSceneNode::ObjSceneNode ( Object* obj_, double const(&transMat_)[4][4] )
    : TransSceneNode { {}, transMat_ }, obj { obj_ } 
{
}

void ObjSceneNode::retrieve ( std::stack<Object*> &objStack, double (&totalTransMat)[4][4] ) 
{
    this->transMul ( totalTransMat, this->transMat );
    /* WARNING: A GIANT NO-NO FIX THIS ASAP */
    static_cast<Cube*>(this->obj)->transform ( totalTransMat );
    objStack.push ( obj );
    for ( auto const& child: this->children ) 
    {
        child->retrieve ( objStack, totalTransMat );
    }
}
