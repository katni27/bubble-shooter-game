#ifndef GameH
#define GameH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <vector>

using namespace std;

enum GameStates {
    Ready,
    Shoot
};

enum DifficultyLevel {
    Easy = 3,
    Normal,
    Hard
};

TColor Colors[4] = {clRed, clGreen, clBlue, clYellow};

const int OffsetCount = 6;

int Offset[2][OffsetCount][2] = { { {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1} }, // четные
                                    { {1, 0}, {1, 1}, {0, 1}, {-1, 0}, {0, -1}, {1, -1} } }; // нечетные

const double PI = 3.1415926;
const int BoardWidth = 775;
const int BoardHeight = 600;
const int ColumnCount = 15;
const int RowCount = 10;
const int StartRowCount = 5;
const int BubbleRadius = 25;
const int BubbleSpeed = 20;

class TGameForm : public TForm {
__published:
    TTimer *Timer1;
    TShape *PlayerBubble;
    TShape *NextBubble;
    TLabel *ScoreTitle;
    TLabel *ScoreNumber;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *N6;
    TMenuItem *N7;
    TMenuItem *N8;
    TMenuItem *N9;
    TMenuItem *N10;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall ShapeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall N3Click(TObject *Sender);
    void __fastcall N6Click(TObject *Sender);
    void __fastcall N7Click(TObject *Sender);
    void __fastcall N8Click(TObject *Sender);
private:
    int colorCount;
    int gameScore;
    double mouseAngle;
    GameStates gameState;
    TPoint playerStartPos;
    TShape *bubble[RowCount][ColumnCount];
public:
    __fastcall TGameForm(TComponent* Owner);
    void setMouseAngle(int x, int y);
    void ShootBubble();
    void newGame(DifficultyLevel level);
    void nextBubble();
    void addBubble(int row, int col);
	void deleteBubbles(int row, int col);
    vector<TPoint> getNeighbors(int row, int col);
	vector<TPoint> findGroupBubbles(int row, int col);
    double getDistanceBubbles(int x1, int y1, int x2, int y2);
    TPoint getBubbleCoordinate(int row, int col);
    TPoint getNewBubblePosition(int row, int col);
};
extern PACKAGE TGameForm *GameForm;
#endif
