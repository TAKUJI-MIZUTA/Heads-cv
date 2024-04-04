// ---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�F���x�v���i�W�J����  ���W���[��
// �� �� �ҁFT.Kawai
// �� �� ���F2002.08.05
// �X �V ���F2002.09.04
// 2007.10.03 �װ�d�l�Ή�
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

#include "Bas_TempDetails.h"

#include "HeadsData.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// ��ڰĕ��i�W�J���� Class
// ---------------------------------------------------------------------------
class TTempDetails {
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
    AnsiString P_Siyou2; // �d�l

    // ��2007.10.4 �~���[�d�l�Ή�
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // ��2007.10.4 �~���[�d�l�Ή�

    bool __fastcall Temp_Data_Sakusei(void); // ���x�v�ް� �쐬
    bool __fastcall Get_Temp_Data(AnsiString a_SE, AnsiString Syurui);
    // ���x�v�ް��擾
    bool __fastcall Search_TEMP_CODE(AnsiString Code); // ���x�v
    AnsiString __fastcall Search_HD_TEMP(AnsiString a_SE,
        AnsiString Key); // ���x�v

    bool __fastcall Write_Temp_Data(AnsiString a_SE);

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // ���x�v���i�W�J���� ���s
    int __fastcall Get_Temp_YobiSu(int i_TmpCd); // ���x�v�\���i�擾

};

// ---------------------------------------------------------------------------
// ���{��֐����F ���x�v���i�W�J����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall TempDetails(HeadsData* pcHeadsData) {
    bool ret;

    TTempDetails tempDetails; // ���x�v�ް� �쐬Class��������,�֐��I����j��

    ret = tempDetails.Excute(pcHeadsData); // ���x�v�ް� �쐬���s

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���x�v���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADS÷���ް����߲�����
    m_pcHeadsData = pcHeadsData;

    // HEADS÷���ް���AnsiString�^�z��Ɏ擾
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********     �w���x�v���i�W�J�����x�J�n     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     �w���x�v���i�W�J�����x�J�n     *********");
    Write_Error_Log("*******************************************************");

    // ***********************************
    // ***  ���x�v�ް��쐬
    // ***********************************
    Temp_Data_Sakusei();

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********     �w���x�v���i�W�J�����x�I��     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     �w���x�v���i�W�J�����x�I��     *********");
    Write_Error_Log("*******************************************************");

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���x�v�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Temp_Data_Sakusei(void) {
    AnsiString s_Tmp_Syurui; // ɽ�َ��
    // 2003.12.22
    AnsiString s_Text;
    // **********
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
    // 2022.04.21 CX-01D�ǉ�_E
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
    // ***               �a�����x�v�f�[�^�@�쐬                     ***
    // ***                                                          ***
    // ****************************************************************

    // ***********************************
    // ***  S1���x�v�ް��@�쐬
    // ***********************************
    s_Tmp_Syurui = P_HEADS_DATA[898].TrimRight(); // S1���x�v���

    if (s_Tmp_Syurui == "00") {
        G_Log_Text = "S1���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    else {
        Get_Temp_Data("S1", s_Tmp_Syurui);
    }

    // 2003.12.24 YX-80 �ǉ�
    ////***********************************
    ////***  S3���x�v�ް��@�쐬
    ////***********************************
    // s_Tmp_Syurui = P_HEADS_DATA[928].TrimRight();   // S3���x�v���
    //
    // if (s_Tmp_Syurui == "00"){
    // G_Log_Text = "S3���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_Temp_Data("S3", s_Tmp_Syurui);
    // }
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // �΍s��
    // *********************
        // ***********************************
        // ***  S4���x�v�ް��@�쐬
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[943].TrimRight(); // S4���x�v���

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "S4���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("S4", s_Tmp_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  S3���x�v�ް��@�쐬
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[928].TrimRight(); // S3���x�v���

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "S3���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("S3", s_Tmp_Syurui);
        }
    }
    // *********************

    // ***********************************
    // ***  E1���x�v�ް��@�쐬
    // ***********************************
    s_Tmp_Syurui = P_HEADS_DATA[958].TrimRight(); // E1���x�v���

    if (s_Tmp_Syurui == "00") {
        G_Log_Text = "E1���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    else {
        Get_Temp_Data("E1", s_Tmp_Syurui);
    }

    // 2003.12.24 YX-80 �ǉ�
    ////***********************************
    ////***  E3���x�v�ް��@�쐬
    ////***********************************
    // s_Tmp_Syurui = P_HEADS_DATA[988].TrimRight();   // E3���x�v���
    //
    // if (s_Tmp_Syurui == "00"){
    // G_Log_Text = "E3���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_Temp_Data("E3", s_Tmp_Syurui);
    // }
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // �΍s��
    // *********************
        // ***********************************
        // ***  E4���x�v�ް��@�쐬
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[1003].TrimRight(); // E4���x�v���

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "E4���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("E4", s_Tmp_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  E3���x�v�ް��@�쐬
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[988].TrimRight(); // E3���x�v���

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "E3���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("E3", s_Tmp_Syurui);
        }
    }
    // *********************

    // ****************************************************************
    // ***                                                          ***
    // ***               �`�����x�v�f�[�^�@�쐬                     ***
    // ***                                                          ***
    // ****************************************************************

    // ***********************************
    // ***  S2���x�v�ް��@�쐬
    // ***********************************
    s_Tmp_Syurui = P_HEADS_DATA[913].TrimRight(); // S2���x�v���

    if (s_Tmp_Syurui == "00") {
        G_Log_Text = "S2���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    else {
        Get_Temp_Data("S2", s_Tmp_Syurui);
    }

    // 2003.12.24 YX-80 �ǉ�
    ////***********************************
    ////***  S4���x�v�ް��@�쐬
    ////***********************************
    // s_Tmp_Syurui = P_HEADS_DATA[943].TrimRight();   // S4���x�v���
    //
    // if (s_Tmp_Syurui == "00"){
    // G_Log_Text = "S4���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_Temp_Data("S4", s_Tmp_Syurui);
    // }
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // �΍s��
    // *********************
        // ***********************************
        // ***  S3���x�v�ް��@�쐬
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[928].TrimRight(); // S3���x�v���

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "S3���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("S3", s_Tmp_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  S4���x�v�ް��@�쐬
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[943].TrimRight(); // S4���x�v���

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "S4���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("S4", s_Tmp_Syurui);
        }
    }
    // *********************

    // ***********************************
    // ***  E2���x�v�ް��@�쐬
    // ***********************************
    s_Tmp_Syurui = P_HEADS_DATA[973].TrimRight(); // E2���x�v���

    if (s_Tmp_Syurui == "00") {
        G_Log_Text = "E2���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    else {
        Get_Temp_Data("E2", s_Tmp_Syurui);
    }

    // 2003.12.24 YX-80 �ǉ�
    // ***********************************
    // ***  E4���x�v�ް��@�쐬
    // ***********************************
    // s_Tmp_Syurui = P_HEADS_DATA[1003].TrimRight();  // E4���x�v���
    //
    // if (s_Tmp_Syurui == "00"){
    // G_Log_Text = "E4���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_Temp_Data("E4", s_Tmp_Syurui);
    // }
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // �΍s��
    // *********************
        // ***********************************
        // ***  E3���x�v�ް��@�쐬
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[988].TrimRight(); // E3���x�v���

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "E3���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("E3", s_Tmp_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  E4���x�v�ް��@�쐬
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[1003].TrimRight(); // E4���x�v���

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "E4���x�v�ް�  ���x�v��ނ��00��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("E4", s_Tmp_Syurui);
        }
    }
    // *********************

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���x�v���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F AnsiString a_SE   �F ���x�vSE�敪(S1�`S4,E1�`E4)
// �F AnsiString Syurui �F ���x�v���
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Get_Temp_Data(AnsiString a_SE, AnsiString Syurui)
{
    AnsiString s_Code; // ���i����

    // *********************************************
    // ***  ���x�v���i�\��Ͻ�(HD_TEMP) ����
    // *********************************************
    s_Code = Search_HD_TEMP(a_SE, Syurui);

    if (s_Code == "\0") {
        G_ErrLog_Text = Syurui + "�ް��쐬���@" + a_SE + "���x�v��� �" + Syurui +
            "� �́A���x�v���i�\��Ͻ�(HD_TEMP)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);

        return false;
    }

    return true;

}

