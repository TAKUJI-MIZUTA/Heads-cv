// ----------------------------------------------------------------------------------------
// “úã»ìŠ—lŒü‚¯@o}—v‹óMƒT[ƒrƒXƒVƒXƒeƒ€
// Ì§²ÙŠT—vFƒtƒŒ[ƒ€•”•i“WŠJˆ—  ƒ‚ƒWƒ…[ƒ‹
// ì ¬ ÒFN.Uchida
// ì ¬ “úF2002.06.21
// X V “úF2002.10.08
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>

#pragma hdrstop

#include <io.h>
#include <Filectrl.hpp>

#include "HDPConst.h"   // ’è”’è‹`—p
#include "HDPValue.h"   // •Ï”’è‹`—p
#include "HDPCom.h"     // ŠÖ”’è‹`—p

#include "DataModule.h"

#include "Bas_FrameDetails.h"

#include "HeadsData.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// ƒtƒŒ[ƒ€•”•i“WŠJˆ— Class
// ---------------------------------------------------------------------------
class TFrameDetails {
private:
    FILE *fp;

    // ”Ä—p
    AnsiString P_SQL; // SQL•¶—p
    AnsiString P_Key; // ŒŸõ—p·°

    // CSVÌ§²Ùì¬—p
    AnsiString P_BBunrui; // •”•i•ª—Ş
    AnsiString P_Sikibetu; // ¯•Êq
    AnsiString P_BKatasiki; // •”•iŒ^®
    AnsiString P_BMeisyou; // •”•i–¼Ì
    AnsiString P_ZaisituNM; // Ş¿–¼Ì
    AnsiString P_EZaisituNM; // Ş¿–¼Ì
    AnsiString P_ZaisituCD; // Ş¿ƒR[ƒh
    AnsiString P_Zuban; // }”Ô
    AnsiString P_SZuban; // ì}—p}”Ô
    int P_1DaiSuryo; // ‚P‘ä•ª”—Ê
    int P_Yobisu; // —\”õ”
    int P_Soukei; // ‘Œv
    int P_RecNo; // Úº°ÄŞ¶³İÀ

    AnsiString P_Siyou1; // ”õl‚P
    AnsiString P_Siyou2; // ”õl‚Q
    AnsiString P_Siyou3; // ”õl‚R
    AnsiString P_Siyou4; // ”õl‚S
    AnsiString P_Siyou5; // ”õl‚T

    // ÌÚ°Ñ\¬•”•iº°ÄŞÃ°ÌŞÙ(FRAMECD.CSV)—p
    AnsiString P_FRAMECD[5];

    // ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)—p
    AnsiString P_DNO[6]; // }”Ô
    AnsiString P_WKDNO[6]; // ì}—p}”Ô
    AnsiString P_MatNM[6]; // Ş¿–¼Ì
    AnsiString P_MatCD[6]; // Ş¿ƒR[ƒh
    int P_PLATE_NUM[6]; // –‡”
    int P_PLATE_NUM_SUS[6]; // –‡”(SUS—p)
    int P_SUS; // SUSg—pƒtƒ‰ƒO

    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];

    bool __fastcall S_Frame_Hontai_Sakusei(void); // SÌÚ°ÑÃŞ°À@   ì¬
    bool __fastcall E_Frame_Hontai_Sakusei(void); // EÌÚ°ÑÃŞ°À@   ì¬
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
    bool __fastcall S_FrameG_Hontai_Sakusei(void); // SÌÚ°Ñ¶Ş½·ØÃŞ°À@   ì¬
    bool __fastcall E_FrameG_Hontai_Sakusei(void); // EÌÚ°Ñ¶Ş½·ØÃŞ°À@   ì¬
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E
    bool __fastcall Frame_Buhin_Sakusei(void); // ÌÚ°Ñ•”•iÃŞ°À@ì¬

    bool __fastcall Search_HD_FRM_NZ(AnsiString Key, AnsiString Kbn);
    // ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À@ŒŸõ
    bool __fastcall Search_HD_FRM_NZ_Buhin(AnsiString Key);
    // ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À@ŒŸõ

    bool __fastcall Write_SE_Frame_Data(AnsiString Kbn, bool Seek_Result);
    // S,EÌÚ°ÑÃŞ°À@‘‚«‚İ
    bool __fastcall Write_FrameBuhin_Data(bool Seek_Result); // ÌÚ°Ñ•”•iÃŞ°À ‘‚«‚İ
    bool __fastcall Syukei_Frame_Yobisu(AnsiString a_Sekbn); // ÌÚ°Ñ—\”õ”WŒv

    AnsiString __fastcall Get_AnaakeSiyou(AnsiString Kbn); // E–¾d—l æ“¾
    AnsiString __fastcall Get_BuhinKatasiki(AnsiString Code); // •”•iŒ^®@æ“¾
    void __fastcall BLT_Cover_Out(void);
    // 2014/08/22 ‰—p‹Zp•ÒW
    void __fastcall BLT_Cover_Out_Small(void);
    // 2022.01.14 ƒXƒy[ƒT’Ç‰Á_S
    AnsiString __fastcall Get_AnaakeSiyou_S(AnsiString Kbn); // E–¾d—l æ“¾
    // 2022.01.14 ƒXƒy[ƒT’Ç‰Á_E

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // ÌÚ°Ñ•”•i“WŠJˆ— Às
    int __fastcall Get_Blt_YobiSu(double d_BltSz, double d_BltLen,
        AnsiString MatCD, HeadsData* pcHeadsData);

};

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ƒtƒŒ[ƒ€•”•i “WŠJˆ—
// ŠT  —v      F
// ˆø  ”      F ‚È‚µ
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:³í false:ˆÙí]
// ”õ  l      F
// ---------------------------------------------------------------------------
bool __fastcall FrameDetails(HeadsData* pcHeadsData) {
    bool ret;

    TFrameDetails flameDetails; // ÌßÚ°Ä–{‘ÌÃŞ°ÀCE–¾ÃŞ°À ì¬Class©“®¶¬,ŠÖ”I—¹Œã”jŠü

    ret = flameDetails.Excute(pcHeadsData); // ÌßÚ°Ä–{‘ÌÃŞ°ÀCE–¾ÃŞ°À ì¬Às

    return ret;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ÌÚ°Ñ•”•i“WŠJˆ— ì¬Às
// ŠT  —v      F
// ˆø  ”      F ‚È‚µ
// –ß‚è’l      F “ÇŒ‹‰Ê [true:³í false:ˆÙí]
// ”õ  l      F ‚È‚µ
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    m_pcHeadsData = pcHeadsData;
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // Û¸Şì¬
    Write_Log("********************************************************");
    Write_Log("**********      wÌÚ°Ñ•”•i“WŠJˆ—xŠJn      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********      wÌÚ°Ñ•”•i“WŠJˆ—xŠJn      *********");
    Write_Error_Log("*******************************************************");

    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
    // ***********************************
    // ***  SÌÚ°Ñ¶Ş½·ØÃŞ°À@ì¬
    // ***********************************
    G_Log_Text = "----------   wSÌÚ°Ñ¶Ş½·ØÃŞ°Àxì¬ŠJn   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    S_FrameG_Hontai_Sakusei();

    G_Log_Text = "----------   wSÌÚ°Ñ¶Ş½·ØÃŞ°Àxì¬I—¹   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E

    // ***********************************
    // ***  SÌÚ°ÑÃŞ°À@ì¬
    // ***********************************
    G_Log_Text = "----------     wSÌÚ°ÑÃŞ°Àxì¬ŠJn      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    S_Frame_Hontai_Sakusei();

    G_Log_Text = "----------     wSÌÚ°ÑÃŞ°Àxì¬I—¹      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
    // ***********************************
    // ***  EÌÚ°Ñ¶Ş½·ØÃŞ°À@ì¬
    // ***********************************
    G_Log_Text = "----------   wEÌÚ°Ñ¶Ş½·ØÃŞ°Àxì¬ŠJn   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    E_FrameG_Hontai_Sakusei();

    G_Log_Text = "----------   wEÌÚ°Ñ¶Ş½·ØÃŞ°Àxì¬I—¹   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E

    // ***********************************
    // ***  EÌÚ°ÑÃŞ°À@ì¬
    // ***********************************
    G_Log_Text = "----------     wEÌÚ°ÑÃŞ°Àxì¬ŠJn      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    E_Frame_Hontai_Sakusei();

    G_Log_Text = "----------     wEÌÚ°ÑÃŞ°Àxì¬I—¹      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);


    // ***********************************
    // ***  ÌÚ°Ñ•”•iÃŞ°À@ì¬
    // ***********************************
    G_Log_Text = "----------     wÌÚ°Ñ•”•iÃŞ°Àxì¬ŠJn      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    Frame_Buhin_Sakusei();

    G_Log_Text = "----------     wÌÚ°Ñ•”•iÃŞ°Àxì¬I—¹      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // Û¸Şì¬
    Write_Log("********************************************************");
    Write_Log("**********      wÌÚ°Ñ•”•i“WŠJˆ—xI—¹      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********      wÌÚ°Ñ•”•i“WŠJˆ—xI—¹      *********");
    Write_Error_Log("*******************************************************");
    return true;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ÌÚ°Ñ•”•iÃŞ°Àì¬ˆ— Às
// ŠT  —v      F
// ˆø  ”      F ‚È‚µ
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:³í false:ˆÙí]
// ”õ  l      F ‚È‚µ
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Frame_Buhin_Sakusei(void) {
    FILE *wkfp;

    bool seek_ret;
    AnsiString s_FilePass;
    AnsiString s_Katasiki;
    int i_FrameCode;

    char buf[1024]; /* •¶š—ñƒoƒbƒtƒ@ */
    char *tok; /* ƒg[ƒNƒ“ƒ|ƒCƒ“ƒ^ */
    int i; /* ƒ‹[ƒvƒJƒEƒ“ƒ^ */
    char token[100][100]; /* ƒg[ƒNƒ“ƒoƒbƒtƒ@ */

    // 2017.06.23 LX-90D-NUP’Ç‰Á
    AnsiString s_Text;
    // *************************

    // ***************************
    // **  ÌÚ°ÑŒ^®•ÏŠ·Ã°ÌŞÙ
    // ***************************
    s_FilePass = G_Input_Csv_Dir + "FRAMECD.csv";

    if ((wkfp = fopen(s_FilePass.c_str(), "r")) == NULL) {
        // ´×°Û¸Şì¬
        G_ErrLog_Text = "w" + s_FilePass + "x‚Ì‚n‚o‚d‚m‚É¸”s‚µ‚Ü‚µ‚½II";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // •Â‚¶‚é
        return false;
    }

    // “Ç‚İ‰Â”\‚ÌŠÔ
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), wkfp) != NULL) {
        i = 0; // ƒg[ƒNƒ“ƒJƒEƒ“ƒ^‰Šú‰»
        memset(token, 0x00, sizeof(token)); // ƒg[ƒNƒ“ƒoƒbƒtƒ@‰Šú‰»
        tok = GetToken(buf, ","); // ","‚Å€–Ú‚ğ‹æØ‚é
        while (tok) { // ƒg[ƒNƒ“‚ª‘¶İ‚·‚éŠÔ
            sprintf(token[i++], "%-.99s", tok); // ƒg[ƒNƒ“‚ğƒoƒbƒtƒ@‚É“]‘—
            tok = GetToken(NULL, ","); // Ÿ‚Ìƒg[ƒNƒ“‚ğŒŸõ

            if (i > 100) {
                break;
            }
        }

        // CSV“Ç‚İŒ‹‰Ê@ƒZƒbƒg
        P_FRAMECD[1] = token[0]; // FRAME.CSVFº°ÄŞ
        P_FRAMECD[2] = token[1]; // FRAME.CSVF•”•i•ª—Şº°ÄŞ
        P_FRAMECD[3] = token[2]; // FRAME.CSVF•”•i‹æ•ª
        P_FRAMECD[4] = token[3]; // FRAME.CSVF•”•i–¼Ì

        s_Katasiki = token[4]; // —\”õ•i”æ“¾—p

        // 2017.06.23 LX-90D-NUP’Ç‰Á
        // 2014/08/22 ‰—p‹Zp•ÒW
        // ƒR[ƒh‚ª"75"(BNWRN-,’÷•tBNW) ‚©‚Â ZƒtƒŒ[ƒ€‚Å‚È‚¢ê‡Ao—Í‘ÎÛŠO
        //if(P_FRAMECD[1] == "75" &&
        //   P_HEADS_DATA[36].Pos("Z") == 0) {
        //
        //   continue;
        //
        //}
        s_Text = G_KS_Syurui + "|" + G_KS_Model;
        if( P_FRAMECD[1] == "75" ) {
            // ’÷•tÎŞÙÄ(×)
            if (( s_Text == "‚k‚w|‚X‚O‚c") && ( P_HEADS_DATA[37].ToIntDef(0)) == 94 ){
                // LX-90D-KNUP  o—Í‘ÎÛ
            } else if (( s_Text == "‚k‚w|‚X‚O‚c") && ( P_HEADS_DATA[37].ToIntDef(0)) == 394 ){
                // LX-90D-TNUP  o—Í‘ÎÛ
            } else if( P_HEADS_DATA[36].Pos("Z") == 0) {
                // ZÌÚ°Ñ  o—Í‘ÎÛŠO
                continue;
            }
        }
        // *************************

        i_FrameCode = P_FRAMECD[1].ToIntDef(1);

        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
        //if (i_FrameCode != 1 && i_FrameCode != 8) {
        if (i_FrameCode != 1 && i_FrameCode != 8 && i_FrameCode != 81 && i_FrameCode != 88) {
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E

            // ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)ŒŸõ·°
            P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
                P_HEADS_DATA[1267].TrimRight();

            // *********************************************
            // ***  ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)ŒŸõ
            // *********************************************
            seek_ret = Search_HD_FRM_NZ_Buhin(P_Key);

            // **********************
            // ***  —\”õ•iÃŞ°À@æ“¾
            // **********************
            P_Yobisu = 0;

            // —\”õ”‚Ìæ“¾
            P_Yobisu = Syukei_Buhin_Yobisu(s_Katasiki, P_ZaisituCD,
                m_pcHeadsData);

            // ***************************
            // ***  ÌÚ°Ñ•”•iÃŞ°À@‘
            // ***************************
            Write_FrameBuhin_Data(seek_ret);

        }

    }

    fclose(wkfp); // •Â‚¶‚é
    // delete wkfp;

    return true;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F —\”õ•iÃŞ°À@WŒvˆ—
// ŠT  —v      F —\”õ•iÃŞ°À‚©‚ç—\”õ•i”‚ğ‹‚ß‚é
// ˆø  ”      F ‚È‚µ
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:³í false:ˆÙí]
// ”õ  l      F 02.08.03’Ç‰Á
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Syukei_Frame_Yobisu(AnsiString a_Sekbn) {
    // CSV“Ç—p
    FILE *wkfp;

    char buf[1024]; /* •¶š—ñƒoƒbƒtƒ@ */
    char *tok; /* ƒg[ƒNƒ“ƒ|ƒCƒ“ƒ^ */
    int i; /* ƒ‹[ƒvƒJƒEƒ“ƒ^ */
    char token[100][100]; /* ƒg[ƒNƒ“ƒoƒbƒtƒ@ */

    AnsiString s_Yobi_Text[5];
    AnsiString a_Kbn;
    AnsiString wk_FilePass; // "D00001.csv"

    P_RecNo = 0; // Úº°ÄŞ¶³İÀ(’ŠoğŒˆê’vŒ”)

    // -----------------------------------------------
    // 2007.10.03 ’Ç‰Á
    // Ğ×°Bd—l‚Ìê‡‚ÍA—\”õ•iÃŞ°À‚È‚µ‚Æ‚µ‚Äˆ—
    if (m_pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B) {
        P_Yobisu = 0;
        return true;
    }
    // 2007.10.03
    // -----------------------------------------------

    // ÌßÚ°Ä—\”õ•iÃŞ°ÀÊß½
    wk_FilePass = G_AnaFilePass + G_SAKUBAN + ".csv";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ´×°Û¸Şì¬
        G_ErrLog_Text = "—\”õ•iÃŞ°À w" + wk_FilePass + "x‚Ì‚n‚o‚d‚m‚É¸”s‚µ‚Ü‚µ‚½II";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // “Ç‚İ‰Â”\‚ÌŠÔ
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), wkfp) != NULL) {
            i = 0; // ƒg[ƒNƒ“ƒJƒEƒ“ƒ^‰Šú‰»
            memset(token, 0x00, sizeof(token)); // ƒg[ƒNƒ“ƒoƒbƒtƒ@‰Šú‰»
            tok = GetToken(buf, ","); // ","‚Å€–Ú‚ğ‹æØ‚é
            while (tok) { // ƒg[ƒNƒ“‚ª‘¶İ‚·‚éŠÔ
                sprintf(token[i++], "%-.99s", tok); // ƒg[ƒNƒ“‚ğƒoƒbƒtƒ@‚É“]‘—
                tok = GetToken(NULL, ","); // Ÿ‚Ìƒg[ƒNƒ“‚ğŒŸõ

                if (i > 100) {
                    break;
                }
            }
            // ÃŞ°À—á
            s_Yobi_Text[0] = token[0];
            // —\”õ•iÃŞ°À 1—ñ–Ú ( 'PLTE','GSKT','ADHS','THRM'... )
            s_Yobi_Text[1] = token[1];
            // —\”õ•iÃŞ°À 2—ñ–Ú ( ÌŞ×İ¸,'D','E','1','2'... )
            s_Yobi_Text[2] = token[8]; // —\”õ•iÃŞ°À —\”õ”( 10, 1, 5, 2, 8, ... )

            // 1‘ä•ª”—Ê‚ÌWŒv

            if (a_Sekbn == "S")
                a_Kbn = "2";
            else if (a_Sekbn == "E")
                a_Kbn = "3";

            if (s_Yobi_Text[0].Trim() == "FRME" && s_Yobi_Text[1].Trim()
                == a_Kbn) {
                P_Yobisu = s_Yobi_Text[2].ToIntDef(0);
                P_RecNo++;
            }
        }
    }

    // ŠY“–—\”õ•iÃŞ°À‚ª‘¶İ‚µ‚È‚©‚Á‚½ê‡
    if (P_RecNo == 0)
        P_Yobisu = 0;

    // —\”õ•iÃŞ°À Ì§²Ù¸Û°½Ş
    fclose(wkfp);
    return true;
}

