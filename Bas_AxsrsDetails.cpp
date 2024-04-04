// ---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�F�t���i���i�W�J����  ���W���[��
// �� �� �ҁFT.Kawai
// �� �� ���F2002.08.17
// �X �V ���F2002.09.10
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

#include "Bas_AxsrsDetails.h"

#include "HeadsData.h"

#include <math.h>       //2006.02.06 �؂�グ

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// �t���i���i�W�J���� Class
// ---------------------------------------------------------------------------
class TAxsrsDetails {
private:
    FILE *fp;

    // CSV̧�ق���擾
    AnsiString P_Sikibetu; // ���ʎq
    AnsiString P_Katasiki; // ���i�^��
    AnsiString P_Yobihin; // �\���i��
    AnsiString P_Hosoku; // ���̕⑫

    // Ͻ�����擾
    AnsiString P_Zaisitu; // �ގ�����
    AnsiString P_EZaisitu; // �ގ�����
    AnsiString P_Zuban; // �}��
    AnsiString P_SZuban; // ��}�p�}��
    int P_Num; // ����
    int P_Yobisu; // �\����
    AnsiString P_Siyou; // �d�l
    AnsiString P_Siyou2; // �d�l2
    AnsiString P_Siyou5; // �d�l5
    AnsiString P_Unit; // �P�ʁi���j

    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
    AnsiString P_GasNameB;  // �a�V�[�g�K�X�P�b�g����
    AnsiString P_GasNameA;  // �`�V�[�g�K�X�P�b�g����
    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

    // ------------------------------
    // 2007.10.03 �װ�d�l�Ή�
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // 2007.10.03
    // ------------------------------

    bool __fastcall Axsrs_Data_Sakusei(void); // �t���i�ް� �쐬
    bool __fastcall Get_Drpn_Data(AnsiString); // ��������ް��擾
    bool __fastcall Get_Anch_Data(void); // �ݶ��ް��擾
    bool __fastcall Get_Spana_Data(void); // �����ް��擾
    // 2014/08/22 ���p�Z�p�ҏW
    bool __fastcall Get_Spana_Data(AnsiString BoltSize); // �����ް��擾 (�{���g�T�C�Y���w��)
    bool __fastcall Get_HMark_Data(void); // ����}�[�N�擾
    bool __fastcall Get_Meiban_Data(void); // ���擾
    // 2012.10.15 �{���g�������ӃX�e�b�J�[�ǉ�
    bool __fastcall Get_Rakka_Data(void); // �{���g�������ӃX�e�b�J�[
    // ***************************************
    bool __fastcall Get_Fune_Data(bool); // ���p�t���i�ް��擾
    // 2004.02.24
    bool __fastcall Get_BFRG_Data(void); // ��ײ�����ݼ��ް��擾
    // **********
    // 2020.04.23  ���p�ȊO��ڰ�_S�@
    bool __fastcall Get_Strainer_Data(void); // ��ڰ��ް��擾
    // 2020.04.23  ���p�ȊO��ڰ�_E
    // 2014.12.22 �o�חp�~�t�����W�ǉ�
    bool __fastcall Get_TFLG_Data(void); // �o�חp�~��ݼ��ް��擾
    bool __fastcall TAxsrsDetails::Search_HD_TEMPORARY(AnsiString Tugite,
        AnsiString Kokei, AnsiString *s_DNO, AnsiString *s_SPEC1,
        AnsiString *s_SPEC2); // �o�חp�~��ݼނ�Ͻ�����擾
    int __fastcall TAxsrsDetails::Get_TMGASKET(AnsiString Tugite,
        AnsiString Syurui, int Fl); // �o�חp�~��ݼ޶޽��ėL��
    // *********************************
    // AnsiString __fastcall Search_HD_MATERIAL(AnsiString Key);            // �ގ����ތ���
    bool __fastcall Write_Funeyou_Data(AnsiString s_Code, AnsiString s_Spec);
    bool __fastcall Write_Axsrs_Data(AnsiString a_SE);
    bool __fastcall Get_IFrng_Data(void);
    // 2004.02.24
    bool __fastcall Get_STGkt_Data(void);
    // **********
    bool __fastcall EtcData_Write(void);
    bool __fastcall Write_Err_Data(void);
    bool __fastcall Get_IFrgBltSize(AnsiString sKikaku, AnsiString *s_retSize,
        int *iBltNum);
    bool __fastcall Get_SSTDBltSpec(AnsiString sKatasiki, int iFrgDiv,
        AnsiString *sSBLTSpec);

    // 2015.03.19 OGI ���p���i�A�L���[�����[�^�o�͑Ή�
    bool __fastcall Search_HD_FRM_NZ_Accum(AnsiString Key, AnsiString Type, int Code);

    //2017.10.10 ��Ķ޽��čގ��ύX_S
    AnsiString __fastcall Get_SheetGasketMat(AnsiString s_Side);
    //2017.10.10 ��Ķ޽��čގ��ύX_E

    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
    bool __fastcall Get_SeatGas_Name(void); // �V�[�g�K�X�P�b�g���̎擾
    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // �t���i���i�W�J���� ���s
};

// ---------------------------------------------------------------------------
// ���{��֐����F �t���i���i�W�J����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall AxsrsDetails(HeadsData* pcHeadsData) {
    bool ret;

    TAxsrsDetails axsrsDetails; // �t���i�ް� �쐬Class��������,�֐��I����j��

    ret = axsrsDetails.Excute(pcHeadsData); // �t���i�ް� �쐬���s

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �t���i���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADS÷���ް����߲�����
    m_pcHeadsData = pcHeadsData;

    // HEADS÷���ް���AnsiString�z��Ɏ擾
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********     �w�t���i���i�W�J�����x�J�n     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     �w�t���i���i�W�J�����x�J�n     *********");
    Write_Error_Log("*******************************************************");

    // ***********************************
    // ***  �t���i�ް��쐬
    // ***********************************
    Axsrs_Data_Sakusei();

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********     �w�t���i���i�W�J�����x�I��     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     �w�t���i���i�W�J�����x�I��     *********");
    Write_Error_Log("*******************************************************");

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �t���i�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Axsrs_Data_Sakusei(void) {
    bool ret;
    int i_Pos;
    AnsiString s_Drp_Syurui; // ������ݎ��
    AnsiString s_Ank_Syurui; // ������ݎ��
    ret = true;

    // ***********************************
    // ***  ��������ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   ��������ް�  �쐬�J�n   ----------";
    Write_Log(G_Log_Text);

    // s_Drp_Syurui = P_HEADS_DATA[264].TrimRight();   // ������ݎ��

    // 2020.10.29 ������ݒf�M�ޒǉ�_E
    //// 2018.04.04 ������ݒf�M�ޒǉ�_S
    ////if (P_HEADS_DATA[264].TrimRight() != "1.00") {
    //if ((P_HEADS_DATA[264].TrimRight() != "1.00") &&
    //    (P_HEADS_DATA[264].TrimRight() != "2.00")) {
    //// 2018.04.04 ������ݒf�M�ޒǉ�_E
    if ((P_HEADS_DATA[264].TrimRight() != "1.00") &&
        (P_HEADS_DATA[264].TrimRight() != "2.00") &&
        (P_HEADS_DATA[264].TrimRight() != "3.00")) {
    // 2020.10.29 ������ݒf�M�ޒǉ�_E
        G_Log_Text = "��������ް�  ������ݗL�����" + s_Drp_Syurui + "��̈׍쐬���܂���B";
        Write_Log(G_Log_Text);
    }
    else {
        // �t���[���^���Ō������āA�Y��������΁u�i�v�u�o�v�Ō�������
        if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
            if (Get_Drpn_Data(P_HEADS_DATA[36].TrimRight()) == false) {
                if ((P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
                    1) == "J") ||
                    (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 2,
                    1) == "J")) {
                    Get_Drpn_Data("J");
                }
                else {
                    Get_Drpn_Data("P");
                }
            }
        }
        else {
            if (Get_Drpn_Data(P_HEADS_DATA[36].TrimRight()) == false) {
                if ((P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
                    1) == "J") ||
                    (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
                    1) == "J")) {
                    Get_Drpn_Data("J");
                }
                else {
                    Get_Drpn_Data("P");
                }
            }
        }
    }
    G_Log_Text = "----------   ��������ް�  �쐬�I��   ----------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  �ݶ������ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   �ݶ������ް� �쐬�J�n   ----------";
    Write_Log(G_Log_Text);

    Get_Anch_Data();

    G_Log_Text = "----------   �ݶ������ް� �쐬�I��   ----------";
    Write_Log(G_Log_Text);

    // 2015.03.23 OGI �t���i���Ɂu���`�F�b�g�X�p�i�v�o�͑Ή�
    // 0�F�X�p�i�L�A���p�p��L
    // 1�F�X�p�i���A���p�p�
    // 2�F�X�p�i�L�A���p�p�
    // 3�F�X�p�i���A���p�p��L     //
    //if (P_HEADS_DATA[262].ToIntDef(0) == 0) {
     if ( (P_HEADS_DATA[262].ToIntDef(0) == 0) || (P_HEADS_DATA[262].ToIntDef(0) == 2) ) {
        // ***********************************
        // ***  �����ް��@�쐬
        // ***********************************
        G_Log_Text = "----------   �����ް� �쐬�J�n   ----------";
        Write_Log(G_Log_Text);

        Get_Spana_Data();

        G_Log_Text = "----------   �����ް� �쐬�I��   ----------";
        Write_Log(G_Log_Text);
    }
    // ***********************************
    // ***  ����}�[�N�ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   ����}�[�N �쐬�J�n   ----------";
    Write_Log(G_Log_Text);

    Get_HMark_Data();

    G_Log_Text = "----------   ����}�[�N �쐬�I��   ----------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  �����ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   ����}�[�N �쐬�J�n   ----------";
    Write_Log(G_Log_Text);

    Get_Meiban_Data();

    G_Log_Text = "----------   ����}�[�N �쐬�I��   ----------";
    Write_Log(G_Log_Text);

    // 2012.10.15 �{���g�������ӃX�e�b�J�[�ǉ�
    // ***********************************
    // ***  ���ė������ӽï���ް��@�쐬
    // ***********************************
    G_Log_Text = "-------   ���ė������ӽï�� �쐬�J�n   ------";
    Write_Log(G_Log_Text);

    Get_Rakka_Data();

    G_Log_Text = "-------   ���ė������ӽï�� �쐬�I��   ------";
    Write_Log(G_Log_Text);
    // ***************************************

    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
    // ***********************************
    // ***  �V�[�g�K�X�P�b�g���́@�擾
    // ***********************************
    G_Log_Text = "-----   �V�[�g�K�X�P�b�g���� �擾�J�n   -----";

    Write_Log(G_Log_Text);

    Get_SeatGas_Name();

    G_Log_Text = "-----   �V�[�g�K�X�P�b�g���� �擾�I��   -----";
    Write_Log(G_Log_Text);
    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

    // ***********************************
    // ***  ���t�����W�ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   ���t�����W �쐬�J�n   ----------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos == 0) { // ���p�ȊO
        Get_IFrng_Data();
    }
    G_Log_Text = "----------   ���t�����W �쐬�I��   ----------";
    Write_Log(G_Log_Text);

    // 2004.03.16 ��ײ�����ݼޒǉ�
    // ***********************************
    // ***  ��ײ�����ݼ��ް��@�쐬
    // ***********************************
    G_Log_Text = "---------   ��ײ�����ݼ� �쐬�J�n   ---------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos == 0) { // ���p�ȊO
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[265].TrimRight() != "0000")
        //{ // ��ײ�����ݼލގ����O�O�O�O�ȊO�̏ꍇ
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-") {
            // ��ײ�����ݼލގ����O�O�O�O�ȊO�̏ꍇ
        // ***********************
            Get_BFRG_Data();
        }
    }
    G_Log_Text = "---------   ��ײ�����ݼ� �쐬�I��   ---------";
    Write_Log(G_Log_Text);
    // *************************

    // 2004.02.04 ����߯�ݸޒǉ�
    // ***********************************
    // ***  ����߯�ݸ��ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   ����߯�ݸ� �쐬�J�n   ----------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos == 0) { // ���p�ȊO
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1068].TrimRight() == "0000") { // ���t�����W�ގ����O�O�O�O�̏ꍇ
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "-") {
            // ���t�����W�ގ����O�O�O�O�̏ꍇ
        // ***********************
            Get_STGkt_Data();
        }
    }
    G_Log_Text = "----------   ����߯�ݸ� �쐬�I��   ----------";
    Write_Log(G_Log_Text);
    // *************************

    // 2020.04.23 ���p�ȊO��ڰŒǉ�_S
    // ***********************************
    // ***  ���p�ȊO��ڰŁ@�쐬
    // ***********************************
    G_Log_Text = "---------   ���p�ȊO��ڰ� �쐬�J�n   ---------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos == 0) { // ���p�ȊO
        if (Type_MAT(P_HEADS_DATA[261].TrimRight()) != "-") {
            // ��ڰōގ����O�O�O�O�ȊO�̏ꍇ
            Get_Strainer_Data();
        }
    }
    G_Log_Text = "---------   ���p�ȊO��ڰ� �쐬�I��   ---------";
    Write_Log(G_Log_Text);
    // 2020.04.23 ���p�ȊO��ڰŒǉ�_E

    // ***********************************
    // ***  ���p���i�ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   �D�p���i�ް� �쐬�J�n   ----------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos != 0) {
        Get_Fune_Data(true);
    } // else {
    // Get_Fune_Data(false);
    // G_Log_Text = "RecNo:543��ڰѣ�ɢM����܂܂�Ȃ��̂ŁA���p���i�ް��͍쐬���܂���B";
    // Write_Log(G_Log_Text);
    // }

    G_Log_Text = "----------   �D�p���i�ް� �쐬�I��   ----------";
    Write_Log(G_Log_Text);

    // 2014.12.22 �o�חp�~�t�����W�ǉ�
    // ***********************************
    // ***  �o�חp�~��ݼ��ް��@�쐬
    // ***********************************
    G_Log_Text = "----------   �o�חp�~��ݼ��ް� �쐬�J�n   ----------";
    Write_Log(G_Log_Text);
    Get_TFLG_Data();
    G_Log_Text = "----------   �o�חp�~��ݼ��ް� �쐬�I��   ----------";
    Write_Log(G_Log_Text);
    // *********************************

    EtcData_Write();
    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ������ݕ��i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Drpn_Data(AnsiString aFrmType) {
    AnsiString s_Code; // ���i����
    AnsiString s_Key;
    AnsiString s_SQL;
    AnsiString s_Text;
    bool bRet;

    bRet = false;
    // 2009.09.04 �������Ͻ��ł͂Ȃ�P_HEADS_DATA�����ް������߂�
    // (����w�����ɂ��킹��)
    // TQuery *wkQuery;
    // wkQuery = new TQuery(DataModule1);
    //
    // // �����L�[�ύX 02.08.30
    // //s_Key = G_K_Syurui + "-" + G_K_Model1;
    // s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
    //
    // //*********************************************
    // //***  �������Ͻ�(HD_DR_PAN) ����
    // //*********************************************
    // G_Log_Text = "�������Ͻ�(HD_DR_PAN)�������L�[  �w" + s_Key + "�x�Ō����B";
    // Write_Log(G_Log_Text);
    //
    // s_SQL = "";
    // s_SQL = s_SQL + "SELECT * FROM HD_DR_PAN";
    // s_SQL = s_SQL + "  WHERE PLATE_MODEL = '" + s_Key + "'";
    // s_SQL = s_SQL + "    AND FRAME_TYPE = '" + aFrmType + "'";
    //
    // wkQuery->Close();
    // wkQuery->DatabaseName = ODBC_DSN_DNO;
    // wkQuery->SQL->Clear();
    // wkQuery->SQL->Add(s_SQL);
    // wkQuery->Open();
    // wkQuery->First();
    //
    // //if (!wkQuery->Eof) {
    // if (wkQuery->RecordCount == 0){
    // G_ErrLog_Text = "�ް��쐬���@ �v���[�g�^���" + s_Key +"�+�" + aFrmType + "� �́A�������Ͻ�(HD_DR_PAN)�ɑ��݂��܂���B";
    // Write_Error_Log(G_ErrLog_Text);
    // }
    //
    // while(!wkQuery->Eof)
    // {
    // // ������ݍގ����ނ̎擾
    // // ���i����
    // s_Code    = wkQuery->FieldByName("DR_PAN_MAT_CODE")->AsString.TrimRight();
    // P_Sikibetu = "DRPN--";                                            // ���ʎq
    // P_Katasiki = "�h�����p��";                                        // ���i�^��
    // P_Zuban    = wkQuery->FieldByName("DNO")->AsString.TrimRight();   // �}��
    // P_SZuban   = wkQuery->FieldByName("DNO")->AsString.TrimRight();   // ��}�p�}��
    // P_Num      = 1;
    // P_Unit     = "";
    // P_Hosoku   = "";
    //
    //
    // //*********************************
    // //***  �ގ�����Ͻ�����
    // //*********************************
    // P_Zaisitu = Search_HD_MATERIAL(s_Code);
    // P_EZaisitu =Search_HD_E_MATERIAL(s_Code);
    //
    // // �\���i��
    // P_Yobisu = 0;
    // // �d�l�P
    // //2008.03.31 ������ݎd�l�ύX
    // ////2003.09.08 �d�l�ύX
    // ////P_Siyou = "�k�P��"+ HanToZen( P_HEADS_DATA[1293].TrimRight() )+
    // ////          " �k�Q��"+ HanToZen( P_HEADS_DATA[1294].TrimRight() )+
    // ////          " �k�R��"+ HanToZen( P_HEADS_DATA[1295].TrimRight() );
    // //s_Text = P_HEADS_DATA[1294].TrimRight();  //�k�Q���@
    // //if (s_Text == "0") {
    // //   P_Siyou = "�k�P��"+ HanToZen( P_HEADS_DATA[1293].TrimRight() );
    // //} else {
    // //   P_Siyou = "�k�P��"+ HanToZen( P_HEADS_DATA[1293].TrimRight() )+
    // //             " �k�Q��"+ HanToZen( P_HEADS_DATA[1294].TrimRight() )+
    // //             " �k�T��"+ HanToZen( P_HEADS_DATA[1295].TrimRight() );
    // //}
    // //�k�P���@
    // P_Siyou = "�k�P��"+ HanToZen( P_HEADS_DATA[1293].TrimRight() );
    // //�k�Q���@
    // s_Text = P_HEADS_DATA[1294].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " �k�Q��"+ HanToZen( P_HEADS_DATA[1294].TrimRight() );
    // }
    // //�k�R���@
    // s_Text = P_HEADS_DATA[871].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " �k�R��"+ HanToZen( P_HEADS_DATA[871].TrimRight() );
    // }
    // //�k�S���@
    // s_Text = P_HEADS_DATA[872].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " �k�S��"+ HanToZen( P_HEADS_DATA[872].TrimRight() );
    // }
    // //�k�T���@
    // s_Text = P_HEADS_DATA[1295].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " �k�T��"+ HanToZen( P_HEADS_DATA[1295].TrimRight() );
    // }
    // //�k�U���@
    // s_Text = P_HEADS_DATA[873].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " �k�U��"+ HanToZen( P_HEADS_DATA[873].TrimRight() );
    // }
    // //�k�V���@
    // s_Text = P_HEADS_DATA[874].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " �k�V��"+ HanToZen( P_HEADS_DATA[874].TrimRight() );
    // }
    // //*******************
    // //**************************
    //
    // P_Siyou5 = "";
    // // ���O
    // G_Log_Text = "������ݍގ�����    �" + s_Code + "����擾�B";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
    // Write_Log(G_Log_Text);
    //
    // //*********************************
    // //***  �t���i�ް�(�������)�@�쐬
    // //*********************************
    // Write_Axsrs_Data(s_Code);
    //
    // wkQuery->Next();
    // bRet = true;
    // }
    //
    // delete wkQuery;

    // ***************************
    // ***  ������ݐ}��
    // ***************************
    // 2020.10.28 ������ݒf�M�ޒǉ�_S
    //// 2018.04.04 ������ݒf�M�ޒǉ�_S
    ////if (P_HEADS_DATA[264].TrimRight() != "1.00") {
    //if ((P_HEADS_DATA[264].TrimRight() != "1.00") &&
    //    (P_HEADS_DATA[264].TrimRight() != "2.00")) {
    //// 2018.04.04 ������ݒf�M�ޒǉ�_E
    if ((P_HEADS_DATA[264].TrimRight() != "1.00") &&
        (P_HEADS_DATA[264].TrimRight() != "2.00") &&
        (P_HEADS_DATA[264].TrimRight() != "3.00")) {
    // 2020.10.28 ������ݒf�M�ޒǉ�_E
        // ������� ���Ȃ����ߋL�q���܂���
    }
    else if (P_HEADS_DATA[1330].Trim() == "") {
        // �}�Ԃ��Ȃ����ߋL�q���܂���
    }
    else {

        // ������ݍގ����ނ̎擾
        // ���i����
        s_Code = "1001";
        P_Sikibetu = "DRPN--"; // ���ʎq
        P_Katasiki = "�h�����p��"; // ���i�^��
        s_Text = P_HEADS_DATA[1330].Trim();
        s_Text = s_Text.SubString(15, s_Text.Length() - 14);
        s_Text = s_Text.SubString(1, s_Text.Pos("�@L") - 1);
        P_Zuban = s_Text; // �}��
        P_SZuban = s_Text; // ��}�p�}��
        P_Num = 1;
        P_Unit = "";
        P_Hosoku = "";

        // *********************************
        // ***  �ގ�����Ͻ�����
        // *********************************
        P_Zaisitu = Search_HD_MATERIAL(s_Code);
        P_EZaisitu = Search_HD_E_MATERIAL(s_Code);

        // �\���i��
        P_Yobisu = 0;
        // �d�l�P
        // �k�P���@
        P_Siyou = "�k�P��" + HanToZen(P_HEADS_DATA[1293].TrimRight());
        // �k�Q���@
        s_Text = P_HEADS_DATA[874].TrimRight();
        if (s_Text == "0") {
            // ������ݐ��@��4�܂ŋL�q����
            // �k�V���@�����݂���ꍇ�A�k�Q���@�͋L�q���Ȃ��B
            s_Text = P_HEADS_DATA[1294].TrimRight();
            if (s_Text != "0") {
                P_Siyou = P_Siyou + " �k�Q��" +
                    HanToZen(P_HEADS_DATA[1294].TrimRight());
            }
        }
        // �k�R���@(���L�q)
        // s_Text = P_HEADS_DATA[871].TrimRight();
        // if (s_Text != "0") {
        // P_Siyou = P_Siyou + " �k�R��"+ HanToZen( P_HEADS_DATA[871].TrimRight() );
        // }
        // �k�S���@(���L�q)
        // s_Text = P_HEADS_DATA[872].TrimRight();
        // if (s_Text != "0") {
        // P_Siyou = P_Siyou + " �k�S��"+ HanToZen( P_HEADS_DATA[872].TrimRight() );
        // }
        // �k�T���@
        s_Text = P_HEADS_DATA[1295].TrimRight();
        if (s_Text != "0") {
            P_Siyou = P_Siyou + " �k�T��" +
                HanToZen(P_HEADS_DATA[1295].TrimRight());
        }
        // �k�U���@
        s_Text = P_HEADS_DATA[873].TrimRight();
        if (s_Text != "0") {
            P_Siyou = P_Siyou + " �k�U��" +
                HanToZen(P_HEADS_DATA[873].TrimRight());
        }
        // �k�V���@
        s_Text = P_HEADS_DATA[874].TrimRight();
        if (s_Text != "0") {
            P_Siyou = P_Siyou + " �k�V��" +
                HanToZen(P_HEADS_DATA[874].TrimRight());
        }
        P_Siyou5 = "";
        // 2020/10/22 ������ݒf�M�ޒǉ�_S
        //// 2018.04.04 ������ݒf�M�ޒǉ�_S
        //if (P_HEADS_DATA[264].TrimRight() == "2.00") {
        //// �f�M�ޕt
        //    P_Katasiki = "�h�����p��(�f�M�ޕt)"; // ���i�^��
        //}
        //// 2018.04.04 ������ݒf�M�ޒǉ�_E
        if (P_HEADS_DATA[264].TrimRight() == "2.00") {
            // �f�M�ޕt
            P_Katasiki = "�h�����p��(�f�M�ޕt(�C��))"; // ���i�^��
        } else if (P_HEADS_DATA[264].TrimRight() == "3.00") {
            // �f�M�ޕt
            P_Katasiki = "�h�����p��(�f�M�ޕt(����F��i))"; // ���i�^��
        }
        // 2020/10/22 ������ݒf�M�ޒǉ�_E
        // ���O
        G_Log_Text = "������ݍގ�����    �" + s_Code + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  �t���i�ް�(�������)�@�쐬
        // *********************************
        Write_Axsrs_Data(s_Code);

        bRet = true;
    }

    // **********************************************************
    return bRet;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �ݶ����ĕ��i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Anch_Data(void) {
    AnsiString s_Code; // ���i����
    AnsiString s_Key;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;
    AnsiString s_Text;
    AnsiString s_Syubetu;
    // 2006.02.06 �ϐ��ǉ�
    AnsiString s_Text2; // �ėp������Q
    // *******************
    // 2012.01.20 �����ގ�
    AnsiString s_ChinaM;
    int i_RecNo;
    // *******************

    // ***************************
    // ***  �ݶ����ā@����
    // ***************************
    // �wC026�x
    // ��ʺ��ނ̎擾
    s_Text = P_HEADS_DATA[1045].TrimRight(); // SPANC
    // 2022.09.21 �h�����p���p�A���J�[DB��`�ύX_S
    //if (s_Text == "00" || s_Text == "01" || s_Text == "06" || s_Text == "07") {
    //    s_Text = P_HEADS_DATA[264].TrimRight(); // ������ݗL��
    //    if (s_Text == "1.00") {
    //        s_Syubetu = "1";
    //    }
    //    else {
    //        s_Syubetu = "0";
    //    }
    //    // 2003.12.22 ΰٲ݂�SUS�ɂ��敪�̍폜
    //    // } else if (s_Text == "02" ) {
    //    // s_Text = P_HEADS_DATA[37].TrimRight();   // �ڰь^������
    //    // switch (s_Text.ToIntDef(0)){
    //    // case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
    //    // s_Syubetu = "1";
    //    // break;
    //    // default:
    //    // s_Text = P_HEADS_DATA[1048].TrimRight();  // SPANM
    //    // if (s_Text == "1001") {
    //    // s_Syubetu = "1";
    //    // } else {
    //    // s_Syubetu = "0";
    //    // }
    //    // }
    //    // *************************************
    //}
    if (s_Text == "00" || s_Text == "01" || s_Text == "06" || s_Text == "07" ||
        s_Text == "11") {
        s_Text = P_HEADS_DATA[264].TrimRight(); // ������ݗL��
        if (s_Text == "1.00" || s_Text == "2.00" || s_Text == "3.00" ) {
            s_Syubetu = "1";
        }
        else {
            s_Syubetu = "0";
        }
    }
    // 2022.09.21 �h�����p���p�A���J�[DB��`�ύX_E
    else {
        s_Syubetu = "0";
    }

    // �ݶ����Ē�Ͻ� ����
    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_ANCHOR_SIZE";
    // 2012.06.08 �ݶ����Č��������ύX(RX-10-EP)
    // s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "'";
    switch (P_HEADS_DATA[37].ToIntDef(0)) { // �t���[���R�[�h
    case 71:
    case 72:
    case 81:
    case 91:
    case 381:
    case 391: // �d�o�V���[�Y
        s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
        s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "E'";
        break;
    //2017.04.20 Z�ڰђǉ�
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 334:
    case 335:
    case 336:
    case 337:
    case 338:
    case 344:
    case 345:
    case 346:
    case 347:
    case 348:  // �y�t���[��
        s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
        s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "Z'";
        break;
    //********************
    //2023.04.05 Y�ڰђǉ�_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
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
    case 354:  // �x�t���[��
        s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
        s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "Y'";
        break;
    //2023.04.05 Y�ڰђǉ�_E
    default:
        s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
        s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "'";
        break;
    }
    // *****************************************
    s_SQL = s_SQL + "    AND ANCHOR_STAT = " + s_Syubetu + "";
    s_SQL = s_SQL + "    AND ABCHOR_TYPE = " + P_HEADS_DATA[1045].TrimRight
        () + ";";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    P_Siyou5 = "";
    if (wkQuery->Eof) {
        s_Text = "";
    }
    else {
        P_Sikibetu = "ACBL--"; // ���i����
        // 2003.12.22 ΰٲ݂�BOLT_TYPE�̒����ɂ��ύX�̒ǉ�
        ////2003.12.05 �ж٤ΰٲ݂̒�������͂ł���悤�ɂ���
        ////if ( P_HEADS_DATA[1045].TrimRight() != "03" && P_HEADS_DATA[1045].TrimRight() != "02" &&
        ////     P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047].TrimRight() != "" ) {
        // if ( P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047].TrimRight() != "" ) {
        ////*************************************************
        if (P_HEADS_DATA[1045].TrimRight() == "02") {
            // 2004.03.15 ΰٲݒ����ǉ�
            if (P_HEADS_DATA[1047].ToIntDef(0) == 60) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "60";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 70) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "70";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 80) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "80";
                // 2004.04.27 ΰٲݒ����ǉ�
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 90) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "90";
                // ************************
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 100) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "10";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 120) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "12";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 130) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "13";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 150) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "15";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 190) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "19";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 230) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "23";
            }
            else if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
                .TrimRight() != "") {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "�~" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "�k " + wkQuery->FieldByName("BOLT_TYPE")
                    ->AsString.TrimRight();
            }
            else {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight());
                if (P_Siyou == "�l�P�Q") {
                    P_Siyou =
                        P_Siyou + "�~" +
                        HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight()) + "�k " + wkQuery->FieldByName
                        ("BOLT_TYPE")->AsString.TrimRight() + "80";
                }
                else {
                    P_Siyou =
                        P_Siyou + "�~" +
                        HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight()) + "�k " + wkQuery->FieldByName
                        ("BOLT_TYPE")->AsString.TrimRight() + "13";
                }
            }
            ////if ( P_HEADS_DATA[1047].ToIntDef(0) == 100 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "�~" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "�k "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "10";
            ////} else if ( P_HEADS_DATA[1047].ToIntDef(0) == 130 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "�~" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "�k "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "13";
            ////} else if ( P_HEADS_DATA[1047].ToIntDef(0) == 150 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "�~" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "�k "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "15";
            ////} else if ( P_HEADS_DATA[1047].ToIntDef(0) == 190 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "�~" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "�k "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "19";
            ////} else if ( P_HEADS_DATA[1047].ToIntDef(0) == 230 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "�~" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "�k "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "23";
            ////} else if ( P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047].TrimRight() != "" ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "�~" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "�k "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
            ////} else {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "�~" + HanToZen(wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight()) + "�k "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "13";
            ////}
            // ************************
            // 2006.02.06 �жٱݶ���BOLT_TYPE�̒����ɂ��ύX�̒ǉ�
        }
        else if (P_HEADS_DATA[1045].TrimRight() == "03") {
            //2015.11.19 �P�~�J���A���J�[�S�ʕύX
            //s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
            //if (s_Text == "AP-20") {
            //    if (P_HEADS_DATA[1047].ToIntDef(0) == 0) {
            //        s_Text = "AP-20";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 180) {
            //        s_Text = "AP-20S";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 250) {
            //        s_Text = "AP-20";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 300) {
            //        s_Text = "AP-20L";
            //    }
            //    else {
            //        s_Text = "";
            //    }
            //}
            //else if (s_Text == "AP-24") {
            //    if (P_HEADS_DATA[1047].ToIntDef(0) == 0) {
            //        s_Text = "AP-24";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 270) {
            //        s_Text = "AP-24S";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 375) {
            //        s_Text = "AP-24";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 420) {
            //        s_Text = "AP-24L";
            //    }
            //    else {
            //        s_Text = "";
            //    }
            //}
            //else if (s_Text == "AP-30") {
            //    if (P_HEADS_DATA[1047].ToIntDef(0) == 0) {
            //        s_Text = "AP-30";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 310) {
            //        s_Text = "AP-30S";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 430) {
            //        s_Text = "AP-30";
            //    }
            //    else {
            //        s_Text = "";
            //    }
            //    // 2012.07.03 AP-12�ǉ�
            //}
            //else if (s_Text == "AP-12") {
            //    if (P_HEADS_DATA[1047].ToIntDef(0) == 0) {
            //        s_Text = "AP-12";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 90) {
            //        s_Text = "AP-12S";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 130) {
            //        s_Text = "AP-12";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 250) {
            //        s_Text = "AP-12L";
            //    }
            //    else {
            //        s_Text = "";
            //    }
            //    // ********************
            //}
            //if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
            //    .TrimRight() != "") {
            //    if (s_Text == "AP-20L" && P_HEADS_DATA[1047].ToIntDef(0) >
            //        290 && P_HEADS_DATA[1047].ToIntDef(0) <= 295) {
            //        s_Text2 = "295";
            //    }
            //    else if (s_Text == "AP-24" && P_HEADS_DATA[1047].ToIntDef(0) >
            //        370 && P_HEADS_DATA[1047].ToIntDef(0) <= 375) {
            //        s_Text2 = "375";
            //    }
            //    else {
            //        s_Text2 =
            //            FormatFloat("0",
            //            ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
            //    }
            //    P_Siyou =
            //        HanToZen(wkQuery->FieldByName("BOLT_SIZE")
            //        ->AsString.TrimRight()) + "�~" + HanToZen(s_Text2) +
            //        "�k " + s_Text;
            //}
            //else {
            //    P_Siyou =
            //        HanToZen(wkQuery->FieldByName("BOLT_SIZE")
            //        ->AsString.TrimRight()) + "�~" +
            //        HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
            //        ->AsString.TrimRight()) + "�k " + s_Text;
            //}
            //// ****************************************************
            s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
            if (s_Text == "HP-12") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 71 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 250) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-20") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 141 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 350) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-24") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 371 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 375) {
                    s_Text2 =  "375";
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 450) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-30") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 500) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            }
            P_Siyou =
                HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                ->AsString.TrimRight()) + "�~" +
                HanToZen(s_Text2) + "�k " + s_Text;
            // **********************************
        }
        // 2022.10.06 �_�u���i�b�g�C��_S
        //else if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
        //    .TrimRight() != "") {
        //    // *************************************************
        //    P_Siyou =
        //        HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
        //        ) + "�~" + HanToZen
        //        (FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) + "�k " +
        //        wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
        //}
        // 2022.10.06 �_�u���i�b�g�C��_E
        // 2021.04.07 �_�u���i�b�g�ǉ�_S
        else if (P_HEADS_DATA[1045].TrimRight() == "11") {
            // �k�^�A���J�[�{���g�E�_�u���i�b�g
            if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
                .TrimRight() != "") {
                P_Siyou =
                    HanToZen
                    (wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()) +
                    "�~" + HanToZen
                    (FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) + "�k " +
                    wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() +
                    "�A�i�b�g�i�P��~�P�A�R��~�P�j";
            }
            else {
                P_Siyou =
                    HanToZen
                    (wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()) +
                    "�~" + HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                   ->AsString.TrimRight()) + "�k " + wkQuery->FieldByName
                    ("BOLT_TYPE")->AsString.TrimRight() +
                    "�A�i�b�g�i�P��~�P�A�R��~�P�j";
            }
        }
        else if (P_HEADS_DATA[1045].TrimRight() == "13") {
            // �P�~�J���A���J�[�E�_�u���i�b�g
            s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
            if (s_Text == "HP-12") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 71 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 250) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-20") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 141 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 350) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-24") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 450) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-30") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 500) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            }
            P_Siyou =
                HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                ->AsString.TrimRight()) + "�~" +
                HanToZen(s_Text2) + "�k " + s_Text +
                "�A�i�b�g�i�P��~�P�A�R��~�P�j";
        }
        else if (P_HEADS_DATA[1045].TrimRight() == "15") {
            // �Z�b�g�{���g�E�_�u���i�b�g
            if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
                .TrimRight() != "") {
                P_Siyou =
                    HanToZen
                    (wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()) +
                    "�~" + HanToZen
                    (FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) + "�k " +
                    wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() +
                    "�A�i�b�g�i�P��~�P�A�R��~�P�j";
            }
            else {
                P_Siyou =
                    HanToZen
                    (wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()) +
                    "�~" + HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                   ->AsString.TrimRight()) + "�k " + wkQuery->FieldByName
                    ("BOLT_TYPE")->AsString.TrimRight() +
                    "�A�i�b�g�i�P��~�P�A�R��~�P�j";
            }
        }
        // 2021.04.07 �_�u���i�b�g�ǉ�_E
        // 2022.10.06 �_�u���i�b�g�C��_S
        else if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
            .TrimRight() != "") {
            P_Siyou =
                HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                ) + "�~" + HanToZen
                (FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) + "�k " +
                wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
        }
         // 2022.10.06 �_�u���i�b�g�C��_E
        else {
            P_Siyou =
                HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                ) + "�~" + HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                ->AsString.TrimRight()) + "�k " + wkQuery->FieldByName
                ("BOLT_TYPE")->AsString.TrimRight();
        }

        P_Siyou = ZenToHan(P_Siyou);
        // ***************************
        // ***  �ݶ����ā@�ގ�
        // ***************************
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1048].TrimRight() != "" && P_HEADS_DATA[1048].TrimRight
        //    () != "0000") {
        if (Type_MAT(P_HEADS_DATA[1048].TrimRight()) != "-") {
        // ***********************
            P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1048].TrimRight());
            // 2004.02.05
            // P_EZaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1048].TrimRight());
            P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[1048].TrimRight());
            // **********
        }
        else {
            s_Text = P_HEADS_DATA[1045].TrimRight();
            // 2021.04.07 �_�u���i�b�g�ǉ�_S
            //if (s_Text == "00" || s_Text == "01" || s_Text == "03" ||
            //    s_Text == "05" || s_Text == "06" || s_Text == "07") {
            if (s_Text == "00" || s_Text == "01" || s_Text == "03" ||
                s_Text == "05" || s_Text == "06" || s_Text == "07" ||
                s_Text == "11" || s_Text == "13" || s_Text == "15" ) {
            // 2021.04.07 �_�u���i�b�g�ǉ�_E
                switch (P_HEADS_DATA[37].ToIntDef(0)) {
                case 14:
                case 15:
                case 16:
                case 27:
                case 28:
                case 29:
                case 63:
                case 64:
                    P_Zaisitu = "�r�t�r�R�O�S";
                    P_EZaisitu = "�r�t�r�R�O�S";
                    break;
                default:
                    // 2016.11.07 �ގ����ޕύX
                    //if (P_HEADS_DATA[1048].TrimRight() == "1001") {
                    if (Type_MAT(P_HEADS_DATA[1048].TrimRight()) == "SUS304") {
                    // ***********************
                        P_Zaisitu = "�r�t�r�R�O�S";
                        P_EZaisitu = "�r�t�r�R�O�S";
                    }
                    else {
                        P_Zaisitu = "�r�r�S�O�O";
                        P_EZaisitu = "�r�r�S�O�O";
                    }
                    break;
                }
            }
            else if (s_Text == "02") {
                switch (P_HEADS_DATA[37].ToIntDef(0)) {
                case 14:
                case 15:
                case 16:
                case 27:
                case 28:
                case 29:
                case 63:
                case 64:
                    P_Zaisitu = "�r�t�r�R�O�S";
                    P_EZaisitu = "�r�t�r�R�O�S";
                    break;
                default:
                    // 2016.11.07 �ގ����ޕύX
                    //if (P_HEADS_DATA[1048].TrimRight() == "1001") {
                    if (Type_MAT(P_HEADS_DATA[1048].TrimRight()) == "SUS304") {
                    // ***********************
                        P_Zaisitu = "�r�t�r�R�O�S";
                        P_EZaisitu = "�r�t�r�R�O�S";
                    }
                    else {
                        // 2004.01.07 �ގ��ύX
                        // P_Zaisitu = "�b�D�r�D";
                        // P_EZaisitu = "�b�D�r�D";
                        P_Zaisitu = "�r�r�S�O�O";
                        P_EZaisitu = "�r�r�S�O�O";
                        // *******************
                    }
                    break;
                }

            }
            else if (s_Text == "04") {
                // 2004.01.19 �ގ��ύX
                // P_Zaisitu = "�r�c�Q�S�^�r�r�S�O�O";
                // P_EZaisitu = "�r�c�Q�S�^�r�r�S�O�O";
                // 2004.02.02 �ގ��ύX
                // P_Zaisitu = "�r�q�Q�R�T�^�r�r�S�O�O";
                // P_EZaisitu = "�r�q�Q�R�T�^�r�r�S�O�O";
                P_Zaisitu = "�r�c�R�S�T�^�r�r�S�O�O";
                P_EZaisitu = "�r�c�R�S�T�^�r�r�S�O�O";
                // *******************
                // *******************
            }
            else if (s_Text == "09") {
                P_Zaisitu = "";
                P_EZaisitu = "";
            }
            else {
                P_Zaisitu = "";
                P_EZaisitu = "";
            }
        }
        if (P_Zaisitu == "�r�t�r�R�O�S") {
            P_Zuban = wkQuery->FieldByName("DNO2")->AsString.TrimRight(); // �}��
            P_SZuban = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
            // ��}�p�}��
        }
        else {
            P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight(); // �}��
            P_SZuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
            // ��}�p�}��
        }
        P_Yobihin = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();
        // �\���i���ʎq
        P_Num = wkQuery->FieldByName("NUM")->AsString.ToIntDef(1); // �\���i���ʎq
        P_Unit = "��";
        P_Yobisu = Syukei_Buhin_Yobisu(ZenToHan(P_Yobihin), "0",
            m_pcHeadsData); // �\����

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
            if (P_Zaisitu == "�r�r�S�O�O") {
                P_Zaisitu = "�p�Q�R�T�a";
                P_EZaisitu = "�p�Q�R�T�a";
            }
        }
        // *******************

        // ***************************
        // ***  �ݶ����ā@���
        // ***************************
        s_Text = P_HEADS_DATA[1045].TrimRight();
        if (s_Text == "00" || s_Text == "01" || s_Text == "06" ||
            s_Text == "07") {
            P_Katasiki = "�A���J�[�{���g�E�i�b�g";
            P_Sikibetu = "ACBL-L"; // ���i����
        }
        else if (s_Text == "02") {
            P_Katasiki = "�z�[���C���A���J�[";
            P_Sikibetu = "ACBL-H"; // ���i����
        }
        else if (s_Text == "03") {
            P_Katasiki = "�P�~�J���E�A���J�[";
            P_Sikibetu = "ACBL-C"; // ���i����
        }
        else if (s_Text == "04") {
            P_Katasiki = "�t�V�t���{���g�E�i�b�g";
            P_Sikibetu = "ACBL-D"; // ���i����
        }
        else if (s_Text == "05") {
            P_Katasiki = "�Z�b�g�{���g�E�i�b�g";
            P_Sikibetu = "ACBL-S"; // ���i����
        }
        else if (s_Text == "09") {
            P_Katasiki = "";
        }
        // 2021.04.07 �_�u���i�b�g�ǉ�_S
        else if (s_Text == "11") {
            P_Katasiki = "�A���J�[�{���g�E�_�u���i�b�g";
            P_Sikibetu = "ACBLDL"; // ���i����
        }
        else if (s_Text == "13") {
            P_Katasiki = "�P�~�J���E�A���J�[�E�_�u���i�b�g";
            P_Sikibetu = "ACBLDC"; // ���i����
        }
        else if (s_Text == "15") {
            P_Katasiki = "�Z�b�g�{���g�E�_�u���i�b�g";
            P_Sikibetu = "ACBLDS"; // ���i����
        }
        // 2021.04.07 �_�u���i�b�g�ǉ�_E
        else {
            P_Katasiki = "";
        }
        if (s_Text == "00" || s_Text == "01") {
            P_Siyou += "  �k�^";
        }
        else if (s_Text == "06") {
            // 2014.09.22 �ݶ����ޕύX
            //P_Siyou += "  �i�^";
            //P_Sikibetu = "ACBL-J"; // ���i����
            P_Siyou += "  �i�`�^";
            P_Sikibetu = "ACBL-I"; // ���i����
            // ***********************
        }
        else if (s_Text == "07") {
            // 2014.09.22 �ݶ����ޕύX
            //P_Siyou += "  �i�`�^";
            //P_Sikibetu = "ACBL-D"; // ���i����
            P_Siyou += "  �i�^";
            P_Sikibetu = "ACBL-J"; // ���i����
            // ***********************
        }
        else if (s_Text == "04") {
            P_Siyou += "  �h�^";
        // 2021.04.07 �_�u���i�b�g�ǉ�_S
        }
        else if (s_Text == "11") {
            P_Siyou += "  �k�^";
        // 2021.04.07 �_�u���i�b�g�ǉ�_E
        }
        P_Hosoku = "";

        // ���O
        G_Log_Text = "�ݶ����čގ�����   �" + s_Code + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
        Write_Log(G_Log_Text);

        // 2012.09.20 �ݶ����Ďd��ǉ�
        // ***************************
        // ***  �ݶ����ā@�d��
        // ***************************
        s_Text = P_HEADS_DATA[1324].TrimRight();
        P_Siyou2 = s_Text;
        // ***************************

        // *********************************
        // ***  �t���i�ް�(�ݶ�����)�@�쐬
        // *********************************
        Write_Axsrs_Data(s_Code);

    }

    // //*********************************************
    // //***  �ݶ�����Ͻ�(HD_ANCHOR) ����
    // //*********************************************
    // s_Key = P_HEADS_DATA[166].TrimRight();   // �ݶ����ĺ���
    //
    // G_Log_Text = "�ݶ�����Ͻ�(HD_ANCHOR)�������L�[  �w" + s_Key + "�x�Ō����B";
    // Write_Log(G_Log_Text);
    //
    // s_SQL = "";
    // s_SQL = s_SQL + "SELECT * FROM HD_ANCHOR";
    // s_SQL = s_SQL + "  WHERE ANCHOR_CODE = '" + s_Key + "'";
    //
    // wkQuery->Close();
    // wkQuery->DatabaseName = ODBC_DSN_DNO;
    // wkQuery->SQL->Clear();
    // wkQuery->SQL->Add(s_SQL);
    // wkQuery->Open();
    // wkQuery->First();
    //
    // if (wkQuery->RecordCount == 0){
    // G_ErrLog_Text = "�ް��쐬��    �ݶ����Č^���" + s_Key +"� �́A�ݶ�����Ͻ�(HD_ANCHOR)�ɑ��݂��܂���B";
    // Write_Error_Log(G_ErrLog_Text);
    // }
    //
    // while(!wkQuery->Eof)
    // {
    // // �ގ����ނ̎擾
    // s_Code     = wkQuery->FieldByName("MAT_CODE")->AsString.TrimRight();    // �ݶ����čގ�����
    // P_Sikibetu = "ACBL--";                                                  // ���i����
    // P_Katasiki = wkQuery->FieldByName("PARTS_NAME")->AsString.TrimRight();  // ���i�^��
    // P_Zuban    = wkQuery->FieldByName("DNO")->AsString.TrimRight();         // �}��
    // P_SZuban   = wkQuery->FieldByName("DNO")->AsString.TrimRight();         // ��}�p�}��
    // P_Yobihin  = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();   // �\���i���ʎq
    //
    // //*********************************
    // //***  �ގ�����Ͻ�����
    // //*********************************
    // P_Zaisitu = Search_HD_MATERIAL(s_Code);
    //
    // //*********************************
    // //***  �\���i�� �擾
    // //*********************************
    // P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin,"0");      // �\����
    //
    // P_Siyou = P_HEADS_DATA[1324].TrimRight();       // �d�l�P
    //
    // // ���O
    // G_Log_Text = "�ݶ����čގ�����   �" + s_Code + "����擾�B";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
    // Write_Log(G_Log_Text);
    //
    // //*********************************
    // //***  �t���i�ް�(�ݶ�����)�@�쐬
    // //*********************************
    // Write_Axsrs_Data(s_Code);
    // wkQuery->Next();
    // }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ����Ľ��ŕ��i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
