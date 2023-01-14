#include "sdk.h"

Vector3 SDK::GetBoneWithRotation(uintptr_t mesh, int id)
{
	uintptr_t bonearray = 0;
	bonearray = read<uintptr_t>(mesh + 0x5c0);
	if (!bonearray) bonearray = read<uintptr_t>(mesh + 0x5c0 + 0x10);
	
	FTransform ComponentToWorld = read<FTransform>(mesh + 0x240);

	FTransform bone = read<FTransform>(bonearray + (id * 0x60));

	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

Camera SDK::GetViewAngles()
{
	Camera LocalCamera;
	
	uint64_t CHAIN6969696969696969 = read<uint64_t>(LocalPtrs::Gworld + 0x110);
	LocalCamera.Location = read<Vector3>(CHAIN6969696969696969);

	uint64_t ViewMatrix = read<uint64_t>(LocalPtrs::LocalPlayers + 0xd0);
	uint64_t ViewMatrixCorrect = read<uint64_t>(ViewMatrix + 0x8);
	if (Debug::PrintPointers) Util::PrintPtr("ViewMatrixCorrect: ", ViewMatrixCorrect);
	LocalCamera.FieldOfView = 80.f / (read<double>(ViewMatrixCorrect + 0x7F0) / 1.19f);

	LocalCamera.Rotation.x = read<double>(ViewMatrixCorrect + 0x9C0);
	LocalCamera.Rotation.y = read<double>(LocalPtrs::RootComponent + 0x148);
	LocalCamera.Rotation.x = (asin(LocalCamera.Rotation.x)) * (180.0 / M_PI);

	if (LocalCamera.Rotation.y < 0)
		LocalCamera.Rotation.y = 360 + LocalCamera.Rotation.y;

	if (Debug::PrintLocations) {
		Util::Print3D("Location: ", LocalCamera.Location);
		Util::Print3D("Rotation: ", LocalCamera.Rotation);
		Util::Print2D("Fov: ", Vector2(LocalCamera.FieldOfView, 69));
	}
	


	return LocalCamera;
}

Vector2 SDK::ProjectWorldToScreen(Vector3 WorldLocation)
{

	vCamera = SDK::GetViewAngles(); //get ur players newest view angles

	D3DMATRIX tempMatrix = Matrix(vCamera.Rotation);

	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - vCamera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return Vector2((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z);
}
