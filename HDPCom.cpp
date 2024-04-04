// ----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：共通関数用
// 作 成 者：N.Uchida
// 作 成 日：2002.05.23
// 更 新 日：2002.06.19
// 2007.10.04 ﾐﾗｰ仕様対応　Search_KIKAKU_CODE、Syukei_Buhin_Yobisuに引数追加
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>

#pragma hdrstop

#include <math.h>
#include <direct.h>
#include <sys\stat.h>
#include <mbstring.h>

#ifndef LSI_C   /* Borland C++ 5.5 */
#include <mbstring.h>
#define CT_ANK          _MBC_SINGLE
#define CT_KJ1          _MBC_LEAD
#define CT_KJ2          _MBC_TRAIL
#define CT_ILGL         _MBC_ILLEGAL
#define chkctype(x, y)  _mbbtype(x, y)
#define iskana(x)       _ismbbkana(x)
#else           /* LSI C-86 */
#include <jctype.h>
#endif

#include <SysUtils.hpp>
#include <Filectrl.hpp>
#include <IniFiles.hpp>
#include <Registry.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "HeadsData.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
TColor G_ClNoEditColor = clLtGray;

// ---------------------------------------------------------------------------
// 日本語関数名：ﾃﾞｨﾚｸﾄﾘﾌﾙﾊﾟｽ変換処理
// 概  要      ：INI内設定ﾃﾞｨﾚｸﾄﾘを ﾌﾙﾊﾟｽに変換する
// 引  数      ：curdir  カレントディレクトリ
// dir     設定ディレクトリ
// 戻り値      ：なし
// 備  考      ：
// ---------------------------------------------------------------------------
static AnsiString __fastcall IniDirToFullPath(AnsiString curdir, AnsiString dir)
{
    AnsiString WrkCurPath;
    AnsiString WrkSetPath;
    AnsiString DriveStr;
    int len;

    len = dir.Length();
    WrkCurPath = IncludeTrailingBackslash(curdir);
    WrkSetPath = WrkCurPath + dir;
    DriveStr = ExtractFileDrive(dir);
    if (len > 1) {
        if (dir[2] == ':') // ドライブ指定あり？
        {
            WrkSetPath = dir;
        }
        else if (dir[1] == '\\') // １階層上からの相対パス？
        {
            if (dir[2] == '\\') // 絶対パス？
            {
                if (DriveStr.Length() >= len) {
                    WrkSetPath = ExtractFileDrive(WrkCurPath) + ":" +
                        dir.SubString(2, len - 1);
                }
                else {
                    WrkSetPath = dir;
                }
            }
            else // １階層上からの相対パス？
            {
                WrkSetPath = ExtractFileDir(ExtractFileDir(WrkCurPath)) + dir;
            }
        }
    }
    return IncludeTrailingBackslash(WrkSetPath);
}

// ---------------------------------------------------------------------------
// 日本語関数名：IniFileの読み込み処理
// 概  要      ：INIﾌｧｲﾙ読み込み、環境値を セットする
// 引  数      ：なし
// 戻り値      ：なし
// 備  考      ：
// ---------------------------------------------------------------------------
void __fastcall IniFileRead(void) {
    AnsiString AExeDir; // 実行ﾌｧｲﾙのディレクトリ
    AnsiString AExeName;
    try {
        IniFileRead1(); // IniFile1の読み込み処理
    }
    __finally {
        // 実行中ﾌｧｲﾙのディレクトリ
        AExeDir = ExtractFileDir(Application->ExeName);
        // 実行中ﾌｧｲﾙのディレクトリ('\\'なし)

        // --- 環境変数に展開
        AExeName = ExtractFileName(Application->ExeName);

        // ﾛｸﾞﾌｧｲﾙ(ﾌﾙﾊﾟｽ)，ｴﾗｰﾛｸﾞﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
        if (AExeName == "HD_PARTS.exe") {
            // 部品展開処理
            G_LogFilePath = IncludeTrailingBackslash(G_Log_File_Dir) +
                CSYSLOGFILENAME_P;
            G_ErrLogFilePath = IncludeTrailingBackslash(G_ErrLog_File_Dir) +
                CSYSERRLOGFILENAME_P;
        }
        else {
            // Excel作成処理
            G_LogFilePath = IncludeTrailingBackslash(G_Log_File_Dir) +
                CSYSLOGFILENAME;
            G_ErrLogFilePath = IncludeTrailingBackslash(G_ErrLog_File_Dir) +
                CSYSERRLOGFILENAME;
        }

        // ﾃﾝﾌﾟﾚｰﾄﾌｫﾙﾀﾞ
        G_Template_Dir = IncludeTrailingBackslash(G_HD_Root) +
            IncludeTrailingBackslash(G_HEADS_CV_Dir) + IncludeTrailingBackslash
            (G_TEMPLATE_Dir);

        // HEADS入力ﾌｫﾙﾀﾞ + CSVﾌｫﾙﾀﾞ(HEADSﾃﾞｰﾀが格納されている。)
        G_Input_Csv_Dir = IncludeTrailingBackslash(G_HD_Root) +
            IncludeTrailingBackslash(G_HEADS_CV_Dir) + IncludeTrailingBackslash
            (G_MASTER_Dir);

        // HEADS出力先ﾌｫﾙﾀﾞ (※ 後の処理でｵｰﾀﾞｰか見積かを判断し、ﾌｫﾙﾀﾞを付与する。)
        // TfrmHDP010::FormActivateｲﾍﾞﾝﾄ内の処理で取得
        // マルチサーバ対応 2008/11/17 MOD START
        // G_xls_Output_Dir = IncludeTrailingBackslash(G_HD_Root)
        // + IncludeTrailingBackslash(G_HEADS_OUT_Dir);
        G_xls_Output_Dir = IncludeTrailingBackslash(G_HEADS_OUT_Dir);
        // マルチサーバ対応 2008/11/17 MOD END

        // マルチサーバ対応 2009/1/13 MOD START
        // G_csv_Output_Dir = IncludeTrailingBackslash(G_HD_Root)
        // + IncludeTrailingBackslash(G_HEADS_DATA_Dir);
        G_csv_Output_Dir = IncludeTrailingBackslash(G_HEADS_DATA_Dir);
        // マルチサーバ対応 2009/1/13 MOD END

    }
}

// ---------------------------------------------------------------------------
// 日本語関数名：IniFile1の読み込み処理
// 概  要      ：INIﾌｧｲﾙ1読み込み
// 引  数      ：なし
// 戻り値      ：なし
// 備  考      ：環境設定ファイル = HD_CV.INI
// ---------------------------------------------------------------------------
void __fastcall IniFileRead1(void) {
    TIniFile *ini; // INIﾌｧｲﾙｱｸｾｽ ｵﾌﾞｼﾞｪｸﾄ
    AnsiString AIniPath; // 設定ﾌｧｲﾙﾊﾟｽ
    AnsiString AIniFile; // 設定ﾌｧｲﾙﾊﾟｽ + ﾌｧｲﾙ名
    AnsiString ASection; // ｾｸｼｮﾝ名
    AnsiString Awrk, Asub; // 作業用
    AnsiString Amm, Add; // 作業用 m, d

    char inipath[MAX_PATH];

//    // 環境iniファイルのディレクトリ
//    GetSystemDirectory(inipath, sizeof(inipath));
//    AIniPath = inipath;

        AIniPath = ExtractFilePath(Application->ExeName);
        //AIniPath = "C:\\HEADS_ROOT01\\HEADS-CV\\";

    // 環境iniファイルのへのフルパスファイル名
    AIniFile = AIniPath + CSYSINIFILENAME;

    // iniファイルから設定内容を読み込む
    ini = new TIniFile(AIniFile);

    // --- 基本情報取得
    ASection = "GENERAL"; // --  読み込みｾｸｼｮﾝｾｯﾄ : 基本
    G_HD_Version = GetIniKey(ini->ReadString(ASection, "HD_VERSION", ""));
    // HEADSﾊﾞｰｼﾞｮﾝ
    G_Zuban_DB = GetIniKey(ini->ReadString(ASection, "ZUBAN_DBFILE", ""));
    // 図番ﾃﾞｰﾀﾍﾞｰｽﾌｧｲﾙ名
    G_Material_DB = GetIniKey(ini->ReadString(ASection, "MAT_CODE_FILE",
        "")); // 材質ﾃﾞｰﾀﾍﾞｰｽﾌｧｲﾙ名
    G_Inspect_DB = GetIniKey(ini->ReadString(ASection, "INSPECTDB", ""));
    // 検査要領書ﾃﾞｰﾀﾍﾞｰｽﾌｧｲﾙ名
//Add_Str 2019/02/19 Ri
        // 製作指示書A4(旧)、A3(新)フラグの取得
        G_OLD_FLG = GetIniKey(ini->ReadString(ASection, "OLD_FLG", "")).ToIntDef(0);
//Add_End 2019/02/19 Ri
    // @@@@@@@@@@@@@@
    // 02.08.17追加
    // @@@@@@@@@@@@@@
    G_Output_Mode = GetIniKey(ini->ReadString(ASection, "DOC_OUTPUT_MODE", ""))
        .ToIntDef(0); // 0:打ち出し区分,1:テスト用
    //2022.09.30 Eﾌﾚｰﾑﾍﾞｰｽ追加_S
    G_EBASE_FLG = GetIniKey(ini->ReadString(ASection, "EFRAME_BASE_PLATE", ""))
        .ToIntDef(0); // 0:無,1:有
    //2022.09.30 Eﾌﾚｰﾑﾍﾞｰｽ追加_E

    // --- ログ情報取得
    ASection = "LOG";
    // ﾛｸﾞﾌｧｲﾙ ﾌｫﾙﾀﾞ
    G_Log_File_Dir = GetIniKey(ini->ReadString(ASection, "LOG_FILE_DIR", ""));
    // 過去ﾛｸﾞﾌｧｲﾙ ﾌｫﾙﾀﾞ
    G_Rsrv_Log_Dir =
        GetIniKey(ini->ReadString(ASection, "RESERVE_LOG_DIR", ""));
    // ﾛｸﾞﾌｧｲﾙ最大ｻｲｽﾞ(bytes)
    G_Log_Max_Size =
        StrToCurr(GetIniKey(ini->ReadString(ASection, "LOG_MAX_SIZE", "0")));
    // ｴﾗｰﾛｸﾞﾌｧｲﾙ ﾌｫﾙﾀﾞ
    G_ErrLog_File_Dir =
        GetIniKey(ini->ReadString(ASection, "ERR_LOG_FILE_DIR", ""));
    // 過去ｴﾗｰﾛｸﾞﾌｧｲﾙ ﾌｫﾙﾀﾞ
    G_Rsrv_ErrLog_Dir =
        GetIniKey(ini->ReadString(ASection, "RESERVE_ERR_LOG_DIR", ""));
    // ｴﾗｰﾛｸﾞﾌｧｲﾙ最大ｻｲｽﾞ(bytes)
    G_ErrLog_Max_Size =
        StrToCurr(GetIniKey(ini->ReadString(ASection, "ERR_LOG_MAX_SIZE",
        "0")));
    // EXCEL処理内容ﾛｸﾞ　出力制御ﾌﾗｸﾞ
    G_Log_Write_Flg =
        StrToInt(GetIniKey(ini->ReadString(ASection, "EXCEL_LOG_WRITE", "0")));

    // --- フォルダ情報取得
    ASection = "FOLDER";
    G_HD_Root = GetIniKey(ini->ReadString(ASection, "HD_ROOT", "")); // ﾙｰﾄﾌｫﾙﾀﾞ
    G_HEADS_CV_Dir = GetIniKey(ini->ReadString(ASection, "HEADS_CV", ""));
    // ｼｽﾃﾑﾌｫﾙﾀﾞ
    G_MASTER_Dir = GetIniKey(ini->ReadString(ASection, "MASTER", ""));
    // ﾏｽﾀﾌｫﾙﾀﾞ
    G_TEMPLATE_Dir = GetIniKey(ini->ReadString(ASection, "TEMPLATE", ""));
    // ﾃﾝﾌﾟﾚｰﾄﾌｫﾙﾀﾞ

    G_HEADS_INP_Dir = GetIniKey(ini->ReadString(ASection, "HEADS_INPUT",
        "")); // HEADS入力ﾌｫﾙﾀﾞ
    G_TRIGGER_Dir = GetIniKey(ini->ReadString(ASection, "TRIGGER", ""));
    // ﾄﾘｶﾞﾌｫﾙﾀﾞ
    G_CSV_Dir = GetIniKey(ini->ReadString(ASection, "CSV", "")); // CSVﾌｫﾙﾀﾞ
    G_MATERIAL_Dir = GetIniKey(ini->ReadString(ASection, "MATERIAL", ""));
    // 材質ｺｰﾄﾞﾏｽﾀﾌｫﾙﾀﾞ

    G_HEADS_OUT_Dir = GetIniKey(ini->ReadString(ASection, "HEADS_OUTPUT",
        "")); // HEADS出力ﾌｫﾙﾀﾞ
    G_OUT_ORDER_Dir = GetIniKey(ini->ReadString(ASection, "OUT_ORDER", ""));
    // ｵｰﾀﾞ図面ﾌｫﾙﾀﾞ
    G_OUT_ESTIM_Dir = GetIniKey(ini->ReadString(ASection, "OUT_ESTIMATE",
        "")); // 見積図面ﾌｫﾙﾀﾞ

    G_HEADS_DATA_Dir = GetIniKey(ini->ReadString(ASection, "HEADS_DATA",
        "")); // HEADS管理ﾃﾞｰﾀ出力ﾌｫﾙﾀﾞ
    G_DAT_ORDER_Dir = GetIniKey(ini->ReadString(ASection, "DATA_ORDER", ""));
    // ｵｰﾀﾞ図面管理ﾃﾞｰﾀﾌｫﾙﾀﾞ
    G_DAT_ESTIM_Dir = GetIniKey(ini->ReadString(ASection, "DATA_ESTIMATE",
        "")); // 見積図面管理ﾃﾞｰﾀﾌｫﾙﾀﾞ

    // 2004.09.23追加
    G_OUT_DIRECT_Dir =
        GetIniKey(ini->ReadString(ASection, "OUT_DIRECTION", ""));
    // マル標出力ﾃﾞｰﾀﾌｫﾙﾀﾞ

    // Windows 2008Server 移植対応
    ASection = "recep_environment";
    G_Err_Log_FileName =
        GetIniKey(ini->ReadString(ASection, "log_file_Unregistered", ""));
    // 型式未対応ﾛｸﾞ


    // 2014/08/07 応用技術追加
    // 言語識別文字列の読み込み
    ASection = "LANGUAGE";
    G_Language = GetIniKey(ini->ReadString(ASection, "LANGUAGE", ""));


    delete ini; // INIﾌｧｲﾙｱｸｾｽ ｵﾌﾞｼﾞｪｸﾄ 削除
}