// 2014/08/22 ���p�Z�p�ҏW
bool __fastcall TAxsrsDetails::Get_Spana_Data(void) {
    // ���s���ʂ̕ێ�
    bool Result_BDI  = true;
    bool Result_BDIS = true;

    // �{���g�T�C�Y���w�肵�č쐬(BDI)
    AnsiString BoltSizeL = P_HEADS_DATA[1019].TrimRight();
    Result_BDI = Get_Spana_Data(BoltSizeL);

    // Z�t���[���̏ꍇ�ABDIS(���a�{���g)
    if(P_HEADS_DATA[36].Pos("Z") > 0) {
        // �{���g�T�C�Y���w�肵�č쐬(BDIS)
        AnsiString BoltSizeS = P_HEADS_DATA[1108].TrimRight();

        if(BoltSizeL != BoltSizeS) {
            // BDI�ƈقȂ�ꍇ�̂ݍ쐬
            Result_BDIS = Get_Spana_Data(BoltSizeS);
        }
    }

    if(Result_BDI  == false ||
       Result_BDIS == false) {
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ����Ľ��ŕ��i�W�J���� �{���g�T�C�Y���w�肵�č쐬
// �T  �v      �F
// ��  ��      �F AnsiString BoltSize : �{���g�T�C�Y
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
// 2014/08/22 ���p�Z�p�ҏW
bool __fastcall TAxsrsDetails::Get_Spana_Data(AnsiString BoltSize) {
    AnsiString s_Code; // ���i����
    AnsiString s_Key;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;

    // *********************************************
    // ***  ����Ͻ�(HD_ANCHOR) ����
    // *********************************************
    s_Key = BoltSize; // ���Ļ���
    if (s_Key == "") {
        return false;
    }
    if (int(s_Key.ToDouble() * 100) == 163) {
        s_Key = "1 5/8-UN";
    }
    else if (int(s_Key.ToDouble() * 100) == 200) {
        s_Key = "2-8UN";
    }
    else {
        s_Key = "M" + s_Key.SubString(1, 2);
    }

    G_Log_Text = "����Ͻ�(HD_SPANA)�������L�[  �w" + s_Key + "�x�Ō����B";
    Write_Log(G_Log_Text);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_SPANA";
    s_SQL = s_SQL + "  WHERE MODEL_CODE = '" + s_Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "�ް��쐬��    ���Ča�" + s_Key +
            "� �́A����Ͻ�(HD_ANCHOR)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        // �ގ����ނ̎擾
        P_Sikibetu = "RSPN--"; // ���i����
        P_Katasiki = "���`�F�b�g�X�p�i"; // ���i�^��
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // �}��
        P_SZuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // ��}�p�}��
        P_Yobihin = "SPNR01"; // �\���i���ʎq
        P_Siyou = wkQuery->FieldByName("SIZE")->AsString.TrimRight(); // �d�l
        P_Siyou2 = "";
        P_Siyou5 = "";
        P_Num = 1;
        P_Unit = "";
        P_Hosoku = "";

        // *********************************
        // ***  �ގ��Ͷ���ݽ��ٌŒ�
        // *********************************
        P_Zaisitu = "�b�D�r";
        P_EZaisitu = "�b�D�r";

        // *********************************
        // ***  �\���i�� �擾
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // �\����

        // ���O
        G_Log_Text = "�ݶ����čގ�����   �" + s_Code + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  �t���i�ް�(����Ľ���)�@�쐬
        // *********************************
        Write_Axsrs_Data(s_Code);
        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �����i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Meiban_Data(void) {
    AnsiString s_Code; // ���i����
    AnsiString s_Key;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;
    AnsiString s_MeibanName;

    P_Siyou = "";
    P_Siyou5 = "";
    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_MODEL_PL";
    // 2004.07.15 �����Ȗ��ɕύX
    // if (P_HEADS_DATA[268].TrimRight() == "1" ) {         // ���ݏȎd�l
    // if(P_HEADS_DATA[269].TrimRight() == "9"){          // ���N�x�w��X
    // s_SQL = s_SQL + "  WHERE MODEL_NO = '���ݏȎd�l'";
    // s_SQL = s_SQL + "    AND B_YEAR = 9";
    // }else if(P_HEADS_DATA[269].TrimRight() == "11"){   // ���N�x�w��11
    // s_SQL = s_SQL + "  WHERE MODEL_NO = '���ݏȎd�l'";
    // s_SQL = s_SQL + "    AND B_YEAR = 11";
    // }else{
    // if(P_HEADS_DATA[846].TrimRight() == "1") {      // �a���X�`�[���@�`�|�O�T
    // s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-05'";
    // }else if(P_HEADS_DATA[847].TrimRight() == "1") {// �`���X�`�[���@�`�|�O�V
    // s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-07'";
    // }else {                                         // ��L�ȊO�@�@�@�`�|�O�R
    // s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-03'";
    // }
    // }
    // P_Siyou2 = P_HEADS_DATA[1372].TrimRight();
    // s_MeibanName = "����(�a��)";
    if (P_HEADS_DATA[268].TrimRight() == "1") { // �����Ȏd�l
        if (P_HEADS_DATA[269].TrimRight() == "11") { // ���N�x�w��11
            s_SQL = s_SQL + "  WHERE MODEL_NO = '�����Ȏd�l'";
            s_SQL = s_SQL + "    AND B_YEAR = 11";
        }
        else {
            if (P_HEADS_DATA[846].TrimRight() == "1") { // �a���X�`�[���@�`�|�O�T
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-05'";
            }
            else if (P_HEADS_DATA[847].TrimRight() == "1") { // �`���X�`�[���@�`�|�O�V
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-07'";
            }
            else { // ��L�ȊO�@�@�@�`�|�O�R
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-03'";
            }
        }
        P_Siyou2 = P_HEADS_DATA[1372].TrimRight();
        s_MeibanName = "����(�a��)" + P_Siyou2;
        // ***************************
        // 2008.06.26 �_�Ж��p�~
        // }else if(P_HEADS_DATA[1063].Pos("�_�ЎY��") != 0){
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '�_�ЎY�ƌ�'";
        // if(P_HEADS_DATA[1].TrimRight() == "0" ) {          // �a���Ȃ�_�Ђi
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "����(�a��)";
        // }else{                                             // �p���Ȃ�_�Ђd
        // s_SQL = s_SQL + "    AND WAEI = 'E'";
        // s_MeibanName = "����(�p��)";
        // }
        // ***********************
        // 2011.05.30 ��n����
        // 2012.01.20 ��n�t���O
        // }else if((P_HEADS_DATA[5].TrimRight() == "42842") && (P_HEADS_DATA[1].TrimRight() == "0" )) {
        // // ��n����(42842)(�a��)����
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '��n���a'";
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "��n����(�a��)";
        // }else if((P_HEADS_DATA[5].TrimRight() == "43935") && (P_HEADS_DATA[1].TrimRight() == "0" )) {
        // // ��n����(43935)(�a��)����
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '��n���a'";
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "��n����(�a��)";
        // }else if((P_HEADS_DATA[401].TrimRight() == "42842") && (P_HEADS_DATA[1].TrimRight() == "0" )) {
        // // ��n����(42842)(�a��)����
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '��n���a'";
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "��n����(�a��)";
        // }else if((P_HEADS_DATA[401].TrimRight() == "43935") && (P_HEADS_DATA[1].TrimRight() == "0" )) {
        // // ��n����(43935)(�a��)����
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '��n���a'";
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "��n����(�a��)";
    }
    else if (P_HEADS_DATA[1107].TrimRight() == "CHINA") {
        // ��n����(����)����
        s_SQL = s_SQL + "  WHERE MODEL_NO = '��n���a'";
        s_SQL = s_SQL + "    AND WAEI = 'J'";
        s_MeibanName = "��n����(����)";
        // *********************
        // *******************
        // 2006.09.23 ���p����
    }
    else if (P_HEADS_DATA[36].Pos("M") != 0) { // ���p
        if (P_HEADS_DATA[1].TrimRight() == "0") { // �a���Ȃ甕�p���a
            s_SQL = s_SQL + "  WHERE MODEL_NO = '���p���a'";
            s_SQL = s_SQL + "    AND WAEI = 'J'";
            s_MeibanName = "����(�a��)";
        }
        else { // �p���Ȃ甕�p���p
            s_SQL = s_SQL + "  WHERE MODEL_NO = '���p���p'";
            s_SQL = s_SQL + "    AND WAEI = 'E'";
            s_MeibanName = "����(�p��)";
        }
        // *******************
    }
    else {
        // 2003.11.26 �^������
        // 2004.03.16 RX-00�ǉ�
        // if( G_KS_Syurui == "FX-01" ){
        // if( G_KS_Syurui == "�e�w" && G_KS_Model == "�O�P" ){
        if ((G_KS_Syurui == "�e�w" && G_KS_Model == "�O�P") ||
            (G_KS_Syurui == "�q�w" && G_KS_Model == "�O�O")) {
            // ********************
            // *******************
            if (P_HEADS_DATA[1].TrimRight() == "0") { // �a���Ȃ�`�|�P�O
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-10'";
                s_MeibanName = "����(�a��)";
            }
            else { // �p���Ȃ�a�|�P�O
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'B-10'";
                s_MeibanName = "����(�p��)";
            }
        }
        else {
            if (P_HEADS_DATA[1].TrimRight() == "0") { // �a���Ȃ�`�|�P
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-01'";
                s_MeibanName = "����(�a��)";
            }
            else { // �p���Ȃ�a�|�P
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'B-01'";
                s_MeibanName = "����(�p��)";
            }
        }
    }
    // *********************************************
    // ***  Ͻ�(HD_MODEL_PL) ����
    // *********************************************

    G_Log_Text = "����Ͻ�(HD_MODEL_PL)�������L�[  �w" + s_Key + "�x�Ō����B";
    Write_Log(G_Log_Text);

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "�ް��쐬��    �" + s_Key +
            "� �́A����Ͻ�(HD_MODEL_PL)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        // �ގ����ނ̎擾
        P_Sikibetu = "MEIBAN-"; // ���i����
        P_Katasiki = s_MeibanName; // ���i�^��
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // �}��
        P_SZuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // ��}�p�}��
        P_Yobihin = ""; // �\���i���ʎq
        P_Siyou = wkQuery->FieldByName("SIZE_W")->AsString.TrimRight(); // �d�l
        P_Siyou = P_Siyou + "�~";
        P_Siyou = P_Siyou + wkQuery->FieldByName("SIZE_H")
            ->AsString.TrimRight(); // �d�l
        P_Siyou = P_Siyou + "�~T0.8";
        P_Siyou2 = "";
        P_Siyou5 = "";
        P_Num = 1;
        P_Unit = "";
        if (P_HEADS_DATA[29].ToIntDef(0) != 0) {
            P_Hosoku = Search_KIKAKU_CODE(P_HEADS_DATA[29].TrimRight(), true,
                true, P_HEADS_DATA[1].TrimRight());
        }
        else {
            P_Hosoku = "";
        }

        // P_Katasiki = Search_KIKAKU_CODE(P_HEADS_DATA[29].TrimRight()); // ���̖��̂́A�K�p�K�i�ɕύX

        // *********************************
        // ***  �ގ��͂r�t�r�R�O�S�Œ�
        // *********************************
        P_Zaisitu = "�r�t�r�R�O�S";
        P_EZaisitu = "�r�t�r�R�O�S";

        // *********************************
        // ***  �\���i�� �擾
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // �\����

        // ���O
        G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  �t���i�ް�(����)�@�쐬
        // *********************************
        Write_Axsrs_Data(s_Code);
        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ����}�[�N���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_HMark_Data(void) {
    AnsiString s_Code; // ���i����

    // 2020.11.24 RX-00����}�[�N�ǉ�_S
    //// 2004.04.16 RX-00����}�[�N����
    //AnsiString s_Key;
    //s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
    //if (s_Key == "RX-00") { // RX-00
    //    return true;
    //}
    // 2020.11.24 RX-00����}�[�N�ǉ�_E

    // �ގ����ނ̎擾
    P_Sikibetu = "MARK--"; // ���i����
    P_Katasiki = "����}�[�N"; // ���i�^��
    P_Zuban = "�d�S�Q�U�O�S�U"; // �}��
    P_SZuban = "�d�S�Q�U�O�S�U"; // ��}�p�}��
    P_Yobihin = ""; // �\���i���ʎq
    P_Siyou = "200�~50"; // �d�l
    P_Siyou2 = ""; // �d�l2
    P_Siyou5 = "";
    P_Num = 1;
    P_Unit = "";

    P_Zaisitu = "�e�g����";
    P_EZaisitu = "�e�g����";

    // *********************************
    // ***  �\���i�� �擾
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // �\����

    // ���O
    G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
    Write_Log(G_Log_Text);

    // *********************************
    // ***  �t���i�ް�(����Ľ���)�@�쐬
    // *********************************
    Write_Axsrs_Data(s_Code);

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���ė������ӽï�����i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2012.10.15�ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Rakka_Data(void) {
    AnsiString s_Code; // ���i����
    AnsiString s_Key;

    s_Key = P_HEADS_DATA[1277].TrimRight();

    if (s_Key == "3") { // ���ĉ~�`�z��ï���v
    }
    else {
        return true;
    }

    // �ގ����ނ̎擾
    P_Sikibetu = "RAKKA-"; // ���i����
    P_Katasiki = "�{���g�������ӃX�e�b�J�["; // ���i�^��
    // 2014.03.06 �ï�����L�C��
    P_Hosoku = ""; // ���i����
    // ************************
    P_Zuban = "�d�S�Q�X�T�U�S"; // �}��
    P_SZuban = "�d�S�Q�X�T�U�S"; // ��}�p�}��
    P_Yobihin = ""; // �\���i���ʎq
    P_Siyou = "90�~90"; // �d�l
    P_Siyou2 = ""; // �d�l2
    P_Siyou5 = "";
    P_Num = 1;
    P_Unit = "";

    P_Zaisitu = "�e�g����";
    P_EZaisitu = "�e�g����";

    // *********************************
    // ***  �\���i�� �擾
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // �\����

    // ���O
    G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
    Write_Log(G_Log_Text);

    // *********************************
    // ***  �t���i�ް�(����Ľ���)�@�쐬
    // *********************************
    Write_Axsrs_Data(s_Code);

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �u���C���h�t�����W���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2004.03.16 �ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_BFRG_Data() {
    AnsiString s_Code; // ���i����
    AnsiString s_Key;
    AnsiString s_SQL;
    AnsiString s_Spec;
    AnsiString s_Dno2;
    AnsiString s_WKDno2;
    //2017.10.10 ��Ķ޽��čގ��ύX_S
    //AnsiString s_RtMat;
    AnsiString s_GasMat;
    //2017.10.10 ��Ķ޽��čގ��ύX_E
    AnsiString s_SiyouA;
    AnsiString s_SiyouB;
    int iCntA;
    int iCntB;
    // 2010.07.14 �΍s���ǉ�
    int iFL;
    // *********************

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    // *********************************************
    // ***�@���p�t���iϽ�(HD_ACCESSORY) ����
    // *********************************************
    s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);

    // 2015.03.20 OGI
    //   �m�Y���K�i�R�[�h�̒ǉ��ɔ����āA���p�t���i�}�X�^(HD_ACCESSORY) ��
    //  �uSERIES_MODEL�v�̓o�^���e�͕ς�����̂�Bas_AxsrsDetails.cpp��
    //   ���p���i�W�J�����́uCX-10-JIS16K�v�̃n�[�h�R�[�h�͍폜����
    //// 2011.10.28 CX-10-JIS16K�ǉ�
    //if (G_KS_Syurui == "�b�w" && (G_KS_Model == "�P�O�c" || G_KS_Model == "�P�O")) {
    //    if (P_HEADS_DATA[893].TrimRight().SubString(1, 2) == "02") {
    //        // JIS16K
    //        s_Key = s_Key + "-JIS16K";
    //    }
    //}
    // ***************************

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_ACCESSORY";
    s_SQL = s_SQL + "  WHERE SERIES_MODEL = '" + s_Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "�ް��쐬��    �ذ�ތ^���" + s_Key +
            "� �́A���p�t���iϽ�(HD_ACCESSORY)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        P_Hosoku = "";
        // ���ނ̎擾
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // ���i����
        P_Sikibetu = wkQuery->FieldByName("PL_SYMBOL")->AsString.TrimRight();
        // ���i�\���ʎq
        P_Katasiki = wkQuery->FieldByName("PARTS_NAME")
            ->AsString.TrimRight(); // ���i����
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // �ގ�����
        P_EZaisitu = Search_HD_MAT_JtoE(P_Zaisitu); // �ގ�����
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // �}��
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // �}��
        s_Dno2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        // �X�g���[�i�p�}�ԂQ
        s_WKDno2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        // �X�g���[�i�p�}�ԂQ
        P_Num = StrToInt(wkQuery->FieldByName("NUMS")->AsString.TrimRight
            ()); // ����
        P_Unit = "";
        P_Yobihin = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();
        // �\���i���ʎq

        // �~(��ײ���)��ݼށA�~��Ķ޽��āA�S�ʍ��~��Ķ޽��ĈȊO�����O����
        if (s_Code != "23" && s_Code != "25" && s_Code != "26") {
            wkQuery->Next();
            continue;
        }

        // 2015.06.23 ���p���i�d���Ή�(�m�Y���K�i�̎�ނ��ǉ�����鍀��)
        // �m�Y���R�[�h�̎擾
        AnsiString s_Nzl_Kind = "";
        AnsiString sTugite = P_HEADS_DATA[893].TrimRight(); // S1ɽ�ٌp��
        if (sTugite.ToIntDef(0) >= 1000) {
            s_Nzl_Kind = sTugite.SubString(1, 2) + "0" + sTugite.SubString(4, 1);
        }
        else {
            s_Nzl_Kind = sTugite.SubString(2, 1) + "0" + sTugite.SubString(4, 1);
        }
        AnsiString sNRLKind = wkQuery->FieldByName("NRL_KIND")->AsString.TrimRight();
        // PARTS_CODE���ȉ��ł���΃m�Y���K�i��r���s��
        // 23 �~(��ײ���)��ݼ�
        // 25 �~��Ķ޽���
        // 26 �S�ʍ��~��Ķ޽���
        if( s_Nzl_Kind != sNRLKind ){
            wkQuery->Next();
            continue;
        }
        // *************************************************************

        iCntA = 0;
        iCntB = 0;
        // 2010.07.14 �΍s���ǉ�
        // 2022.04.21 CX-01D�ǉ�_S
        if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c") {
            iFL = 1; // �΍s��
        }
        //if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
        // 2022.04.21 CX-01D�ǉ�_E
        else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
            iFL = 1; // �΍s��
        }
        // 2017.07.17 �΍s���ǉ�
        else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") {
            iFL = 1; // �΍s��
        }
        // *********************
        else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
            iFL = 1; // �΍s��
        }
        // 2017.07.17 �΍s���ǉ�
        else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
            iFL = 1; // �΍s��
        }
        // *********************
        else {
            iFL = 0; // ��`��
        }
        // �~(��ײ���)��ݼ�
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && (s_Code == "23")) {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "23") {
        // ***********************
            // S1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[891].TrimRight() == "6" || P_HEADS_DATA[891].TrimRight() == "7" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[891].TrimRight() == "6" ||
                P_HEADS_DATA[891].TrimRight() == "7") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntB += P_Num;
            }
            // S2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[906].TrimRight() == "6" || P_HEADS_DATA[906].TrimRight() == "7" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[906].TrimRight() == "6" ||
                P_HEADS_DATA[906].TrimRight() == "7") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntA += P_Num;
            }
            // S3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[921].TrimRight() == "6" || P_HEADS_DATA[921].TrimRight() == "7" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[921].TrimRight() == "6" ||
                P_HEADS_DATA[921].TrimRight() == "7") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // S4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[936].TrimRight() == "6" || P_HEADS_DATA[936].TrimRight() == "7" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[936].TrimRight() == "6" ||
                P_HEADS_DATA[936].TrimRight() == "7") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // E1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[951].TrimRight() == "6" || P_HEADS_DATA[951].TrimRight() == "7" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[951].TrimRight() == "6" ||
                P_HEADS_DATA[951].TrimRight() == "7") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntB += P_Num;
            }
            // E2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[966].TrimRight() == "6" || P_HEADS_DATA[966].TrimRight() == "7" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[966].TrimRight() == "6" ||
                P_HEADS_DATA[966].TrimRight() == "7") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntA += P_Num;
            }
            // E3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[981].TrimRight() == "6" || P_HEADS_DATA[981].TrimRight() == "7" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "6" ||
                P_HEADS_DATA[981].TrimRight() == "7") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[996].TrimRight() == "6" || P_HEADS_DATA[996].TrimRight() == "7" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "6" ||
                P_HEADS_DATA[996].TrimRight() == "7") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // if( (P_HEADS_DATA[891].TrimRight() == "6" || P_HEADS_DATA[891].TrimRight() == "7") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[906].TrimRight() == "6" || P_HEADS_DATA[906].TrimRight() == "7") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[921].TrimRight() == "6" || P_HEADS_DATA[921].TrimRight() == "7") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[936].TrimRight() == "6" || P_HEADS_DATA[936].TrimRight() == "7") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[951].TrimRight() == "6" || P_HEADS_DATA[951].TrimRight() == "7") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[966].TrimRight() == "6" || P_HEADS_DATA[966].TrimRight() == "7") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[981].TrimRight() == "6" || P_HEADS_DATA[981].TrimRight() == "7") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[996].TrimRight() == "6" || P_HEADS_DATA[996].TrimRight() == "7") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // *********************
        }
        // �~��Ķ޽��āA�S�ʍ��~��Ķ޽���
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && (s_Code == "25" ||
        //    s_Code == "26")) {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            (s_Code == "25" || s_Code == "26")) {
        // ***********************
            // 2010.07.14 �΍s���ǉ�
            // S1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[891].TrimRight() == "6" || P_HEADS_DATA[891].TrimRight() == "7" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {      // ���o�[�u�[�c
            if ((P_HEADS_DATA[891].TrimRight() == "6" ||
                P_HEADS_DATA[891].TrimRight() == "7") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    // ***********************
                    iCntB += 0;
                }
                else {
                    iCntB += P_Num;
                }
            }
            // S2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[906].TrimRight() == "6" || P_HEADS_DATA[906].TrimRight() == "7" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {      // ���o�[�u�[�c
            if ((P_HEADS_DATA[906].TrimRight() == "6" ||
                P_HEADS_DATA[906].TrimRight() == "7") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    // ***********************
                    iCntA += 0;
                }
                else {
                    iCntA += P_Num;
                }
            }
            // S3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[921].TrimRight() == "6" || P_HEADS_DATA[921].TrimRight() == "7" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[921].TrimRight() == "6" ||
                P_HEADS_DATA[921].TrimRight() == "7") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        iCntB += P_Num;
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        iCntA += P_Num;
                    }
                }
            }
            // S4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[936].TrimRight() == "6" || P_HEADS_DATA[936].TrimRight() == "7" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[936].TrimRight() == "6" ||
                P_HEADS_DATA[936].TrimRight() == "7") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        iCntA += P_Num;
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        iCntB += P_Num;
                    }
                }
            }
            // E1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[951].TrimRight() == "6" || P_HEADS_DATA[951].TrimRight() == "7" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {      // ���o�[�u�[�c
            if ((P_HEADS_DATA[951].TrimRight() == "6" ||
                P_HEADS_DATA[951].TrimRight() == "7") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    // ***********************
                    iCntB += 0;
                }
                else {
                    iCntB += P_Num;
                }
            }
            // E2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[966].TrimRight() == "6" || P_HEADS_DATA[966].TrimRight() == "7" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {      // ���o�[�u�[�c
            if ((P_HEADS_DATA[966].TrimRight() == "6" ||
                 P_HEADS_DATA[966].TrimRight() == "7") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "7")) {
                if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    // ***********************
                    iCntA += 0;
                }
                else {
                    iCntA += P_Num;
                }
            }
            // E3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[981].TrimRight() == "6" || P_HEADS_DATA[981].TrimRight() == "7" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "6" ||
                P_HEADS_DATA[981].TrimRight() == "7") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        iCntB += P_Num;
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        iCntA += P_Num;
                    }
                }
            }
            // E4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[996].TrimRight() == "6" || P_HEADS_DATA[996].TrimRight() == "7" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "6" ||
                P_HEADS_DATA[996].TrimRight() == "7") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {  //���o�[�u�[�c
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        iCntA += P_Num;
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {  //���o�[�u�[�c
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        iCntB += P_Num;
                    }
                }
            }
            // if( (P_HEADS_DATA[891].TrimRight() == "6" || P_HEADS_DATA[891].TrimRight() == "7") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1") {  //���o�[�u�[�c
            // iCntB+=0;
            // }else{
            // iCntB+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[906].TrimRight() == "6" || P_HEADS_DATA[906].TrimRight() == "7") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1") {  //���o�[�u�[�c
            // iCntA+=0;
            // }else{
            // iCntA+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[921].TrimRight() == "6" || P_HEADS_DATA[921].TrimRight() == "7") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") {  //���o�[�u�[�c
            // iCntB+=0;
            // }else{
            // iCntB+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[936].TrimRight() == "6" || P_HEADS_DATA[936].TrimRight() == "7") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1") {  //���o�[�u�[�c
            // iCntA+=0;
            // }else{
            // iCntA+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[951].TrimRight() == "6" || P_HEADS_DATA[951].TrimRight() == "7") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1") {  //���o�[�u�[�c
            // iCntB+=0;
            // }else{
            // iCntB+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[966].TrimRight() == "6" || P_HEADS_DATA[966].TrimRight() == "7") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1") {  //���o�[�u�[�c
            // iCntA+=0;
            // }else{
            // iCntA+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[981].TrimRight() == "6" || P_HEADS_DATA[981].TrimRight() == "7") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1") {  //���o�[�u�[�c
            // iCntB+=0;
            // }else{
            // iCntB+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[996].TrimRight() == "6" || P_HEADS_DATA[996].TrimRight() == "7") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1") {  //���o�[�u�[�c
            // iCntA+=0;
            // }else{
            // iCntA+=P_Num;
            // }
            // }
            // *********************
            if (s_Code == "25") { // �~(��ײ���)��ݼ�SS400�ȊO�̏ꍇ�~��Ķ޽���
                // 2016.11.07 �ގ����ޕύX
                //if (P_HEADS_DATA[265].ToIntDef(0) == 1040) {
                if (Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                // ***********************
                    iCntA = 0;
                    iCntB = 0;
                }
            }
            else if (s_Code == "26") { // �~(��ײ���)��ݼ�SS400�̏ꍇ�S�ʍ��~��Ķ޽���
                // 2018.02.09 �ގ�����C��_S
                //// 2016.11.07 �ގ����ޕύX
                ////if (P_HEADS_DATA[265].ToIntDef(0) != 1040) {
                //if (Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                //// ***********************
                if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                // 2018.02.09 �ގ�����C��_E
                    iCntA = 0;
                    iCntB = 0;
                }
            }
        }

        // 2019.08.26 E������ůĒǉ�_S�@
        // E������ů�
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "27") {
            // S1
            if ((P_HEADS_DATA[891].TrimRight() == "6" ||
                 P_HEADS_DATA[891].TrimRight() == "7") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                iCntB += P_Num;
            }
            // S2
            if ((P_HEADS_DATA[906].TrimRight() == "6" ||
                 P_HEADS_DATA[906].TrimRight() == "7") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                iCntA += P_Num;
            }
            // S3
            if ((P_HEADS_DATA[921].TrimRight() == "6" ||
                 P_HEADS_DATA[921].TrimRight() == "7") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // S4
            if ((P_HEADS_DATA[936].TrimRight() == "6" ||
                 P_HEADS_DATA[936].TrimRight() == "7") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // E1
            if ((P_HEADS_DATA[951].TrimRight() == "6" ||
                 P_HEADS_DATA[951].TrimRight() == "7") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                iCntB += P_Num;
            }
            // E2
            if ((P_HEADS_DATA[966].TrimRight() == "6" ||
                 P_HEADS_DATA[966].TrimRight() == "7") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                iCntA += P_Num;
            }
            // E3
            if ((P_HEADS_DATA[981].TrimRight() == "6" ||
                 P_HEADS_DATA[981].TrimRight() == "7") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            if ((P_HEADS_DATA[996].TrimRight() == "6" ||
                 P_HEADS_DATA[996].TrimRight() == "7") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
        }
        // 2019.08.26 E������ůĒǉ�_E�@

        // ���ނ��Ȃ� �~(��ײ���)��ݼ �܂��́A�~��Ķ޽���
        // 2019.08.26 E������ůĒǉ�_S�@
        //if (P_HEADS_DATA[265].ToIntDef(0) == 0 && (s_Code == "23" ||
        //    s_Code == "25" || s_Code == "26")) {
        if (P_HEADS_DATA[265].ToIntDef(0) == 0 && (s_Code == "23" ||
            s_Code == "25"|| s_Code == "26" || s_Code == "27")) {
        // 2019.08.26 E������ůĒǉ�_E�@
            wkQuery->Next();
            continue;
        }

        // *********************************
        // ***  �\���i�� �擾
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // �\����

        // ���O
        G_Log_Text = "���i����           �" + s_Code + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
        Write_Log(G_Log_Text);

        if (s_Code == "23") { // �~(��ײ���)��ݼ�
            P_Siyou = P_HEADS_DATA[1370].TrimRight();
            P_Siyou2 = "";
            P_Siyou5 = "A";
            P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
            P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            P_Num = iCntA;
            P_Unit = "";
            P_Hosoku = "";
            if (iCntA > 0)
                Write_Axsrs_Data(s_Code);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Axsrs_Data(s_Code);
        }
        // 2005.01.07 �S�ʍ��~��Ķ޽��Ă̏ꍇ�ގ��ύX
        // if( s_Code == "25" || s_Code == "26" ){ // �~��Ķ޽��āA�S�ʍ��~��Ķ޽���
        if (s_Code == "25") { // �~��Ķ޽���
            // ********************************************
            P_Siyou = P_HEADS_DATA[1370].TrimRight();
            P_Siyou2 = "";
            P_Siyou5 = "A";
            //2017.10.10 ��Ķ޽��čގ��ύX_S
            //P_Zaisitu =
            //    Search_HD_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[836].TrimRight(), 2));
            //P_EZaisitu =
            //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[836].TrimRight(), 2));
            s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ��Ķ޽��čގ�CODE
            s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
            P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
            P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
            //2017.10.10 ��Ķ޽��čގ��ύX_E
            P_Num = iCntA;
            P_Unit = "";
            P_Hosoku = "";
            if (iCntA > 0)
                Write_Axsrs_Data(s_Code);
            P_Siyou5 = "B";
            //2017.10.10 ��Ķ޽��čގ��ύX_S
            //P_Zaisitu =
            //    Search_HD_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[835].TrimRight(), 2));
            //P_EZaisitu =
            //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[835].TrimRight(), 2));
            s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ��Ķ޽��čގ�CODE
            s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
            P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
            P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
            //2017.10.10 ��Ķ޽��čގ��ύX_E
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Axsrs_Data(s_Code);
        }
        // 2005.01.07 �S�ʍ��~��Ķ޽��Ă̏ꍇ�ގ��ύX
        if (s_Code == "26") { // �S�ʍ��~��Ķ޽���
            P_Siyou = P_HEADS_DATA[1370].TrimRight();
            P_Siyou2 = "";
            P_Siyou5 = "A";
            //2017.10.10 ��Ķ޽��čގ��ύX_S
            //P_Zaisitu =
            //    Search_HD_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[836].TrimRight(), 4));
            //P_EZaisitu =
            //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[836].TrimRight(), 4));
            s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ��Ķ޽��čގ�CODE
            s_GasMat = Chg_Zaisitu_Code(s_GasMat, 4);
            P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
            P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
            //2017.10.10 ��Ķ޽��čގ��ύX_E
            P_Num = iCntA;
            P_Unit = "";
            P_Hosoku = "";

            if (iCntA > 0)
                Write_Axsrs_Data(s_Code);
            P_Siyou5 = "B";
            //2017.10.10 ��Ķ޽��čގ��ύX_S
            //P_Zaisitu =
            //    Search_HD_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[835].TrimRight(), 4));
            //P_EZaisitu =
            //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[835].TrimRight(), 4));
            s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ��Ķ޽��čގ�CODE
            s_GasMat = Chg_Zaisitu_Code(s_GasMat, 4);
            P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
            P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
            //2017.10.10 ��Ķ޽��čގ��ύX_E
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Axsrs_Data(s_Code);
        }
        // ********************************************
        // 2019.08.26 E������ůĒǉ�_S�@
        if (s_Code == "27") { // E������ů�
            P_Siyou5 = "A";
            P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
            P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            // 2022.09.21 E���{���g�i�b�gSUS�w��_S
            if (P_HEADS_DATA[162].TrimRight() == "1.00") {
                P_Zaisitu = "SUS304";
                P_EZaisitu = "SUS304";
            }
            // 2022.09.21 E���{���g�i�b�gSUS�w��_E
            P_Num = iCntA;
            P_Unit = "";
            P_Hosoku = "";
            if (iCntA > 0)
                Write_Axsrs_Data(s_Code);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Axsrs_Data(s_Code);
        }
        // 2019.08.26 E������ůĒǉ�_E�@

        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���p���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Fune_Data(bool bMflg) {
    AnsiString s_Code; // ���i����
    AnsiString s_Key;
    AnsiString s_SQL;
    AnsiString s_Spec;
    AnsiString s_Dno2;
    AnsiString s_WKDno2;
    //2017.10.10 ��Ķ޽��čގ��ύX_S
    //AnsiString s_RtMat;
    AnsiString s_GasMat;
    //2017.10.10 ��Ķ޽��čގ��ύX_E
    int iCnt; // 2002/10/24
    int iCnt2; // 2004.05.31
    int iCntA; // 2004.06.30
    int iCntB; // 2004.06.30
    int iCnt2A; // 2008.01.18
    int iCnt2B; // 2008.01.18
    // 2010.07.14 �΍s���ǉ�
    int iFL;
    // *********************
    // 2016.08.30 �ގ����ޕύX
    AnsiString s_TextA;
    AnsiString s_TextB;
    // ***********************
    // 2018.05.28  ��Ķ޽��ē\�����敪_S
    AnsiString s_GasSiyou;
    AnsiString sSpecCode;
    // **********************************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
    // 2018.05.28  ��Ķ޽��ē\�����敪_E

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    // 2015.03.20 OGI ���p���i�d���Ή�(�m�Y���K�i�̎�ނ��ǉ�����鍀��)
    // �m�Y���R�[�h�̎擾
    AnsiString s_Nzl_Kind = "";
    AnsiString sTugite = P_HEADS_DATA[893].TrimRight(); // S1ɽ�ٌp��
    if (sTugite.ToIntDef(0) >= 1000) {
        s_Nzl_Kind = sTugite.SubString(1, 2) + "0" + sTugite.SubString(4, 1);
    }
    else {
        s_Nzl_Kind = sTugite.SubString(2, 1) + "0" + sTugite.SubString(4, 1);
    }

    // *********************************************
    // ***�@���p�t���iϽ�(HD_ACCESSORY) ����
    // *********************************************
    // �����L�[�ύX 02.08.30
    // s_Key = G_K_Syurui + "-" + G_K_Model1;
    s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);

    // 2015.03.20 OGI
    //   �m�Y���K�i�R�[�h�̒ǉ��ɔ����āA���p�t���i�}�X�^(HD_ACCESSORY) ��
    //  �uSERIES_MODEL�v�̓o�^���e�͕ς�����̂�Bas_AxsrsDetails.cpp��
    //   ���p���i�W�J�����́uCX-10-JIS16K�v�̃n�[�h�R�[�h�͍폜����
    //// 2011.10.28 CX-10-JIS16K�ǉ�
    //if (G_KS_Syurui == "�b�w" && (G_KS_Model == "�P�O�c" || G_KS_Model == "�P�O")) {
    //  if (P_HEADS_DATA[893].TrimRight().SubString(1, 2) == "02") {
    //      // JIS16K
    //      s_Key = s_Key + "-JIS16K";
    //  }
    //}
    // ***************************

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_ACCESSORY";
    s_SQL = s_SQL + "  WHERE SERIES_MODEL = '" + s_Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "�ް��쐬��    �ذ�ތ^���" + s_Key +
            "� �́A���p�t���iϽ�(HD_ACCESSORY)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        P_Hosoku = "";
        // ���ނ̎擾
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // ���i����
        s_Spec = wkQuery->FieldByName("PARTS_SPEC")->AsString.TrimRight(); // �d�l
        P_Sikibetu = wkQuery->FieldByName("PL_SYMBOL")->AsString.TrimRight();
        // ���i�\���ʎq
        P_Katasiki = wkQuery->FieldByName("PARTS_NAME")
            ->AsString.TrimRight(); // ���i����
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // �ގ�����
        P_EZaisitu = Search_HD_MAT_JtoE(P_Zaisitu); // �ގ�����
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // �}��
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // �}��
        s_Dno2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        // �X�g���[�i�p�}�ԂQ
        s_WKDno2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        // �X�g���[�i�p�}�ԂQ
        P_Num = StrToInt(wkQuery->FieldByName("NUMS")->AsString.TrimRight
            ()); // ����
        P_Unit = "";
        P_Yobihin = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();
        // �\���i���ʎq


        // 2015.03.20 OGI ���p���i�d���Ή�(�m�Y���K�i�̎�ނ��ǉ�����鍀��)
        AnsiString sNRLKind = wkQuery->FieldByName("NRL_KIND")->AsString.TrimRight();
        if( s_Code == "18" || s_Code == "19" || s_Code == "22" ||
            s_Code == "24" || s_Code == "27" || s_Code == "28" ) {
            // PARTS_CODE���ȉ��ł���΃m�Y���K�i��r���s��
            // 18 S�������BN
            // 19 �[�ǉ��~��
            // 22 ����ݼ�
            // 24 ��Ķ޽���
            // 27 E���X�^�b�h�{���g�i�b�g
            // 28 ��������ėpײŰ
            if( s_Nzl_Kind != sNRLKind ){
                wkQuery->Next();
                continue;
            }
        }

        // 2015.06.23 ���p���i�d���Ή�(�m�Y���K�i�̎�ނ��ǉ�����鍀��)
        // 2019.08.26 E������ůĒǉ�_S�@
        //if (s_Code == "23" || s_Code == "25" || s_Code == "26") {
        //    // PARTS_CODE���ȉ��ł���΃m�Y���K�i��r���s��
        //    // 23 �~(��ײ���)��ݼ�
        //    // 25 �~��Ķ޽���
        //    // 26 �S�ʍ��~��Ķ޽���
        if (s_Code == "23" || s_Code == "25" || s_Code == "26" || s_Code ==
           "27") {
            // PARTS_CODE���ȉ��ł���΃m�Y���K�i��r���s��
            // 23 �~(��ײ���)��ݼ�
            // 25 �~��Ķ޽���
            // 26 �S�ʍ��~��Ķ޽���
            // 27 E������ů�
        // 2019.08.26 E������ůĒǉ�_E
            if( s_Nzl_Kind != sNRLKind ){
                wkQuery->Next();
                continue;
            }
        }
        // *************************************************************

        // 2015.03.23 OGI ���p���i���Ɂu�ړ��~���A���u���V�A�Q�[�W�v���[�g�v���o�͂���Ȃ��悤�ɑ΍�
        if( s_Code == "14" || s_Code == "15" || s_Code == "16" ) {
            //14�F�ړ��~�
            //15�F�����׼
            //16�F�ް����ڰ�
            if ( (P_HEADS_DATA[262].ToIntDef(0) == 1) || (P_HEADS_DATA[262].ToIntDef(0) == 2) ) {
                wkQuery->Next();
                continue;
            }
        }

        // ���p�łȂ��ꍇ�́A���t�����W�֌W�A�~�t�����W�֌W�ȊO�����O����
        // 2019.08.26 E������ůĒǉ�_S�@
        //if (bMflg == false &&
        //    (s_Code != "22" && s_Code != "23" && s_Code != "24" && s_Code !=
        //    "25")) {
        if (bMflg == false &&
            (s_Code != "22" && s_Code != "23" && s_Code != "24" && s_Code !=
            "25" && s_Code != "27")) {
        // 2019.08.26 E������ůĒǉ�_E
            wkQuery->Next();
            continue;
        }
        // ��2002/10/24
        // ���ނ����� ���t�����W �܂��́A��Ķ޽���
        iCnt = 0;
        // 2017.11.22 �Б��ݸޒ[��_S
        //// 2003.10.22 �[�ǗL�������ǉ�
        if( s_Code == "19" ) {
        //if (s_Code == "19" && P_HEADS_DATA[1136].ToIntDef(0) != 0) { // �[�ǂ��t��
        // 2017.11.22 �Б��ݸޒ[��_E
            // 2016.08.30 �ގ����ޕύX
            //// ***************************
            //// 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //if (P_HEADS_DATA[1060].Pos("1051") > 0) { // 1051 STPG
            //    // if ( P_HEADS_DATA[138].Pos( "STPG" ) > 0 ) {
            //    // *********************************
            //    // 2004.05.13 ���p�[�Ǎގ��ǉ�
            //    // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //}
            //else if (P_HEADS_DATA[1060].Pos("1043") > 0) { // 1043 CAC703
            //    // } else if ( P_HEADS_DATA[138].Pos( "1043" ) > 0 ) {   //1043 CAC703
            //    // *********************************
            //    // 2004.06.21 ���p�[�Ǎގ��ύX
            //    // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //}
            //else if (P_HEADS_DATA[1060].Pos("1151") > 0) { // 1151 STKM13A
            //    // } else if ( P_HEADS_DATA[138].Pos( "1151" ) > 0 ) {   //1151 STKM13A
            //    // *********************************
            //    // } else if ( P_HEADS_DATA[138].Pos( "1052" ) > 0 ) {   //1052 STKM13A
            //    // ***************************
            //    // ***************************
            //}
            //else {
            //    // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //    if (P_HEADS_DATA[1060].Pos("1014") > 0) { // 1014 TP270�̏ꍇ�͂s�o�R�S�O
            //        // if ( P_HEADS_DATA[138].Pos( "TP" ) > 0 ) {         //�[�ǂ��`�^���n�ގ��̏ꍇ�͂s�o�R�S�O
            //        // *********************************
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1015") > 0)
            //    { // 1015 TP270PD�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1032") > 0)
            //    { // 1032 TP340�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1033") > 0)
            //    { // 1033 TP340PD�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1057") > 0)
            //    { // 1057 TP480�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1058") > 0)
            //    { // 1058 TP480PD�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // *********************************
            //        // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1075") > 0)
            //    { // �[�ǂ��r�a�|�Q�U�T�|�f���D�V�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1089") > 0)
            //    { // �[�ǂ��r�a�|�Q�U�T�|�f���D�Q�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1090") > 0)
            //    { // �[�ǂ��a�|�Q�U�T�|�f���D�Q�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // *************************************************
            //        // 2010.12.02 SB-265-Gr.11�ǉ�
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1076") > 0)
            //    { // �[�ǂ��r�a�|�Q�U�T�|�f���D�P�P�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // ***************************
            //        // 2004.01.28 ASME
            //        // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1085") > 0)
            //    { // �[�ǂ��r�a�|�Q�U�T�|�f���D�P�̏ꍇ�͂s�o�R�S�O
            //        // }else if ( P_HEADS_DATA[138].Pos( "1085" ) > 0 ) { //�[�ǂ��r�a�|�Q�U�T�|�f���D�P�̏ꍇ�͂s�o�R�S�O
            //        // *********************************
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1095") > 0)
            //    { // �[�ǂ��a�|�Q�U�T�|�f���D�P�̏ꍇ�͂s�o�R�S�O
            //        // }else if ( P_HEADS_DATA[138].Pos( "1095" ) > 0 ) { //�[�ǂ��a�|�Q�U�T�|�f���D�P�̏ꍇ�͂s�o�R�S�O
            //        // *********************************
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // ***************
            //    }
            //    else {
            //        P_Zaisitu = "�r�t�r�R�O�S";
            //        P_EZaisitu = "�r�t�r�R�O�S";
            //    }
            //    P_Sikibetu = "RSTP-B";
            //    P_Num = 4;
            //    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
            //    Write_Funeyou_Data(s_Code, s_Spec);
            //}
            // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //if (P_HEADS_DATA[1061].Pos("1051") > 0) { // 1051 STPG
            //    // if ( P_HEADS_DATA[139].Pos( "STPG" ) > 0 ) {
            //    // *********************************
            //    wkQuery->Next();
            //    continue;
            //    // 2004.05.13 ���p�[�Ǎގ��ǉ�
            //    // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //}
            //else if (P_HEADS_DATA[1061].Pos("1043") > 0) { // 1043 CAC703
            //    // } else if ( P_HEADS_DATA[139].Pos( "1043" ) > 0 ) {   //1043 CAC703
            //    // *********************************
            //    wkQuery->Next();
            //    continue;
            //    // 2004.06.21 ���p�[�Ǎގ��ύX
            //    // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //}
            //else if (P_HEADS_DATA[1061].Pos("1151") > 0) { // 1151 STKM13A
            //    // } else if ( P_HEADS_DATA[139].Pos( "1151" ) > 0 ) {   //1151 STKM13A
            //    // *********************************
            //    // } else if ( P_HEADS_DATA[139].Pos( "1052" ) > 0 ) {   //1052 STKM13A
            //    // ***************************
            //    wkQuery->Next();
            //    continue;
            //    // ***************************
            //}
            //else {
            //    // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //    if (P_HEADS_DATA[1061].Pos("1014") > 0) { // 1014 TP270�̏ꍇ�͂s�o�R�S�O
            //        // if ( P_HEADS_DATA[139].Pos( "TP" ) > 0 ) {         //�[�ǂ��`�^���n�ގ��̏ꍇ�͂s�o�R�S�O
            //        // *********************************
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1015") > 0)
            //    { // 1015 TP270PD�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1032") > 0)
            //    { // 1032 TP340�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1033") > 0)
            //    { // 1033 TP340PD�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1057") > 0)
            //    { // 1057 TP480�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1058") > 0)
            //    { // 1058 TP480PD�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // *********************************
            //        // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1075") > 0)
            //    { // �[�ǂ��r�a�|�Q�U�T�|�f���D�V�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1089") > 0)
            //    { // �[�ǂ��r�a�|�Q�U�T�|�f���D�Q�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1090") > 0)
            //    { // �[�ǂ��a�|�Q�U�T�|�f���D�Q�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // *************************************************
            //        // 2010.12.02 SB-265-Gr.11�ǉ�
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1076") > 0)
            //    { // �[�ǂ��r�a�|�Q�U�T�|�f���D�P�P�̏ꍇ�͂s�o�R�S�O
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // *************************************************
            //        // **************************
            //        // 2004.01.28 ASME
            //        // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1085") > 0)
            //    { // �[�ǂ��r�a�|�Q�U�T�|�f���D�P�̏ꍇ�͂s�o�R�S�O
            //        // }else if ( P_HEADS_DATA[139].Pos( "1085" ) > 0 ) { //�[�ǂ��r�a�|�Q�U�T�|�f���D�P�̏ꍇ�͂s�o�R�S�O
            //        // *********************************
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // 2004.07.07 �[�Ǎގ��f�[�^�ʒu�ύX
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1095") > 0)
            //    { // �[�ǂ��a�|�Q�U�T�|�f���D�P�̏ꍇ�͂s�o�R�S�O
            //        // }else if ( P_HEADS_DATA[139].Pos( "1095" ) > 0 ) { //�[�ǂ��a�|�Q�U�T�|�f���D�P�̏ꍇ�͂s�o�R�S�O
            //        // *********************************
            //        P_Zaisitu = "�s�o�R�S�O";
            //        P_EZaisitu = "�s�o�R�S�O";
            //        // ***************
            //    }
            //    else {
            //        P_Zaisitu = "�r�t�r�R�O�S";
            //        P_EZaisitu = "�r�t�r�R�O�S";
            //    }
            //    P_Num = 4;
            //    P_Sikibetu = "RSTP-A";
            //}
            s_TextB = P_HEADS_DATA[1060].SubString(1,4);
            s_TextB = Type_MAT(s_TextB);
            if ( s_TextB == "STPG" ) {      // �r�s�o�f
                // �[�ǉ��~�߂Ȃ�
            }
            else if ( s_TextB == "ALBC" ) { // �`�k�a�b
                // �[�ǉ��~�߂Ȃ�
            }
            else if ( s_TextB == "CS" ) {   // �b�r(�r�s�j�l�P�R�`)
                // �[�ǉ��~�߂Ȃ�
            }
            // 2019.05.15 SGP�[�ǒǉ�_S
            else if ( s_TextB == "SGP") {   // �r�f�o
                // �[�ǉ��~�߂Ȃ�
            }
            // 2019.04.12 SGP�[�ǒǉ�_E
            // 2017.11.22 �Б��ݸޒ[��_S
            else if ( s_TextB == "-" ) {    // �ގ����w��
                // �[�ǂȂ�
            }
            // 2017.11.22 �Б��ݸޒ[��_E
            else {
                // 2021.05.17 �[�ǉ��~�ߔp�~_S
                //if( s_TextB == "Ti" ) {     // �`�^��
                //    P_Zaisitu = "�s�o�R�S�O";
                //    P_EZaisitu = "�s�o�R�S�O";
                //} else {
                //    P_Zaisitu = "�r�t�r�R�O�S";
                //    P_EZaisitu = "�r�t�r�R�O�S";
                //}
                //P_Sikibetu = "RSTP-B";
                //P_Num = 4;
                //P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                //Write_Funeyou_Data(s_Code, s_Spec);
                // 2021.05.17 �[�ǉ��~�ߔp�~_E
            }

            s_TextA = P_HEADS_DATA[1061].SubString(1,4);
            s_TextA = Type_MAT(s_TextA);
            if ( s_TextA == "STPG" ) {      // �r�s�o�f
                // �[�ǉ��~�߂Ȃ�
                wkQuery->Next();
                continue;
            }
            else if ( s_TextA == "ALBC" ) { // �`�k�a�b
                // �[�ǉ��~�߂Ȃ�
                wkQuery->Next();
                continue;
            }
            else if ( s_TextA == "CS" ) {   // �b�r(�r�s�j�l�P�R�`)
                // �[�ǉ��~�߂Ȃ�
                wkQuery->Next();
                continue;
            }
            // 2019.05.15 SGP�[�ǒǉ�_S
            else if ( s_TextA == "SGP") {   // �r�f�o
                // �[�ǉ��~�߂Ȃ�
                wkQuery->Next();
                continue;
            }
            // 2019.04.12 SGP�[�ǒǉ�_E
            // 2017.11.22 �Б��ݸޒ[��_S
            else if ( s_TextA == "-" ) {    // �ގ����w��
                // �[�ǂȂ�
                wkQuery->Next();
                continue;
            }
            // 2017.11.22 �Б��ݸޒ[��_E
            else {
                // 2021.05.17 �[�ǉ��~�ߔp�~_S
                //if( s_TextA == "Ti" ) {     // �`�^��
                //    P_Zaisitu = "�s�o�R�S�O";
                //    P_EZaisitu = "�s�o�R�S�O";
                //} else {
                //    P_Zaisitu = "�r�t�r�R�O�S";
                //    P_EZaisitu = "�r�t�r�R�O�S";
                //}
                //P_Sikibetu = "RSTP-A";
                //P_Num = 4;
                wkQuery->Next();
                continue;
                // 2021.05.17 �[�ǉ��~�ߔp�~_E
            }
            // ***********************
        }
        // 2004.05.31 ��������ėpײŰ�ǉ�
        // 2017.11.22 �Б��ݸޒ[��_S
        //if (s_Code == "28" && P_HEADS_DATA[1136].ToIntDef(0) != 0) { // �[�ǂ��t��
        if (s_Code == "28") {
        // 2017.11.22 �Б��ݸޒ[��_E
            iCnt2 = 0;
            // 2016.08.30 �ގ����ޕύX
            //// 2004.06.21 ���p�[�Ǎގ��ύX
            //// 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
            //if (P_HEADS_DATA[1060].Pos("1043") > 0 || P_HEADS_DATA[1060].Pos
            //    ("1151") > 0) { // 1043 CAC703  1151 STKM13A
            //    // if ( P_HEADS_DATA[138].Pos( "1043" ) > 0 || P_HEADS_DATA[138].Pos( "1151" ) > 0) {  //1043 CAC703  1151 STKM13A
            //    // *********************************
            //    iCnt2 += P_Num;
            //}
            //// 2004.07.02 �[�Ǎގ��f�[�^�ʒu�ύX
            //if (P_HEADS_DATA[1061].Pos("1043") > 0 || P_HEADS_DATA[1061].Pos
            //    ("1151") > 0) { // 1043 CAC703  1151 STKM13A
            //    // if ( P_HEADS_DATA[139].Pos( "1043" ) > 0 || P_HEADS_DATA[139].Pos( "1151" ) > 0) {  //1043 CAC703  1151 STKM13A
            //    // *********************************
            //    iCnt2 += P_Num;
            //}
            s_TextB = P_HEADS_DATA[1060].SubString(1,4);
            s_TextB = Type_MAT(s_TextB);
            if ( s_TextB == "ALBC" ) {    // �`�k�a�b
                iCnt2 += P_Num;
                // 2017.11.22 �Б��ݸޒ[��_S
                iCnt2 += P_Num;
                // 2017.11.22 �Б��ݸޒ[��_E
            }
            else if ( s_TextB == "CS" ) { // �b�r(�r�s�j�l�P�R�`)
                iCnt2 += P_Num;
                // 2017.11.22 �Б��ݸޒ[��_S
                iCnt2 += P_Num;
                // 2017.11.22 �Б��ݸޒ[��_E
            }

            s_TextA = P_HEADS_DATA[1061].SubString(1,4);
            s_TextA = Type_MAT(s_TextA);
            if ( s_TextA == "ALBC" ) {    // �`�k�a�b
                iCnt2 += P_Num;
                // 2017.11.22 �Б��ݸޒ[��_S
                iCnt2 += P_Num;
                // 2017.11.22 �Б��ݸޒ[��_E
            }
            else if ( s_TextA == "CS" ) { // �b�r(�r�s�j�l�P�R�`)
                iCnt2 += P_Num;
                // 2017.11.22 �Б��ݸޒ[��_S
                iCnt2 += P_Num;
                // 2017.11.22 �Б��ݸޒ[��_E
            }
            // ***********************
            // if ( P_HEADS_DATA[138].Pos( "1043" ) > 0 || P_HEADS_DATA[138].Pos( "1052" ) > 0) {  //1043 CAC703  1052 STKM13A
            // iCnt2 += P_Num;
            // }
            // if ( P_HEADS_DATA[139].Pos( "1043" ) > 0 || P_HEADS_DATA[139].Pos( "1052" ) > 0) {  //1043 CAC703  1052 STKM13A
            // iCnt2 += P_Num;
            // }
            // ***************************
            if (iCnt2 == 0) {
                wkQuery->Next();
                continue;
            }
            P_Num = iCnt2;
        }
        // ******************************
        // 2010.07.14 �΍s���ǉ�
        if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
            iFL = 1; // �΍s��
        }
        else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
            iFL = 1; // �΍s��
        }
        // 2022.11.15 HERP�C��_S
        else if  (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c") {
            iFL = 1; // �΍s��
        }
        else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") {
            iFL = 1; // �΍s��
        }
        else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
            iFL = 1; // �΍s��
        }
        // 2022.11.15 HERP�C��_E
        else {
            iFL = 0; // ��`��
        }
        // *********************
        // 2004.06.29 ���t�����WAB�敪�ǉ�
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1068].ToIntDef(0) != 0 && (s_Code == "22")) {
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) != "-" &&
            s_Code == "22") {
        // ***********************
            iCntA = 0;
            iCntB = 0;
            // 2010.07.14 �΍s���ǉ�
            // S1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[891].TrimRight() == "1" ||
                P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntB += P_Num;
            }
            // S2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[906].TrimRight() == "1" ||
                 P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntA += P_Num;
            }
            // S3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // S4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[936].TrimRight() == "1" ||
                 P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // E1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[951].TrimRight() == "1" ||
                 P_HEADS_DATA[951].TrimRight() == "2") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntB += P_Num; // E1
            }
            // E2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[966].TrimRight() == "1" ||
                 P_HEADS_DATA[966].TrimRight() == "2") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntA += P_Num; // E2
            }
            // E3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "1" ||
                 P_HEADS_DATA[981].TrimRight() == "2") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "1" ||
                P_HEADS_DATA[996].TrimRight() == "2") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // *********************
            P_Siyou5 = "A";
            // 2016.11.07 �ގ����ޕύX
            //if (P_HEADS_DATA[1068].ToIntDef(0) > 0) {
            if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) != "-") {
            // ***********************
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1068].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[1068].TrimRight());
            }
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
        }
        // 2008.01.18 �޽��čގ���AB�����œǂݍ���
        if (s_Code == "24") {
            iCntA = 0;
            iCntB = 0;
            iCnt2A = 0;
            iCnt2B = 0;
        }
        // ***************************************
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1068].ToIntDef(0) != 0 && (s_Code == "24")) {
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) != "-" &&
            s_Code == "24") {
        // ***********************
            // 2010.07.14 CX-10�ݸޒ[�ǎ΍s���ǉ�
            // ���t�����W�p�V�[�g�K�X�P�b�g
            // S1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {      // ���o�[�u�[�c
            if ((P_HEADS_DATA[891].TrimRight() == "1" ||
                P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    // ***********************
                    if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // �[�ǂ��t��
                        iCntB += 1;
                    }
                    else {
                        iCntB += 0;
                    }
                }
                else {
                    iCntB += 1;
                }
            }
            // S2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {      // ���o�[�u�[�c
            if ((P_HEADS_DATA[906].TrimRight() == "1" ||
                P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    // ***********************
                    if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // �[�ǂ��t��
                        iCntA += 1;
                    }
                    else {
                        iCntA += 0;
                    }
                }
                else {
                    iCntA += 1;
                }
            }
            // S3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            if ((P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                    else {
                        iCntB += 1;
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                    else {
                        iCntA += 1;
                    }
                }
            }
            // S4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            if ((P_HEADS_DATA[936].TrimRight() == "1" ||
                P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                    else {
                        iCntA += 1;
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                    else {
                        iCntB += 1;
                    }
                }
            }
            // E1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {      // ���o�[�u�[�c
            if ((P_HEADS_DATA[951].TrimRight() == "1" ||
                P_HEADS_DATA[951].TrimRight() == "2") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    // ***********************
                    if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // �[�ǂ��t��
                        iCntB += 1;
                    }
                    else {
                        iCntB += 0;
                    }
                }
                else {
                    iCntB += 1;
                }
            }
            // E2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {      // ���o�[�u�[�c
            if ((P_HEADS_DATA[966].TrimRight() == "1" ||
                P_HEADS_DATA[966].TrimRight() == "2") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    // ***********************
                    if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // �[�ǂ��t��
                        iCntA += 1;
                    }
                    else {
                        iCntA += 0;
                    }
                }
                else {
                    iCntA += 1;
                }
            }
            // E3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            if ((P_HEADS_DATA[981].TrimRight() == "1" ||
                 P_HEADS_DATA[981].TrimRight() == "2") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                    else {
                        iCntB += 1;
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                    else {
                        iCntA += 1;
                    }
                }
            }
            // E4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            if ((P_HEADS_DATA[996].TrimRight() == "1" ||
                 P_HEADS_DATA[996].TrimRight() == "2") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                    else {
                        iCntA += 1;
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {  // ���o�[�u�[�c
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        // ***********************
                        if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                    else {
                        iCntB += 1;
                    }
                }
            }
            ////2008.01.18 �޽��čގ���AB�����œǂݍ���
            // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            ////    if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////if( P_HEADS_DATA[1068].ToIntDef(0) != 0 && (s_Code == "22" || s_Code == "24") ){
            //////    if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if ( s_Code == "22" ) {       //���t�����W�̍ގ��w��L��
            //////        if ( P_HEADS_DATA[1068].ToIntDef(0) > 0 ) {
            //////           P_Zaisitu  = Search_HD_MATERIAL(P_HEADS_DATA[1068].TrimRight());
            //////           P_EZaisitu  = Search_HD_E_MATERIAL(P_HEADS_DATA[1068].TrimRight());
            //////        }
            //////    }
            ////////2004.04.16 �X�g���[�i�r�����~�ߗp�y�ђ[�Ǘp�V�[�g�K�X�P�b�g�ύX
            //////////2003.10.17 �X�g���[�i�J�o�[�p�V�[�g�K�X�P�b�g�ǉ�
            ////////    if ( s_Code == "24" ) {       //�V�[�g�K�X�P�b�g
            ////////        if ( s_Key == "UX-40" || s_Key == "LX-10" ||s_Key == "LX-40" ) {       // UX-10,LX-10,LX-40
            ////////            //�d�R�X�g���[�i���r�R�[��
            ////////            if( (P_HEADS_DATA[981].TrimRight() == "5" ) && (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////////            //�d�S�X�g���[�i���r�S�[��
            ////////            if( (P_HEADS_DATA[996].TrimRight() == "5" ) && (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////////        }
            ////////    }
            //////////*************************************************
            //////////2004.02.24 �[�Ǘp�V�[�g�K�X�P�b�g�ǉ�
            ////////    if( s_Code == "24"){ //�@�[�Ǘp�V�[�g�K�X�P�b�g
            ////////        if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[893].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[908].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[923].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[938].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[953].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[968].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[983].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[998].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////    }
            ////////***************************************************************
            //////*******************************
            ////    P_Num = iCnt;
            ////***************************************
            // **********************************
        }

        // 2004.04.16 �X�g���[�i�r�����~�ߗp�y�ђ[�Ǘp�V�[�g�K�X�P�b�g�ύX
        if (s_Code == "24") { // �V�[�g�K�X�P�b�g
            // �X�g���[�i�r�����~�ߗp�V�[�g�K�X�P�b�g
            // 2009.10.28 �X�g���[�i�r�����~�ߗp�V�[�g�K�X�P�b�g�ύX
            // ���ׂĂ̌^���ɂr�����~�߂����݂���
            ////2009.06.19 �X�g���[�i�r�����~�ߗp�V�[�g�K�X�P�b�g�ύX
            ////2007.02.13 �X�g���[�i�r�����~�ߗp�V�[�g�K�X�P�b�g�ύX
            ////if ( s_Key != "UX-10" && s_Key != "UX-90" && s_Key != "RX-70" ) {       // UX-10,UX-90,RX-70�ȊO�͂r�����~�߂����݂���
            ////if ( s_Key != "UX-90" && s_Key != "UX-100" && s_Key != "UX-130" &&
            ////     s_Key != "SX-70" && s_Key != "SX-90L" && s_Key != "SX-90M" &&
            ////     s_Key != "RX-70" && s_Key != "RX-90" ) { // UX-90,100,130,SX-70,90L,90M,RX-70,90�ȊO�͂r�����~�߂����݂���
            // if ( s_Key != "UX-90" && s_Key != "UX-100" && s_Key != "UX-100R" &&
            // s_Key != "UX-130" && s_Key != "SX-70" && s_Key != "SX-90L" &&
            // s_Key != "SX-90M" && s_Key != "RX-70" && s_Key != "RX-90" &&
            // s_Key != "RX-100" ) {
            // // UX-90,100,100R,130,SX-70,90L,90M,RX-70,90,100�ȊO�͂r�����~�߂����݂���
            // **************************************
            ////**************************************
            // 2017.11.07 E1E2��ڰŒǉ�_S
            // �d�P�X�g���[�i���r�P�m�Y��
            if ((P_HEADS_DATA[951].TrimRight() == "5") &&
                (P_HEADS_DATA[891].TrimRight() == "1" ||
                 P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                iCntB += 1; // ��`��
            }
            // �d�Q�X�g���[�i���r�Q�m�Y��
            if ((P_HEADS_DATA[966].TrimRight() == "5") &&
                (P_HEADS_DATA[906].TrimRight() == "1" ||
                 P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                iCntA += 1; // ��`��
            }
            // 2017.11.07 E1E2��ڰŒǉ�_E
            // 2010.07.14 �΍s���ǉ�
            // �d�R�X�g���[�i���r�R�m�Y��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[981].TrimRight() == "5" )
            // && ( P_HEADS_DATA[921].TrimRight() == "1"
            // || P_HEADS_DATA[921].TrimRight() == "2" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "5") &&
                (P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += 1; // ��`��
                }
                else {
                    iCntA += 1; // �΍s��
                }
            }
            // �d�S�X�g���[�i���r�S�m�Y��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[996].TrimRight() == "5" )
            // && ( P_HEADS_DATA[936].TrimRight() == "1"
            // || P_HEADS_DATA[936].TrimRight() == "2")
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "5") &&
                (P_HEADS_DATA[936].TrimRight() == "1" ||
                P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += 1; // ��`��
                }
                else {
                    iCntB += 1; // �΍s��
                }
            }
            ////2008.01.18 �޽��čގ���AB�����œǂݍ���
            ////�d�R�X�g���[�i���r�R�m�Y��
            // if( (P_HEADS_DATA[981].TrimRight() == "5" ) && (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
            ////�d�S�X�g���[�i���r�S�m�Y��
            // if( (P_HEADS_DATA[996].TrimRight() == "5" ) && (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
            //////�d�R�X�g���[�i���r�R�m�Y��
            ////if( (P_HEADS_DATA[981].TrimRight() == "5" ) && (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCnt+=1;
            //////�d�S�X�g���[�i���r�S�m�Y��
            ////if( (P_HEADS_DATA[996].TrimRight() == "5" ) && (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCnt+=1;
            ////***************************************
            // *********************

            // }
            // *****************************************************
            // 2009.10.28 �X�g���[�i�d���c�o�p�V�[�g�K�X�P�b�g�p�~
            ////2004.06.01 �X�g���[�i�d���c�o�p�V�[�g�K�X�P�b�g�ύX
            ////�X�g���[�i�d���c�o�p�V�[�g�K�X�P�b�g
            ////2009.06.19 �X�g���[�i�d�����~�ߗp�V�[�g�K�X�P�b�g�ύX
            ////2007.02.13 �X�g���[�i�d�����~�ߗp�V�[�g�K�X�P�b�g�ύX
            ////if ( s_Key == "UX-90" || s_Key == "RX-70" ) {                           // UX-90,RX-70�͂d���c�o�����݂���
            ////if ( s_Key == "UX-90" || s_Key == "UX-100" || s_Key == "UX-130" ||
            ////     s_Key == "SX-70" || s_Key == "SX-90L" || s_Key == "SX-90M" ||
            ////     s_Key == "RX-70" || s_Key == "RX-90" ) { // UX-90,100,130,SX-70,90L,90M,RX-70,90�͂d���c�o�����݂���
            // if ( s_Key == "UX-90" || s_Key == "UX-100" || s_Key == "UX-100R" ||
            // s_Key == "UX-130" || s_Key == "SX-70" || s_Key == "SX-90L" ||
            // s_Key == "SX-90M" || s_Key == "RX-70" || s_Key == "RX-90" ||
            // s_Key == "RX-100" ) {
            // // UX-90,100,100R,130,SX-70,90L,90M,RX-70,90,100�͂d���c�o�����݂���
            ////**************************************
            ////**************************************
            // //�K�X�P�b�g�̓��^���A�`���[�u�A�p�C�v�m�Y���ɂ������Ȃ�
            // //2008.01.18 �޽��čގ���AB�����œǂݍ���
            // if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCntB+=1;
            // if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCntA+=1;
            // //if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCnt+=1;
            // //if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCnt+=1;
            // //***************************************
            // }else if ( s_Key == "LX-10" ) {
            if (s_Key == "LX-10") {
                // ***************************************************
                // 2008.01.18 �޽��čގ���AB�����œǂݍ���
                // iCnt2 = 0;
                // �K�X�P�b�g�̓��^���A�`���[�u�A�p�C�v�m�Y���ɂ������Ȃ�
                // 2011.06.20 ɽ�ًK�i�ύX
                // if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCnt2B+=1;
                // if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCnt2A+=1;
                if (P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983]
                    .TrimRight().SubString(3, 1) != "1")
                    iCnt2B += 1;
                if (P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998]
                    .TrimRight().SubString(3, 1) != "1")
                    iCnt2A += 1;
                // ***********************

                // 2018.05.28 ��Ķ޽��ē\�����敪_S
                if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
                    iCnt2A = iCnt2A + iCnt2B;
                    iCnt2B = 0;
                }
                // 2018.05.28 ��Ķ޽��ē\�����敪_E

                if (iCnt2B != 0) {
                    //2017.10.10 ��Ķ޽��čގ��ύX_S
                    //P_Zaisitu =
                    //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                    //    (P_HEADS_DATA[835].TrimRight(), 2));
                    //P_EZaisitu =
                    //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                    //    (P_HEADS_DATA[835].TrimRight(), 2));
                    s_GasMat = Get_SheetGasketMat("B");  // ��Ķ޽��čގ�CODE
                    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                    //2017.10.10 ��Ķ޽��čގ��ύX_E
                    P_Num = iCnt2B;
                    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0",
                        m_pcHeadsData);
                    s_Spec = "JIS10K-80A"; // �d�l
                    P_Siyou5 = "B";

                    // 2018.05.28 ��Ķ޽��ē\�����敪_S
                    if (iCnt2A != 0) {
                        // ��Ķ޽��ē\����(B��)
                        if (sSpecCode == "A") {
                            // �~���[�d�l �`���@
                            s_Spec = s_Spec + "�@�`���@�a���^�a���@�`��"; // �d�l
                        } else if (sSpecCode == "B") {
                            // �~���[�d�l �a���@
                            s_Spec = s_Spec + "�@�`���@�`���^�a���@�a��"; // �d�l
                        } else {
                            s_Spec = s_Spec + "�@�a���p";                 // �d�l
                        }
                    }
                    // 2018.05.28 ��Ķ޽��ē\�����敪_E

                    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
                    P_Sikibetu = "STGKTR";                   // ���i����
                    P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
                    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

                    Write_Funeyou_Data(s_Code, s_Spec);
                    s_Spec = "JIS10K-100A"; // �d�l
                }
                if (iCnt2A != 0) {
                    //2017.10.10 ��Ķ޽��čގ��ύX_S
                    //P_Zaisitu =
                    //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                    //    (P_HEADS_DATA[836].TrimRight(), 2));
                    //P_EZaisitu =
                    //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                    //    (P_HEADS_DATA[836].TrimRight(), 2));
                    s_GasMat = Get_SheetGasketMat("A");  // ��Ķ޽��čގ�CODE
                    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                    //2017.10.10 ��Ķ޽��čގ��ύX_E
                    P_Num = iCnt2A;
                    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0",
                        m_pcHeadsData);
                    s_Spec = "JIS10K-80A"; // �d�l
                    P_Siyou5 = "A";

                    // 2018.05.28 ��Ķ޽��ē\�����敪_S
                    if (iCnt2B != 0) {
                        // ��Ķ޽��ē\����(A��)
                        if (sSpecCode == "A") {
                            // �~���[�d�l �`���@
                            s_Spec = s_Spec + "�@�`���@�`���^�a���@�a��"; // �d�l
                        } else if (sSpecCode == "B") {
                            // �~���[�d�l �a���@
                            s_Spec = s_Spec + "�@�`���@�a���^�a���@�`��"; // �d�l
                        } else {
                            s_Spec = s_Spec + "�@�`���p";                 // �d�l
                        }
                    }
                    // 2018.05.28 ��Ķ޽��ē\�����敪_E

                    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
                    P_Sikibetu = "STGKTR";                   // ���i����
                    P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
                    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

                    Write_Funeyou_Data(s_Code, s_Spec);
                    s_Spec = "JIS10K-100A"; // �d�l
                }
                // if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCnt2+=1;
                // if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCnt2+=1;
                // if ( iCnt2 != 0) {
                // s_Spec = "JIS10K-80A";        // �d�l
                // P_Num = iCnt2;
                // P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin,"0", m_pcHeadsData);
                // Write_Funeyou_Data(s_Code, s_Spec);
                // s_Spec = "JIS10K-100A";        // �d�l
                // }
                // ***************************************
            }
            // ***************************************************
            // �[�Ǘp�V�[�g�K�X�P�b�g
            // 2010.07.14 CX-10�ݸޒ[�ǎ΍s���ǉ�
            // S1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {            // ���o�[�u�[�c
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4" ) {     // �p�C�v�m�Y���i�t�����W�t���j
            if ((P_HEADS_DATA[891].TrimRight() == "1" ||
                 P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    iCntB += 0;
                }
                else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "4")
                { // �p�C�v�m�Y���i�t�����W�t���j
                    // ***********************
                    iCntB += 0;
                }
                else {
                    if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // �[�ǂ��t��
                        iCntB += 1;
                    }
                    else {
                        iCntB += 0;
                    }
                }
            }
            // S2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {            // ���o�[�u�[�c
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4" ) {     // �p�C�v�m�Y���i�t�����W�t���j
            if ((P_HEADS_DATA[906].TrimRight() == "1" ||
                P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    iCntA += 0;
                }
                else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "4")
                { // �p�C�v�m�Y���i�t�����W�t���j
                    // ***********************
                    iCntA += 0;
                }
                else {
                    if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // �[�ǂ��t��
                        iCntA += 1;
                    }
                    else {
                        iCntA += 0;
                    }
                }
            }
            // S3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {        // ���o�[�u�[�c
                    // iCntB+=0;
                    // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) { // �p�C�v�m�Y���i�t�����W�t���j
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        iCntB += 0;
                    }
                    else if (P_HEADS_DATA[923].TrimRight().SubString(3,
                        1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {        // ���o�[�u�[�c
                    // iCntA+=0;
                    // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) { // �p�C�v�m�Y���i�t�����W�t���j
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        iCntA += 0;
                    }
                    else if (P_HEADS_DATA[923].TrimRight().SubString(3,
                        1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                }
            }
            // S4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[936].TrimRight() == "1" ||
                 P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {        // ���o�[�u�[�c
                    // iCntA+=0;
                    // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) { // �p�C�v�m�Y���i�t�����W�t���j
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        iCntA += 0;
                    }
                    else if (P_HEADS_DATA[938].TrimRight().SubString(3,
                        1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {        // ���o�[�u�[�c
                    // iCntB+=0;
                    // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) { // �p�C�v�m�Y���i�t�����W�t���j
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        iCntB += 0;
                    }
                    else if (P_HEADS_DATA[938].TrimRight().SubString(3,
                        1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                }
            }
            // E1
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {            // ���o�[�u�[�c
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4" ) {     // �p�C�v�m�Y���i�t�����W�t���j
            if ((P_HEADS_DATA[951].TrimRight() == "1" ||
                 P_HEADS_DATA[951].TrimRight() == "2") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    iCntB += 0;
                }
                else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "4")
                { // �p�C�v�m�Y���i�t�����W�t���j
                    // ***********************
                    iCntB += 0;
                }
                else {
                    if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // �[�ǂ��t��
                        iCntB += 1;
                    }
                    else {
                        iCntB += 0;
                    }
                }
            }
            // E2
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {            // ���o�[�u�[�c
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4" ) {     // �p�C�v�m�Y���i�t�����W�t���j
            if ((P_HEADS_DATA[966].TrimRight() == "1" ||
                P_HEADS_DATA[966].TrimRight() == "2") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1")
                { // ���o�[�u�[�c
                    iCntA += 0;
                }
                else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "4")
                { // �p�C�v�m�Y���i�t�����W�t���j
                    // ***********************
                    iCntA += 0;
                }
                else {
                    if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // �[�ǂ��t��
                        iCntA += 1;
                    }
                    else {
                        iCntA += 0;
                    }
                }
            }
            // E3
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "1" ||
                P_HEADS_DATA[981].TrimRight() == "2") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {        // ���o�[�u�[�c
                    // iCntB+=0;
                    // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) { // �p�C�v�m�Y���i�t�����W�t���j
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        iCntB += 0;
                    }
                    else if (P_HEADS_DATA[983].TrimRight().SubString(3,
                        1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {        // ���o�[�u�[�c
                    // iCntA+=0;
                    // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) { // �p�C�v�m�Y���i�t�����W�t���j
                    // 2011.06.20 ɽ�ًK�i�ύX
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        iCntA += 0;
                    }
                    else if (P_HEADS_DATA[983].TrimRight().SubString(3,
                        1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                }
            }
            // E4
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "1" ||
                 P_HEADS_DATA[996].TrimRight() == "2") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // ��`��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {        // ���o�[�u�[�c
                    // iCntA+=0;
                    // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) { // �p�C�v�m�Y���i�t�����W�t���j
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        iCntA += 0;
                    }
                    else if (P_HEADS_DATA[998].TrimRight().SubString(3,
                        1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                }
                else { // �΍s��
                    // 2011.06.20 ɽ�ًK�i�ύX
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {        // ���o�[�u�[�c
                    // iCntB+=0;
                    // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) { // �p�C�v�m�Y���i�t�����W�t���j
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ���o�[�u�[�c
                        iCntB += 0;
                    }
                    else if (P_HEADS_DATA[998].TrimRight().SubString(3,
                        1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                }
            }
            ////2008.01.08 �޽��čގ���AB�����œǂݍ���
            // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[893].TrimRight().SubString(2,1) == "3") iCntB+=1;
            // }
            // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[908].TrimRight().SubString(2,1) == "3") iCntA+=1;
            // }
            // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[923].TrimRight().SubString(2,1) == "3") iCntB+=1;
            // }
            // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[938].TrimRight().SubString(2,1) == "3") iCntA+=1;
            // }
            // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[953].TrimRight().SubString(2,1) == "3") iCntB+=1;
            // }
            // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[968].TrimRight().SubString(2,1) == "3") iCntA+=1;
            // }
            // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[983].TrimRight().SubString(2,1) == "3") iCntB+=1;
            // }
            // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[998].TrimRight().SubString(2,1) == "3") iCntA+=1;
            // }
            ////if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[893].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[908].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[923].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[938].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[953].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[968].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[983].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[998].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////P_Num = iCnt;

            // 2018.05.28 ��Ķ޽��ē\�����敪_S
            if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
                iCntA = iCntA + iCntB;
                iCntB = 0;
            }
            s_GasSiyou = s_Spec;
            // 2018.05.28 ��Ķ޽��ē\�����敪_E

            if (iCntB != 0) {
                //2017.10.10 ��Ķ޽��čގ��ύX_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("B");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ��Ķ޽��čގ��ύX_E
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";

                // 2018.05.28 ��Ķ޽��ē\�����敪_S
                if (iCntA != 0) {
                    // ��Ķ޽��ē\����(B��)
                    if (sSpecCode == "A") {
                        // �~���[�d�l �`���@
                        s_Spec = s_Spec + "�@�`���@�a���^�a���@�`��"; // �d�l
                    } else if (sSpecCode == "B") {
                        // �~���[�d�l �a���@
                        s_Spec = s_Spec + "�@�`���@�`���^�a���@�a��"; // �d�l
                    } else {
                        s_Spec = s_Spec + "�@�a���p";                 // �d�l
                    }
                }
                // 2018.05.28 ��Ķ޽��ē\�����敪_E

                // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
                if ( P_GasNameB == "�����O�K�X�P�b�g" ) {
                    // �ݸ޶޽���
                    P_Sikibetu = "STGKTR";                   // ���i����
                    P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
                } else if ( P_GasNameB == "�S�ʍ��t�����W�K�X�P�b�g" ) {
                    // �S�ʍ���ݼ޶޽���
                    P_Sikibetu = "STGKTF";                   // ���i����
                    P_Katasiki = "�S�ʍ��t�����W�K�X�P�b�g"; // ���i�^��
                } else {
                    // ���̑�
                }
                // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

                Write_Funeyou_Data(s_Code, s_Spec);
            }

            // 2018.05.28 ��Ķ޽��ē\�����敪_S
            s_Spec = s_GasSiyou;
            // 2018.05.28 ��Ķ޽��ē\�����敪_E

            if (iCntA != 0) {
                //2017.10.10 ��Ķ޽��čގ��ύX_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("A");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ��Ķ޽��čގ��ύX_E
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";


                // 2018.05.28 ��Ķ޽��ē\�����敪_S
                if (iCntB != 0) {
                    // ��Ķ޽��ē\����(A��)
                    if (sSpecCode == "A") {
                        // �~���[�d�l �`���@
                        s_Spec = s_Spec + "�@�`���@�`���^�a���@�a��"; // �d�l
                    } else if (sSpecCode == "B") {
                        // �~���[�d�l �a���@
                        s_Spec = s_Spec + "�@�`���@�a���^�a���@�`��"; // �d�l
                    } else {
                        s_Spec = s_Spec + "�@�`���p";                 // �d�l
                    }
                }
                // 2018.05.28 ��Ķ޽��ē\�����敪_E

                // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
                if ( P_GasNameA == "�����O�K�X�P�b�g" ) {
                    // �ݸ޶޽���
                    P_Sikibetu = "STGKTR";                   // ���i����
                    P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
                } else if ( P_GasNameA == "�S�ʍ��t�����W�K�X�P�b�g" ) {
                    // �S�ʍ���ݼ޶޽���
                    P_Sikibetu = "STGKTF";                   // ���i����
                    P_Katasiki = "�S�ʍ��t�����W�K�X�P�b�g"; // ���i�^��
                } else {
                    // ���̑�
                }
                // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

                Write_Funeyou_Data(s_Code, s_Spec);
            }

            // 2018.05.28 ��Ķ޽��ē\�����敪_S
            s_Spec = s_GasSiyou;
            // 2018.05.28 ��Ķ޽��ē\�����敪_E

            P_Num = 0;
            // ***************************************
            // **********************************
        }
        // ***************************************************************

        // 2004.04.15 �X�g���[�i�p�V�[�g�K�X�P�b�g�ύX
        ////���ނ�����@�~��ݼށ@�~��Ķ޽���
        // if( P_HEADS_DATA[265].ToIntDef(0) != 0 && (s_Code == "23" || s_Code == "25") ){
        // if( P_HEADS_DATA[891].TrimRight() == "5" || P_HEADS_DATA[891].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[906].TrimRight() == "5" || P_HEADS_DATA[906].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[921].TrimRight() == "5" || P_HEADS_DATA[921].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[936].TrimRight() == "5" || P_HEADS_DATA[936].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[951].TrimRight() == "5" || P_HEADS_DATA[951].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[966].TrimRight() == "5" || P_HEADS_DATA[966].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[981].TrimRight() == "5" || P_HEADS_DATA[981].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[996].TrimRight() == "5" || P_HEADS_DATA[996].TrimRight() == "6" ) iCnt+=P_Num;
        // if ( s_Code == "23" ) {       //�~�t�����W�̍ގ��w��L��
        // if ( P_HEADS_DATA[265].ToIntDef(0) > 0 ) {
        // P_Zaisitu  = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
        // P_EZaisitu  = Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
        // }
        // }
        // P_Num = iCnt;
        // }
        ////��2002/10/24
        // **************************************************************
        // ���ނ�����@�~��ݼ�
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && s_Code == "23") {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" && s_Code == "23") {
        // ***********************
            // 2004.06.29 �~�t�����WAB�敪�ǉ�
            // if( P_HEADS_DATA[891].TrimRight() == "5" || P_HEADS_DATA[891].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "5" || P_HEADS_DATA[906].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "5" || P_HEADS_DATA[921].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "5" || P_HEADS_DATA[936].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "5" || P_HEADS_DATA[951].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "5" || P_HEADS_DATA[966].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "5" || P_HEADS_DATA[981].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "5" || P_HEADS_DATA[996].TrimRight() == "6" ) iCnt+=P_Num;
            ////�~�t�����W�̍ގ��w��L��
            // if ( P_HEADS_DATA[265].ToIntDef(0) > 0 ) {
            // P_Zaisitu  = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
            // P_EZaisitu  = Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            // }
            // P_Num = iCnt;
            iCntA = 0;
            iCntB = 0;
            // 2010.07.14 �΍s���ǉ�
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "5" ||
                P_HEADS_DATA[891].TrimRight() == "6") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "5" ||
                P_HEADS_DATA[906].TrimRight() == "6") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "5" ||
                P_HEADS_DATA[921].TrimRight() == "6") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "5" ||
                P_HEADS_DATA[936].TrimRight() == "6") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5" ||
                P_HEADS_DATA[951].TrimRight() == "6") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5" ||
                P_HEADS_DATA[966].TrimRight() == "6") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5" ||
                P_HEADS_DATA[981].TrimRight() == "6") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5" ||
                P_HEADS_DATA[996].TrimRight() == "6") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // if( P_HEADS_DATA[891].TrimRight() == "5" || P_HEADS_DATA[891].TrimRight() == "6") iCntB+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "5" || P_HEADS_DATA[906].TrimRight() == "6") iCntA+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "5" || P_HEADS_DATA[921].TrimRight() == "6") iCntB+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "5" || P_HEADS_DATA[936].TrimRight() == "6") iCntA+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "5" || P_HEADS_DATA[951].TrimRight() == "6") iCntB+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "5" || P_HEADS_DATA[966].TrimRight() == "6") iCntA+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "5" || P_HEADS_DATA[981].TrimRight() == "6") iCntB+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "5" || P_HEADS_DATA[996].TrimRight() == "6") iCntA+=P_Num;
            // *********************
            P_Siyou5 = "A";
            // 2016.11.07 �ގ����ޕύX
            //if (P_HEADS_DATA[265].ToIntDef(0) > 0) {
            if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-") {
            // ***********************
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            }
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
            // *********************************
        }
        // ���ނ�����@�~��Ķ޽���
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && s_Code == "25") {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "25") {
        // ***********************
            // 2008.01.08 �޽��čގ���AB�����œǂݍ���
            iCntA = 0;
            iCntB = 0;
            // 2010.07.14 �΍s���ǉ�
            // 2016.11.07 �ގ����ޕύX
            //// S1
            //if (P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    iCntB += P_Num;
            //}
            //// S2
            //if (P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    iCntA += P_Num;
            //}
            //// S3
            //if (P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    if (iFL == 0) {
            //        iCntB += P_Num; // ��`��
            //    }
            //    else {
            //        iCntA += P_Num; // �΍s��
            //    }
            //}
            //// S4
            //if (P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    if (iFL == 0) {
            //        iCntA += P_Num; // ��`��
            //    }
            //    else {
            //        iCntB += P_Num; // �΍s��
            //    }
            //}
            //// E1
            //if (P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    iCntB += P_Num;
            //}
            //// E2
            //if (P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    iCntA += P_Num;
            //}
            //// E3
            //if (P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    if (iFL == 0) {
            //        iCntB += P_Num; // ��`��
            //    }
            //    else {
            //        iCntA += P_Num; // �΍s��
            //    }
            //}
            //// E4
            //if (P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    if (iFL == 0) {
            //        iCntA += P_Num; // ��`��
            //    }
            //    else {
            //        iCntB += P_Num; // �΍s��
            //    }
            //}
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // ***********************
            ////��ڰňȊO�ŕ~(��ײ���)��ݼ�SS400�ȊO�̏ꍇ�~��Ķ޽���
            // if( P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntA+=P_Num;
            // *********************
            ////2004.06.25 �~��Ķ޽��ĕύX
            //////2004.06.01 �X�g���[�i�d���c�o�p�V�[�g�K�X�P�b�g�ύX
            ////////��ڰł����,�����,�߲��ɽ�قɂ������Ȃ�
            //////if( P_HEADS_DATA[951].TrimRight() == "5" && P_HEADS_DATA[953].TrimRight().SubString(2,1) != "1") iCnt+=P_Num;
            //////if( P_HEADS_DATA[966].TrimRight() == "5" && P_HEADS_DATA[968].TrimRight().SubString(2,1) != "1") iCnt+=P_Num;
            //////if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCnt+=P_Num;
            //////if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCnt+=P_Num;
            //////***************************************************
            //////��ڰňȊO�͂��ׂĂ�
            ////if( P_HEADS_DATA[891].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[906].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[921].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[936].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[951].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[966].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[981].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[996].TrimRight() == "6" ) iCnt+=P_Num;
            ////��ڰňȊO�ŕ~(��ײ���)��ݼ�SS400�ȊO�̏ꍇ�~��Ķ޽���
            // if( P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            ////****************************
            // P_Num = iCnt;
            if (iCntB != 0) {
                //2017.10.10 ��Ķ޽��čގ��ύX_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("B");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ��Ķ޽��čގ��ύX_E
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            if (iCntA != 0) {
                //2017.10.10 ��Ķ޽��čގ��ύX_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("A");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ��Ķ޽��čގ��ύX_E
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            P_Num = 0;
            // ***************************************
        }
        // ���ނ�����@�S�ʍ��~�޽���
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && s_Code == "26") {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "26") {
        // ***********************
            // 2008.01.08 �޽��čގ���AB�����œǂݍ���
            iCntA = 0;
            iCntB = 0;
            // 2010.07.14 �΍s���ǉ�
            // ��ڰł͂͂��ׂĂ�
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // ��ڰňȊO�ŕ~(��ײ���)��ݼ�SS400�̏ꍇ�S�ʍ��~��Ķ޽���
            // 2016.11.07 �ގ����ޕύX
            //// S1
            //if (P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    iCntB += P_Num;
            //}
            //// S2
            //if (P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    iCntA += P_Num;
            //}
            //// S3
            //if (P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    if (iFL == 0) {
            //        iCntB += P_Num; // ��`��
            //    }
            //    else {
            //        iCntA += P_Num; // �΍s��
            //    }
            //}
            // S4
            //if (P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    if (iFL == 0) {
            //        iCntA += P_Num; // ��`��
            //    }
            //    else {
            //        iCntB += P_Num; // �΍s��
            //    }
            //}
            //// E1
            //if (P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    iCntB += P_Num;
            //}
            //// E2
            //if (P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    iCntA += P_Num;
            //}
            //// E3
            //if (P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    if (iFL == 0) {
            //        iCntB += P_Num; // ��`��
            //    }
            //    else {
            //        iCntA += P_Num; // �΍s��
            //    }
            //}
            //// E4
            //if (P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    if (iFL == 0) {
            //        iCntA += P_Num; // ��`��
            //    }
            //    else {
            //        iCntB += P_Num; // �΍s��
            //    }
            //}
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // ***********************
            ////��ڰł͂͂��ׂĂ�
            // if( P_HEADS_DATA[951].TrimRight() == "5" ) iCntB+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "5" ) iCntA+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "5" ) iCntB+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "5" ) iCntA+=P_Num;
            ////��ڰňȊO�ŕ~(��ײ���)��ݼ�SS400�̏ꍇ�S�ʍ��~��Ķ޽���
            // if( P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntA+=P_Num;
            //////��ڰł͂͂��ׂĂ�
            ////if( P_HEADS_DATA[951].TrimRight() == "5" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[966].TrimRight() == "5" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[981].TrimRight() == "5" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[996].TrimRight() == "5" ) iCnt+=P_Num;
            //////2004.06.25 �S�ʍ��~��Ķ޽��ĕύX
            //////��ڰňȊO�ŕ~(��ײ���)��ݼ�SS400�̏ꍇ�S�ʍ��~��Ķ޽���
            ////if( P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            //////**********************************
            // *********************
            // P_Num = iCnt;
            if (iCntB != 0) {
                //2017.10.10 ��Ķ޽��čގ��ύX_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("B");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ��Ķ޽��čގ��ύX_E
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            if (iCntA != 0) {
                //2017.10.10 ��Ķ޽��čގ��ύX_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("A");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ��Ķ޽��čގ��ύX_E
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            P_Num = 0;
            // ***************************************
        }
        // *******************************************

        // 2019.08.26 E������ůĒǉ�_S�@
        // E������ů�
        if (s_Code == "27") {
            iCntA = 0;
            iCntB = 0;
            // 2022.09.21 E���{���g�i�b�gSUS�w��_S
            if (P_HEADS_DATA[162].TrimRight() == "1.00") {
                P_Zaisitu = "SUS304";
                P_EZaisitu = "SUS304";
            }
            // 2022.09.21 E���{���g�i�b�gSUS�w��_E
            // �~��ݼ�
            if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" ) {
                // S1
                if (P_HEADS_DATA[891].TrimRight() == "6") {
                    iCntB += P_Num;
                }
                // S2
                if (P_HEADS_DATA[906].TrimRight() == "6") {
                    iCntA += P_Num;
                }
                // S3
                if (P_HEADS_DATA[921].TrimRight() == "6") {
                    if (iFL == 0) {
                        iCntB += P_Num; // ��`��
                    }
                    else {
                        iCntA += P_Num; // �΍s��
                    }
                }
                // S4
                if (P_HEADS_DATA[936].TrimRight() == "6") {
                    if (iFL == 0) {
                        iCntA += P_Num; // ��`��
                    }
                    else {
                        iCntB += P_Num; // �΍s��
                    }
                }
                // E1
                if (P_HEADS_DATA[951].TrimRight() == "6") {
                    iCntB += P_Num;
                }
                // E2
                if (P_HEADS_DATA[966].TrimRight() == "6") {
                    iCntA += P_Num;
                }
                // E3
                if (P_HEADS_DATA[981].TrimRight() == "6") {
                    if (iFL == 0) {
                        iCntB += P_Num; // ��`��
                    }
                    else {
                        iCntA += P_Num; // �΍s��
                    }
                }
                // E4
                if (P_HEADS_DATA[996].TrimRight() == "6") {
                    if (iFL == 0) {
                        iCntA += P_Num; // ��`��
                    }
                    else {
                        iCntB += P_Num; // �΍s��
                    }
                }
            }

            // ��ڰ�
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            P_Siyou5 = "A";
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
        }
        // 2019.08.26 E������ůĒǉ�_E�@

        // *********************************
        // ***  �\���i�� �擾
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);

        // ���O
        G_Log_Text = "���i����           �" + s_Code + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�d�l               �" + s_Spec + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "���i�\����         �" + P_Sikibetu + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "���i����           �" + P_Katasiki + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "����               �" + AnsiString(P_Num) + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  ���p���i�ް��@�쐬
        // *********************************
        // //��2002/10/24
        // // Head Data �̘a�^�p�̎w���ƽï���̘a�^�p���Ⴄ�Ƃ��͏o�͂�������
        // if( P_HEADS_DATA[1].TrimRight() == "0" && (s_Code == "8" || s_Code == "9" || s_Code == "10")){//�a��̂ɉp
        // wkQuery->Next();
        // continue;
        // }
        // if( P_HEADS_DATA[1].TrimRight() == "1" && (s_Code == "11" || s_Code == "12" || s_Code == "13")){//�p��̂ɘa
        // wkQuery->Next();
        // continue;
        // }
        // 2003/03/13 �X�e�b�J�[�͐}�ԂP������āA�d�l���̕\�L���t���łQ�p�^�[���쐬�Ƃ���
        if (s_Code == "10" || s_Code == "11" || s_Code == "12" ||
            s_Code == "13") { // �R�[�h�W�ƂX�Ő}�ԂP���擾���A��͖���
            wkQuery->Next();
            continue;
        }
        if (s_Code == "8") { // �X�e�b�J�[
            if (P_HEADS_DATA[1].TrimRight() == "0") { // �a��
                s_Spec = P_HEADS_DATA[45].TrimRight() + "�p �����^�o��"; // �a���̖�(�a�j�ō쐬
            }
            else { // �p��
                s_Spec = P_HEADS_DATA[134].TrimRight() + " �h�m�^�n�t�s";
                // �a���̖�(�p�j�ō쐬
            }
        }
        if (s_Code == "9") { // �X�e�b�J�[
            if (P_HEADS_DATA[1].TrimRight() == "0") { // �a��
                s_Spec = P_HEADS_DATA[76].TrimRight() + "�p �����^�o��"; // �`���̖�(�a�j�ō쐬
            }
            else { // �p��
                s_Spec = P_HEADS_DATA[135].TrimRight() + " �h�m�^�n�t�s";
                // �`���̖�(�p�j�ō쐬
            }
        }

        // 2017.11.22 �Б��ݸޒ[��_S
        //// �[�ǂ��t���Ȃ��ꍇ
        //// 2003.10.17 �[�ǗL�������ύX
        //// if( P_HEADS_DATA[1136].TrimRight() == "0" ){
        //if (P_HEADS_DATA[1136].ToIntDef(0) == 0) {
        //    // ***************************
        //    // 2003.10.30 ����߯�ݸޒǉ�
        //    // // �[�ǉ�~�A�����BN�A�ʌ`�فA�������A���ݺ���A������ݺ���A����߯�ݸނ͕t���Ȃ�
        //    // if( s_Code == "19" || s_Code == "18" || s_Code == "1" || s_Code == "3" ||
        //    // s_Code == "5"  || s_Code == "7"  || s_Code == "4"){
        //    // wkQuery->Next();
        //    // continue;
        //    // }
        //    // 2004.05.20 ��������ėpײŰ�ǉ�
        //    // // �[�ǉ�~�A�����BN�A�ʌ`�فA�������A���ݺ���A������ݺ���A����߯�ݸނ͕t���Ȃ�
        //    // if( s_Code == "19" || s_Code == "18" || s_Code == "1" || s_Code == "3" ||
        //    // s_Code == "5"  || s_Code == "7"  || s_Code == "4" || s_Code == "2" ||
        //    // s_Code == "6"){
        //    // wkQuery->Next();
        //    // continue;
        //    // }
        //    // �[�ǉ�~�A�����BN�A�ʌ`�فA�������A���ݺ���A������ݺ���A����߯�ݸށA��������ėpײŰ�͕t���Ȃ�
        //    if (s_Code == "19" || s_Code == "18" || s_Code == "1" ||
        //        s_Code == "3" || s_Code == "5" || s_Code == "7" ||
        //        s_Code == "4" || s_Code == "2" || s_Code == "6" ||
        //        s_Code == "28") {
        //        wkQuery->Next();
        //        continue;
        //    }
        //    // *************************
        //}
        // �[�ǂ̗L���ɂ�鐔�ʕω�
        s_TextB = P_HEADS_DATA[1060].SubString(1,4);
        s_TextB = Type_MAT(s_TextB);
        s_TextA = P_HEADS_DATA[1061].SubString(1,4);
        s_TextA = Type_MAT(s_TextA);
        // �������A����߯�ݸ�(������)�A���ݺ���A����߯�ݸ�(���ݺ��)�A������ݺ���A
        // ����߯�ݸ�(������ݺ��)�A�ʌ`��
        if ( s_Code == "1" || s_Code == "2" || s_Code == "3" || s_Code == "4" ||
             s_Code == "5" || s_Code == "6" || s_Code == "7" ) {
            if ( s_TextA == "-" && s_TextB == "-" ) {
                // �[�ǂȂ�
                wkQuery->Next();
                continue;
            } else if ( s_TextA != "-" && s_TextB != "-" ) {
                // �����[��
            } else {
                // �Б��[��
                P_Num = P_Num / 2;
            }
        }
        // 2017.11.22 �Б��ݸޒ[��_E

        // 2004.04.15 ���t�����W,�V�[�g�K�X�P�b�g�o�͕ύX
        ////���ނ��Ȃ� ���t�����W �܂��́A��Ķ޽���
        // if( P_HEADS_DATA[1068].ToIntDef(0) == 0 && (s_Code == "22" || s_Code == "24") ){
        // wkQuery->Next();
        // continue;
        // }
        // ���ނ��Ȃ� ���t�����W(��Ķ޽��Ăͺ��ނȂ��ł���)
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[1068].ToIntDef(0) == 0 && s_Code == "22") {
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "-" &&
            s_Code == "22") {
        // ***********************
            wkQuery->Next();
            continue;
        }
        if ((s_Code == "22" || s_Code == "24") && P_Num == 0) {
            wkQuery->Next();
            continue;
        }
        // ***********************************************

        // ���ނ��Ȃ� �~�t�����W �܂��́A�~��Ķ޽���
        // 2016.11.07 �ގ����ޕύX
        //if (P_HEADS_DATA[265].ToIntDef(0) == 0 && (s_Code == "23" ||
        //    s_Code == "25" || s_Code == "26" || s_Code == "27")) {
        if ((Type_MAT(P_HEADS_DATA[265].TrimRight()) == "-") &&
            (s_Code == "23" || s_Code == "25" || s_Code == "26" || s_Code == "27")) {
        // ***********************
            wkQuery->Next();
            continue;
        }

        // 2004.04.15 �~�t�����W,�~��Ķ޽���,�S�ʍ��~�޽��ďo�͕ύX
        if ((s_Code == "23" || s_Code == "25" || s_Code == "26") && P_Num == 0)
        {
            wkQuery->Next();
            continue;
        }
        // **************************************************************

        if (s_Code == "14") {
            if (P_HEADS_DATA[36].Pos("J") != 0) { // �i�^�͈ړ��~�߁i�`�F�[���j�s�v
                wkQuery->Next();
                continue;
            }
        }
        if (s_Code == "21") {
            if (P_HEADS_DATA[36].Pos("J") != 0) { // �i�^�̓A�C�v���[�g�̃T�|�[�g�p�͕s�v
                wkQuery->Next();
                continue;
            }
        }
        if (s_Code.ToIntDef(0) >= 1 && s_Code.ToIntDef(0) <= 6) { // ���p�R�b�N�֌W
            // 2010.10.18 ���d�H���p�R�b�N�p�~
            // ��c�w���ɂ��
            // if ( P_HEADS_DATA[1063].Pos("���d�H") == 0 ){        // ���d�H�ȊO�̂ݗL��
            wkQuery->Next();
            continue;
            // }
            // *********************************
        }
        // 2010.10.18 ���d�H���p�R�b�N�p�~
        // ��c�w���ɂ��
        // if ( s_Code == "7" ) {       // �ʌ`��
        // if ( P_HEADS_DATA[1063].Pos("���d�H") != 0 ){        // ���d�H�̂ݗL��
        // wkQuery->Next();
        // continue;
        // }
        // }
        // *********************************
        if (s_Code == "18") { // �r���X�^�b�h
            P_EZaisitu = "SS400  ZINC";
            // 2022.09.21 SUS�X�^�b�h�ǉ�_S
            if (P_HEADS_DATA[162].TrimRight() == "1.00") {
                P_Zaisitu = "SUS304";
                P_EZaisitu = "SUS304";
            }
            // 2022.09.21 SUS�X�^�b�h�ǉ�_E
            // S���X�^�b�h�̃T�C�Y�\�L�i�d�l���j���A�[�ǗL���őł�������@03.06.24
            // 2017.11.22 �Б��ݸޒ[��_S
            //// 2003.10.17 �[�ǗL�������ύX
            //// if( P_HEADS_DATA[1136].TrimRight() == "0" ){          // �[�ǖ���
            //if (P_HEADS_DATA[1136].ToIntDef(0) == 0) { // �[�ǖ���
            //    // ***************************
            //    Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight(), 0,
            //        &s_Spec);
            //}
            //else { // �[�ǗL��
            //    // 2004.11.26 ALBC�[�ǗpSTUD�ǉ�
            //    // Get_SSTDBltSpec(s_Key + "-"+ P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //    if (s_Key == "UX-90" || s_Key == "RX-70") { // UX-90,RX-70
            //        // 2016.08.30 �ގ����ޕύX
            //        //if (P_HEADS_DATA[1060].Pos("1043") >
            //        //    0 || P_HEADS_DATA[1060].Pos("1151") > 0)
            //        //{ // 1043 CAC703  1151 STKM13A
            //        //    if (P_HEADS_DATA[1061].Pos("1043") >
            //        //        0 || P_HEADS_DATA[1061].Pos("1151") > 0)
            //        //    { // 1043 CAC703  1151 STKM13A
            //        //        // B��A���Ƃ�ALBC�[��
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //        //    }
            //        //    else {
            //        //        // B��ALBC�[��
            //        //        P_Num = P_Num / 2;
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //        //        Write_Funeyou_Data(s_Code, s_Spec);
            //        //        // A�����[��
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //        //    }
            //        //}
            //        //else {
            //        //    if (P_HEADS_DATA[1061].Pos("1043") >
            //        //        0 || P_HEADS_DATA[1061].Pos("1151") > 0)
            //        //    { // 1043 CAC703  1151 STKM13A
            //        //        // B�����[��
            //        //        P_Num = P_Num / 2;
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //        //        Write_Funeyou_Data(s_Code, s_Spec);
            //        //        // A��ALBC�[��
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //        //    }
            //        //    else {
            //        //        // B��A���Ƃ����[��
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //        //    }
            //        //}
            //        s_TextB = P_HEADS_DATA[1060].SubString(1,4);
            //        s_TextB = Type_MAT(s_TextB);
            //        s_TextA = P_HEADS_DATA[1061].SubString(1,4);
            //        s_TextA = Type_MAT(s_TextA);
            //        if ( s_TextB == "ALBC" || s_TextB == "CS" ) {
            //            if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
            //                // B��A���Ƃ�ALBC�[��
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //            }
            //            else {
            //                // B��ALBC�[��
            //                P_Num = P_Num / 2;
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //                Write_Funeyou_Data(s_Code, s_Spec);
            //                // A�����[��
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //            }
            //        }
            //        else {
            //            if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
            //                // B�����[��
            //                P_Num = P_Num / 2;
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //                Write_Funeyou_Data(s_Code, s_Spec);
            //                // A��ALBC�[��
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //            }
            //            else {
            //                // B��A���Ƃ����[��
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //            }
            //        }
            //        // ***********************
            //    }
            //    else {
            //        Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight
            //            (), 1, &s_Spec);
            //    }
            //    // *****************************
            //}
            s_TextB = P_HEADS_DATA[1060].SubString(1,4);
            s_TextB = Type_MAT(s_TextB);
            s_TextA = P_HEADS_DATA[1061].SubString(1,4);
            s_TextA = Type_MAT(s_TextA);
            // 2019.05.15 SGP�[�ǒǉ�_S
            if ( s_TextB == "SGP") {   // �r�f�o
                // SGP�[�ǂ̏ꍇ����ޓW�J�p�~
                s_TextB == "-";
            }
            if ( s_TextA == "SGP") {   // �r�f�o
                // SGP�[�ǂ̏ꍇ����ޓW�J�p�~
                  s_TextA == "-";
            }
            // 2019.04.12 SGP�[�ǒǉ�_E
            if ( s_TextA == "-" && s_TextB == "-" ) { // �����[�ǖ���
                // 2018.03.20 �����ݸޒ[�ǖ����̏ꍇ����ޓW�J�p�~_S
                //Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight(), 0,
                //    &s_Spec);
                wkQuery->Next();
                continue;
                // 2018.03.20 �����ݸޒ[�ǖ����̏ꍇ����ޓW�J�p�~_E
            }
            else if ( s_TextB == "-" ) { // B���[�ǖ���
                P_Num = P_Num / 2;
                // B���[�ǖ���
                Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight(), 0,
                    &s_Spec);
                Write_Funeyou_Data(s_Code, s_Spec);
                if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
                    // A��ALBC�[��
                    Get_SSTDBltSpec(s_Key + "-" +
                        P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                }
                else {
                    // 2021.09.30 �ݸޒ[�ǖ����̏ꍇ����ޓW�J�p�~_S
                    //Write_Funeyou_Data(s_Code, s_Spec);
                    // 2021.09.30 �ݸޒ[�ǖ����̏ꍇ����ޓW�J�p�~_E
                    // A�����[��
                    Get_SSTDBltSpec(s_Key + "-" +
                        P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                }
            }
            else if ( s_TextA == "-" ) { // A���[�ǖ���
                P_Num = P_Num / 2;
                // A���[�ǖ���
                Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight(), 0,
                    &s_Spec);
                Write_Funeyou_Data(s_Code, s_Spec);
                if ( s_TextB == "ALBC" || s_TextB == "CS" ) {
                    // B��ALBC�[��
                    Get_SSTDBltSpec(s_Key + "-" +
                        P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                }
                else {
                    // 2021.09.30 �ݸޒ[�ǖ����̏ꍇ����ޓW�J�p�~_S
                    //Write_Funeyou_Data(s_Code, s_Spec);
                    // 2021.09.30 �ݸޒ[�ǖ����̏ꍇ����ޓW�J�p�~_E
                    // B�����[��
                    Get_SSTDBltSpec(s_Key + "-" +
                        P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                }
            }
            else { // �����[�ǗL��
                if (s_Key == "UX-90" ||s_Key == "SX-80S" || s_Key == "SX-80M"||
                    s_Key == "SX-80L" || s_Key == "RX-70" ) {
                    // UX-90,SX-80S,SX-80M,SX-80L,RX-70
                    if ( s_TextB == "ALBC" || s_TextB == "CS" ) {
                        if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
                            // B��A���Ƃ�ALBC�[��
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                        }
                        else {
                            // B��ALBC�[��
                            P_Num = P_Num / 2;
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                            Write_Funeyou_Data(s_Code, s_Spec);
                            // A�����[��
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                        }
                    }
                    else {
                        if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
                            // B�����[��
                            P_Num = P_Num / 2;
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                            Write_Funeyou_Data(s_Code, s_Spec);
                            // A��ALBC�[��
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                        }
                        else {
                            // B��A���Ƃ����[��
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                        }
                    }
                }
                else {
                    Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight
                        (), 1, &s_Spec);
                }
            }
            // 2017.11.22 �Б��ݸޒ[��_E
        }
        // ��ڰ�
        // 2019.09.09 E������ůĒǉ�_S�@
        //if (s_Code == "17" || s_Code == "27") {
        if (s_Code == "17") {
        // 2019.09.09 E������ůĒǉ�_E�@
            // �e�m�Y����ނɽ�ڰł��ݒ肳��Ă��Ȃ�
            if ((P_HEADS_DATA[891].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[906].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[921].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[936].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[951].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[966].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[981].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[996].ToIntDef(0) != 5)) {
                wkQuery->Next();
                continue;
            }
            // 2009.10.28 �X�g���[�i�͏����ɂ���Đ}�Ԃ��ς��ꍇ������
            //// �X�g���[�i�͏����ɂ���Đ}�Ԃ��ς��ꍇ������ 03-01-08
            //// 2003.10.22 �^���ǉ� RX-11,RX-13
            ////if ( s_Key == "RX-10" ) {                    // RX-10
            // if ( s_Key == "RX-10" || s_Key == "RX-11" || s_Key == "RX-13" ) { // RX-10,RX-11,RX-13
            ////********************************
            // // 2004.04.15 RX-10-TNJM,TNPM,TNHJM,TNHPM�ǉ�
            // if (P_HEADS_DATA[36].TrimRight() == "NPM" || P_HEADS_DATA[36].TrimRight() == "NHPM" || P_HEADS_DATA[36].TrimRight() == "TNPM" || P_HEADS_DATA[36].TrimRight() == "TNHPM") {                                      // �t���[�����m�o�l�A�m�g�o�l
            // // �t���[�����m�o�l�A�m�g�o�l�A�s�m�o�l�A�s�m�g�o�l
            // } else if (P_HEADS_DATA[36].TrimRight() == "NJM" || P_HEADS_DATA[36].TrimRight() == "NHJM" || P_HEADS_DATA[36].TrimRight() == "TNJM" || P_HEADS_DATA[36].TrimRight() == "TNHJM") {
            // // �t���[�����m�i�l�A�m�g�i�l�A�s�m�i�l�A�s�m�g�i�l
            // // �}�ԂQ��K�p
            // P_Zuban    = s_Dno2;
            // P_SZuban   = s_WKDno2;
            // } else {
            // G_ErrLog_Text = "�X�g���[�i�����ɊY�����܂���B";
            // Write_Error_Log(G_ErrLog_Text);
            // wkQuery->Next();
            // continue;
            // }
            // //if (P_HEADS_DATA[36].TrimRight() == "NPM" || P_HEADS_DATA[36].TrimRight() == "NHPM") {                                      // �t���[�����m�o�l�A�m�g�o�l
            // //} else if (P_HEADS_DATA[36].TrimRight() == "NJM" || P_HEADS_DATA[36].TrimRight() == "NHJM") {                               // �t���[�����m�i�l�A�m�g�i�l
            // //   // �}�ԂQ��K�p
            // //   P_Zuban    = s_Dno2;
            // //   P_SZuban   = s_WKDno2;
            // //} else {
            // //   G_ErrLog_Text = "�X�g���[�i�����ɊY�����܂���B";
            // //   Write_Error_Log(G_ErrLog_Text);
            // //   wkQuery->Next();
            // //   continue;
            // //}
            // //*******************************************
            // } else if ( s_Key == "UX-90" ) {             // UX-90
            // //2007.02.13 �ް��ʒu�ύX
            // //if (P_HEADS_DATA[159].ToIntDef(0) > 350) {   // �����������R�T�P���ȏ�
            // if (P_HEADS_DATA[38].ToIntDef(0) > 350) {   // �����������R�T�P���ȏ�
            // //***********************
            // // �}�ԂQ��K�p
            // P_Zuban    = s_Dno2;
            // P_SZuban   = s_WKDno2;
            // }
            // }
            if (s_Key == "UX-90" || s_Key == "SX-90M") {
                // UX-90,SX-90M
                if (P_HEADS_DATA[38].ToIntDef(0) > 350) {
                    // ����������351���ȏ�
                    // 2016.11.07 �ގ����ޕύX
                    //if (P_HEADS_DATA[261].Pos("1003") > 0) {
                    //    // ��ڰł�SUS316�̏ꍇ��E359301
                    //    // 2014.06.24 �}�ԕύX
                    //    // P_Zuban    = "�d�R�T�X�R�O�P";
                    //    P_Zuban = "�d�R�T�X�R�O�P�|�q�P";
                    //    // *******************
                    //    P_SZuban = "�d�R�T�X�R�O�P";
                    //}
                    //else if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    //    // ��ڰł�TP340�̏ꍇ��E359302
                    //    // 2014.06.24 �}�ԕύX
                    //    // P_Zuban    = "�d�R�T�X�R�O�Q";
                    //    P_Zuban = "�d�R�T�X�R�O�Q�|�q�Q";
                    //    // *******************
                    //    P_SZuban = "�d�R�T�X�R�O�Q";
                    //}
                    if (Type_MAT(P_HEADS_DATA[261].TrimRight()) == "SUS316") {
                        // ��ڰł�SUS316�̏ꍇ
                        P_Zuban = "�d�R�T�X�R�O�P�|�q�P";
                        P_SZuban = "�d�R�T�X�R�O�P";
                    }
                    else if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                             P_HEADS_DATA[261].Pos("3032") > 0 ||
                             P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ��ڰł�TP340�̏ꍇ
                        P_Zuban = "�d�R�T�X�R�O�Q�|�q�Q";
                        P_SZuban = "�d�R�T�X�R�O�Q";
                    }
                    // ***********************
                    else {
                        P_Zuban = "";
                        P_SZuban = "";
                    }
                }
                else {
                    // ����������350���ȉ�
                    // 2016.11.07 �ގ����ޕύX
                    //if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                        P_HEADS_DATA[261].Pos("3032") > 0 ||
                        P_HEADS_DATA[261].Pos("3132") > 0) {
                    // ***********************
                        // ��ڰł�TP340�̏ꍇ�͐}�ԂQ
                        P_Zuban = s_Dno2;
                        P_SZuban = s_WKDno2;
                    }
                }
                // 2014.06.24 RX-90��ڰŒǉ�
            }
            else if (s_Key == "RX-90") {
                // RX-90
                if (P_HEADS_DATA[38].ToIntDef(0) > 291) {
                    // ����������291���ȏ�
                    // 2016.11.07 �ގ����ޕύX
                    //if (P_HEADS_DATA[261].Pos("1003") > 0) {
                    //    // ��ڰł�SUS316�̏ꍇ��E359338
                    //    P_Zuban = "�d�R�T�X�R�R�W�|�q�Q";
                    //    P_SZuban = "�d�R�T�X�R�R�W";
                    //}
                    //else if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    //    // ��ڰł�TP340�̏ꍇ��E359572
                    //    P_Zuban = "�d�R�T�X�T�V�Q�|�q�R";
                    //    P_SZuban = "�d�R�T�X�T�V�Q";
                    //}
                    if (Type_MAT(P_HEADS_DATA[261].TrimRight()) == "SUS316") {
                        // ��ڰł�SUS316�̏ꍇ
                        P_Zuban = "�d�R�T�X�R�R�W�|�q�Q";
                        P_SZuban = "�d�R�T�X�R�R�W";
                    }
                    else if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                             P_HEADS_DATA[261].Pos("3032") > 0 ||
                             P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ��ڰł�TP340�̏ꍇ
                        P_Zuban = "�d�R�T�X�T�V�Q�|�q�R";
                        P_SZuban = "�d�R�T�X�T�V�Q";
                    }
                    // ***********************
                    else {
                        P_Zuban = "";
                        P_SZuban = "";
                    }
                }
                else {
                    // ����������290���ȉ�
                    // 2016.11.07 �ގ����ޕύX
                    //if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                        P_HEADS_DATA[261].Pos("3032") > 0 ||
                        P_HEADS_DATA[261].Pos("3132") > 0) {
                    // ***********************
                        // ��ڰł�TP340�̏ꍇ�͐}�ԂQ
                        P_Zuban = s_Dno2;
                        P_SZuban = s_WKDno2;
                    }
                }
                // *************************
            }
            else if (s_Key != "UX-01" && s_Key != "UX-10" && s_Key != "UX-20" &&
                s_Key != "UX-40" && s_Key != "LX-00" && s_Key != "LX-10" &&
                s_Key != "LX-20" && s_Key != "LX-40" && s_Key != "RX-90") {
                // UX-01,UX-10,UX-20,UX-40,LX-00,LX-10,LX-20,LX-40�ȊO
                // 2016.11.07 �ގ����ޕύX
                //if (P_HEADS_DATA[261].Pos("1032") > 0) {
                if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                    P_HEADS_DATA[261].Pos("3032") > 0 ||
                    P_HEADS_DATA[261].Pos("3132") > 0) {
                // ***********************
                    // ��ڰł�TP340�̏ꍇ�͐}�ԂQ
                    P_Zuban = s_Dno2;
                    P_SZuban = s_WKDno2;
                }
            }
            // **********************************************************

            if (s_Code == "17") { // ��ڰł̏ꍇ�ͽ�ڰōގ����o��
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[261].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[261].TrimRight());
            }
        }

        // ��2002/10/24

        // 2010.04.19 UX-90 ����ڰĐ}��
        // UX-90�̏ꍇ�ψ��敪�Ő}�Ԃ𕪂���
        // ����ڰ�
        if (s_Code == "20" || s_Code == "21") {
            if (s_Key == "UX-90") {
                // UX-90
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER��200207�̏ꍇ�͐}��2
                    P_Zuban = s_Dno2;
                    P_SZuban = s_WKDno2;
                }
                // 2012.08.23 HEADS.VER�ǉ�
                if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER��300207�̏ꍇ�͐}��2
                    P_Zuban = s_Dno2;
                    P_SZuban = s_WKDno2;
                }
                // ************************
            }
        }
        // ****************************

        // 2022.09.30 E��-��ް���ڰĒǉ�_S
        if (s_Code == "29") {
            if (s_Key == "UX-90") {
                // UX-90
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VER��200207�̏ꍇ�͐}�Ԗ���
                    P_Zuban = "";
                    P_SZuban = "";
                }
                // 2012.08.23 HEADS.VER�ǉ�
                if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VER��300207�̏ꍇ�͐}�Ԗ���
                    P_Zuban = "";
                    P_SZuban = "";
                }
            }

            if ( P_Zuban == "" ) { // �}�Ԗ���
                wkQuery->Next();
                continue;
            }

            if ( G_EBASE_FLG == 0 || P_HEADS_DATA[1382].ToIntDef(0) == 0 ) {
                // E��-��ް���ڰďo�͕s�v
                wkQuery->Next();
                continue;
            }

        }
        // 2022.09.30 E��-��ް���ڰĒǉ�_E


        Write_Funeyou_Data(s_Code, s_Spec);
        wkQuery->Next();
    }

    delete wkQuery;


    // 2015.03.19 OGI ���p���i�A�L���[�����[�^�o�͑Ή�
    if ((P_HEADS_DATA[981].ToIntDef(0) == 8 ) ||
        (P_HEADS_DATA[996].ToIntDef(0) == 8)) {

        AnsiString s_Text;
        s_Text = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
            P_HEADS_DATA[1267].TrimRight();

        P_Sikibetu = "ACCU-E"; // ���i����

        P_Katasiki = "�A�L�������[�^"; // ���i�^��
        Search_HD_FRM_NZ_Accum(s_Text, "E", 76);
        Write_Funeyou_Data("76", "");

        P_Katasiki = "�A�L�������[�^�T�|�[�g"; // ���i�^��
        Search_HD_FRM_NZ_Accum(s_Text, "E", 77);
        Write_Funeyou_Data("77", "");

        // 2015.08.04 ����ڰ���t���i�ǉ�
        P_Katasiki = "�A�L�������[�^��t���i"; // ���i�^��
        Search_HD_FRM_NZ_Accum(s_Text, "E", 78);
        Write_Funeyou_Data("78", "");
        // ******************************
    }

    return true;
}

