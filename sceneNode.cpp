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

void GroupSceneNode::retrieve ( std::stack<Object*> &objStack, std::stack<matType>& matStack ) 
{
    for ( auto const& child: this->children ) 
    {
        child->retrieve ( objStack, matStack );
    }
}

void TransSceneNode::transform ( matType const& concMat, bool left )
{
    if ( left ) 
    {
        this->leftMul ( this->mat, concMat );
        return;
    }
    this->rightMul ( this->mat, concMat );
}

void TransSceneNode::leftMul ( matType& mat1, matType const &mat2 )
{
    for ( int i = 0; i < 4; ++i ) 
    {
        for ( int j = 0; j < 4; ++j ) 
        {
            mat1[i][j] = mat2[i][0]*mat1[0][j]+mat2[i][1]*mat1[1][j]+mat2[i][2]*mat1[2][j]+mat2[i][3]*mat1[3][j];
        }
    }
}

void TransSceneNode::rightMul ( matType& mat1, matType const& mat2 )
{
    for ( int i = 0; i < 4; ++i ) 
    {
        for ( int j = 0; j < 4; ++j ) 
        {
            mat1[i][j] = mat1[i][0]*mat2[0][j]+mat1[i][1]*mat2[1][j]+mat1[i][2]*mat2[2][j]+mat1[i][3]*mat2[3][j];
        }
    }
}

TransSceneNode::TransSceneNode ( std::vector<SceneNode*> const& children_, matType const& mat_ ) 
    : SceneNode { children_ } 
{
    for ( int i = 0; i < 4; ++i ) 
    {
        for ( int j = 0; j < 4; ++j ) 
        {
            this->mat[i][j] = mat_[i][j];
        }
    }
}

TransGroupSceneNode::TransGroupSceneNode ( std::vector<SceneNode*> const& children_, matType const& mat_ )
    : TransSceneNode { children_, mat_ }
{
}

void TransGroupSceneNode::retrieve ( std::stack<Object*> &objStack, std::stack<matType>& matStack )
{
    matType totalMat { matStack.top () };
    this->leftMul ( totalMat, this->mat );
    /* push matrix */
    matStack.push ( std::move ( totalMat ) );
    for ( auto const& child: this->children ) 
    {
        child->retrieve ( objStack, matStack );
    }
    /* pop matrix */
    matStack.pop ();
}

ObjSceneNode::ObjSceneNode ( Object* obj_, std::vector<SceneNode*> const& children_, matType const& mat_ )
    : TransSceneNode { children_, mat_ }, obj { obj_ } 
{
}


ObjSceneNode::ObjSceneNode ( Object* obj_, matType const& mat_ )
    : TransSceneNode { {}, mat_ }, obj { obj_ } 
{
}

void ObjSceneNode::retrieve ( std::stack<Object*> &objStack, std::stack<matType>& matStack ) 
{
    matType totalMat { matStack.top () };
    this->leftMul ( totalMat, this->mat );

    this->obj->transform ( totalMat );
    objStack.push ( obj );

    /* push matrix */
    matStack.push ( std::move ( totalMat ) );

    for ( auto const& child: this->children ) 
    {
        child->retrieve ( objStack, matStack );
    }
    /* pop matrix */
    matStack.pop ();
}
