// ----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�F�t���[�����i�W�J����  ���W���[��
// �� �� �ҁFN.Uchida
// �� �� ���F2002.06.21
// �X �V ���F2002.10.08
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>

#pragma hdrstop

#include <io.h>
#include <Filectrl.hpp>

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_FrameDetails.h"

#include "HeadsData.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// �t���[�����i�W�J���� Class
// ---------------------------------------------------------------------------
class TFrameDetails {
private:
    FILE *fp;

    // �ėp
    AnsiString P_SQL; // SQL���p
    AnsiString P_Key; // �����p��

    // CSV̧�ٍ쐬�p
    AnsiString P_BBunrui; // ���i����
    AnsiString P_Sikibetu; // ���ʎq
    AnsiString P_BKatasiki; // ���i�^��
    AnsiString P_BMeisyou; // ���i����
    AnsiString P_ZaisituNM; // �ގ�����
    AnsiString P_EZaisituNM; // �ގ�����
    AnsiString P_ZaisituCD; // �ގ��R�[�h
    AnsiString P_Zuban; // �}��
    AnsiString P_SZuban; // ��}�p�}��
    int P_1DaiSuryo; // �P�䕪����
    int P_Yobisu; // �\����
    int P_Soukei; // ���v
    int P_RecNo; // ں��޶���

    AnsiString P_Siyou1; // ���l�P
    AnsiString P_Siyou2; // ���l�Q
    AnsiString P_Siyou3; // ���l�R
    AnsiString P_Siyou4; // ���l�S
    AnsiString P_Siyou5; // ���l�T

    // �ڰэ\�����i����ð���(FRAMECD.CSV)�p
    AnsiString P_FRAMECD[5];

    // �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�p
    AnsiString P_DNO[6]; // �}��
    AnsiString P_WKDNO[6]; // ��}�p�}��
    AnsiString P_MatNM[6]; // �ގ�����
    AnsiString P_MatCD[6]; // �ގ��R�[�h
    int P_PLATE_NUM[6]; // ����
    int P_PLATE_NUM_SUS[6]; // ����(SUS�p)
    int P_SUS; // SUS�g�p�t���O

    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];

    bool __fastcall S_Frame_Hontai_Sakusei(void); // S�ڰ��ް��@   �쐬
    bool __fastcall E_Frame_Hontai_Sakusei(void); // E�ڰ��ް��@   �쐬
    // 2022.05.23 �t���[���K�X�؂�ǉ�_S
    bool __fastcall S_FrameG_Hontai_Sakusei(void); // S�ڰѶ޽���ް��@   �쐬
    bool __fastcall E_FrameG_Hontai_Sakusei(void); // E�ڰѶ޽���ް��@   �쐬
    // 2022.05.23 �t���[���K�X�؂�ǉ�_E
    bool __fastcall Frame_Buhin_Sakusei(void); // �ڰѕ��i�ް��@�쐬

    bool __fastcall Search_HD_FRM_NZ(AnsiString Key, AnsiString Kbn);
    // �ڰ�ɽ�ٕ��i�\��Ͻ��@����
    bool __fastcall Search_HD_FRM_NZ_Buhin(AnsiString Key);
    // �ڰ�ɽ�ٕ��i�\��Ͻ��@����

    bool __fastcall Write_SE_Frame_Data(AnsiString Kbn, bool Seek_Result);
    // S,E�ڰ��ް��@��������
    bool __fastcall Write_FrameBuhin_Data(bool Seek_Result); // �ڰѕ��i�ް� ��������
    bool __fastcall Syukei_Frame_Yobisu(AnsiString a_Sekbn); // �ڰї\�����W�v

    AnsiString __fastcall Get_AnaakeSiyou(AnsiString Kbn); // �E���d�l �擾
    AnsiString __fastcall Get_BuhinKatasiki(AnsiString Code); // ���i�^���@�擾
    void __fastcall BLT_Cover_Out(void);
    // 2014/08/22 ���p�Z�p�ҏW
    void __fastcall BLT_Cover_Out_Small(void);
    // 2022.01.14 �X�y�[�T�ǉ�_S
    AnsiString __fastcall Get_AnaakeSiyou_S(AnsiString Kbn); // �E���d�l �擾
    // 2022.01.14 �X�y�[�T�ǉ�_E

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // �ڰѕ��i�W�J���� ���s
    int __fastcall Get_Blt_YobiSu(double d_BltSz, double d_BltLen,
        AnsiString MatCD, HeadsData* pcHeadsData);

};