// 2015.03.19 OGI ���p���i�A�L���[�����[�^�o�͑Ή�
// ---------------------------------------------------------------------------
// ���{��֐����F HD_FRM_NZð��ٌ��������i�A�L���[�����[�^��p�j
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Search_HD_FRM_NZ_Accum(AnsiString Key,
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
        P_EZaisitu = "";
        P_Zuban = "";
        P_SZuban = "";
        P_Num = 0;
        P_Yobisu = 0;
        P_Siyou = "";
        P_Siyou2 = "";
        P_Siyou5 = "";
        P_Unit = "";

        delete wkQuery;
        return false;
    }
    else {
        if( P_HEADS_DATA[1116].ToIntDef(0)==2 ) {  // 2L
            // �ގ�����
            P_Zaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight());
            P_EZaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight());
            // �}��
            P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight(); // �}��
            // ��}�p�}��
            P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        }
        else if( P_HEADS_DATA[1116].ToIntDef(0)==3 ) {  // 3L
            // �ގ�����
            P_Zaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE2")->AsString.TrimRight());
            P_EZaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE2")->AsString.TrimRight());
            // �}��
            P_Zuban = wkQuery->FieldByName("DNO2")->AsString.TrimRight(); // �}��
            // ��}�p�}��
            P_SZuban = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        }
        else if( P_HEADS_DATA[1116].ToIntDef(0)==5 ) {  // 5L
            // �ގ�����
            P_Zaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE3")->AsString.TrimRight());
            P_EZaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE3")->AsString.TrimRight());
            // �}��
            P_Zuban = wkQuery->FieldByName("DNO3")->AsString.TrimRight(); // �}��
            // ��}�p�}��
            P_SZuban = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        }
        else
        {
            // �ގ�����
            P_Zaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight());
            P_EZaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight());
            // �}��
            P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight(); // �}��
            // ��}�p�}��
            P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        }

        // ����
        P_Num = 1;
        // �\����
        P_Yobisu = 0;
        // �d�l
        P_Yobisu = 0;
        P_Siyou = "";
        P_Siyou2 = "";
        P_Siyou5 = "";
        P_Unit = "";
    }

    delete wkQuery;
    return true;
}



