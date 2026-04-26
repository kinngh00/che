unit ce_crypto;

{$mode objfpc}{$H+}

interface

function XS(const s: string): PChar;

implementation

function XS(const s: string): PChar;
var
  i: Integer;
  k: Byte;
  temp: string;
begin
  k := $7A;
  SetLength(temp, Length(s));
  for i := 1 to Length(s) do
  begin
    temp[i] := Chr(Ord(s[i]) xor k);
    k := (k + 7) mod 256;
  end;
  Result := PChar(temp);
end;

end.
