#ifndef __INCLUDED_VIEW_OPENGL_4_H__
#define __INCLUDED_VIEW_OPENGL_4_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <map>
#include <vector>

/* FIX THIS */
#include "object.h"

class SceneNode;

class View 
{
private:
    struct GraphicsContainer {
        GraphicsContainer ( GLuint const& width_, GLuint const& height_ );
        GLuint loadInShader(GLenum const &shaderType, char const *fname);

        GLFWwindow* window;
        GLuint width, height;
        GLuint vao, vbo;
        GLuint shaderProg;
        GLuint vertShader, fragShader;
    };

    SceneNode* root;
public:
    View ( GLuint const& width_, GLuint const& height_, SceneNode* root_ ); 
    void draw ();
    /* MAKE THIS PRIVATE */
    GraphicsContainer graphics;
};

#endif