// ---------------------------------------------------------------------------
// 日本語関数名：IniFile(HD_PARTS.ini)の読み込み処理
// 概  要      ：INIﾌｧｲﾙ読み込み
// 引  数      ：なし
// 戻り値      ：なし
// 備  考      ：環境設定ファイル = HD_PARTS.ini
// ---------------------------------------------------------------------------
void __fastcall IniFileRead_Parts(void) {
    TIniFile *ini; // INIﾌｧｲﾙｱｸｾｽ ｵﾌﾞｼﾞｪｸﾄ
    AnsiString AIniPath; // 設定ﾌｧｲﾙﾊﾟｽ
    AnsiString AIniFile; // 設定ﾌｧｲﾙﾊﾟｽ + ﾌｧｲﾙ名
    AnsiString ASection; // ｾｸｼｮﾝ名

    char inipath[MAX_PATH];

//    // 環境iniファイルのディレクトリ
//    GetSystemDirectory(inipath, sizeof(inipath));
//    AIniPath = inipath;

    AIniPath = ExtractFilePath(Application->ExeName);
        //AIniPath = "D:\\HEADS_ROOT01\\HEADS-CV\\";
        //AIniPath = "C:\\HEADS_ROOT01\\HEADS-CV\\";

    // 環境iniファイルのへのフルパスファイル名
    AIniFile = AIniPath + CSYSINIFILENAME_P;

    // iniファイルから設定内容を読み込む
    ini = new TIniFile(AIniFile);

    // --- ﾌｧｲﾙ名称情報取得
    ASection = "FILENAME"; // --  読み込みｾｸｼｮﾝｾｯﾄ : 基本
    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ名称
    G_AnaakeFileNM = GetIniKey(ini->ReadString(ASection, "ANAAKEDATA", ""));

    // 予備品ﾃﾞｰﾀ名称
    G_YobihinFileNM = GetIniKey(ini->ReadString(ASection, "YOBIHINDATA", ""));

    delete ini; // INIﾌｧｲﾙｱｸｾｽ ｵﾌﾞｼﾞｪｸﾄ 削除
}

// ---------------------------------------------------------------------------
// [関数]  IniFile内容読み取り処理
// [概要]  読み取った内容からｺﾒﾝﾄ部分を抜き取る。
// [戻値]  引数列からコメント行を抜き取った文字列
// [引数]  A_Key: 文字列
// ---------------------------------------------------------------------------
AnsiString __fastcall GetIniKey(AnsiString A_Key) {
    static AnsiString A_RKey;
    int i_Point;

    i_Point = A_Key.Pos(";");

    if (i_Point != 0) {
        A_RKey = Trim(A_Key.SubString(1, i_Point - 1));
    }
    else {
        A_RKey = Trim(A_Key);
    }

    return (A_RKey);
}

// -----------------------------------------------------------------------------
// 関数名      ：ＮＯﾊｲﾌﾝ編集
// 概  要      ：ﾊｲﾌﾝ["-"]を取り除く
// 引  数      ：InpData  : 入力値
// 戻り値      ：AnsiString
// 備  考      ：
// -----------------------------------------------------------------------------
AnsiString __fastcall NO_Haifun(AnsiString InpData) {
    static AnsiString A_Data;
    int i, len;

    A_Data = "";
    len = InpData.Length();
    for (i = 1; i <= len; i++) {
        if (InpData.SubString(i, 1) != "-") // "-" 以外のとき
        {
            A_Data = A_Data + InpData.SubString(i, 1);
        }
    }
    return A_Data;
}

// -----------------------------------------------------------------------------
// 関数名      ：ＮＯｽﾗｯｼｭ編集
// 概  要      ：日付の区切り文字["/"]を取り除く
// 引  数      ：InpData  : 入力値
// 戻り値      ：AnsiString
// 備  考      ：
// -----------------------------------------------------------------------------
AnsiString __fastcall No_Slush(AnsiString InpData) {
    static AnsiString A_Data;
    int i, len;

    A_Data = "";
    len = InpData.Length();
    for (i = 1; i <= len; i++) {
        if (InpData.SubString(i, 1) != "/") // "/" 以外のとき
        {
            A_Data = A_Data + InpData.SubString(i, 1);
        }
    }
    return A_Data;
}

// -----------------------------------------------------------------------------
// 関数名      ：ＮＯカンマ編集
// 概  要      ：3 桁区切りの記号と通貨記号を取り除く
// 引  数      ：InpData  : 入力値
// 戻り値      ：AnsiString
// 備  考      ：
// -----------------------------------------------------------------------------
AnsiString __fastcall F_NO_Comma(AnsiString InpData) {
    static AnsiString A_Data;
    int i, len;

    A_Data = "";
    len = InpData.Length();
    for (i = 1; i <= len; i++) {
        if ((InpData.SubString(i, 1) != ",") && (InpData.SubString(i,
            1) != "\\")) // "," and "￥" 以外のとき
        {
            A_Data = A_Data + InpData.SubString(i, 1);
        }
    }
    return A_Data;
}

// -----------------------------------------------------------------------------
// 関数名      ：ＮＯコロン区切り編集
// 概  要      ：コロンまでを取得
// 引  数      ：InpData  : 入力値
// 戻り値      ：AnsiString
// 備  考      ：
// -----------------------------------------------------------------------------
AnsiString __fastcall NO_CoronCut(AnsiString InpData) {
    static AnsiString A_Data;
    int i, endlen;

    A_Data = "";
    endlen = InpData.Pos(":") - 1;
    if (endlen < 1) {
        endlen = InpData.Length();
    }
    for (i = 1; i <= endlen; i++) {
        if (InpData.SubString(i, 1) != ":") // ":" 以外のとき
        {
            A_Data = A_Data + InpData.SubString(i, 1);
        }
    }
    return A_Data;
}

// -----------------------------------------------------------------------------
// 関数名      ：数値入力項目　数値チェック
// 概  要      ：入力項目における数値チェック
// 引  数      ：InpData  : 入力値
// ：OupData  : 戻り値
// ：i_Seisu  : 整数部有効桁数
// ：i_Syosu  : 小数部有効桁数
// ：i_Fugou  : ﾏｲﾅｽ符号  0:無し　1:有り
// 戻り値      ：0:ｴﾗｰ無し　1:桁数ｵｰﾊﾞ　2:有効範囲ｵｰﾊﾞ　3:属性ｴﾗｰ
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall Numeric_Check(AnsiString InpData, double *OutData, int i_Seisu,
    int i_Syosu, int i_Fugou) {
    int i_Point;
    int i_Keta1, i_Keta2;
    double dval, dvalfabs;
    double d;

    AnsiString wkinpdata;

    InpData = F_NO_Comma(InpData.Trim());

    // ------属性チェック------
    dval = StrToDblDef(InpData); // double値に変換
    if (dval == 0.0f) {
        if (!InpData.IsEmpty()) {
            if (AllFloatChk(InpData.c_str(), (short)i_Fugou)) {
                return 3;
            }
        }
        InpData = "0";
    }

    // ------小数点位置------
    dvalfabs = fabs(dval); // 絶対値
    wkinpdata = dvalfabs; // 絶対値
    i_Point = wkinpdata.Pos(".");
    // 小数点以下の数値が存在しないとき
    if (i_Point == 0) {
        i_Keta1 = strlen(wkinpdata.c_str());
        i_Keta2 = 0;
    }
    else {
        i_Keta1 = i_Point - 1;
        i_Keta2 = strlen(wkinpdata.c_str()) - i_Point;
    }

    // ------ 文字桁数チェック------
    if (i_Keta1 > i_Seisu) // 整数部
    {
        return 1; // error
    }
    if (i_Keta2 > i_Syosu) // 小数部
    {
        return 1; // error
    }

    // ------有効範囲チェック（ﾏｲﾅｽ）------
    if ((i_Fugou == 0) && (dval < 0.0)) {
        return 2;
    }
    // ------有効範囲チェック（整数桁）------
    d = pow10(i_Seisu);
    if (dvalfabs >= d) // 整数桁オーバー？
    {
        return 2;
    }

    (*OutData) = dval;

    // 正常終了
    return 0;
}

// ---------------------------------------------------------------------------
// [関数]  文字列 桁数調整処理
// [概要]  文字列 桁数調整(桁数分文字を返す)
// [戻値]  文字列 桁数調整後の文字列
// [引数]  A_Text:   文字列
// i_Length:   桁数
// ---------------------------------------------------------------------------
AnsiString __fastcall TxtEdit(AnsiString A_Text, int i_Length) {
    int i_cnt;
    static AnsiString A_RText;

    for (i_cnt = 1; i_cnt <= i_Length; i_cnt++) {
        A_Text = A_Text + " ";
    }

    for (i_cnt = 1; i_cnt <= i_Length; i_cnt++) {
        if (A_Text.SubString(i_cnt, 1) == "") {
            i_cnt = i_cnt - 1;
            break;
        }
    }

    A_RText = "";
    if (i_cnt == 0) {
        for (i_cnt = 1; i_cnt <= i_Length; i_cnt++) {
            A_RText = A_RText + " ";
        }
    }
    else if (i_cnt <= i_Length) {
        A_RText = A_Text.SubString(1, i_cnt);
        for (i_cnt = 1; i_cnt <= i_Length; i_cnt++) {
            A_RText = A_RText + " ";

        }
        A_RText = A_RText.SubString(1, i_Length);
    }
    else {
        A_RText = A_Text.SubString(1, i_Length);
    }

    if (StrByteType(A_RText.c_str(), i_Length - 1) == mbLeadByte)
        // 最後の桁数目が２バイト文字の第１バイト目？
    {
        A_RText = A_RText.SubString(1, i_Length - 1) + " ";
    }
    else {
        A_RText = A_RText.SubString(1, i_Length);
    }

    return A_RText;
}

// ---------------------------------------------------------------------------
// [関数]  文字列 桁数調整処理(後ろ空白削除版)
// [概要]  文字列 桁数調整(桁数分以内の後ろ空白削除文字を返す)
// [戻値]  文字列 桁数調整後の文字列
// [引数]  A_Text:    文字列
// i_Length:  桁数
// ---------------------------------------------------------------------------
AnsiString __fastcall TxtEditRt(AnsiString A_Text, int i_Length) {
    static AnsiString A_RText;
    int len;

    A_RText = A_Text.TrimRight();
    if (A_RText.Length() > i_Length) {
        A_RText.SetLength(i_Length);
    }

    len = A_RText.Length();
    if (A_RText.IsLeadByte(len)) // 最後の桁数目が２バイト文字の第１バイト目？
    {
        len--;
        A_RText.SetLength(len);
    }

    return A_RText;
}

// -----------------------------------------------------------------------------
// 日本語関数名：数字検査処理
// 概  要      ：文字列が全て'0'～'9'の文字コードで構成されているか検査する
// 引  数      ：char *Str 検査文字列, flg 方法[0:正,1:負]
// 戻り値      ：検査結果 [0:正常 1:異常]
// 機能説明    ：文字列が全て'0'～'9'の文字コードで構成されているか検査する
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall AllNumChk(char *Str, short flg) {
    int i; // ループカウンタ

    if (Str[0] == NULL) // 文字無し？
    {
        return 2; // 戻り値[2:異常]
    }

    i = strlen(Str);
    while (i-- > 0) // 文字列長文
    {
        if ((Str[i] < '0') || (Str[i] > '9')) // 数字'0'～'9'以外
        {
            if (Str[i] == '-') // '-'？
            {
                if (flg == 0) // '-'はｴﾗｰ とする？
                {
                    return 1; // 戻り値[1:異常]
                }
            }
            else {
                return 1; // 戻り値[1:異常]
            }
        }
    }
    return 0; // 戻り値[0:正常]
}

// -----------------------------------------------------------------------------
// 日本語関数名：小数数字検査処理
// 概  要      ：文字列が全て'0'～'9'と'.'or'-'の文字コードで構成されているか検査する
// 引  数      ：char *Str 検査文字列, flg 方法[0:正,1:負]
// 戻り値      ：検査結果 [0:正常 1:異常]
// 機能説明    ：文字列が全て'0'～'9'と'.'or'-'の文字コードで構成されているか検査する
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall AllFloatChk(char *Str, short flg) {
    int i; // ループカウンタ

    if (Str[0] == NULL) // 文字無し？
    {
        return 2; // 戻り値[2:異常]
    }

    i = strlen(Str);
    while (i-- > 0) // 文字列長文
    {
        if (((Str[i] < '0') || (Str[i] > '9')) && (Str[i] != '.'))
            // 数字'0'～'9'と'.' 以外
        {
            if (Str[i] == '-') // '-'？
            {
                if (flg == 0) // '-'はｴﾗｰ とする？
                {
                    return 1; // 戻り値[1:異常]
                }
            }
            else {
                return 1; // 戻り値[1:異常]
            }
        }
    }
    return 0; // 戻り値[0:正常]
}

// -----------------------------------------------------------------------------
// 日本語関数名：英数字検査処理
// 概  要      ：文字列が全て英数字の文字コードで構成されているか検査する
// 引  数      ：char *Str 検査文字列, flg 方法[0:数値なし,1:数値あり]
// 戻り値      ：検査結果 [0:正常 1:異常]
// 機能説明    ：文字列が全て'0'～'9'と'.'or'-'の文字コードで構成されているか検査する
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall AllAlphaChk(char *Str, short flg) {
    int i; // ループカウンタ

    if (Str[0] == NULL) // 文字無し？
    {
        return 2; // 戻り値[2:異常]
    }

    i = strlen(Str);
    while (i-- > 0) // 文字列長文
    {
        if (((Str[i] < ' ') || (Str[i] > '~'))) // 半角英数字'？
        {
            if (((Str[i] >= '0') || (Str[i] <= '9'))) // 数字'0'～'9'？
            {
                if (flg == 0) // 数字はｴﾗｰ とする？
                {
                    return 1; // 戻り値[1:異常]
                }
            }
            else {
                return 1; // 戻り値[1:異常]
            }
        }
    }
    return 0; // 戻り値[0:正常]
}

