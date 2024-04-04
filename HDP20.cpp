// ---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�F�o�}�v����M�T�[�r�X(���i�W�J����)
// �� �� �ҁFN.Uchida
// �� �� ���F2002.06.18
// �X �V ���F2002.08.23
// 2007.10.04 �װ�d�l�Ή�
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>

#include <dir.h>
#include <io.h>
#include <direct.h>

#pragma hdrstop

#include <sys\stat.h>

#include "HDPConst.h"         // �萔��`�p
#include "HDPValue.h"         // �ϐ���`�p
#include "HDPCom.h"           // �֐���`�p

#include "DataModule.h"

#include "HDP20.h"
#include "Bas_PlateDetails.h"    // ��ڰĕ��i�W�J����
#include "Bas_FrameDetails.h"    // �ڰѕ��i�W�J����
#include "Bas_NozzleDetails.h"   // ɽ�ٕ��i�W�J����
#include "Bas_TempDetails.h"     // ���x�v���i�W�J����
#include "Bas_PressGDetails.h"   // ���͌v���i�W�J����
#include "Bas_AxsrsDetails.h"    // �t���i���i�W�J����

#include "HeadsData.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmHDP020 *frmHDP020;

// ---------------------------------------------------------------------------
__fastcall TfrmHDP020::TfrmHDP020(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
// ���{��֐����F �E�B���h�E����������
// �T  �v      �F �\��������������
// ��  ��      �F �����l
// �߂�l      �F �Ȃ�
// ��  �l      �F ���ѐݒ�Ǎ���
// ---------------------------------------------------------------------------
void __fastcall TfrmHDP020::FormCreate(TObject *Sender) {
    // INI�ݒ�t�@�C���̓ǂݍ���
    IniFileRead();
    IniFileRead_Parts();

    // *************************
    // ***  ۸�̧�ف@��������
    // *************************
    if (!Log_File_SizeCheck()) {
        return;
    }

    // ۸ލ쐬
    G_Log_Text =
        "*****************************************************************************";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    G_Log_Text =
        "*****                                                                   *****";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    G_Log_Text = "*****                  ���i�W�J�����V�X�e�����N�����܂����B             *****";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    G_Log_Text =
        "*****                                                                   *****";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    G_Log_Text =
        "*****************************************************************************";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // ***************************
    // ***  ���i�W�J�����@�J�n
    // ***************************
    Heads_Parts_Main();

    Screen->Cursor = crDefault;
//    PostMessage(Handle, WM_CLOSE, 0, 0);

//    Release(); // �t�H�[����j�����C�֘A�t�����Ă��郁������������܂��B

    Application->Terminate();
}

// ---------------------------------------------------------------------------
// ���{��֐����F
// �T  �v      �F
// ��  ��      �F �����l
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
void __fastcall TfrmHDP020::Heads_Parts_Main(void) {
    AnsiString s_FilePass;
    AnsiString hs_FilePass;
    bool bRet;
    HeadsData* pcHeadsDataNone;
    HeadsData* pcHeadsDataMrr;
    HeadsData* pcHeadsData;
    int i;
    int j;

    try {
        Refresh();
        Screen->Cursor = crHourGlass;

        // *************************
        // ***  ��Ԏ擾
        // *************************
        if (!Get_Sakuban()) {
            return;
        }

        // **********************************
        // ***  �K�i����,�ƊE,�h���F  �Ǎ�
        // **********************************
        if (!Read_CodeMasta_Data()) {
            return;
        }

        // *************************
        // ***  �ް��ް��ڑ�
        // *************************
        if (!Connect_DataBase()) {
            return;
        }

        // ******************************
        // ***  CSV̧�ُo�͐�̫�������
        // ******************************
        if (P_Sakuban_Mode == "S") {
            // ���ް
            G_csv_Output_Dir = G_csv_Output_Dir + G_OUT_ORDER_Dir;

            // ��ڰčE���ް��i�[��
            // �}���`�T�[�o�Ή� 2009/1/13 MOD START
            // G_AnaFilePass = IncludeTrailingBackslash(G_HD_Root)
            // + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
            // + IncludeTrailingBackslash(G_DAT_ORDER_Dir);
            G_AnaFilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir) +
                IncludeTrailingBackslash(G_DAT_ORDER_Dir);
            // �}���`�T�[�o�Ή� 2009/1/13 MOD END

            // �\���i�ް��Ǎ���
            // G_YobiFilePass = G_Input_Csv_Dir;
        }
        else {
            // ����
            G_csv_Output_Dir = G_csv_Output_Dir + G_OUT_ESTIM_Dir;

            // ��ڰčE���ް��i�[��
            // �}���`�T�[�o�Ή� 2009/1/13 MOD START
            // G_AnaFilePass = IncludeTrailingBackslash(G_HD_Root)
            // + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
            // + IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
            G_AnaFilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir) +
                IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
            // �}���`�T�[�o�Ή� 2009/1/13 MOD END

            // �\���i�ް��Ǎ���
            // G_YobiFilePass = G_Input_Csv_Dir;
        }

        // ���i�W�J�ް��o�͐�
        G_csv_Output_Dir = IncludeTrailingBackslash(G_csv_Output_Dir) +
            IncludeTrailingBackslash(G_SAKUBAN);

        // ��ڰčE���ް��i�[��
        G_AnaFilePass = G_AnaFilePass + IncludeTrailingBackslash(G_SAKUBAN);

        // �\���i�ް��i�[��
        // G_YobiFilePass =
        if (!OutPut_Folder_Check(G_csv_Output_Dir)) {
            return;
        }

        // *******************************************
        // ***  HEADS�ް��@�Ǎ�
        // ***  ( 02.08.07 �Ǎ�̧�ٖ��ύX ) csv��txt
        // *******************************************
        // s_FilePass = G_Input_Csv_Dir + G_HEADS_ID;
        s_FilePass = G_Input_Txt_Dir + G_HEADS_ID;

        // ------------------------------------------------------------------
        // 2007.10.02 �װ�d�l�Ή� �ő�3̧�ٓǍ�
        // �ϐ�G_HEADS_DATA���g�p�����AHeadsData�׽�ϐ����g�p��������ɕύX
        /*
         if (!Read_HEADS_Data(s_FilePass)){
         G_ErrLog_Text = "HEADS�ް��w" + s_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
         Write_Error_Log(G_ErrLog_Text);
         return;
         } else {
         G_Log_Text = "HEADS�ް��w" + s_FilePass + "�x��ǂݍ��݂܂����B";
         Write_Log(G_Log_Text);
         }
         */

        // ���d�l�p��HEADS�e�L�X�g�f�[�^
        pcHeadsDataNone = new HeadsData();

        // HEADS�e�L�X�g�f�[�^(���d�l)�Ǎ�
        bRet = pcHeadsDataNone->ReadAllData(s_FilePass.c_str());

        if (!bRet) {
            G_ErrLog_Text = "HEADS�ް��w" + s_FilePass + "�x(���d�l)�̂n�o�d�m�Ɏ��s���܂����I�I";
            Write_Error_Log(G_ErrLog_Text);
            return;
        }
        else {
            G_Log_Text = "HEADS�ް��w" + s_FilePass + "�x(���d�l)��ǂݍ��݂܂����B";
            Write_Log(G_Log_Text);
        }

        // �~���[�d�l�̏ꍇ�́AA�AB�t�@�C�����ǂݍ���
        if (pcHeadsDataNone->GetMirrorFlag()) {

            // �~���[�d�l�p��HEADS�e�L�X�g�f�[�^
            pcHeadsDataMrr = new HeadsData[2];

            // HEADS�e�L�X�g�f�[�^(�~���[A�d�l)�Ǎ�
            G_HEADS_ID = G_SAKUBAN + "A" + ".txt";
            s_FilePass = G_Input_Txt_Dir + G_HEADS_ID;
            bRet = pcHeadsDataMrr[0].ReadAllData(s_FilePass.c_str());

            if (!bRet) {
                G_ErrLog_Text = "HEADS�ް��w" + s_FilePass +
                    "�x(�װA�d�l)�̂n�o�d�m�Ɏ��s���܂����I�I";
                Write_Error_Log(G_ErrLog_Text);
                return;
            }
            else {
                G_Log_Text = "HEADS�ް��w" + s_FilePass + "�x(�װA�d�l)��ǂݍ��݂܂����B";
                Write_Log(G_Log_Text);
            }

            // HEADS�e�L�X�g�f�[�^(�~���[B�d�l)�Ǎ�
            G_HEADS_ID = G_SAKUBAN + "B" + ".txt";
            s_FilePass = G_Input_Txt_Dir + G_HEADS_ID;
            bRet = pcHeadsDataMrr[1].ReadAllData(s_FilePass.c_str());

            if (!bRet) {
                G_ErrLog_Text = "HEADS�ް��w" + s_FilePass +
                    "�x(�װB�d�l)�̂n�o�d�m�Ɏ��s���܂����I�I";
                Write_Error_Log(G_ErrLog_Text);
                return;
            }
            else {
                G_Log_Text = "HEADS�ް��w" + s_FilePass + "�x(�װB�d�l)��ǂݍ��݂܂����B";
                Write_Log(G_Log_Text);
            }
        }

        // 2007.10.02 �װ�d�l�Ή� �ő�3̧�ٓǍ�
        // ------------------------------------------------------------------

        // *************************
        // ***  HD_PARTS�̍폜
        // *************************
        s_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
        DeleteFile(s_FilePass);

        // ****************************
        // ***  HEARTS�p�ް��폜
        // ****************************
        if (AllNumChk(G_SAKUBAN.c_str(), 0) == 0) {
            hs_FilePass = IncludeTrailingBackslash
                (G_xls_Output_Dir + G_OUT_ORDER_Dir);
            hs_FilePass = IncludeTrailingBackslash(hs_FilePass + G_SAKUBAN);
            hs_FilePass = hs_FilePass + G_SAKUBAN + ".csv";

            // hs_FilePass = G_xls_Output_Dir + G_OUT_ORDER_Dir
            // + "\\" + G_SAKUBAN + "\\"
            // + G_SAKUBAN + ".csv";
        }
        else {
            hs_FilePass = IncludeTrailingBackslash
                (G_xls_Output_Dir + G_OUT_ESTIM_Dir);
            hs_FilePass = IncludeTrailingBackslash(hs_FilePass + G_SAKUBAN);
            hs_FilePass = hs_FilePass + G_SAKUBAN + ".csv";

            // hs_FilePass = G_xls_Output_Dir + G_OUT_ESTIM_Dir
            // + "\\" + G_SAKUBAN + "\\"
            // + G_SAKUBAN + ".csv";
        }
        DeleteFile(hs_FilePass);

        // ---------------------------------------------
        // 2007.10.02 �װ�d�l�Ή�
        // HEADS÷��̧�ِ���ٰ�߂���

        for (i = 0; i < 3; i++) {

            // �\���i�d����h�~�p����������
            G_YOBICHK_RecCnt = 0;

            // 2007.10.03 �ǉ�
            // �O�̂��ߗ\���i�d����h�~�p�����z���������
            for (j = 0; j < 100; j++) {
                G_YOBICHK_NAME[j] = "";
                G_YOBICHK_MAT[j] = "";
            }

            // --------------------------
            // ��������d�l�̕ϐ����
            // --------------------------
            switch (i) {
            case 0:
                pcHeadsData = pcHeadsDataNone; // ���d�l
                break;
            case 1:
                pcHeadsData = &pcHeadsDataMrr[0]; // �װA�d�l
                break;
            case 2:
                pcHeadsData = &pcHeadsDataMrr[1]; // �װB�d�l
                break;
            }

            // �d�l���߁A�d�l���ߺ��ނ��
            pcHeadsData->SetSpecType(i);
            pcHeadsData->SetSpecCode();

            // �װ�d�l�̏ꍇ�A���d�l�̏����͍s��Ȃ�
            if ((i == 0) && (pcHeadsDataNone->GetMirrorFlag()))
                continue;

            // *************************************
            // ***  ��ڰĕ��i�@�W�J����
            // *************************************
            if (!PlateDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  �ڰѕ��i�@�W�J����
            // *************************************
            if (!FrameDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  ɽ�ٕ��i�@�W�J����
            // *************************************
            if (!NozzleDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  ���x�v���i�@�W�J����
            // *************************************
            if (!TempDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  ���͌v���i�@�W�J����
            // *************************************
            if (!PressGDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  �t���i���i�@�W�J����
            // *************************************
            if (!AxsrsDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // �װ�d�l�łȂ��ꍇ�A���d�l�̂ݏ������Ĕ�����
            if (!pcHeadsDataNone->GetMirrorFlag())
                break;
        }

        if (pcHeadsDataNone->GetMirrorFlag())
            delete[]pcHeadsDataMrr;
        delete pcHeadsDataNone;

        // 2007.10.02 �װ�d�l�Ή�
        //
        // ---------------------------------------------

        delete DataModule1;

        // ۸ލ쐬
        G_Log_Text = "���i�W�J�����V�X�e�����I�����܂����B\n\n\n\n\n";
        Write_Log(G_Log_Text);
        Write_Error_Log(G_Log_Text);
    }
    __finally {
        Screen->Cursor = crDefault;
        Close();
        // PostMessage(Handle, WM_CLOSE, 0,0);
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���O�t�@�C���@�T�C�Y�`�F�b�N
// �T  �v      �F ۸�̧�ق̻��ނ��擾���A�K��l�𒴂��Ă�����ߋ�̫��ނɈړ�������
// ��  ��      �F �Ȃ�
// �߂�l      �F ���� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::Log_File_SizeCheck(void) {
    AnsiString to_filename; // �쐬�ߋ�۸�̧��(���߽)
    struct stat statbuf;
    double d_FileSize;

    // ***************************
    // ***  ۸�̧�ف@��������
    // ***************************
    // �쐬��۸�̧�ف@��������
    stat(G_LogFilePath.c_str(), &statbuf);
    d_FileSize = statbuf.st_size;

    // ۸�̧�ق̻��ނ��ݒ�l�ȏ�Ȃ�w�N���������b�x+̧�ٖ��ŉߋ�۸�̫��ނֺ�߰
    if (d_FileSize >= G_Log_Max_Size) {

        // �ߋ�۸ޗp�߽�쐬
        to_filename = G_Rsrv_Log_Dir;
        to_filename = to_filename + FormatDateTime("yyyymmdd", Date());
        to_filename = to_filename + FormatDateTime("hhnnss", Time()) +
            CSYSLOGFILENAME_P;

        if (CopyFile(G_LogFilePath.c_str(), to_filename.c_str(), false)) {
            DeleteFile(G_LogFilePath.c_str());
        }
        else {
            return false;
        }
    }

    // ***************************
    // ***  �װ۸�̧�ف@��������
    // ***************************
    // �쐬��װ۸�̧�ف@��������
    stat(G_ErrLogFilePath.c_str(), &statbuf);
    d_FileSize = statbuf.st_size;

    // �װ۸�̧�ق̻��ނ��ݒ�l�ȏ�Ȃ�w�N���������b�x+̧�ٖ��ŉߋ��װ۸�̫��ނֺ�߰
    if (d_FileSize >= G_ErrLog_Max_Size) {

        // �ߋ��װ۸ޗp�߽�쐬
        to_filename = G_Rsrv_ErrLog_Dir;
        to_filename = to_filename + FormatDateTime("yyyymmdd", Date());
        to_filename = to_filename + FormatDateTime("hhnnss", Time()) +
            CSYSERRLOGFILENAME_P;

        if (CopyFile(G_ErrLogFilePath.c_str(), to_filename.c_str(), false)) {
            DeleteFile(G_ErrLogFilePath.c_str());
        }
        else {
            return false;
        }
    }

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �K�i���ށC�ƊE���ށC�h���F�@�Ǎ�
// �T  �v      �F �wKIKAKU.CSV�x,�wGYOKAI.CSV�x,�wTOSOU.CSV�x��ǂݍ���
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::Read_CodeMasta_Data(void) {
    char buf[1024]; /* ������o�b�t�@ */
    char *tok; /* �g�[�N���|�C���^ */
    int i; /* ���[�v�J�E���^ */
    char token[100][100]; /* �g�[�N���o�b�t�@ */

    int RecNo;

    AnsiString wk_FilePass;
    FILE *fp;

    // ***********************
    // **  �K�i���� ���
    // ***********************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "KIKAKU.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // �g�[�N���J�E���^������
        memset(token, 0x00, sizeof(token)); // �g�[�N���o�b�t�@������
        tok = GetToken(buf, ","); // ","�ō��ڂ���؂�
        while (tok) { // �g�[�N�������݂����
            sprintf(token[i++], "%-.99s", tok); // �g�[�N�����o�b�t�@�ɓ]��
            tok = GetToken(NULL, ","); // ���̃g�[�N��������

            if (i > 70) {
                break;
            }
        }

        G_KIKAKU[RecNo][0] = token[0];
        G_KIKAKU[RecNo][1] = token[1];

        RecNo++;
    }

    fclose(fp); // ����

    G_KIKAKU_RecCnt = RecNo;

    // ***********************
    // **  �ƊE���� ���
    // ***********************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "GYOKAI.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // �g�[�N���J�E���^������
        memset(token, 0x00, sizeof(token)); // �g�[�N���o�b�t�@������
        tok = GetToken(buf, ","); // ","�ō��ڂ���؂�
        while (tok) { // �g�[�N�������݂����
            sprintf(token[i++], "%-.99s", tok); // �g�[�N�����o�b�t�@�ɓ]��
            tok = GetToken(NULL, ","); // ���̃g�[�N��������

            if (i > 70) {
                break;
            }
        }

        G_GYOKAI[RecNo][0] = token[0];
        G_GYOKAI[RecNo][1] = token[1];

        RecNo++;
    }

    fclose(fp); // ����

    G_GYOKAI_RecCnt = RecNo;

    // ***********************
    // **  �h���F ���
    // ***********************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "TOSOU.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // �g�[�N���J�E���^������
        memset(token, 0x00, sizeof(token)); // �g�[�N���o�b�t�@������
        tok = GetToken(buf, ","); // ","�ō��ڂ���؂�
        while (tok) { // �g�[�N�������݂����
            sprintf(token[i++], "%-.99s", tok); // �g�[�N�����o�b�t�@�ɓ]��
            tok = GetToken(NULL, ","); // ���̃g�[�N��������

            if (i > 70) {
                break;
            }
        }

        G_TOSOU[RecNo][0] = token[0];
        G_TOSOU[RecNo][1] = token[1];
        G_TOSOU[RecNo][2] = token[2];

        RecNo++;
    }

    fclose(fp); // ����

    G_TOSOU_RecCnt = RecNo;

    // ***************************
    // **  ��ڰČ^���ϊ�ð���
    // ***************************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "PHENKAN.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    RecNo = 0;
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

        G_PHENKAN[RecNo][0] = token[0];
        G_PHENKAN[RecNo][1] = token[1];
        G_PHENKAN[RecNo][2] = token[2];
        G_PHENKAN[RecNo][3] = token[3];
        G_PHENKAN[RecNo][4] = token[4];
        G_PHENKAN[RecNo][5] = token[5];
        G_PHENKAN[RecNo][6] = token[6];
        G_PHENKAN[RecNo][7] = token[7];
        G_PHENKAN[RecNo][8] = token[8];
        G_PHENKAN[RecNo][9] = token[9];

        RecNo++;
    }

    fclose(fp); // ����

    G_PHENKAN_RecCnt = RecNo;

    // ******************************
    // **  ɽ�ٍ\�����i���� ð���
    // ******************************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "NOZZLECD.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    RecNo = 0;
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

        G_NOZZLECD[RecNo][0] = token[0]; // ����
        G_NOZZLECD[RecNo][1] = token[1]; // ���i���޺���
        G_NOZZLECD[RecNo][2] = token[2]; // ���l(���i�敪)
        G_NOZZLECD[RecNo][3] = token[3]; // ���l(���i����)
        G_NOZZLECD[RecNo][4] = token[4]; // �\���i�W�v�p(?)
        G_NOZZLECD[RecNo][5] = token[5]; // �A��(?)

        RecNo++;
    }

    fclose(fp); // ����

    G_NOZZLECD_RecCnt = RecNo;

    // ******************************
    // **  ���x�v�\�����i���� ð���
    // ******************************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "TEMPCD.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    RecNo = 0;
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

        G_TEMPCD[RecNo][0] = token[0]; // ����
        G_TEMPCD[RecNo][1] = token[1]; // ���i���޺���
        G_TEMPCD[RecNo][2] = token[2]; // ���l(���i�敪)
        G_TEMPCD[RecNo][3] = token[3]; // ���l(���i����)
        G_TEMPCD[RecNo][4] = token[4]; // �\���i�W�v�p(?)
        G_TEMPCD[RecNo][5] = token[5]; // �A��(?)

        RecNo++;
    }

    fclose(fp); // ����

    G_TEMPCD_RecCnt = RecNo;

    // ******************************
    // **  ���͌v�\�����i���� ð���
    // ******************************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "PRESSGCD.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    RecNo = 0;
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

        G_PRESSCD[RecNo][0] = token[0]; // ����
        G_PRESSCD[RecNo][1] = token[1]; // ���ʎq
        G_PRESSCD[RecNo][2] = token[2]; // ���l(?)
        G_PRESSCD[RecNo][3] = token[3]; // ���i����
        G_PRESSCD[RecNo][4] = token[4]; // �\���i

        RecNo++;
    }

    fclose(fp); // ����

    G_PRESSCD_RecCnt = RecNo;

    // delete fp;
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �f�[�^�x�[�X�@�ڑ�
// �T  �v      �F mdb��ODBC�o�R�Őڑ�����
// ��  ��      �F �Ȃ�
// �߂�l      �F �ڑ����� [true:���� false:���s]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::Connect_DataBase(void) {
    try {
        // Access�ւ�ODBC�ڑ�
        DataModule1 = new TDataModule1(this);

        // �}�ԃf�[�^�x�[�X(HD_DNO.mdb)
        G_ErrLog_Text = "�}�ԃf�[�^�x�[�X(HD_DNO.mdb)";

        // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
        //DataModule1->Database_DNO->DatabaseName = ODBC_DSN_DNO;
        DataModule1->Database_DNO->ConnectionName = ODBC_DSN_DNO;
        // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
        DataModule1->Database_DNO->Params->Clear();
        DataModule1->Database_DNO->LoginPrompt = false;
        DataModule1->Database_DNO->Connected = true;

        G_Log_Text = "�}�ԃf�[�^�x�[�X(HD_DNO.mdb)�Ɛڑ����܂����B";
        Write_Log(G_Log_Text);

        // �����v�̏��f�[�^�x�[�X(HD_INSPCTL.mdb)
        G_ErrLog_Text = "�����v�̏��f�[�^�x�[�X(HD_INSPCTL.mdb)";

        // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
        //DataModule1->Database_MAT->DatabaseName = ODBC_DSN_INS;
        DataModule1->Database_MAT->ConnectionName = ODBC_DSN_INS;
        // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
        DataModule1->Database_MAT->Params->Clear();
        DataModule1->Database_MAT->LoginPrompt = false;
        DataModule1->Database_MAT->Connected = true;

        G_Log_Text = "�����v�̏��f�[�^�x�[�X(HD_INSPCTL.mdb)�Ɛڑ����܂����B";
        Write_Log(G_Log_Text);

        // �ގ��f�[�^�x�[�X(HD_MATERIAL.mdb)
        G_ErrLog_Text = "�ގ��f�[�^�x�[�X(HD_MATERIAL.mdb)";

        // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
        //DataModule1->Database_INS->DatabaseName = ODBC_DSN_MAT;
        DataModule1->Database_INS->ConnectionName = ODBC_DSN_MAT;
        // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
        DataModule1->Database_INS->Params->Clear();
        DataModule1->Database_INS->LoginPrompt = false;
        DataModule1->Database_INS->Connected = true;

        G_Log_Text = "�ގ��f�[�^�x�[�X(HD_MATERIAL.mdb)�Ɛڑ����܂����B";
        Write_Log(G_Log_Text);
    }
    catch (EDatabaseError &Error) {
        Write_Error_Log(Error.Message);
        G_ErrLog_Text = G_ErrLog_Text + "�ւ̐ڑ��Ɏ��s���܂����I�I";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ԁ@�擾
// �T  �v      �F �N�����Ɏ擾����p�����[�^����A��Ԃ��擾����B
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::Get_Sakuban(void) {
    // *******************
    // ***  ��Ԏ擾
    // *******************

    // ��Ԃ�HD_sakuban.txt����擾����悤�ύX�@2003-02-05
    G_SAKUBAN = Get_Sakuban_File();
    if (G_SAKUBAN == "") {
        G_ErrLog_Text = "HD_sakuban.txt����Ԃ��擾�ł��܂���ł����I�I";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        G_HEADS_ID = G_SAKUBAN + ".txt";
    }
    // �����ײ݂���̧��ID���擾
    // G_HEADS_ID = ParamStr(1);     // HEADS�ް� ̧��ID
    // //G_HEADS_ID = "984010.txt";
    // //G_HEADS_ID = "083953.txt";

    // // ̧��ID�����Ԃ��擾
    // int i_Pos;
    //
    // i_Pos = G_HEADS_ID.Pos(".");
    // if (i_Pos != 0){
    // G_SAKUBAN = G_HEADS_ID.SubString(1, i_Pos-1);
    // } else {
    // G_ErrLog_Text = "����(" + G_HEADS_ID + ")�ɂ���Ԃ��擾�ł��܂���ł����I�I";
    // Write_Error_Log(G_ErrLog_Text);
    // return false;
    // }

    // ۸ލ쐬
    G_Log_Text = "�t�@�C���h�c�w" + G_HEADS_ID + "�x����A���(" + G_SAKUBAN + ")���擾���܂���";
    Write_Log(G_Log_Text);

    // *************************
    // ***  ��Ԏ�ށ@�擾
    // *************************
    if (AllNumChk(G_SAKUBAN.c_str(), 0) == 0) {
        P_Sakuban_Mode = "S";
    }
    else {
        P_Sakuban_Mode = "M";
    }

    // **************************************************************
    // 02.08.07�ǉ� HEADS����̫��ޥ̧�ٌ`���ύX csv��txt
    // �����ް� "C:\HEADS_ROOT\HEADS_DATA\ESTIMATE\D00001\D00001.txt"
    // �����ް� "C:\HEADS_ROOT\HEADS_DATA\ORDER\000001\000001.txt"
    // ***************************************************************
    if (P_Sakuban_Mode == "S") {
        // �}���`�T�[�o�Ή� 2009/1/13 MOD START
        // G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HD_Root)
        // + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
        // + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
        // + IncludeTrailingBackslash(G_SAKUBAN);
        G_Input_Txt_Dir = IncludeTrailingBackslash(G_HEADS_DATA_Dir) +
            IncludeTrailingBackslash(G_DAT_ORDER_Dir) + IncludeTrailingBackslash
            (G_SAKUBAN);
        // �}���`�T�[�o�Ή� 2009/1/13 MOD END
    }
    else if (P_Sakuban_Mode == "M") {
        // �}���`�T�[�o�Ή� 2009/1/13 MOD START
        // G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HD_Root)
        // + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
        // + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
        // + IncludeTrailingBackslash(G_SAKUBAN);
        G_Input_Txt_Dir = IncludeTrailingBackslash(G_HEADS_DATA_Dir) +
            IncludeTrailingBackslash(G_DAT_ESTIM_Dir) + IncludeTrailingBackslash
            (G_SAKUBAN);
        // �}���`�T�[�o�Ή� 2009/1/13 MOD END
    }

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �쐬�t�@�C���o�͐�t�H���_�@�`�F�b�N
// �T  �v      �F �����Ώۂ�̫��ނ��������A̫��ނ����݂��Ȃ���΍쐬����B
// ��  ��      �F AnsiString Check_Pass  // HEADS�o��̫��� + ���
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::OutPut_Folder_Check(AnsiString Check_Pass) {
    if (!DirectoryExists(Check_Pass)) {
        if (!ForceDirectories(Check_Pass)) {
            // �t�H���_�쐬�@���s
            G_ErrLog_Text = "�t�H���_�w " + Check_Pass + "�x���쐬�ł��܂���ł����I�I";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
        else {
            // �t�H���_�쐬�@����
            G_Log_Text = "�t�H���_�w " + Check_Pass + "�x��V�K�쐬���܂����B"; ;
            Write_Log(G_Log_Text);
        }
    }

    return true;
}

// -----------------------------------------------------------------------------
// ���{��֐����F Excel�N������
// �T�v        �F Excel�̋N�����s��
// �p�����^    �F bool  showflg   �N���\���t���O
// �߂�l      �F �N������[0:����N��,1:�װ]
// �@�\����    �F
// ���l        �F
// -----------------------------------------------------------------------------
short __fastcall TfrmHDP020::CallExcel(bool showflg) {
    try {
        if (exApplication.Type() != varDispatch) // Excel���N���H
        {
            // Excel��޼ު�Ă��쐬
            exApplication = Variant::CreateObject("Excel.Application");

            // Excel��޼ު�Ă��\���Ƃ���
            exApplication.Exec(PropertySet("Visible") << "false");
        }

        // Excel��޼ު�Ă�WorkBooks�ڸ��݂��擾
        exWorkbooks = exWorkCell.Exec(PropertyGet("Workbooks"));

        // Excel��޼ު�Ă�\��/��\���Ƃ���
        exApplication.Exec(PropertySet("Visible") << "false");

        // �x��ү���ނ�\�����Ȃ��ɂ���
        exApplication.Exec(PropertySet("DisplayAlerts") << "false");

        return 0; // �߂�l[0:����N��]
    }
    catch (...) {
        // �װ۸ޒǉ�
        G_ErrLog_Text = "Excel���N���ł��܂���ł����I�I";
        Write_Error_Log(G_ErrLog_Text);

        QuitExcel();
        return (-1); // �߂�l[-1:�N���ُ�]
    }
}

// -----------------------------------------------------------------------------
// ���{��֐����F Excel�I������
// �T�v        �F Excel�̏I�����s��
// �p�����^    �F
// �߂�l      �F
// �@�\����    �F
// ���l        �F
// -----------------------------------------------------------------------------
void __fastcall TfrmHDP020::QuitExcel(void) {
    // Variant�̊J��
    exWorkCell.Clear();
    exWorkbooks.Clear();
    exWorkbook.Clear();
    exWorksheets.Clear();
    exWorksheet.Clear();

    // Excel�I������
    exApplication.Exec(Function("Quit"));

    // Variant�̊J��
    exApplication.Clear();

}

// ---------------- <<< End of File >>> ---------------- //
