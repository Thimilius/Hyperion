//------------------------ Header Guard ------------------------
#pragma once

#define CALLBACK    __stdcall
#define WIN_API extern "C" __declspec(dllimport)
#define WIN_API_CALLCONV __stdcall

struct HINSTANCE__;
struct HWND__;
struct HKEY__;
struct HDC__;
struct HICON__;
struct HMENU__;

typedef unsigned char BYTE;
typedef signed int BOOL;
typedef unsigned long DWORD;
typedef DWORD *LPDWORD;
typedef int INT;
typedef unsigned int UINT;
typedef long LONG;
typedef unsigned long ULONG;
typedef long *LPLONG;
typedef long long LONGLONG;
typedef LONGLONG *LPLONGLONG;
typedef void *LPVOID;
typedef const void *LPCVOID;
typedef const wchar_t *LPCTSTR;
typedef char CHAR;
typedef CHAR *LPSTR;

typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;
typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;
typedef long HRESULT;

typedef void *HANDLE;
typedef HINSTANCE__ *HINSTANCE;
typedef HINSTANCE HMODULE;
typedef HWND__ *HWND;
typedef HKEY__ *HKEY;
typedef HDC__ *HDC;
typedef HICON__ *HICON;
typedef HICON__ *HCURSOR;
typedef HMENU__ *HMENU;

struct Critical_Section {
  void *Data1[1];
  long Data2[2];
  void *Data3[3];
};

struct Overlapped {
  void *Data1[3];
  unsigned long Data2[2];
};

typedef Overlapped *LPOverlapped;

// RANT: Apparently Microsoft forgot to include this definition in their header files. What the hell!?!?
typedef struct _PROCESSOR_POWER_INFORMATION {
  ULONG Number;
  ULONG MaxMhz;
  ULONG CurrentMhz;
  ULONG MhzLimit;
  ULONG MaxIdleState;
  ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;
