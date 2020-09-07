#version 330 core
    
layout(location = 0)in vec3 VertexPosition;    
//in vec4 VertexColor;  
uniform mat4 MVP;   
//out vec4 Color;    
    
void main()    
{    
  // Color = VertexColor;    
  gl_Position = MVP * vec4(VertexPosition, 1.0); 
}   