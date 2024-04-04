// ***************************************************************
// モジュール名  ：HD_Data.h
// 作成者        ：西濱
// 作成日        ：2007-10-02
// 最終更新日    ：2007-10-02
// 概要          ：HeadsData クラス定義
// ***************************************************************

#define DEF_HEADSCONVDATA_ALL 1500      // HEADSテキストファイル行総数
#define MAX_PATH               260      // 1行あたりの最大バイト数
#define DEF_HEADSCONVDATA_REV  320      // HEADS変換データのREVNOデータ位置
#define DEF_HEADSCONVDATA_MRR  314      // HEADS変換データのミラー仕様データ位置

#define DEF_SPEC_TYPE_NONE  0           // HEADSテキストデータ 元仕様
#define DEF_SPEC_TYPE_MRR_A 1           // HEADSテキストデータ ミラーA仕様
#define DEF_SPEC_TYPE_MRR_B 2           // HEADSテキストデータ ミラーB仕様

class HeadsData {
private:

public:
    HeadsData(); // コンストラクタ
    ~HeadsData(); // デストラクタ
    bool ReadAllData(char* pchFilename); // HEADSテキストデータ読込
    bool GetMirrorFlag(); // ミラー仕様かどうか判定
    char* GetSpecCode(); // 仕様タイプコードを取得する
    void SetSpecCode(); // 仕様タイプコードを設定する
    int GetSpecType(); // 仕様タイプを取得する
    void SetSpecType(int nSpecType); // 仕様タイプを設定する
    bool SetAnsiStringData(AnsiString asHeadsData[]);
    // HEADSﾃｷｽﾄﾃﾞｰﾀをAnsiString配列に格納
    void GetOneLineData(int nIndex, char* pchData); // 指定要素番号のHEADSﾃｷｽﾄﾃﾞｰﾀを取得

private:
    char Data[DEF_HEADSCONVDATA_ALL][MAX_PATH]; // HEADSテキストデータ
    int nSpecType; // 仕様タイプ　　　(0:元仕様/1:ミラーA仕様/2:ミラーB仕様)
    char chSpecCode[2]; // 仕様タイプコード(""/"A"/"B")
};
