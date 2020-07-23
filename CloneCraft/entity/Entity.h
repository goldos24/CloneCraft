#pragma once

#include <cmath>
#include <functional>
#include <vector>
#include "../world/forwardDeclarations.h"
#include "../maths/maths.h"
#include "../game/forwardDeclarations.h"
#include "../stringEncoder/stringEncoding.h"

struct Entity
{
	Entity();

	enum class ID : uint64_t
	{
		player = CptrToInt("player\0\0"),
		sheep = CptrToInt("sheep\0\0\0"),
		invalid = CptrToInt("invalid")
	};

	maths::Vec3<float> position, rotation, movement;
	maths::Vec3<float> hitbox;
	ID id;

	static std::shared_ptr<Entity> createEntityFromID(ID entityID, ...);
	static void parseEntities(std::string entityData, std::vector<std::shared_ptr<Entity>>& entityVector);
	static ID idFromString(std::string);

	bool isStandingOnASurface(world::World& world);

	void applyMovement(float elapsedTime);

	void applyAcceleration(float elapsedTime, maths::Vec3<float> acceleration);

	void applyFriction(float elapsedTime, float friction);

	bool isColliding(world::World& world);

	void clipMovement(float elapsedTime, world::World& world);

	virtual void renderModel();

	void render(maths::Vec3<float> cameraPosition, maths::Vec3<float> cameraRotation);

	virtual void update(Game&, float);

	void parseProperty(std::string propertyName, std::istream& entityParserStream);

	std::string encode();

	virtual void parseSpecialProperty(std::string propertyName, std::istream& entityParserStream) = 0;
};