// ---------------------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ÌÚ°Ñ•”•iÃŞ°À@‘‚«‚İ
// ŠT  —v      F
// ˆø  ”      F Seek_Result:ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½ÀŒŸõŒ‹‰Ê(true:‘¶İ,false:‘¶İ‚µ‚È‚¢)
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:ÃŞ°À—L‚è false:ÃŞ°À–³‚µ]
// ”õ  l      F
// ---------------------------------------------------------------------------------------
bool __fastcall TFrameDetails::Write_FrameBuhin_Data(bool Seek_Result) {
    AnsiString s_Text;
    // 2009.02.25 WX-50,CX-10Dƒx[ƒXƒvƒŒ[ƒg’Ç‰Á
    AnsiString s_Text2;
    // *****************************************
    // 2012.01.20 ’†‘Ş¿
    AnsiString s_ChinaM;
    int i_RecNo;
    // *******************
    AnsiString wk_FilePass;
    AnsiString sPltMat;
    AnsiString sS1NoMat;
    AnsiString sS2NoMat;
    AnsiString sE1NoMat;
    AnsiString sBltMat;
    // 2022.01.14 ƒXƒy[ƒT’Ç‰Á_S
    AnsiString sAGasMat;
    AnsiString sBGasMat;
    // 2022.01.14 ƒXƒy[ƒT’Ç‰Á_E
    bool bSUS;
    bool bS1SUS;
    bool bS2SUS;

    long l_Daisu;

    // ------------------------------------------
    // 2007.10.03 Ğ×°d—l‘Î‰ d—lÀ²Ìßº°ÄŞ
    AnsiString sSpecCode;
    // ****************************
    // d—lÀ²Ìßº°ÄŞæ“¾(""/"A"/"B")
    // ****************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03 Ğ×°d—l‘Î‰ d—lÀ²Ìßº°ÄŞ
    // ------------------------------------------

    // ****************************
    // ***  •Ï”ƒZƒbƒg
    // ****************************
    // •”•i•ª—Ş
    P_BBunrui = "FRAME";
    // ¯•Êq
    P_Sikibetu = P_FRAMECD[2];

    // Ş¿–¼Ì‰Šú‰»
    P_ZaisituNM = "";
    P_EZaisituNM = "";

    // 2014/08/22 ‰—p‹Zp•ÒW
    // •”•iŒ^®
    if (Seek_Result == true && P_FRAMECD[1] != "75") {
        P_BKatasiki = Get_BuhinKatasiki(P_FRAMECD[1]);
    }
    else {
        P_BKatasiki = "";
    }

    // •”•i–¼Ì
    s_Text = P_FRAMECD[1];
    // 2014/08/22 ‰—p‹Zp•ÒW
    if (s_Text == "18" || s_Text == "23" || s_Text == "24" || s_Text == "25" ||
        s_Text == "26" || s_Text == "27" || s_Text == "28" || s_Text == "75") {
        P_BMeisyou = "";
    }
    else {
        P_BMeisyou = P_FRAMECD[4];
    }

    // }”ÔCì}—p}”Ô
    // 2003.11.05 –‡”‹æ•ªŠÖŒW®ƒ‚ğƒ‚É•ÏX
    if (Seek_Result == true) {
        // 2006.01.10 –‡”‹æ•ª‚É‚æ‚é}”Ô‚Ì•ÏX‚É‰º•”¶Ş²ÄŞÊŞ°(15)‚ğ’Ç‰Á
        // if (P_FRAMECD[1] == "09" || P_FRAMECD[1] == "14" || P_FRAMECD[1] == "16"){
        if (P_FRAMECD[1] == "09" || P_FRAMECD[1] == "14" ||
            P_FRAMECD[1] == "15" || P_FRAMECD[1] == "16") {
            // ***********************************************************
            bSUS = false;
            // 2003.11.06@ƒvƒŒ[ƒgŞ¿ƒf[ƒ^ˆÊ’u•ÏX
            // sPltMat = P_HEADS_DATA[160].TrimRight();
            sPltMat = P_HEADS_DATA[43].TrimRight();
            // **************************************
            // 2003.12.05 ASME PLATE
            // if ((P_SUS != 0 ) && (sPltMat != "1014" ) && ( sPltMat != "1015" ) &&
            // ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠOiTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]j
            // 2016.11.07 Ş¿º°ÄŞ•ÏX
            //if ((P_SUS != 0) && (sPltMat != "1014") && (sPltMat != "1015") &&
            //    (sPltMat != "1032") && (sPltMat != "1033") &&
            //    (sPltMat != "1057") && (sPltMat != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2’Ç‰Á
            //    (sPltMat != "1075") && (sPltMat != "1089") &&
            //    (sPltMat != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11’Ç‰Á
            //    (sPltMat != "1076") &&
            //    // ***************************
            //    (sPltMat != "1085") && (sPltMat != "1095")) {
            //    // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠOiTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]j
            //    // *********************
            //    bSUS = true;
            //}
            // 2019.05.09 t0.8ÌßÚ°Ä”»’f’Ç‰Á_S
            //if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
            //    // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠO
            //    bSUS = true;
            //}
            if ((P_SUS == 1) && (Type_MAT(sPltMat) != "Ti")) {
                // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠO
                bSUS = true;
            }
            if ((P_SUS == 2) && (P_HEADS_DATA[1013].TrimRight() == "08")) {
                // ƒvƒŒ[ƒg”ÂŒú‚ª‚”‚OD‚W
                bSUS = true;
            }
            // 2019.05.09 t0.8ÌßÚ°Ä”»’f’Ç‰Á_E
            // ***********************
            // 2022.04.08 UX-30L’Ç‰Á_S
            if ((P_SUS == 3) && (P_HEADS_DATA[1381].ToIntDef(0) == 1 )) {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
            else if ((P_SUS == 3) && (P_HEADS_DATA[1381].ToIntDef(0) == 2 )) {
                P_Zuban = P_DNO[2];
                P_SZuban = P_WKDNO[2];
                if (P_MatNM[2] != "") {
                    P_ZaisituNM = P_MatNM[2];
                    P_EZaisituNM = P_MatNM[2];
                }
                else if (P_MatCD[2] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                   P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                   P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                    P_Siyou3 = "2";
            }
            else if ((P_SUS == 3) && (P_HEADS_DATA[1381].ToIntDef(0) == 3)) {
                P_Zuban = P_DNO[3];
                P_SZuban = P_WKDNO[3];
                if (P_MatNM[3] != "") {
                    P_ZaisituNM = P_MatNM[3];
                    P_EZaisituNM = P_MatNM[3];
                }
                else if (P_MatCD[3] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "3";
            }
            else if ((P_SUS == 3) &&  (P_HEADS_DATA[1381].ToIntDef(0) == 4)) {
                P_Zuban = P_DNO[4];
                P_SZuban = P_WKDNO[4];
                if (P_MatNM[4] != "") {
                    P_ZaisituNM = P_MatNM[4];
                    P_EZaisituNM = P_MatNM[4];
                }
                else if (P_MatCD[4] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[4]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[4]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "4";
            }
            else if ((P_SUS == 3) && (P_HEADS_DATA[1381].ToIntDef(0) == 5)) {
                P_Zuban = P_DNO[5];
                P_SZuban = P_WKDNO[5];
                if (P_MatNM[5] != "") {
                    P_ZaisituNM = P_MatNM[5];
                    P_EZaisituNM = P_MatNM[5];
                }
                else if (P_MatCD[5] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[5]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[5]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "5";
            }
            //if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[1])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[1]))) {
            // 2022.04.08 UX-30L’Ç‰Á_E
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[2])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[2]))) {
                P_Zuban = P_DNO[2];
                P_SZuban = P_WKDNO[2];
                if (P_MatNM[2] != "") {
                    P_ZaisituNM = P_MatNM[2];
                    P_EZaisituNM = P_MatNM[2];
                }
                else if (P_MatCD[2] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "2";
            }
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[3])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[3]))) {
                P_Zuban = P_DNO[3];
                P_SZuban = P_WKDNO[3];
                if (P_MatNM[3] != "") {
                    P_ZaisituNM = P_MatNM[3];
                    P_EZaisituNM = P_MatNM[3];
                }
                else if (P_MatCD[3] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "3";
            }
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[4])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[4]))) {
                P_Zuban = P_DNO[4];
                P_SZuban = P_WKDNO[4];
                if (P_MatNM[4] != "") {
                    P_ZaisituNM = P_MatNM[4];
                    P_EZaisituNM = P_MatNM[4];
                }
                else if (P_MatCD[4] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[4]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[4]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "4";
            }
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[5])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[5]))) {
                P_Zuban = P_DNO[5];
                P_SZuban = P_WKDNO[5];
                if (P_MatNM[5] != "") {
                    P_ZaisituNM = P_MatNM[5];
                    P_EZaisituNM = P_MatNM[5];
                }
                else if (P_MatCD[5] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[5]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[5]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "5";
            }
            else {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
            // 2003.12.12 ‚x‚w|‚W‚OƒtƒŒ[ƒ€“–”Â’Ç‰Á
        }
        else if (P_FRAMECD[1] == "06") { // ‚rƒtƒŒ[ƒ€“–”Â
            bS1SUS = false;
            bS2SUS = false;
            sS1NoMat = P_HEADS_DATA[894].TrimRight();
            sS2NoMat = P_HEADS_DATA[909].TrimRight();
            // 2016.11.07 Ş¿º°ÄŞ•ÏX
            //if ((sS1NoMat != "1014") && (sS1NoMat != "1015") &&
            //    (sS1NoMat != "1032") && (sS1NoMat != "1033") &&
            //    (sS1NoMat != "1057") && (sS1NoMat != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2’Ç‰Á
            //    (sS1NoMat != "1075") && (sS1NoMat != "1089") &&
            //    (sS1NoMat != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11’Ç‰Á
            //    (sS1NoMat != "1076") &&
            //    // ***************************
            //    (sS1NoMat != "1085") && (sS1NoMat != "1095")) {
            //    // ‚r‚PƒmƒYƒ‹Ş¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠOiTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]j
            //    bS1SUS = true;
            //}
            //if ((sS2NoMat != "1014") && (sS2NoMat != "1015") &&
            //    (sS2NoMat != "1032") && (sS2NoMat != "1033") &&
            //    (sS2NoMat != "1057") && (sS2NoMat != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2’Ç‰Á
            //    (sS2NoMat != "1075") && (sS2NoMat != "1089") &&
            //    (sS2NoMat != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11’Ç‰Á
            //    (sS2NoMat != "1076") &&
            //    // ***************************
            //    (sS2NoMat != "1085") && (sS2NoMat != "1095")) {
            //    // ‚r‚PƒmƒYƒ‹Ş¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠOiTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]j
            //    bS2SUS = true;
            //}
            if (Type_MAT(sS1NoMat) != "Ti") {
                // ‚r‚PƒmƒYƒ‹Ş¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠO
                bS1SUS = true;
            }
            if (Type_MAT(sS2NoMat) != "Ti") {
                // ‚r‚QƒmƒYƒ‹Ş¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠO
                bS2SUS = true;
            }
            // ***********************
            if ((bS1SUS == true) && (bS2SUS == true)) {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
            }
            else if ((bS1SUS == false) && (bS2SUS == true)) {
                P_Zuban = P_DNO[2];
                P_SZuban = P_WKDNO[2];
            }
            else if ((bS1SUS == true) && (bS2SUS == false)) {
                P_Zuban = P_DNO[3];
                P_SZuban = P_WKDNO[3];
            }
            else {
                P_Zuban = P_DNO[4];
                P_SZuban = P_WKDNO[4];
            }
            P_ZaisituNM = Search_HD_MATERIAL(sS1NoMat) + "^" +
                Search_HD_MATERIAL(sS2NoMat);
            P_EZaisituNM = Search_HD_E_MATERIAL(sS1NoMat) + "^" +
                Search_HD_E_MATERIAL(sS2NoMat);
        }
        else if (P_FRAMECD[1] == "13") { // ‚dƒtƒŒ[ƒ€“–”Â
            //2019.11.12 }”Ô’Ç‰Á_S
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            //2019.11.12 }”Ô’Ç‰Á_E
            sE1NoMat = P_HEADS_DATA[954].TrimRight();
            P_ZaisituNM = Search_HD_MATERIAL(sE1NoMat);
            P_EZaisituNM = Search_HD_E_MATERIAL(sE1NoMat);
        }
        //2015/03/19 ƒAƒLƒ…ƒ€ƒŒ[ƒ^‚Ìê‡ ADD_S
        else if (P_FRAMECD[1] == "76") {
             if (P_HEADS_DATA[1116].ToIntDef(0) <= P_PLATE_NUM[1]) {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
            else if (P_HEADS_DATA[1116].ToIntDef(0) <= P_PLATE_NUM[2]) {
                P_Zuban = P_DNO[2];
                P_SZuban = P_WKDNO[2];
                if (P_MatNM[2] != "") {
                    P_ZaisituNM = P_MatNM[2];
                    P_EZaisituNM = P_MatNM[2];
                }
                else if (P_MatCD[2] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "2";
            }
            else if (P_HEADS_DATA[1116].ToIntDef(0) <= P_PLATE_NUM[3]) {
                P_Zuban = P_DNO[3];
                P_SZuban = P_WKDNO[3];
                if (P_MatNM[3] != "") {
                    P_ZaisituNM = P_MatNM[3];
                    P_EZaisituNM = P_MatNM[3];
                }
                else if (P_MatCD[3] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "3";
            }
            else {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
        }
        //2015/03/19 ƒAƒLƒ…ƒ€ƒŒ[ƒ^‚Ìê‡ ADD_E
        //2018/10/28 ‚r‚t‚rƒ{ƒ‹ƒg}”Ô’Ç‰Á_S
        else if (P_FRAMECD[1] == "18" || P_FRAMECD[1] == "23" ||
            P_FRAMECD[1] == "75") { // ’÷•tƒ{ƒ‹ƒg

            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];

            sBltMat = P_HEADS_DATA[1041].TrimRight(); // Ş¿w’è
            if (sBltMat == "1001" || sBltMat == "30C9" ||
                sBltMat == "31C9") { // ’÷•tƒ{ƒ‹ƒgŞ¿w’è‚r‚t‚r
                if (P_DNO[2] != "") {
                    P_Zuban = P_DNO[2];
                }
                if (P_WKDNO[2] != "") {
                    P_SZuban = P_WKDNO[2];
                }
            }
            else if (sBltMat == "" || sBltMat == "0000") { // Ş¿w’è–³‚µ
                sBltMat = P_HEADS_DATA[1021].TrimRight(); // ‚a‚n‚k‚Q‚Q
                if (sBltMat == "SUS304" || sBltMat == "SS304") {
                    // ’÷•tƒ{ƒ‹ƒg‚a‚n‚k‚Q‚Q ‚r‚t‚r
                    if (P_DNO[2] != "") {
                        P_Zuban = P_DNO[2];
                    }
                    if (P_WKDNO[2] != "") {
                        P_SZuban = P_WKDNO[2];
                    }
                }
            }
        }
        //2019/10/28 ‚r‚t‚rƒ{ƒ‹ƒg}”Ô’Ç‰Á_E
        else {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "";
        }
        // *************************************
    }
    else {
        P_Zuban = "";
        P_SZuban = "";
        P_Siyou3 = "";
    }
    // ***************************************

    // ‚P‘ä•ª”—Ê
    if (Seek_Result == true) {
        if (P_FRAMECD[1] == "09" || P_FRAMECD[1] == "14" ||
            P_FRAMECD[1] == "15" || P_FRAMECD[1] == "16" ||
            P_FRAMECD[1] == "17" || P_FRAMECD[1] == "22") {
            P_1DaiSuryo = 1;
            // 2006.01.10 }”Ô‚ª"‚d‚O‚O‚O‚O‚O‚O"‚à‚µ‚­‚Í"E000000"‚Ìê‡”—Ê‚ğ‚O‚É‚·‚é
            if (P_Zuban == "‚d‚O‚O‚O‚O‚O‚O" || P_Zuban == "E000000") {
                P_1DaiSuryo = 0;
            }
            // **********************************************************************
        }
        else if (P_FRAMECD[1] == "18") { // ’÷•tƒ{ƒ‹ƒg
            P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0);
        }
        // 2014/08/22 ‰—p‹Zp•ÒW
        else if (P_FRAMECD[1] == "75") { // ’÷•tƒ{ƒ‹ƒg(¬Œa)
            P_1DaiSuryo = P_HEADS_DATA[1113].ToIntDef(0);
        }
        else if (P_FRAMECD[1] == "19") { // ‰ô‚è~‚ß
            // 2017.06.23 LX-90D’Ç‰Á
            //P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
            //    P_HEADS_DATA[1287].ToIntDef(0);
            P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                P_HEADS_DATA[1287].ToIntDef(0) +
                P_HEADS_DATA[1113].ToIntDef(0);
            // *********************
        }
        else if (P_FRAMECD[1] == "20") { // ƒXƒ‰ƒXƒgƒƒbƒVƒƒ
            s_Text = G_KS_Syurui + "|" + G_KS_Model;
            // 2011.02.22 ½×½ÄÜ¯¼¬”—ÊğŒ®•ÏX(UX-90H21)
            // 2009.06.19 ½×½ÄÜ¯¼¬”—ÊğŒ®•ÏX(UX-100R’Ç‰Á)
            // 2008.05.21 ½×½ÄÜ¯¼¬”—ÊğŒ®•ÏX(RX-100’Ç‰Á)
            // 2006.02.06 ½×½ÄÜ¯¼¬”—ÊğŒ®•ÏX(SX-90M,L’Ç‰Á)
            // 2006.01.10 ½×½ÄÜ¯¼¬”—ÊğŒ®•ÏX(RX-90’Ç‰Á)
            // if (s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚q‚w|‚V‚O"){
            // if (s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O"){
            // if (s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚X‚O‚l" ||
            // s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O"){
            // if (s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚X‚O‚l" ||
            // s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O" ||
            // s_Text == "‚q‚w|‚P‚O‚O"){
            // if (s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚X‚O‚l" ||
            // s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O" ||
            // s_Text == "‚q‚w|‚P‚O‚O" || s_Text == "‚t‚w|‚P‚O‚O‚q"){
            if (s_Text == "‚t‚w|‚X‚O") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER‚ª200207(‹ŒÌÚ°Ñ)
                    P_1DaiSuryo = 4;
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER‚ª300207(‹ŒÌÚ°Ñ)
                    P_1DaiSuryo = 4;
                }
                else {
                    // H21
                    P_1DaiSuryo = 8;
                }
            }
            // 2016.07.25 ½×½ÄÜ¯¼¬”—ÊğŒ®•ÏX
            //else if (s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚X‚O‚l" ||
            //    s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O" ||
            //    s_Text == "‚q‚w|‚P‚O‚O" || s_Text == "‚t‚w|‚P‚O‚O‚q") {
                // ********************************************
                // ***********************************************
                // *********************************************
                // **********************************************
                // *******************************************
            //    P_1DaiSuryo = 4;
            //}
            else if (s_Text == "‚t‚w|‚W‚O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "‚t‚w|‚P‚O‚O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "‚t‚w|‚P‚O‚O‚q") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "‚t‚w|‚P‚P‚O‚q") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "‚t‚w|‚P‚R‚O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "‚t‚w|‚P‚R‚O‚q") {
                P_1DaiSuryo = 4;
            }
            // 2017.06.23 LX-90D’Ç‰Á
               else if (s_Text == "‚k‚w|‚X‚O‚c") {
                P_1DaiSuryo = 8;
            }
            // *********************
            else if (s_Text == "‚r‚w|‚X‚O‚l") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "‚r‚w|‚X‚O‚k") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "‚q‚w|‚X‚O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "‚q‚w|‚P‚O‚O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "‚v‚w|‚X‚O") {
                P_1DaiSuryo = 4;
            // *********************************
                // 2012.08.23 ½×½ÄÜ¯¼¬”—ÊğŒ®•ÏX(SX-80S)
            }
            else if (s_Text == "‚r‚w|‚W‚O‚r") {
                P_1DaiSuryo = 8;
                // *****************************************
            }
            // 2016.07.25 ½×½ÄÜ¯¼¬”—ÊğŒ®•ÏX
            else if (s_Text == "‚r‚w|‚W‚O‚l") {
                P_1DaiSuryo = 8;
            }
            else if (s_Text == "‚r‚w|‚W‚O‚k") {
                P_1DaiSuryo = 8;
            }
            // *********************************
            else {
                P_1DaiSuryo = P_HEADS_DATA[1020].ToIntDef(0) * 2;
            }
        }
        else if (P_FRAMECD[1] == "23") {
            P_1DaiSuryo = P_HEADS_DATA[1287].ToIntDef(0);
        }
        else if (P_FRAMECD[1] == "21") { // ƒVƒ‡[ƒgƒ{ƒ‹ƒgƒƒbƒVƒƒ
            s_Text = G_KS_Syurui + "|" + G_KS_Model;
            // 2011.02.22 ½×½ÄÜ¯¼¬”—ÊğŒ®•ÏX(UX-90H21)
            // 2009.06.19 ¼®°ÄÎŞÙÄÜ¯¼¬”—ÊğŒ®•ÏX(UX-100R’Ç‰Á)
            // 2008.05.21 ¼®°ÄÎŞÙÄÜ¯¼¬”—ÊğŒ®•ÏX(RX-100’Ç‰Á)
            // 2006.02.06 ¼®°ÄÎŞÙÄÜ¯¼¬”—ÊğŒ®•ÏX(SX-90M,L’Ç‰Á)
            // 2006.01.10 ¼®°ÄÎŞÙÄÜ¯¼¬”—ÊğŒ®•ÏX(RX-90’Ç‰Á)
            // if (s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚q‚w|‚V‚O"){
            // if (s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚X‚O‚l" ||
            // s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O"){
            // if (s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚X‚O‚l" ||
            // s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O" ||
            // s_Text == "‚q‚w|‚P‚O‚O"){
            // if (s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚X‚O‚l" ||
            // s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O" ||
            // s_Text == "‚q‚w|‚P‚O‚O" || s_Text == "‚t‚w|‚P‚O‚O‚q"){
            if (s_Text == "‚t‚w|‚X‚O") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER‚ª200207(‹ŒÌÚ°Ñ)
                    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                        P_HEADS_DATA[1287].ToIntDef(0) - 4;
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER‚ª300207(‹ŒÌÚ°Ñ)
                    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                        P_HEADS_DATA[1287].ToIntDef(0) - 4;
                }
                else {
                    // H21
                    // 2012.02.13 Ü¯¼¬”—ÊğŒ®•ÏX(UX-90H21)
                    // if ((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                    // (P_HEADS_DATA[966].ToIntDef(0) != 0) ||
                    // (P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                    // (P_HEADS_DATA[996].ToIntDef(0) != 0)) {
                    // // EÉ½ŞÙ‚ ‚è
                    // P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) + P_HEADS_DATA[1287].ToIntDef(0) - 4;
                    // } else {
                    // // EÉ½ŞÙ‚È‚µ
                    // P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) + P_HEADS_DATA[1287].ToIntDef(0) - 8;
                    // }
                    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                        P_HEADS_DATA[1287].ToIntDef(0) - 8;
                    // ***************************************
                }
            }
            // 2016.07.25 ¼®°ÄÎŞÙÄÜ¯¼¬”—ÊğŒ®•ÏX
            //else if (s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚X‚O‚l" ||
            //    s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O" ||
            //    s_Text == "‚q‚w|‚P‚O‚O" || s_Text == "‚t‚w|‚P‚O‚O‚q") {
            //    // ************************************************
            //    // ***************************************************
            //    // *************************************************
            //    // **************************************************
            //    // *******************************************
            //    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
            //        P_HEADS_DATA[1287].ToIntDef(0) - 4;
            else if (s_Text == "‚t‚w|‚W‚O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "‚t‚w|‚P‚O‚O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "‚t‚w|‚P‚O‚O‚q") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "‚t‚w|‚P‚P‚O‚q") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "‚t‚w|‚P‚R‚O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "‚t‚w|‚P‚R‚O‚q") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            // 2017.06.23 LX-90D’Ç‰Á
            else if (s_Text == "‚k‚w|‚X‚O‚c") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) +
                    P_HEADS_DATA[1113].ToIntDef(0) - 8;
            }
            // *********************
            else if (s_Text == "‚r‚w|‚X‚O‚l") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "‚r‚w|‚X‚O‚k") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "‚q‚w|‚X‚O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "‚q‚w|‚P‚O‚O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "‚v‚w|‚X‚O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            // *************************************
                // 2012.08.23 ¼®°ÄÎŞÙÄÜ¯¼¬”—ÊğŒ®•ÏX(SX-80S)
            }
            else if (s_Text == "‚r‚w|‚W‚O‚r") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 8;
                // *********************************************
            }
            // 2016.07.25 ¼®°ÄÎŞÙÄÜ¯¼¬”—ÊğŒ®•ÏX
            else if (s_Text == "‚r‚w|‚W‚O‚l") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 8;
            }
            else if (s_Text == "‚r‚w|‚W‚O‚k") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 8;
            }
            // *************************************
            else {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0);
            }
            // 2009.02.16 Ü¯¼¬’Ç‰Á
        }
        else if (P_FRAMECD[1] == "52") { // ‰ô‚è~‚ß
            P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                P_HEADS_DATA[1287].ToIntDef(0);
            // *******************
            // 2012.02.13 Ü¯¼¬”—ÊğŒ®•ÏX(UX-90H21)
        }
        else if (P_FRAMECD[1] == "53") { // ã‰º’[•”ƒ{ƒ‹ƒg—pƒƒbƒVƒƒ
            s_Text = G_KS_Syurui + "|" + G_KS_Model;
            if (s_Text == "‚t‚w|‚X‚O") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER‚ª200207(‹ŒÌÚ°Ñ)
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER‚ª300207(‹ŒÌÚ°Ñ)
                }
                else {
                    // H21
                    if ((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                        (P_HEADS_DATA[966].ToIntDef(0) != 0) ||
                        (P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                        (P_HEADS_DATA[996].ToIntDef(0) != 0)) {
                        // EÉ½ŞÙ‚ ‚è
                        P_1DaiSuryo = 4;
                    }
                    else {
                        // EÉ½ŞÙ‚È‚µ
                        P_1DaiSuryo = 0;
                    }
                }
                // 2012.08.23 ã‰º’[•”ÎŞÙÄ—pÜ¯¼¬”—ÊğŒ®•ÏX(SX-80S)
            }
            else if (s_Text == "‚r‚w|‚W‚O‚r") {
                P_1DaiSuryo = 2;
                // ***************************************************
            }
            // ***************************************
            // 2017.06.23 LX-90D,SX-80LM’Ç‰Á
            else if (s_Text == "‚k‚w|‚X‚O‚c") {
                P_1DaiSuryo = 2;
            }
            else if (s_Text == "‚r‚w|‚W‚O‚k") {
                P_1DaiSuryo = 2;
            }
            else if (s_Text == "‚r‚w|‚W‚O‚l") {
                P_1DaiSuryo = 2;
            }
            // *****************************
        }
        else if (P_FRAMECD[1] == "48") {
            // 2003.12.24 ‚x‚w|‚W‚OƒtƒŒ[ƒ€“–”Â’Ç‰Á
        }
        else if (P_FRAMECD[1] == "06") { // ‚rƒtƒŒ[ƒ€“–”Â
            P_1DaiSuryo = 1;
        }
        else if (P_FRAMECD[1] == "13") { // ‚dƒtƒŒ[ƒ€“–”Â
            sE1NoMat = P_HEADS_DATA[954].TrimRight();
            // 2016.11.07 Ş¿º°ÄŞ•ÏX
            //if (sE1NoMat == "1001" || sE1NoMat == "1003")
            if (Type_MAT(sE1NoMat) == "SUS304" || Type_MAT(sE1NoMat) == "SUS316")
            // ***********************
            { // ‚d‚PƒmƒYƒ‹Ş¿‚ª SUS304[1001],SUS316[1003]j
                P_1DaiSuryo = 1;
            }
            else {
                P_1DaiSuryo = 0;
            }
            // *************************************
            // 2004.03.16 ‚q‚w|‚O‚Oƒx[ƒXƒvƒŒ[ƒg’Ç‰Á
        }
        else if (P_FRAMECD[1] == "49") { // ƒx[ƒXƒvƒŒ[ƒg
            P_1DaiSuryo = 1;
            // ***************************************
        }
        // 2021.10.25 ½Íß°»°’Ç‰Á_S
        else if (P_FRAMECD[1] == "79") { // ƒXƒy[ƒT[
            s_Text = G_KS_Syurui + "|" + G_KS_Model;
            // 2022.01.24 ƒXƒy[ƒT’Ç‰Á_S
            // ‚rƒtƒŒ[ƒ€ƒXƒy[ƒT
            sAGasMat = P_HEADS_DATA[836].TrimRight();
            sBGasMat = P_HEADS_DATA[835].TrimRight();
            P_1DaiSuryo = 0;
            //if (s_Text == "‚v‚g|‚R‚O") {
            //    if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
            //        (P_HEADS_DATA[966].ToIntDef(0) == 0) &&
            //        (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
            //        (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
            //        // EÉ½ŞÙ–³‚¢ê‡
            //        P_1DaiSuryo = 1;
            //    }
            //    else {
            //        P_1DaiSuryo = 2;
            //        // EÉ½ŞÙ—L‚éê‡
            //    }
            //}
            //else {
            //    P_1DaiSuryo = 0;
            //}
            if (s_Text == "‚t‚w|‚X‚O") {
                // UX-90
                if (sAGasMat == "2067" || sBGasMat == "2067" ) {
                    // GZ-NBR
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "‚k‚w|‚P‚O") {
                // LX-10
                if (sAGasMat == "2063" || sBGasMat == "2063" ||
                    sAGasMat == "2064" || sBGasMat == "2064" ) {
                    // TCG
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "‚v‚g|‚R‚O") {
                // WH-30
                P_1DaiSuryo = 1;
            }
            //2023.03.20 CX-03-YC³_S
            ////2023.03.09 CX-03-Y’Ç‰Á_S
            //else if (((s_Text == "‚b‚w|‚O‚R") || (s_Text == "‚b‚w|‚O‚R‚c") ||
            //          (s_Text == "‚b‚w‚v|‚O‚R")) &&
            //          (P_HEADS_DATA[36].Pos("Y") != 0)) {
            //    P_1DaiSuryo = 2;
            //}
            ////2023.03.09 CX-03-Y’Ç‰Á_E
            //2023.03.20 CX-03-YC³_E

            // *************************
            // ***  E–¾d—l‚Ìì¬
            // *************************
            if (P_1DaiSuryo == 1) {
                P_Siyou1 = Get_AnaakeSiyou_S("S");
                G_Log_Text = "‚rƒtƒŒ[ƒ€ƒXƒy[ƒT[E–¾d—l  w" + P_Siyou1 + "x‚ğæ“¾B";
                Write_Log(G_Log_Text);
            }
            // 2022.01.24 ƒXƒy[ƒT’Ç‰Á_E
        }
        else if (P_FRAMECD[1] == "80") { // ƒVƒ€ƒvƒŒ[ƒg[
            s_Text = G_KS_Syurui + "|" + G_KS_Model;
            if (s_Text == "‚v‚g|‚R‚O") {
                P_1DaiSuryo = 4;
            }
            else {
                P_1DaiSuryo = 0;
            }
        }
        // 2021.10.25 ½Íß°»°’Ç‰Á_E

        // 2022.01.24 ƒXƒy[ƒT’Ç‰Á_S
        // 2022.xx.xx EƒXƒy[ƒT[CODE•ÏX_S
        //else if (P_FRAMECD[1] == "81") { // ‚dƒtƒŒ[ƒ€ƒXƒy[ƒT[
        else if (P_FRAMECD[1] == "82") { // ‚dƒtƒŒ[ƒ€ƒXƒy[ƒT[
        // 2022.xx.xx EƒXƒy[ƒT[CODE•ÏX_E
            s_Text = G_KS_Syurui + "|" + G_KS_Model;
            sAGasMat = P_HEADS_DATA[836].TrimRight();
            sBGasMat = P_HEADS_DATA[835].TrimRight();
            P_1DaiSuryo = 0;
            if (s_Text == "‚t‚w|‚X‚O") {
                // UX-90
                if (sAGasMat == "2067" || sBGasMat == "2067" ) {
                    // GZ-NBR
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "‚k‚w|‚P‚O") {
                // LX-10
                if (sAGasMat == "2063" || sBGasMat == "2063" ||
                    sAGasMat == "2064" || sBGasMat == "2064" ) {
                    // TCG
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "‚v‚g|‚R‚O") {
                // WH-30
                P_1DaiSuryo = 1;
            }

            // *************************
            // ***  E–¾d—l‚Ìì¬
            // *************************
            if (P_1DaiSuryo == 1) {
                P_Siyou1 = Get_AnaakeSiyou_S("E");
                G_Log_Text = "‚dƒtƒŒ[ƒ€ƒXƒy[ƒT[E–¾d—l  w" + P_Siyou1 + "x‚ğæ“¾B";
                Write_Log(G_Log_Text);
            }
            if ( P_Siyou1  == "E0000") {
                // ƒmƒYƒ‹‚ª‚È‚¢ê‡‚dƒXƒy[ƒT[•s—v
                P_1DaiSuryo = 0;
            }
        }
        // 2022.01.24 ƒXƒy[ƒT’Ç‰Á_E
    }
    else {
        P_1DaiSuryo = 0;
    }

    // ‘Œv
    l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // ‘ä”
    P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;

    // d—l‚P
    // HanToZen(s_JPKbn);
    if (Seek_Result == true) {

        s_Text = P_FRAMECD[1];

        // 2014/08/22 ‰—p‹Zp•ÒW
        if (s_Text == "18" || s_Text == "23" || s_Text == "75") { // ’÷•tƒ{ƒ‹ƒg

            // QÆ‚·‚éƒf[ƒ^ƒCƒ“ƒfƒbƒNƒX
            int i_BoltRadiusNo = 1019;  // ƒ{ƒ‹ƒgŒa
            int i_BoltLengthNo = 1376;  // ƒ{ƒ‹ƒg‘S’·

            if(s_Text == "75") {
                // BNWRN-,’÷•tBNW‚Ìê‡AQÆƒf[ƒ^ƒCƒ“ƒfƒbƒNƒX‚ğ•ÏX
                i_BoltRadiusNo = 1108;
                i_BoltLengthNo = 1377;

                // 2014/09/04 ‰—p‹Zp•ÒW
                if(StrToDblDef(P_HEADS_DATA[i_BoltLengthNo].TrimRight(), 0.0) <= 0.0) {
                    // Sƒ{ƒ‹ƒg‘S’·‚ª0mm(–¢İ’è)‚Ìê‡ALƒ{ƒ‹ƒg‘S’·‚ğÌ—p‚·‚é
                    i_BoltLengthNo = 1376;
                }
            }

            // Ş¿Œˆ’èƒƒWƒbƒN
            sBltMat = P_HEADS_DATA[1041].TrimRight(); // Ş¿w’è
            if (sBltMat == "1001") {
                P_ZaisituNM = "SUS304/C3604";
                P_EZaisituNM = "SUS304/C3604";
                // 2011.05.30 ÎŞÙÄŞ¿w’è’Ç‰Á
            }
            // 2016.09.01 Ş¿º°ÄŞ•ÏX
            else if (sBltMat == "30C9") {
                P_ZaisituNM = "SUS304/C3604";
                P_EZaisituNM = "SUS304/C36004";
            }
            else if (sBltMat == "31C9") {
                P_ZaisituNM = "SUS304/C3604";
                P_EZaisituNM = "SUS304/C3604";
            }
            else if (sBltMat == "1040") {
                P_ZaisituNM = "SS400/SS400";
                P_EZaisituNM = "SS400/SS400";
            }
            // 2016.11.07 Ş¿º°ÄŞ•ÏX
            else if (sBltMat == "1062") {
                P_ZaisituNM = "SS400/SS400";
                P_EZaisituNM = "SS400/SS400";
            }
            // ***********************
            else if (sBltMat == "30C1") {
                P_ZaisituNM = "SS400/SS400";
                P_EZaisituNM = "SS400/SS400";
            }
            else if (sBltMat == "31C1") {
                P_ZaisituNM = "SS400/SS400";
                P_EZaisituNM = "SS400/SS400";
            }
            else if (sBltMat == "1069") {
                P_ZaisituNM = "SNB7/S45C";
                P_EZaisituNM = "SNB7/S45C";
            }
            else if (sBltMat == "30C2") {
                P_ZaisituNM = "SNB7/S45C";
                P_EZaisituNM = "SNB7/S45C";
            }
            else if (sBltMat == "31C2") {
                P_ZaisituNM = "SNB7/S45C";
                P_EZaisituNM = "SNB7/S45C";
            }
            // **********************
            else if (sBltMat == "1086") {
                P_ZaisituNM = "SA193B7/SA194-2H";
                P_EZaisituNM = "SA193B7/SA194-2H";
                // ***************************
            }
            // 2016.09.01 Ş¿º°ÄŞ•ÏX
            else if (sBltMat == "32C2") {
                P_ZaisituNM = "SA193B7/SA194-2H";
                P_EZaisituNM = "SA193B7/SA194-2H";
            // **********************
            }
            // 2015.01.09 ÎŞÙÄŞ¿w’è’Ç‰Á
            else if (sBltMat == "1169") {
                P_ZaisituNM = "A193B7/A194-2H";
                P_EZaisituNM = "A193-B7/A194-2H";
            }
            else if (sBltMat == "1171") {
                P_ZaisituNM = "A193B7/A194-2H";
                P_EZaisituNM = "A193-B7/A194-2H";
            }
            // 2016.09.01 Ş¿º°ÄŞ•ÏX
            else if (sBltMat == "33C2") {
                P_ZaisituNM = "A193B7/A194-2H";
                P_EZaisituNM = "A193-B7/A194-2H";
            // **********************
            }
            // ***************************
            // 2016.05.10 ÎŞÙÄŞ¿w’è’Ç‰Á
            else if (sBltMat == "1186") {
                // 2016.11.07 Ş¿º°ÄŞ•ÏX
                //P_ZaisituNM = "SNB7/S45C eqv.";
                P_ZaisituNM = "SNB7/S45C‘Š“–";
                // ***********************
                P_EZaisituNM = "SNB7/S45C eqv.";
            }
            // ***************************
            else {
                sBltMat = P_HEADS_DATA[1021].TrimRight(); // BOL22
                if (sBltMat == "SUS304") {
                    P_ZaisituNM = "SUS304/C3604";
                    P_EZaisituNM = "SUS304/C3604";
                }
                else if (sBltMat == "SS400") {
                    P_ZaisituNM = "SS400/SS400";
                    P_EZaisituNM = "SS400/SS400";
                }
                else if (sBltMat == "S45C-D") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                else if (sBltMat == "SCM435") {
                    P_ZaisituNM = "SCM435/S45C";
                    P_EZaisituNM = "SCM435/S45C";
                }
                else if (sBltMat == "SCM445") {
                    P_ZaisituNM = "SCM435/S45C";
                    P_EZaisituNM = "SCM435/S45C";
                }
                else if (sBltMat == "S45CH") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                else if (sBltMat == "SNB 7") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                else if (sBltMat == "SNB7") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                // 2014/08/26 ‰—p‹Zp•ÒW
                else if (sBltMat == "45/35") {
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
                else if (sBltMat == "35CrMo") {
                    P_ZaisituNM = "35CrMo/32CrMo";
                    P_EZaisituNM = "35CrMo/32CrMo";
                }
                // 2021.09.22 RX-70Z’Ç‰Á_S
                else if (sBltMat == "35CrMo2") {
                    P_ZaisituNM = "35CrMo/45";
                    P_EZaisituNM = "35CrMo/45";
                }
                // 2021.09.22 RX-70Z’Ç‰Á_E
                // 2015.01.09 ÎŞÙÄŞ¿w’è’Ç‰Á
                else if (sBltMat == "SA193-B7") {
                    P_ZaisituNM = "SA193B7/SA194-2H";
                    P_EZaisituNM = "SA193B7/SA194-2H";
                }
                else if (sBltMat == "A193-B7") {
                    P_ZaisituNM = "A193-B7/A194-2H";
                    P_EZaisituNM = "A193-B7/A194-2H";
                }
                // ***************************
                // 2016.11.07 Ş¿º°ÄŞ•ÏX
                else if (sBltMat == "C.S.") {
                    P_ZaisituNM = "SS400/SS400";
                    P_EZaisituNM = "SS400/SS400";
                }
                else if (sBltMat == "SNB7 eqv.") {
                    P_ZaisituNM = "SNB7/S45C‘Š“–";
                    P_EZaisituNM = "SNB7/S45C eqv.";
                }
                else if (sBltMat == "B7") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                else if (sBltMat == "SS304") {
                    P_ZaisituNM = "SUS304/C3604";
                    P_EZaisituNM = "SUS304/C3604";
                }
                // 2017.06.23 LX-90D
                else if (sBltMat == "SA540-B23") {
                    P_ZaisituNM = "SA540B23/SA194-2H";
                    P_EZaisituNM = "SA540B23/SA194-2H";
                }
                // *****************
                // ***********************
                // 2018.06.19 LX-30Z’Ç‰Á_S
                else if (sBltMat == "Q235B") {
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
                else if (sBltMat == "Q235B2") {
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
                // 2018.06.19 LX-30Z’Ç‰Á_E
                // 2020.05.27 LX-50Z’Ç‰Á_S
                else if (sBltMat == "45") {
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
                else if (sBltMat == "45_2") {
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
                // 2020.05.27 LX-50Z’Ç‰Á_E
                // 2021.09.22 RX-50Z,70Z’Ç‰Á_S
                else if (sBltMat == "42CrMo") {
                    P_ZaisituNM = "42CrMo/45";
                    P_EZaisituNM = "42CrMo/45";
                }
                else if (sBltMat == "42CrMo2") {
                    P_ZaisituNM = "42CrMo/45";
                    P_EZaisituNM = "42CrMo/45";
                }
                // 2021.09.22 RX-50Z,70Z’Ç‰Á_E
                else {
                    P_ZaisituNM = "";
                    P_EZaisituNM = "";
                    sBltMat = "0";
                }
            }

            // 2019.11.21 LX-90D-NUPŞ¿•ÏX_S
            //// 2017.06.23 LX-90D-NUP’Ç‰Á
            //s_Text = G_KS_Syurui + "|" + G_KS_Model;
            //if( P_FRAMECD[1] == "18" ) {
            //    if (( s_Text == "‚k‚w|‚X‚O‚c") && ( P_HEADS_DATA[37].ToIntDef(0)) == 94 ){
            //        // LX-90D-KNUP
            //        P_ZaisituNM = "SA540B23/SA194-2H";
            //        P_EZaisituNM = "SA540B23/SA194-2H";
            //    } else if (( s_Text == "‚k‚w|‚X‚O") && ( P_HEADS_DATA[37].ToIntDef(0)) == 394 ){
            //        // LX-90D-TNUP
            //        P_ZaisituNM = "SA540B23/SA194-2H";
            //        P_EZaisituNM = "SA540B23/SA194-2H";
            //    }
            //}
            //// *************************
            // 2019.11.21 LX-90D-NUPŞ¿•ÏX_E

            // 2020.05.27 LX-50Z’Ç‰Á_E
            s_Text = G_KS_Syurui + "|" + G_KS_Model;
            // 2020.05.27 LX-50Z’Ç‰Á_E

            // 2018.06.19 LX-30Z’Ç‰Á_S
            if (sBltMat == "Q235B2") {
                if (( s_Text == "‚k‚w|‚R‚O" ) && ( P_FRAMECD[1] == "75" )){
                    // LX-30Z ×ÎŞÙÄ 45/35
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
            }
            // 2018.06.19 LX-30Z’Ç‰Á_E

            // 2020.05.27 LX-50Z’Ç‰Á_S
            if ( sBltMat == "45_2" ) {
                if (( s_Text == "‚k‚w|‚T‚O" ) && ( P_FRAMECD[1] == "75" )){
                    // LX-50Z ×ÎŞÙÄ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }
            // 2020.05.27 LX-50Z’Ç‰Á_E

            // 2021.09.22 RX-50Z,70Z’Ç‰Á_S
            if ( sBltMat == "45_2" ) {
                if (( s_Text == "‚q‚w|‚T‚O" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-50Z ×ÎŞÙÄ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                } else if (( s_Text == "‚q‚w|‚V‚O" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-70Z ×ÎŞÙÄ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }

            if ( sBltMat == "42CrMo2" ) {
                if (( s_Text == "‚q‚w|‚T‚O" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-50Z ×ÎŞÙÄ 45/35
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
            }

            if ( sBltMat == "35CrMo2" ) {
                if (( s_Text == "‚q‚w|‚V‚O" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-70Z ×ÎŞÙÄ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }
            // 2021.09.22 RX-50Z,70Z’Ç‰Á_E

            // 2012.01.20 ’†‘Ş¿
            i_RecNo = 1107;
            s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

            s_Text = G_KS_Syurui + "|" + G_KS_Model;
            if (s_Text == "‚t‚w|‚P‚O") { // ’†‘Ş¿
            }
            else if (s_Text == "‚k‚w|‚O‚O") { // ’†‘Ş¿
            }
            else if (s_Text == "‚k‚w|‚P‚O") { // ’†‘Ş¿
            }
            else if (s_Text == "‚k‚w|‚R‚O") { // ’†‘Ş¿
            }
            else if (s_Text == "‚k‚w|‚T‚O") { // ’†‘Ş¿
            }
            else if (s_Text == "‚r‚w|‚Q‚O") { // ’†‘Ş¿
            }
            else if (s_Text == "‚r‚w|‚S‚P") { // ’†‘Ş¿
            }
            else if (s_Text == "‚r‚w|‚S‚R") { // ’†‘Ş¿
            }
            else if (s_Text == "‚q‚w|‚P‚P") { // ’†‘Ş¿
            }
            else if (s_Text == "‚q‚w|‚P‚R") { // ’†‘Ş¿
            }
            else if (s_Text == "‚q‚w|‚R‚O") { // ’†‘Ş¿
            }
            else if (s_Text == "‚q‚w|‚T‚O") { // ’†‘Ş¿
            }
            else if (s_Text == "‚q‚w|‚V‚O") { // ’†‘Ş¿
            }
            else {
                s_ChinaM = "";
            }

            if (s_ChinaM == "CHINA") { // ’†‘Ş¿
                if (P_ZaisituNM == "SS400/SS400") {
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }
            // *******************

            s_Text = P_HEADS_DATA[i_BoltRadiusNo].TrimRight();
            if (s_Text == "1.630") {
                //2020.10.02 ƒ{ƒ‹ƒg¡–@•ÏX_S
                //P_Siyou1 =
                //    "‚P@‚T^‚W|‚W‚t‚m~" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight())
                //    + "‚k~‘S‚r";
                P_Siyou1 =
                    "1 5/8-8UN~" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight())
                    + "‚k~‘S‚r";
                //2020.10.02 ƒ{ƒ‹ƒg¡–@•ÏX_E

            }
            else if (s_Text == "2.000") {
                P_Siyou1 = "‚Q|‚W‚t‚m~" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight())
                    + "‚k~‘S‚r";

            }
            else {
                P_Siyou1 = "‚l" + HanToZen(P_HEADS_DATA[i_BoltRadiusNo].SubString(1, 2)) +
                    "~" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight()) + "‚k~‘S‚r";

            }
            P_Yobisu = Get_Blt_YobiSu(StrToDblDef(s_Text.TrimRight(), 0.0),
                StrToDblDef(P_HEADS_DATA[i_BoltLengthNo].TrimRight(), 0.0), sBltMat,
                m_pcHeadsData);
            P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;
        }
        else if (s_Text == "26") {
            P_Siyou1 = HanToZen(P_HEADS_DATA[1092].TrimRight()) + "‚k";

        }
        else if (s_Text == "27") {
            P_Siyou1 = HanToZen(P_HEADS_DATA[1096].TrimRight()) + "‚k@ƒLƒƒƒbƒv•t‚«";

        }
        else if (s_Text == "28") {
            P_Siyou1 = HanToZen(P_HEADS_DATA[1097].TrimRight()) + "‚k@ƒLƒƒƒbƒv•t‚«";

            // 2012.02.13 Ü¯¼¬”—ÊğŒ®•ÏX(UX-90H21)
            //// 2010.04.19 UX-90¼®°ÄÎŞÙÄÜ¯¼¬•ÏX
            // } else if (s_Text == "21" ) {
        }
        else if (s_Text == "53") {
            // ã‰º’[•”ƒ{ƒ‹ƒg—pƒƒbƒVƒƒ
            // ***************************************
            s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
            if (s_Text2 == "‚t‚w|‚X‚O") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER‚ª200207(‹ŒÌÚ°Ñ)
                    P_Siyou1 = "";
                    // 2010.12.14 UX-90—‚Æ‚µ‚İGB’Ç‰Á
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER‚ª300207(‹ŒÌÚ°Ñ—‚µ‚İGB)
                    // ********************************
                }
                else {
                    if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                        // S1Œpèº°ÄŞ
                        s_Text2 = P_HEADS_DATA[893].TrimRight();
                    }
                    else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                        // S2Œpèº°ÄŞ
                        s_Text2 = P_HEADS_DATA[908].TrimRight();
                    }
                    else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                        // S3Œpèº°ÄŞ
                        s_Text2 = P_HEADS_DATA[923].TrimRight();
                    }
                    else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                        // S4Œpèº°ÄŞ
                        s_Text2 = P_HEADS_DATA[938].TrimRight();
                    }
                    else if ((P_HEADS_DATA[951].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[951].ToIntDef(0) == 2)) {
                        // E1Œpèº°ÄŞ
                        s_Text2 = P_HEADS_DATA[953].TrimRight();
                    }
                    else if ((P_HEADS_DATA[966].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[966].ToIntDef(0) == 2)) {
                        // E2Œpèº°ÄŞ
                        s_Text2 = P_HEADS_DATA[968].TrimRight();
                    }
                    else if ((P_HEADS_DATA[981].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[981].ToIntDef(0) == 2)) {
                        // E3Œpèº°ÄŞ
                        s_Text2 = P_HEADS_DATA[983].TrimRight();
                    }
                    else if ((P_HEADS_DATA[996].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[996].ToIntDef(0) == 2)) {
                        // E4Œpèº°ÄŞ
                        s_Text2 = P_HEADS_DATA[998].TrimRight();
                    }
                    else {
                        // É½ŞÙ‚È‚µ
                        s_Text2 = "999";
                    }

                    // 2011.06.20 É½ŞÙ‹KŠi•ÏX
                    // if (s_Text2 == "414" || s_Text2 == "424" || s_Text2 == "434" ||
                    // s_Text2 == "514" || s_Text2 == "524" || s_Text2 == "534" ||
                    // s_Text2 == "814" || s_Text2 == "824" || s_Text2 == "834" ||
                    // s_Text2 == "914" || s_Text2 == "924" || s_Text2 == "934") {
                    if (s_Text2 == "0414" || s_Text2 == "0424" ||
                        s_Text2 == "0434" || s_Text2 == "0514" ||
                        s_Text2 == "0524" || s_Text2 == "0534" ||
                        s_Text2 == "0816" || s_Text2 == "0826" ||
                        s_Text2 == "0836" || s_Text2 == "0916" ||
                        s_Text2 == "0926" || s_Text2 == "0936") {
                        // ***********************
                        // ANSI,JPI
                        P_Siyou1 = "ƒÓ‚`‚V‚X¦¦è”z’ˆÓ‚Ì‚±‚Æ¦¦";
                    }
                    else if (s_Text2 == "999") {
                        // É½ŞÙ‚È‚µ
                        P_Siyou1 = "“WŠJ•s”\";
                    }
                    else {
                        // JIS,DIN
                        P_Siyou1 = "ƒÓ‚`‚W‚T";
                    }
                }
                // 2014.04.15 Ü¯¼¬”—ÊğŒ®’Ç‰Á(UX-10-EP,LX-10,EP)
                //// 2012.06.08 Ü¯¼¬”—ÊğŒ®•ÏX(RX-10-EP)
                // } else if (s_Text2 == "‚q‚w|‚P‚P" || s_Text2 == "‚q‚w|‚P‚R") {
            }
            else if (s_Text2 == "‚t‚w|‚P‚O" || s_Text2 == "‚k‚w|‚P‚O" ||
                s_Text2 == "‚q‚w|‚P‚P" || s_Text2 == "‚q‚w|‚P‚R") {
                // ************************************************
                if ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                    (P_HEADS_DATA[996].ToIntDef(0) != 0)) {
                    // E3,E4É½ŞÙ—L‚èŞ
                    P_1DaiSuryo = 2;
                    P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;
                    P_Siyou4 = "®";
                }
                else {
                    // É½ŞÙ‚È‚µ
                    P_1DaiSuryo = 0;
                    P_Soukei = 0;
                    P_Siyou4 = "";
                }
                // ****************************************
            }
            else {
                P_Siyou1 = "";
            }
            // *********************************
            // 2011.03.11 RX-70»Îß°Ä’‹L’Ç‰Á
        }
        else if (s_Text == "16") {
            s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
            if (s_Text2 == "‚q‚w|‚V‚O") {
                if (P_HEADS_DATA[1046].TrimRight() != "") {
                    P_Siyou1 = P_HEADS_DATA[1046].TrimRight() + "—p";
                }
                else {
                    P_Siyou1 = "";
                }
            }
            else {
                P_Siyou1 = "";
            }
            // ************************
        }
        // 2022.01.14 ƒXƒy[ƒT’Ç‰Á_S
        // 2022.08.05EƒXƒy[ƒT[CODE•ÏX_S
        //else if (s_Text == "79"|| s_Text == "81") {
        else if (s_Text == "79"|| s_Text == "82") {
        // 2022.08.05EƒXƒy[ƒT[CODE•ÏX_E
            // ƒXƒy[ƒT
            P_Siyou1 = "E–¾" + P_Siyou1;
            //2023.03.20 CX-03-YC³_S
            ////2023.03.09 CX-03-Y’Ç‰Á_S
            //s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
            //if (((s_Text2 == "‚b‚w|‚O‚R") || (s_Text2 == "‚b‚w|‚O‚R‚c") ||
            //     (s_Text2 == "‚b‚w‚v|‚O‚R")) &&
            //     (P_HEADS_DATA[36].Pos("Y") != 0)) {
            //    P_Siyou1 = "";
            //}
            ////2023.03.09 CX-03-Y’Ç‰Á_E
            //2023.03.20 CX-03-YC³_E
        }
        // 2022.01.14 ƒXƒy[ƒT’Ç‰Á_E
        else {
            P_Siyou1 = "";
        }

    }
    else {
        P_Siyou1 = "";
    }

    // d—l‚Q(S,EÌÚ°Ñí—Ş)
    if (Seek_Result == true) {
        s_Text = P_FRAMECD[1];
        if (s_Text == "09" || s_Text == "16") {
            if (P_HEADS_DATA[1290].TrimRight() != "") {
                P_Siyou2 = P_HEADS_DATA[1290].TrimRight() + "—p";
            }
            else {
                P_Siyou2 = "";
            }

        }
        else if (s_Text == "14") {
            if (P_HEADS_DATA[1290].TrimRight() != "") {
                P_Siyou2 = P_HEADS_DATA[1290].TrimRight();
            }
            else {
                P_Siyou2 = "";
            }

            // 2010.12.14 ‰ºGB’‹L’Ç‰Á
        }
        else if (s_Text == "15") {
            if (P_HEADS_DATA[1046].TrimRight() != "") {
                P_Siyou2 = P_HEADS_DATA[1046].TrimRight();
            }
            else {
                P_Siyou2 = "";
            }
            // ***********************

        }
        // 2014/08/22 ‰—p‹Zp•ÒW
        else if (s_Text == "18" || s_Text == "23" || s_Text == "24" ||
                 s_Text == "25" || s_Text == "75") {
            P_Siyou2 = P_HEADS_DATA[1317].TrimRight();
        }
        else {
            P_Siyou2 = "";
        }

    }
    else {
        P_Siyou2 = "";
    }

    // d—l‚S  ( 02.08.09 ’Ç‰Á )
    // 2014/08/22 ‰—p‹Zp•ÒW
    if ((s_Text == "18") || (s_Text == "23") || (s_Text == "14") ||
        (s_Text == "15") || (s_Text == "75")) {
        P_Siyou4 = "®";
    }
    // 2009.02.25 WX-50,CX-10Dƒx[ƒXƒvƒŒ[ƒg’Ç‰Á
    else if (s_Text == "49") {
        s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
        // 2009.07.31 ƒx[ƒXƒvƒŒ[ƒg”—Ê•ÏX
        // RX-00    ‚P
        // ‚»‚Ì‘¼   ‚P®
        // if (s_Text2 == "‚v‚w|‚T‚R") {
        // P_Siyou4 = "®";
        // } else if  (s_Text2 == "‚b‚w|‚P‚O‚c") {
        // P_Siyou4 = "®";
        // }
        if (s_Text2 == "‚q‚w|‚O‚O") {
            P_Siyou4 = "";
        }
        else {
            P_Siyou4 = "®";
        }
        // *********************************
        // 2010.04.19 UX-90¼®°ÄÎŞÙÄÜ¯¼¬•ÏX
    }
    else if (s_Text == "21") {
        s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
        if (s_Text2 == "‚t‚w|‚X‚O") {
            if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                // HEAD.VER‚ª200207(‹ŒÌÚ°Ñ)
                P_Siyou4 = "";
            }
            else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                // EÉ½ŞÙ–³‚¢ê‡
                P_Siyou4 = "";
            }
            else {
                // VÌÚ°Ñ‚ÅEÉ½ŞÙ•t‚«
                P_Siyou4 = "®";
            }
        }
        else {
            P_Siyou4 = "";
        }
        // *********************************
    }
    // *****************************************
    else {
        P_Siyou4 = "";
    }

    // d—l‚T
    P_Siyou5 = "";

    // ****************************
    // ***  S,EÌÚ°ÑÃŞ°À  ƒZƒbƒg
    // ****************************
    // •”•i“WŠJÃŞ°À@OPEN
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

    // 1  •”•i•ª—Ş
    s_Text = P_BBunrui;
    fputs(s_Text.c_str(), fp);
    // 2  ¯•Êq
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), fp);
    // 3  •”•iŒ^®
    s_Text = "," + P_BKatasiki;
    fputs(s_Text.c_str(), fp);
    // 4  •”•i–¼Ì
    s_Text = "," + P_BMeisyou;
    fputs(s_Text.c_str(), fp);
    // 5  Ş¿–¼Ì
    s_Text = "," + P_ZaisituNM;
    fputs(s_Text.c_str(), fp);
    // 6  }”Ô
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), fp);
    // 7  ì}—p}”Ô
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), fp);
    // 8  1‘ä•ª”—Ê
    s_Text = "," + AnsiString(P_1DaiSuryo);
    fputs(s_Text.c_str(), fp);
    // 9  —\”õ”
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), fp);

    // 10 ‘Œv(1‘ä•ª”—Ê~HEADSÃŞ°À“à‘ä”+—\”õ”)
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), fp);

    // 11 d—l‚P
    s_Text = "," + P_Siyou1;
    fputs(s_Text.c_str(), fp);
    // 12 d—l‚Q
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), fp);
    // 13 d—l‚R
    s_Text = "," + P_Siyou3;
    fputs(s_Text.c_str(), fp);
    // 14 d—l‚S
    s_Text = "," + P_Siyou4;
    fputs(s_Text.c_str(), fp);
    // 15 d—l‚T
    s_Text = "," + P_Siyou5;
    fputs(s_Text.c_str(), fp);
    // 16  Ş¿–¼Ì(‰p)
    s_Text = "," + P_EZaisituNM;
    fputs(s_Text.c_str(), fp);

    // ------------------------------
    // 2007.10.03 Ğ×°d—l‘Î‰
    // 17 d—lÀ²Ìßº°ÄŞ
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), fp);
    // 2007.10.03
    // ------------------------------

    // ‰üs•¶š
    s_Text = "\n";
    // CSVÃŞ°À@Ì§²Ùo—Í
    fputs(s_Text.c_str(), fp);

    // ƒ{ƒ‹ƒg’·’Z—L‚è‚Ìˆ—
    if (Seek_Result == true) {
        s_Text = P_FRAMECD[1];
        P_1DaiSuryo = P_HEADS_DATA[1287].ToIntDef(0);
        // ‘Œv
        l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // ‘ä”
        P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;
        if ((s_Text == "18" || s_Text == "23")) {
            if ((P_1DaiSuryo > 0)) {
                s_Text = P_HEADS_DATA[1019].TrimRight(); // BDI
                if (s_Text == "1.630") {
                    //2020.10.02 ƒ{ƒ‹ƒg¡–@•ÏX_S
                    //P_Siyou1 =
                    //    "‚P@‚T^‚W|‚W‚t‚m~" + HanToZen(P_HEADS_DATA[1377].TrimRight())
                    //    + "‚k~‘S‚r";
                    P_Siyou1 =
                        "1 5/8-8UN~" + HanToZen(P_HEADS_DATA[1377].TrimRight())
                        + "‚k~‘S‚r";
                    //2020.10.02 ƒ{ƒ‹ƒg¡–@•ÏX_E

                }
                else if (s_Text == "2.000") {
                    P_Siyou1 =
                        "‚Q|‚W‚t‚m~" + HanToZen(P_HEADS_DATA[1377].TrimRight())
                        + "‚k~‘S‚r";

                }
                else {
                    P_Siyou1 =
                        "‚l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) +
                        "~" + HanToZen(P_HEADS_DATA[1377].TrimRight()) + "‚k~‘S‚r";

                }
                P_Yobisu = Get_Blt_YobiSu(StrToDblDef(s_Text.TrimRight(), 0.0),
                    StrToDblDef(P_HEADS_DATA[1377].TrimRight(), 0.0), sBltMat,
                    m_pcHeadsData);
                P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;

                // 1  •”•i•ª—Ş
                s_Text = P_BBunrui;
                fputs(s_Text.c_str(), fp);
                // 2  ¯•Êq
                s_Text = "," + P_Sikibetu;
                fputs(s_Text.c_str(), fp);
                // 3  •”•iŒ^®
                s_Text = ",";
                fputs(s_Text.c_str(), fp);
                // 4  •”•i–¼Ì
                s_Text = ",";
                fputs(s_Text.c_str(), fp);
                // 5  Ş¿–¼Ì
                s_Text = "," + P_ZaisituNM;
                fputs(s_Text.c_str(), fp);
                // 6  }”Ô
                s_Text = "," + P_Zuban;
                fputs(s_Text.c_str(), fp);
                // 7  ì}—p}”Ô
                s_Text = "," + P_SZuban;
                fputs(s_Text.c_str(), fp);
                // 8  1‘ä•ª”—Ê
                s_Text = "," + AnsiString(P_1DaiSuryo);
                fputs(s_Text.c_str(), fp);
                // 9  —\”õ”
                s_Text = "," + AnsiString(P_Yobisu);
                fputs(s_Text.c_str(), fp);

                // 10 ‘Œv(1‘ä•ª”—Ê~HEADSÃŞ°À“à‘ä”+—\”õ”)
                s_Text = "," + AnsiString(P_Soukei);
                fputs(s_Text.c_str(), fp);

                // 11 d—l‚P
                s_Text = "," + P_Siyou1;
                fputs(s_Text.c_str(), fp);
                // 12 d—l‚Q
                s_Text = "," + P_Siyou2;
                fputs(s_Text.c_str(), fp);
                // 13 d—l‚R
                s_Text = "," + P_Siyou3;
                fputs(s_Text.c_str(), fp);
                // 14 d—l‚S
                s_Text = "," + P_Siyou4;
                fputs(s_Text.c_str(), fp);
                // 15 d—l‚T
                s_Text = "," + P_Siyou5;
                fputs(s_Text.c_str(), fp);
                // 16  Ş¿–¼Ì(‰p)
                s_Text = "," + P_EZaisituNM;
                fputs(s_Text.c_str(), fp);

                // ------------------------------
                // 2007.10.03 Ğ×°d—l‘Î‰
                // 17 d—lÀ²Ìßº°ÄŞ
                s_Text = "," + sSpecCode;
                fputs(s_Text.c_str(), fp);
                // 2007.10.03
                // ------------------------------

                // ‰üs•¶š
                s_Text = "\n";
                // CSVÃŞ°À@Ì§²Ùo—Í
                fputs(s_Text.c_str(), fp);
            }

            // 2017.06.23 LX-90D-NUP’Ç‰Á
            // 2014/08/22 ‰—p‹Zp•ÒW
            //if(P_HEADS_DATA[36].Pos("Z") == 0) {
            //    // ZƒtƒŒ[ƒ€‚Å–³‚¢ê‡A‚±‚Ìƒ^ƒCƒ~ƒ“ƒO‚Åƒ{ƒ‹ƒgƒJƒo[o—Í‚Ö
            //    BLT_Cover_Out();
            //}
            s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
            if (( s_Text2 == "‚k‚w|‚X‚O‚c") && ( P_HEADS_DATA[37].ToIntDef(0)) == 94 ){
                // LX-90D-KNUP‚Ìê‡A•Ê‚Ìƒ^ƒCƒ~ƒ“ƒO‚Åƒ{ƒ‹ƒgƒJƒo[o—Í‚Ö
            } else if (( s_Text2 == "‚k‚w|‚X‚O") && ( P_HEADS_DATA[37].ToIntDef(0)) == 394 ){
                // LX-90D-TNUP‚Ìê‡A•Ê‚Ìƒ^ƒCƒ~ƒ“ƒO‚Åƒ{ƒ‹ƒgƒJƒo[o—Í‚Ö
            } else if (P_HEADS_DATA[36].Pos("Z") == 0) {
                // ZƒtƒŒ[ƒ€‚Å–³‚¢ê‡A‚±‚Ìƒ^ƒCƒ~ƒ“ƒO‚Åƒ{ƒ‹ƒgƒJƒo[o—Í‚Ö
                BLT_Cover_Out();
            }
            // *************************
        }
    }

    // 2017.06.23 LX-90D-NUP’Ç‰Á
    s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
    // *************************

    // 2014/08/22 ‰—p‹Zp•ÒW
    if(P_FRAMECD[1] == "75" &&
       P_HEADS_DATA[36].Pos("Z") > 0) {
        // ƒR[ƒh‚ª"75"(BNWRN-,’÷•tBNW) ‚©‚Â ZƒtƒŒ[ƒ€‚Ìê‡Aƒ{ƒ‹ƒgƒJƒo[o—Í‚Ö
        BLT_Cover_Out();
        BLT_Cover_Out_Small();
    // 2017.06.23 LX-90D-NUP’Ç‰Á
    } else if ( P_FRAMECD[1] == "75" &&
                s_Text2 == "‚k‚w|‚X‚O‚c" &&
                P_HEADS_DATA[37].ToIntDef(0) == 94 ){
        // LX-90D-KNUP‚Ìê‡Aƒ{ƒ‹ƒgƒJƒo[o—Í‚Ö
        BLT_Cover_Out();
    } else if ( P_FRAMECD[1] == "75" &&
                s_Text2 == "‚k‚w|‚X‚O" &&
                P_HEADS_DATA[37].ToIntDef(0) == 394 ){
        // LX-90D-TNUP‚Ìê‡Aƒ{ƒ‹ƒgƒJƒo[o—Í‚Ö
        BLT_Cover_Out();
    // *************************
    }

    fclose(fp);

    return true;
}

