// ---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�F���͌v���i�W�J����  ���W���[��
// �� �� �ҁFT.Kawai
// �� �� ���F2002.08.06
// �X �V ���F2002.09.04
// 2007.10.03 �װ�d�l�Ή�
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop

#include <io.h>                                       "
#include <Filectrl.hpp>

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_PressGDetails.h"

#include "HeadsData.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// ���͌v���i�W�J���� Class
// ---------------------------------------------------------------------------
class TPressGDetails {
private:
    FILE *fp;

    // CSV̧�ق���擾
    AnsiString P_Sikibetu; // ���ʎq
    AnsiString P_Katasiki; // ���i�^��
    AnsiString P_Yobihin; // �\���i��

    // Ͻ�����擾
    AnsiString P_Zaisitu; // �ގ�����
    AnsiString P_EZaisitu; // �ގ�����
    AnsiString P_Zuban; // �}��
    AnsiString P_SZuban; // ��}�p�}��
    int P_Num; // ����
    int P_Yobisu; // �\����
    AnsiString P_Siyou; // �d�l
    AnsiString P_Unit; // �P��

    // ------------------------------
    // 2007.10.03 �װ�d�l�Ή�
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // 2007.10.03
    // ------------------------------

    bool __fastcall PressG_Data_Sakusei(void); // ���͌v�ް� �쐬
    bool __fastcall Get_PressG_Data(AnsiString a_SE, AnsiString Syurui);
    // ���͌v�ް��擾
    bool __fastcall Search_PRESS_CODE(AnsiString Code); // ���͌v
    AnsiString __fastcall Search_HD_PGAGE(AnsiString a_SE,
        AnsiString Key); // ���͌v

    // bool __fastcall Write_Press_Data(AnsiString a_SE);
    bool __fastcall Write_PressG_Data(AnsiString a_SE);

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // ���͌v���i�W�J���� ���s
    int __fastcall Get_Press_YobiSu(int i_TmpCd); // ���͌v�\���i�擾

};

// ---------------------------------------------------------------------------
// ���{��֐����F ���͌v���i�W�J����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall PressGDetails(HeadsData* pcHeadsData) {
    bool ret;

    TPressGDetails pressGDetails; // ���͌v�ް� �쐬Class��������,�֐��I����j��

    ret = pressGDetails.Excute(pcHeadsData); // ���͌v�ް� �쐬���s

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���͌v���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADS÷���ް����߲�����
    m_pcHeadsData = pcHeadsData;

    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********     �w���͌v���i�W�J�����x�J�n     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     �w���͌v���i�W�J�����x�J�n     *********");
    Write_Error_Log("*******************************************************");

    // ***********************************
    // ***  ���͌v�ް��쐬
    // ***********************************
    PressG_Data_Sakusei();

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********     �w���͌v���i�W�J�����x�I��     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     �w���͌v���i�W�J�����x�I��     *********");
    Write_Error_Log("*******************************************************");
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���͌v�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::PressG_Data_Sakusei(void) {
    AnsiString s_Prs_Syurui; // ���͌v���
    // 2003.12.24 YX-80 �ǉ�
    AnsiString s_Text;
    // *********************
    // 2017.07.11 �΍s���ǉ�
    int iFL;
    // *********************


    // 2017.07.11 �΍s���ǉ�
    // ****************************************************************
    // ***                                                          ***
    // ***                   ��      �s      ��                     ***
    // ***                                                          ***
    // ****************************************************************
   // 2022.04.21 CX-01D�ǉ�_S
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c") {
        iFL = 1; // �΍s��
    }
    //if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
    else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
    // 2022.04.21 CX-01D�ǉ�_S
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
        iFL = 1; // �΍s��
    }
    else {
        iFL = 0; // ��`��
    }
    // *********************


    // ****************************************************************
    // ***                                                          ***
    // ***               �a�����͌v�f�[�^�@�쐬                     ***
    // ***                                                          ***
    // ****************************************************************

    // ***********************************
    // ***  S1���͌v�ް��@�쐬
    // ***********************************
    s_Prs_Syurui = P_HEADS_DATA[901].TrimRight(); // S1���͌v���


    if (s_Prs_Syurui == "00") {
        G_Log_Text = "S1���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    else {
        Get_PressG_Data("S1", s_Prs_Syurui);
    }

    // 2003.12.24 YX-80 �ǉ�
    ////***********************************
    ////***  S3���͌v�ް��@�쐬
    ////***********************************
    // s_Prs_Syurui = P_HEADS_DATA[931].TrimRight();   // S3���͌v���
    //
    // if (s_Prs_Syurui == "00"){
    // G_Log_Text = "S3���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17"){
    // G_Log_Text = "S3���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_PressG_Data("S3", s_Prs_Syurui);
    // }
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // �΍s��
    // *********************
        // ***********************************
        // ***  S4���͌v�ް��@�쐬
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[946].TrimRight(); // S4���͌v���

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "S4���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "S4���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("S4", s_Prs_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  S3���͌v�ް��@�쐬
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[931].TrimRight(); // S3���͌v���

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "S3���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "S3���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("S3", s_Prs_Syurui);
        }
    }
    // *********************

    // ***********************************
    // ***  E1���͌v�ް��@�쐬
    // ***********************************
    s_Prs_Syurui = P_HEADS_DATA[961].TrimRight(); // E1���͌v���

    if (s_Prs_Syurui == "00") {
        G_Log_Text = "E1���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    else {
        Get_PressG_Data("E1", s_Prs_Syurui);
    }

    // 2003.12.24 YX-80 �ǉ�
    ////***********************************
    ////***  E3���͌v�ް��@�쐬
    ////***********************************
    // s_Prs_Syurui = P_HEADS_DATA[991].TrimRight();   // E3���͌v���
    //
    // if (s_Prs_Syurui == "00"){
    // G_Log_Text = "E3���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17"){
    // G_Log_Text = "E3���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_PressG_Data("E3", s_Prs_Syurui);
    // }
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // �΍s��
    // *********************
        // ***********************************
        // ***  E4���͌v�ް��@�쐬
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[1006].TrimRight(); // E4���͌v���

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "E4���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "E4���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("E4", s_Prs_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  E3���͌v�ް��@�쐬
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[991].TrimRight(); // E3���͌v���

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "E3���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "E3���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("E3", s_Prs_Syurui);
        }
    }
    // *********************

    // ****************************************************************
    // ***                                                          ***
    // ***               �`�����͌v�f�[�^�@�쐬                     ***
    // ***                                                          ***
    // ****************************************************************

    // ***********************************
    // ***  S2���͌v�ް��@�쐬
    // ***********************************
    s_Prs_Syurui = P_HEADS_DATA[916].TrimRight(); // S2���͌v���

    if (s_Prs_Syurui == "00") {
        G_Log_Text = "S2���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    else {
        Get_PressG_Data("S2", s_Prs_Syurui);
    }

    // 2003.12.24 YX-80 �ǉ�
    ////***********************************
    ////***  S4���͌v�ް��@�쐬
    ////***********************************
    // s_Prs_Syurui = P_HEADS_DATA[946].TrimRight();   // S4���͌v���
    //
    // if (s_Prs_Syurui == "00"){
    // G_Log_Text = "S4���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_PressG_Data("S4", s_Prs_Syurui);
    // }
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // �΍s��
    // *********************
        // ***********************************
        // ***  S3���͌v�ް��@�쐬
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[931].TrimRight(); // S3���͌v���

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "S3���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "S3���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("S3", s_Prs_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  S4���͌v�ް��@�쐬
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[946].TrimRight(); // S4���͌v���

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "S4���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "S4���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("S4", s_Prs_Syurui);
        }
    }
    // *********************

    // ***********************************
    // ***  E2���͌v�ް��@�쐬
    // ***********************************
    s_Prs_Syurui = P_HEADS_DATA[976].TrimRight(); // E2���͌v���

    if (s_Prs_Syurui == "00") {
        G_Log_Text = "E2���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    else {
        Get_PressG_Data("E2", s_Prs_Syurui);
    }

    // 2003.12.24 YX-80 �ǉ�
    ////***********************************
    ////***  E4���͌v�ް��@�쐬
    ////***********************************
    // s_Prs_Syurui = P_HEADS_DATA[1006].TrimRight();  // E4���͌v���
    //
    // if (s_Prs_Syurui == "00"){
    // G_Log_Text = "E4���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_PressG_Data("E4", s_Prs_Syurui);
    // }
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // �΍s��
    // *********************
        // ***********************************
        // ***  E3���͌v�ް��@�쐬
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[991].TrimRight(); // E3���͌v���

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "E3���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "E3���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("E3", s_Prs_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  E4���͌v�ް��@�쐬
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[1006].TrimRight(); // E4���͌v���

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "E4���͌v�ް�  ���͌v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "E4���͌v�ް�  �����v�Ȃ̂ŉ����͍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("E4", s_Prs_Syurui);
        }
    }
    // *********************

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���͌v���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F AnsiString a_SE   �F ���͌vSE�敪(S1�`S4,E1�`E4)
// �F AnsiString Syurui �F ���͌v���
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::Get_PressG_Data(AnsiString a_SE,
    AnsiString Syurui) {
    AnsiString s_Code; // ���i����

    // *********************************************
    // ***  ���͌v���i�\��Ͻ�(HD_TEMP) ����
    // *********************************************
    s_Code = Search_HD_PGAGE(a_SE, Syurui);

    if (s_Code == "\0") {
        G_ErrLog_Text = Syurui + "�ް��쐬���@" + a_SE + "���͌v��� �" + Syurui +
            "� �́A���͌v���i�\��Ͻ�(HD_TEMP)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);

        return false;
    }

    return true;

}