// ---------------------------------------------------------------------------
// ���{��֐����F �t���i�ް�̧�ُ����ݏ���
// �T  �v      �F �������,�ݶ�����
// ��  ��      �F AnsiString a_SE   �F ���͌vSE�敪(S1�`S4,E1�`E4)
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Write_Axsrs_Data(AnsiString a_SE) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;

    AnsiString wk_FilePass;

    // ----------------------------------------------
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    AnsiString sSpecCode;
    // **********************************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
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
    s_Text = "ACCS";
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
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), wkfp);

    // 13 �d�l�R
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 14 �d�l�S
    s_Text = "," + P_Unit;
    fputs(s_Text.c_str(), wkfp);

    // 15 �d�l�T
    s_Text = "," + P_Siyou5;
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
// ���{��֐����F ���p�t���i�ް�̧�ُ����ݏ���
// �T  �v      �F
// ��  ��      �F AnsiString s_Code   �F ���p���i����
// �F AnsiString s_Spec   �F ���i��߯�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Write_Funeyou_Data(AnsiString s_Code,
    AnsiString s_Spec) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;

    AnsiString wk_FilePass;

    // ----------------------------------------------
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    AnsiString sSpecCode;
    // **********************************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
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
    s_Text = "SHIP";
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
    if (s_Code == "16")
        // RecNo[1126] �ް�ނ̒���
            s_Text = ",�k��" + HanToZen(P_HEADS_DATA[1126].Trim()) + " �`��������" +
            HanToZen(P_HEADS_DATA[1089].Trim()) + " �`��������" +
            HanToZen(P_HEADS_DATA[1015].Trim());
    else if (s_Code == "17")
        // RecNo[1126] ��ڰŒ����P
        // if ( ( P_HEADS_DATA[1124].TrimRight() != "" ) && (P_HEADS_DATA[1125].TrimRight() != "") ){
        // if ( P_HEADS_DATA[1124].ToDouble() > P_HEADS_DATA[1125].ToDouble() ) {
        // s_Text = ",�k�P��" + HanToZen( P_HEADS_DATA[1124].Trim() );
        // } else {
        // s_Text = ",�k�P��" + HanToZen( P_HEADS_DATA[1125].Trim() );
        // }
        // } else if (P_HEADS_DATA[1125].TrimRight() != "") {
        // s_Text = ",�k�P��" + HanToZen( P_HEADS_DATA[1125].Trim() );
        // } else {
            s_Text = ",�k�P��" + HanToZen(P_HEADS_DATA[1124].Trim());
    // }
    else
        s_Text = "," + s_Spec;
    fputs(s_Text.c_str(), wkfp);

    // 12 �d�l�Q
    if (s_Code == "16")
        s_Text = ",�e�X�g��ō�";
    else if (s_Code == "17")
        // RecNo[1126] ��ڰŒ����P
        // 2009.12.01 L2���@�ύX
        // s_Text = ",�k�Q��" + HanToZen( P_HEADS_DATA[1125].Trim() );
        if (P_HEADS_DATA[1125].Trim() != "0") {
            s_Text = ",�k�Q��" + HanToZen(P_HEADS_DATA[1125].Trim());
        }
        else {
            s_Text = ",";
        }
    // *********************
    else
        s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 13 �d�l�R
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 14 �d�l�S
    if (s_Code == "14")
        s_Text = ",��";
    else if (s_Code == "18" || s_Code == "27")
        s_Text = ",�g";
    // 2010.04.19 UX-90 ����ڰ�
    else if (s_Code == "20" || s_Code == "21") {
        // 1�䕪����1�ȉ��̏ꍇ"��"��t����
        if (P_Num <= 1) {
            s_Text = ",��";
        }
    }

    // 2015.03.19 OGI ���p���i�A�L���[�����[�^�o�͑Ή�
    // 2015.08.04 ����ڰ���t���i�ǉ�
    //else if (s_Code == "76" || s_Code == "77")
    else if (s_Code == "76" || s_Code == "77" || s_Code == "78")
    // ******************************
        s_Text = ",��";

    // ************************

    // 2022.09.30 E��-��ް���ڰĒǉ�_S
    else if (s_Code == "29")
         s_Text = ",��";
    // 2022.09.30 E��-��ް���ڰĒǉ�_E

    else
        s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 15 �d�l�T
    // 2003.11.25 �`�a�ύX
    // if( s_Code == "17" ){  //��ڰ�
    // // RecNo[1071]�a���̖�
    // if( P_HEADS_DATA[1071].TrimRight() == "�C��" || P_HEADS_DATA[1071].TrimRight() == "SEA WATER" ) {
    // s_Text = ",B";
    // }
    // else {
    // s_Text = ",A";
    // }
    // }else {
    // s_Text = ",";
    // }
    if (s_Code == "17") { // ��ڰ�
        // RecNo[951]�d�P�m�Y�����
        // RecNo[981]�d�R�m�Y�����
        if (P_HEADS_DATA[951].TrimRight() == "5" || P_HEADS_DATA[981].TrimRight
            () == "5") {
            s_Text = ",B";
        }
        else {
            s_Text = ",A";
        }
        // 2004.06.29 ����ݼޤ�~��ݼ�AB�敪�ǉ�
        // 2008.01.08 ��Ķ޽��Ă�AB�敪�ǉ�
        // }else if ( s_Code == "22" || s_Code == "23" ){
    }
    else if (s_Code == "22" || s_Code == "23" || s_Code == "24" ||
        s_Code == "25" || s_Code == "26") {
        // **************************************
        // ********************************
        s_Text = "," + P_Siyou5;
    }
    else {
        s_Text = ",";
    }
    // *******************
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
// ���{��֐����F ���t�����W���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_IFrng_Data(void) {
    AnsiString s_Code; // ���i����
    int iCntA;
    int iCntB;
    int iBltNum;
    // 2010.07.14 �΍s���ǉ�
    int iFL;
    // *********************
    //2017.04.20 ASME,JPI�ގ��ύX
    int i_KikakuS1;
    int i_KikakuS3;
    int i_KikakuE1;
    int i_KikakuE3;
    //***************************
    //2018.11.08 ���t�����W�d�l�ύX_S
    int i_KikakuS2;
    int i_KikakuS4;
    int i_KikakuE2;
    int i_KikakuE4;
    //2018.11.08 ���t�����W�d�l�ύX_E
    //2017.10.10 ��Ķ޽��čގ��ύX_S
    AnsiString s_GasMat;
    //2017.10.10 ��Ķ޽��čގ��ύX_E

    // 2018.05.28  ��Ķ޽��ē\�����敪_S
    AnsiString sSpecCode;
    // **********************************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
    // 2018.05.28  ��Ķ޽��ē\�����敪_E

    // 2016.11.07 �ގ����ޕύX
    //if (P_HEADS_DATA[1068].TrimRight() == "0000") { // ���t�����W�ގ����O�O�O�O�̏ꍇ�͖���
    if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "-") {
        // ���t�����W�ގ����O�O�O�O�̏ꍇ�͖���
    // ***********************
        return false;
    }
    iCntA = 0;
    iCntB = 0;
    // 2010.07.14 �΍s���ǉ�
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
        iFL = 1; // �΍s��
    }
    // 2022.11.15 HERP�C��_S
    else if  (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
        iFL = 1; // �΍s��
    }
    // 2022.11.15 HERP�C��_E
    else {
        iFL = 0; // ��`��
    }
    // ���ʎZ�o�i�T�j�^���[�ɂ͕t���Ȃ��j
    // S1
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
    // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight()
        == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        iCntB += 1;
    }
    // S2
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
    // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight()
        == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        iCntA += 1;
    }
    // S3
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
    // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight()
        == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) {
            iCntB += 1; // ��`��
        }
        else {
            iCntA += 1; // �΍s��
        }
    }
    // S4
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
    // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight()
        == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) {
            iCntA += 1; // ��`��
        }
        else {
            iCntB += 1; // �΍s��
        }
    }
    // E1
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
    // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight()
        == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        iCntB += 1;
    }
    // E2
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
    // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight()
        == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        iCntA += 1;
    }
    // E3
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
    // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight()
        == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) {
            iCntB += 1; // ��`��
        }
        else {
            iCntA += 1; // �΍s��
        }
    }
    // E4
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
    // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight()
        == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) {
            iCntA += 1; // ��`��
        }
        else {
            iCntB += 1; // �΍s��
        }
    }
    ////���ʎZ�o�i�T�j�^���[�ɂ͕t���Ȃ��j
    // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
    // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
    // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
    // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
    // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
    // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
    // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
    // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
    // *********************
    // �ގ����ނ̎擾
    P_Sikibetu = "IFRG--"; // ���i����
    P_Katasiki = "���t�����W"; // ���i�^��
    P_Zuban = ""; // �}�Łi�w���i�Ȃ̂Ő}�Ԃ͖����j
    P_SZuban = ""; // ��}�p�}�ԁi�w���i�Ȃ̂Ő}�Ԃ͖����j
    P_Yobihin = ""; // �\���i���ʎq
    //2018.11.08 ���t�����W�d�l�ύX_S
    //P_Siyou = P_HEADS_DATA[1368].TrimRight(); // �d�l
    P_Siyou = P_HEADS_DATA[1369].TrimRight(); // �d�l
    //2018.11.08 ���t�����W�d�l�ύX_E
    P_Siyou2 = ""; // �d�l2
    P_Siyou5 = "A";
    P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1068].TrimRight());
    P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[1068].TrimRight());
    //2017.04.20 ASME,JPI�ގ��ύX
    //2018.11.08 ���t�����W�d�l�ύX_S
    ////S1,S3,E1,E3�K�i��ASME,JPI�̏ꍇS25C
    //i_KikakuS1 = 0;
    //i_KikakuS1 = GetNzKikaku(P_HEADS_DATA[893]);
    //i_KikakuS3 = 0;
    //i_KikakuS3 = GetNzKikaku(P_HEADS_DATA[923]);
    //i_KikakuE1 = 0;
    //i_KikakuE1 = GetNzKikaku(P_HEADS_DATA[953]);
    //i_KikakuE3 = 0;
    //i_KikakuE3 = GetNzKikaku(P_HEADS_DATA[983]);
    //if ( i_KikakuS1 == 1 || i_KikakuS3 == 1 || i_KikakuE1 == 1 || i_KikakuE3 == 1 ) {
    //S2,S4,E2,E4�K�i��ASME,JPI�̏ꍇS25C
    i_KikakuS2 = 0;
    i_KikakuS2 = GetNzKikaku(P_HEADS_DATA[908]);
    i_KikakuS4 = 0;
    i_KikakuS4 = GetNzKikaku(P_HEADS_DATA[938]);
    i_KikakuE2 = 0;
    i_KikakuE2 = GetNzKikaku(P_HEADS_DATA[968]);
    i_KikakuE4 = 0;
    i_KikakuE4 = GetNzKikaku(P_HEADS_DATA[998]);
    if ( i_KikakuS2 == 1 || i_KikakuS4 == 1 || i_KikakuE2 == 1 || i_KikakuE4 == 1 ) {
    //2018.11.08 ���t�����W�d�l�ύX_E
        // ASME,JPI
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "SS400") {
        // ���t�����W�ގ���SS400�̏ꍇS25C�ɕύX
            P_Zaisitu="S25C";
            P_EZaisitu="S25C";
        }
    }
    //***************************
    P_Num = iCntA;
    P_Unit = "";
    P_Hosoku = "";

    // *********************************
    // ***  �\���i�� �擾
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // �\����

    // ���O
    G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
    Write_Log(G_Log_Text);

    Write_Axsrs_Data(s_Code);

    //2018.11.08 ���t�����W�d�l�ύX_S
    P_Siyou = P_HEADS_DATA[1368].TrimRight(); // �d�l
    P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1068].TrimRight());
    P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[1068].TrimRight());
    //S1,S3,E1,E3�K�i��ASME,JPI�̏ꍇS25C
    i_KikakuS1 = 0;
    i_KikakuS1 = GetNzKikaku(P_HEADS_DATA[893]);
    i_KikakuS3 = 0;
    i_KikakuS3 = GetNzKikaku(P_HEADS_DATA[923]);
    i_KikakuE1 = 0;
    i_KikakuE1 = GetNzKikaku(P_HEADS_DATA[953]);
    i_KikakuE3 = 0;
    i_KikakuE3 = GetNzKikaku(P_HEADS_DATA[983]);
    if ( i_KikakuS1 == 1 || i_KikakuS3 == 1 || i_KikakuE1 == 1 || i_KikakuE3 == 1 ) {
        // ASME,JPI
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "SS400") {
        // ���t�����W�ގ���SS400�̏ꍇS25C�ɕύX
            P_Zaisitu="S25C";
            P_EZaisitu="S25C";
        }
    }
    //2018.11.08 ���t�����W�d�l�ύX_E
    P_Siyou5 = "B";
    P_Num = iCntB;
    Write_Axsrs_Data(s_Code);

    // *********************************
    // ***  �t���i�ް�(��Ķ޽���)�@�쐬
    // *********************************
    iCntA = 0;
    iCntB = 0;
    // ���ʎZ�o�i�T�j�^���[�ɂ͕t���Ȃ��j
    // 2010.07.14 �΍s���ǉ�
    // S1
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
    // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {             // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1136].ToIntDef(0) != 0 ) {                         // �[�ǂ��t��
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
    if ((P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight()
        == "2" || P_HEADS_DATA[891].TrimRight() == "3") &&
        (P_HEADS_DATA[893].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[891].TrimRight() == "3") { // �G�A�����t��
            if (P_HEADS_DATA[893].TrimRight().SubString(3,
                1) != "1" && P_HEADS_DATA[893].TrimRight().SubString(3,
                1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                iCntB += 1;
            }
            else {
                iCntB += 0;
            }
        }
        else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1")
        { // ���o�[�u�[�c
            if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 1;
            }
            else {
                iCntB += 0;
            }
        }
        else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntB += 1;
        }
        else {
            if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 2;
            }
            else {
                iCntB += 1;
            }
        }
    }
    // S2
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
    // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {             // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1137].ToIntDef(0) != 0 ) {                         // �[�ǂ��t��
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
    if ((P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight()
        == "2" || P_HEADS_DATA[906].TrimRight() == "3") &&
        (P_HEADS_DATA[908].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[906].TrimRight() == "3") { // �G�A�����t��
            if (P_HEADS_DATA[908].TrimRight().SubString(3,
                1) != "1" && P_HEADS_DATA[908].TrimRight().SubString(3,
                1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                iCntA += 1;
            }
            else {
                iCntA += 0;
            }
        }
        else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1")
        { // ���o�[�u�[�c
            if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 1;
            }
            else {
                iCntA += 0;
            }
        }
        else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntA += 1;
        }
        else {
            if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 2;
            }
            else {
                iCntA += 1;
            }
        }
    }
    // S3
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
    // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( iFL == 0 ) {                                                        // ��`��
    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // } else {
    // if ( P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // } else {                                                                 // �΍s��
    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // } else {
    // if ( P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // }
    if ((P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight()
        == "2" || P_HEADS_DATA[921].TrimRight() == "3") &&
        (P_HEADS_DATA[923].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1, 2) != "07")) {
        if (iFL == 0) { // ��`��
            if (P_HEADS_DATA[921].TrimRight() == "3") { // �t�����t��
                if (P_HEADS_DATA[923].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[923].TrimRight().SubString(3,
                    1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                iCntB += 1;
            }
            else {
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 2;
                }
                else {
                    iCntB += 1;
                }
            }
        }
        else { // �΍s��
            if (P_HEADS_DATA[921].TrimRight() == "3") { // �t�����t��
                if (P_HEADS_DATA[923].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[923].TrimRight().SubString(3,
                    1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                iCntA += 1;
            }
            else {
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 2;
                }
                else {
                    iCntA += 1;
                }
            }
        }
    }
    // ***********************
    // S4
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
    // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( iFL == 0 ) {                                                        // ��`��
    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // } else {
    // if ( P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // } else {                                                                 // �΍s��
    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // } else {
    // if ( P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // }
    if ((P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight()
        == "2" || P_HEADS_DATA[936].TrimRight() == "3") &&
        (P_HEADS_DATA[938].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1, 2) != "07")) {
        if (iFL == 0) { // ��`��
            if (P_HEADS_DATA[936].TrimRight() == "3") { // �t�����t��
                if (P_HEADS_DATA[938].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[938].TrimRight().SubString(3,
                    1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                iCntA += 1;
            }
            else {
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 2;
                }
                else {
                    iCntA += 1;
                }
            }
        }
        else { // �΍s��
            if (P_HEADS_DATA[936].TrimRight() == "3") { // �t�����t��
                if (P_HEADS_DATA[938].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[938].TrimRight().SubString(3,
                    1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                iCntB += 1;
            }
            else {
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 2;
                }
                else {
                    iCntB += 1;
                }
            }
        }
    }
    // ***********************
    // E1
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
    // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {             // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1140].ToIntDef(0) != 0 ) {                         // �[�ǂ��t��
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
    if ((P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight()
        == "2" || P_HEADS_DATA[951].TrimRight() == "3") &&
        (P_HEADS_DATA[953].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[951].TrimRight() == "3") { // �G�A�����t��
            if (P_HEADS_DATA[953].TrimRight().SubString(3,
                1) != "1" && P_HEADS_DATA[953].TrimRight().SubString(3,
                1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                iCntB += 1;
            }
            else {
                iCntB += 0;
            }
        }
        else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1")
        { // ���o�[�u�[�c
            if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 1;
            }
            else {
                iCntB += 0;
            }
        }
        else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntB += 1;
        }
        else {
            if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 2;
            }
            else {
                iCntB += 1;
            }
        }
    }
    // E2
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
    // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {             // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1141].ToIntDef(0) != 0 ) {                         // �[�ǂ��t��
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
    if ((P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight()
        == "2" || P_HEADS_DATA[966].TrimRight() == "3") &&
        (P_HEADS_DATA[968].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[966].TrimRight() == "3") { // �G�A�����t��
            if (P_HEADS_DATA[968].TrimRight().SubString(3,
                1) != "1" && P_HEADS_DATA[968].TrimRight().SubString(3,
                1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                iCntA += 1;
            }
            else {
                iCntA += 0;
            }
        }
        else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1")
        { // ���o�[�u�[�c
            if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 1;
            }
            else {
                iCntA += 0;
            }
        }
        else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntA += 1;
        }
        else {
            if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 2;
            }
            else {
                iCntA += 1;
            }
        }
    }
    // E3
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
    // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( iFL == 0 ) {                                                        // ��`��
    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // } else {
    // if ( P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // } else {                                                                 // �΍s��
    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // } else {
    // if ( P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // }
    if ((P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight()
        == "2" || P_HEADS_DATA[981].TrimRight() == "3") &&
        (P_HEADS_DATA[983].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1, 2) != "07")) {
        if (iFL == 0) { // ��`��
            if (P_HEADS_DATA[981].TrimRight() == "3") { // �t�����t��
                if (P_HEADS_DATA[983].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[983].TrimRight().SubString(3,
                    1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                iCntB += 1;
            }
            else {
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 2;
                }
                else {
                    iCntB += 1;
                }
            }
        }
        else { // �΍s��
            if (P_HEADS_DATA[981].TrimRight() == "3") { // �t�����t��
                if (P_HEADS_DATA[983].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[983].TrimRight().SubString(3,
                    1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                iCntA += 1;
            }
            else {
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 2;
                }
                else {
                    iCntA += 1;
                }
            }
        }
    }
    // ***********************
    // E4
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
    // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( iFL == 0 ) {                                                        // ��`��
    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // } else {
    // if ( P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // } else {                                                                 // �΍s��
    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
    // if ( P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // } else {
    // if ( P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {                     // �[�ǂ��t��
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // }
    if ((P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight()
        == "2" || P_HEADS_DATA[996].TrimRight() == "3") &&
        (P_HEADS_DATA[998].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1, 2) != "07")) {
        if (iFL == 0) { // ��`��
            if (P_HEADS_DATA[996].TrimRight() == "3") { // �t�����t��
                if (P_HEADS_DATA[998].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[998].TrimRight().SubString(3,
                    1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                iCntA += 1;
            }
            else {
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 2;
                }
                else {
                    iCntA += 1;
                }
            }
        }
        else { // �΍s��
            if (P_HEADS_DATA[996].TrimRight() == "3") { // �t�����t��
                if (P_HEADS_DATA[998].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[998].TrimRight().SubString(3,
                    1) != "4") { // ���o�[�ł��p�C�v�ł��Ȃ�
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                iCntB += 1;
            }
            else {
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 2;
                }
                else {
                    iCntB += 1;
                }
            }
        }
    }
    // ***********************
    // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1") { //���o�[�u�[�c
    // if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=1;
    // }
    // } else if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // } else {
    // if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1") { //���o�[�u�[�c
    // if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=1;
    // }
    // } else if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // } else {
    // if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") { //���o�[�u�[�c
    // if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=1;
    // }
    // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // } else {
    // if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1") { //���o�[�u�[�c
    // if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=1;
    // }
    // } else if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // } else {
    // if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1") { //���o�[�u�[�c
    // if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=1;
    // }
    // } else if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // } else {
    // if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1") { //���o�[�u�[�c
    // if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=1;
    // }
    // } else if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // } else {
    // if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1") { //���o�[�u�[�c
    // if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=1;
    // }
    // } else if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // } else {
    // if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1") { //���o�[�u�[�c
    // if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=1;
    // }
    // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // } else {
    // if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // *********************

    // �ގ����ނ̎擾
    P_Sikibetu = "STGKT-"; // ���i����
    P_Katasiki = "�V�[�g�K�X�P�b�g"; // ���i�^��
    P_Zuban = ""; // �}�Łi�w���i�Ȃ̂Ő}�Ԃ͖����j
    P_SZuban = ""; // ��}�p�}�ԁi�w���i�Ȃ̂Ő}�Ԃ͖����j
    P_Yobihin = ""; // �\���i���ʎq
    //2018.11.08 ���t�����W�d�l�ύX_S
    //P_Siyou = P_HEADS_DATA[1368].TrimRight(); // �d�l
    P_Siyou = P_HEADS_DATA[1369].TrimRight(); // �d�l
    //2018.11.08 ���t�����W�d�l�ύX_E
    // 2018.05.28  ��Ķ޽��ē\�����敪_S

    //2018.11.08 ���t�����W�d�l�ύX_S
    //if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
    //    iCntA = iCntA + iCntB;
    //    iCntB = 0;
    //}
    if ( (Get_SheetGasketMat("A") == Get_SheetGasketMat("B") ) &&
         (P_HEADS_DATA[1368].TrimRight() == P_HEADS_DATA[1369].TrimRight() ) ) {
        iCntA = iCntA + iCntB;
        iCntB = 0;
    }
    //2018.11.08 ���t�����W�d�l�ύX_E

    if ( iCntA > 0 && iCntB > 0 ) {
        // ��Ķ޽��ē\����(A��)
        if (sSpecCode == "A") {
            // �~���[�d�l �`���@
            P_Siyou = P_Siyou + "�@�`���@�`���^�a���@�a��"; // �d�l
        } else if (sSpecCode == "B") {
            // �~���[�d�l �a���@
            P_Siyou = P_Siyou + "�@�`���@�a���^�a���@�`��"; // �d�l
        } else {
            P_Siyou = P_Siyou + "�@�`���p";                 // �d�l
        }
    }
    // 2018.05.29  ��Ķ޽��ē\�����敪_E
    P_Siyou2 = ""; // �d�l2
    P_Siyou5 = "A";
    //2017.10.10 ��Ķ޽��čގ��ύX_S
    //P_Zaisitu = Search_HD_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[836].TrimRight(), 2));
    //P_EZaisitu = Search_HD_E_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[836].TrimRight(), 2));
    s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ��Ķ޽��čގ�CODE
    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
    //2017.10.10 ��Ķ޽��čގ��ύX_E
    P_Num = iCntA;
    P_Unit = "";
    P_Hosoku = "";

    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
    if ( P_GasNameA == "�����O�K�X�P�b�g" ) {
        // �ݸ޶޽���
        P_Sikibetu = "STGKTR";                   // ���i����
        P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
    } else if ( P_GasNameA == "�S�ʍ��t�����W�K�X�P�b�g" ) {
        // �S�ʍ���ݼ޶޽���
        P_Sikibetu = "STGKTF";                   // ���i����
        P_Katasiki = "�S�ʍ��t�����W�K�X�P�b�g"; // ���i�^��
    } else {
        // ���̑�
    }
    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

    // *********************************
    // ***  �\���i�� �擾
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // �\����

    // ���O
    G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
    Write_Log(G_Log_Text);

    // 2018.05.28  ��Ķ޽��ē\�����敪_S
    //if (iCntA > 0)
    //    Write_Axsrs_Data(s_Code);
    if (iCntA > 0) {
        Write_Axsrs_Data(s_Code);
    }

    //2018.11.08 ���t�����W�d�l�ύX_S
    P_Siyou = P_HEADS_DATA[1368].TrimRight(); // �d�l
    //2018.11.08 ���t�����W�d�l�ύX_E

    if ( iCntB > 0 ) {
        // ��Ķ޽��ē\����(B��)
        if (sSpecCode == "A") {
            // �~���[�d�l �`���@
            P_Siyou = P_Siyou + "�@�`���@�a���^�a���@�`��"; // �d�l
        } else if (sSpecCode == "B") {
            // �~���[�d�l �a���@
            P_Siyou = P_Siyou + "�@�`���@�`���^�a���@�a��"; // �d�l
        } else {
            P_Siyou = P_Siyou + "�@�a���p";                 // �d�l
        }
    }
    // 2018.05.28  ��Ķ޽��ē\�����敪_E

    P_Siyou5 = "B";
    P_Num = iCntB;
    //2017.10.10 ��Ķ޽��čގ��ύX_S
    //P_Zaisitu = Search_HD_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[835].TrimRight(), 2));
    //P_EZaisitu = Search_HD_E_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[835].TrimRight(), 2));
    s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ��Ķ޽��čގ�CODE
    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
    //2017.10.10 ��Ķ޽��čގ��ύX_E
    // 2020.12.16 ����ݼ޼�Ķ޽��ďC��_S
    //if (iCntB > 0)
    if (iCntB > 0){
    // 2020.12.16 ����ݼ޼�Ķ޽��ďC��_E
        // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
        if ( P_GasNameB == "�����O�K�X�P�b�g" ) {
            // �ݸ޶޽���
            P_Sikibetu = "STGKTR";                   // ���i����
            P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
        } else if ( P_GasNameB == "�S�ʍ��t�����W�K�X�P�b�g" ) {
            // �S�ʍ���ݼ޶޽���
            P_Sikibetu = "STGKTF";                   // ���i����
            P_Katasiki = "�S�ʍ��t�����W�K�X�P�b�g"; // ���i�^��
        } else {
            // ���̑�
        }
        // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

        Write_Axsrs_Data(s_Code);

    // 2020.12.16 ����ݼ޼�Ķ޽��ďC��_S
    }
    // 2020.12.16 ����ݼ޼�Ķ޽��ďC��_E

    // *********************************
    // ***  �t���i�ް�(����ů�)�@�쐬
    // *********************************
    iCntA = 0;
    iCntB = 0;
    // ���ʎZ�o�i�T�j�^���[�ɂ͕t���Ȃ��j
    // 2010.07.14 �΍s���ǉ�
    // S1
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
    // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight()
        == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // �[�ǂ��t��
            iCntB += 1;
            // 2011.06.20 ɽ�ًK�i�ύX
            // } else if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
        }
        else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntB += 1;
        }
    }
    // S2
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
    // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight()
        == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // �[�ǂ��t��
            iCntA += 1;
            // 2011.06.20 ɽ�ًK�i�ύX
            // } else if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
        }
        else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntA += 1;
        }
    }
    // S3
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
    // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight()
        == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // ��`��
            if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 1;
                // 2011.06.20 ɽ�ًK�i�ύX
                // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntB += 1;
            }
        }
        else { // �΍s��
            if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 1;
                // 2011.06.20 ɽ�ًK�i�ύX
                // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntA += 1;
            }
        }
    }
    // S4
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
    // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight()
        == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // ��`��
            if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 1;
                // 2011.06.20 ɽ�ًK�i�ύX
                // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntA += 1;
            }
        }
        else { // �΍s��
            if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 1;
                // 2011.06.20 ɽ�ًK�i�ύX
                // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntB += 1;
            }
        }
    }
    // E1
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
    // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight()
        == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // �[�ǂ��t��
            iCntB += 1;
            // 2011.06.20 ɽ�ًK�i�ύX
            // } else if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
        }
        else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntB += 1;
        }
    }
    // E2
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
    // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight()
        == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // �[�ǂ��t��
            iCntA += 1;
            // 2011.06.20 ɽ�ًK�i�ύX
            // } else if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4") {        // �p�C�v�m�Y���i�t�����W�t���j
        }
        else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntA += 1;
        }
    }
    // E3
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
    // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight()
        == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // ��`��
            if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 1;
                // 2011.06.20 ɽ�ًK�i�ύX
                // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntB += 1;
            }
        }
        else { // �΍s��
            if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 1;
                // 2011.06.20 ɽ�ًK�i�ύX
                // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntA += 1;
            }
        }
    }
    // E4
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
    // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight()
        == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // ��`��
            if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 1;
                // 2011.06.20 ɽ�ًK�i�ύX
                // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") {    // �p�C�v�m�Y���i�t�����W�t���j
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntA += 1;
            }
        }
        else { // �΍s��
            if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 1;
                // 2011.06.20 ɽ�ًK�i�ύX
                // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") {    // �p�C�v�m�Y���i�t�����W�t���j
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntB += 1;
            }
        }
    }
    // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=1;
    // } else if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // }
    // }
    // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=1;
    // } else if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // }
    // }
    // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=1;
    // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // }
    // }
    // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=1;
    // } else if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // }
    // }
    // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=1;
    // } else if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // }
    // }
    // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=1;
    // } else if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // }
    // }
    // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntB+=1;
    // } else if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=1;
    // }
    // }
    // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { //�@�[�ǂ��t��
    // iCntA+=1;
    // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") {         //�p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=1;
    // }
    // }
    // *********************

    //2018.11.08 ���t�����W�d�l�ύX_E
    //Get_IFrgBltSize(ZenToHan(P_HEADS_DATA[1368].TrimRight()), &P_Siyou,
    //    &iBltNum);
    Get_IFrgBltSize(ZenToHan(P_HEADS_DATA[1369].TrimRight()), &P_Siyou,
        &iBltNum);
    //2018.11.08 ���t�����W�d�l�ύX_E

    // �ގ����ނ̎擾
    P_Sikibetu = "BFBLT-"; // ���i����
    P_Katasiki = "�{���g�i�b�g"; // ���i�^��
    P_Zuban = ""; // �}�Łi�w���i�Ȃ̂Ő}�Ԃ͖����j
    P_SZuban = ""; // ��}�p�}�ԁi�w���i�Ȃ̂Ő}�Ԃ͖����j
    P_Yobihin = ""; // �\���i���ʎq
    P_Siyou2 = ""; // �d�l2
    P_Siyou5 = "A";
    P_Zaisitu = "SUS304";
    P_EZaisitu = "SUS304";
    // 2012.05.09 ����ůčގ��ύX
    // 2016.11.07 �ގ����ޕύX
    //if ((P_HEADS_DATA[1068].TrimRight() == "1040") ||
    //    (P_HEADS_DATA[1068].TrimRight() == "1062")) {
    //    // ���t�����W�ގ���SS400,C.S.
    //    if ((P_HEADS_DATA[1051].TrimRight() == "0000") ||
    //        (P_HEADS_DATA[1051].TrimRight() == "1040") ||
    //        (P_HEADS_DATA[1051].TrimRight() == "1062")) {
    //        // �`���t�����W�ގ����w��Ȃ�,SS400,C.S.
    //        P_Zaisitu = "SS400";
    //        P_EZaisitu = "SS400";
    //    }
    //}
    if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "SS400") {
        // ���t�����W�ގ���SS400,C.S.
        if (Type_MAT(P_HEADS_DATA[1051].TrimRight()) == "-" ||
            Type_MAT(P_HEADS_DATA[1051].TrimRight()) == "SS400") {
            // �`���t�����W�ގ����w��Ȃ�,SS400,C.S.
            P_Zaisitu = "SS400";
            P_EZaisitu = "SS400";
        }
    }
    //2017.04.20 ASME,JPI�ގ��ύX
    if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "S25C") {
        // ���t�����W�ގ���S25C
        if (Type_MAT(P_HEADS_DATA[1051].TrimRight()) == "-" ||
            Type_MAT(P_HEADS_DATA[1051].TrimRight()) == "SS400") {
            // �`���t�����W�ގ����w��Ȃ�,SS400,C.S.
            P_Zaisitu = "SS400";
            P_EZaisitu = "SS400";
        }
    }
    //***************************
    // ***********************
    // **************************
    P_Num = iCntA * iBltNum;
    P_Unit = "�g";
    P_Hosoku = "";

    // *********************************
    // ***  �\���i�� �擾
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // �\����

    // ���O
    G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
    Write_Log(G_Log_Text);

    // *********************************
    // ***  �t���i�ް�(����Ľ���)�@�쐬
    // *********************************
    if (iCntA > 0)
        Write_Axsrs_Data(s_Code);

    //2018.11.08 ���t�����W�d�l�ύX_S
    Get_IFrgBltSize(ZenToHan(P_HEADS_DATA[1368].TrimRight()), &P_Siyou,
        &iBltNum);
    //2018.11.08 ���t�����W�d�l�ύX_E

    P_Siyou5 = "B";
    // 2012.05.09 ����ůčގ��ύX
    P_Zaisitu = "SUS304";
    P_EZaisitu = "SUS304";
    // 2016.11.07 �ގ����ޕύX
    //if ((P_HEADS_DATA[1068].TrimRight() == "1040") ||
    //    (P_HEADS_DATA[1068].TrimRight() == "1062")) {
    //    // ���t�����W�ގ���SS400,C.S.
    //    if ((P_HEADS_DATA[1050].TrimRight() == "0000") ||
    //        (P_HEADS_DATA[1050].TrimRight() == "1040") ||
    //        (P_HEADS_DATA[1050].TrimRight() == "1062")) {
    //        // �a���t�����W�ގ����w��Ȃ�,SS400,C.S.
    //        P_Zaisitu = "SS400";
    //        P_EZaisitu = "SS400";
    //    }
    //}
    if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "SS400") {
        // ���t�����W�ގ���SS400,C.S.
        if (Type_MAT(P_HEADS_DATA[1050].TrimRight()) == "-" ||
            Type_MAT(P_HEADS_DATA[1050].TrimRight()) == "SS400") {
            // �a���t�����W�ގ����w��Ȃ�,SS400,C.S.
            P_Zaisitu = "SS400";
            P_EZaisitu = "SS400";
        }
    }
    // ***********************
    // **************************
    P_Num = iCntB * iBltNum;
    if (iCntB > 0)
        Write_Axsrs_Data(s_Code);
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �V�[�g�K�X�P�b�g���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2004.02.24�ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_STGkt_Data(void) {
    AnsiString s_Code; // ���i����
    int iCntA;
    int iCntB;
    // 2010.07.14 �΍s���ǉ�
    int iFL;
    // *********************
    //2017.10.10 ��Ķ޽��čގ��ύX_S
    AnsiString s_GasMat;
    //2017.10.10 ��Ķ޽��čގ��ύX_E
    // 2018.05.28  ��Ķ޽��ē\�����敪_S
    AnsiString s_GasSiyouA;
    AnsiString s_GasSiyouB;
    AnsiString sSpecCode;
    // **********************************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
    // 2018.05.28  ��Ķ޽��ē\�����敪_E

    // *********************************
    // ***  �t���i�ް�(��Ķ޽���)�@�쐬
    // *********************************
    iCntA = 0;
    iCntB = 0;
    // 2010.07.14 �΍s���ǉ�
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
        iFL = 1; // �΍s��
    }
    // 2022.11.15 HERP�C��_S
    else if  (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") {
        iFL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
        iFL = 1; // �΍s��
    }
    // 2022.11.15 HERP�C��_E
    else {
        iFL = 0; // ��`��
    }
    // ���ʎZ�o�i�T�j�^���[�ɂ͕t���Ȃ��j
    // S1
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[891].TrimRight() == "1"
    // || P_HEADS_DATA[891].TrimRight() == "2"
    // || P_HEADS_DATA[891].TrimRight() == "3" )
    // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {             // ���o�[�u�[�c
    // iCntB+=0;
    // } else if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
    if ((P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight()
        == "2" || P_HEADS_DATA[891].TrimRight() == "3") &&
        (P_HEADS_DATA[893].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1") { // ���o�[�u�[�c
            iCntB += 0;
        }
        else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntB += 0;
        }
        else {
            if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 1;
                // 2018.05.28  ��Ķ޽��ē\�����敪_S
                //P_Siyou = P_HEADS_DATA[1184].TrimRight(); // �d�l
                s_GasSiyouB = P_HEADS_DATA[1184].TrimRight(); // �a�d�l
                // 2018.05.28  ��Ķ޽��ē\�����敪_E
            }
            else {
                iCntB += 0;
            }
        }
    }
    // S2
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[906].TrimRight() == "1"
    // || P_HEADS_DATA[906].TrimRight() == "2"
    // || P_HEADS_DATA[906].TrimRight() == "3" )
    // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {             // ���o�[�u�[�c
    // iCntA+=0;
    // } else if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
    if ((P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight()
        == "2" || P_HEADS_DATA[906].TrimRight() == "3") &&
        (P_HEADS_DATA[908].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1") { // ���o�[�u�[�c
            iCntA += 0;
        }
        else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntA += 0;
        }
        else {
            if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 1;
                // 2018.05.28  ��Ķ޽��ē\�����敪_S
                //P_Siyou = P_HEADS_DATA[1188].TrimRight(); // �d�l
                s_GasSiyouA = P_HEADS_DATA[1188].TrimRight(); // �`�d�l
                // 2018.05.28  ��Ķ޽��ē\�����敪_E
            }
            else {
                iCntA += 0;
            }
        }
    }
    // S3
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[921].TrimRight() == "1"
    // || P_HEADS_DATA[921].TrimRight() == "2"
    // || P_HEADS_DATA[921].TrimRight() == "3" )
    // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight()
        == "2" || P_HEADS_DATA[921].TrimRight() == "3") &&
        (P_HEADS_DATA[923].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // ��`��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") {           // ���o�[�u�[�c
            // iCntB+=0;
            // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") {    // �p�C�v�m�Y���i�t�����W�t���j
            if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                iCntB += 0;
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntB += 0;
            }
            else {
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 1;
                    // 2018.05.28  ��Ķ޽��ē\�����敪_S
                    //P_Siyou = P_HEADS_DATA[1192].TrimRight(); // �d�l
                    s_GasSiyouB = P_HEADS_DATA[1192].TrimRight(); // �a�d�l
                    // 2018.05.28  ��Ķ޽��ē\�����敪_E
                }
                else {
                    iCntB += 0;
                }
            }
        }
        else { // �΍s��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") {           // ���o�[�u�[�c
            // iCntA+=0;
            // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") {    // �p�C�v�m�Y���i�t�����W�t���j
            if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                iCntA += 0;
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntA += 0;
            }
            else {
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 1;
                    // 2018.05.28  ��Ķ޽��ē\�����敪_S
                    //P_Siyou = P_HEADS_DATA[1192].TrimRight(); // �d�l
                    s_GasSiyouA = P_HEADS_DATA[1192].TrimRight(); // �`�d�l
                    // 2018.05.28  ��Ķ޽��ē\�����敪_E
                }
                else {
                    iCntA += 0;
                }
            }
        }
    }
    // S4
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[936].TrimRight() == "1"
    // || P_HEADS_DATA[936].TrimRight() == "2"
    // || P_HEADS_DATA[936].TrimRight() == "3" )
    // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight()
        == "2" || P_HEADS_DATA[936].TrimRight() == "3") &&
        (P_HEADS_DATA[938].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // ��`��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                iCntA += 0;
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntA += 0;
            }
            else {
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 1;
                    // 2018.05.28  ��Ķ޽��ē\�����敪_S
                    //P_Siyou = P_HEADS_DATA[1196].TrimRight(); // �d�l
                    s_GasSiyouA = P_HEADS_DATA[1196].TrimRight(); // �`�d�l
                    // 2018.05.28  ��Ķ޽��ē\�����敪_E
                }
                else {
                    iCntA += 0;
                }
            }
        }
        else { // �΍s��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                iCntB += 0;
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntB += 0;
            }
            else {
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 1;
                    // 2018.05.28  ��Ķ޽��ē\�����敪_S
                    //P_Siyou = P_HEADS_DATA[1196].TrimRight(); // �d�l
                    s_GasSiyouB = P_HEADS_DATA[1196].TrimRight(); // �a�d�l
                    // 2018.05.28  ��Ķ޽��ē\�����敪_E
                }
                else {
                    iCntB += 0;
                }
            }
        }
    }
    // E1
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[951].TrimRight() == "1"
    // || P_HEADS_DATA[951].TrimRight() == "2"
    // || P_HEADS_DATA[951].TrimRight() == "3" )
    // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {             // ���o�[�u�[�c
    // iCntB+=0;
    // } else if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
    if ((P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight()
        == "2" || P_HEADS_DATA[951].TrimRight() == "3") &&
        (P_HEADS_DATA[953].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1") { // ���o�[�u�[�c
            iCntB += 0;
        }
        else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntB += 0;
        }
        else {
            if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntB += 1;
                // 2018.05.28  ��Ķ޽��ē\�����敪_S
                //P_Siyou = P_HEADS_DATA[1200].TrimRight(); // �d�l
                s_GasSiyouB = P_HEADS_DATA[1200].TrimRight(); // �a�d�l
                // 2018.05.28  ��Ķ޽��ē\�����敪_E
            }
            else {
                iCntB += 0;
            }
        }
    }
    // E2
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[966].TrimRight() == "1"
    // || P_HEADS_DATA[966].TrimRight() == "2"
    // || P_HEADS_DATA[966].TrimRight() == "3" )
    // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {             // ���o�[�u�[�c
    // iCntA+=0;
    // } else if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4" ) {      // �p�C�v�m�Y���i�t�����W�t���j
    if ((P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight()
        == "2" || P_HEADS_DATA[966].TrimRight() == "3") &&
        (P_HEADS_DATA[968].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1") { // ���o�[�u�[�c
            iCntA += 0;
        }
        else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "4")
        { // �p�C�v�m�Y���i�t�����W�t���j
            // ***********************
            iCntA += 0;
        }
        else {
            if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // �[�ǂ��t��
                iCntA += 1;
                // 2018.05.28  ��Ķ޽��ē\�����敪_S
                //P_Siyou = P_HEADS_DATA[1204].TrimRight(); // �d�l
                s_GasSiyouA = P_HEADS_DATA[1204].TrimRight(); // �`�d�l
                // 2018.05.28  ��Ķ޽��ē\�����敪_E
            }
            else {
                iCntA += 0;
            }
        }
    }
    // E3
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[981].TrimRight() == "1"
    // || P_HEADS_DATA[981].TrimRight() == "2"
    // || P_HEADS_DATA[981].TrimRight() == "3" )
    // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight()
        == "2" || P_HEADS_DATA[981].TrimRight() == "3") &&
        (P_HEADS_DATA[983].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // ��`��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                iCntB += 0;
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntB += 0;
            }
            else {
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 1;
                    // 2018.05.28  ��Ķ޽��ē\�����敪_S
                    //P_Siyou = P_HEADS_DATA[1208].TrimRight(); // �d�l
                    s_GasSiyouB = P_HEADS_DATA[1208].TrimRight(); // �a�d�l
                    // 2018.05.28  ��Ķ޽��ē\�����敪_E
                }
                else {
                    iCntB += 0;
                }
            }
        }
        else { // �΍s��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                iCntA += 0;
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntA += 0;
            }
            else {
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 1;
                    // 2018.05.28  ��Ķ޽��ē\�����敪_S
                    //P_Siyou = P_HEADS_DATA[1208].TrimRight(); // �d�l
                    s_GasSiyouA = P_HEADS_DATA[1208].TrimRight(); // �`�d�l
                    // 2018.05.28  ��Ķ޽��ē\�����敪_E
                }
                else {
                    iCntA += 0;
                }
            }
        }
    }
    // E4
    // 2011.06.20 ɽ�ًK�i�ύX
    // if ( ( P_HEADS_DATA[996].TrimRight() == "1"
    // || P_HEADS_DATA[996].TrimRight() == "2"
    // || P_HEADS_DATA[996].TrimRight() == "3" )
    // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight()
        == "2" || P_HEADS_DATA[996].TrimRight() == "3") &&
        (P_HEADS_DATA[998].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // ��`��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                iCntA += 0;
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntA += 0;
            }
            else {
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntA += 1;
                    // 2018.05.28  ��Ķ޽��ē\�����敪_S
                    //P_Siyou = P_HEADS_DATA[1212].TrimRight(); // �d�l
                    s_GasSiyouA = P_HEADS_DATA[1212].TrimRight(); // �`�d�l
                    // 2018.05.28  ��Ķ޽��ē\�����敪_E
                }
                else {
                    iCntA += 0;
                }
            }
        }
        else { // �΍s��
            // 2011.06.20 ɽ�ًK�i�ύX
            // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {         // ���o�[�u�[�c
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) {  // �p�C�v�m�Y���i�t�����W�t���j
            if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
            { // ���o�[�u�[�c
                iCntB += 0;
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // �p�C�v�m�Y���i�t�����W�t���j
                // ***********************
                iCntB += 0;
            }
            else {
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // �[�ǂ��t��
                    iCntB += 1;
                    // 2018.05.28  ��Ķ޽��ē\�����敪_S
                    //P_Siyou = P_HEADS_DATA[1212].TrimRight(); // �d�l
                    s_GasSiyouB = P_HEADS_DATA[1212].TrimRight(); // �a�d�l
                    // 2018.05.28  ��Ķ޽��ē\�����敪_E
                }
                else {
                    iCntB += 0;
                }
            }
        }
    }
    // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" || P_HEADS_DATA[891].TrimRight() == "3") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1") {        // ���o�[�u�[�c
    // iCntB+=0;
    // } else if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=0;
    // } else {
    // if (P_HEADS_DATA[1136].ToIntDef(0) != 0) {                     // �[�ǂ��t��
    // iCntB+=1;
    // P_Siyou = P_HEADS_DATA[1184].TrimRight();                    // �d�l
    // } else {
    // iCntB+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" || P_HEADS_DATA[906].TrimRight() == "3") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1") {        // ���o�[�u�[�c
    // iCntA+=0;
    // } else if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=0;
    // } else {
    // if (P_HEADS_DATA[1137].ToIntDef(0) != 0) {                     // �[�ǂ��t��
    // iCntA+=1;
    // P_Siyou = P_HEADS_DATA[1188].TrimRight();                    // �d�l
    // } else {
    // iCntA+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" || P_HEADS_DATA[921].TrimRight() == "3") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") {        // ���o�[�u�[�c
    // iCntB+=0;
    // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=0;
    // } else {
    // if (P_HEADS_DATA[1138].ToIntDef(0) != 0) {                     // �[�ǂ��t��
    // iCntB+=1;
    // P_Siyou = P_HEADS_DATA[1192].TrimRight();                    // �d�l
    // } else {
    // iCntB+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" || P_HEADS_DATA[936].TrimRight() == "3") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1") {        // ���o�[�u�[�c
    // iCntA+=0;
    // } else if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=0;
    // } else {
    // if (P_HEADS_DATA[1139].ToIntDef(0) != 0) {                     // �[�ǂ��t��
    // iCntA+=1;
    // P_Siyou = P_HEADS_DATA[1196].TrimRight();                    // �d�l
    // } else {
    // iCntA+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" || P_HEADS_DATA[951].TrimRight() == "3") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1") {        // ���o�[�u�[�c
    // iCntB+=0;
    // } else if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=0;
    // } else {
    // if (P_HEADS_DATA[1140].ToIntDef(0) != 0) {                     // �[�ǂ��t��
    // iCntB+=1;
    // P_Siyou = P_HEADS_DATA[1200].TrimRight();                    // �d�l
    // } else {
    // iCntB+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" || P_HEADS_DATA[966].TrimRight() == "3") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1") {        // ���o�[�u�[�c
    // iCntA+=0;
    // } else if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=0;
    // } else {
    // if (P_HEADS_DATA[1141].ToIntDef(0) != 0) {                     // �[�ǂ��t��
    // iCntA+=1;
    // P_Siyou = P_HEADS_DATA[1204].TrimRight();                    // �d�l
    // } else {
    // iCntA+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" || P_HEADS_DATA[981].TrimRight() == "3") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1") {        // ���o�[�u�[�c
    // iCntB+=0;
    // } else if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
    // iCntB+=0;
    // } else {
    // if (P_HEADS_DATA[1142].ToIntDef(0) != 0) {                     // �[�ǂ��t��
    // iCntB+=1;
    // P_Siyou = P_HEADS_DATA[1208].TrimRight();                    // �d�l
    // } else {
    // iCntB+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" || P_HEADS_DATA[996].TrimRight() == "3") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1") {        // ���o�[�u�[�c
    // iCntA+=0;
    // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") { // �p�C�v�m�Y���i�t�����W�t���j
    // iCntA+=0;
    // } else {
    // if (P_HEADS_DATA[1143].ToIntDef(0) != 0) {                     // �[�ǂ��t��
    // iCntA+=1;
    // P_Siyou = P_HEADS_DATA[1212].TrimRight();                    // �d�l
    // } else {
    // iCntA+=0;
    // }
    // }
    // }
    // *********************

    // �ގ����ނ̎擾
    P_Sikibetu = "STGKT-"; // ���i����
    P_Katasiki = "�V�[�g�K�X�P�b�g"; // ���i�^��
    P_Zuban = ""; // �}�Łi�w���i�Ȃ̂Ő}�Ԃ͖����j
    P_SZuban = ""; // ��}�p�}�ԁi�w���i�Ȃ̂Ő}�Ԃ͖����j
    P_Yobihin = ""; // �\���i���ʎq

    // 2020.12.16 ��Ķ޽��ďC��_S
    // 2018.05.28  ��Ķ޽��ē\�����敪_S
    //if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
    //    iCntA = iCntA + iCntB;
    //    iCntB = 0;
    //}
    if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B") &&
        s_GasSiyouA == s_GasSiyouB) {
        iCntA = iCntA + iCntB;
        iCntB = 0;
    }
    // 2020.12.16 ��Ķ޽��ďC��_E

    P_Siyou = s_GasSiyouA;
    if ( iCntA > 0 && iCntB > 0 ) {
        // ��Ķ޽��ē\����(A��)
        if (sSpecCode == "A") {
            // �~���[�d�l �`���@
            P_Siyou = P_Siyou + "�@�`���@�`���^�a���@�a��"; // �d�l
        } else if (sSpecCode == "B") {
            // �~���[�d�l �a���@
            P_Siyou = P_Siyou + "�@�`���@�a���^�a���@�`��"; // �d�l
        } else {
            P_Siyou = P_Siyou + "�@�`���p";                 // �d�l
        }
    }
    // 2018.05.28  ��Ķ޽��ē\�����敪_E
    P_Siyou2 = ""; // �d�l2
    P_Siyou5 = "A";
    //2017.10.10 ��Ķ޽��čގ��ύX_S
    //P_Zaisitu = Search_HD_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[836].TrimRight(), 2));
    //P_EZaisitu = Search_HD_E_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[836].TrimRight(), 2));
    s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ��Ķ޽��čގ�CODE
    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
    //2017.10.10 ��Ķ޽��čގ��ύX_E
    P_Num = iCntA;
    P_Unit = "";
    P_Hosoku = "";

    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
    if ( P_GasNameA == "�����O�K�X�P�b�g" ) {
        // �ݸ޶޽���
        P_Sikibetu = "STGKTR";                   // ���i����
        P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
    } else if ( P_GasNameA == "�S�ʍ��t�����W�K�X�P�b�g" ) {
        // �S�ʍ���ݼ޶޽���
        P_Sikibetu = "STGKTF";                   // ���i����
        P_Katasiki = "�S�ʍ��t�����W�K�X�P�b�g"; // ���i�^��
    } else {
        // ���̑�
    }
    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

    // *********************************
    // ***  �\���i�� �擾
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // �\����

    // ���O
    G_Log_Text = "���i�^��           �" + P_Katasiki + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
    Write_Log(G_Log_Text);

    // 2018.05.28  ��Ķ޽��ē\�����敪_S
    //if (iCntA > 0)
    //    Write_Axsrs_Data(s_Code);
    if (iCntA > 0) {
        Write_Axsrs_Data(s_Code);
    }

    P_Siyou = s_GasSiyouB; // �d�l
    if ( iCntB > 0 ) {
        // ��Ķ޽��ē\����(B��)
        if (sSpecCode == "A") {
            // �~���[�d�l �`���@
            P_Siyou = P_Siyou + "�@�`���@�a���^�a���@�`��"; // �d�l
        } else if (sSpecCode == "B") {
            // �~���[�d�l �a���@
            P_Siyou = P_Siyou + "�@�`���@�`���^�a���@�a��"; // �d�l
        } else {
            P_Siyou = P_Siyou + "�@�a���p";                 // �d�l
        }
    }
    // 2018.05.28  ��Ķ޽��ē\�����敪_S
    P_Siyou5 = "B";
    P_Num = iCntB;
    //2017.10.10 ��Ķ޽��čގ��ύX_S
    //P_Zaisitu = Search_HD_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[835].TrimRight(), 2));
    //P_EZaisitu = Search_HD_E_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[835].TrimRight(), 2));
    s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ��Ķ޽��čގ�CODE
    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
    //2017.10.10 ��Ķ޽��čގ��ύX_E

    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
    if ( P_GasNameB == "�����O�K�X�P�b�g" ) {
        // �ݸ޶޽���
        P_Sikibetu = "STGKTR";                   // ���i����
        P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
    } else if ( P_GasNameB == "�S�ʍ��t�����W�K�X�P�b�g" ) {
        // �S�ʍ���ݼ޶޽���
        P_Sikibetu = "STGKTF";                   // ���i����
        P_Katasiki = "�S�ʍ��t�����W�K�X�P�b�g"; // ���i�^��
    } else {
        // ���̑�
    }
    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

    // 2020.12.16 ��Ķ޽��ďC��_S
    //if (iCntB > 0)
    if (iCntB > 0) {
    // 2020.12.16 ��Ķ޽��ďC��_E

        Write_Axsrs_Data(s_Code);

    // 2020.12.16 ��Ķ޽��ďC��_S
    }
    // 2020.12.16 ��Ķ޽��ďC��_E

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���p�ȊO�X�g���[�i���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2020.04.23�ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails:: Get_Strainer_Data(void) {
    AnsiString s_Code;   // ���i����
    AnsiString s_Key;    // ������
    AnsiString s_SQL;
    AnsiString s_Spec;   // �d�l
    AnsiString s_Dno2;   // ��ڰŗp�}��2
    AnsiString s_WKDno2; // ��ڰŗp��Ɛ}��2
    AnsiString s_GasMat; // �޽��čގ�CODE
    AnsiString sTugite;  // ɽ�ٌp��
    int iCntA;
    int iCntB;
    int iCnt2A;
    int iCnt2B;
    int iFL;
    AnsiString s_TextA;
    AnsiString s_TextB;
    AnsiString s_GasSiyou;
    AnsiString sSpecCode;
    // **********************************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    // �m�Y���R�[�h�̎擾
    AnsiString s_Nzl_Kind = "";
    if (P_HEADS_DATA[951].ToIntDef(0) == 5) {
        sTugite = P_HEADS_DATA[953].TrimRight(); // E1ɽ�ٌp��
    } else if (P_HEADS_DATA[966].ToIntDef(0) == 5) {
        sTugite = P_HEADS_DATA[968].TrimRight(); // E2ɽ�ٌp��
    } else if (P_HEADS_DATA[981].ToIntDef(0) == 5) {
        sTugite = P_HEADS_DATA[983].TrimRight(); // E3ɽ�ٌp��
    } else if (P_HEADS_DATA[996].ToIntDef(0) == 5) {
        sTugite = P_HEADS_DATA[998].TrimRight(); // E3ɽ�ٌp��
    }
    if (sTugite.ToIntDef(0) >= 1000) {
        s_Nzl_Kind = sTugite.SubString(1, 2) + "0" + sTugite.SubString(4, 1);
    }
    else {
        s_Nzl_Kind = sTugite.SubString(2, 1) + "0" + sTugite.SubString(4, 1);
    }

    // *********************************************
    // ***�@���p�t���iϽ�(HD_ACCESSORY) ����
    // *********************************************
    // �����L�[
    s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_ACCESSORY";
    s_SQL = s_SQL + "  WHERE SERIES_MODEL = '" + s_Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "�ް��쐬��    �ذ�ތ^���" + s_Key +
            "� �́A���p�t���iϽ�(HD_ACCESSORY)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        P_Hosoku = "";
        // ���ނ̎擾
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // ���i����
        s_Spec = wkQuery->FieldByName("PARTS_SPEC")->AsString.TrimRight(); // �d�l
        P_Sikibetu = wkQuery->FieldByName("PL_SYMBOL")->AsString.TrimRight();
        // ���i�\���ʎq
        P_Katasiki = wkQuery->FieldByName("PARTS_NAME")
            ->AsString.TrimRight(); // ���i����
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // �ގ�����
        P_EZaisitu = Search_HD_MAT_JtoE(P_Zaisitu); // �ގ�����
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // �}��
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // ��Ɛ}��
        s_Dno2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        // �X�g���[�i�p�}�ԂQ
        s_WKDno2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        // �X�g���[�i�p��Ɛ}�ԂQ
        P_Num = StrToInt(wkQuery->FieldByName("NUMS")->AsString.TrimRight
            ()); // ����
        P_Unit = "";
        P_Yobihin = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();
        // �\���i���ʎq


        // ���p�łȂ��̂ŁA�X�g���[�i�A�X�g���[�i�p�K�X�P�b�g�֌W�ȊO�����O����
        if (s_Code != "17" && s_Code != "23" && s_Code != "24" &&
            s_Code != "26" && s_Code != "27") {
            // 17 ��ڰ�
            // 23 �~(��ײ���)��ݼ�
            // 24 ��Ķ޽���
            // 26 �S�ʍ��~��Ķ޽���
            // 27 ����ů�
            wkQuery->Next();
            continue;
        }

        AnsiString sNRLKind = wkQuery->FieldByName("NRL_KIND")->AsString.TrimRight();
        // PARTS_CODE���ȉ��ł���΃m�Y���K�i��r���s��
        // 23 �~(��ײ���)��ݼ�
        // 24 ��Ķ޽���
        // 25 �~��Ķ޽���
        // 26 �S�ʍ��~��Ķ޽���
        // 27 ����ů�
        if (( s_Code == "23" || s_Code == "24" || s_Code == "25" ||
              s_Code == "26" || s_Code == "27" ) &&
            ( s_Nzl_Kind != sNRLKind )){
                wkQuery->Next();
                continue;
        }

        // ���ނ����� ���t�����W �܂��́A��Ķ޽���
        if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
            iFL = 1; // �΍s��
        }
        else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
            iFL = 1; // �΍s��
        }
        // 2022.11.15 HERP�C��_S
        else if  (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c") {
            iFL = 1; // �΍s��
        }
        else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") {
            iFL = 1; // �΍s��
        }
        else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
            iFL = 1; // �΍s��
        }
        // 2022.11.15 HERP�C��_E
        else {
            iFL = 0; // ��`��
        }

        // �~(�u���C���h)�t�����W
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" && s_Code == "23") {
            iCntA = 0;
            iCntB = 0;

            // S1
            if (P_HEADS_DATA[891].TrimRight() == "5" ) {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "5" ) {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "5" ) {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "5" ) {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5" ) {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5" ) {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5" ) {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5" ) {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            P_Siyou5 = "A";
            if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-") {
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            }
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
        }

        // �V�[�g�K�X�P�b�g
        if (s_Code == "24") {
            iCntA = 0;
            iCntB = 0;
            iCnt2A = 0;
            iCnt2B = 0;

            // �X�g���[�i�r�����~�ߗp�V�[�g�K�X�P�b�g
            // ���ׂĂ̌^���ɂr�����~�߂����݂���
            // �d�P�X�g���[�i���r�P�m�Y��
            if ((P_HEADS_DATA[951].TrimRight() == "5") &&
                (P_HEADS_DATA[891].TrimRight() == "1" ||
                 P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                iCntB += 1; // ��`��
            }
            // �d�Q�X�g���[�i���r�Q�m�Y��
            if ((P_HEADS_DATA[966].TrimRight() == "5") &&
                (P_HEADS_DATA[906].TrimRight() == "1" ||
                 P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                iCntA += 1; // ��`��
            }
            // �d�R�X�g���[�i���r�R�m�Y��
            if ((P_HEADS_DATA[981].TrimRight() == "5") &&
                (P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntB += 1; // ��`��
                }
                else {
                    iCntA += 1; // �΍s��
                }
            }
            // �d�S�X�g���[�i���r�S�m�Y��
            if ((P_HEADS_DATA[996].TrimRight() == "5") &&
                (P_HEADS_DATA[936].TrimRight() == "1" ||
                P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntA += 1; // ��`��
                }
                else {
                    iCntB += 1; // �΍s��
                }
            }

            if (s_Key == "LX-10") {
                if (P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983]
                    .TrimRight().SubString(3, 1) != "1")
                    iCnt2B += 1;
                if (P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998]
                    .TrimRight().SubString(3, 1) != "1")
                    iCnt2A += 1;

                if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
                    iCnt2A = iCnt2A + iCnt2B;
                    iCnt2B = 0;
                }

                if (iCnt2B != 0) {
                    s_GasMat = Get_SheetGasketMat("B");  // ��Ķ޽��čގ�CODE
                    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                    P_Num = iCnt2B;
                    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0",
                        m_pcHeadsData);
                    s_Spec = "JIS10K-80A"; // �d�l
                    P_Siyou5 = "B";

                    if (iCnt2A != 0) {
                        // ��Ķ޽��ē\����(B��)
                        if (sSpecCode == "A") {
                            // �~���[�d�l �`���@
                            s_Spec = s_Spec + "�@�`���@�a���^�a���@�`��"; // �d�l
                        } else if (sSpecCode == "B") {
                            // �~���[�d�l �a���@
                            s_Spec = s_Spec + "�@�`���@�`���^�a���@�a��"; // �d�l
                        } else {
                            s_Spec = s_Spec + "�@�a���p";                 // �d�l
                        }
                    }

                    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
                    P_Sikibetu = "STGKTR";                   // ���i����
                    P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
                    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

                    Write_Funeyou_Data(s_Code, s_Spec);
                    s_Spec = "JIS10K-100A"; // �d�l
                }
                if (iCnt2A != 0) {
                    s_GasMat = Get_SheetGasketMat("A");  // ��Ķ޽��čގ�CODE
                    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                    P_Num = iCnt2A;
                    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0",
                        m_pcHeadsData);
                    s_Spec = "JIS10K-80A"; // �d�l
                    P_Siyou5 = "A";

                    if (iCnt2B != 0) {
                        // ��Ķ޽��ē\����(A��)
                        if (sSpecCode == "A") {
                            // �~���[�d�l �`���@
                            s_Spec = s_Spec + "�@�`���@�`���^�a���@�a��"; // �d�l
                        } else if (sSpecCode == "B") {
                            // �~���[�d�l �a���@
                            s_Spec = s_Spec + "�@�`���@�a���^�a���@�`��"; // �d�l
                        } else {
                            s_Spec = s_Spec + "�@�`���p";                 // �d�l
                        }
                    }

                    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
                    P_Sikibetu = "STGKTR";                   // ���i����
                    P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
                    // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

                    Write_Funeyou_Data(s_Code, s_Spec);
                    s_Spec = "JIS10K-100A"; // �d�l
                }
            }

            if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
                iCntA = iCntA + iCntB;
                iCntB = 0;
            }
            s_GasSiyou = s_Spec;

            if (iCntB != 0) {
                s_GasMat = Get_SheetGasketMat("B");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ��Ķ޽��čގ��ύX_E
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";

                if (iCntA != 0) {
                    // ��Ķ޽��ē\����(B��)
                    if (sSpecCode == "A") {
                        // �~���[�d�l �`���@
                        s_Spec = s_Spec + "�@�`���@�a���^�a���@�`��"; // �d�l
                    } else if (sSpecCode == "B") {
                        // �~���[�d�l �a���@
                        s_Spec = s_Spec + "�@�`���@�`���^�a���@�a��"; // �d�l
                    } else {
                        s_Spec = s_Spec + "�@�a���p";                 // �d�l
                    }
                }

                // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
                if ( P_GasNameB == "�����O�K�X�P�b�g" ) {
                    // �ݸ޶޽���
                    P_Sikibetu = "STGKTR";                   // ���i����
                    P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
                } else if ( P_GasNameB == "�S�ʍ��t�����W�K�X�P�b�g" ) {
                    // �S�ʍ���ݼ޶޽���
                    P_Sikibetu = "STGKTF";                   // ���i����
                    P_Katasiki = "�S�ʍ��t�����W�K�X�P�b�g"; // ���i�^��
                } else {
                    // ���̑�
                }
                // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

                Write_Funeyou_Data(s_Code, s_Spec);
            }

            s_Spec = s_GasSiyou;

            if (iCntA != 0) {
                s_GasMat = Get_SheetGasketMat("A");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";
                if (iCntB != 0) {
                    // ��Ķ޽��ē\����(A��)
                    if (sSpecCode == "A") {
                        // �~���[�d�l �`���@
                        s_Spec = s_Spec + "�@�`���@�`���^�a���@�a��"; // �d�l
                    } else if (sSpecCode == "B") {
                        // �~���[�d�l �a���@
                        s_Spec = s_Spec + "�@�`���@�a���^�a���@�`��"; // �d�l
                    } else {
                        s_Spec = s_Spec + "�@�`���p";                 // �d�l
                    }
                }

                // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_S
                if ( P_GasNameA == "�����O�K�X�P�b�g" ) {
                    // �ݸ޶޽���
                    P_Sikibetu = "STGKTR";                   // ���i����
                    P_Katasiki = "�����O�K�X�P�b�g";         // ���i�^��
                } else if ( P_GasNameA == "�S�ʍ��t�����W�K�X�P�b�g" ) {
                    // �S�ʍ���ݼ޶޽���
                    P_Sikibetu = "STGKTF";                   // ���i����
                    P_Katasiki = "�S�ʍ��t�����W�K�X�P�b�g"; // ���i�^��
                } else {
                    // ���̑�
                }
                // 2020.12.16 �V�[�g�K�X�P�b�g���̕ύX_E

                Write_Funeyou_Data(s_Code, s_Spec);
            }

            s_Spec = s_GasSiyou;

            P_Num = 0;
        }

        //�S�ʍ��~�K�X�P�b�g
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "26") {
            iCntA = 0;
            iCntB = 0;

            // ��ڰł͂͂��ׂĂ�
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            if (iCntB != 0) {
                s_GasMat = Get_SheetGasketMat("B");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            if (iCntA != 0) {
                s_GasMat = Get_SheetGasketMat("A");  // ��Ķ޽��čގ�CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ��Ķ޽��čގ��ύX_E
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            P_Num = 0;
        }

        // �d���{���g�i�b�g
        if (s_Code == "27") {
            iCntA = 0;
            iCntB = 0;

            // 2022.09.21 E���{���g�i�b�gSUS�w��_S
            if (P_HEADS_DATA[162].TrimRight() == "1.00") {
                P_Zaisitu = "SUS304";
                P_EZaisitu = "SUS304";
            }
            // 2022.09.21 E���{���g�i�b�gSUS�w��_E

            // ��ڰ�
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // ��`��
                }
                else {
                    iCntA += P_Num; // �΍s��
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // ��`��
                }
                else {
                    iCntB += P_Num; // �΍s��
                }
            }
            P_Siyou5 = "A";
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
        }

        // *********************************
        // ***  �\���i�� �擾
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);

        // ���O
        G_Log_Text = "���i����           �" + s_Code + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�d�l               �" + s_Spec + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "���i�\����         �" + P_Sikibetu + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "���i����           �" + P_Katasiki + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�ގ�����           �" + P_Zaisitu + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�}��               �" + P_Zuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��}�p�}��         �" + P_SZuban + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "����               �" + AnsiString(P_Num) + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�\���i���ʎq       �" + P_Yobihin + "����擾�B";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  ��ڰ��ް��@�쐬
        // *********************************

        // �~�t�����W�A�K�X�P�b�g�A�d���{���g�i�b�g�����O����
        if ((Type_MAT(P_HEADS_DATA[265].TrimRight()) == "-") &&
            (s_Code == "23" || s_Code == "24" || s_Code == "26" || s_Code == "27")) {
            // 23 �~(��ײ���)��ݼ�
            // 24 ��Ķ޽���
            // 26 �S�ʍ��~��Ķ޽���
            // 27 E������ů�
            wkQuery->Next();
            continue;
        }

        // �X�g���[�i
        if (s_Code == "17") {
        // 2019.09.09 E������ůĒǉ�_E�@
            // �e�m�Y����ނɽ�ڰł��ݒ肳��Ă��Ȃ�
            if ((P_HEADS_DATA[891].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[906].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[921].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[936].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[951].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[966].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[981].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[996].ToIntDef(0) != 5)) {
                wkQuery->Next();
                continue;
            }
            if (s_Key == "UX-90" || s_Key == "SX-90M") {
                // UX-90,SX-90M
                if (P_HEADS_DATA[38].ToIntDef(0) > 350) {
                    if (Type_MAT(P_HEADS_DATA[261].TrimRight()) == "SUS316") {
                        // ��ڰł�SUS316�̏ꍇ
                        P_Zuban = "�d�R�T�X�R�O�P�|�q�P";
                        P_SZuban = "�d�R�T�X�R�O�P";
                    }
                    else if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                             P_HEADS_DATA[261].Pos("3032") > 0 ||
                             P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ��ڰł�TP340�̏ꍇ
                        P_Zuban = "�d�R�T�X�R�O�Q�|�q�Q";
                        P_SZuban = "�d�R�T�X�R�O�Q";
                    }
                    else {
                        P_Zuban = "";
                        P_SZuban = "";
                    }
                }
                else {
                    // ����������350���ȉ�
                    if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                        P_HEADS_DATA[261].Pos("3032") > 0 ||
                        P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ��ڰł�TP340�̏ꍇ�͐}�ԂQ
                        P_Zuban = s_Dno2;
                        P_SZuban = s_WKDno2;
                    }
                }
            }
            else if (s_Key == "RX-90") {
                // RX-90
                if (P_HEADS_DATA[38].ToIntDef(0) > 291) {
                    // ����������291���ȏ�
                    if (Type_MAT(P_HEADS_DATA[261].TrimRight()) == "SUS316") {
                        // ��ڰł�SUS316�̏ꍇ
                        P_Zuban = "�d�R�T�X�R�R�W�|�q�Q";
                        P_SZuban = "�d�R�T�X�R�R�W";
                    }
                    else if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                             P_HEADS_DATA[261].Pos("3032") > 0 ||
                             P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ��ڰł�TP340�̏ꍇ
                        P_Zuban = "�d�R�T�X�T�V�Q�|�q�R";
                        P_SZuban = "�d�R�T�X�T�V�Q";
                    }
                    else {
                        P_Zuban = "";
                        P_SZuban = "";
                    }
                }
                else {
                    // ����������290���ȉ�
                    //if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                        P_HEADS_DATA[261].Pos("3032") > 0 ||
                        P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ��ڰł�TP340�̏ꍇ�͐}�ԂQ
                        P_Zuban = s_Dno2;
                        P_SZuban = s_WKDno2;
                    }
                }
            }
            else if (s_Key != "UX-01" && s_Key != "UX-10" && s_Key != "UX-20" &&
                s_Key != "UX-40" && s_Key != "LX-00" && s_Key != "LX-10" &&
                s_Key != "LX-20" && s_Key != "LX-40" && s_Key != "RX-90") {
                // UX-01,UX-10,UX-20,UX-40,LX-00,LX-10,LX-20,LX-40�ȊO
                if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                    P_HEADS_DATA[261].Pos("3032") > 0 ||
                    P_HEADS_DATA[261].Pos("3132") > 0) {
                    // ��ڰł�TP340�̏ꍇ�͐}�ԂQ
                    P_Zuban = s_Dno2;
                    P_SZuban = s_WKDno2;
                }
            }

       //        '''''''''''''''
            Write_Error_Log("s_Code3");
            Write_Error_Log(s_Code);
        // ''''''''''''''''



            // ��ڰł̏ꍇ�ͽ�ڰōގ����o��
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[261].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[261].TrimRight());
        }

        Write_Funeyou_Data(s_Code, s_Spec);
        wkQuery->Next();
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �\���i�݂̂̕��i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::EtcData_Write(void) {
    // CSV�Ǎ��p
    FILE *wkfp;

    char buf[1024]; // ������o�b�t�@
    char *tok; // �g�[�N���|�C���^
    int i; // ���[�v�J�E���^
    char token[100][100]; // �g�[�N���o�b�t�@
    int i_LineNum;

    AnsiString s_Yobi_Text[5];
    AnsiString s_YBKey;
    AnsiString s_YBZaisitu;

    AnsiString wk_FilePass; // "D00001.csv"
    int i_Yobisu;

    // 2005-10-07
    int n; // �\���i��r�p���[�v�J�E���^
    int i_Chk; // �d���\���i�`�F�b�N�J�E���^
    int i_ErrNum; // �W�J�ł��Ȃ������\���i�J�E���^
    AnsiString s_ErrName[100]; // �W�J�ł��Ȃ������\���i��
    int i_ErrYobiNum[100]; // �W�J�ł��Ȃ������\���i����

    i_Yobisu = 0;
    i_LineNum = 0;
    i_ErrNum = 0; // 2005-10-07

    // -----------------------------------------------
    // 2007.10.03 �ǉ�
    // �װB�d�l�̏ꍇ�́A�\���i�ް��Ȃ��Ƃ��ď���
    if (m_pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B) {
        return i_Yobisu;
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
            s_Yobi_Text[3] = token[11]; // �\���i�ް� �ގ��R�[�h( 1011, ... )
            s_Yobi_Text[4] = token[2]; // �\���i�ް� �ގ��R�[�h( 1011, ... )

            // ����������
            if (AllNumChk(s_Yobi_Text[1].c_str(), 0) == 0) {
                int i_No;

                i_No = StrToInt(s_Yobi_Text[1]) + 100;
                s_YBKey = s_Yobi_Text[0] + IntToStr(i_No).SubString(2, 2);
                s_YBZaisitu = s_Yobi_Text[3].TrimRight();
                i_LineNum++;
            }
            else {
                // ********************************************
                // �\���i�ް� 2��ڂ�
                // ��̧�ޯĂ�������NULL�������ꍇ �������Ȃ�?
                // ********************************************
                s_YBKey = "";
            }

            if (s_YBKey != "\0") {
                if ((s_YBKey.SubString(1, 4) == "ADHS") || (s_YBKey.SubString(1,
                    4) == "OILR")) {
                    P_Sikibetu = "ETCP--"; // ���i����
                    P_Katasiki = s_Yobi_Text[4]; // ���i�^��
                    P_Zuban = ""; // �}�Łi�w���i�Ȃ̂Ő}�Ԃ͖����j
                    P_SZuban = ""; // ��}�p�}�ԁi�w���i�Ȃ̂Ő}�Ԃ͖����j
                    P_Yobihin = ""; // �\���i���ʎq
                    P_Siyou = ""; // �d�l
                    P_Siyou2 = ""; // �d�l2
                    P_Siyou5 = "";
                    P_Zaisitu = "";
                    P_EZaisitu = "";
                    P_Num = 0;
                    P_Yobisu = s_Yobi_Text[2].ToIntDef(0);
                    P_Unit = "";
                    P_Hosoku = "";
                    Write_Axsrs_Data("");

                    G_YOBICHK_NAME[G_YOBICHK_RecCnt] =
                        s_Yobi_Text[0] + s_Yobi_Text[1];
                    G_YOBICHK_MAT[G_YOBICHK_RecCnt] = s_Yobi_Text[3];
                    G_YOBICHK_RecCnt++;
                }
                else { // 2005-10-07 �W�J�ł��Ă��Ȃ��\���i�`�F�b�N
                    i_Chk = 0;
                    for (n = 0; n < G_YOBICHK_RecCnt; n++) {
                        // �d���`�F�b�N
                        if ((s_Yobi_Text[0] + s_Yobi_Text[1])
                            == G_YOBICHK_NAME[n]) {
                            i_Chk++;
                        }

                    }
                    // �d�����Ȃ��\���i���擾
                    if (i_Chk == 0) {
                        s_ErrName[i_ErrNum] = s_Yobi_Text[4];
                        i_ErrYobiNum[i_ErrNum] = s_Yobi_Text[2].ToIntDef(0);
                        i_ErrNum++;
                    }
                }
            }
        }
    }

    // �W�J�s�ǂ��`�F�b�N
    if (i_LineNum != G_YOBICHK_RecCnt) {
        P_Sikibetu = "ERROR-"; // ���i����
        P_Katasiki = "���W�J�ł��Ȃ������\���i�����݂��܂��B"; // ���i�^��
        P_Zuban = ""; // �}�Łi�w���i�Ȃ̂Ő}�Ԃ͖����j
        P_SZuban = ""; // ��}�p�}�ԁi�w���i�Ȃ̂Ő}�Ԃ͖����j
        P_Yobihin = ""; // �\���i���ʎq
        P_Siyou = ""; // �d�l
        P_Siyou2 = ""; // �d�l2
        P_Siyou5 = "";
        P_Zaisitu = "";
        P_EZaisitu = "";
        P_Num = 0;
        P_Yobisu = 0;
        P_Unit = "";
        P_Hosoku = "";
        Write_Err_Data();
    }

    // 2005-10-07 �W�J�ł��Ȃ������\���i���o��
    if (i_LineNum != G_YOBICHK_RecCnt) {
        for (n = 0; n < i_ErrNum; n++) {
            P_Sikibetu = "ERROR-"; // ���i����
            P_Katasiki = s_ErrName[n]; // ���i�^��
            P_Zuban = ""; // �}�Łi�w���i�Ȃ̂Ő}�Ԃ͖����j
            P_SZuban = ""; // ��}�p�}�ԁi�w���i�Ȃ̂Ő}�Ԃ͖����j
            P_Yobihin = ""; // �\���i���ʎq
            P_Siyou = ""; // �d�l
            P_Siyou2 = ""; // �d�l2
            P_Siyou5 = "";
            P_Zaisitu = "";
            P_EZaisitu = "";
            P_Num = 0;
            P_Yobisu = i_ErrYobiNum[n];
            P_Unit = "";
            P_Hosoku = "";
            Write_Err_Data();
        }
    }

    // �\���i�ް� ̧�ٸ۰��
    fclose(wkfp);
    return i_Yobisu;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �t���i�ް�̧�ُ����ݏ���
// �T  �v      �F �������,�ݶ�����
// ��  ��      �F AnsiString a_SE   �F ���͌vSE�敪(S1�`S4,E1�`E4)
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Write_Err_Data(void) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;

    AnsiString wk_FilePass;

    // ----------------------------------------------
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    AnsiString sSpecCode;
    // **********************************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
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
    s_Text = "ERROR";
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
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), wkfp);

    // 13 �d�l�R
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 14 �d�l�S
    s_Text = "," + P_Unit;
    fputs(s_Text.c_str(), wkfp);

    // 15 �d�l�T
    s_Text = "," + P_Siyou5;
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
// ���{��֐����F ���t�����W�p�{���g�T�C�Y���}�X�^����擾
// �T  �v      �F
// ��  ��      �F I:sKikaku     �K�i������
// :  O:s_retSize   �{���g�T�C�Y�d�l
// :  O:iBltNum     �{���g�{��
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_IFrgBltSize(AnsiString sKikaku,
    AnsiString *s_retSize, int *iBltNum) {
    AnsiString s_Key;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;

    G_Log_Text = "����ݼ����Ļ���Ͻ�(HD_IFRG_BLT)�������L�[  �w" + sKikaku + "�x�Ō����B";
    Write_Log(G_Log_Text);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_IFRG_BLT";
    s_SQL = s_SQL + "  WHERE NZ_KIKAKUK = '" + sKikaku + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "�ް��쐬��    ɽ�ًK�i�" + sKikaku +
            "� �́A����ݼ����Ļ���Ͻ�(HD_IFRG_BLT)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
    //2021.10.19 �T�C�Y�Ɏd�グ�ǉ�_S
    //    *s_retSize = wkQuery->FieldByName("BLT_SIZE")->AsString.TrimRight()
    //        + "�~�S�r"; // �T�C�Y
        *s_retSize = wkQuery->FieldByName("BLT_SIZE")->AsString.TrimRight()
            + "�~�S�r  �O���N�����[�g"; // �T�C�Y
    //2021.10.19 �T�C�Y�Ɏd�グ�ǉ�_E
        *iBltNum = wkQuery->FieldByName("BLT_NUM")->AsCurrency; // �P�ӏ�������̖{��

        // ���O
        G_Log_Text = "����ݼ����Ļ���   �" + *s_retSize + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "����ݼ����Ė{��   �" + AnsiString(*iBltNum) + "����擾�B";
        Write_Log(G_Log_Text);

        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �r���X�^�b�h�{���g�T�C�Y�i�d�l���\�L�j���}�X�^����擾
// �T  �v      �F
// ��  ��      �F I:sKatasiki   �t���[���^��
// :  I:iFrgDiv     �[�ǗL��
// :  O:sSBLTSpec   �{���g�d�l
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_SSTDBltSpec(AnsiString sKatasiki,
    int iFrgDiv, AnsiString *sSBLTSpec) {
    AnsiString s_Key;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;

    G_Log_Text = "S����������Ďd�lϽ�(HD_SSTD_SPEC)�������L�[  �w" + sKatasiki + "�x�Ō����B";
    Write_Log(G_Log_Text);


    // 2015.03.20 OGI ���p���i�d���Ή�(�m�Y���K�i�̎�ނ��ǉ�����鍀��)
    //s_SQL = "";
    //s_SQL = s_SQL + "SELECT * FROM HD_SSTD_SPEC";
    //s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + sKatasiki + "'";
    //s_SQL = s_SQL + "    AND FRG_DIV = " + AnsiString(iFrgDiv);
    // �m�Y���R�[�h�̎擾
    AnsiString s_Nzl_Kind = "";
    AnsiString sTugite = P_HEADS_DATA[893].TrimRight(); // S1ɽ�ٌp��
    if (sTugite.ToIntDef(0) >= 1000) {
        s_Nzl_Kind = sTugite.SubString(1, 2) + "0" + sTugite.SubString(4, 1);
    }
    else {
        s_Nzl_Kind = sTugite.SubString(2, 1) + "0" + sTugite.SubString(4, 1);
    }
    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_SSTD_SPEC";
    s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + sKatasiki + "'";
    s_SQL = s_SQL + "    AND NRL_KIND = " + s_Nzl_Kind;
    s_SQL = s_SQL + "    AND FRG_DIV = " + AnsiString(iFrgDiv);


    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "�ް��쐬��    �t���[���^���" + sKatasiki +
            "� �́AS����������Ďd�lϽ�(HD_SSTD_SPEC)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        *sSBLTSpec = wkQuery->FieldByName("BLT_SPEC")->AsString.TrimRight
            (); // �T�C�Y

        // ���O
        G_Log_Text = "S����������Ďd�l   �" + *sSBLTSpec + "����擾�B";
        Write_Log(G_Log_Text);

        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}


// ---------------------------------------------------------------------------
// ���{��֐����F �o�חp�~�t�����W���i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2014.12.22 �ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_TFLG_Data() {
    AnsiString s_TFLG;       // �o�חp�~��ݼނ̗L��
    AnsiString s_SILICAGEL;  // �ض��ق̗L��
    AnsiString s_Code;       // ���i����
    AnsiString s_Nzl_Syurui; // ɽ�َ��
    AnsiString s_Nzl_Kokei;  // ɽ�ٌ��a
    AnsiString s_Nzl_Tugite; // ɽ�ٌp��
    AnsiString s_DNO[9];     // ��ݼސ}��
    AnsiString s_SPEC1[9];   // ��ݼގd�l1
    AnsiString s_SPEC2[9];   // ��ݼގd�l2
    int i_INOUT[9];          // �o�����L��
    int i_SI[9];             // �ض��ٗL��
    int i_GAS[9];            // �޽��ėL��
    int i_FL;                // �΍s���׸�
    int i_WkRec;             // �W�v�ް���
    bool bMatch;
    AnsiString s_WK_DNO[8];  // ��ݼސ}��(�W�v)
    AnsiString s_WK_SPEC1[8];// ��ݼގd�l1(�W�v)
    AnsiString s_WK_SPEC2[8];// ��ݼގd�l2(�W�v)
    int i_WK_FL[8];          // ��ݼސ���(�W�v)
    int i_WK_SI[8];          // �ض��ٗL��(�W�v)
    int i_WK_GAS[8];         // �޽��Đ���(�W�v)
    int i;
    int j;
    AnsiString s_code;
    AnsiString s_Text;
    // 2020.04.15 �����ܗʒǉ�_S
    AnsiString s_AB[9];           // AB�敪
    AnsiString s_Silicagel[9];    // �V���J�Q����(kg)
    AnsiString s_WK_Silicagel[8]; // �V���J�Q����(kg)(�W�v)
    // 2020.04.15 �����ܗʒǉ�_E

    s_TFLG = P_HEADS_DATA[1114].TrimRight();   // �o�חp�~��ݼ޺���
    if ( s_TFLG == "01" ) {
        s_TFLG = "1";      // �o�חp�~��ݼ�(SS400)
        s_SILICAGEL = "0"; // �ض��ٖ�
    } else if ( s_TFLG == "02" ) {
        s_TFLG = "1";      // �o�חp�~��ݼ�(SS400)
        s_SILICAGEL = "1"; // �ض��ٗL
    } else if ( s_TFLG == "03" ) {
        s_TFLG = "2";      // �o�חp�~��ݼ�(SUS304)
        s_SILICAGEL = "0"; // �ض��ٖ�
    } else if ( s_TFLG == "04" ) {
        s_TFLG = "2";      // �o�חp�~��ݼ�(SUS304)
        s_SILICAGEL = "1"; // �ض��ٗL
    } else {
        G_Log_Text = "�o�חp�~��ݼނ͕s�K�v";
        Write_Log(G_Log_Text);
        return true;
    }

    //******************
    //***  �΍s�����f
    //******************
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
        i_FL = 1; // �΍s��
    } else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
        i_FL = 1; // �΍s��
    } else {
        i_FL = 0; // ��`��
    }

    //**************
    //***  S1ɽ��
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight();   // S1ɽ�َ��
    s_Nzl_Kokei  = P_HEADS_DATA[892].TrimRight();   // S1ɽ�ٌ��a
    s_Nzl_Tugite = P_HEADS_DATA[893].TrimRight();   // S1ɽ�ٌp��
    s_DNO[1] = "";                                  // S1��ݼސ}��
    s_SPEC1[1] = "";                                // S1��ݼގd�l1
    s_SPEC2[1] = "";                                // S1��ݼގd�l2
    i_INOUT[1] = 0;                                 // S1�o�����L��
    i_GAS[1] = 0;                                   // S1�޽��ėL��
    i_SI[1] = 0;                                    // S1�ض��ٗL��

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[1] = 1;   // S1�o�����L��
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[1], &s_SPEC1[1], &s_SPEC2[1]);
        i_GAS[1] = Get_TMGASKET(s_Nzl_Tugite, "N1", i_FL);

    }

    //**************
    //***  S2ɽ��
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight();   // S2ɽ�َ��
    s_Nzl_Kokei  = P_HEADS_DATA[907].TrimRight();   // S2ɽ�ٌ��a
    s_Nzl_Tugite = P_HEADS_DATA[908].TrimRight();   // S2ɽ�ٌp��
    s_DNO[2] = "";                                  // S2��ݼސ}��
    s_SPEC1[2] = "";                                // S2��ݼގd�l1
    s_SPEC2[2] = "";                                // S2��ݼގd�l2
    i_INOUT[2] = 0;                                 // S2�o�����L��
    i_GAS[2] = 0;                                   // S2�޽��ėL��
    i_SI[2] = 0;                                    // S2�ض��ٗL��

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[2] = 1;   // S2�o�����L��_
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[2], &s_SPEC1[2], &s_SPEC2[2]);
        i_GAS[2] = Get_TMGASKET(s_Nzl_Tugite, "N2", i_FL);

    }

    //**************
    //***  S3ɽ��
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight();   // S3ɽ�َ��
    s_Nzl_Kokei  = P_HEADS_DATA[922].TrimRight();   // S3ɽ�ٌ��a
    s_Nzl_Tugite = P_HEADS_DATA[923].TrimRight();   // S3ɽ�ٌp��
    s_DNO[3] = "";                                  // S3��ݼސ}��
    s_SPEC1[3] = "";                                // S3��ݼގd�l1
    s_SPEC2[3] = "";                                // S3��ݼގd�l2
    i_INOUT[3] = 0;                                 // S3�o�����L��
    i_GAS[3] = 0;                                   // S3�޽��ėL��
    i_SI[3] = 0;                                    // S3�ض��ٗL��

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[3] = 1;   // S3�o�����L��_
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[3], &s_SPEC1[3], &s_SPEC2[3]);
        i_GAS[3] = Get_TMGASKET(s_Nzl_Tugite, "N3", i_FL);

    }

    //**************
    //***  S4ɽ��
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight();   // S4ɽ�َ��
    s_Nzl_Kokei  = P_HEADS_DATA[937].TrimRight();   // S4ɽ�ٌ��a
    s_Nzl_Tugite = P_HEADS_DATA[938].TrimRight();   // S4ɽ�ٌp��
    s_DNO[4] = "";                                  // S4��ݼސ}��
    s_SPEC1[4] = "";                                // S4��ݼގd�l1
    s_SPEC2[4] = "";                                // S4��ݼގd�l2
    i_INOUT[4] = 0;                                 // S4�o�����L��
    i_GAS[4] = 0;                                   // S4�޽��ėL��
    i_SI[4] = 0;                                    // S4�ض��ٗL��

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[4] = 1;   // S4�o�����L��_
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[4], &s_SPEC1[4], &s_SPEC2[4]);
        i_GAS[4] = Get_TMGASKET(s_Nzl_Tugite, "N4", i_FL);

    }


    //**************
    //***  E1ɽ��
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight();   // E1ɽ�َ��
    s_Nzl_Kokei  = P_HEADS_DATA[952].TrimRight();   // E1ɽ�ٌ��a
    s_Nzl_Tugite = P_HEADS_DATA[953].TrimRight();   // E1ɽ�ٌp��
    s_DNO[5] = "";                                  // E1��ݼސ}��
    s_SPEC1[5] = "";                                // E1��ݼގd�l1
    s_SPEC2[5] = "";                                // E1��ݼގd�l2
    i_INOUT[5] = 0;                                 // E1�o�����L��
    i_GAS[5] = 0;                                   // E1�޽��ėL��
    i_SI[5] = 0;                                    // E1�ض��ٗL��

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[5] = 1;   // E1�o�����L��
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[5], &s_SPEC1[5], &s_SPEC2[5]);
        i_GAS[5] = Get_TMGASKET(s_Nzl_Tugite, "N1", i_FL);

    }


    //**************
    //***  E2ɽ��
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight();   // E2ɽ�َ��
    s_Nzl_Kokei  = P_HEADS_DATA[967].TrimRight();   // E2ɽ�ٌ��a
    s_Nzl_Tugite = P_HEADS_DATA[968].TrimRight();   // E2ɽ�ٌp��
    s_DNO[6] = "";                                  // E2��ݼސ}��
    s_SPEC1[6] = "";                                // E2��ݼގd�l1
    s_SPEC2[6] = "";                                // E2��ݼގd�l2
    i_INOUT[6] = 0;                                 // E2�o�����L��
    i_GAS[6] = 0;                                   // E2�޽��ėL��
    i_SI[6] = 0;                                    // E2�ض��ٗL��

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[6] = 1;   // E2�o�����L��
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[6], &s_SPEC1[6], &s_SPEC2[6]);
        i_GAS[6] = Get_TMGASKET(s_Nzl_Tugite, "N2", i_FL);

    }


    //**************
    //***  E3ɽ��
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight();   // E3ɽ�َ��
    s_Nzl_Kokei  = P_HEADS_DATA[982].TrimRight();   // E3ɽ�ٌ��a
    s_Nzl_Tugite = P_HEADS_DATA[983].TrimRight();   // E3ɽ�ٌp��
    s_DNO[7] = "";                                  // E3��ݼސ}��
    s_SPEC1[7] = "";                                // E3��ݼގd�l1
    s_SPEC2[7] = "";                                // E3��ݼގd�l2
    i_INOUT[7] = 0;                                 // E3�o�����L��
    i_GAS[7] = 0;                                   // E3�޽��ėL��
    i_SI[7] = 0;                                    // E3�ض��ٗL��

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[7] = 1;   // E3�o�����L��
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[7], &s_SPEC1[7], &s_SPEC2[7]);
        i_GAS[7] = Get_TMGASKET(s_Nzl_Tugite, "N3", i_FL);

    }


    //**************
    //***  E4ɽ��
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight();   // E4ɽ�َ��
    s_Nzl_Kokei  = P_HEADS_DATA[997].TrimRight();   // E4ɽ�ٌ��a
    s_Nzl_Tugite = P_HEADS_DATA[998].TrimRight();   // E4ɽ�ٌp��
    s_DNO[8] = "";                                  // E4��ݼސ}��
    s_SPEC1[8] = "";                                // E4��ݼގd�l1
    s_SPEC2[8] = "";                                // E4��ݼގd�l2
    i_INOUT[8] = 0;                                 // E4�o�����L��
    i_GAS[8] = 0;                                   // E2�޽��ėL��
    i_SI[8] = 0;                                    // E4�ض��ٗL��

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[8] = 1;   // E4�o�����L��
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[8], &s_SPEC1[8], &s_SPEC2[8]);
        i_GAS[8] = Get_TMGASKET(s_Nzl_Tugite, "N4", i_FL);

    }


    //******************
    //***  �����܈ʒu
    //******************
    if (i_FL == 0) {
        // ��`��
        // �a��
        if (i_INOUT[3] == 1 ) {
            //S3
            i_SI[3] = 1;
        } else if (i_INOUT[7] == 1) {
            //E3
            i_SI[7] = 1;
        } else if (i_INOUT[1] == 1) {
            //S1
            i_SI[1] = 1;
        } else if (i_INOUT[5] == 1) {
            //E1
            i_SI[5] = 1;
        }
        // �`��
        if (i_INOUT[4] == 1 ) {
            //S4
            i_SI[4] = 1;
        } else if (i_INOUT[8] == 1) {
            //E4
            i_SI[8] = 1;
        } else if (i_INOUT[2] == 1) {
            //S2
            i_SI[2] = 1;
        } else if (i_INOUT[6] == 1) {
            //E2
            i_SI[6] = 1;
        }
        // 2020.04.15 �����ܗʒǉ�_S
        s_AB[1] ="B";
        s_AB[2] ="A";
        s_AB[3] ="B";
        s_AB[4] ="A";
        s_AB[5] ="B";
        s_AB[6] ="A";
        s_AB[7] ="B";
        s_AB[8] ="A";
        // 2020.04.15 �����ܗʒǉ�_E
    } else {
        // �΍s��
        // �a��
        if (i_INOUT[4] == 1 ) {
            //S4
            i_SI[4] = 1;
        } else if (i_INOUT[8] == 1) {
            //E4
            i_SI[8] = 1;
        } else if (i_INOUT[1] == 1) {
            //S1
            i_SI[1] = 1;
        } else if (i_INOUT[5] == 1) {
            //E1
            i_SI[5] = 1;
        }
        // �`��
        if (i_INOUT[3] == 1 ) {
            //S3
            i_SI[3] = 1;
        } else if (i_INOUT[7] == 1) {
            //E3
            i_SI[7] = 1;
        } else if (i_INOUT[2] == 1) {
            //S2
            i_SI[2] = 1;
        } else if (i_INOUT[6] == 1) {
            //E2
            i_SI[6] = 1;
        }
        // 2020.04.15 �����ܗʒǉ�_S
        s_AB[1] ="B";
        s_AB[2] ="A";
        s_AB[3] ="A";
        s_AB[4] ="B";
        s_AB[5] ="B";
        s_AB[6] ="A";
        s_AB[7] ="A";
        s_AB[8] ="B";
        // 2020.04.15 �����ܗʒǉ�_E
    }

    if (s_SILICAGEL == "0") {
        // �ض��ٖ�
        i_SI[1] = 0;   // S1
        i_SI[2] = 0;   // S2
        i_SI[3] = 0;   // S3
        i_SI[4] = 0;   // S4
        i_SI[5] = 0;   // E1
        i_SI[6] = 0;   // E2
        i_SI[7] = 0;   // E3
        i_SI[8] = 0;   // E4
        // 2020.04.15 �����ܗʒǉ�_S
        s_Silicagel[1] = "  0.00";
        s_Silicagel[2] = "  0.00";
        s_Silicagel[3] = "  0.00";
        s_Silicagel[4] = "  0.00";
        s_Silicagel[5] = "  0.00";
        s_Silicagel[6] = "  0.00";
        s_Silicagel[7] = "  0.00";
        s_Silicagel[8] = "  0.00";
        // 2020.04.15 �����ܗʒǉ�_E
    }
    // 2020.04.15 �����ܗʒǉ�_S
    else {
        // �ض��َ擾
        for ( i = 0; i < 8 ; i++ ) {

            if ( i_SI[i] == 0 ) {
                // �ض��ٖ�
                s_Silicagel[i] = "  0.00";
            }
            else {
                // �ض��ٗL
                s_Silicagel[i] = Get_Silicagel(s_AB[i]);
            }

        }

    }
    // 2020.04.15 �����ܗʒǉ�_E

    //******************
    //***   ���i�W�v
    //******************

    //------------------------------------------------
    // ���W�v
    // �[s_DNO]�}��,[s_SPEC1]�d�l1,[s_SPEC2]�d�l2,
    //  [i_INOUT]�o�����L��,[i_SI]�ض��ٗL����
    //  �S�Ĉ�v�����ް���1�ɂ܂Ƃ߂�
    //------------------------------------------------
    i_WkRec = 0;
    for ( i = 0; i < 8 ; i++ ) {

        s_WK_DNO[i] = "";
        s_WK_SPEC1[i] = "";
        s_WK_SPEC2[i] = "";
        i_WK_FL[i] = 0;
        i_WK_SI[i] = 0;
        i_WK_GAS[i] = 0;
        // 2020.04.15 �����ܗʒǉ�_S
        s_WK_Silicagel[1] = "  0.00";
        // 2020.04.15 �����ܗʒǉ�_E

    }

    for ( i = 1; i < 9 ; i++ ) {

        if ( i_INOUT[i] == 1 ) {
            //*****************************************
            // ���Ɋi�[����Ă��镔�i�ް��Ɣ�r
            //*****************************************
            bMatch = false;
            for ( j = 0; j < i_WkRec ; j++ ) {

                // 2020.04.15 �����ܗʒǉ�_S
                //if( ( s_DNO[i] == s_WK_DNO[j] ) &&
                //    ( s_SPEC1[i] == s_WK_SPEC1[j] ) &&
                //    ( s_SPEC2[i] == s_WK_SPEC2[j] ) &&
                //    ( i_SI[i] == i_WK_SI[j] ) ) {
                //
                //    // ���ʏW�v
                //    i_WK_FL[j] = i_WK_FL[j] + i_INOUT[i];
                //    i_WK_GAS[j] = i_WK_GAS[j] + i_GAS[i];
                //
                //    bMatch = true;
                //
                //}
                if( ( s_DNO[i] == s_WK_DNO[j] ) &&
                    ( s_SPEC1[i] == s_WK_SPEC1[j] ) &&
                    ( s_SPEC2[i] == s_WK_SPEC2[j] ) &&
                    ( i_SI[i] == i_WK_SI[j] ) &&
                    ( s_Silicagel[i] == s_WK_Silicagel[j] ) ) {

                    // ���ʏW�v
                    i_WK_FL[j] = i_WK_FL[j] + i_INOUT[i];
                    i_WK_GAS[j] = i_WK_GAS[j] + i_GAS[i];

                    bMatch = true;

                }
                // 2020.04.15 �����ܗʒǉ�_E

            }
            //*****************************************
            // �Y���������ʂȂ��̏ꍇܰ����i�ް��Ɋi�[
            //*****************************************
            if ( bMatch ) {
            } else {
                s_WK_DNO[i_WkRec] = s_DNO[i];
                s_WK_SPEC1[i_WkRec] = s_SPEC1[i];
                s_WK_SPEC2[i_WkRec] = s_SPEC2[i];
                i_WK_FL[i_WkRec] = i_INOUT[i];
                i_WK_SI[i_WkRec] = i_SI[i];
                i_WK_GAS[i_WkRec] = i_GAS[i];
                // 2020.05.08 �����ܗʒǉ�_S
                s_WK_Silicagel[i_WkRec] = s_Silicagel[i];
                // 2020.05.08 �����ܗʒǉ�_E

                i_WkRec++;
            }

        }

    }

    //**********************
    //***  �o�חp�~��ݼ�
    //**********************
    for ( i = 0; i < i_WkRec ; i++ ) {
        P_Sikibetu = "TFRG--";                         // ���ʎq
        P_Katasiki = "�o�חp�~�t�����W";             // ���i�^��
        P_Hosoku = "";                                 // ���i����
        if ( s_TFLG == "1" ) {
            // SS400,t3.2
            P_Zaisitu = Search_HD_MATERIAL("1040");    // �ގ�����
            P_EZaisitu = Search_HD_E_MATERIAL("1040"); // �ގ�����(�p)
            P_Siyou = "�@���R�D�Q";                    // �d�l
        } else if ( s_TFLG == "2" ) {
            // SUS304,t3
            P_Zaisitu = Search_HD_MATERIAL("1001");    // �ގ�����
            P_EZaisitu = Search_HD_E_MATERIAL("1001"); // �ގ�����(�p)
            P_Siyou = "�@���R�@�@";                    // �d�l
        }
        P_Zuban = s_WK_DNO[i];                         // �}��
        P_SZuban = s_WK_DNO[i];                        // ��}�p�}��
        P_Num =  i_WK_FL[i];                           // 1�䕪����
        P_Yobisu = 0;                                  // �\����
        P_Siyou = s_WK_SPEC1[i] + P_Siyou;             // �d�l1
        if ( i_WK_SI[i]  == 1 ) {
            // �����ܕt
            P_Siyou = P_Siyou + "�@�����ܕt";          // �d�l1
            // 2020.04.15 �����ܗʒǉ�_S
            P_Siyou = P_Siyou + s_WK_Silicagel[i].SubString(2,5); // �d�l1
            P_Siyou = P_Siyou +" kg";         // �d�l1
            // 2020.04.15 �����ܗʒǉ�_E
        }
        P_Siyou2 = "";                                 // �d�l2
        P_Unit = "";                                   // �d�l4
        P_Siyou5 = "";                                 // �d�l5
        s_code = "23";                                 // �~(��ײ���)��ݼ�
        Write_Axsrs_Data(s_code);
    }

    //**********************
    //***  ��ݼ޶޽���
    //**********************
    for ( i = 0; i < i_WkRec ; i++ ) {
        if (i_WK_GAS[i] == 0) {
        } else {
            if ( s_WK_SPEC2[i] == "FR" ) {
                // �ݸ޶޽���
                P_Sikibetu = "STGKTR";                 // ���ʎq
                P_Katasiki = "�����O�K�X�P�b�g";       // ���i�^��
                s_code = "25";                         // �~��Ķ޽���
            } else if ( s_WK_SPEC2[i] == "FF" ) {
                // 2020.12.16 �޽��Ė��ύX_S
                //// �S�ʍ��~�޽���
                //P_Sikibetu = "STGKTF";                 // ���ʎq
                //P_Katasiki = "�S�ʍ��K�X�P�b�g";       // ���i�^��
                // �S�ʍ���ݼ޶޽���
                P_Sikibetu = "STGKTF";                 // ���ʎq
                P_Katasiki = "�S�ʍ��t�����W�K�X�P�b�g"; // ���i�^��
                // 2020.12.16 �޽��Ė��ύX_S
                s_code = "26";                         // �S�ʍ��~�޽���
            } else {
                // ���̑�
                P_Sikibetu = "STGKT-";                 // ���ʎq
                P_Katasiki = "";                       // ���i�^��
                s_code = "";
            }
            P_Hosoku = "";                             // ���i����
            P_Zaisitu = Search_HD_MATERIAL("2019");    // �ގ�����
            P_Zuban = "";                              // �}��
            P_SZuban = "";                             // ��}�p�}��
            P_Num =  i_WK_GAS[i];                      // 1�䕪����
            P_Yobisu = 0;                              // �\����
            P_Siyou = s_WK_SPEC1[i];                   // �d�l1
            P_Siyou2 = "";                             // �d�l2
            P_Unit = "";                               // �d�l4
            P_Siyou5 = "";                             // �d�l5
            Write_Axsrs_Data(s_code);
        }

    }

    return true;
}


