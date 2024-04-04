// ----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�Fɽ�ٕ��i�W�J����  ���W���[��
// �� �� �ҁFN.Uchida
// �� �� ���F2002.06.24
// �X �V ���F2002.10.16
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

#include "Bas_NozzleDetails.h"

#include "HeadsData.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// ��ڰĕ��i�W�J���� Class
// ---------------------------------------------------------------------------
class TNozzleDetails {
private:
    FILE *fp;

    AnsiString P_Sikibetu; // ���ʎq
    AnsiString P_Katasiki; // ���i�^��
    AnsiString P_Zaisitu; // �ގ�����
    AnsiString P_EZaisitu; // �ގ�����
    AnsiString P_ZaisituCD; // �ގ��R�[�h
    AnsiString P_Zuban; // �}��
    AnsiString P_Zuban1; // �}��1
    AnsiString P_Zuban2; // �}��2
    AnsiString P_Zuban3; // �}��3
    AnsiString P_Zuban4; // �}��4
    AnsiString P_SZuban; // ��}�p�}��
    AnsiString P_SZuban1; // ��}�p�}��1
    AnsiString P_SZuban2; // ��}�p�}��2
    AnsiString P_SZuban3; // ��}�p�}��3
    AnsiString P_SZuban4; // ��}�p�}��4
    int P_Yobisu; // �\����
    AnsiString P_Siyou1; // �d�l�P
    AnsiString P_Siyou2; // �d�l�Q
    AnsiString P_Siyou3; // �d�l�R
    AnsiString P_Hosoku; // �[�Ǘp�⑫
    AnsiString P_BV; // �[�Ǘp�⑫
    int P_SUS; // SUS�g�p�t���O

    // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
    AnsiString P_Zuban5; // �}��5
    AnsiString P_SZuban5; // ��}�p�}��5
    // ***********************************

    AnsiString P_Yobihin; // �\���i��

    // ��2002/10/24
    AnsiString P_Model; // �^��
    // ��2002/10/24

    int P_WriteFlg; // ɽ���ް��o���׸�(0:�o��,1:���o��)

    // ��2007.10.4 �~���[�d�l�Ή�
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // ��2007.10.4 �~���[�d�l�Ή�

    bool __fastcall Nozzle_Data_Sakusei(void); // ɽ�ٖ{���ް� �쐬
    void __fastcall Tankan_Data_Sakusei(void); // �[���ް��쐬
    // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
    // bool __fastcall Get_Nozzle_Data(AnsiString Syurui,AnsiString Tugite);     // �eɽ���ް��擾
    bool __fastcall Get_Nozzle_Data(AnsiString Syurui, AnsiString Tugite,
        AnsiString Kokei, AnsiString Ondo); // �eɽ���ް��擾
    // ***********************************
    bool __fastcall Get_Tankan_Data(AnsiString Syurui); // �e�[���ް��擾
    bool __fastcall Get_LA_Nozzle_Data(AnsiString Syurui, AnsiString Tugite);
    // �e�t�����G�A����ɽ���ް��擾
    bool __fastcall Get_LA_Tugite_Data(AnsiString Syurui, AnsiString Tugite,
        AnsiString NzlTgl); // �e�t�����G�A�����p���ް��擾

    void __fastcall Get_Siyou_Data(AnsiString Syurui, int Kubun,
        AnsiString nMatName); // �d�l�ް��擾

    bool __fastcall Write_Nozzle_Data(void);
    AnsiString __fastcall Search_NOZZLE_CODE(AnsiString Code);
    // ɽ�ٍ\�����i����ð��ٌ���

    // �ڰ�ɽ�ٕ��i�\��Ͻ� ����
    bool __fastcall Search_HD_FRM_NZ(AnsiString Key, AnsiString Type, int Code);
    bool __fastcall TNozzleDetails::Search_HD_FRNG_DNO(int Model, int Typ,
        AnsiString Key, int flgAnsi);
    // AnsiString __fastcall TNozzleDetails::Search_HD_FRNG_MST(AnsiString Key, int FSize, AnsiString SEDiv, AnsiString PG, AnsiString TMP, AnsiString Drn, AnsiString Air);

    // 2019.04.11 �����O���i�ǉ�_S
    bool __fastcall Get_Ring_Data(AnsiString Dno, AnsiString Code,
        AnsiString Syurui);  // �e�ݸޕ��i�ް��擾
    // 2019.04.11 �����O���i�ǉ�_E

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // ɽ�ٕ��i�W�J���� ���s
    AnsiString __fastcall ChgAirDrnSize(double dSize);
    // 2008.06.26 ɽ�ٌ��a3/4�ǉ�
    AnsiString __fastcall ChgNozzleSize(double dSize);
    // **************************
};

