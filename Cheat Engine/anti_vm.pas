unit anti_vm;

{$mode objfpc}{$H+}

interface

function IsSandboxed: Boolean;
function IsVM: Boolean;

implementation

uses
  Windows, SysUtils;

function CheckProc(const name: string): Boolean;
var
  h: THandle;
  pe: TProcessEntry32;
begin
  Result := False;
  h := CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if h = INVALID_HANDLE_VALUE then Exit;
  pe.dwSize := SizeOf(pe);
  if Process32First(h, pe) then
    repeat
      if SameText(pe.szExeFile, name) then
      begin
        Result := True;
        Break;
      end;
    until not Process32Next(h, pe);
  CloseHandle(h);
end;

function IsSandboxed: Boolean;
begin
  Result := CheckProc('vmsrvc.exe') or CheckProc('vmusrvc.exe')
         or CheckProc('VBoxService.exe') or CheckProc('VBoxTray.exe')
         or CheckProc('xenservice.exe') or CheckProc('qemu-ga.exe')
         or CheckProc('Cuckoo.exe') or CheckProc('SbieSvc.exe');
end;

function IsVM: Boolean;
var
  h: THandle;
  buf: array[0..255] of Char;
begin
  // Safe VM checks: registry + files + processes only
  // Removed dangerous IO port backdoor (in eax, dx) that crashes on real hardware
  Result := False;

  // VMWare registry
  if RegOpenKeyEx(HKEY_LOCAL_MACHINE, 'SOFTWARE\VMware, Inc.\VMware Tools', 0, KEY_QUERY_VALUE, h) = ERROR_SUCCESS then
  begin
    Result := True;
    RegCloseKey(h);
    Exit;
  end;

  // VirtualBox registry
  if RegOpenKeyEx(HKEY_LOCAL_MACHINE, 'SOFTWARE\Oracle\VirtualBox Guest Additions', 0, KEY_QUERY_VALUE, h) = ERROR_SUCCESS then
  begin
    Result := True;
    RegCloseKey(h);
    Exit;
  end;

  // VMWare driver file
  if FileExists('C:\WINDOWS\system32\drivers\vmmouse.sys') then
  begin
    Result := True;
    Exit;
  end;

  // VBox driver file
  if FileExists('C:\WINDOWS\system32\drivers\VBoxMouse.sys') then
  begin
    Result := True;
    Exit;
  end;
end;

end.