// ---------------------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F •”•iŒ^®@æ“¾
// ŠT  —v      F
// ˆø  ”      F Code  :ÌÚ°Ñ\¬•”•iº°ÄŞÃ°ÌŞÙ(FRAMECD.csv)“àº°ÄŞ
// –ß‚è’l      F ˆ—Œ‹‰Ê æ“¾‚µ‚½•”•iŒ^®
// ”õ  l      F
// ---------------------------------------------------------------------------------------
AnsiString __fastcall TFrameDetails::Get_BuhinKatasiki(AnsiString Code) {
    AnsiString s_BKata;
    AnsiString s_JPKbn;
    int i_Pos;
    // 2019.11.22 CX-20Œ^’Ç‰Á_S
     AnsiString s_Model;
    // 2019.11.22 CX-20Œ^’Ç‰Á_E

    // 2003.09.16 •”•iŒ^®•ÏX
    // if (Code == "19" || Code == "20" || Code == "21"){
    // if ( P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),1) == "R" ) {
    // s_BKata = G_KS_Syurui + "|" + G_KS_Model  + "|" +  HanToZen(P_HEADS_DATA[36].SubString(1,P_HEADS_DATA[36].Length()-1));
    // } else {
    // s_BKata = G_KS_Syurui + "|" + G_KS_Model  + "|" +  HanToZen(P_HEADS_DATA[36].TrimRight());
    // }

    // } else if (Code == "14" || Code == "15" || Code == "16"){
    // i_Pos = P_HEADS_DATA[36].Pos("H");
    // if (i_Pos == 0){
    // i_Pos = P_HEADS_DATA[36].Pos("U");
    // if (i_Pos == 0){
    // i_Pos = P_HEADS_DATA[36].Pos("S");
    // }
    // }

    // if (i_Pos != 0){
    // s_JPKbn = P_HEADS_DATA[36].SubString(1, i_Pos-1)
    // + P_HEADS_DATA[36].SubString(i_Pos+1, P_HEADS_DATA[36].Length() );
    // } else {
    // s_JPKbn = P_HEADS_DATA[36].TrimRight();
    // }

    // s_BKata = G_KS_Syurui + "|" + G_KS_Model  + "|" +  HanToZen(s_JPKbn);

    // } else if (Code == "18" || Code == "23" || Code == "24" || Code == "25" || Code == "26" || Code == "27" || Code == "28"){
    // s_BKata = P_FRAMECD[4];

    // } else {
    // s_BKata = G_KS_Syurui + "|" + G_KS_Model;
    // }

    // 2019.11.22 CX-20Œ^’Ç‰Á_S
    //if (Code == "09" || Code == "14" || Code == "15" || Code == "16" ||
    //    Code == "17" || Code == "19" || Code == "20" || Code == "21") {
    //    if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
    //        s_BKata = G_KS_Syurui + "|" + G_KS_Model + "|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
    //    1) == "M") {
    //        s_BKata = G_KS_Syurui + "|" + G_KS_Model + "|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
    //        2) == "MR") {
    //        s_BKata = G_KS_Syurui + "|" + G_KS_Model + "|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 2));
    //    }
    //    else {
    //        s_BKata = G_KS_Syurui + "|" + G_KS_Model + "|" +
    //            HanToZen(P_HEADS_DATA[36].TrimRight());
    //    }
    //}
    //// 2014/08/22 ‰—p‹Zp•ÒW
    //else if (Code == "18" || Code == "23" || Code == "24" || Code == "25" ||
    //         Code == "26" || Code == "27" || Code == "28" || Code == "75") {
    //    s_BKata = P_FRAMECD[4];
    //}
    //else {
    //    s_BKata = G_KS_Syurui + "|" + G_KS_Model;
    //}
    //// ***********************
    s_Model = G_KS_Model;

    if (G_KS_Syurui == "‚b‚w" && G_KS_Model == "‚Q‚P") {
        s_Model ="‚Q‚O";
    }
    if (G_KS_Syurui == "‚b‚w" && G_KS_Model == "‚Q‚R") {
        s_Model ="‚Q‚O";
    }

    if (Code == "09" || Code == "14" || Code == "15" || Code == "16" ||
        Code == "17" || Code == "19" || Code == "20" || Code == "21") {
        if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
            s_BKata = G_KS_Syurui + "|" + s_Model + "|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
        1) == "M") {
            s_BKata = G_KS_Syurui + "|" + s_Model + "|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
            2) == "MR") {
            s_BKata = G_KS_Syurui + "|" + s_Model + "|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 2));
        }
        else {
            s_BKata = G_KS_Syurui + "|" + s_Model + "|" +
                HanToZen(P_HEADS_DATA[36].TrimRight());
        }
    }
    else if (Code == "18" || Code == "23" || Code == "24" || Code == "25" ||
             Code == "26" || Code == "27" || Code == "28" || Code == "75") {
        s_BKata = P_FRAMECD[4];
    }
    else {
        s_BKata = G_KS_Syurui + "|" + s_Model;
    }
    // 2019.11.22 CX-20Œ^’Ç‰Á_E

    return s_BKata;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)ŒŸõ  (ÌÚ°Ñ•”•iÃŞ°Àì¬—p)