// ---------------------------------------------------------------------------
// ���{��֐����F �t���[�����i �W�J����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall FrameDetails(HeadsData* pcHeadsData) {
    bool ret;

    TFrameDetails flameDetails; // ��ڰĖ{���ް��C�E���ް� �쐬Class��������,�֐��I����j��

    ret = flameDetails.Excute(pcHeadsData); // ��ڰĖ{���ް��C�E���ް� �쐬���s

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �ڰѕ��i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    m_pcHeadsData = pcHeadsData;
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********      �w�ڰѕ��i�W�J�����x�J�n      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********      �w�ڰѕ��i�W�J�����x�J�n      *********");
    Write_Error_Log("*******************************************************");

    // 2022.05.23 �t���[���K�X�؂�ǉ�_S
    // ***********************************
    // ***  S�ڰѶ޽���ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   �wS�ڰѶ޽���ް��x�쐬�J�n   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    S_FrameG_Hontai_Sakusei();

    G_Log_Text = "----------   �wS�ڰѶ޽���ް��x�쐬�I��   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    // 2022.05.23 �t���[���K�X�؂�ǉ�_E

    // ***********************************
    // ***  S�ڰ��ް��@�쐬
    // ***********************************
    G_Log_Text = "----------     �wS�ڰ��ް��x�쐬�J�n      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    S_Frame_Hontai_Sakusei();

    G_Log_Text = "----------     �wS�ڰ��ް��x�쐬�I��      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // 2022.05.23 �t���[���K�X�؂�ǉ�_S
    // ***********************************
    // ***  E�ڰѶ޽���ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   �wE�ڰѶ޽���ް��x�쐬�J�n   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    E_FrameG_Hontai_Sakusei();

    G_Log_Text = "----------   �wE�ڰѶ޽���ް��x�쐬�I��   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    // 2022.05.23 �t���[���K�X�؂�ǉ�_E

    // ***********************************
    // ***  E�ڰ��ް��@�쐬
    // ***********************************
    G_Log_Text = "----------     �wE�ڰ��ް��x�쐬�J�n      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    E_Frame_Hontai_Sakusei();

    G_Log_Text = "----------     �wE�ڰ��ް��x�쐬�I��      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);


    // ***********************************
    // ***  �ڰѕ��i�ް��@�쐬
    // ***********************************
    G_Log_Text = "----------     �w�ڰѕ��i�ް��x�쐬�J�n      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    Frame_Buhin_Sakusei();

    G_Log_Text = "----------     �w�ڰѕ��i�ް��x�쐬�I��      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********      �w�ڰѕ��i�W�J�����x�I��      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********      �w�ڰѕ��i�W�J�����x�I��      *********");
    Write_Error_Log("*******************************************************");
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �ڰѕ��i�ް��쐬���� ���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Frame_Buhin_Sakusei(void) {
    FILE *wkfp;

    bool seek_ret;
    AnsiString s_FilePass;
    AnsiString s_Katasiki;
    int i_FrameCode;

    char buf[1024]; /* ������o�b�t�@ */
    char *tok; /* �g�[�N���|�C���^ */
    int i; /* ���[�v�J�E���^ */
    char token[100][100]; /* �g�[�N���o�b�t�@ */

    // 2017.06.23 LX-90D-NUP�ǉ�
    AnsiString s_Text;
    // *************************

    // ***************************
    // **  �ڰь^���ϊ�ð���
    // ***************************
    s_FilePass = G_Input_Csv_Dir + "FRAMECD.csv";

    if ((wkfp = fopen(s_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�w" + s_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), wkfp) != NULL) {
        i = 0; // �g�[�N���J�E���^������
        memset(token, 0x00, sizeof(token)); // �g�[�N���o�b�t�@������
        tok = GetToken(buf, ","); // ","�ō��ڂ���؂�
        while (tok) { // �g�[�N�������݂����
            sprintf(token[i++], "%-.99s", tok); // �g�[�N�����o�b�t�@�ɓ]��
            tok = GetToken(NULL, ","); // ���̃g�[�N��������

            if (i > 100) {
                break;
            }
        }

        // CSV�Ǎ��݌��ʁ@�Z�b�g
        P_FRAMECD[1] = token[0]; // FRAME.CSV�F����
        P_FRAMECD[2] = token[1]; // FRAME.CSV�F���i���޺���
        P_FRAMECD[3] = token[2]; // FRAME.CSV�F���i�敪
        P_FRAMECD[4] = token[3]; // FRAME.CSV�F���i����

        s_Katasiki = token[4]; // �\���i���擾�p

        // 2017.06.23 LX-90D-NUP�ǉ�
        // 2014/08/22 ���p�Z�p�ҏW
        // �R�[�h��"75"(BNWRN-,���tBNW) ���� Z�t���[���łȂ��ꍇ�A�o�͑ΏۊO
        //if(P_FRAMECD[1] == "75" &&
        //   P_HEADS_DATA[36].Pos("Z") == 0) {
        //
        //   continue;
        //
        //}
        s_Text = G_KS_Syurui + "�|" + G_KS_Model;
        if( P_FRAMECD[1] == "75" ) {
            // ���t����(��)
            if (( s_Text == "�k�w�|�X�O�c") && ( P_HEADS_DATA[37].ToIntDef(0)) == 94 ){
                // LX-90D-KNUP  �o�͑Ώ�
            } else if (( s_Text == "�k�w�|�X�O�c") && ( P_HEADS_DATA[37].ToIntDef(0)) == 394 ){
                // LX-90D-TNUP  �o�͑Ώ�
            } else if( P_HEADS_DATA[36].Pos("Z") == 0) {
                // Z�ڰ�  �o�͑ΏۊO
                continue;
            }
        }
        // *************************

        i_FrameCode = P_FRAMECD[1].ToIntDef(1);

        // 2022.05.23 �t���[���K�X�؂�ǉ�_S
        //if (i_FrameCode != 1 && i_FrameCode != 8) {
        if (i_FrameCode != 1 && i_FrameCode != 8 && i_FrameCode != 81 && i_FrameCode != 88) {
        // 2022.05.23 �t���[���K�X�؂�ǉ�_E

            // �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)������
            P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
                P_HEADS_DATA[1267].TrimRight();

            // *********************************************
            // ***  �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)����
            // *********************************************
            seek_ret = Search_HD_FRM_NZ_Buhin(P_Key);

            // **********************
            // ***  �\���i�ް��@�擾
            // **********************
            P_Yobisu = 0;

            // �\�����̎擾
            P_Yobisu = Syukei_Buhin_Yobisu(s_Katasiki, P_ZaisituCD,
                m_pcHeadsData);

            // ***************************
            // ***  �ڰѕ��i�ް��@����
            // ***************************
            Write_FrameBuhin_Data(seek_ret);

        }

    }

    fclose(wkfp); // ����
    // delete wkfp;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �\���i�ް��@�W�v����
// �T  �v      �F �\���i�ް�����\���i�������߂�
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 02.08.03�ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Syukei_Frame_Yobisu(AnsiString a_Sekbn) {
    // CSV�Ǎ��p
    FILE *wkfp;

    char buf[1024]; /* ������o�b�t�@ */
    char *tok; /* �g�[�N���|�C���^ */
    int i; /* ���[�v�J�E���^ */
    char token[100][100]; /* �g�[�N���o�b�t�@ */

    AnsiString s_Yobi_Text[5];
    AnsiString a_Kbn;
    AnsiString wk_FilePass; // "D00001.csv"

    P_RecNo = 0; // ں��޶���(���o������v����)

    // -----------------------------------------------
    // 2007.10.03 �ǉ�
    // �װB�d�l�̏ꍇ�́A�\���i�ް��Ȃ��Ƃ��ď���
    if (m_pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B) {
        P_Yobisu = 0;
        return true;
    }
    // 2007.10.03
    // -----------------------------------------------

    // ��ڰė\���i�ް��߽
    wk_FilePass = G_AnaFilePass + G_SAKUBAN + ".csv";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�\���i�ް� �w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // �Ǎ��݉\�̊�
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), wkfp) != NULL) {
            i = 0; // �g�[�N���J�E���^������
            memset(token, 0x00, sizeof(token)); // �g�[�N���o�b�t�@������
            tok = GetToken(buf, ","); // ","�ō��ڂ���؂�
            while (tok) { // �g�[�N�������݂����
                sprintf(token[i++], "%-.99s", tok); // �g�[�N�����o�b�t�@�ɓ]��
                tok = GetToken(NULL, ","); // ���̃g�[�N��������

                if (i > 100) {
                    break;
                }
            }
            // �ް���
            s_Yobi_Text[0] = token[0];
            // �\���i�ް� 1��� ( 'PLTE','GSKT','ADHS','THRM'... )
            s_Yobi_Text[1] = token[1];
            // �\���i�ް� 2��� ( ���ݸ,'D','E','1','2'... )
            s_Yobi_Text[2] = token[8]; // �\���i�ް� �\����( 10, 1, 5, 2, 8, ... )

            // 1�䕪���ʂ̏W�v

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

    // �Y���\���i�ް������݂��Ȃ������ꍇ
    if (P_RecNo == 0)
        P_Yobisu = 0;

    // �\���i�ް� ̧�ٸ۰��
    fclose(wkfp);
    return true;
}

// ---------------------------------------------------------------------------------------
// ���{��֐����F �ڰѕ��i�ް��@��������
// �T  �v      �F
// ��  ��      �F Seek_Result:�ڰ�ɽ�ٕ��i�\��Ͻ���������(true:����,false:���݂��Ȃ�)
// �߂�l      �F �������� [true:�ް��L�� false:�ް�����]
// ��  �l      �F
// ---------------------------------------------------------------------------------------
bool __fastcall TFrameDetails::Write_FrameBuhin_Data(bool Seek_Result) {
    AnsiString s_Text;
    // 2009.02.25 WX-50,CX-10D�x�[�X�v���[�g�ǉ�
    AnsiString s_Text2;
    // *****************************************
    // 2012.01.20 �����ގ�
    AnsiString s_ChinaM;
    int i_RecNo;
    // *******************
    AnsiString wk_FilePass;
    AnsiString sPltMat;
    AnsiString sS1NoMat;
    AnsiString sS2NoMat;
    AnsiString sE1NoMat;
    AnsiString sBltMat;
    // 2022.01.14 �X�y�[�T�ǉ�_S
    AnsiString sAGasMat;
    AnsiString sBGasMat;
    // 2022.01.14 �X�y�[�T�ǉ�_E
    bool bSUS;
    bool bS1SUS;
    bool bS2SUS;

    long l_Daisu;

    // ------------------------------------------
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    AnsiString sSpecCode;
    // ****************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // ****************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    // ------------------------------------------

    // ****************************
    // ***  �ϐ��Z�b�g
    // ****************************
    // ���i����
    P_BBunrui = "FRAME";
    // ���ʎq
    P_Sikibetu = P_FRAMECD[2];

    // �ގ����̏�����
    P_ZaisituNM = "";
    P_EZaisituNM = "";

    // 2014/08/22 ���p�Z�p�ҏW
    // ���i�^��
    if (Seek_Result == true && P_FRAMECD[1] != "75") {
        P_BKatasiki = Get_BuhinKatasiki(P_FRAMECD[1]);
    }
    else {
        P_BKatasiki = "";
    }

    // ���i����
    s_Text = P_FRAMECD[1];
    // 2014/08/22 ���p�Z�p�ҏW
    if (s_Text == "18" || s_Text == "23" || s_Text == "24" || s_Text == "25" ||
        s_Text == "26" || s_Text == "27" || s_Text == "28" || s_Text == "75") {
        P_BMeisyou = "";
    }
    else {
        P_BMeisyou = P_FRAMECD[4];
    }

    // �}�ԁC��}�p�}��
    // 2003.11.05 �����敪�֌W�����������ɕύX
    if (Seek_Result == true) {
        // 2006.01.10 �����敪�ɂ��}�Ԃ̕ύX�ɉ����޲���ް(15)��ǉ�
        // if (P_FRAMECD[1] == "09" || P_FRAMECD[1] == "14" || P_FRAMECD[1] == "16"){
        if (P_FRAMECD[1] == "09" || P_FRAMECD[1] == "14" ||
            P_FRAMECD[1] == "15" || P_FRAMECD[1] == "16") {
            // ***********************************************************
            bSUS = false;
            // 2003.11.06�@�v���[�g�ގ��f�[�^�ʒu�ύX
            // sPltMat = P_HEADS_DATA[160].TrimRight();
            sPltMat = P_HEADS_DATA[43].TrimRight();
            // **************************************
            // 2003.12.05 ASME PLATE
            // if ((P_SUS != 0 ) && (sPltMat != "1014" ) && ( sPltMat != "1015" ) &&
            // ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]�j
            // 2016.11.07 �ގ����ޕύX
            //if ((P_SUS != 0) && (sPltMat != "1014") && (sPltMat != "1015") &&
            //    (sPltMat != "1032") && (sPltMat != "1033") &&
            //    (sPltMat != "1057") && (sPltMat != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
            //    (sPltMat != "1075") && (sPltMat != "1089") &&
            //    (sPltMat != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11�ǉ�
            //    (sPltMat != "1076") &&
            //    // ***************************
            //    (sPltMat != "1085") && (sPltMat != "1095")) {
            //    // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
            //    // *********************
            //    bSUS = true;
            //}
            // 2019.05.09 t0.8��ڰĔ��f�ǉ�_S
            //if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
            //    // �v���[�g�ގ����`�^���n�ȊO
            //    bSUS = true;
            //}
            if ((P_SUS == 1) && (Type_MAT(sPltMat) != "Ti")) {
                // �v���[�g�ގ����`�^���n�ȊO
                bSUS = true;
            }
            if ((P_SUS == 2) && (P_HEADS_DATA[1013].TrimRight() == "08")) {
                // �v���[�g�������O�D�W
                bSUS = true;
            }
            // 2019.05.09 t0.8��ڰĔ��f�ǉ�_E
            // ***********************
            // 2022.04.08 UX-30L�ǉ�_S
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
            // 2022.04.08 UX-30L�ǉ�_E
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
            // 2003.12.12 �x�w�|�W�O�t���[�����ǉ�
        }
        else if (P_FRAMECD[1] == "06") { // �r�t���[������
            bS1SUS = false;
            bS2SUS = false;
            sS1NoMat = P_HEADS_DATA[894].TrimRight();
            sS2NoMat = P_HEADS_DATA[909].TrimRight();
            // 2016.11.07 �ގ����ޕύX
            //if ((sS1NoMat != "1014") && (sS1NoMat != "1015") &&
            //    (sS1NoMat != "1032") && (sS1NoMat != "1033") &&
            //    (sS1NoMat != "1057") && (sS1NoMat != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
            //    (sS1NoMat != "1075") && (sS1NoMat != "1089") &&
            //    (sS1NoMat != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11�ǉ�
            //    (sS1NoMat != "1076") &&
            //    // ***************************
            //    (sS1NoMat != "1085") && (sS1NoMat != "1095")) {
            //    // �r�P�m�Y���ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
            //    bS1SUS = true;
            //}
            //if ((sS2NoMat != "1014") && (sS2NoMat != "1015") &&
            //    (sS2NoMat != "1032") && (sS2NoMat != "1033") &&
            //    (sS2NoMat != "1057") && (sS2NoMat != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
            //    (sS2NoMat != "1075") && (sS2NoMat != "1089") &&
            //    (sS2NoMat != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11�ǉ�
            //    (sS2NoMat != "1076") &&
            //    // ***************************
            //    (sS2NoMat != "1085") && (sS2NoMat != "1095")) {
            //    // �r�P�m�Y���ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
            //    bS2SUS = true;
            //}
            if (Type_MAT(sS1NoMat) != "Ti") {
                // �r�P�m�Y���ގ����`�^���n�ȊO
                bS1SUS = true;
            }
            if (Type_MAT(sS2NoMat) != "Ti") {
                // �r�Q�m�Y���ގ����`�^���n�ȊO
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
            P_ZaisituNM = Search_HD_MATERIAL(sS1NoMat) + "�^" +
                Search_HD_MATERIAL(sS2NoMat);
            P_EZaisituNM = Search_HD_E_MATERIAL(sS1NoMat) + "�^" +
                Search_HD_E_MATERIAL(sS2NoMat);
        }
        else if (P_FRAMECD[1] == "13") { // �d�t���[������
            //2019.11.12 �}�Ԓǉ�_S
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            //2019.11.12 �}�Ԓǉ�_E
            sE1NoMat = P_HEADS_DATA[954].TrimRight();
            P_ZaisituNM = Search_HD_MATERIAL(sE1NoMat);
            P_EZaisituNM = Search_HD_E_MATERIAL(sE1NoMat);
        }
        //2015/03/19 �A�L�������[�^�̏ꍇ ADD_S
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
        //2015/03/19 �A�L�������[�^�̏ꍇ ADD_E
        //2018/10/28 �r�t�r�{���g�}�Ԓǉ�_S
        else if (P_FRAMECD[1] == "18" || P_FRAMECD[1] == "23" ||
            P_FRAMECD[1] == "75") { // ���t�{���g

            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];

            sBltMat = P_HEADS_DATA[1041].TrimRight(); // �ގ��w��
            if (sBltMat == "1001" || sBltMat == "30C9" ||
                sBltMat == "31C9") { // ���t�{���g�ގ��w��r�t�r
                if (P_DNO[2] != "") {
                    P_Zuban = P_DNO[2];
                }
                if (P_WKDNO[2] != "") {
                    P_SZuban = P_WKDNO[2];
                }
            }
            else if (sBltMat == "" || sBltMat == "0000") { // �ގ��w�薳��
                sBltMat = P_HEADS_DATA[1021].TrimRight(); // �a�n�k�Q�Q
                if (sBltMat == "SUS304" || sBltMat == "SS304") {
                    // ���t�{���g�a�n�k�Q�Q �r�t�r
                    if (P_DNO[2] != "") {
                        P_Zuban = P_DNO[2];
                    }
                    if (P_WKDNO[2] != "") {
                        P_SZuban = P_WKDNO[2];
                    }
                }
            }
        }
        //2019/10/28 �r�t�r�{���g�}�Ԓǉ�_E
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

    // �P�䕪����
    if (Seek_Result == true) {
        if (P_FRAMECD[1] == "09" || P_FRAMECD[1] == "14" ||
            P_FRAMECD[1] == "15" || P_FRAMECD[1] == "16" ||
            P_FRAMECD[1] == "17" || P_FRAMECD[1] == "22") {
            P_1DaiSuryo = 1;
            // 2006.01.10 �}�Ԃ�"�d�O�O�O�O�O�O"��������"E000000"�̏ꍇ���ʂ��O�ɂ���
            if (P_Zuban == "�d�O�O�O�O�O�O" || P_Zuban == "E000000") {
                P_1DaiSuryo = 0;
            }
            // **********************************************************************
        }
        else if (P_FRAMECD[1] == "18") { // ���t�{���g
            P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0);
        }
        // 2014/08/22 ���p�Z�p�ҏW
        else if (P_FRAMECD[1] == "75") { // ���t�{���g(���a)
            P_1DaiSuryo = P_HEADS_DATA[1113].ToIntDef(0);
        }
        else if (P_FRAMECD[1] == "19") { // ���~��
            // 2017.06.23 LX-90D�ǉ�
            //P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
            //    P_HEADS_DATA[1287].ToIntDef(0);
            P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                P_HEADS_DATA[1287].ToIntDef(0) +
                P_HEADS_DATA[1113].ToIntDef(0);
            // *********************
        }
        else if (P_FRAMECD[1] == "20") { // �X���X�g���b�V��
            s_Text = G_KS_Syurui + "�|" + G_KS_Model;
            // 2011.02.22 �׽�ܯ�����ʏ������ύX(UX-90H21)
            // 2009.06.19 �׽�ܯ�����ʏ������ύX(UX-100R�ǉ�)
            // 2008.05.21 �׽�ܯ�����ʏ������ύX(RX-100�ǉ�)
            // 2006.02.06 �׽�ܯ�����ʏ������ύX(SX-90M,L�ǉ�)
            // 2006.01.10 �׽�ܯ�����ʏ������ύX(RX-90�ǉ�)
            // if (s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" || s_Text == "�q�w�|�V�O"){
            // if (s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O"){
            // if (s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�X�O�l" ||
            // s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O"){
            // if (s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�X�O�l" ||
            // s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O" ||
            // s_Text == "�q�w�|�P�O�O"){
            // if (s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�X�O�l" ||
            // s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O" ||
            // s_Text == "�q�w�|�P�O�O" || s_Text == "�t�w�|�P�O�O�q"){
            if (s_Text == "�t�w�|�X�O") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER��200207(���ڰ�)
                    P_1DaiSuryo = 4;
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER��300207(���ڰ�)
                    P_1DaiSuryo = 4;
                }
                else {
                    // H21
                    P_1DaiSuryo = 8;
                }
            }
            // 2016.07.25 �׽�ܯ�����ʏ������ύX
            //else if (s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�X�O�l" ||
            //    s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O" ||
            //    s_Text == "�q�w�|�P�O�O" || s_Text == "�t�w�|�P�O�O�q") {
                // ********************************************
                // ***********************************************
                // *********************************************
                // **********************************************
                // *******************************************
            //    P_1DaiSuryo = 4;
            //}
            else if (s_Text == "�t�w�|�W�O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "�t�w�|�P�O�O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "�t�w�|�P�O�O�q") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "�t�w�|�P�P�O�q") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "�t�w�|�P�R�O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "�t�w�|�P�R�O�q") {
                P_1DaiSuryo = 4;
            }
            // 2017.06.23 LX-90D�ǉ�
               else if (s_Text == "�k�w�|�X�O�c") {
                P_1DaiSuryo = 8;
            }
            // *********************
            else if (s_Text == "�r�w�|�X�O�l") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "�r�w�|�X�O�k") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "�q�w�|�X�O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "�q�w�|�P�O�O") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "�v�w�|�X�O") {
                P_1DaiSuryo = 4;
            // *********************************
                // 2012.08.23 �׽�ܯ�����ʏ������ύX(SX-80S)
            }
            else if (s_Text == "�r�w�|�W�O�r") {
                P_1DaiSuryo = 8;
                // *****************************************
            }
            // 2016.07.25 �׽�ܯ�����ʏ������ύX
            else if (s_Text == "�r�w�|�W�O�l") {
                P_1DaiSuryo = 8;
            }
            else if (s_Text == "�r�w�|�W�O�k") {
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
        else if (P_FRAMECD[1] == "21") { // �V���[�g�{���g���b�V��
            s_Text = G_KS_Syurui + "�|" + G_KS_Model;
            // 2011.02.22 �׽�ܯ�����ʏ������ύX(UX-90H21)
            // 2009.06.19 ��������ܯ�����ʏ������ύX(UX-100R�ǉ�)
            // 2008.05.21 ��������ܯ�����ʏ������ύX(RX-100�ǉ�)
            // 2006.02.06 ��������ܯ�����ʏ������ύX(SX-90M,L�ǉ�)
            // 2006.01.10 ��������ܯ�����ʏ������ύX(RX-90�ǉ�)
            // if (s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" || s_Text == "�q�w�|�V�O"){
            // if (s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�X�O�l" ||
            // s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O"){
            // if (s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�X�O�l" ||
            // s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O" ||
            // s_Text == "�q�w�|�P�O�O"){
            // if (s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�X�O�l" ||
            // s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O" ||
            // s_Text == "�q�w�|�P�O�O" || s_Text == "�t�w�|�P�O�O�q"){
            if (s_Text == "�t�w�|�X�O") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER��200207(���ڰ�)
                    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                        P_HEADS_DATA[1287].ToIntDef(0) - 4;
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER��300207(���ڰ�)
                    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                        P_HEADS_DATA[1287].ToIntDef(0) - 4;
                }
                else {
                    // H21
                    // 2012.02.13 ܯ�����ʏ������ύX(UX-90H21)
                    // if ((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                    // (P_HEADS_DATA[966].ToIntDef(0) != 0) ||
                    // (P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                    // (P_HEADS_DATA[996].ToIntDef(0) != 0)) {
                    // // Eɽ�ق���
                    // P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) + P_HEADS_DATA[1287].ToIntDef(0) - 4;
                    // } else {
                    // // Eɽ�قȂ�
                    // P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) + P_HEADS_DATA[1287].ToIntDef(0) - 8;
                    // }
                    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                        P_HEADS_DATA[1287].ToIntDef(0) - 8;
                    // ***************************************
                }
            }
            // 2016.07.25 ��������ܯ�����ʏ������ύX
            //else if (s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�X�O�l" ||
            //    s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O" ||
            //    s_Text == "�q�w�|�P�O�O" || s_Text == "�t�w�|�P�O�O�q") {
            //    // ************************************************
            //    // ***************************************************
            //    // *************************************************
            //    // **************************************************
            //    // *******************************************
            //    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
            //        P_HEADS_DATA[1287].ToIntDef(0) - 4;
            else if (s_Text == "�t�w�|�W�O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "�t�w�|�P�O�O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "�t�w�|�P�O�O�q") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "�t�w�|�P�P�O�q") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "�t�w�|�P�R�O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "�t�w�|�P�R�O�q") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            // 2017.06.23 LX-90D�ǉ�
            else if (s_Text == "�k�w�|�X�O�c") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) +
                    P_HEADS_DATA[1113].ToIntDef(0) - 8;
            }
            // *********************
            else if (s_Text == "�r�w�|�X�O�l") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "�r�w�|�X�O�k") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "�q�w�|�X�O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "�q�w�|�P�O�O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "�v�w�|�X�O") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            // *************************************
                // 2012.08.23 ��������ܯ�����ʏ������ύX(SX-80S)
            }
            else if (s_Text == "�r�w�|�W�O�r") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 8;
                // *********************************************
            }
            // 2016.07.25 ��������ܯ�����ʏ������ύX
            else if (s_Text == "�r�w�|�W�O�l") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 8;
            }
            else if (s_Text == "�r�w�|�W�O�k") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 8;
            }
            // *************************************
            else {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0);
            }
            // 2009.02.16 ܯ���ǉ�
        }
        else if (P_FRAMECD[1] == "52") { // ���~��
            P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                P_HEADS_DATA[1287].ToIntDef(0);
            // *******************
            // 2012.02.13 ܯ�����ʏ������ύX(UX-90H21)
        }
        else if (P_FRAMECD[1] == "53") { // �㉺�[���{���g�p���b�V��
            s_Text = G_KS_Syurui + "�|" + G_KS_Model;
            if (s_Text == "�t�w�|�X�O") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER��200207(���ڰ�)
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER��300207(���ڰ�)
                }
                else {
                    // H21
                    if ((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                        (P_HEADS_DATA[966].ToIntDef(0) != 0) ||
                        (P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                        (P_HEADS_DATA[996].ToIntDef(0) != 0)) {
                        // Eɽ�ق���
                        P_1DaiSuryo = 4;
                    }
                    else {
                        // Eɽ�قȂ�
                        P_1DaiSuryo = 0;
                    }
                }
                // 2012.08.23 �㉺�[�����ėpܯ�����ʏ������ύX(SX-80S)
            }
            else if (s_Text == "�r�w�|�W�O�r") {
                P_1DaiSuryo = 2;
                // ***************************************************
            }
            // ***************************************
            // 2017.06.23 LX-90D,SX-80LM�ǉ�
            else if (s_Text == "�k�w�|�X�O�c") {
                P_1DaiSuryo = 2;
            }
            else if (s_Text == "�r�w�|�W�O�k") {
                P_1DaiSuryo = 2;
            }
            else if (s_Text == "�r�w�|�W�O�l") {
                P_1DaiSuryo = 2;
            }
            // *****************************
        }
        else if (P_FRAMECD[1] == "48") {
            // 2003.12.24 �x�w�|�W�O�t���[�����ǉ�
        }
        else if (P_FRAMECD[1] == "06") { // �r�t���[������
            P_1DaiSuryo = 1;
        }
        else if (P_FRAMECD[1] == "13") { // �d�t���[������
            sE1NoMat = P_HEADS_DATA[954].TrimRight();
            // 2016.11.07 �ގ����ޕύX
            //if (sE1NoMat == "1001" || sE1NoMat == "1003")
            if (Type_MAT(sE1NoMat) == "SUS304" || Type_MAT(sE1NoMat) == "SUS316")
            // ***********************
            { // �d�P�m�Y���ގ��� SUS304[1001],SUS316[1003]�j
                P_1DaiSuryo = 1;
            }
            else {
                P_1DaiSuryo = 0;
            }
            // *************************************
            // 2004.03.16 �q�w�|�O�O�x�[�X�v���[�g�ǉ�
        }
        else if (P_FRAMECD[1] == "49") { // �x�[�X�v���[�g
            P_1DaiSuryo = 1;
            // ***************************************
        }
        // 2021.10.25 ��߰���ǉ�_S
        else if (P_FRAMECD[1] == "79") { // �X�y�[�T�[
            s_Text = G_KS_Syurui + "�|" + G_KS_Model;
            // 2022.01.24 �X�y�[�T�ǉ�_S
            // �r�t���[���X�y�[�T
            sAGasMat = P_HEADS_DATA[836].TrimRight();
            sBGasMat = P_HEADS_DATA[835].TrimRight();
            P_1DaiSuryo = 0;
            //if (s_Text == "�v�g�|�R�O") {
            //    if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
            //        (P_HEADS_DATA[966].ToIntDef(0) == 0) &&
            //        (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
            //        (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
            //        // Eɽ�ٖ����ꍇ
            //        P_1DaiSuryo = 1;
            //    }
            //    else {
            //        P_1DaiSuryo = 2;
            //        // Eɽ�ٗL��ꍇ
            //    }
            //}
            //else {
            //    P_1DaiSuryo = 0;
            //}
            if (s_Text == "�t�w�|�X�O") {
                // UX-90
                if (sAGasMat == "2067" || sBGasMat == "2067" ) {
                    // GZ-NBR
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "�k�w�|�P�O") {
                // LX-10
                if (sAGasMat == "2063" || sBGasMat == "2063" ||
                    sAGasMat == "2064" || sBGasMat == "2064" ) {
                    // TCG
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "�v�g�|�R�O") {
                // WH-30
                P_1DaiSuryo = 1;
            }
            //2023.03.20 CX-03-Y�C��_S
            ////2023.03.09 CX-03-Y�ǉ�_S
            //else if (((s_Text == "�b�w�|�O�R") || (s_Text == "�b�w�|�O�R�c") ||
            //          (s_Text == "�b�w�v�|�O�R")) &&
            //          (P_HEADS_DATA[36].Pos("Y") != 0)) {
            //    P_1DaiSuryo = 2;
            //}
            ////2023.03.09 CX-03-Y�ǉ�_E
            //2023.03.20 CX-03-Y�C��_E

            // *************************
            // ***  �E���d�l�̍쐬
            // *************************
            if (P_1DaiSuryo == 1) {
                P_Siyou1 = Get_AnaakeSiyou_S("S");
                G_Log_Text = "�r�t���[���X�y�[�T�[�E���d�l  �w" + P_Siyou1 + "�x���擾�B";
                Write_Log(G_Log_Text);
            }
            // 2022.01.24 �X�y�[�T�ǉ�_E
        }
        else if (P_FRAMECD[1] == "80") { // �V���v���[�g�[
            s_Text = G_KS_Syurui + "�|" + G_KS_Model;
            if (s_Text == "�v�g�|�R�O") {
                P_1DaiSuryo = 4;
            }
            else {
                P_1DaiSuryo = 0;
            }
        }
        // 2021.10.25 ��߰���ǉ�_E

        // 2022.01.24 �X�y�[�T�ǉ�_S
        // 2022.xx.xx E�X�y�[�T�[CODE�ύX_S
        //else if (P_FRAMECD[1] == "81") { // �d�t���[���X�y�[�T�[
        else if (P_FRAMECD[1] == "82") { // �d�t���[���X�y�[�T�[
        // 2022.xx.xx E�X�y�[�T�[CODE�ύX_E
            s_Text = G_KS_Syurui + "�|" + G_KS_Model;
            sAGasMat = P_HEADS_DATA[836].TrimRight();
            sBGasMat = P_HEADS_DATA[835].TrimRight();
            P_1DaiSuryo = 0;
            if (s_Text == "�t�w�|�X�O") {
                // UX-90
                if (sAGasMat == "2067" || sBGasMat == "2067" ) {
                    // GZ-NBR
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "�k�w�|�P�O") {
                // LX-10
                if (sAGasMat == "2063" || sBGasMat == "2063" ||
                    sAGasMat == "2064" || sBGasMat == "2064" ) {
                    // TCG
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "�v�g�|�R�O") {
                // WH-30
                P_1DaiSuryo = 1;
            }

            // *************************
            // ***  �E���d�l�̍쐬
            // *************************
            if (P_1DaiSuryo == 1) {
                P_Siyou1 = Get_AnaakeSiyou_S("E");
                G_Log_Text = "�d�t���[���X�y�[�T�[�E���d�l  �w" + P_Siyou1 + "�x���擾�B";
                Write_Log(G_Log_Text);
            }
            if ( P_Siyou1  == "E0000") {
                // �m�Y�����Ȃ��ꍇ�d�X�y�[�T�[�s�v
                P_1DaiSuryo = 0;
            }
        }
        // 2022.01.24 �X�y�[�T�ǉ�_E
    }
    else {
        P_1DaiSuryo = 0;
    }

    // ���v
    l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // �䐔
    P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;

    // �d�l�P
    // HanToZen(s_JPKbn);
    if (Seek_Result == true) {

        s_Text = P_FRAMECD[1];

        // 2014/08/22 ���p�Z�p�ҏW
        if (s_Text == "18" || s_Text == "23" || s_Text == "75") { // ���t�{���g

            // �Q�Ƃ���f�[�^�C���f�b�N�X
            int i_BoltRadiusNo = 1019;  // �{���g�a
            int i_BoltLengthNo = 1376;  // �{���g�S��

            if(s_Text == "75") {
                // BNWRN-,���tBNW�̏ꍇ�A�Q�ƃf�[�^�C���f�b�N�X��ύX
                i_BoltRadiusNo = 1108;
                i_BoltLengthNo = 1377;

                // 2014/09/04 ���p�Z�p�ҏW
                if(StrToDblDef(P_HEADS_DATA[i_BoltLengthNo].TrimRight(), 0.0) <= 0.0) {
                    // S�{���g�S����0mm(���ݒ�)�̏ꍇ�AL�{���g�S�����̗p����
                    i_BoltLengthNo = 1376;
                }
            }

            // �ގ����胍�W�b�N
            sBltMat = P_HEADS_DATA[1041].TrimRight(); // �ގ��w��
            if (sBltMat == "1001") {
                P_ZaisituNM = "SUS304/C3604";
                P_EZaisituNM = "SUS304/C3604";
                // 2011.05.30 ���čގ��w��ǉ�
            }
            // 2016.09.01 �ގ����ޕύX
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
            // 2016.11.07 �ގ����ޕύX
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
            // 2016.09.01 �ގ����ޕύX
            else if (sBltMat == "32C2") {
                P_ZaisituNM = "SA193B7/SA194-2H";
                P_EZaisituNM = "SA193B7/SA194-2H";
            // **********************
            }
            // 2015.01.09 ���čގ��w��ǉ�
            else if (sBltMat == "1169") {
                P_ZaisituNM = "A193B7/A194-2H";
                P_EZaisituNM = "A193-B7/A194-2H";
            }
            else if (sBltMat == "1171") {
                P_ZaisituNM = "A193B7/A194-2H";
                P_EZaisituNM = "A193-B7/A194-2H";
            }
            // 2016.09.01 �ގ����ޕύX
            else if (sBltMat == "33C2") {
                P_ZaisituNM = "A193B7/A194-2H";
                P_EZaisituNM = "A193-B7/A194-2H";
            // **********************
            }
            // ***************************
            // 2016.05.10 ���čގ��w��ǉ�
            else if (sBltMat == "1186") {
                // 2016.11.07 �ގ����ޕύX
                //P_ZaisituNM = "SNB7/S45C eqv.";
                P_ZaisituNM = "SNB7/S45C����";
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
                // 2014/08/26 ���p�Z�p�ҏW
                else if (sBltMat == "45/35") {
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
                else if (sBltMat == "35CrMo") {
                    P_ZaisituNM = "35CrMo/32CrMo";
                    P_EZaisituNM = "35CrMo/32CrMo";
                }
                // 2021.09.22 RX-70Z�ǉ�_S
                else if (sBltMat == "35CrMo2") {
                    P_ZaisituNM = "35CrMo/45";
                    P_EZaisituNM = "35CrMo/45";
                }
                // 2021.09.22 RX-70Z�ǉ�_E
                // 2015.01.09 ���čގ��w��ǉ�
                else if (sBltMat == "SA193-B7") {
                    P_ZaisituNM = "SA193B7/SA194-2H";
                    P_EZaisituNM = "SA193B7/SA194-2H";
                }
                else if (sBltMat == "A193-B7") {
                    P_ZaisituNM = "A193-B7/A194-2H";
                    P_EZaisituNM = "A193-B7/A194-2H";
                }
                // ***************************
                // 2016.11.07 �ގ����ޕύX
                else if (sBltMat == "C.S.") {
                    P_ZaisituNM = "SS400/SS400";
                    P_EZaisituNM = "SS400/SS400";
                }
                else if (sBltMat == "SNB7 eqv.") {
                    P_ZaisituNM = "SNB7/S45C����";
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
                // 2018.06.19 LX-30Z�ǉ�_S
                else if (sBltMat == "Q235B") {
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
                else if (sBltMat == "Q235B2") {
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
                // 2018.06.19 LX-30Z�ǉ�_E
                // 2020.05.27 LX-50Z�ǉ�_S
                else if (sBltMat == "45") {
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
                else if (sBltMat == "45_2") {
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
                // 2020.05.27 LX-50Z�ǉ�_E
                // 2021.09.22 RX-50Z,70Z�ǉ�_S
                else if (sBltMat == "42CrMo") {
                    P_ZaisituNM = "42CrMo/45";
                    P_EZaisituNM = "42CrMo/45";
                }
                else if (sBltMat == "42CrMo2") {
                    P_ZaisituNM = "42CrMo/45";
                    P_EZaisituNM = "42CrMo/45";
                }
                // 2021.09.22 RX-50Z,70Z�ǉ�_E
                else {
                    P_ZaisituNM = "";
                    P_EZaisituNM = "";
                    sBltMat = "0";
                }
            }

            // 2019.11.21 LX-90D-NUP�ގ��ύX_S
            //// 2017.06.23 LX-90D-NUP�ǉ�
            //s_Text = G_KS_Syurui + "�|" + G_KS_Model;
            //if( P_FRAMECD[1] == "18" ) {
            //    if (( s_Text == "�k�w�|�X�O�c") && ( P_HEADS_DATA[37].ToIntDef(0)) == 94 ){
            //        // LX-90D-KNUP
            //        P_ZaisituNM = "SA540B23/SA194-2H";
            //        P_EZaisituNM = "SA540B23/SA194-2H";
            //    } else if (( s_Text == "�k�w�|�X�O") && ( P_HEADS_DATA[37].ToIntDef(0)) == 394 ){
            //        // LX-90D-TNUP
            //        P_ZaisituNM = "SA540B23/SA194-2H";
            //        P_EZaisituNM = "SA540B23/SA194-2H";
            //    }
            //}
            //// *************************
            // 2019.11.21 LX-90D-NUP�ގ��ύX_E

            // 2020.05.27 LX-50Z�ǉ�_E
            s_Text = G_KS_Syurui + "�|" + G_KS_Model;
            // 2020.05.27 LX-50Z�ǉ�_E

            // 2018.06.19 LX-30Z�ǉ�_S
            if (sBltMat == "Q235B2") {
                if (( s_Text == "�k�w�|�R�O" ) && ( P_FRAMECD[1] == "75" )){
                    // LX-30Z ������ 45/35
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
            }
            // 2018.06.19 LX-30Z�ǉ�_E

            // 2020.05.27 LX-50Z�ǉ�_S
            if ( sBltMat == "45_2" ) {
                if (( s_Text == "�k�w�|�T�O" ) && ( P_FRAMECD[1] == "75" )){
                    // LX-50Z ������ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }
            // 2020.05.27 LX-50Z�ǉ�_E

            // 2021.09.22 RX-50Z,70Z�ǉ�_S
            if ( sBltMat == "45_2" ) {
                if (( s_Text == "�q�w�|�T�O" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-50Z ������ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                } else if (( s_Text == "�q�w�|�V�O" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-70Z ������ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }

            if ( sBltMat == "42CrMo2" ) {
                if (( s_Text == "�q�w�|�T�O" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-50Z ������ 45/35
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
            }

            if ( sBltMat == "35CrMo2" ) {
                if (( s_Text == "�q�w�|�V�O" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-70Z ������ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }
            // 2021.09.22 RX-50Z,70Z�ǉ�_E

            // 2012.01.20 �����ގ�
            i_RecNo = 1107;
            s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

            s_Text = G_KS_Syurui + "�|" + G_KS_Model;
            if (s_Text == "�t�w�|�P�O") { // �����ގ�
            }
            else if (s_Text == "�k�w�|�O�O") { // �����ގ�
            }
            else if (s_Text == "�k�w�|�P�O") { // �����ގ�
            }
            else if (s_Text == "�k�w�|�R�O") { // �����ގ�
            }
            else if (s_Text == "�k�w�|�T�O") { // �����ގ�
            }
            else if (s_Text == "�r�w�|�Q�O") { // �����ގ�
            }
            else if (s_Text == "�r�w�|�S�P") { // �����ގ�
            }
            else if (s_Text == "�r�w�|�S�R") { // �����ގ�
            }
            else if (s_Text == "�q�w�|�P�P") { // �����ގ�
            }
            else if (s_Text == "�q�w�|�P�R") { // �����ގ�
            }
            else if (s_Text == "�q�w�|�R�O") { // �����ގ�
            }
            else if (s_Text == "�q�w�|�T�O") { // �����ގ�
            }
            else if (s_Text == "�q�w�|�V�O") { // �����ގ�
            }
            else {
                s_ChinaM = "";
            }

            if (s_ChinaM == "CHINA") { // �����ގ�
                if (P_ZaisituNM == "SS400/SS400") {
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }
            // *******************

            s_Text = P_HEADS_DATA[i_BoltRadiusNo].TrimRight();
            if (s_Text == "1.630") {
                //2020.10.02 �{���g���@�ύX_S
                //P_Siyou1 =
                //    "�P�@�T�^�W�|�W�t�m�~" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight())
                //    + "�k�~�S�r";
                P_Siyou1 =
                    "1 5/8-8UN�~" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight())
                    + "�k�~�S�r";
                //2020.10.02 �{���g���@�ύX_E

            }
            else if (s_Text == "2.000") {
                P_Siyou1 = "�Q�|�W�t�m�~" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight())
                    + "�k�~�S�r";

            }
            else {
                P_Siyou1 = "�l" + HanToZen(P_HEADS_DATA[i_BoltRadiusNo].SubString(1, 2)) +
                    "�~" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight()) + "�k�~�S�r";

            }
            P_Yobisu = Get_Blt_YobiSu(StrToDblDef(s_Text.TrimRight(), 0.0),
                StrToDblDef(P_HEADS_DATA[i_BoltLengthNo].TrimRight(), 0.0), sBltMat,
                m_pcHeadsData);
            P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;
        }
        else if (s_Text == "26") {
            P_Siyou1 = HanToZen(P_HEADS_DATA[1092].TrimRight()) + "�k";

        }
        else if (s_Text == "27") {
            P_Siyou1 = HanToZen(P_HEADS_DATA[1096].TrimRight()) + "�k�@�L���b�v�t��";

        }
        else if (s_Text == "28") {
            P_Siyou1 = HanToZen(P_HEADS_DATA[1097].TrimRight()) + "�k�@�L���b�v�t��";

            // 2012.02.13 ܯ�����ʏ������ύX(UX-90H21)
            //// 2010.04.19 UX-90��������ܯ���ύX
            // } else if (s_Text == "21" ) {
        }
        else if (s_Text == "53") {
            // �㉺�[���{���g�p���b�V��
            // ***************************************
            s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
            if (s_Text2 == "�t�w�|�X�O") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER��200207(���ڰ�)
                    P_Siyou1 = "";
                    // 2010.12.14 UX-90���Ƃ�����GB�ǉ�
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER��300207(���ڰї�������GB)
                    // ********************************
                }
                else {
                    if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                        // S1�p�躰��
                        s_Text2 = P_HEADS_DATA[893].TrimRight();
                    }
                    else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                        // S2�p�躰��
                        s_Text2 = P_HEADS_DATA[908].TrimRight();
                    }
                    else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                        // S3�p�躰��
                        s_Text2 = P_HEADS_DATA[923].TrimRight();
                    }
                    else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                        // S4�p�躰��
                        s_Text2 = P_HEADS_DATA[938].TrimRight();
                    }
                    else if ((P_HEADS_DATA[951].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[951].ToIntDef(0) == 2)) {
                        // E1�p�躰��
                        s_Text2 = P_HEADS_DATA[953].TrimRight();
                    }
                    else if ((P_HEADS_DATA[966].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[966].ToIntDef(0) == 2)) {
                        // E2�p�躰��
                        s_Text2 = P_HEADS_DATA[968].TrimRight();
                    }
                    else if ((P_HEADS_DATA[981].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[981].ToIntDef(0) == 2)) {
                        // E3�p�躰��
                        s_Text2 = P_HEADS_DATA[983].TrimRight();
                    }
                    else if ((P_HEADS_DATA[996].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[996].ToIntDef(0) == 2)) {
                        // E4�p�躰��
                        s_Text2 = P_HEADS_DATA[998].TrimRight();
                    }
                    else {
                        // ɽ�قȂ�
                        s_Text2 = "999";
                    }

                    // 2011.06.20 ɽ�ًK�i�ύX
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
                        P_Siyou1 = "�ӂ`���V�X������z���ӂ̂��Ɓ���";
                    }
                    else if (s_Text2 == "999") {
                        // ɽ�قȂ�
                        P_Siyou1 = "�W�J�s�\";
                    }
                    else {
                        // JIS,DIN
                        P_Siyou1 = "�ӂ`���W�T";
                    }
                }
                // 2014.04.15 ܯ�����ʏ������ǉ�(UX-10-EP,LX-10,EP)
                //// 2012.06.08 ܯ�����ʏ������ύX(RX-10-EP)
                // } else if (s_Text2 == "�q�w�|�P�P" || s_Text2 == "�q�w�|�P�R") {
            }
            else if (s_Text2 == "�t�w�|�P�O" || s_Text2 == "�k�w�|�P�O" ||
                s_Text2 == "�q�w�|�P�P" || s_Text2 == "�q�w�|�P�R") {
                // ************************************************
                if ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                    (P_HEADS_DATA[996].ToIntDef(0) != 0)) {
                    // E3,E4ɽ�ٗL���
                    P_1DaiSuryo = 2;
                    P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;
                    P_Siyou4 = "��";
                }
                else {
                    // ɽ�قȂ�
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
            // 2011.03.11 RX-70��߰Ē��L�ǉ�
        }
        else if (s_Text == "16") {
            s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
            if (s_Text2 == "�q�w�|�V�O") {
                if (P_HEADS_DATA[1046].TrimRight() != "") {
                    P_Siyou1 = P_HEADS_DATA[1046].TrimRight() + "�p";
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
        // 2022.01.14 �X�y�[�T�ǉ�_S
        // 2022.08.05E�X�y�[�T�[CODE�ύX_S
        //else if (s_Text == "79"|| s_Text == "81") {
        else if (s_Text == "79"|| s_Text == "82") {
        // 2022.08.05E�X�y�[�T�[CODE�ύX_E
            // �X�y�[�T
            P_Siyou1 = "�E��" + P_Siyou1;
            //2023.03.20 CX-03-Y�C��_S
            ////2023.03.09 CX-03-Y�ǉ�_S
            //s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
            //if (((s_Text2 == "�b�w�|�O�R") || (s_Text2 == "�b�w�|�O�R�c") ||
            //     (s_Text2 == "�b�w�v�|�O�R")) &&
            //     (P_HEADS_DATA[36].Pos("Y") != 0)) {
            //    P_Siyou1 = "";
            //}
            ////2023.03.09 CX-03-Y�ǉ�_E
            //2023.03.20 CX-03-Y�C��_E
        }
        // 2022.01.14 �X�y�[�T�ǉ�_E
        else {
            P_Siyou1 = "";
        }

    }
    else {
        P_Siyou1 = "";
    }

    // �d�l�Q(S,E�ڰю��)
    if (Seek_Result == true) {
        s_Text = P_FRAMECD[1];
        if (s_Text == "09" || s_Text == "16") {
            if (P_HEADS_DATA[1290].TrimRight() != "") {
                P_Siyou2 = P_HEADS_DATA[1290].TrimRight() + "�p";
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

            // 2010.12.14 ��GB���L�ǉ�
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
        // 2014/08/22 ���p�Z�p�ҏW
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

    // �d�l�S  ( 02.08.09 �ǉ� )
    // 2014/08/22 ���p�Z�p�ҏW
    if ((s_Text == "18") || (s_Text == "23") || (s_Text == "14") ||
        (s_Text == "15") || (s_Text == "75")) {
        P_Siyou4 = "��";
    }
    // 2009.02.25 WX-50,CX-10D�x�[�X�v���[�g�ǉ�
    else if (s_Text == "49") {
        s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
        // 2009.07.31 �x�[�X�v���[�g���ʕύX
        // RX-00    �P
        // ���̑�   �P��
        // if (s_Text2 == "�v�w�|�T�R") {
        // P_Siyou4 = "��";
        // } else if  (s_Text2 == "�b�w�|�P�O�c") {
        // P_Siyou4 = "��";
        // }
        if (s_Text2 == "�q�w�|�O�O") {
            P_Siyou4 = "";
        }
        else {
            P_Siyou4 = "��";
        }
        // *********************************
        // 2010.04.19 UX-90��������ܯ���ύX
    }
    else if (s_Text == "21") {
        s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
        if (s_Text2 == "�t�w�|�X�O") {
            if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                // HEAD.VER��200207(���ڰ�)
                P_Siyou4 = "";
            }
            else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                // Eɽ�ٖ����ꍇ
                P_Siyou4 = "";
            }
            else {
                // �V�ڰт�Eɽ�ٕt��
                P_Siyou4 = "��";
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

    // �d�l�T
    P_Siyou5 = "";

    // ****************************
    // ***  S,E�ڰ��ް�  �Z�b�g
    // ****************************
    // ���i�W�J�ް��@OPEN
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

    // 1  ���i����
    s_Text = P_BBunrui;
    fputs(s_Text.c_str(), fp);
    // 2  ���ʎq
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), fp);
    // 3  ���i�^��
    s_Text = "," + P_BKatasiki;
    fputs(s_Text.c_str(), fp);
    // 4  ���i����
    s_Text = "," + P_BMeisyou;
    fputs(s_Text.c_str(), fp);
    // 5  �ގ�����
    s_Text = "," + P_ZaisituNM;
    fputs(s_Text.c_str(), fp);
    // 6  �}��
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), fp);
    // 7  ��}�p�}��
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), fp);
    // 8  1�䕪����
    s_Text = "," + AnsiString(P_1DaiSuryo);
    fputs(s_Text.c_str(), fp);
    // 9  �\����
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), fp);

    // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), fp);

    // 11 �d�l�P
    s_Text = "," + P_Siyou1;
    fputs(s_Text.c_str(), fp);
    // 12 �d�l�Q
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), fp);
    // 13 �d�l�R
    s_Text = "," + P_Siyou3;
    fputs(s_Text.c_str(), fp);
    // 14 �d�l�S
    s_Text = "," + P_Siyou4;
    fputs(s_Text.c_str(), fp);
    // 15 �d�l�T
    s_Text = "," + P_Siyou5;
    fputs(s_Text.c_str(), fp);
    // 16  �ގ�����(�p)
    s_Text = "," + P_EZaisituNM;
    fputs(s_Text.c_str(), fp);

    // ------------------------------
    // 2007.10.03 �װ�d�l�Ή�
    // 17 �d�l���ߺ���
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), fp);
    // 2007.10.03
    // ------------------------------

    // ���s����
    s_Text = "\n";
    // CSV�ް��@̧�ُo��
    fputs(s_Text.c_str(), fp);

    // �{���g���Z�L��̏���
    if (Seek_Result == true) {
        s_Text = P_FRAMECD[1];
        P_1DaiSuryo = P_HEADS_DATA[1287].ToIntDef(0);
        // ���v
        l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // �䐔
        P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;
        if ((s_Text == "18" || s_Text == "23")) {
            if ((P_1DaiSuryo > 0)) {
                s_Text = P_HEADS_DATA[1019].TrimRight(); // BDI
                if (s_Text == "1.630") {
                    //2020.10.02 �{���g���@�ύX_S
                    //P_Siyou1 =
                    //    "�P�@�T�^�W�|�W�t�m�~" + HanToZen(P_HEADS_DATA[1377].TrimRight())
                    //    + "�k�~�S�r";
                    P_Siyou1 =
                        "1 5/8-8UN�~" + HanToZen(P_HEADS_DATA[1377].TrimRight())
                        + "�k�~�S�r";
                    //2020.10.02 �{���g���@�ύX_E

                }
                else if (s_Text == "2.000") {
                    P_Siyou1 =
                        "�Q�|�W�t�m�~" + HanToZen(P_HEADS_DATA[1377].TrimRight())
                        + "�k�~�S�r";

                }
                else {
                    P_Siyou1 =
                        "�l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) +
                        "�~" + HanToZen(P_HEADS_DATA[1377].TrimRight()) + "�k�~�S�r";

                }
                P_Yobisu = Get_Blt_YobiSu(StrToDblDef(s_Text.TrimRight(), 0.0),
                    StrToDblDef(P_HEADS_DATA[1377].TrimRight(), 0.0), sBltMat,
                    m_pcHeadsData);
                P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;

                // 1  ���i����
                s_Text = P_BBunrui;
                fputs(s_Text.c_str(), fp);
                // 2  ���ʎq
                s_Text = "," + P_Sikibetu;
                fputs(s_Text.c_str(), fp);
                // 3  ���i�^��
                s_Text = ",";
                fputs(s_Text.c_str(), fp);
                // 4  ���i����
                s_Text = ",";
                fputs(s_Text.c_str(), fp);
                // 5  �ގ�����
                s_Text = "," + P_ZaisituNM;
                fputs(s_Text.c_str(), fp);
                // 6  �}��
                s_Text = "," + P_Zuban;
                fputs(s_Text.c_str(), fp);
                // 7  ��}�p�}��
                s_Text = "," + P_SZuban;
                fputs(s_Text.c_str(), fp);
                // 8  1�䕪����
                s_Text = "," + AnsiString(P_1DaiSuryo);
                fputs(s_Text.c_str(), fp);
                // 9  �\����
                s_Text = "," + AnsiString(P_Yobisu);
                fputs(s_Text.c_str(), fp);

                // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
                s_Text = "," + AnsiString(P_Soukei);
                fputs(s_Text.c_str(), fp);

                // 11 �d�l�P
                s_Text = "," + P_Siyou1;
                fputs(s_Text.c_str(), fp);
                // 12 �d�l�Q
                s_Text = "," + P_Siyou2;
                fputs(s_Text.c_str(), fp);
                // 13 �d�l�R
                s_Text = "," + P_Siyou3;
                fputs(s_Text.c_str(), fp);
                // 14 �d�l�S
                s_Text = "," + P_Siyou4;
                fputs(s_Text.c_str(), fp);
                // 15 �d�l�T
                s_Text = "," + P_Siyou5;
                fputs(s_Text.c_str(), fp);
                // 16  �ގ�����(�p)
                s_Text = "," + P_EZaisituNM;
                fputs(s_Text.c_str(), fp);

                // ------------------------------
                // 2007.10.03 �װ�d�l�Ή�
                // 17 �d�l���ߺ���
                s_Text = "," + sSpecCode;
                fputs(s_Text.c_str(), fp);
                // 2007.10.03
                // ------------------------------

                // ���s����
                s_Text = "\n";
                // CSV�ް��@̧�ُo��
                fputs(s_Text.c_str(), fp);
            }

            // 2017.06.23 LX-90D-NUP�ǉ�
            // 2014/08/22 ���p�Z�p�ҏW
            //if(P_HEADS_DATA[36].Pos("Z") == 0) {
            //    // Z�t���[���Ŗ����ꍇ�A���̃^�C�~���O�Ń{���g�J�o�[�o�͂�
            //    BLT_Cover_Out();
            //}
            s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
            if (( s_Text2 == "�k�w�|�X�O�c") && ( P_HEADS_DATA[37].ToIntDef(0)) == 94 ){
                // LX-90D-KNUP�̏ꍇ�A�ʂ̃^�C�~���O�Ń{���g�J�o�[�o�͂�
            } else if (( s_Text2 == "�k�w�|�X�O") && ( P_HEADS_DATA[37].ToIntDef(0)) == 394 ){
                // LX-90D-TNUP�̏ꍇ�A�ʂ̃^�C�~���O�Ń{���g�J�o�[�o�͂�
            } else if (P_HEADS_DATA[36].Pos("Z") == 0) {
                // Z�t���[���Ŗ����ꍇ�A���̃^�C�~���O�Ń{���g�J�o�[�o�͂�
                BLT_Cover_Out();
            }
            // *************************
        }
    }

    // 2017.06.23 LX-90D-NUP�ǉ�
    s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
    // *************************

    // 2014/08/22 ���p�Z�p�ҏW
    if(P_FRAMECD[1] == "75" &&
       P_HEADS_DATA[36].Pos("Z") > 0) {
        // �R�[�h��"75"(BNWRN-,���tBNW) ���� Z�t���[���̏ꍇ�A�{���g�J�o�[�o�͂�
        BLT_Cover_Out();
        BLT_Cover_Out_Small();
    // 2017.06.23 LX-90D-NUP�ǉ�
    } else if ( P_FRAMECD[1] == "75" &&
                s_Text2 == "�k�w�|�X�O�c" &&
                P_HEADS_DATA[37].ToIntDef(0) == 94 ){
        // LX-90D-KNUP�̏ꍇ�A�{���g�J�o�[�o�͂�
        BLT_Cover_Out();
    } else if ( P_FRAMECD[1] == "75" &&
                s_Text2 == "�k�w�|�X�O" &&
                P_HEADS_DATA[37].ToIntDef(0) == 394 ){
        // LX-90D-TNUP�̏ꍇ�A�{���g�J�o�[�o�͂�
        BLT_Cover_Out();
    // *************************
    }

    fclose(fp);

    return true;
}

// ---------------------------------------------------------------------------------------
// ���{��֐����F ���i�^���@�擾
// �T  �v      �F
// ��  ��      �F Code  :�ڰэ\�����i����ð���(FRAMECD.csv)�ະ��
// �߂�l      �F �������� �擾�������i�^��
// ��  �l      �F
// ---------------------------------------------------------------------------------------
AnsiString __fastcall TFrameDetails::Get_BuhinKatasiki(AnsiString Code) {
    AnsiString s_BKata;
    AnsiString s_JPKbn;
    int i_Pos;
    // 2019.11.22 CX-20�^�ǉ�_S
     AnsiString s_Model;
    // 2019.11.22 CX-20�^�ǉ�_E

    // 2003.09.16 ���i�^���ύX
    // if (Code == "19" || Code == "20" || Code == "21"){
    // if ( P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),1) == "R" ) {
    // s_BKata = G_KS_Syurui + "�|" + G_KS_Model  + "�|" +  HanToZen(P_HEADS_DATA[36].SubString(1,P_HEADS_DATA[36].Length()-1));
    // } else {
    // s_BKata = G_KS_Syurui + "�|" + G_KS_Model  + "�|" +  HanToZen(P_HEADS_DATA[36].TrimRight());
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

    // s_BKata = G_KS_Syurui + "�|" + G_KS_Model  + "�|" +  HanToZen(s_JPKbn);

    // } else if (Code == "18" || Code == "23" || Code == "24" || Code == "25" || Code == "26" || Code == "27" || Code == "28"){
    // s_BKata = P_FRAMECD[4];

    // } else {
    // s_BKata = G_KS_Syurui + "�|" + G_KS_Model;
    // }

    // 2019.11.22 CX-20�^�ǉ�_S
    //if (Code == "09" || Code == "14" || Code == "15" || Code == "16" ||
    //    Code == "17" || Code == "19" || Code == "20" || Code == "21") {
    //    if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
    //        s_BKata = G_KS_Syurui + "�|" + G_KS_Model + "�|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
    //    1) == "M") {
    //        s_BKata = G_KS_Syurui + "�|" + G_KS_Model + "�|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
    //        2) == "MR") {
    //        s_BKata = G_KS_Syurui + "�|" + G_KS_Model + "�|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 2));
    //    }
    //    else {
    //        s_BKata = G_KS_Syurui + "�|" + G_KS_Model + "�|" +
    //            HanToZen(P_HEADS_DATA[36].TrimRight());
    //    }
    //}
    //// 2014/08/22 ���p�Z�p�ҏW
    //else if (Code == "18" || Code == "23" || Code == "24" || Code == "25" ||
    //         Code == "26" || Code == "27" || Code == "28" || Code == "75") {
    //    s_BKata = P_FRAMECD[4];
    //}
    //else {
    //    s_BKata = G_KS_Syurui + "�|" + G_KS_Model;
    //}
    //// ***********************
    s_Model = G_KS_Model;

    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�Q�P") {
        s_Model ="�Q�O";
    }
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�Q�R") {
        s_Model ="�Q�O";
    }

    if (Code == "09" || Code == "14" || Code == "15" || Code == "16" ||
        Code == "17" || Code == "19" || Code == "20" || Code == "21") {
        if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
            s_BKata = G_KS_Syurui + "�|" + s_Model + "�|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
        1) == "M") {
            s_BKata = G_KS_Syurui + "�|" + s_Model + "�|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
            2) == "MR") {
            s_BKata = G_KS_Syurui + "�|" + s_Model + "�|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 2));
        }
        else {
            s_BKata = G_KS_Syurui + "�|" + s_Model + "�|" +
                HanToZen(P_HEADS_DATA[36].TrimRight());
        }
    }
    else if (Code == "18" || Code == "23" || Code == "24" || Code == "25" ||
             Code == "26" || Code == "27" || Code == "28" || Code == "75") {
        s_BKata = P_FRAMECD[4];
    }
    else {
        s_BKata = G_KS_Syurui + "�|" + s_Model;
    }
    // 2019.11.22 CX-20�^�ǉ�_E

    return s_BKata;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)����  (�ڰѕ��i�ް��쐬�p)
