#version 420 core

in vec4 colorToFS;

out vec4 fragmentColor;

void main()
{
	fragmentColor = colorToFS;
}
