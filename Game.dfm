object GameForm: TGameForm
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'Bubble Shooter'
  ClientHeight = 600
  ClientWidth = 775
  Color = clMoneyGreen
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnMouseDown = FormMouseDown
  PixelsPerInch = 96
  TextHeight = 13
  object PlayerBubble: TShape
    Left = 360
    Top = 528
    Width = 50
    Height = 50
    Brush.Color = clYellow
    Pen.Color = clWhite
    Shape = stCircle
  end
  object NextBubble: TShape
    Left = 160
    Top = 528
    Width = 50
    Height = 50
    Brush.Color = clYellow
    Pen.Color = clWhite
    Shape = stCircle
  end
  object ScoreTitle: TLabel
    Left = 656
    Top = 528
    Width = 59
    Height = 25
    Caption = 'Score:'
    Color = clMoneyGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = cl3DDkShadow
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object ScoreNumber: TLabel
    Left = 656
    Top = 559
    Width = 11
    Height = 25
    Caption = '0'
    Color = clMoneyGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = cl3DDkShadow
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Timer1: TTimer
    Interval = 30
    OnTimer = Timer1Timer
    Left = 360
    Top = 256
  end
  object MainMenu1: TMainMenu
    Left = 232
    Top = 248
    object N1: TMenuItem
      Caption = #1048#1075#1088#1072
      object N3: TMenuItem
        Caption = #1053#1086#1074#1072#1103' '#1080#1075#1088#1072
        OnClick = N3Click
      end
      object N4: TMenuItem
        Caption = #1059#1088#1086#1074#1077#1085#1100' '#1089#1083#1086#1078#1085#1086#1089#1090#1080
        object N6: TMenuItem
          Caption = #1051#1077#1075#1082#1086
          OnClick = N6Click
        end
        object N7: TMenuItem
          Caption = #1053#1086#1088#1084#1072#1083#1100#1085#1086
          OnClick = N7Click
        end
        object N8: TMenuItem
          Caption = #1058#1103#1078#1077#1083#1086
          OnClick = N8Click
        end
      end
      object N5: TMenuItem
        Caption = #1042#1099#1093#1086#1076
      end
    end
    object N2: TMenuItem
      Caption = #1057#1087#1088#1072#1074#1082#1072
      object N9: TMenuItem
        Caption = #1055#1088#1072#1074#1080#1083#1072' '#1080#1075#1088#1099
      end
      object N10: TMenuItem
        Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
      end
    end
  end
end
