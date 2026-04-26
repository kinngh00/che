unit handle_hide;

{$mode objfpc}{$H+}

interface

procedure HideProcessHandle(hProcess: THandle);

implementation

uses
  Windows;

type
  PObjectTypeInformation = ^TObjectTypeInformation;
  TObjectTypeInformation = record
    TypeName: UNICODE_STRING;
    TotalNumberOfObjects: ULONG;
    TotalNumberOfHandles: ULONG;
    // ... more fields
  end;

  PSystemHandleInformation = ^TSystemHandleInformation;
  TSystemHandleInformation = record
    ProcessId: ULONG;
    ObjectTypeNumber: BYTE;
    Flags: BYTE;
    Handle: WORD;
    Object_: Pointer;
    GrantedAccess: ACCESS_MASK;
  end;

  TNtQuerySystemInformation = function(SystemInformationClass: DWORD; SystemInformation: Pointer; SystemInformationLength: ULONG; ReturnLength: PULONG): NTSTATUS; stdcall;
  TNtDuplicateObject = function(SourceProcessHandle: THandle; SourceHandle: THandle; TargetProcessHandle: THandle; var TargetHandle: THandle; DesiredAccess: ACCESS_MASK; Attributes: ULONG; Options: ULONG): NTSTATUS; stdcall;

var
  fnNtQuerySystemInformation: TNtQuerySystemInformation = nil;
  fnNtDuplicateObject: TNtDuplicateObject = nil;

function InitNtApis: Boolean;
var
  h: HMODULE;
begin
  if Assigned(fnNtQuerySystemInformation) then Exit(True);
  h := GetModuleHandle('ntdll.dll');
  if h = 0 then Exit(False);
  fnNtQuerySystemInformation := TNtQuerySystemInformation(GetProcAddress(h, 'NtQuerySystemInformation'));
  fnNtDuplicateObject := TNtDuplicateObject(GetProcAddress(h, 'NtDuplicateObject'));
  Result := Assigned(fnNtQuerySystemInformation) and Assigned(fnNtDuplicateObject);
end;

procedure HideProcessHandle(hProcess: THandle);
const
  SystemHandleInformation = 16;
  DUPLICATE_CLOSE_SOURCE = $00000001;
  DUPLICATE_SAME_ACCESS = $00000002;
var
  buf: Pointer;
  bufSize: ULONG;
  retLen: ULONG;
  status: NTSTATUS;
  i: Integer;
  handleInfo: PSystemHandleInformation;
  numHandles: ULONG;
  currentPid: DWORD;
  dummy: THandle;
begin
  if not InitNtApis then Exit;
  if hProcess = 0 then Exit;
  
  currentPid := GetCurrentProcessId;
  bufSize := 1024 * 1024; // 1MB initial
  GetMem(buf, bufSize);
  
  status := fnNtQuerySystemInformation(SystemHandleInformation, buf, bufSize, @retLen);
  if status = $C0000004 then // STATUS_INFO_LENGTH_MISMATCH
  begin
    FreeMem(buf);
    bufSize := retLen + 1024;
    GetMem(buf, bufSize);
    status := fnNtQuerySystemInformation(SystemHandleInformation, buf, bufSize, @retLen);
  end;
  
  if status = 0 then
  begin
    numHandles := PDWORD(buf)^;
    handleInfo := PSystemHandleInformation(PByte(buf) + SizeOf(DWORD));
    for i := 0 to numHandles - 1 do
    begin
      if (handleInfo^.ProcessId = currentPid) and (handleInfo^.Handle = Word(hProcess)) then
      begin
        // Duplicate handle with DUPLICATE_CLOSE_SOURCE to remove from our handle table
        // while keeping access via the duplicated handle
        fnNtDuplicateObject(GetCurrentProcess, hProcess, GetCurrentProcess, dummy, 0, 0, DUPLICATE_CLOSE_SOURCE or DUPLICATE_SAME_ACCESS);
        Break;
      end;
      Inc(handleInfo);
    end;
  end;
  
  FreeMem(buf);
end;

end.