// ---------------------------------------------------------------------------
// ���{��֐����F ɽ�ٕ��i�W�J����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall NozzleDetails(HeadsData* pcHeadsData) {
    bool ret;

    TNozzleDetails nozzleDetails; // ɽ���ް� �쐬Class��������,�֐��I����j��

    ret = nozzleDetails.Excute(pcHeadsData); // ɽ���ް� �쐬���s

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ɽ�ٕ��i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADS÷���ް����߲�����
    m_pcHeadsData = pcHeadsData;

    // HEADS÷���ް���AnsiString�^�z��Ɏ擾
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********      �wɽ�ٕ��i�W�J�����x�J�n      **********");
    Write_Log("********************************************************");
    Write_Error_Log("********************************************************");
    Write_Error_Log("**********      �wɽ�ٕ��i�W�J�����x�J�n      **********");
    Write_Error_Log("********************************************************");

    // ***********************************
    // ***  ɽ���ް��쐬
    // ***********************************
    Nozzle_Data_Sakusei();

    // ********************
    // ***  �[���ް��쐬
    // ********************
    Write_Log("********************************************");
    Write_Log("**********      �w�[�ǁx�J�n      **********");
    Write_Log("********************************************");
    Write_Error_Log("********************************************");
    Write_Error_Log("**********      �w�[�ǁx�J�n      **********");
    Write_Error_Log("********************************************");

    Tankan_Data_Sakusei();

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********      �wɽ�ٕ��i�W�J�����x�I��      **********");
    Write_Log("********************************************************");
    Write_Error_Log("********************************************************");
    Write_Error_Log("**********      �wɽ�ٕ��i�W�J�����x�I��      **********");
    Write_Error_Log("********************************************************");

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ɽ���ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Nozzle_Data_Sakusei(void) {
    AnsiString s_Nzl_Syurui; // ɽ�َ��
    AnsiString s_Nzl_Tugite; // ɽ�ٌp��
    // 2004.12.21 �ϐ��ǉ�
    AnsiString s_Text;
    // *******************
    // 2005.02.08 �ϐ��ǉ�
    AnsiString s_Nzl_Kokei; // ɽ�ٌ��a
    AnsiString s_Nzl_Ondo; // ɽ�ى��x�v
    // *******************

    // **************************************************************************
    // ***                                                                    ***
    // ***                                                                    ***
    // ***                        �m�Y���f�[�^�@�쐬                          ***
    // ***                                                                    ***
    // ***                                                                    ***
    // **************************************************************************

    // ***********************************
    // ***  S1ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    S1ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[893].TrimRight(); // S1ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[894].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S1ɽ���ް�  ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ɽ�َ��7(�~��ݼގ~��)�ǉ�
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        // if (Get_Nozzle_Data("S1",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[892].TrimRight(); // S1ɽ�ٌ��a
        s_Nzl_Ondo = P_HEADS_DATA[898].TrimRight(); // S1ɽ�ى��x�v
        if (Get_Nozzle_Data("S1", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S1ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  S2ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    S2ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[908].TrimRight(); // S2ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[909].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S2ɽ���ް�  ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ɽ�َ��7(�~��ݼގ~��)�ǉ�
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        // if (Get_Nozzle_Data("S2",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[907].TrimRight(); // S2ɽ�ٌ��a
        s_Nzl_Ondo = P_HEADS_DATA[913].TrimRight(); // S2ɽ�ى��x�v
        if (Get_Nozzle_Data("S2", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S2ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  S3ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    S3ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[923].TrimRight(); // S3ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[924].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S3ɽ���ް�  ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ɽ�َ��7(�~��ݼގ~��)�ǉ�
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        // if (Get_Nozzle_Data("S3",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[922].TrimRight(); // S3ɽ�ٌ��a
        s_Nzl_Ondo = P_HEADS_DATA[928].TrimRight(); // S3ɽ�ى��x�v
        if (Get_Nozzle_Data("S3", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S3ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  S4ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    S4ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[938].TrimRight(); // S4ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[939].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S4ɽ���ް�  ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ɽ�َ��7(�~��ݼގ~��)�ǉ�
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        // if (Get_Nozzle_Data("S4",s_Nzl_Tugite))
        s_Nzl_Kokei = P_HEADS_DATA[937].TrimRight(); // S4ɽ�ٌ��a
        s_Nzl_Ondo = P_HEADS_DATA[943].TrimRight(); // S4ɽ�ى��x�v
        if (Get_Nozzle_Data("S4", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S4ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E1ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    E1ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[953].TrimRight(); // E1ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[954].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E1ɽ���ް�  ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ɽ�َ��7(�~��ݼގ~��)�ǉ�
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        // if (Get_Nozzle_Data("E1",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[952].TrimRight(); // E1ɽ�ٌ��a
        s_Nzl_Ondo = P_HEADS_DATA[958].TrimRight(); // E1ɽ�ى��x�v
        if (Get_Nozzle_Data("E1", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            // 2020.03.05 YX-80Eɽ�ٕύX_S
            //// 2004.12.21 YX-80Eɽ�ْǉ�
            //if (s_Nzl_Syurui == "1") {
            //    s_Text = P_HEADS_DATA[34].TrimRight();
            //    if (s_Text.SubString(1, 2) == "YX") {
            //        if (P_HEADS_DATA[170].ToIntDef(0) < 150) {
            //            P_SZuban = "�d�R�S�S�X�R�U�Q�P";
            //        }
            //        else {
            //            P_SZuban = "�d�R�S�S�X�R�U�Q�Q";
            //        }
            //    }
            //}
            //// *************************
            P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
            if (P_Model == "YX-80") {
                if (P_HEADS_DATA[170].ToIntDef(0) < 150) {
                    P_SZuban = "E344936_1";
                }
                else {
                    P_SZuban = "E344936_2";
                }
            }
            if (P_Model == "YX-83") {
                if (P_HEADS_DATA[1290].TrimRight() == "�`�|") {
                    P_SZuban = "E385512_1";
                }
                else {
                    P_SZuban = "E385512_2";
                }
            }
            // 2020.03.05 YX-80Eɽ�ٕύX_E
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E1ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E2ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    E2ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[968].TrimRight(); // E2ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[969].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E2ɽ���ް�  ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ɽ�َ��7(�~��ݼގ~��)�ǉ�
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        // if (Get_Nozzle_Data("E2",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[967].TrimRight(); // E2ɽ�ٌ��a
        s_Nzl_Ondo = P_HEADS_DATA[973].TrimRight(); // E2ɽ�ى��x�v
        if (Get_Nozzle_Data("E2", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E2ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E3ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    E3ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[983].TrimRight(); // E3ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[984].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E3ɽ���ް�  ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ɽ�َ��7(�~��ݼގ~��)�ǉ�
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2015.03.20 OGI �m�Y����ޏ���"8"�̒ǉ�
    //else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
    //    (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
    //    || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || (s_Nzl_Syurui == "8")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        // if (Get_Nozzle_Data("E3",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[982].TrimRight(); // E3ɽ�ٌ��a
        s_Nzl_Ondo = P_HEADS_DATA[988].TrimRight(); // E3ɽ�ى��x�v
        if (Get_Nozzle_Data("E3", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E3ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E4ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    E4ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[998].TrimRight(); // E4ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[999].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E4ɽ���ް�  ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ɽ�َ��7(�~��ݼގ~��)�ǉ�
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2015.03.20 OGI �m�Y����ޏ���"8"�̒ǉ�
    //else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
    //  (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
    //  || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || (s_Nzl_Syurui == "8")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        // if (Get_Nozzle_Data("E4",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[997].TrimRight(); // E4ɽ�ٌ��a
        s_Nzl_Ondo = P_HEADS_DATA[1003].TrimRight(); // E4ɽ�ى��x�v
        if (Get_Nozzle_Data("E4", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "-----    E4ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // **************************************************************************
    // ***                                                                    ***
    // ***                                                                    ***
    // ***                 �t�����G�A�����m�Y���f�[�^�@�쐬                   ***
    // ***                                                                    ***
    // ***                                                                    ***
    // **************************************************************************

    // 2008.03.21 ����ɽ�ْǉ�
    // ***************************************
    // ***  S1�G�A����ɽ���ް��@�쐬
    // ***************************************
    G_Log_Text = "------    S1�t�����G�A����ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[893].TrimRight(); // S1ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[894].TrimRight();
    P_BV = P_HEADS_DATA[1216].TrimRight() + P_HEADS_DATA[1217].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S1ɽ���ް�  �t�����G�A����ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("S1", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S1�t�����G�A����ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***************************************
    // ***  S2�G�A����ɽ���ް��@�쐬
    // ***************************************
    G_Log_Text = "------    S2�t�����G�A����ɽ���ް�  �쐬�J�n    -------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[908].TrimRight(); // S2ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[909].TrimRight();
    P_BV = P_HEADS_DATA[1220].TrimRight() + P_HEADS_DATA[1221].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S2ɽ���ް�  �t�����G�A����ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("S2", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S2�t�����G�A����ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);
    // ***********************

    // ***************************************
    // ***  S3�t����ɽ���ް��@�쐬
    // ***************************************
    G_Log_Text = "------    S3�t�����G�A����ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[923].TrimRight(); // S3ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[924].TrimRight();
    P_BV = P_HEADS_DATA[1224].TrimRight() + P_HEADS_DATA[1225].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S3ɽ���ް�  �t�����G�A����ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("S3", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S3�t�����G�A����ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***************************************
    // ***  S4�t����ɽ���ް��@�쐬
    // ***************************************
    G_Log_Text = "------    S4�t�����G�A����ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[938].TrimRight(); // S4ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[939].TrimRight();
    P_BV = P_HEADS_DATA[1228].TrimRight() + P_HEADS_DATA[1229].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S4ɽ���ް�  �t�����G�A����ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("S4", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S4�t�����G�A����ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // 2008.03.21 ����ɽ�ْǉ�
    // ***************************************
    // ***  E1�G�A����ɽ���ް��@�쐬
    // ***************************************
    G_Log_Text = "------    E1�t�����G�A����ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[953].TrimRight(); // E1ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[954].TrimRight();
    P_BV = P_HEADS_DATA[1232].TrimRight() + P_HEADS_DATA[1233].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E1ɽ���ް�  �t�����G�A����ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("E1", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    G_Log_Text = "------    E1�t�����G�A����ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***************************************
    // ***  E2�G�A����ɽ���ް��@�쐬
    // ***************************************
    G_Log_Text = "------    E2�t�����G�A����ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[968].TrimRight(); // E2ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[969].TrimRight();
    P_BV = P_HEADS_DATA[1236].TrimRight() + P_HEADS_DATA[1237].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E2ɽ���ް�  �t�����G�A����ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("E2", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E2�t�����G�A����ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);
    // ***********************

    // ***********************************
    // ***  E3�t����ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    E3�t�����G�A����ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[983].TrimRight(); // E3ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[984].TrimRight();
    P_BV = P_HEADS_DATA[1240].TrimRight() + P_HEADS_DATA[1241].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E3ɽ���ް�  �t�����G�A����ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("E3", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E3�t�����G�A����ɽ���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E4�t����ɽ���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    E4�t�����G�A����ɽ���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[998].TrimRight(); // E4ɽ�ٌp��
    P_ZaisituCD = P_HEADS_DATA[999].TrimRight();
    P_BV = P_HEADS_DATA[1244].TrimRight() + P_HEADS_DATA[1245].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E4ɽ���ް�  �t�����G�A����ɽ�َ�ނ��0��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ɽ�ًK�i�ύX
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("E4", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ɽ�َ�ނ��" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E4�t�����G�A����ɽ���ް�  �쐬�I��    ------*";
    Write_Log(G_Log_Text);

    // **************************************************************************
    // ***                                                                    ***
    // ***                                                                    ***
    // ***              S1�`E4�t�����G�A�����p��f�[�^�@�쐬                  ***
    // ***                                                                    ***
    // ***                                                                    ***
    // **************************************************************************

    // //***********************************
    // //***  S1�������p���ް��@�쐬
    // //***********************************
    // G_Log_Text = "------    S1�������p���ް�  �쐬�J�n    ------";
    // Write_Log(G_Log_Text);
    //
    // s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight();   // S1ɽ�َ��
    // s_Nzl_Tugite = P_HEADS_DATA[905].TrimRight();   // S1�������p��
    // P_ZaisituCD  = P_HEADS_DATA[894].TrimRight();
    //
    // if(( P_HEADS_DATA[36].Pos("M") > 0 ) && (P_HEADS_DATA[1136].ToIntDef(0) != 0) )  {
    // P_ZaisituCD = Chg_Zaisitu_Code( P_ZaisituCD, 6);
    // }
    //
    // P_BV         = P_HEADS_DATA[1216].TrimRight() + P_HEADS_DATA[1217].TrimRight();
    //
    // if( s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2" ){
    // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
    // if (Get_LA_Tugite_Data("S1",s_Nzl_Tugite, P_HEADS_DATA[893].TrimRight())){
    // Write_Nozzle_Data();
    // }
    // } else {
    // G_Log_Text = "S1�������p�肪�A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // } else {
    // G_Log_Text = "S1ɽ�َ�ނ��A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    //
    // G_Log_Text = "------    S1�������p���ް�  �쐬�I��    ------";
    // Write_Log(G_Log_Text);
    //
    // //***********************************
    // //***  S2�������p���ް��@�쐬
    // //***********************************
    // G_Log_Text = "------    S2�������p���ް�  �쐬�J�n    ------";
    // Write_Log(G_Log_Text);
    //
    // s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight();   // S2ɽ�َ��
    // s_Nzl_Tugite = P_HEADS_DATA[920].TrimRight();   // S2�������p��
    // P_ZaisituCD  = P_HEADS_DATA[909].TrimRight();
    //
    // if(( P_HEADS_DATA[36].Pos("M") > 0 ) && (P_HEADS_DATA[1137].ToIntDef(0) != 0) )  {
    // P_ZaisituCD = Chg_Zaisitu_Code( P_ZaisituCD, 6);
    // }
    //
    // P_BV         = P_HEADS_DATA[1220].TrimRight() + P_HEADS_DATA[1221].TrimRight();
    //
    // if( s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2" ){
    // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
    // if (Get_LA_Tugite_Data("S2",s_Nzl_Tugite, P_HEADS_DATA[908].TrimRight())){
    // Write_Nozzle_Data();
    // }
    // } else {
    // G_Log_Text = "S2�������p�肪�A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // } else {
    // G_Log_Text = "S2ɽ�َ�ނ��A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    //
    // G_Log_Text = "------    S2�������p���ް�  �쐬�I��    ------";
    // Write_Log(G_Log_Text);

    // ***********************************
    // ***  S3�t�����p���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    S3�t�����p���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[935].TrimRight(); // S3�t�����p��
    P_ZaisituCD = P_HEADS_DATA[924].TrimRight();

    if ((P_HEADS_DATA[36].Pos("M") > 0) && (P_HEADS_DATA[1138].ToIntDef(0)
        != 0)) {
        P_ZaisituCD = Chg_Zaisitu_Code(P_ZaisituCD, 6);
    }

    P_BV = P_HEADS_DATA[1224].TrimRight() + P_HEADS_DATA[1225].TrimRight();

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2") {
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
        if (s_Nzl_Tugite != "0000" && s_Nzl_Tugite != "9999") {
            // ***********************
            if (Get_LA_Tugite_Data("S3", s_Nzl_Tugite,
                P_HEADS_DATA[923].TrimRight())) {
                Write_Nozzle_Data();
            }
        }
        else {
            G_Log_Text = "S3�t�����p�肪�A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
    }
    else {
        G_Log_Text = "S3ɽ�َ�ނ��A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S3�t�����p���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  S4�t�����p���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    S4�t�����p���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[950].TrimRight(); // S4�t�����p��
    P_ZaisituCD = P_HEADS_DATA[939].TrimRight();

    if ((P_HEADS_DATA[36].Pos("M") > 0) && (P_HEADS_DATA[1139].ToIntDef(0)
        != 0)) {
        P_ZaisituCD = Chg_Zaisitu_Code(P_ZaisituCD, 6);
    }

    P_BV = P_HEADS_DATA[1228].TrimRight() + P_HEADS_DATA[1229].TrimRight();

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2") {
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
        if (s_Nzl_Tugite != "0000" && s_Nzl_Tugite != "9999") {
            // ***********************
            if (Get_LA_Tugite_Data("S4", s_Nzl_Tugite,
                P_HEADS_DATA[938].TrimRight())) {
                Write_Nozzle_Data();
            }
        }
        else {
            G_Log_Text = "S4�t�����p�肪�A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
    }
    else {
        G_Log_Text = "S4ɽ�َ�ނ��A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S4�t�����p���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // //***********************************
    // //***  E1�������p���ް��@�쐬
    // //***********************************
    // G_Log_Text = "------    E1�������p���ް�  �쐬�J�n    ------";
    // Write_Log(G_Log_Text);
    //
    // s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight();   // E1ɽ�َ��
    // s_Nzl_Tugite = P_HEADS_DATA[965].TrimRight();   // E1�������p��
    // P_ZaisituCD  = P_HEADS_DATA[954].TrimRight();
    // P_BV         = P_HEADS_DATA[1232].TrimRight() + P_HEADS_DATA[1233].TrimRight();
    //
    // if( s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2" ){
    // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
    // if (Get_LA_Tugite_Data("E1",s_Nzl_Tugite,P_HEADS_DATA[953].TrimRight())){
    // Write_Nozzle_Data();
    // }
    // } else {
    // G_Log_Text = "E1�������p�肪�A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // } else {
    // G_Log_Text = "E1ɽ�َ�ނ��A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    //
    // G_Log_Text = "------    E1�������p���ް�  �쐬�I��    ------";
    // Write_Log(G_Log_Text);
    //
    // //***********************************
    // //***  E2�������p���ް��@�쐬
    // //***********************************
    // G_Log_Text = "------    E2�������p���ް�  �쐬�J�n    ------";
    // Write_Log(G_Log_Text);
    //
    // s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight();   // E2ɽ�َ��
    // s_Nzl_Tugite = P_HEADS_DATA[980].TrimRight();   // E2�������p��
    // P_ZaisituCD  = P_HEADS_DATA[969].TrimRight();
    // P_BV         = P_HEADS_DATA[1236].TrimRight() + P_HEADS_DATA[1237].TrimRight();
    //
    // if( s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2" ){
    // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
    // if (Get_LA_Tugite_Data("E2",s_Nzl_Tugite,P_HEADS_DATA[968].TrimRight())){
    // Write_Nozzle_Data();
    // }
    // } else {
    // G_Log_Text = "E2�������p�肪�A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    // } else {
    // G_Log_Text = "E2ɽ�َ�ނ��A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }
    //
    // G_Log_Text = "------    E2�������p���ް�  �쐬�I��    ------";
    // Write_Log(G_Log_Text);

    // ***********************************
    // ***  E3�t�����p���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    E3�t�����p���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[995].TrimRight(); // E3�t�����p��
    P_ZaisituCD = P_HEADS_DATA[984].TrimRight();
    P_BV = P_HEADS_DATA[1240].TrimRight() + P_HEADS_DATA[1241].TrimRight();

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2") {
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
        if (s_Nzl_Tugite != "0000" && s_Nzl_Tugite != "9999") {
            // ***********************
            if (Get_LA_Tugite_Data("E3", s_Nzl_Tugite,
                P_HEADS_DATA[983].TrimRight())) {
                Write_Nozzle_Data();
            }
        }
        else {
            G_Log_Text = "E3�t�����p�肪�A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
    }
    else {
        G_Log_Text = "E3ɽ�َ�ނ��A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E3�t�����p���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E4�t�����p���ް��@�쐬
    // ***********************************
    G_Log_Text = "------    E4�t�����p���ް�  �쐬�J�n    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ɽ�َ��
    s_Nzl_Tugite = P_HEADS_DATA[1010].TrimRight(); // E4�t�����p��
    P_ZaisituCD = P_HEADS_DATA[999].TrimRight();
    P_BV = P_HEADS_DATA[1244].TrimRight() + P_HEADS_DATA[1245].TrimRight();

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2") {
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
        if (s_Nzl_Tugite != "0000" && s_Nzl_Tugite != "9999") {
            // ***********************
            if (Get_LA_Tugite_Data("E4", s_Nzl_Tugite,
                P_HEADS_DATA[998].TrimRight())) {
                Write_Nozzle_Data();
            }
        }
        else {
            G_Log_Text = "E4�t�����p�肪�A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
    }
    else {
        G_Log_Text = "E4ɽ�َ�ނ��A�" + s_Nzl_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E4�t�����p���ް�  �쐬�I��    ------";
    Write_Log(G_Log_Text);

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �t�����G�A�����p��f�[�^�@�쐬����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 02.08.05�ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Get_LA_Tugite_Data(AnsiString Syurui,
    AnsiString Tugite, AnsiString NzlTgl) {
    AnsiString s_Text;

    AnsiString s_Code; //
    AnsiString s_BuhinKbn; // ���i�敪(Nozzle.csv�������擾)
    AnsiString s_BootsFlg; // �ް��׸�
    AnsiString s_FCode; // ��ݼ޺���
    // 2012.07.12 ɽ�َd�l�ύX
    AnsiString s_SPEC1; // SPEC1
    AnsiString s_SPEC2; // SPEC2
    // ***********************

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;

    // ************************************
    // ***  ɽ�ٍ\�����i����ð�� ٌ���
    // ************************************
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( NzlTgl.SubString(1,1) == "6" && NzlTgl.SubString(3,1) == "7" ) ||
    // ( NzlTgl.SubString(1,1) == "6" && NzlTgl.SubString(3,1) == "8" ) ) {    // �T�j�^��
    // if ( Tugite.SubString(1,1) == "7" && Tugite.SubString(3,1) == "9" ) {             // �t���ǂo�s
    if ((NzlTgl.SubString(1, 2) == "06" && NzlTgl.SubString(4, 1) == "7") ||
        (NzlTgl.SubString(1, 2) == "06" && NzlTgl.SubString(4, 1) == "8"))
    { // �T�j�^��
        if (Tugite.SubString(1, 2) == "07" && Tugite.SubString(4, 1) == "9")
        { // �t���ǂo�s
            // ***********************
            if (Syurui == "S3" || Syurui == "S4") {
                s_Code = "46";
            }
            else if (Syurui == "E3" || Syurui == "E4") {
                s_Code = "47";
            }
        }
    }
    else { // �T�j�^���ȊO
        // 2011.06.20 ɽ�ًK�i�ύX
        // if ( Tugite.SubString(1,1) == "7" && Tugite.SubString(3,1) == "9" ) {             // �t���Ȋǂo�s
        if (Tugite.SubString(1, 2) == "07" && Tugite.SubString(4, 1) == "9")
        { // �t���Ȋǂo�s
            // ***********************
            if (Syurui == "S3" || Syurui == "S4") {
                s_Code = "30";
            }
            else if (Syurui == "E3" || Syurui == "E4") {
                s_Code = "31";
            }
            // 2011.06.20 ɽ�ًK�i�ύX
            // } else if( Tugite.SubString(3,1) == "1" || Tugite.SubString(3,1) == "2" ) {       // �t���Ȋǂv�e
        }
        else if (Tugite.SubString(4, 1) == "1" || Tugite.SubString(4, 1) == "2")
        { // �t���Ȋǂv�e
            // ***********************
            if (Syurui == "S3" || Syurui == "S4") {
                s_Code = "44";
            }
            else if (Syurui == "E3" || Syurui == "E4") {
                s_Code = "45";
            }
        }
        else { // �t���Ȋǃt���[�e
            if (Syurui == "S3" || Syurui == "S4") {
                s_Code = "42";
            }
            else if (Syurui == "E3" || Syurui == "E4") {
                s_Code = "43";
            }
        }
    }
    if (s_Code == "") {
        G_ErrLog_Text = Syurui + "ɽ���ް��쐬���A�����Ɉ�v���܂���ł����B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }

    s_BuhinKbn = Search_NOZZLE_CODE(s_Code);

    // *********************************************
    // ***  �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ) ����
    // *********************************************
    // ��2002/10/24
    // s_Text = P_HEADS_DATA[34].TrimRight() + "-"
    // + P_HEADS_DATA[35].SubString(1,2) + "-"
    // + P_HEADS_DATA[1267].TrimRight();

    int i_Mojisu;
    i_Mojisu = P_HEADS_DATA[35].TrimRight().Length();
    s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu, 1);
    if (s_Text.ToIntDef(999) != 999) {
        s_Text = P_HEADS_DATA[34].TrimRight() + "-" + P_HEADS_DATA[35].TrimRight
            ().SubString(1, i_Mojisu - 2) + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Text = P_HEADS_DATA[34].TrimRight() + "-" + P_HEADS_DATA[35].TrimRight
            ().SubString(1, i_Mojisu - 1) + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    P_Model = s_Text;
    // ��2002/10/24
    s_Text = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "�ڰ�ɽ�ٕ��i�\��Ͻ��������� �" + s_Text + "� �" +
        P_HEADS_DATA[36].TrimRight() + "� �" + s_BuhinKbn + "� �" +
        AnsiString(s_Code.ToIntDef(0)) + "� �Ō������܂��B";

    Write_Log(G_Log_Text);

    if (!Search_HD_FRM_NZ(s_Text, s_BuhinKbn, s_Code.ToIntDef(0))) {
        G_ErrLog_Text = Syurui + "�ް��쐬���@������ �" + s_Text + "� �" +
            P_HEADS_DATA[36].TrimRight() + "� �" + s_BuhinKbn + "� �" +
            AnsiString(s_Code.ToIntDef(0)) +
            "� �́A�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�ɑ��݂��܂���B";

        Write_Log(G_ErrLog_Text);
        Write_Error_Log(G_ErrLog_Text);

        return false;
    }

    // *********************************
    // ***  �ގ� �擾(02.10.26�ǉ�)
    // *********************************
    P_Zaisitu = Search_HD_MATERIAL(P_ZaisituCD);
    P_EZaisitu = Search_HD_E_MATERIAL(P_ZaisituCD);

    // 2016.11.07 �ގ����ޕύX
    //// 2003.12.25 ASME PLATE
    //// if ((P_SUS != 0 ) && (P_Zaisitu.SubString(1,2) != "TP" )) {  // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]�j
    //if ((P_SUS != 0) && (P_ZaisituCD != "1014") && (P_ZaisituCD != "1015") &&
    //    (P_ZaisituCD != "1032") && (P_ZaisituCD != "1033") &&
    //    (P_ZaisituCD != "1057") && (P_ZaisituCD != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
    //    (P_ZaisituCD != "1075") && (P_ZaisituCD != "1089") &&
    //    (P_ZaisituCD != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11�ǉ�
    //    (P_ZaisituCD != "1076") &&
    //    // ***************************
    //    (P_ZaisituCD != "1085") && (P_ZaisituCD != "1095")) {
    //    // �m�Y���ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
    //    // *********************
    if ( (P_SUS != 0) && (Type_MAT(P_ZaisituCD) != "Ti") ) {  // �v���[�g�ގ����`�^���n�ȊO
    // ***********************
        // �}��
        // 2005.02.08 JPI�ǉ�
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_Zuban = P_Zuban4;
        // 2011.06.20 ɽ�ًK�i�ύX
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_Zuban = P_Zuban4;
        // 2013.04.03 ɽ�ًK�i�ǉ�
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_Zuban = P_Zuban4;
        // ***********************
        else
            P_Zuban = P_Zuban3;

        // ��}�p�}��
        // 2005.02.08 JPI�ǉ�
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_SZuban = P_SZuban4;
        // 2011.06.20 ɽ�ًK�i�ύX
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // *****************
                P_SZuban = P_SZuban4;
        // 2013.04.03 ɽ�ًK�i�ǉ�
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_SZuban = P_SZuban4;
        // ***********************
        else
            P_SZuban = P_SZuban3;
    }
    else {
        // �}��
        // 2005.02.08 JPI�ǉ�
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_Zuban = P_Zuban2;
        // 2011.06.20 ɽ�ًK�i�ύX
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_Zuban = P_Zuban2;
        // 2013.04.03 ɽ�ًK�i�ǉ�
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_Zuban = P_Zuban2;
        // ***********************
        else
            P_Zuban = P_Zuban1;

        // ��}�p�}��
        // 2005.02.08 JPI�ǉ�
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_SZuban = P_SZuban2;
        // 2011.06.20 ɽ�ًK�i�ύX
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_SZuban = P_SZuban2;
        // 2013.04.03 ɽ�ًK�i�ǉ�
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_SZuban = P_SZuban2;
        // ***********************
        else
            P_SZuban = P_SZuban1;
    }

    // **************************************
    // ***  �\���i�� �擾(02.08.05�ǉ�)
    // **************************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, P_ZaisituCD, m_pcHeadsData);

    // **********************************
    // ***  �d�l�P�@�擾
    // **********************************
    // �ް��׸ނ̎擾
    // 2011.06.20 ɽ�ًK�i�ύX
    // if (Tugite.SubString(2,1) == "1" || Tugite.SubString(2,1) == "2" || Tugite.SubString(2,1) == "3"){
    if (Tugite.SubString(3, 1) == "1" || Tugite.SubString(3, 1) == "2" ||
        Tugite.SubString(3, 1) == "3") {
        // ***********************
        s_BootsFlg = "1";
    }
    else {
        s_BootsFlg = "0";
    }

    // ��ݼ޺��ނ̎擾
    // 2011.06.20 ɽ�ًK�i�ύX
    // s_FCode = Tugite.SubString(1,1) + "0" + Tugite.SubString(3,1);
    if (Tugite.ToIntDef(0) >= 1000) {
        s_FCode = Tugite.SubString(1, 2) + "0" + Tugite.SubString(4, 1);
    }
    else {
        s_FCode = Tugite.SubString(2, 1) + "0" + Tugite.SubString(4, 1);
    }
    // ***********************

    // ��ݼގd�l�ϊ��\  ����
    G_Log_Text = "��ݼގd�l�ϊ��\���A�" + s_FCode + "��Ƣ" + s_BootsFlg + "��Ō����B";
    Write_Log(G_Log_Text);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_FRNG_SPEC";
    s_SQL = s_SQL + "  WHERE FCODE = '" + s_FCode + "'";
    s_SQL = s_SQL + "    AND BOOTS = " + s_BootsFlg + "";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

//cccc �m�Y���d�l�̎擾��
    if (wkQuery->Eof) {
        P_Siyou1 = "";
    }
    else {
        // 2012.07.12 ɽ�َd�l�ύX
        // P_Siyou1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight() + "-";
        P_Siyou1 = "";
        s_SPEC1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight(); // SPEC1
        s_SPEC2 = wkQuery->FieldByName("SPEC2")->AsString.TrimRight(); // SPEC2
        // ***********************

        if (Syurui == "S1") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[895].TrimRight(), 0.0));
        }
        else if (Syurui == "S2") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[910].TrimRight(), 0.0));
        }
        else if (Syurui == "S3") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[925].TrimRight(), 0.0));
        }
        else if (Syurui == "S4") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[940].TrimRight(), 0.0));
        }
        else if (Syurui == "E1") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[955].TrimRight(), 0.0));
        }
        else if (Syurui == "E2") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[970].TrimRight(), 0.0));
        }
        else if (Syurui == "E3") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[985].TrimRight(), 0.0));
        }
        else if (Syurui == "E4") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[1000].TrimRight(), 0.0));
        }
        // if (Syurui == "S1"){
        // s_Text = atoi(P_HEADS_DATA[905].c_str());
        // } else if (Syurui == "S2"){
        // s_Text = atoi(P_HEADS_DATA[920].c_str());
        // } else if (Syurui == "S3"){
        // s_Text = atoi(P_HEADS_DATA[935].c_str());
        // } else if (Syurui == "S4"){
        // s_Text = atoi(P_HEADS_DATA[950].c_str());
        // } else if (Syurui == "E1"){
        // s_Text = atoi(P_HEADS_DATA[965].c_str());
        // } else if (Syurui == "E2"){
        // s_Text = atoi(P_HEADS_DATA[980].c_str());
        // } else if (Syurui == "E3"){
        // s_Text = atoi(P_HEADS_DATA[995].c_str());
        // } else if (Syurui == "E4"){
        // s_Text = atoi(P_HEADS_DATA[1010].c_str());
        // }

        // 2012.07.12 ɽ�َd�l�ύX
        // P_Siyou1 = P_Siyou1 + s_Text;
        // P_Siyou1 = P_Siyou1 + " " + wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
        if (s_SPEC1.SubString(1, 2) == "GB") {
            P_Siyou1 = "GB DN" + s_Text + "-" + s_SPEC1.SubString(4, 4) + " " +
                s_SPEC2;
        }
        else if (s_SPEC1.SubString(1, 3) == "DIN") {
            P_Siyou1 = "DIN DN" + s_Text + "-" + s_SPEC1.SubString(5, 4) + " " +
                s_SPEC2;
        }
        else {
            P_Siyou1 = s_SPEC1 + "-" + s_Text + " " + s_SPEC2;
        }
        // ***********************

        G_Log_Text = "�d�l�P    �" + P_Siyou1 + "����擾�B";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "��ݼގd�l�ϊ��\���A�����I���B";
    Write_Log(G_Log_Text);

    // **********************
    // ***  �d�l�Q�@�擾
    // **********************
    if (P_HEADS_DATA[36].Pos("B") != 0) { // �a�V���[�Y
        // 2016.11.07 �ގ����ޕύX
        //if (ZenToHan(P_Zaisitu).SubString(1, 2) == "TP") { // �`�^���̏ꍇ�͎d��T
        //    P_Siyou2 = "�d��T";
        //    // 2004.01.28 ASME
        //}
        //else if (ZenToHan(P_Zaisitu).Pos("B-265") > 0) { // �`�^���̏ꍇ�͎d��T
        //    P_Siyou2 = "�d��T";
        //    // ***************
        //}
        //else { // �`�^���ȊO�̏ꍇ�͎d��P
        //    P_Siyou2 = "�d��U";
        //}
        if ( Type_MAT(P_ZaisituCD) == "Ti" ) {        // �`�^���̏ꍇ�͎d��T
            P_Siyou2 = "�d��T";
        }
        else {                         // �`�^���ȊO�̏ꍇ�͎d��U
            P_Siyou2 = "�d��U";
        }
        // ***********************
    }
    else {
        if (Syurui == "S1") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[904].TrimRight());
        }
        else if (Syurui == "S2") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[919].TrimRight());
        }
        else if (Syurui == "S3") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[934].TrimRight());
        }
        else if (Syurui == "S4") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[949].TrimRight());
        }
        else if (Syurui == "E1") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[964].TrimRight());
        }
        else if (Syurui == "E2") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[979].TrimRight());
        }
        else if (Syurui == "E3") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[994].TrimRight());
        }
        else if (Syurui == "E4") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[1009].TrimRight());
        }
    }

    // **********************
    // ***  �d�l�R�@�擾
    // **********************
    if (Syurui == "S1") {
        P_Siyou3 = "N1";
    }
    else if (Syurui == "S2") {
        P_Siyou3 = "N2";
    }
    else if (Syurui == "S3") {
        P_Siyou3 = "N3";
    }
    else if (Syurui == "S4") {
        P_Siyou3 = "N4";
    }
    else if (Syurui == "E1") {
        P_Siyou3 = "N1";
    }
    else if (Syurui == "E2") {
        P_Siyou3 = "N2";
    }
    else if (Syurui == "E3") {
        P_Siyou3 = "N3";
    }
    else if (Syurui == "E4") {
        P_Siyou3 = "N4";
    }

    P_Katasiki = P_Katasiki + "  " + P_Siyou1;
    P_Siyou1 = P_BV;

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �t�����G�A����ɽ�ٕ��i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 02.08.05�ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Get_LA_Nozzle_Data(AnsiString Syurui,
    AnsiString Tugite) {
    AnsiString s_Text;

    AnsiString s_Code; //
    AnsiString s_BuhinKbn; // ���i�敪(Nozzle.csv�������擾)

    // 2011.06.20 ɽ�ًK�i�ύX
    // s_Text = Tugite.SubString(2,1);
    s_Text = Tugite.SubString(3, 1);
    // ***********************

    // ************************************
    // ***  ɽ�ٍ\�����i����ð�� ٌ���
    // ************************************

    // 2008.03.21 ����ɽ�� �t��ɽ���ْٰǉ�
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( Tugite.SubString(1,1) == "7" && Tugite.SubString(3,1) == "9" ) {        // �t���m�Y���@�o�s
    if (Tugite.SubString(1, 2) == "07" && Tugite.SubString(4, 1) == "9")
    { // �t���m�Y���@�o�s
        // ***********************
        // if (Syurui == "S3" || Syurui == "S4"){
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4") {
            s_Code = "27";
            // } else if (Syurui == "E3" || Syurui == "E4"){
        }
        else if (Syurui == "E1" || Syurui == "E2" || Syurui == "E3" ||
            Syurui == "E4") {
            s_Code = "11";
        }
        // 2011.06.20 ɽ�ًK�i�ύX
        // } else if ( Tugite.SubString(3,1) == "1" || Tugite.SubString(3,1) == "2") {  // �t���m�Y���@�v�e
    }
    else if (Tugite.SubString(4, 1) == "1" || Tugite.SubString(4, 1) == "2")
    { // �t���m�Y���@�v�e
        // ***********************
        // if (Syurui == "S3" || Syurui == "S4"){
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4") {
            s_Code = "40";
            // } else if (Syurui == "E3" || Syurui == "E4"){
        }
        else if (Syurui == "E1" || Syurui == "E2" || Syurui == "E3" ||
            Syurui == "E4") {
            s_Code = "41";
        }
        // 2011.06.20 ɽ�ًK�i�ύX
        // } else if ( Tugite.SubString(3,1) == "6" || Tugite.SubString(3,1) == "8") {  // �t���m�Y���@�w���[��
    }
    else if (Tugite.SubString(1, 2) == "06" && Tugite.SubString(4, 1) == "8")
    { // �t���m�Y���@�w���[��
        // ***********************
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4") {
            s_Code = "72";
        }
        else if (Syurui == "E1" || Syurui == "E2" || Syurui == "E3" ||
            Syurui == "E4") {
            s_Code = "73";
        }
    }
    else {
        // if (Syurui == "S3" || Syurui == "S4"){                                     // �t���m�Y���@�t���[�e
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4") { // �t���m�Y���@�t���[�e
            s_Code = "34";
            // } else if (Syurui == "E3" || Syurui == "E4"){
        }
        else if (Syurui == "E1" || Syurui == "E2" || Syurui == "E3" ||
            Syurui == "E4") {
            s_Code = "37";
        }
    }
    // ************************************
    if (s_Code == "") {
        G_ErrLog_Text = Syurui + "ɽ���ް��쐬���A�����Ɉ�v���܂���ł����B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }

    s_BuhinKbn = Search_NOZZLE_CODE(s_Code);

    // *********************************************
    // ***  �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ) ����
    // *********************************************
    s_Text = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "�ڰ�ɽ�ٕ��i�\��Ͻ��������� �" + s_Text + "� �" +
        P_HEADS_DATA[36].TrimRight() + "� �" + s_BuhinKbn + "� �" +
        AnsiString(s_Code.ToIntDef(0)) + "� �Ō������܂��B";

    Write_Log(G_Log_Text);

    if (!Search_HD_FRM_NZ(s_Text, s_BuhinKbn, s_Code.ToIntDef(0))) {
        G_ErrLog_Text = Syurui + "�ް��쐬���@������ �" + s_Text + "� �" +
            P_HEADS_DATA[36].TrimRight() + "� �" + s_BuhinKbn + "� �" +
            AnsiString(s_Code.ToIntDef(0)) +
            "� �́A�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�ɑ��݂��܂���B";

        Write_Error_Log(G_ErrLog_Text);

        return false;
    }
    // P_Katasiki = P_Katasiki + "  " + P_BV;

    // *********************************
    // ***  �ގ� �擾(02.10.26�ǉ�)
    // *********************************
    P_Zaisitu = Search_HD_MATERIAL(P_ZaisituCD);
    P_EZaisitu = Search_HD_E_MATERIAL(P_ZaisituCD);

    // 2016.11.07 �ގ����ޕύX
    //// 2003.12.25 ASME PLATE
    //// if ((P_SUS != 0 ) && (P_Zaisitu.SubString(1,2) != "TP" )) {  // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]�j
    //if ((P_SUS != 0) && (P_ZaisituCD != "1014") && (P_ZaisituCD != "1015") &&
    //    (P_ZaisituCD != "1032") && (P_ZaisituCD != "1033") &&
    //    (P_ZaisituCD != "1057") && (P_ZaisituCD != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
    //    (P_ZaisituCD != "1075") && (P_ZaisituCD != "1089") &&
    //    (P_ZaisituCD != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11�ǉ�
    //    (P_ZaisituCD != "1076") &&
    //    // ***************************
    //    (P_ZaisituCD != "1085") && (P_ZaisituCD != "1095")) {
    //    // �m�Y���ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
    //    // *********************
    if ( (P_SUS != 0) && (Type_MAT(P_ZaisituCD) != "Ti") ) {  // �v���[�g�ގ����`�^���n�ȊO
    // ***********************
        // �}��
        // 2005.02.08 JPI�ǉ�
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_Zuban = P_Zuban4;
        // 2011.06.20 ɽ�ًK�i�ύX
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_Zuban = P_Zuban4;
        // 2013.04.03 ɽ�ًK�i�ǉ�
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_Zuban = P_Zuban4;
        // ***********************
        else
            P_Zuban = P_Zuban3;

        // ��}�p�}��
        // 2005.02.08 JPI�ǉ�
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_SZuban = P_SZuban4;
        // 2011.06.20 ɽ�ًK�i�ύX
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_SZuban = P_SZuban4;
        // 2013.04.03 ɽ�ًK�i�ǉ�
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_SZuban = P_SZuban4;
        // ***********************
        else
            P_SZuban = P_SZuban3;
    }
    else {
        // �}��
        // 2005.02.08 JPI�ǉ�
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_Zuban = P_Zuban2;
        // 2011.06.20 ɽ�ًK�i�ύX
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_Zuban = P_Zuban2;
        // 2013.04.03 ɽ�ًK�i�ǉ�
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_Zuban = P_Zuban2;
        // ***********************
        else
            P_Zuban = P_Zuban1;

        // ��}�p�}��
        // 2005.02.08 JPI�ǉ�
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_SZuban = P_SZuban2;
        // 2011.06.20 ɽ�ًK�i�ύX
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_SZuban = P_SZuban2;
        // 2013.04.03 ɽ�ًK�i�ǉ�
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_SZuban = P_SZuban2;
        // ***********************
        else
            P_SZuban = P_SZuban1;
    }

    // *********************************
    // ***  �\���i�� �擾(02.08.05�ǉ�)
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, P_ZaisituCD, m_pcHeadsData);

    Get_Siyou_Data(Syurui, 2, P_Zaisitu);

    P_Katasiki = P_Katasiki + "  " + P_Siyou1;
    P_Siyou1 = P_BV;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ɽ�ٕ��i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F Syurui:ɽ�َ��(S1�`E4)�CTugite:ɽ�ٌp��(S1�`E4)
