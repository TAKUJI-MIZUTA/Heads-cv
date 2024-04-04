//---------------------------------------------------------------------------

#ifndef Bas_SijisyoHA3
#define Bas_SijisyoHA3
//---------------------------------------------------------------------------
#endif

extern bool __fastcall SijisyoA3(HeadsData* pcHeadsData);          // 製作指示書

AnsiString __fastcall Sheet_Exist_CheckA3(AnsiString path, HeadsData* pcHeadsData); // 過去Excelｼｰﾄ(製作指示書)存在ﾁｪｯｸ
AnsiString __fastcall Get_Zaisitu_NameA3(AnsiString Code);          // 材質ｺｰﾄﾞﾏｽﾀ検索
AnsiString __fastcall Chg_Zaisitu_CodeA3(AnsiString Code,AnsiString Typ ); //材質コード変換マスタ検索
AnsiString __fastcall Get_AturyokuKeiA3(AnsiString s_Code);         // 圧力計種類取得

AnsiString __fastcall Get_SiageMeisyouA3(AnsiString Code);          // 仕上名称　取得
AnsiString __fastcall Get_Tankan_DataA3(AnsiString Syurui, HeadsData* pcHeadsData);   // 端管種類　取得
AnsiString __fastcall Get_Flange_DataA3(AnsiString Tugite);         // ﾌﾗﾝｼﾞ種類取得
bool       __fastcall Get_Start_PlateA3(AnsiString &Side_B, AnsiString &Side_A, HeadsData* pcHeadsData);
AnsiString __fastcall Get_T_ZaZaisituA3(AnsiString TanSyurui, AnsiString TanName, HeadsData* pcHeadsData);

void       __fastcall Sheet_Compare_SyoriA3(AnsiString Excel_OldPass);    //
bool       __fastcall Sheet_Cell_CompareA3(int Row, int Col);             //
//2017.10.04 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
AnsiString __fastcall Get_SheetGasketMatA3(AnsiString s_Side, HeadsData* pcHeadsData);  // シートガスケット材質取得
//2017.10.04 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
//2019.10.10 温度計保護管追加_S
AnsiString __fastcall Get_Well_Temp(AnsiString Key, AnsiString s_Kikaku,AnsiString s_FrngSize);  // 温度計保護管寸法取得
//2019.10.10 温度計保護管追加_E
// 2022.04.04 機器番号機器名称特記追加_S
extern bool __fastcall Tokki_Code(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2022.04.04 機器番号機器名称特記追加_E
// 2002.04.11 就航船名追加_S
extern AnsiString __fastcall Tokki_Zen(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2022.04.11 就航船名追加_E


