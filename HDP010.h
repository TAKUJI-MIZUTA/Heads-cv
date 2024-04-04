//---------------------------------------------------------------------------

#ifndef HDP010H
#define HDP010H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
// 2021.06.18 FireDACへの移行 DEL_S
//#include <DBTables.hpp>
// 2021.06.18 FireDACへの移行 DEL_E
#include <Grids.hpp>

#include <Filectrl.hpp>

//---------------------------------------------------------------------------
class TfrmHDP010 : public TForm
{
__published:	// IDE 管理のコンポーネント
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormPaint(TObject *Sender);
   void __fastcall FormActivate(TObject *Sender);

private:	   // ユーザー宣言
   bool __fastcall Log_File_SizeCheck(void);                      // ログファイル　サイズチェック
   bool __fastcall Connect_DataBase(void);                        // ﾃﾞｰﾀﾍﾞｰｽ接続処理
   bool __fastcall Get_Sakuban(void);                             // 作番取得
   bool __fastcall OutPut_Folder_Check(AnsiString Check_Pass);    // 出力先ﾌｫﾙﾀﾞﾁｪｯｸ
   bool __fastcall Read_CodeMasta_Data(void);                     // 規格ｺｰﾄﾞ,業界ｺｰﾄﾞ,塗装色 読込
   //2009.03.03 圧力容器強度計算変更
   bool __fastcall Aturyoku_Run(void);                            // 圧力容器強度計算書作成ﾌﾟﾛｸﾞﾗﾑ  起動
   //*******************************
   //2017.10.19 塗装自主検査票_S
   bool __fastcall TosoJisyuKensa_Run(void);                      // 塗装自主検査票作成ﾌﾟﾛｸﾞﾗﾑ  起動
   //2017.10.19 塗装自主検査票_E
   //2020.04.20 温風乾燥出力_S
   bool __fastcall HotAirDrying_Run(void);                        // 温風乾燥出力ﾌﾟﾛｸﾞﾗﾑ  起動
   //2020.04.20 温風乾燥出力_E

   // EXCEL用
   short __fastcall CallExcel(bool showflg);                      // Excel起動処理
   void  __fastcall QuitExcel(void);

   void __fastcall Heads_Excel_Main(void);
   void __fastcall Excel_Run(void);

   // 2004.09.23 追加
   void __fastcall MovePDFFile(void);                             // PDFﾌｧｲﾙの出力
   void __fastcall OutPut_File_Move(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg);       // ﾌｧｲﾙの検索、ｺﾋﾟｰ
   void __fastcall MoveFolder_Job(void);                          // フォルダの移動
   bool __fastcall MoveDirectoryEvo(AnsiString FromFile, AnsiString ToFile);       //2004/11/02
   bool __fastcall TfrmHDP010::removeDirectoryEvo(AnsiString fileName);            //2004/11/03

   int prvFrmLoadFlg;

   void __fastcall MoveFolder_Job1(void);                          // フォルダの移動
   void __fastcall OutPut_File_Move1(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg);       // ﾌｧｲﾙの検索、ｺﾋﾟｰ
   bool __fastcall MoveDirectoryEvo1(AnsiString FromFile, AnsiString ToFile);       //2004/11/02

   bool __fastcall MoveDirectoryEvoRev(AnsiString FromFile, AnsiString ToFile, AnsiString sRev);       //2015/10/16
   void __fastcall OutPut_File_MoveRev(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg, AnsiString sRev);       // ﾌｧｲﾙの検索、ｺﾋﾟｰ

   AnsiString P_SQLText;
   AnsiString P_Sakuban_Mode;    // 作番種類(S:製造番号，M:見積番号)
   AnsiString P_UtiKbn;          // 打出し区分

   //↓2007.10.9 ミラー仕様対応
   HeadsData*  m_pcHeadsData;
   AnsiString P_HEADS_DATA[1500];
   //↑2007.10.9 ミラー仕様対応

public:		// ユーザー宣言
   __fastcall TfrmHDP010(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmHDP010 *frmHDP010;
//---------------------------------------------------------------------------
#endif
