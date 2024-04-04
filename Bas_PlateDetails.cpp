// ---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�F��ڰĕ��i�W�J����  ���W���[��
// �� �� �ҁFN.Uchida
// �� �� ���F2002.06.18
// �X �V ���F2002.10.16
// �F2002.10.23 �K�X�P�b�g�����ǉ�
// 2007.10.03 �~���[�d�l�Ή�
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

#include "Bas_PlateDetails.h"

#include "HeadsData.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// ��ڰĕ��i�W�J���� Class
// ---------------------------------------------------------------------------
class TPlateDetails {
private:
    FILE *fp;
    // FILE *wkfp;

    // ��2002/10/23
    // BFRGð��ق��̂܂�
    long BP_Hp;
    AnsiString BBP_Name;
    AnsiString BBP_Label;
    long BBP_Code;
    long BBP_Num;
    AnsiString ABP_Name;
    AnsiString ABP_Label;
    long ABP_Code;
    long ABP_Num;
    AnsiString Note;
    // D�޽���ð��ق��̂܂�
    AnsiString DP_B1s1_Name;
    AnsiString DP_B1s1_Label;
    long DP_B1s1_Code;
    long DP_B1s1_Num;
    AnsiString DP_B0s1_Name;
    AnsiString DP_B0s1_Label;
    long DP_B0s1_Code;
    long DP_B0s1_Num;
    AnsiString DP_B1s2_Name;
    AnsiString DP_B1s2_Label;
    long DP_B1s2_Code;
    long DP_B1s2_Num;
    AnsiString DP_B0s2_Name;
    AnsiString DP_B0s2_Label;
    long DP_B0s2_Code;
    long DP_B0s2_Num;
    long DPU_Meq_Sub;
    AnsiString DP_B1s3_Name;
    AnsiString DP_B1s3_Label;
    long DP_B1s3_Code;
    long DP_B1s3_Num;
    AnsiString DP_B0s3_Name;
    AnsiString DP_B0s3_Label;
    long DP_B0s3_Code;
    long DP_B0s3_Num;
    AnsiString DP_B1s4_Name;
    AnsiString DP_B1s4_Label;
    long DP_B1s4_Code;
    long DP_B1s4_Num;
    AnsiString DP_B0s4_Name;
    AnsiString DP_B0s4_Label;
    long DP_B0s4_Code;
    long DP_B0s4_Num;
    long DPL_Meq_Sub;
    AnsiString DD_Name;
    AnsiString DD_Label;
    long DD_Code;
    long DD_Num;
    // E�޽���ð��ق��̂܂�
    AnsiString EP_B1s1_Name;
    AnsiString EP_B1s1_Label;
    long EP_B1s1_Code;
    long EP_B1s1_Num;
    AnsiString EP_B0s1_Name;
    AnsiString EP_B0s1_Label;
    long EP_B0s1_Code;
    long EP_B0s1_Num;
    AnsiString EP_B1s2_Name;
    AnsiString EP_B1s2_Label;
    long EP_B1s2_Code;
    long EP_B1s2_Num;
    AnsiString EP_B0s2_Name;
    AnsiString EP_B0s2_Label;
    long EP_B0s2_Code;
    long EP_B0s2_Num;
    long EPUH_Meq_Sub;
    long EPU_Meq_Sub;
    AnsiString EP_B1s3_Name;
    AnsiString EP_B1s3_Label;
    long EP_B1s3_Code;
    long EP_B1s3_Num;
    AnsiString EP_B0s3_Name;
    AnsiString EP_B0s3_Label;
    long EP_B0s3_Code;
    long EP_B0s3_Num;
    AnsiString EP_B1s4_Name;
    AnsiString EP_B1s4_Label;
    long EP_B1s4_Code;
    long EP_B1s4_Num;
    AnsiString EP_B0s4_Name;
    AnsiString EP_B0s4_Label;
    long EP_B0s4_Code;
    long EP_B0s4_Num;
    long EPLH_Meq_Sub;
    long ED_Hp;
    // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_S
    //AnsiString ED_Name;
    //AnsiString ED_Label;
    //long ED_Code;
    //long ED_Num;
    AnsiString ED_B1_Name;
    AnsiString ED_B1_Label;
    long ED_B1_Code;
    long ED_B1_Num;
    AnsiString ED_B0_Name;
    AnsiString ED_B0_Label;
    long ED_B0_Code;
    long ED_B0_Num;
    // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_E
    AnsiString DE_Kubun;
    // ��2002/10/23

    // ��02.08.20 Add
    AnsiString P_Sakuban_Mode; // ��Ԏ��(S:�����ԍ��CM:���ϔԍ�)
    AnsiString P_Mat_Code; // �ގ�����(HEARTS�ް�������p)
    AnsiString P_Ana_Plate; // �E���ް�1���,��ڰČ^��
    AnsiString P_Unit; // �P�ʁi���j
    // ��

    AnsiString P_MixFlg; // Я���׸�
    AnsiString P_HeadsVer; // HEADS Version
    AnsiString P_SoSyoAscii; // �^�����̔��p

    AnsiString P_Katasiki; // ���i�^��
    AnsiString P_Zaisitu; // �ގ�����
    AnsiString P_EZaisitu; // �ގ�����
    AnsiString P_Zuban; // �}��
    AnsiString P_SZuban; // ��}�p�}��
    // 2003.11.26�ϐ��ǉ�
    AnsiString P_Zuban2; // �}�ԂQ
    AnsiString P_SZuban2; // ��}�p�}�ԂQ
    // *******************
    long P_Suryo; // �P�䕪����
    long P_Yobisu; // �\����
    long P_YobisuPltAll; // �\����
    long P_PltGsktYobiA; // �\���i�̃v���[�g���K�X�P�b�g�t���̏ꍇ��A�v���[�g����
    long P_PltGsktYobiB; // �\���i�̃v���[�g���K�X�P�b�g�t���̏ꍇ��B�v���[�g����
    long P_PltGsktYobiD; // �\���i�̃v���[�g���K�X�P�b�g�t���̏ꍇ��D�v���[�g����
    long P_PltGsktYobiE; // �\���i�̃v���[�g���K�X�P�b�g�t���̏ꍇ��E�v���[�g����
    // 2004.01.19 �ϐ��ǉ�
    long P_PltGsktYobiS; // �\���i�̃v���[�g�ɕt���^�󎞖ڋʖӕ��⋭�K�X�P�b�g����
    // *******************
    // 2004.01.16 �ϐ��ǉ�
    long P_PltYobiA; // �\���i��A�v���[�g����
    long P_PltYobiB; // �\���i��B�v���[�g����
    long P_PltYobiD; // �\���i��D�v���[�g����
    long P_PltYobiE; // �\���i��E�v���[�g����
    // *******************
    long P_Soukei; // ���v
    int P_RecNo; // ں��޶���
    bool bMouAteFlg; // �ӓ����A�t���[���ގ��Ƃ���ꍇ�̃t���O

    // ��ڰĕ��i �ް��쐬�p
    AnsiString P_PLATECD[5]; // ��ڰč\�����i����ð����ް�
    int P_JokenFlg1; // �ǂݔ�΂������׸ނP(0:�Ǎ���,1:�ǂݔ�΂�)
    int P_JokenFlg2; // �ǂݔ�΂������׸ނP(0:�Ǎ���,1:�ǂݔ�΂�)
    AnsiString P_SSyurui; // ���̎��
    AnsiString P_SModel; // ��������
    AnsiString P_MAT_CODE1; // �ގ����ނP
    AnsiString P_SideDiv; // A���E�a��
    AnsiString P_YobiCD[50]; // �\���i���ރ���
    AnsiString P_YobiMAT[50]; // �\���i�ގ����ރ���
    int P_YobiMemoCnt; // �\���i�����̃J�E���^

    // ��2007.10.4 �~���[�d�l�Ή�
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // ��2007.10.4 �~���[�d�l�Ή�

    // 2017.08.17 YX-83�ǉ�_S
    int P_FL; // �΍s���׸�(0:��`��,1:�΍s��)
    int P_YX836; // YX-836�׸�
    // 2017.08.17 YX-83�ǉ�_E

    // ��2002/10/24
    bool __stdcall Write_Ateita_Data_Sub(AnsiString, AnsiString, AnsiString,
        AnsiString, long, AnsiString, AnsiString);
    bool __stdcall Write_M_Gskt_Data_Sub(AnsiString, bool, AnsiString);
    bool __stdcall Write_D_Gskt_Data_Sub(AnsiString, AnsiString, AnsiString,
        AnsiString, long, AnsiString, AnsiString, AnsiString, long, AnsiString);
    bool __stdcall Write_D_Gskt_Data_Distance();
    bool __stdcall Write_E_Gskt_Data_Sub(AnsiString, AnsiString, AnsiString,
        AnsiString, long, AnsiString, AnsiString, AnsiString, long, AnsiString);
    bool __stdcall Write_E_Gskt_Data_Distance(void);
    bool __stdcall Search_HD_PLATE_3(AnsiString, AnsiString, AnsiString,
        AnsiString);
    bool __stdcall Search_HD_BFRG(AnsiString, AnsiString, AnsiString);
    bool __stdcall Search_HD_D_GASKET(AnsiString, AnsiString, AnsiString);
    bool __stdcall Search_HD_E_GASKET(AnsiString, AnsiString, AnsiString);
    // ��2002/10/24

    bool __fastcall Hontai_Data_Sakusei(void); // ��ڰĖ{���ް��C�E���ް� �쐬
    bool __fastcall Search_HD_PLATE(AnsiString Key); // ��ڰĕ��iϽ� ����
    bool __fastcall Syukei_Anaake_Data(AnsiString Syurui, AnsiString Model);
    // ��ڰčE���ް��W�v
    bool __fastcall Write_Hontai_Data(void); // ��ڰĖ{���ް�(��ڰĖ{��) CSV̧�ٍ쐬
    bool __fastcall Write_Anaake_Data(AnsiString Syurui, AnsiString Model);
    // ��ڰĖ{���ް�(��ڰčE��) CSV̧�ٍ쐬
    bool __fastcall Syukei_Hontai_Yobisu(AnsiString Kata1, AnsiString Kata2,
        AnsiString Siyou); // ��ڰĖ{�̗\�����ް��W�v
    // ��2002/10/23
    bool __fastcall Write_Ateita_Data(void); // ���� CSV̧�ٍ쐬
    // ��2002/10/23
    bool __fastcall Plate_Data_Sakusei(void); // ��ڰĕ��i�@�ް��쐬
    bool __fastcall Search_HD_PLATE_2(AnsiString Key, AnsiString PType,
        AnsiString PCode);
    bool __fastcall Write_PlateBuhin_Data(bool ret); // ��ڰĕ��i�ް�CSV�쐬
    bool __fastcall Write_PlateBuhin_Data_GSKT(bool ret);
    // ��ڰĕ��i�ް�CSV�쐬(�޽��ėp)
    bool __fastcall Write_HeartsBuhin_Data(AnsiString s_Mat_Code);
    // HEARTS�p�ް�CSV�쐬
    // ��2002/10/23
    bool __fastcall Get_Gskt_MastKey(int DEmode); // �K�X�P�b�g�����L�[�擾
    bool __fastcall Write_M_Gskt_Data(void); // ���ԃv���[�g�K�X�P�b�g CSV̧�ٍ쐬
    // 2006.01.10 M��ڰĶ޽���(WX��p)���ٰ�ݒǉ�
    bool __fastcall Write_WXM_Gskt_Data(void); // ������ڰĶ޽���(WX��p) CSV̧�ٍ쐬
    // ******************************************
    bool __fastcall Write_D_Gskt_Data(void); // �c�v���[�g�K�X�P�b�g CSV̧�ٍ쐬
    bool __fastcall Write_E_Gskt_Data(void); // �d�v���[�g�K�X�P�b�g CSV̧�ٍ쐬
    // 2006.01.10 DE��ڰĶ޽���(RX-00��p) CSV̧�ٍ쐬���ٰ�ݒǉ�
    bool __fastcall Write_RX00D_Gskt_Data(void);
    // D��ڰĶ޽���(RX-00��p) CSV̧�ٍ쐬
    bool __fastcall Write_RX00E_Gskt_Data(void);
    // E��ڰĶ޽���(RX-00��p) CSV̧�ٍ쐬
    bool __fastcall Write_WX_CE_Gskt_Data(void);
    // E��ڰĶ޽���(WX���Đ�p) CSV̧�ٍ쐬
    // **********************************************************
    int __stdcall Search_GSKT_Yobi(AnsiString PartsCD, AnsiString MatCD);