// ŠT  —v      F
// ˆø  ”      F Key FŒŸõ·°
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:ÃŞ°À—L‚è false:ÃŞ°À–³‚µ]
// ”õ  l      F
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Search_HD_FRM_NZ_Buhin(AnsiString Key) {
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E

    AnsiString s_Frame;
    AnsiString s_Parts_Code;
    AnsiString s_Text;
    bool b_Retry;

    int i_Length;

    // ÌÚ°Ñ‚Ì‰E’[•¶šÁª¯¸
    s_Frame = P_HEADS_DATA[36].TrimRight();

    i_Length = s_Frame.Length();
    s_Frame = s_Frame.SubString(i_Length, 1);
    if (s_Frame == "R") {
        s_Frame = P_HEADS_DATA[36].TrimRight();
        s_Frame = s_Frame.SetLength(i_Length - 1);
    }
    else {
        s_Frame = P_HEADS_DATA[36].TrimRight();
    }

    // ŒŸõ—p•”•iº°ÄŞæ“¾
    if (P_FRAMECD[1].ToIntDef(0) != 9) {
        s_Parts_Code = AnsiString(P_FRAMECD[1].ToIntDef(0));
    }
    else {
        // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
        // 2011.06.20 É½ŞÙ‹KŠi•ÏX
        // s_Text = P_HEADS_DATA[953].SubString(1,1);
        // switch(s_Text.ToIntDef(0)) {
        // case 4: case 5: case 8: case 9:
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        switch (s_Text.ToIntDef(0)) {
        case 4:
        case 5:
        case 8:
        case 9:
        case 11:
        case 12:
        case 13:
        case 21:
        case 22:
        case 23:
            // ***********************
            s_Parts_Code = "109";
            break;
            // 2013/03/27 É½ŞÙ‹KŠi’Ç‰Á
        case 14:
        case 24:
        case 31:
        case 32:
        case 33:
        case 34:
        case 44:
        case 48:
            s_Parts_Code = "109";
            break;
            // ***********************
        default:
            s_Parts_Code = "9";
            break;
        }
    }

    // ƒƒO
    G_Log_Text = "ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À‚ğŒŸõ·° ¢" + Key + "£ ¢" + s_Frame + "£ ¢" +
        P_FRAMECD[3] + "£ ¢" + s_Parts_Code + "£ ‚ÅŒŸõ‚µ‚Ü‚·B";
    Write_Log(G_Log_Text);

    P_SQL = "";
    P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
    P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'";
    P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'";
    P_SQL = P_SQL + "    AND PARTS_TYPE  = '" + P_FRAMECD[3] + "'";

    // 2006.12.21 ANSI‚Ìê‡‚Å‚à•ªŠò‚³‚¹‚é
    // if ( s_Parts_Code == "9" ) {
    if ((s_Parts_Code == "9") || (s_Parts_Code == "109")) {
        // ***********************************
        b_Retry = true;
        // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
        // 2011.06.20 É½ŞÙ‹KŠi•ÏX
        // s_Text = P_HEADS_DATA[953].SubString(1,1);
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        // ***********************
        if ((P_HEADS_DATA[951].ToIntDef(0) == 0) && (P_HEADS_DATA[966].ToIntDef
            (0) == 0) && // E‚ª–³‚¢ê‡
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
                // 2013/03/27 É½ŞÙ‹KŠi’Ç‰Á
            case 44:
            case 48: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
            case 11:
            case 12:
            case 13:
            case 14: // GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
            case 21:
            case 22:
            case 23:
            case 24: // DIN
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
            case 31:
            case 32:
            case 33:
            case 34: // EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 9";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ã(E1/E2‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
                // 2013/03/27 É½ŞÙ‹KŠi’Ç‰Á
            case 44:
            case 48: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
            case 11:
            case 12:
            case 13:
            case 14: // GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
            case 21:
            case 22:
            case 23:
            case 24: // DIN
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
            case 31:
            case 32:
            case 33:
            case 34: // EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 209";
                break;
            }
        }
        else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ‰º(E3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
                // 2013/03/27 É½ŞÙ‹KŠi’Ç‰Á
            case 44:
            case 48: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
            case 11:
            case 12:
            case 13:
            case 14: // GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
            case 21:
            case 22:
            case 23:
            case 24: // DIN
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
            case 31:
            case 32:
            case 33:
            case 34: // EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 409";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0))
            && // ã‰º(E1/E2‚Ì‚Ç‚¿‚ç‚©‚ÆAE3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
                // 2013/03/27 É½ŞÙ‹KŠi’Ç‰Á
            case 44:
            case 48: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
            case 11:
            case 12:
            case 13:
            case 14: // GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
            case 21:
            case 22:
            case 23:
            case 24: // DIN
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
            case 31:
            case 32:
            case 33:
            case 34: // EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 609";
                break;
            }
        }
    }
    else {
        b_Retry = false;
        P_SQL = P_SQL + "    AND PARTS_CODE = " + s_Parts_Code;
    }

    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(P_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        if (b_Retry == true) {
            G_ErrLog_Text =
                "ŒŸõ·°w" + Key +
                "x‚ÍAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚É‘¶İ‚µ‚È‚¢‚Ì‚ÅAE–³ƒ^ƒCƒv‚ÅÄŒŸõ‚µ‚Ü‚·B";
            Write_Error_Log(G_ErrLog_Text);

            P_SQL = "";
            P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
            P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'";
            P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'";
            P_SQL = P_SQL + "    AND PARTS_TYPE  = '" + P_FRAMECD[3] + "'";
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9:
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
            default:
                P_SQL = P_SQL + "    AND PARTS_CODE = 9";
                break;
            }
            // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
            //wkQuery->DatabaseName = ODBC_DSN_DNO;
            wkQuery->ConnectionName = ODBC_DSN_DNO;
            // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E
            wkQuery->SQL->Clear();
            wkQuery->SQL->Add(P_SQL);
            wkQuery->Open();
            wkQuery->First();
        }
    }
    if (wkQuery->Eof) {
        // ´×°Û¸Ş
        G_ErrLog_Text = "ŒŸõ·° ¢" + Key + "£ ¢" + s_Frame + "£ ¢" +
            P_FRAMECD[3] + "£ ¢" + s_Parts_Code +
            "£ ‚ÍAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚É‘¶İ‚µ‚Ü‚¹‚ñB";

        Write_Error_Log(G_ErrLog_Text);
        Write_Log(G_ErrLog_Text);

        // }”Ô
        P_DNO[1] = "";
        P_DNO[2] = "";
        P_DNO[3] = "";
        P_DNO[4] = "";
        P_DNO[5] = "";
        // ì}—p}”Ô
        P_WKDNO[1] = "";
        P_WKDNO[2] = "";
        P_WKDNO[3] = "";
        P_WKDNO[4] = "";
        P_WKDNO[5] = "";
        // Ş¿–¼Ì
        P_MatNM[1] = "";
        P_MatNM[2] = "";
        P_MatNM[3] = "";
        P_MatNM[4] = "";
        P_MatNM[5] = "";
        // Ş¿ƒR[ƒh
        P_MatCD[1] = "";
        P_MatCD[2] = "";
        P_MatCD[3] = "";
        P_MatCD[4] = "";
        P_MatCD[5] = "";
        // –‡”
        P_PLATE_NUM[1] = 0;
        P_PLATE_NUM[2] = 0;
        P_PLATE_NUM[3] = 0;
        P_PLATE_NUM[4] = 0;
        P_PLATE_NUM[5] = 0;
        // –‡”
        P_PLATE_NUM_SUS[1] = 0;
        P_PLATE_NUM_SUS[2] = 0;
        P_PLATE_NUM_SUS[3] = 0;
        P_PLATE_NUM_SUS[4] = 0;
        P_PLATE_NUM_SUS[5] = 0;
        // SUSg—pƒtƒ‰ƒO
        P_SUS = 0;

        delete wkQuery;
        return false;
    }
    else {
        // Ş¿–¼Ì
        P_MatNM[1] = wkQuery->FieldByName("MAT_NAME1")->AsString.TrimRight();
        P_MatNM[2] = wkQuery->FieldByName("MAT_NAME2")->AsString.TrimRight();
        P_MatNM[3] = wkQuery->FieldByName("MAT_NAME3")->AsString.TrimRight();
        P_MatNM[4] = wkQuery->FieldByName("MAT_NAME4")->AsString.TrimRight();
        P_MatNM[5] = wkQuery->FieldByName("MAT_NAME5")->AsString.TrimRight();
        P_MatCD[1] = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
        P_MatCD[2] = wkQuery->FieldByName("MAT_CODE2")->AsString.TrimRight();
        P_MatCD[3] = wkQuery->FieldByName("MAT_CODE3")->AsString.TrimRight();
        P_MatCD[4] = wkQuery->FieldByName("MAT_CODE4")->AsString.TrimRight();
        P_MatCD[5] = wkQuery->FieldByName("MAT_CODE5")->AsString.TrimRight();
        // }”Ô
        P_DNO[1] = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_DNO[2] = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        P_DNO[3] = wkQuery->FieldByName("DNO3")->AsString.TrimRight();
        P_DNO[4] = wkQuery->FieldByName("DNO4")->AsString.TrimRight();
        P_DNO[5] = wkQuery->FieldByName("DNO5")->AsString.TrimRight();
        // ì}—p}”Ô
        P_WKDNO[1] = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_WKDNO[2] = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        P_WKDNO[3] = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        P_WKDNO[4] = wkQuery->FieldByName("WKDNO4")->AsString.TrimRight();
        P_WKDNO[5] = wkQuery->FieldByName("WKDNO5")->AsString.TrimRight();
        // –‡”
        P_PLATE_NUM[1] = wkQuery->FieldByName("PLATE_NUM1")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[2] = wkQuery->FieldByName("PLATE_NUM2")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[3] = wkQuery->FieldByName("PLATE_NUM3")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[4] = wkQuery->FieldByName("PLATE_NUM4")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[5] = wkQuery->FieldByName("PLATE_NUM5")
            ->AsString.ToIntDef(0);
        // –‡”(SUS—p)
        P_PLATE_NUM_SUS[1] = wkQuery->FieldByName("HEIGHT1")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[2] = wkQuery->FieldByName("HEIGHT2")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[3] = wkQuery->FieldByName("HEIGHT3")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[4] = wkQuery->FieldByName("HEIGHT4")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[5] = wkQuery->FieldByName("HEIGHT5")
            ->AsString.ToIntDef(0);
        // SUSg—pƒtƒ‰ƒO
        P_SUS = wkQuery->FieldByName("LENGTH")->AsString.ToIntDef(0);

        G_Log_Text = "Ş¿–¼Ì    w" + P_ZaisituNM + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "}”Ô‚P      w" + P_DNO[1] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "}”Ô‚Q      w" + P_DNO[2] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "}”Ô‚R      w" + P_DNO[3] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "}”Ô‚S      w" + P_DNO[4] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);

        G_Log_Text = "ì}—p}”Ô‚Pw" + P_WKDNO[1] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "ì}—p}”Ô‚Qw" + P_WKDNO[2] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "ì}—p}”Ô‚Rw" + P_WKDNO[3] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "ì}—p}”Ô‚Sw" + P_WKDNO[4] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);

        G_Log_Text = "–‡”‚P      w" + AnsiString(P_PLATE_NUM[1]) + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "–‡”‚Q      w" + AnsiString(P_PLATE_NUM[2]) + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "–‡”‚R      w" + AnsiString(P_PLATE_NUM[3]) + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F EÌÚ°ÑÃŞ°Àì¬ˆ— Às
