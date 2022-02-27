object FmRna: TFmRna
  Left = 0
  Top = 0
  Caption = 'Redes Neurais Artificiais'
  ClientHeight = 575
  ClientWidth = 1143
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 8
    Width = 1128
    Height = 509
    ActivePage = TabSheet1
    MultiLine = True
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Treinamento'
      object Chart1: TChart
        Left = 3
        Top = 3
        Width = 835
        Height = 475
        Legend.Visible = False
        Title.Font.Color = clNavy
        Title.Text.Strings = (
          'Curva de Treinamento da Rede Neural')
        BottomAxis.MinimumOffset = 3
        LeftAxis.MinimumOffset = 5
        View3D = False
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series1: TFastLineSeries
          SeriesColor = clRed
          ValueFormat = '#.##0,00###'
          LinePen.Color = clRed
          LinePen.Width = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series4: TFastLineSeries
          SeriesColor = clBlue
          Title = 'Series2'
          LinePen.Color = clBlue
          LinePen.Width = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Button1: TButton
        Left = 844
        Top = 433
        Width = 121
        Height = 45
        Caption = 'Treinamento'
        TabOrder = 1
        OnClick = Button1Click
      end
      object Button2: TButton
        Left = 995
        Top = 433
        Width = 122
        Height = 45
        Caption = 'Cancelar'
        TabOrder = 2
        OnClick = Button2Click
      end
      object Memo1: TMemo
        Left = 844
        Top = 3
        Width = 273
        Height = 424
        ScrollBars = ssBoth
        TabOrder = 3
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Padr'#245'es de Treinamento'
      ImageIndex = 1
      object Shape1: TShape
        Left = 960
        Top = 3
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape2: TShape
        Left = 960
        Top = 34
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape3: TShape
        Left = 960
        Top = 66
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape4: TShape
        Left = 960
        Top = 98
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape5: TShape
        Left = 960
        Top = 128
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape6: TShape
        Left = 960
        Top = 159
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape7: TShape
        Left = 960
        Top = 190
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape8: TShape
        Left = 960
        Top = 221
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape9: TShape
        Left = 960
        Top = 251
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape10: TShape
        Left = 960
        Top = 283
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape11: TShape
        Left = 960
        Top = 316
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape12: TShape
        Left = 960
        Top = 347
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape13: TShape
        Left = 960
        Top = 378
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape14: TShape
        Left = 960
        Top = 413
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape15: TShape
        Left = 960
        Top = 445
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape16: TShape
        Left = 1064
        Top = 143
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape17: TShape
        Left = 1064
        Top = 182
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape18: TShape
        Left = 1064
        Top = 221
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape19: TShape
        Left = 1064
        Top = 260
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Shape20: TShape
        Left = 1064
        Top = 299
        Width = 25
        Height = 33
        Shape = stCircle
      end
      object Label1: TLabel
        Left = 848
        Top = 3
        Width = 106
        Height = 13
        Caption = 'Camada Intermedi'#225'ria'
      end
      object Label2: TLabel
        Left = 1034
        Top = 124
        Width = 83
        Height = 13
        Caption = 'Camada de Sa'#237'da'
      end
      object ListBox1: TListBox
        Left = 3
        Top = 26
        Width = 121
        Height = 189
        ItemHeight = 13
        Items.Strings = (
          'SINAL 1Hz'
          'SINAL 2Hz'
          'SINAL 3Hz'
          'SINAL 4Hz'
          'SINAL 5Hz'
          'SINAL 6Hz'
          'SINAL 7Hz'
          'SINAL 8Hz'
          'SINAL 9Hz'
          'SINAL 10Hz')
        TabOrder = 0
        OnClick = ListBox1Click
      end
      object Chart2: TChart
        Left = 130
        Top = 3
        Width = 708
        Height = 475
        Legend.Visible = False
        Title.Text.Strings = (
          'TChart')
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.Maximum = 99.000000000000000000
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Maximum = 1.000000000000000000
        LeftAxis.MaximumOffset = 5
        LeftAxis.Minimum = -1.000000000000000000
        LeftAxis.MinimumOffset = 5
        View3D = False
        TabOrder = 1
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series2: TFastLineSeries
          SeriesColor = clRed
          LinePen.Color = clRed
          LinePen.Width = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series3: TFastLineSeries
          SeriesColor = clBlue
          LinePen.Color = clBlue
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object ListBox2: TListBox
        Left = 3
        Top = 260
        Width = 121
        Height = 197
        ItemHeight = 13
        Items.Strings = (
          '1.2'
          '1.6'
          '2.2'
          '2.4'
          '2.8'
          '3.1'
          '3.5'
          '3.7'
          '4.2'
          '4.4'
          '4.6'
          '4.9'
          '5.2'
          '5.6'
          '5.85'
          '6.2'
          '6.6'
          '6.75'
          '6.8'
          '7.1'
          '7.19'
          '7.4'
          '7.79'
          '8.2'
          '8.6'
          '8.9'
          '9.2'
          '9.3'
          '9.6'
          '9.9'
          '10.2'
          '10.6')
        TabOrder = 2
        OnClick = ListBox2Click
      end
    end
  end
  object Panel1: TPanel
    Left = 8
    Top = 523
    Width = 1116
    Height = 46
    Color = clHighlightText
    ParentBackground = False
    TabOrder = 1
    object Button3: TButton
      Left = 888
      Top = -4
      Width = 97
      Height = 41
      Caption = 'Abrir arquivo'
      TabOrder = 0
      OnClick = Button3Click
    end
  end
  object ProcurarArquivo: TOpenDialog
    FileName = 'foundfile'
    Options = [ofEnableSizing]
    Left = 528
    Top = 531
  end
end