// -----------------------------------------------------------------------------
// 関数名　　　：入力文字列チェック
// 概  要      ：入力項目における使用禁止文字の有無チェック
// 引  数      ：AnsiString Str  チェック文字列
// ：short   dashflg "'"ﾀﾞｯｼｭｴﾗｰﾌﾗｸﾞ(省略可能:0)
// 戻り値      ：0:正常、１：異常
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall StrErrCheck(AnsiString Str, short dashflg) {
    if (Str.IsEmpty()) // チェック対象がブランク？
    {
        return 0;
    }
    else if (Str.Pos(",") == 1) // 『，』
    {
        return 1;
    }
    else if (Str.Pos("'") != 0) // 『’』
    {
        if (dashflg) {
            if (Str.Pos("'") == 1) {
                return 1;
            }
        }
        else {
            return 1;
        }
    }
    else if (Str.Pos("\"") == 1) // 『”』
    {
        return 1;
    }
    else if (Str.Pos("|") == 1) // 『|』
    {
        return 1;
    }
    else // その他
    {
        return 0;
    }

    return 0;
}

// -----------------------------------------------------------------------------
// 関数名　　　：入力文字列チェック
// 概  要      ：入力項目における使用禁止文字の有無チェック
// 引  数      ：TObject *Sender 呼出し元
// 戻り値      ：0:正常、１：異常
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall TextErrCheck(TObject *Sender) {
    TEdit *wkedit;
    TMemo *wkmemo;
    TComboBox *wkcombobox;
    bool dashflg = false;
    AnsiString Str;

    Str = "";
    if ((wkedit = dynamic_cast<TEdit*>(Sender)) != NULL) {
        Str = wkedit->Text.Trim();
    }
    else if ((wkmemo = dynamic_cast<TMemo*>(Sender)) != NULL) {
        Str = wkmemo->Text.Trim();
        dashflg = true;
    }
    else if ((wkcombobox = dynamic_cast<TComboBox*>(Sender)) != NULL) {
        int len = wkcombobox->Text.Pos(":") - 1;
        Str = Trim(wkcombobox->Text.SubString(1, len));
    }

    return StrErrCheck(Str, dashflg);
}

// -----------------------------------------------------------------------------
// 関数名　　　：日付チェック処理
// 概  要      ：日付のチェックを行う
// 引  数      ：A_ymd    (yyyy/mm/dd形式)
// *Outdata ﾁｪｯｸ後日付文字列格納先
// 戻り値      ：0:異常なし 1:桁数エラー 2:日付ｴﾗｰ
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall DateCheck(AnsiString A_ymd, AnsiString *Outdata) {
    int i_errsw = 0;
    int len;
    int MyPos;
    AnsiString year, month, day;
    AnsiString A_md, a_wk;

    a_wk = A_ymd.Trim();
    len = a_wk.Length();

    // 最初のｽﾗｯｼｭを探す
    MyPos = a_wk.AnsiPos("/");
    if (MyPos == 0) {
        (*Outdata) = a_wk;
        return 1;
    }

    // 次のｽﾗｯｼｭを探す
    MyPos = a_wk.SubString(MyPos + 1, len - MyPos + 1).AnsiPos("/");
    if (MyPos == 0) {
        (*Outdata) = a_wk;
        return 1;
    }

    MyPos = a_wk.AnsiPos("/");
    // 年を取得
    year = a_wk.SubString(1, MyPos - 1);

    // 年からｽﾗｯｼｭまで削除
    A_md = a_wk.Delete(1, MyPos);

    // 月と日の区切りを探す
    MyPos = A_md.AnsiPos("/");

    // 月までを取得
    month = A_md.SubString(1, MyPos - 1);

    // 月から区切りまでを削除して日を取得
    day = A_md.Delete(1, MyPos);

    if (year.Length() == 3) {
        (*Outdata) = a_wk;
        return 1;
    }

    if (year.Length() == 1) {
        year = FormatDateTime("yy", Date()).SubString(1, 1) + year;
    }

    if (StrToIntDef(year, 0) < 100) {
        if (StrToIntDef(year, 0) > 80) {
            year = "19" + year;
        }
        else {
            year = "20" + year;
        }
    }

    // 年確定
    len = year.Trim().Length();
    year = year.SubString(len - 3, 4);

    // 月確定
    month = "0" + month;
    len = month.Trim().Length();
    month = month.SubString(len - 1, 2);

    // 日確定
    day = "0" + day;
    len = day.Trim().Length();
    day = day.SubString(len - 1, 2);

    // 日付書式確定
    a_wk = year + "/" + month + "/" + day;
    (*Outdata) = a_wk;

    try {
        if (a_wk.Length() != 10) {
            i_errsw = 1;
        }
        else {
            try {
                StrToDate(A_ymd);
            }
            catch (EConvertError &error) {
                i_errsw = 2;
            }
        }
    }
    __finally {
        // Windows 2008Server 移植対応
        // return (short)i_errsw;
    }
    return (short)i_errsw;
}

// -----------------------------------------------------------------------------
// 関数名　　　：数値項目ｶﾝﾏ編集モジュール
// 概  要      ：数字にｶﾝﾏをつけて返す
// 引  数      ：AnsiString &AStr  対象文字列先
// ：int   syosu  小数点以下桁数
// 戻り値      ：0:成功 1:失敗
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall KanmaStr(AnsiString &AStr, int syosu) {
    AnsiString A_wk;
    double d_wk;
    char wrk[64], buf[64];

    A_wk = F_NO_Comma(AStr.Trim());
    if (A_wk.IsEmpty()) {
        return 0;
    }
    else {
        try {
            d_wk = StrToDblDef(A_wk);
            sprintf(wrk, "#,%%.%df", syosu);
            sprintf(buf, wrk, 0.0f);
            AStr = FormatFloat(buf, d_wk);
            return 0;
        }
        catch (EConvertError &Error) {
            return 1;
        }
    }
}

// -----------------------------------------------------------------------------
// 関数名　　　：数値項目ｶﾝﾏ編集モジュール
// 概  要      ：数字にｶﾝﾏをつけて返す
// 引  数      ：TObject *Sender 入力ｺﾝﾄﾛｰﾙ
// ：int syosu  小数点以下桁数
// 戻り値      ：0:成功 1:失敗
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall AddKanma(TObject *Sender, int syosu) {
    short sRtn;
    TMemo *wkmemo;
    AnsiString AStr;

    if ((wkmemo = dynamic_cast<TMemo*>(Sender)) != NULL) {
        AStr = wkmemo->Lines->Text.Trim();
        if (AStr.IsEmpty()) {
            AStr = "0";
        }
        sRtn = KanmaStr(AStr, syosu);
        wkmemo->Lines->Text = AStr;
        return sRtn;
    }
    else {
        return 1;
    }
}

// -----------------------------------------------------------------------------
// 関数名      ：値補正処理
// 概  要      ：値を四捨五入,切上,切捨で補正する
// パラメータ  ：int     sw   : 方法(0:四捨五入,1:切り上げ,2:切り捨て)
// ：double  dval : 対象値
// ：int     k    : 丸める少数桁位置
// 戻り値      ：値補正値(エラー時は未変換)
// 備  考      ：
// -----------------------------------------------------------------------------
double __fastcall Hasu_Round(int sw, double dval, int k) {
    double d, x;

    try {
        d = pow10(k);
        switch (sw) {
        case 0:
            // 四捨五入
            x = floor(((dval < 0) ? (-dval) : (dval)) * d + 0.5);
            return (((dval < 0) ? (-x) : (x)) / d);
        case 1:
            // 切り上げ
            x = ceil(((dval < 0) ? (-dval) : (dval)) * d);
            return (((dval < 0) ? (-x) : (x)) / d);
        case 2:
            // 切り捨て
            x = floor(((dval < 0) ? (-dval) : (dval)) * d + 0.00001);
            // 小数 桁落ち対策
            return (((dval < 0) ? (-x) : (x)) / d);
        default:
            return dval;
        }
    }
    catch (...) {
        return dval;
    }
}