// ŠT  —v      F
// ˆø  ”      F ‚È‚µ
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:³í false:ˆÙí]
// ”õ  l      F ‚È‚µ
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::E_Frame_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    // 2012.01.20 ’†‘Ş¿
    AnsiString s_ChinaM;
    // *******************

    // **********************************************
    // ***  ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)@ŒŸõ
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "ŒŸõ·°w" + P_Key + "x‚ÅAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚ğŒŸõB";
    Write_Log(G_Log_Text);

    // seek_ret¥¥¥Ï½À‚É‘¶İ‚µ‚½‚©‚µ‚È‚¢‚©
    // Write_SE_Frame_Data()‚Åg—p‚·‚éB
    seek_ret = Search_HD_FRM_NZ(P_Key, "E");

    G_Log_Text = "ŒŸõ·°w" + P_Key + "x‚ÅAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚ğŒŸõI—¹B";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  Ş¿–¼Ì‚ÌŒŸõ(ASME‚ªŠÜ‚Ü‚ê‚Ä‚¢‚é‚©)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "Ş¿w" + P_ZaisituNM + "x‚ªAASME‚Å‚·B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("‚`‚r‚l‚d") != 0) {
            G_ErrLog_Text = "Ş¿w" + P_ZaisituNM + "x‚ªA‚`‚r‚l‚d‚Å‚·B";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  E–¾d—l‚Ìì¬
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("E");
    G_Log_Text = "E–¾d—l  w" + P_Siyou1 + "x‚ğæ“¾B";
    Write_Log(G_Log_Text);

    // *************************
    // ***  Ş¿‚Ìæ“¾
    // *************************
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E

    // **********************************************
    // ***  }”ÔEì}—p}”Ô‚Ìæ“¾  02.09.03’Ç‰Á
    // **********************************************
    bSUS = false;
    // 2003.11.06@ƒvƒŒ[ƒgŞ¿ƒf[ƒ^ˆÊ’u•ÏX
    // sPltMat = P_HEADS_DATA[160].TrimRight();
    sPltMat = P_HEADS_DATA[43].TrimRight();
    // **************************************
    // 2003.12.05 ASME PLATE
    // if ((P_SUS != 0 ) && (sPltMat != "1014" ) && ( sPltMat != "1015" ) &&
    // ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠOiTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]j
    // 2016.11.07 Ş¿º°ÄŞ•ÏX
    //if ((P_SUS != 0) && (sPltMat != "1014") && (sPltMat != "1015") &&
    //    (sPltMat != "1032") && (sPltMat != "1033") && (sPltMat != "1057") &&
    //    (sPltMat != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2’Ç‰Á
    //    (sPltMat != "1075") && (sPltMat != "1089") && (sPltMat != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11’Ç‰Á
    //    (sPltMat != "1076") &&
    //    // ***************************
    //    (sPltMat != "1085") && (sPltMat != "1095")) {
    //    // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠOiTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]j
    //    // *********************
    //    bSUS = true;
    //}
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠO
        bSUS = true;
    }
    // ***********************
    // 2003.11.05 –‡”‹æ•ªŠÖŒW®ƒ‚ğƒ‚É•ÏX
    if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <= P_PLATE_NUM[1]))
        || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[1]))) {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "1";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[2])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[2]))) {
        P_Zuban = P_DNO[2];
        P_SZuban = P_WKDNO[2];
        if (P_MatNM[2] != "") {
            P_ZaisituNM = P_MatNM[2];
            P_EZaisituNM = P_MatNM[2];
        }
        else if (P_MatCD[2] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[2]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "2";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[3])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[3]))) {
        P_Zuban = P_DNO[3];
        P_SZuban = P_WKDNO[3];
        if (P_MatNM[3] != "") {
            P_ZaisituNM = P_MatNM[3];
            P_EZaisituNM = P_MatNM[3];
        }
        else if (P_MatCD[3] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[4])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[4]))) {
        P_Zuban = P_DNO[4];
        P_SZuban = P_WKDNO[4];
        if (P_MatNM[4] != "") {
            P_ZaisituNM = P_MatNM[4];
            P_EZaisituNM = P_MatNM[4];
        }
        else if (P_MatCD[4] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[4]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[4]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "4";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[5])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[5]))) {
        P_Zuban = P_DNO[5];
        P_SZuban = P_WKDNO[5];
        if (P_MatNM[5] != "") {
            P_ZaisituNM = P_MatNM[5];
            P_EZaisituNM = P_MatNM[5];
        }
        else if (P_MatCD[5] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[5]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[5]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "5";
    }
    else {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "1";
    }
    // ***************************************

    switch (P_HEADS_DATA[37].ToIntDef(0)) { // ƒtƒŒ[ƒ€ƒR[ƒh
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // ‚aƒVƒŠ[ƒY
        // s_Text = P_HEADS_DATA[34].TrimRight();
        // if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX"){
        // P_ZaisituNM = "‚r‚t‚r‚R‚O‚S";
        // P_EZaisituNM = "‚r‚t‚r‚R‚O‚S";
        //
        // } else {
        // P_ZaisituNM = "SS400/SUS304¶ÊŞ°Øİ¸Ş";
        // P_EZaisituNM = "SS400/SUS304COVERING";
        // }

        break;
    // 2022.04.28 UX-30L’Ç‰Á_S
    // 2023.03.09 CX-00-Y’Ç‰Á_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y’Ç‰Á_E
    case 39:
    case 40:
    case 43:
    case 49:
    case 50:
    case 52:
    case 53:
    case 54:
    case 339:
    case 340:
    case 343:
    case 349:
    case 350:
    case 352:
    case 353:
    case 354: // ‚xƒVƒŠ[ƒY
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "‚r‚Q‚V‚T‚i‚q";
            P_EZaisituNM = "‚r‚Q‚V‚T‚i‚q";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    // 2022.04.28 UX-30L’Ç‰Á_E
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        // 2016.11.07 Ş¿º°ÄŞ•ÏX
        //if (s_Text == "0000" || s_Text == "1040") {
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
        // ***********************
            P_ZaisituNM = "‚r‚r‚S‚O‚O";
            P_EZaisituNM = "‚r‚r‚S‚O‚O";
        }
        else {
            // 2011.05.30 ÌÚ°ÑŞ¿w’è•ÏX
            // P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            // P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
            // ***************************
        }

        break;
    }

    // 2012.01.20 ’†‘Ş¿
    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "|" + G_KS_Model;
    if (s_Text == "‚t‚w|‚P‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚O‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚P‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚R‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚T‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚Q‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚S‚P") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚S‚R") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚P‚P") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚P‚R") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚R‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚T‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚V‚O") { // ’†‘Ş¿
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // ’†‘Ş¿
        // 2012.02.07 ’†‘ÌÚ°ÑŞ¿
        // if ( P_ZaisituNM == "‚r‚r‚S‚O‚O" ) {
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        // 2016.11.07 Ş¿º°ÄŞ•ÏX
        //if (s_Text == "1040") {
        if (Type_MAT(s_Text) == "SS400") {
        // ***********************
        }
        else if (P_ZaisituNM == "‚r‚r‚S‚O‚O" || P_ZaisituNM == "‚bD‚rD") {
            // ***********************
            P_ZaisituNM = "‚p‚Q‚R‚T‚a";
            P_EZaisituNM = "‚p‚Q‚R‚T‚a";
        }
    }
    // *******************

    // 2008.08.21 WX-50ÌÚ°Ñ}”Ôİ’èğŒ’Ç‰Á
    s_Text = G_KS_Syurui + "|" + G_KS_Model;
    if (s_Text == "‚v‚w|‚T‚R") {
        // WHC=0,REVE=0 ‚a‘¤—nÚ
        if ((P_HEADS_DATA[243].ToIntDef(0) == 0) && (P_HEADS_DATA[137].ToIntDef
            (0) == 0)) {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            if (P_MatNM[2] != "") {
                P_ZaisituNM = P_MatNM[2];
                P_EZaisituNM = P_MatNM[2];
            }
            else if (P_MatCD[2] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[2]);
            }
            else if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "2";
            // WHC=0,REVE=1 ‚`‘¤—nÚ
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) != 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "1";
            // WHC=1,REVE=0 ‚`‘¤—nÚ
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) != 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "1";
            // WHC=1,REVE=1 ‚a‘¤—nÚ
        }
        else {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            if (P_MatNM[2] != "") {
                P_ZaisituNM = P_MatNM[2];
                P_EZaisituNM = P_MatNM[2];
            }
            else if (P_MatCD[2] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[2]);
            }
            else if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "2";
        }
    }
    // ************************************

    // *************************
    // ***  —\”õ”‚Ìæ“¾
    // *************************
    Syukei_Frame_Yobisu("E");

    // *************************
    // ***  EÌÚ°ÑÃŞ°À‚Ìo—Í
    // *************************
    Write_SE_Frame_Data("E", seek_ret);

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F SÌÚ°ÑÃŞ°Àì¬ˆ— Às
// ŠT  —v      F
// ˆø  ”      F ‚È‚µ
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:³í false:ˆÙí]
// ”õ  l      F ‚È‚µ
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::S_Frame_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    // 2012.01.20 ’†‘Ş¿
    AnsiString s_ChinaM;
    // *******************

    // **********************************************
    // ***  ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)@ŒŸõ
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "ŒŸõ·°w" + P_Key + "x‚ÅAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚ğŒŸõB";
    Write_Log(G_Log_Text);

    // seek_ret¥¥¥Ï½À‚É‘¶İ‚µ‚½‚©‚µ‚È‚¢‚©
    // ¦’ Write_SE_Frame_Data()‚Åg—p‚·‚éB
    seek_ret = Search_HD_FRM_NZ(P_Key, "S");

    G_Log_Text = "ŒŸõ·°w" + P_Key + "x‚ÅAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚ğŒŸõI—¹B";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  Ş¿–¼Ì‚ÌŒŸõ(ASME‚ªŠÜ‚Ü‚ê‚Ä‚¢‚é‚©)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "Ş¿w" + P_ZaisituNM + "x‚ªAASME‚Å‚·B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("‚`‚r‚l‚d") != 0) {
            G_ErrLog_Text = "Ş¿w" + P_ZaisituNM + "x‚ªA‚`‚r‚l‚d‚Å‚·B";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  E–¾d—l‚Ìì¬
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("S");
    G_Log_Text = "E–¾d—l  w" + P_Siyou1 + "x‚ğæ“¾B";
    Write_Log(G_Log_Text);

    // *************************
    // ***  Ş¿‚Ìæ“¾
    // *************************
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E

    switch (P_HEADS_DATA[37].ToIntDef(0)) {
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // ‚aƒVƒŠ[ƒY
        // s_Text = P_HEADS_DATA[34].TrimRight();
        // if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX"){
        // P_ZaisituNM = "‚r‚t‚r‚R‚O‚S";
        // P_EZaisituNM = "‚r‚t‚r‚R‚O‚S";
        //
        // } else {
        // P_ZaisituNM = "SS400/SUS304¶ÊŞ°Øİ¸Ş";
        // P_EZaisituNM = "SS400/SUS304COVERING";
        // }
        if (P_ZaisituNM == "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        break;
    // 2022.04.28 UX-30L’Ç‰Á_S
    // 2023.03.09 CX-00-Y’Ç‰Á_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y’Ç‰Á_E
    case 39:
    case 40:
    case 43:
    case 49:
    case 50:
    case 52:
    case 53:
    case 54:
    case 339:
    case 340:
    case 343:
    case 349:
    case 350:
    case 352:
    case 353:
    case 354: // ‚xƒVƒŠ[ƒY
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "‚r‚Q‚V‚T‚i‚q";
            P_EZaisituNM = "‚r‚Q‚V‚T‚i‚q";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    // 2022.04.28 UX-30L’Ç‰Á_E
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        // 2016.11.07 Ş¿º°ÄŞ•ÏX
        //if (s_Text == "0000" || s_Text == "1040") {
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
        // ***********************
            P_ZaisituNM = "‚r‚r‚S‚O‚O";
            P_EZaisituNM = "‚r‚r‚S‚O‚O";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }

        break;
    }

    // 2012.01.20 ’†‘Ş¿
    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "|" + G_KS_Model;
    if (s_Text == "‚t‚w|‚P‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚O‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚P‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚R‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚T‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚Q‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚S‚P") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚S‚R") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚P‚P") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚P‚R") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚R‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚T‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚V‚O") { // ’†‘Ş¿
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // ’†‘Ş¿
        // 2012.02.07 ’†‘ÌÚ°ÑŞ¿
        // if ( P_ZaisituNM == "‚r‚r‚S‚O‚O" ) {
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        // 2016.11.07 Ş¿º°ÄŞ•ÏX
        //if (s_Text == "1040") {
        if (Type_MAT(s_Text) == "SS400") {
        // ***********************
        }
        else if (P_ZaisituNM == "‚r‚r‚S‚O‚O" || P_ZaisituNM == "‚bD‚rD") {
            // ***********************
            P_ZaisituNM = "‚p‚Q‚R‚T‚a";
            P_EZaisituNM = "‚p‚Q‚R‚T‚a";
        }
    }
    // *******************

    // **********************************************
    // ***  }”ÔEì}—p}”Ô‚Ìæ“¾  02.09.03’Ç‰Á
    // **********************************************
    bSUS = false;
    // 2003.11.06@ƒvƒŒ[ƒgŞ¿ƒf[ƒ^ˆÊ’u•ÏX
    // sPltMat = P_HEADS_DATA[160].TrimRight();
    sPltMat = P_HEADS_DATA[43].TrimRight();
    // **************************************
    // 2003.12.05 ASME PLATE
    // if ((P_SUS != 0 ) && (sPltMat != "1014" ) && ( sPltMat != "1015" ) &&
    // ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠOiTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]j
    // 2016.11.07 Ş¿º°ÄŞ•ÏX
    //if ((P_SUS != 0) && (sPltMat != "1014") && (sPltMat != "1015") &&
    //    (sPltMat != "1032") && (sPltMat != "1033") && (sPltMat != "1057") &&
    //    (sPltMat != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2’Ç‰Á
    //    (sPltMat != "1075") && (sPltMat != "1089") && (sPltMat != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11’Ç‰Á
    //    (sPltMat != "1076") &&
    //    // ***************************
    //    (sPltMat != "1085") && (sPltMat != "1095")) {
    //    // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠOiTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]j
    //    // *********************
    //    bSUS = true;
    //}
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠO
        bSUS = true;
    }
    // ***********************
    // 2003.11.05 –‡”‹æ•ªŠÖŒW®ƒ‚ğƒ‚É•ÏX
    if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <= P_PLATE_NUM[1]))
        || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[1]))) {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        P_Siyou3 = "1";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[2])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[2]))) {
        P_Zuban = P_DNO[2];
        P_SZuban = P_WKDNO[2];
        P_Siyou3 = "2";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[3])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[3]))) {
        P_Zuban = P_DNO[3];
        P_SZuban = P_WKDNO[3];
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[4])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[4]))) {
        P_Zuban = P_DNO[4];
        P_SZuban = P_WKDNO[4];
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[5])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[5]))) {
        P_Zuban = P_DNO[5];
        P_SZuban = P_WKDNO[5];
        P_Siyou3 = "3";
    }
    else {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        P_Siyou3 = "1";
    }
    // ***************************************

    // 2008.08.21 WX-50ÌÚ°Ñ}”Ôİ’èğŒ’Ç‰Á
    s_Text = G_KS_Syurui + "|" + G_KS_Model;
    if (s_Text == "‚v‚w|‚T‚R") {
        // WHC=0,REVE=0 ‚a‘¤—nÚ
        if ((P_HEADS_DATA[243].ToIntDef(0) == 0) && (P_HEADS_DATA[137].ToIntDef
            (0) == 0)) {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            P_Siyou3 = "2";
            // WHC=0,REVE=1 ‚`‘¤—nÚ
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) != 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            P_Siyou3 = "1";
            // WHC=1,REVE=0 ‚`‘¤—nÚ
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) != 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            P_Siyou3 = "1";
            // WHC=1,REVE=1 ‚a‘¤—nÚ
        }
        else {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            P_Siyou3 = "2";
        }
    }
    // ************************************

    // *************************
    // ***  —\”õ”‚Ìæ“¾
    // *************************
    Syukei_Frame_Yobisu("S");

    // *************************
    // ***  SÌÚ°ÑÃŞ°À‚Ìo—Í
    // *************************
    Write_SE_Frame_Data("S", seek_ret);

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F S,EÌÚ°ÑÃŞ°À@‘‚«‚İ
// ŠT  —v      F
// ˆø  ”      F Key FŒŸõ·°
// F Kbn FS¥¥¥SÌÚ°Ñ—pˆ—CE¥¥¥EÌÚ°Ñˆ—
// F Seek_Result:ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½ÀŒŸõŒ‹‰Ê(true:‘¶İ,false:‘¶İ‚µ‚È‚¢)
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:ÃŞ°À—L‚è false:ÃŞ°À–³‚µ]
// ”õ  l      F
// ---------------------------------------------------------------------------------------
bool __fastcall TFrameDetails::Write_SE_Frame_Data(AnsiString Kbn,
    bool Seek_Result) {
    AnsiString s_Text;
    AnsiString wk_FilePass;
    // 2019.11.22 CX-20Œ^’Ç‰Á_S
    AnsiString s_Model;
    // 2019.11.22 CX-20Œ^’Ç‰Á_E

    long l_Daisu;

    // ------------------------------------------
    // 2007.10.03 Ğ×°d—l‘Î‰ d—lÀ²Ìßº°ÄŞ
    AnsiString sSpecCode;
    // ****************************
    // d—lÀ²Ìßº°ÄŞæ“¾(""/"A"/"B")
    // ****************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // ------------------------------------------

    // ****************************
    // ***  •Ï”ƒZƒbƒg
    // ****************************
    // •”•i•ª—Ş
    P_BBunrui = "FRAME";
    // ¯•Êq
    if (Kbn == "S") {
        P_Sikibetu = "FRAM-S";
    }
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
    else if (Kbn == "SG") {
        P_Sikibetu = "FRAMSG";
    }
    else if (Kbn == "EG") {
        P_Sikibetu = "FRAMEG";
    }
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E
    else {
        P_Sikibetu = "FRAM-E";
    }
    // •”•iŒ^®
    // 2019.11.22 CX-20Œ^’Ç‰Á_S
    //if (Seek_Result == true) {
    //    // 2003.09.10 •”•iŒ^®•ÏX
    //    // if( P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) ==  "R" ) {
    //    // P_BKatasiki = G_KS_Syurui + "|" + G_KS_Model + "|" + HanToZen(P_HEADS_DATA[36].SubString(1,P_HEADS_DATA[36].Length()-1));
    //    // } else {
    //    // P_BKatasiki = G_KS_Syurui + "|" + G_KS_Model + "|" + HanToZen(P_HEADS_DATA[36].TrimRight());
    //    // }
    //    if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
    //        P_BKatasiki = G_KS_Syurui + "|" + G_KS_Model + "|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
    //    1) == "M") {
    //        P_BKatasiki = G_KS_Syurui + "|" + G_KS_Model + "|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
    //        2) == "MR") {
    //        P_BKatasiki = G_KS_Syurui + "|" + G_KS_Model + "|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 2));
    //    }
    //    else {
    //        P_BKatasiki = G_KS_Syurui + "|" + G_KS_Model + "|" +
    //            HanToZen(P_HEADS_DATA[36].TrimRight());
    //    }
    //    // ***********************
    //}
    //else {
    //    P_BKatasiki = "";
    //}
    s_Model = G_KS_Model;

    if (G_KS_Syurui == "‚b‚w" && G_KS_Model == "‚Q‚P") {
        s_Model ="‚Q‚O";
    }
    if (G_KS_Syurui == "‚b‚w" && G_KS_Model == "‚Q‚R") {
        s_Model ="‚Q‚O";
    }

    if (Seek_Result == true) {
        if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
            P_BKatasiki = G_KS_Syurui + "|" + s_Model + "|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
        1) == "M") {
            P_BKatasiki = G_KS_Syurui + "|" + s_Model + "|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
            2) == "MR") {
            P_BKatasiki = G_KS_Syurui + "|" + s_Model + "|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 2));
        }
        else {
            P_BKatasiki = G_KS_Syurui + "|" + s_Model + "|" +
                HanToZen(P_HEADS_DATA[36].TrimRight());
        }
    }
    else {
        P_BKatasiki = "";
    }
    // 2019.11.22 CX-20Œ^’Ç‰Á_E

    // •”•i–¼Ì
    if (Kbn == "S") {
        P_BMeisyou = "‚rDƒtƒŒ[ƒ€";
    }
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
    else if (Kbn == "SG") {
        P_BMeisyou = "‚rDƒtƒŒ[ƒ€iƒKƒX—n’fj";
    }
    else if (Kbn == "EG") {
        P_BMeisyou = "‚dDƒtƒŒ[ƒ€iƒKƒX—n’fj";
    }
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E
    else {
        P_BMeisyou = "‚dDƒtƒŒ[ƒ€";
    }
    // ‚P‘ä•ª”—Ê
    if (Seek_Result == true) {
        P_1DaiSuryo = 1;
    }
    else {
        P_1DaiSuryo = 0;
    }

    // ‘Œv
    l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // ‘ä”
    P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;

    // d—l‚P
    if (Seek_Result == true) {
        P_Siyou1 = "E–¾" + P_Siyou1;
    }
    else {
        P_Siyou1 = "";
    }

    // d—l‚Q(S,EÌÚ°Ñí—Ş)
    if (Seek_Result == true) {
        if (Kbn == "S") {
            P_Siyou2 = P_HEADS_DATA[1296].TrimRight(); // SÌÚ°Ñí—Ş
        }
        else {
            P_Siyou2 = P_HEADS_DATA[1297].TrimRight(); // EÌÚ°Ñí—Ş
        }
    }
    else {
        P_Siyou2 = "";
    }

    // d—l‚S
    P_Siyou4 = "";
    // d—l‚T
    P_Siyou5 = "";

    // ****************************
    // ***  S,EÌÚ°ÑÃŞ°À  ƒZƒbƒg
    // ****************************
    // •”•i“WŠJÃŞ°À@OPEN
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

    // 1  •”•i•ª—Ş
    s_Text = P_BBunrui;
    fputs(s_Text.c_str(), fp);
    // 2  ¯•Êq
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), fp);
    // 3  •”•iŒ^®
    s_Text = "," + P_BKatasiki;
    fputs(s_Text.c_str(), fp);
    // 4  •”•i–¼Ì
    s_Text = "," + P_BMeisyou;
    fputs(s_Text.c_str(), fp);
    // 5  Ş¿–¼Ì
    s_Text = "," + P_ZaisituNM;
    fputs(s_Text.c_str(), fp);
    // 6  }”Ô
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), fp);
    // 7  ì}—p}”Ô
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), fp);
    // 8  1‘ä•ª”—Ê
    s_Text = "," + AnsiString(P_1DaiSuryo);
    fputs(s_Text.c_str(), fp);
    // 9  —\”õ”
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), fp);

    // 10 ‘Œv(1‘ä•ª”—Ê~HEADSÃŞ°À“à‘ä”+—\”õ”)
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), fp);

    // 11 d—l‚P
    s_Text = "," + P_Siyou1;
    fputs(s_Text.c_str(), fp);
    // 12 d—l‚Q
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), fp);
    // 13 d—l‚R
    s_Text = "," + P_Siyou3;
    fputs(s_Text.c_str(), fp);
    // 14 d—l‚S
    s_Text = "," + P_Siyou4;
    fputs(s_Text.c_str(), fp);
    // 15 d—l‚T
    s_Text = "," + P_Siyou5;
    fputs(s_Text.c_str(), fp);
    // 16  Ş¿–¼Ì(‰p)
    s_Text = "," + P_EZaisituNM;
    fputs(s_Text.c_str(), fp);

    // ------------------------------
    // 2007.10.03 Ğ×°d—l‘Î‰
    // 17 d—lÀ²Ìßº°ÄŞ
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), fp);
    // 2007.10.03
    // ------------------------------

    // ‰üs•¶š
    s_Text = "\n";
    // CSVÃŞ°À@Ì§²Ùo—Í
    fputs(s_Text.c_str(), fp);

    fclose(fp);

    return true;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)@ŒŸõ
