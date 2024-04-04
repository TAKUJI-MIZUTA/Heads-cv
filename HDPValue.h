// ----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：変数定義
// 作 成 者：N.Uchida
// 作 成 日：2002.05.28
// 更 新 日：2002.06.18 2007.07
// ---------------------------------------------------------------------------

#ifndef C_HDPVALUEH
#define C_HDPVALUEH

#include "HDPConst.h"   // 定数定義用

#ifdef  VDEF
#define   EXTERN
#else
#define   EXTERN  extern
#endif

/* ---*---*--- HD_CV.iniﾌｧｲﾙ用変数 ---*---*--- */
EXTERN AnsiString G_HD_Version; // HEADSﾊﾞｰｼﾞｮﾝ
EXTERN AnsiString G_Zuban_DB; // 図番ﾃﾞｰﾀﾍﾞｰｽﾌｧｲﾙ名
EXTERN AnsiString G_Material_DB; // 材質ﾃﾞｰﾀﾍﾞｰｽﾌｧｲﾙ名
EXTERN AnsiString G_Inspect_DB; // 検査要領書ﾃﾞｰﾀﾍﾞｰｽﾌｧｲﾙ名
EXTERN int G_Output_Mode; // 出力ﾓｰﾄﾞ
//Add_Str 2019/02/19 Ri
//製作指示書A4(旧)、A3(新)フラグの取得
EXTERN int G_OLD_FLG;     // 0:新(A3)、1:旧(A4)、2:新旧両方
//Add_End 2019/02/19 Ri
//2022.09.30 Eﾌﾚｰﾑﾍﾞｰｽ追加_S
EXTERN int G_EBASE_FLG;   // 0:無、1:有
//2022.09.30 Eﾌﾚｰﾑﾍﾞｰｽ追加_E
EXTERN AnsiString G_HD_Root; // ﾙｰﾄﾌｫﾙﾀﾞ
EXTERN AnsiString G_HEADS_CV_Dir; // ｼｽﾃﾑﾌｫﾙﾀﾞ
EXTERN AnsiString G_MASTER_Dir; // ﾏｽﾀﾌｫﾙﾀﾞ
EXTERN AnsiString G_TEMPLATE_Dir; // ﾃﾝﾌﾟﾚｰﾄﾌｫﾙﾀﾞ

EXTERN AnsiString G_HEADS_INP_Dir; // HEADS入力ﾌｫﾙﾀﾞ
EXTERN AnsiString G_TRIGGER_Dir; // ﾄﾘｶﾞﾌｫﾙﾀﾞ
EXTERN AnsiString G_CSV_Dir; // CSVﾌｫﾙﾀﾞ
EXTERN AnsiString G_MATERIAL_Dir; // 材質ｺｰﾄﾞﾏｽﾀﾌｫﾙﾀﾞ

EXTERN AnsiString G_HEADS_OUT_Dir; // HEADS出力ﾌｫﾙﾀﾞ
EXTERN AnsiString G_OUT_ORDER_Dir; // ｵｰﾀﾞ図面ﾌｫﾙﾀﾞ
EXTERN AnsiString G_OUT_ESTIM_Dir; // 見積図面ﾌｫﾙﾀﾞ

EXTERN AnsiString G_HEADS_DATA_Dir; // HEADS管理ﾃﾞｰﾀ出力ﾌｫﾙﾀﾞ
EXTERN AnsiString G_DAT_ORDER_Dir; // ｵｰﾀﾞ図面管理ﾃﾞｰﾀﾌｫﾙﾀﾞ
EXTERN AnsiString G_DAT_ESTIM_Dir; // 見積図面管理ﾃﾞｰﾀﾌｫﾙﾀﾞ

// 2004.09.23追加
EXTERN AnsiString G_OUT_DIRECT_Dir; // マル標出力ﾃﾞｰﾀﾌｫﾙﾀﾞ

// 2004.10.22追加
EXTERN int G_HEADS_DATA_Rev; // 要求ファイルリビジョン

// 2004.11.03追加
EXTERN bool bRetRev; // 既存に対応するためのフラグ

EXTERN AnsiString G_Log_File_Dir; // ﾛｸﾞﾌｧｲﾙ     ﾌｫﾙﾀﾞ
EXTERN AnsiString G_Rsrv_Log_Dir; // 過去ﾛｸﾞﾌｧｲﾙ ﾌｫﾙﾀﾞ
EXTERN double G_Log_Max_Size; // ﾛｸﾞﾌｧｲﾙ最大ｻｲｽﾞ(bytes)

EXTERN AnsiString G_ErrLog_File_Dir; // ｴﾗｰﾛｸﾞﾌｧｲﾙ     ﾌｫﾙﾀﾞ
EXTERN AnsiString G_Rsrv_ErrLog_Dir; // 過去ｴﾗｰﾛｸﾞﾌｧｲﾙ ﾌｫﾙﾀﾞ
EXTERN double G_ErrLog_Max_Size; // ｴﾗｰﾛｸﾞﾌｧｲﾙ最大ｻｲｽﾞ(bytes)