//                Kokei:ɽ�ٌ��a(S1�`E4), Ondo:���x�v(S1�`E4)
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
// 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
// bool __fastcall TNozzleDetails::Get_Nozzle_Data(AnsiString Syurui,AnsiString Tugite)
// {
bool __fastcall TNozzleDetails::Get_Nozzle_Data(AnsiString Syurui,
    AnsiString Tugite, AnsiString Kokei, AnsiString Ondo) {
    // ***********************************

    AnsiString s_Text;

    AnsiString s_Code; //
    AnsiString s_BuhinKbn; // ���i�敪(Nozzle.csv�������擾)

    // 2011.06.20 ɽ�ًK�i�ύX
    // s_Text = Tugite.SubString(2,1);
    s_Text = Tugite.SubString(3, 1);
    // ***********************
    s_Code = "";

    // ************************************
    // ***  ɽ�ٍ\�����i����ð�� ٌ���
    // ************************************
    if (s_Text == "1") { // ���o�[�u�[�c
        if (Syurui.SubString(1, 1) == "S") {
            s_Code = "02";
        }
        else {
            s_Code = "28";
        }
    }
    else if (s_Text == "2") { // �`���[�u�t�����W
        if (Syurui.SubString(1, 1) == "S") {
            s_Code = "03";
        }
        else {
            s_Code = "29";
        }
    }
    else if (s_Text == "3") { // ���^���u�[�c
        // 2004.04.16 RX-00�m�Y���p�{�X�ǉ�
        // if (Syurui.SubString(1,1) == "S"){
        // s_Code = "05";
        // } else {
        // s_Code = "12";
        // }
        P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
        if (P_Model == "RX-00") {
            if (Syurui.SubString(1, 1) == "S") {
                s_Code = "50";
            }
            else {
                s_Code = "51";
            }
        }
        else {
            if (Syurui.SubString(1, 1) == "S") {
                s_Code = "05";
            }
            else {
                s_Code = "12";
            }
        }
        // ********************************
    }
    else {
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (Tugite.SubString(1,1) == "6"){     //�T�j�^��
        // if (Tugite.SubString(3,1) == "7"){               //�l�W
        // if (Syurui.SubString(1,1) == "S"){
        // s_Code = "32";
        // } else {
        // s_Code = "35";
        // }
        // } else if (Tugite.SubString(3,1) == "8"){        //�w���[���i�N�����v�j
        if (Tugite.SubString(1, 2) == "06") { // �T�j�^��
            if (Tugite.SubString(4, 1) == "7") { // �l�W
                if (Syurui.SubString(1, 1) == "S") {
                    s_Code = "32";
                }
                else {
                    s_Code = "35";
                }
            }
            else if (Tugite.SubString(4, 1) == "8") { // �w���[���i�N�����v�j
                // ***********************
                if (Syurui.SubString(1, 1) == "S") {
                    s_Code = "33";
                }
                else {
                    s_Code = "36";
                }
            }
            // 2008.03.21 RX-00 ȼލ���ɽ�ْǉ�
            // 2011.06.20 ɽ�ًK�i�ύX
            // } else if (Tugite.SubString(1,1) == "7"){     //�o�s�l�W���݃m�Y��
        }
        else if (Tugite.SubString(1, 2) == "07") { // �o�s�l�W���݃m�Y��
            // *******************************
            if (Syurui.SubString(1, 1) == "S") {
                s_Code = "70";
            }
            else {
                s_Code = "71";
            }
            // ********************************
        }
        else { // �m�Y���i�t�����W�t���j
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( (Tugite.SubString(3,1) == "1") || (Tugite.SubString(3,1) == "2") ){               //�v�e�i�n�ځj
            if ((Tugite.SubString(4, 1) == "1") || (Tugite.SubString(4,
                1) == "2")) { // �v�e�i�n�ځj
                // ***********************
                if (Syurui.SubString(1, 1) == "S") {
                    s_Code = "38";
                }
                else {
                    s_Code = "39";
                }
            }
            else {
                if (Syurui.SubString(1, 1) == "S") {
                    s_Code = "07";
                }
                else {
                    s_Code = "10";
                }
            }
        }
    }
    if (s_Code == "") {
        G_ErrLog_Text = Syurui + "ɽ���ް��쐬���A�����Ɉ�v���܂���ł����B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }

    // �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�����p�̕��i�敪�̎擾
    s_BuhinKbn = Search_NOZZLE_CODE(s_Code);

    // *********************************************
    // ***  �ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ) ����
    // *********************************************
    s_Text = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "�ڰ�ɽ�ٕ��i�\��Ͻ��������� �" + s_Text + "� �" +
        P_HEADS_DATA[36].TrimRight() + "� �" + s_BuhinKbn + "� �" +
        AnsiString(s_Code.ToIntDef(0)) + "� �Ō������܂��B";

    Write_Log(G_Log_Text);

    if (!Search_HD_FRM_NZ(s_Text, s_BuhinKbn, s_Code.ToIntDef(0))) {
        G_ErrLog_Text = Syurui + "�ް��쐬���@������ �" + s_Text + "� �" +
            P_HEADS_DATA[36].TrimRight() + "� �" + s_BuhinKbn + "� �" +
            AnsiString(s_Code.ToIntDef(0)) +
            "� �́A�ڰ�ɽ�ٕ��i�\��Ͻ�(HD_FRM_NZ)�ɑ��݂��܂���B";

        Write_Error_Log(G_ErrLog_Text);

        // return false;
    }

    // *********************************
    // ***  �ގ� �擾(02.10.26�ǉ�)
    // *********************************
    // 2012.02.07 SUS316L�������ݼ�
    if (s_Code == "03" || s_Code == "29") {
        P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
        // 2012.08.23 LX-00�ȊO��SUS316L
        // if ( P_Model == "UX-40" ||
        // P_Model == "LX-40" ||
        // P_Model == "LX-50" ||
        // P_Model == "SX-41" ||
        // P_Model == "SX-43" ||
        // P_Model == "SX-70" ||
        // P_Model == "SX-73" ||
        // P_Model == "RX-50" ||
        // P_Model == "RX-70" ) {
        // // ���a200A�ȏ��SUSD316����ނ�SUSD316L�ɕύX
        // 2012.12.06 LX-00�폜
        // if ( P_Model != "LX-00" ) {
        // // LX-00�ȊO��SUSD316����ނ�SUSD316L�ɕύX
        //// *****************************
        // 2016.11.07 �ގ����ޕύX
        //if (P_ZaisituCD == "1003") {
        //    P_ZaisituCD = "1004";
        //}
        if ((P_ZaisituCD == "1003")||(P_ZaisituCD == "1004")||(P_ZaisituCD == "3103")) {
            P_ZaisituCD = "3104";
        }
        if (P_ZaisituCD == "1014") {
            P_ZaisituCD = "3114";
        }
        // ***********************
        // }
        // ********************
    }
    // *****************************
    P_Zaisitu = Search_HD_MATERIAL(P_ZaisituCD);
    P_EZaisitu = Search_HD_E_MATERIAL(P_ZaisituCD);

    // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
    ////2003.12.25 ASME PLATE
    ////if ((P_SUS != 0 ) && (P_Zaisitu.SubString(1,2) != "TP" )) {  // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]�j
    // if ((P_SUS != 0 ) && ( P_ZaisituCD != "1014" ) && ( P_ZaisituCD != "1015" ) &&
    // ( P_ZaisituCD != "1032" ) && ( P_ZaisituCD != "1033" ) &&
    // ( P_ZaisituCD != "1057" ) && ( P_ZaisituCD != "1058" ) &&
    // ( P_ZaisituCD != "1085" ) && ( P_ZaisituCD != "1095" )) {
    // // �m�Y���ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
    ////*********************
    // // �}��
    // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
    // P_Zuban = P_Zuban4;
    // else
    // P_Zuban = P_Zuban3;
    //
    // // ��}�p�}��
    // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
    // P_SZuban = P_SZuban4;
    // else
    // P_SZuban = P_SZuban3;
    // } else {
    // // �}��
    // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
    // P_Zuban = P_Zuban2;
    // // 02.10.16 ���ĉ�
    // //else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
    // //   P_Zuban = P_Zuban2;
    // else
    // P_Zuban = P_Zuban1;
    //
    // // ��}�p�}��
    // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
    // P_SZuban = P_SZuban2;
    // // 02.10.16 ���ĉ�
    // //else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
    // //   P_SZuban = P_SZuban2;
    // else
    // P_SZuban = P_SZuban1;
    // }
    // 2013.04.03 ɽ�ًK�i�ǉ�
    //// 2011.06.20 ɽ�ًK�i�ύX
    ////ANSI,JPI
    ////if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
    ////    ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" )) {
    ////ANSI/JPI/DIN/GB
    // if (( Tugite.SubString(1,2) == "04" ) || ( Tugite.SubString(1,2) == "05" ) ||
    // ( Tugite.SubString(1,2) == "08" ) || ( Tugite.SubString(1,2) == "09" ) ||
    // ( Tugite.SubString(1,2) == "11" ) || ( Tugite.SubString(1,2) == "12" ) ||
    // ( Tugite.SubString(1,2) == "13" ) || ( Tugite.SubString(1,2) == "21" ) ||
    // ( Tugite.SubString(1,2) == "22" ) || ( Tugite.SubString(1,2) == "23" )) {
    if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05") ||
        (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1, 2) == "09") ||
        (Tugite.SubString(1, 2) == "11") || (Tugite.SubString(1, 2) == "12") ||
        (Tugite.SubString(1, 2) == "13") || (Tugite.SubString(1, 2) == "14") ||
        (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1, 2) == "22") ||
        (Tugite.SubString(1, 2) == "23") || (Tugite.SubString(1, 2) == "24") ||
        (Tugite.SubString(1, 2) == "31") || (Tugite.SubString(1, 2) == "32") ||
        (Tugite.SubString(1, 2) == "33") || (Tugite.SubString(1, 2) == "34") ||
        (Tugite.SubString(1, 2) == "44") || (Tugite.SubString(1, 2) == "48")) {
        //// ***********************
        // ***********************
        if (P_SUS == 0) {
            P_Zuban = P_Zuban2; // �}��
            P_SZuban = P_SZuban2; // ��}�p�}��

            // 2008.08.21 WX-50�ް}�Ԑݒ�����ǉ�
            P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
            if (P_Model == "WX-53") {
                // WHC=0,REVE=0 �a���n�ځA�`���K�X�P�b�g
                if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban2; // ANSI�n�ڐ}��
                        P_SZuban = P_SZuban2; // ANSI�n�ڍ�}�p�}��
                    }
                    else {
                        P_Zuban = P_Zuban4; // ANSI�޽��Đ}��
                        P_SZuban = P_SZuban4; // ANSI�޽��č�}�p�}��
                    }
                    // WHC=0,REVE=1 �a���K�X�P�b�g�A�`���n��
                }
                else if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) != 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban4; // ANSI�޽��Đ}��
                        P_SZuban = P_SZuban4; // ANSI�޽��č�}�p�}��
                    }
                    else {
                        P_Zuban = P_Zuban2; // ANSI�n�ڐ}��
                        P_SZuban = P_SZuban2; // ANSI�n�ڍ�}�p�}��
                    }
                    // WHC=1,REVE=0 �a���K�X�P�b�g�A�`���n��
                }
                else if ((P_HEADS_DATA[243].ToIntDef(0) != 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban4; // ANSI�޽��Đ}��
                        P_SZuban = P_SZuban4; // ANSI�޽��č�}�p�}��
                    }
                    else {
                        P_Zuban = P_Zuban2; // ANSI�n�ڐ}��
                        P_SZuban = P_SZuban2; // ANSI�n�ڍ�}�p�}��
                    }
                    // WHC=1,REVE=1 �a���n�ځA�`���K�X�P�b�g
                }
                else {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban2; // ANSI�n�ڐ}��
                        P_SZuban = P_SZuban2; // ANSI�n�ڍ�}�p�}��
                    }
                    else {
                        P_Zuban = P_Zuban4; // ANSI�޽��Đ}��
                        P_SZuban = P_SZuban4; // ANSI�޽��č�}�p�}��
                    }
                }
            }
            // ************************************
            // 2011.06.20 ɽ�ًK�i�ύX
            else if ((P_Model == "LX-10") && ((Tugite.SubString(1,
                2) == "11") || (Tugite.SubString(1, 2) == "21"))) {
                P_Zuban = P_Zuban1; // GB PN1.0 DIN10�}��
                P_SZuban = P_SZuban1; // GB PN1.0 DIN10��}�p�}��
            }
            // ************************
            // 2013.04.03 ɽ�ًK�i�ǉ�
            else if ((P_Model == "LX-10") && (Tugite.SubString(1, 2) == "31")) {
                P_Zuban = P_Zuban1; // EN10�}��
                P_SZuban = P_SZuban1; // EN10��}�p�}��
            }
            // ************************
            // 2019.11.06 CX-23�ǉ�_S
            else if ((P_Model == "CX-23") &&
                     ((Tugite.SubString(1, 2) == "11") ||
                      (Tugite.SubString(1, 2) == "12") ||
                      (Tugite.SubString(1, 2) == "21") ||
                      (Tugite.SubString(1, 2) == "22") ||
                      (Tugite.SubString(1, 2) == "31") ||
                      (Tugite.SubString(1, 2) == "32"))) {
                P_Zuban = P_Zuban1;   // GB PN10,16 DIN PN10,16 EN PN10,16 �}��
                P_SZuban = P_SZuban1; // GB PN10,16 DIN PN10,16 EN PN10,16 ��}�p�}��
            }
            // 2019.11.06 CX-23�ǉ�_E
            // 2022.04.27 CX-21�ǉ�_S
            else if ((P_Model == "CX-21") &&
                     ((Tugite.SubString(1, 2) == "11") ||
                      (Tugite.SubString(1, 2) == "12") ||
                      (Tugite.SubString(1, 2) == "21") ||
                      (Tugite.SubString(1, 2) == "22") ||
                      (Tugite.SubString(1, 2) == "31") ||
                      (Tugite.SubString(1, 2) == "32"))) {
                P_Zuban = P_Zuban1;   // GB PN10,16 DIN PN10,16 EN PN10,16 �}��
                P_SZuban = P_SZuban1; // GB PN10,16 DIN PN10,16 EN PN10,16 ��}�p�}��
            }
            // 2022.04.27 CX-21�ǉ�_E
        }
        else if ((P_SUS == 25) && (Kokei.ToDouble() == 1) &&
            (Ondo.ToIntDef(0) != 0)) {
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
        }
        else if ((P_SUS == 50) && (Kokei.ToDouble() == 2)) {
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
        }
        else if ((P_SUS == 100) && (Kokei.ToDouble() == 4)) {
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
        }
        else if ((P_SUS == 150) && (Kokei.ToDouble() == 6)) {
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
            //2020.03.05 YX-83�C��_S
            if ((P_Model == "YX-83") && (Type_MAT(P_ZaisituCD) == "Ti")) {
                P_Zuban = P_Zuban4; // �}��
                P_SZuban = P_SZuban4; // ��}�p�}��
            }
            //2020.03.05 YX-83�C��_E
        }
        else if ((P_SUS == 200) && (Kokei.ToDouble() == 8)) {
            //2020.03.05 YX-83�C��_S
            //P_Zuban = P_Zuban6; // �}��
            //P_SZuban = P_SZuban6; // ��}�p�}��
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
            //2020.03.05 YX-83�C��_E
        }
        else {
            // 2016.11.07 �ގ����ޕύX
            //if ((P_ZaisituCD != "1014") && (P_ZaisituCD != "1015") &&
            //    (P_ZaisituCD != "1032") && (P_ZaisituCD != "1033") &&
            //    (P_ZaisituCD != "1057") && (P_ZaisituCD != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
            //    (P_ZaisituCD != "1075") && (P_ZaisituCD != "1089") &&
            //    (P_ZaisituCD != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11�ǉ�
            //    (P_ZaisituCD != "1076") &&
            //    // ***************************
            //    (P_ZaisituCD != "1085") && (P_ZaisituCD != "1095")) {
            //    // �m�Y���ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032]
            //    // TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
            if (Type_MAT(P_ZaisituCD) != "Ti") {  // �m�Y���ގ����`�^���n�ȊO
            // ***********************
                P_Zuban = P_Zuban4; // �}��
                P_SZuban = P_SZuban4; // ��}�p�}��
            }
            else {
                P_Zuban = P_Zuban2; // �}��
                P_SZuban = P_SZuban2; // ��}�p�}��
            }
        }
        // JIS��
    }
    else {
        if (P_SUS == 0) {
            P_Zuban = P_Zuban1; // �}��
            P_SZuban = P_SZuban1; // ��}�p�}��

            // 2008.08.21 WX-50�ް}�Ԑݒ�����ǉ�
            P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
            if (P_Model == "WX-53") {
                // WHC=0,REVE=0 �a���n�ځA�`���K�X�P�b�g
                if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban1; // JIS�n�ڐ}��
                        P_SZuban = P_SZuban1; // JIS�n�ڍ�}�p�}��
                    }
                    else {
                        P_Zuban = P_Zuban3; // JIS�޽��Đ}��
                        P_SZuban = P_SZuban3; // JIS�޽��č�}�p�}��
                    }
                    // WHC=0,REVE=1 �a���K�X�P�b�g�A�`���n��
                }
                else if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) != 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban3; // JIS�޽��Đ}��
                        P_SZuban = P_SZuban3; // JIS�޽��č�}�p�}��
                    }
                    else {
                        P_Zuban = P_Zuban1; // JIS�n�ڐ}��
                        P_SZuban = P_SZuban1; // JIS�n�ڍ�}�p�}��
                    }
                    // WHC=1,REVE=0 �a���K�X�P�b�g�A�`���n��
                }
                else if ((P_HEADS_DATA[243].ToIntDef(0) != 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban3; // JIS�޽��Đ}��
                        P_SZuban = P_SZuban3; // JIS�޽��č�}�p�}��
                    }
                    else {
                        P_Zuban = P_Zuban1; // JIS�n�ڐ}��
                        P_SZuban = P_SZuban1; // JIS�n�ڍ�}�p�}��
                    }
                    // WHC=1,REVE=1 �a���n�ځA�`���K�X�P�b�g
                }
                else {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban1; // JIS�n�ڐ}��
                        P_SZuban = P_SZuban1; // JIS�n�ڍ�}�p�}��
                    }
                    else {
                        P_Zuban = P_Zuban3; // JIS�޽��Đ}��
                        P_SZuban = P_SZuban3; // JIS�޽��č�}�p�}��
                    }
                }
            }
            // ************************************
            // 2010.08.27 LX-10-NP,NHP JIS16K,20K�ǉ�
            // 2011.06.20 ɽ�ًK�i�ύX
            // else if ( ( P_Model == "LX-10" ) &&
            // ( ( Tugite.SubString(1,1) == "2" ) || ( Tugite.SubString(1,1) == "3" ) ) ) {
            else if ((P_Model == "LX-10") && ((Tugite.SubString(1,
                2) == "02") || (Tugite.SubString(1, 2) == "03"))) {
                // ***********************
                P_Zuban = P_Zuban2; // �}��
                P_SZuban = P_SZuban2; // ��}�p�}��
            }
            // **************************************
            // 2013.05.22 RX-00�޽�ύX
            else if ((s_Code == "50" || s_Code == "51") &&
                (P_ZaisituCD != "1047")) {
                P_Zuban = P_Zuban3; // �}��
                P_SZuban = P_SZuban3; // ��}�p�}��
            }
            // ************************
            // 2018.07.05 LX-30Z�ǉ�_S
            else if ((P_Model == "LX-30") && ( P_HEADS_DATA[36].Pos("Z") > 0) &&
                     ( Tugite.SubString(1, 2) == "02" )) {
                // �k�w�|�R�O�|�y  �i�h�r�P�U�j
                P_Zuban = P_Zuban3; // �}��
                P_SZuban = P_SZuban3; // ��}�p�}��
            }
            // 2018.06.19 LX-30Z�ǉ�_E
        }
        else if ((P_SUS == 25) && (Kokei.ToDouble() == 25) &&
            (Ondo.ToIntDef(0) != 0)) {
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
        }
        else if ((P_SUS == 50) && (Kokei.ToDouble() == 50)) {
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
        }
        else if ((P_SUS == 100) && (Kokei.ToDouble() == 100)) {
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
        }
        else if ((P_SUS == 150) && (Kokei.ToDouble() == 150)) {
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
            //2020.03.05 YX-83�C��_S
            if ((P_Model == "YX-83") && (Type_MAT(P_ZaisituCD) == "Ti")) {
                P_Zuban = P_Zuban4; // �}��
                P_SZuban = P_SZuban4; // ��}�p�}��
            }
            //2020.03.05 YX-83�C��_E
        }
        else if ((P_SUS == 200) && (Kokei.ToDouble() == 200)) {
            P_Zuban = P_Zuban5; // �}��
            P_SZuban = P_SZuban5; // ��}�p�}��
        }
        else {
            // 2016.11.07 �ގ����ޕύX
            //if ((P_ZaisituCD != "1014") && (P_ZaisituCD != "1015") &&
            //    (P_ZaisituCD != "1032") && (P_ZaisituCD != "1033") &&
            //    (P_ZaisituCD != "1057") && (P_ZaisituCD != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
            //    (P_ZaisituCD != "1075") && (P_ZaisituCD != "1089") &&
            //    (P_ZaisituCD != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11�ǉ�
            //    (P_ZaisituCD != "1076") &&
            //    // ***************************
            //    (P_ZaisituCD != "1085") && (P_ZaisituCD != "1095")) {
            //    // �m�Y���ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032]
            //    // TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
            if (Type_MAT(P_ZaisituCD) != "Ti") {  // �m�Y���ގ����`�^���n�ȊO
            // ***********************
                P_Zuban = P_Zuban3; // �}��
                P_SZuban = P_SZuban3; // ��}�p�}��
            }
            else {
                P_Zuban = P_Zuban1; // �}��
                P_SZuban = P_SZuban1; // ��}�p�}��
            }
        }
    }

    // 2009.02.16 RX-50�ް̏ꍇDIN,#150��JIS�Ƃ��Ĉ���
    P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
    if (P_Model == "RX-50") {
        // ANSI#300,JPI#300
        // 2011.06.20 ɽ�ًK�i�ύX
        // if (( Tugite.SubString(1,1) == "5" ) || ( Tugite.SubString(1,1) == "9" )) {
        if ((Tugite.SubString(1, 2) == "05") || (Tugite.SubString(1,
        2) == "09")) {
            // ***********************
            if (P_SUS == 0) {
                P_Zuban = P_Zuban2; // �}��
                P_SZuban = P_SZuban2; // ��}�p�}��
            }
            // 2013.04.03 ɽ�ًK�i�ǉ�
        }
        else if ((Tugite.SubString(1, 2) == "13") || (Tugite.SubString(1,
            2) == "14") || (Tugite.SubString(1, 2) == "23") ||
            (Tugite.SubString(1, 2) == "24") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "41") || (Tugite.SubString(1, 2) == "44") ||
            (Tugite.SubString(1, 2) == "48")) {
            if (P_SUS == 0) {
                P_Zuban = P_Zuban2; // �}��
                P_SZuban = P_SZuban2; // ��}�p�}��
            }
            // ************************
            // JIS,DIN,ANSI#150,JPI#150��
        }
        else {
            if (P_SUS == 0) {
                P_Zuban = P_Zuban1; // �}��
                P_SZuban = P_SZuban1; // ��}�p�}��
            }
        }
    }
    // *************************************************

    // 2023.03.06 CX-00-Y Pɽ�ْǉ�_S
    if (((P_Model == "CX-01") || (P_Model == "CX-03") || (P_Model == "CX-01D") ||
         (P_Model == "CX-03D") || (P_Model == "CXW-01") || (P_Model == "CXW-03" )) &&
        ((s_Code == "07") || (s_Code == "38")) &&
        (((P_HEADS_DATA[37].ToIntDef(0)) == 1) ||
         ((P_HEADS_DATA[37].ToIntDef(0)) == 2) ||
         ((P_HEADS_DATA[37].ToIntDef(0)) == 3) ||
         ((P_HEADS_DATA[37].ToIntDef(0)) == 4))) {
        if (Syurui == "S1" || Syurui == "S3" || Syurui == "E1" || Syurui == "E4") {
            if (Type_MAT(P_ZaisituCD) != "Ti") {  // �m�Y���ގ����`�^���n�ȊO
                P_Zuban = P_Zuban3; // �}��
                P_SZuban = P_SZuban3; // ��}�p�}��
            }
            else {
                P_Zuban = P_Zuban1; // �}��
                P_SZuban = P_SZuban1; // ��}�p�}��
            }
        }
        else {
            if (Type_MAT(P_ZaisituCD) != "Ti") {  // �m�Y���ގ����`�^���n�ȊO
                P_Zuban = P_Zuban4; // �}��
                P_SZuban = P_SZuban4; // ��}�p�}��
            }
            else {
                P_Zuban = P_Zuban2; // �}��
                P_SZuban = P_SZuban2; // ��}�p�}��
            }
        }
    }
    // 2023.03.06 CX-00-Y Pɽ�ْǉ�_E

    //2020.02.05 YX-83�C��_S
    G_Log_Text = "�}    ��     �" + P_Zuban + "����擾�B";
    //2020.02.05 YX-83�C��_E

    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // ***********************************

    // *********************************
    // ***  �\���i�� �擾(02.08.05�ǉ�)
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, P_ZaisituCD, m_pcHeadsData);

    // *********************************
    // ***  �d�l�P�E�Q�E�R�@�擾
    // *********************************
    Get_Siyou_Data(Syurui, 0, "");

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ɽ�ٍ\�����i����ð��ف@����
// �T  �v      �F
// ��  ��      �F AnsiString Code  // �����Ώۺ���
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall TNozzleDetails::Search_NOZZLE_CODE(AnsiString Code) {
    int i;
    AnsiString s_Name;

    s_Name = "";

    P_Hosoku = "";
    for (i = 0; i < G_NOZZLECD_RecCnt; i++) {
        if (G_NOZZLECD[i][0] == Code) {
            s_Name = G_NOZZLECD[i][2]; // ���i�敪

            P_Sikibetu = G_NOZZLECD[i][1]; // ���ʎq(CSV̧�ٍ쐬�p)
            P_Katasiki = G_NOZZLECD[i][3]; // ���i�^��(CSV̧�ٍ쐬�p)

            P_Yobihin = G_NOZZLECD[i][4]; // �\���i
            break;
        }
    }

    return s_Name;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �d�l1,2,3 �ް��擾����
// �T  �v      �F
// ��  ��      �F AnsiString Syurui �F���(S1�`S4, E1�`E4)
// �F int        Kubun  �F�敪(0:ɽ��, 1:�[��,2:�t����������ɽ��)
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
void __fastcall TNozzleDetails::Get_Siyou_Data(AnsiString Syurui, int Kubun,
    AnsiString nMatName) {
    AnsiString s_Text;
    AnsiString s_BootsFlg; // �ް��׸�
    AnsiString s_FCode; // ��ݼ޺���
    // 2012.07.12 ɽ�َd�l�ύX
    AnsiString s_SPEC1; // SPEC1
    AnsiString s_SPEC2; // SPEC2
    // ***********************

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;

    // **********************
    // ***  �d�l�P�@�擾
    // **********************
    if (Kubun == 1) {
        if (Syurui == "S1") {
            s_Text = P_HEADS_DATA[1186].TrimRight(); // S1�[�ǌp��p�P
        }
        else if (Syurui == "S2") {
            s_Text = P_HEADS_DATA[1190].TrimRight(); // S2�[�ǌp��p�P
        }
        else if (Syurui == "S3") {
            s_Text = P_HEADS_DATA[1194].TrimRight(); // S3�[�ǌp��p�P
        }
        else if (Syurui == "S4") {
            s_Text = P_HEADS_DATA[1198].TrimRight(); // S4�[�ǌp��p�P
        }
        else if (Syurui == "E1") {
            s_Text = P_HEADS_DATA[1202].TrimRight(); // E1�[�ǌp��p�P
        }
        else if (Syurui == "E2") {
            s_Text = P_HEADS_DATA[1206].TrimRight(); // E2�[�ǌp��p�P
        }
        else if (Syurui == "E3") {
            s_Text = P_HEADS_DATA[1210].TrimRight(); // E3�[�ǌp��p�P
        }
        else if (Syurui == "E4") {
            s_Text = P_HEADS_DATA[1214].TrimRight(); // E4�[�ǌp��p�P
        }

    }
    else {
        // �ް��׸ނ̎擾
        if (Syurui == "S1") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[893].TrimRight(); // S1ɽ�ٌp��
            }
            else {
                s_Text = P_HEADS_DATA[905].TrimRight(); // S1�����p��
            }
        }
        else if (Syurui == "S2") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[908].TrimRight(); // S2ɽ�ٌp��
            }
            else {
                s_Text = P_HEADS_DATA[920].TrimRight(); // S2�����p��
            }
        }
        else if (Syurui == "S3") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[923].TrimRight(); // S3ɽ�ٌp��
            }
            else {
                s_Text = P_HEADS_DATA[935].TrimRight(); // S3�t���p��
            }
        }
        else if (Syurui == "S4") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[938].TrimRight(); // S4ɽ�ٌp��
            }
            else {
                s_Text = P_HEADS_DATA[950].TrimRight(); // S4�t���p��
            }
        }
        else if (Syurui == "E1") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[953].TrimRight(); // E1ɽ�ٌp��
            }
            else {
                s_Text = P_HEADS_DATA[965].TrimRight(); // E1�����p��
            }
        }
        else if (Syurui == "E2") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[968].TrimRight(); // E2ɽ�ٌp��
            }
            else {
                s_Text = P_HEADS_DATA[980].TrimRight(); // E2�����p��
            }
        }
        else if (Syurui == "E3") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[983].TrimRight(); // E3ɽ�ٌp��
            }
            else {
                s_Text = P_HEADS_DATA[995].TrimRight(); // E3�t���p��
            }
        }
        else if (Syurui == "E4") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[998].TrimRight(); // E4ɽ�ٌp��
            }
            else {
                s_Text = P_HEADS_DATA[1010].TrimRight(); // E4�t���p��
            }
        }

        // 2011.06.20 ɽ�ًK�i�ύX
        // if (s_Text.SubString(2,1) == "1" || s_Text.SubString(2,1) == "2" || s_Text.SubString(2,1) == "3"){
        if (s_Text.SubString(3, 1) == "1" || s_Text.SubString(3, 1) == "2" ||
            s_Text.SubString(3, 1) == "3") {
            // ***********************
            s_BootsFlg = "1";
        }
        else {
            s_BootsFlg = "0";
        }

        // ��ݼ޺��ނ̎擾
        // �敪�Q(�t����������ɽ��)�쐬���͌p����Ď擾
        if (Kubun == 2) {
            if (Syurui == "S1") {
                s_Text = P_HEADS_DATA[893].TrimRight(); // S1ɽ�ٌp��
            }
            else if (Syurui == "S2") {
                s_Text = P_HEADS_DATA[908].TrimRight(); // S2ɽ�ٌp��
            }
            else if (Syurui == "S3") {
                s_Text = P_HEADS_DATA[923].TrimRight(); // S3ɽ�ٌp��
            }
            else if (Syurui == "S4") {
                s_Text = P_HEADS_DATA[938].TrimRight(); // S4ɽ�ٌp��
            }
            else if (Syurui == "E1") {
                s_Text = P_HEADS_DATA[953].TrimRight(); // E1ɽ�ٌp��
            }
            else if (Syurui == "E2") {
                s_Text = P_HEADS_DATA[968].TrimRight(); // E2ɽ�ٌp��
            }
            else if (Syurui == "E3") {
                s_Text = P_HEADS_DATA[983].TrimRight(); // E3ɽ�ٌp��
            }
            else if (Syurui == "E4") {
                s_Text = P_HEADS_DATA[998].TrimRight(); // E4ɽ�ٌp��
            }
        }
        // 2008.03.21 RX-00 ȼލ���ɽ�ْǉ�
        // 2011.06.20 ɽ�ًK�i�ύX
        // else if (s_Text.SubString(1,3) == "749"){     //�o�s�l�W���݃m�Y��
        else if (s_Text.SubString(1, 4) == "0749") { // �o�s�l�W���݃m�Y��
            // ***********************
            s_BootsFlg = "2";
        }
        // ********************************

        // 2011.06.20 ɽ�ًK�i�ύX
        // s_FCode = s_Text.SubString(1,1) + "0" + s_Text.SubString(3,1);
        if (s_Text.ToIntDef(0) >= 1000) {
            s_FCode = s_Text.SubString(1, 2) + "0" + s_Text.SubString(4, 1);
        }
        else {
            s_FCode = s_Text.SubString(2, 1) + "0" + s_Text.SubString(4, 1);
        }
        // ***********************

        // ��ݼގd�l�ϊ��\  ����
        G_Log_Text = "��ݼގd�l�ϊ��\���A�" + s_FCode + "��Ƣ" + s_BootsFlg + "��Ō����B";
        Write_Log(G_Log_Text);

        s_SQL = "";
        s_SQL = s_SQL + "SELECT * FROM HD_FRNG_SPEC";
        s_SQL = s_SQL + "  WHERE FCODE = '" + s_FCode + "'";
        s_SQL = s_SQL + "    AND BOOTS = " + s_BootsFlg + "";

        wkQuery->Close();
        // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
        //wkQuery->DatabaseName = ODBC_DSN_DNO;
        wkQuery->ConnectionName = ODBC_DSN_DNO;
        // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
        wkQuery->SQL->Clear();
        wkQuery->SQL->Add(s_SQL);
        wkQuery->Open();
        wkQuery->First();

        if (wkQuery->Eof) {
            P_Siyou1 = "";
        }
        else {
            // 2012.07.12 ɽ�َd�l�ύX
            // P_Siyou1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight() + "-";
            P_Siyou1 = "";
            s_SPEC1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight();
            // SPEC1
            s_SPEC2 = wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
            // SPEC2
            // ***********************

            if (Syurui == "S1") {
                if (Kubun == 0) {
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if (s_Text.SubString(1,1) == "6" ) {         // �T�j�^��
                    if (s_Text.SubString(1, 2) == "06") { // �T�j�^��
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[892].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ɽ�ٌ��a3/4�ǉ�
                        // s_Text = atoi(P_HEADS_DATA[892].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[892].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[892].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "S2") {
                if (Kubun == 0) {
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if (s_Text.SubString(1,1) == "6" ) {         // �T�j�^��
                    if (s_Text.SubString(1, 2) == "06") { // �T�j�^��
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[907].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ɽ�ٌ��a3/4�ǉ�
                        // s_Text = atoi(P_HEADS_DATA[907].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[907].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[907].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "S3") {
                if (Kubun == 0) {
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if (s_Text.SubString(1,1) == "6" ) {         // �T�j�^��
                    if (s_Text.SubString(1, 2) == "06") { // �T�j�^��
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[922].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ɽ�ٌ��a3/4�ǉ�
                        // s_Text = atoi(P_HEADS_DATA[922].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[922].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[922].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "S4") {
                if (Kubun == 0) {
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if (s_Text.SubString(1,1) == "6" ) {         // �T�j�^��
                    if (s_Text.SubString(1, 2) == "06") { // �T�j�^��
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[937].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ɽ�ٌ��a3/4�ǉ�
                        // s_Text = atoi(P_HEADS_DATA[937].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[937].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[937].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "E1") {
                if (Kubun == 0) {
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if (s_Text.SubString(1,1) == "6" ) {         // �T�j�^��
                    if (s_Text.SubString(1, 2) == "06") { // �T�j�^��
                        // ************************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[952].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ɽ�ٌ��a3/4�ǉ�
                        // s_Text = atoi(P_HEADS_DATA[952].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[952].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[952].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "E2") {
                if (Kubun == 0) {
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if (s_Text.SubString(1,1) == "6" ) {         // �T�j�^��
                    if (s_Text.SubString(1, 2) == "06") { // �T�j�^��
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[967].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ɽ�ٌ��a3/4�ǉ�
                        // s_Text = atoi(P_HEADS_DATA[967].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[967].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[967].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "E3") {
                if (Kubun == 0) {
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if (s_Text.SubString(1,1) == "6" ) {         // �T�j�^��
                    if (s_Text.SubString(1, 2) == "06") { // �T�j�^��
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[982].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ɽ�ٌ��a3/4�ǉ�
                        // s_Text = atoi(P_HEADS_DATA[982].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[982].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[982].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "E4") {
                if (Kubun == 0) {
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if (s_Text.SubString(1,1) == "6" ) {         // �T�j�^��
                    if (s_Text.SubString(1, 2) == "06") { // �T�j�^��
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[997].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ɽ�ٌ��a3/4�ǉ�
                        // s_Text = atoi(P_HEADS_DATA[997].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[997].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[997].TrimRight
                        (), 0.0));
                }
            }

            // 2012.07.12 ɽ�َd�l�ύX
            // P_Siyou1 = P_Siyou1 + s_Text;
            // P_Siyou1 = P_Siyou1 + " " + wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
            //
            ////2008.03.06 RX-21 ȼލ���ɽ�ْǉ�
            // if (s_BootsFlg == "2"){     //�o�s�l�W���݃m�Y��
            // P_Siyou1 = wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
            // }
            ////********************************
            if (s_SPEC1.SubString(1, 2) == "GB") {
                P_Siyou1 = "GB DN" + s_Text + "-" + s_SPEC1.SubString(4, 4) +
                    " " + s_SPEC2;
            }
            else if (s_SPEC1.SubString(1, 3) == "DIN") {
                P_Siyou1 = "DIN DN" + s_Text + "-" + s_SPEC1.SubString(5, 4) +
                    " " + s_SPEC2;
            }
            else if (s_BootsFlg == "2") { // �o�s�l�W���݃m�Y��
                P_Siyou1 = s_SPEC2;
            }
            else {
                P_Siyou1 = s_SPEC1 + "-" + s_Text + " " + s_SPEC2;
            }
            // ***********************

            G_Log_Text = "�d�l�P    �" + P_Siyou1 + "����擾�B";
            Write_Log(G_Log_Text);

        }

        G_Log_Text = "��ݼގd�l�ϊ��\���A�����I���B";
        Write_Log(G_Log_Text);
    }

    // **********************
    // ***  �d�l�Q�@�擾
    // **********************
    switch (Kubun) {
    case 0:
        if (Syurui == "S1") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[904].TrimRight());
        }
        else if (Syurui == "S2") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[919].TrimRight());
        }
        else if (Syurui == "S3") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[934].TrimRight());
        }
        else if (Syurui == "S4") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[949].TrimRight());
        }
        else if (Syurui == "E1") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[964].TrimRight());
        }
        else if (Syurui == "E2") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[979].TrimRight());
        }
        else if (Syurui == "E3") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[994].TrimRight());
        }
        else if (Syurui == "E4") {
            P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[1009].TrimRight());
        }
        break;

    case 2:
        if (P_HEADS_DATA[36].Pos("B") != 0) { // �a�V���[�Y
            if (ZenToHan(nMatName).SubString(1, 2) == "TP") { // �`�^���̏ꍇ�͎d��T
                P_Siyou2 = "�d��T";
                // 2004.01.28 ASME
            }
            else if (ZenToHan(nMatName).Pos("B-265") > 0) { // �`�^���̏ꍇ�͎d��T
                P_Siyou2 = "�d��T";
                // ***************
            }
            else { // �`�^���ȊO�̏ꍇ�͎d��P
                P_Siyou2 = "�d��U";
            }
        }
        else {
            if (Syurui == "S1") {
                P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[904].TrimRight());
            }
            else if (Syurui == "S2") {
                P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[919].TrimRight());
            }
            else if (Syurui == "S3") {
                P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[934].TrimRight());
            }
            else if (Syurui == "S4") {
                P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[949].TrimRight());
            }
            else if (Syurui == "E1") {
                P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[964].TrimRight());
            }
            else if (Syurui == "E2") {
                P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[979].TrimRight());
            }
            else if (Syurui == "E3") {
                P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[994].TrimRight());
            }
            else if (Syurui == "E4") {
                P_Siyou2 = "�d��" + HanToZen(P_HEADS_DATA[1009].TrimRight());
            }
        }
        break;
    case 1:
        if ((Syurui == "S1") || (Syurui == "S3") || (Syurui == "E1") ||
            (Syurui == "E3")) {
            // 2016.08.26 �ގ����ޕύX
            //s_Text = Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
            //s_Text = ZenToHan(s_Text);
            //if (s_Text.SubString(1, 2) == "TP" || s_Text.SubString(1,
            //    4) == "STPG") { // �`�^���̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //    // 2004.01.28 ASME SGP
            //}
            //else if (s_Text.Pos("B-265") > 0) { // �`�^���̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //}
            //else if (s_Text.Pos("SGP") > 0) { // �r�f�o�̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //    // *******************
            //    // 2004.05.13 ���p�[�Ǎގ��ǉ�
            //}
            //else if (s_Text.Pos("CAC703") > 0) { // �b�`�b�V�O�R�̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //}
            //else if (s_Text.Pos("STKM13A") > 0) { // �r�s�j�l�P�R�`�̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //    // ***************************
            //}
            //else { // �`�^���ȊO�̏ꍇ�͎d��P
            //    P_Siyou2 = "�d��P";
            //}
            s_Text = P_HEADS_DATA[1060].SubString(1,4);
            s_Text = Type_MAT(s_Text);
            if ( s_Text == "Ti" ) {        // �`�^���̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "STPG" ) { // �r�s�o�f�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "SGP" ) {  // �r�f�o�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "CS" ) {   // �b�r(�r�s�j�l�P�R�`)�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            //2021.03.10 NW2201�ǉ�_S
            else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            //2021.03.10 NW2201�ǉ�_E
            else {                         // ���̑��̏ꍇ�͎d��P
                 P_Siyou2 = "�d��P";
            }
            // ***********************
        }
        else if (((Syurui == "S4") || (Syurui == "E4")) &&
            (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c")) {
            // 2016.08.26 �ގ����ޕύX
            //s_Text = Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
            //s_Text = ZenToHan(s_Text);
            //if (s_Text.SubString(1, 2) == "TP" || s_Text.SubString(1,
            //    4) == "STPG") { // �`�^���̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //}
            //else if (s_Text.Pos("B-265") > 0) { // �`�^���̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //}
            //else if (s_Text.Pos("SGP") > 0) { // �r�f�o�̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //}
            //else if (s_Text.Pos("CAC703") > 0) { // �b�`�b�V�O�R�̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //}
            //else if (s_Text.Pos("STKM13A") > 0) { // �r�s�j�l�P�R�`�̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //}
            //else { // �`�^���ȊO�̏ꍇ�͎d��P
            //    P_Siyou2 = "�d��P";
            //}
            //// *********************
            s_Text = P_HEADS_DATA[1060].SubString(1,4);
            s_Text = Type_MAT(s_Text);
            if ( s_Text == "Ti" ) {        // �`�^���̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "STPG" ) { // �r�s�o�f�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "SGP" ) {  // �r�f�o�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "CS" ) {   // �b�r(�r�s�j�l�P�R�`)�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            //2021.03.10 NW2201�ǉ�_S
            else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            //2021.03.10 NW2201�ǉ�_E
            else {                         // ���̑��̏ꍇ�͎d��P
                 P_Siyou2 = "�d��P";
            }
            // ***********************
        }
        else if ((Syurui == "S2") || (Syurui == "S4") || (Syurui == "E2") ||
            (Syurui == "E4")) {
            // 2016.08.26 �ގ����ޕύX
            //s_Text = Search_HD_MATERIAL(P_HEADS_DATA[1061].SubString(1, 4));
            //s_Text = ZenToHan(s_Text);
            //if (s_Text.SubString(1, 2) == "TP" || s_Text.SubString(1,
            //    4) == "STPG") { // �`�^���̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //    // 2004.01.28 ASME SGP
            //}
            //else if (s_Text.Pos("B-265") > 0) { // �`�^���̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //}
            //else if (s_Text.Pos("SGP") > 0) { // �r�f�o�̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //    // *******************
            //    // 2004.05.13 ���p�[�Ǎގ��ǉ�
            //}
            //else if (s_Text.Pos("CAC703") > 0) { // �b�`�b�V�O�R�̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //}
            //else if (s_Text.Pos("STKM13A") > 0) { // �r�s�j�l�P�R�`�̏ꍇ�͎d��T
            //    P_Siyou2 = "�d��T";
            //    // ***************************
            //}
            //else { // �`�^���ȊO�̏ꍇ�͎d��P
            //    P_Siyou2 = "�d��P";
            //}
            s_Text = P_HEADS_DATA[1061].SubString(1,4);
            s_Text = Type_MAT(s_Text);
            if ( s_Text == "Ti" ) {        // �`�^���̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "STPG" ) { // �r�s�o�f�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "SGP" ) {  // �r�f�o�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            else if ( s_Text == "CS" ) {   // �b�r(�r�s�j�l�P�R�`)�̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            //2021.03.10 NW2201�ǉ�_S
            else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
                 P_Siyou2 = "�d��T";
            }
            //2021.03.10 NW2201�ǉ�_E
            else {                         // ���̑��̏ꍇ�͎d��P
                 P_Siyou2 = "�d��P";
            }
            // ***********************
        }
        break;
    }

    // **********************
    // ***  �d�l�R�@�擾
    // **********************
    if (Syurui == "S1") {
        P_Siyou3 = "N1";
    }
    else if (Syurui == "S2") {
        P_Siyou3 = "N2";
    }
    else if (Syurui == "S3") {
        P_Siyou3 = "N3";
    }
    else if (Syurui == "S4") {
        P_Siyou3 = "N4";
    }
    else if (Syurui == "E1") {
        P_Siyou3 = "N1";
    }
    else if (Syurui == "E2") {
        P_Siyou3 = "N2";
    }
    else if (Syurui == "E3") {
        P_Siyou3 = "N3";
    }
    else if (Syurui == "E4") {
        P_Siyou3 = "N4";
    }

    delete wkQuery;
}

// ---------------------------------------------------------------------------
// ���{��֐����F HD_FRM_NZð��ٌ�������
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Search_HD_FRM_NZ(AnsiString Key,
    AnsiString Type, int Code) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    int i_Length;
    AnsiString s_SQL;
    AnsiString s_Frame;

    s_Frame = P_HEADS_DATA[36].TrimRight(); // �ڰ�
    i_Length = s_Frame.Length();
    s_Frame = s_Frame.SubString(i_Length, 1);
    if (s_Frame == "R") {
        s_Frame = P_HEADS_DATA[36].TrimRight();
        s_Frame = s_Frame.SetLength(i_Length - 1);
    }
    else {
        s_Frame = P_HEADS_DATA[36].TrimRight();
    }

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_FRM_NZ";
    s_SQL = s_SQL + "  WHERE PLATE_MODEL = '" + Key + "'";
    s_SQL = s_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'";
    s_SQL = s_SQL + "    AND PARTS_TYPE = '" + Type + "'";
    s_SQL = s_SQL + "    AND PARTS_CODE = " + Code + "";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        P_Zaisitu = "";
        P_Zuban1 = "";
        P_Zuban2 = "";
        P_Zuban3 = "";
        P_Zuban4 = "";
        P_SZuban1 = "";
        P_SZuban2 = "";
        P_SZuban3 = "";
        P_SZuban4 = "";
        // SUS�g�p�t���O
        P_SUS = 0;
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        P_Zuban5 = "";
        P_SZuban5 = "";
        // ***********************************

        delete wkQuery;
        return false;
    }
    else {
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME1")->AsString.TrimRight
            (); // �ގ�����
        P_Zuban1 = wkQuery->FieldByName("DNO1")->AsString.TrimRight(); // �}��
        P_Zuban2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight(); // �}��
        P_Zuban3 = wkQuery->FieldByName("DNO3")->AsString.TrimRight(); // �}��
        P_Zuban4 = wkQuery->FieldByName("DNO4")->AsString.TrimRight(); // �}��
        P_SZuban1 = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        // ��}�p�}��
        P_SZuban2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        P_SZuban3 = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        // ��}�p�}��
        P_SZuban4 = wkQuery->FieldByName("WKDNO4")->AsString.TrimRight();
        // SUS�g�p�t���O
        P_SUS = wkQuery->FieldByName("LENGTH")->AsString.ToIntDef(0);
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        P_Zuban5 = wkQuery->FieldByName("DNO5")->AsString.TrimRight(); // �}��
        P_SZuban5 = wkQuery->FieldByName("WKDNO5")->AsString.TrimRight();
        // ��}�p�}��
        // ***********************************

        G_Log_Text = "�ގ�����    �" + P_Zaisitu + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}    ��1    �" + P_Zuban1 + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}    ��2    �" + P_Zuban2 + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}    ��3    �" + P_Zuban3 + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}    ��4    �" + P_Zuban4 + "����擾�B";
        Write_Log(G_Log_Text);
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        G_Log_Text = "�}    ��5    �" + P_Zuban5 + "����擾�B";
        Write_Log(G_Log_Text);
        // ***********************************
        G_Log_Text = "��}�p�}��1  �" + P_SZuban1 + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��2  �" + P_SZuban2 + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��3  �" + P_SZuban3 + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��4  �" + P_SZuban4 + "����擾�B";
        Write_Log(G_Log_Text);
        // 2005.02.08 ɽ�ٌ��a�A���x�v�L���ǉ�
        G_Log_Text = "��}�p�}��5  �" + P_SZuban5 + "����擾�B";
        Write_Log(G_Log_Text);
        // ***********************************
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �[���ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
void __fastcall TNozzleDetails::Tankan_Data_Sakusei(void) {
    int i;
    AnsiString TANKAN[8] = {"S1", "S2", "S3", "S4", "E1", "E2", "E3", "E4"};
    // 2016.08.26 �ގ����ޕύX
    AnsiString s_Text;
    // ***********************
    // 2019.04.11 �����O���i�ǉ�_S
    AnsiString RINGDNO[8];
    int j;
    // 2019.04.11 �����O���i�ǉ�_E

    for (i = 0; i < 8; i++) {
        // �[���ް��쐬
        if (Get_Tankan_Data(TANKAN[i])) {
            // 2019.04.11 �����O���i�ǉ�_S
            RINGDNO[i] = P_Zuban;
            // 2019.04.11 �����O���i�ǉ�_E
            // ����
            if (TANKAN[i] == "S1" || TANKAN[i] == "S2" || TANKAN[i] == "S3" ||
                TANKAN[i] == "S4") {
                P_Sikibetu = "FRNG-S";
            }
            else {
                P_Sikibetu = "FRNG-E";
            }

            // �ގ�
            if ((TANKAN[i].SubString(2, 1) == "1") || (TANKAN[i].SubString(2,
                1) == "3")) {
                P_Zaisitu =
                    Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
                // 2003.12.22 SGP(SS400)
                // 2016.08.26 �ގ����ޕύX
                //if (P_HEADS_DATA[1060].SubString(1, 4) == "1060") {
                s_Text = P_HEADS_DATA[1060].SubString(1,4);
                s_Text = Type_MAT(s_Text);
                if ( s_Text == "SGP" ) { // �r�f�o
                // ***********************
                    P_Zaisitu = P_Zaisitu + "�i�r�r�S�O�O�j";
                    P_EZaisitu = P_EZaisitu + "�i�r�r�S�O�O�j";
                }
                // *********************
            }
            // 2008.07.24 CX-10D�ǉ�
            else if ((TANKAN[i].SubString(2, 1) == "4") &&
                (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c")) {
                P_Zaisitu =
                    Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
                // 2016.08.26 �ގ����ޕύX
                //if (P_HEADS_DATA[1060].SubString(1, 4) == "1060") {
                s_Text = P_HEADS_DATA[1060].SubString(1,4);
                s_Text = Type_MAT(s_Text);
                if ( s_Text == "SGP" ) { // �r�f�o
                // ***********************
                    P_Zaisitu = P_Zaisitu + "�i�r�r�S�O�O�j";
                    P_EZaisitu = P_EZaisitu + "�i�r�r�S�O�O�j";
                }
            }
            // *********************
            else {
                P_Zaisitu =
                    Search_HD_MATERIAL(P_HEADS_DATA[1061].SubString(1, 4));
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[1061].SubString(1, 4));
                // 2003.12.22 SGP(SS400)
                 // 2016.08.26 �ގ����ޕύX
                //if (P_HEADS_DATA[1061].SubString(1, 4) == "1060") {
                s_Text = P_HEADS_DATA[1061].SubString(1,4);
                s_Text = Type_MAT(s_Text);
                if ( s_Text == "SGP" ) { // �r�f�o
                // ***********************
                    P_Zaisitu = P_Zaisitu + "�i�r�r�S�O�O�j";
                    P_EZaisitu = P_EZaisitu + "�i�r�r�S�O�O�j";
                }
                // *********************
            }

            if (P_Zaisitu == "") { // �[�Ǎގ��ɊY���Ȃ��ŁA�t�����[�ǂ̏ꍇ
                if ((P_Katasiki.SubString(1, 6) == "�[�ǂw") ||
                    (P_Katasiki.SubString(1, 6) == "�[�ǂx")) {
                    // 2012.05.09 �t�����[�Ǎގ�����
                    //// �t�����[�ǂ̍ގ��́A�v���[�g�ގ�����ϊ�����
                    ////2004.02.05
                    ////P_Zaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[43].TrimRight(), 1));
                    ////P_EZaisitu = P_Zaisitu;
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[43].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[43].TrimRight(), 1));
                    ////**********

                    // 2013.02.14 �ގ��擾�ύX
                    // if ( TANKAN[i] == "S1" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1248].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1248].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "S2" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1249].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1249].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "S3" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1250].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1250].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "S4" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1251].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1251].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "E1" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1252].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1252].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "E2" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1253].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1253].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "E3" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1254].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1254].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "E4" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1255].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1255].TrimRight(), 1));
                    // }
                    if (TANKAN[i] == "S1") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1248].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1248].TrimRight());
                    }
                    else if (TANKAN[i] == "S2") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1249].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1249].TrimRight());
                    }
                    else if (TANKAN[i] == "S3") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1250].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1250].TrimRight());
                    }
                    else if (TANKAN[i] == "S4") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1251].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1251].TrimRight());
                    }
                    else if (TANKAN[i] == "E1") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1252].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1252].TrimRight());
                    }
                    else if (TANKAN[i] == "E2") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1253].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1253].TrimRight());
                    }
                    else if (TANKAN[i] == "E3") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1254].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1254].TrimRight());
                    }
                    else if (TANKAN[i] == "E4") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1255].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1255].TrimRight());
                    }
                    // *************************
                    // *****************************
                }
            }

            // �\����
            P_Yobisu = 0;

            // if ( P_Zaisitu != "" ) {
            // �d�l1,2
            Get_Siyou_Data(TANKAN[i], 1, "");

            Write_Nozzle_Data();
            // }
        }
        else {
            // 2019.04.11 �����O���i�ǉ�_S
            RINGDNO[i] = "";
            // 2019.04.11 �����O���i�ǉ�_E
            G_Log_Text = TANKAN[i] + "�[���ް�  �[�ǎ�ނ��0��̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        }
    }

    // 2019.04.11 �����O���i�ǉ�_S
    for (j = 0; j < 8; j++) {
        if ( RINGDNO[j] == "") {
            // �[�ǐ}�Ԗ���
            G_Log_Text = TANKAN[j]
                + "�ݸޕ��i�ް�  �[�ǐ}�����̈׍쐬���܂���B";
            Write_Log(G_Log_Text);
        } else {
            // �[�ǐ}�ԗL��
            // �ݸޕ��i�ް��쐬
            if ( Get_Ring_Data(RINGDNO[j],"1",TANKAN[j])) {
                // �Z�p���j�b�v���L��
                Write_Nozzle_Data();
            } else {
                // �Z�p���j�b�v������
                G_Log_Text = TANKAN[j]
                    + "�ݸޕ��i�ް�  �Z�pƯ��ٖ����̈׍쐬���܂���B";
                Write_Log(G_Log_Text);
            }
            if ( Get_Ring_Data(RINGDNO[j],"2",TANKAN[j])) {
                // �s�|�R�l�N�^�L��
                Write_Nozzle_Data();
            } else {
                // �s�|�R�l�N�^����
                G_Log_Text = TANKAN[j]
                    + "�ݸޕ��i�ް�  T-�ȸ������̈׍쐬���܂���B";
                Write_Log(G_Log_Text);
            }
            // 2020.03.18 �X�g���[�g�G���{�ǉ�_S
            if (G_KS_Syurui == "�q�w" && G_KS_Model == "�R�O") {
                //RX-30�^
                if ((P_HEADS_DATA[38].ToIntDef(0) >= 281) ||
                    (P_HEADS_DATA[163].ToIntDef(0) >= 281) ||
                    (P_HEADS_DATA[170].ToIntDef(0) >= 281)) {
                    if ( Get_Ring_Data(RINGDNO[j],"3",TANKAN[j])) {
                        // ��ذĴ��ޗL��
                        Write_Nozzle_Data();
                    } else {
                        // ��ذĴ��ޖ���
                        G_Log_Text = TANKAN[j]
                            + "�ݸޕ��i�ް�  ��ذĴ��ޖ����̈׍쐬���܂���B";
                        Write_Log(G_Log_Text);
                    }
                }
            }
            // 2020.03.18 �X�g���[�g�G���{�ǉ�_E
        }

    }
    // 2019.04.11 �����O���i�ǉ�_E

}

