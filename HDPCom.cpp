// ----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�F���ʊ֐��p
// �� �� �ҁFN.Uchida
// �� �� ���F2002.05.23
// �X �V ���F2002.06.19
// 2007.10.04 �װ�d�l�Ή��@Search_KIKAKU_CODE�ASyukei_Buhin_Yobisu�Ɉ����ǉ�
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>

#pragma hdrstop

#include <math.h>
#include <direct.h>
#include <sys\stat.h>
#include <mbstring.h>

#ifndef LSI_C   /* Borland C++ 5.5 */
#include <mbstring.h>
#define CT_ANK          _MBC_SINGLE
#define CT_KJ1          _MBC_LEAD
#define CT_KJ2          _MBC_TRAIL
#define CT_ILGL         _MBC_ILLEGAL
#define chkctype(x, y)  _mbbtype(x, y)
#define iskana(x)       _ismbbkana(x)
#else           /* LSI C-86 */
#include <jctype.h>
#endif

#include <SysUtils.hpp>
#include <Filectrl.hpp>
#include <IniFiles.hpp>
#include <Registry.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "HeadsData.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
TColor G_ClNoEditColor = clLtGray;

// ---------------------------------------------------------------------------
// ���{��֐����F�ިڸ�����߽�ϊ�����
// �T  �v      �FINI���ݒ��ިڸ�؂� ���߽�ɕϊ�����
// ��  ��      �Fcurdir  �J�����g�f�B���N�g��
// dir     �ݒ�f�B���N�g��
// �߂�l      �F�Ȃ�
// ��  �l      �F
// ---------------------------------------------------------------------------
static AnsiString __fastcall IniDirToFullPath(AnsiString curdir, AnsiString dir)
{
    AnsiString WrkCurPath;
    AnsiString WrkSetPath;
    AnsiString DriveStr;
    int len;

    len = dir.Length();
    WrkCurPath = IncludeTrailingBackslash(curdir);
    WrkSetPath = WrkCurPath + dir;
    DriveStr = ExtractFileDrive(dir);
    if (len > 1) {
        if (dir[2] == ':') // �h���C�u�w�肠��H
        {
            WrkSetPath = dir;
        }
        else if (dir[1] == '\\') // �P�K�w�ォ��̑��΃p�X�H
        {
            if (dir[2] == '\\') // ��΃p�X�H
            {
                if (DriveStr.Length() >= len) {
                    WrkSetPath = ExtractFileDrive(WrkCurPath) + ":" +
                        dir.SubString(2, len - 1);
                }
                else {
                    WrkSetPath = dir;
                }
            }
            else // �P�K�w�ォ��̑��΃p�X�H
            {
                WrkSetPath = ExtractFileDir(ExtractFileDir(WrkCurPath)) + dir;
            }
        }
    }
    return IncludeTrailingBackslash(WrkSetPath);
}

// ---------------------------------------------------------------------------
// ���{��֐����FIniFile�̓ǂݍ��ݏ���
// �T  �v      �FINI̧�ٓǂݍ��݁A���l�� �Z�b�g����
// ��  ��      �F�Ȃ�
// �߂�l      �F�Ȃ�
// ��  �l      �F
// ---------------------------------------------------------------------------
void __fastcall IniFileRead(void) {
    AnsiString AExeDir; // ���ş�ق̃f�B���N�g��
    AnsiString AExeName;
    try {
        IniFileRead1(); // IniFile1�̓ǂݍ��ݏ���
    }
    __finally {
        // ���s��̧�ق̃f�B���N�g��
        AExeDir = ExtractFileDir(Application->ExeName);
        // ���s��̧�ق̃f�B���N�g��('\\'�Ȃ�)

        // --- ���ϐ��ɓW�J
        AExeName = ExtractFileName(Application->ExeName);

        // ۸�̧��(���߽)�C�װ۸�̧��(���߽)
        if (AExeName == "HD_PARTS.exe") {
            // ���i�W�J����
            G_LogFilePath = IncludeTrailingBackslash(G_Log_File_Dir) +
                CSYSLOGFILENAME_P;
            G_ErrLogFilePath = IncludeTrailingBackslash(G_ErrLog_File_Dir) +
                CSYSERRLOGFILENAME_P;
        }
        else {
            // Excel�쐬����
            G_LogFilePath = IncludeTrailingBackslash(G_Log_File_Dir) +
                CSYSLOGFILENAME;
            G_ErrLogFilePath = IncludeTrailingBackslash(G_ErrLog_File_Dir) +
                CSYSERRLOGFILENAME;
        }

        // ����ڰ�̫���
        G_Template_Dir = IncludeTrailingBackslash(G_HD_Root) +
            IncludeTrailingBackslash(G_HEADS_CV_Dir) + IncludeTrailingBackslash
            (G_TEMPLATE_Dir);

        // HEADS����̫��� + CSV̫���(HEADS�ް����i�[����Ă���B)
        G_Input_Csv_Dir = IncludeTrailingBackslash(G_HD_Root) +
            IncludeTrailingBackslash(G_HEADS_CV_Dir) + IncludeTrailingBackslash
            (G_MASTER_Dir);

        // HEADS�o�͐�̫��� (�� ��̏����ŵ��ް�����ς��𔻒f���A̫��ނ�t�^����B)
        // TfrmHDP010::FormActivate����ē��̏����Ŏ擾
        // �}���`�T�[�o�Ή� 2008/11/17 MOD START
        // G_xls_Output_Dir = IncludeTrailingBackslash(G_HD_Root)
        // + IncludeTrailingBackslash(G_HEADS_OUT_Dir);
        G_xls_Output_Dir = IncludeTrailingBackslash(G_HEADS_OUT_Dir);
        // �}���`�T�[�o�Ή� 2008/11/17 MOD END

        // �}���`�T�[�o�Ή� 2009/1/13 MOD START
        // G_csv_Output_Dir = IncludeTrailingBackslash(G_HD_Root)
        // + IncludeTrailingBackslash(G_HEADS_DATA_Dir);
        G_csv_Output_Dir = IncludeTrailingBackslash(G_HEADS_DATA_Dir);
        // �}���`�T�[�o�Ή� 2009/1/13 MOD END

    }
}

// ---------------------------------------------------------------------------
// ���{��֐����FIniFile1�̓ǂݍ��ݏ���
// �T  �v      �FINI̧��1�ǂݍ���
// ��  ��      �F�Ȃ�
// �߂�l      �F�Ȃ�
// ��  �l      �F���ݒ�t�@�C�� = HD_CV.INI
// ---------------------------------------------------------------------------
void __fastcall IniFileRead1(void) {
    TIniFile *ini; // INI̧�ٱ��� ��޼ު��
    AnsiString AIniPath; // �ݒ�̧���߽
    AnsiString AIniFile; // �ݒ�̧���߽ + ̧�ٖ�
    AnsiString ASection; // ����ݖ�
    AnsiString Awrk, Asub; // ��Ɨp
    AnsiString Amm, Add; // ��Ɨp m, d

    char inipath[MAX_PATH];

//    // ��ini�t�@�C���̃f�B���N�g��
//    GetSystemDirectory(inipath, sizeof(inipath));
//    AIniPath = inipath;

        AIniPath = ExtractFilePath(Application->ExeName);
        //AIniPath = "C:\\HEADS_ROOT01\\HEADS-CV\\";

    // ��ini�t�@�C���̂ւ̃t���p�X�t�@�C����
    AIniFile = AIniPath + CSYSINIFILENAME;

    // ini�t�@�C������ݒ���e��ǂݍ���
    ini = new TIniFile(AIniFile);

    // --- ��{���擾
    ASection = "GENERAL"; // --  �ǂݍ��ݾ���ݾ�� : ��{
    G_HD_Version = GetIniKey(ini->ReadString(ASection, "HD_VERSION", ""));
    // HEADS�ް�ޮ�
    G_Zuban_DB = GetIniKey(ini->ReadString(ASection, "ZUBAN_DBFILE", ""));
    // �}���ް��ް�̧�ٖ�
    G_Material_DB = GetIniKey(ini->ReadString(ASection, "MAT_CODE_FILE",
        "")); // �ގ��ް��ް�̧�ٖ�
    G_Inspect_DB = GetIniKey(ini->ReadString(ASection, "INSPECTDB", ""));
    // �����v�̏��ް��ް�̧�ٖ�
//Add_Str 2019/02/19 Ri
        // ����w����A4(��)�AA3(�V)�t���O�̎擾
        G_OLD_FLG = GetIniKey(ini->ReadString(ASection, "OLD_FLG", "")).ToIntDef(0);
//Add_End 2019/02/19 Ri
    // @@@@@@@@@@@@@@
    // 02.08.17�ǉ�
    // @@@@@@@@@@@@@@
    G_Output_Mode = GetIniKey(ini->ReadString(ASection, "DOC_OUTPUT_MODE", ""))
        .ToIntDef(0); // 0:�ł��o���敪,1:�e�X�g�p
    //2022.09.30 E�ڰ��ް��ǉ�_S
    G_EBASE_FLG = GetIniKey(ini->ReadString(ASection, "EFRAME_BASE_PLATE", ""))
        .ToIntDef(0); // 0:��,1:�L
    //2022.09.30 E�ڰ��ް��ǉ�_E

    // --- ���O���擾
    ASection = "LOG";
    // ۸�̧�� ̫���
    G_Log_File_Dir = GetIniKey(ini->ReadString(ASection, "LOG_FILE_DIR", ""));
    // �ߋ�۸�̧�� ̫���
    G_Rsrv_Log_Dir =
        GetIniKey(ini->ReadString(ASection, "RESERVE_LOG_DIR", ""));
    // ۸�̧�ٍő廲��(bytes)
    G_Log_Max_Size =
        StrToCurr(GetIniKey(ini->ReadString(ASection, "LOG_MAX_SIZE", "0")));
    // �װ۸�̧�� ̫���
    G_ErrLog_File_Dir =
        GetIniKey(ini->ReadString(ASection, "ERR_LOG_FILE_DIR", ""));
    // �ߋ��װ۸�̧�� ̫���
    G_Rsrv_ErrLog_Dir =
        GetIniKey(ini->ReadString(ASection, "RESERVE_ERR_LOG_DIR", ""));
    // �װ۸�̧�ٍő廲��(bytes)
    G_ErrLog_Max_Size =
        StrToCurr(GetIniKey(ini->ReadString(ASection, "ERR_LOG_MAX_SIZE",
        "0")));
    // EXCEL�������e۸ށ@�o�͐����׸�
    G_Log_Write_Flg =
        StrToInt(GetIniKey(ini->ReadString(ASection, "EXCEL_LOG_WRITE", "0")));

    // --- �t�H���_���擾
    ASection = "FOLDER";
    G_HD_Root = GetIniKey(ini->ReadString(ASection, "HD_ROOT", "")); // ٰ�̫���
    G_HEADS_CV_Dir = GetIniKey(ini->ReadString(ASection, "HEADS_CV", ""));
    // ����̫���
    G_MASTER_Dir = GetIniKey(ini->ReadString(ASection, "MASTER", ""));
    // Ͻ�̫���
    G_TEMPLATE_Dir = GetIniKey(ini->ReadString(ASection, "TEMPLATE", ""));
    // ����ڰ�̫���

    G_HEADS_INP_Dir = GetIniKey(ini->ReadString(ASection, "HEADS_INPUT",
        "")); // HEADS����̫���
    G_TRIGGER_Dir = GetIniKey(ini->ReadString(ASection, "TRIGGER", ""));
    // �ض�̫���
    G_CSV_Dir = GetIniKey(ini->ReadString(ASection, "CSV", "")); // CSV̫���
    G_MATERIAL_Dir = GetIniKey(ini->ReadString(ASection, "MATERIAL", ""));
    // �ގ�����Ͻ�̫���

    G_HEADS_OUT_Dir = GetIniKey(ini->ReadString(ASection, "HEADS_OUTPUT",
        "")); // HEADS�o��̫���
    G_OUT_ORDER_Dir = GetIniKey(ini->ReadString(ASection, "OUT_ORDER", ""));
    // ���ސ}��̫���
    G_OUT_ESTIM_Dir = GetIniKey(ini->ReadString(ASection, "OUT_ESTIMATE",
        "")); // ���ϐ}��̫���

    G_HEADS_DATA_Dir = GetIniKey(ini->ReadString(ASection, "HEADS_DATA",
        "")); // HEADS�Ǘ��ް��o��̫���
    G_DAT_ORDER_Dir = GetIniKey(ini->ReadString(ASection, "DATA_ORDER", ""));
    // ���ސ}�ʊǗ��ް�̫���
    G_DAT_ESTIM_Dir = GetIniKey(ini->ReadString(ASection, "DATA_ESTIMATE",
        "")); // ���ϐ}�ʊǗ��ް�̫���

    // 2004.09.23�ǉ�
    G_OUT_DIRECT_Dir =
        GetIniKey(ini->ReadString(ASection, "OUT_DIRECTION", ""));
    // �}���W�o���ް�̫���

    // Windows 2008Server �ڐA�Ή�
    ASection = "recep_environment";
    G_Err_Log_FileName =
        GetIniKey(ini->ReadString(ASection, "log_file_Unregistered", ""));
    // �^�����Ή�۸�


    // 2014/08/07 ���p�Z�p�ǉ�
    // ���ꎯ�ʕ�����̓ǂݍ���
    ASection = "LANGUAGE";
    G_Language = GetIniKey(ini->ReadString(ASection, "LANGUAGE", ""));


    delete ini; // INI̧�ٱ��� ��޼ު�� �폜
}