// ---------------------------------------------------------------------------
// ���{��֐����F �o�חp�~�t�����W���}�X�^����擾
// �T  �v      �F
// ��  ��      �F I:Tugite      �m�Y���p��R�[�h
//             �F I:Kokei       �m�Y�����a
//             �F O:s_DNO       �~�t�����W�}��
//             �F O:s_SPEC1     �~�t�����W�K�i����
//             �F O:s_SPEC2     �K�X�P�b�g���
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2014.12.22 �ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Search_HD_TEMPORARY( AnsiString Tugite,
    AnsiString Kokei, AnsiString *s_DNO, AnsiString *s_SPEC1,
    AnsiString *s_SPEC2) {

    AnsiString s_Text1;
    AnsiString s_Text2;
    AnsiString s_Key;

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_SQL;

    // *********************************************
    // ***�@�o�חp�~��ݼ�Ͻ�(HD_TEMPORARY) ����
    // *********************************************

    G_Log_Text = "�o�חp�~��ݼ�Ͻ�(HD_TEMPORARY)�������L�[  �w" + Tugite + "," + Kokei + "�x�Ō����B";
    Write_Log(G_Log_Text);

    s_Text1 = Tugite.SubString(1,2);
    s_Text2 = Tugite.SubString(4,1);
    s_Key= s_Text1 + "0" +s_Text2;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_TEMPORARY WHERE ";
    s_SQL = s_SQL + "  FCODE = '" + s_Key + "'";
    s_SQL = s_SQL + "    AND FLANGE_SIZE = '" + Kokei + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "�ް��쐬��    �" + Tugite + "," + Kokei +
            "� �́A�o�חp�~��ݼ�Ͻ�(HD_TEMPORARY)�ɑ��݂��܂���B";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        // �~�t�����W�}��
        *s_DNO = wkQuery->FieldByName("DNO")->AsString.TrimRight();
        // �~�t�����W�K�i����
        *s_SPEC1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight();
        // �K�X�P�b�g���
        *s_SPEC2  = wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
        // �\���i���ʎq

        // ���O
        G_Log_Text = "�~��ݼސ}��   �" + *s_DNO + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�~��ݼދK�i����   �" + *s_SPEC1 + "����擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�޽��Ď��   �" + *s_SPEC2 + "����擾�B";
        Write_Log(G_Log_Text);

        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}