// ---------------------------------------------------------------------------
// ���{��֐����F �[���ް��쐬
// �T  �v      �F
// ��  ��      �F AnsiString Syurui�F�[�ǎ��(S1�`E4)
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Get_Tankan_Data(AnsiString Syurui) {
    AnsiString s_Tkn_Syurui; // �[�ǎ��

    /* 6��ڂ��e�ގ�����p��ɕύX
     int HEADS[8][6] = {{ 1136,  901,  898,  895,  892,  894 },
     { 1137,  916,  913,  910,  907,  909 },
     { 1138,  931,  928,  925,  922,  924 },
     { 1139,  946,  943,  940,  937,  939 },
     { 1140,  961,  958,  955,  952,  954 },
     { 1141,  976,  973,  970,  967,  969 },
     { 1142,  991,  988,  985,  982,  984 },
     { 1143, 1006, 1003, 1000,  997,  999 }};

     int HEADS[8][6] = {{ 1136,  901,  898,  895,  892,  893 },
     { 1137,  916,  913,  910,  907,  908 },
     { 1138,  931,  928,  925,  922,  923 },
     { 1139,  946,  943,  940,  937,  938 },
     { 1140,  961,  958,  955,  952,  953 },
     { 1141,  976,  973,  970,  967,  968 },
     { 1142,  991,  988,  985,  982,  983 },
     { 1143, 1006, 1003, 1000,  997,  998 }};
     */

    // 6��ڂ��e�ގ�����G�A�����A�t�����p��ɕύX  2002.10.20 S.Y
    int HEADS[8][11] = {
        {1136, 901, 898, 895, 892, 905, 1144, 1216, 1217, 893, 1184},
        {1137, 916, 913, 910, 907, 920, 1145, 1220, 1221, 908, 1188},
        {1138, 931, 928, 925, 922, 935, 1146, 1224, 1225, 923, 1192},
        {1139, 946, 943, 940, 937, 950, 1147, 1228, 1229, 938, 1196},
        {1140, 961, 958, 955, 952, 965, 1148, 1232, 1233, 953, 1200},
        {1141, 976, 973, 970, 967, 980, 1149, 1236, 1237, 968, 1204},
        {1142, 991, 988, 985, 982, 995, 1150, 1240, 1241, 983, 1208},
        {1143, 1006, 1003, 1000, 997, 1010, 1151, 1244, 1245, 998, 1212}};

    int s_Prs_Syurui; // ���͌v���
    int s_Tmp_Syurui; // ���x�v���
    int iWFflg; // WF�[�ǁH
    double s_Ank_Kokei; // ���������a
    double s_Nzl_Kokei; // ɽ�ٌ��a
    AnsiString s_Nzl_Tugite; // ɽ�ٌp��
    AnsiString s_Nzl_Syurui; // ɽ�َ��
    AnsiString s_NzCd; // ɽ�ٌp�躰��
    AnsiString s_PGtyp; // ���͌v��ށi�����p�j
    AnsiString s_TPtyp; // ���x�v��ށi�����p�j
    AnsiString s_DRtyp; // �t������ށi�����p�j
    AnsiString s_ARtyp; // �G�A������ށi�����p�j
    AnsiString s_KeyMat; // �ގ��L�[�i�����O�[�ǂ̂r�s�o�f�j
    AnsiString s_MatCD; // �[�Ǎގ��R�[�h
    AnsiString s_ULdiv; // �㉺�敪

    // 2009.10.28 ���p�[�����ǉ�
    AnsiString s_Text;
    AnsiString s_Mat; // �[���ގ�
    AnsiString s_BootsFlg; // �ް��׸�
    AnsiString s_FCode; // ��ݼ޺���
    AnsiString s_ENzl_Syurui; // Eɽ�َ��
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;
    // *************************
    // 2023.02.22 �����O�[�Ǐ����ύX_S
    AnsiString s_Model;
    // 2023.02.22 �����O�[�Ǐ����ύX_E

    int i_HeadsNo;

    s_Nzl_Syurui = "";

    P_Sikibetu = "";
    P_Katasiki = "";
    P_Hosoku = "";
    P_Zaisitu = "";
    P_Zuban = "";
    P_SZuban = "";
    P_Yobisu = 0;

    P_Siyou1 = "";
    P_Siyou2 = "";
    P_Siyou3 = "";

    if (Syurui == "S1") {
        i_HeadsNo = 0;
        s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        s_ULdiv = "U";
    }
    else if (Syurui == "S2") {
        i_HeadsNo = 1;
        s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1061].SubString(1, 4);
        s_ULdiv = "U";
    }
    else if (Syurui == "S3") {
        i_HeadsNo = 2;
        s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        s_ULdiv = "L";
    }
    else if (Syurui == "S4") {
        i_HeadsNo = 3;
        s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        // 2008.07.24 CX-10D�ǉ�
        // s_MatCD = P_HEADS_DATA[1061].SubString(1,4);
        if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
            s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        }
        else {
            s_MatCD = P_HEADS_DATA[1061].SubString(1, 4);
        }
        // *********************
        s_ULdiv = "L";
    }
    else if (Syurui == "E1") {
        i_HeadsNo = 4;
        s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        s_ULdiv = "U";
    }
    else if (Syurui == "E2") {
        i_HeadsNo = 5;
        s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1061].SubString(1, 4);
        s_ULdiv = "U";
    }
    else if (Syurui == "E3") {
        i_HeadsNo = 6;
        s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        s_ULdiv = "L";
    }
    else if (Syurui == "E4") {
        i_HeadsNo = 7;
        s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        // 2008.07.24 CX-10D�ǉ�
        // s_MatCD = P_HEADS_DATA[1061].SubString(1,4);
        if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
            s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        }
        else {
            s_MatCD = P_HEADS_DATA[1061].SubString(1, 4);
        }
        // *********************
        s_ULdiv = "L";
    }

    // 2021.10.04 �����O�[�ǂ̏ꍇ P_Siyou1�����Z�b�g_S
    if ( P_HEADS_DATA[HEADS[i_HeadsNo][0]].TrimRight().ToIntDef(0) == 3 ) {
        P_Siyou1 = "";
    }
    // 2021.10.04 �����O�[�ǂ̏ꍇ P_Siyou1�����Z�b�g_E

    // 2009.10.28 ���p�[�����ǉ�
    if ((P_HEADS_DATA[36].Pos("M") != 0) && (Syurui == "S1" || Syurui == "S2" ||
        Syurui == "S3" || Syurui == "S4")) {
        s_Text = P_HEADS_DATA[HEADS[i_HeadsNo][9]].TrimRight();

        // 2011.06.20 ɽ�ًK�i�ύX
        // s_FCode = s_Text.SubString(1,1) + "0" + s_Text.SubString(3,1);
        if (s_Text.ToIntDef(0) >= 1000) {
            s_FCode = s_Text.SubString(1, 2) + "0" + s_Text.SubString(4, 1);
        }
        else {
            s_FCode = s_Text.SubString(2, 1) + "0" + s_Text.SubString(4, 1);
        }
        // ***********************

        s_BootsFlg = "1";

        // ��ݼގd�l�ϊ��\  ����
        G_Log_Text = "��ݼގd�l�ϊ��\���A�" + s_FCode + "��Ƣ" + s_BootsFlg + "��Ō����B";
        Write_Log(G_Log_Text);

        s_SQL = "";
        s_SQL = s_SQL + "SELECT * FROM HD_FRNG_SPEC";
        s_SQL = s_SQL + "  WHERE FCODE = '" + s_FCode + "'";
        s_SQL = s_SQL + "    AND BOOTS = " + s_BootsFlg + "";

        wkQuery->Close();
        // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
        //wkQuery->DatabaseName = ODBC_DSN_DNO;
        wkQuery->ConnectionName = ODBC_DSN_DNO;
        // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
        wkQuery->SQL->Clear();
        wkQuery->SQL->Add(s_SQL);
        wkQuery->Open();
        wkQuery->First();

        if (wkQuery->Eof) {
        }
        else {

            // 2016.08.26 �ގ����ޕύX
            //s_Mat = Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
            //s_Mat = ZenToHan(s_Mat);
            //if ((s_Mat.TrimRight() == "CAC703") ||
            //    (s_Mat.TrimRight() == "STKM13A")) {
            //    // ALBC,CS
            // 2018.03.19 �Б��[�ǂɑΉ�_S
            //s_Mat = P_HEADS_DATA[1060].SubString(1,4);
            s_Mat = s_MatCD;
            // 2018.03.19 �Б��[�ǂɑΉ�_E
            s_Mat = Type_MAT(s_Mat);
            if ( s_Mat == "ALBC" || s_Mat == "CS" ) {
                // �`�k�a�b�A�b�r(�r�s�j�l�P�R�`)
            // ***********************
                // 2014.12.12 �ݸޒ[�����L�ύX
                // ���͌v���
                s_Prs_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][1]].TrimRight().ToIntDef(0);
                // �o���L�� ����
                if (s_Prs_Syurui == 0) {
                    P_Siyou1 = "";
                } else {
                    P_Siyou1 = "�o���t";
                }
                // ***************************
                if (Syurui == "S3") {
                    s_ENzl_Syurui = P_HEADS_DATA[981].TrimRight();
                }
                else if (Syurui == "S4") {
                    s_ENzl_Syurui = P_HEADS_DATA[996].TrimRight();
                }
                else {
                    s_ENzl_Syurui = "";
                }

                //2016.05.18 �����O�[�ǒǉ�
                // ɽ�ٌp�躰��
                s_NzCd = P_HEADS_DATA[HEADS[i_HeadsNo][9]].SubString(1, 2);
                if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O" && s_NzCd == "41") {
                }
                // 2023.02.22 �����O�[�Ǐ����ύX_S
                else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O�r" && s_NzCd == "41") {
                }
                //else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�O") {
                //}
                 else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P" && P_HEADS_DATA[36].Pos("Y") == 0) {
                }
                else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c" && P_HEADS_DATA[36].Pos("Y") == 0) {
                }
                // 2023.02.22 �����O�[�Ǐ����ύX_E
                else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O") {
                }
                else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
                }
                // 2023.02.22 �����O�[�Ǐ����ύX_S
                //else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�O") {
                //}
                 else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�P" && P_HEADS_DATA[36].Pos("Y") == 0) {
                }
                // 2023.02.22 �����O�[�Ǐ����ύX_E
                //if (s_ENzl_Syurui == "5") {
                else if (s_ENzl_Syurui == "5") {
                //*************************�@�@
                    P_Siyou1 = P_Siyou1 + "�s���V�T�A";
                }
                else {
                    P_Siyou1 = P_Siyou1 + "�s���W�O�A";
                }

            }

            P_Siyou1 = P_Siyou1 + wkQuery->FieldByName("SPEC1")
                ->AsString.TrimRight();
            P_Siyou1 = P_Siyou1 + "-";

            s_Text =
                ChgNozzleSize(StrToDblDef
                (P_HEADS_DATA[HEADS[i_HeadsNo][4]].TrimRight(), 0.00));

            P_Siyou1 = P_Siyou1 + s_Text + "�`�p";

        }
    }
    // *************************

    // ���͌v���
    s_Prs_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][1]].TrimRight().ToIntDef(0);
    // ���x�v���
    s_Tmp_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][2]].TrimRight().ToIntDef(0);
    // ���������a
    s_Ank_Kokei = StrToDblDef(P_HEADS_DATA[HEADS[i_HeadsNo][3]].TrimRight(), 0);
    // ɽ�ٌ��a
    s_Nzl_Kokei = StrToDblDef(P_HEADS_DATA[HEADS[i_HeadsNo][4]].TrimRight(), 0);
    // 2011.06.20 ɽ�ًK�i�ύX
    // �G�A�����t�����p��(�擪�ꌅ)
    // s_Nzl_Tugite  = P_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(1,1);
    s_Nzl_Tugite = P_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(1, 2);
    // ɽ�ٌp�躰��
    // s_NzCd = P_HEADS_DATA[HEADS[i_HeadsNo][9]].SubString(1,1);
    s_NzCd = P_HEADS_DATA[HEADS[i_HeadsNo][9]].SubString(1, 2);
    // ***********************

    // ***********************************
    // ***  �[���ް��@�쐬
    // ***********************************
    s_Tkn_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][0]].TrimRight(); // �[�ǎ��
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(3,1) == "1" ) ||
    // ( P_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(3,1) == "2" ) ) {
    if ((P_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(4, 1) == "1") ||
        (P_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(4, 1) == "2")) {
        // ***********************
        iWFflg = 1;
    }
    else {
        iWFflg = 0;
    }
    // (1)
    if (s_Tkn_Syurui == "0") {
        return false;
    }
    P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
    // 2008.07.24 CX-10�[��
    if (P_Model == "CX-10" || P_Model == "CX-10D") {
        // �ގ�����
        // 2016.08.26 �ގ����ޕύX
        //s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        //if (s_KeyMat.TrimRight() == "�b�`�b�V�O�R") {
        //    s_KeyMat = "ALBC";
        //}
        //else if (s_KeyMat.TrimRight() == "�r�s�o�f") {
        //    s_KeyMat = "STPG";
        //}
        //else {
        //    s_KeyMat = "-";
        //}
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // �`�k�a�b
        } else if ( s_KeyMat == "STPG") {
            // �r�s�o�f
        } else {
            // ���̑�
            s_KeyMat = "-";
        }
        // ***********************
        s_ULdiv = "-"; // �ʒu�Œ�
        s_PGtyp = "-"; // �R�b�N�Ȃ�
        s_TPtyp = "-"; // ���x�v���͌v���Ȃ�
        s_DRtyp = "-"; // �t���Ȃ�
        s_ARtyp = "-"; // �G�A���Ȃ�

        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);

        if (P_Katasiki == "") {
            P_Katasiki = "�Y���[�ǂȂ�";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);
            return true;
        }
    }
    // 2016.04.22 CX-00�[��
    // 2023.02.22 �����O�[�Ǐ����ύX_S
    //// 2021.01.06 CX(W)-03�ǉ�(CX(W)-00��CX(W)-01)_S
    ////else if (P_Model == "CX-00" || P_Model == "CXW-00") {
    //else if (P_Model == "CX-01" || P_Model == "CXW-01") {
    //// 2021.01.06 CX(W)-03�ǉ�(CX(W)-00��CX(W)-01)_E
    else if ((P_Model == "CX-01" || P_Model == "CXW-01") && P_HEADS_DATA[36].Pos("Y") == 0) {
    // 2023.02.22 �����O�[�Ǐ����ύX_E
        // �ގ�����
        // 2016.08.26 �ގ����ޕύX
        //s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        //if (s_KeyMat.TrimRight() == "�b�`�b�V�O�R") {
        //    s_KeyMat = "ALBC";
        //}
        //else if (s_KeyMat.TrimRight() == "�r�s�o�f") {
        //    s_KeyMat = "STPG";
        //}
        //else {
        //    s_KeyMat = "-";
        //}
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // �`�k�a�b
        } else if ( s_KeyMat == "STPG") {
            // �r�s�o�f
        } else {
            // ���̑�
            s_KeyMat = "-";
        }
        // ***********************
        s_ULdiv = "-"; // �ʒu�Œ�
        s_PGtyp = "-"; // �R�b�N�Ȃ�
        s_TPtyp = "-"; // ���x�v���͌v���Ȃ�
        s_DRtyp = "-"; // �t���Ȃ�
        s_ARtyp = "-"; // �G�A���Ȃ�

        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);

        if (P_Katasiki == "") {
            P_Katasiki = "�Y���[�ǂȂ�";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);
            return true;
        }
    }
    // ********************
    // 2022.04.21 CX-01D�ǉ�_S
    // 2023.02.22 �����O�[�Ǐ����ύX_S
    //else if (P_Model == "CX-01D" || P_Model == "CXW-01D") {
    else if ((P_Model == "CX-01D" || P_Model == "CXW-01D") && P_HEADS_DATA[36].Pos("Y") == 0) {
    // 2023.02.22 �����O�[�Ǐ����ύX_E
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // �`�k�a�b
        } else if ( s_KeyMat == "STPG") {
            // �r�s�o�f
        } else {
            // ���̑�
            s_KeyMat = "-";
        }
        s_ULdiv = "-"; // �ʒu�Œ�
        s_PGtyp = "-"; // �R�b�N�Ȃ�
        s_TPtyp = "-"; // ���x�v���͌v���Ȃ�
        s_DRtyp = "-"; // �t���Ȃ�
        s_ARtyp = "-"; // �G�A���Ȃ�

        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);

        if (P_Katasiki == "") {
            P_Katasiki = "�Y���[�ǂȂ�";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);
            return true;
        }
    }
    // 2022.04.21 CX-01D�ǉ�_E
    // 2013.04.03 SX-30 JIS30K�[��
    // 2016.07.25 SX-30S�ǉ�
    //else if (P_Model == "SX-30" && s_NzCd == "41") {
    else if ( (P_Model == "SX-30" || P_Model == "SX-30S") &&
               s_NzCd =="41" ) {
    // *********************�@�@
        // �ގ�����
        // 2016.08.26 �ގ����ޕύX
        //s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        //if (s_KeyMat.TrimRight() == "�b�`�b�V�O�R") {
        //    s_KeyMat = "ALBC";
        //}
        //else if (s_KeyMat.TrimRight() == "�r�s�j�l�P�R�`") {
        //    s_KeyMat = "CS";
        //}
        //else {
        //    s_KeyMat = "-";
        //}
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // �`�k�a�b
        } else if ( s_KeyMat == "CS") {
            // �b�r(�r�s�j�l�P�R�`)
        } else {
            // ���̑�
            s_KeyMat = "-";
        }
        // ***********************
        s_ULdiv = "-"; // �ʒu�Œ�
        s_PGtyp = "-"; // �R�b�N�Ȃ�
        s_TPtyp = "-"; // ���x�v���͌v���Ȃ�
        s_DRtyp = "-"; // �t���Ȃ�
        s_ARtyp = "-"; // �G�A���Ȃ�

        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);

        if (P_Katasiki == "") {
            P_Katasiki = "�Y���[�ǂȂ�";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);
            return true;
        }
    }
    // ***************************
    // ��2003/1/9
    else if ((P_HEADS_DATA[36].Pos("M") != 0) && (Syurui.SubString(1,
        1) == "S")) {
        // if ( ( P_HEADS_DATA[36].Pos("M") != 0 ) &&
        // ( Syurui.SubString(1,1) == "S" ) ) {
        // ********************
        // 2004.05.20 ���p�[�Ǎގ��ǉ�
        // �ގ�����
        // 2016.08.26 �ގ����ޕύX
        //s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        //if (s_KeyMat.TrimRight() == "�b�`�b�V�O�R") {
        //    s_KeyMat = "ALBC";
        //}
        //else if (s_KeyMat.TrimRight() == "�r�s�o�f") {
        //    s_KeyMat = "STPG";
        //}
        //else if (s_KeyMat.TrimRight() == "�r�s�j�l�P�R�`") {
        //    s_KeyMat = "CS";
        //}
        //else {
        //    s_KeyMat = "-";
        //}
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // �`�k�a�b
        } else if ( s_KeyMat == "STPG") {
            // �r�s�o�f
        } else if ( s_KeyMat == "CS") {
            // �b�r(�r�s�j�l�P�R�`)
        // 2019.04.12 SGP�[�ǒǉ�_S
        } else if ( s_KeyMat == "SGP") {
            // �r�f�o
        // 2019.04.12 SGP�[�ǒǉ�_E
        } else {
            // ���̑�
            s_KeyMat = "-";
        }
        // ***********************
        //// �ގ��i�r�s�o�f�j����
        // s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        // if ( s_KeyMat.TrimRight() == "�r�s�o�f") {
        // s_KeyMat = "STPG";
        // } else {
        // s_KeyMat = "-";
        // }
        // ***************************
        // ���p�R�b�N�^�ʌ`�� ����
        // 2011.04.22 ���d�H���p�R�b�N�p�~
        // ��c�w���ɂ��
        // if ( P_HEADS_DATA[1063].Pos("���d�H") != 0 ){
        // s_PGtyp = "C";
        // } else {
        // s_PGtyp = "T";
        // }
        s_PGtyp = "T";
        // *********************************
        // �o���L�� ����
        // if( P_Model == "RX-70" ) {
        if (s_Prs_Syurui == 0) {
            s_TPtyp = "-";
        }
        else {
            s_TPtyp = "P";
        }
        // } else {
        // s_TPtyp = "-";
        // }
        s_DRtyp = "-";
        s_ARtyp = "-";
        // 2003.09.24 �q�w�|�V�O�̂o�����ʂ���ߌ^�����ʂ�����
        // if( P_Model == "RX-70" ) {
        // P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, false);
        // } else {
        // P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);
        // }
        AnsiString s_Frame; // �^�����
        AnsiString s_Ver; // �o�[�W�����m��.
        int i_Length; // ������

        // 2004.05.13 ���p�[�Ǎގ��ǉ�
        if (P_Model == "RX-70" && (s_KeyMat == "STPG" || s_KeyMat == "-")) {
            // if( P_Model == "RX-70" ){
            // ***************************
            s_Frame = P_HEADS_DATA[36].TrimRight(); // �ڰ�
            i_Length = s_Frame.Length();
            s_Frame = s_Frame.SubString(i_Length, 1);
            s_Ver = P_HEADS_DATA[1267].TrimRight();
            if (s_Frame == "R") {
                s_Frame = P_HEADS_DATA[36].TrimRight();
                s_Frame = s_Frame.SetLength(i_Length - 1);
            }
            else {
                s_Frame = P_HEADS_DATA[36].TrimRight();
            }

            if (s_Frame == "NPM" || s_Frame == "KNPM" || s_Frame == "TNPM") {
                if (s_Ver == "200207") {
                    s_KeyMat = "HP";
                }
                else {
                    s_KeyMat = "P";
                }
            }
            else if (s_Frame == "NHPM" || s_Frame == "KNHPM" ||
                s_Frame == "TNHPM") {
                s_KeyMat = "HP";
            }
            else {
                s_KeyMat = "-";
            }
        }
        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);
        // ***************************************************
        if (P_Katasiki == "") {
            P_Katasiki = "�Y���[�ǂȂ�";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);

            if (s_DRtyp == "S") {
                P_Katasiki = P_Katasiki + "  " +
                    P_HEADS_DATA[HEADS[i_HeadsNo][7]].TrimRight() +
                    P_HEADS_DATA[HEADS[i_HeadsNo][8]].TrimRight();
            }
            return true;
        }

        // ��2003/1/9
    }
    else {
        if (s_Tkn_Syurui == "0") {
            return false;
        }

        // 02.10.16 �V�K�ǉ�����
        // 2006.04.14 �G�A���[�ǒǉ�
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4" || Syurui == "E1" || Syurui == "E2" ||
            Syurui == "E3" || Syurui == "E4") {
            // if (Syurui == "S3" || Syurui == "S4" || Syurui == "E3" || Syurui == "E4"){
            // *************************
            // ɽ�َ�� = "3" ���� ɽ�ٌp��̐擪�񌅖� = "4"
            // 2011.06.20 ɽ�ًK�i�ύX
            // if (s_Nzl_Syurui == "3" && P_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(2,1) != "4" && s_Nzl_Kokei != 0 ){
            if (s_Nzl_Syurui == "3" && P_HEADS_DATA[HEADS[i_HeadsNo][5]]
                .SubString(3, 1) != "4" && s_Nzl_Kokei != 0) {
                // ***********************
                // 2011.06.20 ɽ�ًK�i�ύX
                // if (s_Nzl_Tugite == "7"){
                if (s_Nzl_Tugite == "07") {
                    // ***********************
                    P_Katasiki = "�[�ǂw";
                    P_Zuban = "�d�h�P�S�S�O�U�O";
                    P_SZuban = P_Zuban;
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( s_NzCd == "4" || s_NzCd == "5" ){  //ANSI
                    // Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    ////2007.07.30 �t���G�A���[�ǕύX
                    // } else if ( s_NzCd == "8" || s_NzCd == "9" ){  //JPI
                    // Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    ////*****************************
                    if (s_NzCd == "04" || s_NzCd == "05") { // ANSI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "08" || s_NzCd == "09") { // JPI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "11" || s_NzCd == "12" || s_NzCd == "13")
                    { // GB
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "21" || s_NzCd == "22" || s_NzCd == "23")
                    { // DIN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                        // ***********************
                        // 2013.04.03 ɽ�ًK�i�ǉ�
                    }
                    else if (s_NzCd == "44") { // ANSI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "48") { // JPI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "14") { // GB
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "24") { // DIN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "31" || s_NzCd == "32" ||
                        s_NzCd == "33" || s_NzCd == "34") { // EN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                        // ***********************
                    }
                    else {
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 0);
                    }
                    // ***********************
                    // �[��X�̏ꍇ�́A���i�����ɉt�����o���u�̋L�q��ǉ�����
                    P_Katasiki = P_Katasiki + "  " +
                        P_HEADS_DATA[HEADS[i_HeadsNo][7]].TrimRight() +
                        P_HEADS_DATA[HEADS[i_HeadsNo][8]].TrimRight();
                    return true;
                }
                else {
                    P_Katasiki = "�[�ǂx";
                    P_Zuban = "�d�h�P�S�S�O�U�P";
                    P_SZuban = P_Zuban;
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( s_NzCd == "4" || s_NzCd == "5" ){  //ANSI
                    // Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    ////2007.07.30 �t���G�A���[�ǕύX
                    // } else if ( s_NzCd == "8" || s_NzCd == "9" ){  //JPI
                    // Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    ////*****************************
                    if (s_NzCd == "04" || s_NzCd == "05") { // ANSI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "08" || s_NzCd == "09") { // JPI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "11" || s_NzCd == "12" || s_NzCd == "13")
                    { // GB
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "21" || s_NzCd == "22" || s_NzCd == "23")
                    { // DIN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                        // ***********************
                        // 2013.04.03 ɽ�ًK�i�ǉ�
                    }
                    else if (s_NzCd == "44") { // ANSI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "48") { // JPI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "14") { // GB
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "24") { // DIN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "31" || s_NzCd == "32" ||
                        s_NzCd == "33" || s_NzCd == "34") { // EN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                        // ***********************
                    }
                    else {
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 0);
                    }
                    return true;
                }
            }
        }

        // ���͌v�����L�[�쐬
        if (s_Prs_Syurui == 0) {
            s_PGtyp = "-";
        }
        else if ((s_Prs_Syurui > 5) && (s_Prs_Syurui < 16)) { // �u��
            s_PGtyp = "K";
        }
        else if (s_Prs_Syurui == 5) { // ����
            s_PGtyp = "S";
            // 2000.02.26 ���͌v�ǉ�
        }
        else if ((s_Prs_Syurui > 17) && (s_Prs_Syurui < 21)) { // ����
            s_PGtyp = "S";
            // *********************
        }
        else {
            s_PGtyp = "B"; // ������
        }

        // ���x�v�����L�[�쐬
        if (s_Tmp_Syurui == 0) {
            s_TPtyp = "-";
        }
        else {
            s_TPtyp = "T";
        }
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "E1" ||
            Syurui == "E2") {
            // �G�A�����L��
            s_DRtyp = "-";
            if (s_Ank_Kokei > 0) {
                s_ARtyp = "A";
            }
            else {
                s_ARtyp = "-";
            }
        }
        else {
            // �t�����T�C�Y
            s_ARtyp = "-";
            if (s_Ank_Kokei > 0) {
                // 2011.08.04 ɽ�ًK�i�ύX(���C��)
                // if ( s_Nzl_Tugite != "7" ) {
                if (s_Nzl_Tugite != "07") {
                    // ***********************
                    s_DRtyp = "L";
                }
                else {
                    s_DRtyp = "S";
                }
            }
            else {
                s_DRtyp = "-";
            }
        }

        // 2023.02.22 �����O�[�Ǐ����ύX_S
        //P_Katasiki = Search_HD_FRNG_MST(P_Model, FormatFloat(0, s_Nzl_Kokei),
        //    Syurui.SubString(1, 1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, false);

        s_Model = P_Model;

        if ((P_Model == "CX-01" || P_Model == "CXW-01" ||
             P_Model == "CX-01D" || P_Model == "CXW-01D") &&
            P_HEADS_DATA[36].Pos("Y") != 0) {

            s_Model = P_Model+ "Y";

        }

        P_Katasiki = Search_HD_FRNG_MST(s_Model, FormatFloat(0, s_Nzl_Kokei),
            Syurui.SubString(1, 1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, false);
        // 2023.02.22 �����O�[�Ǐ����ύX_E
        if (P_Katasiki == "") {
            // P_Katasiki = Search_HD_FRNG_MST("-", s_Nzl_Kokei, Syurui.SubString(1,1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp);
            P_Katasiki = "�Y���[�ǂȂ�";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 0);

            if (s_DRtyp == "S") {
                // P_Hosoku = P_HEADS_DATA[HEADS[i_HeadsNo][7]].TrimRight() + P_HEADS_DATA[HEADS[i_HeadsNo][8]].TrimRight();
                P_Katasiki = P_Katasiki + "  " +
                    P_HEADS_DATA[HEADS[i_HeadsNo][7]].TrimRight() +
                    P_HEADS_DATA[HEADS[i_HeadsNo][8]].TrimRight();
            }
            return true;
        }
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �����O���i�ް��쐬
// �T  �v      �F
// ��  ��      �F AnsiString Dno�F�ݸސ}��
//             �F AnsiString Code�F���i����
//             �F AnsiString Syurui�F�[�ǎ��(S1�`E4)
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2019.04.11�ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Get_Ring_Data(AnsiString Dno, AnsiString Code,
    AnsiString Syurui) {

    int HEADS[8][11] = {
        {1136, 901, 898, 895, 892, 905, 1144},
        {1137, 916, 913, 910, 907, 920, 1145},
        {1138, 931, 928, 925, 922, 935, 1146},
        {1139, 946, 943, 940, 937, 950, 1147},
        {1140, 961, 958, 955, 952, 965, 1148},
        {1141, 976, 973, 970, 967, 980, 1149},
        {1142, 991, 988, 985, 982, 995, 1150},
        {1143, 1006, 1003, 1000, 997, 1010, 1151}};
        //1��ځF�[�ǎ��
        //2��ځF���͌v���
        //3��ځF���x�v���
        //4��ځF�t���A�������a
        //5��ځFɽ�ٌ��a
        //6��ځF�t���A�����p��
        //7�s�ځFɽ�ٌp��

    AnsiString s_Tkn_Syurui;  // �[�ǎ��
    int i_Prs_Syurui;         // ���͌v���
    int i_Tmp_Syurui;         // ���x�v���
    double d_Ank_Kokei;       // �t���A�������a
    double d_Nzl_Kokei;       // ɽ�ٌ��a
    AnsiString s_AnCd;        // �t���A�����p�躰��
    AnsiString s_NzCd;        // ɽ�ٌp�躰��
    AnsiString s_Ring_Size;   // �����O���a�i�����p�j
    AnsiString s_PGtyp;       // ���͌v��ށi�����p�j
    AnsiString s_Text;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;

    int i_HeadsNo;

    // i_HeadsNo
    if (Syurui == "S1") {
        i_HeadsNo = 0;
    }
    else if (Syurui == "S2") {
        i_HeadsNo = 1;
    }
    else if (Syurui == "S3") {
        i_HeadsNo = 2;
    }
    else if (Syurui == "S4") {
        i_HeadsNo = 3;
    }
    else if (Syurui == "E1") {
        i_HeadsNo = 4;
    }
    else if (Syurui == "E2") {
        i_HeadsNo = 5;
    }
    else if (Syurui == "E3") {
        i_HeadsNo = 6;
    }
    else if (Syurui == "E4") {
        i_HeadsNo = 7;
    }

    // �[�ǎ��
    s_Tkn_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][0]].TrimRight();
    // ���͌v���
    i_Prs_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][1]].TrimRight().ToIntDef(0);
    // ���x�v���
    i_Tmp_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][2]].TrimRight().ToIntDef(0);
    // ���������a
    d_Ank_Kokei = StrToDblDef(P_HEADS_DATA[HEADS[i_HeadsNo][3]].TrimRight(), 0);
    // ɽ�ٌ��a
    d_Nzl_Kokei = StrToDblDef(P_HEADS_DATA[HEADS[i_HeadsNo][4]].TrimRight(), 0);
    // �����t���p��
    s_AnCd = P_HEADS_DATA[HEADS[i_HeadsNo][5]].TrimRight();
    // ɽ�ٌp�躰��
    s_NzCd = P_HEADS_DATA[HEADS[i_HeadsNo][6]].TrimRight();

    // �����O���a�i�����p�j
    s_Ring_Size = FormatFloat(0, d_Nzl_Kokei);

    // ���͌v��ށi�����p�j
    if (i_Prs_Syurui==0) {
        s_PGtyp = "-";
    } else {
        s_PGtyp = "P";
    }

    // *********************************************
    // ***�@�����O�}�X�^(HD_RING) ����
    // *********************************************
    G_Log_Text = "�����O�}�X�^���A�" + Dno + "��Ƣ" + Code + "��Ƣ"
        + s_Ring_Size + "��Ƣ" + s_PGtyp + "��Ō����B";
    Write_Log(G_Log_Text);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_RING";
    s_SQL = s_SQL + "  WHERE DNO = '" + Dno + "'";
    s_SQL = s_SQL + "    AND PARTS_CODE = '" + Code + "'";
    s_SQL = s_SQL + "    AND RING_SIZE = '" + s_Ring_Size + "'";
    s_SQL = s_SQL + "    AND RING_TYPE = '" + s_PGtyp + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        G_ErrLog_Text = "�ް��쐬��    �ݸސ}�ʢ" + Dno +
            "� ���i���ޢ" + s_Text + "� ���a�" + s_Ring_Size +
            "� �ݸގ�ʢ" + s_PGtyp +
            "� �́A�ݸ�Ͻ�(HD_RING)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        // ���i�\���ʎq
        P_Sikibetu = "FRNG-P";                      //���ʎq
        P_Katasiki = wkQuery->FieldByName("PARTS_NAME")
            ->AsString.TrimRight();                 // ���i����
        P_Hosoku = "";

        // �[�Ǘp�⑫
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")
            ->AsString.TrimRight();                 // �ގ�����
        P_EZaisitu = Search_HD_MAT_JtoE(P_Zaisitu); // �ގ�����(�p)
        P_Zuban = "";                               // �}��
        P_SZuban = "";                              // ��}�p�}��
        P_Siyou1 = wkQuery->FieldByName("SPEC1")
            ->AsString.TrimRight();                 // �d�l1�@
        P_Siyou2 = wkQuery->FieldByName("SPEC2")
            ->AsString.TrimRight();                 // �d�l2
    }
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ɽ��
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Write_Nozzle_Data(void) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;

    AnsiString wk_FilePass;

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
    s_Text = "NOZZLE";
    fputs(s_Text.c_str(), wkfp);

    // 2  ���ʎq
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), wkfp);

    // 3  ���i�^��
    s_Text = "," + P_Katasiki;
    fputs(s_Text.c_str(), wkfp);

    // 4  ���i����
    s_Text = "," + P_Hosoku;
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
    s_Text = "," + P_Siyou1;
    fputs(s_Text.c_str(), wkfp);
    // 12 �d�l�Q
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), wkfp);
    // �d�l�R
    s_Text = "," + P_Siyou3;
    fputs(s_Text.c_str(), wkfp);

    // 13 �d�l�R,�S,�T
    s_Text = ",,";
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
// ���{��֐����F HD_FRNG_DNOð��ٌ�������
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Search_HD_FRNG_DNO(int Model, int Typ,
    AnsiString Key, int flgAnsi) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;
    AnsiString s_Frame;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_FRNG_DNO";
    s_SQL = s_SQL + "  WHERE MODEL_TYPE = " + Model + "";
    s_SQL = s_SQL + "    AND FRANGE_TYPE = " + Typ + "";
    s_SQL = s_SQL + "    AND FLG_ANSI = " + flgAnsi + "";
    s_SQL = s_SQL + "    AND FRANGE_NAME = '" + Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        P_Zuban = "";
        P_SZuban = "";

        delete wkQuery;
        return false;
    }
    else {
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // �}��
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // ��}�p�}��

        G_Log_Text = "�}    ��1    �" + P_Zuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��2  �" + P_SZuban + "����擾�B";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �t�������a�\�L�ϊ�
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F �ϊ�����
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall TNozzleDetails::ChgAirDrnSize(double dSize) {
    AnsiString sSizeStr;

    sSizeStr = "";
    if (dSize == 10.0) {
        sSizeStr = "3/8";
    }
    else if (dSize == 15.0) {
        sSizeStr = "1/2";
    }
    else if (dSize == 20.0) {
        sSizeStr = "3/4";
    }
    else if (dSize == 25.0) {
        sSizeStr = "1";
    }
    else if (dSize == 32.0) {
        sSizeStr = "1 1/4";
    }
    else if (dSize == 40.0) {
        sSizeStr = "1 1/2";
    }
    else if (dSize == 50.0) {
        sSizeStr = "2";
    }
    else if (dSize == 65.0) {
        sSizeStr = "2 1/2";
    }
    else if (dSize == 80.0) {
        sSizeStr = "3";
    }
    else if (dSize == 90.0) {
        sSizeStr = "3 1/2";
    }
    else if (dSize == 100.0) {
        sSizeStr = "4";
    }
    else if (dSize == 125.0) {
        sSizeStr = "5";
    }
    else if (dSize == 150.0) {
        sSizeStr = "6";
    }
    // 2020.09.16 S���a�ǉ�_S
    else if (dSize == 0.37) {
        sSizeStr = "3/8";
    }
    else if (dSize == 0.5) {
        sSizeStr = "1/2";
    }
    else if (dSize == 0.75) {
        sSizeStr = "3/4";
    }
    else if (dSize == 1.0) {
        sSizeStr = "1";
    }
    else if (dSize == 1.5) {
        sSizeStr = "1 1/2";
    }
    else if (dSize == 2.0) {
        sSizeStr = "2";
    }

    else if (dSize == 2.5) {
        sSizeStr = "2 1/2";
    }
    else if (dSize == 3.0) {
        sSizeStr = "3";
    }
    else if (dSize == 4.0) {
        sSizeStr = "4";
    }
    // 2020.09.16 S���a�ǉ�_E
    return sSizeStr;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �t�������a�\�L�ϊ�
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F �ϊ�����
// ��  �l      �F 2008.06.26�ǉ�
// ---------------------------------------------------------------------------
AnsiString __fastcall TNozzleDetails::ChgNozzleSize(double dSize) {
    AnsiString sSizeStr;
    int i_Kokei;

    sSizeStr = "";
    if (dSize == 0.75) {
        sSizeStr = "3/4";
    }
    else {
        i_Kokei = (int)dSize;
        sSizeStr = i_Kokei;
    }
    return sSizeStr;
}
