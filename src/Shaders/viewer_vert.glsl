#version 450

layout (location = 0) in vec3 position;
uniform mat4 PVM; 

void main () {
    gl_Position = PVM * vec4 ( position.xyz, 1.0 );
}