EXTERN int G_Log_Write_Flg; // ﾛｸﾞ書き込みﾌﾗｸﾞ(0:書く,1:書かない)

// Windows 2008Server 移植対応
EXTERN AnsiString G_Err_Log_FileName; // 未登録の型式ファイル名
EXTERN AnsiString G_Err_Log_FileFName; // 未登録の型式フルパス

/* ---*---*--- HD_PARTS.iniﾌｧｲﾙ用変数 ---*---*--- */
EXTERN AnsiString G_AnaakeFileNM; // 孔明ﾌｧｲﾙ名称
EXTERN AnsiString G_YobihinFileNM; // 予備品ﾌｧｲﾙ名称

EXTERN AnsiString G_AnaFilePass; // 孔明ﾌｧｲﾙﾊﾟｽ(1階層上まで)
// EXTERN   AnsiString G_YobiFilePass;       // 予備数ﾌｧｲﾙﾊﾟｽ

/* ---*---*--- 環境用使用変数 ---*---*--- */
EXTERN AnsiString G_LogFilePath; // ﾌﾙﾊﾟｽ . ﾛｸﾞﾌｧｲﾙ
EXTERN AnsiString G_ErrLogFilePath; // . ｴﾗｰﾛｸﾞﾌｧｲﾙ

EXTERN AnsiString G_Template_Dir; // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ格納先ﾌｫﾙﾀﾞ
EXTERN AnsiString G_Sakuban_Dir; // 作番ﾌｫﾙﾀﾞ
EXTERN AnsiString G_xls_Output_Dir; // ｴｸｾﾙｼｰﾄ出力先ﾌｫﾙﾀﾞ
EXTERN AnsiString G_csv_Output_Dir; // CSVﾌｧｲﾙ出力先ﾌｫﾙﾀﾞ
EXTERN AnsiString G_Input_Csv_Dir; // HEADS入力ﾌｫﾙﾀﾞ + CSVﾌｫﾙﾀﾞ
EXTERN AnsiString G_Input_Txt_Dir; // 02.08.07追加
// HEADS入力ﾌｫﾙﾀﾞ + TXTﾌｫﾙﾀﾞ
// 2004/10/25 add
EXTERN AnsiString G_xls_Output_Dir_old; // ｴｸｾﾙｼｰﾄ出力先ﾌｫﾙﾀﾞ前回
// 2004/10/25 add

EXTERN AnsiString G_HEADS_ID; // ﾍｯﾂﾞﾃﾞｰﾀ ﾌｧｲﾙID
EXTERN AnsiString G_SAKUBAN; // 作番
EXTERN AnsiString G_HEADS_DATA[1500]; // HEADSデータ

// 2017.11.16 規格コード追加_S
//EXTERN AnsiString G_KIKAKU[30][5]; // 規格ｺｰﾄﾞ
EXTERN AnsiString G_KIKAKU[40][5]; // 規格ｺｰﾄﾞ
// 2017.11.16 規格コード追加_E
EXTERN long G_KIKAKU_RecCnt; // 規格ｺｰﾄﾞ 件数
EXTERN AnsiString G_GYOKAI[30][2]; // 業界
EXTERN long G_GYOKAI_RecCnt; // 業界 件数
EXTERN AnsiString G_TOSOU[20][3]; // 塗装色
EXTERN long G_TOSOU_RecCnt; // 業界 件数
// 2015.01.16 ノズル色変更
EXTERN AnsiString G_BOOTCL[300][3]; // ブーツ色ﾞ
EXTERN long G_BOOTCL_RecCnt; // ブーツ色 件数
// ***********************
// 2016.09.14 ステッカー追加
EXTERN AnsiString G_STACKER[300][3]; // ステッカー
EXTERN long G_STACKER_RecCnt; // ステッカー 件数
// *************************

// 2019.07.17 ﾌﾟﾚｰﾄ変換ﾃｰﾌﾞﾙ件数変更_S
//// 2004.04.22 ﾌﾟﾚｰﾄ変換ﾃｰﾌﾞﾙ件数変更
//// EXTERN   AnsiString G_PHENKAN[100][9];    // ﾌﾟﾚｰﾄ変換ﾃｰﾌﾞﾙ
//EXTERN AnsiString G_PHENKAN[200][9]; // ﾌﾟﾚｰﾄ変換ﾃｰﾌﾞﾙ
EXTERN AnsiString G_PHENKAN[300][9]; // ﾌﾟﾚｰﾄ変換ﾃｰﾌﾞﾙ
// 2019.07.17 ﾌﾟﾚｰﾄ変換ﾃｰﾌﾞﾙ件数変更_E
EXTERN long G_PHENKAN_RecCnt; // ﾌﾟﾚｰﾄ変換ﾃｰﾌﾞﾙ件数