// -----------------------------------------------------------------------------
// 関数名      ：値補正処理(long double)
// 概  要      ：値を四捨五入,切上,切捨で補正する
// パラメータ  ：int          sw    : 方法(0:四捨五入,1:切り上げ,2:切り捨て)
// ：long double  ldval : 対象値
// ：int          k     : 丸める少数桁位置
// 戻り値      ：値補正値(エラー時は未変換)
// 備  考      ：
// -----------------------------------------------------------------------------
Extended __fastcall Hasu_RoundE(int sw, Extended ldval, int k) {
    Extended ld, ldx;

    try {
        ld = pow10l(k);
        switch (sw) {
        case 0:
            // 四捨五入
            ldx = floorl(((ldval < 0) ? (-ldval) : (ldval)) * ld + 0.5);
            return (((ldval < 0) ? (-ldx) : (ldx)) / ld);
        case 1:
            // 切り上げ
            ldx = ceill(((ldval < 0) ? (-ldval) : (ldval)) * ld);
            return (((ldval < 0) ? (-ldx) : (ldx)) / ld);
        case 2:
            // 切り捨て
            ldx = floorl(((ldval < 0) ? (-ldval) : (ldval)) * ld + 0.00001);
            // 小数 桁落ち対策
            return (((ldval < 0) ? (-ldx) : (ldx)) / ld);
        default:
            return ldval;
        }
    }
    catch (...) {
        return ldval;
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： 文字列 半角カタカナ変換処理
// 概  要      ： 文字列内の半角カタカナを全角に変換する
// 引  数      ： AText  :  文字列
// ： &AConv :  変換後文字列先
// ： iLen   :  文字数最大
// 戻り値      ： 変換後の結果 0:正常, 1:err
// 備  考      ：
// ---------------------------------------------------------------------------
short __fastcall HalfKanaConv(AnsiString AText, AnsiString &AConv, int iLen) {
    const static WORD s_z[] = {
        0x8142 /* 。 */ , 0x8175 /* 「 */ , 0x8176 /* 」 */ , 0x8141 /* 、 */ ,
        0x8145 /* ・ */ , 0x8392 /* ヲ */ , 0x8340 /* ァ */ , 0x8342 /* ィ */ ,
        0x8344 /* ゥ */ , 0x8346 /* ェ */ , 0x8348 /* ォ */ , 0x8383 /* ャ */ ,
        0x8385 /* ュ */ , 0x8387 /* ョ */ , 0x8362 /* ッ */ , 0x815b /* ー */ ,
        0x8341 /* ア */ , 0x8343 /* イ */ , 0x8345 /* ウ */ , 0x8347 /* エ */ ,
        0x8349 /* オ */ , 0x834a /* カ */ , 0x834c /* キ */ , 0x834e /* ク */ ,
        0x8350 /* ケ */ , 0x8352 /* コ */ , 0x8354 /* サ */ , 0x8356 /* シ */ ,
        0x8358 /* ス */ , 0x835a /* セ */ , 0x835c /* ソ */ , 0x835e /* タ */ ,
        0x8360 /* チ */ , 0x8363 /* ツ */ , 0x8365 /* テ */ , 0x8367 /* ト */ ,
        0x8369 /* ナ */ , 0x836a /* ニ */ , 0x836b /* ヌ */ , 0x836c /* ネ */ ,
        0x836d /* ノ */ , 0x836e /* ハ */ , 0x8371 /* ヒ */ , 0x8374 /* フ */ ,
        0x8377 /* ヘ */ , 0x837a /* ホ */ , 0x837d /* マ */ , 0x837e /* ミ */ ,
        0x8380 /* ム */ , 0x8381 /* メ */ , 0x8382 /* モ */ , 0x8384 /* ヤ */ ,
        0x8386 /* ユ */ , 0x8388 /* ヨ */ , 0x8389 /* ラ */ , 0x838a /* リ */ ,
        0x838b /* ル */ , 0x838c /* レ */ , 0x838d /* ロ */ , 0x838f /* ワ */ ,
        0x8393 /* ン */ , 0x814a /* ゛ */ , 0x814b /* ゜ */ , };
    short sRtn;
    AnsiString ASetStr;
    int mode;
    int i, len;
    DWORD code;
    UCHAR ch, next;

    sRtn = 0;

    // ﾁｪｯｸ・変換
    ASetStr = "";
    len = AText.Length();

    mode = CT_ANK;
    for (i = 0; i < len; i++) {
        ch = (UCHAR)AText[i + 1];
        mode = chkctype(ch, mode);
        if ((mode == CT_ANK) && (iskana(ch))) {
            code = s_z[ch - 0xa1];
            i++;

            next = (i < len) ? ((UCHAR)AText[i + 1]) : ('\0');
            if ((next == 0xde) /* 濁点 */ && ((ch == 0xb3) ||
                ((ch >= 0xb6) && (ch <= 0xc4)) || ((ch >= 0xca) && (ch <=
                0xce)))) {
                if (ch == 0xb3) /* ウ */ {
                    code = 0x8394; /* ヴ */
                }
                else {
                    code++;
                }
            }
            else if ((next == 0xdf) /* 半濁点 */ &&
                (((ch >= 0xca) && (ch <= 0xce)))) {
                code += 2;
            }
            else {
                i--; // １文字戻す
            }
            ASetStr += (char)(code >> 8);
            ASetStr += (char)(code);
        }
        else {
            ASetStr += (char)(ch);
        }
    }

    if (iLen > 0) {
        if (ASetStr.Length() > iLen) {
            ASetStr.SetLength(iLen);
        }
        len = ASetStr.Length();
        if (ASetStr.IsLeadByte(len)) // 最後の桁数目が２バイト文字の第１バイト目？
        {
            len--;
            ASetStr.SetLength(len);
        }
    }
    AConv = ASetStr;

    return sRtn;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ｵﾌﾞｼﾞｪｸﾄ位置中央設定処理
// 概  要      ： ｵﾌﾞｼﾞｪｸﾄ位置を中央に設定する
// 引  数      ： TObject *parent     基準ｵﾌﾞｼﾞｪｸﾄ
// ： TObject *object     位置設定ｵﾌﾞｼﾞｪｸﾄ
// ： int     subx, suby  位置補正分(省略可能:[0,0])
// 戻り値      ： 変換後の結果 0:正常, 1:err
// 備  考      ：
// ---------------------------------------------------------------------------
short __fastcall SetPosCenter(TObject *parent, TObject *object, int subx,
    int suby) {
    TControl *ctrlPar, *ctrlObj;
    TPoint ppos, pos1, pos2, setpos, add;

    try {
        if ((ctrlPar = dynamic_cast<TControl*>(parent)) == NULL) {
            return 1;
        }
        if ((ctrlObj = dynamic_cast<TControl*>(object)) == NULL) {
            return 1;
        }

        ppos = TPoint(ctrlObj->Left + subx, ctrlObj->Top + suby);
        add.x = ((ctrlPar->ClientWidth - ctrlObj->Width) / 2);
        add.y = ((ctrlPar->ClientHeight - ctrlObj->Height) / 2);
        pos1 = ctrlPar->ClientToScreen(TPoint(0, 0));
        pos2.x = pos1.x + add.x + ppos.x;
        pos2.y = pos1.y + add.y + ppos.y;
        setpos = ctrlObj->ScreenToClient(pos2);

        ctrlObj->Left = setpos.x;
        ctrlObj->Top = setpos.y;
    }
    catch (...) {
        return (-1);
    }
    return 0;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 日付項目別ﾁｪｯｸ処理
// 概  要      ： 日付を項目別にﾁｪｯｸする
// 引  数      ： AnsiString AStr   判定文字列
// ： int        type   項目種類[DateKindT::]
// ： int year, month   基準年月(type:日で使用)(省略可能:0)
// ： int        m2def  ２月=29日ﾌﾗｸﾞ(type:日で使用)(省略可能:1)
// 戻り値      ： 変換後の結果値, 又は err時:未処理値
// 備  考      ：
// ---------------------------------------------------------------------------
short __fastcall DateSubChk(AnsiString AStr, int type, int year, int month,
    int m2def) {
    AnsiString AWrkStr;
    USHORT yy, mm, dd;
    double d_val;
    int i_val;
    int iRtn;
    int chklen, chkmin, chkmax;

    AWrkStr = Trim(AStr);

    // ﾌﾞﾗﾝｸﾁｪｯｸ
    if (AWrkStr.IsEmpty()) {
        return 4; // ﾌﾞﾗﾝｸ
    }

    // 使用禁止文字ﾁｪｯｸ
    if (StrErrCheck(AWrkStr) || AllNumChk(AWrkStr.c_str(), 0)) {
        return 5; // 使用禁止文字
    }

    // 数値項目のチェック
    switch (type) {
    case dsDays: // dd
        chklen = 2;
        chkmin = 1;
        chkmax = 31;
        break;
    case dsSetDay: // 指定dd
        chklen = 2;
        chkmin = 1;
        if ((month == 2) && (m2def)) {
            chkmax = 29; // ２月 最大日数
        }
        else {
            year = YYtoYYYY(year);
            if ((year <= 0) || (month <= 0)) {
                Date().DecodeDate(&yy, &mm, &dd);
                if (year <= 0)
                    year = yy;
                if (month <= 0)
                    month = mm;
            }
            chkmax = GetMonthDays(year, month); // 指定年月 最大日数
        }
        break;
    case dsMonth: // mm
        chklen = 2;
        chkmin = 1;
        chkmax = 12;
        break;
    case dsSubYear: // yy
        chklen = 2;
        chkmin = 0;
        chkmax = 99;
        break;
    case dsYear: // yyyy
    default:
        chklen = 4;
        chkmin = 1;
        chkmax = 9999;
        break;
    }
    d_val = 0;
    iRtn = Numeric_Check(AStr, &d_val, chklen, 0, 0);
    if (iRtn) {
        return (short)iRtn;
    }
    i_val = (int)(d_val + 0.1);
    if ((i_val < chkmin) || (i_val > chkmax)) // 有効範囲外のとき
    {
        return 6; // 範囲ｴﾗｰ(入力ｴﾗｰ)
    }

    return 0;
}

// -----------------------------------------------------------------------------
// 関数名      ：YY→YYYY変換処理
// 概  要      ：YYをYYYYに変換する
// 引  数      ：YY_Str  : 入力YY値,  flg:ｾﾞﾛｻﾌﾟﾚｽﾌﾗｸﾞ[省略時1:0あり]
// 戻り値      ：AnsiString
// 備  考      ：
// -----------------------------------------------------------------------------
AnsiString __fastcall YYtoYYYY_Str(AnsiString YY_Str, short flg) {
    static AnsiString A_Str;
    int ival, len;

    ival = YYtoYYYY(YY_Str.ToIntDef(0));
    if (flg) {
        ZeroSupress(IntToStr(ival), A_Str, 4); // YY → YYYY
    }
    else {
        A_Str = IntToStr(ival);
        len = A_Str.Length();
        if (len < 4) // " "不足？
        {
            A_Str = AnsiString::StringOfChar(' ', (4 - len)) + A_Str;
        }
    }
    return A_Str;
}

// -----------------------------------------------------------------------------
// 関数名      ：YY→YYYY変換処理
// 概  要      ：YYをYYYYに変換する
// 引  数      ：yy  : 入力YY値
// 戻り値      ：YYYY値
// 備  考      ：
// -----------------------------------------------------------------------------
short __fastcall YYtoYYYY(int yy) {
    if (yy < 100) {
        return (short)((yy > 80) ? (yy + 1900) : (yy + 2000));
    }
    else {
        return (short)yy;
    }
}

// -----------------------------------------------------------------------------
// 日本語関数名：日付文字(YYYY/MM/DD) 変換処理
// 概  要      ：日付文字(YYYY/MM/DD)を 数値に変換
// 引  数      ：char *cYMD,  日付文字
// WORD &Year, &Month, &Day  数値格納先
// 戻り値      ：0:正常, !0:err
// 備  考      ：区切り文字:"/"
// -----------------------------------------------------------------------------
short __fastcall DecodeYMD(char *cYMD, WORD *Year, WORD *Month, WORD *Day) {
    short sRtn;
    TDate date;
    char *p;
    int i, len;
    AnsiString Awrk;

    sRtn = 0;
    try {
        (*Year) = 0;
        (*Month) = 0;
        (*Day) = 0;

        len = strlen(cYMD);
        p = StrPos(cYMD, "/");
        if (p != NULL) {
            Awrk = "";
            i = 0;
            while ((&cYMD[i]) != (p)) {
                Awrk += cYMD[i++];
            }
            (*Year) = (WORD)atoi(Awrk.c_str());

            i++;
            p = StrPos(&cYMD[i], "/");
            if (p != NULL) {
                Awrk = "";
                while ((&cYMD[i]) != (p)) {
                    Awrk += cYMD[i++];
                }
                (*Month) = (WORD)atoi(Awrk.c_str());

                i++;
                Awrk = "";
                while (i < len) {
                    Awrk += cYMD[i++];
                }
                (*Day) = (WORD)atoi(Awrk.c_str());
            }
        }
    }
    __finally {
        // Windows 2008Server 移植対応
        // return sRtn;
    }
    return sRtn;
}

// -----------------------------------------------------------------------------
// 日本語関数名：日付文字(MM/DD)  変換処理
// 概  要      ：日付文字(MM/DD)を 数値に変換
// 引  数      ：char *cYMD,   日付文字
// WORD &Month, &Day  数値格納先
// 戻り値      ：0:正常, !0:err
// 備  考      ：区切り文字:"/"
// -----------------------------------------------------------------------------
short __fastcall DecodeMD(char *cYMD, WORD *Month, WORD *Day) {
    short sRtn;
    TDate date;
    char *p;
    int i, len;
    AnsiString Awrk;

    sRtn = 0;
    try {
        (*Month) = 0;
        (*Day) = 0;

        len = strlen(cYMD);
        p = StrPos(cYMD, "/");
        if (p != NULL) {
            Awrk = "";
            i = 0;
            if (p != NULL) {
                Awrk = "";
                while ((&cYMD[i]) != (p)) {
                    Awrk += cYMD[i++];
                }
                (*Month) = (WORD)atoi(Awrk.c_str());

                i++;
                Awrk = "";
                while (i < len) {
                    Awrk += cYMD[i++];
                }
                (*Day) = (WORD)atoi(Awrk.c_str());
            }
        }
    }
    __finally {
        // Windows 2008Server 移植対応
        // return sRtn;
    }
    return sRtn;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 日付加減算出処理
// 概  要      ： 日付の算出を行う
// 引  数      ： TDateTime posdate  日付基準値
// ： year,month,day     日付加減算値(月日は範囲外でもよい)
// 戻り値      ： TDateTime 計算結果日付（失敗時は元の日付）
// 備  考      ：
// ---------------------------------------------------------------------------
TDateTime __fastcall DateCalc(TDateTime date, int year, int month, int day) {
    unsigned short y, m, d;

    try {
        date.DecodeDate(&y, &m, &d);
        year += y;
        month += m;
        if (month <= 0) {
            year += month / 12 - 1;
            month = 12 - (-month) % 12;
        }
        else if (month > 12) {
            year += month / 12;
            month %= 12;
        }
        date = TDateTime((short)year, (short)month, d) + day;
        return date;
    }
    catch (...) {
        return date;
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： 日付変換処理
// 概  要      ： 日付の変換を行う
// 引  数      ： year,month,day   日付値(月日は範囲外でもよい)
// 戻り値      ： TDateTime 日付値（失敗時は現日付）
// 備  考      ： 異常値でも変換可能
// ---------------------------------------------------------------------------
TDateTime __fastcall DateConv(int year, int month, int day) {
    TDate date;

    try {
        if (month <= 0) {
            year += month / 12 - 1;
            month = 12 - (-month) % 12;
        }
        else if (month > 12) {
            year += month / 12;
            month %= 12;
        }
        date = TDate((short)year, (short)month, 1) + (day - 1);
        return date;
    }
    catch (...) {
        return Date();
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： 範囲日数取得処理
// 概  要      ： 範囲日の日数を取得する
// 引  数      ： TDateTime posdate  日付基準値
// ： TDateTime posdate  日付基準値
// 戻り値      ： 範囲日数（失敗時は0）
// 備  考      ：
// ---------------------------------------------------------------------------
int __fastcall GetDateSclDays(TDateTime datepos, TDateTime dateend) {
    int rtndays = 0;

    try {
        rtndays = (int)dateend - (int)datepos;
        return rtndays;
    }
    catch (...) {
        return rtndays;
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： 指定月日数取得処理
// 概  要      ： 指定月の日数を取得する
// 引  数      ： int year, int month  指定年月
// 戻り値      ： 範囲日数（失敗時は0）
// 備  考      ：
// ---------------------------------------------------------------------------
int __fastcall GetMonthDays(int year, int month) {
    USHORT y, m, d = 0;
    TDateTime date;

    try {
        year = YYtoYYYY(year);
        date = TDateTime((short)(year + (month / 12)),
            (short)((month % 12) + 1), 1) - 1;
        date.DecodeDate(&y, &m, &d);
    }
    catch (...) {
    }
    return d;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ｾﾞﾛｻﾌﾟﾚｽ用処理
// 概  要      ： ｾﾞﾛｻﾌﾟﾚｽを行う
// 引  数      ： AnsiString StrMath  対象数値文字列
// ： AnsiString &SetStr  処理後文字列格納先
// ： int        len      設定長さ
// 戻り値      ： 0:正常, 1:err
// 備  考      ： ("1"→"01")
// ---------------------------------------------------------------------------
short __fastcall ZeroSupress(AnsiString StrMath, AnsiString &SetStr, int len) {
    int orglen;

    orglen = StrMath.Length();
    if (orglen < len) // "0"不足？
    {
        SetStr = AnsiString::StringOfChar('0', (len - orglen)) + StrMath;
    }
    else {
        SetStr = StrMath;
    }
    return (orglen <= len);
}

// ---------------------------------------------------------------------------
// 日本語関数名：ファイル名チェック処理
// 概  要      ：ファイル名が正しく入力されているか検査する
// 引  数      ：char   *dir  // ファイル名へのポインタ(NULL終了)
// ：short  flg   // 判定方法[0:ﾌｧｲﾙ名, 1:ﾃﾞｨﾚｸﾄﾘ名]
// 戻り値      ：検査結果 [0:正常 1:異常]
// 機能説明    ：ファイル名が未入力ならば異常とする
// ：ファイル名として使用できない文字が含まれていないか検査する
// 備  考      ：wimdowsﾌｧｲﾙｼｽﾃﾑ用
// ：ファイル名の両端をTrimされている(空白ではない)こと。
// ---------------------------------------------------------------------------
short __fastcall WinFileNameChk(char *dir, short flg) {
    int i, len; // ループカウンタ, 長さ

    if (dir[0] == NULL)
        return 1; // 未入力なら異常終了

    len = strlen(dir);
    for (i = 0; i < len; i++) // 文字列分繰り返す
    {
        switch (dir[i]) // ディレクトリ名に使えない文字をチェック
        {
        case '"': // '"'
        case '|': // '|'
        case ';': // ';'
        case '/': // '/'
        case '*': // '*'
        case '<': // '<'
        case '>': // '>'
        case '?': // '?'
            return 1; // 戻り値 : [1:異常]
        case ':': // ':'
        case '\\': // '\\'
            if (flg == 0) // ﾌｧｲﾙ名ﾁｪｯｸ？
            {
                return 1; // 戻り値 : [1:異常]
            }
            break;
        }
    }
    return 0; // 戻り値 : [0:正常]
}

// ---------------------------------------------------------------------------
// 日本語関数名： 文字列小数変換例外処理
// 概  要      ： 文字列小数変換の正常ﾁｪｯｸを行う
// 引  数      ： AnsiString f_str   変換対象文字列
// ： double     err     エラー時のデフォルト値
// 戻り値      ： double     小数変換値 または エラー時デフォルト値
// 備  考      ： StrToIntDef() の double 版
// ---------------------------------------------------------------------------
double __fastcall StrToDblDef(AnsiString f_Str, double err) {
    char *p, *endptr;
    double value;

    p = f_Str.c_str();
    value = strtod(p, &endptr); // 小数文字変換
    if (endptr[0] && (endptr == p)) // 変換文字無し？
    {
        return err; // ｴﾗｰ時の値
    }
    return value; // 変換値
}

/* -----------------------------------------------------------------------------
 // 日本語関数名：トークン取得処理
 // 概  要      ：文字列を区切り文字で区切る
 // パラメータ  ：char *Str 文字列
 //             ：char *Sep 区切り文字
 // 戻り値      ：トークン文字列へのポインタ
 // 機能説明    ：文字列を区切り文字で区切る
 // 備  考      ：文字列がNULLの場合は続きのトークンを取得する
 //--------------------------------------------------------------------------- */
char* __fastcall GetToken(char *Str, char *Sep) {
    /* --- C : char* 版 --- */
    static char *astr = NULL; /* 文字列格納ﾎﾟｲﾝﾀ */
    static char *atkn = NULL; /* トークン文字列格納ﾎﾟｲﾝﾀ */
    static int strflg = 0; /* 文字列フラグ */
    static int cnt = 1; /* ループカウンタ */
    static int len = 0; /* 文字長長保持 */
    static int tlen = 0; /* トークン文字格納位置 */

    if (Str) /* トークン処理初回？ */ {
        if (astr != NULL) /* 文字列メモリあり？ */ {
            free(astr); /* メモリの解放 */
            astr = NULL;
        }
        if (atkn != NULL) /* トークン文字列メモリあり？ */ {
            free(atkn); /* トークンメモリの解放 */
            atkn = NULL;
        }
        len = strlen(Str); /* 文字長保持 */
        if ((astr = (char*)malloc(len + 2)) != NULL) /* メモリ取得成功？ */ {
            memset(astr, '\0', (len + 2));
            strncpy(astr, Str, len); /* 文字列をバッファに取得 */
            cnt = 0; /* ループカウンタ初期化 */
            strflg = 0; /* 文字列フラグ初期化] */
        }
        else {
            return NULL;
        }
        atkn = (char*)malloc(len + 2); /* トークンメモリ取得 */
    }
    if ((astr == NULL) || (atkn == NULL)) /* 取得メモリなし？ */ {
        return NULL;
    }
    memset(atkn, '\0', (len + 2)); /* トークン初期化 */
    tlen = 0;

    for (; cnt < len; cnt++) /* 文字列長分 */ {
        if (ByteChrChk(astr[cnt])) /* ２バイト文字の最初の文字？ */ {
            atkn[tlen++] = astr[cnt]; /* ２バイト文字１バイト目取得 */
            atkn[tlen++] = astr[cnt + 1]; /* ２バイト文字２バイト目取得 */
            cnt++; /* ループカウンタ補正 */
            continue; /* 次の文字へ */
        }
        if (astr[cnt] == '\r') {
            cnt++; /* 次の文字へ */
            break; /* ループ脱出 */
        }
        if (astr[cnt] == '\n') /* '\n'(改行コード) */ {
            cnt++; /* 次の文字へ */
            break; /* ループ脱出 */
        }
        if (!strflg) /* 文字列内以外 */ {
            if (astr[cnt] == '\"') /* '"'(ﾀﾞﾌﾞﾙｸｫｰﾃｰｼｮﾝ) */ {
                strflg = 1; /* 文字列フラグ｢有効｣ */
                continue; /* 次の文字へ */
            }
            if (strncmp(&astr[cnt], Sep, strlen(Sep)) == 0) /* 区切り文字？ */ {
                cnt += strlen(Sep); /* 対象文字列更新 */
                break; /* ループ脱出 */
            }
        }
        else {
            /* '"'(ﾀﾞﾌﾞﾙｸｫｰﾃｰｼｮﾝ) and (次の文字が区切り文字 or 改行) */
            if ((astr[cnt] == '\"') && (((astr[cnt + 1] == '\n') ||
                ((astr[cnt + 1] == '\r') && (astr[cnt + 2] == '\n'))) ||
                (strncmp(&astr[cnt + 1], Sep, strlen(Sep)) == 0))) {
                strflg = 0; /* 文字列フラグ｢無効｣ */
                continue; /* 次の文字へ */
            }
        }
        atkn[tlen++] = astr[cnt]; /* 文字追加 */
    }

    if ((tlen == 0) && (cnt >= len)) /* トークン文字列長が｢0｣ かつ 文字列終了？ */ {
        if (astr != NULL) /* 文字列メモリあり？ */ {
            free(astr); /* メモリの解放 */
            astr = NULL;
        }
        if (atkn != NULL) /* トークン文字列メモリあり？ */ {
            free(atkn); /* トークンメモリの解放 */
            atkn = NULL;
        }
        return NULL; /* 戻り値[NULL] */
    }
    else {
        return atkn; /* 戻り値[トークン文字列ポインタ] */
    }
}

/************************************
 ２バイト文字の第１バイト目か判定
 ２バイト文字なら０以外を返す
 *************************************/
int __fastcall ByteChrChk(char a) {
    return _ismbblead((unsigned int)a);
}

// -----------------------------------------------------------------------------
// 関数名　　　：ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙｺﾋﾟｰ処理
// 概  要      ：既存のﾌｫﾙﾀﾞにあるﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙをｺﾋﾟｰする
// パラメータ  ：AnsiString F_filename  // ｺﾋﾟｰ元ﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
// ：           T_filename  // ｺﾋﾟｰ先ﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
// 戻り値      ：0:ｺﾋﾟｰ成功 1:ｺﾋﾟｰ失敗
// 備  考      ：
// -----------------------------------------------------------------------------
bool __fastcall Template_Copy(AnsiString F_filename, AnsiString T_filename) {

    if (CopyFile(F_filename.c_str(), T_filename.c_str(), false)) {
        return true;
    }
    else {
        return false;
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： ログファイル　作成
// 概  要      ： ｴﾗｰ内容に[年月日時分秒]を付与して作成する。
// 引  数      ： AnsiString LogText   ログメッセージ
// 戻り値      ：
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall Write_Log(AnsiString LogText) {
    AnsiString s_LogText;

    // ﾌﾗｸﾞﾁｪｯｸ
    if (G_Log_Write_Flg == 1) {
        return true;
    }

    // ***************************
    // ***  ﾛｸﾞﾌｧｲﾙ　作成
    // ***************************
    s_LogText = "[" + FormatDateTime("yyyy/mm/dd", Date()) + " " +
        FormatDateTime("hh:nn:ss", Time()) + "]  ";

    s_LogText = s_LogText + LogText + "\n";

    FILE *fp;
    AnsiString Err_Msg;

    if ((fp = fopen(G_LogFilePath.c_str(), "a+")) == NULL) {
        fclose(fp);
        // delete fp;
        return false;
    }

    fputs(s_LogText.c_str(), fp);
    fclose(fp);
    // delete fp;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： エラーログファイル　作成
// 概  要      ： ｴﾗｰ内容に[年月日時分秒]を付与して作成する。
// 引  数      ： AnsiString LogText   ログメッセージ
// 戻り値      ：
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall Write_Error_Log(AnsiString LogText) {
    AnsiString s_LogText;

    // ***************************
    // ***  ｴﾗｰﾛｸﾞﾌｧｲﾙ　作成
    // ***************************
    s_LogText = "[" + FormatDateTime("yyyy/mm/dd", Date()) + " " +
        FormatDateTime("hh:nn:ss", Time()) + "]  ";

    s_LogText = s_LogText + LogText + "\n";

    FILE *fp;
    AnsiString Err_Msg;

    if ((fp = fopen(G_ErrLogFilePath.c_str(), "a+")) == NULL) {
        fclose(fp);
        // delete fp;
        return false;
    }

    fputs(s_LogText.c_str(), fp);
    fclose(fp);
    // delete fp;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： エクセル　エラーログセット
// 概  要      ：
// 引  数      ： AnsiString inText   // 書き込み文字列
// 戻り値      ： なし
// 備  考      ： なし
// ---------------------------------------------------------------------------
void __fastcall Excel_Err_DataSet(AnsiString inText) {
    AnsiString s_Old_SheetName;
    AnsiString s_CellText;

    int i;

    // 現在のシート名称を保存
    s_Old_SheetName = exWorksheet.OlePropertyGet("Name");
    // ｼｰﾄの指定
    // exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_ERR_SHEETNAME);
    // Windows 2008Server 移植対応
    exWorksheet = exWorksheets.OlePropertyGet("Item",
        (OleVariant)EXCEL_ERR_SHEETNAME);
    // 最終行にセット
    s_CellText = "Def";
    i = 1;
    while (s_CellText != "") {
        // exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << i << 2);
        // s_CellText = exWorkCell.Exec(PropertyGet("Value"));
        // Windows 2008Server 移植対応
        exWorkCell = exWorksheet.OlePropertyGet("Cells", i, 2);
        s_CellText = exWorkCell.OlePropertyGet("Value");

        i++;
    }

    // エラーログ書き込み
    // exWorkCell.Exec(PropertySet("Value") << inText );
    // 保存しておいたシートを再指定
    // exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);
    // Windows 2008Server 移植対応
    exWorkCell.OlePropertySet("Value", (OleVariant)inText);
    exWorksheet = exWorksheets.OlePropertyGet("Item",
        (OleVariant)s_Old_SheetName);

}

// ---------------------------------------------------------------------------
// 日本語関数名： エクセル　セルデータセット
// 概  要      ：
// 引  数      ： int Row            // 行番号
// ： int Col            // 列番号
// ： AnsiString inText  // 書き込み文字列
// 戻り値      ： なし
// 備  考      ： なし
// ---------------------------------------------------------------------------
void __fastcall Excel_Cell_DataSet(int Row, int Col, AnsiString inText) {
    if (inText.SubString(1, 1) != "'")
        inText = "'" + inText;

    // exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << Row << Col);
    // exWorkCell.Exec(PropertySet("Value") << inText );
    // Windows 2008Server 移植対応
    exWorkCell = exWorksheet.OlePropertyGet("Cells", Row, Col);
    exWorkCell.OlePropertySet("Value", (OleVariant)inText);
}

// ---------------------------------------------------------------------------
// 日本語関数名： エクセル　セルデータゲット（OGI 2015.02.20）
// 概  要      ：
// 引  数      ： int Row            // 行番号
//             ： int Col            // 列番号
// 戻り値      ： AnsiString OutText // 読み込み文字列
// 備  考      ： なし
// ---------------------------------------------------------------------------
AnsiString __fastcall Excel_Cell_DataGet(int Row, int Col) {
    exWorkCell = exWorksheet.OlePropertyGet("Cells", Row, Col);
	return exWorkCell.OlePropertyGet("Value");
}

//Add_Str 2019/02/22 Ri
// ---------------------------------------------------------------------------
// 日本語関数名： エクセル　下に罫線（OGI 2019.02.22）
// 概  要      ：
// 引  数      ： int Row            // 行番号
//             ： int Col            // 列番号
// 戻り値      ： なし
// 備  考      ： なし
// ---------------------------------------------------------------------------
void __fastcall Excel_Cell_SetUnderline(int Row1, int Col1, int Row2, int Col2){
	//exWorkCell = exWorksheet.OlePropertyGet("Cells", Row, Col);
	//exWorkCell.OlePropertySet("Underline",true);
	//exWorkCell.OlePropertyGet("Font").OlePropertySet("Underline",true);

	exWorksheet.OlePropertyGet("Range", exWorksheet.OlePropertyGet("Cells", Row1, Col1), exWorksheet.OlePropertyGet("Cells", Row2, Col2)).OlePropertyGet("Borders").OlePropertyGet("Item", 9).OlePropertySet("LineStyle",1);

	//exWorkCell.OlePropertyGet("Borders").OlePropertyGet("Item", 9).OlePropertySet("LineStyle",1);


}
//Add_End 2019/02/22 Ri

// ---------------------------------------------------------------------------
// 日本語関数名： エクセル　ヘッダセット
// 概  要      ： 標準部品表で、舶用の場合のみヘッダに「分類３」を出力
// 引  数      ： -
// 戻り値      ： なし
// 備  考      ： なし
//
// 2007.11.15 ミラー仕様対応 ミラー時は"分類５"を出力する
// ---------------------------------------------------------------------------
void __fastcall Excel_HeaderSet(bool bMirror) {
    AnsiString sOutput;

    // --------------------------------------
    // 2007.11.15 ミラー仕様時の出力を変更
    if (!bMirror) {
        sOutput = "分類３　&P/&N";
    }
    else {
        sOutput = "分類５　&P/&N";
    }
    // 2007.11.15
    // --------------------------------------

    // exWorkCell = exWorksheet.Exec(PropertyGet("PageSetUp"));
    // exWorkCell.Exec(PropertySet("RightHeader") << sOutput );
    // Windows 2008Server 移植対応
    exWorkCell = exWorksheet.OlePropertyGet("PageSetUp");
    exWorkCell.OlePropertySet("RightHeader", (OleVariant)sOutput);
}

// ---------------------------------------------------------------------------
// 日本語関数名： エクセル　標準部品表ページ削除（セル削除）
// 概  要      ：
// 引  数      ： int Row            // 行番号
// ： int Col            // 列番号
// ： AnsiString inText  // 書き込み文字列
// 戻り値      ： なし
// 備  考      ： なし
// ---------------------------------------------------------------------------
void __fastcall Excel_Cell_Delete(int iLastPage) {
	AnsiString sDeleteCells;
    sDeleteCells = "A" + FormatFloat("0", DefExcelPageLine * iLastPage + 1) +
        ":CD500";
	exWorkCell = exWorksheet.Exec(PropertyGet("Range") << sDeleteCells).Exec
        (Function("Delete"));
}

// ---------------------------------------------------------------------------
// 日本語関数名： 全角文字→半角文字
// 概  要      ：
// 引  数      ： AnsiString str   変換対象文字列
// 戻り値      ： 変換後の結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall ZenToHan(AnsiString str) {
    AnsiString strw;
    char *c1;

    c1 = (char*)GlobalAlloc(GPTR, str.Length() + 1);

    LCMapString(GetUserDefaultLCID(), LCMAP_HALFWIDTH, str.c_str(),
        str.Length() + 1, c1, str.Length() + 1);
    strw = c1;
    GlobalFree(c1);
    return strw;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 半角文字→全角文字
// 概  要      ：
// 引  数      ： AnsiString str   変換対象文字列
// 戻り値      ： 変換後の結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString HanToZen(AnsiString str) {
    char s[256];
    LCMapString(GetUserDefaultLCID(), LCMAP_FULLWIDTH, str.c_str(),
        str.Length() + 1, s, 256);

    return AnsiString(s);
}

// ---------------------------------------------------------------------------
// 日本語関数名： 規格ｺｰﾄﾞﾃｰﾌﾞﾙ　検索
// 概  要      ：
// 引  数      ： AnsiString Code  // 検索対象ｺｰﾄﾞ
// bool bWaEi
// bool bDocType
// AnsiString LangCode   // 和文英文
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code, bool bWaEi,
    bool bDocTyp, AnsiString LangCode) {
    int i;

    AnsiString s_Name;

    s_Name = "";
    // if ( Code == "" ) return "";
    for (i = 0; i < G_KIKAKU_RecCnt; i++) {
        if (G_KIKAKU[i][0].ToIntDef(0) == Code.ToIntDef(0)) {
            if (bDocTyp == true) {
                if (bWaEi == true) {
                    if (Code.ToIntDef(0) == 0) {

                        // ---------------------------------------
                        // 2007.10.04 ﾐﾗｰ仕様対応
                        // 関数引数を使用する方式に変更
                        // if (G_HEADS_DATA[1].TrimRight() == "0") {    //和文
                        if (LangCode == "0") {
                            // ---------------------------------------

                            s_Name = G_KIKAKU[i][1] + "（銘板　和文）";
                        }
                        else {
                            s_Name = G_KIKAKU[i][1] + "（銘板　英文）";
                        }
                    }
                    else {
                        s_Name = G_KIKAKU[i][1];
                    }
                }
                else {
                    s_Name = G_KIKAKU[i][2];
                }
            }
            else {
                if (bWaEi == true) {
                    s_Name = G_KIKAKU[i][3];
                }
                else {
                    s_Name = G_KIKAKU[i][4];
                }
            }
            break;
        }
    }

    return s_Name;
}

////---------------------------------------------------------------------------
//// 日本語関数名： 規格ｺｰﾄﾞﾃｰﾌﾞﾙ　検索
//// 概  要      ：
//// 引  数      ： AnsiString Code  // 検索対象ｺｰﾄﾞ
//// 戻り値      ： 検索結果
//// 備  考      ：
////---------------------------------------------------------------------------
// AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code, bool bWaEi)
// {
// int i;
//
// AnsiString s_Name;
//
// s_Name = "";
// if ( Code == "" ) return "";
// for(i=0; i < G_KIKAKU_RecCnt; i++)
// {
// if (G_KIKAKU[i][0] == Code){
// if ( bWaEi == true ) {
// if ( Code == "00" ) {
// if (G_HEADS_DATA[1].TrimRight() == "0") {    //和文
// s_Name = G_KIKAKU[i][1] + "（銘板　和文）";
// } else {
// s_Name = G_KIKAKU[i][1] + "（銘板　英文）";
// }
// } else {
// s_Name = G_KIKAKU[i][1];
// }
// } else {
// s_Name = G_KIKAKU[i][2];
// }
// break;
// }
// }
//
// return s_Name;
// }

// ---------------------------------------------------------------------------
// 日本語関数名： 規格ｺｰﾄﾞﾃｰﾌﾞﾙ　検索
// 概  要      ：
// 引  数      ： AnsiString Code  // 検索対象ｺｰﾄﾞ
// ：            Sw    // J:和文，E:英文
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_TOSOU_CODE(AnsiString Code, AnsiString Sw) {
    int i;

    AnsiString s_Name;

    s_Name = "";

    for (i = 0; i < G_TOSOU_RecCnt; i++) {
        if (G_TOSOU[i][0] == Code) {
            if (Sw == "J") {
                s_Name = G_TOSOU[i][1]; // 和文
            }
            else {
                s_Name = G_TOSOU[i][2]; // 英文
            }
            break;
        }
    }

    return s_Name;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HEADSデータ　読込
// 概  要      ： 引数で渡されたHEADSﾌｧｲﾙを読み込み文字列配列にセットする。
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall Read_HEADS_Data(AnsiString filepass) {
    char buf[1024]; /* 文字列バッファ */
    char *tok; /* トークンポインタ */
    int i; /* ループカウンタ */

    AnsiString wk_FilePass;
    FILE *fp;

    // *******************************
    // ***  HEADSデータ　読込
    // *******************************
    wk_FilePass = filepass;
    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        fclose(fp); // 閉じる
        return false;
    }

    // 読込み可能の間
    i = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        tok = GetToken(buf, "\n");
        G_HEADS_DATA[i] = tok;
        // コンデンサ対応？
        if (i == 34) {
            if (G_HEADS_DATA[i].SubString(3, 1) == "C") {
                G_HEADS_DATA[i] = G_HEADS_DATA[i].SubString(1, 2);
            }
        }
        i++;
    }

    // ***************************************
    // No:0～315 のﾃﾞｰﾀを No:507～822にｺﾋﾟｰ
    // 実際のHEADSﾃﾞｰﾀには格納されていない為。
    // (02.08.21追加)
    // ***************************************
    for (i = 507; i < 822; i++) {
        //2019.10.10 NK量産外注記_S　　　
        //if (i != 539) { // 塗装色の和英対応のため539を使用。上書きするとマズいのでチェックで外す   2003/06/09
        if (i == 537) {
            // ＮＫ量産外型式注意喚起出力のため537を使用。上書きするとマズいのでチェックで外す
        } else if (i == 539) {
            // 塗装色の和英対応のため539を使用。上書きするとマズいのでチェックで外す
        } else {
        //2019.10.10 NK量産外注記_E
            G_HEADS_DATA[i] = G_HEADS_DATA[i - 507];
        }
    }

    fclose(fp); // 閉じる
    // delete fp;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HEADSデータ　読込
// 概  要      ： HEADSﾌｧｲﾙを読み込み、リビジョンデータ(318)を取得する。
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2004/10/25　add
// ---------------------------------------------------------------------------
bool __fastcall Read_HEADS_Data_Rev(AnsiString filepass) {
    char buf[1024]; /* 文字列バッファ */
    char *tok; /* トークンポインタ */
    int i; /* ループカウンタ */
    AnsiString work; /* 作業用 */
    AnsiString work_DATA[1500];

    AnsiString wk_FilePass;
    FILE *fp;

    // *******************************
    // ***  HEADSデータ　読込
    // *******************************
    wk_FilePass = filepass;
    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        fclose(fp); // 閉じる
        return false;
    }

    // 読込み可能の間
    i = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        tok = GetToken(buf, "\n");
        work_DATA[i] = tok;

        // change 2005-09-22 リビジョン番号は319行目？
        // if (i == 317){
        if (i == 319) {
			G_HEADS_DATA_Rev = work_DATA[i].ToIntDef(0);
            break;
        }
        i++;
    }

    fclose(fp); // 閉じる
    // delete fp;
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 予備品ﾃﾞｰﾀ　集計処理(ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀ作成用)
// 概  要      ： 予備品ﾃﾞｰﾀとﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙから予備品数を求める(1件のみ)
// 引  数      ： AnsiString a_Katasiki : ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ 5列目
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 02.08.03追加
// ---------------------------------------------------------------------------
int __fastcall Syukei_Buhin_Yobisu(AnsiString a_Katasiki, AnsiString a_Zaisitu,
    HeadsData* pcHeadsData) {
    // CSV読込用
    FILE *wkfp;

    char buf[1024]; // 文字列バッファ
    char *tok; // トークンポインタ
    int i; // ループカウンタ
    char token[100][100]; // トークンバッファ

    AnsiString s_Yobi_Text[5];
    AnsiString s_YBKey;
    AnsiString s_YBZaisitu;

    AnsiString wk_FilePass; // "D00001.csv"
    int i_RecNo;
    int i_Yobisu;
    int i_lop;

    i_RecNo = 0; // ﾚｺｰﾄﾞｶｳﾝﾀ(抽出条件一致件数)
    i_Yobisu = 0;

    // ---------------------------------------
    // 2007.10.04 ﾐﾗｰ仕様対応
    // ﾐﾗｰB仕様の場合、予備品を計上しない
    if (pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B)
        return 0;
    // 2007.10.04
    // ---------------------------------------

    // 一度計上した予備品は計上しない
    for (i_lop = 0; i_lop < G_YOBICHK_RecCnt; i_lop++) {
        if ((a_Katasiki == G_YOBICHK_NAME[i_lop]) &&
            (a_Zaisitu == G_YOBICHK_MAT[i_lop])) {
            return 0;
        }
    }

	// ﾌﾟﾚｰﾄ予備品ﾃﾞｰﾀﾊﾟｽ
	wk_FilePass = G_AnaFilePass + G_SAKUBAN + ".csv";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "予備品ﾃﾞｰﾀ 『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // 読込み可能の間
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), wkfp) != NULL) {
            i = 0; // トークンカウンタ初期化
            memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
            tok = GetToken(buf, ","); // ","で項目を区切る
            while (tok) { // トークンが存在する間
                sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
                tok = GetToken(NULL, ","); // 次のトークンを検索

                if (i > 100) {
                    break;
                }
            }
            // ﾃﾞｰﾀ例
            s_Yobi_Text[0] = token[0];
            // 予備品ﾃﾞｰﾀ 1列目 ( 'PLTE','GSKT','ADHS','THRM'... )
            s_Yobi_Text[1] = token[1];
            // 予備品ﾃﾞｰﾀ 2列目 ( ﾌﾞﾗﾝｸ,'D','E','1','2'... )
            s_Yobi_Text[2] = token[8]; // 予備品ﾃﾞｰﾀ 予備数( 10, 1, 5, 2, 8, ... )
            s_Yobi_Text[3] = token[11]; // 予備品ﾃﾞｰﾀ 材質コード( 1011, ... )

            // 検索ｷｰ生成
            if (AllNumChk(s_Yobi_Text[1].c_str(), 0) == 0) {
                int i_No;

                i_No = StrToInt(s_Yobi_Text[1]) + 100;
                s_YBKey = s_Yobi_Text[0] + IntToStr(i_No).SubString(2, 2);
                s_YBZaisitu = s_Yobi_Text[3].TrimRight();
            }
            else {
                // ********************************************
                // 予備品ﾃﾞｰﾀ 2列目が
                // ｱﾙﾌｧﾍﾞｯﾄもしくはNULLだった場合 検索しない?
                // ********************************************
                s_YBKey = "";
            }

            if (s_YBKey != "\0") {
                if (s_YBKey == a_Katasiki) {
                    if (((a_Zaisitu != "0") && (s_YBZaisitu == a_Zaisitu)) ||
                        (a_Zaisitu == "0")) {
                        i_Yobisu += s_Yobi_Text[2].ToIntDef(0);
                        i_RecNo++;
                        G_YOBICHK_NAME[G_YOBICHK_RecCnt] = a_Katasiki;
                        G_YOBICHK_MAT[G_YOBICHK_RecCnt] = a_Zaisitu;
                        G_YOBICHK_RecCnt++;
                    }
                }
            }
        }
    }

    // 該当予備品ﾃﾞｰﾀが存在しなかった場合
    if (i_RecNo == 0)
        i_Yobisu = 0;

    // 予備品ﾃﾞｰﾀ ﾌｧｲﾙｸﾛｰｽﾞ
    fclose(wkfp);
    return i_Yobisu;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HD_MATERIALﾃｰﾌﾞﾙ検索処理
// 概  要      ：
// 引  数      ： AnsiString Key   ： 材質ｺｰﾄﾞ
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_HD_MATERIAL(AnsiString Key) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;
    AnsiString s_Name;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
    s_SQL = s_SQL + "  WHERE MAT_CODE = '" + Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_MAT;
    wkQuery->ConnectionName = ODBC_DSN_MAT;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // 各項目の初期化
    s_Name = "";

    while (!wkQuery->Eof) {
        // if(G_HEADS_DATA[1].TrimRight() == "0" ){
        // 和文名称の取得
        s_Name = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
        // } else {
        // s_Name = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
        // }

        wkQuery->Next();
    }

    delete wkQuery;
    return s_Name;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HD_MATERIALﾃｰﾌﾞﾙ検索処理
// 概  要      ：
// 引  数      ： AnsiString Key   ： 材質ｺｰﾄﾞ
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_HD_E_MATERIAL(AnsiString Key) {
	// 2021.06.18 FireDACへの移行 MOD_S
	//TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
	TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
	// 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;
    AnsiString s_Name;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
    s_SQL = s_SQL + "  WHERE MAT_CODE = '" + Key + "'";

    wkQuery->Close();
	// 2021.06.18 FireDACへの移行 MOD_S
	//wkQuery->DatabaseName = ODBC_DSN_MAT;
	wkQuery->ConnectionName = ODBC_DSN_MAT;
	// 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // 各項目の初期化
    s_Name = "";

    while (!wkQuery->Eof) {
        // if(G_HEADS_DATA[1].TrimRight() == "0" ){
        // 和文名称の取得
        // s_Name = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
        // } else {
        // 英文名称の取得
        s_Name = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
        // }

        wkQuery->Next();
    }

    delete wkQuery;
    return s_Name;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HD_MATERIALﾃｰﾌﾞﾙ検索処理
// 概  要      ：
// 引  数      ： AnsiString Key   ： 材質ｺｰﾄﾞ
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_HD_MAT_JtoE(AnsiString Key) {
	// 2021.06.18 FireDACへの移行 MOD_S
	//TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
	TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
	// 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;
    AnsiString s_Name;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
    s_SQL = s_SQL + "  WHERE JAPANESE_NAME = '" + Key + "'";

    wkQuery->Close();
	// 2021.06.18 FireDACへの移行 MOD_S
	//wkQuery->DatabaseName = ODBC_DSN_MAT;
	wkQuery->ConnectionName = ODBC_DSN_MAT;
	// 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // 各項目の初期化
    s_Name = "";

    while (!wkQuery->Eof) {
        // 英文名称の取得
        s_Name = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();

        wkQuery->Next();
    }

    delete wkQuery;
    return s_Name;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 材質ｺｰﾄﾞ変換ﾏｽﾀ　変換後コード取得
// 概  要      ： 引数として渡されたｺｰﾄﾞで材質ｺｰﾄﾞ変換ﾏｽﾀを検索する。
// 引  数      ： Code：検索用ｺｰﾄﾞ
// ： Type：変換タイプ
// 戻り値      ： 変換結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall Chg_Zaisitu_Code(AnsiString Code, AnsiString Typ) {
    // 2021.06.18 FireDACへの移行 MOD_S
	//TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
	TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
	// 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_Name;
    AnsiString s_SQL;
    AnsiString s_ChgName;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_CHG_MATERIAL";
    s_SQL = s_SQL + "  WHERE MAT_CODE_KEY = '" + Code + "';";

    wkQuery->Close();
	// 2021.06.18 FireDACへの移行 MOD_S
	//wkQuery->DatabaseName = ODBC_DSN_DNO;
	wkQuery->ConnectionName = ODBC_DSN_DNO;
	// 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        s_Name = "";
    }
    else {
        s_ChgName = "MAT_CODE_RESULT" + Typ;
        s_Name = wkQuery->FieldByName(s_ChgName)->AsString.TrimRight();
    }

    delete wkQuery;

    return s_Name;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HD_FRNG_MSTﾃｰﾌﾞﾙ検索処理
// 概  要      ：
// 引  数      ：
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_HD_FRNG_MST(AnsiString Key, AnsiString FSize,
    AnsiString SEDiv, AnsiString PG, AnsiString TMP, AnsiString Drn,
    AnsiString Air, bool bRing) {
	// 2021.06.18 FireDACへの移行 MOD_S
	//TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
	wkQuery = new TFDQuery(DataModule1);
	// 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;
    AnsiString s_Tankan;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_FRNG_MST";
    s_SQL = s_SQL + "  WHERE SERIES_MODEL = '" + Key + "'";
	// 2021.06.24 SQL文の対応　MOD_S
	//s_SQL = s_SQL + "    AND SIZE = '" + FSize + "'";
	s_SQL = s_SQL + "    AND [SIZE] = '" + FSize + "'";
	// 2021.06.24 SQL文の対応　MOD_E
    s_SQL = s_SQL + "    AND SEDIV = '" + SEDiv + "'";
    s_SQL = s_SQL + "    AND PGAGE = '" + PG + "'";
    //2020.01.08 ﾘﾝｸﾞ端管検索変更_S
    //if (bRing == false)
    //    s_SQL = s_SQL + "    AND TEMP = '" + TMP + "'"; // リング端管のＰ座有無は見ない
    s_SQL = s_SQL + "    AND TEMP = '" + TMP + "'";
    //2020.01.08 ﾘﾝｸﾞ端管検索変更_E
    s_SQL = s_SQL + "    AND DRN = '" + Drn + "'";
    s_SQL = s_SQL + "    AND AIR = '" + Air + "'";

    wkQuery->Close();
	// 2021.06.18 FireDACへの移行 MOD_S
	//wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
	// 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        s_Tankan = "";
    }
    else {
        s_Tankan = wkQuery->FieldByName("TANKAN")->AsString.TrimRight(); // 図番

        G_Log_Text = "端管名称    ｢" + s_Tankan + "｣を取得。";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;
    return s_Tankan;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HD_FRNG_MSTﾃｰﾌﾞﾙ検索処理
// 概  要      ：
// 引  数      ：
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall Get_Yobi_Code(AnsiString Key) {
    AnsiString sRetCode;

    switch (Key.ToIntDef(0)) {
    case 6: // Ｄプレートガスケット
        sRetCode = "GSKT02";
        break;
    case 7: // ＤプレートガスケットＡ
        sRetCode = "GSKT08";
        break;
    case 8: // ＤプレートガスケットＢ
        sRetCode = "GSKT09";
        break;
    case 55: // EプレートガスケットＡ
    case 56: // ＥプレートガスケットＢ
    case 57: // Ｅプレートガスケット
    case 60: // ＥプレートガスケットＥ
        sRetCode = "GSKT03";
        break;
    case 13: // Ｄディスタンスピース
    case 78: // ディスタンスピース
        sRetCode = "GSKT06";
        break;
    case 15: // ＤプレートガスケットＢ－４
        sRetCode = "GSKT13";
        break;
    case 16: // ＤプレートガスケットＡ－２
        sRetCode = "GSKT12";
        break;
    case 79: // Ｅノズルガスケット
        sRetCode = "GSKT04";
        break;
    case 64: // Ｅディスタンスピース
        sRetCode = "GSKT14";
        break;
    default:
        sRetCode = "";
        break;
    }

    return sRetCode;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 型式総称変換
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall Get_PHE_Model(AnsiString *rs_Syurui, AnsiString *rs_Model,
    AnsiString s_Syurui, AnsiString s_Model) {

    int i;
    int i_Mojisu;
    AnsiString s_Text;
    // AnsiString s_Syurui;
    // AnsiString s_Model;

    // ****************************************************************************
    // ***                                                                      ***
    // ***                        プレート型式の決定                            ***
    // ***                                                                      ***
    // ****************************************************************************

    // -----------------------------------------------
    // 2007.10.04 関数引数から取得するよう変更
    // s_Syurui = G_HEADS_DATA[34].TrimRight();                // HEADSﾃﾞｰﾀ 種類
    // -----------------------------------------------
    // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ

    // RecNo.542ﾓﾃﾞﾙの取得方法の変更
    // -----------------------------------------------
    // 2007.10.04 関数引数から取得するよう変更
    // s_Model  = G_HEADS_DATA[35].TrimRight(); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
    // -----------------------------------------------
    // 2009.06.19 UX-100R追加
    // EXの場合
    // s_Model後端部に"C","P","R","A","B"がある場合
    // その文字を順にs_Modelから除いていく
    //
    // EX以外の場合
    // s_Model後端部に"C","P","A","B"がある場合
    // その文字を順にs_Modelから除いていく
    // 次に条件に合わせて以下の文字ををs_Modelから除く
    // 最後の文字が"D"の場合
    // ：最後から２文字目の文字が"C","P","A","B"の場合
    // ：その文字を順にs_Modelから除いていく
    // ：次に最後から２文字目の文字(数字)を除く
    // 最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
    // その他の場合                       ：最後の文字(数字)
    ////2006.02.06 SX-90M,L追加
    ////s_Model後端部に"C","P","R","A","B"がある場合
    ////その文字を順にs_Modelから除いていく
    ////次に条件に合わせて以下の文字ををs_Modelから除く
    ////EXの場合                           ：除かない
    ////2008.07.24 CX-10D追加
    ////最後の文字が"D"の場合
    ////　　：最後から２文字目の文字が "C","P","R","A","B"の場合
    ////　　：その文字を順にs_Modelから除いていく
    ////　　：次に最後から２文字目の文字(数字)を除く
    ////*********************
    ////最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
    ////その他の場合                       ：最後の文字(数字)
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // if (i_Mojisu == 3){
    // s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,2);
    // } else if (i_Mojisu == 4){
    // s_Text = G_HEADS_DATA[35].TrimRight().SubString(4,1);
    // if (s_Text.ToIntDef(999) != 999){
    // s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,3);
    // } else {
    // s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,2);
    // }
    // } else if (i_Mojisu == 5){
    // s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,3);
    // }
    if (s_Model.SubString(s_Model.Length(), 1) == "B") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    if (s_Model.SubString(s_Model.Length(), 1) == "A") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    // 2009.06.19 UX-100R追加
    // if ( s_Model.SubString(s_Model.Length(),1) == "R" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
    if (s_Syurui.SubString(1, 2) == "EX" && s_Model.SubString(s_Model.Length(),
        1) == "R") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    // **********************
    if (s_Model.SubString(s_Model.Length(), 1) == "P") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    if (s_Model.SubString(s_Model.Length(), 1) == "C") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    if (s_Syurui.SubString(1, 2) == "EX") {
        // 2008.07.24 CX-10D追加
    }
    else if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "B") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "A") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }
        // 2009.06.19 UX-100R追加
        // if ( s_Model.SubString(s_Model.Length()-1,1) == "R" ) {
        // s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
        // }
        // **********************
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "P") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "C") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }

        s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        // *********************
    }
    else if (s_Model.SubString(s_Model.Length(), 1).ToIntDef(999) == 999) {
        s_Model = s_Model.SubString(1, s_Model.Length() - 2) + s_Model.SubString
            (s_Model.Length(), 1);
    }
    else {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    // ***********************

    *rs_Syurui = ""; // 総称種類
    *rs_Model = ""; // 総称ﾓﾃﾞﾙ

    G_Log_Text = "ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙを、種類『" + s_Syurui + "』モデル『" + s_Model +
        "』で検索開始。";
    Write_Log(G_Log_Text);

    for (i = 0; i < G_PHENKAN_RecCnt; i++) {
        if (G_PHENKAN[i][0] == s_Syurui && G_PHENKAN[i][1] == s_Model) {

            *rs_Syurui = ZenToHan(G_PHENKAN[i][2]); // 総称種類
            *rs_Model = ZenToHan(G_PHENKAN[i][3]); // 総称ﾓﾃﾞﾙ

            break;
        }
    }

    G_Log_Text = "総称種類            『" + *rs_Syurui + "』を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "総称ﾓﾃﾞﾙ            『" + *rs_Model + "』を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙ  検索終了。";
    Write_Log(G_Log_Text);

    return true;
}

// -----------------------------------------------------------------------------
// 日本語関数名： ノズル継手の規格を取得
// 概  要      ：
// パラメータ  ： 規格コード
// 戻り値      ： 0:JIS 1:ANSI/JPI 2:サニタリ
// 機能説明    ：
// 備  考      ：
// -----------------------------------------------------------------------------
int __fastcall GetNzKikaku(AnsiString a_KCd) {
    int i_Kikaku;

    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if (a_KCd.SubString(1,1) == "6" ) {        // サニタリ
    // i_Kikaku = 2;
    // } else if (a_KCd.SubString(1,1) == "8" ||
    // a_KCd.SubString(1,1) == "9" ||
    // a_KCd.SubString(1,1) == "4" ||
    // a_KCd.SubString(1,1) == "5") {
    if (a_KCd.SubString(1, 2) == "06") { // サニタリ
        i_Kikaku = 2;
        // 2014.06.24 ﾉｽﾞﾙ規格変更
        // } else if (a_KCd.SubString(1,2) == "04" ||
        // a_KCd.SubString(1,2) == "05" ||
        // a_KCd.SubString(1,2) == "08" ||
        // a_KCd.SubString(1,2) == "09" ||
        // a_KCd.SubString(1,2) == "11" ||
        // a_KCd.SubString(1,2) == "12" ||
        // a_KCd.SubString(1,2) == "13" ||
        // a_KCd.SubString(1,2) == "21" ||
        // a_KCd.SubString(1,2) == "22" ||
        // a_KCd.SubString(1,2) == "23") {
        //// ***********************
        // i_Kikaku = 1;
        //// 2013.04.03 ﾉｽﾞﾙ規格追加
        // } else if (a_KCd.SubString(1,2) == "14" ||
        // a_KCd.SubString(1,2) == "24" ||
        // a_KCd.SubString(1,2) == "31" ||
        // a_KCd.SubString(1,2) == "32" ||
        // a_KCd.SubString(1,2) == "33" ||
        // a_KCd.SubString(1,2) == "34" ||
        // a_KCd.SubString(1,2) == "44" ||
        // a_KCd.SubString(1,2) == "48") {
        // i_Kikaku = 1;
        //// *********************
    }
    else if (a_KCd.SubString(1, 2) == "04" || a_KCd.SubString(1, 2) == "05" ||
        a_KCd.SubString(1, 2) == "08" || a_KCd.SubString(1, 2) == "09" ||
        a_KCd.SubString(1, 2) == "44" || a_KCd.SubString(1, 2) == "48") {
        // ANSI,JPI
        i_Kikaku = 1;
        // *************************
    }
    else {
        i_Kikaku = 0;
    }
    return i_Kikaku;
}

// ---------------------------------------------------------------------------
// 日本語関数名： フランジシリカゲル量取得
// 概  要      ： Silicagel.TXTからフランジシリカゲル量を取得する
// 引  数      ： AnsiString Key
// 戻り値      ： シリカゲル量(kg)
// 備  考      ： 2020/03/12 追加
// ---------------------------------------------------------------------------
AnsiString __fastcall Get_Silicagel( AnsiString Key ) {
    // CSV読込用
    FILE *wkfp;

    char buf[1024]; // 文字列バッファ
    char *tok; // トークンポインタ
    int i; // ループカウンタ
    char token[100][100]; // トークンバッファ

    AnsiString s_Silicagel;
    AnsiString wk_FilePass;
    int i_RecNo;
    int i_Yobisu;
    int i_lop;

    i_RecNo = 0; // ﾚｺｰﾄﾞｶｳﾝﾀ(抽出条件一致件数)
    i_Yobisu = 0;

    // Silicagel.TXT ﾃﾞｰﾀﾊﾟｽ
    wk_FilePass = G_AnaFilePass + "Silicagel.TXT";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "Silicagel.TXT 『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // 読込み可能の間
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), wkfp) != NULL) {
            i = 0; // トークンカウンタ初期化
            memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
            tok = GetToken(buf, ","); // ","で項目を区切る
            while (tok) { // トークンが存在する間
                sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
                tok = GetToken(NULL, ","); // 次のトークンを検索

                if (i > 100) {
                    break;
                }
            }
        }
    }

    // ｼﾘｶｹﾞﾙﾃﾞｰﾀが存在しなかった場合
    if ( Key == "A" ) {
        if (token[0] == "") {
            s_Silicagel = "  0.00";
        }
        else {
            s_Silicagel = token[0];
        }
    }
    else {
        if (token[1] == "") {
            s_Silicagel = "  0.00";
        }
        else {
            s_Silicagel = token[1];
        }
    }

    // Silicagel.TXT ﾌｧｲﾙｸﾛｰｽﾞ
    fclose(wkfp);
    return s_Silicagel;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 温風乾燥注記取得
// 概  要      ： HotAirDrying.TXTから温風乾燥注記を取得する
// 引  数      ： なし
// 戻り値      ： 温風乾燥注記
// 備  考      ： 2020/03/12 追加
// ---------------------------------------------------------------------------
AnsiString __fastcall Get_HotAirDrying(void) {
    // CSV読込用
    FILE *wkfp;

    char buf[1024]; // 文字列バッファ
    char *tok; // トークンポインタ
    int i; // ループカウンタ
    char token[100][100]; // トークンバッファ

    AnsiString s_HotAirDrying;
    AnsiString wk_FilePass;
    int i_RecNo;
    int i_Yobisu;
    int i_lop;

    i_RecNo = 0; // ﾚｺｰﾄﾞｶｳﾝﾀ(抽出条件一致件数)
    i_Yobisu = 0;

    // HotAirDrying.TXT ﾃﾞｰﾀﾊﾟｽ
    wk_FilePass = G_AnaFilePass + "HotAirDrying.TXT";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "HotAirDrying.TXT 『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // 読込み可能の間
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), wkfp) != NULL) {
            i = 0; // トークンカウンタ初期化
            memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
            tok = GetToken(buf, ","); // ","で項目を区切る
            while (tok) { // トークンが存在する間
                sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
                tok = GetToken(NULL, ","); // 次のトークンを検索

                if (i > 100) {
                    break;
                }
            }
        }
    }

    // 温風乾燥注記が存在しなかった場合
    if (token[0] == "") {
        s_HotAirDrying = "";
    }
    else {
        s_HotAirDrying = token[0];
    }

    // HotAirDrying.TXT ﾌｧｲﾙｸﾛｰｽﾞ
    fclose(wkfp);
    return s_HotAirDrying;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 処理作番取得
// 概  要      ： HD_sakuban.txtから処理対象の作番を取得する
// 引  数      ： -
// 戻り値      ： 処理結果 [作番]
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall Get_Sakuban_File(void) {
    // CSV読込用
    FILE *wkfp;

    char buf[1024]; // 文字列バッファ
    char *tok; // トークンポインタ
    int i; // ループカウンタ
    char token[100][100]; // トークンバッファ

    AnsiString s_sakubanTXT;
    AnsiString wk_FilePass;
    int i_RecNo;
    int i_Yobisu;
    int i_lop;

    i_RecNo = 0; // ﾚｺｰﾄﾞｶｳﾝﾀ(抽出条件一致件数)
    i_Yobisu = 0;

    // ﾌﾟﾚｰﾄ予備品ﾃﾞｰﾀﾊﾟｽ
    wk_FilePass = G_HD_Root + "HD_sakuban.txt";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "予備品ﾃﾞｰﾀ 『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // 読込み可能の間
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), wkfp) != NULL) {
            i = 0; // トークンカウンタ初期化
            memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
            tok = GetToken(buf, ","); // ","で項目を区切る
            while (tok) { // トークンが存在する間
                sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
                tok = GetToken(NULL, ","); // 次のトークンを検索

                if (i > 100) {
                    break;
                }
            }
        }
    }

    // 該当予備品ﾃﾞｰﾀが存在しなかった場合
    if (token[0] == "")
        s_sakubanTXT = 0;
    else
        s_sakubanTXT = token[0];

    // 予備品ﾃﾞｰﾀ ﾌｧｲﾙｸﾛｰｽﾞ
    fclose(wkfp);
    return s_sakubanTXT;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 液抜き口径表記変換