// �T  �v      �F
// ��  ��      �F Key �F������
// �߂�l      �F �������� [true:�ް��L�� false:�ް�����]
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Search_HD_FRM_NZ_Buhin(AnsiString Key) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_Frame;
    AnsiString s_Parts_Code;
    AnsiString s_Text;
    bool b_Retry;

    int i_Length;

    // �ڰт̉E�[��������
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

    // �����p���i���ގ擾
    if (P_FRAMECD[1].ToIntDef(0) != 9) {
        s_Parts_Code = AnsiString(P_FRAMECD[1].ToIntDef(0));
    }
    else {
        // E1ɽ�ٌp��擪�ꌅ�ŕ���
        // 2011.06.20 ɽ�ًK�i�ύX
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
            // 2013/03/27 ɽ�ًK�i�ǉ�
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

    // ���O
    G_Log_Text = "�ڰ�ɽ�ٕ��i�\��Ͻ��������� �" + Key + "� �" + s_Frame + "� �" +
        P_FRAMECD[3] + "� �" + s_Parts_Code + "� �Ō������܂��B";
    Write_Log(G_Log_Text);

    P_SQL = "";
    P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
    P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'";
    P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'";
    P_SQL = P_SQL + "    AND PARTS_TYPE  = '" + P_FRAMECD[3] + "'";

    // 2006.12.21 ANSI�̏ꍇ�ł����򂳂���
    // if ( s_Parts_Code == "9" ) {
    if ((s_Parts_Code == "9") || (s_Parts_Code == "109")) {
        // ***********************************
        b_Retry = true;
        // E1ɽ�ٌp��擪�ꌅ�ŕ���
        // 2011.06.20 ɽ�ًK�i�ύX
        // s_Text = P_HEADS_DATA[953].SubString(1,1);
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        // ***********************
        if ((P_HEADS_DATA[951].ToIntDef(0) == 0) && (P_HEADS_DATA[966].ToIntDef
            (0) == 0) && // �E�������ꍇ
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
                // 2013/03/27 ɽ�ًK�i�ǉ�
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
            (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ��(E1/E2�̂ǂ��炩)�ɍE���L��ꍇ
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
                // 2013/03/27 ɽ�ًK�i�ǉ�
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
            (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ��(E3/E4�̂ǂ��炩)�ɍE���L��ꍇ
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
                // 2013/03/27 ɽ�ًK�i�ǉ�
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
            && // �㉺(E1/E2�̂ǂ��炩�ƁAE3/E4�̂ǂ��炩)�ɍE���L��ꍇ
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
                // 2013/03/27 ɽ�ًK�i�ǉ�
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

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(P_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        if (b_Retry == true) {
            G_ErrLog_Text =
                "�������w" + Key +
                "�x�́A�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�ɑ��݂��Ȃ��̂ŁA�E���^�C�v�ōČ������܂��B";
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
            // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
            //wkQuery->DatabaseName = ODBC_DSN_DNO;
            wkQuery->ConnectionName = ODBC_DSN_DNO;
            // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
            wkQuery->SQL->Clear();
            wkQuery->SQL->Add(P_SQL);
            wkQuery->Open();
            wkQuery->First();
        }
    }
    if (wkQuery->Eof) {
        // �װ۸�
        G_ErrLog_Text = "������ �" + Key + "� �" + s_Frame + "� �" +
            P_FRAMECD[3] + "� �" + s_Parts_Code +
            "� �́A�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�ɑ��݂��܂���B";

        Write_Error_Log(G_ErrLog_Text);
        Write_Log(G_ErrLog_Text);

        // �}��
        P_DNO[1] = "";
        P_DNO[2] = "";
        P_DNO[3] = "";
        P_DNO[4] = "";
        P_DNO[5] = "";
        // ��}�p�}��
        P_WKDNO[1] = "";
        P_WKDNO[2] = "";
        P_WKDNO[3] = "";
        P_WKDNO[4] = "";
        P_WKDNO[5] = "";
        // �ގ�����
        P_MatNM[1] = "";
        P_MatNM[2] = "";
        P_MatNM[3] = "";
        P_MatNM[4] = "";
        P_MatNM[5] = "";
        // �ގ��R�[�h
        P_MatCD[1] = "";
        P_MatCD[2] = "";
        P_MatCD[3] = "";
        P_MatCD[4] = "";
        P_MatCD[5] = "";
        // ����
        P_PLATE_NUM[1] = 0;
        P_PLATE_NUM[2] = 0;
        P_PLATE_NUM[3] = 0;
        P_PLATE_NUM[4] = 0;
        P_PLATE_NUM[5] = 0;
        // ����
        P_PLATE_NUM_SUS[1] = 0;
        P_PLATE_NUM_SUS[2] = 0;
        P_PLATE_NUM_SUS[3] = 0;
        P_PLATE_NUM_SUS[4] = 0;
        P_PLATE_NUM_SUS[5] = 0;
        // SUS�g�p�t���O
        P_SUS = 0;

        delete wkQuery;
        return false;
    }
    else {
        // �ގ�����
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
        // �}��
        P_DNO[1] = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_DNO[2] = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        P_DNO[3] = wkQuery->FieldByName("DNO3")->AsString.TrimRight();
        P_DNO[4] = wkQuery->FieldByName("DNO4")->AsString.TrimRight();
        P_DNO[5] = wkQuery->FieldByName("DNO5")->AsString.TrimRight();
        // ��}�p�}��
        P_WKDNO[1] = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_WKDNO[2] = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        P_WKDNO[3] = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        P_WKDNO[4] = wkQuery->FieldByName("WKDNO4")->AsString.TrimRight();
        P_WKDNO[5] = wkQuery->FieldByName("WKDNO5")->AsString.TrimRight();
        // ����
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
        // ����(SUS�p)
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
        // SUS�g�p�t���O
        P_SUS = wkQuery->FieldByName("LENGTH")->AsString.ToIntDef(0);

        G_Log_Text = "�ގ�����    �w" + P_ZaisituNM + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}�ԂP      �w" + P_DNO[1] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}�ԂQ      �w" + P_DNO[2] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}�ԂR      �w" + P_DNO[3] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}�ԂS      �w" + P_DNO[4] + "�x���擾�B";
        Write_Log(G_Log_Text);

        G_Log_Text = "��}�p�}�ԂP�w" + P_WKDNO[1] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}�ԂQ�w" + P_WKDNO[2] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}�ԂR�w" + P_WKDNO[3] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}�ԂS�w" + P_WKDNO[4] + "�x���擾�B";
        Write_Log(G_Log_Text);

        G_Log_Text = "�����P      �w" + AnsiString(P_PLATE_NUM[1]) + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�����Q      �w" + AnsiString(P_PLATE_NUM[2]) + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�����R      �w" + AnsiString(P_PLATE_NUM[3]) + "�x���擾�B";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F E�ڰ��ް��쐬���� ���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::E_Frame_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    // 2012.01.20 �����ގ�
    AnsiString s_ChinaM;
    // *******************

    // **********************************************
    // ***  �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�@����
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "�������w" + P_Key + "�x�ŁA�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�������B";
    Write_Log(G_Log_Text);

    // seek_ret���Ͻ��ɑ��݂��������Ȃ���
    // Write_SE_Frame_Data()�Ŏg�p����B
    seek_ret = Search_HD_FRM_NZ(P_Key, "E");

    G_Log_Text = "�������w" + P_Key + "�x�ŁA�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�������I���B";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  �ގ����̂̌���(ASME���܂܂�Ă��邩)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "�ގ��w" + P_ZaisituNM + "�x���AASME�ł��B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("�`�r�l�d") != 0) {
            G_ErrLog_Text = "�ގ��w" + P_ZaisituNM + "�x���A�`�r�l�d�ł��B";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  �E���d�l�̍쐬
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("E");
    G_Log_Text = "�E���d�l  �w" + P_Siyou1 + "�x���擾�B";
    Write_Log(G_Log_Text);

    // *************************
    // ***  �ގ��̎擾
    // *************************
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    // **********************************************
    // ***  �}�ԁE��}�p�}�Ԃ̎擾  02.09.03�ǉ�
    // **********************************************
    bSUS = false;
    // 2003.11.06�@�v���[�g�ގ��f�[�^�ʒu�ύX
    // sPltMat = P_HEADS_DATA[160].TrimRight();
    sPltMat = P_HEADS_DATA[43].TrimRight();
    // **************************************
    // 2003.12.05 ASME PLATE
    // if ((P_SUS != 0 ) && (sPltMat != "1014" ) && ( sPltMat != "1015" ) &&
    // ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]�j
    // 2016.11.07 �ގ����ޕύX
    //if ((P_SUS != 0) && (sPltMat != "1014") && (sPltMat != "1015") &&
    //    (sPltMat != "1032") && (sPltMat != "1033") && (sPltMat != "1057") &&
    //    (sPltMat != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
    //    (sPltMat != "1075") && (sPltMat != "1089") && (sPltMat != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11�ǉ�
    //    (sPltMat != "1076") &&
    //    // ***************************
    //    (sPltMat != "1085") && (sPltMat != "1095")) {
    //    // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
    //    // *********************
    //    bSUS = true;
    //}
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // �v���[�g�ގ����`�^���n�ȊO
        bSUS = true;
    }
    // ***********************
    // 2003.11.05 �����敪�֌W�����������ɕύX
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

    switch (P_HEADS_DATA[37].ToIntDef(0)) { // �t���[���R�[�h
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // �a�V���[�Y
        // s_Text = P_HEADS_DATA[34].TrimRight();
        // if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX"){
        // P_ZaisituNM = "�r�t�r�R�O�S";
        // P_EZaisituNM = "�r�t�r�R�O�S";
        //
        // } else {
        // P_ZaisituNM = "SS400/SUS304��ް�ݸ�";
        // P_EZaisituNM = "SS400/SUS304COVERING";
        // }

        break;
    // 2022.04.28 UX-30L�ǉ�_S
    // 2023.03.09 CX-00-Y�ǉ�_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y�ǉ�_E
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
    case 354: // �x�V���[�Y
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "�r�Q�V�T�i�q";
            P_EZaisituNM = "�r�Q�V�T�i�q";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    // 2022.04.28 UX-30L�ǉ�_E
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        // 2016.11.07 �ގ����ޕύX
        //if (s_Text == "0000" || s_Text == "1040") {
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
        // ***********************
            P_ZaisituNM = "�r�r�S�O�O";
            P_EZaisituNM = "�r�r�S�O�O";
        }
        else {
            // 2011.05.30 �ڰэގ��w��ύX
            // P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            // P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
            // ***************************
        }

        break;
    }

    // 2012.01.20 �����ގ�
    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    if (s_Text == "�t�w�|�P�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�O�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�P�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�R�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�T�O") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�Q�O") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�S�P") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�S�R") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�P�P") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�P�R") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�R�O") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�T�O") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�V�O") { // �����ގ�
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // �����ގ�
        // 2012.02.07 �����ڰэގ�
        // if ( P_ZaisituNM == "�r�r�S�O�O" ) {
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        // 2016.11.07 �ގ����ޕύX
        //if (s_Text == "1040") {
        if (Type_MAT(s_Text) == "SS400") {
        // ***********************
        }
        else if (P_ZaisituNM == "�r�r�S�O�O" || P_ZaisituNM == "�b�D�r�D") {
            // ***********************
            P_ZaisituNM = "�p�Q�R�T�a";
            P_EZaisituNM = "�p�Q�R�T�a";
        }
    }
    // *******************

    // 2008.08.21 WX-50�ڰѐ}�Ԑݒ�����ǉ�
    s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    if (s_Text == "�v�w�|�T�R") {
        // WHC=0,REVE=0 �a���n��
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
            // WHC=0,REVE=1 �`���n��
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
            // WHC=1,REVE=0 �`���n��
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
            // WHC=1,REVE=1 �a���n��
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
    // ***  �\�����̎擾
    // *************************
    Syukei_Frame_Yobisu("E");

    // *************************
    // ***  E�ڰ��ް��̏o��
    // *************************
    Write_SE_Frame_Data("E", seek_ret);

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F S�ڰ��ް��쐬���� ���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::S_Frame_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    // 2012.01.20 �����ގ�
    AnsiString s_ChinaM;
    // *******************

    // **********************************************
    // ***  �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�@����
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "�������w" + P_Key + "�x�ŁA�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�������B";
    Write_Log(G_Log_Text);

    // seek_ret���Ͻ��ɑ��݂��������Ȃ���
    // ���� Write_SE_Frame_Data()�Ŏg�p����B
    seek_ret = Search_HD_FRM_NZ(P_Key, "S");

    G_Log_Text = "�������w" + P_Key + "�x�ŁA�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�������I���B";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  �ގ����̂̌���(ASME���܂܂�Ă��邩)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "�ގ��w" + P_ZaisituNM + "�x���AASME�ł��B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("�`�r�l�d") != 0) {
            G_ErrLog_Text = "�ގ��w" + P_ZaisituNM + "�x���A�`�r�l�d�ł��B";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  �E���d�l�̍쐬
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("S");
    G_Log_Text = "�E���d�l  �w" + P_Siyou1 + "�x���擾�B";
    Write_Log(G_Log_Text);

    // *************************
    // ***  �ގ��̎擾
    // *************************
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    switch (P_HEADS_DATA[37].ToIntDef(0)) {
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // �a�V���[�Y
        // s_Text = P_HEADS_DATA[34].TrimRight();
        // if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX"){
        // P_ZaisituNM = "�r�t�r�R�O�S";
        // P_EZaisituNM = "�r�t�r�R�O�S";
        //
        // } else {
        // P_ZaisituNM = "SS400/SUS304��ް�ݸ�";
        // P_EZaisituNM = "SS400/SUS304COVERING";
        // }
        if (P_ZaisituNM == "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        break;
    // 2022.04.28 UX-30L�ǉ�_S
    // 2023.03.09 CX-00-Y�ǉ�_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y�ǉ�_E
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
    case 354: // �x�V���[�Y
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "�r�Q�V�T�i�q";
            P_EZaisituNM = "�r�Q�V�T�i�q";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    // 2022.04.28 UX-30L�ǉ�_E
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        // 2016.11.07 �ގ����ޕύX
        //if (s_Text == "0000" || s_Text == "1040") {
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
        // ***********************
            P_ZaisituNM = "�r�r�S�O�O";
            P_EZaisituNM = "�r�r�S�O�O";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }

        break;
    }

    // 2012.01.20 �����ގ�
    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    if (s_Text == "�t�w�|�P�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�O�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�P�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�R�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�T�O") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�Q�O") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�S�P") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�S�R") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�P�P") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�P�R") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�R�O") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�T�O") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�V�O") { // �����ގ�
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // �����ގ�
        // 2012.02.07 �����ڰэގ�
        // if ( P_ZaisituNM == "�r�r�S�O�O" ) {
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        // 2016.11.07 �ގ����ޕύX
        //if (s_Text == "1040") {
        if (Type_MAT(s_Text) == "SS400") {
        // ***********************
        }
        else if (P_ZaisituNM == "�r�r�S�O�O" || P_ZaisituNM == "�b�D�r�D") {
            // ***********************
            P_ZaisituNM = "�p�Q�R�T�a";
            P_EZaisituNM = "�p�Q�R�T�a";
        }
    }
    // *******************

    // **********************************************
    // ***  �}�ԁE��}�p�}�Ԃ̎擾  02.09.03�ǉ�
    // **********************************************
    bSUS = false;
    // 2003.11.06�@�v���[�g�ގ��f�[�^�ʒu�ύX
    // sPltMat = P_HEADS_DATA[160].TrimRight();
    sPltMat = P_HEADS_DATA[43].TrimRight();
    // **************************************
    // 2003.12.05 ASME PLATE
    // if ((P_SUS != 0 ) && (sPltMat != "1014" ) && ( sPltMat != "1015" ) &&
    // ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]�j
    // 2016.11.07 �ގ����ޕύX
    //if ((P_SUS != 0) && (sPltMat != "1014") && (sPltMat != "1015") &&
    //    (sPltMat != "1032") && (sPltMat != "1033") && (sPltMat != "1057") &&
    //    (sPltMat != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
    //    (sPltMat != "1075") && (sPltMat != "1089") && (sPltMat != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11�ǉ�
    //    (sPltMat != "1076") &&
    //    // ***************************
    //    (sPltMat != "1085") && (sPltMat != "1095")) {
    //    // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
    //    // *********************
    //    bSUS = true;
    //}
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // �v���[�g�ގ����`�^���n�ȊO
        bSUS = true;
    }
    // ***********************
    // 2003.11.05 �����敪�֌W�����������ɕύX
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

    // 2008.08.21 WX-50�ڰѐ}�Ԑݒ�����ǉ�
    s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    if (s_Text == "�v�w�|�T�R") {
        // WHC=0,REVE=0 �a���n��
        if ((P_HEADS_DATA[243].ToIntDef(0) == 0) && (P_HEADS_DATA[137].ToIntDef
            (0) == 0)) {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            P_Siyou3 = "2";
            // WHC=0,REVE=1 �`���n��
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) != 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            P_Siyou3 = "1";
            // WHC=1,REVE=0 �`���n��
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) != 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            P_Siyou3 = "1";
            // WHC=1,REVE=1 �a���n��
        }
        else {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            P_Siyou3 = "2";
        }
    }
    // ************************************

    // *************************
    // ***  �\�����̎擾
    // *************************
    Syukei_Frame_Yobisu("S");

    // *************************
    // ***  S�ڰ��ް��̏o��
    // *************************
    Write_SE_Frame_Data("S", seek_ret);

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------------------
// ���{��֐����F S,E�ڰ��ް��@��������
// �T  �v      �F
// ��  ��      �F Key �F������
// �F Kbn �FS���S�ڰїp�����CE���E�ڰя���
// �F Seek_Result:�ڰ�ɽ�ٕ��i�\��Ͻ���������(true:����,false:���݂��Ȃ�)
// �߂�l      �F �������� [true:�ް��L�� false:�ް�����]
// ��  �l      �F
// ---------------------------------------------------------------------------------------
bool __fastcall TFrameDetails::Write_SE_Frame_Data(AnsiString Kbn,
    bool Seek_Result) {
    AnsiString s_Text;
    AnsiString wk_FilePass;
    // 2019.11.22 CX-20�^�ǉ�_S
    AnsiString s_Model;
    // 2019.11.22 CX-20�^�ǉ�_E

    long l_Daisu;

    // ------------------------------------------
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    AnsiString sSpecCode;
    // ****************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // ****************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // ------------------------------------------

    // ****************************
    // ***  �ϐ��Z�b�g
    // ****************************
    // ���i����
    P_BBunrui = "FRAME";
    // ���ʎq
    if (Kbn == "S") {
        P_Sikibetu = "FRAM-S";
    }
    // 2022.05.23 �t���[���K�X�؂�ǉ�_S
    else if (Kbn == "SG") {
        P_Sikibetu = "FRAMSG";
    }
    else if (Kbn == "EG") {
        P_Sikibetu = "FRAMEG";
    }
    // 2022.05.23 �t���[���K�X�؂�ǉ�_E
    else {
        P_Sikibetu = "FRAM-E";
    }
    // ���i�^��
    // 2019.11.22 CX-20�^�ǉ�_S
    //if (Seek_Result == true) {
    //    // 2003.09.10 ���i�^���ύX
    //    // if( P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) ==  "R" ) {
    //    // P_BKatasiki = G_KS_Syurui + "�|" + G_KS_Model + "�|" + HanToZen(P_HEADS_DATA[36].SubString(1,P_HEADS_DATA[36].Length()-1));
    //    // } else {
    //    // P_BKatasiki = G_KS_Syurui + "�|" + G_KS_Model + "�|" + HanToZen(P_HEADS_DATA[36].TrimRight());
    //    // }
    //    if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
    //        P_BKatasiki = G_KS_Syurui + "�|" + G_KS_Model + "�|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
    //    1) == "M") {
    //        P_BKatasiki = G_KS_Syurui + "�|" + G_KS_Model + "�|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
    //        2) == "MR") {
    //        P_BKatasiki = G_KS_Syurui + "�|" + G_KS_Model + "�|" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 2));
    //    }
    //    else {
    //        P_BKatasiki = G_KS_Syurui + "�|" + G_KS_Model + "�|" +
    //            HanToZen(P_HEADS_DATA[36].TrimRight());
    //    }
    //    // ***********************
    //}
    //else {
    //    P_BKatasiki = "";
    //}
    s_Model = G_KS_Model;

    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�Q�P") {
        s_Model ="�Q�O";
    }
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�Q�R") {
        s_Model ="�Q�O";
    }

    if (Seek_Result == true) {
        if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
            P_BKatasiki = G_KS_Syurui + "�|" + s_Model + "�|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
        1) == "M") {
            P_BKatasiki = G_KS_Syurui + "�|" + s_Model + "�|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
            2) == "MR") {
            P_BKatasiki = G_KS_Syurui + "�|" + s_Model + "�|" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 2));
        }
        else {
            P_BKatasiki = G_KS_Syurui + "�|" + s_Model + "�|" +
                HanToZen(P_HEADS_DATA[36].TrimRight());
        }
    }
    else {
        P_BKatasiki = "";
    }
    // 2019.11.22 CX-20�^�ǉ�_E

    // ���i����
    if (Kbn == "S") {
        P_BMeisyou = "�r�D�t���[��";
    }
    // 2022.05.23 �t���[���K�X�؂�ǉ�_S
    else if (Kbn == "SG") {
        P_BMeisyou = "�r�D�t���[���i�K�X�n�f�j";
    }
    else if (Kbn == "EG") {
        P_BMeisyou = "�d�D�t���[���i�K�X�n�f�j";
    }
    // 2022.05.23 �t���[���K�X�؂�ǉ�_E
    else {
        P_BMeisyou = "�d�D�t���[��";
    }
    // �P�䕪����
    if (Seek_Result == true) {
        P_1DaiSuryo = 1;
    }
    else {
        P_1DaiSuryo = 0;
    }

    // ���v
    l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // �䐔
    P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;

    // �d�l�P
    if (Seek_Result == true) {
        P_Siyou1 = "�E��" + P_Siyou1;
    }
    else {
        P_Siyou1 = "";
    }

    // �d�l�Q(S,E�ڰю��)
    if (Seek_Result == true) {
        if (Kbn == "S") {
            P_Siyou2 = P_HEADS_DATA[1296].TrimRight(); // S�ڰю��
        }
        else {
            P_Siyou2 = P_HEADS_DATA[1297].TrimRight(); // E�ڰю��
        }
    }
    else {
        P_Siyou2 = "";
    }

    // �d�l�S
    P_Siyou4 = "";
    // �d�l�T
    P_Siyou5 = "";

    // ****************************
    // ***  S,E�ڰ��ް�  �Z�b�g
    // ****************************
    // ���i�W�J�ް��@OPEN
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

    // 1  ���i����
    s_Text = P_BBunrui;
    fputs(s_Text.c_str(), fp);
    // 2  ���ʎq
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), fp);
    // 3  ���i�^��
    s_Text = "," + P_BKatasiki;
    fputs(s_Text.c_str(), fp);
    // 4  ���i����
    s_Text = "," + P_BMeisyou;
    fputs(s_Text.c_str(), fp);
    // 5  �ގ�����
    s_Text = "," + P_ZaisituNM;
    fputs(s_Text.c_str(), fp);
    // 6  �}��
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), fp);
    // 7  ��}�p�}��
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), fp);
    // 8  1�䕪����
    s_Text = "," + AnsiString(P_1DaiSuryo);
    fputs(s_Text.c_str(), fp);
    // 9  �\����
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), fp);

    // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), fp);

    // 11 �d�l�P
    s_Text = "," + P_Siyou1;
    fputs(s_Text.c_str(), fp);
    // 12 �d�l�Q
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), fp);
    // 13 �d�l�R
    s_Text = "," + P_Siyou3;
    fputs(s_Text.c_str(), fp);
    // 14 �d�l�S
    s_Text = "," + P_Siyou4;
    fputs(s_Text.c_str(), fp);
    // 15 �d�l�T
    s_Text = "," + P_Siyou5;
    fputs(s_Text.c_str(), fp);
    // 16  �ގ�����(�p)
    s_Text = "," + P_EZaisituNM;
    fputs(s_Text.c_str(), fp);

    // ------------------------------
    // 2007.10.03 �װ�d�l�Ή�
    // 17 �d�l���ߺ���
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), fp);
    // 2007.10.03
    // ------------------------------

    // ���s����
    s_Text = "\n";
    // CSV�ް��@̧�ُo��
    fputs(s_Text.c_str(), fp);

    fclose(fp);

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�@����
// �T  �v      �F
// ��  ��      �F Key �F������
// �F Kbn �FS���S�ڰїp�����CE���E�ڰя���
// �߂�l      �F �������� [true:�ް��L�� false:�ް�����]
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Search_HD_FRM_NZ(AnsiString Key, AnsiString Kbn)
{
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_Frame;
    AnsiString s_Text;
    // 2011.06.20 ɽ�ًK�i�ύX
    AnsiString s_Text2;
    // ***********************

    int i_Length;
    bool E1Hol, E2Hole, E3Hole, E4Hole;

    // �ڰт̉E�[��������
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
    P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ��ڰČ^��
    P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // �ڰь^��

    if (Kbn == "S") {
        P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

        // S1ɽ�ٌp��擪�ꌅ�ŕ���
        // 2011.06.20 ɽ�ًK�i�ύX
        // s_Text = P_HEADS_DATA[893].SubString(1,1);
        s_Text = P_HEADS_DATA[893].SubString(1, 2);
        // 2009.02.16 ɽ�ٌp������ύX
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1ɽ�ٌp��擪�ꌅ�ŕ���
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2ɽ�ٌp��擪�ꌅ�ŕ���
            // s_Text = P_HEADS_DATA[908].SubString(1,1);
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3ɽ�ٌp��擪�ꌅ�ŕ���
            // s_Text = P_HEADS_DATA[923].SubString(1,1);
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4ɽ�ٌp��擪�ꌅ�ŕ���
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
            P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // �ڰ�ɽ�َ��
            break;
            // 2013/03/27 ɽ�ًK�i�ǉ�
        case 14:
        case 24:
        case 31:
        case 32:
        case 33:
        case 34:
        case 44:
        case 48: // ANSI/JPI/DIN/GB/EN
            P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // �ڰ�ɽ�َ��
            break;
            // ***********************
        default:
            P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // �ڰ�ɽ�َ��
            break;
        }
        // ***************************

    }
    else if (Kbn == "E") {
        P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

        // E1ɽ�ٌp��擪�ꌅ�ŕ���
        // 2011.06.20 ɽ�ًK�i�ύX
        // s_Text = P_HEADS_DATA[953].SubString(1,1);
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        // 2009.02.16 ɽ�ٌp������ύX
        // �p���ނ�E�ڰт�Sɽ�قŔ��f����
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1ɽ�ٌp��擪�ꌅ�ŕ���
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2ɽ�ٌp��擪�ꌅ�ŕ���
            // s_Text = P_HEADS_DATA[908].SubString(1,1);
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3ɽ�ٌp��擪�ꌅ�ŕ���
            // s_Text = P_HEADS_DATA[923].SubString(1,1);
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4ɽ�ٌp��擪�ꌅ�ŕ���
            // s_Text = P_HEADS_DATA[938].SubString(1,1);
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }
        // ***************************

        if ((P_HEADS_DATA[951].ToIntDef(0) == 0) && (P_HEADS_DATA[966].ToIntDef
            (0) == 0) && // �E�������ꍇ
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ɽ�ًK�i�ύX
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
                // 2013.04.03 ɽ�ًK�i�ǉ�
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
            (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ��(E1/E2�̂ǂ��炩)�ɍE���L��ꍇ
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ɽ�ًK�i�ύX
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
                // 2013.04.03 ɽ�ًK�i�ǉ�
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
            (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ��(E3/E4�̂ǂ��炩)�ɍE���L��ꍇ
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ɽ�ًK�i�ύX
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
                // 2013.04.03 ɽ�ًK�i�ǉ�
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
            && // �㉺(E1/E2�̂ǂ��炩�ƁAE3/E4�̂ǂ��炩)�ɍE���L��ꍇ
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ɽ�ًK�i�ύX
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
                // 2013.04.03 ɽ�ًK�i�ǉ�
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
    // 2022.05.23 �t���[���K�X�؂�ǉ�_S
    else if (Kbn == "SG") {
        P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

        // S1ɽ�ٌp��擪�ꌅ�ŕ���
        s_Text = P_HEADS_DATA[893].SubString(1, 2);
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4ɽ�ٌp��擪�ꌅ�ŕ���
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
            P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // �ڰ�ɽ�َ��
            break;
        case 14:
        case 24:
        case 31:
        case 32:
        case 33:
        case 34:
        case 44:
        case 48: // ANSI/JPI/DIN/GB/EN
            P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // �ڰ�ɽ�َ��
            break;
        default:
            P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // �ڰ�ɽ�َ��
            break;
        }

    }
    else if (Kbn == "EG") {
        P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

        // E1ɽ�ٌp��擪�ꌅ�ŕ���
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        // �p���ނ�E�ڰт�Sɽ�قŔ��f����
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }

        if ((P_HEADS_DATA[951].ToIntDef(0) == 0) && (P_HEADS_DATA[966].ToIntDef
            (0) == 0) && // �E�������ꍇ
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
            (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ��(E1/E2�̂ǂ��炩)�ɍE���L��ꍇ
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
            (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ��(E3/E4�̂ǂ��炩)�ɍE���L��ꍇ
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ɽ�ًK�i�ύX
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
            && // �㉺(E1/E2�̂ǂ��炩�ƁAE3/E4�̂ǂ��炩)�ɍE���L��ꍇ
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
                // 2013.04.03 ɽ�ًK�i�ǉ�
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
    // 2022.05.23 �t���[���K�X�؂�ǉ�_E

    // 2009.02.16 RX-50�̏ꍇDIN,#150��JIS�Ƃ��Ĉ���
    // 2011.06.20 ɽ�ًK�i�ύX
    // s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    // if (s_Text == "�q�w�|�T�O") {
    s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
    if (s_Text2 == "�q�w�|�T�O") {
        // ***********************

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ��ڰČ^��
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // �ڰь^��

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            // ***********************

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // �ڰ�ɽ�َ��
                break;
                // 2013.04.03 ɽ�ًK�i�ǉ�
            // 2021.09.22 RX-50Z�ǉ�_S
            //case 13:
            // 2021.09.22 RX-50Z�ǉ�_E
            case 14:
            // 2021.09.22 RX-50Z�ǉ�_S
            //case 23:
            // 2021.09.22 RX-50Z�ǉ�_E
            case 24:
            // 2021.09.22 RX-50Z�ǉ�_S
            //case 33:
            // 2021.09.22 RX-50Z�ǉ�_E
            case 34:
            case 41:
            case 44:
            case 48:
                // 2021.09.22 RX-50Z�ǉ�_S
                //// JIS30K,PN25/40,#400(�ڰтȂ�)
                // JIS30K,PN40,#400(�ڰтȂ�)
                // 2021.09.22 RX-50Z�ǉ�_E
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
                // ***********************
            // 2021.09,22 RX-50Z�ǉ�_S
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // Z�ڰ� PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // �ڰ�ɽ�َ��
                } else {
                    // Z�ڰшȊO PN25(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                }
                break;
            // 2021.09.22 RX-50Z�ǉ�_E
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // �ڰ�ɽ�َ��
                break;
            }

        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = P_HEADS_DATA[953].SubString(1,1);
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // �p���ނ�E�ڰт�Sɽ�قŔ��f����
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            // ***********************

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // �ڰ�ɽ�َ��
                break;
                // 2013.04.03 ɽ�ًK�i�ǉ�
            // 2021.09.22 RX-50Z�ǉ�_S
            //case 13:
            // 2021.09.22 RX-50Z�ǉ�_E
            case 14:
            // 2021.09.22 RX-50Z�ǉ�_S
            //case 23:
            // 2021.09.22 RX-50Z�ǉ�_E
            case 24:
            // 2021.09.22 RX-50Z�ǉ�_S
            //case 33:
            // 2021.09.22 RX-50Z�ǉ�_E
            case 34:
            case 41:
            case 44:
            case 48:
                // 2021.09.22 RX-50Z�ǉ�_S
                //// JIS30K,PN25/40,#400(�ڰтȂ�)
                // JIS30K,PN40,#400(�ڰтȂ�)
                // 2021.09.22 RX-50Z�ǉ�_E
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
                // ***********************
            // 2021.09.22 RX-50Z�ǉ�_S
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // Z�ڰ� PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // �ڰ�ɽ�َ��
                } else {
                    // Z�ڰшȊO PN25(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                }
                break;
            // 2021.09.22 RX-50Z�ǉ�_E
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // �ڰ�ɽ�َ��
                break;
            }
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // �ڰ�ɽ�َ��
                break;
                // 2013.04.03 ɽ�ًK�i�ǉ�
            case 14:
            case 24:
            case 34:
            case 41:
            case 44:
            case 48:
                // JIS30K,PN40,#400(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // Z�ڰ� PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // �ڰ�ɽ�َ��
                } else {
                    // Z�ڰшȊO PN25(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                }
                break;
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // �ڰ�ɽ�َ��
                break;
            }

        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // �p���ނ�E�ڰт�Sɽ�قŔ��f����
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // �ڰ�ɽ�َ��
                break;
            case 14:
            case 24:
            case 34:
            case 41:
            case 44:
            case 48:
                // JIS30K,PN40,#400(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // Z�ڰ� PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // �ڰ�ɽ�َ��
                } else {
                    // Z�ڰшȊO PN25(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                }
                break;
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // �ڰ�ɽ�َ��
                break;
            }
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_E
    }
    // *********************************************

    // 2010.08.27 LX-10-S�̏ꍇJIS16K,20K���ڰ�ɽ�َ�ʂ�ǉ�����
    // 2011.06.20 ɽ�ًK�i�ύX
    // s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    // if (s_Text == "�k�w�|�P�O") {
    s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
    if (s_Text2 == "�k�w�|�P�O") {
        // ***********************

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ��ڰČ^��
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // �ڰь^��

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
                // case 2: case 3:                                    // JIS16K,20K
            case 2:
            case 3:
            case 12:
            case 22: // JIS16K,20K/GB1.6/DN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 201"; // �ڰ�ɽ�َ��
                break;
                // 2013.04.03 ɽ�ًK�i�ǉ�
                ////case 4: case 5: case 8: case 9:                    // ANSI/JPI/DIN
                // case 4: case 5: case 8: case 9:                    // ANSI/JPI
            case 4:
            case 8: // #150
                P_SQL = P_SQL + "    AND PARTS_CODE = 201"; // �ڰ�ɽ�َ��
                break;
            case 32: // EN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 201"; // �ڰ�ɽ�َ��
                break;
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS30K,#300,400(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34:
                // PN25/40(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
                // ***********************
                // default:                                           // JIS10K
            default: // JIS10K/GB1.0/DN10
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // �ڰ�ɽ�َ��
                break;
            }
            // ***********************

        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ɽ�ٌp��擪�ꌅ�ŕ���
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = P_HEADS_DATA[953].SubString(1,1);
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // �p���ނ�E�ڰт�Sɽ�قŔ��f����
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // �E�������ꍇ
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 ɽ�ًK�i�ǉ�
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 108";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // �ڰ�ɽ�َ��
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // �ڰ�ɽ�َ��
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 8";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ��(E1/E2�̂ǂ��炩)�ɍE���L��ꍇ
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 ɽ�ًK�i�ǉ�
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 308";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 308"; // �ڰ�ɽ�َ��
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 308"; // �ڰ�ɽ�َ��
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 208";
                    break;
                }
            }
            else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ��(E3/E4�̂ǂ��炩)�ɍE���L��ꍇ
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 ɽ�ًK�i�ǉ�
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 508";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 508"; // �ڰ�ɽ�َ��
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 508"; // �ڰ�ɽ�َ��
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 408";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0))
                && // �㉺(E1/E2�̂ǂ��炩�ƁAE3/E4�̂ǂ��炩)�ɍE���L��ꍇ
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 ɽ�ًK�i�ǉ�
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 708";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 708"; // �ڰ�ɽ�َ��
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 708"; // �ڰ�ɽ�َ��
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 608";
                    break;
                }
            }
            // ***********************
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2:
            case 3:
            case 12:
            case 22: // JIS16K,20K/GB1.6/DN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 281"; // �ڰ�ɽ�َ��
                break;
            case 4:
            case 8: // #150
                P_SQL = P_SQL + "    AND PARTS_CODE = 281"; // �ڰ�ɽ�َ��
                break;
            case 32: // EN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 281"; // �ڰ�ɽ�َ��
                break;
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS30K,#300,400(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34:
                // PN25/40(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            default: // JIS10K/GB1.0/DN10
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // �ڰ�ɽ�َ��
                break;
            }

        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // �p���ނ�E�ڰт�Sɽ�قŔ��f����
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // �E�������ꍇ
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // �ڰ�ɽ�َ��
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // �ڰ�ɽ�َ��
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 88";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // ��(E1/E2�̂ǂ��炩)�ɍE���L��ꍇ
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 388"; // �ڰ�ɽ�َ��
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 388"; // �ڰ�ɽ�َ��
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 288";
                    break;
                }
            }
            else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // ��(E3/E4�̂ǂ��炩)�ɍE���L��ꍇ
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 588"; // �ڰ�ɽ�َ��
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 588"; // �ڰ�ɽ�َ��
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 488";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0))
                && // �㉺(E1/E2�̂ǂ��炩�ƁAE3/E4�̂ǂ��炩)�ɍE���L��ꍇ
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 788"; // �ڰ�ɽ�َ��
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 788"; // �ڰ�ɽ�َ��
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(�ڰтȂ�)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 688";
                    break;
                }
            }
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_E
    }
    // **********************************************************

    // 2018.06.19 LX-30Z�ǉ�_S
    // JIS10K
    //   �r�t���[���F"1"
    //   �d�t���[���F"8"
    //   �d���[���[�F"9"�@�@�@
    // ANSI#150/JPI#150/DIN/GB/DIN
    //   �r�t���[���F"1"
    //   �d�t���[���F"8"
    //   �d���[���[�F"109"�@�@�@
    // JIS16K
    //   �r�t���[���F"101"�@�@�@
    //   �d�t���[���F"108"
    //   �d���[���[�F"109"�@
    // JIS20K,30K/ANSI#300,400/JPI#300,400(�ڰтȂ�)�@
    //   �r�t���[���F"999"�@�@�@
    //   �d�t���[���F"999"
    //   �d���[���[�F"9"or"109"�@
    s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
    if ((s_Text2 == "�k�w�|�R�O") && ( P_HEADS_DATA[36].Pos("Z") > 0)) {
        // �k�w�|�R�O�|�y

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ��ڰČ^��
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // �ڰь^��

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // �ڰ�ɽ�َ��
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
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // �ڰ�ɽ�َ��
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // �ڰ�ɽ�َ��
            break;
            }
        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // �p���ނ�E�ڰт�Sɽ�قŔ��f����
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // �ڰ�ɽ�َ��
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
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // �ڰ�ɽ�َ��
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // �ڰ�ɽ�َ��
            break;
            }
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // �ڰ�ɽ�َ��
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
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // �ڰ�ɽ�َ��
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // �ڰ�ɽ�َ��
            break;
            }
        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // �p���ނ�E�ڰт�Sɽ�قŔ��f����
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // �ڰ�ɽ�َ��
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
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // �ڰ�ɽ�َ��
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(�ڰтȂ�)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // �ڰ�ɽ�َ��
            break;
            }
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_E
    }
    // 2018.06.19 LX-30Z�ǉ�_E

    // 2019.11.08 CX-23�ǉ�_S
    // JIS10K,16K,20K/DIN10,16/GB10,16/EN10,16
    //   �r�t���[���F"1"
    //   �d�t���[���F"8"
    //   �d���[���[�F"9"�@�@�@
    // ANSI#150/JPI#150
    //   �r�t���[���F"101"
    //   �d�t���[���F"108"
    //   �d���[���[�F"109"�@�@�@
    // JIS30K/ANSI#300,400/JPI#300,400/DIN25,40/GB25,40/EN25,40 (�ڰтȂ�)�@
    //   �r�t���[���F"999"�@�@�@
    //   �d�t���[���F"999"
    //   �d���[���[�F"9"or"109"�@
    s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
    // 2022.04.27 CX-21�ǉ�_S
    //if (s_Text2 == "�b�w�|�Q�R") {
    //    // �b�w�|�Q�R
    if ((s_Text2 == "�b�w�|�Q�P") || (s_Text2 == "�b�w�|�Q�R")) {
        // �b�w�|�Q�P�A�b�w�|�Q�R
    // 2022.04.27 CX-21�ǉ�_E

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ��ڰČ^��
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // �ڰь^��

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // �ڰ�ɽ�َ��
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
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40�@
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // �ڰ�ɽ�َ��
            break;
            }
        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // �p���ނ�E�ڰт�Sɽ�قŔ��f����
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // �ڰ�ɽ�َ��
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
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40�@
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // �ڰ�ɽ�َ��
            break;
            }
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ɽ�ٌp��擪�ꌅ�ŕ���(�ި̫��)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // �ڰ�ɽ�َ��
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
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40�@
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // �ڰ�ɽ�َ��
            break;
            }
        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ɽ�ٌp��擪�ꌅ�ŕ���
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // �p���ނ�E�ڰт�Sɽ�قŔ��f����
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ɽ�ٌp��擪�ꌅ�ŕ���
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // �ڰ�ɽ�َ��
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
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40�@
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // �ڰ�ɽ�َ��
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // �ڰ�ɽ�َ��
            break;
            }
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_E
    }
    // 2019.11.08 CX-23�ǉ�_E

    // 2022.05.23 �t���[���K�X�؂�ǉ�_S
    G_Log_Text = "������ �w" + P_SQL + "�x�Ō������܂��B";
    Write_Log(G_Log_Text);
    // 2022.05.23 �t���[���K�X�؂�ǉ�_E

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(P_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        if (Kbn == "E") { // E�t���[���̏ꍇ�́A�E�������ŊY�����郌�R�[�h�������ꍇ�A�E�����ōČ�������
            G_ErrLog_Text =
                "�������w" + Key +
                "�x�́A�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�ɑ��݂��Ȃ��̂ŁA�E���^�C�v�ōČ������܂��B";
            Write_Error_Log(G_ErrLog_Text);

            P_SQL = "";
            P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
            P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ��ڰČ^��
            P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // �ڰь^��
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ɽ�ًK�i�ύX
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
                // 2013/03/27 ɽ�ًK�i�ǉ�
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // �ڰ�ɽ�َ��
                break;
                // ***********************
            default:
                P_SQL = P_SQL + "    AND PARTS_CODE = 8";
                break;
            }
            // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
            //wkQuery->DatabaseName = ODBC_DSN_DNO;
            wkQuery->ConnectionName = ODBC_DSN_DNO;
            // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
            wkQuery->SQL->Clear();
            wkQuery->SQL->Add(P_SQL);
            wkQuery->Open();
            wkQuery->First();
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_S
        else if (Kbn == "SG" || Kbn == "EG") {
            // SE�t���[���K�X�؂�}�ԂȂ��̏ꍇ�́ASE�t���[���Ɠ����}�ԁi�W�J���Ȃ��j
            // �װ۸�
            G_ErrLog_Text =
                "�������w" + Key + "�x�́A�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�ɑ��݂��܂���B";
            Write_Error_Log(G_ErrLog_Text);

            // �ގ�����
            P_MatNM[1] = "";
            P_MatNM[2] = "";
            P_MatNM[3] = "";
            P_MatNM[4] = "";
            P_MatNM[5] = "";
            // �}��
            P_DNO[1] = "NONE";
            P_DNO[2] = "";
            P_DNO[3] = "";
            P_DNO[4] = "";
            P_DNO[5] = "";
            // ��}�p�}��
            P_WKDNO[1] = "";
            P_WKDNO[2] = "";
            P_WKDNO[3] = "";
            P_WKDNO[4] = "";
            P_WKDNO[5] = "";
            // ����
            P_PLATE_NUM[1] = 0;
            P_PLATE_NUM[2] = 0;
            P_PLATE_NUM[3] = 0;
            P_PLATE_NUM[4] = 0;
            P_PLATE_NUM[5] = 0;
            // ����(SUS�p)
            P_PLATE_NUM_SUS[1] = 0;
            P_PLATE_NUM_SUS[2] = 0;
            P_PLATE_NUM_SUS[3] = 0;
            P_PLATE_NUM_SUS[4] = 0;
            P_PLATE_NUM_SUS[5] = 0;
            // SUS�g�p�t���O
            P_SUS = 0;

            delete wkQuery;
            return false;
        }
        // 2022.05.23 �t���[���K�X�؂�ǉ�_E
        else {
            // �װ۸�
            G_ErrLog_Text =
                "�������w" + Key + "�x�́A�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�ɑ��݂��܂���B";
            Write_Error_Log(G_ErrLog_Text);

            // �ގ�����
            P_MatNM[1] = "";
            P_MatNM[2] = "";
            P_MatNM[3] = "";
            P_MatNM[4] = "";
            P_MatNM[5] = "";
            // �}��
            P_DNO[1] = "";
            P_DNO[2] = "";
            P_DNO[3] = "";
            P_DNO[4] = "";
            P_DNO[5] = "";
            // ��}�p�}��
            P_WKDNO[1] = "";
            P_WKDNO[2] = "";
            P_WKDNO[3] = "";
            P_WKDNO[4] = "";
            P_WKDNO[5] = "";
            // ����
            P_PLATE_NUM[1] = 0;
            P_PLATE_NUM[2] = 0;
            P_PLATE_NUM[3] = 0;
            P_PLATE_NUM[4] = 0;
            P_PLATE_NUM[5] = 0;
            // ����(SUS�p)
            P_PLATE_NUM_SUS[1] = 0;
            P_PLATE_NUM_SUS[2] = 0;
            P_PLATE_NUM_SUS[3] = 0;
            P_PLATE_NUM_SUS[4] = 0;
            P_PLATE_NUM_SUS[5] = 0;
            // SUS�g�p�t���O
            P_SUS = 0;

            delete wkQuery;
            return false;
        }
    }
    if (!wkQuery->Eof) {
        // �ގ�����
        P_ZaisituNM = wkQuery->FieldByName("MAT_NAME1")->AsString.TrimRight();
        P_EZaisituNM = P_ZaisituNM;
        // �ގ�����
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
        // �}��
        P_DNO[1] = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_DNO[2] = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        P_DNO[3] = wkQuery->FieldByName("DNO3")->AsString.TrimRight();
        P_DNO[4] = wkQuery->FieldByName("DNO4")->AsString.TrimRight();
        P_DNO[5] = wkQuery->FieldByName("DNO5")->AsString.TrimRight();
        // ��}�p�}��
        P_WKDNO[1] = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_WKDNO[2] = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        P_WKDNO[3] = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        P_WKDNO[4] = wkQuery->FieldByName("WKDNO4")->AsString.TrimRight();
        P_WKDNO[5] = wkQuery->FieldByName("WKDNO5")->AsString.TrimRight();
        // ����
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
        // ����(SUS�p)
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
        // SUS�g�p�t���O
        P_SUS = wkQuery->FieldByName("LENGTH")->AsString.ToIntDef(0);

        G_Log_Text = "�ގ�����    �w" + P_ZaisituNM + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}�ԂP      �w" + P_DNO[1] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}�ԂQ      �w" + P_DNO[2] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}�ԂR      �w" + P_DNO[3] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}�ԂS      �w" + P_DNO[4] + "�x���擾�B";
        Write_Log(G_Log_Text);

        G_Log_Text = "��}�p�}�ԂP�w" + P_WKDNO[1] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}�ԂQ�w" + P_WKDNO[2] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}�ԂR�w" + P_WKDNO[3] + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}�ԂS�w" + P_WKDNO[4] + "�x���擾�B";
        Write_Log(G_Log_Text);

        G_Log_Text = "�����P      �w" + AnsiString(P_PLATE_NUM[1]) + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�����Q      �w" + AnsiString(P_PLATE_NUM[2]) + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�����R      �w" + AnsiString(P_PLATE_NUM[3]) + "�x���擾�B";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �E���d�l �擾
