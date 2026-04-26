unit window_hide;

{$mode objfpc}{$H+}

interface

procedure RandomizeWindowIdentity;

implementation

uses
  Windows, SysUtils, Forms;

function RandomString(len: Integer): string;
var
  i: Integer;
const
  chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
begin
  SetLength(Result, len);
  for i := 1 to len do
    Result[i] := chars[Random(Length(chars)) + 1];
end;

procedure RandomizeWindowIdentity;
var
  h: HWND;
begin
  Randomize;
  h := Application.Handle;
  if h <> 0 then
  begin
    // Randomize window class name and title at runtime
    // Note: This only affects the top-level application window
    // Child forms need individual handling
    SetWindowText(h, PChar(RandomString(8) + ' ' + RandomString(4)));
  end;
end;

end.
