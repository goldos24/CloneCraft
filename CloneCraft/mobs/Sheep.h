#pragma once

#include "../entity/Entity.h"

struct Sheep : Entity
{
	Sheep();

	enum class LegMovementMode : char
	{
		Forward,
		Backward,
		None
	};

	void renderModel();

	void drawLeg(maths::Vec3<float> position);

	maths::Vec3<float> legRotation;

	void update(Game&, float);

	LegMovementMode legMoveMode = LegMovementMode::Forward;

	void parseSpecialProperty(std::string propertyName, std::istream& entityParserStream);
};