// �T  �v      �F
// ��  ��      �F Kbn �FS���S�ڰїp�����CE���E�ڰя���
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall TFrameDetails::Get_AnaakeSiyou(AnsiString Kbn) {
    AnsiString s_Text;

    AnsiString s_S1Nzl_Syurui; // S1ɽ�َ��
    AnsiString s_S1Nzl_Tugite; // S1ɽ�ٌp��
    AnsiString s_S2Nzl_Syurui; // S2ɽ�َ��
    AnsiString s_S2Nzl_Tugite; // S2ɽ�ٌp��
    AnsiString s_S3Nzl_Syurui; // S3ɽ�َ��
    AnsiString s_S3Nzl_Tugite; // S3ɽ�ٌp��
    AnsiString s_S4Nzl_Syurui; // S4ɽ�َ��
    AnsiString s_S4Nzl_Tugite; // S4ɽ�ٌp��

    AnsiString s_E1Nzl_Syurui; // E1ɽ�َ��
    AnsiString s_E1Nzl_Tugite; // E1ɽ�ٌp��
    AnsiString s_E2Nzl_Syurui; // E2ɽ�َ��
    AnsiString s_E2Nzl_Tugite; // E2ɽ�ٌp��
    AnsiString s_E3Nzl_Syurui; // E3ɽ�َ��
    AnsiString s_E3Nzl_Tugite; // E3ɽ�ٌp��
    AnsiString s_E4Nzl_Syurui; // E4ɽ�َ��
    AnsiString s_E4Nzl_Tugite; // E4ɽ�ٌp��

    AnsiString s_AnaSiyou; // �E���d�l
    AnsiString s_S1;
    AnsiString s_S2;
    AnsiString s_S3;
    AnsiString s_S4;
    AnsiString s_S[5];
    AnsiString s_E[5];

    int i_OgataFlg; // 0:���^���^,1:��^

    // *************************
    // ***  HEADS�ް��@���
    // *************************
    if (Kbn == "S") {
        s_S1Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1ɽ�َ��
        s_S1Nzl_Tugite = P_HEADS_DATA[893].TrimRight(); // S1ɽ�ٌp��
        s_S2Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2ɽ�َ��
        s_S2Nzl_Tugite = P_HEADS_DATA[908].TrimRight(); // S2ɽ�ٌp��
        s_S3Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ɽ�َ��
        s_S3Nzl_Tugite = P_HEADS_DATA[923].TrimRight(); // S3ɽ�ٌp��
        s_S4Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ɽ�َ��
        s_S4Nzl_Tugite = P_HEADS_DATA[938].TrimRight(); // S4ɽ�ٌp��
    }
    else {
        s_E1Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1ɽ�َ��
        s_E1Nzl_Tugite = P_HEADS_DATA[953].TrimRight(); // E1ɽ�ٌp��
        s_E2Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2ɽ�َ��
        s_E2Nzl_Tugite = P_HEADS_DATA[968].TrimRight(); // E2ɽ�ٌp��
        s_E3Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ɽ�َ��
        s_E3Nzl_Tugite = P_HEADS_DATA[983].TrimRight(); // E3ɽ�ٌp��
        s_E4Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ɽ�َ��
        s_E4Nzl_Tugite = P_HEADS_DATA[998].TrimRight(); // E4ɽ�ٌp��
    }

    // 2003.09.09 ��^�t���[���E��
    s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    // 2003.09.12 �d�t���[���E��
    // if (s_Text == "�t�w�|�W�O" || s_Text == "�t�w�|�X�O" ||
    // s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�V�O" ||
    // s_Text == "�r�w�|�X�O" || s_Text == "�q�w�|�V�O"){
    // i_OgataFlg = 1;
    // } else {
    // i_OgataFlg = 0;
    // }
    // 2009.06.19 ��^�ڰя������ύX(UX-100R�ǉ�)
    ////2008.08.01 ��^�ڰя������ύX(CX-10,CX-10D�ǉ�)
    ////////2006.02.06 ��^�ڰя������ύX(RX-90,SX-90M,L�ǉ�)
    //////if (s_Text == "�t�w�|�W�O" || s_Text == "�t�w�|�X�O" ||
    //////    s_Text == "�t�w�|�P�O�O" || s_Text == "�r�w�|�V�O" ||
    //////    s_Text == "�r�w�|�X�O" || s_Text == "�q�w�|�V�O"){
    ////if (s_Text == "�t�w�|�W�O" || s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" ||
    ////    s_Text == "�r�w�|�V�O" || s_Text == "�r�w�|�X�O" || s_Text == "�r�w�|�X�O�l" ||
    ////    s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O"){
    //////*************************************************
    // if (s_Text == "�t�w�|�W�O" || s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" ||
    // s_Text == "�t�w�|�P�R�O" || s_Text == "�r�w�|�V�O" || s_Text == "�r�w�|�X�O" ||
    // s_Text == "�r�w�|�X�O�l" || s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" ||
    // s_Text == "�q�w�|�X�O" || s_Text == "�q�w�|�P�O�O" || s_Text == "�v�w�|�X�O" ||
    // s_Text == "�b�w�|�P�O" || s_Text == "�b�w�|�P�O�c" || s_Text == "�m�w�|�X�O"){
    ////***********************************************
    if (s_Text == "�t�w�|�W�O" || s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" ||
        s_Text == "�t�w�|�P�O�O�q" || s_Text == "�t�w�|�P�R�O" || s_Text == "�r�w�|�V�O" ||
        s_Text == "�r�w�|�X�O" || s_Text == "�r�w�|�X�O�l" || s_Text == "�r�w�|�X�O�k" ||
        s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O" || s_Text == "�q�w�|�P�O�O" ||
        s_Text == "�v�w�|�X�O" || s_Text == "�b�w�|�P�O" || s_Text == "�b�w�|�P�O�c" ||
        s_Text == "�m�w�|�X�O") {
        // ******************************************
        i_OgataFlg = 1;
        // 2012.08.23 ��^�ڰя������ύX(SX-80S�ǉ�)
    }
    else if (s_Text == "�r�w�|�W�O�r") {
        i_OgataFlg = 1;
        // *****************************************
    }
    // 2017.06.23 LX-90D,SX-80LM�ǉ�
    else if (s_Text == "�k�w�|�X�O�c" || s_Text == "�r�w�|�W�O�k" || s_Text == "�r�w�|�W�O�l" ) {
        i_OgataFlg = 1;
    }
    // *****************************
    // 2019.01.21 LX-50�E���ύX_S
    else if (s_Text == "�k�w�|�T�O" ) {
        i_OgataFlg = 1;
    }
    // 2019.01.21 LX-50�E���ύX_E
    // 2019.01.21 LX-50S�ǉ�_S
    else if (s_Text == "�k�w�|�T�O�r" ) {
        i_OgataFlg = 1;
    }
    // 2019.01.21 LX-50S�ǉ�_E
    // 2021.03.11 RX-30�ύX_S
    else if (s_Text == "�q�w�|�R�O" ) {
        i_OgataFlg = 1;
    }
    // 2021.03.11 RX-30�ύX_E
    // 2023.08.21 UX-10�ύX_S
    //else if (s_Text == "�t�w�|�P�O" || s_Text == "�t�w�|�Q�O" || s_Text == "�t�w�|�R�O" ||
    //    s_Text == "�k�w�|�Q�O" || s_Text == "�r�w�|�S�O" || s_Text == "�r�w�|�S�P" ||
    //    s_Text == "�r�w�|�S�R" || s_Text == "�d�w�|�P�T") {
    //    i_OgataFlg = 2;
    //}
    else if (s_Text == "�t�w�|�P�O") {
        i_OgataFlg = 1;
    }
    else if (s_Text == "�t�w�|�Q�O" || s_Text == "�t�w�|�R�O" ||
        s_Text == "�k�w�|�Q�O" || s_Text == "�r�w�|�S�O" || s_Text == "�r�w�|�S�P" ||
        s_Text == "�r�w�|�S�R" || s_Text == "�d�w�|�P�T") {
        i_OgataFlg = 2;
    }
    // 2023.08.21 UX-10�ύX_E
    else {
        i_OgataFlg = 0;
    }
    // *************************
    // ***************************

    if (Kbn == "S") {
        // *************************
        // ***  S1�@����
        // *************************
        if (s_S1Nzl_Syurui == "0") {
            s_S[1] = "0";
        }
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = s_S1Nzl_Tugite.SubString(2,1);
            s_Text = s_S1Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[1] = "T";
                // 2003.09.09 ��^�t���[���E��
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
                G_ErrLog_Text = "S1ɽ�ٌp��w" + s_S1Nzl_Tugite +
                    "�x�ɂ��AS1�E���ް����ُ�ł��B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  S2�@����
        // *************************
        if (s_S2Nzl_Syurui == "0") {
            s_S[2] = "0";
        }
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = s_S2Nzl_Tugite.SubString(2,1);
            s_Text = s_S2Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[2] = "T";
                // 2003.09.09 ��^�t���[���E��
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
                G_ErrLog_Text = "S2ɽ�ٌp��w" + s_S2Nzl_Tugite +
                    "�x�ɂ��AS2�E���ް����ُ�ł��B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  S3�@����
        // *************************
        if (s_S3Nzl_Syurui == "0") {
            s_S[3] = "0";
        }
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = s_S3Nzl_Tugite.SubString(2,1);
            s_Text = s_S3Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[3] = "T";
                // 2003.09.09 ��^�t���[���E��
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
                G_ErrLog_Text = "S3ɽ�ٌp��w" + s_S3Nzl_Tugite +
                    "�x�ɂ��AS3�E���ް����ُ�ł��B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  S4�@����
        // *************************
        if (s_S4Nzl_Syurui == "0") {
            s_S[4] = "0";
        }
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = s_S4Nzl_Tugite.SubString(2,1);
            s_Text = s_S4Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[4] = "T";
                // 2003.09.09 ��^�t���[���E��
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
                G_ErrLog_Text = "S4ɽ�ٌp��w" + s_S4Nzl_Tugite +
                    "�x�ɂ��AS4�E���ް����ُ�ł��B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // �e�擾���ڌ���
        s_AnaSiyou = "S" + s_S[1] + s_S[2] + s_S[3] + s_S[4];

    }
    else {
        // *************************
        // ***  E1�@����
        // *************************
        if (s_E1Nzl_Syurui == "0") {
            s_E[1] = "0";
        }
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = s_E1Nzl_Tugite.SubString(2,1);
            s_Text = s_E1Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[1] = "T";
                // 2003.09.09 ��^�t���[���E��
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[1] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 �d�t���[���E��
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
                G_ErrLog_Text = "E1ɽ�ٌp��w" + s_E1Nzl_Tugite +
                    "�x�ɂ��AE1�E���ް����ُ�ł��B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  E2�@����
        // *************************
        if (s_E2Nzl_Syurui == "0") {
            s_E[2] = "0";
        }
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = s_E2Nzl_Tugite.SubString(2,1);
            s_Text = s_E2Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[2] = "T";
                // 2003.09.09 ��^�t���[���E��
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[2] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 �d�t���[���E��
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
                G_ErrLog_Text = "E2ɽ�ٌp��w" + s_E2Nzl_Tugite +
                    "�x�ɂ��AE2�E���ް����ُ�ł��B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  E3�@����
        // *************************
        if (s_E3Nzl_Syurui == "0") {
            s_E[3] = "0";
        }
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = s_E3Nzl_Tugite.SubString(2,1);
            s_Text = s_E3Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[3] = "T";
                // 2003.09.09 ��^�t���[���E��
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[3] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 �d�t���[���E��
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
                G_ErrLog_Text = "E3ɽ�ٌp��w" + s_E3Nzl_Tugite +
                    "�x�ɂ��AE3�E���ް����ُ�ł��B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  E4�@����
        // *************************
        if (s_E4Nzl_Syurui == "0") {
            s_E[4] = "0";
        }
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // s_Text = s_E4Nzl_Tugite.SubString(2,1);
            s_Text = s_E4Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[4] = "T";
                // 2003.09.09 ��^�t���[���E��
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[4] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 �d�t���[���E��
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
                G_ErrLog_Text = "E4ɽ�ٌp��w" + s_E4Nzl_Tugite +
                    "�x�ɂ��AE4�E���ް����ُ�ł��B";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // �e�擾���ڌ���
        s_AnaSiyou = "E" + s_E[1] + s_E[2] + s_E[3] + s_E[4];
    }

    return s_AnaSiyou;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �{���g�J�o�[
// �T  �v      �F
// ��  ��      �F AnsiString *P_PARTS �F�W�v�Ώ�ں���
// �߂�l      �F �Ȃ�
// ��  �l      �F ɽ�ٕ��i�ް��ȍ~�Ŏg�p
// ---------------------------------------------------------------------------
void __fastcall TFrameDetails::BLT_Cover_Out(void) {
    int i_RecNo;
    int i_ErrFlg;
    int i_Value_1;
    int i_Value_2;
    int i_Daisuu;
    AnsiString s_Text;
    // 2016.07.25 �{���g�J�o�[���@�ύX
    int i_Value_L; //L���ėp���Ķ�ް����
    int i_Value_S; //S���ėp���Ķ�ް����
    int i_Value_Z; //�㉺���ėp���Ķ�ް����
    // *******************************

    // -----------------------------------------
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    AnsiString sSpecCode;
    // ***************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // ***************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // -----------------------------------------

    // ***************************
    // ***  �{���g�J�o�[
    // ***************************
    // �wC018�x
    // 2004.06.08 �J�o�[�o�͏����ύX
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

    G_Log_Text = "�{���g�J�o�[         �w" + s_Text + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);

    // **********************************************************
    // ***  ���Ė{��L,S�@���l����
    // ***    �wC019�x,�wC020�x,�wC021�x�ׂ̈̏����O����
    // **********************************************************
    i_ErrFlg = 0;
    s_Text = P_HEADS_DATA[39];
    i_Daisuu = s_Text.ToIntDef(0);

    // ���Ė{��L
    i_RecNo = 1286;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (AllFloatChk(s_Text.c_str(), 0) == 0) {
        i_Value_1 = s_Text.ToIntDef(0); // ���Ė{��L
    }
    else {
        G_ErrLog_Text = "���Ė{���k      RecNo:" + FormatFloat("0000", i_RecNo) +
            "  �s���Ȓl�w" + s_Text + "�x �ł��B";
        Write_Error_Log(G_ErrLog_Text); // ÷�Ĵװ۸�
        Excel_Err_DataSet(G_ErrLog_Text); // ���ٴװ۸�

        i_ErrFlg = 1;
    }

    // ���Ė{��S
    i_RecNo = 1287;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (AllFloatChk(s_Text.c_str(), 0) == 0) {
        i_Value_2 = s_Text.ToIntDef(0); // ���Ė{��S
    }
    else {
        G_ErrLog_Text = "���Ė{���r      RecNo:" + FormatFloat("0000", i_RecNo) +
            "  �s���Ȓl�w" + s_Text + "�x �ł��B";
        Write_Error_Log(G_ErrLog_Text); // ÷�Ĵװ۸�
        Excel_Err_DataSet(G_ErrLog_Text); // ���ٴװ۸�

        i_ErrFlg = 1;
    }

    // 2016.07.25 �{���g�J�o�[���@�ύX
    s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    if (s_Text == "�t�w�|�X�O") {
        if (P_HEADS_DATA[1267].TrimRight() == "200207") {
            // HEAD.VER��200207(���ڰ�)
            i_Value_L = 4; //L���ėp���Ķ�ް����
            i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
        }
        else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
            // HEAD.VER��300207(���ڰ�)
            i_Value_L = 4; //L���ėp���Ķ�ް����
            i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
        }
        else {
            // H21
            i_Value_L = 4; //L���ėp���Ķ�ް����
            i_Value_Z = 4; //�㉺���ėp���Ķ�ް����
        }
    }
    else if (s_Text == "�t�w�|�W�O") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�t�w�|�P�O�O") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�t�w�|�P�O�O�q") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�t�w�|�P�P�O�q") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�t�w�|�P�R�O") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�t�w�|�P�R�O�q") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    // 2017.06.23 LX-90D�ǉ�
    else if (s_Text == "�k�w�|�X�O�c") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 2; //�㉺���ėp���Ķ�ް����
        if ( P_HEADS_DATA[37].ToIntDef(0) == 94 ){
            // LX-90D-KNUP
            i_Value_2 = 12; // ���Ė{��S
        } else if ( P_HEADS_DATA[37].ToIntDef(0) == 394 ){
            // LX-90D-TNUP
            i_Value_2 = 12; // ���Ė{��S
        }
    }
    // *********************
    else if (s_Text == "�r�w�|�X�O�l") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�r�w�|�X�O�k") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�q�w�|�X�O") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�q�w�|�P�O�O") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�v�w�|�X�O") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else if (s_Text == "�r�w�|�W�O�r") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        // 2017.06.23 ���ʕύX
        //i_Value_Z = 4; //�㉺���ėp���Ķ�ް����
        i_Value_Z = 2; //�㉺���ėp���Ķ�ް����
        // *******************�@
    }
    else if (s_Text == "�r�w�|�W�O�l") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        // 2017.06.23 ���ʕύX
        //i_Value_Z = 4; //�㉺���ėp���Ķ�ް����
        i_Value_Z = 2; //�㉺���ėp���Ķ�ް����
        // *******************�@
    }
    else if (s_Text == "�r�w�|�W�O�k") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        // 2017.06.23 ���ʕύX
        i_Value_Z = 4; //�㉺���ėp���Ķ�ް����
        //i_Value_Z = 2; //�㉺���ėp���Ķ�ް����
        // *******************�@
    }
    else if (s_Text == "�r�w�|�V�O") {
        i_Value_L = 4; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }
    else {
        i_Value_L = 0; //L���ėp���Ķ�ް����
        i_Value_Z = 0; //�㉺���ėp���Ķ�ް����
    }

    //S�y�я㉺S���ėp���Ķ�ް���ʂ��ݒ肳��Ă���ꍇ�����D��
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
    // �wC019�x,�wC020�x
    if (i_ErrFlg == 0) {
        i_RecNo = 1092; // BLCP1
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCP1 �wC019�x
            s_Text = s_Text + "L";
            P_Siyou1 = HanToZen(s_Text); // �d�l�P
            G_Log_Text = "BLCP1                �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // BLCP1�{�� �wC020�x
            // 2016.07.25 �{���g�J�o�[���@�ύX
            //P_1DaiSuryo = i_Value_1 + i_Value_2;
            P_1DaiSuryo = i_Value_L + i_Value_S + i_Value_Z;
            // *******************************
            P_Yobisu = 0; // �\����
            P_Soukei = P_1DaiSuryo * i_Daisuu; // ���v
            G_Log_Text = "BLCP1 �{��           �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // 1  ���i����
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ���ʎq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  ���i�^��
            s_Text = ",���t�{���g�J�o�[";
            fputs(s_Text.c_str(), fp);
            // 4  ���i����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  �}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ��}�p�}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1�䕪����
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  �\����
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 �d�l�P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 2014/08/22 ���p�Z�p�ҏW
            // 12 �d�l�Q
            // 2014.12.19 ���Ča����
            //s_Text = ",�l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "�p";
             s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",�P�@�T�^�W�|�W�t�m�p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",�Q�|�W�t�m�p";
            } else {
                s_Text = ",�l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "�p";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 �d�l�R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 �d�l�S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 �d�l�T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // ------------------------------
            // 2007.10.03 �װ�d�l�Ή�
            // 17 �d�l���ߺ���
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);
            // 2007.10.03
            // ------------------------------

            // ���s����
            s_Text = "\n";
            // CSV�ް��@̧�ُo��
            fputs(s_Text.c_str(), fp);
        }
    }
    // ***************************
    // ***  BLCEL
    // ***************************
    // �wC021�x,�wC022�x
    if (i_ErrFlg == 0) {
        i_RecNo = 1096; // BLCEL
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCEL �wC021�x
            s_Text = s_Text + "L �L���b�v�t";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCEL                �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // BLCEL�{�� �wC022�x
            // 2016.07.25 �{���g�J�o�[���@�ύX
            //P_1DaiSuryo = i_Value_1;
            P_1DaiSuryo = i_Value_L;
            // *******************************
            P_Yobisu = 0; // �\����
            P_Soukei = P_1DaiSuryo * i_Daisuu; // ���v
            G_Log_Text = "BLCEL �{��           �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // 1  ���i����
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ���ʎq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  ���i�^��
            s_Text = ",���t�{���g�J�o�[";
            fputs(s_Text.c_str(), fp);
            // 4  ���i����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  �}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ��}�p�}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1�䕪����
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  �\����
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 �d�l�P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 2014/08/22 ���p�Z�p�ҏW
            // 12 �d�l�Q
            // 2014.12.19 ���Ča����
            //s_Text = ",�l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "�p";
            s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",�P�@�T�^�W�|�W�t�m�p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",�Q�|�W�t�m�p";
            } else {
                s_Text = ",�l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "�p";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 �d�l�R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 �d�l�S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 �d�l�T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // ------------------------------
            // 2007.10.03 �װ�d�l�Ή�
            // 17 �d�l���ߺ���
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);
            // 2007.10.03
            // ------------------------------

            // ���s����
            s_Text = "\n";
            // CSV�ް��@̧�ُo��
            fputs(s_Text.c_str(), fp);
        }
    }

    // ***************************
    // ***  BLCES
    // ***************************
    // �wC023�x,�wC024�x
    if (i_ErrFlg == 0) {
        i_RecNo = 1097; // BLCES
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCEL �wC023�x
            s_Text = s_Text + "L �L���b�v�t";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCES                �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // BLCEL�{�� �wC024�x
            // 2016.07.25 �{���g�J�o�[���@�ύX
            //P_1DaiSuryo = i_Value_2;
            P_1DaiSuryo = i_Value_S;
            // *******************************
            P_Yobisu = 0; // �\����
            P_Soukei = P_1DaiSuryo * i_Daisuu; // ���v
            G_Log_Text = "BLCES �{��           �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // 1  ���i����
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ���ʎq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  ���i�^��
            s_Text = ",���t�{���g�J�o�[";
            fputs(s_Text.c_str(), fp);
            // 4  ���i����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  �}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ��}�p�}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1�䕪����
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  �\����
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 �d�l�P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 2014/08/22 ���p�Z�p�ҏW
            // 12 �d�l�Q
            // 2014.12.19 ���Ča����
            //s_Text = ",�l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "�p";
            s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",�P�@�T�^�W�|�W�t�m�p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",�Q�|�W�t�m�p";
            } else {
                s_Text = ",�l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "�p";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 �d�l�R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 �d�l�S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 �d�l�T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // ------------------------------
            // 2007.10.03 �װ�d�l�Ή�
            // 17 �d�l���ߺ���
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);
            // 2007.10.03
            // ------------------------------

            // ���s����
            s_Text = "\n";
            // CSV�ް��@̧�ُo��
            fputs(s_Text.c_str(), fp);
        }
    }

    // 2016.07.25 �{���g�J�o�[���@�ύX
    // ***************************
    // ***  BLCESS
    // ***************************
    if (i_ErrFlg == 0) {
        i_RecNo = 1112; // BLCESS
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if ((s_Text.ToIntDef(0) != 0)&& (i_Value_Z != 0)) {
            // BLCESS
            s_Text = s_Text + "L �L���b�v�t";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCESS                �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // BLCESS�{��
            P_1DaiSuryo = i_Value_Z;
            P_Yobisu = 0;                       // �\����
            P_Soukei = P_1DaiSuryo * i_Daisuu;  // ���v
            G_Log_Text = "BLCESS �{��           �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // 1  ���i����
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ���ʎq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  ���i�^��
            s_Text = ",���t�{���g�J�o�[";
            fputs(s_Text.c_str(), fp);
            // 4  ���i����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  �}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ��}�p�}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1�䕪����
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  �\����
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 �d�l�P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",�P�@�T�^�W�|�W�t�m�p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",�Q�|�W�t�m�p";
            } else {
                s_Text = ",�l" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "�p";
            }
            fputs(s_Text.c_str(), fp);
            // 13 �d�l�R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 �d�l�S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 �d�l�T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 17 �d�l���ߺ���
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);

            // ���s����
            s_Text = "\n";
            // CSV�ް��@̧�ُo��
            fputs(s_Text.c_str(), fp);

        }
    }
    // *******************************

}

