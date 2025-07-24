#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vcl.h>
#include <vector>
#include <queue>
#pragma hdrstop
#include "Game.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TGameForm *GameForm;

__fastcall TGameForm::TGameForm(TComponent* Owner) : TForm(Owner) {}

// Инициализация игры
void __fastcall TGameForm::FormCreate(TObject *Sender) {
    srand(time(0));
    // Стартовая позиция шарика игрока
    playerStartPos.x = PlayerBubble->Left;
    playerStartPos.y = PlayerBubble->Top;
    
    // Инизиализация шариков
    for (int i = 0; i < RowCount; ++i) {
        for (int j = 0; j < ColumnCount; ++j) {
            TPoint bubblePos = getBubbleCoordinate(i, j);
            bubble[i][j] = new TShape(this);
            bubble[i][j]->Parent = this;
            bubble[i][j]->Height = 2 * BubbleRadius;
            bubble[i][j]->Width = 2 * BubbleRadius;
            bubble[i][j]->Left = bubblePos.x;
            bubble[i][j]->Top = bubblePos.y;
            bubble[i][j]->Shape = stCircle;
            bubble[i][j]->Pen->Color = clWhite;
            bubble[i][j]->Visible = false;
            bubble[i][j]->Brush->Color = clBlack;
            bubble[i][j]->OnMouseDown = GameForm->ShapeMouseDown;
        }
    }

    newGame(Easy);
}

// Таймер
void __fastcall TGameForm::Timer1Timer(TObject *Sender) {
    if (gameState == Shoot) {
        ShootBubble();
    }
    ScoreNumber->Caption = gameScore;
}

void TGameForm::newGame(DifficultyLevel level) {
    colorCount = level;
    gameScore = 0;
    NextBubble->Brush->Color = Colors[rand() % colorCount];
    nextBubble();

    for (int i = 0; i < RowCount; ++i) {
        for (int j = 0; j < ColumnCount; ++j) {
            if (i >= StartRowCount) {
                bubble[i][j]->Visible = false;
                continue;
            }
			bubble[i][j]->Brush->Color = Colors[rand() % colorCount];
			bubble[i][j]->Visible = true;
        }
    }
}

void TGameForm::nextBubble() {
    PlayerBubble->Left = playerStartPos.x;
    PlayerBubble->Top = playerStartPos.y;
    PlayerBubble->Brush->Color = NextBubble->Brush->Color;
    NextBubble->Brush->Color = Colors[rand() % colorCount];
    gameState = Ready;
}

// Возвращает позиции соседей шарика
vector<TPoint> TGameForm::getNeighbors(int row, int col) {
    vector<TPoint> neighbors;
    TPoint bubblePos;

    for (int i = 0; i < OffsetCount; ++i) {
        bubblePos.y = row + Offset[row % 2][i][1];
        bubblePos.x = col + Offset[row % 2][i][0];

        if (bubblePos.x >= 0 && bubblePos.y >= 0 && bubblePos.x < ColumnCount && 
            bubblePos.y < RowCount) {
            neighbors.push_back(bubblePos);
        }
    }

    return neighbors;
}

// Находит шарику новую позицию на игровом поле
TPoint TGameForm::getNewBubblePosition(int row, int col) {
    double minDist = 5 * BubbleRadius;
    TPoint result;
    vector<TPoint> neighbors = getNeighbors(row, col);

    for (int i = 0; i < (int) neighbors.size(); ++i) {
        TPoint bubblePos = neighbors[i];

        // Если ячейка занята
        if (bubble[bubblePos.y][bubblePos.x]->Visible) {
            continue;
        }

        double dist = getDistanceBubbles(PlayerBubble->Left + BubbleRadius,
            PlayerBubble->Top + BubbleRadius,
            bubble[bubblePos.y][bubblePos.x]->Left + BubbleRadius,
            bubble[bubblePos.y][bubblePos.x]->Top + BubbleRadius);
        
        if (minDist > dist) {
            minDist = dist;
            result = bubblePos;
        }
    }

    return result;
}

// Находит группу шариков одного цвета
vector<TPoint> TGameForm::findGroupBubbles(int row, int col) {
	static bool used[RowCount][ColumnCount];
	queue<TPoint> neighbors;
	vector<TPoint> result;
	TPoint startPos;
	
	startPos.x = col;
	startPos.y = row;
	for (int i = 0; i < RowCount; ++i) {
		for (int j = 0; j < ColumnCount; ++j) {
			used[i][j] = false;
		}
	}
	
	used[startPos.y][startPos.x] = true;
	neighbors.push(startPos);
	result.push_back(startPos);

	while (!neighbors.empty()) {
		TPoint neighbor = neighbors.front();
		neighbors.pop();
		vector<TPoint> newNeighbors = getNeighbors(neighbor.y, neighbor.x);
		for (int i = 0; i < (int) newNeighbors.size(); ++i) {
			TPoint newNeighbor = newNeighbors[i];
			if (!used[newNeighbor.y][newNeighbor.x] &&
				bubble[newNeighbor.y][newNeighbor.x]->Brush->Color ==
				bubble[neighbor.y][neighbor.x]->Brush->Color &&
                bubble[neighbor.y][neighbor.x]->Visible) {
				used[newNeighbor.y][newNeighbor.x] = true;
				neighbors.push(newNeighbor);
				result.push_back(newNeighbor);
			}
		}
	}

	return result;
}