// ---------------------------------------------------------------------------
// ���{��֐����FIniFile(HD_PARTS.ini)�̓ǂݍ��ݏ���
// �T  �v      �FINI̧�ٓǂݍ���
// ��  ��      �F�Ȃ�
// �߂�l      �F�Ȃ�
// ��  �l      �F���ݒ�t�@�C�� = HD_PARTS.ini
// ---------------------------------------------------------------------------
void __fastcall IniFileRead_Parts(void) {
    TIniFile *ini; // INI̧�ٱ��� ��޼ު��
    AnsiString AIniPath; // �ݒ�̧���߽
    AnsiString AIniFile; // �ݒ�̧���߽ + ̧�ٖ�
    AnsiString ASection; // ����ݖ�

    char inipath[MAX_PATH];

//    // ��ini�t�@�C���̃f�B���N�g��
//    GetSystemDirectory(inipath, sizeof(inipath));
//    AIniPath = inipath;

    AIniPath = ExtractFilePath(Application->ExeName);
        //AIniPath = "D:\\HEADS_ROOT01\\HEADS-CV\\";
        //AIniPath = "C:\\HEADS_ROOT01\\HEADS-CV\\";

    // ��ini�t�@�C���̂ւ̃t���p�X�t�@�C����
    AIniFile = AIniPath + CSYSINIFILENAME_P;

    // ini�t�@�C������ݒ���e��ǂݍ���
    ini = new TIniFile(AIniFile);

    // --- ̧�ٖ��̏��擾
    ASection = "FILENAME"; // --  �ǂݍ��ݾ���ݾ�� : ��{
    // ��ڰčE���ް�����
    G_AnaakeFileNM = GetIniKey(ini->ReadString(ASection, "ANAAKEDATA", ""));

    // �\���i�ް�����
    G_YobihinFileNM = GetIniKey(ini->ReadString(ASection, "YOBIHINDATA", ""));

    delete ini; // INI̧�ٱ��� ��޼ު�� �폜
}

// ---------------------------------------------------------------------------
// [�֐�]  IniFile���e�ǂݎ�菈��
// [�T�v]  �ǂݎ�������e������ĕ����𔲂����B
// [�ߒl]  �����񂩂�R�����g�s�𔲂������������
// [����]  A_Key: ������
// ---------------------------------------------------------------------------
AnsiString __fastcall GetIniKey(AnsiString A_Key) {
    static AnsiString A_RKey;
    int i_Point;

    i_Point = A_Key.Pos(";");

    if (i_Point != 0) {
        A_RKey = Trim(A_Key.SubString(1, i_Point - 1));
    }
    else {
        A_RKey = Trim(A_Key);
    }

    return (A_RKey);
}

// -----------------------------------------------------------------------------
// �֐���      �F�m�nʲ�ݕҏW
// �T  �v      �Fʲ��["-"]����菜��
// ��  ��      �FInpData  : ���͒l
// �߂�l      �FAnsiString
// ��  �l      �F
// -----------------------------------------------------------------------------
AnsiString __fastcall NO_Haifun(AnsiString InpData) {
    static AnsiString A_Data;
    int i, len;

    A_Data = "";
    len = InpData.Length();
    for (i = 1; i <= len; i++) {
        if (InpData.SubString(i, 1) != "-") // "-" �ȊO�̂Ƃ�
        {
            A_Data = A_Data + InpData.SubString(i, 1);
        }
    }
    return A_Data;
}

// -----------------------------------------------------------------------------
// �֐���      �F�m�n�ׯ���ҏW
// �T  �v      �F���t�̋�؂蕶��["/"]����菜��
// ��  ��      �FInpData  : ���͒l
// �߂�l      �FAnsiString
// ��  �l      �F
// -----------------------------------------------------------------------------
AnsiString __fastcall No_Slush(AnsiString InpData) {
    static AnsiString A_Data;
    int i, len;

    A_Data = "";
    len = InpData.Length();
    for (i = 1; i <= len; i++) {
        if (InpData.SubString(i, 1) != "/") // "/" �ȊO�̂Ƃ�
        {
            A_Data = A_Data + InpData.SubString(i, 1);
        }
    }
    return A_Data;
}

// -----------------------------------------------------------------------------
// �֐���      �F�m�n�J���}�ҏW
// �T  �v      �F3 ����؂�̋L���ƒʉ݋L������菜��
// ��  ��      �FInpData  : ���͒l
// �߂�l      �FAnsiString
// ��  �l      �F
// -----------------------------------------------------------------------------
AnsiString __fastcall F_NO_Comma(AnsiString InpData) {
    static AnsiString A_Data;
    int i, len;

    A_Data = "";
    len = InpData.Length();
    for (i = 1; i <= len; i++) {
        if ((InpData.SubString(i, 1) != ",") && (InpData.SubString(i,
            1) != "\\")) // "," and "��" �ȊO�̂Ƃ�
        {
            A_Data = A_Data + InpData.SubString(i, 1);
        }
    }
    return A_Data;
}

// -----------------------------------------------------------------------------
// �֐���      �F�m�n�R������؂�ҏW
// �T  �v      �F�R�����܂ł��擾
// ��  ��      �FInpData  : ���͒l
// �߂�l      �FAnsiString
// ��  �l      �F
// -----------------------------------------------------------------------------
AnsiString __fastcall NO_CoronCut(AnsiString InpData) {
    static AnsiString A_Data;
    int i, endlen;

    A_Data = "";
    endlen = InpData.Pos(":") - 1;
    if (endlen < 1) {
        endlen = InpData.Length();
    }
    for (i = 1; i <= endlen; i++) {
        if (InpData.SubString(i, 1) != ":") // ":" �ȊO�̂Ƃ�
        {
            A_Data = A_Data + InpData.SubString(i, 1);
        }
    }
    return A_Data;
}

// -----------------------------------------------------------------------------
// �֐���      �F���l���͍��ځ@���l�`�F�b�N
// �T  �v      �F���͍��ڂɂ����鐔�l�`�F�b�N
// ��  ��      �FInpData  : ���͒l
// �FOupData  : �߂�l
// �Fi_Seisu  : �������L������
// �Fi_Syosu  : �������L������
// �Fi_Fugou  : ϲŽ����  0:�����@1:�L��
// �߂�l      �F0:�װ�����@1:�������ށ@2:�L���͈͵��ށ@3:�����װ
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall Numeric_Check(AnsiString InpData, double *OutData, int i_Seisu,
    int i_Syosu, int i_Fugou) {
    int i_Point;
    int i_Keta1, i_Keta2;
    double dval, dvalfabs;
    double d;

    AnsiString wkinpdata;

    InpData = F_NO_Comma(InpData.Trim());

    // ------�����`�F�b�N------
    dval = StrToDblDef(InpData); // double�l�ɕϊ�
    if (dval == 0.0f) {
        if (!InpData.IsEmpty()) {
            if (AllFloatChk(InpData.c_str(), (short)i_Fugou)) {
                return 3;
            }
        }
        InpData = "0";
    }

    // ------�����_�ʒu------
    dvalfabs = fabs(dval); // ��Βl
    wkinpdata = dvalfabs; // ��Βl
    i_Point = wkinpdata.Pos(".");
    // �����_�ȉ��̐��l�����݂��Ȃ��Ƃ�
    if (i_Point == 0) {
        i_Keta1 = strlen(wkinpdata.c_str());
        i_Keta2 = 0;
    }
    else {
        i_Keta1 = i_Point - 1;
        i_Keta2 = strlen(wkinpdata.c_str()) - i_Point;
    }

    // ------ ���������`�F�b�N------
    if (i_Keta1 > i_Seisu) // ������
    {
        return 1; // error
    }
    if (i_Keta2 > i_Syosu) // ������
    {
        return 1; // error
    }

    // ------�L���͈̓`�F�b�N�iϲŽ�j------
    if ((i_Fugou == 0) && (dval < 0.0)) {
        return 2;
    }
    // ------�L���͈̓`�F�b�N�i�������j------
    d = pow10(i_Seisu);
    if (dvalfabs >= d) // �������I�[�o�[�H
    {
        return 2;
    }

    (*OutData) = dval;

    // ����I��
    return 0;
}

// ---------------------------------------------------------------------------
// [�֐�]  ������ ������������
// [�T�v]  ������ ��������(������������Ԃ�)
// [�ߒl]  ������ ����������̕�����
// [����]  A_Text:   ������
// i_Length:   ����
// ---------------------------------------------------------------------------
AnsiString __fastcall TxtEdit(AnsiString A_Text, int i_Length) {
    int i_cnt;
    static AnsiString A_RText;

    for (i_cnt = 1; i_cnt <= i_Length; i_cnt++) {
        A_Text = A_Text + " ";
    }

    for (i_cnt = 1; i_cnt <= i_Length; i_cnt++) {
        if (A_Text.SubString(i_cnt, 1) == "") {
            i_cnt = i_cnt - 1;
            break;
        }
    }

    A_RText = "";
    if (i_cnt == 0) {
        for (i_cnt = 1; i_cnt <= i_Length; i_cnt++) {
            A_RText = A_RText + " ";
        }
    }
    else if (i_cnt <= i_Length) {
        A_RText = A_Text.SubString(1, i_cnt);
        for (i_cnt = 1; i_cnt <= i_Length; i_cnt++) {
            A_RText = A_RText + " ";

        }
        A_RText = A_RText.SubString(1, i_Length);
    }
    else {
        A_RText = A_Text.SubString(1, i_Length);
    }

    if (StrByteType(A_RText.c_str(), i_Length - 1) == mbLeadByte)
        // �Ō�̌����ڂ��Q�o�C�g�����̑�P�o�C�g�ځH
    {
        A_RText = A_RText.SubString(1, i_Length - 1) + " ";
    }
    else {
        A_RText = A_RText.SubString(1, i_Length);
    }

    return A_RText;
}

// ---------------------------------------------------------------------------
// [�֐�]  ������ ������������(���󔒍폜��)
// [�T�v]  ������ ��������(�������ȓ��̌��󔒍폜������Ԃ�)
// [�ߒl]  ������ ����������̕�����
// [����]  A_Text:    ������
// i_Length:  ����
// ---------------------------------------------------------------------------
AnsiString __fastcall TxtEditRt(AnsiString A_Text, int i_Length) {
    static AnsiString A_RText;
    int len;

    A_RText = A_Text.TrimRight();
    if (A_RText.Length() > i_Length) {
        A_RText.SetLength(i_Length);
    }

    len = A_RText.Length();
    if (A_RText.IsLeadByte(len)) // �Ō�̌����ڂ��Q�o�C�g�����̑�P�o�C�g�ځH
    {
        len--;
        A_RText.SetLength(len);
    }

    return A_RText;
}

// -----------------------------------------------------------------------------
// ���{��֐����F������������
// �T  �v      �F�����񂪑S��'0'�`'9'�̕����R�[�h�ō\������Ă��邩��������
// ��  ��      �Fchar *Str ����������, flg ���@[0:��,1:��]
// �߂�l      �F�������� [0:���� 1:�ُ�]
// �@�\����    �F�����񂪑S��'0'�`'9'�̕����R�[�h�ō\������Ă��邩��������
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall AllNumChk(char *Str, short flg) {
    int i; // ���[�v�J�E���^

    if (Str[0] == NULL) // ���������H
    {
        return 2; // �߂�l[2:�ُ�]
    }

    i = strlen(Str);
    while (i-- > 0) // �����񒷕�
    {
        if ((Str[i] < '0') || (Str[i] > '9')) // ����'0'�`'9'�ȊO
        {
            if (Str[i] == '-') // '-'�H
            {
                if (flg == 0) // '-'�ʹװ �Ƃ���H
                {
                    return 1; // �߂�l[1:�ُ�]
                }
            }
            else {
                return 1; // �߂�l[1:�ُ�]
            }
        }
    }
    return 0; // �߂�l[0:����]
}

// -----------------------------------------------------------------------------
// ���{��֐����F����������������
// �T  �v      �F�����񂪑S��'0'�`'9'��'.'or'-'�̕����R�[�h�ō\������Ă��邩��������
// ��  ��      �Fchar *Str ����������, flg ���@[0:��,1:��]
// �߂�l      �F�������� [0:���� 1:�ُ�]
// �@�\����    �F�����񂪑S��'0'�`'9'��'.'or'-'�̕����R�[�h�ō\������Ă��邩��������
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall AllFloatChk(char *Str, short flg) {
    int i; // ���[�v�J�E���^

    if (Str[0] == NULL) // ���������H
    {
        return 2; // �߂�l[2:�ُ�]
    }

    i = strlen(Str);
    while (i-- > 0) // �����񒷕�
    {
        if (((Str[i] < '0') || (Str[i] > '9')) && (Str[i] != '.'))
            // ����'0'�`'9'��'.' �ȊO
        {
            if (Str[i] == '-') // '-'�H
            {
                if (flg == 0) // '-'�ʹװ �Ƃ���H
                {
                    return 1; // �߂�l[1:�ُ�]
                }
            }
            else {
                return 1; // �߂�l[1:�ُ�]
            }
        }
    }
    return 0; // �߂�l[0:����]
}