// 概  要      ：
// 引  数      ：
// 戻り値      ： 変換結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall ChgIDFSize(double dSize) {
    AnsiString sSizeStr;

    sSizeStr = "";
    if (dSize == 1.0) {
        sSizeStr = "1";
    }
    else if (dSize == 1.5) {
        // 2005.02.23 少数に変更
        // sSizeStr = "1 1/2";
        sSizeStr = "1.5";
    }
    else if (dSize == 2.0) {
        sSizeStr = "2";
    }
    else if (dSize == 2.5) {
        // 2005.02.23 少数に変更
        // sSizeStr = "2 1/2";
        sSizeStr = "2.5";
    }
    else if (dSize == 3.0) {
        sSizeStr = "3";
    }
    else if (dSize == 4.0) {
        sSizeStr = "4";
    }
    return sSizeStr;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 材質種類の取得
// 概  要      ：
// 引  数      ： AnsiString Key   ： 材質ｺｰﾄﾞ
// 戻り値      ： 材質の種類
// 備  考      ： 2016.08.29追加
// ---------------------------------------------------------------------------
AnsiString __fastcall Type_MAT(AnsiString Key) {

    AnsiString s_Type;

    // 各項目の初期化
    s_Type = "";

    if ( Key == "1014" || Key == "3014" || Key == "3114" ||  Key == "3214" ||
         Key == "3314" || Key == "3414" ) {
        // TP270
        s_Type = "Ti";
    }
    else if ( Key == "1015" || Key == "3115" || Key == "3215" ||
              Key == "3315" || Key == "3415" || Key == "3515" ) {
        // TP270-Pd
        s_Type = "Ti";
    }
    else if ( Key == "1032" || Key == "3032" || Key == "3132" ||
              Key == "3232" || Key == "3332" || Key == "3432" ) {
        // TP340
        s_Type = "Ti";
    }
    else if ( Key == "1033" || Key == "3133" || Key == "3233" ||
              Key == "3333" ) {
        // TP340-Pd
        s_Type = "Ti";
    }
    else if ( Key == "1057" || Key == "3057" || Key == "3157" ) {
        // TP480
        s_Type = "Ti";
    }
    else if ( Key == "1058" || Key == "3158" ) {
        // TP480-Pd
        s_Type = "Ti";
    }
    else if ( Key == "1085" ) {
        // SB-265-Gr.1(旧)
        s_Type = "Ti";
    }
    else if ( Key == "1089" ) {
        // SB-265-Gr.2(旧)
        s_Type = "Ti";
    }
    else if ( Key == "1075" ) {
        // SB-265-Gr.7(旧)
        s_Type = "Ti";
    }
    else if ( Key == "1076" ) {
        // SB-265-Gr.11(旧)
        s_Type = "Ti";
    }
    else if ( Key == "1095" ) {
        // SB-265-Gr.1(旧)
        s_Type = "Ti";
    }
    else if ( Key == "1090" ) {
        // B-265-Gr.2(旧)
        s_Type = "Ti";
    }
    //2016.11.08 材質ｺｰﾄﾞ変更
    //else if ( Key == "1051" || Key == "3151" ) {
    //    // STPG
    //    s_Type = "STPG";
    //}
    //else if ( Key == "1060" || Key == "3160" ) {
    //    // SGP
    //    s_Type = "SGP";
    //}
    else if ( Key == "1051" ) {
        // STPG
        s_Type = "STPG";
    }
    else if ( Key == "1060" ) {
        // SGP
        s_Type = "SGP";
    }
     // **********************
    else if ( Key == "1043" || Key == "1187" ) {
        // CAC703(AL-BRONZE)
        s_Type = "ALBC";
    }
    else if ( Key == "1151" ) {
        // STKM13A(CS)
        s_Type = "CS";
    }
    else if ( Key == "" || Key == "0000"|| Key == "1098" ) {
        // 材質未指定
        s_Type = "-";
    }
    //2016.11.08 材質ｺｰﾄﾞ変更
    else if ( Key == "0" ) {
        // 材質未指定
        s_Type = "-";
    }
    else if ( Key == "1001" || Key == "3001" || Key == "3101" ) {
        // SUS304
        s_Type = "SUS304";
    }
    else if ( Key == "1003" || Key == "3003" || Key == "3103" ) {
        // SUS304
        s_Type = "SUS316";
    }
    // 2018.03.20 温度計座材質変更_S
    else if ( Key == "1004" || Key == "3004" || Key == "3104" ) {
        // SUS316L
        s_Type = "SUS316L";
    }
    // 2018.03.20 温度計座材質変更_E
    else if ( Key == "1040" || Key == "1062" ) {
        // SS400
        s_Type = "SS400";
    }
    // **********************
    // 2022.04.28 UX-30L追加_S
    else if ( Key == "1168" ) {
        // S275JR
        s_Type = "S275JR";
    }
    // 2022.04.28 UX-30L追加_E
    //2018.02.09 硫酸用途注記追加_S
    else if ( Key == "1010" || Key == "3010" || Key == "3110" ||
              Key == "3210" || Key == "3310" || Key == "3410" ||
              Key == "3510" || Key == "3810" ) {
        // HASTELLOY B
        s_Type = "HAS";
    }
    else if ( Key == "1009" || Key == "3009" || Key == "3109" ||
              Key == "3209" || Key == "3309" || Key == "3409" ||
              Key == "3809" ) {
        // HASTELLOY B2
        s_Type = "HAS";
    }
    else if ( Key == "1067" || Key == "1073" || Key == "3073" ||
              Key == "3273" || Key == "3373" || Key == "3473" ||
              Key == "3873" ) {
        // HASTELLOY C-22
        s_Type = "HAS";
    }
    else if ( Key == "1011" || Key == "1072" || Key == "3072" ||
              Key == "3172" || Key == "3272" || Key == "3372" ||
              Key == "3472" || Key == "3872" ) {
        // HASTELLOY C-276
        s_Type = "HAS";
    }
    else if ( Key == "1077" || Key == "3077" || Key == "3277" ||
              Key == "3377" || Key == "3477" || Key == "3577" ||
              Key == "3877" ) {
        // HASTELLOY C-2000
        s_Type = "HAS";
    }
    else if ( Key == "1020" || Key == "3020" || Key == "3220" ||
              Key == "3320" || Key == "3420" || Key == "3820" ) {
        // HASTELLOY G
        s_Type = "HAS";
    }
    else if ( Key == "2009" ) {
        // L-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2013" ) {
        // LR-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2027" ) {
        // ﾊﾞｲﾄﾝ
        s_Type = "FPM";
    }
    else if ( Key == "2072" ) {
        // R-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2073" ) {
        // G-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2092" ) {
        // AG-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2093" ) {
        // A-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2096" ) {
        // AR-FPM
        s_Type = "FPM";
    }
    //2018.02.09 硫酸用途注記追加_E
    //2021.03.10 ﾆｯｹﾙ追加_S
    else if ( Key == "3053" || Key == "3054" || Key == "3153" ||
              Key == "3154" || Key == "3253" || Key == "3254" ||
              Key == "3353" || Key == "3354" || Key == "3453" ||
              Key == "3454" ) {
        // ﾆｯｹﾙ
        s_Type = "NI";
    }
    //2021.03.10 ﾆｯｹﾙ追加_E
    //2021.05.17 TCG追加_S
    else if ( Key == "2058" || Key == "2060" || Key == "2061" ||
              Key == "2062" || Key == "2063" || Key == "2064" ||
              Key == "2065" || Key == "2069" ) {
        // TCG
        s_Type = "TCG";
    }
    //2021.05.17 TCG追加_E
    else {
        // その他
        s_Type = "ELSE";
    }

    return s_Type;
}