// Координаты шарика на игровом поле
TPoint TGameForm::getBubbleCoordinate(int row, int col) {
    TPoint result;

    result.x = col * (2 * BubbleRadius);
    result.y = row * (2 * BubbleRadius - 5);

    if (row % 2) {
        result.x += BubbleRadius;
    }

    return result;
}

// Устанавливает направление полета шарика
// Принимает координаты мыши
void TGameForm::setMouseAngle(int x, int y) {
    TPoint player;
    player.x = playerStartPos.x + BubbleRadius;
    player.y = playerStartPos.y + BubbleRadius;

    double angle = atan2(player.y - y, x - player.x);

    if (angle < 0) {
        angle += 2 * PI;
    }

    // Нижняя градица угла полета шарика
    const double lowerBound = 8 * (PI / 180);
    // Верхняя градица угла полета шарика
    const double upperBound = 172 * (PI / 180);

    if (angle > PI / 2 && angle < 3 * PI / 2 && angle > upperBound) {
        angle = upperBound;
    } else if (angle < lowerBound || angle >= 3 * PI / 2) {
        angle = lowerBound;
    }

    mouseAngle = angle;
}

// Добавляет шарик на игровое поле
void TGameForm::addBubble(int row, int col) {
    TPoint pos = getNewBubblePosition(row, col);

    bubble[pos.y][pos.x]->Brush->Color = PlayerBubble->Brush->Color;
    bubble[pos.y][pos.x]->Visible = true;
	
	deleteBubbles(pos.y, pos.x);
    nextBubble();
}

// Удаляет группу шариков
void TGameForm::deleteBubbles(int row, int col) {
	vector<TPoint> cluster = findGroupBubbles(row, col);
	
    // Если в группе 3 или более шарика
	if (cluster.size() >= 3) {
		for (int i = 0; i < (int) cluster.size(); ++i) {
            // Делаем их невидимыми
			bubble[cluster[i].y][cluster[i].x]->Visible = false;
            // Устанавливаем нейтральный цвет
            bubble[cluster[i].y][cluster[i].x]->Brush->Color = clBlack;
		}
        // Добавляем очки
        gameScore += cluster.size();
	}
}

void TGameForm::ShootBubble() {
    PlayerBubble->Left += BubbleSpeed * cos(mouseAngle);
    PlayerBubble->Top -= BubbleSpeed * sin(mouseAngle);

    if (PlayerBubble->Left <= 0) {
        mouseAngle = PI - mouseAngle;
        PlayerBubble->Left = 0;
    } else if (PlayerBubble->Left + 2 * BubbleRadius >= BoardWidth) {
        mouseAngle = PI - mouseAngle;
        PlayerBubble->Left = BoardWidth - 2 * BubbleRadius;
    }

    for (int i = 0; i < RowCount; ++i) {
        for (int j = 0; j < ColumnCount; ++j) {
            if (!bubble[i][j]->Visible && PlayerBubble->Top > 0) {
                continue;
            }

            TPoint bubblePos = getBubbleCoordinate(i, j);

            if (getDistanceBubbles(PlayerBubble->Left + BubbleRadius,
                PlayerBubble->Top + BubbleRadius,
                bubblePos.x + BubbleRadius,
                bubblePos.y + BubbleRadius) <= 2 * BubbleRadius) {
                addBubble(i, j);
                return;
            }
        }
    }
}

// Расстояние между шариками
double TGameForm::getDistanceBubbles(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void __fastcall TGameForm::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y) {
    if (gameState != Shoot) {
        setMouseAngle(X, Y);
        gameState = Shoot;
    }
}

void __fastcall TGameForm::ShapeMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y) {
    if (gameState != Shoot) {
        TPoint pos = GameForm->ScreenToClient(Mouse->CursorPos);
        setMouseAngle(pos.x, pos.y);
        gameState = Shoot;
    }
}

void __fastcall TGameForm::N3Click(TObject *Sender) {
    newGame(colorCount);
}

void __fastcall TGameForm::N6Click(TObject *Sender) {
    newGame(Easy);
}

void __fastcall TGameForm::N7Click(TObject *Sender) {
    newGame(Normal);
}

void __fastcall TGameForm::N8Click(TObject *Sender) {
    newGame(Hard);
}