EXTERN AnsiString G_NOZZLECD[100][6]; // ﾉｽﾞﾙ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ
EXTERN long G_NOZZLECD_RecCnt; // ﾉｽﾞﾙ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ件数

EXTERN AnsiString G_TEMPCD[100][5]; // 温度計構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ
EXTERN long G_TEMPCD_RecCnt; // 温度計構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ件数

EXTERN AnsiString G_PRESSCD[100][4]; // 圧力計構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ
EXTERN long G_PRESSCD_RecCnt; // 圧力計構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ件数

EXTERN AnsiString G_K_Syurui; // 型式種類
EXTERN AnsiString G_K_Model1; // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式1)
EXTERN AnsiString G_K_Model2; // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式2)
EXTERN AnsiString G_KS_Syurui; // 総称種類
EXTERN AnsiString G_KS_Model; // 総称ﾓﾃﾞﾙ
EXTERN AnsiString G_Model_Zen1; // 型式ﾓﾃﾞﾙ全角1
EXTERN AnsiString G_Model_Zen2; // 型式ﾓﾃﾞﾙ全角2

EXTERN AnsiString G_Log_Text; // ﾛｸﾞﾃｷｽﾄ(汎用)
EXTERN AnsiString G_ErrLog_Text; // ｴﾗｰﾛｸﾞﾃｷｽﾄ(汎用)

// ↓2002/10/23
EXTERN AnsiString G_D_GsktType;
EXTERN AnsiString G_D_GsktAna;
EXTERN AnsiString G_D_GsktABKubun;
EXTERN AnsiString G_E_GsktType;
EXTERN AnsiString G_E_GsktAna;
EXTERN AnsiString G_E_GsktABKubun;
EXTERN AnsiString G_S_Model;
EXTERN AnsiString G_FrmType;
EXTERN AnsiString G_GsktType;
EXTERN AnsiString G_FrmDiv;
// ↑2002/10/23

// Excel用変数
EXTERN Variant exApplication; // ｱﾌﾟﾘｹｰｼｮﾝｵﾌﾞｼﾞｪｸﾄ
EXTERN Variant exWorkbooks; // ﾜｰｸﾌﾞｯｸｺﾚｸｼｮﾝ
EXTERN Variant exWorkbook; // ﾜｰｸﾌﾞｯｸｵﾌﾞｼﾞｪｸﾄ
EXTERN Variant exWorksheets; // ﾜｰｸｼｰﾄｺﾚｸｼｮﾝ
EXTERN Variant exWorksheet; // ﾜｰｸｼｰﾄｵﾌﾞｼﾞｪｸﾄ
EXTERN Variant exWorkCell; // ｾﾙ用
EXTERN Variant vRange; // ﾚﾝｼﾞｵﾌﾞｼﾞｪｸﾄ用

EXTERN Variant exWorkbook_old; // ﾜｰｸﾌﾞｯｸｵﾌﾞｼﾞｪｸﾄ  (旧ﾘﾋﾞｼﾞｮﾝ用)
EXTERN Variant exWorksheets_old; // ﾜｰｸｼｰﾄｺﾚｸｼｮﾝ     (旧ﾘﾋﾞｼﾞｮﾝ用)
EXTERN Variant exWorksheet_old; // ﾜｰｸｼｰﾄｵﾌﾞｼﾞｪｸﾄ   (旧ﾘﾋﾞｼﾞｮﾝ用)
EXTERN Variant exWorkCell_old; // ｾﾙ用             (旧ﾘﾋﾞｼﾞｮﾝ用)

EXTERN AnsiString G_YOBICHK_NAME[100]; // 予備品重複計上防止用メモ
EXTERN AnsiString G_YOBICHK_MAT[100]; // 予備品重複計上防止用メモ
EXTERN long G_YOBICHK_RecCnt; // 予備品重複計上防止用カウンタ

// ↓2007/03
EXTERN bool bCopySeisekishoFlag; // 成績書をコピーするか否かのフラグ
// ↑2007/03

// 2014/08/07 応用技術追加
EXTERN AnsiString G_Language;    // 言語識別文字列("JP" = 日本語, "EN" = 英語)

// 2019/05/17 応用技術追加
EXTERN bool bIsNotKatashiki; 	// 未登録型式かどうかのフラグ
EXTERN bool bMirrorFlag; 	    // ミラー仕様かどうかのフラグ
EXTERN bool bHasOldDataFlag; 	// 旧データがあるかどうかのフラグ
EXTERN AnsiString gP_Sakuban_Mode;    // 作番種類(S:製造番号，M:見積番号)

#endif
// ---------------- <<< End of File >>> ---------------- //
