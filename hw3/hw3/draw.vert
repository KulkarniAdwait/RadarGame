#version 150

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;
uniform mat4 uWorldToDevice;

void main() 
{
    //Adwait Kulkarni
    gl_Position = uWorldToDevice * vPosition;
    //gl_Position = vPosition;
    color = vColor;
}
