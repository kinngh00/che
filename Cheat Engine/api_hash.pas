unit api_hash;

{$mode objfpc}{$H+}

interface

function GH(modHash, apiHash: DWORD): Pointer;

implementation

uses
  Windows;

function GH(modHash, apiHash: DWORD): Pointer;
var
  pPEB: PByte;
  pLdr: PDWORD;
  pList: Pointer;
  flink: PDWORD;
  dllBase: PDWORD;
  exportDir: PIMAGE_EXPORT_DIRECTORY;
  names: PDWORD;
  funcs: PDWORD;
  ordinals: PWord;
  i: Integer;
  h: DWORD;
  n: PAnsiChar;
begin
  Result := nil;
  {$IFDEF CPUX64}
  pPEB := PByte(gs:[$30]);
  {$ELSE}
  pPEB := PByte(fs:[$18]);
  {$ENDIF}
  pLdr := PDWORD(pPEB + $60);
  pList := Pointer(PDWORD(pLdr^ + $18)^);
  flink := PDWORD(pList);
  while flink <> pList do
  begin
    dllBase := PDWORD(PByte(flink) + $30);
    h := 0;
    n := PAnsiChar(PDWORD(PByte(flink) + $58)^);
    while n^ <> #0 do
    begin
      h := ((h shr 13) or (h shl 19)) + Ord(LowerCase(n^)[1]);
      Inc(n);
    end;
    if h = modHash then
    begin
      exportDir := PIMAGE_EXPORT_DIRECTORY(dllBase^ + PIMAGE_NT_HEADERS(dllBase^ + PDWORD(dllBase^ + $3C)^)^.OptionalHeader.DataDirectory[0].VirtualAddress);
      names := PDWORD(dllBase^ + exportDir^.AddressOfNames);
      funcs := PDWORD(dllBase^ + exportDir^.AddressOfFunctions);
      ordinals := PWord(dllBase^ + exportDir^.AddressOfNameOrdinals);
      for i := 0 to exportDir^.NumberOfNames - 1 do
      begin
        h := 0;
        n := PAnsiChar(dllBase^ + names[i]);
        while n^ <> #0 do
        begin
          h := ((h shr 13) or (h shl 19)) + Ord(n^);
          Inc(n);
        end;
        if h = apiHash then
        begin
          Result := Pointer(dllBase^ + funcs[ordinals[i]]);
          Exit;
        end;
      end;
    end;
    flink := PDWORD(flink^);
  end;
end;

end.
