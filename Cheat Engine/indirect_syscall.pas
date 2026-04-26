unit indirect_syscall;

{$mode objfpc}{$H+}

interface

uses
  Windows;

function iscOpenProcess(dwDesiredAccess: DWORD; bInheritHandle: BOOL; dwProcessId: DWORD): THandle;
function iscReadProcessMemory(hProcess: THandle; lpBaseAddress: Pointer; lpBuffer: Pointer; nSize: SIZE_T; lpNumberOfBytesRead: PSIZE_T): BOOL;
function iscWriteProcessMemory(hProcess: THandle; lpBaseAddress: Pointer; lpBuffer: Pointer; nSize: SIZE_T; lpNumberOfBytesWritten: PSIZE_T): BOOL;

implementation

type
  TStub = function(a1: DWORD; a2: DWORD; a3: DWORD; a4: DWORD): DWORD; stdcall;

var
  stubOpen: TStub = nil;
  stubRead: TStub = nil;
  stubWrite: TStub = nil;

function FindSyscallInNtdll(apiName: PChar): Pointer;
var
  h: HMODULE;
  p: PByte;
  i: Integer;
begin
  Result := nil;
  h := GetModuleHandle('ntdll.dll');
  if h = 0 then Exit;
  p := GetProcAddress(h, apiName);
  if p = nil then Exit;
  for i := 0 to 31 do
  begin
    if (p[i] = $0F) and (p[i+1] = $05) then
    begin
      Result := @p[i];
      Exit;
    end;
  end;
end;

function GetNtdllSyscallNumber(const apiName: string): DWORD;
var
  h: HMODULE;
  p: PByte;
begin
  Result := 0;
  h := GetModuleHandle('ntdll.dll');
  if h = 0 then Exit;
  p := GetProcAddress(h, PChar(apiName));
  if p = nil then Exit;
  if (p[0] = $4C) and (p[1] = $8B) and (p[2] = $D1) and (p[3] = $B8) then
    Result := PDWORD(p + 4)^;
end;

function BuildIndirectStub(syscallAddr: Pointer; syscallNum: DWORD): TStub;
var
  code: PByte;
  oldProtect: DWORD;
  dwSize: SIZE_T;
begin
  Result := nil;
  if syscallAddr = nil then Exit;
  
  // Allocate RW first (stealth: avoid RWX directly)
  code := VirtualAlloc(nil, 32, MEM_COMMIT or MEM_RESERVE, PAGE_READWRITE);
  if code = nil then Exit;
  
  // Build stub:
  // mov r10, rcx      (49 89 CA)
  // mov eax, NUM      (B8 XX XX XX XX)
  // jmp [syscallAddr] (FF 25 00 00 00 00 + 8-byte absolute addr)
  code[0] := $49; code[1] := $89; code[2] := $CA;
  code[3] := $B8;
  PDWORD(@code[4])^ := syscallNum;
  code[8] := $FF; code[9] := $25; code[10] := $00; code[11] := $00; code[12] := $00; code[13] := $00;
  PPointer(@code[14])^ := syscallAddr;
  
  // Change to RX (execute-only, no write)
  dwSize := 32;
  if not VirtualProtect(code, dwSize, PAGE_EXECUTE_READ, oldProtect) then
  begin
    VirtualFree(code, 0, MEM_RELEASE);
    Exit;
  end;
  
  Result := TStub(code);
end;

function iscOpenProcess(dwDesiredAccess: DWORD; bInheritHandle: BOOL; dwProcessId: DWORD): THandle;
begin
  if stubOpen = nil then
    stubOpen := BuildIndirectStub(FindSyscallInNtdll('NtOpenProcess'), GetNtdllSyscallNumber('NtOpenProcess'));
  if stubOpen = nil then
    Exit(OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId));
  Result := THandle(stubOpen(dwDesiredAccess, DWORD(bInheritHandle), dwProcessId, 0));
end;

function iscReadProcessMemory(hProcess: THandle; lpBaseAddress: Pointer; lpBuffer: Pointer; nSize: SIZE_T; lpNumberOfBytesRead: PSIZE_T): BOOL;
begin
  if stubRead = nil then
    stubRead := BuildIndirectStub(FindSyscallInNtdll('NtReadVirtualMemory'), GetNtdllSyscallNumber('NtReadVirtualMemory'));
  if stubRead = nil then
    Exit(ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead));
  Result := stubRead(DWORD(hProcess), DWORD(lpBaseAddress), DWORD(lpBuffer), DWORD(nSize)) = 0;
end;

function iscWriteProcessMemory(hProcess: THandle; lpBaseAddress: Pointer; lpBuffer: Pointer; nSize: SIZE_T; lpNumberOfBytesWritten: PSIZE_T): BOOL;
begin
  if stubWrite = nil then
    stubWrite := BuildIndirectStub(FindSyscallInNtdll('NtWriteVirtualMemory'), GetNtdllSyscallNumber('NtWriteVirtualMemory'));
  if stubWrite = nil then
    Exit(WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten));
  Result := stubWrite(DWORD(hProcess), DWORD(lpBaseAddress), DWORD(lpBuffer), DWORD(nSize)) = 0;
end;

end.
