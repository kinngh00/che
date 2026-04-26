unit peb_clean;

{$mode objfpc}{$H+}

interface

procedure CleanPEB;
procedure CleanTEB;
procedure CleanHeapFlags;
procedure HideDebugRegisters;

implementation

uses
  Windows;

procedure CleanPEB;
var
  pPEB: PByte;
begin
  {$IFDEF CPUX64}
  pPEB := PByte(gs:[$60]);
  {$ELSE}
  pPEB := PByte(fs:[$30]);
  {$ENDIF}
  // BeingDebugged (offset 0x2)
  PByte(pPEB + $02)^ := 0;
  // NtGlobalFlag (offset 0xBC x86 / 0xBC x64)
  {$IFDEF CPUX64}
  PDWORD(pPEB + $BC)^ := 0;
  {$ELSE}
  PDWORD(pPEB + $68)^ := 0;
  {$ENDIF}
  // ProcessHeap Flags (offset 0x30 x86 / 0x30 x64 -> Heap + 0x40/0x70)
  // Skip for now as heap structure varies
end;

procedure CleanTEB;
var
  pTEB: PByte;
begin
  {$IFDEF CPUX64}
  pTEB := PByte(gs:[$30]);
  {$ELSE}
  pTEB := PByte(fs:[$18]);
  {$ENDIF}
  // LastErrorValue (offset 0x68)
  PDWORD(pTEB + $68)^ := 0;
  // CrossTebFlags (offset 0x1C)
  PWORD(pTEB + $1C)^ := 0;
end;

procedure CleanHeapFlags;
var
  pPEB: PByte;
  pHeap: Pointer;
  pFlags: PDWORD;
  pForceFlags: PDWORD;
begin
  {$IFDEF CPUX64}
  pPEB := PByte(gs:[$60]);
  pHeap := PPointer(pPEB + $30)^;
  if pHeap <> nil then
  begin
    pFlags := PDWORD(PByte(pHeap) + $70);
    pForceFlags := PDWORD(PByte(pHeap) + $74);
    pFlags^ := pFlags^ and not $50000062; // Clear debug flags
    pForceFlags^ := 0;
  end;
  {$ELSE}
  pPEB := PByte(fs:[$30]);
  pHeap := PPointer(pPEB + $18)^;
  if pHeap <> nil then
  begin
    pFlags := PDWORD(PByte(pHeap) + $40);
    pForceFlags := PDWORD(PByte(pHeap) + $44);
    pFlags^ := pFlags^ and not $50000062;
    pForceFlags^ := 0;
  end;
  {$ENDIF}
end;

procedure HideDebugRegisters;
{$IFDEF CPUX64}
asm
  xor rax, rax
  mov dr0, rax
  mov dr1, rax
  mov dr2, rax
  mov dr3, rax
end;
{$ELSE}
asm
  xor eax, eax
  mov dr0, eax
  mov dr1, eax
  mov dr2, eax
  mov dr3, eax
end;
{$ENDIF}

end.
