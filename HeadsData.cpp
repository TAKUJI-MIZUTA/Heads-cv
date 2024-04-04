// #include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <system.hpp>
#include "HeadsData.h"

// -----------------------------------------------------------
// 関数名        HeadsData
//
// 処理概要    ：HeadsDataクラス コンストラクタ
//
// 引数        ：(なし)
//
// 戻り値      ：(なし)
//
// 備考          2007.10.01 新規作成
// -----------------------------------------------------------
HeadsData::HeadsData() {
}

// -----------------------------------------------------------
// 関数名        HeadsData
//
// 処理概要    ：HeadsDataクラス デストラクタ
//
// 引数        ：(なし)
//
// 戻り値      ：(なし)
//
// 備考          2007.10.01 新規作成
// -----------------------------------------------------------
HeadsData::~HeadsData() {
}

// -----------------------------------------------------------
// 関数名        ReadAllData
//
// 処理概要    ：HEADS変換データを読み込み、データを配列に格納する
//
// 引数        ：*pchFilename:HEADSテキストデータファイルパス
//
// 戻り値      ：TRUE（正常終了）
//               FALSE（続行不可）
//
// 備考          2007.10.01 ミラー仕様対応 (新規作成)
// -----------------------------------------------------------
bool HeadsData::ReadAllData(char *pchFilename) {
    FILE *fp;
    char wkChar[MAX_PATH];
    char *tmpP;
    int i;
    char tmp; // Data[34]の3文字目
    char tmpSub[2]; // Data[34]の1～2文字目

    // 配列初期化
    memset(Data, '\0', sizeof(Data));

    // HEADS変換データファイルを開く
    fp = fopen(pchFilename, "r");
    if (fp != NULL) {
        // HEADS変換データを読み込む
        for (i = 0; i < DEF_HEADSCONVDATA_ALL; i++) {
            if (!feof(fp)) {
                fgets(wkChar, MAX_PATH, fp);
                tmpP = strtok(wkChar, "\r\n"); // 改行を取り除く
                if (tmpP != NULL) {
                    tmpP = strtok(wkChar + 1, "\""); // 最初のカンマから次のカンマまでの間を抽出する
                    if (tmpP != NULL) {
                        strcpy(Data[i], tmpP);
                    }
                }
            }
        }
        fclose(fp);

        // =========================================
        // データ補正処理
        // HDPCom.cppのRead_HEADS_Data関数を移植
        // =========================================
        // -----------------------------------------
        // コンデンサ対応？
        // Data[34]の3文字目が"C"なら、1～2文字目のみ取り出す
        // -----------------------------------------
        strncpy(&tmp, Data[34] + 2, 1);
        if (tmp == 'C') {
            strncpy(tmpSub, Data[34], 2);
            memset(Data[34], 0x00, sizeof(Data[34]));
            strncpy(Data[34], tmpSub, 2);
        }

        // -----------------------------------------
        // No:0～315 のﾃﾞｰﾀを No:507～822にｺﾋﾟｰ
        // 実際のHEADSﾃﾞｰﾀには格納されていない為。
        // -----------------------------------------
        for (i = 507; i < 822; i++) {
            //2019.10.31 NK量産外注記_S
            //if (i != 539) { // 塗装色の和英対応のため539を使用。上書きするとマズいのでチェックで外す   2003/06/09
            if (i == 536) {
                // ＮＫ量産外型式注意喚起出力のため536を使用。上書きするとマズいのでチェックで外す
            } else if (i == 539) {
                // 塗装色の和英対応のため539を使用。上書きするとマズいのでチェックで外す
            } else {
            //2019.10.31 NK量産外注記_E
                strcpy(Data[i], Data[i - 507]);
            }
        }
    }
    else {
        // ファイル読込失敗
        return (false);
    }
    return (true);
}

