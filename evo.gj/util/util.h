#pragma once

#include <atomic>
#include <mutex>
//#include <TlHelp32.h>
#include <d3d9.h>
#include <dwmapi.h>
#include <xmmintrin.h>
#include <array>
#include <vector>
#include <cstdlib>
#include <random>
#include <direct.h>
#include <fstream>
#include <string>
#include <sstream>

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

#include "../render/ImGui/imgui.h"
#include "../render/ImGui/imgui_impl_dx9.h"
#include "../render/ImGui/imgui_impl_win32.h"

#include "../cheat/driver/driver.h"

extern HWND GameHwnd;
extern RECT GameRect;
extern HWND MyHwnd;
extern MSG Message;

extern int Width;
extern int Height;

extern class Vector2;
extern class Vector3;

class Util{
public:
	static HWND get_process_wnd(uint32_t pid);
	static int get_fps();
	static UINT GetProcessId(const wchar_t* process_name);
	static UINT GetProcessThreadNumByID(DWORD dwPID);
	static void DrawCornerBox(int X, int Y, int W, int H, const ImColor color, int thickness);
	static void PrintPtr(std::string text, uintptr_t ptr);
	static void Print2D(std::string text, Vector2 pos);
	static void Print3D(std::string text, Vector3 pos);
	static double GetCrossDistance(double x1, double y1, double x2, double y2);
};

//oof my code was clean until I pasted this in below smh

#define M_PI 3.14159265358979323846264338327950288419716939937510

class Vector2
{
public:
	Vector2() : x(0.f), y(0.f)
	{

	}

	Vector2(double _x, double _y) : x(_x), y(_y)
	{

	}
	~Vector2()
	{

	}

	double x;
	double y;
};
class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	double x;
	double y;
	double z;

	inline double Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double Distance(Vector3 v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double Length() {
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(double flNum) { return Vector3(x * flNum, y * flNum, z * flNum); }
};
struct FQuat
{
	double x;
	double y;
	double z;
	double w;
};
struct FTransform
{
	FQuat rot;
	Vector3 translation;
	char pad[4];
	Vector3 scale;
	char pad1[4];
	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};
extern D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2);
extern D3DMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0));

struct Camera
{
	Vector3 Location;
	Vector3 Rotation;
	float FieldOfView;
};

template<class type>
class tarray
{
public:
	tarray() : data(nullptr), count(std::int32_t()), maxx(std::int32_t()) { }
	tarray(type* data, std::int32_t count, std::int32_t maxx) : data(data), count(count), maxx(maxx) { }

	const bool is_valid() const noexcept
	{
		return !(this->data == nullptr);
	}

	const std::int32_t size() const noexcept
	{
		return this->count;
	}

	type& operator[](std::int32_t index) noexcept
	{
		return this->data[index];
	}

	const type& operator[](std::int32_t index) const noexcept
	{
		return this->data[index];
	}

	bool is_valid_index(std::int32_t index) const noexcept
	{
		return index < this->size();
	}

protected:
	type* data;
	std::int32_t count;
	std::int32_t maxx;
};