// ---------------------------------------------------------------------------
// 日本語関数名： ALLOY材からJIS材への変換
// 概  要      ：
// 引  数      ： AnsiString Key   ： 材質ｺｰﾄﾞ
// 戻り値      ： 材質の種類
// 備  考      ： 2024.02.05追加
// ---------------------------------------------------------------------------
AnsiString __fastcall Alloy_MAT(AnsiString Key) {


    AnsiString s_Mat;

    //ALLOY材の場合JIS材に変換
    if ( Key == "3001" ) {
        // ALLOY 304 → SUS304
        s_Mat = "3101";
    }
    else if ( Key == "3002" ) {
        // ALLOY 304L → SUS304L
        s_Mat = "3102";
    }
    else if ( Key == "3003" ) {
        // ALLOY 316 → SUS316
        s_Mat = "3103";
    }
    else if ( Key == "3004" ) {
        // ALLOY 316L → SUS316L
        s_Mat = "3104";
    }
    else if ( Key == "3006" ) {
        // ALLOY 317 → SUS317
        s_Mat = "3106";
    }
    else if ( Key == "3007" ) {
        // ALLOY 310S → SUS310S
        s_Mat = "3107";
    }
    else if ( Key == "3008" ) {
        // ALLOY 317L → SUS317L
        s_Mat = "3108";
    }
    else {
        // その他
        s_Mat = Key;
    }

     return s_Mat;
}