// ---------------------------------------------------------------------------
// ���{��֐����F �o�חp�~�t�����W�K�X�P�b�g�L��
// �T  �v      �F
// ��  ��      �F I:Tugite      �m�Y���p��R�[�h
//             �F I:Syurui      �m�Y���ʒu
//             �F I:Fl          ������� [0:��`�� 1:�΍s��]
// �߂�l      �F �K�X�P�b�g�L�� [1:�L 0:��]
// ��  �l      �F 2014.12.22 �ǉ�
// ---------------------------------------------------------------------------
int __fastcall TAxsrsDetails::Get_TMGASKET(AnsiString Tugite,
    AnsiString Syurui, int Fl) {

    int i_Tan_mat;
    AnsiString s_Boots;
       AnsiString s_Text;


    // �[���ގ�
    if (Syurui=="N1") {
        i_Tan_mat = P_HEADS_DATA[1136].ToIntDef(0);
    } else if (Syurui=="N2") {
        i_Tan_mat = P_HEADS_DATA[1137].ToIntDef(0);
    } else if (Syurui=="N3") {
        if (Fl==0) {
            // ��`��
            i_Tan_mat = P_HEADS_DATA[1136].ToIntDef(0);
        } else {
            // �΍s��
            i_Tan_mat = P_HEADS_DATA[1137].ToIntDef(0);
        }
    } else if (Syurui=="N4") {
        if (Fl==0) {
             // ��`��
            i_Tan_mat = P_HEADS_DATA[1137].ToIntDef(0);
        } else {
            // �΍s��
            i_Tan_mat = P_HEADS_DATA[1136].ToIntDef(0);
        }
    }

    // �u�[�c���
    s_Boots = Tugite.SubString(3,1);

    if (s_Boots != "1") {
        // ���o�[�u�[�c�ȊO
        return(1);
    } else if (i_Tan_mat != 0) {
        // �[�ǂ��t��
        return(1);
    } else if (P_HEADS_DATA[1068].TrimRight() != "0000") {
        // ���t�����W���t��
        return(1);
    } else {
        // �K�X�P�b�g�s�v
        return(0);
    }

}


