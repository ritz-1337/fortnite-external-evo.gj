#include "aimbot.h"

void aimbot() {
	if (!TargetPawn) return;

	auto mesh = read<uintptr_t>(TargetPawn + 0x310); //https://fn.dumps.host/?class=ACharacter&member=Mesh
	if (!mesh) {
		ClosestDistance = FLT_MAX;
		TargetPawn = NULL;
		bIsTargeting = FALSE;
	}
	Vector3 Head3D = SDK::GetBoneWithRotation(mesh, 68);
	Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);
	
	auto distance = Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2);
	if (distance > FovSize or Head2D.x == 0 or Head2D.y == 0) {
		ClosestDistance = FLT_MAX;
		TargetPawn = NULL;
		bIsTargeting = FALSE;
	}

	float x = Head2D.x; float y = Head2D.y;
	float AimSpeed = Smooth;

	Vector2 ScreenCenter = { (double)Width / 2 , (double)Height / 2 };
	Vector2 Target;

	if (x != 0)
	{
		if (x > ScreenCenter.x)
		{
			Target.x = -(ScreenCenter.x - x);
			Target.x /= AimSpeed;
			if (Target.x + ScreenCenter.x > ScreenCenter.x * 2) Target.x = 0;
		}

		if (x < ScreenCenter.x)
		{
			Target.x = x - ScreenCenter.x;
			Target.x /= AimSpeed;
			if (Target.x + ScreenCenter.x < 0) Target.x = 0;
		}
	}
	if (y != 0)
	{
		if (y > ScreenCenter.y)
		{
			Target.y = -(ScreenCenter.y - y);
			Target.y /= AimSpeed;
			if (Target.y + ScreenCenter.y > ScreenCenter.y * 2) Target.y = 0;
		}

		if (y < ScreenCenter.y)
		{
			Target.y = y - ScreenCenter.y;
			Target.y /= AimSpeed;
			if (Target.y + ScreenCenter.y < 0) Target.y = 0;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, Target.x, Target.y, NULL, NULL);

	
}
