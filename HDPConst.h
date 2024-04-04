// ----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�F�萔��`
// �� �� �ҁFN.Uchida
// �� �� ���F2002.05.23
// �X �V ���F2002.06.18 2007.03
// ---------------------------------------------------------------------------
#ifndef C_HDPCONSTH
#define C_HDPCONSTH

/* ---*---*--- �V�X�e������ ---*---*--- */
#define CSYSNAME   " �o�}�v����M�T�[�r�X�V�X�e�� "
#define CSYSVER    " Ver.1.00 "

/* ---*---*--- �t�@�C������ ---*---*--- */
#define CSYSINIFILENAME       "HD_CV.INI"            // ���ݒ�t�@�C����
#define CSYSINIFILENAME_P     "HD_PARTS.INI"         // ���ݒ�t�@�C����(���i�W�J�p)
#define CSYSLOGFILENAME       "Log_Excel.log"        // ���O�t�@�C����(Excel�쐬����)
#define CSYSLOGFILENAME_P     "Log_Parts.log"        // ���O�t�@�C����(���i�W�J����)
#define CSYSERRLOGFILENAME    "Log_Excel_Err.log"    // �G���[���O�t�@�C����(Excel�쐬����)
#define CSYSERRLOGFILENAME_P  "Log_Parts_Err.log"    // �G���[���O�t�@�C����(���i�W�J����)
// 2021.06.18 FireDAC�ւ̈ڍs MOD_S
#define CSYSCONNDEFFILENAME   "FDConnectionDefs.ini" // �ڑ���`�t�@�C����
// 2021.06.18 FireDAC�ւ̈ڍs MOD_E

/* ---*---*--- �c�a�p�ڑ����� ---*---*--- */
#define ODBC_DSN_DNO      "DSN_DNO"                // �}�ԃf�[�^�x�[�X(HD_DNO.mdb)
#define ODBC_DSN_INS      "DSN_INSPCTL"            // �����v�̏��f�[�^�x�[�X(HD_INSPCTL.mdb)
#define ODBC_DSN_MAT      "DSN_MATERIAL"           // �ގ��f�[�^�x�[�X(HD_MATERIAL.mdb)

/* ---*---*--- EXCEL �Œ�V�[�g�� ---*---*--- */
#define EXCEL_ERR_SHEETNAME    "�G���[���O"        // EXCEL�V�[�g��(�G���[���O�p)

// --> 2007.03 Add
/* ---*---*--- �������я� �K�i�R�[�h ---*---*--- */
#define SEISEKISYO_STANDARDNO_DNV        12                   // �������я�DNV
#define SEISEKISYO_STANDARDNO_ABS        13                   // �������я�ABS
#define SEISEKISYO_STANDARDNO_LR         14                   // �������я�LR
#define SEISEKISYO_STANDARDNO_NK         15                   // �������я�NK
#define SEISEKISYO_STANDARDNO_GL         16                   // �������я�GL
#define SEISEKISYO_STANDARDNO_KR         17                   // �������я�KR
#define SEISEKISYO_STANDARDNO_BV         24                   // �������я�BV
#define SEISEKISYO_STANDARDNO_CCS        27                   // �������я�CCS
// <-- 2007.03 Add

#endif
/* --*-- end --*-- */
