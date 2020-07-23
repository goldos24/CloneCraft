#include "Sheep.h"
#include "../maths/matrix.h"
#include "../game/game.h"

Sheep::Sheep()
{
	this->hitbox = maths::Vec3<float>(1.f, 0.9f, 0.7f);
	this->position = maths::Vec3<float>(0.f, 16.f, 0.f);
	this->rotation.y = 45;
	this->id = Entity::ID::sheep;
}

void Sheep::drawLeg(maths::Vec3<float> pos)
{
	float legWidth = 0.1f, legHeight = 0.3f;

	maths::Mat4<float> legTransformationMatrix;
	legTransformationMatrix.loadIdentity();
	legTransformationMatrix.translate(pos);
	legTransformationMatrix.rotate(this->legRotation);

	maths::Vec3<float> NNN = legTransformationMatrix * maths::Vec3<float>(-legWidth, -legHeight, -legWidth); // Oh no...
	maths::Vec3<float> NNP = legTransformationMatrix * maths::Vec3<float>(-legWidth, -legHeight,  legWidth);
	maths::Vec3<float> NPN = legTransformationMatrix * maths::Vec3<float>(-legWidth,  0, -legWidth);
	maths::Vec3<float> NPP = legTransformationMatrix * maths::Vec3<float>(-legWidth,  0,  legWidth);
	maths::Vec3<float> PNN = legTransformationMatrix * maths::Vec3<float>( legWidth, -legHeight, -legWidth);
	maths::Vec3<float> PNP = legTransformationMatrix * maths::Vec3<float>( legWidth, -legHeight,  legWidth);
	maths::Vec3<float> PPN = legTransformationMatrix * maths::Vec3<float>( legWidth,  0, -legWidth);
	maths::Vec3<float> PPP = legTransformationMatrix * maths::Vec3<float>( legWidth,  0,  legWidth);

	// I know, macros are bad, but I'll #undef it later
#define glVertexVec3f(vector) glVertex3f((vector).x, (vector).y, (vector).z) 

	glColor3f(.45f * 1.5f, .25f * 1.5f, .15f * 1.5f);

	glVertexVec3f(NNN); 
	glVertexVec3f(NNP); 
	glVertexVec3f(PNP); 
	glVertexVec3f(PNN); 

	glColor3f(.9f, .9f, .9f);

	glVertexVec3f(NNN); 
	glVertexVec3f(NNP); 
	glVertexVec3f(NPP); 
	glVertexVec3f(NPN); 

	glColor3f(.8f, .8f, .8f);

	glVertexVec3f(PNN); 
	glVertexVec3f(PNP); 
	glVertexVec3f(PPP); 
	glVertexVec3f(PPN); 

	glColor3f(.95f, .95f, .95f);

	glVertexVec3f(NNN); 
	glVertexVec3f(PNN); 
	glVertexVec3f(PPN); 
	glVertexVec3f(NPN); 

	glColor3f(.75f, .75f, .75f);

	glVertexVec3f(NNP); 
	glVertexVec3f(PNP); 
	glVertexVec3f(PPP); 
	glVertexVec3f(NPP); 

	glColor3f(1.f, 1.f, 1.f);

	glVertexVec3f(NPN); 
	glVertexVec3f(NPP); 
	glVertexVec3f(PPP); 
	glVertexVec3f(PPN); 
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

	maths::Mat4<float> headTransformationMatrix;
	headTransformationMatrix.loadIdentity();
	headTransformationMatrix.translate(maths::Vec3<float>(-width, high, 0));
	//headTransformationMatrix.rotate(0, 0, 45);

	maths::Vec3<float> NNN = headTransformationMatrix * maths::Vec3<float>(-headSize, -headSize, -headSize); // Oh no...
	maths::Vec3<float> NNP = headTransformationMatrix * maths::Vec3<float>(-headSize, -headSize, headSize);
	maths::Vec3<float> NPN = headTransformationMatrix * maths::Vec3<float>(-headSize, headSize, -headSize);
	maths::Vec3<float> NPP = headTransformationMatrix * maths::Vec3<float>(-headSize, headSize, headSize);
	maths::Vec3<float> PNN = headTransformationMatrix * maths::Vec3<float>(headSize, -headSize, -headSize);
	maths::Vec3<float> PNP = headTransformationMatrix * maths::Vec3<float>(headSize, -headSize, headSize);
	maths::Vec3<float> PPN = headTransformationMatrix * maths::Vec3<float>(headSize, headSize, -headSize);
	maths::Vec3<float> PPP = headTransformationMatrix * maths::Vec3<float>(headSize, headSize, headSize);


	glColor3f(.7f, .7f, .7f);
	
	glVertexVec3f(NNN);
	glVertexVec3f(NNP);
	glVertexVec3f(PNP);
	glVertexVec3f(PNN);

	glColor3f(.9f, .5f, .3f);

	glVertexVec3f(NNN);
	glVertexVec3f(NNP);
	glVertexVec3f(NPP);
	glVertexVec3f(NPN);

	glColor3f(.8f, .8f, .8f);

	glVertexVec3f(PNN);
	glVertexVec3f(PNP);
	glVertexVec3f(PPP);
	glVertexVec3f(PPN);

	glColor3f(.95f, .95f, .95f);

	glVertexVec3f(NNN);
	glVertexVec3f(PNN);
	glVertexVec3f(PPN);
	glVertexVec3f(NPN);

	glColor3f(.75f, .75f, .75f);

	glVertexVec3f(NNP);
	glVertexVec3f(PNP);
	glVertexVec3f(PPP);
	glVertexVec3f(NPP);

	glColor3f(1.f, 1.f, 1.f);

	glVertexVec3f(NPN);
	glVertexVec3f(NPP);
	glVertexVec3f(PPP);
	glVertexVec3f(PPN);
	
#undef glVertexVec3f

	this->drawLeg(maths::Vec3<float>((width - 0.15f),low, (length - 0.15f)));
	this->drawLeg(maths::Vec3<float>((width - 0.15f),low, -(length - 0.15f)));
	this->drawLeg(maths::Vec3<float>(-(width - 0.15f),low, -(length - 0.15f)));
	this->drawLeg(maths::Vec3<float>(-(width - 0.15f),low, (length - 0.15f)));

	glEnd();
}

void Sheep::update(Game& game, float elapsedTime)
{
	switch (this->legMoveMode)
	{
	case Sheep::LegMovementMode::Forward:
		this->legRotation.z += elapsedTime * 180;
		break;
	case Sheep::LegMovementMode::Backward:
		this->legRotation.z -= elapsedTime * 180;
		break;
	default:
		this->legRotation.z += elapsedTime * ((this->legRotation.z < 0) ? 180 : -180);
		break;
	}


	maths::capDegrees(legRotation.z);

	if (!maths::isAngleInRange(legRotation.z, 315, 45))
	{
		if (this->legRotation.z < 180)
			this->legMoveMode = Sheep::LegMovementMode::Backward;
		else
			this->legMoveMode = Sheep::LegMovementMode::Forward;
	}

	this->movement.y -= elapsedTime * 18.f;
	this->applyAcceleration(elapsedTime, maths::Vec3<float>(maths::sind(-this->rotation.y) * this->movementSpeed, 0.f, maths::cosd(-this->rotation.y) * this->movementSpeed));
	this->clipMovement(elapsedTime, game.gameWorld);
	this->applyMovement(elapsedTime);
	this->applyFriction(elapsedTime, 1.2f);
}

void Sheep::parseSpecialProperty(std::string propertyName, std::istream& entityParserStream) {}