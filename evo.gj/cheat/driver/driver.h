#pragma once
#include "../../util/util.h"
#include "driverdefs.h"


#define code_virtual CTL_CODE(FILE_DEVICE_UNKNOWN, 0x269, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_physical CTL_CODE(FILE_DEVICE_UNKNOWN, 0x472, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_image CTL_CODE(FILE_DEVICE_UNKNOWN, 0xfee, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_security 0xcabd5e5
#define physical_read 0x27b
#define physical_write 0xdfd
#define virtual_read 0xfc6
#define virtual_write 0x359

typedef struct t_virtual {
	INT32 security_code;
	INT32 process_id;
	INT32 virtual_mode;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} e_virtual, * s_virtual;

typedef struct t_physical {
	INT32 security_code;
	INT32 process_id;
	INT32 physical_mode;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} e_physical, * s_physical;

typedef struct t_image {
	INT32 security_code;
	INT32 process_id;
	ULONGLONG* address;
} e_image, * s_image;
extern uintptr_t ProcId;
extern uintptr_t BaseId;
extern HANDLE driver_handle;
class driver {
public:
	static bool find_driver();

	static void read_virtual_memory(PVOID address, PVOID buffer, DWORD size);
	static void write_virtual_memory(PVOID address, PVOID buffer, DWORD size);
	static void read_physical_memory(PVOID address, PVOID buffer, DWORD size);
	static void write_physical_memory(PVOID address, PVOID buffer, DWORD size);
	static uintptr_t find_image();
	static INT32 find_process(LPCTSTR process_name);
};


template <typename T>
T read(uint64_t address) {
	T buffer{ };
	driver::read_physical_memory((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

template<typename T>
void write(uint64_t address, T buffer) {
	driver::read_physical_memory((PVOID)address, &buffer, sizeof(T));
}