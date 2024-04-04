//---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：承認図表紙作成モジュール  ヘッダファイル
//　 作 成 者：N.Uchida
//　 作 成 日：2002.05.30
//　 更 新 日：2002.09.05
//---------------------------------------------------------------------------

#ifndef Bas_SyoninH
#define Bas_SyoninH
//---------------------------------------------------------------------------
#endif
class HeadsData;

extern bool __fastcall Syonin(HeadsData* pcHeadsData);       // 承認図表紙

extern AnsiString __fastcall JapDayToEngDay(AnsiString);     // 日付変換処理
extern AnsiString __fastcall GetMeibanZuban(void);
extern void __fastcall SyuturyokuZumen_Set(char Pattern,AnsiString Lang, HeadsData* pcHeadsData);