//2022/12/15 桁数追加対応 Add_S
// ---------------------------------------------------------------------------
// 日本語関数名： 変換後の製造番号を取得
// 概  要      ：
// 引  数      ： AnsiString str   製造番号
// 戻り値      ： 変換後の結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall GetSeizouBangou(AnsiString str) {
	AnsiString strw;
	AnsiString strTmp;

	if (str.Length() > 7)
	{
	  strTmp = str.SubString(1, 2);
	  if (strTmp == "10")
	  {
		 strw = str.SubString(3,2) + "-" + str.SubString(5,4);
	  }
	  else
	  {
		 strw = str.SubString(1,4) + "-" + str.SubString(5,4);
	  }
	}
	else
	{
	  strw = str.SubString(1,2) + "-" + str.SubString(3,4);
	}

	return strw;
}
//2022/12/15 桁数追加対応 Add_E

//2023/4/21 桁数追加対応 Add_S
// ---------------------------------------------------------------------------
// 日本語関数名： 変換後の製造番号を取得
// 概  要      ：
// 引  数      ： AnsiString str   製造番号
// 戻り値      ： 変換後の結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall GetFileSeizouBangou(AnsiString str) {
	AnsiString strw;
	AnsiString strTmp;

	if (str.Length() > 7)
	{
	  strTmp = str.SubString(1, 2);
	  if (strTmp == "10")
	  {
		 strw = str.SubString(3,6);
	  }
	  else
	  {
		 strw = str;
	  }
	}
	else
	{
	  strw = str;
	}

	return strw;
}
//2023/4/21 桁数追加対応 Add_E

// ---------- <<< End of File >>> ----------