// ---------------------------------------------------------------------------
// ���{��֐����F HD_PGAGEð��ٌ�������
// �T  �v      �F
// ��  ��      �F AnsiString a_SE   �F ���͌vSE�敪(S1�`S4,E1�`E4)
// �F AnsiString Key    �F ���͌v���
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall TPressGDetails::Search_HD_PGAGE(AnsiString a_SE,
    AnsiString Key) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;
    AnsiString s_Code;
    AnsiString s_BaseMatM; // �����n��ގ�
    AnsiString s_BaseMatL; // ������n��ގ�
    AnsiString s_Memori;
    AnsiString s_SaniSize; // �T�j�^���m�Y�����a
    // 2020.01.14 �ݸޒ[�ǕύX_S
    AnsiString s_FrngSize; // �m�Y���t�����W���a
    // 2020.01.14 �ݸޒ[�ǕύX_E
    // 2003.12.24 YX-80 �ǉ�
    AnsiString s_Text;
    // *********************
    // 2012.03.15 ���ǋȊǔ��f�ύX
    AnsiString s_Tmp_Syurui; // ���x�v���
    AnsiString s_BOOTS; // �ް��(N:1,T:2,K:3,P:4)
    bool b_Tmp; // ���x�v�L��
    bool b_YX; // ���͌v�z�ǋȊ�ɽ��
    bool b_STorCU; // ���͌v�z��(����:true,�Ȋ�:false)
    // ***************************
    bool b_NZorFR;
    int i_wYobi;
    // 2017.07.11 �΍s���ǉ�
    int iFL;
    // *********************

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PGAGE";
    s_SQL = s_SQL + "  WHERE PRES_TYPE = '" + Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // 2008.03.21 ���͌v��t���װ�d�l�Ή�
    AnsiString sSpecCode; // �d�l���ߺ���
    // **********************************************
    // �d�l���߂��擾
    // **********************************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // **********************************

    // �e���ڂ̏�����
    s_Code = "";
    P_Zuban = "";
    P_SZuban = "";
    P_Num = 0;
    P_Zaisitu = "";
    P_Siyou = "";

    // 2017.07.11 �΍s���ǉ�
    // �΍s��
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
        iFL = 1; // �΍s��
    }
    else {
        iFL = 0; // ��`��
    }
    // *********************

    if (a_SE == "S1") {
        if (P_HEADS_DATA[1136].ToIntDef(0) != 0) {
            // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
            s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
            // *********************************
            b_NZorFR = true;
        }
        else {
            s_BaseMatM = P_HEADS_DATA[894].TrimRight();
            // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
            // 2016.11.07 �ގ����ޕύX
            //if (s_BaseMatM.ToIntDef(0) >= 2000) {
            if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                (s_BaseMatM.ToIntDef(0) < 3000)) {
            // ***********************
                s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
            // *************************************************
            // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
            ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
            b_NZorFR = false;
            // s_Text = P_HEADS_DATA[893].TrimRight();
            // if(s_Text.SubString(2,1) != "4" ) {
            // b_NZorFR = true; //�q��z��(�ް�)
            // } else {
            // b_NZorFR = false; //�߲��ɽ��
            // }
            ////**********************************************
            // **************************************************
            // 2018.03.26 ���͌v�ގ��ύX_S
            if (P_HEADS_DATA[893].SubString(3, 1) == "2") {
                // �[�ǖ����A
                // �������ݼ�==SUS316L�A
                // ��ڰ�!=SUS316L�̏ꍇ�A
                // �v���[�g�ގ����g���@�@�@
                if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                    (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                    // 2024.02.06 ALLOY�ގ��ϊ�_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                    // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
            }
            // 2018.03.26 ���͌v�ގ��ύX_S
        }
        s_Memori = P_HEADS_DATA[902].TrimRight() + "/" +
            P_HEADS_DATA[903].TrimRight() + "�l�o��";
        s_BaseMatL = P_HEADS_DATA[835].TrimRight();
    }
    else if (a_SE == "S2") {
        if (P_HEADS_DATA[1137].ToIntDef(0) != 0) {
            // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
            s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
            // *********************************
            b_NZorFR = true;
        }
        else {
            s_BaseMatM = P_HEADS_DATA[909].TrimRight();
            // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
            // 2016.11.07 �ގ����ޕύX
            //if (s_BaseMatM.ToIntDef(0) >= 2000) {
            if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                (s_BaseMatM.ToIntDef(0) < 3000)) {
            // ***********************
                s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
            // *************************************************
            // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
            ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
            b_NZorFR = false;
            // s_Text = P_HEADS_DATA[908].TrimRight();
            // if(s_Text.SubString(2,1) != "4" ) {
            // b_NZorFR = true; //�q��z��(�ް�)
            // } else {
            // b_NZorFR = false; //�߲��ɽ��
            // }
            ////**********************************************
            // **************************************************
            // 2018.03.26 ���͌v�ގ��ύX_S
            if (P_HEADS_DATA[908].SubString(3, 1) == "2") {
                // �[�ǖ����A
                // �������ݼ�==SUS316L�A
                // ��ڰ�!=SUS316L�̏ꍇ�A
                // �v���[�g�ގ����g���@�@�@
                if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                    (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
            }
            // 2018.03.26 ���͌v�ގ��ύX_S
        }
        s_Memori = P_HEADS_DATA[917].TrimRight() + "/" +
            P_HEADS_DATA[918].TrimRight() + "�l�o��";
        s_BaseMatL = P_HEADS_DATA[836].TrimRight();
    }
    else if (a_SE == "S3") {
        // 2003.12.24 YX-80 �ǉ�
        // if (P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {
        // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
        // b_NZorFR = true;
        // } else {
        // s_BaseMatM = P_HEADS_DATA[924].TrimRight();
        // b_NZorFR = false;
        // }
        // s_Memori = P_HEADS_DATA[932].TrimRight() + "/" + P_HEADS_DATA[933].TrimRight() + "�l�o��";
        // s_BaseMatL =  P_HEADS_DATA[835].TrimRight();
        // 2017.07.11 �΍s���ǉ�
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
            // �΍s��
        // *********************
            if (P_HEADS_DATA[1138].ToIntDef(0) != 0) {
                // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
                // 2016.11.07 �ގ����ޕύX
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
                // *************************************************
                // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
                ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[923].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //�q��z��(�ް�)
                // } else {
                // b_NZorFR = false; //�߲��ɽ��
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 ���͌v�ގ��ύX_S
                if (P_HEADS_DATA[923].SubString(3, 1) == "2") {
                    // �[�ǖ����A
                    // �������ݼ�==SUS316L�A
                    // ��ڰ�!=SUS316L�̏ꍇ�A
                    // �v���[�g�ގ����g���@�@�@
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                        // 2024.02.06 ALLOY�ގ��ϊ�_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                        // 2024.02.06 ALLOY�ގ��ϊ�_E
                    }
                }
                // 2018.03.26 ���͌v�ގ��ύX_S
            }
            s_Memori = P_HEADS_DATA[932].TrimRight() + "/" +
                P_HEADS_DATA[933].TrimRight() + "�l�o��";
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        else {
            if (P_HEADS_DATA[1138].ToIntDef(0) != 0) {
                // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
                // 2016.11.07 �ގ����ޕύX
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                    // 2024.02.06 ALLOY�ގ��ϊ�_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                    // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
                // *************************************************
                // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
                ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[923].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //�q��z��(�ް�)
                // } else {
                // b_NZorFR = false; //�߲��ɽ��
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 ���͌v�ގ��ύX_S
                if (P_HEADS_DATA[923].SubString(3, 1) == "2") {
                    // �[�ǖ����A
                    // �������ݼ�==SUS316L�A
                    // ��ڰ�!=SUS316L�̏ꍇ�A
                    // �v���[�g�ގ����g���@�@�@
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                        // 2024.02.06 ALLOY�ގ��ϊ�_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                        // 2024.02.06 ALLOY�ގ��ϊ�_E
                    }
                }
                // 2018.03.26 ���͌v�ގ��ύX_S
            }
            s_Memori = P_HEADS_DATA[932].TrimRight() + "/" +
                P_HEADS_DATA[933].TrimRight() + "�l�o��";
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        // *********************
    }
    else if (a_SE == "S4") {
        // 2003.12.24 YX-80 �ǉ�
        // if (P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {
        // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
        // b_NZorFR = true;
        // } else {
        // s_BaseMatM = P_HEADS_DATA[939].TrimRight();
        // b_NZorFR = false;
        // }
        // s_Memori = P_HEADS_DATA[947].TrimRight() + "/" + P_HEADS_DATA[948].TrimRight() + "�l�o��";
        // s_BaseMatL =  P_HEADS_DATA[836].TrimRight();
        // 2017.07.11 �΍s���ǉ�
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
            // �΍s��
        // *********************
            if (P_HEADS_DATA[1139].ToIntDef(0) != 0) {
                // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
                // 2016.11.07 �ގ����ޕύX
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                    // 2024.02.06 ALLOY�ގ��ϊ�_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                    // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
                // *************************************************
                // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
                ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[938].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //�q��z��(�ް�)
                // } else {
                // b_NZorFR = false; //�߲��ɽ��
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 ���͌v�ގ��ύX_S
                if (P_HEADS_DATA[938].SubString(3, 1) == "2") {
                    // �[�ǖ����A
                    // �������ݼ�==SUS316L�A
                    // ��ڰ�!=SUS316L�̏ꍇ�A
                    // �v���[�g�ގ����g���@�@�@
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                        // 2024.02.06 ALLOY�ގ��ϊ�_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                        // 2024.02.06 ALLOY�ގ��ϊ�_E
                    }
                }
                // 2018.03.26 ���͌v�ގ��ύX_S
            }
            s_Memori = P_HEADS_DATA[947].TrimRight() + "/" +
                P_HEADS_DATA[948].TrimRight() + "�l�o��";
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        else {
            if (P_HEADS_DATA[1139].ToIntDef(0) != 0) {
                // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
                // 2016.11.07 �ގ����ޕύX
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                    // 2024.02.06 ALLOY�ގ��ϊ�_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                    // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
                // *************************************************
                // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
                ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[938].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //�q��z��(�ް�)
                // } else {
                // b_NZorFR = false; //�߲��ɽ��
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 ���͌v�ގ��ύX_S
                if (P_HEADS_DATA[938].SubString(3, 1) == "2") {
                    // �[�ǖ����A
                    // �������ݼ�==SUS316L�A
                    // ��ڰ�!=SUS316L�̏ꍇ�A
                    // �v���[�g�ގ����g���@�@�@
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                        // 2024.02.06 ALLOY�ގ��ϊ�_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                        // 2024.02.06 ALLOY�ގ��ϊ�_E
                    }
                }
                // 2018.03.26 ���͌v�ގ��ύX_S
            }
            s_Memori = P_HEADS_DATA[947].TrimRight() + "/" +
                P_HEADS_DATA[948].TrimRight() + "�l�o��";
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        // *********************
    }
    else if (a_SE == "E1") {
        if (P_HEADS_DATA[1140].ToIntDef(0) != 0) {
            // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
            s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
            // *********************************
            b_NZorFR = true;
        }
        else {
            s_BaseMatM = P_HEADS_DATA[954].TrimRight();
            // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
            // 2016.11.07 �ގ����ޕύX
            //if (s_BaseMatM.ToIntDef(0) >= 2000) {
            if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                (s_BaseMatM.ToIntDef(0) < 3000)) {
            // ***********************
                s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
            // *************************************************
            // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
            ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
            b_NZorFR = false;
            // s_Text = P_HEADS_DATA[953].TrimRight();
            // if(s_Text.SubString(2,1) != "4" ) {
            // b_NZorFR = true; //�q��z��(�ް�)
            // } else {
            // b_NZorFR = false; //�߲��ɽ��
            // }
            ////**********************************************
            // **************************************************
            // 2018.03.26 ���͌v�ގ��ύX_S
            if (P_HEADS_DATA[953].SubString(3, 1) == "2") {
                // �[�ǖ����A
                // �������ݼ�==SUS316L�A
                // ��ڰ�!=SUS316L�̏ꍇ�A
                // �v���[�g�ގ����g���@�@�@
                if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                    (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                    // 2024.02.06 ALLOY�ގ��ϊ�_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                    // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
            }
            // 2018.03.26 ���͌v�ގ��ύX_S
        }
        s_Memori = P_HEADS_DATA[962].TrimRight() + "/" +
            P_HEADS_DATA[963].TrimRight() + "�l�o��";
        s_BaseMatL = P_HEADS_DATA[835].TrimRight();
    }
    else if (a_SE == "E2") {
        if (P_HEADS_DATA[1141].ToIntDef(0) != 0) {
            // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
            s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
            // *********************************
            b_NZorFR = true;
        }
        else {
            s_BaseMatM = P_HEADS_DATA[969].TrimRight();
            // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
            // 2016.11.07 �ގ����ޕύX
            //if (s_BaseMatM.ToIntDef(0) >= 2000) {
            if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                (s_BaseMatM.ToIntDef(0) < 3000)) {
            // ***********************
                s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
            // *************************************************
            // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
            ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
            b_NZorFR = false;
            // s_Text = P_HEADS_DATA[968].TrimRight();
            // if(s_Text.SubString(2,1) != "4" ) {
            // b_NZorFR = true; //�q��z��(�ް�)
            // } else {
            // b_NZorFR = false; //�߲��ɽ��
            // }
            ////**********************************************
            // **************************************************
            // 2018.03.26 ���͌v�ގ��ύX_S
            if (P_HEADS_DATA[968].SubString(3, 1) == "2") {
                // �[�ǖ����A
                // �������ݼ�==SUS316L�A
                // ��ڰ�!=SUS316L�̏ꍇ�A
                // �v���[�g�ގ����g���@�@�@
                if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                    (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
            }
            // 2018.03.26 ���͌v�ގ��ύX_S
        }
        s_Memori = P_HEADS_DATA[977].TrimRight() + "/" +
            P_HEADS_DATA[978].TrimRight() + "�l�o��";
        s_BaseMatL = P_HEADS_DATA[836].TrimRight();
    }
    else if (a_SE == "E3") {
        // 2003.12.24 YX-80 �ǉ�
        // if (P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {
        // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
        // b_NZorFR = true;
        // } else {
        // s_BaseMatM = P_HEADS_DATA[984].TrimRight();
        // b_NZorFR = false;
        // }
        // s_Memori = P_HEADS_DATA[992].TrimRight() + "/" + P_HEADS_DATA[993].TrimRight() + "�l�o��";
        // s_BaseMatL =  P_HEADS_DATA[835].TrimRight();
        // 2017.07.11 �΍s���ǉ�
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
            // �΍s��
        // *********************
            if (P_HEADS_DATA[1142].ToIntDef(0) != 0) {
                // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
                // 2016.11.07 �ގ����ޕύX
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
                // *************************************************
                // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
                ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[983].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //�q��z��(�ް�)
                // } else {
                // b_NZorFR = false; //�߲��ɽ��
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 ���͌v�ގ��ύX_S
                if (P_HEADS_DATA[983].SubString(3, 1) == "2") {
                    // �[�ǖ����A
                    // �������ݼ�==SUS316L�A
                    // ��ڰ�!=SUS316L�̏ꍇ�A
                    // �v���[�g�ގ����g���@�@�@
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                        // 2024.02.06 ALLOY�ގ��ϊ�_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                        // 2024.02.06 ALLOY�ގ��ϊ�_E
                    }
                }
                // 2018.03.26 ���͌v�ގ��ύX_S
            }
            s_Memori = P_HEADS_DATA[992].TrimRight() + "/" +
                P_HEADS_DATA[993].TrimRight() + "�l�o��";
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        else {
            if (P_HEADS_DATA[1142].ToIntDef(0) != 0) {
                // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
                // 2016.11.07 �ގ����ޕύX
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                    // 2024.02.06 ALLOY�ގ��ϊ�_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                    // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
                // *************************************************
                // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
                ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[983].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //�q��z��(�ް�)
                // } else {
                // b_NZorFR = false; //�߲��ɽ��
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 ���͌v�ގ��ύX_S
                if (P_HEADS_DATA[983].SubString(3, 1) == "2") {
                    // �[�ǖ����A
                    // �������ݼ�==SUS316L�A
                    // ��ڰ�!=SUS316L�̏ꍇ�A
                    // �v���[�g�ގ����g���@�@�@
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                        // 2024.02.06 ALLOY�ގ��ϊ�_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                        // 2024.02.06 ALLOY�ގ��ϊ�_E
                    }
                }
                // 2018.03.26 ���͌v�ގ��ύX_S
            }
            s_Memori = P_HEADS_DATA[992].TrimRight() + "/" +
                P_HEADS_DATA[993].TrimRight() + "�l�o��";
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        // *********************
    }
    else if (a_SE == "E4") {
        // 2003.12.24 YX-80 �ǉ�
        // if (P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {
        // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
        // b_NZorFR = true;
        // } else {
        // s_BaseMatM = P_HEADS_DATA[999].TrimRight();
        // b_NZorFR = false;
        // }
        // s_Memori = P_HEADS_DATA[1007].TrimRight() + "/" + P_HEADS_DATA[1008].TrimRight() + "�l�o��";
        // s_BaseMatL =  P_HEADS_DATA[836].TrimRight();
        // 2017.07.11 �΍s���ǉ�
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
            // �΍s��
        // *********************
            if (P_HEADS_DATA[1143].ToIntDef(0) != 0) {
                // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
                // 2016.11.07 �ގ����ޕύX
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                    // 2024.02.06 ALLOY�ގ��ϊ�_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                    // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
                // *************************************************
                // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
                ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[998].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //�q��z��(�ް�)
                // } else {
                // b_NZorFR = false; //�߲��ɽ��
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 ���͌v�ގ��ύX_S
                if (P_HEADS_DATA[998].SubString(3, 1) == "2") {
                    // �[�ǖ����A
                    // �������ݼ�==SUS316L�A
                    // ��ڰ�!=SUS316L�̏ꍇ�A
                    // �v���[�g�ގ����g���@�@�@
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                        // 2024.02.06 ALLOY�ގ��ϊ�_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                        // 2024.02.06 ALLOY�ގ��ϊ�_E
                    }
                }
                // 2018.03.26 ���͌v�ގ��ύX_S
            }
            s_Memori = P_HEADS_DATA[1007].TrimRight() + "/" +
                P_HEADS_DATA[1008].TrimRight() + "�l�o��";
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        else {
            if (P_HEADS_DATA[1143].ToIntDef(0) != 0) {
                // 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // 2006.02.06 ɽ�ٍގ�����т̏ꍇ��ڰčގ����g�p����
                // 2016.11.07 �ގ����ޕύX
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                    // 2024.02.06 ALLOY�ގ��ϊ�_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                    // 2024.02.06 ALLOY�ގ��ϊ�_E
                }
                // *************************************************
                // 2007.12.17 �q��z��(�ް�)���߲��ɽ�ق̋�ʂ���߂�
                ////2006.04.14 �߲��ɽ�قƋq��z��(�ް�)����ʂ���
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[998].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //�q��z��(�ް�)
                // } else {
                // b_NZorFR = false; //�߲��ɽ��
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 ���͌v�ގ��ύX_S
                if (P_HEADS_DATA[998].SubString(3, 1) == "2") {
                    // �[�ǖ����A
                    // �������ݼ�==SUS316L�A
                    // ��ڰ�!=SUS316L�̏ꍇ�A
                    // �v���[�g�ގ����g���@�@�@
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ��ڰčގ�
                        // 2024.02.06 ALLOY�ގ��ϊ�_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                        // 2024.02.06 ALLOY�ގ��ϊ�_E
                    }
                }
                // 2018.03.26 ���͌v�ގ��ύX_S
            }
            s_Memori = P_HEADS_DATA[1007].TrimRight() + "/" +
                P_HEADS_DATA[1008].TrimRight() + "�l�o��";
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        // *********************
    }
    // i_wYobi = Get_Press_YobiSu(Key.ToIntDef(0));
    i_wYobi = 0;

    if (a_SE == "S1") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[892].TrimRight(), 0.0));
    }
    else if (a_SE == "S2") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[907].TrimRight(), 0.0));
    }
    else if (a_SE == "S3") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[922].TrimRight(), 0.0));
    }
    else if (a_SE == "S4") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[937].TrimRight(), 0.0));
    }
    else if (a_SE == "E1") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[952].TrimRight(), 0.0));
    }
    else if (a_SE == "E2") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[967].TrimRight(), 0.0));
    }
    else if (a_SE == "E3") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[982].TrimRight(), 0.0));
    }
    else if (a_SE == "E4") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[997].TrimRight(), 0.0));
    }

    // 2020.01.14 �ݸޒ[�ǕύX_S
    if (a_SE == "S1") {
        s_FrngSize = P_HEADS_DATA[892].TrimRight();
    }
    else if (a_SE == "S2") {
        s_FrngSize = P_HEADS_DATA[907].TrimRight();
    }
    else if (a_SE == "S3") {
        s_FrngSize = P_HEADS_DATA[922].TrimRight();
    }
    else if (a_SE == "S4") {
        s_FrngSize = P_HEADS_DATA[937].TrimRight();
    }
    else if (a_SE == "E1") {
        s_FrngSize = P_HEADS_DATA[952].TrimRight();
    }
    else if (a_SE == "E2") {
        s_FrngSize = P_HEADS_DATA[967].TrimRight();
    }
    else if (a_SE == "E3") {
        s_FrngSize = P_HEADS_DATA[982].TrimRight();
    }
    else if (a_SE == "E4") {
        s_FrngSize = P_HEADS_DATA[997].TrimRight();
    }
    // 2020.01.14 �ݸޒ[�ǕύX_E

    // 2012.03.15 ���ǋȊǔ��f�ύX
    s_Tmp_Syurui = "";
    if (a_SE == "S1") {
        s_BOOTS = P_HEADS_DATA[893].SubString(3, 1); // S1�ް��
        s_Tmp_Syurui = P_HEADS_DATA[898].TrimRight(); // S1���x�v���
    }
    else if (a_SE == "S2") {
        s_BOOTS = P_HEADS_DATA[908].SubString(3, 1); // S2�ް��
        s_Tmp_Syurui = P_HEADS_DATA[913].TrimRight(); // S2���x�v���
    }
    else if (a_SE == "S3") {
        s_BOOTS = P_HEADS_DATA[923].SubString(3, 1); // S3�ް��
        s_Tmp_Syurui = P_HEADS_DATA[928].TrimRight(); // S3���x�v���
    }
    else if (a_SE == "S4") {
        s_BOOTS = P_HEADS_DATA[938].SubString(3, 1); // S4�ް��
        s_Tmp_Syurui = P_HEADS_DATA[943].TrimRight(); // S4���x�v���
    }
    else if (a_SE == "E1") {
        s_BOOTS = P_HEADS_DATA[953].SubString(3, 1); // E1�ް��
        s_Tmp_Syurui = P_HEADS_DATA[958].TrimRight(); // E1���x�v���
    }
    else if (a_SE == "E2") {
        s_BOOTS = P_HEADS_DATA[968].SubString(3, 1); // E2�ް��
        s_Tmp_Syurui = P_HEADS_DATA[973].TrimRight(); // E2���x�v���
    }
    else if (a_SE == "E3") {
        s_BOOTS = P_HEADS_DATA[983].SubString(3, 1); // E3�ް��
        s_Tmp_Syurui = P_HEADS_DATA[988].TrimRight(); // E3���x�v���
    }
    else if (a_SE == "E4") {
        s_BOOTS = P_HEADS_DATA[998].SubString(3, 1); // E4�ް��
        s_Tmp_Syurui = P_HEADS_DATA[1003].TrimRight(); // E4���x�v���
    }
    if (s_Tmp_Syurui == "00") {
        b_Tmp = false;
    }
    else {
        b_Tmp = true;
    }
    if (P_HEADS_DATA[823].ToIntDef(0) == 111 && P_HEADS_DATA[1011].TrimRight()
        .ToIntDef(0) == 80) { // YX-80
        b_YX = true; // �߲��ɽ�و��͌v�z�ǁF�Ȋ�
    }
    // 2017.07.11 YX-83�ǉ�
    else if (P_HEADS_DATA[823].ToIntDef(0) == 111 && P_HEADS_DATA[1011].TrimRight()
        .ToIntDef(0) == 83) { // YX-83
        b_YX = true; // �߲��ɽ�و��͌v�z�ǁF�Ȋ�
    }
    // ********************
    else {
        b_YX = false; // �߲��ɽ�و��͌v�z�ǁF����
    }

    if (s_BOOTS == "4") {
        // �߲��ɽ��
        if (b_YX == true) {
            // �߲��ɽ�و��͌v�z�ǁF�Ȋ�
            if (b_Tmp == true) {
                // ���x�v�L
                b_STorCU = false; // �Ȋ�
            }
            else {
                // ���x�v��
                b_STorCU = false; // �Ȋ�
            }
        }
        else {
            // �߲��ɽ�و��͌v�z�ǁF����
            b_STorCU = true; // ����
        }
    }
    else if (b_NZorFR == true) {
        // �[��
        if (a_SE.SubString(1, 1) == "S") {
            // �r���[��
            if (b_Tmp == true) {
                // ���x�v�L
                b_STorCU = false; // �Ȋ�
            }
            else {
                // ���x�v��
                b_STorCU = true; // ����
            }
        }
        else {
            // �d���[��
            b_STorCU = true; // ����
        }
    }
    else {
        // �q��z��
        b_STorCU = true; // ����
    }
    // ***************************

    while (!wkQuery->Eof) {
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // ���i����
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // �}��
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // ��}�p�}��
        P_Num = wkQuery->FieldByName("NUM")->AsCurrency; // ����
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // �ގ�����
        P_EZaisitu = P_Zaisitu; // �ގ�����
        P_Siyou = wkQuery->FieldByName("PARTS_SPEC")->AsString.TrimRight
            (); // �d�l
        P_Unit = "";

        // 2004.01.27 �����v�Q�ǉ�
        AnsiString s_Prs_Syurui; // ���͌v���
        // 2008.03.21 ���͌v��t���װ�d�l�Ή�
        if (sSpecCode == "B") { // �װB�d�l
            if (a_SE == "S2" && (Key == "16" || Key == "17")) { // S2���͌v���
                s_Prs_Syurui = P_HEADS_DATA[901].TrimRight();
                if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") { // S1���͌v���
                    if (s_Code.ToIntDef(0) == 21) {
                        P_Zuban = "�d�S�Q�T�V�W�Q�|�q�Q"; // �}��
                        P_SZuban = "�d�S�Q�T�V�W�Q"; // ��}�p�}��
                    }
                }
            }
        }
        else { // �װA�d�l,�װ�ȊO
            if (a_SE == "S1" && (Key == "16" || Key == "17")) { // S1���͌v���
                s_Prs_Syurui = P_HEADS_DATA[916].TrimRight();
                if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") { // S2���͌v���
                    if (s_Code.ToIntDef(0) == 21) {
                        P_Zuban = "�d�S�Q�T�V�W�Q�|�q�Q"; // �}��
                        P_SZuban = "�d�S�Q�T�V�W�Q"; // ��}�p�}��
                    }
                }
            }
        }
        // if ( a_SE == "S1" && ( Key == "16" || Key == "17" ) ){  // S1���͌v���
        // s_Prs_Syurui = P_HEADS_DATA[916].TrimRight();
        // if ( s_Prs_Syurui == "16" || s_Prs_Syurui == "17" ){ // S2���͌v���
        // if ( s_Code.ToIntDef(0) == 21 ) {
        // //2006.06.28 �}�ԕύX
        // //P_Zuban   = "�d�S�Q�T�V�W�Q";      // �}��
        // P_Zuban   = "�d�S�Q�T�V�W�Q�|�q�Q";// �}��
        // //*******************
        // P_SZuban  = "�d�S�Q�T�V�W�Q";      // ��}�p�}��
        // }
        // }
        // }
        // **********************************
        // *************************

        // *********************************
        // ***  ���͌v�\�����ið��ٌ���
        // *********************************
        Search_PRESS_CODE(s_Code);

        // *********************************
        // ***  �\���i�� �擾(02.08.05�ǉ�)
        // *********************************
        // P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin,"0");
        P_Yobisu = 0;
        switch (s_Code.ToIntDef(0)) {
        case 11: // �u�������͌v
            // 2004.02.04 �T�j�^�����͌v
            // P_Zaisitu = Search_HD_E_MATERIAL(s_BaseMatM);
            // if (P_Zaisitu.Pos("�r�t�r�R�O�S") != 0 ) {
            // P_Zaisitu = "�r�t�r�R�O�S";
            // P_EZaisitu = "�r�t�r�R�O�S";
            // } else {
            // P_Zaisitu = "�r�t�r�R�P�U";
            // P_EZaisitu = "�r�t�r�R�P�U";
            // }
            if (Key == "14" || Key == "15") { // �T�j�^�����͌v
                P_Zaisitu = Search_HD_MATERIAL(s_BaseMatM);
                if (P_Zaisitu.Pos("�r�t�r�R�O�S") != 0) {
                    P_Zaisitu = "�r�t�r�R�O�S";
                    P_EZaisitu = "�r�t�r�R�O�S";
                }
                else {
                    P_Zaisitu = "�r�t�r�R�P�U";
                    P_EZaisitu = "�r�t�r�R�P�U";
                }
            }
            // *************************
        case 1: // ���͌v�{��
        case 20: // ��j�����͌v
            P_Siyou = P_Siyou + "  " + s_Memori;
            P_Yobisu = i_wYobi;
            break;
        case 2: // �Ȋ�
        case 3: // ����
            // 2006.04.14 �߲��,���ޒǉ�
        case 25: // �߲��
        case 26: // ����
            // *************************
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 1);
            // 2014.06.24 ���ލގ��ύX
            if (P_Zaisitu == "1161") { // �`�^���Q��
                P_Zaisitu = "1032"; // �s�o�R�S�O�ɒu��������
            }
            // ***********************
            // 2014.07.08 ���ލގ��ύX
            if (P_Zaisitu == "1162") { // �`�^���Q��
                P_Zaisitu = "1033"; // �s�o�R�S�O�|�o���ɒu��������
            }
            // ***********************
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            P_Yobisu = i_wYobi;
            break;
        case 4: // ���[�g���R�b�N
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 4);
            if (Key == "06" || Key == "07" || Key == "02")
            { // ���͌v�U�E�V�̏ꍇ�A�ϊ���̺��ނ��C��
                if (P_Zaisitu == "1071") { // �b�R�U�O�O
                    P_Zaisitu = "1003"; // �r�t�r�R�P�U�ɒu��������
                }
                // 2011.02.09 �ގ��ύX
                // 2016.11.07 �ގ����ޕύX
                //if (P_Zaisitu == "1001") { // �r�t�r�R�O�S
                if (Type_MAT(P_Zaisitu) == "SUS304")  { // �r�t�r�R�O�S
                // ***********************
                    P_Zaisitu = "1003"; // �r�t�r�R�P�U�ɒu��������
                }
                if (P_Zaisitu == "1005") { // �r�t�r�R�P�U�i�P�k
                    P_Zaisitu = "1003"; // �r�t�r�R�P�U�ɒu��������
                }
                if (P_Zaisitu == "1030") { // �r�t�r�R�P�T�i�P
                    P_Zaisitu = "1003"; // �r�t�r�R�P�U�ɒu��������
                }
                // ********************
            // 2016.06.08 ���[�g���R�b�N�ގ��ύX
            }
            else if (Key == "01")
            {
               // ���͌v�P�̏ꍇ�A�b�R�U�O�O
                // 2017.08.17 C3600�p�~_S
                //P_Zaisitu = "1071"; // �b�R�U�O�O
                // 2017.08.17 C3600�p�~_E�@�@
            // *********************************
            }
            // 2017.08.17 C3600�p�~_S
            if (P_Zaisitu == "31CC") { // �b�R�U�O�O
                P_Zaisitu = "1003"; // �r�t�r�R�P�U�ɒu��������
            }
            // 2017.08.17 C3600�p�~_E
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            P_Yobisu = i_wYobi;
            break;
        case 18: // �`�[�Y
            // P_Zaisitu = Chg_Zaisitu_Code( s_BaseMatM, 3);
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // 2004.02.05
            // P_Zaisitu = Search_HD_E_MATERIAL(s_BaseMatM);
            P_Zaisitu = Search_HD_MATERIAL(s_BaseMatM);
            // **********
            if (P_Zaisitu.Pos("�r�t�r�R�O�S") != 0) {
                P_Zaisitu = "�r�t�r�R�O�S";
                P_EZaisitu = "�r�t�r�R�O�S";
            }
            else {
                P_Zaisitu = "�r�t�r�R�P�U";
                P_EZaisitu = "�r�t�r�R�P�U";
            }
            P_Yobisu = i_wYobi;
            P_Siyou = s_SaniSize + P_Siyou; // �`�[�Y�́u�m�Y�����a�~���͌v��t�����a�v�ŕ\�L
            if (Key.ToIntDef(0) == 14) { // �T�j�^���[�l�W
                P_Sikibetu = "CHES-N";
            }
            else { // �T�j�^���[�w���[��
                P_Sikibetu = "CHES-H";
            }
            break;
        case 5: // �V�[�g�p�b�L���O
        case 14: // �p�b�L���O
        case 22: // �p�b�L���O
            // 2004.02.10 �X�`�[�������ǉ�
            // if ( Key.ToIntDef(0) == 5 ) { // �X�`�[���p�͂d�o�c�l�Œ�
            if (Key.ToIntDef(0) == 5 || Key.ToIntDef(0) == 18 || Key.ToIntDef(0)
                == 19 || Key.ToIntDef(0) == 20) { // �X�`�[���p�͂d�o�c�l�Œ�
                // ***************************
                P_Zaisitu = Search_HD_MATERIAL("2026");
                P_EZaisitu = Search_HD_E_MATERIAL("2026");
            }
            else {
                P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatL, 1);
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            P_Yobisu = i_wYobi;
            if (s_Code.ToIntDef(0) == 22)
            { // �߯�ݸނQ�Q�̓m�Y�����߯�ݸނȂ̂ŃT�j�^���̌��a�Ŏd�l�쐬
                P_Siyou = s_SaniSize + P_Siyou;
            }
            break;
        case 8: // �\�P�b�g
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 1);
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            break;
        case 6: // �o���u�[�a�u
        case 7: // �u�b�V��
        case 9: // ���^�[���p�C�v
        case 10: // �Q�[�W�R�b�N
        case 12: // �t���[�e
        case 15: // �_�C���t������
        case 16: // �X�^�b�h�a�m
        case 17: // �O���[�u��
        case 19: // �N�����v�p��
        case 23: // �N�����v�p��
            P_Yobisu = i_wYobi;
            if (s_Code.ToIntDef(0) == 23)
            { // �N�����v�p��Q�R�̓m�Y�����N�����v�p��Ȃ̂ŃT�j�^���̌��a�Ŏd�l�쐬
                P_Siyou = s_SaniSize + P_Siyou;
            }
            break;
        case 13: // �a�D�m
            P_Yobisu = P_Num * i_wYobi;
            break;
        case 24: // �����v�z��
            P_Unit = "��";
            if ((P_HEADS_DATA[823].ToIntDef(0) == 2 ||
                P_HEADS_DATA[823].ToIntDef(0) == 3 || P_HEADS_DATA[823].ToIntDef
                (0) == 8) && P_HEADS_DATA[1011].ToIntDef(0) == 10) { // ���^
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
                // 2007.02.15 LX-00�ǉ�
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 0) { // LX-00(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
                // ********************
                // 2008.04.14 ���^�E��^�ύX
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 2 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 20) { // UX-20(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 2 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 30) { // UX-30(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 2 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 40) { // UX-40(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 20) { // LX-20(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 30) { // LX-30(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 40) { // LX-40(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 50) { // LX-50(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 4 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 40) { // SX-40(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 8 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 30) { // RX-30(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            // 2022.04.08 ���p�^���ǉ�_S
            else if (P_HEADS_DATA[823].ToIntDef(0) == 8 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 50) { // RX-50(���^)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�P�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
            }
            // 2022.04.08 ���p�^���ǉ�_E
            else { // ��^
                // P_Siyou = P_Siyou.SubString(1,P_Siyou.Pos("��")-1) + "�Q" + P_Siyou.SubString(P_Siyou.Pos("��"),P_Siyou.Length());
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("��") - 1) + "�Q�D�T" +
                    P_Siyou.SubString(P_Siyou.Pos("��"), P_Siyou.Length());
                // *************************
            }
            break;
        default:
            break;
        }
        if (s_Code.ToIntDef(0) == 1 || s_Code.ToIntDef(0) == 11 ||
            s_Code.ToIntDef(0) == 20) {
            if (Key.ToIntDef(0) >= 1 && Key.ToIntDef(0) <= 5) { // ������
                P_Sikibetu = "PRSG-B";
                // 2004.02.10 �u���h�������ǉ�
            }
            else if (Key.ToIntDef(0) >= 18 && Key.ToIntDef(0) <= 20) { // ������
                P_Sikibetu = "PRSG-B";
                // ***************************
            }
            else if (Key.ToIntDef(0) >= 6 && Key.ToIntDef(0) <= 13) { // �u��
                P_Sikibetu = "PRSG-D";
                // 2007.12.17 �u�������ǉ�
            }
            //2020.01.14 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_S
            //else if (Key.ToIntDef(0) >= 21 && Key.ToIntDef(0) <= 24) { // �u��
            //    P_Sikibetu = "PRSG-D";
            //    // ***********************
            //}
            //2020.01.14 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_E
            else if (Key.ToIntDef(0) == 14) { // ���ذȼ�
                P_Sikibetu = "PRSG-N";
            }
            else if (Key.ToIntDef(0) == 15) { // ���ذ�ٰ�
                P_Sikibetu = "PRSG-H";
            }
            else if (Key.ToIntDef(0) == 16) { // ���p
                P_Sikibetu = "PRSG-S";
            }
            else if (Key.ToIntDef(0) == 17) { // ���p����
                P_Sikibetu = "PRSG-E";
            }
            //2020.01.14 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_S
            else if (Key.ToIntDef(0) == 21 || Key.ToIntDef(0) == 22) { // ������
                P_Sikibetu = "PRSG-B";
            }
            //2020.01.14 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_E
        }

        // 2015.03.23 OGI ���͌v��ނɂ��o�͎d�l�ύX�Ή�
        AnsiString s1_Prs_Syurui = P_HEADS_DATA[901].TrimRight(); // S1���͌v���
        AnsiString s2_Prs_Syurui = P_HEADS_DATA[916].TrimRight(); // S2���͌v���
        AnsiString s3_Prs_Syurui = P_HEADS_DATA[931].TrimRight(); // S3���͌v���
        AnsiString s4_Prs_Syurui = P_HEADS_DATA[946].TrimRight(); // S4���͌v���
        if( s1_Prs_Syurui.ToIntDef(0) == 16 &&
            s2_Prs_Syurui.ToIntDef(0) == 16 &&
            s3_Prs_Syurui.ToIntDef(0) == 16 &&
            s4_Prs_Syurui.ToIntDef(0) == 16 ) {
            if( s_Code.ToIntDef(0) == 27 ) {
                // ��j�����͌v
                P_Num = 2;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // �o���u�[�a�u
                P_Num = 2;
                // 2020.01.14 �ݸޒ[�ǕύX_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 �ݸޒ[�ǕύX_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // ���͌v��t��
                // AB�����Ɉ�j�����͌v���t���ꍇ�́A���͌v��t��̐}�ԁA��}�p�}�Ԃ�DB�ƈقȂ�
                P_Zuban  = "E358526";
                P_SZuban = "E358526" ;
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) == 16 &&
            s2_Prs_Syurui.ToIntDef(0) != 16 &&
            s3_Prs_Syurui.ToIntDef(0) == 16 &&
            s4_Prs_Syurui.ToIntDef(0) != 16 ) {
            if( s_Code.ToIntDef(0) == 27 ) {
                // ��j�����͌v
                P_Num = 2;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // �o���u�[�a�u
                P_Num = 2;
                // 2020.01.14 �ݸޒ[�ǕύX_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 �ݸޒ[�ǕύX_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // ���͌v��t��
                // 2017.11.21 �Б��ݸޒ[��_S
                 P_Zuban  = "E425782-R2";
                 P_SZuban = "E425782G" ;
                // 2017.11.21 �Б��ݸޒ[��_E
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) != 16 &&
            s2_Prs_Syurui.ToIntDef(0) == 16 &&
            s3_Prs_Syurui.ToIntDef(0) != 16 &&
            s4_Prs_Syurui.ToIntDef(0) == 16 ) {
            if( s_Code.ToIntDef(0) == 27 ) {
                // ��j�����͌v
                P_Num = 2;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // �o���u�[�a�u
                P_Num = 2;
                // 2020.01.14 �ݸޒ[�ǕύX_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 �ݸޒ[�ǕύX_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // ���͌v��t��
                // 2017.11.21 �Б��ݸޒ[��_S
                 P_Zuban  = "E425782-R2";
                 P_SZuban = "E425782G" ;
                // 2017.11.21 �Б��ݸޒ[��_E
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) == 17 &&
            s2_Prs_Syurui.ToIntDef(0) == 17 &&
            s3_Prs_Syurui.ToIntDef(0) == 17 &&
            s4_Prs_Syurui.ToIntDef(0) == 17 ) {
            if( s_Code.ToIntDef(0) == 20 ) {
                // ��j�����͌v
                P_Num = 1;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // �o���u�[�a�u
                P_Num = 2;
                // 2020.01.14 �ݸޒ[�ǕύX_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 �ݸޒ[�ǕύX_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // ���͌v��t��
                // AB�����Ɉ�j�����͌v���t���ꍇ�́A���͌v��t��̐}�ԁA��}�p�}�Ԃ�DB�ƈقȂ�
                P_Zuban  = "E425782-R2";
                P_SZuban = "E425782" ;
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) == 17 &&
            s2_Prs_Syurui.ToIntDef(0) != 17 &&
            s3_Prs_Syurui.ToIntDef(0) == 17 &&
            s4_Prs_Syurui.ToIntDef(0) != 17 ) {
            if( s_Code.ToIntDef(0) == 20 ) {
                // ��j�����͌v
                P_Num = 1;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // �o���u�[�a�u
                P_Num = 2;
                // 2020.01.14 �ݸޒ[�ǕύX_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 �ݸޒ[�ǕύX_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // ���͌v��t��
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) != 17 &&
            s2_Prs_Syurui.ToIntDef(0) == 17 &&
            s3_Prs_Syurui.ToIntDef(0) != 17 &&
            s4_Prs_Syurui.ToIntDef(0) == 17 ) {
            if( s_Code.ToIntDef(0) == 20 ) {
                // ��j�����͌v
                P_Num = 1;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // �o���u�[�a�u
                P_Num = 2;
                // 2020.01.14 �ݸޒ[�ǕύX_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 �ݸޒ[�ǕύX_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // ���͌v��t��
            }
        }


        // 2012.03.15 ���ǋȊǔ��f�ύX
        ////2006.11.22 �x�w�|�W�O�i�Ȋǁj�����ǉ�
        ////if(( b_NZorFR == true && s_Code.ToIntDef(0) == 3 ) ||  // �[�ǂ̎��͋Ȋǁ@����ȊO�͒���
        ////   ( b_NZorFR == false && s_Code.ToIntDef(0) == 2 ) ) {
        // s_Text = P_HEADS_DATA[34].TrimRight();
        // if(( s_Text.SubString(1,2) == "YX" && s_Code.ToIntDef(0) == 3 ) ||  // YX-80�̎��͋Ȋ�
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == true && s_Code.ToIntDef(0) == 3 ) ||  // �[�ǂ̎��͋Ȋǁ@����ȊO�͒���                    ){
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == false && s_Code.ToIntDef(0) == 2 ) ) {
        ////*************************************
        ////2006.04.14 ��-їp���͌v�ύX
        ////�[�ǂ̎��̓p�C�v�{�G���{�@����ȊO�̓\�P�b�g
        ////2011.09.28 �x�w�|�W�O�i�p�C�v�{�G���{�j�����ǉ�
        ////} else if (( b_NZorFR == true && s_Code.ToIntDef(0) == 8 ) ||
        ////           ( b_NZorFR == false && s_Code.ToIntDef(0) == 25 ) ||
        ////           ( b_NZorFR == false && s_Code.ToIntDef(0) == 26 ) ) {
        // } else if (( s_Text.SubString(1,2) == "YX" && s_Code.ToIntDef(0) == 8 ) ||  // YX-80�̎��̓p�C�v�{�G���{
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == true && s_Code.ToIntDef(0) == 8 ) ||  //�[�ǂ̎��̓p�C�v�{�G���{
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == false && s_Code.ToIntDef(0) == 25 ) ||  //�[�ǈȊO�̓\�P�b�g
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == false && s_Code.ToIntDef(0) == 26 ) ) {
        ////***********************************************
        ////***************************
        if (b_STorCU == false && s_Code.ToIntDef(0) == 3) {
            // �Ȋ�2�g�p(����3�s��)
        }
        else if (b_STorCU == true && s_Code.ToIntDef(0) == 2) {
            // ����3�g�p(�Ȋ�2�s��)
        }
        else if (b_STorCU == false && s_Code.ToIntDef(0) == 8) {
            // �p�C�v25�{�G���{26�g�p(�\�P�b�g8�s��)
        }
        else if (b_STorCU == true && s_Code.ToIntDef(0) == 25) {
            // �\�P�b�g8�g�p(�p�C�v25�{(�G���{26)�s��)
        }
        else if (b_STorCU == true && s_Code.ToIntDef(0) == 26) {
            // �\�P�b�g8�g�p((�p�C�v25)�{�G���{26�s��)
            // ***************************
        }
        else {
            if (((s_Code.ToIntDef(0) == 20) || (s_Code.ToIntDef(0) == 21) ||
                (s_Code.ToIntDef(0) == 24)) && // �����v���i�͏㉺�łP�Z�b�g
                ((a_SE == "S3") || (a_SE == "S4"))) {
                // 2008.03.21 ���͌v��t���װ�d�l�Ή�
            }
            else if ((sSpecCode == "B") && (s_Code.ToIntDef(0) == 21) &&
                (a_SE == "S1" && (Key == "16" || Key == "17")) &&
                ((P_HEADS_DATA[916].TrimRight() == "16") ||
                (P_HEADS_DATA[916].TrimRight() == "17"))) {
                // �װB�d�l ɽ�وʒuS1�ň��͌v��t���S1,S2�Ŏg�p����ꍇ�͈��͌v��t����͂���
            }
            else if ((sSpecCode != "B") && (s_Code.ToIntDef(0) == 21) &&
                (a_SE == "S2" && (Key == "16" || Key == "17")) &&
                ((P_HEADS_DATA[901].TrimRight() == "16") ||
                (P_HEADS_DATA[901].TrimRight() == "17"))) {
                // �װA�d�l,�װ�ȊO ɽ�وʒuS2�ň��͌v��t���S1,S2�Ŏg�p����ꍇ�͈��͌v��t����͂���
                ////2004.01.27 �����v�Q�ǉ�
                // } else if ( ( s_Code.ToIntDef(0) == 21 ) && ( a_SE == "S2" && ( Key == "16" || Key == "17" ) ) &&
                // ( ( P_HEADS_DATA[901].TrimRight()  == "16" ) || ( P_HEADS_DATA[901].TrimRight() == "17" ) ) ) {
                ////*************************
                // **********************************
            }
            else {
                Write_PressG_Data(a_SE);
            }
        }
        wkQuery->Next();
    }


    delete wkQuery;
    return s_Code;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���͌v�\�����i����ð��ف@����
