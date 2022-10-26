#include "aimbot.h"

void aimbot() {
	if (!TargetPawn) return;

	auto mesh = read<uintptr_t>(TargetPawn + 0x310);
	if (!mesh) {
		ClosestDistance = FLT_MAX;
		TargetPawn = NULL;
	}
	Vector3 Head3D = SDK::GetBoneWithRotation(mesh, 68);
	Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);

	auto dx = Head2D.x - (Width / 2);
	auto dy = Head2D.y - (Height / 2);
	auto dz = 0;
	auto dist = sqrtf(dx * dx + dy * dy) / 100.0f;
	
	if (dist < FovSize && dist <= ClosestDistance) {
		
		if (Head2D.x != 0 || Head2D.y != 0) {
			
			if ((Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2) <= FovSize)) {
				float x = Head2D.x; float y = Head2D.y;
				float ScreenCenterX = (Width / 2);
				float ScreenCenterY = (Height / 2);

				float AimSpeed = Smooth;

				float TargetX = 0;
				float TargetY = 0;

				if (x != 0)
				{
					if (x > ScreenCenterX)
					{
						TargetX = -(ScreenCenterX - x);
						TargetX /= AimSpeed;
						if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
					}

					if (x < ScreenCenterX)
					{
						TargetX = x - ScreenCenterX;
						TargetX /= AimSpeed;
						if (TargetX + ScreenCenterX < 0) TargetX = 0;
					}
				}
				if (y != 0)
				{
					if (y > ScreenCenterY)
					{
						TargetY = -(ScreenCenterY - y);
						TargetY /= AimSpeed;
						if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
					}

					if (y < ScreenCenterY)
					{
						TargetY = y - ScreenCenterY;
						TargetY /= AimSpeed;
						if (TargetY + ScreenCenterY < 0) TargetY = 0;
					}
				}

				mouse_event(MOUSEEVENTF_MOVE, TargetX, TargetY, NULL, NULL);

			}
			else {
				bIsTargeting = false;
			}
		}
		else {
			bIsTargeting = false;
		}
	}
	else {
		ClosestDistance = FLT_MAX;
		TargetPawn = NULL;
		bIsTargeting = false;
	}

	
}