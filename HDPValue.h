// ----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
// ̧�يT�v�F�ϐ���`
// �� �� �ҁFN.Uchida
// �� �� ���F2002.05.28
// �X �V ���F2002.06.18 2007.07
// ---------------------------------------------------------------------------

#ifndef C_HDPVALUEH
#define C_HDPVALUEH

#include "HDPConst.h"   // �萔��`�p

#ifdef  VDEF
#define   EXTERN
#else
#define   EXTERN  extern
#endif

/* ---*---*--- HD_CV.ini̧�ٗp�ϐ� ---*---*--- */
EXTERN AnsiString G_HD_Version; // HEADS�ް�ޮ�
EXTERN AnsiString G_Zuban_DB; // �}���ް��ް�̧�ٖ�
EXTERN AnsiString G_Material_DB; // �ގ��ް��ް�̧�ٖ�
EXTERN AnsiString G_Inspect_DB; // �����v�̏��ް��ް�̧�ٖ�
EXTERN int G_Output_Mode; // �o��Ӱ��
//Add_Str 2019/02/19 Ri
//����w����A4(��)�AA3(�V)�t���O�̎擾
EXTERN int G_OLD_FLG;     // 0:�V(A3)�A1:��(A4)�A2:�V������
//Add_End 2019/02/19 Ri
//2022.09.30 E�ڰ��ް��ǉ�_S
EXTERN int G_EBASE_FLG;   // 0:���A1:�L
//2022.09.30 E�ڰ��ް��ǉ�_E
EXTERN AnsiString G_HD_Root; // ٰ�̫���
EXTERN AnsiString G_HEADS_CV_Dir; // ����̫���
EXTERN AnsiString G_MASTER_Dir; // Ͻ�̫���
EXTERN AnsiString G_TEMPLATE_Dir; // ����ڰ�̫���

EXTERN AnsiString G_HEADS_INP_Dir; // HEADS����̫���
EXTERN AnsiString G_TRIGGER_Dir; // �ض�̫���
EXTERN AnsiString G_CSV_Dir; // CSV̫���
EXTERN AnsiString G_MATERIAL_Dir; // �ގ�����Ͻ�̫���

EXTERN AnsiString G_HEADS_OUT_Dir; // HEADS�o��̫���
EXTERN AnsiString G_OUT_ORDER_Dir; // ���ސ}��̫���
EXTERN AnsiString G_OUT_ESTIM_Dir; // ���ϐ}��̫���

EXTERN AnsiString G_HEADS_DATA_Dir; // HEADS�Ǘ��ް��o��̫���
EXTERN AnsiString G_DAT_ORDER_Dir; // ���ސ}�ʊǗ��ް�̫���
EXTERN AnsiString G_DAT_ESTIM_Dir; // ���ϐ}�ʊǗ��ް�̫���

// 2004.09.23�ǉ�
EXTERN AnsiString G_OUT_DIRECT_Dir; // �}���W�o���ް�̫���

// 2004.10.22�ǉ�
EXTERN int G_HEADS_DATA_Rev; // �v���t�@�C�����r�W����

// 2004.11.03�ǉ�
EXTERN bool bRetRev; // �����ɑΉ����邽�߂̃t���O

EXTERN AnsiString G_Log_File_Dir; // ۸�̧��     ̫���
EXTERN AnsiString G_Rsrv_Log_Dir; // �ߋ�۸�̧�� ̫���
EXTERN double G_Log_Max_Size; // ۸�̧�ٍő廲��(bytes)

EXTERN AnsiString G_ErrLog_File_Dir; // �װ۸�̧��     ̫���
EXTERN AnsiString G_Rsrv_ErrLog_Dir; // �ߋ��װ۸�̧�� ̫���
EXTERN double G_ErrLog_Max_Size; // �װ۸�̧�ٍő廲��(bytes)

EXTERN int G_Log_Write_Flg; // ۸ޏ��������׸�(0:����,1:�����Ȃ�)

