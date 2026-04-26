unit syscalls;

{$mode objfpc}{$H+}

interface

uses
  Windows, SysUtils;

type
  PSIZE_T = ^SIZE_T;
  NTSTATUS = LONG;
  PULONG = ^ULONG;

function scOpenProcess(dwDesiredAccess: DWORD; bInheritHandle: BOOL; dwProcessId: DWORD): THandle;
function scReadProcessMemory(hProcess: THandle; lpBaseAddress: Pointer; lpBuffer: Pointer; nSize: SIZE_T; lpNumberOfBytesRead: PSIZE_T): BOOL;
function scWriteProcessMemory(hProcess: THandle; lpBaseAddress: Pointer; lpBuffer: Pointer; nSize: SIZE_T; lpNumberOfBytesWritten: PSIZE_T): BOOL;
function scVirtualProtectEx(hProcess: THandle; lpAddress: Pointer; dwSize: SIZE_T; flNewProtect: DWORD; var lpflOldProtect: DWORD): BOOL;
function scNtProtectVirtualMemory(ProcessHandle: THandle; BaseAddress: Pointer; NumberOfBytesToProtect: SIZE_T; NewAccessProtection: DWORD; var OldAccessProtection: DWORD): BOOL;

implementation

type
  PUNICODE_STRING = ^UNICODE_STRING;
  UNICODE_STRING = record
    Length: WORD;
    MaximumLength: WORD;
    Buffer: PWideChar;
  end;

  POBJECT_ATTRIBUTES = ^OBJECT_ATTRIBUTES;
  OBJECT_ATTRIBUTES = record
    Length: ULONG;
    RootDirectory: THandle;
    ObjectName: PUNICODE_STRING;
    Attributes: ULONG;
    SecurityDescriptor: Pointer;
    SecurityQualityOfService: Pointer;
  end;

  PCLIENT_ID = ^CLIENT_ID;
  CLIENT_ID = record
    UniqueProcess: THandle;
    UniqueThread: THandle;
  end;

  TNtOpenProcess = function(var ProcessHandle: THandle; DesiredAccess: ACCESS_MASK; var ObjectAttributes: OBJECT_ATTRIBUTES; var ClientId: CLIENT_ID): NTSTATUS; stdcall;
  TNtReadVirtualMemory = function(ProcessHandle: THandle; BaseAddress: Pointer; Buffer: Pointer; BufferSize: SIZE_T; NumberOfBytesRead: PSIZE_T): NTSTATUS; stdcall;
  TNtWriteVirtualMemory = function(ProcessHandle: THandle; BaseAddress: Pointer; Buffer: Pointer; BufferSize: SIZE_T; NumberOfBytesWritten: PSIZE_T): NTSTATUS; stdcall;
  TNtProtectVirtualMemory = function(ProcessHandle: THandle; var BaseAddress: Pointer; var NumberOfBytesToProtect: SIZE_T; NewAccessProtection: ULONG; OldAccessProtection: Pointer): NTSTATUS; stdcall;

var
  hNtdll: HMODULE = 0;
  fnNtOpenProcess: TNtOpenProcess = nil;
  fnNtReadVirtualMemory: TNtReadVirtualMemory = nil;
  fnNtWriteVirtualMemory: TNtWriteVirtualMemory = nil;
  fnNtProtectVirtualMemory: TNtProtectVirtualMemory = nil;

procedure InitObjectAttributes(var p: OBJECT_ATTRIBUTES; n: PUNICODE_STRING; a: ULONG; r: THandle; s: Pointer);
begin
  p.Length := SizeOf(OBJECT_ATTRIBUTES);
  p.RootDirectory := r;
  p.ObjectName := n;
  p.Attributes := a;
  p.SecurityDescriptor := s;
  p.SecurityQualityOfService := nil;
end;

