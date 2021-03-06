string vert = "#version 330 core\n\
\
layout (location = 0) in vec3 position;\
layout (location = 1) in vec3 normal;\
\
uniform mat4 model;\
uniform mat4 view;\
uniform mat4 projection;\
\
out vec2 texPos;\
out vec3 normal_frag;\
\
void main(){\
	mat4 MVP = projection*view*model;\
	normal_frag = -normalize(mat3(MVP)*normal);\
	\
	texPos = vec2(position.x+0.5f,-position.y+0.5f);\
	gl_Position = MVP * vec4(position, 1.0f);\
}";