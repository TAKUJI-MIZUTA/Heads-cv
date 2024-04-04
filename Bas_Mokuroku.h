//---------------------------------------------------------------------------

#ifndef Bas_MokurokuH
#define Bas_MokurokuH
//---------------------------------------------------------------------------
#endif

extern bool __fastcall Mokuroku(HeadsData* pcHeadsData);          // 図面目録
extern void __fastcall SyuturyokuZumen_Set_S(AnsiString s_UtiKbn, bool bMarine, HeadsData* pcHeadsData);
extern AnsiString __fastcall spc(int iNum);

// 2007.11.15 ミラー仕様対応 機器名称取得関数
extern AnsiString __fastcall GetEquipmentName(AnsiString s_NameOrg, AnsiString sSpecCode);
// 2007.11.15

//extern void __fastcall SyuturyokuZumen_Set_S(char Pattern, AnsiString Lang);

// 2020.07.14 保護カバー注記追加_S
extern bool __fastcall Tokki_Code(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2020.07.14 保護カバー注記追加_E

// 2022.04.05 就航船名追加_S
extern AnsiString __fastcall Tokki_Zen(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2022.04.05 就航船名追加_E

