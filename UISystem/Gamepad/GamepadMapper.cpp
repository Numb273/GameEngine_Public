#include "GamepadMapper.h"
#include "../GameLogicFSM/Messaging/MessageSystem.h"
#include "../ResourceManagement/Utilities/StringUtility.h"
#include <math.h>

enum BASIC_CONTROL
{
	MOVEMENT_X,
	MOVEMENT_Z,
	ROTATION_X,
	ROTATION_Y,
	FIRED,
	RELOAD
};

enum RHS_BUTTONS
{
	A,
	B,
	X,
	Y
};

GamepadMapper::GamepadMapper() : InputMapper()
{
	rawRotation = Vector3(-1, 0, 0);
	id = 0;
	gamepad = nullptr;
}

void GamepadMapper::FillInputs()
{
	gamepad->Update();

	/*
	No checks are needed for whether the controller is still
	connected. InputManager handles that with polling functions.
	*/

	// 0 - 1 = left stick
	if (!gamepad->LStickInDeadzone())
	{
		inputs[MOVEMENT_X] = gamepad->LeftStick_X();
		inputs[MOVEMENT_Z] = gamepad->LeftStick_Y();
	}

	// 2 - 3 = right stick
	if (!gamepad->RStickInDeadzone())
	{
		inputs[ROTATION_Y] = gamepad->RightStick_X();
		inputs[ROTATION_X] = gamepad->RightStick_Y();
	}

	AnnounceTeamChanges();

	// 4 -5 = triggers
	inputs[RELOAD] = gamepad->LeftTrigger();
	inputs[FIRED] = gamepad->RightTrigger();

	gamepad->RefreshState();
}

void GamepadMapper::ClearInputs()
{
	for (int i = 0; i < 6; i++)
	{
		inputs[i] = 0;
	}
}

//Y Movement is not needed for a top down shooter.
Vector3 GamepadMapper::GetMovement()
{
	Vector3 pos(inputs[MOVEMENT_X], 0, -inputs[MOVEMENT_Z]);

	return pos;
}

float GamepadMapper::GetRotation()
{
	float newRotation = 0;

	if (inputs[ROTATION_Y] != 0 || inputs[ROTATION_X] != 0)
	{
		float rotation = atan2(inputs[ROTATION_X], inputs[ROTATION_Y])
			* static_cast<float>(180.0f / M_PI) + 90.0f;

		//So we dont keep bouncing back and forth
		if (rotation < 0)
		{
			rotation = 360 - (-rotation);
		}

		//To stop the player from spinning like mad
		newRotation = rotation - prevRotation;
		prevRotation = rotation;

		//The x and y values before being converted to degrees.
		rawRotation = Vector3((inputs[ROTATION_Y]), 0, (inputs[ROTATION_X]));
	}

	return newRotation;
}

bool GamepadMapper::Fired()
{
	return inputs[FIRED] > 0;
}

bool GamepadMapper::Reload()
{
	return inputs[RELOAD] > 0;
}

void GamepadMapper::AnnounceTeamChanges() const
{
	const string playerid = "Player" + to_string(id);

	if (gamepad->GetButtonPressed(A))
	{
		MessageSystem::GetInstance()->TransmitMessage(StringUtility::Hash(playerid + "SwitchGreenTeam"));
	}

	if (gamepad->GetButtonPressed(X))
	{
		MessageSystem::GetInstance()->TransmitMessage(StringUtility::Hash(playerid + "SwitchBlueTeam"));
	}

	if (gamepad->GetButtonPressed(Y))
	{
		MessageSystem::GetInstance()->TransmitMessage(StringUtility::Hash(playerid + "SwitchYellowTeam"));
	}

	if (gamepad->GetButtonPressed(B))
	{
		MessageSystem::GetInstance()->TransmitMessage(StringUtility::Hash(playerid + "SwitchRedTeam"));
	}
}