// -----------------------------------------------------------------------------
// ���{��֐����F�p������������
// �T  �v      �F�����񂪑S�ĉp�����̕����R�[�h�ō\������Ă��邩��������
// ��  ��      �Fchar *Str ����������, flg ���@[0:���l�Ȃ�,1:���l����]
// �߂�l      �F�������� [0:���� 1:�ُ�]
// �@�\����    �F�����񂪑S��'0'�`'9'��'.'or'-'�̕����R�[�h�ō\������Ă��邩��������
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall AllAlphaChk(char *Str, short flg) {
    int i; // ���[�v�J�E���^

    if (Str[0] == NULL) // ���������H
    {
        return 2; // �߂�l[2:�ُ�]
    }

    i = strlen(Str);
    while (i-- > 0) // �����񒷕�
    {
        if (((Str[i] < ' ') || (Str[i] > '~'))) // ���p�p����'�H
        {
            if (((Str[i] >= '0') || (Str[i] <= '9'))) // ����'0'�`'9'�H
            {
                if (flg == 0) // �����ʹװ �Ƃ���H
                {
                    return 1; // �߂�l[1:�ُ�]
                }
            }
            else {
                return 1; // �߂�l[1:�ُ�]
            }
        }
    }
    return 0; // �߂�l[0:����]
}

// -----------------------------------------------------------------------------
// �֐����@�@�@�F���͕�����`�F�b�N
// �T  �v      �F���͍��ڂɂ�����g�p�֎~�����̗L���`�F�b�N
// ��  ��      �FAnsiString Str  �`�F�b�N������
// �Fshort   dashflg "'"�ޯ���װ�׸�(�ȗ��\:0)
// �߂�l      �F0:����A�P�F�ُ�
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall StrErrCheck(AnsiString Str, short dashflg) {
    if (Str.IsEmpty()) // �`�F�b�N�Ώۂ��u�����N�H
    {
        return 0;
    }
    else if (Str.Pos(",") == 1) // �w�C�x
    {
        return 1;
    }
    else if (Str.Pos("'") != 0) // �w�f�x
    {
        if (dashflg) {
            if (Str.Pos("'") == 1) {
                return 1;
            }
        }
        else {
            return 1;
        }
    }
    else if (Str.Pos("\"") == 1) // �w�h�x
    {
        return 1;
    }
    else if (Str.Pos("|") == 1) // �w|�x
    {
        return 1;
    }
    else // ���̑�
    {
        return 0;
    }

    return 0;
}

// -----------------------------------------------------------------------------
// �֐����@�@�@�F���͕�����`�F�b�N
// �T  �v      �F���͍��ڂɂ�����g�p�֎~�����̗L���`�F�b�N
// ��  ��      �FTObject *Sender �ďo����
// �߂�l      �F0:����A�P�F�ُ�
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall TextErrCheck(TObject *Sender) {
    TEdit *wkedit;
    TMemo *wkmemo;
    TComboBox *wkcombobox;
    bool dashflg = false;
    AnsiString Str;

    Str = "";
    if ((wkedit = dynamic_cast<TEdit*>(Sender)) != NULL) {
        Str = wkedit->Text.Trim();
    }
    else if ((wkmemo = dynamic_cast<TMemo*>(Sender)) != NULL) {
        Str = wkmemo->Text.Trim();
        dashflg = true;
    }
    else if ((wkcombobox = dynamic_cast<TComboBox*>(Sender)) != NULL) {
        int len = wkcombobox->Text.Pos(":") - 1;
        Str = Trim(wkcombobox->Text.SubString(1, len));
    }

    return StrErrCheck(Str, dashflg);
}

// -----------------------------------------------------------------------------
// �֐����@�@�@�F���t�`�F�b�N����
// �T  �v      �F���t�̃`�F�b�N���s��
// ��  ��      �FA_ymd    (yyyy/mm/dd�`��)
// *Outdata ��������t������i�[��
// �߂�l      �F0:�ُ�Ȃ� 1:�����G���[ 2:���t�װ
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall DateCheck(AnsiString A_ymd, AnsiString *Outdata) {
    int i_errsw = 0;
    int len;
    int MyPos;
    AnsiString year, month, day;
    AnsiString A_md, a_wk;

    a_wk = A_ymd.Trim();
    len = a_wk.Length();

    // �ŏ��̽ׯ����T��
    MyPos = a_wk.AnsiPos("/");
    if (MyPos == 0) {
        (*Outdata) = a_wk;
        return 1;
    }

    // ���̽ׯ����T��
    MyPos = a_wk.SubString(MyPos + 1, len - MyPos + 1).AnsiPos("/");
    if (MyPos == 0) {
        (*Outdata) = a_wk;
        return 1;
    }

    MyPos = a_wk.AnsiPos("/");
    // �N���擾
    year = a_wk.SubString(1, MyPos - 1);

    // �N����ׯ���܂ō폜
    A_md = a_wk.Delete(1, MyPos);

    // ���Ɠ��̋�؂��T��
    MyPos = A_md.AnsiPos("/");

    // ���܂ł��擾
    month = A_md.SubString(1, MyPos - 1);

    // �������؂�܂ł��폜���ē����擾
    day = A_md.Delete(1, MyPos);

    if (year.Length() == 3) {
        (*Outdata) = a_wk;
        return 1;
    }

    if (year.Length() == 1) {
        year = FormatDateTime("yy", Date()).SubString(1, 1) + year;
    }

    if (StrToIntDef(year, 0) < 100) {
        if (StrToIntDef(year, 0) > 80) {
            year = "19" + year;
        }
        else {
            year = "20" + year;
        }
    }

    // �N�m��
    len = year.Trim().Length();
    year = year.SubString(len - 3, 4);

    // ���m��
    month = "0" + month;
    len = month.Trim().Length();
    month = month.SubString(len - 1, 2);

    // ���m��
    day = "0" + day;
    len = day.Trim().Length();
    day = day.SubString(len - 1, 2);

    // ���t�����m��
    a_wk = year + "/" + month + "/" + day;
    (*Outdata) = a_wk;

    try {
        if (a_wk.Length() != 10) {
            i_errsw = 1;
        }
        else {
            try {
                StrToDate(A_ymd);
            }
            catch (EConvertError &error) {
                i_errsw = 2;
            }
        }
    }
    __finally {
        // Windows 2008Server �ڐA�Ή�
        // return (short)i_errsw;
    }
    return (short)i_errsw;
}

// -----------------------------------------------------------------------------
// �֐����@�@�@�F���l���ڶ�ϕҏW���W���[��
// �T  �v      �F�����ɶ�ς����ĕԂ�
// ��  ��      �FAnsiString &AStr  �Ώە������
// �Fint   syosu  �����_�ȉ�����
// �߂�l      �F0:���� 1:���s
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall KanmaStr(AnsiString &AStr, int syosu) {
    AnsiString A_wk;
    double d_wk;
    char wrk[64], buf[64];

    A_wk = F_NO_Comma(AStr.Trim());
    if (A_wk.IsEmpty()) {
        return 0;
    }
    else {
        try {
            d_wk = StrToDblDef(A_wk);
            sprintf(wrk, "#,%%.%df", syosu);
            sprintf(buf, wrk, 0.0f);
            AStr = FormatFloat(buf, d_wk);
            return 0;
        }
        catch (EConvertError &Error) {
            return 1;
        }
    }
}

// -----------------------------------------------------------------------------
// �֐����@�@�@�F���l���ڶ�ϕҏW���W���[��
// �T  �v      �F�����ɶ�ς����ĕԂ�
// ��  ��      �FTObject *Sender ���ͺ��۰�
// �Fint syosu  �����_�ȉ�����
// �߂�l      �F0:���� 1:���s
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall AddKanma(TObject *Sender, int syosu) {
    short sRtn;
    TMemo *wkmemo;
    AnsiString AStr;

    if ((wkmemo = dynamic_cast<TMemo*>(Sender)) != NULL) {
        AStr = wkmemo->Lines->Text.Trim();
        if (AStr.IsEmpty()) {
            AStr = "0";
        }
        sRtn = KanmaStr(AStr, syosu);
        wkmemo->Lines->Text = AStr;
        return sRtn;
    }
    else {
        return 1;
    }
}

// -----------------------------------------------------------------------------
// �֐���      �F�l�␳����
// �T  �v      �F�l���l�̌ܓ�,�؏�,�؎̂ŕ␳����
// �p�����[�^  �Fint     sw   : ���@(0:�l�̌ܓ�,1:�؂�グ,2:�؂�̂�)
// �Fdouble  dval : �Ώےl
// �Fint     k    : �ۂ߂鏭�����ʒu
// �߂�l      �F�l�␳�l(�G���[���͖��ϊ�)
// ��  �l      �F
// -----------------------------------------------------------------------------
double __fastcall Hasu_Round(int sw, double dval, int k) {
    double d, x;

    try {
        d = pow10(k);
        switch (sw) {
        case 0:
            // �l�̌ܓ�
            x = floor(((dval < 0) ? (-dval) : (dval)) * d + 0.5);
            return (((dval < 0) ? (-x) : (x)) / d);
        case 1:
            // �؂�グ
            x = ceil(((dval < 0) ? (-dval) : (dval)) * d);
            return (((dval < 0) ? (-x) : (x)) / d);
        case 2:
            // �؂�̂�
            x = floor(((dval < 0) ? (-dval) : (dval)) * d + 0.00001);
            // ���� �������΍�
            return (((dval < 0) ? (-x) : (x)) / d);
        default:
            return dval;
        }
    }
    catch (...) {
        return dval;
    }
}

