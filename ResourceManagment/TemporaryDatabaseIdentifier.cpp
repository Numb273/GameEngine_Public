#include "TemporaryDatabaseIdentifier.h"

#include "DataBase.h"
#include "../Game/GameObject.h"
#include "../Game/PhysicsObject.h"

TemporaryDatabaseIdentifier::TemporaryDatabaseIdentifier(std::string resourceManager, std::string resourceName)
{
	this->resourceManager = resourceManager;
	this->resourceName = resourceName;

	unloadLocked = false;
}

void TemporaryDatabaseIdentifier::UnloadFromDatabase(DataBase* database)
{
	if (!unloadLocked)
	{
		if (resourceManager == "OBJMeshes") {
			OBJMeshUnload(database);
		}
		else if (resourceManager == "PhysicsObjects") {
			PhysicsEntityUnload(database);
		}
		else if (resourceManager == "GameObjects") {
			GameEntityUnload(database);
		}

		unloadLocked = true;
	}
}

void TemporaryDatabaseIdentifier::OBJMeshUnload(DataBase* database)
{
	database->GetTable("OBJMeshes")->GetResources()->Unload(resourceName);
}

void TemporaryDatabaseIdentifier::GameEntityUnload(DataBase* database)
{
	GameObject* entity = static_cast<GameObject*>(database->GetTable("GameObjects")->GetResources()->Find(resourceName));
	SceneNode* node = entity->GetSceneNode();

	Renderer* renderer = static_cast<Renderer*>(database->GetTable("GRenderer")->GetResources()->Find("Renderer"));
	renderer->RemoveSceneNode(node);

	database->GetTable("GameObjects")->GetResources()->Unload(resourceName);
}

void TemporaryDatabaseIdentifier::PhysicsEntityUnload(DataBase* database)
{
	Renderer* renderer = static_cast<Renderer*>(database->GetTable("GRenderer")->GetResources()->Find("Renderer"));
	PhysicsEngine* physicsEngine = static_cast<PhysicsEngine*>(database->GetTable("PhysicsEngine")->GetResources()->Find("PhysicsEngine"));

	PhysicsObject* entity = static_cast<PhysicsObject*>(database->GetTable("PhysicsObjects")->GetResources()->Find(resourceName));

	SceneNode* node = entity->GetSceneNode();
	RigidBody* rigidBody = entity->GetRigidBody();

	renderer->RemoveSceneNode(node);
	physicsEngine->RemoveRigidBody(rigidBody);

	database->GetTable("PhysicsObjects")->GetResources()->Unload(resourceName);
}
