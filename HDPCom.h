//---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：共通関数用ヘッダファイル
//　 作 成 者：N.Uchida
//　 作 成 日：2002.05.23
//　 更 新 日：2002.06.18
//             2007.10.04 ﾐﾗｰ仕様対応 HeadsDataｸﾗｽ追加、関数に引数追加
//---------------------------------------------------------------------------

#ifndef C_HDPCOMH
#define C_HDPCOMH

#include <dstring.h>
#include <systdate.h>
#include <Grids.hpp>
#define  DefExcelPageLine1  65
#define  DefExcelPageLine   69
//---------------------------------------------------------------
class HeadsData;
//---------------------------------------------------------------
extern void  __fastcall IniFileRead(void);                                 // .iniﾌｧｲﾙ読込用
extern void  __fastcall IniFileRead1(void);                                // .iniﾌｧｲﾙ1読込用
extern void  __fastcall IniFileRead_Parts(void);                           // .iniﾌｧｲﾙ1読込用

extern short __fastcall Numeric_Check(AnsiString,double*,int,int,int);     // 数値範囲ﾁｪｯｸ
extern void  __fastcall SetInputEnabled(TObject*,bool);                    // 入力項目欄設定ﾓｼﾞｭｰﾙ
extern short __fastcall StrErrCheck(AnsiString,short=0);                   // 使用禁止文字ﾁｪｯｸ
extern short __fastcall AllNumChk(char*,short=0);                          // 数字検査処理
extern short __fastcall AllFloatChk(char*,short=0);                        // 小数数字検査処理
extern short __fastcall AllAlphaChk(char*,short=0);                        // 英数字検査処理
extern short __fastcall TextErrCheck(TObject*);                            // 使用禁止文字ﾁｪｯｸ
extern short __fastcall DateCheck(AnsiString,AnsiString*);                 // 日付ﾁｪｯｸ
extern short __fastcall KanmaStr(AnsiString&,int);                         // ｶﾝﾏ文字列取得用
extern short __fastcall AddKanma(TObject*,int);                            // ｶﾝﾏ編集用
extern AnsiString __fastcall TxtEdit(AnsiString,int);                      // ﾃｷｽﾄ長設定
extern AnsiString __fastcall TxtEditRt(AnsiString,int);                    // ﾃｷｽﾄ長設定(後ろ空白削除版)
extern AnsiString __fastcall GetIniKey(AnsiString);                        // .iniﾌｧｲﾙ読込用文字取得
extern AnsiString __fastcall NO_Haifun(AnsiString);                        // ﾊｲﾌﾝ削除用
extern AnsiString __fastcall F_NO_Comma(AnsiString);                       // ｶﾝﾏ削除用
extern AnsiString __fastcall NO_CoronCut(AnsiString);                      // コロンまで取得用
extern AnsiString __fastcall YYtoYYYY_Str(AnsiString,short=1);             // YY→YYYY変換処理
extern short  __fastcall YYtoYYYY(int);                                    // YY→YYYY数値変換処理
extern double __fastcall Hasu_Round(int,double,int);                       // 値補正処理
extern Extended __fastcall Hasu_RoundE(int,Extended,int);                  // 値補正処理(Extended 型)
extern short  __fastcall SetPosCenter(TObject*,TObject*,int=0,int=0);      // ｵﾌﾞｼﾞｪｸﾄ位置中央設定処理

#pragma option push -b-
enum DateKindT  { dsDays, dsSetDay, dsMonth, dsSubYear, dsYear };
#pragma option pop
extern short  __fastcall DateSubChk(AnsiString,int,int=0,int=0,int=1);

extern TDateTime __fastcall DateConv(int,int,int);                            // 日付変換処理
extern TDateTime __fastcall DateCalc(TDateTime,int,int,int);                  // 日付加減算出処理
extern short  __fastcall DecodeYMD(char*,WORD*,WORD*,WORD*);                  // 日付文字列変換処理(Y/M/D)
extern short  __fastcall DecodeMD(char*,WORD*,WORD*);                         // 日付文字列変換処理(M/D)
extern int    __fastcall GetDateSclDays(TDateTime,TDateTime);                 // 範囲日数取得処理
extern int    __fastcall GetMonthDays(int,int);                               // 指定月日数取得処理
extern short  __fastcall ZeroSupress(AnsiString,AnsiString&,int);             // ｾﾞﾛｻﾌﾟﾚｽ用("1"→"01")
extern short  __fastcall HalfKanaConv(AnsiString,AnsiString&,int=0);          // 半角カタカナ変換
extern short  __fastcall WinFileNameChk(char*,short);                         // ファイル名(ﾃﾞｨﾚｸﾄﾘ名)ﾁｪｯｸﾓｼﾞｭｰﾙ
extern double __fastcall StrToDblDef(AnsiString,double=0);                    // 文字列ToDoubleDef=0
extern char* __fastcall GetToken(char *Str, char *Sep);                       // トークン取得処理
extern int __fastcall  ByteChrChk(char a);