    // ��2002/10/23
public:
    bool __fastcall Excute(HeadsData* cHeadsData); // ��ڰĕ��i�W�J���� ���s
    // bool __fastcall Syukei_Buhin_Yobisu(AnsiString s_Katasiki, AnsiString a_Zaisitu);                // ��ڰĕ��i�\�����ް��W�v
    int __fastcall Get_Gskt_YobiSu(int i_TmpCd, AnsiString MatCD);

};

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰĕ��i�W�J����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall PlateDetails(HeadsData* pcHeadsData) {
    bool ret;

    TPlateDetails plateDetails; // ��ڰĖ{���ް��C�E���ް� �쐬Class��������,�֐��I����j��

    ret = plateDetails.Excute(pcHeadsData); // ��ڰĖ{���ް��C�E���ް� �쐬���s

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰĕ��i�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADS÷���ް����
    m_pcHeadsData = pcHeadsData;

    // HEADS÷���ް���AnsiString�^�z��Ɏ擾
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********     �w��ڰĕ��i�W�J�����x�J�n      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     �w��ڰĕ��i�W�J�����x�J�n      *********");
    Write_Error_Log("*******************************************************");

    // ***********************************
    // ***  ��ڰĖ{���ް��C�E���ް��쐬
    // ***********************************
    Hontai_Data_Sakusei();

    // ��2002/10/24
    Write_Ateita_Data(); // ���� CSV̧�ٍ쐬
    // ��2002/10/24

    // ***********************************
    // ***  ��ڰĕ��i�@�ް��쐬
    // ***********************************
    Plate_Data_Sakusei();
    bMouAteFlg = false;

    // 2017.08.17 YX-83�ǉ�_S
    // ***********************************
    // ***  �΍s���t���O
    // ***********************************
    // 2022.04.21 CX-01D�ǉ�_S
    if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c") {
        P_FL = 1; // �΍s��
    }
    //if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
    else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c") {
    // 2022.04.21 CX-01D�ǉ�_E
        P_FL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") {
        P_FL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
        P_FL = 1; // �΍s��
    }
    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
        P_FL = 1; // �΍s��
    }
    else {
        P_FL = 0; // ��`��
    }
    // 2017.08.17 YX-83�ǉ�_E

    // ��2002/10/24
    // 2006.01.10 WX�̏ꍇWrite_WXM_Gskt_Data()��CSV̧�ق��쐬����
    // Write_M_Gskt_Data();   // ���ԃv���[�g�K�X�P�b�g CSV̧�ٍ쐬
    if (G_KS_Syurui == "�v�w") {
        Write_WXM_Gskt_Data(); // M��ڰĶ޽���(WX��p) CSV̧�ٍ쐬
    }
    // 2021.09.22 WH-30�ǉ�_S
    else if (G_KS_Syurui == "�v�g") {
        Write_WXM_Gskt_Data(); // M��ڰĶ޽���(WX��p) CSV̧�ٍ쐬
    }
    // 2021.09,22 WH-30�ǉ�_E
    else {
        Write_M_Gskt_Data(); // ���ԃv���[�g�K�X�P�b�g CSV̧�ٍ쐬
    }
    // ***********************************************************
    Get_Gskt_MastKey(0); // �K�X�P�b�g�����L�[�擾
    // 2006.01.10 RX-00�̏ꍇWrite_RX00D_Gskt_Data()��CSV̧�ق��쐬����
    // Write_D_Gskt_Data();   // �c�v���[�g�K�X�P�b�g CSV̧�ٍ쐬
    if (G_KS_Syurui == "�q�w" && G_KS_Model == "�O�O") {
        Write_RX00D_Gskt_Data(); // D��ڰĶ޽���(RX-00��p) CSV̧�ٍ쐬
    }
    // 2023.07.11 GX-50�̏ꍇWrite_RX00D_Gskt_Data()��CSV̧�ق��쐬����_S
    else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�T�O") {
        Write_RX00D_Gskt_Data(); // D��ڰĶ޽���(RX-00��p) CSV̧�ٍ쐬
    }
    // 2023.07.11 GX-50�̏ꍇWrite_RX00D_Gskt_Data()��CSV̧�ق��쐬����_E
    else {
        Write_D_Gskt_Data(); // �c�v���[�g�K�X�P�b�g CSV̧�ٍ쐬
    }
    // ******************************************************************
    Get_Gskt_MastKey(1); // �K�X�P�b�g�����L�[�擾
    // 2006.01.10 RX-00�̏ꍇWrite_RX00E_Gskt_Data()��CSV̧�ق��쐬����
    // WX�̶��Ă̏ꍇWrite_WX_CE_Gskt_Data()��CSV̧�ق��쐬����
    // Write_E_Gskt_Data();   // �d�v���[�g�K�X�P�b�g CSV̧�ٍ쐬
    int iSuryo;
    int iMod;
    if (G_KS_Syurui == "�q�w" && G_KS_Model == "�O�O") {
        Write_RX00E_Gskt_Data(); // E��ڰĶ޽���(RX-00��p) CSV̧�ٍ쐬
    }
    // 2023.07.11 GX-50�̏ꍇWrite_RX00E_Gskt_Data()��CSV̧�ق��쐬����_S
    else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�T�O") {
        Write_RX00E_Gskt_Data(); // E��ڰĶ޽���(RX-00��p) CSV̧�ٍ쐬
    }
    // 2023.07.11 GX-50�̏ꍇWrite_RX00E_Gskt_Data()��CSV̧�ق��쐬����_E
    else if (G_KS_Syurui == "�v�w") {
        iSuryo = int(P_HEADS_DATA[38].ToIntDef(0));
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // ���� �d�J�Z�b�g
            Write_WX_CE_Gskt_Data(); // E��ڰĶ޽���(WX���Đ�p) CSV̧�ٍ쐬
        }
        else {
            // � �d�v���[�g
            Write_E_Gskt_Data(); // �d�v���[�g�K�X�P�b�g CSV̧�ٍ쐬
        }
    }
    // 2021.09.22 WH-30�ǉ�_S
    else if (G_KS_Syurui == "�v�g") {
        iSuryo = int(P_HEADS_DATA[38].ToIntDef(0));
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // ���� �d�J�Z�b�g
            Write_WX_CE_Gskt_Data(); // E��ڰĶ޽���(WX���Đ�p) CSV̧�ٍ쐬
        }
        else {
            // � �d�v���[�g
            Write_E_Gskt_Data(); // �d�v���[�g�K�X�P�b�g CSV̧�ٍ쐬
        }
    }
    // 2021.09.22 WH-30�ǉ�_E
    else {
        Write_E_Gskt_Data(); // �d�v���[�g�K�X�P�b�g CSV̧�ٍ쐬
    }
    // ******************************************************************
    // ��2002/10/24

    // ۸ލ쐬
    Write_Log("********************************************************");
    Write_Log("**********     �w��ڰĕ��i�W�J�����x�I��      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     �w��ڰĕ��i�W�J�����x�I��      *********");
    Write_Error_Log("*******************************************************");

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰĖ{���ް��C�E���ް��@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Hontai_Data_Sakusei(void) {

    int i;
    int i_Mojisu;
    AnsiString s_Text;

    AnsiString s_Syurui; // HEADS�ް� ���(RecNo 541)
    AnsiString s_Model; // HEADS�ް� ����(RecNo 542)
    AnsiString s_Key;

    // 2004.04.23 �ϐ��������ʒu�ύX
    P_PltYobiA = 0; // �\���i��A�v���[�g����
    P_PltYobiB = 0; // �\���i��B�v���[�g����
    P_PltYobiD = 0; // �\���i��D�v���[�g����
    P_PltYobiE = 0; // �\���i��E�v���[�g����
    // *****************************

    P_YobiMemoCnt = 0;
    // ****************************************************************************
    // ***                                                                      ***
    // ***                        �v���[�g�^���̌���                            ***
    // ***                                                                      ***
    // ****************************************************************************
    s_Syurui = P_HEADS_DATA[34].TrimRight(); // HEADS�ް� ���
    P_HeadsVer = P_HEADS_DATA[1267].TrimRight(); // HEADS�ް� ���@�[�W����
    // HEADS�ް� ����
    // s_Model  = P_HEADS_DATA[35].TrimRight().SubString(1,2); // HEADS�ް� ����

    // RecNo.542���ق̎擾���@�̕ύX
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADS�ް� ����
    // 2009.06.19 UX-100R�ǉ�
    // EX�̏ꍇ
    // s_Model��[����"C","P","R","A","B"������ꍇ
    // ���̕���������s_Model���珜���Ă���
    //
    // EX�ȊO�̏ꍇ
    // s_Model��[����"C","P","A","B"������ꍇ
    // ���̕���������s_Model���珜���Ă���
    // ���ɏ����ɍ��킹�Ĉȉ��̕�������s_Model���珜��
    // �Ō�̕�����"D"�̏ꍇ
    // �F�Ōォ��Q�����ڂ̕�����"C","P","A","B"�̏ꍇ
    // �F���̕���������s_Model���珜���Ă���
    // �F���ɍŌォ��Q�����ڂ̕���(����)������
    // �Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
    // ���̑��̏ꍇ                       �F�Ō�̕���(����)
    ////2006.02.06 SX-90M,L�ǉ�
    ////s_Model��[����"C","P","R","A","B"������ꍇ
    ////���̕���������s_Model���珜���Ă���
    ////���ɏ����ɍ��킹�Ĉȉ��̕�������s_Model���珜��
    ////EX�̏ꍇ                           �F�����Ȃ�
    ////2008.07.24 CX-10D�ǉ�
    ////�Ō�̕�����"D"�̏ꍇ
    ////�@�@�F�Ōォ��Q�����ڂ̕�����"C","P","R","A","B"�̏ꍇ
    ////�@�@�F���̕���������s_Model���珜���Ă���
    ////�@�@�F���ɍŌォ��Q�����ڂ̕���(����)������
    ////*********************
    ////�Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
    ////���̑��̏ꍇ                       �F�Ō�̕���(����)
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // if (i_Mojisu == 3){
    // s_Model = P_HEADS_DATA[35].TrimRight().SubString(1,2);
    // } else if (i_Mojisu == 4){
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(4,1);
    // if (s_Text.ToIntDef(999) != 999){
    // s_Model = P_HEADS_DATA[35].TrimRight().SubString(1,3);
    // } else {
    // s_Model = P_HEADS_DATA[35].TrimRight().SubString(1,2);
    // }
    // } else if (i_Mojisu == 5){
    // s_Model = P_HEADS_DATA[35].TrimRight().SubString(1,3);
    // }
    if (s_Model.SubString(s_Model.Length(), 1) == "B") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    if (s_Model.SubString(s_Model.Length(), 1) == "A") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    // 2009.06.19 UX-100R�ǉ�
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
        // 2008.07.24 CX-10D�ǉ�
    }
    else if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "B") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "A") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }
        // 2009.06.19 UX-100R�ǉ�
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

    // ��2002/10/23
    G_S_Model = s_Syurui + "-" + s_Model;
    // ��2002/10/23

    G_K_Syurui = ""; // �^�����
    G_K_Model1 = ""; // �^������(Я���^��1)
    G_K_Model2 = ""; // �^������(Я���^��2)
    G_KS_Syurui = ""; // ���̎��
    G_KS_Model = ""; // ��������
    G_Model_Zen1 = ""; // �^�����ّS�p1
    G_Model_Zen2 = ""; // �^�����ّS�p2

    G_Log_Text = "��ڰČ^���ϊ�ð��ق��A��ށw" + s_Syurui + "�x���f���w" + s_Model +
        "�x�Ō����J�n�B";
    Write_Log(G_Log_Text);

    // 2004.06.07 PHENKAN.CSV�����ύX
    int endflg = 1;
    // ******************************

    for (i = 0; i < G_PHENKAN_RecCnt; i++) {
        if (G_PHENKAN[i][0] == s_Syurui && G_PHENKAN[i][1] == s_Model) {

            // 2004.06.07 PHENKAN.CSV�����ύX
            endflg = 0;
            // ******************************

            P_MixFlg = G_PHENKAN[i][4]; // Я���׸�

            G_K_Syurui = G_PHENKAN[i][0]; // �^�����
            G_K_Model1 = G_PHENKAN[i][5]; // �^������(Я���^��1)
            G_K_Model2 = G_PHENKAN[i][6]; // �^������(Я���^��2)
            G_KS_Syurui = G_PHENKAN[i][2]; // ���̎��
            G_KS_Model = G_PHENKAN[i][3]; // ��������
            G_Model_Zen1 = G_PHENKAN[i][7]; // �^�����ّS�p1
            G_Model_Zen2 = G_PHENKAN[i][8]; // �^�����ّS�p2

            // ��ڰĕ��i�ް��쐬�����p(Plate_Data_Sakusei())
            P_SSyurui = G_PHENKAN[i][2]; // ���̎��
            P_SModel = G_PHENKAN[i][3]; // ��������

            break;
        }
    }

    // 2004.06.07 PHENKAN.CSV�����ύX
    if (endflg == 1) {
        return true;
    }
    // ******************************

    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADS�ް� ����
    // 2009.06.19 UX-100R�ǉ�
    // EX�̏ꍇ
    // s_Model��[����"C","P","R","A","B"������ꍇ
    // P_SoSyoAscii��G_KS_Model�̌��ɂ��̕�����ǉ�����
    //
    // EX�ȊO�̏ꍇ
    // s_Model��[����"C","P","A","B"������ꍇ
    // P_SoSyoAscii��G_KS_Model�̌��ɂ��̕�����ǉ�����
    // �A��s_Model�̍Ō�̕�����"D"�̏ꍇ
    // P_SoSyoAscii��G_KS_Model��"D"�̑O�̈ʒu��"C","P","A","B"����ǉ�����
    ////2006.02.06 SX-90M,L�ǉ�
    ////P_HEADS_DATA[35]��[����"C","P","R","A","B"������ꍇ
    ////P_SoSyoAscii��ZenToHan(G_KS_Model)�̌��ɂ��̕�����ǉ�����
    ////2008.07.24 CX-10D�ǉ�
    ////�A��P_HEADS_DATA[35]�̍Ō�̕�����"D"�̏ꍇ
    ////ZenToHan(G_KS_Model)��"D"�̑O�̈ʒu��"C","P","R","A","B"����ǉ�����
    ////*********************
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    // if (s_Text.ToIntDef(999) != 999){
    // P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" + P_HEADS_DATA[1267].TrimRight();;
    // } else {
    // P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();;;
    // }
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R�ǉ�
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (s_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D�ǉ�
    // P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = ZenToHan(G_KS_Model).SubString(1,
            ZenToHan(G_KS_Model).Length() - 1) + s_Text + "D";
        P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) +
            s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    G_Log_Text = "�^�����            �w" + G_K_Syurui + "�x���擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�^������(Я���^��1) �w" + G_K_Model1 + "�x���擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�^������(Я���^��2) �w" + G_K_Model2 + "�x���擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "���̎��            �w" + G_KS_Syurui + "�x���擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��������            �w" + G_KS_Model + "�x���擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�^�����ّS�p1       �w" + G_Model_Zen1 + "�x���擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "�^�����ّS�p2       �w" + G_Model_Zen2 + "�x���擾�B";
    Write_Log(G_Log_Text);

    G_Log_Text = "��ڰČ^���ϊ�ð���  �����I���B";
    Write_Log(G_Log_Text);

    // 2017.08.17 YX-83�ǉ�_S
    // ***********************************
    // ***  �x�w�|�W�R�t���O
    // ***********************************
    if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
        if( P_HEADS_DATA[1013].TrimRight() != "08" ) {
            P_YX836 = 1; // YX-836(Я���^��1)
        }
        else {
            P_YX836 = 0; // YX-838
        }
    }
    else {
        P_YX836 = 0; // YX-83�ł͂Ȃ�
    }
    // 2017.08.17 YX-83�ǉ�_E

    // ****************************************************************************
    // ***                                                                      ***
    // ***                       �~�b�N�X�^���P �����@�쐬                      ***
    // ***                                                                      ***
    // ****************************************************************************
    G_Log_Text = "Я���׸�            �w" + P_MixFlg + "�x���擾�B";
    Write_Log(G_Log_Text);
    if (P_MixFlg != "0" && P_MixFlg != "1") {
        return true;
    }

    // **********************************
    // ***  ��ڰĕ��i�\��Ͻ��@����
    // **********************************
    // 02.10.16 �������ύX
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADS�ް� ����
    // 2009.06.19 UX-100R�ǉ�
    // EX�̏ꍇ
    // s_Model��[����"C","P","R","A","B"������ꍇ
    // s_Key��G_K_Model1�̌��ɂ��̕�����ǉ�����
    //
    // EX�ȊO�̏ꍇ
    // s_Model��[����"C","P","A","B"������ꍇ
    // s_Key��G_K_Model1�̌��ɂ��̕�����ǉ�����
    // �A��s_Model�̍Ō�̕�����"D"�̏ꍇ
    // s_Key��G_K_Model1��"D"�̑O�̈ʒu��"C","P","A","B"����ǉ�����
    ////2006.02.06 SX-90M,L�ǉ�
    ////P_HEADS_DATA[35]��[����"C","P","R","A","B"������ꍇ
    ////s_Key��G_K_Model1�̌��ɂ��̕�����ǉ�����
    ////2008.07.24 CX-10D�ǉ�
    ////�A��P_HEADS_DATA[35]�̍Ō�̕�����"D"�̏ꍇ
    ////s_Key��G_K_Model1��"D"�̑O�̈ʒu��"C","P","R","A","B"����ǉ�����
    ////*********************
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    // if (s_Text.ToIntDef(999) != 999){
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    ////   P_SoSyoAscii = s_Key;
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R�ǉ�
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (s_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D�ǉ�
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = G_K_Model1.SubString(1, G_K_Model1.Length() - 1) +
            s_Text + "D";
        s_Key = G_K_Syurui + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    Search_HD_PLATE(s_Key);

    // **********************************
    // ***  ��ڰčE���ް��̌����C�W�v
    // **********************************
    // 2017.08.17 YX-83�ǉ�_S
    //Syukei_Anaake_Data(G_K_Syurui, G_K_Model1);
    //YX-836�̏ꍇ����ڰĔ��������Ĕ�r
    //YX-836 ����
    if (P_YX836 == 1) {
        Syukei_Anaake_Data(G_K_Syurui, "83" + P_HEADS_DATA[1013].SubString(2, 1));
    } else {
        Syukei_Anaake_Data(G_K_Syurui, G_K_Model1);
    }
    // 2017.08.17 YX-83�ǉ�_E

    // **********************************
    // ***  �\���i�ް��̌����C�W�v
    // **********************************
    // P_Yobisu = 0;

    // *************************************************
    // ***  ���i�W�J�ް�(��ڰĖ{��ں��ޕ�) �ւ̏o��
    // *************************************************
    // 02.09.09 �ύX
    // P_Katasiki = G_KS_Syurui + "�|" + G_Model_Zen1 + HanToZen(P_HEADS_DATA[1258].TrimRight());
    /* 02.09.11 ���ĉ�
     P_Katasiki = HanToZen(P_HEADS_DATA[1256].TrimRight()) + "�|"
     + HanToZen(P_HEADS_DATA[1257].TrimRight())
     + HanToZen(P_HEADS_DATA[1258].TrimRight());
     */
    Write_Hontai_Data();

    // P_Yobisu = 0;

    // *************************************************
    // ***  ���i�W�J�ް�(��ڰčE��ں��ޕ�) �ւ̏o��
    // *************************************************
    // 2017.08.17 YX-83�ǉ�_S
    //Write_Anaake_Data(G_K_Syurui, G_K_Model1);
    //YX-836�̏ꍇ����ڰĔ��������Ĕ�r
    //YX-836 DE��ڰĔ�(t8�Œ�)
    if (P_YX836 == 1) {
        Write_Anaake_Data(G_K_Syurui, "83" + P_HEADS_DATA[1013].SubString(2, 1));
    } else {
        Write_Anaake_Data(G_K_Syurui, G_K_Model1);
    }
    // 2017.08.17 YX-83�ǉ�_E

    // 2017.08.17 YX-83�ǉ�_S
    // ***********************************
    // ***  �x�w�|�W�R�t���O
    // ***********************************
    if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
        if( P_HEADS_DATA[1013].TrimRight() != "08" ) {
            P_YX836 = 2; // YX-836(Я���^��2)
        }
        else {
            P_YX836 = 0; // YX-838
        }
    }
    else {
        P_YX836 = 0; // YX-83�ł͂Ȃ�
    }
    // 2017.08.17 YX-83�ǉ�_E

    // ****************************************************************************
    // ***                                                                      ***
    // ***                       �~�b�N�X�^���Q �����@�쐬                      ***
    // ***                                                                      ***
    // ****************************************************************************
    if (P_MixFlg != "1") {
        // 2017.08.17 YX-83�ǉ�_S
        //return true;
        if (P_YX836 == 2) {
        }
        else {
            return true;
        }
        // 2017.08.17 YX-83�ǉ�_E
    }

    // **********************************
    // ***  ��ڰĕ��i�\��Ͻ��@����
    // **********************************
    // 02.10.16 �������ύX
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADS�ް� ����
    // 2009.06.19 UX-100R�ǉ�
    // EX�̏ꍇ
    // s_Model��[����"C","P","R","A","B"������ꍇ
    // s_Key��G_K_Model2�̌��ɂ��̕�����ǉ�����
    //
    // EX�ȊO�̏ꍇ
    // s_Model��[����"C","P","A","B"������ꍇ
    // s_Key��G_K_Model2�̌��ɂ��̕�����ǉ�����
    // �A��s_Model�̍Ō�̕�����"D"�̏ꍇ
    // s_Key��G_K_Model2��"D"�̑O�̈ʒu��"C","P","A","B"����ǉ�����
    ////2006.02.06 SX-90M,L�ǉ�
    ////P_HEADS_DATA[35]��[����"C","P","R","A","B"������ꍇ
    ////s_Key��G_K_Model2�̌��ɂ��̕�����ǉ�����
    ////2008.07.24 CX-10D�ǉ�
    ////�A��P_HEADS_DATA[35]�̍Ō�̕�����"D"�̏ꍇ
    ////s_Key��G_K_Model2��"D"�̑O�̈ʒu��"C","P","R","A","B"����ǉ�����
    ////*********************
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    ////2006.01.10 MIX��ڰ�2��G_K_Model2(�^������(Я���^��2))�Ō�������
    ////if (s_Text.ToIntDef(999) != 999){
    ////   s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" + P_HEADS_DATA[1267].TrimRight();
    ////} else {
    ////   s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    ////}
    // if (s_Text.ToIntDef(999) != 999){
    // s_Key = G_K_Syurui + "-" + G_K_Model2 + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model2 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    ////*************************************************************
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R�ǉ�
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (s_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D�ǉ�
    // s_Key = G_K_Syurui + "-" + G_K_Model2 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = G_K_Model2.SubString(1, G_K_Model2.Length() - 1) +
            s_Text + "D";
        s_Key = G_K_Syurui + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Key = G_K_Syurui + "-" + G_K_Model2 + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    // 2017.08.17 YX-83�ǉ�_S
    if (P_YX836 == 2) {
        s_Key = "YX-83" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // 2017.08.17 YX-83�ǉ�_E

    Search_HD_PLATE(s_Key);

    // **********************************
    // ***  ��ڰčE���ް��̌����C�W�v
    // **********************************
    // 2017.08.17 YX-83�ǉ�_S
    //Syukei_Anaake_Data(G_K_Syurui, G_K_Model2);
    //YX-836�̏ꍇ����ڰĔ��������Ĕ�r
    //YX-836 DE��ڰĔ�(t8�Œ�)
    if (P_YX836 == 2) {
        Syukei_Anaake_Data(G_K_Syurui, "838");
    } else {
        Syukei_Anaake_Data(G_K_Syurui, G_K_Model2);
    }
    // 2017.08.17 YX-83�ǉ�_E

    // **********************************
    // ***  �\���i�ް��̌����C�W�v
    // **********************************
    // P_Yobisu = 0;

    // *************************************************
    // ***  ���i�W�J�ް�(��ڰĖ{��ں��ޕ�) �ւ̏o��
    // *************************************************
    // 02.09.09 �ύX
    // P_Katasiki = G_KS_Syurui + "�|" + G_Model_Zen2 + HanToZen(P_HEADS_DATA[1258].TrimRight());
    /* 02.09.11 ���ĉ�
     P_Katasiki = HanToZen(P_HEADS_DATA[1256].TrimRight()) + "�|"
     + HanToZen(P_HEADS_DATA[1257].TrimRight())
     + HanToZen(P_HEADS_DATA[1258].TrimRight());
     */
    Write_Hontai_Data();
    // *************************************************
    // ***  ���i�W�J�ް�(��ڰčE��ں��ޕ�) �ւ̏o��
    // *************************************************
    // 2017.08.17 YX-83�ǉ�_S
    //Write_Anaake_Data(G_K_Syurui, G_K_Model2);
    //YX-836�̏ꍇ����ڰĔ��������Ĕ�r
    //YX-836 DE��ڰĔ�(t8�Œ�)
    if (P_YX836 == 2) {
        Write_Anaake_Data(G_K_Syurui, "838");
    } else {
        Write_Anaake_Data(G_K_Syurui, G_K_Model2);
    }
    // 2017.08.17 YX-83�ǉ�_E

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰĕ��i�\��Ͻ��@����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Search_HD_PLATE(AnsiString Key) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL; // SQL��

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PLATE";
    s_SQL = s_SQL + "  WHERE MODEL_CODE = '" + Key + "'";
    s_SQL = s_SQL + "    AND PLATE_TYPE = 'D'";
    s_SQL = s_SQL + "    AND PARTS_CODE = 3";

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
        P_Mat_Code = "";
    }
    else {
        P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_Mat_Code = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰčE���ް��@�W�v
// �T  �v      �F
// ��  ��      �F Syurui :��ڰČ^���ϊ�ð��ٓ� ��ڰČ^�����
// �F Model  :��ڰČ^���ϊ�ð��ٓ� Я���^��
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F TYouryosyo ����
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Syukei_Anaake_Data(AnsiString Syurui,
    AnsiString Model) {
    // CSV�Ǎ��p
    char buf[1024]; /* ������o�b�t�@ */
    char *tok; /* �g�[�N���|�C���^ */
    int i; /* ���[�v�J�E���^ */
    char token[100][100]; /* �g�[�N���o�b�t�@ */

    AnsiString s_Ana_Text[5];
    AnsiString wk_FilePass;
    AnsiString s_Text;
    int i_Pos;
    int i_ZaisituFlg;
    // 2016.04.28 CXW-00�ǉ�
    AnsiString s_Syurui;
    AnsiString s_Model;
    // *********************

    P_Suryo = 0; // 1�䕪����
    P_RecNo = 0; // ں��޶���(���o������v����)
    i_ZaisituFlg = 0; // �ގ��׸�
    P_YobisuPltAll = 0; // �v���[�g�{�̗p�\���i��
    P_PltGsktYobiA = 0;
    P_PltGsktYobiB = 0;
    P_PltGsktYobiD = 0;
    P_PltGsktYobiE = 0;
    // 2004.01.19 �ϐ��ǉ�
    P_PltGsktYobiS = 0; // �\���i�̃v���[�g�ɕt���^�󎞖ڋʖӕ��⋭�K�X�P�b�g����
    // *******************

    // 2004.04.23 �ϐ��������ʒu�ύX
    // 2004.01.16 �ϐ��ǉ�
    // P_PltYobiA = 0;      // �\���i��A�v���[�g����
    // P_PltYobiB = 0;      // �\���i��B�v���[�g����
    // P_PltYobiD = 0;      // �\���i��D�v���[�g����
    // P_PltYobiE = 0;      // �\���i��E�v���[�g����
    // *******************

    // *******************
    // 2007.10.03 �װ�d�l�Ή� ��ڰčE���ް��߽�̎d�l���ߺ��ޒǉ�
    AnsiString sMirrorCode;
    // *******************

    // *****************************
    // 2007.10.03 �װ�d�l�Ή� ��ڰčE���ް��߽��ύX
    sMirrorCode = AnsiString(m_pcHeadsData->GetSpecCode());

    // ��ڰčE���ް��߽
    // wk_FilePass = G_AnaFilePass + G_AnaakeFileNM;
	// wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN + ".csv";
	//2023/4/21 �����ԍ������ǉ��Ή� Mod
	//wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN + sMirrorCode + ".csv";
	wk_FilePass = G_AnaFilePass + "HOL" + GetFileSeizouBangou(G_SAKUBAN) + sMirrorCode + ".csv";
    // *****************************

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "��ڰčE���ް� �w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // �Ǎ��݉\�̊�
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), fp) != NULL) {
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

            s_Ana_Text[0] = token[0]; // ��ڰčE���ް��� �^��
            s_Ana_Text[1] = token[1]; // ��ڰčE���ް��� �^��(?) �p��1����
            s_Ana_Text[2] = token[2]; // ��ڰčE���ް��� �^�� 'A' or 'B'
            s_Ana_Text[3] = token[3]; // ��ڰčE���ް��� �d�l(?)
            s_Ana_Text[4] = token[4]; // ��ڰčE���ް��� 1�䕪����

            // 2009.06.19 UX-100R�ǉ�
            // EX�̏ꍇ
            // s_Ana_Text[0]��[����"C","P","R","A","B"������ꍇ
            // ���̕���������s_Ana_Text[0]���珜���Ă���
            //
            // EX�ȊO�̏ꍇ
            // s_Ana_Text[0]��[����"C","P","A","B"������ꍇ
            // ���̕���������s_Ana_Text[0]���珜���Ă���
            // ���ɏ����ɍ��킹�Ĉȉ��̕�������s_Ana_Text[0]���珜��
            // �Ō�̕�����"D"�̏ꍇ
            // �F�Ōォ��Q�����ڂ̕�����"C","P","A","B"�̏ꍇ
            // �F���̕���������s_Ana_Text[0]���珜���Ă���
            // �F���ɍŌォ��Q�����ڂ̕���(����)������
            // �Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
            // ���̑��̏ꍇ                       �F�Ō�̕���(����)
            ////2006.02.06 SX-90M,L�ǉ�
            ////s_Ana_Text[0]��[����"C","P","R","A","B"������ꍇ
            ////���̕���������s_Ana_Text[0]���珜���Ă���
            ////���ɏ����ɍ��킹�Ĉȉ��̕�������s_Ana_Text[0]���珜��
            ////EX�̏ꍇ                           �F�����Ȃ�
            ////2008.07.24 CX-10D�ǉ�
            ////�Ō�̕�����"D"�̏ꍇ
            ////�@�@�F�Ōォ��Q�����ڂ̕�����"C","P","R","A","B"�̏ꍇ
            ////�@�@�F���̕���������s_Ana_Text[0]���珜���Ă���
            ////�@�@�F���ɍŌォ��Q�����ڂ̕���(����)������
            ////*********************
            ////�Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
            ////���̑��̏ꍇ                       �F�Ō�̕���(����)
            // **********************
            //
            ////��2002/10/23
            ////D,E��ڰĂ̎����ް����L������
            // if( s_Ana_Text[1] == "D" ){
            // if ( s_Ana_Text[0].SubString(s_Ana_Text[0].Length(),1).ToIntDef(999)==999 ){
            // if (s_Ana_Text[0].SubString(1,2) == "EX" ) {
            // G_D_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-1);
            // } else {
            // G_D_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-2);
            // }
            // }else{
            // if (s_Ana_Text[0].SubString(1,2) == "EX" ) {
            // G_D_GsktType    = s_Ana_Text[0].TrimRight();
            // } else {
            // G_D_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-1);
            // }
            // }
            // G_D_GsktABKubun = s_Ana_Text[2];
            // G_D_GsktAna     = s_Ana_Text[3];
            // }
            // if( s_Ana_Text[1] == "E" ){
            // if ( s_Ana_Text[0].SubString(s_Ana_Text[0].Length(),1).ToIntDef(999)==999 ){
            // if (s_Ana_Text[0].SubString(1,2) == "EX" ) {
            // G_E_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-1);
            // } else {
            // G_E_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-2);
            // }
            // }else{
            // if (s_Ana_Text[0].SubString(1,2) == "EX" ) {
            // G_E_GsktType    = s_Ana_Text[0].TrimRight();
            // } else {
            // G_E_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-1);
            // }
            // }
            // G_E_GsktABKubun = s_Ana_Text[2];
            // G_E_GsktAna     = s_Ana_Text[3];
            // }
            ////��2002/10/23
            //
            // D,E��ڰĂ̎����ް����L������
            if (s_Ana_Text[1] == "D") {
                s_Text = s_Ana_Text[0].TrimRight();
                if (s_Text.SubString(s_Text.Length(), 1) == "B") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(s_Text.Length(), 1) == "A") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                // 2009.06.19 UX-100R�ǉ�
                // if ( s_Text.SubString(s_Text.Length(),1) == "R" ) { s_Text = s_Text.SubString(1,s_Text.Length()-1); }
                if (s_Text.SubString(1, 2) == "EX" && s_Text.SubString
                    (s_Text.Length(), 1) == "R") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                // **********************
                if (s_Text.SubString(s_Text.Length(), 1) == "P") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(s_Text.Length(), 1) == "C") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(1, 2) == "EX") {
                    G_D_GsktType = s_Text;
                    // 2008.07.24 CX-10D�ǉ�
                }
                else if (s_Text.SubString(s_Text.Length(), 1) == "D") {
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "B") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "A") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    // 2009.06.19 UX-100R�ǉ�
                    // if ( s_Text.SubString(s_Text.Length()-1,1) == "R" ) {
                    // s_Text = s_Text.SubString(1,s_Text.Length()-2) + "D";
                    // }
                    // **********************
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "P") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "C") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }

                    G_D_GsktType =
                        s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    // *********************
                }
                else if (s_Text.SubString(s_Text.Length(),
                    1).ToIntDef(999) == 999) {
                    G_D_GsktType = s_Text.SubString(1, s_Text.Length() - 2) +
                        s_Text.SubString(s_Text.Length(), 1);
                }
                else {
                    G_D_GsktType = s_Text.SubString(1, s_Text.Length() - 1);
                }
                G_D_GsktABKubun = s_Ana_Text[2];
                G_D_GsktAna = s_Ana_Text[3];
            }
            if (s_Ana_Text[1] == "E") {
                s_Text = s_Ana_Text[0].TrimRight();
                if (s_Text.SubString(s_Text.Length(), 1) == "B") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(s_Text.Length(), 1) == "A") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                // 2009.06.19 UX-100R�ǉ�
                // if ( s_Text.SubString(s_Text.Length(),1) == "R" ) { s_Text = s_Text.SubString(1,s_Text.Length()-1); }
                if (s_Text.SubString(1, 2) == "EX" && s_Text.SubString
                    (s_Text.Length(), 1) == "R") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                // **********************
                if (s_Text.SubString(s_Text.Length(), 1) == "P") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(s_Text.Length(), 1) == "C") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(1, 2) == "EX") {
                    G_E_GsktType = s_Text;
                    // 2008.07.24 CX-10D�ǉ�
                }
                else if (s_Text.SubString(s_Text.Length(), 1) == "D") {
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "B") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "A") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    // 2009.06.19 UX-100R�ǉ�
                    // if ( s_Text.SubString(s_Text.Length()-1,1) == "R" ) {
                    // s_Text = s_Text.SubString(1,s_Text.Length()-2) + "D";
                    // }
                    // **********************
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "P") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "C") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }

                    G_E_GsktType =
                        s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    // *********************
                }
                else if (s_Text.SubString(s_Text.Length(),
                    1).ToIntDef(999) == 999) {
                    G_E_GsktType = s_Text.SubString(1, s_Text.Length() - 2) +
                        s_Text.SubString(s_Text.Length(), 1);
                }
                else {
                    G_E_GsktType = s_Text.SubString(1, s_Text.Length() - 1);
                }
                G_E_GsktABKubun = s_Ana_Text[2];
                G_E_GsktAna = s_Ana_Text[3];
            }
            // ***********************

            // 1�䕪���ʂ̏W�v
            // 2009.06.19 UX-100R�ǉ�
            // EX�̏ꍇ
            // s_Ana_Text[0]��[����"C","P","R","A","B"������ꍇ
            // ���̕���������s_Ana_Text[0]���珜���Ă���
            //
            // EX�ȊO�̏ꍇ
            // s_Ana_Text[0]��[����"C","P","A","B"������ꍇ
            // ���̕���������s_Ana_Text[0]���珜���Ă���
            // ���ɏ����ɍ��킹�Ĉȉ��̕�������s_Ana_Text[0]���珜��
            // �Ō�̕�����"D"�̏ꍇ
            // �F�Ōォ��Q�����ڂ̕�����"C","P","A","B"�̏ꍇ
            // �F���̕���������s_Ana_Text[0]���珜���Ă���
            // �F���ɍŌォ��Q�����ڂ̕���(����)������
            // �Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
            // ���̑��̏ꍇ                       �F�Ō�̕���(����)
            ////2006.02.06 SX-90M,L�ǉ�
            ////s_Ana_Text[0]��[����"C","P","R","A","B"������ꍇ
            ////���̕���������s_Ana_Text[0]���珜���Ă���
            ////���ɏ����ɍ��킹�Ĉȉ��̕�������s_Ana_Text[0]���珜��
            ////EX�̏ꍇ                           �F�����Ȃ�
            ////2008.07.24 CX-10D�ǉ�
            ////�Ō�̕�����"D"�̏ꍇ
            ////�@�@�F�Ōォ��Q�����ڂ̕�����"C","P","R","A","B"�̏ꍇ
            ////�@�@�F���̕���������s_Ana_Text[0]���珜���Ă���
            ////�@�@�F���ɍŌォ��Q�����ڂ̕���(����)������
            ////*********************
            ////�Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
            ////���̑��̏ꍇ                       �F�Ō�̕���(����)
            // **********************
            //
            // if (s_Ana_Text[0].SubString(1,2) == Syurui && s_Ana_Text[0].SubString(4,Model.Length()) == Model){
            s_Text = s_Ana_Text[0].TrimRight();
            if (s_Text.SubString(s_Text.Length(), 1) == "B") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(s_Text.Length(), 1) == "A") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            // 2009.06.19 UX-100R�ǉ�
            // if ( s_Text.SubString(s_Text.Length(),1) == "R" ) { s_Text = s_Text.SubString(1,s_Text.Length()-1); }
            if (s_Text.SubString(1, 2) == "EX" && s_Text.SubString
                (s_Text.Length(), 1) == "R") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            // **********************
            if (s_Text.SubString(s_Text.Length(), 1) == "P") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(s_Text.Length(), 1) == "C") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(1, 2) == "EX") {
                // 2008.07.24 CX-10D�ǉ�
            }
            else if (s_Text.SubString(s_Text.Length(), 1) == "D") {
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "B") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "A") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                // 2009.06.19 UX-100R�ǉ�
                // if ( s_Text.SubString(s_Text.Length()-1,1) == "R" ) {
                // s_Text = s_Text.SubString(1,s_Text.Length()-2) + "D";
                // }
                // **********************
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "P") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "C") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }

                s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                // *********************
            }
            else if (s_Text.SubString(s_Text.Length(), 1).ToIntDef(999) == 999)
            {
                s_Text = s_Text.SubString(1, s_Text.Length() - 2) +
                    s_Text.SubString(s_Text.Length(), 1);
            }
            else {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }

            // 2017.08.17 YX-83�ǉ�_S
            if (P_YX836 != 0) {
                // YX-836��s_Model����ڰĔ��ǉ�
                // s_Ana_Text[0]�����̂܂܎g�p
                s_Text = s_Ana_Text[0].TrimRight();
            }
            // 2017.08.17 YX-83�ǉ�_E

            // 2016.04.28 CXW-00�ǉ�
            if (s_Text.SubString(3, 1) == "W") {
                s_Syurui = s_Text.SubString(1, 3);
                s_Model  = s_Text.SubString(5, Model.Length());
            } else {
                s_Syurui = s_Text.SubString(1, 2);
                s_Model  = s_Text.SubString(4, Model.Length());
            }

            //if (s_Text.SubString(1, 2) == Syurui && s_Text.SubString(4,
            //    Model.Length()) == Model) {
            if (s_Syurui == Syurui && s_Model == Model) {
            // *********************
                // ***********************
                P_Suryo = P_Suryo + s_Ana_Text[4].ToIntDef(0);

                // ***********************************
                // ***  �\�����̏W�v (02.08.03�ǉ�)
                // ***********************************
                P_Yobisu = 0;
                // Syukei_Hontai_Yobisu(s_Ana_Text[0], s_Ana_Text[1], s_Ana_Text[2]+s_Ana_Text[3] );
                // 2004.01.16 �ϐ��ǉ�
                // if ( s_Ana_Text[1].TrimRight() == "D" ) {
                // Syukei_Hontai_Yobisu(s_Ana_Text[0], "D", s_Ana_Text[2]+s_Ana_Text[3] );
                // } else if ( s_Ana_Text[1].TrimRight() == "E" ) {
                // Syukei_Hontai_Yobisu(s_Ana_Text[0], "E", s_Ana_Text[2]+s_Ana_Text[3] );
                // } else {
                // Syukei_Hontai_Yobisu(s_Ana_Text[0], "", s_Ana_Text[2]+s_Ana_Text[3] );
                // }
                if (s_Ana_Text[1].TrimRight() == "D") {
                    Syukei_Hontai_Yobisu(s_Ana_Text[0], "D",
                        s_Ana_Text[2] + s_Ana_Text[3]);
                    P_PltYobiD += P_Yobisu; // D�v���[�g�̗\����������
                }
                else if (s_Ana_Text[1].TrimRight() == "E") {
                    Syukei_Hontai_Yobisu(s_Ana_Text[0], "E",
                        s_Ana_Text[2] + s_Ana_Text[3]);
                    P_PltYobiE += P_Yobisu; // E�v���[�g�̗\����������
                    if (s_Ana_Text[2] == "A") { // A�v���[�g�H
                        P_PltYobiA += P_Yobisu; // A�v���[�g�̗\����������
                    }
                    else {
                        P_PltYobiB += P_Yobisu; // B�v���[�g�̗\����������
                    }
                }
                else {
                    Syukei_Hontai_Yobisu(s_Ana_Text[0], "",
                        s_Ana_Text[2] + s_Ana_Text[3]);
                    if (s_Ana_Text[2] == "A") { // A�v���[�g�H
                        P_PltYobiA += P_Yobisu; // A�v���[�g�̗\����������
                    }
                    else {
                        P_PltYobiB += P_Yobisu; // B�v���[�g�̗\����������
                    }
                }
                // *******************
                P_YobisuPltAll += P_Yobisu;

                if (i_ZaisituFlg == 0) {
                    i_ZaisituFlg = 1;

                    // ��ڰčE���ް��� �^��
                    i_Pos = s_Ana_Text[0].Pos("-");
                    if (i_Pos == 0) {
                        s_Text = s_Ana_Text[0];
                    }
                    else {
                        s_Text = s_Ana_Text[0].SubString(1, i_Pos - 1) + "-" +
                            s_Ana_Text[0].SubString(i_Pos + 1,
                            s_Ana_Text[0].Length() - i_Pos);

                    }
                    P_Katasiki = HanToZen(s_Text);

                    // ��ڰčE���ް��� �ގ�
                    P_Zaisitu = token[5];
                    P_EZaisitu = token[5];
                }

                P_RecNo++;
            }
            memset(buf, 0x00, sizeof(buf));
        }
    }

    // �Y���\���i�ް������݂��Ȃ������ꍇ
    if (P_RecNo == 0)
        P_Yobisu = 0;

    fclose(fp); // ����
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �\���i�ް��@�W�v����(��ڰĖ{���ް�,�E���ް��쐬�p)
// �T  �v      �F
// ��  ��      �F AnsiString Kata1 :��ڰčE���ް��� �^�����
// �F AnsiString Kata2 :��ڰčE���ް��� �^��(?) �p��1����
// �F AnsiString Siyou :��ڰčE���ް��� �d�l(?)
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 02.08.03�ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Syukei_Hontai_Yobisu(AnsiString Kata1,
    AnsiString Kata2, AnsiString Siyou) {
    // CSV�Ǎ��p
    char buf[1024]; /* ������o�b�t�@ */
    char *tok; /* �g�[�N���|�C���^ */
    int i; /* ���[�v�J�E���^ */
    char token[100][100]; /* �g�[�N���o�b�t�@ */
    FILE *wkfp;
    bool withGskt;

    AnsiString s_Yobi_Text[7];
    AnsiString wk_FilePass;

    withGskt = false;
    P_RecNo = 0; // ں��޶���(���o������v����)

    // -----------------------------------------------
    // 2007.10.03 �ǉ�
    // �װB�d�l�̏ꍇ�́A�\���i�ް��Ȃ��Ƃ��ď���
    if (m_pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B) {
        P_Yobisu = 0;
        return withGskt;
    }
	// 2007.10.03
    // -----------------------------------------------

	// ��ڰčE���ް��߽
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
            s_Yobi_Text[2] = token[5];
            // �\���i�ް� 6��� ( 'UX-216','UX-20','UX-226'... )
            s_Yobi_Text[3] = token[8]; // �\���i�ް� �\����( 10, 1, 5, 2, 8, ... )
            s_Yobi_Text[4] = token[18];
            // �\���i�ް� 19���( 'A1234', 'B1234', 'A0000', ... )
            s_Yobi_Text[5] = token[14]; // �\���i�ް� 14���( B�K�X�P�b�g�ގ�CD )
            s_Yobi_Text[6] = token[16]; // �\���i�ް� 16���( A�K�X�P�b�g�ގ�CD )

            // 2010.09.21 �\���i�ް� 6��ڂ̕ύX(**-***B��**-***A)
            s_Yobi_Text[2] = s_Yobi_Text[2].Trim();
            if (s_Yobi_Text[2].SubString(s_Yobi_Text[2].Length(), 1) == "B") {
                s_Yobi_Text[2] =
                    s_Yobi_Text[2].SubString(1,
                s_Yobi_Text[2].Length() - 1) + "A";
            }
            // ***************************************************
            // 2020.10.28 �\���i�ް� 6��ڂ̕ύX(**C-***��**-***)_S
            if (s_Yobi_Text[2].SubString(3, 1) == "C") {
                s_Yobi_Text[2] =
                    s_Yobi_Text[2].SubString(1, 2) +
                s_Yobi_Text[2].SubString(4, s_Yobi_Text[2].Length());
            }
            // 2020.10.28 �\���i�ް� 6��ڂ̕ύX(**C-***��**-***)_E

            // 1�䕪���ʂ̏W�v
            if (s_Yobi_Text[0].Trim() == "PLTE" && s_Yobi_Text[1].Trim()
                == Kata2 && s_Yobi_Text[2].Trim() == Kata1 && s_Yobi_Text[4]
                .Trim() == Siyou) {
                P_Yobisu = P_Yobisu + s_Yobi_Text[3].ToIntDef(0);

                // 2004.01.15 �K�X�P�b�g�ގ�����ǉ�
                // if ( s_Yobi_Text[1] != "D" ) {      // D�v���[�g�ȊO
                // if ( s_Yobi_Text[5] != "" ) {    // �K�X�P�b�g�ގ��L��
                // withGskt = true;
                // if ( s_Yobi_Text[4].IsDelimiter("A",1) == true ) {    // A�v���[�g�H
                // P_PltGsktYobiA += s_Yobi_Text[3].ToIntDef(0);      // A�v���[�g�̗\����������
                // } else {
                // P_PltGsktYobiB += s_Yobi_Text[3].ToIntDef(0);      // B�v���[�g�̗\����������
                // }
                // }
                // if ( s_Yobi_Text[1] == "E" ) {      // E�v���[�g
                // P_PltGsktYobiE += s_Yobi_Text[3].ToIntDef(0);      // E�v���[�g�̗\����������
                // }
                // }
                // if ( s_Yobi_Text[1] == "D" ) {      // D�v���[�g
                // P_PltGsktYobiD += s_Yobi_Text[3].ToIntDef(0);      // D�v���[�g�̗\����������
                // }
                if (s_Yobi_Text[1] != "D") { // D�v���[�g�ȊO
                    if (s_Yobi_Text[5] != "") { // �K�X�P�b�g�ގ��L��
                        withGskt = true;
                        if (s_Yobi_Text[4].IsDelimiter("A", 1) == true)
                        { // A�v���[�g�H
                            P_PltGsktYobiA += s_Yobi_Text[3].ToIntDef(0);
                            // A�v���[�g(�޽��ĕt)�̗\����������
                        }
                        else {
                            P_PltGsktYobiB += s_Yobi_Text[3].ToIntDef(0);
                            // B�v���[�g(�޽��ĕt)�̗\����������
                            // 2004.01.19 �ϐ��ǉ�
                            if (s_Yobi_Text[4] == "B*23*") { // A�v���[�g�H
                                P_PltGsktYobiS +=
                                    s_Yobi_Text[3].ToIntDef(0) * 1;
                                // �^�󎞖ڋʖӕ��⋭�K�X�P�b�g�̗\����������
                            }
                            else if (s_Yobi_Text[4] == "B123*") {
                                P_PltGsktYobiS +=
                                    s_Yobi_Text[3].ToIntDef(0) / 2;
                                // �^�󎞖ڋʖӕ��⋭�K�X�P�b�g�̗\����������
                            }
                            // *******************
                        }
                    }
                    if (s_Yobi_Text[1] == "E") { // E�v���[�g
                        if (s_Yobi_Text[5] != "") { // �K�X�P�b�g�ގ��L��
                            P_PltGsktYobiE += s_Yobi_Text[3].ToIntDef(0);
                            // E�v���[�g(�޽��ĕt)�̗\����������
                        }
                    }
                }
                if (s_Yobi_Text[1] == "D") { // D�v���[�g
                    if (s_Yobi_Text[5] != "") { // �K�X�P�b�g�ގ��L��
                        withGskt = true;
                        P_PltGsktYobiD += s_Yobi_Text[3].ToIntDef(0);
                        // D�v���[�g(�޽��ĕt)�̗\����������
                    }
                }
                // *********************************

                P_RecNo++;
            }
        }
    }

    // �Y���\���i�ް������݂��Ȃ������ꍇ
    if (P_RecNo == 0)
        P_Yobisu = 0;

    fclose(wkfp); // ����
    return withGskt;

}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰĖ{���ް�  CSV̧�ٍ쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F TYouryosyo ����
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_Hontai_Data(void) {
    AnsiString s_Text;
    AnsiString wk_FilePass;

    long l_Daisu;

    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    AnsiString sSpecCode;
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());

    // ���i�W�J�ް��@OPEN
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

    // 1  ���i����
    s_Text = "PLATE";
    fputs(s_Text.c_str(), fp);
    // 2  ���ʎq
    s_Text = ",DPLT--";
    fputs(s_Text.c_str(), fp);
    // 3  ���i�^��
    s_Text = "," + P_Katasiki;
    fputs(s_Text.c_str(), fp);
    // 4  ���i����
    s_Text = ",�v���[�g";
    fputs(s_Text.c_str(), fp);
    // 5  �ގ�����
    s_Text = "," + P_Zaisitu;
    fputs(s_Text.c_str(), fp);
    // 6  �}��
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), fp);
    // 7  ��}�p�}��
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), fp);
    // 8  1�䕪����
    s_Text = "," + AnsiString(P_Suryo);
    fputs(s_Text.c_str(), fp);
    // 9  �\����
    s_Text = "," + AnsiString(P_YobisuPltAll);
    fputs(s_Text.c_str(), fp);
    // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
    l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // �䐔
    P_Soukei = P_Suryo * l_Daisu + P_YobisuPltAll;
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), fp);
    // 11 �d�l�P(�E���ް�����)
    if (P_PltGsktYobiA > 0 || P_PltGsktYobiB > 0) { // �K�X�P�b�g�t���\��������ꍇ�͂P
        s_Text = ",1";
    }
    else {
        s_Text = ",0";
    }
    // s_Text = "," + AnsiString(P_RecNo);
    fputs(s_Text.c_str(), fp);
    // 12 �d�l�Q
    s_Text = ",";
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
    // 16  �ގ�����
    s_Text = "," + P_EZaisitu;
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

    // -----  ���O�쐬�@------
    G_Log_Text = "��ڰĖ{���ް�  01 ���i����              �wPLATE�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  02 ���ʎq                �wDPLT--�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  03 ���i�^��              �w" + P_Katasiki + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  04 ���i����              �w�v���[�g�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  05 �ގ�����              �w" + P_Zaisitu + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  06 �}  ��                �w" + P_Zuban + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  07 ��}�p�}��            �w" + P_SZuban + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  08 �P�䕪����            �w" + AnsiString(P_Suryo) +
        "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  09 �\����                �w" +
        AnsiString(P_YobisuPltAll) + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  10 ���v                  �w" + AnsiString(P_Soukei)
        + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  11 �d�l�P(�E���ް�����)  �w" + AnsiString(P_RecNo) +
        "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰĖ{���ް�  17 �d�l�^�C�v�R�[�h      �w" + AnsiString(sSpecCode) +
        "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰčE���ް�  CSV̧�ٍ쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F ���i�W�J�ް�,HEARTS�p�ް��ɍE���ް���������
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_Anaake_Data(AnsiString Syurui,
    AnsiString Model) {

    FILE *wkfp;
    FILE *hsfp;

    // CSV�Ǎ��p
    char buf[1024]; /* ������o�b�t�@ */
    char *tok; /* �g�[�N���|�C���^ */
    int i; /* ���[�v�J�E���^ */
    char token[100][100]; /* �g�[�N���o�b�t�@ */

    // ��ڰčE���ް��p
    AnsiString s_Ana_Kata; // �^��
    AnsiString s_Ana_Kbn; // �敪
    AnsiString s_Ana_ABKbn; // AB�敪
    AnsiString s_Ana_Anaake; // �E��
    AnsiString s_Ana_Suryo; // 1�䕪����
    AnsiString s_Ana_Zaisitu; // ��ڰčގ�
    AnsiString s_Ana_Siyou; // �d�l

    AnsiString s_Text;

    int i_Daisu;

    AnsiString wk_FilePass;
    AnsiString hs_FilePass;

    // *******************
    // 2007.10.03 �װ�d�l�Ή� ��ڰčE���ް��߽�̎d�l���ߺ��ޒǉ�
    AnsiString sSpecCode;
    // *******************

    // 2016.04.28 CXW-00�ǉ�
    AnsiString s_Syurui;
    AnsiString s_Model;
    // *********************

    P_PltGsktYobiA = 0;
    P_PltGsktYobiB = 0;
    P_PltGsktYobiD = 0;
    P_PltGsktYobiE = 0;

    // --------------------------------
    // 2007.10.03 �װ�d�l�Ή�
    // ****************************
    // �d�l���ߺ��ގ擾
    // ****************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03
    // --------------------------------

    // ****************************
    // ***  ���i�W�J�ް��@OPEN
    // ****************************
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

	// ****************************
	// ***  HEARTS�p�ް��߽�擾
	// ****************************
	if (AllNumChk(G_SAKUBAN.c_str(), 0) == 0) {
        hs_FilePass = G_xls_Output_Dir + G_OUT_ORDER_Dir + G_SAKUBAN + "\\" +
			G_SAKUBAN + ".csv";
    }
	else {
		hs_FilePass = G_xls_Output_Dir + G_OUT_ESTIM_Dir + G_SAKUBAN + "\\" +
			G_SAKUBAN + ".csv";
	}

    // ****************************
    // ***  HEARTS�p�ް��@OPEN
    // ****************************
    if ((hsfp = fopen(hs_FilePass.c_str(), "a+")) == NULL) {
        fclose(hsfp);
        return false;
    }

    // *****************************
    // 2007.10.03 �װ�d�l�Ή� ��ڰčE���ް��߽��ύX
    // ��ڰčE���ް��߽

    // wk_FilePass = G_AnaFilePass + G_AnaakeFileNM;
    // wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN  + ".csv";
    //2023/4/21 �����ԍ������ǉ��Ή� Mod
    //wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN + sSpecCode + ".csv";
    wk_FilePass = G_AnaFilePass + "HOL" + GetFileSeizouBangou(G_SAKUBAN) + sSpecCode + ".csv";
    // *****************************

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "��ڰčE���ް� �w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
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

            P_Ana_Plate = token[0]; // ��ڰčE���ް��� �^��
            s_Ana_Kbn = token[1]; // ��ڰčE���ް��� �敪
            s_Ana_ABKbn = token[2]; // ��ڰčE���ް��� AB�敪
            s_Ana_Anaake = token[3]; // ��ڰčE���ް��� �E��
            s_Ana_Suryo = token[4]; // ��ڰčE���ް��� 1�䕪����
            s_Ana_Zaisitu = token[5]; // ��ڰčE���ް��� ��ڰČ^��

            // ***************************************************
            // *** �W�����i�\ �o�͗p�Ɏd�l�̎擾��ύX(02.08.08)
            // ***************************************************
            s_Ana_Siyou = HanToZen(token[0]);
            s_Ana_Siyou = TxtEdit(s_Ana_Anaake, 20);
            s_Ana_Siyou += HanToZen(token[1]);
            s_Ana_Siyou += HanToZen(token[2]);
            s_Ana_Siyou += HanToZen(token[3]);

            P_Yobisu = 0;
            if (s_Ana_Kbn.TrimRight() == "D") {
                Syukei_Hontai_Yobisu(P_Ana_Plate, "D",
                    s_Ana_ABKbn + s_Ana_Anaake);
            }
            else if (s_Ana_Kbn.TrimRight() == "E") {
                Syukei_Hontai_Yobisu(P_Ana_Plate, "E",
                    s_Ana_ABKbn + s_Ana_Anaake);
            }
            else {
                Syukei_Hontai_Yobisu(P_Ana_Plate, "",
                s_Ana_ABKbn + s_Ana_Anaake);
            }

            // �L��ں�������
            // 2009.06.19 UX-100R�ǉ�
            // EX�̏ꍇ
            // P_Ana_Plate��[����"C","P","R","A","B"������ꍇ
            // ���̕���������P_Ana_Plate���珜���Ă���
            //
            // EX�ȊO�̏ꍇ
            // P_Ana_Plate��[����"C","P","A","B"������ꍇ
            // ���̕���������P_Ana_Plate���珜���Ă���
            // ���ɏ����ɍ��킹�Ĉȉ��̕�������P_Ana_Plate���珜��
            // �Ō�̕�����"D"�̏ꍇ
            // �F�Ōォ��Q�����ڂ̕�����"C","P","A","B"�̏ꍇ
            // �F���̕���������P_Ana_Plate���珜���Ă���
            // �F���ɍŌォ��Q�����ڂ̕���(����)������
            // �Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
            // ���̑��̏ꍇ                       �F�Ō�̕���(����)
            ////2006.02.06 SX-90M,L�ǉ�
            ////P_Ana_Plate��[����"C","P","R","A","B"������ꍇ
            ////���̕���������P_Ana_Plate���珜���Ă���
            ////���ɏ����ɍ��킹�Ĉȉ��̕�������P_Ana_Plate���珜��
            ////EX�̏ꍇ                           �F�����Ȃ�
            ////2008.07.24 CX-10D�ǉ�
            ////�Ō�̕�����"D"�̏ꍇ
            ////�@�@�F�Ōォ��Q�����ڂ̕�����"C","P","R","A","B"�̏ꍇ
            ////�@�@�F���̕���������P_Ana_Plate���珜���Ă���
            ////�@�@�F���ɍŌォ��Q�����ڂ̕���(����)������
            ////*********************
            ////�Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
            ////���̑��̏ꍇ                       �F�Ō�̕���(����)
            // **********************
            //
            // if (P_Ana_Plate.SubString(1,2) == Syurui && P_Ana_Plate.SubString(4,Model.Length()) == Model){
            s_Text = P_Ana_Plate.TrimRight();
            if (s_Text.SubString(s_Text.Length(), 1) == "B") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(s_Text.Length(), 1) == "A") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            // 2009.06.19 UX-100R�ǉ�
            // if ( s_Text.SubString(s_Text.Length(),1) == "R" ) { s_Text = s_Text.SubString(1,s_Text.Length()-1); }
            if (s_Text.SubString(1, 2) == "EX" && s_Text.SubString
                (s_Text.Length(), 1) == "R") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            // **********************
            if (s_Text.SubString(s_Text.Length(), 1) == "P") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(s_Text.Length(), 1) == "C") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(1, 2) == "EX") {
                // 2008.07.24 CX-10D�ǉ�
            }
            else if (s_Text.SubString(s_Text.Length(), 1) == "D") {
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "B") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "A") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                // 2009.06.19 UX-100R�ǉ�
                // if ( s_Text.SubString(s_Text.Length()-1,1) == "R" ) {
                // s_Text = s_Text.SubString(1,s_Text.Length()-2) + "D";
                // }
                // **********************
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "P") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "C") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }

                s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                // *********************
            }
            else if (s_Text.SubString(s_Text.Length(), 1).ToIntDef(999) == 999)
            {
                s_Text = s_Text.SubString(1, s_Text.Length() - 2) +
                    s_Text.SubString(s_Text.Length(), 1);
            }
            else {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }

            // 2017.08.17 YX-83�ǉ�_S
            if (P_YX836 != 0) {
                // YX-836��s_Model����ڰĔ��ǉ�
                // P_Ana_Plate�����̂܂܎g�p
                s_Text = P_Ana_Plate.TrimRight();
            }
            // 2017.08.17 YX-83�ǉ�_E

            // 2016.04.28 CXW-00�ǉ�
            if (s_Text.SubString(3, 1) == "W") {
                s_Syurui = s_Text.SubString(1, 3);
                s_Model  = s_Text.SubString(5, Model.Length());
            } else {
                s_Syurui = s_Text.SubString(1, 2);
                s_Model  = s_Text.SubString(4, Model.Length());
            }
            //if (s_Text.SubString(1, 2) == Syurui && s_Text.SubString(4,
            //    Model.Length()) == Model) {
            if (s_Syurui == Syurui && s_Model == Model) {
            // *********************
                // ***********************

                // **************************
                // ** ���i�W�J�ް�̧�ُo��
                // **************************
                // 1  ���i����
                s_Text = "PLATE";
                fputs(s_Text.c_str(), fp);

                // 2  ���ʎq
                s_Text = ",PLHL--";
                fputs(s_Text.c_str(), fp);

                // 3  ���i�^��
                s_Text = ",";
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
                s_Text = "," + s_Ana_Suryo.TrimLeft();
                fputs(s_Text.c_str(), fp);

                // 9  �\����
                s_Text = "," + AnsiString(P_Yobisu);
                fputs(s_Text.c_str(), fp);

                // 10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
                i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // �䐔
                P_Soukei = s_Ana_Suryo.ToIntDef(0) * i_Daisu + P_Yobisu;
                s_Text = "," + AnsiString(P_Soukei);
                fputs(s_Text.c_str(), fp);

                // 11 �d�l�P(��ڰčE���ް��� �^��)
                s_Text = "," + P_Ana_Plate;
                fputs(s_Text.c_str(), fp);
                // 12 �d�l�Q(��ڰčE���ް��� �敪 + AB�敪 + �E��)
                s_Text = "," + s_Ana_Kbn + s_Ana_ABKbn + s_Ana_Anaake;
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
                // 16  �ގ�����
                s_Text = "," + P_EZaisitu;
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

                // **************************
                // ** HEARTS�ް�̧�ُo��
                // **************************
                // 1 ���i����
                s_Text = "\"PLTE\"";
                fputs(s_Text.c_str(), hsfp);

                // 2 ���ʎq
                s_Text = ",\"PLTE00\"";
                fputs(s_Text.c_str(), hsfp);

                // 3 ��ڰČ^��
                s_Text = ",\"" + P_Ana_Plate + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 4 �ގ�����
                s_Text = ",\"" + P_Mat_Code + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 5 ��ڰĎ��
                s_Text = ",\"" + s_Ana_Kbn + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 6 �E��
                s_Text = ",\"" + s_Ana_ABKbn + s_Ana_Anaake + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 7 1�䕪����
                s_Text = ",\"" + s_Ana_Suryo + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 8 �V���敪
                s_Text = ",\"0\"";
                fputs(s_Text.c_str(), hsfp);

                // 9 �����ݶް�敪
                s_Text = ",\"0\"";
                fputs(s_Text.c_str(), hsfp);

                // ���s����
                s_Text = "\n";
                // CSV�ް��@̧�ُo��
                fputs(s_Text.c_str(), hsfp);
            }
        }
    }

    fclose(hsfp); // HEARTS�p�ް� ����
    fclose(fp); // ���i�W�J�ް� ����

    fclose(wkfp); // ��ڰčE���ް� ����

    // delete wkfp;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰĕ��i�@�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Plate_Data_Sakusei(void) {
    // CSV�Ǎ��p
    char buf[1024]; /* ������o�b�t�@ */
    char *tok; /* �g�[�N���|�C���^ */
    int i; /* ���[�v�J�E���^ */
    char token[100][100]; /* �g�[�N���o�b�t�@ */

    AnsiString wk_FilePass;
    AnsiString s_Key;
    AnsiString s_Model;
    int i_Mojisu;
    AnsiString s_Text;

    bool ret;
    int i_SyoriFlg;
    int i_PateCDCode; //
    AnsiString sPltMat;

    P_JokenFlg1 = 0;
    P_JokenFlg2 = 0;

    // 2012.07.31 LX-10���ׂĕ⋭�p�b�`�ǉ�
    ////2010.07.14 LX-10,RX-70���pTi0.5�̏ꍇ�⋭�p�b�`�ǉ�
    // int   i_Pos;
    // AnsiString s_Pltt;
    ////***************************************************
    // ************************************

    // *************************************
    // ***  ��ڰč\�����i����ð��ف@OPEN
    // *************************************
    wk_FilePass = G_Input_Csv_Dir + "PLATECD.CSV";
    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        G_ErrLog_Text = "��ڰč\�����i����ð��� �w" + wk_FilePass +
            "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // �Ǎ��݉\�̊�
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), fp) != NULL) {
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

            P_PLATECD[0] = token[0]; // ��ڰč\�����i����ð����ް��@����
            P_PLATECD[1] = token[1]; // ��ڰč\�����i����ð����ް�  ���i���޺���
            P_PLATECD[2] = token[2]; // ��ڰč\�����i����ð����ް�  ���i�敪
            P_PLATECD[3] = token[3]; // ��ڰč\�����i����ð����ް�  ���i����
            P_PLATECD[4] = token[4]; // ��ڰč\�����i����ð����ް��@�\���i
            P_PLATECD[5] = token[5]; // ��ڰč\�����i����ð����ް�

            i_PateCDCode = P_PLATECD[0].ToIntDef(9999);

            if (i_PateCDCode == 3 || i_PateCDCode == 22 || i_PateCDCode == 41) {
                i_SyoriFlg = 1;

            }
            else if (i_PateCDCode == 1 || i_PateCDCode == 20 ||
                i_PateCDCode == 39) {
                // i_SyoriFlg = P_JokenFlg1;  // ��L�����̺��ނŊ��ɏ����ςȂ�ǂݔ�΂�

            }
            else if (i_PateCDCode == 30 || i_PateCDCode == 57) {
                // i_SyoriFlg = P_JokenFlg2;  // ��L�����̺��ނŊ��ɏ����ςȂ�ǂݔ�΂�
            }
            else {
                i_SyoriFlg = 0;
            }

            /* ���l�̔�r�ɕύX
             if (P_PLATECD[0] == "03" || P_PLATECD[0] == "22" || P_PLATECD[0] == "41"){
             i_SyoriFlg = 1;
             } else if (P_PLATECD[0] == "01" || P_PLATECD[0] == "20" || P_PLATECD[0] == "39"){
             i_SyoriFlg = P_JokenFlg1;
             } else if (P_PLATECD[0] == "30" || P_PLATECD[0] == "57"){
             i_SyoriFlg = P_JokenFlg2;
             } else {
             i_SyoriFlg = 0;
             }
             */

            if (i_SyoriFlg == 0) {
                // *******************************
                // ***  ��ڰĕ��i�\��Ͻ� ����
                // *******************************
                // ��ڰĕ��i�\��Ͻ��������ύX 02.09.12
                s_Model = P_HEADS_DATA[35].TrimRight(); // HEADS�ް� ����
                i_Mojisu = s_Model.Length();
                s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu, 1);
                if (s_Text.ToIntDef(999) != 999) {
                    s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" +
                        P_HEADS_DATA[1267].TrimRight();
                }
                else {
                    s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" +
                        P_HEADS_DATA[1267].TrimRight();
                }
                s_Key = P_SoSyoAscii;

                ret = Search_HD_PLATE_2(s_Key, P_PLATECD[2], P_PLATECD[0]);

                if (i_PateCDCode == 20 || i_PateCDCode == 21) { // �n���K�[
                    P_Suryo = P_Suryo * (P_HEADS_DATA[38].ToIntDef(0));
                    // 2003.11.26 LX-40�n���K�[
                    if (G_KS_Syurui == "�k�w" && G_KS_Model == "�S�O" &&
                        P_HEADS_DATA[38].ToIntDef(0) >= 201) {
                        P_Zuban = P_Zuban2;
                        P_SZuban = P_SZuban2;
                        i_PateCDCode = 21;
                    }
                    // ************************
                    if (P_MAT_CODE1 == "") { // �ގ����ނ�������΁A�v���[�g�ގ����画�f
                        // 2016.11.07 �ގ����ޕύX
                        //// 2003.12.05 ASME PLATE
                        //// if( ( P_HEADS_DATA[43].TrimRight() == "1014" ) || ( P_HEADS_DATA[43].TrimRight() == "1015" )
                        //// ||( P_HEADS_DATA[43].TrimRight() == "1032" ) || ( P_HEADS_DATA[43].TrimRight() == "1033" ) ) {
                        //if ((P_HEADS_DATA[43].TrimRight() == "1014") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1015") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1032") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1033") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1057") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1058")
                        //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
                        //    || (P_HEADS_DATA[43].TrimRight() == "1075") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1089") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1090")
                        //    // *************************************************
                        //    // 2010.12.02 SB-265-Gr.11�ǉ�
                        //    || (P_HEADS_DATA[43].TrimRight() == "1076")
                        //    // ***************************
                        //    || (P_HEADS_DATA[43].TrimRight() == "1085") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1095")) {
                        //    // *********************
                        //    P_MAT_CODE1 = "1014"; // TP270
                        //}
                        //else {
                        //    P_MAT_CODE1 = "1001"; // SUS304
                        //}
                        if ( (Type_MAT(P_HEADS_DATA[43].TrimRight()) == "Ti") ) {  // �v���[�g�ގ����`�^���n
                            P_MAT_CODE1 = "3114"; // TP270
                        } else {
                            P_MAT_CODE1 = "3101"; // SUS304
                        }
                        // ***********************
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                }
                if (i_PateCDCode == 11 || i_PateCDCode == 12 ||
                    i_PateCDCode == 48 || i_PateCDCode == 49 ||
                    i_PateCDCode == 50) { // ���u�̏ꍇ�ގ��̓��^��
                    sPltMat = P_HEADS_DATA[43].TrimRight();
                    // 2016.11.07 �ގ����ޕύX
                    //// 2003.12.05 ASME PLATE
                    //// if (sPltMat == "1014" || sPltMat == "1015" || sPltMat == "1032" || sPltMat == "1033" ) {  // �v���[�g�ގ����`�^���n�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]�j
                    //if (sPltMat == "1014" || sPltMat == "1015" ||
                    //    sPltMat == "1032" || sPltMat == "1033" ||
                    //    sPltMat == "1057" || sPltMat == "1058" ||
                    //    sPltMat == "1085" || sPltMat == "1095") {
                    //    // �v���[�g�ގ����`�^���n�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
                    //    // *********************
                    //    // P_MAT_CODE1 = "1014";  // TP270
                    //    P_MAT_CODE1 = "1032"; // TP340
                    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
                    //}
                    //else if (sPltMat == "1075" || sPltMat == "1089" ||
                    //    sPltMat == "1090") {
                    //    // �v���[�g�ގ����`�^���n�iSB-265-Gr.7[1075] SB-265-Gr.2[1089] B-265-Gr.2[1090]
                    //    P_MAT_CODE1 = "1032"; // TP340
                    //    // *************************************************
                    //    // 2010.12.02 SB-265-Gr.11�ǉ�
                    //}
                    //else if (sPltMat == "1076") {
                    //    // �v���[�g�ގ����`�^���n�iSB-265-Gr.11[1076]�j
                    //    P_MAT_CODE1 = "1032"; // TP340
                    //    // ***************************
                    //}
                    //else {
                    //    P_MAT_CODE1 = "1003"; // SUS316
                    //}
                    if ( (Type_MAT(sPltMat) == "Ti") ) {  // �v���[�g�ގ����`�^���n
                        P_MAT_CODE1 = "3132"; // TP340
                    } else {
                        P_MAT_CODE1 = "3103"; // SUS316
                    }
                    // ***********************
                    P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                    P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                }

                // 2003.12.18 YX-80�v���[�g���i�ǉ�
                // �c�d�K�X�P�b�g
                if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
                    if (i_PateCDCode == 14 || i_PateCDCode == 15 ||
                        i_PateCDCode == 16 || i_PateCDCode == 17 ||
                        i_PateCDCode == 69 || i_PateCDCode == 70 ||
                        i_PateCDCode == 80 || i_PateCDCode == 90) {
                        // DE�v���[�g�̃K�X�P�b�g�͐����Ȃ�
                        P_Suryo = 0;
                        // ���ԃK�X�P�b�g
                    }
                    else if (i_PateCDCode == 32) { // �K�X�P�b�g�a�|�P�i�m�^�Q�|�P���j
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B��
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 33) { // �K�X�P�b�g�a�|�S�i�m�{�Q���j
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = P_Suryo + 2;
                        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B��
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 34) { // �K�X�P�b�g�`�|�Q�i�m���j
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = P_Suryo;
                        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A��
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 35) { // �K�X�P�b�g�`�|�R�i�m�^�Q���j
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = (double)P_Suryo * 0.5;
                        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A��
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                        // �^�󎞖ڋʖӕ��⋭
                    }
                    else if (i_PateCDCode == 38) { // E1�m�Y���Ő��ʕω�
                        if (P_HEADS_DATA[951].TrimRight() == "0") {
                            P_Suryo = 2;
                        }
                        else {
                            P_Suryo = 1;
                        }
                        // 2006.02.06 YX-80�^�󎞖ڋʖӕ��⋭�̍ގ���
                        // B���޽��čގ���TF/NBR(2043),TF/EPDM(2071)
                        // �̏ꍇV#7010(2032)�A
                        // ���̑��̍ގ��̏ꍇB���޽��čގ��ƕύX
                        ////2006.01.10 YX-80�^�󎞖ڋʖӕ��⋭�̍ގ���
                        ////B���޽��čގ�����V#7010(2032)�Œ�ɕύX
                        ////P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight();  // B��
                        // P_MAT_CODE1 = "2032";  // V#7010
                        ////******************************************
                        if (P_HEADS_DATA[835].TrimRight() == "2043")
                        { // B��TF/NBR
                            P_MAT_CODE1 = "2032"; // V#7010
                        }
                        else if (P_HEADS_DATA[835].TrimRight() == "2071")
                        { // B��TF/EPDM
                            P_MAT_CODE1 = "2032"; // V#7010
                        }
                        else { // B��
                            P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B��
                        }
                        // ******************************************
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                        P_Unit = "��";
                        // �d�v���[�g�⋭��
                    }
                    else if (i_PateCDCode == 66) { // E�v���[�g�⋭�̏ꍇ�ގ��̓��^��
                        sPltMat = P_HEADS_DATA[43].TrimRight();
                        // 2016.11.07 �ގ����ޕύX
                        //if (sPltMat == "1014" || sPltMat == "1015" ||
                        //    sPltMat == "1032" || sPltMat == "1033" ||
                        //    sPltMat == "1057" || sPltMat == "1058" ||
                        //    sPltMat == "1085" || sPltMat == "1095") {
                        //    // �v���[�g�ގ����`�^���n�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
                        //    P_MAT_CODE1 = "1032"; // TP340
                        //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
                        //}
                        //else if (sPltMat == "1075" || sPltMat == "1089" ||
                        //    sPltMat == "1090") {
                        //    // �v���[�g�ގ����`�^���n�iSB-265-Gr.7[1075] SB-265-Gr.2[1089] B-265-Gr.2[1090]
                        //    P_MAT_CODE1 = "1032"; // TP340
                        //    // *************************************************
                        //    // 2010.12.02 SB-265-Gr.11�ǉ�
                        //}
                        //else if (sPltMat == "1076") {
                        //    // �v���[�g�ގ����`�^���n�iSB-265-Gr.11[1076]�j
                        //    P_MAT_CODE1 = "1032"; // TP340
                        //    // ***************************
                        //}
                        //else {
                        //    P_MAT_CODE1 = "1001"; // SUS304
                        //}
                        if ( (Type_MAT(sPltMat) == "Ti") ) {  // �v���[�g�ގ����`�^���n
                            P_MAT_CODE1 = "3132"; // TP340
                        } else {
                            P_MAT_CODE1 = "3101"; // SUS304
                        }
                        // ***********************
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                }
                // ********************************

                // 2021.04.19 YX-83�ύX_S
                //// 2017.08.17 YX-83�v���[�g���i�ǉ�_S
                //// �x�w�|�W�R
                //if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
                //    if (i_PateCDCode == 32) { // �K�X�P�b�g�a�P�i�m�^�Q�|�P���j
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                //        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A��
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 107) { // �K�X�P�b�g�a�Q�i�m�^�Q�|�P���j
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                //        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B��
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 108) { // �K�X�P�b�g�a�R�i�m�^�Q�|�P���j
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                //        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A��
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 109 ) { // �K�X�P�b�g�`�P�i�m�^�Q���j
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = (double)P_Suryo * 0.5;
                //        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B��
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 34) { // �K�X�P�b�g�`�Q�i�m�^�Q���j
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = (double)P_Suryo * 0.5;
                //        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A��
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 35) { // �K�X�P�b�g�`�R�i�m�^�Q���j
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = (double)P_Suryo * 0.5;
                //        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B��
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
                    if (i_PateCDCode == 32) { // �K�X�P�b�g�a�|�P�i�m�^�Q�|�P���j
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B��
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 33 ) { // �K�X�P�b�g�a�|�S�i�m���j
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = P_Suryo;
                        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B��
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 34) { // �K�X�P�b�g�`�|�Q�i�m�|�Q���j
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = P_Suryo - 2;
                        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A��
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 35) { // �K�X�P�b�g�`�|�R�i�m�^�Q���j
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = (double)P_Suryo * 0.5;
                        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A��
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                // 2021.04.19 YX-83�ύX_E
                    else if (i_PateCDCode == 38) { // �^�󎞖ڋʖӕ��⋭
                        // E1�m�Y���Ő��ʕω�
                        if (P_HEADS_DATA[951].TrimRight() == "0") {
                            P_Suryo = 2;
                        }
                        else {
                            P_Suryo = 1;
                        }
                        // YX-83�^�󎞖ڋʖӕ��⋭�̍ގ�
                        // B���޽��čގ���TF/NBR(2043),TF/EPDM(2071)
                        // �̏ꍇV#7010(2032)�A
                        // ���̑��̍ގ��̏ꍇB���޽��čގ�
                        if (P_HEADS_DATA[835].TrimRight() == "2043")
                        { // B��TF/NBR
                            P_MAT_CODE1 = "2032"; // V#7010
                        }
                        else if (P_HEADS_DATA[835].TrimRight() == "2071")
                        { // B��TF/EPDM
                            P_MAT_CODE1 = "2032"; // V#7010
                        }
                        else { // B��
                            P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B��
                        }
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                        P_Unit = "��";
                    }
                }
                // 2017.08.17 YX-83�v���[�g���i�ǉ�_E

                // 2007.05.10 UX-90,100,130,SX-90M,L�v���[�g�ގ����`�^���n�ȊO�ł��O�p���p�b�`�ǉ�
                ////2003.11.26 �p�b�`�ǉ�
                //// UX-90�O�p���p�b�`
                // if (i_PateCDCode == 80 || i_PateCDCode == 81 || i_PateCDCode == 82 || i_PateCDCode == 83 || i_PateCDCode == 84 || i_PateCDCode == 85 ) {
                // //2005.03.04 UX-100,130�O�p���p�b�`�ǉ�
                // //if ( G_KS_Syurui == "�t�w" && G_KS_Model == "�X�O" ) {
                // if ( G_KS_Syurui == "�t�w" && ( G_KS_Model == "�X�O" || G_KS_Model == "�P�O�O" || G_KS_Model == "�P�R�O" ) ) {
                // //*************************************
                // sPltMat = P_HEADS_DATA[43].TrimRight();
                ////2003.12.05 ASME PLATE
                ////      if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]�j
                // if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" ) && ( sPltMat != "1057" ) && ( sPltMat != "1058" ) && ( sPltMat != "1085" ) && ( sPltMat != "1095" )) {
                // // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
                ////*********************
                // P_Suryo = 0;
                // }
                // }
                // }
                ////*********************
                //
                ////2006.02.06 SX-90M,L�ǉ�
                ////��ڰčގ��ɂ��SX-90M,L�O�p���߯��̗L�����f
                //// �O�p���p�b�`
                // if (i_PateCDCode == 80 || i_PateCDCode == 81 || i_PateCDCode == 82 || i_PateCDCode == 83 || i_PateCDCode == 84 || i_PateCDCode == 85 ) {
                // if ( G_KS_Syurui == "�r�w" && ( G_KS_Model == "�X�O�l" || G_KS_Model == "�X�O�k" ) ) {
                // sPltMat = P_HEADS_DATA[43].TrimRight();
                // if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" ) && ( sPltMat != "1057" ) && ( sPltMat != "1058" ) && ( sPltMat != "1085" ) && ( sPltMat != "1095" )) {
                // // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
                // P_Suryo = 0;
                // }
                // }
                // }
                ////***********************
                // *******************************************************************************
                // 2012.07.31 LX-10���ׂĕ⋭�p�b�`�ǉ�
                ////2010.07.14 LX-10,RX-70���pTi0.5�̏ꍇ�⋭�p�b�`�ǉ�
                ////2010.09.08 RX-70���ׂĕ⋭�p�b�`�ǉ�
                ////if ( ( G_KS_Syurui == "�k�w" && G_KS_Model == "�P�O" )
                ////  || ( G_KS_Syurui == "�q�w" && G_KS_Model == "�V�O" ) ) {
                // if ( G_KS_Syurui == "�k�w" && G_KS_Model == "�P�O" ) {
                ////************************************
                // i_Pos = P_HEADS_DATA[36].Pos("M");
                // if (i_PateCDCode == 87 || i_PateCDCode == 88 ) {  // �⋭�p�b�`
                // if (i_Pos == 0) {  // ���p�ȊO
                // P_Suryo = 0;
                // }
                // sPltMat = P_HEADS_DATA[43].TrimRight();
                // if ( ( sPltMat != "1014" ) && ( sPltMat != "1015" )
                // && ( sPltMat != "1032" ) && ( sPltMat != "1033" )
                // && ( sPltMat != "1057" ) && ( sPltMat != "1058" )
                // //2010.10.29 SB-265-Gr.7�ǉ�
                // && ( sPltMat != "1075" )
                // //**************************
                // //2010.12.02 SB-265-Gr.11�ǉ�
                // && ( sPltMat != "1076" )
                // //***************************
                // && ( sPltMat != "1085" ) && ( sPltMat != "1089" )
                // && ( sPltMat != "1090" ) && ( sPltMat != "1095" )) {
                // // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015]
                // // TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058]
                // // SB-265-GR.1[1085] SB-265-GR.2[1089] B-265-GR.2[1090]
                // // B-265-GR.1[1095]�j
                // P_Suryo = 0;
                // }
                // s_Pltt = P_HEADS_DATA[1270].TrimRight();
                // if ( s_Pltt != "0.5" ) {
                // P_Suryo = 0;
                // }
                // }
                // }
                // ***************************************************
                // ************************************

                // �ǂݔ�΂��׸޾��
                if (i_PateCDCode == 1 || i_PateCDCode == 20 ||
                    i_PateCDCode == 39) {
                    // P_JokenFlg1 = 1;
                }
                else if (i_PateCDCode == 30 || i_PateCDCode == 57) {
                    // P_JokenFlg2 = 1;
                }

                // ���i�^��
                // 2008.07.24 CX-10D�ǉ�
                // if (i_PateCDCode == 37 || i_PateCDCode == 57){
                // P_Katasiki = P_SSyurui + "�|" + P_SModel + P_HEADS_DATA[1258].TrimRight();
                // } else {
                // P_Katasiki = P_SSyurui + "�|" + P_SModel + P_HEADS_DATA[1258].TrimRight();;
                // }
                if (P_SModel.SubString(P_SModel.Length() - 1, 2) == "�c") {
                    P_Katasiki = P_SSyurui + "�|" + P_SModel.SubString(1,
                        P_SModel.Length() - 2) + P_HEADS_DATA[1258].TrimRight
                        () + "�c";
                }
                else {
                    P_Katasiki = P_SSyurui + "�|" + P_SModel +
                        P_HEADS_DATA[1258].TrimRight();
                }
                // *********************

                // �\�����̎擾
                P_Yobisu = Syukei_Buhin_Yobisu(P_PLATECD[4], "0",
                    m_pcHeadsData);
                if (i_PateCDCode == 20 || i_PateCDCode == 21) { // �n���K�[
                    // 2004.01.16 �\�����̎擾����
                    // P_Yobisu += (P_PltGsktYobiA + P_PltGsktYobiB + P_PltGsktYobiD) * 2;
                    if (G_KS_Syurui == "�d�w" && (G_KS_Model == "�P�P" ||
                        G_KS_Model == "�P�T")) {
                        // 2004.04.23 �n���K�[�\�����̎擾����
                        P_Yobisu += P_PltYobiA + P_PltYobiB + P_PltYobiD;
                        // P_Yobisu += P_YobisuPltAll;
                        // ***********************************
                    }
                    else if (G_KS_Syurui == "�t�w" && (G_KS_Model == "�Q�O" ||
                        G_KS_Model == "�S�O")) {
                        // 2004.04.23 �n���K�[�\�����̎擾����
                        P_Yobisu += P_PltYobiA + P_PltYobiB + P_PltYobiD;
                        // P_Yobisu += P_YobisuPltAll;
                        // ***********************************
                    }
                    else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�S�O") {
                        // 2004.04.23 �n���K�[�\�����̎擾����
                        P_Yobisu += P_PltYobiA + P_PltYobiB + P_PltYobiD;
                        // P_Yobisu += P_YobisuPltAll;
                        // ***********************************
                    }
                    else {
                        // 2004.04.23 �n���K�[�\�����̎擾����
                        P_Yobisu += (P_PltYobiA + P_PltYobiB + P_PltYobiD) * 2;
                        // P_Yobisu += P_YobisuPltAll * 2;
                        // ***********************************
                    }
                }
                else if (i_PateCDCode == 11) { // �d�w�|�P�P�C�P�T�@�c�v���[�g���u
                    P_Yobisu += P_PltYobiD * 4;
                }
                else if (i_PateCDCode == 50) { // �d�w�|�P�T�@�d�v���[�g���u
                    P_Yobisu += P_PltYobiE * 2;
                }
                else if (i_PateCDCode == 80 || i_PateCDCode == 81 ||
                    i_PateCDCode == 82) {
                    // 2007.05.10 UX-90,100,130,SX-90M,L�v���[�g�ގ����`�^���n�ȊO�ł��O�p���p�b�`�ǉ�
                    //// �t�w�|�X�O�@�O�p���p�b�`�i�c�j
                    // sPltMat = P_HEADS_DATA[43].TrimRight();
                    // if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" ) && ( sPltMat != "1057" ) && ( sPltMat != "1058" ) && ( sPltMat != "1085" ) && ( sPltMat != "1095" )) {
                    // // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
                    // } else {
                    // P_Yobisu += P_PltGsktYobiD * 2;
                    // }
                    // �t�w�|�X�O�C�P�O�O�C�P�R�O�C�r�w�|�X�O�l�C�k �O�p���p�b�`�i�c�j
                    P_Yobisu += P_PltGsktYobiD * 2;
                    // *******************************************************************************
                }
                else if (i_PateCDCode == 83 || i_PateCDCode == 84 ||
                    i_PateCDCode == 85) {
                    // 2007.05.10 UX-90,100,130,SX-90M,L�v���[�g�ގ����`�^���n�ȊO�ł��O�p���p�b�`�ǉ�
                    //// �t�w�|�X�O�@�O�p���p�b�`�i�d�j
                    // sPltMat = P_HEADS_DATA[43].TrimRight();
                    // if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" ) && ( sPltMat != "1057" ) && ( sPltMat != "1058" ) && ( sPltMat != "1085" ) && ( sPltMat != "1095" )) {
                    // // �v���[�g�ގ����`�^���n�ȊO�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
                    // } else {
                    // P_Yobisu +=  P_PltGsktYobiE * 2;
                    // }
                    // �t�w�|�X�O�C�P�O�O�C�P�R�O�C�r�w�|�X�O�l�C�k �O�p���p�b�`�i�c�j
                    P_Yobisu += P_PltGsktYobiE * 2;
                    // *******************************************************************************
                }
                // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_S
                else if (i_PateCDCode == 116) {
                    // (S��)�s�f�B�X�^���X�s�[�X
                    P_Yobisu += P_PltGsktYobiD * 2;
                }
                else if (i_PateCDCode == 117) {
                    // (E��)�s�f�B�X�^���X�s�[�X
                    P_Yobisu += P_PltGsktYobiE * 2;
                }
                // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_E
                else if (i_PateCDCode == 87) {
                    // �k�w�|�R�O�@�⋭�p�b�`�i�c�j
                    P_Yobisu += P_PltGsktYobiD * 2;
                }
                else if (i_PateCDCode == 88) {
                    // �k�w�|�R�O�@�⋭�p�b�`�i�d�j
                    P_Yobisu += P_PltGsktYobiE * 2;
                    // 2009.02.16 UX-100�ި��ݽ�߰�D�ǉ�
                }
                else if (i_PateCDCode == 103) {
                    // �t�w�|�P�O�O�@�f�B�X�^���X�s�[�X�c�i�c�j
                    P_Yobisu += P_PltGsktYobiD * 2;
                }
                else if (i_PateCDCode == 104) {
                    // �t�w�|�P�O�O�@�f�B�X�^���X�s�[�X�c�i�d�j
                    P_Yobisu += P_PltGsktYobiE * 2;
                    // *********************************
                    // ***************************
                    // 2004.01.19 YX-80�v���[�g�\�����i�ǉ�
                    // �x�w�|�W�O���ԃK�X�P�b�g
                }
                else if (i_PateCDCode == 32) { // �K�X�P�b�g�a�|�P�i�a�~�P���j
                    P_Yobisu += P_PltGsktYobiB;
                }
                else if (i_PateCDCode == 33) { // �K�X�P�b�g�a�|�S�i�c�~�Q�{�`�~�Q���j
                    // 2021.04.19 YX-83�ύX_S
                    //P_Yobisu += P_PltGsktYobiD * 2 + P_PltGsktYobiA * 2;
                    if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
                        //�K�X�P�b�g�a�|�S�i�c�~�Q�{�`�~�Q���j
                        P_Yobisu += P_PltGsktYobiD * 2 + P_PltGsktYobiA * 2;
                    }
                    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
                        //�K�X�P�b�g�a�|�S�i�`�~�Q���j
                        P_Yobisu += P_PltGsktYobiA * 2;
                    }
                    // 2021.04.19 YX-83�ύX_E
                }
                else if (i_PateCDCode == 34) { // �K�X�P�b�g�`�|�Q�i�c�~�Q�{�a�~�Q���j
                    // 2017.08.17 YX-83�v���[�g���i�ǉ�_S
                    //P_Yobisu += P_PltGsktYobiD * 2 + P_PltGsktYobiB * 2;
                    if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") {
                        // �K�X�P�b�g�`�|�Q�i�c�~�Q�{�a�~�Q���j
                        P_Yobisu += P_PltGsktYobiD * 2 + P_PltGsktYobiB * 2;
                    }
                    else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R") {
                        // 2021.04.19 YX-83�ύX_S
                        //// �K�X�P�b�g�`�Q�i�`�~�P���j
                        //     P_Yobisu += P_PltGsktYobiA;
                        // �K�X�P�b�g�`�|�Q�i�a�~�Q���j
                        P_Yobisu += P_PltGsktYobiB * 2;
                        // 2021.04.19 YX-83�ύX_E
                    }
                    // 2017.08.17 YX-83�v���[�g���i�ǉ�_E
                }
                else if (i_PateCDCode == 35) { // �K�X�P�b�g�`�|�R�i�`�~�P���j
                    P_Yobisu += P_PltGsktYobiA;
                    // �x�w�|�W�O�^�󎞖ڋʖӕ��⋭
                }
                else if (i_PateCDCode == 38) {
                    P_Yobisu += P_PltGsktYobiS;
                    // �x�w�|�W�O�d�v���[�g�⋭��
                }
                else if (i_PateCDCode == 66) { // E�v���[�g�⋭�i�d�~�Q���j
                    P_Yobisu += P_PltYobiE * 2;
                    // ************************************
                // 2021.04.19 YX-83�ύX_S
                //// 2017.08.17 YX-83�v���[�g���i�ǉ�_S
                //else if (i_PateCDCode == 107) { // �K�X�P�b�g�a�Q�i�a�~�P���j
                //    P_Yobisu += P_PltGsktYobiB;
                //}
                //else if (i_PateCDCode == 108) { // �K�X�P�b�g�a�R�i�a�~�P���j
                //    P_Yobisu += P_PltGsktYobiB;
                //}
                //else if (i_PateCDCode == 109) { // �K�X�P�b�g�`�P�i�`�~�P���j
                //    P_Yobisu += P_PltGsktYobiA;
                //// 2017.08.17 YX-83�v���[�g���i�ǉ�_E
                // 2021.04.19 YX-83�ύX_E
                }
                // ***********************************
                // ***  ��ڰčE���ް�  CSV̧�ٍ쐬
                // ***********************************
                Write_PlateBuhin_Data(ret);

                if ((P_PLATECD[4] == "PLTE05") || (P_PLATECD[4].SubString(1,
                    4) == "GSKT")) {
                    Write_HeartsBuhin_Data(P_PLATECD[4]);
                }
            }

        }
    }

    fclose(fp); // ����
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰĕ��i�\��Ͻ��E�ގ�����Ͻ������i��ڰĕ��i �ް��쐬�p�j
// �T  �v      �F
// ��  ��      �F AnsiString Key   : ��ڰČ^��
// �F            PType : ��ڰĎ��
// �F            PCode : ���i����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Search_HD_PLATE_2(AnsiString Key,
    AnsiString PType, AnsiString PCode) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PLATE";
    s_SQL = s_SQL + "  WHERE MODEL_CODE = '" + Key + "'";
    s_SQL = s_SQL + "    AND PLATE_TYPE = '" + PType + "'";
    s_SQL = s_SQL + "    AND PARTS_CODE = " + PCode + "";

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
        P_Zaisitu = "";
        P_EZaisitu = "";
        P_Suryo = 0;
        // 2003.11.26 �ϐ��ǉ�
        P_Zuban2 = "";
        P_SZuban2 = "";
        // *******************
        delete wkQuery;
        // return false;
        return true;
    }
    else {
        P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_MAT_CODE1 = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
        P_Suryo = wkQuery->FieldByName("NUMS")->AsString.ToIntDef(0);
        // 2003.11.26 �ϐ��ǉ�
        P_Zuban2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        P_SZuban2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        // *******************

        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
        // // �ގ�����Ͻ�����
        // s_SQL = "";
        // s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
        // s_SQL = s_SQL + "  WHERE MAT_CODE = '" + P_MAT_CODE1 + "'";
        //
        // wkQuery->Close();
        // wkQuery->DatabaseName = ODBC_DSN_MAT;
        // wkQuery->SQL->Clear();
        // wkQuery->SQL->Add(s_SQL);
        // wkQuery->Open();
        // wkQuery->First();
        //
        // if(wkQuery->Eof){
        // P_Zaisitu = "";
        // G_ErrLog_Text = "�ގ����ށw" + P_MAT_CODE1 + "�x�͑��݂��܂���I�I";
        // Write_Error_Log(G_ErrLog_Text);
        // } else {
        // if(P_HEADS_DATA[1].TrimRight() == "0" ){
        // // �a�����̂̎擾
        // P_Zaisitu = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
        // } else {
        // P_Zaisitu = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
        // }
        // }
        //
        // delete wkQuery;

        return true;
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰčE���ް�  CSV̧�ٍ쐬
// �T  �v      �F
// ��  ��      �F bool ret [true:��ڰĕ��i�\��Ͻ��ɊY��ں��ނ��� false:�Ȃ�]
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F TYouryosyo ����
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_PlateBuhin_Data(bool ret) {
    FILE *wkfp;

    // ��ڰčE���ް��p
    AnsiString s_Ana_Kata;
    AnsiString s_Ana_Suryo;
    AnsiString s_Ana_Anaake;
    AnsiString s_Text;

    int i_Daisu;

    AnsiString wk_FilePass;

    // ------------------------------------------
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    AnsiString sSpecCode;
    // **********************************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // **********************************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03
    // ------------------------------------------

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
    s_Text = "PLATE";
    fputs(s_Text.c_str(), wkfp);
    // 2  ���ʎq
    s_Text = "," + P_PLATECD[1];
    fputs(s_Text.c_str(), wkfp);
    // 3  ���i�^��
    if (ret == true) {
        s_Text = "," + P_Katasiki;
    }
    else {
        s_Text = ",";
    }
    fputs(s_Text.c_str(), wkfp);
    // 4  ���i����
    if (ret == true) {
        s_Text = "," + P_PLATECD[3];
    }
    else {
        s_Text = ",";
    }
    fputs(s_Text.c_str(), wkfp);
    // 5  �ގ�����
    if (ret == true) {
        s_Text = "," + P_Zaisitu;
    }
    else {
        s_Text = ",";
    }
    fputs(s_Text.c_str(), wkfp);
    // 6  �}��
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), wkfp);
    // 7  ��}�p�}��
    if (ret == true) {
        s_Text = "," + P_SZuban;
    }
    else {
        s_Text = ",";
    }
    fputs(s_Text.c_str(), wkfp);

    // 8  1�䕪����,9  �\����,10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
    if (ret == true) {
        s_Text = "," + AnsiString(P_Suryo);
        fputs(s_Text.c_str(), wkfp);

        s_Text = "," + AnsiString(P_Yobisu);
        fputs(s_Text.c_str(), wkfp);

        i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // �䐔
        P_Soukei = P_Suryo * i_Daisu + P_Yobisu;
        s_Text = "," + AnsiString(P_Soukei);
        fputs(s_Text.c_str(), wkfp);
    }
    else {
        s_Text = ",0";
        fputs(s_Text.c_str(), wkfp);
        s_Text = ",0";
        fputs(s_Text.c_str(), wkfp);
        s_Text = ",0";
        fputs(s_Text.c_str(), wkfp);
    }

    // 11 �d�l�P(��ڰčE���ް��� �E��)
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 12 �d�l�Q
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 13 �d�l�R
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 14 �d�l�S
    s_Text = ",";
    // 2017.08.17 YX-83�v���[�g���i�ǉ�_S
    if (P_PLATECD[1] == "IVCR--") {
        s_Text = ",��";
    }
    // 2017.08.17 YX-83�v���[�g���i�ǉ�_E
    fputs(s_Text.c_str(), wkfp);
    // 15 �d�l�T
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 16  �ގ�����(�p)
    s_Text = "," + P_EZaisitu;
    fputs(s_Text.c_str(), wkfp);

    // ------------------------------
    // 2007.10.03 �װ�d�l�Ή�
    // 17 �d�l���ߺ���
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), wkfp);
    // 2007.10.03
    // ------------------------------

    // ���s����
    s_Text = "\n";
    // CSV�ް��@̧�ُo��
    fputs(s_Text.c_str(), wkfp);

    fclose(wkfp); // ����

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰĶ޽����ް�  CSV̧�ٍ쐬
// �T  �v      �F
// ��  ��      �F bool ret [true:��ڰĕ��i�\��Ͻ��ɊY��ں��ނ��� false:�Ȃ�]
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F TYouryosyo ����
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_PlateBuhin_Data_GSKT(bool ret) {
    FILE *wkfp;

    // ��ڰčE���ް��p
    AnsiString s_Ana_Kata;
    AnsiString s_Ana_Suryo;
    AnsiString s_Ana_Anaake;
    AnsiString s_Text;

    int i_Daisu;

    AnsiString wk_FilePass;

    // ----------------------------------------------
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    AnsiString sSpecCode;
    // **********************************************
    // �d�l���ߺ��ގ擾(""/"A"/"B")
    // **********************************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03 �װ�d�l�Ή� �d�l���ߺ���
    // ----------------------------------------------

    // 2007.12.17 UX-90������
    int i_BANA; // �a���E
    int i_AANA; // �`���E
    // **********************

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
    if (bMouAteFlg == true) {
        s_Text = "MOATE"; // �ӓ��p
    }
    else {
        s_Text = "PLATE";
    }
    fputs(s_Text.c_str(), wkfp);

    bMouAteFlg = false;
    // 2  ���ʎq
    s_Text = "," + P_PLATECD[1];
    fputs(s_Text.c_str(), wkfp);
    // 3  ���i�^��
    s_Text = "," + P_Katasiki;
    // 2004.12.15 GX-20�޽��Č^���ύX
    if (P_PLATECD[1] == "DDP-21" || P_PLATECD[1] == "EDPC21") {
        s_Text = ",�f�w�|�Q�P�`";
    }
    else if (P_PLATECD[1] == "DDP-22" || P_PLATECD[1] == "EDPC22") {
        s_Text = ",�f�w�|�Q�Q�`";
    }
    // ******************************
    fputs(s_Text.c_str(), wkfp);
    // 4  ���i����
    s_Text = "," + P_PLATECD[3];
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

    // 8  1�䕪����,9  �\����,10 ���v(1�䕪���ʁ~HEADS�ް����䐔+�\����)
    s_Text = "," + AnsiString(P_Suryo);
    fputs(s_Text.c_str(), wkfp);

    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), wkfp);

    i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // �䐔
    P_Soukei = P_Suryo * i_Daisu + P_Yobisu;
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), wkfp);

    // 11 �d�l�P(��ڰčE���ް��� �E��)
    s_Text = ",";
    // 2007.12.17 UX-90������
    if (G_KS_Syurui == "�t�w" && G_KS_Model == "�X�O" && P_PLATECD[1] == "EPGS-R") {
        i_BANA = 0; // �a���E
        i_AANA = 0; // �`���E
        if (P_HEADS_DATA[951].TrimRight() == "0") {
        }
        else {
            i_BANA = i_BANA + 1; // �a���E
        }
        if (P_HEADS_DATA[966].TrimRight() == "0") {
        }
        else {
            i_AANA = i_AANA + 1; // �`���E
        }
        if (P_HEADS_DATA[981].TrimRight() == "0") {
        }
        else {
            i_BANA = i_BANA + 1; // �a���E
        }
        if (P_HEADS_DATA[996].TrimRight() == "0") {
        }
        else {
            i_AANA = i_AANA + 1; // �`���E
        }
        if (i_AANA == i_BANA) {
        }
        else {
            // �E�̐����`���a���ňقȂ�
            if (sSpecCode == "A") {
                s_Text = ",�`���@";
            }
            else if (sSpecCode == "B") {
                s_Text = ",�a���@";
            }
        }
    }
    // **********************
    fputs(s_Text.c_str(), wkfp);
    // 12 �d�l�Q
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 13 �d�l�R
    s_Text = "," + DE_Kubun;
    fputs(s_Text.c_str(), wkfp);
    // 14 �d�l�S
    s_Text = "," + P_Unit;
    fputs(s_Text.c_str(), wkfp);
    // 15 �d�l�T
    s_Text = "," + P_SideDiv;
    fputs(s_Text.c_str(), wkfp);
    // 16  �ގ�����(�p)
    s_Text = "," + P_EZaisitu;
    fputs(s_Text.c_str(), wkfp);

    // ------------------------------
    // 2007.10.03 �װ�d�l�Ή�
    // 17 �d�l���ߺ���
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), wkfp);
    // 2007.10.03
    // ------------------------------

    // ���s����
    s_Text = "\n";
    // CSV�ް��@̧�ُo��
    fputs(s_Text.c_str(), wkfp);

    fclose(wkfp); // ����

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F HEARTS�p�ް�(���i)  CSV̧�ٍ쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �e���ڂ��g�h�ň͂�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_HeartsBuhin_Data(AnsiString s_Mat_Code) {
    FILE *wkfp;

    // ��ڰčE���ް��p
    AnsiString s_Text;

    AnsiString wk_FilePass;

	// ****************************
	// ***  HEARTS�p�ް��߽�擾
	// ****************************
	if (AllNumChk(G_SAKUBAN.c_str(), 0) == 0) {
		wk_FilePass = G_xls_Output_Dir + G_OUT_ORDER_Dir + "\\" + G_SAKUBAN +
			"\\" + G_SAKUBAN + ".csv";
	}
	else {
		wk_FilePass = G_xls_Output_Dir + G_OUT_ESTIM_Dir + "\\" + G_SAKUBAN +
			"\\" + G_SAKUBAN + ".csv";
	}

    // ****************************
    // ***  HEARTS�p�ް��@OPEN
    // ****************************
    if ((wkfp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(wkfp);
        return false;
    }

    // ****************************
    // *** �擪4��="PLTE"�̏ꍇ
    // ****************************
    if (s_Mat_Code.SubString(1, 4) == "PLTE") {
        // 1  ���i����
        s_Text = "\"PLTE\"";
        fputs(s_Text.c_str(), wkfp);

        // 2  ���ʎq
        s_Text = ",\"" + s_Mat_Code + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 3  ��ڰČ^��
        s_Text = ",\"" + ZenToHan(P_Katasiki) + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 4  �ގ�����
        s_Text = ",\"" + P_MAT_CODE1 + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 5  ��ڰĎ��
        s_Text = ",\"\"";
        fputs(s_Text.c_str(), wkfp);

        // 6  �E��
        s_Text = ",\"\"";
        fputs(s_Text.c_str(), wkfp);

        // 7  ����
        // s_Text = ",\""+ IntToStr(P_Suryo) +"\"";
        s_Text = ",\"" + IntToStr(__int64(P_Suryo)) + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 8  �V���敪
        s_Text = ",\"0\"";
        fputs(s_Text.c_str(), wkfp);

        // 9  �����ݶް�敪
        if ((s_Mat_Code == "PLTE05") && (ZenToHan(P_Katasiki).SubString(1,
            4) == "LX-4") && (P_HEADS_DATA[38].ToIntDef(0) > 200))
            s_Text = ",\"1\"";
        else
            s_Text = ",\"0\"";
        fputs(s_Text.c_str(), wkfp);

        // ���s����
        s_Text = "\n";
        // CSV�ް��@̧�ُo��
        fputs(s_Text.c_str(), wkfp);
    }
    else if (s_Mat_Code.SubString(1, 4) == "GSKT") {
        // 1  ���i����
        s_Text = "\"GSKT\"";
        fputs(s_Text.c_str(), wkfp);

        // 2  ���ʎq
        s_Text = ",\"" + s_Mat_Code + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 3  �ذ�ތ^��
        s_Text = ",\"" + ZenToHan(P_Katasiki) + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 3  �ڰь^��
        s_Text = ",\"" + ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) +
            "-" + P_HEADS_DATA[36].TrimRight() + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 4  �ގ�����
        s_Text = ",\"" + P_MAT_CODE1 + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 5  ����
        // s_Text = ",\""+ IntToStr(P_Suryo) +"\"";
        // Windows Server2008�ڐA
        s_Text = ",\"" + IntToStr(__int64(P_Suryo)) + "\"";
        fputs(s_Text.c_str(), wkfp);

        // ���s����
        s_Text = "\n";
        // CSV�ް��@̧�ُo��
        fputs(s_Text.c_str(), wkfp);
    }

    // --------------------
    // 2007.10.03 �ǉ�
    fclose(wkfp);
    // 2007.10.03
    // --------------------

    return true;
}