// ŠT  —v      F
// ˆø  ”      F Key FŒŸõ·°
// F Kbn FS¥¥¥SÌÚ°Ñ—pˆ—CE¥¥¥EÌÚ°Ñˆ—
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:ÃŞ°À—L‚è false:ÃŞ°À–³‚µ]
// ”õ  l      F
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Search_HD_FRM_NZ(AnsiString Key, AnsiString Kbn)
{
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E

    AnsiString s_Frame;
    AnsiString s_Text;
    // 2011.06.20 É½ŞÙ‹KŠi•ÏX
    AnsiString s_Text2;
    // ***********************

    int i_Length;
    bool E1Hol, E2Hole, E3Hole, E4Hole;

    // ÌÚ°Ñ‚Ì‰E’[•¶šÁª¯¸
    s_Frame = P_HEADS_DATA[36].TrimRight();

    i_Length = s_Frame.Length();
    s_Frame = s_Frame.SubString(i_Length, 1);
    if (s_Frame == "R") {
        s_Frame = P_HEADS_DATA[36].TrimRight();
        s_Frame = s_Frame.SetLength(i_Length - 1);
    }
    else {
        s_Frame = P_HEADS_DATA[36].TrimRight();
    }

    P_SQL = "";
    P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
    P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ÌßÚ°ÄŒ^®
    P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ÌÚ°ÑŒ^®

    if (Kbn == "S") {
        P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

        // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
        // 2011.06.20 É½ŞÙ‹KŠi•ÏX
        // s_Text = P_HEADS_DATA[893].SubString(1,1);
        s_Text = P_HEADS_DATA[893].SubString(1, 2);
        // 2009.02.16 É½ŞÙŒpèğŒ•ÏX
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            // s_Text = P_HEADS_DATA[908].SubString(1,1);
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            // s_Text = P_HEADS_DATA[923].SubString(1,1);
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            // s_Text = P_HEADS_DATA[938].SubString(1,1);
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }

        switch (s_Text.ToIntDef(0)) {
            // case 4: case 5: case 8: case 9:
        case 4:
        case 5:
        case 8:
        case 9:
        case 11:
        case 12:
        case 13:
        case 21:
        case 22:
        case 23: // ANSI/JPI/DIN/GB
            P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            // 2013/03/27 É½ŞÙ‹KŠi’Ç‰Á
        case 14:
        case 24:
        case 31:
        case 32:
        case 33:
        case 34:
        case 44:
        case 48: // ANSI/JPI/DIN/GB/EN
            P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            // ***********************
        default:
            P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
        }
        // ***************************

    }
    else if (Kbn == "E") {
        P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

        // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
        // 2011.06.20 É½ŞÙ‹KŠi•ÏX
        // s_Text = P_HEADS_DATA[953].SubString(1,1);
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        // 2009.02.16 É½ŞÙŒpèğŒ•ÏX
        // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            // s_Text = P_HEADS_DATA[908].SubString(1,1);
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            // s_Text = P_HEADS_DATA[923].SubString(1,1);
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            // s_Text = P_HEADS_DATA[938].SubString(1,1);
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }
        // ***************************

        if ((P_HEADS_DATA[951].ToIntDef(0) == 0) && (P_HEADS_DATA[966].ToIntDef
            (0) == 0) && // E‚ª–³‚¢ê‡
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 É½ŞÙ‹KŠi•ÏX
                // case 4: case 5: case 8: case 9:
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 108";
                break;
                // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 108";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 8";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ã(E1/E2‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 É½ŞÙ‹KŠi•ÏX
                // case 4: case 5: case 8: case 9:                     // ANSI/JPI
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 308";
                break;
                // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 308";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 208";
                break;
            }
        }
        else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ‰º(E3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 É½ŞÙ‹KŠi•ÏX
                // case 4: case 5: case 8: case 9:                     // ANSI/JPI
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 508";
                break;
                // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 508";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 408";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0))
            && // ã‰º(E1/E2‚Ì‚Ç‚¿‚ç‚©‚ÆAE3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 É½ŞÙ‹KŠi•ÏX
                // case 4: case 5: case 8: case 9:                     // ANSI/JPI
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 708";
                break;
                // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 708";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 608";
                break;
            }
        }
        // switch(s_Text.ToIntDef(0)) {
        // case 4: case 5: case 8: case 9:
        // P_SQL = P_SQL + "    AND PARTS_CODE = 108";
        // break;
        // default:
        // P_SQL = P_SQL + "    AND PARTS_CODE = 8";
        // break;
        // }
    }
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
    else if (Kbn == "SG") {
        P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

        // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
        s_Text = P_HEADS_DATA[893].SubString(1, 2);
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }

        switch (s_Text.ToIntDef(0)) {
        case 4:
        case 5:
        case 8:
        case 9:
        case 11:
        case 12:
        case 13:
        case 21:
        case 22:
        case 23: // ANSI/JPI/DIN/GB
            P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
        case 14:
        case 24:
        case 31:
        case 32:
        case 33:
        case 34:
        case 44:
        case 48: // ANSI/JPI/DIN/GB/EN
            P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
        default:
            P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
        }

    }
    else if (Kbn == "EG") {
        P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

        // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }

        if ((P_HEADS_DATA[951].ToIntDef(0) == 0) && (P_HEADS_DATA[966].ToIntDef
            (0) == 0) && // E‚ª–³‚¢ê‡
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 188";
                break;
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 188";
                break;
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 88";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ã(E1/E2‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 388";
                break;
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 388";
                break;
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 288";
                break;
            }
        }
        else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ‰º(E3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 588";
                break;
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 588";
                break;
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 488";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0))
            && // ã‰º(E1/E2‚Ì‚Ç‚¿‚ç‚©‚ÆAE3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 788";
                break;
                // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 788";
                break;
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 688";
                break;
            }
        }
    }
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E

    // 2009.02.16 RX-50‚Ìê‡DIN,#150‚ğJIS‚Æ‚µ‚Äˆµ‚¤
    // 2011.06.20 É½ŞÙ‹KŠi•ÏX
    // s_Text = G_KS_Syurui + "|" + G_KS_Model;
    // if (s_Text == "‚q‚w|‚T‚O") {
    s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
    if (s_Text2 == "‚q‚w|‚T‚O") {
        // ***********************

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ÌßÚ°ÄŒ^®
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ÌÚ°ÑŒ^®

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            // ***********************

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
                // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
            // 2021.09.22 RX-50Z’Ç‰Á_S
            //case 13:
            // 2021.09.22 RX-50Z’Ç‰Á_E
            case 14:
            // 2021.09.22 RX-50Z’Ç‰Á_S
            //case 23:
            // 2021.09.22 RX-50Z’Ç‰Á_E
            case 24:
            // 2021.09.22 RX-50Z’Ç‰Á_S
            //case 33:
            // 2021.09.22 RX-50Z’Ç‰Á_E
            case 34:
            case 41:
            case 44:
            case 48:
                // 2021.09.22 RX-50Z’Ç‰Á_S
                //// JIS30K,PN25/40,#400(ÌÚ°Ñ‚È‚µ)
                // JIS30K,PN40,#400(ÌÚ°Ñ‚È‚µ)
                // 2021.09.22 RX-50Z’Ç‰Á_E
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
                // ***********************
            // 2021.09,22 RX-50Z’Ç‰Á_S
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // ZÌÚ°Ñ PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ÌÚ°ÑÉ½ŞÙí•Ê
                } else {
                    // ZÌÚ°ÑˆÈŠO PN25(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                }
                break;
            // 2021.09.22 RX-50Z’Ç‰Á_E
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            }

        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = P_HEADS_DATA[953].SubString(1,1);
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            // ***********************

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
                // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
            // 2021.09.22 RX-50Z’Ç‰Á_S
            //case 13:
            // 2021.09.22 RX-50Z’Ç‰Á_E
            case 14:
            // 2021.09.22 RX-50Z’Ç‰Á_S
            //case 23:
            // 2021.09.22 RX-50Z’Ç‰Á_E
            case 24:
            // 2021.09.22 RX-50Z’Ç‰Á_S
            //case 33:
            // 2021.09.22 RX-50Z’Ç‰Á_E
            case 34:
            case 41:
            case 44:
            case 48:
                // 2021.09.22 RX-50Z’Ç‰Á_S
                //// JIS30K,PN25/40,#400(ÌÚ°Ñ‚È‚µ)
                // JIS30K,PN40,#400(ÌÚ°Ñ‚È‚µ)
                // 2021.09.22 RX-50Z’Ç‰Á_E
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
                // ***********************
            // 2021.09.22 RX-50Z’Ç‰Á_S
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // ZÌÚ°Ñ PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ÌÚ°ÑÉ½ŞÙí•Ê
                } else {
                    // ZÌÚ°ÑˆÈŠO PN25(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                }
                break;
            // 2021.09.22 RX-50Z’Ç‰Á_E
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            }
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
                // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
            case 14:
            case 24:
            case 34:
            case 41:
            case 44:
            case 48:
                // JIS30K,PN40,#400(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // ZÌÚ°Ñ PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ÌÚ°ÑÉ½ŞÙí•Ê
                } else {
                    // ZÌÚ°ÑˆÈŠO PN25(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                }
                break;
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            }

        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 14:
            case 24:
            case 34:
            case 41:
            case 44:
            case 48:
                // JIS30K,PN40,#400(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // ZÌÚ°Ñ PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ÌÚ°ÑÉ½ŞÙí•Ê
                } else {
                    // ZÌÚ°ÑˆÈŠO PN25(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                }
                break;
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            }
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E
    }
    // *********************************************

    // 2010.08.27 LX-10-S‚Ìê‡JIS16K,20K‚ÌÌÚ°ÑÉ½ŞÙí•Ê‚ğ’Ç‰Á‚·‚é
    // 2011.06.20 É½ŞÙ‹KŠi•ÏX
    // s_Text = G_KS_Syurui + "|" + G_KS_Model;
    // if (s_Text == "‚k‚w|‚P‚O") {
    s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
    if (s_Text2 == "‚k‚w|‚P‚O") {
        // ***********************

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ÌßÚ°ÄŒ^®
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ÌÚ°ÑŒ^®

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
                // case 2: case 3:                                    // JIS16K,20K
            case 2:
            case 3:
            case 12:
            case 22: // JIS16K,20K/GB1.6/DN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 201"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
                // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
                ////case 4: case 5: case 8: case 9:                    // ANSI/JPI/DIN
                // case 4: case 5: case 8: case 9:                    // ANSI/JPI
            case 4:
            case 8: // #150
                P_SQL = P_SQL + "    AND PARTS_CODE = 201"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 32: // EN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 201"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34:
                // PN25/40(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
                // ***********************
                // default:                                           // JIS10K
            default: // JIS10K/GB1.0/DN10
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            }
            // ***********************

        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = P_HEADS_DATA[953].SubString(1,1);
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // E‚ª–³‚¢ê‡
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 108";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 8";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ã(E1/E2‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 308";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 308"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 308"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 208";
                    break;
                }
            }
            else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ‰º(E3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 508";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 508"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 508"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 408";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0))
                && // ã‰º(E1/E2‚Ì‚Ç‚¿‚ç‚©‚ÆAE3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 É½ŞÙ‹KŠi’Ç‰Á
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 708";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 708"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 708"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 608";
                    break;
                }
            }
            // ***********************
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2:
            case 3:
            case 12:
            case 22: // JIS16K,20K/GB1.6/DN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 281"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 4:
            case 8: // #150
                P_SQL = P_SQL + "    AND PARTS_CODE = 281"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 32: // EN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 281"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34:
                // PN25/40(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            default: // JIS10K/GB1.0/DN10
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            }

        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // E‚ª–³‚¢ê‡
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 88";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ã(E1/E2‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 388"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 388"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 288";
                    break;
                }
            }
            else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ‰º(E3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 588"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 588"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 488";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0))
                && // ã‰º(E1/E2‚Ì‚Ç‚¿‚ç‚©‚ÆAE3/E4‚Ì‚Ç‚¿‚ç‚©)‚ÉE‚ª—L‚éê‡
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 788"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 788"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ÌÚ°Ñ‚È‚µ)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 688";
                    break;
                }
            }
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E
    }
    // **********************************************************

    // 2018.06.19 LX-30Z’Ç‰Á_S
    // JIS10K
    //   ‚rƒtƒŒ[ƒ€F"1"
    //   ‚dƒtƒŒ[ƒ€F"8"
    //   ‚dƒ[ƒ‰[F"9"@@@
    // ANSI#150/JPI#150/DIN/GB/DIN
    //   ‚rƒtƒŒ[ƒ€F"1"
    //   ‚dƒtƒŒ[ƒ€F"8"
    //   ‚dƒ[ƒ‰[F"109"@@@
    // JIS16K
    //   ‚rƒtƒŒ[ƒ€F"101"@@@
    //   ‚dƒtƒŒ[ƒ€F"108"
    //   ‚dƒ[ƒ‰[F"109"@
    // JIS20K,30K/ANSI#300,400/JPI#300,400(ÌÚ°Ñ‚È‚µ)@
    //   ‚rƒtƒŒ[ƒ€F"999"@@@
    //   ‚dƒtƒŒ[ƒ€F"999"
    //   ‚dƒ[ƒ‰[F"9"or"109"@
    s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
    if ((s_Text2 == "‚k‚w|‚R‚O") && ( P_HEADS_DATA[36].Pos("Z") > 0)) {
        // ‚k‚w|‚R‚O|‚y

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ÌßÚ°ÄŒ^®
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ÌÚ°ÑŒ^®

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 4:
            case 8:
            case 11:
            case 12:
            case 13:
            case 14:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34: // ANSI#150/JPI#150/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            }
        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 4:
            case 8:
            case 11:
            case 12:
            case 13:
            case 14:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34: // ANSI#150/JPI#150/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            }
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 4:
            case 8:
            case 11:
            case 12:
            case 13:
            case 14:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34: // ANSI#150/JPI#150/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            }
        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 4:
            case 8:
            case 11:
            case 12:
            case 13:
            case 14:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34: // ANSI#150/JPI#150/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(ÌÚ°Ñ‚È‚µ)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            }
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E
    }
    // 2018.06.19 LX-30Z’Ç‰Á_E

    // 2019.11.08 CX-23’Ç‰Á_S
    // JIS10K,16K,20K/DIN10,16/GB10,16/EN10,16
    //   ‚rƒtƒŒ[ƒ€F"1"
    //   ‚dƒtƒŒ[ƒ€F"8"
    //   ‚dƒ[ƒ‰[F"9"@@@
    // ANSI#150/JPI#150
    //   ‚rƒtƒŒ[ƒ€F"101"
    //   ‚dƒtƒŒ[ƒ€F"108"
    //   ‚dƒ[ƒ‰[F"109"@@@
    // JIS30K/ANSI#300,400/JPI#300,400/DIN25,40/GB25,40/EN25,40 (ÌÚ°Ñ‚È‚µ)@
    //   ‚rƒtƒŒ[ƒ€F"999"@@@
    //   ‚dƒtƒŒ[ƒ€F"999"
    //   ‚dƒ[ƒ‰[F"9"or"109"@
    s_Text2 = G_KS_Syurui + "|" + G_KS_Model;
    // 2022.04.27 CX-21’Ç‰Á_S
    //if (s_Text2 == "‚b‚w|‚Q‚R") {
    //    // ‚b‚w|‚Q‚R
    if ((s_Text2 == "‚b‚w|‚Q‚P") || (s_Text2 == "‚b‚w|‚Q‚R")) {
        // ‚b‚w|‚Q‚PA‚b‚w|‚Q‚R
    // 2022.04.27 CX-21’Ç‰Á_E

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ÌßÚ°ÄŒ^®
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ÌÚ°ÑŒ^®

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 41:
            case 5:
            case 44:
            case 9:
            case 48:
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40@
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            }
        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 41:
            case 5:
            case 44:
            case 9:
            case 48:
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40@
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            }
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò(ÃŞ¨Ì«ÙÄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 41:
            case 5:
            case 44:
            case 9:
            case 48:
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40@
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            }
        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // Œpèí—Ş‚ÍEÌÚ°Ñ‚àSÉ½ŞÙ‚Å”»’f‚·‚é
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4É½ŞÙŒpèæ“ªˆêŒ…‚Å•ªŠò
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            case 41:
            case 5:
            case 44:
            case 9:
            case 48:
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40@
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ÌÚ°ÑÉ½ŞÙí•Ê
            break;
            }
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E
    }
    // 2019.11.08 CX-23’Ç‰Á_E

    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
    G_Log_Text = "ŒŸõ·° w" + P_SQL + "x‚ÅŒŸõ‚µ‚Ü‚·B";
    Write_Log(G_Log_Text);
    // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E

    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(P_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        if (Kbn == "E") { // EƒtƒŒ[ƒ€‚Ìê‡‚ÍAE–¾ğŒ‚ÅŠY“–‚·‚éƒŒƒR[ƒh‚ª–³‚¢ê‡AE–³‚µ‚ÅÄŒŸõ‚·‚é
            G_ErrLog_Text =
                "ŒŸõ·°w" + Key +
                "x‚ÍAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚É‘¶İ‚µ‚È‚¢‚Ì‚ÅAE–³ƒ^ƒCƒv‚ÅÄŒŸõ‚µ‚Ü‚·B";
            Write_Error_Log(G_ErrLog_Text);

            P_SQL = "";
            P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
            P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ÌßÚ°ÄŒ^®
            P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ÌÚ°ÑŒ^®
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 É½ŞÙ‹KŠi•ÏX
                // case 4: case 5: case 8: case 9:
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 108";
                break;
                // 2013/03/27 É½ŞÙ‹KŠi’Ç‰Á
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ÌÚ°ÑÉ½ŞÙí•Ê
                break;
                // ***********************
            default:
                P_SQL = P_SQL + "    AND PARTS_CODE = 8";
                break;
            }
            // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
            //wkQuery->DatabaseName = ODBC_DSN_DNO;
            wkQuery->ConnectionName = ODBC_DSN_DNO;
            // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E
            wkQuery->SQL->Clear();
            wkQuery->SQL->Add(P_SQL);
            wkQuery->Open();
            wkQuery->First();
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_S
        else if (Kbn == "SG" || Kbn == "EG") {
            // SEƒtƒŒ[ƒ€ƒKƒXØ‚è}”Ô‚È‚µ‚Ìê‡‚ÍASEƒtƒŒ[ƒ€‚Æ“¯‚¶}”Ôi“WŠJ‚µ‚È‚¢j
            // ´×°Û¸Ş
            G_ErrLog_Text =
                "ŒŸõ·°w" + Key + "x‚ÍAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚É‘¶İ‚µ‚Ü‚¹‚ñB";
            Write_Error_Log(G_ErrLog_Text);

            // Ş¿–¼Ì
            P_MatNM[1] = "";
            P_MatNM[2] = "";
            P_MatNM[3] = "";
            P_MatNM[4] = "";
            P_MatNM[5] = "";
            // }”Ô
            P_DNO[1] = "NONE";
            P_DNO[2] = "";
            P_DNO[3] = "";
            P_DNO[4] = "";
            P_DNO[5] = "";
            // ì}—p}”Ô
            P_WKDNO[1] = "";
            P_WKDNO[2] = "";
            P_WKDNO[3] = "";
            P_WKDNO[4] = "";
            P_WKDNO[5] = "";
            // –‡”
            P_PLATE_NUM[1] = 0;
            P_PLATE_NUM[2] = 0;
            P_PLATE_NUM[3] = 0;
            P_PLATE_NUM[4] = 0;
            P_PLATE_NUM[5] = 0;
            // –‡”(SUS—p)
            P_PLATE_NUM_SUS[1] = 0;
            P_PLATE_NUM_SUS[2] = 0;
            P_PLATE_NUM_SUS[3] = 0;
            P_PLATE_NUM_SUS[4] = 0;
            P_PLATE_NUM_SUS[5] = 0;
            // SUSg—pƒtƒ‰ƒO
            P_SUS = 0;

            delete wkQuery;
            return false;
        }
        // 2022.05.23 ƒtƒŒ[ƒ€ƒKƒXØ‚è’Ç‰Á_E
        else {
            // ´×°Û¸Ş
            G_ErrLog_Text =
                "ŒŸõ·°w" + Key + "x‚ÍAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚É‘¶İ‚µ‚Ü‚¹‚ñB";
            Write_Error_Log(G_ErrLog_Text);

            // Ş¿–¼Ì
            P_MatNM[1] = "";
            P_MatNM[2] = "";
            P_MatNM[3] = "";
            P_MatNM[4] = "";
            P_MatNM[5] = "";
            // }”Ô
            P_DNO[1] = "";
            P_DNO[2] = "";
            P_DNO[3] = "";
            P_DNO[4] = "";
            P_DNO[5] = "";
            // ì}—p}”Ô
            P_WKDNO[1] = "";
            P_WKDNO[2] = "";
            P_WKDNO[3] = "";
            P_WKDNO[4] = "";
            P_WKDNO[5] = "";
            // –‡”
            P_PLATE_NUM[1] = 0;
            P_PLATE_NUM[2] = 0;
            P_PLATE_NUM[3] = 0;
            P_PLATE_NUM[4] = 0;
            P_PLATE_NUM[5] = 0;
            // –‡”(SUS—p)
            P_PLATE_NUM_SUS[1] = 0;
            P_PLATE_NUM_SUS[2] = 0;
            P_PLATE_NUM_SUS[3] = 0;
            P_PLATE_NUM_SUS[4] = 0;
            P_PLATE_NUM_SUS[5] = 0;
            // SUSg—pƒtƒ‰ƒO
            P_SUS = 0;

            delete wkQuery;
            return false;
        }
    }
    if (!wkQuery->Eof) {
        // Ş¿–¼Ì
        P_ZaisituNM = wkQuery->FieldByName("MAT_NAME1")->AsString.TrimRight();
        P_EZaisituNM = P_ZaisituNM;
        // Ş¿–¼Ì
        P_MatNM[1] = wkQuery->FieldByName("MAT_NAME1")->AsString.TrimRight();
        P_MatNM[2] = wkQuery->FieldByName("MAT_NAME2")->AsString.TrimRight();
        P_MatNM[3] = wkQuery->FieldByName("MAT_NAME3")->AsString.TrimRight();
        P_MatNM[4] = wkQuery->FieldByName("MAT_NAME4")->AsString.TrimRight();
        P_MatNM[5] = wkQuery->FieldByName("MAT_NAME5")->AsString.TrimRight();
        P_MatCD[1] = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
        P_MatCD[2] = wkQuery->FieldByName("MAT_CODE2")->AsString.TrimRight();
        P_MatCD[3] = wkQuery->FieldByName("MAT_CODE3")->AsString.TrimRight();
        P_MatCD[4] = wkQuery->FieldByName("MAT_CODE4")->AsString.TrimRight();
        P_MatCD[5] = wkQuery->FieldByName("MAT_CODE5")->AsString.TrimRight();
        // }”Ô
        P_DNO[1] = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_DNO[2] = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        P_DNO[3] = wkQuery->FieldByName("DNO3")->AsString.TrimRight();
        P_DNO[4] = wkQuery->FieldByName("DNO4")->AsString.TrimRight();
        P_DNO[5] = wkQuery->FieldByName("DNO5")->AsString.TrimRight();
        // ì}—p}”Ô
        P_WKDNO[1] = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_WKDNO[2] = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        P_WKDNO[3] = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        P_WKDNO[4] = wkQuery->FieldByName("WKDNO4")->AsString.TrimRight();
        P_WKDNO[5] = wkQuery->FieldByName("WKDNO5")->AsString.TrimRight();
        // –‡”
        P_PLATE_NUM[1] = wkQuery->FieldByName("PLATE_NUM1")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[2] = wkQuery->FieldByName("PLATE_NUM2")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[3] = wkQuery->FieldByName("PLATE_NUM3")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[4] = wkQuery->FieldByName("PLATE_NUM4")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[5] = wkQuery->FieldByName("PLATE_NUM5")
            ->AsString.ToIntDef(0);
        // –‡”(SUS—p)
        P_PLATE_NUM_SUS[1] = wkQuery->FieldByName("HEIGHT1")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[2] = wkQuery->FieldByName("HEIGHT2")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[3] = wkQuery->FieldByName("HEIGHT3")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[4] = wkQuery->FieldByName("HEIGHT4")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[5] = wkQuery->FieldByName("HEIGHT5")
            ->AsString.ToIntDef(0);
        // SUSg—pƒtƒ‰ƒO
        P_SUS = wkQuery->FieldByName("LENGTH")->AsString.ToIntDef(0);

        G_Log_Text = "Ş¿–¼Ì    w" + P_ZaisituNM + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "}”Ô‚P      w" + P_DNO[1] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "}”Ô‚Q      w" + P_DNO[2] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "}”Ô‚R      w" + P_DNO[3] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "}”Ô‚S      w" + P_DNO[4] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);

        G_Log_Text = "ì}—p}”Ô‚Pw" + P_WKDNO[1] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "ì}—p}”Ô‚Qw" + P_WKDNO[2] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "ì}—p}”Ô‚Rw" + P_WKDNO[3] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "ì}—p}”Ô‚Sw" + P_WKDNO[4] + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);

        G_Log_Text = "–‡”‚P      w" + AnsiString(P_PLATE_NUM[1]) + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "–‡”‚Q      w" + AnsiString(P_PLATE_NUM[2]) + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
        G_Log_Text = "–‡”‚R      w" + AnsiString(P_PLATE_NUM[3]) + "x‚ğæ“¾B";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F E–¾d—l æ“¾