// -----------------------------------------------------------
// 関数名        GetMirrorfromHeadsConvData
//
// 処理概要    ：HEADS変換データからミラー仕様を取得する
//
// 引数        ：order:オーダNo
//
// 戻り値      ：TRUE（ミラー仕様である）
//               FALSE（ミラー仕様でない）
//
// 備考          2007.10.01 ミラー仕様対応 (新規作成)
// -----------------------------------------------------------
bool HeadsData::GetMirrorFlag() {
    char chMirrorVal;
    strncpy(&chMirrorVal, Data[DEF_HEADSCONVDATA_MRR - 1], sizeof(chMirrorVal));

    if (chMirrorVal == '1') {
        return (true);
    }
    else {
        return (false);
    }
}

// -----------------------------------------------------------
// 関数名        GetSpecType
//
// 処理概要    ：HEADSテキスト　仕様タイプ取得
//
// 引数        ：(なし)
//
// 戻り値      ：仕様タイプ(0:元仕様/1:ミラーA仕様/2:ミラーB仕様)
//
// 備考          2007.10.02 新規作成
// -----------------------------------------------------------
int HeadsData::GetSpecType() {
    return (nSpecType);
}

// -----------------------------------------------------------
// 関数名        SetSpecType
//
// 処理概要    ：HEADSテキスト　仕様タイプ設定
//
// 引数        ：(なし)
//
// 戻り値      ：仕様タイプ("":元仕様/"A":ミラーA仕様/"B":ミラーB仕様)
//
// 備考          2007.10.02 新規作成
// -----------------------------------------------------------
void HeadsData::SetSpecType(int nSpec) {
    nSpecType = nSpec;
    return;
}

// -----------------------------------------------------------
// 関数名        GetSpecCode
//
// 処理概要    ：HEADSテキスト　仕様タイプコード取得
//
// 引数        ：(なし)
//
// 戻り値      ：仕様タイプ("":元仕様/"A":ミラーA仕様/"B":ミラーB仕様)
//
// 備考          2007.10.02 新規作成
// -----------------------------------------------------------
char* HeadsData::GetSpecCode() {
    return (chSpecCode);
}

// -----------------------------------------------------------
// 関数名        SetSpecCode
//
// 処理概要    ：HEADSテキスト　仕様タイプコード設定
//
// 引数        ：(なし)
//
// 戻り値      ：(なし)
//
// 備考          2007.10.02 新規作成
// -----------------------------------------------------------
void HeadsData::SetSpecCode() {
    switch (nSpecType) {
    case DEF_SPEC_TYPE_NONE:
        strcpy(chSpecCode, "");
        break;
    case DEF_SPEC_TYPE_MRR_A:
        strcpy(chSpecCode, "A");
        break;
    case DEF_SPEC_TYPE_MRR_B:
        strcpy(chSpecCode, "B");
        break;
    default:
        strcpy(chSpecCode, "");
    }

    return;
}

// -----------------------------------------------------------
// 関数名        SetAnsiStringData
//
// 処理概要    ：HEADS変換データを読み込み、データを配列に格納する
//
// 引数        ：*pchFilename:HEADSテキストデータファイルパス
//
// 戻り値      ：TRUE（正常終了）
//               FALSE（続行不可）
//
// 備考          2007.10.01 ミラー仕様対応 (新規作成)
// -----------------------------------------------------------
bool HeadsData::SetAnsiStringData(AnsiString asHeadsData[]) {
    int i;
    char *pchData;

    // 配列初期化
    for (i = 0; i < DEF_HEADSCONVDATA_ALL; i++) {
        asHeadsData[i] = "";

    }

    // HEADS変換データをchar配列→AnsiString配列にｾｯﾄ
    for (i = 0; i < DEF_HEADSCONVDATA_ALL; i++) {
        pchData = Data[i];
        asHeadsData[i] = pchData;
    }

    return (true);
}

// -----------------------------------------------------------
// 関数名        GetOneLineData
//
// 処理概要    ：HEADS変換データの指定要素番号のデータを返す
//
// 引数        ：nIndex:HEADSテキストデータ行番号(0～)
//
// 戻り値      ：指定要素番号のHEADSテキストデータ
//
// 備考          2007.10.04 ミラー仕様対応 (新規作成)
// -----------------------------------------------------------
void HeadsData::GetOneLineData(int nIndex, char* pchData) {
    strcpy(pchData, Data[nIndex]);
    return;
}
