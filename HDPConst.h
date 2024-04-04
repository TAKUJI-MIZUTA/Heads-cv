// ----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：定数定義
// 作 成 者：N.Uchida
// 作 成 日：2002.05.23
// 更 新 日：2002.06.18 2007.03
// ---------------------------------------------------------------------------
#ifndef C_HDPCONSTH
#define C_HDPCONSTH

/* ---*---*--- システム名称 ---*---*--- */
#define CSYSNAME   " 出図要求受信サービスシステム "
#define CSYSVER    " Ver.1.00 "

/* ---*---*--- ファイル名称 ---*---*--- */
#define CSYSINIFILENAME       "HD_CV.INI"            // 環境設定ファイル名
#define CSYSINIFILENAME_P     "HD_PARTS.INI"         // 環境設定ファイル名(部品展開用)
#define CSYSLOGFILENAME       "Log_Excel.log"        // ログファイル名(Excel作成処理)
#define CSYSLOGFILENAME_P     "Log_Parts.log"        // ログファイル名(部品展開処理)
#define CSYSERRLOGFILENAME    "Log_Excel_Err.log"    // エラーログファイル名(Excel作成処理)
#define CSYSERRLOGFILENAME_P  "Log_Parts_Err.log"    // エラーログファイル名(部品展開処理)
// 2021.06.18 FireDACへの移行 MOD_S
#define CSYSCONNDEFFILENAME   "FDConnectionDefs.ini" // 接続定義ファイル名
// 2021.06.18 FireDACへの移行 MOD_E

/* ---*---*--- ＤＢ用接続名称 ---*---*--- */
#define ODBC_DSN_DNO      "DSN_DNO"                // 図番データベース(HD_DNO.mdb)
#define ODBC_DSN_INS      "DSN_INSPCTL"            // 検査要領書データベース(HD_INSPCTL.mdb)
#define ODBC_DSN_MAT      "DSN_MATERIAL"           // 材質データベース(HD_MATERIAL.mdb)

/* ---*---*--- EXCEL 固定シート名 ---*---*--- */
#define EXCEL_ERR_SHEETNAME    "エラーログ"        // EXCELシート名(エラーログ用)

// --> 2007.03 Add
/* ---*---*--- 検査成績書 規格コード ---*---*--- */
#define SEISEKISYO_STANDARDNO_DNV        12                   // 検査成績書DNV
#define SEISEKISYO_STANDARDNO_ABS        13                   // 検査成績書ABS
#define SEISEKISYO_STANDARDNO_LR         14                   // 検査成績書LR
#define SEISEKISYO_STANDARDNO_NK         15                   // 検査成績書NK
#define SEISEKISYO_STANDARDNO_GL         16                   // 検査成績書GL
#define SEISEKISYO_STANDARDNO_KR         17                   // 検査成績書KR
#define SEISEKISYO_STANDARDNO_BV         24                   // 検査成績書BV
#define SEISEKISYO_STANDARDNO_CCS        27                   // 検査成績書CCS
// <-- 2007.03 Add

#endif
/* --*-- end --*-- */
