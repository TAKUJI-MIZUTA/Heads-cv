//---------------------------------------------------------------------------
#ifndef Bas_YouryosyoH
#define Bas_YouryosyoH
//---------------------------------------------------------------------------
extern bool __fastcall Youryosyo(HeadsData* pcHeadsData);    // 検査要領書作成
// 2022.04.12 耐圧保持時間指定追加_S
extern bool __fastcall Tokki_Code(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
extern AnsiString __fastcall Tokki_Zen(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2022.04.12 耐圧保持時間指定追加_E
//---------------------------------------------------------------------------
#endif