// Windows 2008Server �ڐA�Ή�
EXTERN AnsiString G_Err_Log_FileName; // ���o�^�̌^���t�@�C����
EXTERN AnsiString G_Err_Log_FileFName; // ���o�^�̌^���t���p�X

/* ---*---*--- HD_PARTS.ini̧�ٗp�ϐ� ---*---*--- */
EXTERN AnsiString G_AnaakeFileNM; // �E��̧�ٖ���
EXTERN AnsiString G_YobihinFileNM; // �\���i̧�ٖ���

EXTERN AnsiString G_AnaFilePass; // �E��̧���߽(1�K�w��܂�)
// EXTERN   AnsiString G_YobiFilePass;       // �\����̧���߽

/* ---*---*--- ���p�g�p�ϐ� ---*---*--- */
EXTERN AnsiString G_LogFilePath; // ���߽ . ۸�̧��
EXTERN AnsiString G_ErrLogFilePath; // . �װ۸�̧��

EXTERN AnsiString G_Template_Dir; // ����ڰ�̧�يi�[��̫���
EXTERN AnsiString G_Sakuban_Dir; // ���̫���
EXTERN AnsiString G_xls_Output_Dir; // ���ټ�ďo�͐�̫���
EXTERN AnsiString G_csv_Output_Dir; // CSV̧�ُo�͐�̫���
EXTERN AnsiString G_Input_Csv_Dir; // HEADS����̫��� + CSV̫���
EXTERN AnsiString G_Input_Txt_Dir; // 02.08.07�ǉ�
// HEADS����̫��� + TXT̫���
// 2004/10/25 add
EXTERN AnsiString G_xls_Output_Dir_old; // ���ټ�ďo�͐�̫��ޑO��
// 2004/10/25 add

EXTERN AnsiString G_HEADS_ID; // ͯ���ް� ̧��ID
EXTERN AnsiString G_SAKUBAN; // ���
EXTERN AnsiString G_HEADS_DATA[1500]; // HEADS�f�[�^

// 2017.11.16 �K�i�R�[�h�ǉ�_S
//EXTERN AnsiString G_KIKAKU[30][5]; // �K�i����
EXTERN AnsiString G_KIKAKU[40][5]; // �K�i����
// 2017.11.16 �K�i�R�[�h�ǉ�_E
EXTERN long G_KIKAKU_RecCnt; // �K�i���� ����
EXTERN AnsiString G_GYOKAI[30][2]; // �ƊE
EXTERN long G_GYOKAI_RecCnt; // �ƊE ����
EXTERN AnsiString G_TOSOU[20][3]; // �h���F
EXTERN long G_TOSOU_RecCnt; // �ƊE ����
// 2015.01.16 �m�Y���F�ύX
EXTERN AnsiString G_BOOTCL[300][3]; // �u�[�c�F�
EXTERN long G_BOOTCL_RecCnt; // �u�[�c�F ����
// ***********************
// 2016.09.14 �X�e�b�J�[�ǉ�
EXTERN AnsiString G_STACKER[300][3]; // �X�e�b�J�[
EXTERN long G_STACKER_RecCnt; // �X�e�b�J�[ ����
// *************************

// 2019.07.17 ��ڰĕϊ�ð��ٌ����ύX_S
//// 2004.04.22 ��ڰĕϊ�ð��ٌ����ύX
//// EXTERN   AnsiString G_PHENKAN[100][9];    // ��ڰĕϊ�ð���
//EXTERN AnsiString G_PHENKAN[200][9]; // ��ڰĕϊ�ð���
EXTERN AnsiString G_PHENKAN[300][9]; // ��ڰĕϊ�ð���
// 2019.07.17 ��ڰĕϊ�ð��ٌ����ύX_E
EXTERN long G_PHENKAN_RecCnt; // ��ڰĕϊ�ð��ٌ���

EXTERN AnsiString G_NOZZLECD[100][6]; // ɽ�ٍ\�����i����ð���
EXTERN long G_NOZZLECD_RecCnt; // ɽ�ٍ\�����i����ð��ٌ���

