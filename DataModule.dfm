object DataModule1: TDataModule1
  OldCreateOrder = False
  Height = 480
  Width = 696
  object Database_DNO: TFDConnection
    Left = 32
    Top = 26
  end
  object Query_MAT: TFDQuery
    Left = 33
    Top = 87
  end
  object Database_MAT: TFDConnection
    Left = 116
    Top = 26
  end
  object Database_INS: TFDConnection
    Left = 197
    Top = 25
  end
  object FDManagerDN: TFDManager
    FormatOptions.AssignedValues = [fvMapRules]
    FormatOptions.OwnMapRules = True
    FormatOptions.MapRules = <>
    Active = True
    Left = 328
    Top = 24
  end
  object FDGUIxWaitCursor1: TFDGUIxWaitCursor
    Provider = 'Forms'
    Left = 328
    Top = 152
  end
  object FDPhysMSAccessDriverLink1: TFDPhysMSAccessDriverLink
    Left = 328
    Top = 88
  end
end
