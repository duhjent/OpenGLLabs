#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in float pointSize;
out vec4 vertexColor;
void main()
{
    gl_Position = position;
    vertexColor = color;
    gl_PointSize = pointSize;
};

#shader fragment
#version 330 core

out vec4 FragColor;
in vec4 vertexColor;
void main()
{
    FragColor = vertexColor;
};