// ��2002/10/23 �ȍ~

// ---------------------------------------------------------------------------
// ���{��֐����F �����i�@�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_Ateita_Data(void) {
    int ret;

    //
    // "D"
    //
    DE_Kubun = "D";
    Get_Gskt_MastKey(0); // �K�X�P�b�g�����L�[�擾
    // ret = Search_HD_BFRG( G_D_GsktType, G_GsktType, G_FrmDiv );
    // S1
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(1,1), BBP_Code, BBP_Label, BBP_Name, BBP_Num, "D", P_HEADS_DATA[836].TrimRight());
    // S2
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(2,1), ABP_Code, ABP_Label, ABP_Name, ABP_Num, "D", P_HEADS_DATA[835].TrimRight());
    // S3
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3,1), BBP_Code, BBP_Label, BBP_Name, BBP_Num, "D", P_HEADS_DATA[836].TrimRight());
    // S4
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4,1), ABP_Code, ABP_Label, ABP_Name, ABP_Num, "D", P_HEADS_DATA[835].TrimRight());

    //
    // "E"
    //
    DE_Kubun = "E";
    // 2008.07.24 CX-10D�ǉ�
    // P_Katasiki = P_SSyurui + "�|" + P_SModel + P_HEADS_DATA[1258].TrimRight();
    if (P_SModel.SubString(P_SModel.Length() - 1, 2) == "�c") {
        P_Katasiki = P_SSyurui + "�|" + P_SModel.SubString(1,
            P_SModel.Length() - 2) + P_HEADS_DATA[1258].TrimRight() + "�c";
    }
    else {
        P_Katasiki = P_SSyurui + "�|" + P_SModel +
            P_HEADS_DATA[1258].TrimRight();
    }
    // *********************
    Get_Gskt_MastKey(1); // �K�X�P�b�g�����L�[�擾
    ret = Search_HD_BFRG(G_E_GsktType, G_GsktType, G_FrmDiv);

    // 2004.02.24 G_D_GsktABKubun�����ǉ�
    ////S1
    // P_SideDiv ="B";
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(1,1), BBP_Code, BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
    ////S2
    // P_SideDiv ="A";
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(2,1), ABP_Code, ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
    // S3
    // P_SideDiv ="B";
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3,1), BBP_Code, BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
    ////S4
    // P_SideDiv ="A";
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4,1), ABP_Code, ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
    // A�X�^�[�g
    if (G_E_GsktABKubun == "A") {
        // S1:B���̏���
        P_SideDiv = "B";
        ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(1, 1), BBP_Code,
            BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        // S2:A���̏���
        P_SideDiv = "A";
        ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(2, 1), ABP_Code,
            ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        // 2017.08.17 �΍s���ǉ�_S
        //// S3:B���̏���
        //P_SideDiv = "B";
        //ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), BBP_Code,
        //    BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        //// S4:A���̏���
        //P_SideDiv = "A";
        //ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), ABP_Code,
        //    ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        if (P_FL == 1) { // �΍s��
            // �΍s��S3:A���̏���
            P_SideDiv = "A";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), ABP_Code,
                ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
            // �΍s��S4:B���̏���
            P_SideDiv = "B";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), BBP_Code,
                BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        }
        else {
            // S3:B���̏���
            P_SideDiv = "B";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), BBP_Code,
                BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
            // S4:A���̏���
            P_SideDiv = "A";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), ABP_Code,
                ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        }
        // 2017.08.17 �΍s���ǉ�_E
    }
    // B�X�^�[�g
    else {
        // S1:B���̏���
        P_SideDiv = "B";
        ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(1, 1), ABP_Code,
            ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        // S2:A���̏���
        P_SideDiv = "A";
        ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(2, 1), BBP_Code,
            BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        // 2017.08.17 �΍s���ǉ�_S
        //// S3:B���̏���
        //P_SideDiv = "B";
        //ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), ABP_Code,
        //    ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        //// S4:A���̏���
        //P_SideDiv = "A";
        //ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), BBP_Code,
        //    BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        if (P_FL == 1) { // �΍s��
            // �΍s��S3:A���̏���
            P_SideDiv = "A";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), BBP_Code,
                BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
            // �΍s��S4:B���̏���
            P_SideDiv = "B";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), ABP_Code,
                ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        }
        else {
            // S3:B���̏���
            P_SideDiv = "B";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), ABP_Code,
                ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
            // S4:A���̏���
            P_SideDiv = "A";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), BBP_Code,
                BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        }
        // 2017.08.17 �΍s���ǉ�_E
    }
    // **********************************

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �����i�@�ް��쐬 �T�u
// �T  �v      �F
// �F�@�@�@�@�@�@s_Ana    : �E���f�[�^��S1,S2�E�E�̃f�[�^
// �F            PCode    : ���i����
// �F            PLabel   : ���i����
// �F            PName    : ���i��
// �F            PNum     : ���i����
// �F            PMode    : D/E���[�h
// �F            PMatCode : ���i�ގ�����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_Ateita_Data_Sub(AnsiString s_Ana,
    AnsiString PCode, AnsiString PLabel, AnsiString PName, long PNum,
    AnsiString PMode, AnsiString PMatCode) {
    int ret;
    AnsiString sPltMat;

    // �E�̏ꍇ
    if (s_Ana == "0") {
        // �}�X�^�����֐�
        ret = Search_HD_PLATE_3(P_SoSyoAscii, PMode, PCode, PMatCode);
        if (ret == true) {
            // ���ʎq�Ɩ��̂Ɛ��ʂ��㏑������B
            P_PLATECD[1] = PLabel;
            P_PLATECD[3] = PName;
            P_Suryo = PNum;
            // 2003.11.26 PCode�ǉ�
            // if ( PCode == "65" ) { // �⋭�K�X�P�b�g�͍ގ��m�a�q�ɌŒ�
            if (PCode == "65" || PCode == "78") { // �⋭�K�X�P�b�g�͍ގ��m�a�q�ɌŒ�
                // ********************
                PMatCode = "2019";
                P_Zaisitu = "NBR";
                P_EZaisitu = "NBR";
                // 2004.01.16 �\�����̎擾����
                P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                P_Yobisu += P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                // ***************************
            }
            if (PCode == "61" || PCode == "62" || PCode == "63")
            { // �ӓ��̏ꍇ�ގ��̓��^��
                if (P_SoSyoAscii.SubString(1, 5) == "EX-15" ||
                    P_SoSyoAscii.SubString(1, 5) == "LX-40") {
                    sPltMat = P_HEADS_DATA[43].TrimRight();
                    // 2016.11.07 �ގ����ޕύX
                    //// 2003.12.05 ASME PLATE
                    //// if (sPltMat == "1014" || sPltMat == "1015" || sPltMat == "1032" || sPltMat == "1033" ) {  // �v���[�g�ގ����`�^���n�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]�j
                    //if (sPltMat == "1014" || sPltMat == "1015" ||
                    //    sPltMat == "1032" || sPltMat == "1033" ||
                    //    sPltMat == "1057" || sPltMat == "1058" ||
                    //    sPltMat == "1085" || sPltMat == "1095") {
                    //    // �v���[�g�ގ����`�^���n�iTP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]�j
                    //    // *********************
                    //    PMatCode = "1014"; // TP270
                    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
                    //}
                    //else if (sPltMat == "1075" || sPltMat == "1089" ||
                    //    sPltMat == "1090") {
                    //    // �v���[�g�ގ����`�^���n�iSB-265-GR.7[1075] SB-265-GR.2[1089] B-265-GR.2[1090]�j
                    //    PMatCode = "1014"; // TP270
                    //    // *************************************************
                    //    // 2010.12.02 SB-265-Gr.11�ǉ�
                    //}
                    //else if (sPltMat == "1076") {
                    //    // �v���[�g�ގ����`�^���n�iSB-265-GR.11[1076]�j
                    //    PMatCode = "1014"; // TP270
                    //    // ********************************************
                    //}
                    //else {
                    //    PMatCode = "1003"; // SUS316
                    //}
                    if ( (Type_MAT(sPltMat) == "Ti") ) {  // �v���[�g�ގ����`�^���n
                        PMatCode = "3114"; // TP270
                    } else {
                        PMatCode = "3103"; // SUS316
                    }
                    // ***********************
                    // 2004.01.16 �\�����̎擾����
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltYobiE; // �v���[�g�̗\���i����
                    // ***************************
                }
                else if (P_SoSyoAscii.SubString(1, 5) == "EX-11" ||
                    P_SoSyoAscii.SubString(1, 5) == "EX-16") {
                    PMatCode = "1040"; // SS400
                    bMouAteFlg = true; // �t���[�����i�Ƃ��ďo��
                    // 2004.01.16 �\�����̎擾����
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    // ***************************
                    // 2003.12.17 YX-80
                }
                else if (P_SoSyoAscii.SubString(1, 5) == "YX-80") {
                    PMatCode = "2019"; // NBR
                    // 2017.08.17 YX-80���i�ύX_S
                    //P_Unit = "��";
                    if (PCode == "63" ) {
                        P_Unit = "��";
                    } else {
                        P_Unit = "";
                    }
                    // 2017.08.17 YX-80���i�ύX_E
                    // 2004.01.16 �\�����̎擾����
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ***************************
                    // ****************
                }
                // 2017.08.17 YX-83���i�ǉ�_S
                else if (P_SoSyoAscii.SubString(1, 5) == "YX-83") {
                    PMatCode = "2019"; // NBR
                    if (PCode == "63" ) {
                        P_Unit = "��";
                    } else {
                        P_Unit = "";
                    }
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                // 2017.08.17 YX-83���i�ǉ�_E
                else {
                    // 2016.11.07 �ގ����ޕύX
                    //PMatCode = "1003"; // SUS316
                    PMatCode = "3103"; // SUS316
                    // ***********************
                    // 2004.01.16 �\�����̎擾����
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltYobiE; // �v���[�g�̗\���i����
                    // ***************************
                }
                P_Zaisitu = Search_HD_MATERIAL(PMatCode);
                P_EZaisitu = Search_HD_E_MATERIAL(PMatCode);
            }
            // 2019.03.18 YX-80�ύX_S
            if (PCode == "112" || PCode == "113")
            { // �a�f�B�X�^���X�s�[�X�`�a�̏ꍇ�ގ��͂m�a�q
                PMatCode = "2019"; // NBR
                if (P_SoSyoAscii.SubString(1, 5) == "YX-80") {
                    if (PCode == "113" ) {
                        P_Unit = "��";
                    } else {
                        P_Unit = "";
                    }
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                else if (P_SoSyoAscii.SubString(1, 5) == "YX-83") {
                    if (PCode == "113" ) {
                        P_Unit = "��";
                    } else {
                        P_Unit = "";
                    }
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                else {
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltYobiE; // �v���[�g�̗\���i����
                }
                P_Zaisitu = Search_HD_MATERIAL(PMatCode);
                P_EZaisitu = Search_HD_E_MATERIAL(PMatCode);
            }
            // 2019.03.18 YX-80�ύX_E

            // �o�͂���B
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(PMatCode); // HEARTS�p�ް�CSV�쐬
        }
    }

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���ԃv���[�g�K�X�P�b�g���i�@�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_M_Gskt_Data(void) {

    AnsiString s_Text, s_Model, s_Key;
    int i_Mojisu;

    // **********************************
    // ***  ��ڰĕ��i�\��Ͻ��@����
    // **********************************
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADS�ް� ����
    // 2009.06.19 UX-100R�ǉ�
    // EX�̏ꍇ
    // s_Model��[����"C","P","R","A","B"������ꍇ
    // s_Key��G_K_Model1�̌��ɂ��̕�����ǉ�����
    //
    // EX�ȊO�̏ꍇ
    // s_Model��[����"C","P","A","B"������ꍇ
    // s_Key��G_K_Model1�̌��ɂ��̕�����ǉ�����
    // �A��s_Model�̍Ō�̕�����"D"�̏ꍇ
    // s_Key��G_K_Model1��"D"�̑O�̈ʒu��"C","P","A","B"����ǉ�����
    ////2006.02.06 SX-90M,L�ǉ�
    ////P_HEADS_DATA[35]��[����"C","P","R","A","B"������ꍇ
    ////s_Key��G_K_Model1�̌��ɂ��̕�����ǉ�����
    ////2008.07.24 CX-10D�ǉ�
    ////�A��P_HEADS_DATA[35]�̍Ō�̕�����"D"�̏ꍇ
    ////s_Key��G_K_Model1��"D"�̑O�̈ʒu��"C","P","R","A","B"����ǉ�����
    ////*********************
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    // if (s_Text.ToIntDef(999) != 999){
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R�ǉ�
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (G_K_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D�ǉ�
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = G_K_Model1.SubString(1, G_K_Model1.Length() - 1) +
            s_Text + "D";
        s_Key = G_K_Syurui + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    // A�K�X�P�b�g��B�K�X�P�b�g�̍ގ��������ꍇ
    // if( P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836].TrimRight() ){
    // //A�޽��Ă��g�p����B
    // Write_M_Gskt_Data_Sub( P_HEADS_DATA[835].TrimRight(), false );
    // }
    // else{
    // A,B��s���o�͂��܂��B
    Write_M_Gskt_Data_Sub(P_HEADS_DATA[835].TrimRight(), true, "B");
    Write_M_Gskt_Data_Sub(P_HEADS_DATA[836].TrimRight(), true, "A");
    // }

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���ԃv���[�g�K�X�P�b�g���i�@�ް��o�̓T�u
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_M_Gskt_Data_Sub(AnsiString GsktCode,
    bool bOutMode, AnsiString sBADiv) {
    int ret;
    int iSuryo;
    int iMod;

    // �}�X�^�����֐�
    ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "30", GsktCode);

    if (ret == true) {

        //2022/01/06 �K�X�P�b�g�}�ԕύX_S
        if (G_KS_Syurui == "�f�w" && G_KS_Model == "�Q�O" && GsktCode == "2097") {
            // �f�w�|�Q�O �`�|�d�o�c�l�v���[�g�K�X�P�b�g�}�Ԃ͂d�P�O�P�P�T�|�q�Q
            P_Zuban = "�d�P�O�P�P�T�|�q�Q";    //�}��
            P_SZuban = "�d�P�O�P�P�T";         //��}�p�}��
        }
        //2022/01/06 �K�X�P�b�g�}�ԕύX_E

        // ���ʕ␳ ����=���ʁ^�Q�{�O�D�T�@�������؂�グ
        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
        iSuryo = int(P_Suryo);
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if ((P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836].TrimRight()) &&
            (sBADiv == "B") && (iMod == 0)) {
            P_Suryo = ((double)(P_Suryo - 1) * 0.5) - 0.5;
        }
        else {
            P_Suryo = ((double)(P_Suryo - 1) * 0.5) + 0.5;
        }

        P_PLATECD[1] = "IPGS--";
        P_PLATECD[3] = "�v���[�g�K�X�P�b�g";
        // �o�͂���B
        P_Yobisu = Syukei_Buhin_Yobisu("GSKT01", GsktCode, m_pcHeadsData);
        if (sBADiv == "A") {
            P_Yobisu = P_Yobisu + P_PltGsktYobiA;
        }
        else {
            P_Yobisu = P_Yobisu + P_PltGsktYobiB;
        }
        P_Unit = "";
        P_SideDiv = sBADiv;
        Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        Write_HeartsBuhin_Data(GsktCode); // HEARTS�p�ް�CSV�쐬
    }
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �c�v���[�g�K�X�P�b�g���i�@�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_D_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    ret = Search_HD_D_GASKET(G_D_GsktType, G_GsktType, G_FrmDiv);

    DE_Kubun = "D";

    // 2004.06.09 LX-10 TCG�p�ގ�����ǉ�
    AnsiString s_B_GSKT_Mat;
    AnsiString s_A_GSKT_Mat;
    s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
    s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
    if (G_KS_Syurui == "�k�w" && G_KS_Model == "�P�O") { // LX-10
        if (s_B_GSKT_Mat == "2063" || s_A_GSKT_Mat == "2063") {
            // B��A�������ꂩ��2063(AT-TF/EPDM)�̏ꍇ����2063�ɂ���
            s_B_GSKT_Mat = "2063";
            s_A_GSKT_Mat = "2063";
        }
        //2017.10.10 AN-TF/EPDM�ǉ�_S
        else if (s_B_GSKT_Mat == "2064" || s_A_GSKT_Mat == "2064") {
            // B��A�������ꂩ��2064(AN-TF/EPDM)�̏ꍇ����2064�ɂ���
            s_B_GSKT_Mat = "2064";
            s_A_GSKT_Mat = "2064";
        }
        //2017.10.10 AN-TF/EPDM�ǉ�_E
    }

    // �p�^�[������ɂ��K�v�ȃK�X�P�b�g��I��
    // A�X�^�[�g
    if (G_D_GsktABKubun == "A") {
        // S1:B���̏���
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(1, 1), DP_B0s1_Code,
            DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code,
            DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, s_B_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // S2:A���̏���
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(2, 1), DP_B0s2_Code,
            DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code,
            DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, s_A_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // �^���ƃR�[�h�̔�r
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B���ގ��̐��ʂŏo��
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // P_SideDiv ="";
            P_Suryo = DPU_Meq_Sub;
            P_Yobisu = DPU_Meq_Sub * P_PltGsktYobiD;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
        // 2008.07.24 CX-10D�ǉ�
        ////S3:B���̏���
        // P_SideDiv ="B";
        // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(3,1), DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, s_B_GSKT_Mat );
        // s_BLabel = P_PLATECD[1];
        ////S4:A���̏���
        // P_SideDiv ="A";
        // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(4,1), DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, s_A_GSKT_Mat );
        // s_ALabel = P_PLATECD[1];
        // 2017.08.17 �΍s���ǉ�_S
        //if (G_KS_Model.Pos("�c") > 0) { // �΍s��
        if (P_FL == 1) { // �΍s��
        // 2017.08.17 �΍s���ǉ�_E
            // �΍s��S3:A���̏���
            P_SideDiv = "A";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1),
                DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num,
                DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
            // �΍s��S4:B���̏���
            P_SideDiv = "B";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1),
                DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num,
                DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
        }
        else {
            // S3:B���̏���
            P_SideDiv = "B";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1),
                DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num,
                DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
            // S4:A���̏���
            P_SideDiv = "A";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1),
                DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num,
                DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
        }
        // *********************
        // �^���ƃR�[�h�̔�r
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B���ގ��̐��ʂŏo��
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // P_SideDiv ="";
            P_Suryo = DPL_Meq_Sub;
            P_Yobisu = DPL_Meq_Sub * P_PltGsktYobiD;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
    }
    // B�X�^�[�g
    else {
        // 2008.07.24 CX-10D�ǉ�
        ////S4:A���̏���
        // P_SideDiv ="A";
        // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(4,1), DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, s_A_GSKT_Mat );
        // s_BLabel = P_PLATECD[1];
        ////S3:B���̏���
        // P_SideDiv ="B";
        // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(3,1), DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, s_B_GSKT_Mat );
        // s_ALabel = P_PLATECD[1];
        // 2017.08.17 �΍s���ǉ�_S
        //if (G_KS_Model.Pos("�c") > 0) { // �΍s��
        if (P_FL == 1) { // �΍s��
        // 2017.08.17 �΍s���ǉ�_E
            // �΍s��S4:B���̏���
            P_SideDiv = "B";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1),
                DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num,
                DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
            // �΍s��S3:A���̏���
            P_SideDiv = "A";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1),
                DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num,
                DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
        }
        else {
            // S4:A���̏���
            P_SideDiv = "A";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1),
                DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num,
                DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
            // S3:B���̏���
            P_SideDiv = "B";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1),
                DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num,
                DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
        }
        // *********************
        // �^���ƃR�[�h�̔�r
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B���ގ��̐��ʂŏo��
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // P_SideDiv ="";
            P_Suryo = DPU_Meq_Sub;
            P_Yobisu = DPU_Meq_Sub * P_PltGsktYobiD;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
        // S2:A���̏���
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(2, 1), DP_B0s3_Code,
            DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code,
            DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, s_A_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // S1:B���̏���
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(1, 1), DP_B0s4_Code,
            DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code,
            DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, s_B_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // �^���ƃR�[�h�̔�r
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B���ގ��̐��ʂŏo��
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // P_SideDiv ="";
            P_Suryo = DPL_Meq_Sub;
            P_Yobisu = DPL_Meq_Sub * P_PltGsktYobiD;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
    }

    ////�p�^�[������ɂ��K�v�ȃK�X�P�b�g��I��
    ////A�X�^�[�g
    // if( G_D_GsktABKubun == "A" ){
    // //S1:B���̏���
    // P_SideDiv ="B";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(1,1), DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, P_HEADS_DATA[835].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S2:A���̏���
    // P_SideDiv ="A";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(2,1), DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, P_HEADS_DATA[836].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //�^���ƃR�[�h�̔�r
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B���ގ��̐��ʂŏo��
    // P_SideDiv ="";
    // P_Suryo = DPU_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = DPU_Meq_Sub * P_PltGsktYobiD;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // //S3:B���̏���
    // P_SideDiv ="B";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(3,1), DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, P_HEADS_DATA[835].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S4:A���̏���
    // P_SideDiv ="A";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(4,1), DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, P_HEADS_DATA[836].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //�^���ƃR�[�h�̔�r
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B���ގ��̐��ʂŏo��
    // P_SideDiv ="";
    // P_Suryo = DPL_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = DPL_Meq_Sub * P_PltGsktYobiD;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // }
    ////B�X�^�[�g
    // else{
    // //S4:A���̏���
    // P_SideDiv ="A";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(4,1), DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, P_HEADS_DATA[836].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S3:B���̏���
    // P_SideDiv ="B";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(3,1), DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, P_HEADS_DATA[835].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //�^���ƃR�[�h�̔�r
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B���ގ��̐��ʂŏo��
    // P_SideDiv ="";
    // P_Suryo = DPU_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = DPU_Meq_Sub * P_PltGsktYobiD;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // //S2:A���̏���
    // P_SideDiv ="A";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(2,1), DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, P_HEADS_DATA[836].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S1:B���̏���
    // P_SideDiv ="B";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(1,1), DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, P_HEADS_DATA[835].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //�^���ƃR�[�h�̔�r
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B���ގ��̐��ʂŏo��
    // P_SideDiv ="";
    // P_Suryo = DPL_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = DPL_Meq_Sub * P_PltGsktYobiD;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // }
    // **********************************
    // �f�B�X�^���X�s�[�X�o��
    Write_D_Gskt_Data_Distance();

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �c�v���[�g�K�X�P�b�g���i�@�ް��쐬 �T�u
// �T  �v      �F
// �F�@�@�@�@�@�@Ana �@ �@�F�E���f�[�^��S1,S2�E�E�̃f�[�^
// �F            PCode0   : �Ӗڕ��i����
// �F            PLabel0  : �Ӗڕ��i����
// �F            PName0   : �Ӗڕ��i��
// �F            PNum0    : �Ӗڕ��i����
// �F            PCode1   : �E�����i����
// �F            PLabel1  : �Ӗڕ��i����
// �F            PName1   : �Ӗڕ��i��
// �F            PNum1    : �Ӗڕ��i����
// �F            PMatCode : ���i�ގ�����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_D_Gskt_Data_Sub(AnsiString s_Ana,
    AnsiString PCode0, AnsiString PLabel0, AnsiString PName0, long PNum0,
    AnsiString PCode1, AnsiString PLabel1, AnsiString PName1, long PNum1,
    AnsiString PMatCode) {
    int ret;

    // �E�̏ꍇ
    if (s_Ana != "0") {
        // �}�X�^�����֐�
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "D", PCode1, PMatCode);
        if (ret == true) {
            // ���ʎq�Ɩ��̂Ɛ��ʂ��㏑������B
            P_PLATECD[1] = PLabel1;
            P_PLATECD[3] = PName1;
            P_Suryo = PNum1;
            // �o�͂���B
            P_Unit = "";
            // 2003.12.09 UX-90,UX-100,GX-20�ȊO��E��ڰĶ޽��Ă�NBR
            // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode1),PMatCode);
            // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
            // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
            if (PCode1 == "60" || PCode1 == "55" || PCode1 == "56" ||
                PCode1 == "57") { // �d�v���[�g�K�X�P�b�g�i�d�j�͍ގ����m�a�q�Ƃ���
                // 2013.11.06 UX-100�ގ��ύX
                // if ( G_KS_Syurui == "�t�w" && (G_KS_Model == "�X�O" || G_KS_Model == "�P�O�O") ){ // �d�v���[�g�K�X�P�b�gUX-90,100�͍ގ��ς��Ȃ�
                if (G_KS_Syurui == "�t�w" && G_KS_Model == "�X�O")
                { // �d�v���[�g�K�X�P�b�gUX-90�͍ގ��ς��Ȃ�
                    // *************************
                    // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                }
                else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�Q�O")
                { // �d�v���[�g�K�X�P�b�gGX-20�͍ގ��ς��Ȃ�
                    // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    // 2004.12.15 GX-22�޽��Đ}�Ԏ��
                    ////2004.11.22 GX-22�޽��Đ}�ԕύX
                    // if ( DD_Label == "DDP-22" ) {
                    // P_Zuban = "�d�R�S�W�P�O�V";     //�}��
                    // P_SZuban = "�d�R�S�W�P�O�V";    //��}�p�}��
                    // }
                    ////******************************
                    // ******************************
                    // 2006.05.31 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                }
                else if (G_KS_Syurui == "�r�w" && (G_KS_Model == "�X�O�l" ||
                    G_KS_Model == "�X�O�k")) { // �d�v���[�g�K�X�P�b�gSX-90M,90L�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                    // 2009.06.19 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                }
                else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�P�O�O�q")
                { // �d�v���[�g�K�X�P�b�gUX-100R�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                }
                else if (G_KS_Syurui == "�q�w" && G_KS_Model == "�P�O�O")
                { // �d�v���[�g�K�X�P�b�gRX-100�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                }
                else { // �d�v���[�g�K�X�P�b�g��L�^���ȊO�͍ގ����m�a�q�Ƃ���
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
            }
            else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�R�O" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gUX-30�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L�ǉ�_S
            else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�R�O�k" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gUX-30�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L�ǉ�_E
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�Q�O" &&
                (PCode1 == "105" || PCode1 == "106")) {
                // �c�v���[�g�K�X�P�b�gSX-20�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //2016.07.25 SX-30D�K�X�P�b�g�ύX
            //���ԃK�X�P�b�g����̍쐬����ߍގ��ύX����@�@
            //else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O" &&
            //    (PCode1 == "9" || PCode1 == "10")) {
            //    // �c�v���[�g�K�X�P�b�gSX-30�͍ގ��ϊ����Ȃ�
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //*******************************�@�@
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�S�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gSX-41�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�S�R" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gSX-43�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�V�O" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gSX-43�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // ****************************************
            }
            // 2016.04.28 CX-00�ǉ�
            // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_S
            //else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�O" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // �c�v���[�g�K�X�P�b�gCX-00�͍ގ��ϊ����Ȃ�
            else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gCX-01�͍ގ��ϊ����Ȃ�
            // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_S
            //else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�O" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // �c�v���[�g�K�X�P�b�gCXW-00�͍ގ��ϊ����Ȃ�
             else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gCXW-01�͍ގ��ϊ����Ȃ�
            // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // ********************
            // 2022.04.27 CX-01D�ǉ�_S
            else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gCX-01D�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.27 CX-01D�ǉ�_E
            //2016.08.01 SX-30/30S D�K�X�P�b�g�ύX
            //���ԃK�X�P�b�g����̍쐬����ߍގ��ύX����@
            //����������ȕύX���s��
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O�r" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //*******************************�@
            // 2017.08.17 YX-83�v���[�g���i�ǉ�_S
            else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R"  &&
                (PCode1 == "9" || PCode1 == "10")) {
                // �c�v���[�g�K�X�P�b�gYX-83�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2017.08.17 YX-83�v���[�g���i�ǉ�_E
            // 2023.07.11 GX-50�v���[�g���i�ǉ�_E
            else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�T�O"  &&
                PCode1 == "6") {
                // �c�b�v���[�g�K�X�P�b�gGX-50�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2023.07.11 GX-50�v���[�g���i�ǉ�_E
            else {
                // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                // ****************************************
                // 2004.12.15 GX-22�޽��Đ}�Ԏ��
                ////2004.11.22 GX-22�޽��Đ}�ԕύX
                // if ( DD_Label == "DDP-22" ) {
                // P_Zuban = "�d�R�S�W�P�O�V";     //�}��
                // P_SZuban = "�d�R�S�W�P�O�V";    //��}�p�}��
                // }
                ////******************************
                // ******************************
            }
            // ****************************************************
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(PMatCode); // HEARTS�p�ް�CSV�쐬
        }
        // �ӂ̏ꍇ
    }
    else {
        // �}�X�^�����֐�
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "D", PCode0, PMatCode);
        if (ret == true) {
            // ���ʎq�Ɩ��̂Ɛ��ʂ��㏑������B
            P_PLATECD[1] = PLabel0;
            P_PLATECD[3] = PName0;
            P_Suryo = PNum0;
            // �o�͂���B
            P_Unit = "";
            // 2003.12.09 UX-90,UX-100,GX-20�ȊO��E��ڰĶ޽��Ă�NBR
            // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode0),PMatCode);
            // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
            // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
            if (PCode0 == "60" || PCode0 == "55" || PCode0 == "56" ||
                PCode0 == "57") { // �d�v���[�g�K�X�P�b�g�i�d�j�͍ގ����m�a�q�Ƃ���
                // 2013.11.06 UX-100�ގ��ύX
                // if ( G_KS_Syurui == "�t�w" && (G_KS_Model == "�X�O" || G_KS_Model == "�P�O�O") ){ // �d�v���[�g�K�X�P�b�gUX-90,100�͍ގ��ς��Ȃ�
                if (G_KS_Syurui == "�t�w" && G_KS_Model == "�X�O")
                { // �d�v���[�g�K�X�P�b�gUX-90�͍ގ��ς��Ȃ�
                    // *************************
                    // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                }
                else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�Q�O")
                { // �d�v���[�g�K�X�P�b�gGX-20�͍ގ��ς��Ȃ�
                    // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    // 2004.12.15 GX-22�޽��Đ}�Ԏ��
                    ////2004.11.22 GX-22�޽��Đ}�ԕύX
                    // if ( DD_Label == "DDP-22" ) {
                    // P_Zuban = "�d�R�S�W�P�O�V";     //�}��
                    // P_SZuban = "�d�R�S�W�P�O�V";    //��}�p�}��
                    // }
                    ////******************************
                    // ******************************
                    // 2006.05.31 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                }
                else if (G_KS_Syurui == "�r�w" && (G_KS_Model == "�X�O�l" ||
                    G_KS_Model == "�X�O�k")) { // �d�v���[�g�K�X�P�b�gSX-90M,90L�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                    // 2009.06.19 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                }
                else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�P�O�O�q")
                { // �d�v���[�g�K�X�P�b�gUX-100R�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                }
                else if (G_KS_Syurui == "�q�w" && G_KS_Model == "�P�O�O")
                { // �d�v���[�g�K�X�P�b�gRX-100�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                }
                else { // �d�v���[�g�K�X�P�b�g��L�^���ȊO�͍ގ����m�a�q�Ƃ���
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
            }
            else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�R�O" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gUX-30�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L�ǉ�_S
            else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�R�O�k" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gUX-30�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L�ǉ�_E
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�Q�O" &&
                (PCode1 == "105" || PCode1 == "106")) {
                // �c�v���[�g�K�X�P�b�gSX-20�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //2016.07.25 SX-30D�K�X�P�b�g�ύX
            //���ԃK�X�P�b�g����̍쐬����ߍގ��ύX����@�@
            //else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O" &&
            //    (PCode1 == "9" || PCode1 == "10")) {
            //    // �c�v���[�g�K�X�P�b�gSX-30�͍ގ��ϊ����Ȃ�
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //*******************************�@�@
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�S�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gSX-41�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�S�R" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gSX-43�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�V�O" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gSX-43�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // ****************************************
            }
            // 2016.04.28 CX-00�ǉ�
            // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_S
            //else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�O" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // �c�v���[�g�K�X�P�b�gCX-00�͍ގ��ϊ����Ȃ�
            else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gCX-01�͍ގ��ϊ����Ȃ�
            // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_S
            //else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�O" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // �c�v���[�g�K�X�P�b�gCXW-00�͍ގ��ϊ����Ȃ�
            else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gCXW-01�͍ގ��ϊ����Ȃ�
            // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // ********************
            // 2022.04.27 CX-01D�ǉ�_S
            else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �c�v���[�g�K�X�P�b�gCX-01D�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.27 CX-01D�ǉ�_E
            //2016.08.01 SX-30/30S D�K�X�P�b�g�ύX
            //���ԃK�X�P�b�g����̍쐬����ߍގ��ύX����@
            //����������ȕύX���s��
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O�r" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //*******************************�@�@
            // 2017.08.17 YX-83�v���[�g���i�ǉ�_S
            else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R" &&
                (PCode1 == "9" || PCode1 == "10")) {
                // �c�v���[�g�K�X�P�b�gYX-83�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2017.08.17 YX-83�v���[�g���i�ǉ�_E
            // 2023.07.11 GX-50�v���[�g���i�ǉ�_S
            else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�T�O"  &&
                PCode1 == "6") {
                // �c�b�v���[�g�K�X�P�b�gGX-50�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2023.07.11 GX-50�v���[�g���i�ǉ�_E
            else {
                // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode0),PMatCode);
                // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                // ****************************************
                // 2004.12.15 GX-22�޽��Đ}�Ԏ��
                ////2004.11.22 GX-22�޽��Đ}�ԕύX
                // if ( DD_Label == "DDP-22" ) {
                // P_Zuban = "�d�R�S�W�P�O�V";     //�}��
                // P_SZuban = "�d�R�S�W�P�O�V";    //��}�p�}��
                // }
                ////******************************
                // ******************************
            }
            // ****************************************************
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(PMatCode); // HEARTS�p�ް�CSV�쐬
        }
    }

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �c�v���[�g�K�X�P�b�g�f�B�X�^���X�s�[�X���i�@�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_D_Gskt_Data_Distance(void) {
    int ret;

    // �}�X�^�����֐�(�ގ��R�[�h�͌Œ肾�����ȁE�E�E)
    ret = Search_HD_PLATE_3(P_SoSyoAscii, "D", DD_Code, "2019");
    if (ret == true) {
        // ���ʎq�Ɩ��̂Ɛ��ʂ��㏑������B
        P_PLATECD[1] = DD_Label;
        P_PLATECD[3] = DD_Name;
        P_Suryo = DD_Num;
        // 2004.11.22 GX-22�޽��Đ}�ԕύX
        if (DD_Label == "DDP-22") {
            P_Zuban = "�d�R�S�W�P�O�V"; // �}��
            P_SZuban = "�d�R�S�W�P�O�V"; // ��}�p�}��
        }
        // ******************************
        // �o�͂���B
        P_Unit = "��";
        // P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(DD_Code),"2019");
        P_Yobisu = Get_Gskt_YobiSu(DD_Code, "2019");
        P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
        Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        Write_HeartsBuhin_Data(2019); // HEARTS�p�ް�CSV�쐬
    }
    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �d�v���[�g�K�X�P�b�g���i�@�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_E_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    // 2014.07.14 ������
    ret = Search_HD_E_GASKET(G_E_GsktType, G_GsktType, G_FrmDiv);

    DE_Kubun = "E";

    // 2006.07.13 LX-10 TCG�p�ގ�����ǉ��y�сA���ގ��̐��ʂł�
    // HD_PARTS.CSV��DE_Kubun��P_SideDiv���L�q����B
    // (�߰�ؽĂŎg�p����)

    AnsiString s_B_GSKT_Mat;
    AnsiString s_A_GSKT_Mat;
    s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
    s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
    if (G_KS_Syurui == "�k�w" && G_KS_Model == "�P�O") { // LX-10
        if (s_B_GSKT_Mat == "2063" || s_A_GSKT_Mat == "2063") {
            // B��A�������ꂩ��2063(AT-TF/EPDM)�̏ꍇ����2063�ɂ���
            s_B_GSKT_Mat = "2063";
            s_A_GSKT_Mat = "2063";
        }
        //2017.10.10 AN-TF/EPDM�ǉ�_S
        else if (s_B_GSKT_Mat == "2064" || s_A_GSKT_Mat == "2064") {
            // B��A�������ꂩ��2064(AN-TF/EPDM)�̏ꍇ����2064�ɂ���
            s_B_GSKT_Mat = "2064";
            s_A_GSKT_Mat = "2064";
        }
        //2017.10.10 AN-TF/EPDM�ǉ�_E
    }

    // �p�^�[������ɂ��K�v�ȃK�X�P�b�g��I��
    // A�X�^�[�g
    if (G_E_GsktABKubun == "A") {
        // S1:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num, s_B_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // S2:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num, s_A_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // �^���ƃR�[�h�̔�r
        // �E�ɂ���r
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,
            1) != "0" && G_E_GsktAna.SubString(2,
            1) != "0" && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // E1�E2�E&A,B���ގ��̐��ʂŏo��
            P_Suryo = EPUH_Meq_Sub;
            P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B���ގ��̐��ʂŏo��
            P_Suryo = EPU_Meq_Sub;
            P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
        // 2008.07.24 CX-10D�ǉ�
        ////S3:B���̏���
        // P_SideDiv ="B";
        // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(3,1), EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num, s_B_GSKT_Mat );
        // s_BLabel = P_PLATECD[1];
        ////S4:A���̏���
        // P_SideDiv ="A";
        // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(4,1), EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num, s_A_GSKT_Mat );
        // s_ALabel = P_PLATECD[1];
        // 2017.08.17 �΍s���ǉ�_S
        //if (G_KS_Model.Pos("�c") > 0) { // �΍s��
        if (P_FL == 1) { // �΍s��
        // 2017.08.17 �΍s���ǉ�_E
            // �΍s��S3:A���̏���
            P_SideDiv = "A";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1),
                EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num,
                EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
            // �΍s��S4:B���̏���
            P_SideDiv = "B";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1),
                EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num,
                EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
        }
        else {
            // S3:B���̏���
            P_SideDiv = "B";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1),
                EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num,
                EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
            // S4:A���̏���
            P_SideDiv = "A";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1),
                EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num,
                EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
        }
        // *********************
        // �^���ƃR�[�h�̔�r
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,
            1) != "0" && G_E_GsktAna.SubString(4,
            1) != "0" && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // E3�E4�E&A,B���ގ��̐��ʂŏo��
            P_Suryo = EPLH_Meq_Sub;
            P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
        //2023/07/27 SX-10�K�X�P�b�g�W�J�C��_S
        if (G_KS_Syurui == "�r�w" && G_KS_Model == "�P�O" ) {
            if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
                // A,B���ގ��̐��ʂŏo��
                P_Suryo = EPU_Meq_Sub;
                P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
                if (P_Suryo == 0) {
                    P_SideDiv = "";
                }
                else {
                    P_SideDiv = "A";
                }
                Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
                Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
            }
        }
        //2023/07/27 SX-10�K�X�P�b�g�W�J�C��_E
    }
    // B�X�^�[�g
    else {
        // 2008.07.24 CX-10D�ǉ�
        ////S4:A���̏���
        // P_SideDiv ="A";
        // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(4,1), EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num, s_A_GSKT_Mat );
        // s_BLabel = P_PLATECD[1];
        ////S3:B���̏���
        // P_SideDiv ="B";
        // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(3,1), EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num, s_B_GSKT_Mat );
        // s_ALabel = P_PLATECD[1];
        // 2017.08.17 �΍s���ǉ�_S
        //if (G_KS_Model.Pos("�c") > 0) { // �΍s��
        if (P_FL == 1) { // �΍s��
        // 2017.08.17 �΍s���ǉ�_E
            // �΍s��S4:B���̏���
            P_SideDiv = "B";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1),
                EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num,
                EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
            // �΍s��S3:A���̏���
            P_SideDiv = "A";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1),
                EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num,
                EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
        }
        else {
            // S4:A���̏���
            P_SideDiv = "A";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1),
                EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num,
                EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
            // S3:B���̏���
            P_SideDiv = "B";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1),
                EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num,
                EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
        }
        // *********************
        // �^���ƃR�[�h�̔�r
        // �E�ɂ���r
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,
            1) != "0" && G_E_GsktAna.SubString(4,
            1) != "0" && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // E3�E4�E&A,B���ގ��̐��ʂŏo��
            P_Suryo = EPUH_Meq_Sub;
            P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B���ގ��̐��ʂŏo��
            P_Suryo = EPU_Meq_Sub;
            P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
        // S2:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num, s_A_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // S1:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num, s_B_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,
            1) != "0" && G_E_GsktAna.SubString(2,
            1) != "0" && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // E1�E2�E&A,B���ގ��̐��ʂŏo��
            P_Suryo = EPLH_Meq_Sub;
            P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
        }
        //2023/07/27 SX-10�K�X�P�b�g�W�J�C��_S
        if (G_KS_Syurui == "�r�w" && G_KS_Model == "�P�O" ) {
            if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
                // A,B���ގ��̐��ʂŏo��
                P_Suryo = EPU_Meq_Sub;
                P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
                if (P_Suryo == 0) {
                    P_SideDiv = "";
                }
                else {
                    P_SideDiv = "B";
                }
                Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
                Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS�p�ް�CSV�쐬
            }
        }
        //2023/07/27 SX-10�K�X�P�b�g�W�J�C��_E
    }

    ////�p�^�[������ɂ��K�v�ȃK�X�P�b�g��I��
    ////A�X�^�[�g
    // if( G_E_GsktABKubun == "A" ){
    // //S1:B���̏���
    // P_SideDiv ="B";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(1,1), EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num, P_HEADS_DATA[835].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S2:A���̏���
    // P_SideDiv ="A";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(2,1), EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num, P_HEADS_DATA[836].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //�^���ƃR�[�h�̔�r
    // //�E�ɂ���r
    // if( s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,1) != "0" && G_E_GsktAna.SubString(2,1) != "0" && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //E1�E2�E&A,B���ގ��̐��ʂŏo��
    // P_SideDiv ="";
    // P_Suryo = EPUH_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // P_SideDiv ="";
    // //A,B���ގ��̐��ʂŏo��
    // P_Suryo = EPU_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // //S3:B���̏���
    // P_SideDiv ="B";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(3,1), EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num, P_HEADS_DATA[835].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S4:A���̏���
    // P_SideDiv ="A";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(4,1), EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num, P_HEADS_DATA[836].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //�^���ƃR�[�h�̔�r
    // if( s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,1) != "0" && G_E_GsktAna.SubString(4,1) != "0" && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight() ){
    // //A,B���ގ��̐��ʂŏo��
    // P_SideDiv ="";
    // P_Suryo = EPLH_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // }
    ////B�X�^�[�g
    // else{
    // //S4:A���̏���
    // P_SideDiv ="A";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(4,1), EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num, P_HEADS_DATA[836].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S3:B���̏���
    // P_SideDiv ="B";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(3,1), EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num, P_HEADS_DATA[835].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //�^���ƃR�[�h�̔�r
    // //�E�ɂ���r
    // if( s_ALabel == s_BLabel &&  G_E_GsktAna.SubString(3,1) != "0" && G_E_GsktAna.SubString(4,1) != "0" && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //E1�E2�E&A,B���ގ��̐��ʂŏo��
    // P_SideDiv ="";
    // P_Suryo = EPUH_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B���ގ��̐��ʂŏo��
    // P_SideDiv ="";
    // P_Suryo = EPU_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // //S2:A���̏���
    // P_SideDiv ="A";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(2,1), EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num, P_HEADS_DATA[836].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S1:B���̏���
    // P_SideDiv ="B";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(1,1), EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num, P_HEADS_DATA[835].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // if( s_ALabel == s_BLabel &&  G_E_GsktAna.SubString(1,1) != "0" && G_E_GsktAna.SubString(2,1) != "0" && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B���ގ��̐��ʂŏo��
    // P_SideDiv ="";
    // P_Suryo = EPLH_Meq_Sub;
    // //2004.01.20 �\�����̎擾����
    // P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS�p�ް�CSV�쐬
    // }
    // }
    // **********************************

    // �f�B�X�^���X�s�[�X�o��
    Write_E_Gskt_Data_Distance();

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �d�v���[�g�K�X�P�b�g���i�@�ް��쐬 �T�u
// �T  �v      �F
// �F�@�@�@�@�@�@Ana �@ �@�F�E���f�[�^��S1,S2�E�E�̃f�[�^
// �F            PCode0   : �Ӗڕ��i����
// �F            PLabel0  : �Ӗڕ��i����
// �F            PName0   : �Ӗڕ��i��
// �F            PNum0    : �Ӗڕ��i����
// �F            PCode1   : �E�����i����
// �F            PLabel1  : �Ӗڕ��i����
// �F            PName1   : �Ӗڕ��i��
// �F            PNum1    : �Ӗڕ��i����
// �F            PMatCode : ���i�ގ�����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_E_Gskt_Data_Sub(AnsiString s_Ana,
    AnsiString PCode0, AnsiString PLabel0, AnsiString PName0, long PNum0,
    AnsiString PCode1, AnsiString PLabel1, AnsiString PName1, long PNum1,
    AnsiString PMatCode) {
    int ret;

    // �E�̏ꍇ
    if (s_Ana != "0") {
        // �}�X�^�����֐�
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "E", PCode1, PMatCode);
        if (ret == true) {
            // ���ʎq�Ɩ��̂Ɛ��ʂ��㏑������B
            P_PLATECD[1] = PLabel1;
            P_PLATECD[3] = PName1;
            P_Suryo = PNum1;
            // �o�͂���B
            P_Unit = "";
            if (PCode1 == "79") { // �d�m�Y���K�X�P�b�g�͍ގ���ϊ�����
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 1);
                P_Yobisu = Syukei_Buhin_Yobisu("GSKT04", P_Zaisitu,
                    m_pcHeadsData);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // 2006.07.13 YX-80E�ʘH�߯�ݸޒǉ�
            }
            // 2019.03.18 YX-80�ύX_S
            //else if (PCode1 == "100") { // �d�ʘH�p�b�L���O�͍ގ���ϊ�����
            //    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 1);
            //    P_Yobisu = Syukei_Buhin_Yobisu("GSKT59", P_Zaisitu,
            //        m_pcHeadsData);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //    // ********************************
            //    // 2003.12.09 UX-90,UX-100,GX-20�ȊO��E��ڰĶ޽��Ă�NBR
            //    // } else if (PCode1 == "60" ) { // �d�v���[�g�K�X�P�b�g�i�d�j�͍ގ����m�a�q�Ƃ���
            //    // P_Zaisitu = Search_HD_MATERIAL("2019");
            //    // P_EZaisitu = Search_HD_E_MATERIAL("2019");
            //    // //     P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode1),"2019");
            //    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),"2019");
            //    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // �K�X�P�b�g�t���v���[�g�̗\���i����
            //}
            // 2019.03.18 YX-80�ύX_E
            else if (PCode1 == "60" || PCode1 == "55" || PCode1 == "56" ||
                PCode1 == "57") { // �d�v���[�g�K�X�P�b�g�i�d�j�͍ގ����m�a�q�Ƃ���
                // 2013.11.06 UX-100�ގ��ύX
                // if ( G_KS_Syurui == "�t�w" && (G_KS_Model == "�X�O" || G_KS_Model == "�P�O�O") ){ // �d�v���[�g�K�X�P�b�gUX-90,100�͍ގ��ς��Ȃ�
                if (G_KS_Syurui == "�t�w" && G_KS_Model == "�X�O")
                { // �d�v���[�g�K�X�P�b�gUX-90�͍ގ��ς��Ȃ�
                    // *************************
                    // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                }
                // 2022.04.12 LX-50 E�޽��čގ�G-NBR�ɕύX_S
                else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�T�O")
                { // �d�v���[�g�K�X�P�b�gLX-50�͍ގ����f�|�m�a�q�Ƃ���
                    P_Zaisitu = Search_HD_MATERIAL("2075");
                    P_EZaisitu = Search_HD_E_MATERIAL("2075");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2075");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                // 2022.04.12 LX-50 E�޽��čގ�G-NBR�ɕύX_E
                else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�Q�O")
                { // �d�v���[�g�K�X�P�b�gGX-20�͍ގ��ς��Ȃ�
                    // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    // 2004.12.15 GX-22�޽��Đ}�Ԏ��
                    ////2004.11.22 GX-22�޽��Đ}�ԕύX
                    // if ( ED_Label == "EDPC22" ) {
                    // P_Zuban = "�d�R�S�W�P�O�V";     //�}��
                    // P_SZuban = "�d�R�S�W�P�O�V";    //��}�p�}��
                    // }
                    ////******************************
                    // ******************************
                    // 2006.05.31 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                }
                else if (G_KS_Syurui == "�r�w" && (G_KS_Model == "�X�O�l" ||
                    G_KS_Model == "�X�O�k")) { // �d�v���[�g�K�X�P�b�gSX-90M,90L�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                    // 2009.06.19 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                }
                else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�P�O�O�q")
                { // �d�v���[�g�K�X�P�b�gUX-100R�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                }
                else if (G_KS_Syurui == "�q�w" && G_KS_Model == "�P�O�O")
                { // �d�v���[�g�K�X�P�b�gRX-100�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                }
                else { // �d�v���[�g�K�X�P�b�g��L�^���ȊO�͍ގ����m�a�q�Ƃ���
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                // ****************************************************
                // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
            }
            else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�R�O" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gUX-30�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L�ǉ�_S
            else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�R�O�k" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gUX-30�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L�ǉ�_E
            // 2017.07.18 SX-80E�K�X�P�b�g�ǉ�
            else if (PCode1 == "108" || PCode1 == "109" ) {
                if (G_KS_Syurui == "�r�w" && (G_KS_Model == "�W�O�r" ||
                    G_KS_Model == "�W�O�l" || G_KS_Model == "�W�O�k")) { // �d�v���[�g�K�X�P�b�gSX-80S,80M,80L�͍ގ�GZ-NBR
                    P_Zaisitu = Search_HD_MATERIAL("2067");
                    P_EZaisitu = Search_HD_E_MATERIAL("2067");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2067");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                else { // �d�v���[�g�K�X�P�b�g��L�^���ȊO�͍ގ����m�a�q�Ƃ���
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
            }
            // *******************************�@�@
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�Q�O" &&
                (PCode1 == "105" || PCode1 == "106")) {
                // �d�v���[�g�K�X�P�b�gSX-20�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //2016.07.25 SX-30D�K�X�P�b�g�ύX
            //���ԃK�X�P�b�g����̍쐬����ߍގ��ύX����@�@
            //else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O" &&
            //    (PCode1 == "9" || PCode1 == "10")) {
            //    // �d�v���[�g�K�X�P�b�gSX-30�͍ގ��ϊ����Ȃ�
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //*******************************
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�S�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gSX-41�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�S�R" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gSX-43�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�V�O" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gSX-43�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // ****************************************
            }
            // 2016.04.28 CX-00�ǉ�
            // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_S
            //else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�O" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // �d�v���[�g�K�X�P�b�gCX-00�͍ގ��ϊ����Ȃ�
            else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gCX-01�͍ގ��ϊ����Ȃ�
            // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_S
            //else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�O" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //      // �d�v���[�g�K�X�P�b�gCXW-00�͍ގ��ϊ����Ȃ�
            else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gCXW-01�͍ގ��ϊ����Ȃ�
            // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // ********************
            // 2022.04.08 CX-01D�ǉ�_S
            else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gCX-01D�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.27 CX-01D�ǉ�_E
            //2016.08.01 SX-30/30S D�K�X�P�b�g�ύX
            //���ԃK�X�P�b�g����̍쐬����ߍގ��ύX����@
            //����������ȕύX���s��
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O�r" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //*******************************�@
            // 2017.08.23 YX-83�v���[�g���i�ǉ�_S
            else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R" &&
                (PCode1 == "9" || PCode1 == "10" ||PCode1 == "110" || PCode1 == "111")) {
                // �d�v���[�g�K�X�P�b�gYX-83�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2017.08.23 YX-83�v���[�g���i�ǉ�_E
            // 2023.07.11 GX-50�v���[�g���i�ǉ�_S
            else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�T�O" &&
                PCode1 == "6") {
                // �c�b�v���[�g�K�X�P�b�gGX-50�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2023.07.11 GX-50�v���[�g���i�ǉ�_E
            else {
                // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode1),PMatCode);
                // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                // ****************************************
                // 2004.12.15 GX-22�޽��Đ}�Ԏ��
                ////2004.11.22 GX-22�޽��Đ}�ԕύX
                // if ( ED_Label == "EDPC22" ) {
                // P_Zuban = "�d�R�S�W�P�O�V";     //�}��
                // P_SZuban = "�d�R�S�W�P�O�V";    //��}�p�}��
                // }
                ////******************************
                // ******************************
            }
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(PMatCode); // HEARTS�p�ް�CSV�쐬
        }
        // �ӂ̏ꍇ
    }
    else {
        // �}�X�^�����֐�
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "E", PCode0, PMatCode);
        if (ret == true) {
            // ���ʎq�Ɩ��̂Ɛ��ʂ��㏑������B
            P_PLATECD[1] = PLabel0;
            P_PLATECD[3] = PName0;
            P_Suryo = PNum0;
            if (PCode0 == "79") { // �d�m�Y���K�X�P�b�g�͍ގ���ϊ�����
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 1);
                P_Yobisu = Syukei_Buhin_Yobisu("GSKT04", P_Zaisitu,
                    m_pcHeadsData);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (PCode0 == "60" || PCode0 == "55" || PCode0 == "56" ||
                PCode0 == "57") { // �d�v���[�g�K�X�P�b�g�i�d�j�͍ގ����m�a�q�Ƃ���
                // 2003.12.09 UX-90,UX-100,GX-20��NBR�ł͂Ȃ�
                // P_Zaisitu = Search_HD_MATERIAL("2019");
                // P_EZaisitu = Search_HD_E_MATERIAL("2019");
                // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode0),"2019");
                // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),"2019");
                // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                // 2013.11.06 UX-100�ގ��ύX
                // if ( G_KS_Syurui == "�t�w" && (G_KS_Model == "�X�O" || G_KS_Model == "�P�O�O") ){ // �d�v���[�g�K�X�P�b�gUX-90,100�͍ގ��ς��Ȃ�
                if (G_KS_Syurui == "�t�w" && G_KS_Model == "�X�O")
                { // �d�v���[�g�K�X�P�b�gUX-90�͍ގ��ς��Ȃ�
                    // *************************
                    // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                }
                // 2022.04.12 LX-50 E�޽��čގ�G-NBR�ɕύX_S
                else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�T�O")
                { // �d�v���[�g�K�X�P�b�gLX-50�͍ގ����f�|�m�a�q�Ƃ���
                    P_Zaisitu = Search_HD_MATERIAL("2075");
                    P_EZaisitu = Search_HD_E_MATERIAL("2075");
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), "2075");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                // 2022.04.12 LX-50 E�޽��čގ�G-NBR�ɕύX_E
                else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�Q�O")
                { // �d�v���[�g�K�X�P�b�gGX-20�͍ގ��ς��Ȃ�
                    // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    // 2004.12.15 GX-22�޽��Đ}�Ԏ��
                    ////2004.11.22 GX-22�޽��Đ}�ԕύX
                    // if ( ED_Label == "EDPC22" ) {
                    // P_Zuban = "�d�R�S�W�P�O�V";     //�}��
                    // P_SZuban = "�d�R�S�W�P�O�V";    //��}�p�}��
                    // }
                    ////******************************
                    // ******************************
                    // 2006.05.31 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                }
                else if (G_KS_Syurui == "�r�w" && (G_KS_Model == "�X�O�l" ||
                    G_KS_Model == "�X�O�k")) { // �d�v���[�g�K�X�P�b�gSX-90M,90L�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                    // 2009.06.19 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                }
                else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�P�O�O�q")
                { // �d�v���[�g�K�X�P�b�gUX-100R�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                }
                else if (G_KS_Syurui == "�q�w" && G_KS_Model == "�P�O�O")
                { // �d�v���[�g�K�X�P�b�gRX-100�͍ގ��ς��Ȃ�
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                }
                else { // �d�v���[�g�K�X�P�b�g��L�^���ȊO�͍ގ����m�a�q�Ƃ���
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                }
                // ******************************************
                // 2014.07.08 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
            }
            else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�R�O" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gUX-30�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L�ǉ�_S
            else if (G_KS_Syurui == "�t�w" && G_KS_Model == "�R�O�k" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gUX-30�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L�ǉ�_E
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�Q�O" &&
                (PCode1 == "105" || PCode1 == "106")) {
                // �d�v���[�g�K�X�P�b�gSX-20�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //2016.07.25 SX-30D�K�X�P�b�g�ύX
            //���ԃK�X�P�b�g����̍쐬����ߍގ��ύX����@�@
            //else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O" &&
            //    (PCode1 == "9" || PCode1 == "10")) {
            //    // �d�v���[�g�K�X�P�b�gSX-30�͍ގ��ϊ����Ȃ�
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //*******************************
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�S�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gSX-41�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�S�R" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gSX-43�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�V�O" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gSX-43�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // ****************************************
            }
            // 2016.04.28 CX-00�ǉ�
            // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_S
            //else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�O" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // �d�v���[�g�K�X�P�b�gCX-00�͍ގ��ϊ����Ȃ�
            else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gCX-01�͍ގ��ϊ����Ȃ�
            // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_S
            //else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�O" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // �d�v���[�g�K�X�P�b�gCXW-00�͍ގ��ϊ����Ȃ�
            else if (G_KS_Syurui == "�b�w�v" && G_KS_Model == "�O�P" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gCXW-01�͍ގ��ϊ����Ȃ�
            // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // ********************
            // 2022.04.27 CX-01D�ǉ�_S
            else if (G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // �d�v���[�g�K�X�P�b�gCX-01D�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.27 CX-01D�ǉ�_E
            //2016.08.01 SX-30/30S D�K�X�P�b�g�ύX
            //���ԃK�X�P�b�g����̍쐬����ߍގ��ύX����@
            //����������ȕύX���s��
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�R�O�r" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //*******************************
            // 2020.03.04 YX-83�C��_S
            //// 2017.08.23 YX-83�v���[�g���i�ǉ�_S
            //else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R" &&
            //    (PCode1 == "9" || PCode1 == "10" || PCode1 == "110" || PCode1 == "111")) {
            //    // �d�v���[�g�K�X�P�b�gYX-83�͍ގ��ϊ����Ȃ�
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //// 2017.08.23 YX-83�v���[�g���i�ǉ�_E
            else if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R" &&
                (PCode1 == "9" || PCode1 == "110" || PCode1 == "111")) {
                // �c�`�v���[�g�K�X�P�b�gYX-83(��)�͍ގ����m�a�q�Ƃ���
                // �d�a�P�v���[�g�K�X�P�b�gYX-83(��)�͍ގ����m�a�q�Ƃ���
                // �d�a�Q�v���[�g�K�X�P�b�gYX-83(��)�͍ގ����m�a�q�Ƃ���
                P_Zaisitu = Search_HD_MATERIAL("2019");
                P_EZaisitu = Search_HD_E_MATERIAL("2019");
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
            }
            // 2020.03.04 YX-83�C��_E
            // 2023.07.11 GX-50�v���[�g���i�ǉ�_S
            else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�T�O" &&
                PCode1 == "6") {
                // �c�b�v���[�g�K�X�P�b�gGX-50�͍ގ��ϊ����Ȃ�
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2023.07.11 GX-50�v���[�g���i�ǉ�_E
            else {
                // 2004.06.09 DE��ڰĶ޽��čގ����ޕϊ��ǉ�
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode0),PMatCode);
                // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // �K�X�P�b�g�t���v���[�g�̗\���i����
                // ****************************************
                // 2004.12.15 GX-22�޽��Đ}�Ԏ��
                ////2004.11.22 GX-22�޽��Đ}�ԕύX
                // if ( ED_Label == "EDPC22" ) {
                // P_Zuban = "�d�R�S�W�P�O�V";     //�}��
                // P_SZuban = "�d�R�S�W�P�O�V";    //��}�p�}��
                // }
                ////******************************
                // ******************************
            }
            // �o�͂���B
            P_Unit = "";
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(PMatCode); // HEARTS�p�ް�CSV�쐬
        }
    }

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �d�v���[�g�K�X�P�b�g�f�B�X�^���X�s�[�X���i�@�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_E_Gskt_Data_Distance(void) {
    int ret;
    // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_S
    AnsiString ED_Name;
    AnsiString ED_Label;
    long ED_Code;
    long ED_Num;
    // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_E

    // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_S
    // �d�v���[�g�E�����f
    if (G_E_GsktAna.SubString(1, 4) == "0000") {
        ED_Name = ED_B0_Name;
        ED_Label = ED_B0_Label;
        ED_Code = ED_B0_Code;
        ED_Num = ED_B0_Num;
    } else {
        ED_Name = ED_B1_Name;
        ED_Label = ED_B1_Label;
        ED_Code = ED_B1_Code;
        ED_Num = ED_B1_Num;
    }
    // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_E

    // �}�X�^�����֐�(�ގ��R�[�h�͌Œ肾�����ȁE�E�E)
    ret = Search_HD_PLATE_3(P_SoSyoAscii, "E", ED_Code, "2019");
    if (ret == true) {
        // ���ʎq�Ɩ��̂Ɛ��ʂ��㏑������B
        P_PLATECD[1] = ED_Label;
        P_PLATECD[3] = ED_Name;
        P_Suryo = ED_Num;
        // 2004.11.22 GX-22�޽��Đ}�ԕύX
        if (ED_Label == "EDPC22") {
        // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_E
            P_Zuban = "�d�R�S�W�P�O�V"; // �}��
            P_SZuban = "�d�R�S�W�P�O�V"; // ��}�p�}��
        }
        // ******************************
        // �o�͂���B
        P_Unit = "��";
        // P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(ED_Code),"2019");
        P_Yobisu = Get_Gskt_YobiSu(ED_Code, "2019");
        P_Yobisu += P_Suryo * P_PltGsktYobiE; // �K�X�P�b�g�t���v���[�g�̗\���i����
        Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        Write_HeartsBuhin_Data(2019); // HEARTS�p�ް�CSV�쐬
    }

    return ret;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �K�X�P�b�g�}�X�^�̌����L�[���O���ϐ��֊i�[
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Get_Gskt_MastKey(int DEmode) {

    AnsiString s_Text;

    //
    // �ڰ�����
    //
    G_FrmType = G_S_Model;

    //
    // �޽��Ď��
    //
    // �E�[�̈ꕶ�������
    if ((DEmode == 0 && (G_D_GsktType.SubString(1, 5) == "UX-31") ||
        (G_D_GsktType.SubString(1, 5) == "UX-32")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 5) == "UX-31") ||
        (G_E_GsktType.SubString(1, 5) == "UX-32"))) {
        s_Text = P_HEADS_DATA[35].TrimRight().SubString
            (P_HEADS_DATA[35].Length(), 1);
        G_GsktType = s_Text.SubString(s_Text.Length() - 1, 1);
        if (G_GsktType.IsDelimiter("A", 1) == false)
            G_GsktType = "N";
            // 2022.04.19 UX-30L�ǉ�_S
            if ((DEmode == 0 && (G_D_GsktType.SubString(1, 6) == "UX-31L") ||
                (G_D_GsktType.SubString(1, 6) == "UX-32L")) ||
                (DEmode == 1 && (G_E_GsktType.SubString(1, 6) == "UX-31L") ||
                (G_E_GsktType.SubString(1, 6) == "UX-32L"))) {
                G_GsktType = "-";
            }
            // 2022.04.19 UX-30L�ǉ�_E
        // 2006.07.13 LX-10 TCG�p�ގ�����ǉ�
    }
    else if ((DEmode == 0 && (G_D_GsktType.SubString(1, 5) == "LX-11") ||
        (G_D_GsktType.SubString(1, 5) == "LX-12")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 5) == "LX-11") ||
        (G_E_GsktType.SubString(1, 5) == "LX-12"))) {
        AnsiString s_B_GSKT_Mat;
        AnsiString s_A_GSKT_Mat;
        s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
        s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
        if (s_B_GSKT_Mat == "2063" || s_A_GSKT_Mat == "2063") {
            // B��A�������ꂩ��2063(AT-TF/EPDM)�̏ꍇ
            G_GsktType = "T";
            // 2010.02.10 LX-10 2064(AN-TF/EPDM)�ǉ�
        }
        else if (s_B_GSKT_Mat == "2064" || s_A_GSKT_Mat == "2064") {
            // B��A�������ꂩ��2064(AN-TF/EPDM)�̏ꍇ
            G_GsktType = "T";
            // *************************************
        }
        else {
            G_GsktType = "-";
        }
        // **********************************
    }
    // 2017.07.18 SX-80 GZ-NBR�p�ގ�����ǉ�
    else if (DEmode == 1 && (G_E_GsktType.SubString(1, 4) == "SX-8")) {
        AnsiString s_B_GSKT_Mat;
        AnsiString s_A_GSKT_Mat;
        s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
        s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
        if (s_B_GSKT_Mat == "2067" || s_B_GSKT_Mat == "2068") {
            // B����2067(GZ-NBR)�A2068(AGZ-NBR)�̏ꍇ
            if (s_A_GSKT_Mat == "2067" || s_A_GSKT_Mat == "2068") {
                // A��B����2067(GZ-NBR)�A2068(AGZ-NBR)�̏ꍇ
                G_GsktType = "GZ-NBR";
            }
            else {
                // B���݂̂�2067(GZ-NBR)�A2068(AGZ-NBR)�̏ꍇ
                G_GsktType = "GZ-NBRB";
            }
        }
        else  if (s_A_GSKT_Mat == "2067" || s_A_GSKT_Mat == "2068") {
            // A���݂̂�2067(GZ-NBR)�A2068(AGZ-NBR)�̏ꍇ
            G_GsktType = "GZ-NBRA";
        }
        else {
            G_GsktType = "-";
        }
    }
    // *************************************
    else {
        // �R�[�h�̍Ō�̕�����"A"�ȊO�Ȃ�s�v
        if (G_GsktType.IsDelimiter("A", 1) == false)
            G_GsktType = "-";
    }

    //
    // �ڰы敪
    //
    // 36�Ԗڂ̐擪�ꌅ
    s_Text = P_HEADS_DATA[36].TrimRight().SubString(1, 1);
    AnsiString s_FType;
    G_FrmDiv = "-";
    if ((DEmode == 0 && (G_D_GsktType.SubString(1, 5) == "LX-11") ||
        (G_D_GsktType.SubString(1, 5) == "LX-12")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 5) == "LX-11") ||
        (G_E_GsktType.SubString(1, 5) == "LX-12"))) {
        if (s_Text.SubString(1, 1).IsDelimiter("KT", 1) == true) {
            G_FrmDiv = "K";
        }
        else if (s_Text.SubString(1, 1) == 'N') {
            G_FrmDiv = "N";
            // 2005.02.23 LX-10B�ǉ�
        }
        else {
            G_FrmDiv = "K";
            // *********************
        }
    }

    // 2006.01.10 WX�p��ڰĂ��ڰы敪�̐ݒ�
    int iSuryo;
    int iMod;
    // ���[�̂Q����
    if ((DEmode == 0 && (G_D_GsktType.SubString(1, 2) == "WX")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 2) == "WX"))) {
        iSuryo = int(P_HEADS_DATA[38].ToIntDef(0));
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // ���� �d�J�Z�b�g
            G_FrmDiv = "C";
        }
        else {
            // � �d�v���[�g
            G_FrmDiv = "P";
        }
    }
    // *****************************
    // 2021.09.22 WH-30�ǉ�_S
    // ���[�̂Q����
    if ((DEmode == 0 && (G_D_GsktType.SubString(1, 2) == "WH")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 2) == "WH"))) {
        iSuryo = int(P_HEADS_DATA[38].ToIntDef(0));
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // ���� �d�J�Z�b�g
            G_FrmDiv = "C";
        }
        else {
            // � �d�v���[�g
            G_FrmDiv = "P";
        }
    }
    // 2021.09.22 WH-30�ǉ�_E

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ڰĕ��i�\��Ͻ��E�ގ�����Ͻ������i��ڰĕ��i �ް��쐬�p�j
// �T  �v      �F
// ��  ��      �F AnsiString Key      : ��ڰČ^��
// �F            PType    : ��ڰĎ��
// �F            PCode    : ���i����
// �F            PMatCode : ���i�ގ�����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Search_HD_PLATE_3(AnsiString Key,
    AnsiString PType, AnsiString PCode, AnsiString PMatCode) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PLATE";
    s_SQL = s_SQL + "  WHERE MODEL_CODE = '" + Key + "'";
    // s_SQL = s_SQL + "    AND PLATE_TYPE = '" + PType + "'";
    s_SQL = s_SQL + "    AND PARTS_CODE = " + PCode + "";

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
        P_Zaisitu = "";
        P_EZaisitu = "";
        P_Suryo = 0;

        delete wkQuery;
        return false;
        // return true;
    }
    else {
        P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_MAT_CODE1 = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
        P_Suryo = wkQuery->FieldByName("NUMS")->AsString.ToIntDef(0);
        P_Zaisitu = Search_HD_MATERIAL(PMatCode);
        P_EZaisitu = Search_HD_E_MATERIAL(PMatCode);

        // // �ގ�����Ͻ�����
        // s_SQL = "";
        // s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
        // s_SQL = s_SQL + "  WHERE MAT_CODE = '" + PMatCode + "'";
        //
        // wkQuery->Close();
        // wkQuery->DatabaseName = ODBC_DSN_MAT;
        // wkQuery->SQL->Clear();
        // wkQuery->SQL->Add(s_SQL);
        // wkQuery->Open();
        // wkQuery->First();
        //
        // if(wkQuery->Eof){
        // P_Zaisitu = "";
        // G_ErrLog_Text = "�ގ����ށw" + PMatCode + "�x�͑��݂��܂���I�I";
        // Write_Error_Log(G_ErrLog_Text);
        // } else {
        // if(P_HEADS_DATA[1].TrimRight() == "0" ){
        // // �a�����̂̎擾
        // P_Zaisitu = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
        // } else {
        // P_Zaisitu = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
        // }
        // }
        // wkQuery->Close();
        //
        // delete wkQuery;

        // �\�����擾
        // P_Yobisu = Search_GSKT_Yobi(PCode, P_MAT_CODE1);

        return true;

    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F D�޽���Ͻ�����
// �T  �v      �F
// ��  ��      �F AnsiString FRM_TYPE  : ��ڰČ^��
// �F            GSKT_TYPE : ��ڰĎ��
// �F            FRM_DIV   : ���i����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Search_HD_D_GASKET(AnsiString FRM_TYPE,
    AnsiString GSKT_TYPE, AnsiString FRM_DIV) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_D_GASKET";
    s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + FRM_TYPE + "'";
    s_SQL = s_SQL + "    AND GSKT_TYPE = '" + GSKT_TYPE + "'";
    s_SQL = s_SQL + "    AND FRM_DIV = '" + FRM_DIV + "'";

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        DP_B1s1_Name = "";
        DP_B1s1_Label = "";
        DP_B1s1_Code = 0;
        DP_B1s1_Num = 0;
        DP_B0s1_Name = "";
        DP_B0s1_Label = "";
        DP_B0s1_Code = 0;
        DP_B0s1_Num = 0;
        DP_B1s2_Name = "";
        DP_B1s2_Label = "";
        DP_B1s2_Code = 0;
        DP_B1s2_Num = 0;
        DP_B0s2_Name = "";
        DP_B0s2_Label = "";
        DP_B0s2_Code = 0;
        DP_B0s2_Num = 0;
        DPU_Meq_Sub = 0;
        DP_B1s3_Name = "";
        DP_B1s3_Label = "";
        DP_B1s3_Code = 0;
        DP_B1s3_Num = 0;
        DP_B0s3_Name = "";
        DP_B0s3_Label = "";
        DP_B0s3_Code = 0;
        DP_B0s3_Num = 0;
        DP_B1s4_Name = "";
        DP_B1s4_Label = "";
        DP_B1s4_Code = 0;
        DP_B1s4_Num = 0;
        DP_B0s4_Name = "";
        DP_B0s4_Label = "";
        DP_B0s4_Code = 0;
        DP_B0s4_Num = 0;
        DPL_Meq_Sub = 0;
        DD_Name = "";
        DD_Label = "";
        DD_Code = 0;
        DD_Num = 0;

        delete wkQuery;
        // return false;
        return true;
    }
    else {

        DP_B1s1_Name = wkQuery->FieldByName("DP_B1S1_NAME")
            ->AsString.TrimRight();
        DP_B1s1_Label = wkQuery->FieldByName("DP_B1S1_LABEL")
            ->AsString.TrimRight();
        DP_B1s1_Code = wkQuery->FieldByName("DP_B1S1_CODE")
            ->AsString.ToIntDef(0);
        DP_B1s1_Num = wkQuery->FieldByName("DP_B1S1_NUM")->AsString.ToIntDef(0);
        DP_B0s1_Name = wkQuery->FieldByName("DP_B0S1_NAME")
            ->AsString.TrimRight();
        DP_B0s1_Label = wkQuery->FieldByName("DP_B0S1_LABEL")
            ->AsString.TrimRight();
        DP_B0s1_Code = wkQuery->FieldByName("DP_B0S1_CODE")
            ->AsString.ToIntDef(0);
        DP_B0s1_Num = wkQuery->FieldByName("DP_B0S1_NUM")->AsString.ToIntDef(0);
        DP_B1s2_Name = wkQuery->FieldByName("DP_B1S2_NAME")
            ->AsString.TrimRight();
        DP_B1s2_Label = wkQuery->FieldByName("DP_B1S2_LABEL")
            ->AsString.TrimRight();
        DP_B1s2_Code = wkQuery->FieldByName("DP_B1S2_CODE")
            ->AsString.ToIntDef(0);
        DP_B1s2_Num = wkQuery->FieldByName("DP_B1S2_NUM")->AsString.ToIntDef(0);
        DP_B0s2_Name = wkQuery->FieldByName("DP_B0S2_NAME")
            ->AsString.TrimRight();
        DP_B0s2_Label = wkQuery->FieldByName("DP_B0S2_LABEL")
            ->AsString.TrimRight();
        DP_B0s2_Code = wkQuery->FieldByName("DP_B0S2_CODE")
            ->AsString.ToIntDef(0);
        DP_B0s2_Num = wkQuery->FieldByName("DP_B0S2_NUM")->AsString.ToIntDef(0);
        DPU_Meq_Sub = wkQuery->FieldByName("DPU_MEQ_SUB")->AsString.ToIntDef(0);
        DP_B1s3_Name = wkQuery->FieldByName("DP_B1S3_NAME")
            ->AsString.TrimRight();
        DP_B1s3_Label = wkQuery->FieldByName("DP_B1S3_LABEL")
            ->AsString.TrimRight();
        DP_B1s3_Code = wkQuery->FieldByName("DP_B1S3_CODE")
            ->AsString.ToIntDef(0);
        DP_B1s3_Num = wkQuery->FieldByName("DP_B1S3_NUM")->AsString.ToIntDef(0);
        DP_B0s3_Name = wkQuery->FieldByName("DP_B0S3_NAME")
            ->AsString.TrimRight();
        DP_B0s3_Label = wkQuery->FieldByName("DP_B0S3_LABEL")
            ->AsString.TrimRight();
        DP_B0s3_Code = wkQuery->FieldByName("DP_B0S3_CODE")
            ->AsString.ToIntDef(0);
        DP_B0s3_Num = wkQuery->FieldByName("DP_B0S3_NUM")->AsString.ToIntDef(0);
        DP_B1s4_Name = wkQuery->FieldByName("DP_B1S4_NAME")
            ->AsString.TrimRight();
        DP_B1s4_Label = wkQuery->FieldByName("DP_B1S4_LABEL")
            ->AsString.TrimRight();
        DP_B1s4_Code = wkQuery->FieldByName("DP_B1S4_CODE")
            ->AsString.ToIntDef(0);
        DP_B1s4_Num = wkQuery->FieldByName("DP_B1S4_NUM")->AsString.ToIntDef(0);
        DP_B0s4_Name = wkQuery->FieldByName("DP_B0S4_NAME")
            ->AsString.TrimRight();
        DP_B0s4_Label = wkQuery->FieldByName("DP_B0S4_LABEL")
            ->AsString.TrimRight();
        DP_B0s4_Code = wkQuery->FieldByName("DP_B0S4_CODE")
            ->AsString.ToIntDef(0);
        DP_B0s4_Num = wkQuery->FieldByName("DP_B0S4_NUM")->AsString.ToIntDef(0);
        DPL_Meq_Sub = wkQuery->FieldByName("DPL_MEQ_SUB")->AsString.ToIntDef(0);
        DD_Name = wkQuery->FieldByName("DD_NAME")->AsString.TrimRight();
        DD_Label = wkQuery->FieldByName("DD_LABEL")->AsString.TrimRight();
        DD_Code = wkQuery->FieldByName("DD_CODE")->AsString.ToIntDef(0);
        DD_Num = wkQuery->FieldByName("DD_NUM")->AsString.ToIntDef(0);

        wkQuery->Close();

        delete wkQuery;
        return true;
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F E�޽���Ͻ�����
// �T  �v      �F
// ��  ��      �F AnsiString FRM_TYPE  : ��ڰČ^��
// �F            GSKT_TYPE : ��ڰĎ��
// �F            FRM_DIV   : ���i����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Search_HD_E_GASKET(AnsiString FRM_TYPE,
    AnsiString GSKT_TYPE, AnsiString FRM_DIV) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_E_GASKET";
    s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + FRM_TYPE + "'";
    s_SQL = s_SQL + "    AND GSKT_TYPE = '" + GSKT_TYPE + "'";
    s_SQL = s_SQL + "    AND FRM_DIV = '" + FRM_DIV + "'";

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        EP_B1s1_Name = "";
        EP_B1s1_Label = "";
        EP_B1s1_Code = 0;
        EP_B1s1_Num = 0;
        EP_B0s1_Name = "";
        EP_B0s1_Label = "";
        EP_B0s1_Code = 0;
        EP_B0s1_Num = 0;
        EP_B1s2_Name = "";
        EP_B1s2_Label = "";
        EP_B1s2_Code = 0;
        EP_B1s2_Num = 0;
        EP_B0s2_Name = "";
        EP_B0s2_Label = "";
        EP_B0s2_Code = 0;
        EP_B0s2_Num = 0;
        EPUH_Meq_Sub = 0;
        EPU_Meq_Sub = 0;
        EP_B1s3_Name = "";
        EP_B1s3_Label = "";
        EP_B1s3_Code = 0;
        EP_B1s3_Num = 0;
        EP_B0s3_Name = "";
        EP_B0s3_Label = "";
        EP_B0s3_Code = 0;
        EP_B0s3_Num = 0;
        EP_B1s4_Name = "";
        EP_B1s4_Label = "";
        EP_B1s4_Code = 0;
        EP_B1s4_Num = 0;
        EP_B0s4_Name = "";
        EP_B0s4_Label = "";
        EP_B0s4_Code = 0;
        EP_B0s4_Num = 0;
        EPLH_Meq_Sub = 0;
        ED_Hp = 0;
        // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_S
        //ED_Name = "";
        //ED_Label = "";
        //ED_Code = 0;
        //ED_Num = 0;
        ED_B1_Name = "";
        ED_B1_Label = "";
        ED_B1_Code = 0;
        ED_B1_Num = 0;
        ED_B0_Name = "";
        ED_B0_Label = "";
        ED_B0_Code = 0;
        ED_B0_Num = 0;
        // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_E

        delete wkQuery;
        // return false;
        return true;
    }
    else {

        EP_B1s1_Name = wkQuery->FieldByName("EP_B1S1_NAME")
            ->AsString.TrimRight();
        EP_B1s1_Label = wkQuery->FieldByName("EP_B1S1_LABEL")
            ->AsString.TrimRight();
        EP_B1s1_Code = wkQuery->FieldByName("EP_B1S1_CODE")
            ->AsString.ToIntDef(0);
        EP_B1s1_Num = wkQuery->FieldByName("EP_B1S1_NUM")->AsString.ToIntDef(0);
        EP_B0s1_Name = wkQuery->FieldByName("EP_B0S1_NAME")
            ->AsString.TrimRight();
        EP_B0s1_Label = wkQuery->FieldByName("EP_B0S1_LABEL")
            ->AsString.TrimRight();
        EP_B0s1_Code = wkQuery->FieldByName("EP_B0S1_CODE")
            ->AsString.ToIntDef(0);
        EP_B0s1_Num = wkQuery->FieldByName("EP_B0S1_NUM")->AsString.ToIntDef(0);
        EP_B1s2_Name = wkQuery->FieldByName("EP_B1S2_NAME")
            ->AsString.TrimRight();
        EP_B1s2_Label = wkQuery->FieldByName("EP_B1S2_LABEL")
            ->AsString.TrimRight();
        EP_B1s2_Code = wkQuery->FieldByName("EP_B1S2_CODE")
            ->AsString.ToIntDef(0);
        EP_B1s2_Num = wkQuery->FieldByName("EP_B1S2_NUM")->AsString.ToIntDef(0);
        EP_B0s2_Name = wkQuery->FieldByName("EP_B0S2_NAME")
            ->AsString.TrimRight();
        EP_B0s2_Label = wkQuery->FieldByName("EP_B0S2_LABEL")
            ->AsString.TrimRight();
        EP_B0s2_Code = wkQuery->FieldByName("EP_B0S2_CODE")
            ->AsString.ToIntDef(0);
        EP_B0s2_Num = wkQuery->FieldByName("EP_B0S2_NUM")->AsString.ToIntDef(0);
        EPUH_Meq_Sub = wkQuery->FieldByName("EPUH_MEQ_SUB")
            ->AsString.ToIntDef(0);
        EPU_Meq_Sub = wkQuery->FieldByName("EPU_MEQ_SUB")->AsString.ToIntDef(0);
        EP_B1s3_Name = wkQuery->FieldByName("EP_B1S3_NAME")
            ->AsString.TrimRight();
        EP_B1s3_Label = wkQuery->FieldByName("EP_B1S3_LABEL")
            ->AsString.TrimRight();
        EP_B1s3_Code = wkQuery->FieldByName("EP_B1S3_CODE")
            ->AsString.ToIntDef(0);
        EP_B1s3_Num = wkQuery->FieldByName("EP_B1S3_NUM")->AsString.ToIntDef(0);
        EP_B0s3_Name = wkQuery->FieldByName("EP_B0S3_NAME")
            ->AsString.TrimRight();
        EP_B0s3_Label = wkQuery->FieldByName("EP_B0S3_LABEL")
            ->AsString.TrimRight();
        EP_B0s3_Code = wkQuery->FieldByName("EP_B0S3_CODE")
            ->AsString.ToIntDef(0);
        EP_B0s3_Num = wkQuery->FieldByName("EP_B0S3_NUM")->AsString.ToIntDef(0);
        EP_B1s4_Name = wkQuery->FieldByName("EP_B1S4_NAME")
            ->AsString.TrimRight();
        EP_B1s4_Label = wkQuery->FieldByName("EP_B1S4_LABEL")
            ->AsString.TrimRight();
        EP_B1s4_Code = wkQuery->FieldByName("EP_B1S4_CODE")
            ->AsString.ToIntDef(0);
        EP_B1s4_Num = wkQuery->FieldByName("EP_B1S4_NUM")->AsString.ToIntDef(0);
        EP_B0s4_Name = wkQuery->FieldByName("EP_B0S4_NAME")
            ->AsString.TrimRight();
        EP_B0s4_Label = wkQuery->FieldByName("EP_B0S4_LABEL")
            ->AsString.TrimRight();
        EP_B0s4_Code = wkQuery->FieldByName("EP_B0S4_CODE")
            ->AsString.ToIntDef(0);
        EP_B0s4_Num = wkQuery->FieldByName("EP_B0S4_NUM")->AsString.ToIntDef(0);
        EPLH_Meq_Sub = wkQuery->FieldByName("EPLH_MEQ_SUB")
            ->AsString.ToIntDef(0);
        // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_S
        //ED_Name = wkQuery->FieldByName("ED_NAME")->AsString.TrimRight();
        //ED_Label = wkQuery->FieldByName("ED_LABEL")->AsString.TrimRight();
        //ED_Code = wkQuery->FieldByName("ED_CODE")->AsString.ToIntDef(0);
        //ED_Num = wkQuery->FieldByName("ED_NUM")->AsString.ToIntDef(0);
        ED_B1_Name = wkQuery->FieldByName("ED_B1_NAME")->AsString.TrimRight();
        ED_B1_Label = wkQuery->FieldByName("ED_B1_LABEL")->AsString.TrimRight();
        ED_B1_Code = wkQuery->FieldByName("ED_B1_CODE")->AsString.ToIntDef(0);
        ED_B1_Num = wkQuery->FieldByName("ED_B1_NUM")->AsString.ToIntDef(0);
        ED_B0_Name = wkQuery->FieldByName("ED_B0_NAME")->AsString.TrimRight();
        ED_B0_Label = wkQuery->FieldByName("ED_B0_LABEL")->AsString.TrimRight();
        ED_B0_Code = wkQuery->FieldByName("ED_B0_CODE")->AsString.ToIntDef(0);
        ED_B0_Num = wkQuery->FieldByName("ED_B0_NUM")->AsString.ToIntDef(0);
        // 2019.08.01 �r�A�s�f�B�X�^���X�s�[�X�ǉ�_E

        // 2003.10.17 ���o�[�A�`���[�u�̏ꍇ�d�m�Y���K�X�P�b�g�͂��Ȃ�
        // 2011.06.20 ɽ�ًK�i�ύX
        // if( EP_B1s1_Code == 79 && (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" || P_HEADS_DATA[953].TrimRight().SubString(2,1) == "2") ) EP_B1s1_Num = 0;
        // if( EP_B1s2_Code == 79 && (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" || P_HEADS_DATA[968].TrimRight().SubString(2,1) == "2") ) EP_B1s2_Num = 0;
        // if( EP_B1s3_Code == 79 && (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" || P_HEADS_DATA[983].TrimRight().SubString(2,1) == "2") ) EP_B1s3_Num = 0;
        // if( EP_B1s4_Code == 79 && (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" || P_HEADS_DATA[998].TrimRight().SubString(2,1) == "2") ) EP_B1s4_Num = 0;
        if (EP_B1s1_Code == 79 && (P_HEADS_DATA[953].TrimRight().SubString(3,
            1) == "1" || P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "2"))
            EP_B1s1_Num = 0;
        if (EP_B1s2_Code == 79 && (P_HEADS_DATA[968].TrimRight().SubString(3,
            1) == "1" || P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "2"))
            EP_B1s2_Num = 0;
        if (EP_B1s3_Code == 79 && (P_HEADS_DATA[983].TrimRight().SubString(3,
            1) == "1" || P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "2"))
            EP_B1s3_Num = 0;
        if (EP_B1s4_Code == 79 && (P_HEADS_DATA[998].TrimRight().SubString(3,
            1) == "1" || P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "2"))
            EP_B1s4_Num = 0;
        // *************************************************************

        wkQuery->Close();

        delete wkQuery;
        return true;
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F BFRGϽ�����
// �T  �v      �F
// ��  ��      �F AnsiString FRM_TYPE  : ��ڰČ^��
// �F            GSKT_TYPE : ��ڰĎ��
// �F            FRM_DIV   : ���i����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Search_HD_BFRG(AnsiString FRM_TYPE,
    AnsiString GSKT_TYPE, AnsiString FRM_DIV) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_BFRG";
    s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + FRM_TYPE + "'";
    s_SQL = s_SQL + "    AND GSKT_TYPE = '" + GSKT_TYPE + "'";
    s_SQL = s_SQL + "    AND FRM_DIV = '" + FRM_DIV + "'";

    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {

        BP_Hp = 0;
        BBP_Name = "";
        BBP_Label = "";
        BBP_Code = 0;
        BBP_Num = 0;
        ABP_Name = "";
        ABP_Label = "";
        ABP_Code = 0;
        ABP_Num = 0;
        Note = "";

        delete wkQuery;
        // return false;
        return true;
    }
    else {

        BP_Hp = wkQuery->FieldByName("BP_HP")->AsString.ToIntDef(0);
        BBP_Name = wkQuery->FieldByName("BBP_NAME")->AsString.TrimRight();
        BBP_Label = wkQuery->FieldByName("BBP_LABEL")->AsString.TrimRight();
        BBP_Code = wkQuery->FieldByName("BBP_CODE")->AsString.ToIntDef(0);
        BBP_Num = wkQuery->FieldByName("BBP_NUM")->AsString.ToIntDef(0);
        ABP_Name = wkQuery->FieldByName("ABP_NAME")->AsString.TrimRight();
        ABP_Label = wkQuery->FieldByName("ABP_LABEL")->AsString.TrimRight();
        ABP_Code = wkQuery->FieldByName("ABP_CODE")->AsString.ToIntDef(0);
        ABP_Num = wkQuery->FieldByName("ABP_NUM")->AsString.ToIntDef(0);
        Note = wkQuery->FieldByName("NOTE")->AsString.TrimRight();

        wkQuery->Close();

        delete wkQuery;
        return true;
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F �K�X�P�b�g�\���i����
// �T  �v      �F
// ��  ��      �F AnsiString PartsCD  : ���i�ԍ�
// �F            MatCD    : �ގ�����
// �߂�l      �F �������� [�Y���\���i����]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
int __stdcall TPlateDetails::Search_GSKT_Yobi(AnsiString PartsCD,
    AnsiString MatCD) {
    int iLop;
    int iYobiNum;
    int iYobiNum2;
    int iYobiNum3;
    int iYobiNum4;
    AnsiString aYobiCD;
    AnsiString aYobiCD2;
    AnsiString aYobiCD3;
    AnsiString aYobiCD4;
    bool bYobiChk;

    iYobiNum = 0;
    iYobiNum2 = 0;
    iYobiNum3 = 0;
    iYobiNum4 = 0;
    bYobiChk = false;
    for (iLop = 0; iLop < P_YobiMemoCnt && iLop < 50; iLop++) {
        if ((P_YobiCD[iLop].TrimRight() == PartsCD.TrimRight()) && // �\���i���ވ�v
            (P_YobiMAT[iLop].TrimRight() == MatCD.TrimRight())) { // �\���i�ގ����ވ�v
            bYobiChk = true;
        }
    }
    if (bYobiChk == false) { // �\�����擾�ς݂ł͂Ȃ�
        // 2004.02.02 �\�����̎擾����
        // switch( PartsCD.ToIntDef(0)) {
        // case 6:                     // �c�v���[�g�K�X�P�b�g
        // aYobiCD = "GSKT02";
        // break;
        // case 7:                     // �c�v���[�g�K�X�P�b�g�`
        // aYobiCD = "GSKT08";
        // break;
        // case 8:                     // �c�v���[�g�K�X�P�b�g�a
        // aYobiCD = "GSKT09";
        // break;
        // //2004.01.20 �\�����̎擾����
        // //case 55:                    // E�v���[�g�K�X�P�b�g�`
        // //case 56:                    // �d�v���[�g�K�X�P�b�g�a
        // //case 57:                    // �d�v���[�g�K�X�P�b�g
        // case 9:                     // �c�v���[�g�K�X�P�b�g�c�|�`
        // aYobiCD = "GSKT08";
        // break;
        // case 10:                    // �c�v���[�g�K�X�P�b�g�c�|�a
        // aYobiCD = "GSKT09";
        // break;
        // case 55:                    // �d�v���[�g�K�X�P�b�g�`
        // aYobiCD = "GSKT03";
        // break;
        // case 56:                    // �d�v���[�g�K�X�P�b�g�a
        // aYobiCD = "GSKT03";
        // break;
        // case 57:                    // �d�v���[�g�K�X�P�b�g
        // aYobiCD = "GSKT03";
        // break;
        // case 58:                    // �d�v���[�g�K�X�P�b�g�c�|�`
        // aYobiCD = "GSKT03";
        // break;
        // case 59:                    // �d�v���[�g�K�X�P�b�g�c�|�a
        // aYobiCD = "GSKT03";
        // break;
        // //***************************
        // case 60:                    // �d�v���[�g�K�X�P�b�g�d
        // aYobiCD = "GSKT03";
        // break;
        // case 13:                    // �c�f�B�X�^���X�s�[�X
        // //2004.01.20 �\�����̎擾����
        // aYobiCD = "GSKT06";
        // break;
        // //***************************
        // case 78:                    // �f�B�X�^���X�s�[�X
        // aYobiCD = "GSKT06";
        // break;
        // case 15:                    // �c�v���[�g�K�X�P�b�g�a�|�S
        // aYobiCD = "GSKT13";
        // break;
        // case 16:                    // �c�v���[�g�K�X�P�b�g�`�|�Q
        // aYobiCD = "GSKT12";
        // break;
        // case 79:                    // �d�m�Y���K�X�P�b�g
        // aYobiCD = "GSKT04";
        // break;
        // case 64:                    // �d�f�B�X�^���X�s�[�X
        // aYobiCD = "GSKT14";
        // break;
        // default :
        // aYobiCD = "";
        // break;
        // }
        aYobiCD2 = "";
        aYobiCD3 = "";
        aYobiCD4 = "";
        switch (PartsCD.ToIntDef(0)) {
        case 6: // �c�v���[�g�K�X�P�b�g
            if (G_KS_Syurui == "�e�w" && G_KS_Model == "�O�R") { // �c�v���[�g�K�X�P�b�g�k
                aYobiCD = "GSKT14";
            }
            // 2023.07.12 �\���i�ύX_S
            else if (G_KS_Syurui == "�f�w" && G_KS_Model == "�T�O") { // �c�b�v���[�g�K�X�P�b�g
                aYobiCD = "GSKTA3";
            }
            // 2023.07.12 �\���i�ύX_E
            else { // �c�v���[�g�K�X�P�b�g
                aYobiCD = "GSKT27";
            }
            aYobiCD2 = "GSKT02"; // �c�v���[�g�K�X�P�b�g�i���j
            aYobiCD3 = "GSKT08"; // �c�v���[�g�K�X�P�b�g�`
            aYobiCD4 = "GSKT09"; // �c�v���[�g�K�X�P�b�g�a
            break;
        case 7: // �c�v���[�g�K�X�P�b�g�`
            if (G_KS_Syurui == "�e�w" && G_KS_Model == "�O�R") { // �c�v���[�g�K�X�P�b�g�q�`
                aYobiCD = "GSKT15";
            }
            else { // �c�v���[�g�K�X�P�b�g�`
                aYobiCD = "GSKT08";
            }
            break;
        case 8: // �c�v���[�g�K�X�P�b�g�a
            if (G_KS_Syurui == "�e�w" && G_KS_Model == "�O�R") { // �c�v���[�g�K�X�P�b�g�q�a
                aYobiCD = "GSKT16";
            }
            else { // �c�v���[�g�K�X�P�b�g�a
                aYobiCD = "GSKT09";
            }
            break;
        case 9: // �c�v���[�g�K�X�P�b�g�c�|�`
            // 2023.07.27 �\���i�ύX_S
            //// 2017.08.17 �\���i�ύX_S
            ////aYobiCD = "GSKT08";
            //aYobiCD = "GSKT47";
            //// 2017.08.17 �\���i�ύX_E
            if (G_KS_Syurui == "�k�w" && G_KS_Model == "�T�O�r") { // �c�`�K�X�P�b�g
                aYobiCD = "GSKT08";
            }
            else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") { // �c�`�K�X�P�b�g
                aYobiCD = "GSKT08";
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�P�O") { // �c�`�K�X�P�b�g
                aYobiCD = "GSKT08";
            }
            else { // �c�v���[�g�K�X�P�b�g�c�|�`
                aYobiCD = "GSKT47";
            }
            // 2023.07.27 �\���i�ύX_E
            break;
        case 10: // �c�v���[�g�K�X�P�b�g�c�|�a
            // 2023.07.27 �\���i�ύX_S
            //// 2017.08.17 �\���i�ύX_S
            ////aYobiCD = "GSKT09";
            //aYobiCD = "GSKT48";
            //// 2017.08.17 �\���i�ύX_E
            if (G_KS_Syurui == "�k�w" && G_KS_Model == "�T�O�r") { // �c�a�K�X�P�b�g
                aYobiCD = "GSKT09";
            }
            else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c") { // �c�a�K�X�P�b�g
                aYobiCD = "GSKT09";
            }
            else if (G_KS_Syurui == "�r�w" && G_KS_Model == "�P�O") { // �c�a�K�X�P�b�g
                aYobiCD = "GSKT09";
            }
            else { // �c�v���[�g�K�X�P�b�g�c�|�a
                aYobiCD = "GSKT48";
            }
            // 2023.07.27 �\���i�ύX_E
            break;
        case 13: // �c�f�B�X�^���X�s�[�X
            aYobiCD = "GSKT06";
            break;
        // 2023.07.12 �\���i�ύX_S
        case 14: // �v���[�g�K�X�P�b�g�a�P
            aYobiCD = "GSKT11";
            break;
        case 15: // �v���[�g�K�X�P�b�g�a�S
            aYobiCD = "GSKT13";
            break;
        case 16: // �v���[�g�K�X�P�b�g�`�Q
            aYobiCD = "GSKT12";
            break;
        case 17: // �v���[�g�K�X�P�b�g�`�R
            aYobiCD = "GSKT10";
            break;
        // 2023.07.12 �\���i�ύX_E
        case 32: // �v���[�g�K�X�P�b�g�a�|�P
            aYobiCD = "GSKT11";
            break;
        case 33: // �v���[�g�K�X�P�b�g�a�|�S
            aYobiCD = "GSKT13";
            break;
        case 34: // �v���[�g�K�X�P�b�g�`�|�Q
            aYobiCD = "GSKT12";
            break;
        case 35: // �v���[�g�K�X�P�b�g�`�|�R
            aYobiCD = "GSKT10";
            break;
        case 38: // �^�󎞖ڋʖӕ��⋭
            aYobiCD = "GSKT33";
            break;
        case 55: // �d�v���[�g�K�X�P�b�g�`
            // 2017.08.17 �\���i�ύX_S
            //aYobiCD = "GSKT29";
            aYobiCD = "GSKT42";
            // 2017.08.17 �\���i�ύX_E
            break;
        case 56: // �d�v���[�g�K�X�P�b�g�a
            // 2017.08.17 �\���i�ύX_S
            //aYobiCD = "GSKT30";
            aYobiCD = "GSKT43";
            // 2017.08.17 �\���i�ύX_E
            break;
        case 57: // �d�v���[�g�K�X�P�b�g
            aYobiCD = "GSKT28";
            aYobiCD2 = "GSKT03"; // �d�v���[�g�K�X�P�b�g�i���j
            // 2017.08.17 �\���i�ύX_S
            //aYobiCD3 = "GSKT29"; // �d�v���[�g�K�X�P�b�g�`
            aYobiCD3 = "GSKT42"; // �d�v���[�g�K�X�P�b�g�`
            //aYobiCD4 = "GSKT30"; // �d�v���[�g�K�X�P�b�g�a
            aYobiCD4 = "GSKT43"; // �d�v���[�g�K�X�P�b�g�a
            // 2017.08.17 �\���i�ύX_E
            break;
        // 2023.07.12 �\���i�ύX_S
        //// 2017.08.17 �\���i�ύX_S
        ////case 58: // �d�v���[�g�K�X�P�b�g�c�|�`
        ////    aYobiCD = "GSKT29";
        ////    break;
        //case 58: // �d�v���[�g�K�X�P�b�g��c�|�`
        //    aYobiCD = "GSKT47";
        //    break;
        ////case 59: // �d�v���[�g�K�X�P�b�g�c�|�a
        ////    aYobiCD = "GSKT30";
        ////    break;
        //case 59: // �d�v���[�g�K�X�P�b�g���c�|�a
        //    aYobiCD = "GSKT48";
        //    break;
        //// 2017.08.17 �\���i�ύX_E
        case 58: // �d�v���[�g�K�X�P�b�g��c�|�`
            aYobiCD = "GSKT49";
            break;
        case 59: // �d�v���[�g�K�X�P�b�g���c�|�a
            aYobiCD = "GSKT50";
            break;
        // 2023.07.12 �\���i�ύX_E
        case 60: // �d�v���[�g�K�X�P�b�g�d
            // 2017.08.17 �\���i�ύX_S
            //aYobiCD = "GSKT28";
            aYobiCD = "GSKT51";
            aYobiCD2 = "GSKT03"; // �d�v���[�g�K�X�P�b�g�i���j
            //aYobiCD3 = "GSKT29"; // �d�v���[�g�K�X�P�b�g�`
             aYobiCD3 = "GSKT42"; // �d�v���[�g�K�X�P�b�g�`
            //aYobiCD4 = "GSKT30"; // �d�v���[�g�K�X�P�b�g�a
            aYobiCD = "GSKT43";
            // 2017.08.17 �\���i�ύX_E
            break;
        case 62: // �d�v���[�g�ڋʕ⋭�p�b�L���O�`
            if (G_KS_Syurui == "�d�w" && G_KS_Model == "�P�T") { // �v���[�g�t��
                aYobiCD = "";
            }
            else { // �d�v���[�g�ڋʕ⋭�p�b�L���O�`
                aYobiCD = "GSKT31";
            }
            break;
        case 63: // �d�v���[�g�ڋʕ⋭�p�b�L���O�a
            if (G_KS_Syurui == "�d�w" && G_KS_Model == "�P�T") { // �v���[�g�t��
                aYobiCD = "";
            }
            else { // �d�v���[�g�ڋʕ⋭�p�b�L���O�a
                aYobiCD = "GSKT32";
            }
            break;
        case 64: // �d�f�B�X�^���X�s�[�X
            aYobiCD = "GSKT19";
            break;
        case 65: // �d�v���[�g�⋭�K�X�P�b�g
            aYobiCD = "GSKT26";
            aYobiCD3 = "GSKT31"; // �d�v���[�g�ڋʕ⋭�p�b�L���O�`
            aYobiCD4 = "GSKT32"; // �d�v���[�g�ڋʕ⋭�p�b�L���O�a
            break;
        // 2023.07.12 �\���i�ύX_S
        case 69: // �v���[�g�K�X�P�b�g�a�P
            aYobiCD = "GSKT11";
            break;
        case 70: // �v���[�g�K�X�P�b�g�a�S
            aYobiCD = "GSKT13";
            break;
        case 71: // �v���[�g�K�X�P�b�g�`�Q
            aYobiCD = "GSKT12";
            break;
        case 72: // �v���[�g�K�X�P�b�g�`�R
            aYobiCD = "GSKT10";
            break;
        // 2023.07.12 �\���i�ύX_E
        case 78: // �f�B�X�^���X�s�[�X
            aYobiCD = "GSKT06";
            break;
        case 79: // �d�m�Y���K�X�P�b�g
            // 2023.07.12 �\���i�ύX_S
            //aYobiCD = "GSKT04";
            if (G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O") { // �d���ʘH�p�b�L���O
                aYobiCD = "GSKT59";
            }
            else { // �d�m�Y���K�X�P�b�g
                aYobiCD = "GSKT04";
            }
            // 2023.07.12 �\���i�ύX_S
            break;
        case 80: // (�c)�O�p���p�b�`�`
            aYobiCD = "GSKT34";
            break;
        // 2017.08.17 �\���i�ύX_S
        //case 81: // (�c)�O�p���p�b�`�a
        //    aYobiCD = "GSKT35";
        //    break;
        //case 82: // (�c)�O�p���p�b�`�b
        //    aYobiCD = "GSKT36";
        //    break;
        case 83: // (�d)�O�p���p�b�`�`
        //    aYobiCD = "GSKT37";
            aYobiCD = "GSKT34";
            break;
        //case 84: // (�d)�O�p���p�b�`�a
        //    aYobiCD = "GSKT38";
        //    break;
        //case 85: // (�d)�O�p���p�b�`�b
        //    aYobiCD = "GSKT39";
        //    break;
        // 2017.08.17 �\���i�ύX_E
        case 87: // (�c)�⋭�p�b�`
            aYobiCD = "GSKT40";
            break;
        case 88: // (�d)�⋭�p�b�`
            // 2017.08.17 �\���i�ύX_S
            //aYobiCD = "GSKT41";
            aYobiCD = "GSKT40";
            // 2017.08.17 �\���i�ύX_E
            break;
        // 2017.08.17 �\���i�ύX_S
        case 89: // (�c)�c�v���[�g�K�X�P�b�g�c�`�|�q
            aYobiCD = "GSKT60";
            break;
        case 90: // (�c)�c�v���[�g�K�X�P�b�g�c�`�|�k
            aYobiCD = "GSKT61";
            break;
        case 91: // (�c)�c�v���[�g�K�X�P�b�g�c�a�|�q
            aYobiCD = "GSKT62";
            break;
        case 92: // (�c)�c�v���[�g�K�X�P�b�g�c�a�|�k
            aYobiCD = "GSKT63";
            break;
        case 93: // (�d)�c�v���[�g�K�X�P�b�g�c�`�|�q
            aYobiCD = "GSKT60";
            break;
        case 94: // (�d)�c�v���[�g�K�X�P�b�g�c�`�|�k
            aYobiCD = "GSKT61";
            break;
        case 95: // (�d)�c�v���[�g�K�X�P�b�g�c�a�|�q
            aYobiCD = "GSKT62";
            break;
        case 96: // (�d)�c�v���[�g�K�X�P�b�g�c�a�|�k
            aYobiCD = "GSKT63";
            break;
        // 2023.07.12 �\���i�ύX_S
        case 97: // �����O�K�X�P�b�g
            aYobiCD = "GSKT44";
            break;
        case 98: // �����O�K�X�P�b�g�k
            aYobiCD = "GSKT45";
            break;
        case 99: // �����O�K�X�P�b�g�q
            aYobiCD = "GSKT46";
            break;
        // 2023.07.12 �\���i�ύX_E
        case 100: // �d���ʘH�p�b�L���O
            aYobiCD = "GSKT59";
            break;
        case 101: // �c�v���[�g�K�X�P�b�g��c�|�`
            // 2023.07.12 �\���i�ύX_S
            //aYobiCD = "GSKT49";
            if (G_KS_Syurui == "�k�w" && G_KS_Model == "�O�O") { // �c�v���[�g�K�X�P�b�g��c�|�`
                aYobiCD = "GSKT47";
            }
            else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�P�O") { // �c�v���[�g�K�X�P�b�g��c�|�`
                aYobiCD = "GSKT47";
            }
            else { // �c�\�`�K�X�P�b�g
                aYobiCD = "GSKT08";
            }
            // 2023.07.12 �\���i�ύX_S
            break;
        case 102: // �c�v���[�g�K�X�P�b�g���c�|�a
            // 2023.07.12 �\���i�ύX_S
            //aYobiCD = "GSKT50";
            if (G_KS_Syurui == "�k�w" && G_KS_Model == "�O�O") { // �c�v���[�g�K�X�P�b�g��c�|�a
                aYobiCD = "GSKT48";
            }
            else if (G_KS_Syurui == "�k�w" && G_KS_Model == "�P�O") { // �c�v���[�g�K�X�P�b�g��c�|�a
                aYobiCD = "GSKT48";
            }
            else { // �c�\�a�K�X�P�b�g
                aYobiCD = "GSKT09";
            }
            // 2023.07.12 �\���i�ύX_S
            break;
        case 103: // (�c)�f�B�X�^���X�s�[�X�c
            aYobiCD = "GSKT92";
            break;
        case 104: // (�d)�f�B�X�^���X�s�[�X�c
            aYobiCD = "GSKT93";
            break;
        case 105: // �c�v���[�g�K�X�P�b�g�P
            aYobiCD = "GSKT35";
            break;
        case 106: // �c�v���[�g�K�X�P�b�g�Q
            aYobiCD = "GSKT36";
            break;
        case 110: // �d�a�P�v���[�g�K�X�P�b�g
            aYobiCD = "GSKT92";
            break;
        case 111: // �d�a�Q�v���[�g�K�X�P�b�g
            aYobiCD = "GSKT93";
            break;
        // 2017.08.17 �\���i�ύX_E
        // 2023.07.12 �\���i�ύX_S
        case 112: // �a�f�B�X�^���X�s�[�X�`
            aYobiCD = "GSKT31";
            break;
        case 113: // �a�f�B�X�^���X�s�[�X�a
            aYobiCD = "GSKT32";
            break;
        case 114: // �r�f�B�X�^���X�s�[�X
            if (G_KS_Syurui == "�v�g" && G_KS_Model == "�R�O") { // �r�f�B�X�^���X�i�f�|�T�|�Q�j
                aYobiCD = "GSKT37";
            }
            else { // �r�f�B�X�^���X�s�[�X
                aYobiCD = "GSKT06";
            }
            break;
        case 115: // �d���r�f�B�X�^���X�s�[�X
            aYobiCD = "GSKT19";
            break;
        case 116: // �s�f�B�X�^���X�s�[�X
            aYobiCD = "GSKT40";
            break;
        case 117: // �s�f�B�X�^���X�s�[�X
            aYobiCD = "GSKT40";
            break;
        // 2023.07.12 �\���i�ύX_E
        default:
            aYobiCD = "";
            break;
        }
        // ***************************
        iYobiNum = Syukei_Buhin_Yobisu(aYobiCD, MatCD, m_pcHeadsData);
        // 2004.02.02 �\�����̎擾����
        if ((PartsCD.ToIntDef(0) == 6) || (PartsCD.ToIntDef(0) == 57) ||
            (PartsCD.ToIntDef(0) == 60) || (PartsCD.ToIntDef(0) == 65)) {
            if (iYobiNum == 0) {
                iYobiNum2 = Syukei_Buhin_Yobisu(aYobiCD2, MatCD, m_pcHeadsData);
                iYobiNum3 = Syukei_Buhin_Yobisu(aYobiCD3, MatCD, m_pcHeadsData);
                iYobiNum4 = Syukei_Buhin_Yobisu(aYobiCD4, MatCD, m_pcHeadsData);
                if (iYobiNum2 != 0) {
                    iYobiNum = iYobiNum2;
                }
                else {
                    iYobiNum = iYobiNum3 + iYobiNum4;
                }
            }
        }
        // ***************************
        P_YobiCD[P_YobiMemoCnt] = PartsCD;
        P_YobiMAT[P_YobiMemoCnt] = MatCD;
        P_YobiMemoCnt++;
    }
    return iYobiNum;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �K�X�P�b�g�\���i�擾����
// �T  �v      �F
// ��  ��      �F �K�X�P�b�g���i�R�[�h�@�K�X�P�b�g�ގ��R�[�h
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
int __fastcall TPlateDetails::Get_Gskt_YobiSu(int i_TmpCd, AnsiString MatCD) {
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
    s_SQL = s_SQL + "SELECT * FROM HD_GSKT2YOBI";
    s_SQL = s_SQL + "  WHERE GSKTPCD = " + i_TmpCd;

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

    i_YobiSu = Syukei_Buhin_Yobisu(s_YobiName, MatCD, m_pcHeadsData);
    return i_YobiSu;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �q�w�|�O�O��p�c�v���[�g�K�X�P�b�g���i�@�ް��쐬
// �T  �v      �F �c�v���[�g�̃K�X�P�b�g�͈�̕���ؒf���Ďg�p����̂�
// �v���[�g�P������̐��ʂ͂P���ȏ�ɂȂ�Ȃ�
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2006.01.10 �T�u���[�`���ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_RX00D_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    AnsiString s_CLabel;
    AnsiString s_DLabel;

    ret = Search_HD_D_GASKET(G_D_GsktType, G_GsktType, G_FrmDiv);

    AnsiString s_B_GSKT_Mat;
    AnsiString s_A_GSKT_Mat;
    s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
    s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();

    // 2013.04.03 �ގ���r�ǉ�
    AnsiString s_A_GMat;
    AnsiString s_B_GMat;
    AnsiString s_C_GMat;
    AnsiString s_D_GMat;
    // ***********************

    DE_Kubun = "D";
    // �p�^�[������ɂ��K�v�ȃK�X�P�b�g��I��
    // A�X�^�[�g
    if (G_D_GsktABKubun == "A") {
        // S1:B���̏���
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(1, 1), DP_B0s1_Code,
            DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code,
            DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, s_B_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        if (G_D_GsktAna.SubString(1, 1) != "0") {
            // �i�c�v���[�g�K�X�P�b�g�j
            s_A_GMat = s_B_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_A_GMat = "2019";
        }
        // ***********************
        // S2:A���̏���
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(2, 1), DP_B0s2_Code,
            DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code,
            DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, s_A_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        if (G_D_GsktAna.SubString(2, 1) != "0") {
            // �i�c�v���[�g�K�X�P�b�g�j
            s_B_GMat = s_A_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_B_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // if( s_BLabel == s_ALabel ){
        // //S2�޽��Ă�S1�޽��Ă�����̏ꍇ�̐��ʂŏo�́i�d�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        // }
        if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
            // S2�޽��Ă�S1�޽��Ă�����̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // ***********************
        // S3:B���̏���
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1), DP_B0s3_Code,
            DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code,
            DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, s_B_GSKT_Mat);
        s_CLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        if (G_D_GsktAna.SubString(3, 1) != "0") {
            // �i�c�v���[�g�K�X�P�b�g�j
            s_C_GMat = s_B_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_C_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // 2013.04.03 �ގ���r�ǉ�
        // if( s_CLabel == s_ALabel || s_CLabel == s_BLabel ){
        // //S3�޽��Ă�S1�޽��Ă�������S3�޽��Ă�S2�޽��Ă�����̏ꍇ�̐��ʂŏo��
        // if( G_D_GsktAna.SubString(3,1) != "0" ){
        // //�i�c�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret);                       // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); // HEARTS�p�ް�CSV�쐬
        // }else{
        // //�i�d�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        // }
        // }
        if (s_CLabel == s_ALabel ||
            (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
            // S3�޽��Ă�S1�޽��Ă�������S3�޽��Ă�S2�޽��Ă�����̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // ***********************
        // S4:A���̏���
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1), DP_B0s4_Code,
            DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code,
            DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, s_A_GSKT_Mat);
        s_DLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        if (G_D_GsktAna.SubString(4, 1) != "0") {
            // �i�c�v���[�g�K�X�P�b�g�j
            s_D_GMat = s_A_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_D_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // if( s_DLabel == s_ALabel || s_DLabel == s_BLabel || s_DLabel == s_CLabel ){
        // //S4�޽��Ă�S1�޽��Ă�������S4�޽��Ă�S2�޽��Ă�������S4�޽��Ă�S3�޽��Ă�����̏ꍇ�̐��ʂŏo��
        // if( G_D_GsktAna.SubString(4,1) != "0" ){
        // //�i�c�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret);                       // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight()); // HEARTS�p�ް�CSV�쐬
        // }else{
        // //�i�d�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        // }
        // }
        if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
            s_DLabel == s_BLabel ||
            (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
            // S4�޽��Ă�S1�޽��Ă�������S4�޽��Ă�S2�޽��Ă�������S4�޽��Ă�S3�޽��Ă�����̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_D_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // ***********************
    }
    // B�X�^�[�g
    else {
        // S4:A���̏���
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1), DP_B0s1_Code,
            DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code,
            DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, s_A_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        if (G_D_GsktAna.SubString(4, 1) != "0") {
            // �i�c�v���[�g�K�X�P�b�g�j
            s_A_GMat = s_A_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_A_GMat = "2019";
        }
        // ***********************
        // S3:B���̏���
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1), DP_B0s2_Code,
            DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code,
            DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, s_B_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        if (G_D_GsktAna.SubString(3, 1) != "0") {
            // �i�c�v���[�g�K�X�P�b�g�j
            s_B_GMat = s_B_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_B_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // if( s_BLabel == s_ALabel ){
        // S3�޽��Ă�S4�޽��Ă�����̏ꍇ�̐��ʂŏo�́i�d�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        // }
        if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
            // S3�޽��Ă�S4�޽��Ă�����̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // ***********************
        // S2:A���̏���
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(2, 1), DP_B0s3_Code,
            DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code,
            DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, s_A_GSKT_Mat);
        s_CLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        if (G_D_GsktAna.SubString(2, 1) != "0") {
            // �i�c�v���[�g�K�X�P�b�g�j
            s_C_GMat = s_A_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_C_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // if( s_CLabel == s_ALabel || s_CLabel == s_BLabel ){
        // //S2�޽��Ă�S4�޽��Ă�������S2�޽��Ă�S3�޽��Ă�����̏ꍇ�̐��ʂŏo��
        // if( G_D_GsktAna.SubString(2,1) != "0" ){
        // //�i�c�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret);                       // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); // HEARTS�p�ް�CSV�쐬
        // }else{
        // //�i�d�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        // }
        // }
        if (s_CLabel == s_ALabel ||
            (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
            // S2�޽��Ă�S4�޽��Ă�������S2�޽��Ă�S3�޽��Ă�����̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // ***********************
        // S1:B���̏���
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(1, 1), DP_B0s4_Code,
            DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code,
            DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, s_B_GSKT_Mat);
        s_DLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        if (G_D_GsktAna.SubString(1, 1) != "0") {
            // �i�c�v���[�g�K�X�P�b�g�j
            s_D_GMat = s_B_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_D_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // if( s_DLabel == s_ALabel || s_DLabel == s_BLabel || s_DLabel == s_CLabel ){
        // //S1�޽��Ă�S4�޽��Ă�������S1�޽��Ă�S3�޽��Ă�������S1�޽��Ă�S2�޽��Ă�����̏ꍇ�̐��ʂŏo��
        // if( G_D_GsktAna.SubString(1,1) != "0" ){
        // //�i�c�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret);                       // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight()); // HEARTS�p�ް�CSV�쐬
        // }else{
        // //�i�d�v���[�g�K�X�P�b�g�j
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        // Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        // }
        // }
        if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
            s_DLabel == s_BLabel ||
            (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
            // S1�޽��Ă�S4�޽��Ă�������S1�޽��Ă�S3�޽��Ă�������S1�޽��Ă�S2�޽��Ă�����̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_D_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // ***********************
    }
    // �f�B�X�^���X�s�[�X�o��
    Write_D_Gskt_Data_Distance();

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �q�w�|�O�O��p�d�v���[�g�K�X�P�b�g���i�@�ް��쐬
// �T  �v      �F �d�v���[�g�̃K�X�P�b�g�͈�̕���ؒf���Ďg�p����̂�
// �v���[�g�P������̐��ʂ͂P���ȏ�ɂȂ�Ȃ�
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2006.01.10 �T�u���[�`���ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_RX00E_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    AnsiString s_CLabel;
    AnsiString s_DLabel;

    ret = Search_HD_E_GASKET(G_E_GsktType, G_GsktType, G_FrmDiv);

    // 2013.04.03 �ގ���r�ǉ�
    AnsiString s_B_GSKT_Mat;
    AnsiString s_A_GSKT_Mat;
    s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
    s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
    AnsiString s_A_GMat;
    AnsiString s_B_GMat;
    AnsiString s_C_GMat;
    AnsiString s_D_GMat;
    // ***********************

    DE_Kubun = "E";
    // �p�^�[������ɂ��K�v�ȃK�X�P�b�g��I��
    // A�X�^�[�g
    if (G_E_GsktABKubun == "A") {
        // E1:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
            P_HEADS_DATA[835].TrimRight());
        s_ALabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //if (G_D_GsktAna.SubString(1, 1) != "0") {
        if (G_E_GsktAna.SubString(1, 1) != "0") {
        // 2019.08.27 E�޽��Đ��ʒ���_E
            // �i�c�v���[�g�K�X�P�b�g�j
            s_A_GMat = s_B_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_A_GMat = "2019";
        }
        // ***********************
        // E2:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
            P_HEADS_DATA[836].TrimRight());
        s_BLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //if (G_D_GsktAna.SubString(2, 1) != "0") {
        if (G_E_GsktAna.SubString(2, 1) != "0") {
        // 2019.08.27 E�޽��Đ��ʒ���_E
            // �i�c�v���[�g�K�X�P�b�g�j
            s_B_GMat = s_A_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_B_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //// if( s_BLabel == s_ALabel ){
        //// //E2�޽��Ă�E1�޽��Ă�����̏ꍇ�̐��ʂŏo�́i�d�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        //// }
        //if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
        //    // E2�޽��Ă�E1�޽��Ă�����̏ꍇ�̐��ʂŏo�́i�d�v���[�g�K�X�P�b�g�j
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //    Write_HeartsBuhin_Data(s_B_GMat); // HEARTS�p�ް�CSV�쐬
        //}
        // ***********************
        if (G_E_GsktAna.SubString(1, 1) == "0" &&
            G_E_GsktAna.SubString(2, 1) == "0") {
            // E2�޽��Ă�E1�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS�p�ް�CSV�쐬
        } else if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
            // E2�޽��Ă�E1�޽��Ă�����̏ꍇ�̐��ʂŏo��(D��ڰĶ޽���)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // 2019.08.27 E�޽��Đ��ʒ���_E
        // E3:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
            P_HEADS_DATA[835].TrimRight());
        s_CLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //if (G_D_GsktAna.SubString(3, 1) != "0") {
        if (G_E_GsktAna.SubString(3, 1) != "0") {
        // 2019.08.27 E�޽��Đ��ʒ���_E
            // �i�c�v���[�g�K�X�P�b�g�j
            s_C_GMat = s_B_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_C_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //// if( s_CLabel == s_ALabel || s_CLabel == s_BLabel ){
        //// //E3�޽��Ă�E1�޽��Ă�������E3�޽��Ă�E2�޽��Ă�����̏ꍇ�̐��ʂŏo��
        //// if( G_E_GsktAna.SubString(3,1) != "0" ){
        //// //�i�c�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret);                       // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); // HEARTS�p�ް�CSV�쐬
        //// }else{
        //// //�i�d�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        //// }
        //// }
        //if (s_CLabel == s_ALabel ||
        //    (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
        //    // E3�޽��Ă�E1�޽��Ă�������E3�޽��Ă�E2�޽��Ă�����̏ꍇ�̐��ʂŏo��
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //    Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        //}
        // ***********************
        if (G_E_GsktAna.SubString(3, 1) == "0" &&
            G_E_GsktAna.SubString(1, 1) == "0") {
            // E3�޽��Ă�E1�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        } else if (G_E_GsktAna.SubString(3, 1) == "0" &&
                   G_E_GsktAna.SubString(2, 1) == "0") {
            // E3�޽��Ă�E2�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
         } else if (s_CLabel == s_ALabel ||
                   (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
            // E3�޽��Ă�E1�޽��Ă�������E3�޽��Ă�E2�޽��Ă�����̏ꍇ�̐��ʂŏo��
            // (D��ڰĶ޽���)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // 2019.08.27 E�޽��Đ��ʒ���_E
        // E4:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
            P_HEADS_DATA[836].TrimRight());
        s_DLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //if (G_D_GsktAna.SubString(4, 1) != "0") {
        if (G_E_GsktAna.SubString(4, 1) != "0") {
        // 2019.08.27 E�޽��Đ��ʒ���_E
            // �i�c�v���[�g�K�X�P�b�g�j
            s_D_GMat = s_A_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_D_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //// if( s_DLabel == s_ALabel || s_DLabel == s_BLabel || s_DLabel == s_CLabel ){
        //// //E4�޽��Ă�E1�޽��Ă�������E4�޽��Ă�E2�޽��Ă�������E4�޽��Ă�E3�޽��Ă�����̏ꍇ�̐��ʂŏo��
        //// if( G_E_GsktAna.SubString(4,1) != "0" ){
        //// //�i�c�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret);                       // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight()); // HEARTS�p�ް�CSV�쐬
        //// }else{
        //// //�i�d�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        //// }
        //// }
        //if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
        //    s_DLabel == s_BLabel ||
        //    (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
        //    // E4�޽��Ă�E1�޽��Ă�������E4�޽��Ă�E2�޽��Ă�������E4�޽��Ă�E3�޽��Ă�����̏ꍇ�̐��ʂŏo��
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //    Write_HeartsBuhin_Data(s_D_GMat); // HEARTS�p�ް�CSV�쐬
        //}
        //// ***********************
        if (G_E_GsktAna.SubString(4, 1) == "0" &&
            G_E_GsktAna.SubString(1, 1) == "0") {
            // E4�޽��Ă�E1�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        } else if (G_E_GsktAna.SubString(4, 1) == "0" &&
                   G_E_GsktAna.SubString(2, 1) == "0") {
            // E4�޽��Ă�E2�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        } else if (G_E_GsktAna.SubString(4, 1) == "0" &&
                   G_E_GsktAna.SubString(3, 1) == "0") {
            // E4�޽��Ă�E3�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        } else if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
                    s_DLabel == s_BLabel ||
                   (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
            // E4�޽��Ă�E1�޽��Ă�������E4�޽��Ă�E2�޽��Ă�������E4�޽��Ă�E3�޽��Ă�����̏ꍇ�̐��ʂŏo��
            // (D��ڰĶ޽���)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_D_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // 2019.08.27 E�޽��Đ��ʒ���_E
    }
    // B�X�^�[�g
    else {
        // E4:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
            P_HEADS_DATA[836].TrimRight());
        s_ALabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //if (G_D_GsktAna.SubString(4, 1) != "0") {
        if (G_E_GsktAna.SubString(4, 1) != "0") {
        // 2019.08.27 E�޽��Đ��ʒ���_E
            // �i�c�v���[�g�K�X�P�b�g�j
            s_A_GMat = s_A_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_A_GMat = "2019";
        }
        // ***********************
        // E3:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
            P_HEADS_DATA[835].TrimRight());
        s_BLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //if (G_D_GsktAna.SubString(3, 1) != "0") {
        if (G_E_GsktAna.SubString(3, 1) != "0") {
        // 2019.08.27 E�޽��Đ��ʒ���_E
            // �i�c�v���[�g�K�X�P�b�g�j
            s_B_GMat = s_B_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_B_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //// if( s_BLabel == s_ALabel ){
        //// //E3�޽��Ă�E4�޽��Ă�����̏ꍇ�̐��ʂŏo�́i�d�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        //// }
        //if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
        //    // E3�޽��Ă�E4�޽��Ă�����̏ꍇ�̐��ʂŏo��
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //    Write_HeartsBuhin_Data(s_B_GMat); // HEARTS�p�ް�CSV�쐬
        //}
        //// ***********************
        if (G_E_GsktAna.SubString(3, 1) == "0" &&
            G_E_GsktAna.SubString(4, 1) == "0") {
            // E3�޽��Ă�E4�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS�p�ް�CSV�쐬
        } else if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
            // E3�޽��Ă�E4�޽��Ă�����̏ꍇ�̐��ʂŏo��(D��ڰĶ޽���)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // 2019.08.27 E�޽��Đ��ʒ���_E
        // E2:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
            P_HEADS_DATA[836].TrimRight());
        s_CLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //if (G_D_GsktAna.SubString(2, 1) != "0") {
        if (G_E_GsktAna.SubString(2, 1) != "0") {
        // 2019.08.27 E�޽��Đ��ʒ���_E
            // �i�c�v���[�g�K�X�P�b�g�j
            s_C_GMat = s_A_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_C_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //// if( s_CLabel == s_ALabel || s_CLabel == s_BLabel ){
        //// //E2�޽��Ă�E4�޽��Ă�������E2�޽��Ă�E3�޽��Ă�����̏ꍇ�̐��ʂŏo��
        //// if( G_E_GsktAna.SubString(2,1) != "0" ){
        //// //�i�c�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret);                       // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight()); // HEARTS�p�ް�CSV�쐬
        //// }else{
        //// //�i�d�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data("2019");  //HEARTS�p�ް�CSV�쐬
        //// }
        //// }
        //if (s_CLabel == s_ALabel ||
        //    (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
        //    // E2�޽��Ă�E4�޽��Ă�������E2�޽��Ă�E3�޽��Ă�����̏ꍇ�̐��ʂŏo��
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //    Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        //}
        //// ***********************
        if (G_E_GsktAna.SubString(2, 1) == "0" &&
            G_E_GsktAna.SubString(4, 1) == "0") {
            // E2�޽��Ă�E4�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        } else if (G_E_GsktAna.SubString(2, 1) == "0" &&
                   G_E_GsktAna.SubString(3, 1) == "0") {
            // E2�޽��Ă�E3�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        } else if (s_CLabel == s_ALabel ||
                  (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
            // E2�޽��Ă�E4�޽��Ă�������E2�޽��Ă�E3�޽��Ă�����̏ꍇ�̐��ʂŏo��
            // (D��ڰĶ޽���)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // 2019.08.27 E�޽��Đ��ʒ���_E
        // E1:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
            P_HEADS_DATA[835].TrimRight());
        s_DLabel = P_PLATECD[1];
        // 2013.04.03 �ގ���r�ǉ�
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //if (G_D_GsktAna.SubString(1, 1) != "0") {
        if (G_E_GsktAna.SubString(1, 1) != "0") {
        // 2019.08.27 E�޽��Đ��ʒ���_E
            // �i�c�v���[�g�K�X�P�b�g�j
            s_D_GMat = s_B_GSKT_Mat;
        }
        else {
            // �i�d�v���[�g�K�X�P�b�g�j
            s_D_GMat = "2019";
        }
        // �^���ƃR�[�h�̔�r
        // 2019.08.27 E�޽��Đ��ʒ���_S
        //// if( s_DLabel == s_ALabel || s_DLabel == s_BLabel || s_DLabel == s_CLabel ){
        //// //E1�޽��Ă�E4�޽��Ă�������E1�޽��Ă�E3�޽��Ă�������E1�޽��Ă�E2�޽��Ă�����̏ꍇ�̐��ʂŏo��
        //// if( G_E_GsktAna.SubString(1,1) != "0" ){
        //// //�i�c�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret);                       // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); // HEARTS�p�ް�CSV�쐬
        //// }else{
        //// //�i�d�v���[�g�K�X�P�b�g�j
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //// Write_HeartsBuhin_Data("2019");  // HEARTS�p�ް�CSV�쐬
        //// }
        //// }
        //if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
        //    s_DLabel == s_BLabel ||
        //    (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
        //    // E1�޽��Ă�E4�޽��Ă�������E1�޽��Ă�E3�޽��Ă�������E1�޽��Ă�E2�޽��Ă�����̏ꍇ�̐��ʂŏo��
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        //    Write_HeartsBuhin_Data(s_D_GMat); // HEARTS�p�ް�CSV�쐬
        //}
        //// ***********************
        if (G_E_GsktAna.SubString(1, 1) == "0" &&
            G_E_GsktAna.SubString(4, 1) == "0") {
            // E1�޽��Ă�E4�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        } else if (G_E_GsktAna.SubString(1, 1) == "0" &&
                   G_E_GsktAna.SubString(3, 1) == "0") {
            // E1�޽��Ă�E3�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        } else if (G_E_GsktAna.SubString(1, 1) == "0" &&
                   G_E_GsktAna.SubString(2, 1) == "0") {
            // E1�޽��Ă�E2�޽��Ă�E��ڰ¶޽��Ă̏ꍇ�̐��ʂŏo��
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS�p�ް�CSV�쐬
        } else if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
                    s_DLabel == s_BLabel ||
                   (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
            // E1�޽��Ă�E4�޽��Ă�������E1�޽��Ă�E3�޽��Ă�������E1�޽��Ă�E2�޽��Ă�����̏ꍇ�̐��ʂŏo��
            // (D��ڰĶ޽���)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_D_GMat); // HEARTS�p�ް�CSV�쐬
        }
        // 2019.08.27 E�޽��Đ��ʒ���_E
    }
    // �f�B�X�^���X�s�[�X�o��
    Write_E_Gskt_Data_Distance();

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �v�w��p���ԃv���[�g�K�X�P�b�g���i�@�ް��쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2006.01.10 �T�u���[�`���ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_WXM_Gskt_Data(void) {

    AnsiString s_Text, s_Model, s_Key, s_Plate, s_Ring, s_PlateC, s_RingC;
    int i_Mojisu;
    int ret;
    int iSuryo;
    int iMod;

    // 2009.02.16 �߰�ؽĂŎg�p����̂�DE_Kubun��"I"���L�q������ٰ�݂ɕς���
    DE_Kubun = "I";
    // **********************************************************************

    // **********************************
    // ***  ��ڰĕ��i�\��Ͻ��@����
    // **********************************
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADS�ް� ����
    // 2009.06.19 UX-100R�ǉ�
    // EX�̏ꍇ
    // s_Model��[����"C","P","R","A","B"������ꍇ
    // s_Key��G_K_Model1�̌��ɂ��̕�����ǉ�����
    //
    // EX�ȊO�̏ꍇ
    // s_Model��[����"C","P","A","B"������ꍇ
    // s_Key��G_K_Model1�̌��ɂ��̕�����ǉ�����
    // �A��s_Model�̍Ō�̕�����"D"�̏ꍇ
    // s_Key��G_K_Model1��"D"�̑O�̈ʒu��"C","P","A","B"����ǉ�����
    ////2006.02.06 SX-90M,L�ǉ�
    ////P_HEADS_DATA[35]��[����"C","P","R","A","B"������ꍇ
    ////s_Key��G_K_Model1�̌��ɂ��̕�����ǉ�����
    ////2008.07.24 CX-10D�ǉ�
    ////�A��P_HEADS_DATA[35]�̍Ō�̕�����"D"�̏ꍇ
    ////s_Key��G_K_Model1��"D"�̑O�̈ʒu��"C","P","R","A","B"����ǉ�����
    ////*********************
    //
    ////2008.07.24 CX-10D�ǉ�
    ////s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    ////***********************
    // if ( s_Model.SubString(s_Model.Length(),1) == "D" ) {
    // s_Text = G_K_Model1.SubString(1,G_K_Model1.Length()-1) + s_Text + "D";
    // s_Key = G_K_Syurui + "-" + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    ////*********************
    // **********************

    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    //
    // if (s_Text.ToIntDef(999) != 999){
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R�ǉ�
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (G_K_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D�ǉ�
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = G_K_Model1.SubString(1, G_K_Model1.Length() - 1) +
            s_Text + "D";
        s_Key = G_K_Syurui + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    // �޽��Ă̈ʒu
    if (P_HEADS_DATA[243].ToIntDef(0) == 0) {
        if (P_HEADS_DATA[137].ToIntDef(0) == 0) {
            s_Plate = "A";
            s_Ring = "B";
            s_PlateC = P_HEADS_DATA[836].TrimRight();
            s_RingC = P_HEADS_DATA[835].TrimRight();
        }
        else {
            s_Plate = "B";
            s_Ring = "A";
            s_PlateC = P_HEADS_DATA[835].TrimRight();
            s_RingC = P_HEADS_DATA[836].TrimRight();
        }
    }
    else {
        if (P_HEADS_DATA[137].ToIntDef(0) == 0) {
            s_Plate = "B";
            s_Ring = "A";
            s_PlateC = P_HEADS_DATA[835].TrimRight();
            s_RingC = P_HEADS_DATA[836].TrimRight();
        }
        else {
            s_Plate = "A";
            s_Ring = "B";
            s_PlateC = P_HEADS_DATA[836].TrimRight();
            s_RingC = P_HEADS_DATA[835].TrimRight();
        }
    }

    // �n�ڑ��̶޽���(�ݸ�)
    // 2006.06.30 ���ʕύX�ATCG�ǉ�
    ////�}�X�^�����֐�
    // ret = Search_HD_PLATE_3(P_SoSyoAscii , "I", "97", s_RingC);
    //
    ////���ʕ␳
    // if ( ret == true ) {
    // P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
    // iSuryo = int(P_Suryo);
    // iMod = iSuryo - int(iSuryo / 2) * 2;
    // if(iMod == 0){
    // //���� ���ʁ��i���ʁ|�Q�j�^�Q
    // P_Suryo = (double)(P_Suryo - 2) * 0.5;
    // } else {
    // //� ���ʁ��i���ʁ|�P�j�^�Q
    // P_Suryo = (double)(P_Suryo - 1) * 0.5;
    // }
    // P_PLATECD[1] = "IRING-";
    // if ( G_KS_Model == "�P�P" || G_KS_Model == "�P�R") {
    // P_PLATECD[3] = "�����O�K�X�P�b�g";
    // } else {
    // P_PLATECD[3] = "�v���[�g�K�X�P�b�g�a";
    // }
    // //�o�͂���B
    // P_Yobisu = Syukei_Buhin_Yobisu("GSKT44",s_RingC);
    // if (s_Ring == "A") {
    // P_Yobisu = P_Yobisu + P_PltGsktYobiA;
    // } else {
    // P_Yobisu = P_Yobisu + P_PltGsktYobiB;
    // }
    // P_Unit = "��";
    // P_SideDiv = s_Ring;
    // Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
    // Write_HeartsBuhin_Data(s_RingC); // HEARTS�p�ް�CSV�쐬
    // }

    // 2009.12.01 WX-11 �ݸ޶޽���L,�ݸ޶޽���R�Ēǉ�
    ////2006.12.04 WX-11 �ݸ޶޽���L,�ݸ޶޽���R�폜
    ////if ( ( G_KS_Model == "�P�P" || G_KS_Model == "�P�R" ) &&
    // if ( ( G_KS_Model == "�P�R" ) &&
    // ( s_RingC == "2043" || s_RingC == "2060" || s_RingC == "2061" ||
    // s_RingC == "2062" || s_RingC == "2063" || s_RingC == "2064" ||
    // s_RingC == "2065" || s_RingC == "2071" ) ) {
    ////********************************************
    if (((G_KS_Model == "�P�R") && (s_RingC == "2043" || s_RingC == "2060" ||
        s_RingC == "2061" || s_RingC == "2062" || s_RingC == "2063" ||
        s_RingC == "2064" || s_RingC == "2065" || s_RingC == "2071")) ||
        (G_KS_Model == "�P�P")) {
        // **********************************************
        // �����O�K�X�P�b�g�k
        // �}�X�^�����֐�
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "98", s_RingC);

        // ���ʕ␳
        if (ret == true) {
            P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
            iSuryo = int(P_Suryo);
            iMod = iSuryo -int(iSuryo / 2) * 2;
            if (iMod == 0) {
                // ���� ���ʁ��i���ʁ|�Q�j�^�Q
                P_Suryo = (double)(P_Suryo - 2) * 0.5;
            }
            else {
                // � ���ʁ��i���ʁ|�P�j�^�Q
                P_Suryo = (double)(P_Suryo - 1) * 0.5;
            }
            P_PLATECD[1] = "IRINGL";
            P_PLATECD[3] = "�����O�K�X�P�b�g�k";
            // �o�͂���B
            P_Yobisu = Syukei_Buhin_Yobisu("GSKT45", s_RingC, m_pcHeadsData);
            if (s_Ring == "A") {
                P_Yobisu = P_Yobisu + P_PltGsktYobiB;
            }
            else {
                P_Yobisu = P_Yobisu + P_PltGsktYobiA;
            }
            P_Unit = "";
            P_SideDiv = s_Ring;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_RingC); // HEARTS�p�ް�CSV�쐬
        }

        // �����O�K�X�P�b�g�q
        // �}�X�^�����֐�
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "99", s_RingC);

        // ���ʕ␳
        if (ret == true) {
            P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
            iSuryo = int(P_Suryo);
            iMod = iSuryo -int(iSuryo / 2) * 2;
            if (iMod == 0) {
                // ���� ���ʁ��i���ʁ|�Q�j�^�Q
                P_Suryo = (double)(P_Suryo - 2) * 0.5;
            }
            else {
                // � ���ʁ��i���ʁ|�P�j�^�Q
                P_Suryo = (double)(P_Suryo - 1) * 0.5;
            }
            P_PLATECD[1] = "IRINGR";
            P_PLATECD[3] = "�����O�K�X�P�b�g�q";
            // �o�͂���B
            P_Yobisu = Syukei_Buhin_Yobisu("GSKT46", s_RingC, m_pcHeadsData);
            if (s_Ring == "A") {
                P_Yobisu = P_Yobisu + P_PltGsktYobiB;
            }
            else {
                P_Yobisu = P_Yobisu + P_PltGsktYobiA;
            }
            P_Unit = "";
            P_SideDiv = s_Ring;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_RingC); // HEARTS�p�ް�CSV�쐬
        }

    }
    else {

        // �����O�K�X�P�b�g�i�v���[�g�K�X�P�b�g�a�j
        // �}�X�^�����֐�
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "97", s_RingC);

        // ���ʕ␳
        if (ret == true) {
            P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
            iSuryo = int(P_Suryo);
            iMod = iSuryo -int(iSuryo / 2) * 2;
            if (iMod == 0) {
                // ���� ���ʁ��i���ʁ|�Q�j
                P_Suryo = (double)(P_Suryo - 2);
            }
            else {
                // � ���ʁ��i���ʁ|�P�j
                P_Suryo = (double)(P_Suryo - 1);
            }
            P_PLATECD[1] = "IRING-";
            if (G_KS_Model == "�P�P" || G_KS_Model == "�P�R") {
                P_PLATECD[3] = "�����O�K�X�P�b�g";
            }
            // 2021.09.22 WH-30�ǉ�_S �@�@
            else if (G_KS_Model == "�R�O") {
                P_PLATECD[3] = "�����O�K�X�P�b�g";
            }
            // 2021.09.22 WH-30�ǉ�_E
            else {
                P_PLATECD[3] = "�v���[�g�K�X�P�b�g�a";
            }
            // �o�͂���B
            P_Yobisu = Syukei_Buhin_Yobisu("GSKT44", s_RingC, m_pcHeadsData);
            if (s_Ring == "A") {
                P_Yobisu = P_Yobisu + P_PltGsktYobiB * 2;
            }
            else {
                P_Yobisu = P_Yobisu + P_PltGsktYobiA * 2;
            }
            P_Unit = "";
            P_SideDiv = s_Ring;
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_HeartsBuhin_Data(s_RingC); // HEARTS�p�ް�CSV�쐬
        }
    }
    // ****************************

    // �n�ڑ��̶޽���(��ڰ�)
    // �}�X�^�����֐�
    ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "30", s_PlateC);

    // ���ʕ␳
    if (ret == true) {
        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
        iSuryo = int(P_Suryo);
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // ���� ���ʁ��i���ʁ|�Q�j�^�Q
            P_Suryo = (double)(P_Suryo - 2) * 0.5;
        }
        else {
            // � ���ʁ��i���ʁ|�P�j�^�Q
            P_Suryo = (double)(P_Suryo - 1) * 0.5;
        }
        P_PLATECD[1] = "IPGS--";
        if (G_KS_Model == "�P�P" || G_KS_Model == "�P�R") {
            P_PLATECD[3] = "�v���[�g�K�X�P�b�g";
        }
        // 2021.10.13 WH-30�ǉ�_S �@�@
        else if (G_KS_Model == "�R�O") {
            P_PLATECD[3] = "�v���[�g�K�X�P�b�g";
        }
        // 2021.10.13 WH-30�ǉ�_E
        else {
            P_PLATECD[3] = "�v���[�g�K�X�P�b�g�`";
        }
        // �o�͂���B
        P_Yobisu = Syukei_Buhin_Yobisu("GSKT01", s_PlateC, m_pcHeadsData);
        if (s_Plate == "A") {
            P_Yobisu = P_Yobisu + P_PltGsktYobiA;
        }
        else {
            P_Yobisu = P_Yobisu + P_PltGsktYobiB;
        }
        P_Unit = "";
        P_SideDiv = s_Plate;
        Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
        Write_HeartsBuhin_Data(s_PlateC); // HEARTS�p�ް�CSV�쐬
    }

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �v�w�V���[�Y��p�d�J�Z�b�g�v���[�g�K�X�P�b�g���i�@�ް��쐬
// �T  �v      �F �d�J�Z�b�g�̃K�X�P�b�g�͍ގ����`�a�t�ɂȂ��Ă���
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2006.01.10 �T�u���[�`���ǉ�
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_WX_CE_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    ret = Search_HD_E_GASKET(G_E_GsktType, G_GsktType, G_FrmDiv);

    DE_Kubun = "E";
    // �p�^�[������ɂ��K�v�ȃK�X�P�b�g��I��
    // A�X�^�[�g
    if (G_E_GsktABKubun == "A") {
        // E1:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
            P_HEADS_DATA[836].TrimRight());
        s_BLabel = P_PLATECD[1];
        // E2:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
            P_HEADS_DATA[835].TrimRight());
        s_ALabel = P_PLATECD[1];
        // �^���ƃR�[�h�̔�r
        // �E�ɂ���r
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,
            1) != "0" && G_E_GsktAna.SubString(2,
            1) != "0" && P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836]
            .TrimRight()) {
            // E1�E2�E&A,B���ގ��̐��ʂŏo��
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // P_SideDiv ="";
            P_Suryo = EPUH_Meq_Sub;
            P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS�p�ް�CSV�쐬
        }
        if (s_ALabel == s_BLabel && P_HEADS_DATA[835].TrimRight()
            == P_HEADS_DATA[836].TrimRight()) {
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // A,B���ގ��̐��ʂŏo��
            // P_SideDiv ="";
            P_Suryo = EPU_Meq_Sub;
            P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS�p�ް�CSV�쐬
        }
        // E3:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
            P_HEADS_DATA[836].TrimRight());
        s_BLabel = P_PLATECD[1];
        // E4:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
            P_HEADS_DATA[835].TrimRight());
        s_ALabel = P_PLATECD[1];
        // �^���ƃR�[�h�̔�r
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,
            1) != "0" && G_E_GsktAna.SubString(4,
            1) != "0" && P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836]
            .TrimRight()) {
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // E3�E4�E&A,B���ގ��̐��ʂŏo��
            // P_SideDiv ="";
            P_Suryo = EPLH_Meq_Sub;
            P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight());
            // HEARTS�p�ް�CSV�쐬
        }
    }
    // B�X�^�[�g
    else {
        // E4:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
            P_HEADS_DATA[835].TrimRight());
        s_BLabel = P_PLATECD[1];
        // E3:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
            P_HEADS_DATA[836].TrimRight());
        s_ALabel = P_PLATECD[1];
        // �^���ƃR�[�h�̔�r
        // �E�ɂ���r
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,
            1) != "0" && G_E_GsktAna.SubString(4,
            1) != "0" && P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836]
            .TrimRight()) {
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // E3�E4�E&A,B���ގ��̐��ʂŏo��
            // P_SideDiv ="";
            P_Suryo = EPUH_Meq_Sub;
            P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS�p�ް�CSV�쐬
        }
        if (s_ALabel == s_BLabel && P_HEADS_DATA[835].TrimRight()
            == P_HEADS_DATA[836].TrimRight()) {
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // A,B���ގ��̐��ʂŏo��
            // P_SideDiv ="";
            P_Suryo = EPU_Meq_Sub;
            P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS�p�ް�CSV�쐬
        }
        // E2:A���̏���
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
            P_HEADS_DATA[835].TrimRight());
        s_BLabel = P_PLATECD[1];
        // E1:B���̏���
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
            P_HEADS_DATA[836].TrimRight());
        s_ALabel = P_PLATECD[1];
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,
            1) != "0" && G_E_GsktAna.SubString(2,
            1) != "0" && P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836]
            .TrimRight()) {
            // 2006.07.13 �߰�ؽĂŎg�p����̂�DE_Kubun��P_SideDiv���L�q������ٰ�݂ɕς���
            // E1�E2�E&A,B���ގ��̐��ʂŏo��
            // P_SideDiv ="";
            P_Suryo = EPLH_Meq_Sub;
            P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret); // ��ڰĕ��i�ް�CSV�쐬
            Write_PlateBuhin_Data_GSKT(ret); // ��ڰĕ��i�ް�CSV�쐬
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS�p�ް�CSV�쐬
        }
    }
    // �f�B�X�^���X�s�[�X�o��
    Write_E_Gskt_Data_Distance();

    return true;
}