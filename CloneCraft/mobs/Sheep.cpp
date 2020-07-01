#include "Sheep.h"

Sheep::Sheep()
{
	this->hitbox = maths::Vec3<float>(1.f, 0.9f, 0.7f);
}

void Sheep::drawLeg(maths::Vec3<float> pos)
{
	float legWidth = 0.1f, legHeight = 0.3f;

	glColor3f(.45f * 1.5f, .25f * 1.5f, .15f * 1.5f);

	glVertex3f(pos.x + -legWidth, pos.y - legHeight, pos.z + -legWidth);
	glVertex3f(pos.x + -legWidth, pos.y - legHeight, pos.z + legWidth);
	glVertex3f(pos.x + legWidth, pos.y - legHeight, pos.z + legWidth);
	glVertex3f(pos.x + legWidth, pos.y - legHeight, pos.z + -legWidth);

	glColor3f(.9f, .9f, .9f);

	glVertex3f(pos.x + -legWidth, pos.y - legHeight, pos.z + -legWidth);
	glVertex3f(pos.x + -legWidth, pos.y - legHeight, pos.z + legWidth);
	glVertex3f(pos.x + -legWidth, pos.y, pos.z + legWidth);
	glVertex3f(pos.x + -legWidth, pos.y, pos.z + -legWidth);

	glColor3f(.8f, .8f, .8f);

	glVertex3f(pos.x + legWidth, pos.y - legHeight, pos.z + -legWidth);
	glVertex3f(pos.x + legWidth, pos.y - legHeight, pos.z + legWidth);
	glVertex3f(pos.x + legWidth, pos.y, pos.z + legWidth);
	glVertex3f(pos.x + legWidth, pos.y, pos.z + -legWidth);

	glColor3f(.95f, .95f, .95f);

	glVertex3f(pos.x + -legWidth, pos.y - legHeight, pos.z + -legWidth);
	glVertex3f(pos.x + legWidth, pos.y - legHeight, pos.z + -legWidth);
	glVertex3f(pos.x + legWidth, pos.y, pos.z + -legWidth);
	glVertex3f(pos.x + -legWidth, pos.y, pos.z + -legWidth);

	glColor3f(.75f, .75f, .75f);

	glVertex3f(pos.x + -legWidth, pos.y - legHeight, pos.z + legWidth);
	glVertex3f(pos.x + legWidth, pos.y - legHeight, pos.z + legWidth);
	glVertex3f(pos.x + legWidth, pos.y, pos.z + legWidth);
	glVertex3f(pos.x + -legWidth, pos.y, pos.z + legWidth);

	glColor3f(1.f, 1.f, 1.f);

	glVertex3f(pos.x + -legWidth, pos.y, pos.z + -legWidth);
	glVertex3f(pos.x + -legWidth, pos.y, pos.z + legWidth);
	glVertex3f(pos.x + legWidth, pos.y, pos.z + legWidth);
	glVertex3f(pos.x + legWidth, pos.y, pos.z + -legWidth);
}

void Sheep::renderModel()
{
	glDisable(GL_TEXTURE_2D);

	float low = 0.3f, high = 0.9f, width = .5f, length = .35f, headSize = .2f;

	glBegin(GL_QUADS);

	// \\
	The sheep's body

	glColor3f(.7f, .7f, .7f);

	glVertex3f(-width, low, -length);
	glVertex3f(-width, low, length);
	glVertex3f(width, low, length);
	glVertex3f(width, low, -length);
	
	glColor3f(.9f, .9f, .9f);

	glVertex3f(-width, low, -length);
	glVertex3f(-width, low, length);
	glVertex3f(-width, high, length);
	glVertex3f(-width, high, -length);

	glColor3f(.8f, .8f, .8f);

	glVertex3f(width, low, -length);
	glVertex3f(width, low, length);
	glVertex3f(width, high, length);
	glVertex3f(width, high, -length);

	glColor3f(.95f, .95f, .95f);

	glVertex3f(-width, low, -length);
	glVertex3f(width, low, -length);
	glVertex3f(width, high, -length);
	glVertex3f(-width, high, -length);

	glColor3f(.75f, .75f, .75f);

	glVertex3f(-width, low, length);
	glVertex3f(width, low, length);
	glVertex3f(width, high, length);
	glVertex3f(-width, high, length);

	glColor3f(1.f, 1.f, 1.f);

	glVertex3f(-width, high, -length);
	glVertex3f(-width, high, length);
	glVertex3f(width, high, length);
	glVertex3f(width, high, -length);

	// \\
	The sheep's head

	glColor3f(.7f, .7f, .7f);
	
	glVertex3f(-width + -headSize, high - headSize, -headSize);
	glVertex3f(-width + -headSize, high - headSize, headSize);
	glVertex3f(-width + headSize, high - headSize, headSize);
	glVertex3f(-width + headSize, high - headSize, -headSize);

	glColor3f(.9f, .5f, .3f);

	glVertex3f(-width + -headSize, high - headSize, -headSize);
	glVertex3f(-width + -headSize, high - headSize, headSize);
	glVertex3f(-width + -headSize, high + headSize, headSize);
	glVertex3f(-width + -headSize, high + headSize, -headSize);

	glColor3f(.8f, .8f, .8f);

	glVertex3f(-width + headSize, high - headSize, -headSize);
	glVertex3f(-width + headSize, high - headSize, headSize);
	glVertex3f(-width + headSize, high + headSize, headSize);
	glVertex3f(-width + headSize, high + headSize, -headSize);

	glColor3f(.95f, .95f, .95f);
	
	glVertex3f(-width + -headSize, high - headSize, -headSize);
	glVertex3f(-width + headSize, high - headSize, -headSize);
	glVertex3f(-width + headSize, high + headSize, -headSize);
	glVertex3f(-width + -headSize, high + headSize, -headSize);

	glColor3f(.75f, .75f, .75f);

	glVertex3f(-width + -headSize, high - headSize, headSize);
	glVertex3f(-width + headSize, high - headSize, headSize);
	glVertex3f(-width + headSize, high + headSize, headSize);
	glVertex3f(-width + -headSize, high + headSize, headSize);

	glColor3f(1.f, 1.f, 1.f);

	glVertex3f(-width + -headSize, high + headSize, -headSize);
	glVertex3f(-width + -headSize, high + headSize, headSize);
	glVertex3f(-width + headSize, high + headSize, headSize);
	glVertex3f(-width + headSize, high + headSize, -headSize);
	
	this->drawLeg(maths::Vec3<float>((width - 0.15f),low, (length - 0.15f)));
	this->drawLeg(maths::Vec3<float>((width - 0.15f),low, -(length - 0.15f)));
	this->drawLeg(maths::Vec3<float>(-(width - 0.15f),low, -(length - 0.15f)));
	this->drawLeg(maths::Vec3<float>(-(width - 0.15f),low, (length - 0.15f)));

	glEnd();
}