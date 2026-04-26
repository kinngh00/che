unit jitter;

{$mode objfpc}{$H+}

interface

procedure JitterSleep;
procedure DecoyRead;

implementation

uses
  Windows, SysUtils;

var
  jitterInitialized: Boolean = False;
  jitterSeed: DWORD = 0;

procedure InitJitter;
begin
  if not jitterInitialized then
  begin
    jitterSeed := GetTickCount;
    jitterInitialized := True;
  end;
end;

function JitterRandom(max: Integer): Integer;
begin
  InitJitter;
  jitterSeed := jitterSeed * 1103515245 + 12345;
  Result := (jitterSeed shr 16) mod max;
end;

procedure JitterSleep;
begin
  // Random sleep between 1-50ms to break scan timing patterns
  Sleep(1 + JitterRandom(50));
end;

procedure DecoyRead;
var
  h: THandle;
  buf: array[0..63] of Byte;
  read: SIZE_T;
begin
  // Read from an unrelated system process to confuse behavioral analysis
  h := OpenProcess(PROCESS_VM_READ, False, 4); // System PID
  if h <> 0 then
  begin
    ReadProcessMemory(h, Pointer($7FFE0000), @buf[0], 64, read);
    CloseHandle(h);
  end;
end;

end.
