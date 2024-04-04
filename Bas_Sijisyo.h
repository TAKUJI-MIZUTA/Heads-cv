//---------------------------------------------------------------------------

#ifndef Bas_SijisyoH
#define Bas_SijisyoH
//---------------------------------------------------------------------------
#endif

extern bool __fastcall Sijisyo(HeadsData* pcHeadsData);          // 製作指示書

AnsiString __fastcall Sheet_Exist_Check(AnsiString path, HeadsData* pcHeadsData); // 過去Excelｼｰﾄ(製作指示書)存在ﾁｪｯｸ
AnsiString __fastcall Get_Zaisitu_Name(AnsiString Code);          // 材質ｺｰﾄﾞﾏｽﾀ検索
AnsiString __fastcall Chg_Zaisitu_Code(AnsiString Code,AnsiString Typ ); //材質コード変換マスタ検索
AnsiString __fastcall Get_AturyokuKei(AnsiString s_Code);         // 圧力計種類取得

AnsiString __fastcall Get_SiageMeisyou(AnsiString Code);          // 仕上名称　取得
AnsiString __fastcall Get_Tankan_Data(AnsiString Syurui, HeadsData* pcHeadsData);   // 端管種類　取得
AnsiString __fastcall Get_Flange_Data(AnsiString Tugite);         // ﾌﾗﾝｼﾞ種類取得
bool       __fastcall Get_Start_Plate(AnsiString &Side_B, AnsiString &Side_A, HeadsData* pcHeadsData);
AnsiString __fastcall Get_T_ZaZaisitu(AnsiString TanSyurui, AnsiString TanName, HeadsData* pcHeadsData);

void       __fastcall Sheet_Compare_Syori(AnsiString Excel_OldPass);    //
bool       __fastcall Sheet_Cell_Compare(int Row, int Col);             //
//2017.10.04 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
AnsiString __fastcall Get_SheetGasketMat(AnsiString s_Side, HeadsData* pcHeadsData);  // シートガスケット材質取得
//2017.10.04 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E