// ---------------------------------------------------------------------------
// ���{��֐����F HD_TEMPð��ٌ�������
// �T  �v      �F
// ��  ��      �F AnsiString a_SE   �F ���x�vSE�敪(S1�`S4,E1�`E4)
// �F AnsiString Key    �F ���x�v���
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall TTempDetails::Search_HD_TEMP(AnsiString a_SE,
    AnsiString Key) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;
    AnsiString s_Code;
    AnsiString s_FrngSize;
    AnsiString s_BaseMatM; // �����n��ގ�
    AnsiString s_BaseMatL; // ������n��ގ�
    AnsiString s_Memori;
    // 2020.01.14 �ݸޒ[�ǕύX_S
    AnsiString s_PressGCode; // ���͌v��ރR�[�h
    // 2020.01.14 �ݸޒ[�ǕύX_E
    // 2003.12.24
    AnsiString s_Text;
    // **********
    bool bFlgANSI; // �`�m�r�h�H
    int i_wYobi;
    int i_Kikaku;
    // 2017.07.11 �΍s���ǉ�
    int iFL;
    // *********************

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_TEMP";
    s_SQL = s_SQL + "  WHERE TEMP_TYPE = '" + Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // �e���ڂ̏�����
    s_Code = "";
    P_Zuban = "";
    P_SZuban = "";
    P_Num = 0;
    P_Zaisitu = "";
    P_Siyou = "";
    P_Siyou2 = "";

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
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1136].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4); // �[�ǂ��t���ꍇ��BTANM���g��
        //}
        //// 2014.12.19 �ގ��擾��ǉ�
        //else if (P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //   // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1136].TrimRight()) != "-") {
            // �[�ǂ��t���ꍇ��BTANM���g��
            s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1357].TrimRight() == "1160") {
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1357].TrimRight()) != "-") {
           // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // if(P_HEADS_DATA[893].SubString(2,1) == "4") {         // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
            if (P_HEADS_DATA[893].SubString(3, 1) == "4")
            { // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
                // ***********************
                s_BaseMatM = P_HEADS_DATA[894].TrimRight();
                // 2014.06.24 ����ް��������ݼޒǉ�
            }
            else if (P_HEADS_DATA[893].SubString(3, 1) == "2")
            { // �`���[�u�t�����W�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[894].TrimRight();
                // 2018.03.26 ���x�v���ގ��ύX_S
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
                // 2018.03.26 ���x�v���ގ��ύX_E
            }
            else if (P_HEADS_DATA[893].SubString(3, 1) == "3")
            { // ���^���u�[�c�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[894].TrimRight();
                // ********************************
            }
            else { // �u�[�c�m�Y���̏ꍇ�́A�v���[�g�ގ����g��
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
        }
        s_Memori = P_HEADS_DATA[899].TrimRight() + "/" +
            P_HEADS_DATA[900].TrimRight() + "��";
        s_FrngSize = P_HEADS_DATA[892].TrimRight();
        s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[893]);
        // if(P_HEADS_DATA[893].SubString(1,1) == "4" || P_HEADS_DATA[893].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
        // 2020.01.14 �ݸޒ[�ǕύX_S
        s_PressGCode = P_HEADS_DATA[901].TrimRight(); // ���͌v��ރR�[�h
        // 2020.01.14 �ݸޒ[�ǕύX_E
    }
    else if (a_SE == "S2") {
        //// 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1137].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4); // �[�ǂ��t���ꍇ��ATANM���g��
        //}
        //// 2014.12.19 �ގ��擾��ǉ�
        //else if (P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //   // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1137].TrimRight()) != "-") {
            // �[�ǂ��t���ꍇ��ATANM���g��
            s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1358].TrimRight() == "1160") {
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1358].TrimRight()) != "-") {
           // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // if(P_HEADS_DATA[908].SubString(2,1) == "4") {         // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
            if (P_HEADS_DATA[908].SubString(3, 1) == "4")
            { // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
                // ***********************
                s_BaseMatM = P_HEADS_DATA[909].TrimRight();
                // 2014.06.24 ����ް��������ݼޒǉ�
            }
            else if (P_HEADS_DATA[908].SubString(3, 1) == "2")
            { // �`���[�u�t�����W�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[909].TrimRight();
                // 2018.03.26 ���x�v���ގ��ύX_S
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
                // 2018.03.26 ���x�v���ގ��ύX_E
            }
            else if (P_HEADS_DATA[908].SubString(3, 1) == "3")
            { // ���^���u�[�c�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[909].TrimRight();
                // ********************************
            }
            else { // �u�[�c�m�Y���̏ꍇ�́A�v���[�g�ގ����g��
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
        }
        s_Memori = P_HEADS_DATA[914].TrimRight() + "/" +
            P_HEADS_DATA[915].TrimRight() + "��";
        s_FrngSize = P_HEADS_DATA[907].TrimRight();
        s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[908]);
        // if(P_HEADS_DATA[908].SubString(1,1) == "4" || P_HEADS_DATA[908].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
        // 2020.01.14 �ݸޒ[�ǕύX_S
        s_PressGCode = P_HEADS_DATA[916].TrimRight(); // ���͌v��ރR�[�h
        // 2020.01.14 �ݸޒ[�ǕύX_E
    }
    else if (a_SE == "S3") {
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1138].ToIntDef(0) != 0) {
        //    // 2003.12.24 YX-80 �ǉ�
        //    // s_BaseMatM = P_HEADS_DATA[1060].SubString(1,4);       // �[�ǂ��t���ꍇ��BTANM���g��
        //    s_Text = P_HEADS_DATA[34].TrimRight();
        //    if (s_Text.SubString(1, 2) == "YX") {
        //        s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        //        // �[�ǂ��t���ꍇ��ATANM���g��
        //    }
        //    else {
        //        s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        //        // �[�ǂ��t���ꍇ��BTANM���g��
        //    }
        //    // *********************
        //}
        //// 2014.12.19 �ގ��擾��ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //   // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1138].TrimRight()) != "-") {
            // 2017.07.11 �΍s���ǉ�
            if (iFL == 1){
            //s_Text = P_HEADS_DATA[34].TrimRight();
            //if (s_Text.SubString(1, 2) == "YX") {
                // �΍s��
            // *********************
                // �[�ǂ��t���ꍇ��ATANM���g��
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            }
            else {
                // �[�ǂ��t���ꍇ��BTANM���g��
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            }
        }
        // 2017.07.11 �΍s���ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].TrimRight() == "1160") {
        else if (iFL == 1 && P_HEADS_DATA[1358].TrimRight() == "1160") {
        // *********************
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        // 2017.07.11 �΍s���ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].TrimRight() != "-") {
        else if (iFL == 1 && P_HEADS_DATA[1358].TrimRight() != "-") {
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // *********************
        else if (P_HEADS_DATA[1357].TrimRight() == "1160") {
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1357].TrimRight()) != "-") {
           // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        // ***********************
        else {
            // 2003.12.24
            // if(P_HEADS_DATA[993].SubString(2,1) == "4" ) {        // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if(P_HEADS_DATA[923].SubString(2,1) == "4" ) {        // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
            if (P_HEADS_DATA[923].SubString(3, 1) == "4")
            { // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
                // ***********************
                // **********
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // 2014.06.24 ����ް��������ݼޒǉ�
            }
            else if (P_HEADS_DATA[923].SubString(3, 1) == "2")
            { // �`���[�u�t�����W�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // 2018.03.26 ���x�v���ގ��ύX_S
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
                // 2018.03.26 ���x�v���ގ��ύX_E
            }
            else if (P_HEADS_DATA[923].SubString(3, 1) == "3")
            { // ���^���u�[�c�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // ********************************
            }
            else { // �u�[�c�m�Y���̏ꍇ�́A�v���[�g�ގ����g��
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
        }
        s_Memori = P_HEADS_DATA[929].TrimRight() + "/" +
            P_HEADS_DATA[930].TrimRight() + "��";
        s_FrngSize = P_HEADS_DATA[922].TrimRight();
        // 2003.12.24 YX-80 �ǉ�
        // s_BaseMatL =  P_HEADS_DATA[835].TrimRight();
        // i_Kikaku = GetNzKikaku( P_HEADS_DATA[993] );
        // 2017.07.11 �΍s���ǉ�
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
        // *********************
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        else {
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[923]);
        // *********************
        // if(P_HEADS_DATA[993].SubString(1,1) == "4" || P_HEADS_DATA[993].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
        // 2020.01.14 �ݸޒ[�ǕύX_S
        if (iFL == 1){
            //S2 �΍s��
            s_PressGCode = P_HEADS_DATA[916].TrimRight(); // ���͌v��ރR�[�h
        }
        else {
            //S1
            s_PressGCode = P_HEADS_DATA[901].TrimRight(); // ���͌v��ރR�[�h
        }
        // 2020.01.14 �ݸޒ[�ǕύX_E
    }
    else if (a_SE == "S4") {
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1139].ToIntDef(0) != 0) {
        //    // 2003.12.24 YX-80 �ǉ�
        //    // s_BaseMatM = P_HEADS_DATA[1061].SubString(1,4);       // �[�ǂ��t���ꍇ��ATANM���g��
        //    s_Text = P_HEADS_DATA[34].TrimRight();
        //    if (s_Text.SubString(1, 2) == "YX") {
        //        s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        //        // �[�ǂ��t���ꍇ��BTANM���g��
        //    }
        //    else {
        //        s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        //        // �[�ǂ��t���ꍇ��ATANM���g��
        //    }
        //    // *********************
        //}
        //// 2014.12.19 �ގ��擾��ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //   // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1139].TrimRight()) != "-") {
            s_Text = P_HEADS_DATA[34].TrimRight();
            // 2017.07.11 �΍s���ǉ�
            if (iFL == 1){
            //if (s_Text.SubString(1, 2) == "YX") {
                // �΍s��
            // *********************
                // �[�ǂ��t���ꍇ��BTANM���g��
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            }
            else {
                // �[�ǂ��t���ꍇ��ATANM���g��
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            }
        }
        // 2017.07.11 �΍s���ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].TrimRight() == "1160") {
        else if (iFL == 1 && P_HEADS_DATA[1357].TrimRight() == "1160") {
        // ***********************
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        // 2017.07.11 �΍s���ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].TrimRight() != "-") {
        else if (iFL == 1 && P_HEADS_DATA[1357].TrimRight() != "-") {
        // ***********************
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1358].TrimRight() == "1160") {
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1358].TrimRight()) != "-") {
           // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // if(P_HEADS_DATA[938].SubString(2,1) == "4" ) {        // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
            if (P_HEADS_DATA[938].SubString(3, 1) == "4")
            { // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // 2014.06.24 ����ް��������ݼޒǉ�
            }
            else if (P_HEADS_DATA[938].SubString(3, 1) == "2")
            { // �`���[�u�t�����W�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // 2018.03.26 ���x�v���ގ��ύX_S
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
                // 2018.03.26 ���x�v���ގ��ύX_E
            }
            else if (P_HEADS_DATA[938].SubString(3, 1) == "3")
            { // ���^���u�[�c�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // ********************************
            }
            else { // �u�[�c�m�Y���̏ꍇ�́A�v���[�g�ގ����g��
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
        }
        s_Memori = P_HEADS_DATA[944].TrimRight() + "/" +
            P_HEADS_DATA[945].TrimRight() + "��";
        s_FrngSize = P_HEADS_DATA[937].TrimRight();
        // 2003.12.24 YX-80 �ǉ�
        // s_BaseMatL =  P_HEADS_DATA[836].TrimRight();
        // 2017.07.11 �΍s���ǉ�
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
        // *********************
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        else {
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        // *********************
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[938]);
        // if(P_HEADS_DATA[938].SubString(1,1) == "4" || P_HEADS_DATA[938].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
        // 2020.01.14 �ݸޒ[�ǕύX_S
        if (iFL == 1){
            //S1 �΍s��
            s_PressGCode = P_HEADS_DATA[901].TrimRight(); // ���͌v��ރR�[�h
        }
        else {
            //S2
            s_PressGCode = P_HEADS_DATA[916].TrimRight(); // ���͌v��ރR�[�h
        }
        // 2020.01.14 �ݸޒ[�ǕύX_E
    }
    else if (a_SE == "E1") {
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1140].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4); // �[�ǂ��t���ꍇ��BTANM���g��
        //}
        //// 2014.12.19 �ގ��擾��ǉ�
        //else if (P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //   // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1140].TrimRight()) != "-") {
            // �[�ǂ��t���ꍇ��BTANM���g��
            s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1357].TrimRight() == "1160") {
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1357].TrimRight()) != "-") {
           // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // if(P_HEADS_DATA[953].SubString(2,1) == "4" ) {        // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
            if (P_HEADS_DATA[953].SubString(3, 1) == "4")
            { // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
                // ***********************
                s_BaseMatM = P_HEADS_DATA[954].TrimRight();
                // 2014.06.24 ����ް��������ݼޒǉ�
            }
            else if (P_HEADS_DATA[953].SubString(3, 1) == "2")
            { // �`���[�u�t�����W�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[954].TrimRight();
                // 2018.03.26 ���x�v���ގ��ύX_S
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
                // 2018.03.26 ���x�v���ގ��ύX_E
            }
            else if (P_HEADS_DATA[953].SubString(3, 1) == "3")
            { // ���^���u�[�c�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[954].TrimRight();
                // ********************************
            }
            else { // �u�[�c�m�Y���̏ꍇ�́A�v���[�g�ގ����g��
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
        }
        s_Memori = P_HEADS_DATA[959].TrimRight() + "/" +
            P_HEADS_DATA[960].TrimRight() + "��";
        s_FrngSize = P_HEADS_DATA[952].TrimRight();
        s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[953]);
        // if(P_HEADS_DATA[953].SubString(1,1) == "4" || P_HEADS_DATA[953].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
    }
    else if (a_SE == "E2") {
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1141].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4); // �[�ǂ��t���ꍇ��ATANM���g��
        //}
        //// 2014.12.19 �ގ��擾��ǉ�
        //else if (P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //   // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1141].TrimRight()) != "-") {
            // �[�ǂ��t���ꍇ��ATANM���g��
            s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1358].TrimRight() == "1160") {
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1358].TrimRight()) != "-") {
           // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // if(P_HEADS_DATA[968].SubString(2,1) == "4" ) {        // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
            if (P_HEADS_DATA[968].SubString(3, 1) == "4")
            { // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
                // ***********************
                s_BaseMatM = P_HEADS_DATA[969].TrimRight();
                // 2014.06.24 ����ް��������ݼޒǉ�
            }
            else if (P_HEADS_DATA[968].SubString(3, 1) == "2")
            { // �`���[�u�t�����W�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[969].TrimRight();
                // 2018.03.26 ���x�v���ގ��ύX_S
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
                // 2018.03.26 ���x�v���ގ��ύX_E
            }
            else if (P_HEADS_DATA[968].SubString(3, 1) == "3")
            { // ���^���u�[�c�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[969].TrimRight();
                // ********************************
            }
            else { // �u�[�c�m�Y���̏ꍇ�́A�v���[�g�ގ����g��
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
        }
        s_Memori = P_HEADS_DATA[974].TrimRight() + "/" +
            P_HEADS_DATA[975].TrimRight() + "��";
        s_FrngSize = P_HEADS_DATA[967].TrimRight();
        s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[968]);
        // if(P_HEADS_DATA[968].SubString(1,1) == "4" || P_HEADS_DATA[968].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
    }
    else if (a_SE == "E3") {
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1142].ToIntDef(0) != 0) {
        //    // 2003.12.24 YX-80 �ǉ�
        //    // s_BaseMatM = P_HEADS_DATA[1060].SubString(1,4);       // �[�ǂ��t���ꍇ��BTANM���g��
        //    s_Text = P_HEADS_DATA[34].TrimRight();
        //    if (s_Text.SubString(1, 2) == "YX") {
        //        s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        //        // �[�ǂ��t���ꍇ��ATANM���g��
        //    }
        //    else {
        //        s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        //        // �[�ǂ��t���ꍇ��BTANM���g��
        //    }
        //    // *********************
        //}
        //// 2014.12.19 �ގ��擾��ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //   // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1142].TrimRight()) != "-") {
            // 2017.07.11 �΍s���ǉ�
            if (iFL == 1){
            //s_Text = P_HEADS_DATA[34].TrimRight();
            //if (s_Text.SubString(1, 2) == "YX") {
                // �΍s��
            // *********************
                // �[�ǂ��t���ꍇ��ATANM���g��
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            }
            else {
                // �[�ǂ��t���ꍇ��BTANM���g��
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            }
        }
        // 2017.07.11 �΍s���ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].TrimRight() == "1160") {
        else if (iFL == 1 && P_HEADS_DATA[1358].TrimRight() == "1160") {
        // ***********************
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        // 2017.07.11 �΍s���ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].TrimRight() != "-") {
        else if (iFL == 1 && P_HEADS_DATA[1358].TrimRight() != "-") {
        // ***********************
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1357].TrimRight() == "1160") {
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1357].TrimRight()) != "-") {
           // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // if(P_HEADS_DATA[983].SubString(2,1) == "4" ) {        // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
            if (P_HEADS_DATA[983].SubString(3, 1) == "4")
            { // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
                // ***********************
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // 2014.06.24 ����ް��������ݼޒǉ�
            }
            else if (P_HEADS_DATA[983].SubString(3, 1) == "2")
            { // �`���[�u�t�����W�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // 2018.03.26 ���x�v���ގ��ύX_S
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
                // 2018.03.26 ���x�v���ގ��ύX_E
            }
            else if (P_HEADS_DATA[983].SubString(3, 1) == "3")
            { // ���^���u�[�c�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // ********************************
            }
            else { // �u�[�c�m�Y���̏ꍇ�́A�v���[�g�ގ����g��
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
        }
        s_Memori = P_HEADS_DATA[989].TrimRight() + "/" +
            P_HEADS_DATA[990].TrimRight() + "��";
        s_FrngSize = P_HEADS_DATA[982].TrimRight();
        // 2003.12.24 YX-80 �ǉ�
        // s_BaseMatL =  P_HEADS_DATA[835].TrimRight();
        // 2017.07.11 �΍s���ǉ�
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
        // *********************
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        else {
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        // *********************
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[983]);
        // if(P_HEADS_DATA[983].SubString(1,1) == "4" || P_HEADS_DATA[983].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
    }
    else if (a_SE == "E4") {
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1143].ToIntDef(0) != 0) {
        //    // 2003.12.24 YX-80 �ǉ�
        //    // s_BaseMatM = P_HEADS_DATA[1061].SubString(1,4);       // �[�ǂ��t���ꍇ��ATANM���g��
        //    s_Text = P_HEADS_DATA[34].TrimRight();
        //    if (s_Text.SubString(1, 2) == "YX") {
        //        s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        //        // �[�ǂ��t���ꍇ��BTANM���g��
        //    }
        //    else {
        //        s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        //        // �[�ǂ��t���ꍇ��ATANM���g��
        //    }
        //    // *********************
        //}
        //// 2014.12.19 �ގ��擾��ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //   // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1143].TrimRight()) != "-") {
            // 2017.07.11 �΍s���ǉ�
            if (iFL == 1){
            //s_Text = P_HEADS_DATA[34].TrimRight();
            //if (s_Text.SubString(1, 2) == "YX") {
                // �΍s��
            // *********************
                // �[�ǂ��t���ꍇ��BTANM���g��
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            }
            else {
                // �[�ǂ��t���ꍇ��ATANM���g��
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            }
        }
        // 2017.07.11 �΍s���ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].TrimRight() == "1160") {
        else if (iFL == 1 && P_HEADS_DATA[1357].TrimRight() == "1160") {
        // ***********************
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        // 2017.07.11 �΍s���ǉ�
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].TrimRight() != "-") {
        else if (iFL == 1 && P_HEADS_DATA[1357].TrimRight() != "-") {
        // ***********************
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1358].TrimRight() == "1160") {
            // ���x�v�ی�ǂ�SUS403�̏ꍇ��SUS316���g��
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1358].TrimRight()) != "-") {
           // ���x�v�ی�ǂ��t���̏ꍇ�͂��̍ގ����g��
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ɽ�ًK�i�ύX
            // if(P_HEADS_DATA[998].SubString(2,1) == "4" ) {        // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
            if (P_HEADS_DATA[998].SubString(3, 1) == "4")
            { // �t�����W�t���m�Y���̏ꍇ�́A�m�Y���ގ����g��
                // ***********************
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // 2014.06.24 ����ް��������ݼޒǉ�
            }
            else if (P_HEADS_DATA[998].SubString(3, 1) == "2")
            { // �`���[�u�t�����W�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // 2018.03.26 ���x�v���ގ��ύX_S
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
                // 2018.03.26 ���x�v���ގ��ύX_E
            }
            else if (P_HEADS_DATA[998].SubString(3, 1) == "3")
            { // ���^���u�[�c�̏ꍇ�́A�m�Y���ގ����g��
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // ********************************
            }
            else { // �u�[�c�m�Y���̏ꍇ�́A�v���[�g�ގ����g��
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY�ގ��ϊ�_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY�ށ�JIS��
                // 2024.02.06 ALLOY�ގ��ϊ�_E
            }
        }
        s_Memori = P_HEADS_DATA[1004].TrimRight() + "/" +
            P_HEADS_DATA[1005].TrimRight() + "��";
        s_FrngSize = P_HEADS_DATA[997].TrimRight();
        // 2003.12.24 YX-80 �ǉ�
        // s_BaseMatL =  P_HEADS_DATA[836].TrimRight();
        // 2017.07.11 �΍s���ǉ�
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
        // *********************
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        else {
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        // *********************
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[998]);
        // if(P_HEADS_DATA[998].SubString(1,1) == "4" || P_HEADS_DATA[998].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
    }
    if (i_Kikaku != 2)
        s_FrngSize = FormatFloat("0", StrToDblDef(s_FrngSize.TrimRight(), 0.0));

    // i_wYobi = Get_Temp_YobiSu(Key.ToIntDef(0));
    i_wYobi = 0;

    while (!wkQuery->Eof) {
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // ���i����
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // �}��
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // ��}�p�}��
        P_Num = wkQuery->FieldByName("NUM")->AsCurrency; // ����
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // �ގ�����
        P_EZaisitu = P_Zaisitu;
        P_Siyou = wkQuery->FieldByName("PARTS_SPEC")->AsString.TrimRight
            (); // �d�l

        // *********************************
        // ***  ���x�v�\�����ið��ٌ���
        // *********************************
        Search_TEMP_CODE(s_Code);

        // *********************************
        // ***  �\���i�� �擾(02.08.05�ǉ�)
        // *********************************
        // P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin,"0");

        P_Yobisu = 0;
        switch (s_Code.ToIntDef(0)) {
        case 1: // �\�P�b�g
            // if (bFlgANSI == true ) {
            if (i_Kikaku == 1) { // ANSI
                P_Siyou2 = FormatFloat("0", s_FrngSize.ToDouble() * 25) + "A";
            }
            else if (i_Kikaku == 2) { // ����
                P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            }
            else {
                P_Siyou2 = s_FrngSize + "A";
            }
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 1);
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            break;
        case 2: // �ی��
            // 2009.02.16 ���x�v���ޒǉ�
            // if ( Key == "02" ) {
            // P_Siyou2 = "25A";
            // P_Zaisitu = "�r�t�r�S�P�O";
            // } else {
            ////            if (bFlgANSI == true ) {
            // if (i_Kikaku == 1 ) {         // ANSI
            // P_Siyou2 = FormatFloat("0",s_FrngSize.ToDouble() * 25) + "A";
            // } else if ( i_Kikaku == 2 ) { // ����
            // P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            // } else {
            // P_Siyou2 = s_FrngSize + "A";
            // }
            ////            P_Zaisitu = Chg_Zaisitu_Code( s_BaseMatM, 1);
            // // �ی�ǂ̍ގ��́AHEADS�f�[�^����擾����悤�ύX�@2002.12.26
            // //2003.12.24 YX-80 �ǉ�
            // //if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "3" ) )
            // //   P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
            // //else
            // //   P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            // s_Text = P_HEADS_DATA[34].TrimRight();
            // if (s_Text.SubString(1,2) == "YX"){
            // if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "4" ) )
            // P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
            // else
            // P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            // } else {
            // if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "3" ) )
            // P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
            // else
            // P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            // }
            // //*********************
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // }
            if (Key == "01" || Key == "05" || Key == "06") {
                if (i_Kikaku == 1) { // ANSI
                    if (s_FrngSize.ToDouble() <= 1) {
                        P_Siyou2 = "56L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 2) {
                        P_Siyou2 = "71L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 4) {
                        P_Siyou2 = "101L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 14) {
                        P_Siyou2 = "131L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 =
                        P_Siyou2 + FormatFloat("0",
                    s_FrngSize.ToDouble() * 25) + "A";
                    // 2020.01.14 �ݸޒ[�ǕύX_S
                    if (s_FrngSize.ToDouble() == 2) {
                        // 2020.06.03 ���x�v�ύX_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 ���x�v�ύX_E
                            P_Siyou2 = "101L 100A";
                        }
                    }
                    // 2020.01.14 �ݸޒ[�ǕύX_E
                }
                else {
                    if (s_FrngSize.ToDouble() <= 25) {
                        P_Siyou2 = "56L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 50) {
                        P_Siyou2 = "71L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 100) {
                        P_Siyou2 = "101L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 350) {
                        P_Siyou2 = "131L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 = P_Siyou2 + s_FrngSize + "A";
                    // 2020.01.14 �ݸޒ[�ǕύX_S
                    if (s_FrngSize.ToDouble() == 50) {
                        // 2020.06.03 ���x�v�ύX_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 ���x�v�ύX_E
                            P_Siyou2 = "101L 100A";
                        }
                    }
                    // 2020.01.14 �ݸޒ[�ǕύX_E
                }
                // 2012.01.20 ���p���щ��x�v�ǉ�
                // } else if ( Key == "02" ) {
            }
            else if (Key == "02" || Key == "10" || Key == "11") {
                // *****************************
                P_Siyou2 = "56L 25A";
            }
            else if (Key == "03") {
                if (s_FrngSize.ToDouble() <= 2.5) {
                    P_Siyou2 = "65L ";
                }
                else if (s_FrngSize.ToDouble() <= 4) {
                    P_Siyou2 = "105L ";
                }
                else {
                    P_Siyou2 = "";
                }
                P_Siyou2 = P_Siyou2 + ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            }
            else if (Key == "04") {
                P_Siyou2 = "65L 1 1/2S";
            }
            else {
                if (i_Kikaku == 1) { // ANSI
                    P_Siyou2 =
                        FormatFloat("0", s_FrngSize.ToDouble() * 25) + "A";
                }
                else if (i_Kikaku == 2) { // ����
                    P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
                }
                else {
                    P_Siyou2 = s_FrngSize + "A";
                }
            }
            // 2012.01.20 ���p���щ��x�v�ǉ�
            // if ( Key == "02" || Key == "04" ) {
            // 2014.06.24 �ی�Ǎގ��ύX
            // if ( Key == "02" || Key == "04" || Key == "10" || Key == "11" ) {
            //// *****************************
            // P_Zaisitu = "�r�t�r�S�P�O";
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // } else {
            // *************************
            // �ی�ǂ̍ގ��́AHEADS�f�[�^����擾����
            // 2017.07.11 �΍s���ǉ�
            if (iFL == 1){
            //s_Text = P_HEADS_DATA[34].TrimRight();
            //if (s_Text.SubString(1, 2) == "YX") {
            // *********************
                if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2,
                    1) == "4"))
                    P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
                else
                    P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            }
            else {
                if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2,
                    1) == "3"))
                    P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
                else
                    P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            }
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // 2014.06.24 �ی�Ǎގ��ύX
            // }
            // *************************
            P_Yobisu = i_wYobi;
            break;
        case 5: // ��
            // 2009.02.16 ���x�v���ޒǉ�
            if (i_Kikaku == 1) { // ANSI
                P_Siyou2 = FormatFloat("0", s_FrngSize.ToDouble() * 25) + "A";
            }
            else if (i_Kikaku == 2) { // ����
                P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            }
            else {
                P_Siyou2 = s_FrngSize + "A";
            }
            // *************************
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 1);
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            P_Yobisu = i_wYobi;
            break;
        case 3: // ���x�v(��)
            P_Siyou = s_Memori;
            // 2009.02.16 ���x�v���ޒǉ�
            // if ( Key == "02" ) {
            // P_Siyou2 = "25A";
            // } else {
            ////            if (bFlgANSI == true ) {
            // if ( i_Kikaku == 1 ) {              // ANSI
            // P_Siyou2 = FormatFloat("0",s_FrngSize.ToDouble() * 25) + "A";
            // } else {
            // P_Siyou2 = s_FrngSize + "A";
            // }
            // }
            // 2013/06/28 ���x�v���@����
            if (Key == "02") {
                // P_Siyou2 = "215L 25A";
                P_Siyou2 = "40L 25A";
            }
            else {
                if (i_Kikaku == 1) { // ANSI
                    if (s_FrngSize.ToDouble() <= 1) {
                        // P_Siyou2 = "215L ";
                        P_Siyou2 = "40L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 2) {
                        // P_Siyou2 = "235L ";
                        P_Siyou2 = "60L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 4) {
                        // P_Siyou2 = "260L ";
                        P_Siyou2 = "90L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 14) {
                        // P_Siyou2 = "295L ";
                        P_Siyou2 = "120L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 =
                        P_Siyou2 + FormatFloat("0",
                    s_FrngSize.ToDouble() * 25) + "A";
                    // 2020.01.14 �ݸޒ[�ǕύX_S
                    if (s_FrngSize.ToDouble() == 2) {
                        // 2020.06.03 ���x�v�ύX_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 ���x�v�ύX_E
                            P_Siyou2 = "90L 100A";
                        }
                    }
                    // 2020.01.14 �ݸޒ[�ǕύX_E
                }
                else {
                    if (s_FrngSize.ToDouble() <= 25) {
                        // P_Siyou2 = "215L ";
                        P_Siyou2 = "40L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 50) {
                        // P_Siyou2 = "235L ";
                        P_Siyou2 = "60L ";
                        // 2020.01.14 �ݸޒ[�ǕύX_S
                        // 2020.06.03 ���x�v�ύX_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 ���x�v�ύX_E
                            P_Siyou2 = "90L 100A";
                        }
                        // 2020.01.14 �ݸޒ[�ǕύX_E
                    }
                    else if (s_FrngSize.ToDouble() <= 100) {
                        // P_Siyou2 = "260L ";
                        P_Siyou2 = "90L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 350) {
                        // P_Siyou2 = "295L ";
                        P_Siyou2 = "120L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 = P_Siyou2 + s_FrngSize + "A";
                    // 2020.01.14 �ݸޒ[�ǕύX_S
                    if (s_FrngSize.ToDouble() == 50) {
                        // 2020.06.03 ���x�v�ύX_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 ���x�v�ύX_E
                            P_Siyou2 = "90L 100A";
                        }
                    }
                    // 2020.01.14 �ݸޒ[�ǕύX_E
                }
            }
            // **************************
            P_Yobisu = i_wYobi;
            break;
        case 6: // ���x�v(��)
            P_Siyou = s_Memori;
            // 2009.02.16 ���x�v���ޒǉ�
            // P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            if (Key == "04") {
                P_Siyou2 = "170L 1 1/2S";
            }
            else {
                if (s_FrngSize.ToDouble() <= 2.5) {
                    P_Siyou2 = "170L ";
                }
                else if (s_FrngSize.ToDouble() <= 4) {
                    P_Siyou2 = "210L ";
                }
                else {
                    P_Siyou2 = "";
                }
                P_Siyou2 = P_Siyou2 + ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            }
            // *************************
            P_Yobisu = i_wYobi;
            break;
        case 12: // �_�C���������x�v
        case 13: // ���x�v
        case 14: // I�^���x�v
            P_Siyou = P_Siyou + s_Memori;
            P_Yobisu = i_wYobi;
            break;
        case 4: // �P�[�X(��)
        case 7: // �P�[�X(��)
            P_Siyou2 = "";
            P_Yobisu = i_wYobi;
        case 8: // �܃i�b�g
            break;
        case 9: // �p�b�L���O
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatL, 1);
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            P_Yobisu = i_wYobi;
            break;
        case 10: // I�^���p���x�v
        case 11: // L�^���p���x�v
            P_Siyou = P_Siyou + s_Memori;
            // 2012.01.20 ���p���щ��x�v�ǉ�
            ////2009.02.16 ���x�v���ޒǉ�
            // if (i_Kikaku == 1 ) {         // ANSI
            // if (s_FrngSize.ToDouble() <= 2) {
            // P_Siyou2 = "60L ";
            // } else if (s_FrngSize.ToDouble() <= 4) {
            // P_Siyou2 = "85L ";
            // } else if (s_FrngSize.ToDouble() <= 14) {
            // P_Siyou2 = "120L ";
            // } else {
            // P_Siyou2 = "";
            // }
            // P_Siyou2 = P_Siyou2 + FormatFloat("0",s_FrngSize.ToDouble() * 25) + "A";
            // } else {
            // if (s_FrngSize.ToDouble() <= 50) {
            // P_Siyou2 = "60L ";
            // } else if (s_FrngSize.ToDouble() <= 100) {
            // P_Siyou2 = "85L ";
            // } else if (s_FrngSize.ToDouble() <= 350) {
            // P_Siyou2 = "120L ";
            // } else {
            // P_Siyou2 = "";
            // }
            // P_Siyou2 = P_Siyou2 + s_FrngSize + "A";
            // }
            // *************************
            if (Key == "10" || Key == "11") {
                P_Siyou2 = "40L 25A";
            }
            else {
                if (i_Kikaku == 1) { // ANSI
                    if (s_FrngSize.ToDouble() <= 2) {
                        P_Siyou2 = "60L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 4) {
                        P_Siyou2 = "85L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 14) {
                        P_Siyou2 = "120L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 =
                        P_Siyou2 + FormatFloat("0",
                    s_FrngSize.ToDouble() * 25) + "A";
                    // 2020.01.14 �ݸޒ[�ǕύX_S
                    if (s_FrngSize.ToDouble() == 2) {
                        // 2020.06.03 ���x�v�ύX_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 ���x�v�ύX_E
                            P_Siyou2 = "85L 100A";
                        }
                    }
                    // 2020.01.14 �ݸޒ[�ǕύX_E
                }
                else {
                    if (s_FrngSize.ToDouble() <= 50) {
                        P_Siyou2 = "60L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 100) {
                        P_Siyou2 = "85L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 350) {
                        P_Siyou2 = "120L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 = P_Siyou2 + s_FrngSize + "A";
                    // 2020.01.14 �ݸޒ[�ǕύX_S
                    if (s_FrngSize.ToDouble() == 50) {
                        // 2020.06.03 ���x�v�ύX_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 ���x�v�ύX_E
                            P_Siyou2 = "85L 100A";
                        }
                    }
                    // 2020.01.14 �ݸޒ[�ǕύX_E
                }
            }
            // *****************************
            // P_Zaisitu = Chg_Zaisitu_Code( s_BaseMatM, 2);
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // 2009.02.16 ���x�v�ގ��폜
            // if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "3" ) )
            // P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
            // else
            // P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // *************************
            P_Yobisu = i_wYobi;
            break;
        default:
            break;
        }

        Write_Temp_Data(a_SE);
        wkQuery->Next();
    }

    delete wkQuery;
    return s_Code;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���x�v�\�����i����ð��ف@����
