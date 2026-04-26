unit theme_engine;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Graphics, Forms, Controls, StdCtrls, ExtCtrls, ComCtrls,
  Menus, Buttons;

type
  TAppTheme = (atDark, atLight);

  TThemeColors = record
    bgPrimary: TColor;      // Main background
    bgSecondary: TColor;    // Panel/secondary background
    bgTertiary: TColor;     // Input/button background
    textPrimary: TColor;    // Main text
    textSecondary: TColor;  // Secondary/muted text
    accent: TColor;         // Accent/highlight
    accentHover: TColor;    // Accent hover state
    border: TColor;         // Borders
    success: TColor;        // Success states
    warning: TColor;        // Warning states
    error: TColor;          // Error states
  end;

procedure SetTheme(theme: TAppTheme);
function GetCurrentTheme: TAppTheme;
function GetThemeColors: TThemeColors;
procedure ApplyThemeToForm(AForm: TForm);
procedure ApplyThemeToControl(AControl: TControl);

implementation

var
  CurrentTheme: TAppTheme = atDark;

  // Catppuccin Mocha (Dark) - 2026 dev tool trend
  DarkColors: TThemeColors = (
    bgPrimary:    $2E1E1E;  // Base #1E1E2E
    bgSecondary:  $443131;  // Surface0 #313244
    bgTertiary:   $585454;  // Surface1 #45475A
    textPrimary:  $F4D6CD;  // Text #$CDD6F4
    textSecondary:$B4A6A0;  // Subtext0 #$A6ADC8
    accent:       $FAB489;  // Blue #89B4FA
    accentHover:  $B4E8F4;  // Sapphire #74C7EC
    border:       $6E5C5C;  // Overlay0 #6C7086
    success:      $A6E3A1;  // Green #$A6E3A1
    warning:      $F9E2AF;  // Yellow #$F9E2AF
    error:        $A1A0F4;  // Red #$F38BA8 -> swapped B/R for TColor
  );

  // Catppuccin Latte (Light)
  LightColors: TThemeColors = (
    bgPrimary:    $F5F1EF;  // Base #$EFF1F5
    bgSecondary:  $DAD0CC;  // Surface0 #$CCD0DA
    bgTertiary:   $E6E9EF;  // Surface1 #$BCC0CC -> adjusted
    textPrimary:  $696F4C;  // Text #4C4F69
    textSecondary:$8C8FA1;  // Subtext0 #8C8FA1
    accent:       $F5661E;  // Blue #1E66F5 -> swapped B/R
    accentHover:  $D47D1E;  // Sapphire #209FB5 -> adjusted
    border:       $BCC0CC;  // Overlay0 #9CA0B0 -> adjusted
    success:      $40A02B;  // Green #40A02B
    warning:      $DF8E1D;  // Yellow #$DF8E1D
    error:        $3954D1;  // Red #$D20F39 -> swapped B/R
  );

function GetCurrentTheme: TAppTheme;
begin
  Result := CurrentTheme;
end;

function GetThemeColors: TThemeColors;
begin
  if CurrentTheme = atDark then
    Result := DarkColors
  else
    Result := LightColors;
end;

procedure SetTheme(theme: TAppTheme);
var
  i: Integer;
begin
  CurrentTheme := theme;
  for i := 0 to Screen.FormCount - 1 do
    ApplyThemeToForm(Screen.Forms[i]);
end;

procedure ApplyThemeToControl(AControl: TControl);
var
  c: TThemeColors;
  i: Integer;
begin
  c := GetThemeColors;

  if AControl is TPanel then
  begin
    TPanel(AControl).Color := c.bgSecondary;
    TPanel(AControl).Font.Color := c.textPrimary;
  end
  else if AControl is TButton then
  begin
    TButton(AControl).Color := c.bgTertiary;
    TButton(AControl).Font.Color := c.textPrimary;
  end
  else if AControl is TEdit then
  begin
    TEdit(AControl).Color := c.bgTertiary;
    TEdit(AControl).Font.Color := c.textPrimary;
  end
  else if AControl is TMemo then
  begin
    TMemo(AControl).Color := c.bgTertiary;
    TMemo(AControl).Font.Color := c.textPrimary;
  end
  else if AControl is TListBox then
  begin
    TListBox(AControl).Color := c.bgTertiary;
    TListBox(AControl).Font.Color := c.textPrimary;
  end
  else if AControl is TComboBox then
  begin
    TComboBox(AControl).Color := c.bgTertiary;
    TComboBox(AControl).Font.Color := c.textPrimary;
  end
  else if AControl is TLabel then
  begin
    TLabel(AControl).Font.Color := c.textPrimary;
  end
  else if AControl is TPageControl then
  begin
    TPageControl(AControl).Color := c.bgSecondary;
  end
  else if AControl is TStatusBar then
  begin
    TStatusBar(AControl).Color := c.bgSecondary;
    TStatusBar(AControl).Font.Color := c.textSecondary;
  end
  else if AControl is TToolBar then
  begin
    TToolBar(AControl).Color := c.bgSecondary;
  end;

  // Recurse into child controls
  if AControl is TWinControl then
    for i := 0 to TWinControl(AControl).ControlCount - 1 do
      ApplyThemeToControl(TWinControl(AControl).Controls[i]);
end;

procedure ApplyThemeToForm(AForm: TForm);
var
  c: TThemeColors;
  i: Integer;
begin
  if AForm = nil then Exit;
  c := GetThemeColors;

  AForm.Color := c.bgPrimary;
  AForm.Font.Color := c.textPrimary;
  AForm.Font.Name := 'Segoe UI';
  AForm.Font.Size := 9;

  for i := 0 to AForm.ControlCount - 1 do
    ApplyThemeToControl(AForm.Controls[i]);
end;

end.