EXTERN AnsiString G_TEMPCD[100][5]; // ���x�v�\�����i����ð���
EXTERN long G_TEMPCD_RecCnt; // ���x�v�\�����i����ð��ٌ���

EXTERN AnsiString G_PRESSCD[100][4]; // ���͌v�\�����i����ð���
EXTERN long G_PRESSCD_RecCnt; // ���͌v�\�����i����ð��ٌ���

EXTERN AnsiString G_K_Syurui; // �^�����
EXTERN AnsiString G_K_Model1; // �^������(Я���^��1)
EXTERN AnsiString G_K_Model2; // �^������(Я���^��2)
EXTERN AnsiString G_KS_Syurui; // ���̎��
EXTERN AnsiString G_KS_Model; // ��������
EXTERN AnsiString G_Model_Zen1; // �^�����ّS�p1
EXTERN AnsiString G_Model_Zen2; // �^�����ّS�p2

EXTERN AnsiString G_Log_Text; // ۸�÷��(�ėp)
EXTERN AnsiString G_ErrLog_Text; // �װ۸�÷��(�ėp)

// ��2002/10/23
EXTERN AnsiString G_D_GsktType;
EXTERN AnsiString G_D_GsktAna;
EXTERN AnsiString G_D_GsktABKubun;
EXTERN AnsiString G_E_GsktType;
EXTERN AnsiString G_E_GsktAna;
EXTERN AnsiString G_E_GsktABKubun;
EXTERN AnsiString G_S_Model;
EXTERN AnsiString G_FrmType;
EXTERN AnsiString G_GsktType;
EXTERN AnsiString G_FrmDiv;
// ��2002/10/23

// Excel�p�ϐ�
EXTERN Variant exApplication; // ���ع���ݵ�޼ު��
EXTERN Variant exWorkbooks; // ܰ��ޯ��ڸ���
EXTERN Variant exWorkbook; // ܰ��ޯ���޼ު��
EXTERN Variant exWorksheets; // ܰ���ĺڸ���
EXTERN Variant exWorksheet; // ܰ���ĵ�޼ު��
EXTERN Variant exWorkCell; // �ٗp
EXTERN Variant vRange; // �ݼ޵�޼ު�ėp

EXTERN Variant exWorkbook_old; // ܰ��ޯ���޼ު��  (����޼ޮݗp)
EXTERN Variant exWorksheets_old; // ܰ���ĺڸ���     (����޼ޮݗp)
EXTERN Variant exWorksheet_old; // ܰ���ĵ�޼ު��   (����޼ޮݗp)
EXTERN Variant exWorkCell_old; // �ٗp             (����޼ޮݗp)

EXTERN AnsiString G_YOBICHK_NAME[100]; // �\���i�d���v��h�~�p����
EXTERN AnsiString G_YOBICHK_MAT[100]; // �\���i�d���v��h�~�p����
EXTERN long G_YOBICHK_RecCnt; // �\���i�d���v��h�~�p�J�E���^

// ��2007/03
EXTERN bool bCopySeisekishoFlag; // ���я����R�s�[���邩�ۂ��̃t���O
// ��2007/03

// 2014/08/07 ���p�Z�p�ǉ�
EXTERN AnsiString G_Language;    // ���ꎯ�ʕ�����("JP" = ���{��, "EN" = �p��)

// 2019/05/17 ���p�Z�p�ǉ�
EXTERN bool bIsNotKatashiki; 	// ���o�^�^�����ǂ����̃t���O
EXTERN bool bMirrorFlag; 	    // �~���[�d�l���ǂ����̃t���O
EXTERN bool bHasOldDataFlag; 	// ���f�[�^�����邩�ǂ����̃t���O
EXTERN AnsiString gP_Sakuban_Mode;    // ��Ԏ��(S:�����ԍ��CM:���ϔԍ�)

#endif
// ---------------- <<< End of File >>> ---------------- //