// -----------------------------------------------------------------------------
// �֐���      �F�l�␳����(long double)
// �T  �v      �F�l���l�̌ܓ�,�؏�,�؎̂ŕ␳����
// �p�����[�^  �Fint          sw    : ���@(0:�l�̌ܓ�,1:�؂�グ,2:�؂�̂�)
// �Flong double  ldval : �Ώےl
// �Fint          k     : �ۂ߂鏭�����ʒu
// �߂�l      �F�l�␳�l(�G���[���͖��ϊ�)
// ��  �l      �F
// -----------------------------------------------------------------------------
Extended __fastcall Hasu_RoundE(int sw, Extended ldval, int k) {
    Extended ld, ldx;

    try {
        ld = pow10l(k);
        switch (sw) {
        case 0:
            // �l�̌ܓ�
            ldx = floorl(((ldval < 0) ? (-ldval) : (ldval)) * ld + 0.5);
            return (((ldval < 0) ? (-ldx) : (ldx)) / ld);
        case 1:
            // �؂�グ
            ldx = ceill(((ldval < 0) ? (-ldval) : (ldval)) * ld);
            return (((ldval < 0) ? (-ldx) : (ldx)) / ld);
        case 2:
            // �؂�̂�
            ldx = floorl(((ldval < 0) ? (-ldval) : (ldval)) * ld + 0.00001);
            // ���� �������΍�
            return (((ldval < 0) ? (-ldx) : (ldx)) / ld);
        default:
            return ldval;
        }
    }
    catch (...) {
        return ldval;
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F ������ ���p�J�^�J�i�ϊ�����
// �T  �v      �F ��������̔��p�J�^�J�i��S�p�ɕϊ�����
// ��  ��      �F AText  :  ������
// �F &AConv :  �ϊ��㕶�����
// �F iLen   :  �������ő�
// �߂�l      �F �ϊ���̌��� 0:����, 1:err
// ��  �l      �F
// ---------------------------------------------------------------------------
short __fastcall HalfKanaConv(AnsiString AText, AnsiString &AConv, int iLen) {
    const static WORD s_z[] = {
        0x8142 /* �B */ , 0x8175 /* �u */ , 0x8176 /* �v */ , 0x8141 /* �A */ ,
        0x8145 /* �E */ , 0x8392 /* �� */ , 0x8340 /* �@ */ , 0x8342 /* �B */ ,
        0x8344 /* �D */ , 0x8346 /* �F */ , 0x8348 /* �H */ , 0x8383 /* �� */ ,
        0x8385 /* �� */ , 0x8387 /* �� */ , 0x8362 /* �b */ , 0x815b /* �[ */ ,
        0x8341 /* �A */ , 0x8343 /* �C */ , 0x8345 /* �E */ , 0x8347 /* �G */ ,
        0x8349 /* �I */ , 0x834a /* �J */ , 0x834c /* �L */ , 0x834e /* �N */ ,
        0x8350 /* �P */ , 0x8352 /* �R */ , 0x8354 /* �T */ , 0x8356 /* �V */ ,
        0x8358 /* �X */ , 0x835a /* �Z */ , 0x835c /* �\ */ , 0x835e /* �^ */ ,
        0x8360 /* �` */ , 0x8363 /* �c */ , 0x8365 /* �e */ , 0x8367 /* �g */ ,
        0x8369 /* �i */ , 0x836a /* �j */ , 0x836b /* �k */ , 0x836c /* �l */ ,
        0x836d /* �m */ , 0x836e /* �n */ , 0x8371 /* �q */ , 0x8374 /* �t */ ,
        0x8377 /* �w */ , 0x837a /* �z */ , 0x837d /* �} */ , 0x837e /* �~ */ ,
        0x8380 /* �� */ , 0x8381 /* �� */ , 0x8382 /* �� */ , 0x8384 /* �� */ ,
        0x8386 /* �� */ , 0x8388 /* �� */ , 0x8389 /* �� */ , 0x838a /* �� */ ,
        0x838b /* �� */ , 0x838c /* �� */ , 0x838d /* �� */ , 0x838f /* �� */ ,
        0x8393 /* �� */ , 0x814a /* �J */ , 0x814b /* �K */ , };
    short sRtn;
    AnsiString ASetStr;
    int mode;
    int i, len;
    DWORD code;
    UCHAR ch, next;

    sRtn = 0;

    // �����E�ϊ�
    ASetStr = "";
    len = AText.Length();

    mode = CT_ANK;
    for (i = 0; i < len; i++) {
        ch = (UCHAR)AText[i + 1];
        mode = chkctype(ch, mode);
        if ((mode == CT_ANK) && (iskana(ch))) {
            code = s_z[ch - 0xa1];
            i++;

            next = (i < len) ? ((UCHAR)AText[i + 1]) : ('\0');
            if ((next == 0xde) /* ���_ */ && ((ch == 0xb3) ||
                ((ch >= 0xb6) && (ch <= 0xc4)) || ((ch >= 0xca) && (ch <=
                0xce)))) {
                if (ch == 0xb3) /* �E */ {
                    code = 0x8394; /* �� */
                }
                else {
                    code++;
                }
            }
            else if ((next == 0xdf) /* �����_ */ &&
                (((ch >= 0xca) && (ch <= 0xce)))) {
                code += 2;
            }
            else {
                i--; // �P�����߂�
            }
            ASetStr += (char)(code >> 8);
            ASetStr += (char)(code);
        }
        else {
            ASetStr += (char)(ch);
        }
    }

    if (iLen > 0) {
        if (ASetStr.Length() > iLen) {
            ASetStr.SetLength(iLen);
        }
        len = ASetStr.Length();
        if (ASetStr.IsLeadByte(len)) // �Ō�̌����ڂ��Q�o�C�g�����̑�P�o�C�g�ځH
        {
            len--;
            ASetStr.SetLength(len);
        }
    }
    AConv = ASetStr;

    return sRtn;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��޼ު�Ĉʒu�����ݒ菈��
// �T  �v      �F ��޼ު�Ĉʒu�𒆉��ɐݒ肷��
// ��  ��      �F TObject *parent     ���޼ު��
// �F TObject *object     �ʒu�ݒ��޼ު��
// �F int     subx, suby  �ʒu�␳��(�ȗ��\:[0,0])
// �߂�l      �F �ϊ���̌��� 0:����, 1:err
// ��  �l      �F
// ---------------------------------------------------------------------------
short __fastcall SetPosCenter(TObject *parent, TObject *object, int subx,
    int suby) {
    TControl *ctrlPar, *ctrlObj;
    TPoint ppos, pos1, pos2, setpos, add;

    try {
        if ((ctrlPar = dynamic_cast<TControl*>(parent)) == NULL) {
            return 1;
        }
        if ((ctrlObj = dynamic_cast<TControl*>(object)) == NULL) {
            return 1;
        }

        ppos = TPoint(ctrlObj->Left + subx, ctrlObj->Top + suby);
        add.x = ((ctrlPar->ClientWidth - ctrlObj->Width) / 2);
        add.y = ((ctrlPar->ClientHeight - ctrlObj->Height) / 2);
        pos1 = ctrlPar->ClientToScreen(TPoint(0, 0));
        pos2.x = pos1.x + add.x + ppos.x;
        pos2.y = pos1.y + add.y + ppos.y;
        setpos = ctrlObj->ScreenToClient(pos2);

        ctrlObj->Left = setpos.x;
        ctrlObj->Top = setpos.y;
    }
    catch (...) {
        return (-1);
    }
    return 0;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���t���ڕ���������
// �T  �v      �F ���t�����ڕʂ���������
// ��  ��      �F AnsiString AStr   ���蕶����
// �F int        type   ���ڎ��[DateKindT::]
// �F int year, month   ��N��(type:���Ŏg�p)(�ȗ��\:0)
// �F int        m2def  �Q��=29���׸�(type:���Ŏg�p)(�ȗ��\:1)
// �߂�l      �F �ϊ���̌��ʒl, ���� err��:�������l
// ��  �l      �F
// ---------------------------------------------------------------------------
short __fastcall DateSubChk(AnsiString AStr, int type, int year, int month,
    int m2def) {
    AnsiString AWrkStr;
    USHORT yy, mm, dd;
    double d_val;
    int i_val;
    int iRtn;
    int chklen, chkmin, chkmax;

    AWrkStr = Trim(AStr);

    // ���ݸ����
    if (AWrkStr.IsEmpty()) {
        return 4; // ���ݸ
    }

    // �g�p�֎~��������
    if (StrErrCheck(AWrkStr) || AllNumChk(AWrkStr.c_str(), 0)) {
        return 5; // �g�p�֎~����
    }

    // ���l���ڂ̃`�F�b�N
    switch (type) {
    case dsDays: // dd
        chklen = 2;
        chkmin = 1;
        chkmax = 31;
        break;
    case dsSetDay: // �w��dd
        chklen = 2;
        chkmin = 1;
        if ((month == 2) && (m2def)) {
            chkmax = 29; // �Q�� �ő����
        }
        else {
            year = YYtoYYYY(year);
            if ((year <= 0) || (month <= 0)) {
                Date().DecodeDate(&yy, &mm, &dd);
                if (year <= 0)
                    year = yy;
                if (month <= 0)
                    month = mm;
            }
            chkmax = GetMonthDays(year, month); // �w��N�� �ő����
        }
        break;
    case dsMonth: // mm
        chklen = 2;
        chkmin = 1;
        chkmax = 12;
        break;
    case dsSubYear: // yy
        chklen = 2;
        chkmin = 0;
        chkmax = 99;
        break;
    case dsYear: // yyyy
    default:
        chklen = 4;
        chkmin = 1;
        chkmax = 9999;
        break;
    }
    d_val = 0;
    iRtn = Numeric_Check(AStr, &d_val, chklen, 0, 0);
    if (iRtn) {
        return (short)iRtn;
    }
    i_val = (int)(d_val + 0.1);
    if ((i_val < chkmin) || (i_val > chkmax)) // �L���͈͊O�̂Ƃ�
    {
        return 6; // �͈ʹװ(���ʹװ)
    }

    return 0;
}

// -----------------------------------------------------------------------------
// �֐���      �FYY��YYYY�ϊ�����
// �T  �v      �FYY��YYYY�ɕϊ�����
// ��  ��      �FYY_Str  : ����YY�l,  flg:��ۻ��ڽ�׸�[�ȗ���1:0����]
// �߂�l      �FAnsiString
// ��  �l      �F
// -----------------------------------------------------------------------------
AnsiString __fastcall YYtoYYYY_Str(AnsiString YY_Str, short flg) {
    static AnsiString A_Str;
    int ival, len;

    ival = YYtoYYYY(YY_Str.ToIntDef(0));
    if (flg) {
        ZeroSupress(IntToStr(ival), A_Str, 4); // YY �� YYYY
    }
    else {
        A_Str = IntToStr(ival);
        len = A_Str.Length();
        if (len < 4) // " "�s���H
        {
            A_Str = AnsiString::StringOfChar(' ', (4 - len)) + A_Str;
        }
    }
    return A_Str;
}

// -----------------------------------------------------------------------------
// �֐���      �FYY��YYYY�ϊ�����
// �T  �v      �FYY��YYYY�ɕϊ�����
// ��  ��      �Fyy  : ����YY�l
// �߂�l      �FYYYY�l
// ��  �l      �F
// -----------------------------------------------------------------------------
short __fastcall YYtoYYYY(int yy) {
    if (yy < 100) {
        return (short)((yy > 80) ? (yy + 1900) : (yy + 2000));
    }
    else {
        return (short)yy;
    }
}

// -----------------------------------------------------------------------------
// ���{��֐����F���t����(YYYY/MM/DD) �ϊ�����
// �T  �v      �F���t����(YYYY/MM/DD)�� ���l�ɕϊ�
// ��  ��      �Fchar *cYMD,  ���t����
// WORD &Year, &Month, &Day  ���l�i�[��
// �߂�l      �F0:����, !0:err
// ��  �l      �F��؂蕶��:"/"
// -----------------------------------------------------------------------------
short __fastcall DecodeYMD(char *cYMD, WORD *Year, WORD *Month, WORD *Day) {
    short sRtn;
    TDate date;
    char *p;
    int i, len;
    AnsiString Awrk;

    sRtn = 0;
    try {
        (*Year) = 0;
        (*Month) = 0;
        (*Day) = 0;

        len = strlen(cYMD);
        p = StrPos(cYMD, "/");
        if (p != NULL) {
            Awrk = "";
            i = 0;
            while ((&cYMD[i]) != (p)) {
                Awrk += cYMD[i++];
            }
            (*Year) = (WORD)atoi(Awrk.c_str());

            i++;
            p = StrPos(&cYMD[i], "/");
            if (p != NULL) {
                Awrk = "";
                while ((&cYMD[i]) != (p)) {
                    Awrk += cYMD[i++];
                }
                (*Month) = (WORD)atoi(Awrk.c_str());

                i++;
                Awrk = "";
                while (i < len) {
                    Awrk += cYMD[i++];
                }
                (*Day) = (WORD)atoi(Awrk.c_str());
            }
        }
    }
    __finally {
        // Windows 2008Server �ڐA�Ή�
        // return sRtn;
    }
    return sRtn;
}

// -----------------------------------------------------------------------------
// ���{��֐����F���t����(MM/DD)  �ϊ�����
// �T  �v      �F���t����(MM/DD)�� ���l�ɕϊ�
// ��  ��      �Fchar *cYMD,   ���t����
// WORD &Month, &Day  ���l�i�[��
// �߂�l      �F0:����, !0:err
// ��  �l      �F��؂蕶��:"/"
// -----------------------------------------------------------------------------
short __fastcall DecodeMD(char *cYMD, WORD *Month, WORD *Day) {
    short sRtn;
    TDate date;
    char *p;
    int i, len;
    AnsiString Awrk;

    sRtn = 0;
    try {
        (*Month) = 0;
        (*Day) = 0;

        len = strlen(cYMD);
        p = StrPos(cYMD, "/");
        if (p != NULL) {
            Awrk = "";
            i = 0;
            if (p != NULL) {
                Awrk = "";
                while ((&cYMD[i]) != (p)) {
                    Awrk += cYMD[i++];
                }
                (*Month) = (WORD)atoi(Awrk.c_str());

                i++;
                Awrk = "";
                while (i < len) {
                    Awrk += cYMD[i++];
                }
                (*Day) = (WORD)atoi(Awrk.c_str());
            }
        }
    }
    __finally {
        // Windows 2008Server �ڐA�Ή�
        // return sRtn;
    }
    return sRtn;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���t�����Z�o����
// �T  �v      �F ���t�̎Z�o���s��
// ��  ��      �F TDateTime posdate  ���t��l
// �F year,month,day     ���t�����Z�l(�����͔͈͊O�ł��悢)
// �߂�l      �F TDateTime �v�Z���ʓ��t�i���s���͌��̓��t�j
// ��  �l      �F
// ---------------------------------------------------------------------------
TDateTime __fastcall DateCalc(TDateTime date, int year, int month, int day) {
    unsigned short y, m, d;

    try {
        date.DecodeDate(&y, &m, &d);
        year += y;
        month += m;
        if (month <= 0) {
            year += month / 12 - 1;
            month = 12 - (-month) % 12;
        }
        else if (month > 12) {
            year += month / 12;
            month %= 12;
        }
        date = TDateTime((short)year, (short)month, d) + day;
        return date;
    }
    catch (...) {
        return date;
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���t�ϊ�����
// �T  �v      �F ���t�̕ϊ����s��
// ��  ��      �F year,month,day   ���t�l(�����͔͈͊O�ł��悢)
// �߂�l      �F TDateTime ���t�l�i���s���͌����t�j
// ��  �l      �F �ُ�l�ł��ϊ��\
// ---------------------------------------------------------------------------
TDateTime __fastcall DateConv(int year, int month, int day) {
    TDate date;

    try {
        if (month <= 0) {
            year += month / 12 - 1;
            month = 12 - (-month) % 12;
        }
        else if (month > 12) {
            year += month / 12;
            month %= 12;
        }
        date = TDate((short)year, (short)month, 1) + (day - 1);
        return date;
    }
    catch (...) {
        return Date();
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F �͈͓����擾����
// �T  �v      �F �͈͓��̓������擾����
// ��  ��      �F TDateTime posdate  ���t��l
// �F TDateTime posdate  ���t��l
// �߂�l      �F �͈͓����i���s����0�j
// ��  �l      �F
// ---------------------------------------------------------------------------
int __fastcall GetDateSclDays(TDateTime datepos, TDateTime dateend) {
    int rtndays = 0;

    try {
        rtndays = (int)dateend - (int)datepos;
        return rtndays;
    }
    catch (...) {
        return rtndays;
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F �w�茎�����擾����
// �T  �v      �F �w�茎�̓������擾����
// ��  ��      �F int year, int month  �w��N��
// �߂�l      �F �͈͓����i���s����0�j
// ��  �l      �F
// ---------------------------------------------------------------------------
int __fastcall GetMonthDays(int year, int month) {
    USHORT y, m, d = 0;
    TDateTime date;

    try {
        year = YYtoYYYY(year);
        date = TDateTime((short)(year + (month / 12)),
            (short)((month % 12) + 1), 1) - 1;
        date.DecodeDate(&y, &m, &d);
    }
    catch (...) {
    }
    return d;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ��ۻ��ڽ�p����
// �T  �v      �F ��ۻ��ڽ���s��
// ��  ��      �F AnsiString StrMath  �Ώې��l������
// �F AnsiString &SetStr  �����㕶����i�[��
// �F int        len      �ݒ蒷��
// �߂�l      �F 0:����, 1:err
// ��  �l      �F ("1"��"01")
// ---------------------------------------------------------------------------
short __fastcall ZeroSupress(AnsiString StrMath, AnsiString &SetStr, int len) {
    int orglen;

    orglen = StrMath.Length();
    if (orglen < len) // "0"�s���H
    {
        SetStr = AnsiString::StringOfChar('0', (len - orglen)) + StrMath;
    }
    else {
        SetStr = StrMath;
    }
    return (orglen <= len);
}

// ---------------------------------------------------------------------------
// ���{��֐����F�t�@�C�����`�F�b�N����
// �T  �v      �F�t�@�C���������������͂���Ă��邩��������
// ��  ��      �Fchar   *dir  // �t�@�C�����ւ̃|�C���^(NULL�I��)
// �Fshort  flg   // ������@[0:̧�ٖ�, 1:�ިڸ�ؖ�]
// �߂�l      �F�������� [0:���� 1:�ُ�]
// �@�\����    �F�t�@�C�����������͂Ȃ�Έُ�Ƃ���
// �F�t�@�C�����Ƃ��Ďg�p�ł��Ȃ��������܂܂�Ă��Ȃ�����������
// ��  �l      �Fwimdowş�ټ��їp
// �F�t�@�C�����̗��[��Trim����Ă���(�󔒂ł͂Ȃ�)���ƁB
// ---------------------------------------------------------------------------
short __fastcall WinFileNameChk(char *dir, short flg) {
    int i, len; // ���[�v�J�E���^, ����

    if (dir[0] == NULL)
        return 1; // �����͂Ȃ�ُ�I��

    len = strlen(dir);
    for (i = 0; i < len; i++) // �����񕪌J��Ԃ�
    {
        switch (dir[i]) // �f�B���N�g�����Ɏg���Ȃ��������`�F�b�N
        {
        case '"': // '"'
        case '|': // '|'
        case ';': // ';'
        case '/': // '/'
        case '*': // '*'
        case '<': // '<'
        case '>': // '>'
        case '?': // '?'
            return 1; // �߂�l : [1:�ُ�]
        case ':': // ':'
        case '\\': // '\\'
            if (flg == 0) // ̧�ٖ������H
            {
                return 1; // �߂�l : [1:�ُ�]
            }
            break;
        }
    }
    return 0; // �߂�l : [0:����]
}

// ---------------------------------------------------------------------------
// ���{��֐����F �����񏬐��ϊ���O����
// �T  �v      �F �����񏬐��ϊ��̐����������s��
// ��  ��      �F AnsiString f_str   �ϊ��Ώە�����
// �F double     err     �G���[���̃f�t�H���g�l
// �߂�l      �F double     �����ϊ��l �܂��� �G���[���f�t�H���g�l
// ��  �l      �F StrToIntDef() �� double ��
// ---------------------------------------------------------------------------
double __fastcall StrToDblDef(AnsiString f_Str, double err) {
    char *p, *endptr;
    double value;

    p = f_Str.c_str();
    value = strtod(p, &endptr); // ���������ϊ�
    if (endptr[0] && (endptr == p)) // �ϊ����������H
    {
        return err; // �װ���̒l
    }
    return value; // �ϊ��l
}

/* -----------------------------------------------------------------------------
 // ���{��֐����F�g�[�N���擾����
 // �T  �v      �F���������؂蕶���ŋ�؂�
 // �p�����[�^  �Fchar *Str ������
 //             �Fchar *Sep ��؂蕶��
 // �߂�l      �F�g�[�N��������ւ̃|�C���^
 // �@�\����    �F���������؂蕶���ŋ�؂�
 // ��  �l      �F������NULL�̏ꍇ�͑����̃g�[�N�����擾����
 //--------------------------------------------------------------------------- */
char* __fastcall GetToken(char *Str, char *Sep) {
    /* --- C : char* �� --- */
    static char *astr = NULL; /* ������i�[�߲�� */
    static char *atkn = NULL; /* �g�[�N��������i�[�߲�� */
    static int strflg = 0; /* ������t���O */
    static int cnt = 1; /* ���[�v�J�E���^ */
    static int len = 0; /* ���������ێ� */
    static int tlen = 0; /* �g�[�N�������i�[�ʒu */

    if (Str) /* �g�[�N����������H */ {
        if (astr != NULL) /* �����񃁃�������H */ {
            free(astr); /* �������̉�� */
            astr = NULL;
        }
        if (atkn != NULL) /* �g�[�N�������񃁃�������H */ {
            free(atkn); /* �g�[�N���������̉�� */
            atkn = NULL;
        }
        len = strlen(Str); /* �������ێ� */
        if ((astr = (char*)malloc(len + 2)) != NULL) /* �������擾�����H */ {
            memset(astr, '\0', (len + 2));
            strncpy(astr, Str, len); /* ��������o�b�t�@�Ɏ擾 */
            cnt = 0; /* ���[�v�J�E���^������ */
            strflg = 0; /* ������t���O������] */
        }
        else {
            return NULL;
        }
        atkn = (char*)malloc(len + 2); /* �g�[�N���������擾 */
    }
    if ((astr == NULL) || (atkn == NULL)) /* �擾�������Ȃ��H */ {
        return NULL;
    }
    memset(atkn, '\0', (len + 2)); /* �g�[�N�������� */
    tlen = 0;

    for (; cnt < len; cnt++) /* �����񒷕� */ {
        if (ByteChrChk(astr[cnt])) /* �Q�o�C�g�����̍ŏ��̕����H */ {
            atkn[tlen++] = astr[cnt]; /* �Q�o�C�g�����P�o�C�g�ڎ擾 */
            atkn[tlen++] = astr[cnt + 1]; /* �Q�o�C�g�����Q�o�C�g�ڎ擾 */
            cnt++; /* ���[�v�J�E���^�␳ */
            continue; /* ���̕����� */
        }
        if (astr[cnt] == '\r') {
            cnt++; /* ���̕����� */
            break; /* ���[�v�E�o */
        }
        if (astr[cnt] == '\n') /* '\n'(���s�R�[�h) */ {
            cnt++; /* ���̕����� */
            break; /* ���[�v�E�o */
        }
        if (!strflg) /* ��������ȊO */ {
            if (astr[cnt] == '\"') /* '"'(����ٸ��ð���) */ {
                strflg = 1; /* ������t���O��L��� */
                continue; /* ���̕����� */
            }
            if (strncmp(&astr[cnt], Sep, strlen(Sep)) == 0) /* ��؂蕶���H */ {
                cnt += strlen(Sep); /* �Ώە�����X�V */
                break; /* ���[�v�E�o */
            }
        }
        else {
            /* '"'(����ٸ��ð���) and (���̕�������؂蕶�� or ���s) */
            if ((astr[cnt] == '\"') && (((astr[cnt + 1] == '\n') ||
                ((astr[cnt + 1] == '\r') && (astr[cnt + 2] == '\n'))) ||
                (strncmp(&astr[cnt + 1], Sep, strlen(Sep)) == 0))) {
                strflg = 0; /* ������t���O������ */
                continue; /* ���̕����� */
            }
        }
        atkn[tlen++] = astr[cnt]; /* �����ǉ� */
    }

    if ((tlen == 0) && (cnt >= len)) /* �g�[�N�������񒷂��0� ���� ������I���H */ {
        if (astr != NULL) /* �����񃁃�������H */ {
            free(astr); /* �������̉�� */
            astr = NULL;
        }
        if (atkn != NULL) /* �g�[�N�������񃁃�������H */ {
            free(atkn); /* �g�[�N���������̉�� */
            atkn = NULL;
        }
        return NULL; /* �߂�l[NULL] */
    }
    else {
        return atkn; /* �߂�l[�g�[�N��������|�C���^] */
    }
}

/************************************
 �Q�o�C�g�����̑�P�o�C�g�ڂ�����
 �Q�o�C�g�����Ȃ�O�ȊO��Ԃ�
 *************************************/
int __fastcall ByteChrChk(char a) {
    return _ismbblead((unsigned int)a);
}

// -----------------------------------------------------------------------------
// �֐����@�@�@�F����ڰ�̧�ٺ�߰����
// �T  �v      �F������̫��ނɂ�������ڰ�̧�ق��߰����
// �p�����[�^  �FAnsiString F_filename  // ��߰��̧��(���߽)
// �F           T_filename  // ��߰��̧��(���߽)
// �߂�l      �F0:��߰���� 1:��߰���s
// ��  �l      �F
// -----------------------------------------------------------------------------
bool __fastcall Template_Copy(AnsiString F_filename, AnsiString T_filename) {

    if (CopyFile(F_filename.c_str(), T_filename.c_str(), false)) {
        return true;
    }
    else {
        return false;
    }
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���O�t�@�C���@�쐬
// �T  �v      �F �װ���e��[�N���������b]��t�^���č쐬����B
// ��  ��      �F AnsiString LogText   ���O���b�Z�[�W
// �߂�l      �F
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall Write_Log(AnsiString LogText) {
    AnsiString s_LogText;

    // �׸�����
    if (G_Log_Write_Flg == 1) {
        return true;
    }

    // ***************************
    // ***  ۸�̧�ف@�쐬
    // ***************************
    s_LogText = "[" + FormatDateTime("yyyy/mm/dd", Date()) + " " +
        FormatDateTime("hh:nn:ss", Time()) + "]  ";

    s_LogText = s_LogText + LogText + "\n";

    FILE *fp;
    AnsiString Err_Msg;

    if ((fp = fopen(G_LogFilePath.c_str(), "a+")) == NULL) {
        fclose(fp);
        // delete fp;
        return false;
    }

    fputs(s_LogText.c_str(), fp);
    fclose(fp);
    // delete fp;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �G���[���O�t�@�C���@�쐬
// �T  �v      �F �װ���e��[�N���������b]��t�^���č쐬����B
// ��  ��      �F AnsiString LogText   ���O���b�Z�[�W
// �߂�l      �F
// ��  �l      �F
// ---------------------------------------------------------------------------
bool __fastcall Write_Error_Log(AnsiString LogText) {
    AnsiString s_LogText;

    // ***************************
    // ***  �װ۸�̧�ف@�쐬
    // ***************************
    s_LogText = "[" + FormatDateTime("yyyy/mm/dd", Date()) + " " +
        FormatDateTime("hh:nn:ss", Time()) + "]  ";

    s_LogText = s_LogText + LogText + "\n";

    FILE *fp;
    AnsiString Err_Msg;

    if ((fp = fopen(G_ErrLogFilePath.c_str(), "a+")) == NULL) {
        fclose(fp);
        // delete fp;
        return false;
    }

    fputs(s_LogText.c_str(), fp);
    fclose(fp);
    // delete fp;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �G�N�Z���@�G���[���O�Z�b�g
// �T  �v      �F
// ��  ��      �F AnsiString inText   // �������ݕ�����
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
void __fastcall Excel_Err_DataSet(AnsiString inText) {
    AnsiString s_Old_SheetName;
    AnsiString s_CellText;

    int i;

    // ���݂̃V�[�g���̂�ۑ�
    s_Old_SheetName = exWorksheet.OlePropertyGet("Name");
    // ��Ă̎w��
    // exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_ERR_SHEETNAME);
    // Windows 2008Server �ڐA�Ή�
    exWorksheet = exWorksheets.OlePropertyGet("Item",
        (OleVariant)EXCEL_ERR_SHEETNAME);
    // �ŏI�s�ɃZ�b�g
    s_CellText = "Def";
    i = 1;
    while (s_CellText != "") {
        // exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << i << 2);
        // s_CellText = exWorkCell.Exec(PropertyGet("Value"));
        // Windows 2008Server �ڐA�Ή�
        exWorkCell = exWorksheet.OlePropertyGet("Cells", i, 2);
        s_CellText = exWorkCell.OlePropertyGet("Value");

        i++;
    }

    // �G���[���O��������
    // exWorkCell.Exec(PropertySet("Value") << inText );
    // �ۑ����Ă������V�[�g���Ďw��
    // exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);
    // Windows 2008Server �ڐA�Ή�
    exWorkCell.OlePropertySet("Value", (OleVariant)inText);
    exWorksheet = exWorksheets.OlePropertyGet("Item",
        (OleVariant)s_Old_SheetName);

}

// ---------------------------------------------------------------------------
// ���{��֐����F �G�N�Z���@�Z���f�[�^�Z�b�g
// �T  �v      �F
// ��  ��      �F int Row            // �s�ԍ�
// �F int Col            // ��ԍ�
// �F AnsiString inText  // �������ݕ�����
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
void __fastcall Excel_Cell_DataSet(int Row, int Col, AnsiString inText) {
    if (inText.SubString(1, 1) != "'")
        inText = "'" + inText;

    // exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << Row << Col);
    // exWorkCell.Exec(PropertySet("Value") << inText );
    // Windows 2008Server �ڐA�Ή�
    exWorkCell = exWorksheet.OlePropertyGet("Cells", Row, Col);
    exWorkCell.OlePropertySet("Value", (OleVariant)inText);
}

// ---------------------------------------------------------------------------
// ���{��֐����F �G�N�Z���@�Z���f�[�^�Q�b�g�iOGI 2015.02.20�j
// �T  �v      �F
// ��  ��      �F int Row            // �s�ԍ�
//             �F int Col            // ��ԍ�
// �߂�l      �F AnsiString OutText // �ǂݍ��ݕ�����
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
AnsiString __fastcall Excel_Cell_DataGet(int Row, int Col) {
    exWorkCell = exWorksheet.OlePropertyGet("Cells", Row, Col);
	return exWorkCell.OlePropertyGet("Value");
}

//Add_Str 2019/02/22 Ri
// ---------------------------------------------------------------------------
// ���{��֐����F �G�N�Z���@���Ɍr���iOGI 2019.02.22�j
// �T  �v      �F
// ��  ��      �F int Row            // �s�ԍ�
//             �F int Col            // ��ԍ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
void __fastcall Excel_Cell_SetUnderline(int Row1, int Col1, int Row2, int Col2){
	//exWorkCell = exWorksheet.OlePropertyGet("Cells", Row, Col);
	//exWorkCell.OlePropertySet("Underline",true);
	//exWorkCell.OlePropertyGet("Font").OlePropertySet("Underline",true);

	exWorksheet.OlePropertyGet("Range", exWorksheet.OlePropertyGet("Cells", Row1, Col1), exWorksheet.OlePropertyGet("Cells", Row2, Col2)).OlePropertyGet("Borders").OlePropertyGet("Item", 9).OlePropertySet("LineStyle",1);

	//exWorkCell.OlePropertyGet("Borders").OlePropertyGet("Item", 9).OlePropertySet("LineStyle",1);


}
//Add_End 2019/02/22 Ri

// ---------------------------------------------------------------------------
// ���{��֐����F �G�N�Z���@�w�b�_�Z�b�g
// �T  �v      �F �W�����i�\�ŁA���p�̏ꍇ�̂݃w�b�_�Ɂu���ނR�v���o��
// ��  ��      �F -
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
//
// 2007.11.15 �~���[�d�l�Ή� �~���[����"���ނT"���o�͂���
// ---------------------------------------------------------------------------
void __fastcall Excel_HeaderSet(bool bMirror) {
    AnsiString sOutput;

    // --------------------------------------
    // 2007.11.15 �~���[�d�l���̏o�͂�ύX
    if (!bMirror) {
        sOutput = "���ނR�@&P/&N";
    }
    else {
        sOutput = "���ނT�@&P/&N";
    }
    // 2007.11.15
    // --------------------------------------

    // exWorkCell = exWorksheet.Exec(PropertyGet("PageSetUp"));
    // exWorkCell.Exec(PropertySet("RightHeader") << sOutput );
    // Windows 2008Server �ڐA�Ή�
    exWorkCell = exWorksheet.OlePropertyGet("PageSetUp");
    exWorkCell.OlePropertySet("RightHeader", (OleVariant)sOutput);
}

// ---------------------------------------------------------------------------
// ���{��֐����F �G�N�Z���@�W�����i�\�y�[�W�폜�i�Z���폜�j
// �T  �v      �F
// ��  ��      �F int Row            // �s�ԍ�
// �F int Col            // ��ԍ�
// �F AnsiString inText  // �������ݕ�����
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
void __fastcall Excel_Cell_Delete(int iLastPage) {
	AnsiString sDeleteCells;
    sDeleteCells = "A" + FormatFloat("0", DefExcelPageLine * iLastPage + 1) +
        ":CD500";
	exWorkCell = exWorksheet.Exec(PropertyGet("Range") << sDeleteCells).Exec
        (Function("Delete"));
}

// ---------------------------------------------------------------------------
// ���{��֐����F �S�p���������p����
// �T  �v      �F
// ��  ��      �F AnsiString str   �ϊ��Ώە�����
// �߂�l      �F �ϊ���̌���
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall ZenToHan(AnsiString str) {
    AnsiString strw;
    char *c1;

    c1 = (char*)GlobalAlloc(GPTR, str.Length() + 1);

    LCMapString(GetUserDefaultLCID(), LCMAP_HALFWIDTH, str.c_str(),
        str.Length() + 1, c1, str.Length() + 1);
    strw = c1;
    GlobalFree(c1);
    return strw;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ���p�������S�p����
// �T  �v      �F
// ��  ��      �F AnsiString str   �ϊ��Ώە�����
// �߂�l      �F �ϊ���̌���
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString HanToZen(AnsiString str) {
    char s[256];
    LCMapString(GetUserDefaultLCID(), LCMAP_FULLWIDTH, str.c_str(),
        str.Length() + 1, s, 256);

    return AnsiString(s);
}

// ---------------------------------------------------------------------------
// ���{��֐����F �K�i����ð��ف@����
// �T  �v      �F
// ��  ��      �F AnsiString Code  // �����Ώۺ���
// bool bWaEi
// bool bDocType
// AnsiString LangCode   // �a���p��
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code, bool bWaEi,
    bool bDocTyp, AnsiString LangCode) {
    int i;

    AnsiString s_Name;

    s_Name = "";
    // if ( Code == "" ) return "";
    for (i = 0; i < G_KIKAKU_RecCnt; i++) {
        if (G_KIKAKU[i][0].ToIntDef(0) == Code.ToIntDef(0)) {
            if (bDocTyp == true) {
                if (bWaEi == true) {
                    if (Code.ToIntDef(0) == 0) {

                        // ---------------------------------------
                        // 2007.10.04 �װ�d�l�Ή�
                        // �֐��������g�p��������ɕύX
                        // if (G_HEADS_DATA[1].TrimRight() == "0") {    //�a��
                        if (LangCode == "0") {
                            // ---------------------------------------

                            s_Name = G_KIKAKU[i][1] + "�i���@�a���j";
                        }
                        else {
                            s_Name = G_KIKAKU[i][1] + "�i���@�p���j";
                        }
                    }
                    else {
                        s_Name = G_KIKAKU[i][1];
                    }
                }
                else {
                    s_Name = G_KIKAKU[i][2];
                }
            }
            else {
                if (bWaEi == true) {
                    s_Name = G_KIKAKU[i][3];
                }
                else {
                    s_Name = G_KIKAKU[i][4];
                }
            }
            break;
        }
    }

    return s_Name;
}

////---------------------------------------------------------------------------
//// ���{��֐����F �K�i����ð��ف@����
//// �T  �v      �F
//// ��  ��      �F AnsiString Code  // �����Ώۺ���
//// �߂�l      �F ��������
//// ��  �l      �F
////---------------------------------------------------------------------------
// AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code, bool bWaEi)
// {
// int i;
//
// AnsiString s_Name;
//
// s_Name = "";
// if ( Code == "" ) return "";
// for(i=0; i < G_KIKAKU_RecCnt; i++)
// {
// if (G_KIKAKU[i][0] == Code){
// if ( bWaEi == true ) {
// if ( Code == "00" ) {
// if (G_HEADS_DATA[1].TrimRight() == "0") {    //�a��
// s_Name = G_KIKAKU[i][1] + "�i���@�a���j";
// } else {
// s_Name = G_KIKAKU[i][1] + "�i���@�p���j";
// }
// } else {
// s_Name = G_KIKAKU[i][1];
// }
// } else {
// s_Name = G_KIKAKU[i][2];
// }
// break;
// }
// }
//
// return s_Name;
// }

// ---------------------------------------------------------------------------
// ���{��֐����F �K�i����ð��ف@����
// �T  �v      �F
// ��  ��      �F AnsiString Code  // �����Ώۺ���
// �F            Sw    // J:�a���CE:�p��
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_TOSOU_CODE(AnsiString Code, AnsiString Sw) {
    int i;

    AnsiString s_Name;

    s_Name = "";

    for (i = 0; i < G_TOSOU_RecCnt; i++) {
        if (G_TOSOU[i][0] == Code) {
            if (Sw == "J") {
                s_Name = G_TOSOU[i][1]; // �a��
            }
            else {
                s_Name = G_TOSOU[i][2]; // �p��
            }
            break;
        }
    }

    return s_Name;
}

// ---------------------------------------------------------------------------
// ���{��֐����F HEADS�f�[�^�@�Ǎ�
// �T  �v      �F �����œn���ꂽHEADŞ�ق�ǂݍ��ݕ�����z��ɃZ�b�g����B
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall Read_HEADS_Data(AnsiString filepass) {
    char buf[1024]; /* ������o�b�t�@ */
    char *tok; /* �g�[�N���|�C���^ */
    int i; /* ���[�v�J�E���^ */

    AnsiString wk_FilePass;
    FILE *fp;

    // *******************************
    // ***  HEADS�f�[�^�@�Ǎ�
    // *******************************
    wk_FilePass = filepass;
    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    i = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        tok = GetToken(buf, "\n");
        G_HEADS_DATA[i] = tok;
        // �R���f���T�Ή��H
        if (i == 34) {
            if (G_HEADS_DATA[i].SubString(3, 1) == "C") {
                G_HEADS_DATA[i] = G_HEADS_DATA[i].SubString(1, 2);
            }
        }
        i++;
    }

    // ***************************************
    // No:0�`315 ���ް��� No:507�`822�ɺ�߰
    // ���ۂ�HEADS�ް��ɂ͊i�[����Ă��Ȃ��ׁB
    // (02.08.21�ǉ�)
    // ***************************************
    for (i = 507; i < 822; i++) {
        //2019.10.10 NK�ʎY�O���L_S�@�@�@
        //if (i != 539) { // �h���F�̘a�p�Ή��̂���539���g�p�B�㏑������ƃ}�Y���̂Ń`�F�b�N�ŊO��   2003/06/09
        if (i == 537) {
            // �m�j�ʎY�O�^�����ӊ��N�o�͂̂���537���g�p�B�㏑������ƃ}�Y���̂Ń`�F�b�N�ŊO��
        } else if (i == 539) {
            // �h���F�̘a�p�Ή��̂���539���g�p�B�㏑������ƃ}�Y���̂Ń`�F�b�N�ŊO��
        } else {
        //2019.10.10 NK�ʎY�O���L_E
            G_HEADS_DATA[i] = G_HEADS_DATA[i - 507];
        }
    }

    fclose(fp); // ����
    // delete fp;

    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F HEADS�f�[�^�@�Ǎ�
// �T  �v      �F HEADŞ�ق�ǂݍ��݁A���r�W�����f�[�^(318)���擾����B
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2004/10/25�@add
// ---------------------------------------------------------------------------
bool __fastcall Read_HEADS_Data_Rev(AnsiString filepass) {
    char buf[1024]; /* ������o�b�t�@ */
    char *tok; /* �g�[�N���|�C���^ */
    int i; /* ���[�v�J�E���^ */
    AnsiString work; /* ��Ɨp */
    AnsiString work_DATA[1500];

    AnsiString wk_FilePass;
    FILE *fp;

    // *******************************
    // ***  HEADS�f�[�^�@�Ǎ�
    // *******************************
    wk_FilePass = filepass;
    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        fclose(fp); // ����
        return false;
    }

    // �Ǎ��݉\�̊�
    i = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        tok = GetToken(buf, "\n");
        work_DATA[i] = tok;

        // change 2005-09-22 ���r�W�����ԍ���319�s�ځH
        // if (i == 317){
        if (i == 319) {
			G_HEADS_DATA_Rev = work_DATA[i].ToIntDef(0);
            break;
        }
        i++;
    }

    fclose(fp); // ����
    // delete fp;
    return true;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �\���i�ް��@�W�v����(��ڰĕ��i�ް��쐬�p)
// �T  �v      �F �\���i�ް�����ڰč\�����i����ð��ق���\���i�������߂�(1���̂�)
// ��  ��      �F AnsiString a_Katasiki : ��ڰč\�����i����ð��� 5���
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 02.08.03�ǉ�
// ---------------------------------------------------------------------------
int __fastcall Syukei_Buhin_Yobisu(AnsiString a_Katasiki, AnsiString a_Zaisitu,
    HeadsData* pcHeadsData) {
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
    int i_Yobisu;
    int i_lop;

    i_RecNo = 0; // ں��޶���(���o������v����)
    i_Yobisu = 0;

    // ---------------------------------------
    // 2007.10.04 �װ�d�l�Ή�
    // �װB�d�l�̏ꍇ�A�\���i���v�サ�Ȃ�
    if (pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B)
        return 0;
    // 2007.10.04
    // ---------------------------------------

    // ��x�v�サ���\���i�͌v�サ�Ȃ�
    for (i_lop = 0; i_lop < G_YOBICHK_RecCnt; i_lop++) {
        if ((a_Katasiki == G_YOBICHK_NAME[i_lop]) &&
            (a_Zaisitu == G_YOBICHK_MAT[i_lop])) {
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
                if (s_YBKey == a_Katasiki) {
                    if (((a_Zaisitu != "0") && (s_YBZaisitu == a_Zaisitu)) ||
                        (a_Zaisitu == "0")) {
                        i_Yobisu += s_Yobi_Text[2].ToIntDef(0);
                        i_RecNo++;
                        G_YOBICHK_NAME[G_YOBICHK_RecCnt] = a_Katasiki;
                        G_YOBICHK_MAT[G_YOBICHK_RecCnt] = a_Zaisitu;
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
// ���{��֐����F HD_MATERIALð��ٌ�������
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F �ގ�����
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_HD_MATERIAL(AnsiString Key) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;
    AnsiString s_Name;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
    s_SQL = s_SQL + "  WHERE MAT_CODE = '" + Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_MAT;
    wkQuery->ConnectionName = ODBC_DSN_MAT;
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // �e���ڂ̏�����
    s_Name = "";

    while (!wkQuery->Eof) {
        // if(G_HEADS_DATA[1].TrimRight() == "0" ){
        // �a�����̂̎擾
        s_Name = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
        // } else {
        // s_Name = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
        // }

        wkQuery->Next();
    }

    delete wkQuery;
    return s_Name;
}

// ---------------------------------------------------------------------------
// ���{��֐����F HD_MATERIALð��ٌ�������
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F �ގ�����
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_HD_E_MATERIAL(AnsiString Key) {
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	//TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
	TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;
    AnsiString s_Name;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
    s_SQL = s_SQL + "  WHERE MAT_CODE = '" + Key + "'";

    wkQuery->Close();
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	//wkQuery->DatabaseName = ODBC_DSN_MAT;
	wkQuery->ConnectionName = ODBC_DSN_MAT;
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // �e���ڂ̏�����
    s_Name = "";

    while (!wkQuery->Eof) {
        // if(G_HEADS_DATA[1].TrimRight() == "0" ){
        // �a�����̂̎擾
        // s_Name = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
        // } else {
        // �p�����̂̎擾
        s_Name = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
        // }

        wkQuery->Next();
    }

    delete wkQuery;
    return s_Name;
}

// ---------------------------------------------------------------------------
// ���{��֐����F HD_MATERIALð��ٌ�������
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F �ގ�����
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_HD_MAT_JtoE(AnsiString Key) {
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	//TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
	TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;
    AnsiString s_Name;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
    s_SQL = s_SQL + "  WHERE JAPANESE_NAME = '" + Key + "'";

    wkQuery->Close();
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	//wkQuery->DatabaseName = ODBC_DSN_MAT;
	wkQuery->ConnectionName = ODBC_DSN_MAT;
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // �e���ڂ̏�����
    s_Name = "";

    while (!wkQuery->Eof) {
        // �p�����̂̎擾
        s_Name = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();

        wkQuery->Next();
    }

    delete wkQuery;
    return s_Name;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �ގ����ޕϊ�Ͻ��@�ϊ���R�[�h�擾
// �T  �v      �F �����Ƃ��ēn���ꂽ���ނōގ����ޕϊ�Ͻ�����������B
// ��  ��      �F Code�F�����p����
// �F Type�F�ϊ��^�C�v
// �߂�l      �F �ϊ�����
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall Chg_Zaisitu_Code(AnsiString Code, AnsiString Typ) {
    // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	//TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
	TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_E
    AnsiString s_Name;
    AnsiString s_SQL;
    AnsiString s_ChgName;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_CHG_MATERIAL";
    s_SQL = s_SQL + "  WHERE MAT_CODE_KEY = '" + Code + "';";

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
        s_Name = "";
    }
    else {
        s_ChgName = "MAT_CODE_RESULT" + Typ;
        s_Name = wkQuery->FieldByName(s_ChgName)->AsString.TrimRight();
    }

    delete wkQuery;

    return s_Name;
}

// ---------------------------------------------------------------------------
// ���{��֐����F HD_FRNG_MSTð��ٌ�������
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall Search_HD_FRNG_MST(AnsiString Key, AnsiString FSize,
    AnsiString SEDiv, AnsiString PG, AnsiString TMP, AnsiString Drn,
    AnsiString Air, bool bRing) {
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	//TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
	wkQuery = new TFDQuery(DataModule1);
	// 2021.06.18 FireDAC�ւ̈ڍs MOD_E

    AnsiString s_SQL;
    AnsiString s_Tankan;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_FRNG_MST";
    s_SQL = s_SQL + "  WHERE SERIES_MODEL = '" + Key + "'";
	// 2021.06.24 SQL���̑Ή��@MOD_S
	//s_SQL = s_SQL + "    AND SIZE = '" + FSize + "'";
	s_SQL = s_SQL + "    AND [SIZE] = '" + FSize + "'";
	// 2021.06.24 SQL���̑Ή��@MOD_E
    s_SQL = s_SQL + "    AND SEDIV = '" + SEDiv + "'";
    s_SQL = s_SQL + "    AND PGAGE = '" + PG + "'";
    //2020.01.08 �ݸޒ[�ǌ����ύX_S
    //if (bRing == false)
    //    s_SQL = s_SQL + "    AND TEMP = '" + TMP + "'"; // �����O�[�ǂ̂o���L���͌��Ȃ�
    s_SQL = s_SQL + "    AND TEMP = '" + TMP + "'";
    //2020.01.08 �ݸޒ[�ǌ����ύX_E
    s_SQL = s_SQL + "    AND DRN = '" + Drn + "'";
    s_SQL = s_SQL + "    AND AIR = '" + Air + "'";

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
        s_Tankan = "";
    }
    else {
        s_Tankan = wkQuery->FieldByName("TANKAN")->AsString.TrimRight(); // �}��

        G_Log_Text = "�[�ǖ���    �" + s_Tankan + "����擾�B";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;
    return s_Tankan;
}

// ---------------------------------------------------------------------------
// ���{��֐����F HD_FRNG_MSTð��ٌ�������
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F ��������
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall Get_Yobi_Code(AnsiString Key) {
    AnsiString sRetCode;

    switch (Key.ToIntDef(0)) {
    case 6: // �c�v���[�g�K�X�P�b�g
        sRetCode = "GSKT02";
        break;
    case 7: // �c�v���[�g�K�X�P�b�g�`
        sRetCode = "GSKT08";
        break;
    case 8: // �c�v���[�g�K�X�P�b�g�a
        sRetCode = "GSKT09";
        break;
    case 55: // E�v���[�g�K�X�P�b�g�`
    case 56: // �d�v���[�g�K�X�P�b�g�a
    case 57: // �d�v���[�g�K�X�P�b�g
    case 60: // �d�v���[�g�K�X�P�b�g�d
        sRetCode = "GSKT03";
        break;
    case 13: // �c�f�B�X�^���X�s�[�X
    case 78: // �f�B�X�^���X�s�[�X
        sRetCode = "GSKT06";
        break;
    case 15: // �c�v���[�g�K�X�P�b�g�a�|�S
        sRetCode = "GSKT13";
        break;
    case 16: // �c�v���[�g�K�X�P�b�g�`�|�Q
        sRetCode = "GSKT12";
        break;
    case 79: // �d�m�Y���K�X�P�b�g
        sRetCode = "GSKT04";
        break;
    case 64: // �d�f�B�X�^���X�s�[�X
        sRetCode = "GSKT14";
        break;
    default:
        sRetCode = "";
        break;
    }

    return sRetCode;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �^�����̕ϊ�
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
// ---------------------------------------------------------------------------
bool __fastcall Get_PHE_Model(AnsiString *rs_Syurui, AnsiString *rs_Model,
    AnsiString s_Syurui, AnsiString s_Model) {

    int i;
    int i_Mojisu;
    AnsiString s_Text;
    // AnsiString s_Syurui;
    // AnsiString s_Model;

    // ****************************************************************************
    // ***                                                                      ***
    // ***                        �v���[�g�^���̌���                            ***
    // ***                                                                      ***
    // ****************************************************************************

    // -----------------------------------------------
    // 2007.10.04 �֐���������擾����悤�ύX
    // s_Syurui = G_HEADS_DATA[34].TrimRight();                // HEADS�ް� ���
    // -----------------------------------------------
    // HEADS�ް� ����

    // RecNo.542���ق̎擾���@�̕ύX
    // -----------------------------------------------
    // 2007.10.04 �֐���������擾����悤�ύX
    // s_Model  = G_HEADS_DATA[35].TrimRight(); // HEADS�ް� ����
    // -----------------------------------------------
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
    ////�@�@�F�Ōォ��Q�����ڂ̕����� "C","P","R","A","B"�̏ꍇ
    ////�@�@�F���̕���������s_Model���珜���Ă���
    ////�@�@�F���ɍŌォ��Q�����ڂ̕���(����)������
    ////*********************
    ////�Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
    ////���̑��̏ꍇ                       �F�Ō�̕���(����)
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // if (i_Mojisu == 3){
    // s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,2);
    // } else if (i_Mojisu == 4){
    // s_Text = G_HEADS_DATA[35].TrimRight().SubString(4,1);
    // if (s_Text.ToIntDef(999) != 999){
    // s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,3);
    // } else {
    // s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,2);
    // }
    // } else if (i_Mojisu == 5){
    // s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,3);
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

    *rs_Syurui = ""; // ���̎��
    *rs_Model = ""; // ��������

    G_Log_Text = "��ڰČ^���ϊ�ð��ق��A��ށw" + s_Syurui + "�x���f���w" + s_Model +
        "�x�Ō����J�n�B";
    Write_Log(G_Log_Text);

    for (i = 0; i < G_PHENKAN_RecCnt; i++) {
        if (G_PHENKAN[i][0] == s_Syurui && G_PHENKAN[i][1] == s_Model) {

            *rs_Syurui = ZenToHan(G_PHENKAN[i][2]); // ���̎��
            *rs_Model = ZenToHan(G_PHENKAN[i][3]); // ��������

            break;
        }
    }

    G_Log_Text = "���̎��            �w" + *rs_Syurui + "�x���擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��������            �w" + *rs_Model + "�x���擾�B";
    Write_Log(G_Log_Text);
    G_Log_Text = "��ڰČ^���ϊ�ð���  �����I���B";
    Write_Log(G_Log_Text);

    return true;
}

// -----------------------------------------------------------------------------
// ���{��֐����F �m�Y���p��̋K�i���擾
// �T  �v      �F
// �p�����[�^  �F �K�i�R�[�h
// �߂�l      �F 0:JIS 1:ANSI/JPI 2:�T�j�^��
// �@�\����    �F
// ��  �l      �F
// -----------------------------------------------------------------------------
int __fastcall GetNzKikaku(AnsiString a_KCd) {
    int i_Kikaku;

    // 2011.06.20 ɽ�ًK�i�ύX
    // if (a_KCd.SubString(1,1) == "6" ) {        // �T�j�^��
    // i_Kikaku = 2;
    // } else if (a_KCd.SubString(1,1) == "8" ||
    // a_KCd.SubString(1,1) == "9" ||
    // a_KCd.SubString(1,1) == "4" ||
    // a_KCd.SubString(1,1) == "5") {
    if (a_KCd.SubString(1, 2) == "06") { // �T�j�^��
        i_Kikaku = 2;
        // 2014.06.24 ɽ�ًK�i�ύX
        // } else if (a_KCd.SubString(1,2) == "04" ||
        // a_KCd.SubString(1,2) == "05" ||
        // a_KCd.SubString(1,2) == "08" ||
        // a_KCd.SubString(1,2) == "09" ||
        // a_KCd.SubString(1,2) == "11" ||
        // a_KCd.SubString(1,2) == "12" ||
        // a_KCd.SubString(1,2) == "13" ||
        // a_KCd.SubString(1,2) == "21" ||
        // a_KCd.SubString(1,2) == "22" ||
        // a_KCd.SubString(1,2) == "23") {
        //// ***********************
        // i_Kikaku = 1;
        //// 2013.04.03 ɽ�ًK�i�ǉ�
        // } else if (a_KCd.SubString(1,2) == "14" ||
        // a_KCd.SubString(1,2) == "24" ||
        // a_KCd.SubString(1,2) == "31" ||
        // a_KCd.SubString(1,2) == "32" ||
        // a_KCd.SubString(1,2) == "33" ||
        // a_KCd.SubString(1,2) == "34" ||
        // a_KCd.SubString(1,2) == "44" ||
        // a_KCd.SubString(1,2) == "48") {
        // i_Kikaku = 1;
        //// *********************
    }
    else if (a_KCd.SubString(1, 2) == "04" || a_KCd.SubString(1, 2) == "05" ||
        a_KCd.SubString(1, 2) == "08" || a_KCd.SubString(1, 2) == "09" ||
        a_KCd.SubString(1, 2) == "44" || a_KCd.SubString(1, 2) == "48") {
        // ANSI,JPI
        i_Kikaku = 1;
        // *************************
    }
    else {
        i_Kikaku = 0;
    }
    return i_Kikaku;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �t�����W�V���J�Q���ʎ擾
// �T  �v      �F Silicagel.TXT����t�����W�V���J�Q���ʂ��擾����
// ��  ��      �F AnsiString Key
// �߂�l      �F �V���J�Q����(kg)
// ��  �l      �F 2020/03/12 �ǉ�
// ---------------------------------------------------------------------------
AnsiString __fastcall Get_Silicagel( AnsiString Key ) {
    // CSV�Ǎ��p
    FILE *wkfp;

    char buf[1024]; // ������o�b�t�@
    char *tok; // �g�[�N���|�C���^
    int i; // ���[�v�J�E���^
    char token[100][100]; // �g�[�N���o�b�t�@

    AnsiString s_Silicagel;
    AnsiString wk_FilePass;
    int i_RecNo;
    int i_Yobisu;
    int i_lop;

    i_RecNo = 0; // ں��޶���(���o������v����)
    i_Yobisu = 0;

    // Silicagel.TXT �ް��߽
    wk_FilePass = G_AnaFilePass + "Silicagel.TXT";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "Silicagel.TXT �w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
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
        }
    }

    // �ض����ް������݂��Ȃ������ꍇ
    if ( Key == "A" ) {
        if (token[0] == "") {
            s_Silicagel = "  0.00";
        }
        else {
            s_Silicagel = token[0];
        }
    }
    else {
        if (token[1] == "") {
            s_Silicagel = "  0.00";
        }
        else {
            s_Silicagel = token[1];
        }
    }

    // Silicagel.TXT ̧�ٸ۰��
    fclose(wkfp);
    return s_Silicagel;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �����������L�擾
// �T  �v      �F HotAirDrying.TXT���牷���������L���擾����
// ��  ��      �F �Ȃ�
// �߂�l      �F �����������L
// ��  �l      �F 2020/03/12 �ǉ�
// ---------------------------------------------------------------------------
AnsiString __fastcall Get_HotAirDrying(void) {
    // CSV�Ǎ��p
    FILE *wkfp;

    char buf[1024]; // ������o�b�t�@
    char *tok; // �g�[�N���|�C���^
    int i; // ���[�v�J�E���^
    char token[100][100]; // �g�[�N���o�b�t�@

    AnsiString s_HotAirDrying;
    AnsiString wk_FilePass;
    int i_RecNo;
    int i_Yobisu;
    int i_lop;

    i_RecNo = 0; // ں��޶���(���o������v����)
    i_Yobisu = 0;

    // HotAirDrying.TXT �ް��߽
    wk_FilePass = G_AnaFilePass + "HotAirDrying.TXT";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // �װ۸ލ쐬
        G_ErrLog_Text = "HotAirDrying.TXT �w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
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
        }
    }

    // �����������L�����݂��Ȃ������ꍇ
    if (token[0] == "") {
        s_HotAirDrying = "";
    }
    else {
        s_HotAirDrying = token[0];
    }

    // HotAirDrying.TXT ̧�ٸ۰��
    fclose(wkfp);
    return s_HotAirDrying;
}

// ---------------------------------------------------------------------------
// ���{��֐����F ������Ԏ擾
// �T  �v      �F HD_sakuban.txt���珈���Ώۂ̍�Ԃ��擾����
// ��  ��      �F -
// �߂�l      �F �������� [���]
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall Get_Sakuban_File(void) {
    // CSV�Ǎ��p
    FILE *wkfp;

    char buf[1024]; // ������o�b�t�@
    char *tok; // �g�[�N���|�C���^
    int i; // ���[�v�J�E���^
    char token[100][100]; // �g�[�N���o�b�t�@

    AnsiString s_sakubanTXT;
    AnsiString wk_FilePass;
    int i_RecNo;
    int i_Yobisu;
    int i_lop;

    i_RecNo = 0; // ں��޶���(���o������v����)
    i_Yobisu = 0;

    // ��ڰė\���i�ް��߽
    wk_FilePass = G_HD_Root + "HD_sakuban.txt";

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
        }
    }

    // �Y���\���i�ް������݂��Ȃ������ꍇ
    if (token[0] == "")
        s_sakubanTXT = 0;
    else
        s_sakubanTXT = token[0];

    // �\���i�ް� ̧�ٸ۰��
    fclose(wkfp);
    return s_sakubanTXT;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �t�������a�\�L�ϊ�
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F �ϊ�����
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall ChgIDFSize(double dSize) {
    AnsiString sSizeStr;

    sSizeStr = "";
    if (dSize == 1.0) {
        sSizeStr = "1";
    }
    else if (dSize == 1.5) {
        // 2005.02.23 �����ɕύX
        // sSizeStr = "1 1/2";
        sSizeStr = "1.5";
    }
    else if (dSize == 2.0) {
        sSizeStr = "2";
    }
    else if (dSize == 2.5) {
        // 2005.02.23 �����ɕύX
        // sSizeStr = "2 1/2";
        sSizeStr = "2.5";
    }
    else if (dSize == 3.0) {
        sSizeStr = "3";
    }
    else if (dSize == 4.0) {
        sSizeStr = "4";
    }
    return sSizeStr;
}

// ---------------------------------------------------------------------------
// ���{��֐����F �ގ���ނ̎擾
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F �ގ�����
// �߂�l      �F �ގ��̎��
// ��  �l      �F 2016.08.29�ǉ�
// ---------------------------------------------------------------------------
AnsiString __fastcall Type_MAT(AnsiString Key) {

    AnsiString s_Type;

    // �e���ڂ̏�����
    s_Type = "";

    if ( Key == "1014" || Key == "3014" || Key == "3114" ||  Key == "3214" ||
         Key == "3314" || Key == "3414" ) {
        // TP270
        s_Type = "Ti";
    }
    else if ( Key == "1015" || Key == "3115" || Key == "3215" ||
              Key == "3315" || Key == "3415" || Key == "3515" ) {
        // TP270-Pd
        s_Type = "Ti";
    }
    else if ( Key == "1032" || Key == "3032" || Key == "3132" ||
              Key == "3232" || Key == "3332" || Key == "3432" ) {
        // TP340
        s_Type = "Ti";
    }
    else if ( Key == "1033" || Key == "3133" || Key == "3233" ||
              Key == "3333" ) {
        // TP340-Pd
        s_Type = "Ti";
    }
    else if ( Key == "1057" || Key == "3057" || Key == "3157" ) {
        // TP480
        s_Type = "Ti";
    }
    else if ( Key == "1058" || Key == "3158" ) {
        // TP480-Pd
        s_Type = "Ti";
    }
    else if ( Key == "1085" ) {
        // SB-265-Gr.1(��)
        s_Type = "Ti";
    }
    else if ( Key == "1089" ) {
        // SB-265-Gr.2(��)
        s_Type = "Ti";
    }
    else if ( Key == "1075" ) {
        // SB-265-Gr.7(��)
        s_Type = "Ti";
    }
    else if ( Key == "1076" ) {
        // SB-265-Gr.11(��)
        s_Type = "Ti";
    }
    else if ( Key == "1095" ) {
        // SB-265-Gr.1(��)
        s_Type = "Ti";
    }
    else if ( Key == "1090" ) {
        // B-265-Gr.2(��)
        s_Type = "Ti";
    }
    //2016.11.08 �ގ����ޕύX
    //else if ( Key == "1051" || Key == "3151" ) {
    //    // STPG
    //    s_Type = "STPG";
    //}
    //else if ( Key == "1060" || Key == "3160" ) {
    //    // SGP
    //    s_Type = "SGP";
    //}
    else if ( Key == "1051" ) {
        // STPG
        s_Type = "STPG";
    }
    else if ( Key == "1060" ) {
        // SGP
        s_Type = "SGP";
    }
     // **********************
    else if ( Key == "1043" || Key == "1187" ) {
        // CAC703(AL-BRONZE)
        s_Type = "ALBC";
    }
    else if ( Key == "1151" ) {
        // STKM13A(CS)
        s_Type = "CS";
    }
    else if ( Key == "" || Key == "0000"|| Key == "1098" ) {
        // �ގ����w��
        s_Type = "-";
    }
    //2016.11.08 �ގ����ޕύX
    else if ( Key == "0" ) {
        // �ގ����w��
        s_Type = "-";
    }
    else if ( Key == "1001" || Key == "3001" || Key == "3101" ) {
        // SUS304
        s_Type = "SUS304";
    }
    else if ( Key == "1003" || Key == "3003" || Key == "3103" ) {
        // SUS304
        s_Type = "SUS316";
    }
    // 2018.03.20 ���x�v���ގ��ύX_S
    else if ( Key == "1004" || Key == "3004" || Key == "3104" ) {
        // SUS316L
        s_Type = "SUS316L";
    }
    // 2018.03.20 ���x�v���ގ��ύX_E
    else if ( Key == "1040" || Key == "1062" ) {
        // SS400
        s_Type = "SS400";
    }
    // **********************
    // 2022.04.28 UX-30L�ǉ�_S
    else if ( Key == "1168" ) {
        // S275JR
        s_Type = "S275JR";
    }
    // 2022.04.28 UX-30L�ǉ�_E
    //2018.02.09 ���_�p�r���L�ǉ�_S
    else if ( Key == "1010" || Key == "3010" || Key == "3110" ||
              Key == "3210" || Key == "3310" || Key == "3410" ||
              Key == "3510" || Key == "3810" ) {
        // HASTELLOY B
        s_Type = "HAS";
    }
    else if ( Key == "1009" || Key == "3009" || Key == "3109" ||
              Key == "3209" || Key == "3309" || Key == "3409" ||
              Key == "3809" ) {
        // HASTELLOY B2
        s_Type = "HAS";
    }
    else if ( Key == "1067" || Key == "1073" || Key == "3073" ||
              Key == "3273" || Key == "3373" || Key == "3473" ||
              Key == "3873" ) {
        // HASTELLOY C-22
        s_Type = "HAS";
    }
    else if ( Key == "1011" || Key == "1072" || Key == "3072" ||
              Key == "3172" || Key == "3272" || Key == "3372" ||
              Key == "3472" || Key == "3872" ) {
        // HASTELLOY C-276
        s_Type = "HAS";
    }
    else if ( Key == "1077" || Key == "3077" || Key == "3277" ||
              Key == "3377" || Key == "3477" || Key == "3577" ||
              Key == "3877" ) {
        // HASTELLOY C-2000
        s_Type = "HAS";
    }
    else if ( Key == "1020" || Key == "3020" || Key == "3220" ||
              Key == "3320" || Key == "3420" || Key == "3820" ) {
        // HASTELLOY G
        s_Type = "HAS";
    }
    else if ( Key == "2009" ) {
        // L-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2013" ) {
        // LR-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2027" ) {
        // �޲��
        s_Type = "FPM";
    }
    else if ( Key == "2072" ) {
        // R-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2073" ) {
        // G-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2092" ) {
        // AG-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2093" ) {
        // A-FPM
        s_Type = "FPM";
    }
    else if ( Key == "2096" ) {
        // AR-FPM
        s_Type = "FPM";
    }
    //2018.02.09 ���_�p�r���L�ǉ�_E
    //2021.03.10 Ư�ْǉ�_S
    else if ( Key == "3053" || Key == "3054" || Key == "3153" ||
              Key == "3154" || Key == "3253" || Key == "3254" ||
              Key == "3353" || Key == "3354" || Key == "3453" ||
              Key == "3454" ) {
        // Ư��
        s_Type = "NI";
    }
    //2021.03.10 Ư�ْǉ�_E
    //2021.05.17 TCG�ǉ�_S
    else if ( Key == "2058" || Key == "2060" || Key == "2061" ||
              Key == "2062" || Key == "2063" || Key == "2064" ||
              Key == "2065" || Key == "2069" ) {
        // TCG
        s_Type = "TCG";
    }
    //2021.05.17 TCG�ǉ�_E
    else {
        // ���̑�
        s_Type = "ELSE";
    }

    return s_Type;
}


// ---------------------------------------------------------------------------
// ���{��֐����F ALLOY�ނ���JIS�ނւ̕ϊ�
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F �ގ�����
// �߂�l      �F �ގ��̎��
// ��  �l      �F 2024.02.05�ǉ�
// ---------------------------------------------------------------------------
AnsiString __fastcall Alloy_MAT(AnsiString Key) {


    AnsiString s_Mat;

    //ALLOY�ނ̏ꍇJIS�ނɕϊ�
    if ( Key == "3001" ) {
        // ALLOY 304 �� SUS304
        s_Mat = "3101";
    }
    else if ( Key == "3002" ) {
        // ALLOY 304L �� SUS304L
        s_Mat = "3102";
    }
    else if ( Key == "3003" ) {
        // ALLOY 316 �� SUS316
        s_Mat = "3103";
    }
    else if ( Key == "3004" ) {
        // ALLOY 316L �� SUS316L
        s_Mat = "3104";
    }
    else if ( Key == "3006" ) {
        // ALLOY 317 �� SUS317
        s_Mat = "3106";
    }
    else if ( Key == "3007" ) {
        // ALLOY 310S �� SUS310S
        s_Mat = "3107";
    }
    else if ( Key == "3008" ) {
        // ALLOY 317L �� SUS317L
        s_Mat = "3108";
    }
    else {
        // ���̑�
        s_Mat = Key;
    }

     return s_Mat;
}


//2022/12/15 �����ǉ��Ή� Add_S
// ---------------------------------------------------------------------------
// ���{��֐����F �ϊ���̐����ԍ����擾
// �T  �v      �F
// ��  ��      �F AnsiString str   �����ԍ�
// �߂�l      �F �ϊ���̌���
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall GetSeizouBangou(AnsiString str) {
	AnsiString strw;
	AnsiString strTmp;

	if (str.Length() > 7)
	{
	  strTmp = str.SubString(1, 2);
	  if (strTmp == "10")
	  {
		 strw = str.SubString(3,2) + "-" + str.SubString(5,4);
	  }
	  else
	  {
		 strw = str.SubString(1,4) + "-" + str.SubString(5,4);
	  }
	}
	else
	{
	  strw = str.SubString(1,2) + "-" + str.SubString(3,4);
	}

	return strw;
}
//2022/12/15 �����ǉ��Ή� Add_E

//2023/4/21 �����ǉ��Ή� Add_S
// ---------------------------------------------------------------------------
// ���{��֐����F �ϊ���̐����ԍ����擾
// �T  �v      �F
// ��  ��      �F AnsiString str   �����ԍ�
// �߂�l      �F �ϊ���̌���
// ��  �l      �F
// ---------------------------------------------------------------------------
AnsiString __fastcall GetFileSeizouBangou(AnsiString str) {
	AnsiString strw;
	AnsiString strTmp;

	if (str.Length() > 7)
	{
	  strTmp = str.SubString(1, 2);
	  if (strTmp == "10")
	  {
		 strw = str.SubString(3,6);
	  }
	  else
	  {
		 strw = str;
	  }
	}
	else
	{
	  strw = str;
	}

	return strw;
}
//2023/4/21 �����ǉ��Ή� Add_E

// ---------- <<< End of File >>> ----------