// ŠT  —v      F
// ˆø  ”      F Kbn FS¥¥¥SÌÚ°Ñ—pˆ—CE¥¥¥EÌÚ°Ñˆ—
// –ß‚è’l      F ˆ—Œ‹‰Ê
// ”õ  l      F
// ---------------------------------------------------------------------------
AnsiString __fastcall TFrameDetails::Get_AnaakeSiyou(AnsiString Kbn) {
    AnsiString s_Text;

    AnsiString s_S1Nzl_Syurui; // S1É½ŞÙí—Ş
    AnsiString s_S1Nzl_Tugite; // S1É½ŞÙŒpè
    AnsiString s_S2Nzl_Syurui; // S2É½ŞÙí—Ş
    AnsiString s_S2Nzl_Tugite; // S2É½ŞÙŒpè
    AnsiString s_S3Nzl_Syurui; // S3É½ŞÙí—Ş
    AnsiString s_S3Nzl_Tugite; // S3É½ŞÙŒpè
    AnsiString s_S4Nzl_Syurui; // S4É½ŞÙí—Ş
    AnsiString s_S4Nzl_Tugite; // S4É½ŞÙŒpè

    AnsiString s_E1Nzl_Syurui; // E1É½ŞÙí—Ş
    AnsiString s_E1Nzl_Tugite; // E1É½ŞÙŒpè
    AnsiString s_E2Nzl_Syurui; // E2É½ŞÙí—Ş
    AnsiString s_E2Nzl_Tugite; // E2É½ŞÙŒpè
    AnsiString s_E3Nzl_Syurui; // E3É½ŞÙí—Ş
    AnsiString s_E3Nzl_Tugite; // E3É½ŞÙŒpè
    AnsiString s_E4Nzl_Syurui; // E4É½ŞÙí—Ş
    AnsiString s_E4Nzl_Tugite; // E4É½ŞÙŒpè

    AnsiString s_AnaSiyou; // E–¾d—l
    AnsiString s_S1;
    AnsiString s_S2;
    AnsiString s_S3;
    AnsiString s_S4;
    AnsiString s_S[5];
    AnsiString s_E[5];

    int i_OgataFlg; // 0:¬Œ^’†Œ^,1:‘åŒ^

    // *************************
    // ***  HEADSÃŞ°À@¾¯Ä
    // *************************
    if (Kbn == "S") {
        s_S1Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1É½ŞÙí—Ş
        s_S1Nzl_Tugite = P_HEADS_DATA[893].TrimRight(); // S1É½ŞÙŒpè
        s_S2Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2É½ŞÙí—Ş
        s_S2Nzl_Tugite = P_HEADS_DATA[908].TrimRight(); // S2É½ŞÙŒpè
        s_S3Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3É½ŞÙí—Ş
        s_S3Nzl_Tugite = P_HEADS_DATA[923].TrimRight(); // S3É½ŞÙŒpè
        s_S4Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4É½ŞÙí—Ş
        s_S4Nzl_Tugite = P_HEADS_DATA[938].TrimRight(); // S4É½ŞÙŒpè
    }
    else {
        s_E1Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1É½ŞÙí—Ş
        s_E1Nzl_Tugite = P_HEADS_DATA[953].TrimRight(); // E1É½ŞÙŒpè
        s_E2Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2É½ŞÙí—Ş
        s_E2Nzl_Tugite = P_HEADS_DATA[968].TrimRight(); // E2É½ŞÙŒpè
        s_E3Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3É½ŞÙí—Ş
        s_E3Nzl_Tugite = P_HEADS_DATA[983].TrimRight(); // E3É½ŞÙŒpè
        s_E4Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4É½ŞÙí—Ş
        s_E4Nzl_Tugite = P_HEADS_DATA[998].TrimRight(); // E4É½ŞÙŒpè
    }

    // 2003.09.09 ‘åŒ^ƒtƒŒ[ƒ€E–¾
    s_Text = G_KS_Syurui + "|" + G_KS_Model;
    // 2003.09.12 ‚dƒtƒŒ[ƒ€E–¾
    // if (s_Text == "‚t‚w|‚W‚O" || s_Text == "‚t‚w|‚X‚O" ||
    // s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚V‚O" ||
    // s_Text == "‚r‚w|‚X‚O" || s_Text == "‚q‚w|‚V‚O"){
    // i_OgataFlg = 1;
    // } else {
    // i_OgataFlg = 0;
    // }
    // 2009.06.19 ‘åŒ^ÌÚ°ÑğŒ®•ÏX(UX-100R’Ç‰Á)
    ////2008.08.01 ‘åŒ^ÌÚ°ÑğŒ®•ÏX(CX-10,CX-10D’Ç‰Á)
    ////////2006.02.06 ‘åŒ^ÌÚ°ÑğŒ®•ÏX(RX-90,SX-90M,L’Ç‰Á)
    //////if (s_Text == "‚t‚w|‚W‚O" || s_Text == "‚t‚w|‚X‚O" ||
    //////    s_Text == "‚t‚w|‚P‚O‚O" || s_Text == "‚r‚w|‚V‚O" ||
    //////    s_Text == "‚r‚w|‚X‚O" || s_Text == "‚q‚w|‚V‚O"){
    ////if (s_Text == "‚t‚w|‚W‚O" || s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" ||
    ////    s_Text == "‚r‚w|‚V‚O" || s_Text == "‚r‚w|‚X‚O" || s_Text == "‚r‚w|‚X‚O‚l" ||
    ////    s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O"){
    //////*************************************************
    // if (s_Text == "‚t‚w|‚W‚O" || s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" ||
    // s_Text == "‚t‚w|‚P‚R‚O" || s_Text == "‚r‚w|‚V‚O" || s_Text == "‚r‚w|‚X‚O" ||
    // s_Text == "‚r‚w|‚X‚O‚l" || s_Text == "‚r‚w|‚X‚O‚k" || s_Text == "‚q‚w|‚V‚O" ||
    // s_Text == "‚q‚w|‚X‚O" || s_Text == "‚q‚w|‚P‚O‚O" || s_Text == "‚v‚w|‚X‚O" ||
    // s_Text == "‚b‚w|‚P‚O" || s_Text == "‚b‚w|‚P‚O‚c" || s_Text == "‚m‚w|‚X‚O"){
    ////***********************************************
    if (s_Text == "‚t‚w|‚W‚O" || s_Text == "‚t‚w|‚X‚O" || s_Text == "‚t‚w|‚P‚O‚O" ||
        s_Text == "‚t‚w|‚P‚O‚O‚q" || s_Text == "‚t‚w|‚P‚R‚O" || s_Text == "‚r‚w|‚V‚O" ||
        s_Text == "‚r‚w|‚X‚O" || s_Text == "‚r‚w|‚X‚O‚l" || s_Text == "‚r‚w|‚X‚O‚k" ||
        s_Text == "‚q‚w|‚V‚O" || s_Text == "‚q‚w|‚X‚O" || s_Text == "‚q‚w|‚P‚O‚O" ||
        s_Text == "‚v‚w|‚X‚O" || s_Text == "‚b‚w|‚P‚O" || s_Text == "‚b‚w|‚P‚O‚c" ||
        s_Text == "‚m‚w|‚X‚O") {
        // ******************************************
        i_OgataFlg = 1;
        // 2012.08.23 ‘åŒ^ÌÚ°ÑğŒ®•ÏX(SX-80S’Ç‰Á)
    }
    else if (s_Text == "‚r‚w|‚W‚O‚r") {
        i_OgataFlg = 1;
        // *****************************************
    }
    // 2017.06.23 LX-90D,SX-80LM’Ç‰Á
    else if (s_Text == "‚k‚w|‚X‚O‚c" || s_Text == "‚r‚w|‚W‚O‚k" || s_Text == "‚r‚w|‚W‚O‚l" ) {
        i_OgataFlg = 1;
    }
    // *****************************
    // 2019.01.21 LX-50E–¾•ÏX_S
    else if (s_Text == "‚k‚w|‚T‚O" ) {
        i_OgataFlg = 1;
    }
    // 2019.01.21 LX-50E–¾•ÏX_E
    // 2019.01.21 LX-50S’Ç‰Á_S
    else if (s_Text == "‚k‚w|‚T‚O‚r" ) {
        i_OgataFlg = 1;
    }
    // 2019.01.21 LX-50S’Ç‰Á_E
    // 2021.03.11 RX-30•ÏX_S
    else if (s_Text == "‚q‚w|‚R‚O" ) {
        i_OgataFlg = 1;
    }
    // 2021.03.11 RX-30•ÏX_E
    // 2023.08.21 UX-10•ÏX_S
    //else if (s_Text == "‚t‚w|‚P‚O" || s_Text == "‚t‚w|‚Q‚O" || s_Text == "‚t‚w|‚R‚O" ||
    //    s_Text == "‚k‚w|‚Q‚O" || s_Text == "‚r‚w|‚S‚O" || s_Text == "‚r‚w|‚S‚P" ||
    //    s_Text == "‚r‚w|‚S‚R" || s_Text == "‚d‚w|‚P‚T") {
    //    i_OgataFlg = 2;
    //}
    else if (s_Text == "‚t‚w|‚P‚O") {
        i_OgataFlg = 1;
    }
    else if (s_Text == "‚t‚w|‚Q‚O" || s_Text == "‚t‚w|‚R‚O" ||
        s_Text == "‚k‚w|‚Q‚O" || s_Text == "‚r‚w|‚S‚O" || s_Text == "‚r‚w|‚S‚P" ||
        s_Text == "‚r‚w|‚S‚R" || s_Text == "‚d‚w|‚P‚T") {
        i_OgataFlg = 2;
    }
    // 2023.08.21 UX-10•ÏX_E
    else {
        i_OgataFlg = 0;
    }
    // *************************
    // ***************************

    if (Kbn == "S") {
        // *************************
        // ***  S1@•”•ª
        // *************************
        if (s_S1Nzl_Syurui == "0") {
            s_S[1] = "0";
        }
        else {
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = s_S1Nzl_Tugite.SubString(2,1);
            s_Text = s_S1Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[1] = "T";
                // 2003.09.09 ‘åŒ^ƒtƒŒ[ƒ€E–¾
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_S[1] = "K";
            }
            else if (s_Text == "1") {
                if (i_OgataFlg == 1) {
                    s_S[1] = "N";
                }
                else {
                    s_S[1] = "K";
                }
            }
            else if (s_Text == "3") {
                s_S[1] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_S[1] = "1";
            }
            else {
                G_ErrLog_Text = "S1É½ŞÙŒpèw" + s_S1Nzl_Tugite +
                    "x‚É‚æ‚èAS1E–¾ÃŞ°À‚ªˆÙí‚Å‚·B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  S2@•”•ª
        // *************************
        if (s_S2Nzl_Syurui == "0") {
            s_S[2] = "0";
        }
        else {
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = s_S2Nzl_Tugite.SubString(2,1);
            s_Text = s_S2Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[2] = "T";
                // 2003.09.09 ‘åŒ^ƒtƒŒ[ƒ€E–¾
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_S[2] = "K";
            }
            else if (s_Text == "1") {
                if (i_OgataFlg == 1) {
                    s_S[2] = "N";
                }
                else {
                    s_S[2] = "K";
                }
            }
            else if (s_Text == "3") {
                s_S[2] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_S[2] = "2";
            }
            else {
                G_ErrLog_Text = "S2É½ŞÙŒpèw" + s_S2Nzl_Tugite +
                    "x‚É‚æ‚èAS2E–¾ÃŞ°À‚ªˆÙí‚Å‚·B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  S3@•”•ª
        // *************************
        if (s_S3Nzl_Syurui == "0") {
            s_S[3] = "0";
        }
        else {
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = s_S3Nzl_Tugite.SubString(2,1);
            s_Text = s_S3Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[3] = "T";
                // 2003.09.09 ‘åŒ^ƒtƒŒ[ƒ€E–¾
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_S[3] = "K";
            }
            else if (s_Text == "1") {
                if (i_OgataFlg == 1) {
                    s_S[3] = "N";
                }
                else {
                    s_S[3] = "K";
                }
            }
            else if (s_Text == "3") {
                s_S[3] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_S[3] = "3";
            }
            else {
                G_ErrLog_Text = "S3É½ŞÙŒpèw" + s_S3Nzl_Tugite +
                    "x‚É‚æ‚èAS3E–¾ÃŞ°À‚ªˆÙí‚Å‚·B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  S4@•”•ª
        // *************************
        if (s_S4Nzl_Syurui == "0") {
            s_S[4] = "0";
        }
        else {
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = s_S4Nzl_Tugite.SubString(2,1);
            s_Text = s_S4Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[4] = "T";
                // 2003.09.09 ‘åŒ^ƒtƒŒ[ƒ€E–¾
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_S[4] = "K";
            }
            else if (s_Text == "1") {
                if (i_OgataFlg == 1) {
                    s_S[4] = "N";
                }
                else {
                    s_S[4] = "K";
                }
            }
            else if (s_Text == "3") {
                s_S[4] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_S[4] = "4";
            }
            else {
                G_ErrLog_Text = "S4É½ŞÙŒpèw" + s_S4Nzl_Tugite +
                    "x‚É‚æ‚èAS4E–¾ÃŞ°À‚ªˆÙí‚Å‚·B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // Šeæ“¾€–ÚŒ‹‡
        s_AnaSiyou = "S" + s_S[1] + s_S[2] + s_S[3] + s_S[4];

    }
    else {
        // *************************
        // ***  E1@•”•ª
        // *************************
        if (s_E1Nzl_Syurui == "0") {
            s_E[1] = "0";
        }
        else {
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = s_E1Nzl_Tugite.SubString(2,1);
            s_Text = s_E1Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[1] = "T";
                // 2003.09.09 ‘åŒ^ƒtƒŒ[ƒ€E–¾
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[1] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 ‚dƒtƒŒ[ƒ€E–¾
                // if (i_OgataFlg == 1){
                if (i_OgataFlg == 1 || i_OgataFlg == 2) {
                    // *************************
                    s_E[1] = "N";
                }
                else {
                    s_E[1] = "K";
                }
            }
            else if (s_Text == "3") {
                s_E[1] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_E[1] = "1";
            }
            else {
                G_ErrLog_Text = "E1É½ŞÙŒpèw" + s_E1Nzl_Tugite +
                    "x‚É‚æ‚èAE1E–¾ÃŞ°À‚ªˆÙí‚Å‚·B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  E2@•”•ª
        // *************************
        if (s_E2Nzl_Syurui == "0") {
            s_E[2] = "0";
        }
        else {
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = s_E2Nzl_Tugite.SubString(2,1);
            s_Text = s_E2Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[2] = "T";
                // 2003.09.09 ‘åŒ^ƒtƒŒ[ƒ€E–¾
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[2] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 ‚dƒtƒŒ[ƒ€E–¾
                // if (i_OgataFlg == 1){
                if (i_OgataFlg == 1 || i_OgataFlg == 2) {
                    // *************************
                    s_E[2] = "N";
                }
                else {
                    s_E[2] = "K";
                }
            }
            else if (s_Text == "3") {
                s_E[2] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_E[2] = "2";
            }
            else {
                G_ErrLog_Text = "E2É½ŞÙŒpèw" + s_E2Nzl_Tugite +
                    "x‚É‚æ‚èAE2E–¾ÃŞ°À‚ªˆÙí‚Å‚·B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  E3@•”•ª
        // *************************
        if (s_E3Nzl_Syurui == "0") {
            s_E[3] = "0";
        }
        else {
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = s_E3Nzl_Tugite.SubString(2,1);
            s_Text = s_E3Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[3] = "T";
                // 2003.09.09 ‘åŒ^ƒtƒŒ[ƒ€E–¾
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[3] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 ‚dƒtƒŒ[ƒ€E–¾
                // if (i_OgataFlg == 1){
                if (i_OgataFlg == 1 || i_OgataFlg == 2) {
                    // *************************
                    s_E[3] = "N";
                }
                else {
                    s_E[3] = "K";
                }
            }
            else if (s_Text == "3") {
                s_E[3] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_E[3] = "3";
            }
            else {
                G_ErrLog_Text = "E3É½ŞÙŒpèw" + s_E3Nzl_Tugite +
                    "x‚É‚æ‚èAE3E–¾ÃŞ°À‚ªˆÙí‚Å‚·B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  E4@•”•ª
        // *************************
        if (s_E4Nzl_Syurui == "0") {
            s_E[4] = "0";
        }
        else {
            // 2011.06.20 É½ŞÙ‹KŠi•ÏX
            // s_Text = s_E4Nzl_Tugite.SubString(2,1);
            s_Text = s_E4Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[4] = "T";
                // 2003.09.09 ‘åŒ^ƒtƒŒ[ƒ€E–¾
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[4] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 ‚dƒtƒŒ[ƒ€E–¾
                // if (i_OgataFlg == 1){
                if (i_OgataFlg == 1 || i_OgataFlg == 2) {
                    // *************************
                    s_E[4] = "N";
                }
                else {
                    s_E[4] = "K";
                }
            }
            else if (s_Text == "3") {
                s_E[4] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_E[4] = "4";
            }
            else {
                G_ErrLog_Text = "E4É½ŞÙŒpèw" + s_E4Nzl_Tugite +
                    "x‚É‚æ‚èAE4E–¾ÃŞ°À‚ªˆÙí‚Å‚·B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // Šeæ“¾€–ÚŒ‹‡
        s_AnaSiyou = "E" + s_E[1] + s_E[2] + s_E[3] + s_E[4];
    }

    return s_AnaSiyou;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ƒ{ƒ‹ƒgƒJƒo[
// ŠT  —v      F
// ˆø  ”      F AnsiString *P_PARTS FWŒv‘ÎÛÚº°ÄŞ
// –ß‚è’l      F ‚È‚µ
// ”õ  l      F É½ŞÙ•”•iÃŞ°ÀˆÈ~‚Åg—p
// ---------------------------------------------------------------------------
void __fastcall TFrameDetails::BLT_Cover_Out(void) {
    int i_RecNo;
    int i_ErrFlg;
    int i_Value_1;
    int i_Value_2;
    int i_Daisuu;
    AnsiString s_Text;
    // 2016.07.25 ƒ{ƒ‹ƒgƒJƒo[¡–@•ÏX
    int i_Value_L; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    int i_Value_S; //SÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    int i_Value_Z; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    // *******************************

    // -----------------------------------------
    // 2007.10.03 Ğ×°d—l‘Î‰ d—lÀ²Ìßº°ÄŞ
    AnsiString sSpecCode;
    // ***************************
    // d—lÀ²Ìßº°ÄŞæ“¾(""/"A"/"B")
    // ***************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // -----------------------------------------

    // ***************************
    // ***  ƒ{ƒ‹ƒgƒJƒo[
    // ***************************
    // wC018x
    // 2004.06.08 ƒJƒo[o—ÍğŒ•ÏX
    i_RecNo = 1092;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (s_Text == "0") {
        i_RecNo = 1096;
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text == "0") {
            return;
        }
    }
    // i_RecNo = 1092;
    // s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    // if (s_Text != "0"){
    // i_RecNo = 1096;
    // s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    // if (s_Text == "0"){
    // return;
    // }
    // } else {
    // return;
    // }
    // *****************************

    G_Log_Text = "ƒ{ƒ‹ƒgƒJƒo[         w" + s_Text + "x‚ğƒZƒbƒgB";
    Write_Log(G_Log_Text);

    // **********************************************************
    // ***  ÎŞÙÄ–{”L,S@”’lÁª¯¸
    // ***    wC019x,wC020x,wC021x‚Ìˆ×‚Ìˆ—‘OÁª¯¸
    // **********************************************************
    i_ErrFlg = 0;
    s_Text = P_HEADS_DATA[39];
    i_Daisuu = s_Text.ToIntDef(0);

    // ÎŞÙÄ–{”L
    i_RecNo = 1286;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (AllFloatChk(s_Text.c_str(), 0) == 0) {
        i_Value_1 = s_Text.ToIntDef(0); // ÎŞÙÄ–{”L
    }
    else {
        G_ErrLog_Text = "ÎŞÙÄ–{”‚k      RecNo:" + FormatFloat("0000", i_RecNo) +
            "  •s³‚È’lw" + s_Text + "x ‚Å‚·B";
        Write_Error_Log(G_ErrLog_Text); // Ã·½Ä´×°Û¸Ş
        Excel_Err_DataSet(G_ErrLog_Text); // ´¸¾Ù´×°Û¸Ş

        i_ErrFlg = 1;
    }

    // ÎŞÙÄ–{”S
    i_RecNo = 1287;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (AllFloatChk(s_Text.c_str(), 0) == 0) {
        i_Value_2 = s_Text.ToIntDef(0); // ÎŞÙÄ–{”S
    }
    else {
        G_ErrLog_Text = "ÎŞÙÄ–{”‚r      RecNo:" + FormatFloat("0000", i_RecNo) +
            "  •s³‚È’lw" + s_Text + "x ‚Å‚·B";
        Write_Error_Log(G_ErrLog_Text); // Ã·½Ä´×°Û¸Ş
        Excel_Err_DataSet(G_ErrLog_Text); // ´¸¾Ù´×°Û¸Ş

        i_ErrFlg = 1;
    }

    // 2016.07.25 ƒ{ƒ‹ƒgƒJƒo[¡–@•ÏX
    s_Text = G_KS_Syurui + "|" + G_KS_Model;
    if (s_Text == "‚t‚w|‚X‚O") {
        if (P_HEADS_DATA[1267].TrimRight() == "200207") {
            // HEAD.VER‚ª200207(‹ŒÌÚ°Ñ)
            i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
            i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        }
        else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
            // HEAD.VER‚ª300207(‹ŒÌÚ°Ñ)
            i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
            i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        }
        else {
            // H21
            i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
            i_Value_Z = 4; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        }
    }
    else if (s_Text == "‚t‚w|‚W‚O") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚t‚w|‚P‚O‚O") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚t‚w|‚P‚O‚O‚q") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚t‚w|‚P‚P‚O‚q") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚t‚w|‚P‚R‚O") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚t‚w|‚P‚R‚O‚q") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    // 2017.06.23 LX-90D’Ç‰Á
    else if (s_Text == "‚k‚w|‚X‚O‚c") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 2; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        if ( P_HEADS_DATA[37].ToIntDef(0) == 94 ){
            // LX-90D-KNUP
            i_Value_2 = 12; // ÎŞÙÄ–{”S
        } else if ( P_HEADS_DATA[37].ToIntDef(0) == 394 ){
            // LX-90D-TNUP
            i_Value_2 = 12; // ÎŞÙÄ–{”S
        }
    }
    // *********************
    else if (s_Text == "‚r‚w|‚X‚O‚l") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚r‚w|‚X‚O‚k") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚q‚w|‚X‚O") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚q‚w|‚P‚O‚O") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚v‚w|‚X‚O") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else if (s_Text == "‚r‚w|‚W‚O‚r") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        // 2017.06.23 ”—Ê•ÏX
        //i_Value_Z = 4; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 2; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        // *******************@
    }
    else if (s_Text == "‚r‚w|‚W‚O‚l") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        // 2017.06.23 ”—Ê•ÏX
        //i_Value_Z = 4; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 2; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        // *******************@
    }
    else if (s_Text == "‚r‚w|‚W‚O‚k") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        // 2017.06.23 ”—Ê•ÏX
        i_Value_Z = 4; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        //i_Value_Z = 2; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        // *******************@
    }
    else if (s_Text == "‚r‚w|‚V‚O") {
        i_Value_L = 4; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }
    else {
        i_Value_L = 0; //LÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
        i_Value_Z = 0; //ã‰ºÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê
    }

    //S‹y‚Ñã‰ºSÎŞÙÄ—pÎŞÙÄ¶ÊŞ°”—Ê‚ªİ’è‚³‚ê‚Ä‚¢‚éê‡‚»‚ê‚ğ—Dæ
    if (i_Value_L != 0) {
        i_Value_S = i_Value_1 + i_Value_2 - i_Value_L - i_Value_Z;
    }
    else {
        i_Value_L = i_Value_1;
        i_Value_S = i_Value_2;
    }
    // *******************************

    // ***************************
    // ***  BLCP1
    // ***************************
    // wC019x,wC020x
    if (i_ErrFlg == 0) {
        i_RecNo = 1092; // BLCP1
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCP1 wC019x
            s_Text = s_Text + "L";
            P_Siyou1 = HanToZen(s_Text); // d—l‚P
            G_Log_Text = "BLCP1                w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // BLCP1–{” wC020x
            // 2016.07.25 ƒ{ƒ‹ƒgƒJƒo[¡–@•ÏX
            //P_1DaiSuryo = i_Value_1 + i_Value_2;
            P_1DaiSuryo = i_Value_L + i_Value_S + i_Value_Z;
            // *******************************
            P_Yobisu = 0; // —\”õ”
            P_Soukei = P_1DaiSuryo * i_Daisuu; // ‘Œv
            G_Log_Text = "BLCP1 –{”           w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // 1  •”•i•ª—Ş
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ¯•Êq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  •”•iŒ^®
            s_Text = ",’÷•tƒ{ƒ‹ƒgƒJƒo[";
            fputs(s_Text.c_str(), fp);
            // 4  •”•i–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  }”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ì}—p}”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1‘ä•ª”—Ê
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  —\”õ”
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ‘Œv(1‘ä•ª”—Ê~HEADSÃŞ°À“à‘ä”+—\”õ”)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 d—l‚P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 2014/08/22 ‰—p‹Zp•ÒW
            // 12 d—l‚Q
            // 2014.12.19 ÎŞÙÄŒa’ù³
            //s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "—p";
             s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",‚P@‚T^‚W|‚W‚t‚m—p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",‚Q|‚W‚t‚m—p";
            } else {
                s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "—p";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 d—l‚R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 d—l‚S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 d—l‚T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // ------------------------------
            // 2007.10.03 Ğ×°d—l‘Î‰
            // 17 d—lÀ²Ìßº°ÄŞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);
            // 2007.10.03
            // ------------------------------

            // ‰üs•¶š
            s_Text = "\n";
            // CSVÃŞ°À@Ì§²Ùo—Í
            fputs(s_Text.c_str(), fp);
        }
    }
    // ***************************
    // ***  BLCEL
    // ***************************
    // wC021x,wC022x
    if (i_ErrFlg == 0) {
        i_RecNo = 1096; // BLCEL
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCEL wC021x
            s_Text = s_Text + "L ƒLƒƒƒbƒv•t";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCEL                w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // BLCEL–{” wC022x
            // 2016.07.25 ƒ{ƒ‹ƒgƒJƒo[¡–@•ÏX
            //P_1DaiSuryo = i_Value_1;
            P_1DaiSuryo = i_Value_L;
            // *******************************
            P_Yobisu = 0; // —\”õ”
            P_Soukei = P_1DaiSuryo * i_Daisuu; // ‘Œv
            G_Log_Text = "BLCEL –{”           w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // 1  •”•i•ª—Ş
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ¯•Êq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  •”•iŒ^®
            s_Text = ",’÷•tƒ{ƒ‹ƒgƒJƒo[";
            fputs(s_Text.c_str(), fp);
            // 4  •”•i–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  }”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ì}—p}”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1‘ä•ª”—Ê
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  —\”õ”
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ‘Œv(1‘ä•ª”—Ê~HEADSÃŞ°À“à‘ä”+—\”õ”)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 d—l‚P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 2014/08/22 ‰—p‹Zp•ÒW
            // 12 d—l‚Q
            // 2014.12.19 ÎŞÙÄŒa’ù³
            //s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "—p";
            s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",‚P@‚T^‚W|‚W‚t‚m—p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",‚Q|‚W‚t‚m—p";
            } else {
                s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "—p";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 d—l‚R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 d—l‚S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 d—l‚T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // ------------------------------
            // 2007.10.03 Ğ×°d—l‘Î‰
            // 17 d—lÀ²Ìßº°ÄŞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);
            // 2007.10.03
            // ------------------------------

            // ‰üs•¶š
            s_Text = "\n";
            // CSVÃŞ°À@Ì§²Ùo—Í
            fputs(s_Text.c_str(), fp);
        }
    }

    // ***************************
    // ***  BLCES
    // ***************************
    // wC023x,wC024x
    if (i_ErrFlg == 0) {
        i_RecNo = 1097; // BLCES
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCEL wC023x
            s_Text = s_Text + "L ƒLƒƒƒbƒv•t";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCES                w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // BLCEL–{” wC024x
            // 2016.07.25 ƒ{ƒ‹ƒgƒJƒo[¡–@•ÏX
            //P_1DaiSuryo = i_Value_2;
            P_1DaiSuryo = i_Value_S;
            // *******************************
            P_Yobisu = 0; // —\”õ”
            P_Soukei = P_1DaiSuryo * i_Daisuu; // ‘Œv
            G_Log_Text = "BLCES –{”           w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // 1  •”•i•ª—Ş
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ¯•Êq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  •”•iŒ^®
            s_Text = ",’÷•tƒ{ƒ‹ƒgƒJƒo[";
            fputs(s_Text.c_str(), fp);
            // 4  •”•i–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  }”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ì}—p}”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1‘ä•ª”—Ê
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  —\”õ”
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ‘Œv(1‘ä•ª”—Ê~HEADSÃŞ°À“à‘ä”+—\”õ”)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 d—l‚P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 2014/08/22 ‰—p‹Zp•ÒW
            // 12 d—l‚Q
            // 2014.12.19 ÎŞÙÄŒa’ù³
            //s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "—p";
            s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",‚P@‚T^‚W|‚W‚t‚m—p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",‚Q|‚W‚t‚m—p";
            } else {
                s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "—p";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 d—l‚R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 d—l‚S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 d—l‚T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // ------------------------------
            // 2007.10.03 Ğ×°d—l‘Î‰
            // 17 d—lÀ²Ìßº°ÄŞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);
            // 2007.10.03
            // ------------------------------

            // ‰üs•¶š
            s_Text = "\n";
            // CSVÃŞ°À@Ì§²Ùo—Í
            fputs(s_Text.c_str(), fp);
        }
    }

    // 2016.07.25 ƒ{ƒ‹ƒgƒJƒo[¡–@•ÏX
    // ***************************
    // ***  BLCESS
    // ***************************
    if (i_ErrFlg == 0) {
        i_RecNo = 1112; // BLCESS
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if ((s_Text.ToIntDef(0) != 0)&& (i_Value_Z != 0)) {
            // BLCESS
            s_Text = s_Text + "L ƒLƒƒƒbƒv•t";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCESS                w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // BLCESS–{”
            P_1DaiSuryo = i_Value_Z;
            P_Yobisu = 0;                       // —\”õ”
            P_Soukei = P_1DaiSuryo * i_Daisuu;  // ‘Œv
            G_Log_Text = "BLCESS –{”           w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // 1  •”•i•ª—Ş
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ¯•Êq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  •”•iŒ^®
            s_Text = ",’÷•tƒ{ƒ‹ƒgƒJƒo[";
            fputs(s_Text.c_str(), fp);
            // 4  •”•i–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  }”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ì}—p}”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1‘ä•ª”—Ê
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  —\”õ”
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ‘Œv(1‘ä•ª”—Ê~HEADSÃŞ°À“à‘ä”+—\”õ”)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 d—l‚P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",‚P@‚T^‚W|‚W‚t‚m—p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",‚Q|‚W‚t‚m—p";
            } else {
                s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "—p";
            }
            fputs(s_Text.c_str(), fp);
            // 13 d—l‚R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 d—l‚S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 d—l‚T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 17 d—lÀ²Ìßº°ÄŞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);

            // ‰üs•¶š
            s_Text = "\n";
            // CSVÃŞ°À@Ì§²Ùo—Í
            fputs(s_Text.c_str(), fp);

        }
    }
    // *******************************

}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ƒ{ƒ‹ƒgƒJƒo[o—Í(¬Œaƒ{ƒ‹ƒg)
// ŠT  —v      F BLCP1S‚ÆBLCESS‚ğo—Í‚·‚éB
// ˆø  ”      F
// –ß‚è’l      F ‚È‚µ
// ”õ  l      F BLT_Cover_Out(void)‚ğ“¥P‚µ‚Äì¬B
// ---------------------------------------------------------------------------
// 2014/08/22 ‰—p‹Zp•ÒW
void __fastcall TFrameDetails::BLT_Cover_Out_Small(void) {
    int i_RecNo;
    int i_ErrFlg;
    int i_Value;
    int i_Daisuu;
    AnsiString s_Text;

    // ***************************
    // d—lÀ²Ìßº°ÄŞæ“¾(""/"A"/"B")
    // ***************************
    AnsiString sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());

    // ***************************
    // ***  ƒ{ƒ‹ƒgƒJƒo[
    // ***************************
    i_RecNo = 1111;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (s_Text == "0") {
        i_RecNo = 1112;
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text == "0") {
            return;
        }
    }

    G_Log_Text = "¬Œaƒ{ƒ‹ƒgƒJƒo[         w" + s_Text + "x‚ğƒZƒbƒgB";
    Write_Log(G_Log_Text);

    // **********************************************************
    // ***  ÎŞÙÄ–{””’lÁª¯¸
    // **********************************************************
    i_ErrFlg = 0;
    s_Text = P_HEADS_DATA[39];
    i_Daisuu = s_Text.ToIntDef(0);

    // ¬Œaƒ{ƒ‹ƒg–{”S
    i_RecNo = 1113;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (AllFloatChk(s_Text.c_str(), 0) == 0) {
        i_Value = s_Text.ToIntDef(0); // ¬Œaƒ{ƒ‹ƒg–{”S
    }
    else {
        G_ErrLog_Text = "¬ŒaÎŞÙÄ–{”‚r      RecNo:" + FormatFloat("0000", i_RecNo) +
            "  •s³‚È’lw" + s_Text + "x ‚Å‚·B";
        Write_Error_Log(G_ErrLog_Text);     // Ã·½Ä´×°Û¸Ş
        Excel_Err_DataSet(G_ErrLog_Text);   // ´¸¾Ù´×°Û¸Ş

        i_ErrFlg = 1;
    }

    // ***************************
    // ***  BLCP1S
    // ***************************
    if (i_ErrFlg == 0) {
        i_RecNo = 1111; // BLCP1S
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCP1S
            s_Text = s_Text + "L";
            P_Siyou1 = HanToZen(s_Text); // d—l‚P
            G_Log_Text = "BLCP1S               w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // BLCP1S–{”
            P_1DaiSuryo = i_Value;
            P_Yobisu = 0;                       // —\”õ”
            P_Soukei = P_1DaiSuryo * i_Daisuu;  // ‘Œv
            G_Log_Text = "BLCP1S –{”           w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // 1  •”•i•ª—Ş
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ¯•Êq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  •”•iŒ^®
            s_Text = ",’÷•tƒ{ƒ‹ƒgƒJƒo[";
            fputs(s_Text.c_str(), fp);
            // 4  •”•i–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  }”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ì}—p}”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1‘ä•ª”—Ê
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  —\”õ”
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ‘Œv(1‘ä•ª”—Ê~HEADSÃŞ°À“à‘ä”+—\”õ”)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 d—l‚P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 12 d—l‚Q
            // 2014.12.19 ÎŞÙÄŒa’ù³
            //s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "—p";
            s_Text = P_HEADS_DATA[1108].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",‚P@‚T^‚W|‚W‚t‚m—p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",‚Q|‚W‚t‚m—p";
            } else {
                s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "—p";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 d—l‚R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 d—l‚S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 d—l‚T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // 17 d—lÀ²Ìßº°ÄŞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);

            // ‰üs•¶š
            s_Text = "\n";
            // CSVÃŞ°À@Ì§²Ùo—Í
            fputs(s_Text.c_str(), fp);
        }
    }

    // ***************************
    // ***  BLCESS
    // ***************************
    if (i_ErrFlg == 0) {
        i_RecNo = 1112; // BLCESS
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCESS
            s_Text = s_Text + "L ƒLƒƒƒbƒv•t";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCESS                w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // BLCESS–{”
            P_1DaiSuryo = i_Value;
            P_Yobisu = 0;                       // —\”õ”
            P_Soukei = P_1DaiSuryo * i_Daisuu;  // ‘Œv
            G_Log_Text = "BLCESS –{”           w" + s_Text + "x‚ğƒZƒbƒgB";
            Write_Log(G_Log_Text);

            // 1  •”•i•ª—Ş
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ¯•Êq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  •”•iŒ^®
            s_Text = ",’÷•tƒ{ƒ‹ƒgƒJƒo[";
            fputs(s_Text.c_str(), fp);
            // 4  •”•i–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  }”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ì}—p}”Ô
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1‘ä•ª”—Ê
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  —\”õ”
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ‘Œv(1‘ä•ª”—Ê~HEADSÃŞ°À“à‘ä”+—\”õ”)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 d—l‚P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 12 d—l‚Q
            // 2014.12.19 ÎŞÙÄŒa’ù³
            //s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "—p";
            s_Text = P_HEADS_DATA[1108].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",‚P@‚T^‚W|‚W‚t‚m—p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",‚Q|‚W‚t‚m—p";
            } else {
                s_Text = ",‚l" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "—p";
            }
            //**********************
            fputs(s_Text.c_str(), fp);
            // 13 d—l‚R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 d—l‚S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 d—l‚T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 Ş¿–¼Ì
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // 17 d—lÀ²Ìßº°ÄŞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);

            // ‰üs•¶š
            s_Text = "\n";
            // CSVÃŞ°À@Ì§²Ùo—Í
            fputs(s_Text.c_str(), fp);
        }
    }
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ƒ{ƒ‹ƒg—\”õ•iæ“¾ˆ—
// ŠT  —v      F
// ˆø  ”      F ƒ{ƒ‹ƒgƒTƒCƒY@Ş¿ƒR[ƒh
// –ß‚è’l      F “ÇŒ‹‰Ê [true:³í false:ˆÙí]
// ”õ  l      F ‚È‚µ
// ---------------------------------------------------------------------------
int __fastcall TFrameDetails::Get_Blt_YobiSu(double d_BltSz, double d_BltLen,
    AnsiString MatCD, HeadsData* pcHeadsData) {
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E

    AnsiString s_SQL;
    AnsiString s_YobiName;
    int i_Yobisu;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_BLT2YOBI";
    s_SQL = s_SQL + "  WHERE BLTSIZE = " + d_BltSz;

    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC‚Ö‚ÌˆÚs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        s_YobiName = "";
    }
    else {
        s_YobiName = wkQuery->FieldByName("YOBINAME")->AsString.TrimRight();
        // —\”õ•i¯•Êq

        G_Log_Text = "—\”õ•i¯•Êq    ¢" + s_YobiName + "£‚ğæ“¾B";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;

    // CSV“Ç—p
    FILE *wkfp;

    char buf[1024]; // •¶š—ñƒoƒbƒtƒ@
    char *tok; // ƒg[ƒNƒ“ƒ|ƒCƒ“ƒ^
    int i; // ƒ‹[ƒvƒJƒEƒ“ƒ^
    char token[100][100]; // ƒg[ƒNƒ“ƒoƒbƒtƒ@

    AnsiString s_Yobi_Text[5];
    AnsiString s_YBKey;
    AnsiString s_YBZaisitu;

    AnsiString wk_FilePass; // "D00001.csv"
    int i_RecNo;
    int i_lop;

    i_RecNo = 0; // Úº°ÄŞ¶³İÀ(’ŠoğŒˆê’vŒ”)
    i_Yobisu = 0;

    // --------------------------------------------
    // 2007.10.04 Ğ×°d—l‘Î‰
    // Ğ×°Bd—l‚Ìê‡A—\”õ•i‚ÍŒvã‚µ‚È‚¢
    if (pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B)
        return 0;
    // 2007.10.04
    // --------------------------------------------

    // ˆê“xŒvã‚µ‚½—\”õ•i‚ÍŒvã‚µ‚È‚¢
    for (i_lop = 0; i_lop < G_YOBICHK_RecCnt; i_lop++) {
        if ((s_YobiName == G_YOBICHK_NAME[i_lop]) &&
            (MatCD == G_YOBICHK_MAT[i_lop])) {
            return 0;
        }
    }

    // ÌßÚ°Ä—\”õ•iÃŞ°ÀÊß½
    wk_FilePass = G_AnaFilePass + G_SAKUBAN + ".csv";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ´×°Û¸Şì¬
        G_ErrLog_Text = "—\”õ•iÃŞ°À w" + wk_FilePass + "x‚Ì‚n‚o‚d‚m‚É¸”s‚µ‚Ü‚µ‚½II";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // “Ç‚İ‰Â”\‚ÌŠÔ
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), wkfp) != NULL) {
            i = 0; // ƒg[ƒNƒ“ƒJƒEƒ“ƒ^‰Šú‰»
            memset(token, 0x00, sizeof(token)); // ƒg[ƒNƒ“ƒoƒbƒtƒ@‰Šú‰»
            tok = GetToken(buf, ","); // ","‚Å€–Ú‚ğ‹æØ‚é
            while (tok) { // ƒg[ƒNƒ“‚ª‘¶İ‚·‚éŠÔ
                sprintf(token[i++], "%-.99s", tok); // ƒg[ƒNƒ“‚ğƒoƒbƒtƒ@‚É“]‘—
                tok = GetToken(NULL, ","); // Ÿ‚Ìƒg[ƒNƒ“‚ğŒŸõ

                if (i > 100) {
                    break;
                }
            }
            // ÃŞ°À—á
            s_Yobi_Text[0] = token[0];
            // —\”õ•iÃŞ°À 1—ñ–Ú ( 'PLTE','GSKT','ADHS','THRM'... )
            s_Yobi_Text[1] = token[1];
            // —\”õ•iÃŞ°À 2—ñ–Ú ( ÌŞ×İ¸,'D','E','1','2'... )
            s_Yobi_Text[2] = token[8]; // —\”õ•iÃŞ°À —\”õ”( 10, 1, 5, 2, 8, ... )
            s_Yobi_Text[3] = token[11]; // —\”õ•iÃŞ°À Ş¿ƒR[ƒh( 1011, ... )
            s_Yobi_Text[4] = token[24]; // —\”õ•iÃŞ°À ‘S’·

            // ŒŸõ·°¶¬
            if (AllNumChk(s_Yobi_Text[1].c_str(), 0) == 0) {
                int i_No;

                i_No = StrToInt(s_Yobi_Text[1]) + 100;
                s_YBKey = s_Yobi_Text[0] + IntToStr(i_No).SubString(2, 2);
                s_YBZaisitu = s_Yobi_Text[3].TrimRight();
            }
            else {
                // ********************************************
                // —\”õ•iÃŞ°À 2—ñ–Ú‚ª
                // ±ÙÌ§ÍŞ¯Ä‚à‚µ‚­‚ÍNULL‚¾‚Á‚½ê‡ ŒŸõ‚µ‚È‚¢?
                // ********************************************
                s_YBKey = "";
            }

            if (s_YBKey != "\0") {
                if (s_YBKey == s_YobiName) {
                    if ((((MatCD != "0") && (s_YBZaisitu == MatCD)) ||
                        (MatCD == "0")) &&
                        (d_BltLen == s_Yobi_Text[4].ToDouble())) {
                        i_Yobisu += s_Yobi_Text[2].ToIntDef(0);
                        i_RecNo++;
                        G_YOBICHK_NAME[G_YOBICHK_RecCnt] = s_YobiName;
                        G_YOBICHK_MAT[G_YOBICHK_RecCnt] = MatCD;
                        G_YOBICHK_RecCnt++;
                    }
                }
            }
        }
    }

    // ŠY“–—\”õ•iÃŞ°À‚ª‘¶İ‚µ‚È‚©‚Á‚½ê‡
    if (i_RecNo == 0)
        i_Yobisu = 0;

    // —\”õ•iÃŞ°À Ì§²Ù¸Û°½Ş
    fclose(wkfp);
    return i_Yobisu;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F ƒXƒy[ƒT[E–¾d—l æ“¾
// ŠT  —v      F
// ˆø  ”      F Kbn FS¥¥¥SÌÚ°Ñ—pˆ—CE¥¥¥EÌÚ°Ñˆ—
// –ß‚è’l      F ˆ—Œ‹‰Ê
// ”õ  l      F 2022.01.14’Ç‰Á
// ---------------------------------------------------------------------------
AnsiString __fastcall TFrameDetails::Get_AnaakeSiyou_S(AnsiString Kbn) {
    AnsiString s_Text;

    AnsiString s_S1Nzl_Syurui; // S1É½ŞÙí—Ş
    AnsiString s_S2Nzl_Syurui; // S2É½ŞÙí—Ş
    AnsiString s_S3Nzl_Syurui; // S3É½ŞÙí—Ş
    AnsiString s_S4Nzl_Syurui; // S4É½ŞÙí—Ş

    AnsiString s_E1Nzl_Syurui; // E1É½ŞÙí—Ş
    AnsiString s_E2Nzl_Syurui; // E2É½ŞÙí—Ş
    AnsiString s_E3Nzl_Syurui; // E3É½ŞÙí—Ş
    AnsiString s_E4Nzl_Syurui; // E4É½ŞÙí—Ş

    AnsiString s_AnaSiyou; // E–¾d—l
    AnsiString s_S1;
    AnsiString s_S2;
    AnsiString s_S3;
    AnsiString s_S4;
    AnsiString s_S[5];
    AnsiString s_E[5];

    int i_OgataFlg; // 0:¬Œ^’†Œ^,1:‘åŒ^

    // *************************
    // ***  HEADSÃŞ°À@¾¯Ä
    // *************************
    if (Kbn == "S") {
        s_S1Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1É½ŞÙí—Ş
        s_S2Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2É½ŞÙí—Ş
        s_S3Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3É½ŞÙí—Ş
        s_S4Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4É½ŞÙí—Ş
    }
    else {
        s_E1Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1É½ŞÙí—Ş
        s_E2Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2É½ŞÙí—Ş
        s_E3Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3É½ŞÙí—Ş
        s_E4Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4É½ŞÙí—Ş
    }


    if (Kbn == "S") {
        // *************************
        // ***  S1@•”•ª
        // *************************
        if (s_S1Nzl_Syurui == "0") {
            s_S[1] = "0";
        }
        else {
            s_S[1] = "1";
        }

        // *************************
        // ***  S2@•”•ª
        // *************************
        if (s_S2Nzl_Syurui == "0") {
            s_S[2] = "0";
        }
        else {
            s_S[2] = "2";
        }

        // *************************
        // ***  S3@•”•ª
        // *************************
        if (s_S3Nzl_Syurui == "0") {
            s_S[3] = "0";
        }
        else {
            s_S[3] = "3";
        }

        // *************************
        // ***  S4@•”•ª
        // *************************
        if (s_S4Nzl_Syurui == "0") {
            s_S[4] = "0";
        }
        else {
            s_S[4] = "4";
        }

        // Šeæ“¾€–ÚŒ‹‡
        s_AnaSiyou = "S" + s_S[1] + s_S[2] + s_S[3] + s_S[4];

    }
    else {
        // *************************
        // ***  E1@•”•ª
        // *************************
        if (s_E1Nzl_Syurui == "0") {
            s_E[1] = "0";
        }
        else {
            s_E[1] = "1";
        }

        // *************************
        // ***  E2@•”•ª
        // *************************
        if (s_E2Nzl_Syurui == "0") {
            s_E[2] = "0";
        }
        else {
            s_E[2] = "2";
        }

        // *************************
        // ***  E3@•”•ª
        // *************************
        if (s_E3Nzl_Syurui == "0") {
            s_E[3] = "0";
        }
        else {
            s_E[3] = "3";
        }

        // *************************
        // ***  E4@•”•ª
        // *************************
        if (s_E4Nzl_Syurui == "0") {
            s_E[4] = "0";
        }
        else {
            s_E[4] = "4";
        }

        // Šeæ“¾€–ÚŒ‹‡
        s_AnaSiyou = "E" + s_E[1] + s_E[2] + s_E[3] + s_E[4];
    }

    return s_AnaSiyou;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F SÌÚ°Ñ¶Ş½·ØÃŞ°Àì¬ˆ— Às
// ŠT  —v      F
// ˆø  ”      F ‚È‚µ
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:³í false:ˆÙí]
// ”õ  l      F 2022.05.23’Ç‰Á
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::S_FrameG_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    AnsiString s_ChinaM;

    // **********************************************
    // ***  ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)@ŒŸõ
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "ŒŸõ·°w" + P_Key + "x‚ÅAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚ğŒŸõB";
    Write_Log(G_Log_Text);

    // seek_ret¥¥¥Ï½À‚É‘¶İ‚µ‚½‚©‚µ‚È‚¢‚©
    // ¦’ Write_SE_Frame_Data()‚Åg—p‚·‚éB
    seek_ret = Search_HD_FRM_NZ(P_Key, "SG");

    G_Log_Text = "ŒŸõ·°w" + P_Key + "x‚ÅAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚ğŒŸõI—¹B";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  Ş¿–¼Ì‚ÌŒŸõ(ASME‚ªŠÜ‚Ü‚ê‚Ä‚¢‚é‚©)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "Ş¿w" + P_ZaisituNM + "x‚ªAASME‚Å‚·B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("‚`‚r‚l‚d") != 0) {
            G_ErrLog_Text = "Ş¿w" + P_ZaisituNM + "x‚ªA‚`‚r‚l‚d‚Å‚·B";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  E–¾d—l‚Ìì¬
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("S");
    G_Log_Text = "E–¾d—l  w" + P_Siyou1 + "x‚ğæ“¾B";
    Write_Log(G_Log_Text);

    // *************************
    // ***  Ş¿‚Ìæ“¾
    // *************************
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);

    switch (P_HEADS_DATA[37].ToIntDef(0)) {
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // ‚aƒVƒŠ[ƒY
        if (P_ZaisituNM == "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        break;
    // 2023.03.09 CX-00-Y’Ç‰Á_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y’Ç‰Á_E
    case 39:
    case 40:
    case 43:
    case 49:
    case 50:
    case 52:
    case 53:
    case 54:
    case 339:
    case 340:
    case 343:
    case 349:
    case 350:
    case 352:
    case 353:
    case 354: // ‚xƒVƒŠ[ƒY
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "‚r‚Q‚V‚T‚i‚q";
            P_EZaisituNM = "‚r‚Q‚V‚T‚i‚q";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
            P_ZaisituNM = "‚r‚r‚S‚O‚O";
            P_EZaisituNM = "‚r‚r‚S‚O‚O";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }

        break;
    }

    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "|" + G_KS_Model;
    if (s_Text == "‚t‚w|‚P‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚O‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚P‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚R‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚T‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚Q‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚S‚P") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚S‚R") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚P‚P") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚P‚R") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚R‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚T‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚V‚O") { // ’†‘Ş¿
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // ’†‘Ş¿
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "SS400") {
        }
        else if (P_ZaisituNM == "‚r‚r‚S‚O‚O" || P_ZaisituNM == "‚bD‚rD") {
            P_ZaisituNM = "‚p‚Q‚R‚T‚a";
            P_EZaisituNM = "‚p‚Q‚R‚T‚a";
        }
    }

    // **********************************************
    // ***  }”ÔEì}—p}”Ô‚Ìæ“¾  02.09.03’Ç‰Á
    // **********************************************
    bSUS = false;
    sPltMat = P_HEADS_DATA[43].TrimRight();
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠO
        bSUS = true;
    }
    if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <= P_PLATE_NUM[1]))
        || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[1]))) {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        P_Siyou3 = "1";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[2])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[2]))) {
        P_Zuban = P_DNO[2];
        P_SZuban = P_WKDNO[2];
        P_Siyou3 = "2";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[3])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[3]))) {
        P_Zuban = P_DNO[3];
        P_SZuban = P_WKDNO[3];
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[4])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[4]))) {
        P_Zuban = P_DNO[4];
        P_SZuban = P_WKDNO[4];
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[5])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[5]))) {
        P_Zuban = P_DNO[5];
        P_SZuban = P_WKDNO[5];
        P_Siyou3 = "3";
    }
    else {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        P_Siyou3 = "1";
    }

    // *************************
    // ***  —\”õ”‚Ìæ“¾
    // *************************
    Syukei_Frame_Yobisu("S");

    // *************************
    // ***  SÌÚ°ÑÃŞ°À‚Ìo—Í
    // *************************
    if ( P_DNO[1] == "NONE" ) {
    }
    else {
        Write_SE_Frame_Data("SG", seek_ret);
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// “ú–{ŒêŠÖ”–¼F EÌÚ°Ñ¶Ş½·ØÃŞ°Àì¬ˆ— Às
// ŠT  —v      F
// ˆø  ”      F ‚È‚µ
// –ß‚è’l      F ˆ—Œ‹‰Ê [true:³í false:ˆÙí]
// ”õ  l      F 2022.05.23’Ç‰Á
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::E_FrameG_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    AnsiString s_ChinaM;

    // **********************************************
    // ***  ÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)@ŒŸõ
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "ŒŸõ·°w" + P_Key + "x‚ÅAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚ğŒŸõB";
    Write_Log(G_Log_Text);

    // seek_ret¥¥¥Ï½À‚É‘¶İ‚µ‚½‚©‚µ‚È‚¢‚©
    // Write_SE_Frame_Data()‚Åg—p‚·‚éB
    seek_ret = Search_HD_FRM_NZ(P_Key, "EG");

    G_Log_Text = "ŒŸõ·°w" + P_Key + "x‚ÅAÌÚ°ÑÉ½ŞÙ•”•i\¬Ï½À(HD_FRM_NZ)‚ğŒŸõI—¹B";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  Ş¿–¼Ì‚ÌŒŸõ(ASME‚ªŠÜ‚Ü‚ê‚Ä‚¢‚é‚©)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "Ş¿w" + P_ZaisituNM + "x‚ªAASME‚Å‚·B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("‚`‚r‚l‚d") != 0) {
            G_ErrLog_Text = "Ş¿w" + P_ZaisituNM + "x‚ªA‚`‚r‚l‚d‚Å‚·B";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  E–¾d—l‚Ìì¬
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("E");
    G_Log_Text = "E–¾d—l  w" + P_Siyou1 + "x‚ğæ“¾B";
    Write_Log(G_Log_Text);

    // *************************
    // ***  Ş¿‚Ìæ“¾
    // *************************
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);

    // **********************************************
    // ***  }”ÔEì}—p}”Ô‚Ìæ“¾  02.09.03’Ç‰Á
    // **********************************************
    bSUS = false;
    sPltMat = P_HEADS_DATA[43].TrimRight();
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // ƒvƒŒ[ƒgŞ¿‚ªƒ`ƒ^ƒ“ŒnˆÈŠO
        bSUS = true;
    }
    if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <= P_PLATE_NUM[1]))
        || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[1]))) {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "1";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[2])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[2]))) {
        P_Zuban = P_DNO[2];
        P_SZuban = P_WKDNO[2];
        if (P_MatNM[2] != "") {
            P_ZaisituNM = P_MatNM[2];
            P_EZaisituNM = P_MatNM[2];
        }
        else if (P_MatCD[2] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[2]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "2";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[3])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[3]))) {
        P_Zuban = P_DNO[3];
        P_SZuban = P_WKDNO[3];
        if (P_MatNM[3] != "") {
            P_ZaisituNM = P_MatNM[3];
            P_EZaisituNM = P_MatNM[3];
        }
        else if (P_MatCD[3] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[4])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[4]))) {
        P_Zuban = P_DNO[4];
        P_SZuban = P_WKDNO[4];
        if (P_MatNM[4] != "") {
            P_ZaisituNM = P_MatNM[4];
            P_EZaisituNM = P_MatNM[4];
        }
        else if (P_MatCD[4] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[4]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[4]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "4";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[5])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[5]))) {
        P_Zuban = P_DNO[5];
        P_SZuban = P_WKDNO[5];
        if (P_MatNM[5] != "") {
            P_ZaisituNM = P_MatNM[5];
            P_EZaisituNM = P_MatNM[5];
        }
        else if (P_MatCD[5] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[5]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[5]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "5";
    }
    else {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "1";
    }

    switch (P_HEADS_DATA[37].ToIntDef(0)) { // ƒtƒŒ[ƒ€ƒR[ƒh
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // ‚aƒVƒŠ[ƒY
        if (P_ZaisituNM == "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        break;
    // 2023.03.09 CX-00-Y’Ç‰Á_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y’Ç‰Á_E
    case 39:
    case 40:
    case 43:
    case 49:
    case 50:
    case 52:
    case 53:
    case 54:
    case 339:
    case 340:
    case 343:
    case 349:
    case 350:
    case 352:
    case 353:
    case 354: // ‚xƒVƒŠ[ƒY
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "‚r‚Q‚V‚T‚i‚q";
            P_EZaisituNM = "‚r‚Q‚V‚T‚i‚q";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
            P_ZaisituNM = "‚r‚r‚S‚O‚O";
            P_EZaisituNM = "‚r‚r‚S‚O‚O";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }

        break;
    }

    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "|" + G_KS_Model;
    if (s_Text == "‚t‚w|‚P‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚O‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚P‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚R‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚k‚w|‚T‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚Q‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚S‚P") { // ’†‘Ş¿
    }
    else if (s_Text == "‚r‚w|‚S‚R") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚P‚P") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚P‚R") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚R‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚T‚O") { // ’†‘Ş¿
    }
    else if (s_Text == "‚q‚w|‚V‚O") { // ’†‘Ş¿
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // ’†‘Ş¿
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "SS400") {
        }
        else if (P_ZaisituNM == "‚r‚r‚S‚O‚O" || P_ZaisituNM == "‚bD‚rD") {
            P_ZaisituNM = "‚p‚Q‚R‚T‚a";
            P_EZaisituNM = "‚p‚Q‚R‚T‚a";
        }
    }

    // *************************
    // ***  —\”õ”‚Ìæ“¾
    // *************************
    Syukei_Frame_Yobisu("E");

    // *************************
    // ***  EÌÚ°ÑÃŞ°À‚Ìo—Í
    // *************************
    if ( P_DNO[1] == "NONE" ) {
    }
    else {
        Write_SE_Frame_Data("EG", seek_ret);
    }

    delete wkQuery;

    return true;
}