extern bool __fastcall Template_Copy(AnsiString F_filename,AnsiString T_filename);  // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙｺﾋﾟｰ
extern AnsiString __fastcall ZenToHan(AnsiString str);                              // 全角文字→半角文字
extern AnsiString HanToZen(AnsiString str);                                         // 半角文字→全角文字

//----------------------------
// 2007.10.04 ﾐﾗｰ仕様対応
extern AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code,bool bWaEi,bool bDocTyp, AnsiString LangCode);
//extern AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code,bool bWaEi,bool bDocTyp);        // 規格ｺｰﾄﾞﾃｰﾌﾞﾙ検索
//extern AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code,bool bWaEi);        // 規格ｺｰﾄﾞﾃｰﾌﾞﾙ検索
//----------------------------

extern AnsiString __fastcall Search_TOSOU_CODE(AnsiString Code, AnsiString Sw);     // 塗装色ﾃｰﾌﾞﾙ検索

extern void __fastcall Excel_Cell_DataSet(int Row, int Col, AnsiString inText);     // EXCEL ｾﾙﾃﾞｰﾀｾｯﾄ
extern AnsiString __fastcall Excel_Cell_DataGet(int Row, int Col);                  // EXCEL ｾﾙﾃﾞｰﾀｹﾞｯﾄ
//Add_Str 2019/02/22 Ri 
extern void __fastcall Excel_Cell_SetUnderline(int Row1, int Col1, int Row2, int Col2);     				// EXCEL 下に罫線
//Add_End 2019/02/22 Ri
extern void __fastcall Excel_HeaderSet(bool=false);
extern void __fastcall Excel_Cell_Delete(int iLastPage);
extern bool __fastcall Write_Log(AnsiString LogText);             // ﾛｸﾞﾌｧｲﾙ作成
extern bool __fastcall Write_Error_Log(AnsiString LogText);       // ｴﾗｰﾛｸﾞﾌｧｲﾙ作成
extern void __fastcall Excel_Err_DataSet(AnsiString inText);      // EXCEL用 ｴﾗｰﾛｸﾞﾃﾞｰﾀｾｯﾄ

extern bool __fastcall Read_HEADS_Data(AnsiString filepass);      // HEADSデータ読込処理

//2004/10/26 add
extern bool __fastcall Read_HEADS_Data_Rev(AnsiString filepass);  // HEADSデータリビジョン読込処理
//2004/10/26 add

// 2002.08.03 Add
int __fastcall Syukei_Buhin_Yobisu( AnsiString a_Katasiki, AnsiString a_Zaisitu, HeadsData* pcHeadsData);   // 予備品数ﾃﾞｰﾀ集計処理
AnsiString __fastcall Search_HD_MATERIAL(AnsiString Key);            // 材質ｺｰﾄﾞ検索
AnsiString __fastcall Search_HD_E_MATERIAL(AnsiString Key);            // 材質ｺｰﾄﾞ検索
AnsiString __fastcall Search_HD_MAT_JtoE(AnsiString Key);
AnsiString __fastcall Chg_Zaisitu_Code(AnsiString Code,AnsiString Typ );
AnsiString __fastcall Search_HD_FRNG_MST(AnsiString Key, AnsiString FSize, AnsiString SEDiv, AnsiString PG, AnsiString TMP, AnsiString Drn, AnsiString Air, bool bRing);
extern AnsiString __fastcall Get_Yobi_Code(AnsiString Key);
// 2016.08.25 材質ｺｰﾄﾞ変更
AnsiString __fastcall Type_MAT(AnsiString Key);            // 材質種類の取得
// ***********************
// 2020.03.31 乾燥剤量追加_S
AnsiString __fastcall Get_Silicagel(AnsiString Key);       // フランジシリカゲル量取得
// 2020.03.31 乾燥剤量追加_E
// 2020.03.31 温水乾燥注記追加_S
AnsiString __fastcall Get_HotAirDrying(void);              // 温水乾燥注記取得
// 2020.03.31 温水乾燥注記追加_E
// 2024.02.06 ALLOY材質変換_S
AnsiString __fastcall Alloy_MAT(AnsiString Key);           // ALLOY材からJIS材への変換
// 2024.02.06 ALLOY材質変換_E

//----------------------------
// 2007.10.04 ﾐﾗｰ仕様対応
//extern bool __fastcall Get_PHE_Model(AnsiString *s_Syurui, AnsiString *s_Model);
extern bool __fastcall Get_PHE_Model(AnsiString *rs_Syurui, AnsiString *rs_Model, AnsiString s_Syurui, AnsiString s_Model);
//----------------------------

extern int        __fastcall  GetNzKikaku( AnsiString a_KCd );
extern AnsiString __fastcall Get_Sakuban_File(void);
extern AnsiString __fastcall ChgIDFSize(double dSize);

//2022/12/15 桁数追加対応 Add_S
extern AnsiString __fastcall GetSeizouBangou(AnsiString str);
//2022/12/15 桁数追加対応 Add_E

//2023/4/21 桁数追加対応 Add_S
extern AnsiString __fastcall GetFileSeizouBangou(AnsiString str);
//2023/4/21 桁数追加対応 Add_E
#endif

// ---------------- <<< End of File >>> ---------------- //