function InitNtdll: Boolean;
begin
  if hNtdll = 0 then
  begin
    hNtdll := GetModuleHandle('ntdll.dll');
    if hNtdll = 0 then Exit(False);
    fnNtOpenProcess := TNtOpenProcess(GetProcAddress(hNtdll, 'NtOpenProcess'));
    fnNtReadVirtualMemory := TNtReadVirtualMemory(GetProcAddress(hNtdll, 'NtReadVirtualMemory'));
    fnNtWriteVirtualMemory := TNtWriteVirtualMemory(GetProcAddress(hNtdll, 'NtWriteVirtualMemory'));
    fnNtProtectVirtualMemory := TNtProtectVirtualMemory(GetProcAddress(hNtdll, 'NtProtectVirtualMemory'));
  end;
  Result := True;
end;

function scOpenProcess(dwDesiredAccess: DWORD; bInheritHandle: BOOL; dwProcessId: DWORD): THandle;
var
  h: THandle;
  oa: OBJECT_ATTRIBUTES;
  cid: CLIENT_ID;
  status: NTSTATUS;
begin
  if not InitNtdll or not Assigned(fnNtOpenProcess) then
    Exit(OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId));
  InitObjectAttributes(oa, nil, 0, 0, nil);
  cid.UniqueProcess := dwProcessId;
  cid.UniqueThread := 0;
  h := 0;
  status := fnNtOpenProcess(h, dwDesiredAccess, oa, cid);
  if status = 0 then Result := h else Result := 0;
end;

function scReadProcessMemory(hProcess: THandle; lpBaseAddress: Pointer; lpBuffer: Pointer; nSize: SIZE_T; lpNumberOfBytesRead: PSIZE_T): BOOL;
var
  status: NTSTATUS;
  read: SIZE_T;
begin
  if not InitNtdll or not Assigned(fnNtReadVirtualMemory) then
    Exit(ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead));
  read := 0;
  status := fnNtReadVirtualMemory(hProcess, lpBaseAddress, lpBuffer, nSize, @read);
  if lpNumberOfBytesRead <> nil then lpNumberOfBytesRead^ := read;
  Result := status = 0;
end;

function scWriteProcessMemory(hProcess: THandle; lpBaseAddress: Pointer; lpBuffer: Pointer; nSize: SIZE_T; lpNumberOfBytesWritten: PSIZE_T): BOOL;
var
  status: NTSTATUS;
  written: SIZE_T;
begin
  if not InitNtdll or not Assigned(fnNtWriteVirtualMemory) then
    Exit(WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten));
  written := 0;
  status := fnNtWriteVirtualMemory(hProcess, lpBaseAddress, lpBuffer, nSize, @written);
  if lpNumberOfBytesWritten <> nil then lpNumberOfBytesWritten^ := written;
  Result := status = 0;
end;

function scVirtualProtectEx(hProcess: THandle; lpAddress: Pointer; dwSize: SIZE_T; flNewProtect: DWORD; var lpflOldProtect: DWORD): BOOL;
var
  base: Pointer;
  size: SIZE_T;
  old: DWORD;
  status: NTSTATUS;
begin
  if not InitNtdll or not Assigned(fnNtProtectVirtualMemory) then
  begin
    old := 0;
    Result := VirtualProtectEx(hProcess, lpAddress, dwSize, flNewProtect, @old);
    lpflOldProtect := old;
    Exit;
  end;
  base := lpAddress;
  size := dwSize;
  old := 0;
  status := fnNtProtectVirtualMemory(hProcess, base, size, flNewProtect, @old);
  lpflOldProtect := old;
  Result := status = 0;
end;

function scNtProtectVirtualMemory(ProcessHandle: THandle; BaseAddress: Pointer; NumberOfBytesToProtect: SIZE_T; NewAccessProtection: DWORD; var OldAccessProtection: DWORD): BOOL;
begin
  Result := scVirtualProtectEx(ProcessHandle, BaseAddress, NumberOfBytesToProtect, NewAccessProtection, OldAccessProtection);
end;

end.
