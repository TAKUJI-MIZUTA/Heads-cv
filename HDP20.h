// ---------------------------------------------------------------------------

#ifndef HDP20H
#define HDP20H
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <Filectrl.hpp>

// ---------------------------------------------------------------------------
class TfrmHDP020 : public TForm {
__published: // IDE 管理のコンポーネント
    void __fastcall FormCreate(TObject *Sender);

private: // ユーザー宣言
    void __fastcall Heads_Parts_Main(void);
    bool __fastcall Log_File_SizeCheck(void); // ログファイル　サイズチェック
    bool __fastcall Connect_DataBase(void); // ﾃﾞｰﾀﾍﾞｰｽ接続処理
    bool __fastcall Get_Sakuban(void); // 作番取得
    bool __fastcall OutPut_Folder_Check(AnsiString Check_Pass); // 出力先ﾌｫﾙﾀﾞﾁｪｯｸ
    bool __fastcall Read_CodeMasta_Data(void); // 規格ｺｰﾄﾞ,業界ｺｰﾄﾞ,塗装色 読込

    // EXCEL用
    short __fastcall CallExcel(bool showflg); // Excel起動処理
    void __fastcall QuitExcel(void);

    int prvFrmLoadFlg;

    AnsiString P_SQLText;
    AnsiString P_Sakuban_Mode; // 作番種類(S:製造番号，M:見積番号)
    AnsiString P_UtiKbn; // 打出し区分

public: // ユーザー宣言

    __fastcall TfrmHDP020(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmHDP020 *frmHDP020;
// ---------------------------------------------------------------------------
#endif