// ---------------------------------------------------------------------------
// ���{��֐����F �{���g�J�o�[�o��(���a�{���g)
// �T  �v      �F BLCP1S��BLCESS���o�͂���B
// ��  ��      �F
// �߂�l      �F �Ȃ�
// ��  �l      �F BLT_Cover_Out(void)�𓥏P���č쐬�B
// ---------------------------------------------------------------------------
// 2014/08/22 ���p�Z�p�ҏW
void __fastcall TFrameDetails::BLT_Cover_Out_Small(void) {
    int i_RecNo;
    int i_ErrFlg;
    int i_Value;
    int i_Daisuu;
    AnsiString s_Text;

    // ***************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // ***************************
    AnsiString sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());

    // ***************************
    // ***  �{���g�J�o�[
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

    G_Log_Text = "���a�{���g�J�o�[         �w" + s_Text + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);

    // **********************************************************
    // ***  ���Ė{�����l����
    // **********************************************************
    i_ErrFlg = 0;
    s_Text = P_HEADS_DATA[39];
    i_Daisuu = s_Text.ToIntDef(0);

    // ���a�{���g�{��S
    i_RecNo = 1113;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (AllFloatChk(s_Text.c_str(), 0) == 0) {
        i_Value = s_Text.ToIntDef(0); // ���a�{���g�{��S
    }
    else {
        G_ErrLog_Text = "���a���Ė{���r      RecNo:" + FormatFloat("0000", i_RecNo) +
            "  �s���Ȓl�w" + s_Text + "�x �ł��B";
        Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
        Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

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
            P_Siyou1 = HanToZen(s_Text); // �d�l�P
            G_Log_Text = "BLCP1S               �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // BLCP1S�{��
            P_1DaiSuryo = i_Value;
            P_Yobisu = 0;                       // �\����
            P_Soukei = P_1DaiSuryo * i_Daisuu;  // ���v
            G_Log_Text = "BLCP1S �{��           �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // 1  ���i����
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ���ʎq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  ���i�^��
            s_Text = ",���t�{���g�J�o�[";
            fputs(s_Text.c_str(), fp);
            // 4  ���i����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  �}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ��}�p�}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1�䕪����
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  �\����
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 �d�l�P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 12 �d�l�Q
            // 2014.12.19 ���Ča����
            //s_Text = ",�l" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "�p";
            s_Text = P_HEADS_DATA[1108].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",�P�@�T�^�W�|�W�t�m�p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",�Q�|�W�t�m�p";
            } else {
                s_Text = ",�l" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "�p";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 �d�l�R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 �d�l�S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 �d�l�T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // 17 �d�l���ߺ���
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);

            // ���s����
            s_Text = "\n";
            // CSV�ް��@̧�ُo��
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
            s_Text = s_Text + "L �L���b�v�t";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCESS                �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // BLCESS�{��
            P_1DaiSuryo = i_Value;
            P_Yobisu = 0;                       // �\����
            P_Soukei = P_1DaiSuryo * i_Daisuu;  // ���v
            G_Log_Text = "BLCESS �{��           �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            // 1  ���i����
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  ���ʎq
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  ���i�^��
            s_Text = ",���t�{���g�J�o�[";
            fputs(s_Text.c_str(), fp);
            // 4  ���i����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  �}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  ��}�p�}��
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1�䕪����
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  �\����
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 �d�l�P
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 12 �d�l�Q
            // 2014.12.19 ���Ča����
            //s_Text = ",�l" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "�p";
            s_Text = P_HEADS_DATA[1108].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",�P�@�T�^�W�|�W�t�m�p";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",�Q�|�W�t�m�p";
            } else {
                s_Text = ",�l" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "�p";
            }
            //**********************
            fputs(s_Text.c_str(), fp);
            // 13 �d�l�R
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 �d�l�S
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 �d�l�T
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 �ގ�����
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // 17 �d�l���ߺ���
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);

            // ���s����
            s_Text = "\n";
            // CSV�ް��@̧�ُo��
            fputs(s_Text.c_str(), fp);
        }
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F �{���g�\���i�擾����
// �T  �v      �F
// ��  ��      �F �{���g�T�C�Y�@�ގ��R�[�h
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
int __fastcall TFrameDetails::Get_Blt_YobiSu(double d_BltSz, double d_BltLen,
    AnsiString MatCD, HeadsData* pcHeadsData) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;
    AnsiString s_YobiName;
    int i_Yobisu;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_BLT2YOBI";
    s_SQL = s_SQL + "  WHERE BLTSIZE = " + d_BltSz;

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        s_YobiName = "";
    }
    else {
        s_YobiName = wkQuery->FieldByName("YOBINAME")->AsString.TrimRight();
        // �\���i���ʎq

        G_Log_Text = "�\���i���ʎq    �" + s_YobiName + "����擾�B";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;

    // CSV�Ǎ��p
    FILE *wkfp;

    char buf[1024]; // ������o�b�t�@
    char *tok; // �g�[�N���|�C���^
    int i; // ���[�v�J�E���^
    char token[100][100]; // �g�[�N���o�b�t�@

    AnsiString s_Yobi_Text[5];
    AnsiString s_YBKey;
    AnsiString s_YBZaisitu;

    AnsiString wk_FilePass; // "D00001.csv"
    int i_RecNo;
    int i_lop;

    i_RecNo = 0; // ں��޶���(���o������v����)
    i_Yobisu = 0;

    // --------------------------------------------
    // 2007.10.04 �װ�d�l�Ή�
    // �װB�d�l�̏ꍇ�A�\���i�͌v�サ�Ȃ�
    if (pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B)
        return 0;
    // 2007.10.04
    // --------------------------------------------

    // ��x�v�サ���\���i�͌v�サ�Ȃ�
    for (i_lop = 0; i_lop < G_YOBICHK_RecCnt; i_lop++) {
        if ((s_YobiName == G_YOBICHK_NAME[i_lop]) &&
            (MatCD == G_YOBICHK_MAT[i_lop])) {
            return 0;
        }
    }

    // ��ڰė\���i�ް��߽
    wk_FilePass = G_AnaFilePass + G_SAKUBAN + ".csv";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�\���i�ް� �w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // �Ǎ��݉\�̊�
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), wkfp) != NULL) {
            i = 0; // �g�[�N���J�E���^������
            memset(token, 0x00, sizeof(token)); // �g�[�N���o�b�t�@������
            tok = GetToken(buf, ","); // ","�ō��ڂ���؂�
            while (tok) { // �g�[�N�������݂����
                sprintf(token[i++], "%-.99s", tok); // �g�[�N�����o�b�t�@�ɓ]��
                tok = GetToken(NULL, ","); // ���̃g�[�N��������

                if (i > 100) {
                    break;
                }
            }
            // �ް���
            s_Yobi_Text[0] = token[0];
            // �\���i�ް� 1��� ( 'PLTE','GSKT','ADHS','THRM'... )
            s_Yobi_Text[1] = token[1];
            // �\���i�ް� 2��� ( ���ݸ,'D','E','1','2'... )
            s_Yobi_Text[2] = token[8]; // �\���i�ް� �\����( 10, 1, 5, 2, 8, ... )
            s_Yobi_Text[3] = token[11]; // �\���i�ް� �ގ��R�[�h( 1011, ... )
            s_Yobi_Text[4] = token[24]; // �\���i�ް� �S��

            // ����������
            if (AllNumChk(s_Yobi_Text[1].c_str(), 0) == 0) {
                int i_No;

                i_No = StrToInt(s_Yobi_Text[1]) + 100;
                s_YBKey = s_Yobi_Text[0] + IntToStr(i_No).SubString(2, 2);
                s_YBZaisitu = s_Yobi_Text[3].TrimRight();
            }
            else {
                // ********************************************
                // �\���i�ް� 2��ڂ�
                // ��̧�ޯĂ�������NULL�������ꍇ �������Ȃ�?
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

    // �Y���\���i�ް������݂��Ȃ������ꍇ
    if (i_RecNo == 0)
        i_Yobisu = 0;

    // �\���i�ް� ̧�ٸ۰��
    fclose(wkfp);
    return i_Yobisu;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �X�y�[�T�[�E���d�l �擾
// �T  �v      �F
// ��  ��      �F Kbn �FS���S�ڰїp�����CE���E�ڰя���
// �߂�l      �F ��������
// ��  �l      �F 2022.01.14�ǉ�
// ---------------------------------------------------------------------------
AnsiString __fastcall TFrameDetails::Get_AnaakeSiyou_S(AnsiString Kbn) {
    AnsiString s_Text;

    AnsiString s_S1Nzl_Syurui; // S1ɽ�َ��
    AnsiString s_S2Nzl_Syurui; // S2ɽ�َ��
    AnsiString s_S3Nzl_Syurui; // S3ɽ�َ��
    AnsiString s_S4Nzl_Syurui; // S4ɽ�َ��

    AnsiString s_E1Nzl_Syurui; // E1ɽ�َ��
    AnsiString s_E2Nzl_Syurui; // E2ɽ�َ��
    AnsiString s_E3Nzl_Syurui; // E3ɽ�َ��
    AnsiString s_E4Nzl_Syurui; // E4ɽ�َ��

    AnsiString s_AnaSiyou; // �E���d�l
    AnsiString s_S1;
    AnsiString s_S2;
    AnsiString s_S3;
    AnsiString s_S4;
    AnsiString s_S[5];
    AnsiString s_E[5];

    int i_OgataFlg; // 0:���^���^,1:��^

    // *************************
    // ***  HEADS�ް��@���
    // *************************
    if (Kbn == "S") {
        s_S1Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1ɽ�َ��
        s_S2Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2ɽ�َ��
        s_S3Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ɽ�َ��
        s_S4Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ɽ�َ��
    }
    else {
        s_E1Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1ɽ�َ��
        s_E2Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2ɽ�َ��
        s_E3Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ɽ�َ��
        s_E4Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ɽ�َ��
    }


    if (Kbn == "S") {
        // *************************
        // ***  S1�@����
        // *************************
        if (s_S1Nzl_Syurui == "0") {
            s_S[1] = "0";
        }
        else {
            s_S[1] = "1";
        }

        // *************************
        // ***  S2�@����
        // *************************
        if (s_S2Nzl_Syurui == "0") {
            s_S[2] = "0";
        }
        else {
            s_S[2] = "2";
        }

        // *************************
        // ***  S3�@����
        // *************************
        if (s_S3Nzl_Syurui == "0") {
            s_S[3] = "0";
        }
        else {
            s_S[3] = "3";
        }

        // *************************
        // ***  S4�@����
        // *************************
        if (s_S4Nzl_Syurui == "0") {
            s_S[4] = "0";
        }
        else {
            s_S[4] = "4";
        }

        // �e�擾���ڌ���
        s_AnaSiyou = "S" + s_S[1] + s_S[2] + s_S[3] + s_S[4];

    }
    else {
        // *************************
        // ***  E1�@����
        // *************************
        if (s_E1Nzl_Syurui == "0") {
            s_E[1] = "0";
        }
        else {
            s_E[1] = "1";
        }

        // *************************
        // ***  E2�@����
        // *************************
        if (s_E2Nzl_Syurui == "0") {
            s_E[2] = "0";
        }
        else {
            s_E[2] = "2";
        }

        // *************************
        // ***  E3�@����
        // *************************
        if (s_E3Nzl_Syurui == "0") {
            s_E[3] = "0";
        }
        else {
            s_E[3] = "3";
        }

        // *************************
        // ***  E4�@����
        // *************************
        if (s_E4Nzl_Syurui == "0") {
            s_E[4] = "0";
        }
        else {
            s_E[4] = "4";
        }

        // �e�擾���ڌ���
        s_AnaSiyou = "E" + s_E[1] + s_E[2] + s_E[3] + s_E[4];
    }

    return s_AnaSiyou;
}

// ---------------------------------------------------------------------------
// ���{��֐����F S�ڰѶ޽���ް��쐬���� ���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2022.05.23�ǉ�
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
    // ***  �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�@����
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "�������w" + P_Key + "�x�ŁA�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�������B";
    Write_Log(G_Log_Text);

    // seek_ret���Ͻ��ɑ��݂��������Ȃ���
    // ���� Write_SE_Frame_Data()�Ŏg�p����B
    seek_ret = Search_HD_FRM_NZ(P_Key, "SG");

    G_Log_Text = "�������w" + P_Key + "�x�ŁA�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�������I���B";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  �ގ����̂̌���(ASME���܂܂�Ă��邩)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "�ގ��w" + P_ZaisituNM + "�x���AASME�ł��B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("�`�r�l�d") != 0) {
            G_ErrLog_Text = "�ގ��w" + P_ZaisituNM + "�x���A�`�r�l�d�ł��B";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  �E���d�l�̍쐬
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("S");
    G_Log_Text = "�E���d�l  �w" + P_Siyou1 + "�x���擾�B";
    Write_Log(G_Log_Text);

    // *************************
    // ***  �ގ��̎擾
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
    case 64: // �a�V���[�Y
        if (P_ZaisituNM == "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        break;
    // 2023.03.09 CX-00-Y�ǉ�_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y�ǉ�_E
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
    case 354: // �x�V���[�Y
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "�r�Q�V�T�i�q";
            P_EZaisituNM = "�r�Q�V�T�i�q";
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
            P_ZaisituNM = "�r�r�S�O�O";
            P_EZaisituNM = "�r�r�S�O�O";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }

        break;
    }

    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    if (s_Text == "�t�w�|�P�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�O�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�P�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�R�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�T�O") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�Q�O") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�S�P") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�S�R") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�P�P") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�P�R") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�R�O") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�T�O") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�V�O") { // �����ގ�
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // �����ގ�
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "SS400") {
        }
        else if (P_ZaisituNM == "�r�r�S�O�O" || P_ZaisituNM == "�b�D�r�D") {
            P_ZaisituNM = "�p�Q�R�T�a";
            P_EZaisituNM = "�p�Q�R�T�a";
        }
    }

    // **********************************************
    // ***  �}�ԁE��}�p�}�Ԃ̎擾  02.09.03�ǉ�
    // **********************************************
    bSUS = false;
    sPltMat = P_HEADS_DATA[43].TrimRight();
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // �v���[�g�ގ����`�^���n�ȊO
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
    // ***  �\�����̎擾
    // *************************
    Syukei_Frame_Yobisu("S");

    // *************************
    // ***  S�ڰ��ް��̏o��
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
// ���{��֐����F E�ڰѶ޽���ް��쐬���� ���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2022.05.23�ǉ�
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
    // ***  �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�@����
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "�������w" + P_Key + "�x�ŁA�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�������B";
    Write_Log(G_Log_Text);

    // seek_ret���Ͻ��ɑ��݂��������Ȃ���
    // Write_SE_Frame_Data()�Ŏg�p����B
    seek_ret = Search_HD_FRM_NZ(P_Key, "EG");

    G_Log_Text = "�������w" + P_Key + "�x�ŁA�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�������I���B";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  �ގ����̂̌���(ASME���܂܂�Ă��邩)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "�ގ��w" + P_ZaisituNM + "�x���AASME�ł��B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("�`�r�l�d") != 0) {
            G_ErrLog_Text = "�ގ��w" + P_ZaisituNM + "�x���A�`�r�l�d�ł��B";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  �E���d�l�̍쐬
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("E");
    G_Log_Text = "�E���d�l  �w" + P_Siyou1 + "�x���擾�B";
    Write_Log(G_Log_Text);

    // *************************
    // ***  �ގ��̎擾
    // *************************
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);

    // **********************************************
    // ***  �}�ԁE��}�p�}�Ԃ̎擾  02.09.03�ǉ�
    // **********************************************
    bSUS = false;
    sPltMat = P_HEADS_DATA[43].TrimRight();
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // �v���[�g�ގ����`�^���n�ȊO
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

    switch (P_HEADS_DATA[37].ToIntDef(0)) { // �t���[���R�[�h
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // �a�V���[�Y
        if (P_ZaisituNM == "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        break;
    // 2023.03.09 CX-00-Y�ǉ�_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y�ǉ�_E
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
    case 354: // �x�V���[�Y
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "�r�Q�V�T�i�q";
            P_EZaisituNM = "�r�Q�V�T�i�q";
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
            P_ZaisituNM = "�r�r�S�O�O";
            P_EZaisituNM = "�r�r�S�O�O";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }

        break;
    }

    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "�|" + G_KS_Model;
    if (s_Text == "�t�w�|�P�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�O�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�P�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�R�O") { // �����ގ�
    }
    else if (s_Text == "�k�w�|�T�O") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�Q�O") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�S�P") { // �����ގ�
    }
    else if (s_Text == "�r�w�|�S�R") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�P�P") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�P�R") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�R�O") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�T�O") { // �����ގ�
    }
    else if (s_Text == "�q�w�|�V�O") { // �����ގ�
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // �����ގ�
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "SS400") {
        }
        else if (P_ZaisituNM == "�r�r�S�O�O" || P_ZaisituNM == "�b�D�r�D") {
            P_ZaisituNM = "�p�Q�R�T�a";
            P_EZaisituNM = "�p�Q�R�T�a";
        }
    }

    // *************************
    // ***  �\�����̎擾
    // *************************
    Syukei_Frame_Yobisu("E");

    // *************************
    // ***  E�ڰ��ް��̏o��
    // *************************
    if ( P_DNO[1] == "NONE" ) {
    }
    else {
        Write_SE_Frame_Data("EG", seek_ret);
    }

    delete wkQuery;

    return true;
}





