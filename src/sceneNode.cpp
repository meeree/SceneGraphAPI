#include "sceneNode.h"
#include "object.h"

void SceneNode::retrieve ( objPosType& objPosStack, std::stack<QuadMat>& matStack )
{
    for ( auto const& child: this->children ) 
    {
        child->retrieve ( objPosStack, matStack );
    }
}

void SceneNode::addChild ( SceneNode* child )
{
    this->children.push_back ( child );
    child->notify ();
}

/* this is important for nodes that support instancing as the
 * new parent means they have to allocate memory for that new
 * path */
void SceneNode::notify ()
{
}

SceneNode::SceneNode ( std::vector<SceneNode*> const& children_ )
    : children { children_ }
{
    for ( auto const& child: children ) 
    {
        child->notify ();
    }
}

GroupSceneNode::GroupSceneNode ( std::vector<SceneNode*> const& children_ )
    : SceneNode { children_ } 
{
}

void TransSceneNode::transform ( QuadMat const& concMat, bool left )
{
    if ( left ) 
    {
        this->transMat.leftMul ( concMat );
        return;
    }
    this->transMat.rightMul ( concMat );
}

TransSceneNode::TransSceneNode ( std::vector<SceneNode*> const& children_, QuadMat const& transMat_ ) 
    : SceneNode { children_ }, transMat { transMat_ } 
{
}

TransGroupSceneNode::TransGroupSceneNode ( std::vector<SceneNode*> const& children_, QuadMat const& transMat_ )
    : TransSceneNode { children_, transMat_ }
{
}

void TransGroupSceneNode::retrieve ( objPosType& objPosStack, std::stack<QuadMat>& matStack )
{
    QuadMat totalMat { matStack.top () };
    totalMat.leftMul ( this->transMat );
    /* push matrix */
    matStack.push ( std::move ( totalMat ) );
    for ( auto const& child: this->children ) 
    {
        child->retrieve ( objPosStack, matStack );
    }
    /* pop matrix */
    matStack.pop ();
}

ObjSceneNode::ObjSceneNode ( Object* obj_, std::vector<SceneNode*> const& children_, QuadMat const& transMat_ )
    : TransSceneNode { children_, transMat_ }, obj { obj_ }, vertHeadCnt { 0 } 
{
    this->relativeVerts.resize ( this->obj->verts.size () );
}

ObjSceneNode::ObjSceneNode ( Object* obj_, QuadMat const& transMat_ )
    : TransSceneNode { {}, transMat_ }, obj { obj_ }, vertHeadCnt { 0 } 
{
    this->relativeVerts.resize ( this->obj->verts.size () );
}

void ObjSceneNode::retrieve ( objPosType& objPosStack, std::stack<QuadMat>& matStack ) 
{
    QuadMat totalMat { matStack.top () };
    totalMat.leftMul ( this->transMat );

    for ( unsigned i = vertHeadCnt; i < vertHeadCnt + this->obj->verts.size (); i += 3 ) 
    {
        this->relativeVerts[i] = this->obj->verts[i-vertHeadCnt];
        this->relativeVerts[i+1] = this->obj->verts[i+1-vertHeadCnt];
        this->relativeVerts[i+2] = this->obj->verts[i+2-vertHeadCnt];
        double &x { this->relativeVerts[i] };
        double &y { this->relativeVerts[i+1] };
        double &z { this->relativeVerts[i+2] };
        totalMat.transformVert ( x, y, z );
    }

    /* push matrix */
    matStack.push ( std::move ( totalMat ) );
    for ( auto const& child: this->children ) 
    {
        child->retrieve ( objPosStack, matStack );
    }
    /* pop matrix */
    matStack.pop ();

    /* vert head count will tell us our current position in 
     * relative verts: each path should increment it 
     * until it gets to the last set of vertices.
     * When we get to the end position of relativeVerts, as below, 
     * we will reset the counter and put our object and verts on
     * the context stack */
    vertHeadCnt += this->obj->verts.size ();
    if ( vertHeadCnt == this->relativeVerts.size () ) {
        objPosStack.push ( { this->obj, this->relativeVerts } );
        vertHeadCnt = 0;
    }
}

void ObjSceneNode::notify () 
{
    /* allocate space for new path */
    relativeVerts.resize ( relativeVerts.size () + obj->verts.size () ); 
}
