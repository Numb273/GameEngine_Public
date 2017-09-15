#include "TopDownController.h"

#include "CharacterModel.h"
#include "../Physics/RigidBody.h"
#include "../GameLogicFSM/MessageSystem.h"
#include "AudioManager.h"

const Vector3 Y_ROTATION_AXIS = Vector3(0, 1, 0);

TopDownController::TopDownController(InputMapper* input, CharacterModel* playerModel, RigidBody* rigidBody)
	: PlayerController(input)
{
	this->playerModel = playerModel;
	this->rigidBody = rigidBody;

	speedMultiplier = 30;
}

void TopDownController::ApplyRotation()
{
	float inputtedRotationInDegrees = input->GetRotation();

	Matrix4 rotation = Matrix4::Rotation(inputtedRotationInDegrees, Y_ROTATION_AXIS);
	playerModel->SetTransform(playerModel->GetTransform() * rotation);

	if (inputtedRotationInDegrees != 0)
	{
		currentRotation = rotation;
		MessageSystem::GetInstance()->Transmit(Log::Hash("player_rotated"), false);
	}
}

void TopDownController::UpdatePosition()
{
	Vector3 movement = input->GetMovement() * speedMultiplier;

	rigidBody->ApplyForce(movement);

	if (MovementInputted(movement)) 
	{
		AudioManager::GetInstance()->BeginPlay(movementSound);

		MessageSystem::GetInstance()->Transmit(Log::Hash("player_moved"), false);
		MessageSystem::GetInstance()->Transmit(Log::Hash(rigidBody->tag + "moving"), true);
	}
	else
	{
		AudioManager::GetInstance()->StopPlay(movementSound);
	}
}

bool TopDownController::MovementInputted(const Vector3& movement)
{
	return (movement.x != 0 || movement.y != 0 || movement.z != 0);
}