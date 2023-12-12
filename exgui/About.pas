unit About;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, MMSystem, StdCtrls, Jpeg;

type
  TfmAbout = class(TForm)
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  TBGRArray = Array[0..MaxInt div SizeOf(TRGBQuad)-1] of TRGBQuad;
  PBGRArray = ^TBGRArray;

  TCreditLine = record
    Line: String;
    Color: COLORREF;
    Bold: Boolean;
  end;

const
  COLOR_TITLE = $000080FF;
  COLOR_NAME  = $00FFFFFF;//$00FFAA00;
  COLOR_URL   = $00FFAA00;//$008000FF;

var
  CreditLines: Array[0..45] of TCreditLine =
  (
    //---------- Header Start
    //(Line: 'Grand Theft Auto'; Color: $00FF8000; Bold: true),
    (Line: 'San Andreas'; Color: COLOR_NAME; Bold: true),
    (Line: 'Multiplayer'; Color: COLOR_TITLE; Bold: true),
    //---------- Header End

    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer

    //---------- Coder Start
    (Line: 'Coding:'; Color: COLOR_TITLE; Bold: true),
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: 'Kalcor'; Color: COLOR_NAME; Bold: false),
    (Line: 'spookie'; Color: COLOR_NAME; Bold: false),
    //---------- Coder End

    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer

    //---------- Past Coders Start
    (Line: 'Past coders:'; Color: COLOR_TITLE; Bold: true),
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: 'Y_Less'; Color: COLOR_NAME; Bold: false),
    //---------- Past Coders End

    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer

    //---------- Tester Start
    (Line: 'Beta Testing:'; Color: COLOR_TITLE; Bold: true),
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: 'BlueG, cessil, CrazyBob'; Color: COLOR_NAME; Bold: false),
    (Line: 'DamianC, dugi, d0'; Color: COLOR_NAME; Bold: false),
    (Line: 'Jay, JernejL, kaisersouse'; Color: COLOR_NAME; Bold: false),
    (Line: 'KingJ, Matite, Mmartin'; Color: COLOR_NAME; Bold: false),
    (Line: 'RayW, Si|ent, Wicko'; Color: COLOR_NAME; Bold: false),
    //---------- Tester End

    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer

    //---------- URL Start
    (Line: 'www.sa-mp.com'; Color: COLOR_URL; Bold: true),
    //---------- URL End

    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false), // Spacer
    (Line: ''; Color: 666; Bold: false) // Spacer
  );

  fmAbout: TfmAbout;
  CritSect: _RTL_CRITICAL_SECTION;
  GameOver: Boolean;

  DIBWidth: Integer;
  DIBHeight: Integer;

  // Timing
  Ticks: Cardinal;
  TimeScale: Integer = 1;

  // Drawing
  RenderThread: THandle;
  bmi: BITMAPINFO;
  hDC1, hDC2: HDC;
  Buf: PBGRArray;
  hBmp: HBITMAP;
  hNormFont, hBoldFont: HFONT;
  xRect: TRect;

implementation

{$R *.dfm}

var
  CreditsRollY: Integer = 300;
  TempCRY: Integer = 0;
procedure Flip;
var
  i: Integer;
begin
  SetDIBits(hDC2, hBmp, 0, DIBHeight, @Buf[0], bmi, DIB_RGB_COLORS);

  Inc(TempCRY, TimeScale);
  if TempCRY >= 5 then begin
    TempCRY:= 0;
    Dec(CreditsRollY, TimeScale);
    if CreditsRollY < -((High(CreditLines) * 12) + 50) then
      CreditsRollY:= 300;
  end;

  xRect.Top:= CreditsRollY;
  for i:= 0 to High(CreditLines) do begin
    if (CreditLines[i].Color <> 666) and (xRect.Top > -12) and (xRect.Top < 300) then begin
      SetTextColor(hDC2, CreditLines[i].Color);
      if CreditLines[i].Bold then
        SelectObject(hDC2, hBoldFont)
      else
        SelectObject(hDC2, hNormFont);
      DrawText(hDC2, PChar(CreditLines[i].Line), -1, xRect, DT_NOCLIP or DT_CENTER);
    end;
    Inc(xRect.Top, 20);
  end;

  BitBlt(hDC1, 2, 2, DIBWidth, DIBHeight, hDC2, 0, 0, SRCCOPY);
end;

procedure Render;
var
  t: Cardinal;
begin
  timeBeginPeriod(1);
  t:= timeGetTime;
  timeEndPeriod(1);
  TimeScale:= Round(100 / (1000 / (t - Ticks)));
  Ticks:= t;

  ZeroMemory(Buf, (DIBWidth*DIBHeight)*4);

  Flip;
end;

procedure RenderTimer;
begin
  while true do begin
    Sleep(10);
    EnterCriticalSection(CritSect);
    if GameOver then begin
      LeaveCriticalSection(CritSect);
      Exit;
    end;
    Render;
    LeaveCriticalSection(CritSect);
  end;
end;

procedure TfmAbout.FormShow(Sender: TObject);
var
  FontStruct: LogFont;
begin
  CreditsRollY:= 300;

  GetMem(Buf, (DIBWidth*DIBHeight)*4);
  hDC1:= GetDC(fmAbout.Handle);
  hDC2:= CreateCompatibleDC(hDC1);
  hBmp:= CreateCompatibleBitmap(hDC1, DIBWidth, DIBHeight);
  ZeroMemory(@FontStruct, SizeOf(FontStruct));
  FontStruct.lfWidth:= 0;
  FontStruct.lfHeight:= -18;
  FontStruct.lfQuality:= ANTIALIASED_QUALITY;
  FontStruct.lfFaceName:= 'Arial';
  hNormFont:= CreateFontIndirect(FontStruct);
  FontStruct.lfWeight:= FW_BOLD;
  hBoldFont:= CreateFontIndirect(FontStruct);

  SelectObject(hDC2, hBmp);
  SetBkMode(hDC2, TRANSPARENT);

  Ticks:= timeGetTime;
  GameOver:= false;
  BeginThread(nil, 0, @RenderTimer, nil, 0, RenderThread);
end;

procedure TfmAbout.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  EnterCriticalSection(CritSect);

  GameOver:= true;

  DeleteObject(hNormFont);
  DeleteObject(hBoldFont);
  DeleteObject(hBmp);
  DeleteDC(hDC1);
  DeleteDC(hDC2);
  FreeMem(Buf);

  LeaveCriticalSection(CritSect);
end;

procedure TfmAbout.FormCreate(Sender: TObject);
begin
  InitializeCriticalSection(CritSect);

  DIBWidth:= ClientWidth - 4;
  DIBHeight:= ClientHeight - 4;

  xRect:= Rect(0, 0, ClientWidth, ClientHeight);

  with bmi.bmiHeader do begin
    biSize:=          SizeOf(bmi.bmiHeader);
    biWidth:=         DIBWidth;
    biHeight:=       -DIBHeight;
    biPlanes:=        1;
    biBitCount:=      32;
    biCompression:=   BI_RGB;
    biSizeImage:=     0;
    biXPelsPerMeter:= 0;
    biYPelsPerMeter:= 0;
    biClrUsed:=       0;
    biClrImportant:=  0;
  end;
end;

procedure TfmAbout.FormDestroy(Sender: TObject);
begin
  DeleteCriticalSection(CritSect);
end;

procedure TfmAbout.FormClick(Sender: TObject);
begin
  Close;
end;

end.
