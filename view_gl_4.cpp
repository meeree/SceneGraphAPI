#include "view_gl_4.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <array>

#include "sceneNode.h"

View::View ( GLuint const& width_, GLuint const& height_, SceneNode* root_ ) 
    : graphics { width_, height_ }, root { root_ } 
{
}

View::GraphicsContainer::GraphicsContainer ( GLuint const& width_, GLuint const& height_ )
    : width { width_ }, height { height_ } 
{
    if ( !glfwInit () ) {
        std::cerr<<"Failed to init glfw."<<std::endl;
        exit (0);
    }

    this->window = glfwCreateWindow(this->width, this->height, "sandbox", NULL, NULL);
    if ( !window ) {
        std::cerr<<"Failed to init window."<<std::endl;
        glfwTerminate ();
        exit (0);
    }
    glfwMakeContextCurrent ( window );

    glewExperimental = GL_TRUE;
    if ( glewInit () != GLEW_OK ) {
        std::cerr<<"Failed to init glew."<<std::endl;
        exit (0);
    }

    glEnable ( GL_DEPTH_TEST );
    glDepthFunc ( GL_LESS );
    
    this->vertShader = this->loadInShader ( GL_VERTEX_SHADER, "./Shaders/viewer_vert.glsl" );
    this->fragShader = this->loadInShader ( GL_FRAGMENT_SHADER, "./Shaders/viewer_frag.glsl" );

    this->shaderProg = glCreateProgram ();
    glAttachShader ( this->shaderProg, this->vertShader ); 
    glAttachShader ( this->shaderProg, this->fragShader );

    glLinkProgram ( this->shaderProg );
    glUseProgram ( this->shaderProg );

    glGenBuffers ( 1, &this->vbo );

    glEnableVertexAttribArray (0);
    glBindBuffer ( GL_ARRAY_BUFFER, this->vbo );
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );  

}

GLuint View::GraphicsContainer::loadInShader(GLenum const &shaderType, char const *fname) 
{
   std::vector<char> buffer;
   std::ifstream in;
   in.open(fname, std::ios::binary);

   if (in.is_open()) {
      in.seekg(0, std::ios::end);
      size_t const &length = in.tellg();

      in.seekg(0, std::ios::beg);

      buffer.resize(length + 1);
      in.read(&buffer[0], length);
      in.close();
      buffer[length] = '\0';
   } else {
      std::cerr << "Unable to open " << fname << std::endl;
      exit(-1);
   }

   GLchar const *src = &buffer[0];

   GLuint shader = glCreateShader(shaderType);
   glShaderSource(shader, 1, &src, NULL);
   glCompileShader(shader);
   GLint test;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &test);

   if (!test) {
      std::cerr << "Shader compilation failed with this message:" << std::endl;
      std::vector<char> compilationLog(512);
      glGetShaderInfoLog(shader, compilationLog.size(), NULL, &compilationLog[0]);
      std::cerr << &compilationLog[0] << std::endl;
      glfwTerminate();
      exit(-1);
   }

   return shader;
}

void View::draw () 
{
    std::stack<std::array<std::array<double,4>,4>> matStack {};
    matStack.push ( {{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}} );
    root->retrieve ( objStack, matStack );
    while ( objStack.size () > 0 ) 
    {
        Object* obj { objStack.top () };
        objStack.pop ();
        auto const& dynamVerts { obj->getDynamVerts () };
        switch ( obj->type ) 
        {
            case Object::obj_type_t::CUBE:
                std::vector<GLfloat>& placeVec { vertMap[Object::obj_type_t::CUBE] };
                for ( auto const &vert: dynamVerts ) 
                {
                    placeVec.push_back ( vert );
                }
                break;
        }
    }
    
    /* ALL THE STUFF ABOVE SHOULD BE PUT INTO A GENERAL FUNCTION ON ANOTHER CLASS
     * BECAUSE THE BELOW STUFF IS FOR SPECIFIC RENDERING BUT THE ABOVE IS NOT */

    /* clear the scene */
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindVertexArray ( graphics.vao );
    glBindBuffer ( GL_ARRAY_BUFFER, graphics.vbo );

    for ( auto& vertMapping: vertMap ) 
    {
        switch ( vertMapping.first ) 
        {
            case Object::obj_type_t::CUBE:
                /* apply special cube renderer */
                std::vector<GLfloat> &verts { vertMapping.second };
                glBufferData ( GL_ARRAY_BUFFER, verts.size () * sizeof ( GLfloat ), verts.data (), GL_DYNAMIC_DRAW );
                /* draw the cubes */
                glDrawArrays ( GL_TRIANGLES, 0, (GLint)( verts.size()/3 ) );
                verts.clear ();
                break;
        }
    }

    /* update the scene */
    glfwSwapBuffers ( graphics.window );
    glfwPollEvents ();
}

int main () 
{
    /*NOTE: GET INSTANCING WORKING! */
    Cube c2 {};
    Cube c {};

    ObjSceneNode s2 { &c2 };
    ObjSceneNode s { &c, {&s2} };

    View v { 700, 700, &s };
    /* scaling */ 
    s2.transform ( {{{0.1,0,0,0},{0,0.1,0,0},{0,0,0.1,0},{0,0,0,1}}} );
    /* translation */
//    s2.transform ( {{{1,0,0,1},{0,1,0,1},{0,0,1,1},{0,0,0,1}}} );

    auto PVM = glGetUniformLocation ( v.graphics.shaderProg, "PVM" );
    glm::mat4 projection, view, model;
    view = glm::lookAt ( glm::vec3(5,5,-5), glm::vec3(0,0,0), glm::vec3(0,1,0) );
    projection = glm::perspective ( glm::radians ( 45.0f ), 1.0f, 0.001f, 15.0f );

    glUniformMatrix4fv ( PVM, 1, GL_FALSE, glm::value_ptr ( projection * view * model ) );

    while ( true ) 
    {
        v.draw ();
//        model = glm::rotate ( model, 0.01f, glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
//        glUniformMatrix4fv ( PVM, 1, GL_FALSE, glm::value_ptr ( projection * view * model ) );
//        s.transform ( {{{1,0,0,0.02},{0,1,0,0},{0,0,1,0},{0,0,0,1}}}, false );
//        s.transform ( {{{(GLfloat)cos(0.017),(GLfloat)-sin(.017),0,0},{sin(.017),cos(.017),0,0},{0,0,1,0},{0,0,0,1}}} );
        s.transform ( {{{1.0f, 0.0f, 0.0f},{0.0f,cos(0.017),sin(0.017),0.0f},{0.0f,-sin(0.017),cos(0.017),0.0f},{0.0f,0.0f,0.0f,1.0f}}} );
//        s.transform ( {{{cos(0.017),0,sin(.017),0},{0,1,0,0},{-sin(.017),0,cos(.017),0},{0,0,0,1}}} );
    }
}