//---------------------------------------------------------------------------
// ���{��֐����F �V�[�g�K�X�P�b�g�ގ��擾
// �T  �v      �F
// ��  ��      �F s_Side�F���̎��(A:�`��,B:�a��)
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2017.10.10�ǉ�
//---------------------------------------------------------------------------
AnsiString __fastcall TAxsrsDetails::Get_SheetGasketMat(AnsiString s_Side)
{
   int i_Flow;                // ���̎��
   int i_RecNo;
   double d_Bdt;              // �a���݌v���x
   double d_Adt;              // �`���݌v���x

   AnsiString s_Katasiki;     // �^��
   AnsiString s_Syurui;       // ���
   AnsiString s_Flow1;        // �a�X�`�[���敪(CASE1)
   AnsiString s_Flow2;        // �`�X�`�[���敪(CASE1)
   AnsiString s_Flow3;        // �a�X�`�[���敪(CASE2)
   AnsiString s_Flow4;        // �`�X�`�[���敪(CASE2)
   AnsiString s_GasMat;       // �V�[�g�K�X�P�b�g�ގ�
   AnsiString s_Text;

   //***************************
   //***  �^       ��
   //***************************
   i_RecNo = 33;
   s_Katasiki = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  ��       ��
   //***************************
   i_RecNo = 34;
   s_Syurui = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  �a�X�`�[���敪(CASE1)
   //***************************
   i_RecNo = 846;
   s_Flow1 = P_HEADS_DATA[i_RecNo].TrimRight();

    //***************************
   //***  �`�X�`�[���敪(CASE1)
   //***************************
   i_RecNo = 847;
   s_Flow2 = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  �a�X�`�[���敪(CASE2)
   //***************************
   i_RecNo = 869;
   s_Flow3 = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  �`�X�`�[���敪(CASE2)
   //***************************
   i_RecNo = 870;
   s_Flow4 = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  �a���݌v���x
   //***************************
   i_RecNo = 829;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   d_Bdt = s_Text.ToDouble();

   //***************************
   //***  �`���݌v���x
   //***************************
   i_RecNo = 834;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   d_Adt = s_Text.ToDouble();

   //***************************
   //***  �� �� �� ��
   //***************************
   if (s_Katasiki.SubString(3,1) == "C") {
      //�R���f���T�@
      i_Flow = 3;
   } else if (s_Katasiki.SubString(4,1) == "C") {
      //�R���f���T�@
      i_Flow = 3;
   } else if (s_Syurui.SubString(1,2) == "YX") {
      //�R���f���T�@
      i_Flow = 3;
   } else {
      // �t��(���C)
      i_Flow = 0;
   }

   if (i_Flow != 3) {
      if(s_Side == "B") {
         // �a��
         if(s_Flow1 == "1" || s_Flow3 == "1") {
            if (d_Bdt >= 150) {
               // ���C�Q
               i_Flow = 2;
            } else {
               // ���C�P
               i_Flow = 1;
            }
         }
      } else if(s_Side == "A") {
         // �`��
         if(s_Flow2 == "1" || s_Flow4 == "1") {
            if (d_Adt >= 150) {
               // ���C�Q
               i_Flow = 2;
            } else {
               // ���C�P
               i_Flow = 1;
            }
         }
      }
   }

   if (i_Flow == 0) {
      G_Log_Text = "���̎�ށw�t�́x";
   } else if (i_Flow == 1) {
      G_Log_Text = "���̎�ށw���C�P�x";
   } else if (i_Flow == 2) {
      G_Log_Text = "���̎�ށw���C�Q�x";
   } else if (i_Flow == 3) {
      G_Log_Text = "���̎�ށw�R���f���T�x";
   } else {
      G_Log_Text = "���̎�ށw�G���[�x";
   }
   Write_Log(G_Log_Text);

   //***************************
   //***  �� �� �I ��
   //***************************
   s_GasMat="";
   s_Text="";
   if (i_Flow == 1) {
      // ���C�P�i�d�o�c�l�j
      s_GasMat ="2026";
   } else if (i_Flow == 2) {
      // ���C�Q�i�o���J�[�m���D�f�e�R�O�O�j
      s_GasMat ="1188";
   } else {
      if(s_Side=="B") {
         //�a���v���[�g�K�X�P�b�g
         i_RecNo = 835;
         s_GasMat = P_HEADS_DATA[i_RecNo].TrimRight();
      } else if (s_Side=="A") {
         //�`���v���[�g�K�X�P�b�g
         i_RecNo = 836;
         s_GasMat = P_HEADS_DATA[i_RecNo].TrimRight();
      }
   }

   G_Log_Text = "�V�[�g�K�X�P�b�g�ގ��R�[�h�w" + s_GasMat + "�x";
   Write_Log(G_Log_Text);

   return s_GasMat;

}


// ---------------------------------------------------------------------------
// ���{��֐����F �V�[�g�K�X�P�b�g���̎擾
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2020.12.16 �ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_SeatGas_Name() {
    int i_FL;                // �΍s���׸�
    double d_BKou;      // �a�V�[�g�K�X�P�b�g���a
    double d_AKou;      // �`�V�[�g�K�X�P�b�g���a
    AnsiString s_BKou;  // �a�V�[�g�K�X�P�b�g���a
    AnsiString s_AKou;  // �`�V�[�g�K�X�P�b�g���a
    AnsiString s_BKika; // �a�V�[�g�K�X�P�b�g�m�Y���K�i
    AnsiString s_AKika; // �`�V�[�g�K�X�P�b�g�m�Y���K�i


    //******************
    //***  �΍s�����f
    //******************
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
        i_FL = 1; // �΍s��
    } else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
        i_FL = 1; // �΍s��
    } else {
        i_FL = 0; // ��`��
    }


    //******************�@
    //***  �a�����a
    //******************
    s_BKou = "0";     // �a�V�[�g�K�X�P�b�g���a
    s_BKika = "0000"; // �a�V�[�g�K�X�P�b�g�m�Y���K�i

    // S1
    if (P_HEADS_DATA[891].TrimRight() == "1" ||
           P_HEADS_DATA[891].TrimRight() == "2") {
        s_BKou = P_HEADS_DATA[892].TrimRight();
        s_BKika = P_HEADS_DATA[893].TrimRight();
    }
    // S3
    else if (P_HEADS_DATA[921].TrimRight() == "1" ||
             P_HEADS_DATA[921].TrimRight() == "2") {
        if (i_FL == 0) { // ��`��
            s_BKou = P_HEADS_DATA[922].TrimRight();
            s_BKika = P_HEADS_DATA[923].TrimRight();
        }
    }
    // S4
    else if (P_HEADS_DATA[936].TrimRight() == "1" ||
             P_HEADS_DATA[936].TrimRight() == "2") {
        if (i_FL != 0) { // �΍s��
            s_BKou = P_HEADS_DATA[937].TrimRight();
            s_BKika = P_HEADS_DATA[938].TrimRight();
        }
    }
    // E1
    else if (P_HEADS_DATA[951].TrimRight() == "1" ||
             P_HEADS_DATA[951].TrimRight() == "2") {
        s_BKou = P_HEADS_DATA[952].TrimRight();
        s_BKika = P_HEADS_DATA[953].TrimRight();
    }
    // E3
    else if (P_HEADS_DATA[981].TrimRight() == "1" ||
             P_HEADS_DATA[981].TrimRight() == "2") {
        if (i_FL == 0) { // ��`��
            s_BKou = P_HEADS_DATA[982].TrimRight();
            s_BKika = P_HEADS_DATA[983].TrimRight();
        }
    }
    // E4
    else if (P_HEADS_DATA[996].TrimRight() == "1" ||
             P_HEADS_DATA[996].TrimRight() == "2") {
        if (i_FL == 0) { // �΍s��
            s_BKou = P_HEADS_DATA[997].TrimRight();
            s_BKika = P_HEADS_DATA[998].TrimRight();
        }
    }
    d_BKou = s_BKou.ToDouble(); // �a�V�[�g�K�X�P�b�g���a


    //******************�@
    //***  �`�����a
    //******************
    s_AKou = "0";     // �`�V�[�g�K�X�P�b�g���a
    s_AKika = "0000"; // �`�V�[�g�K�X�P�b�g�m�Y���K�i

    // �`�����a
    // S2
    if (P_HEADS_DATA[906].TrimRight() == "1" ||
        P_HEADS_DATA[906].TrimRight() == "2") {
        s_AKou = P_HEADS_DATA[907].TrimRight();
        s_AKika = P_HEADS_DATA[908].TrimRight();
    }
    // S3
    else if (P_HEADS_DATA[921].TrimRight() == "1" ||
             P_HEADS_DATA[921].TrimRight() == "2") {
        if (i_FL != 0) { // �΍s��
            s_AKou = P_HEADS_DATA[922].TrimRight();
            s_AKika = P_HEADS_DATA[923].TrimRight();
        }
    }
    // S4
    else if (P_HEADS_DATA[936].TrimRight() == "1" ||
             P_HEADS_DATA[936].TrimRight() == "2") {
        if (i_FL == 0) { // ��`��
            s_AKou = P_HEADS_DATA[937].TrimRight();
            s_AKika = P_HEADS_DATA[938].TrimRight();
        }
    }
    // E2
    else if (P_HEADS_DATA[966].TrimRight() == "1" ||
             P_HEADS_DATA[966].TrimRight() == "2") {
        s_AKou = P_HEADS_DATA[967].TrimRight();
        s_AKika = P_HEADS_DATA[968].TrimRight();
    }
    // E3
    else if (P_HEADS_DATA[981].TrimRight() == "1" ||
             P_HEADS_DATA[981].TrimRight() == "2") {
        if (i_FL != 0) { // �΍s��
            s_AKou = P_HEADS_DATA[982].TrimRight();
            s_AKika = P_HEADS_DATA[983].TrimRight();
        }
    }
    // E4
    else if (P_HEADS_DATA[996].TrimRight() == "1" ||
             P_HEADS_DATA[996].TrimRight() == "2") {
        if (i_FL == 0) { // ��`��
            s_AKou = P_HEADS_DATA[997].TrimRight();
            s_AKika = P_HEADS_DATA[998].TrimRight();
        }
    }
    d_AKou = s_AKou.ToDouble(); // �`�V�[�g�K�X�P�b�g���a


    //******************�@
    //***  �a������
    //******************
    if ( d_BKou >= 250 ) {
        P_GasNameB = "�S�ʍ��t�����W�K�X�P�b�g";
    } else if (s_BKika.SubString(1,2) == "04" ||
               s_BKika.SubString(1,2) == "05" ||
               s_BKika.SubString(1,2) == "44" ||
               s_BKika.SubString(1,2) == "08" ||
               s_BKika.SubString(1,2) == "09" ||
               s_BKika.SubString(1,2) == "48") {
        if ( d_BKou >= 10 ) {
            P_GasNameB = "�S�ʍ��t�����W�K�X�P�b�g";
        } else {
            P_GasNameB = "�����O�K�X�P�b�g";
        }
    } else {
        P_GasNameB = "�����O�K�X�P�b�g";
    }


    //******************�@
    //***  �`������
    //******************
    if ( d_AKou >= 250 ) {
        P_GasNameA = "�S�ʍ��t�����W�K�X�P�b�g";
    } else if (s_AKika.SubString(1,2) == "04" ||
               s_AKika.SubString(1,2) == "05" ||
               s_AKika.SubString(1,2) == "44" ||
               s_AKika.SubString(1,2) == "08" ||
               s_AKika.SubString(1,2) == "09" ||
               s_AKika.SubString(1,2) == "48") {
        if ( d_AKou >= 10 ) {
            P_GasNameA = "�S�ʍ��t�����W�K�X�P�b�g";
        } else {
            P_GasNameA = "�����O�K�X�P�b�g";
        }
    } else {
        P_GasNameA = "�����O�K�X�P�b�g";
    }


    G_Log_Text = "�`���V�[�g�K�X�P�b�g���́w" + P_GasNameA + "�x";
    Write_Log(G_Log_Text);


    G_Log_Text = "s_BKou�w" + s_BKou + "�x";
    Write_Log(G_Log_Text);
    G_Log_Text = "s_BKika�w" + s_BKika + "�x";
    Write_Log(G_Log_Text);
    G_Log_Text = "s_AKou�w" + s_AKou + "�x";
    Write_Log(G_Log_Text);
    G_Log_Text = "s_AKika�w" + s_AKika + "�x";
    Write_Log(G_Log_Text);


    G_Log_Text = "�a���V�[�g�K�X�P�b�g���́w" + P_GasNameB + "�x";
    Write_Log(G_Log_Text);

    return true;
}