// �T  �v      �F
// ��  ��      �F AnsiString Code  // �����Ώۺ���
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Search_TEMP_CODE(AnsiString Code) {
    int i;

    for (i = 0; i < G_TEMPCD_RecCnt; i++) {
        if (G_TEMPCD[i][0] == Code) {

            P_Sikibetu = G_TEMPCD[i][1]; // ���ʎq(CSV̧�ٍ쐬�p)
            P_Katasiki = G_TEMPCD[i][3]; // ���i����(CSV̧�ٍ쐬�p)

            P_Yobihin = G_TEMPCD[i][4]; // �\���i
            break;
        }
    }

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���x�v�ް�̧�ُ����ݏ���
// �T  �v      �F
// ��  ��      �F AnsiString a_SE   �F ���x�vSE�敪(S1�`S4,E1�`E4)
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Write_Temp_Data(AnsiString a_SE) {
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
    // s_Text = "B_TEMP";
    // else
    // s_Text = "A_TEMP";
    // 2017.07.11 �΍s���ǉ�
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
    // *********************
        if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2, 1) == "4"))
            s_Text = "B_TEMP";
        else
            s_Text = "A_TEMP";
    }
    else {
        if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2, 1) == "3"))
            s_Text = "B_TEMP";
        else
            s_Text = "A_TEMP";
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
    s_Text = ",1";
    fputs(s_Text.c_str(), wkfp);

    // ,9  �\����
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), wkfp);

    // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
    i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // �䐔
    i_Soukei = 1 * i_Daisu + P_Yobisu;
    s_Text = "," + AnsiString(i_Soukei);
    fputs(s_Text.c_str(), wkfp);

    // 11 �d�l�P
    s_Text = "," + P_Siyou;
    fputs(s_Text.c_str(), wkfp);
    // 12 �d�l�Q
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), wkfp);

    // 13 �d�l�R,�S,�T
    s_Text = ",,,";
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
// ���{��֐����F ���x�v�\���i�擾����
// �T  �v      �F
// ��  ��      �F ���x�v�敪(01�`14)
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
int __fastcall TTempDetails::Get_Temp_YobiSu(int i_TmpCd) {
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
    s_SQL = s_SQL + "SELECT * FROM HD_TMP2YOBI";
    s_SQL = s_SQL + "  WHERE TMPPCD = " + i_TmpCd;

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