// �T  �v      �F
// ��  ��      �F AnsiString Code  // �����Ώۺ���
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::Search_PRESS_CODE(AnsiString Code) {
    int i;

    for (i = 0; i < G_PRESSCD_RecCnt; i++) {
        if (G_PRESSCD[i][0] == Code) {

            P_Sikibetu = G_PRESSCD[i][1]; // ���ʎq(CSV̧�ٍ쐬�p)
            P_Katasiki = G_PRESSCD[i][3]; // ���i����(CSV̧�ٍ쐬�p)

            P_Yobihin = G_PRESSCD[i][4]; // �\���i
            break;
        }
    }

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���͌v�ް�̧�ُ����ݏ���
// �T  �v      �F
// ��  ��      �F AnsiString a_SE   �F ���͌vSE�敪(S1�`S4,E1�`E4)
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::Write_PressG_Data(AnsiString a_SE) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;
    // 2017.07.11 �΍s���ǉ�
    int iFL;
    // *********************

    AnsiString wk_FilePass;

    // 2017.07.11 �΍s���ǉ�
    // �΍s��
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
        iFL = 1; // �΍s��
    }
    else {
        iFL = 0; // ��`��
    }
    // *********************

    // ----------------------------------------------
    // 2007.10.03 �װ�d�l�Ή�
    AnsiString sSpecCode; // �d�l���ߺ���
    // **********************************************
    // �d�l���߂��擾
    // **********************************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03
    // ----------------------------------------------

    // **********************************************
    // ***  ���i�W�J�ް��@OPEN
    // ***    �����OPEN�Ȃ̂ŏ㏑�����[�h�ō쐬
    // **********************************************
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((wkfp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(wkfp);
        return false;
    }

    // 1  ���i����

    // 2003.12.11 YX-80�ǉ�
    // if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "3" ) )
    // s_Text = "B_PRSG";
    // else
    // s_Text = "A_PRSG";
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // �΍s��
    // *********************
        if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2, 1) == "4"))
            s_Text = "B_PRSG";
        else
            s_Text = "A_PRSG";
    }
    else {
        if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2, 1) == "3"))
            s_Text = "B_PRSG";
        else
            s_Text = "A_PRSG";
    }
    // *********************

    fputs(s_Text.c_str(), wkfp);

    // 2  ���ʎq
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), wkfp);

    // 3  ���i�^��
    s_Text = "," + P_Katasiki;
    fputs(s_Text.c_str(), wkfp);

    // 4  ���i����
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 5  �ގ�����
    s_Text = "," + P_Zaisitu;
    fputs(s_Text.c_str(), wkfp);

    // 6  �}��
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), wkfp);

    // 7  ��}�p�}��
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), wkfp);

    // 8  1�䕪����
    s_Text = "," + AnsiString(P_Num);
    fputs(s_Text.c_str(), wkfp);

    // ,9  �\����
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), wkfp);

    // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
    i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // �䐔
    i_Soukei = P_Num * i_Daisu + P_Yobisu;
    s_Text = "," + AnsiString(i_Soukei);
    fputs(s_Text.c_str(), wkfp);

    // 11 �d�l�P
    s_Text = "," + P_Siyou;
    fputs(s_Text.c_str(), wkfp);
    // 12 �d�l�Q
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 13 �d�l�R
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 14 �d�l�S
    s_Text = "," + P_Unit;
    fputs(s_Text.c_str(), wkfp);

    // 15 �d�l�T
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 16  �ގ�����(�p)
    s_Text = "," + P_EZaisitu;
    fputs(s_Text.c_str(), wkfp);

    // 17  �d�l���ߺ���
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), wkfp);

    // ���s����
    s_Text = "\n";

    // CSV�ް��@̧�ُo��
    fputs(s_Text.c_str(), wkfp);

    fclose(wkfp); // ����

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���͌v�\���i�擾����
// �T  �v      �F
// ��  ��      �F ���͌v�敪(01�`17)
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
int __fastcall TPressGDetails::Get_Press_YobiSu(int i_TmpCd) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;
    AnsiString s_YobiName;
    int i_YobiSu;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PRSG2YOBI";
    s_SQL = s_SQL + "  WHERE PRSPCD = " + i_TmpCd;

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

    i_YobiSu = Syukei_Buhin_Yobisu(s_YobiName, "0", m_pcHeadsData);
    return i_YobiSu;
}
