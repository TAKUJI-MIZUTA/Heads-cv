
//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F����w����  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.06.12
//�@ �X �V ���F2002.10.21
//             2007.10.10 �װ�d�l�Ή�
//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <io.h>

#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p
#include "DataModule.h"

#include "Bas_SijisyoA3.h"

#define  EXCEL_SHEETNAME    "����w����"          // EXCEL�V�[�g��(����w����)

#include <math.h>       //2004.08.16 �؂�グ

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F ����w�����@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall SijisyoA3(HeadsData* pcHeadsData)
{
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //TQuery *wkQuery;
   //wkQuery = new TQuery(DataModule1);
   TFDQuery *wkQuery;
   wkQuery = new TFDQuery(DataModule1);
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�
   AnsiString s_SQL;             // SQL��
   AnsiString s_Text;            // �ėp������
   // 2020.06.19 ���a���t���Ēǉ�_S
   AnsiString s_Text1;           // �ėp������P
   // 2020.06.19 ���a���t���Ēǉ�_E
   //2004.07.15 �ϐ��ǉ�
   AnsiString s_Text2;           // �ėp������Q
   //*******************
   //2019.10.10 �ϐ��ǉ�_S
   AnsiString s_Text3;           // �ėp������R
   AnsiString s_Text4;           // �ėp������S
   //2019.10.10 �ϐ��ǉ�_E
   // 2020.05.28 �����������L�ǉ�_S
   int        i_Length;          // ������
   // 2020.05.28 �����������L�ǉ�_E
   //2008.07.24 CX-10�ǉ�
   AnsiString P_Model;
   //********************
   AnsiString s_Bunbo;
   AnsiString s_Bunsi;
   AnsiString d_RevNo;           // ����w�����̂��钼�߂�RevNo

   AnsiString s_Side_B;          // ������ڰĐݒ�p �a��
   AnsiString s_Side_A;          //                 �`��
   AnsiString s_Syubetu;         // �ݶ����Ē��Q�Ɨp ����׸�

   AnsiString s_S1TankanNM;      // S1�[�ǖ���
   AnsiString s_S2TankanNM;      // S2�[�ǖ���
   AnsiString s_S3TankanNM;      // S3�[�ǖ���
   AnsiString s_S4TankanNM;      // S4�[�ǖ���

   //2009.07.31 �ϐ��ǉ�
   AnsiString s_S1TugiteNM;      // S1�p�薼��
   AnsiString s_S2TugiteNM;      // S2�p�薼��
   AnsiString s_S3TugiteNM;      // S3�p�薼��
   AnsiString s_S4TugiteNM;      // S4�p�薼��
   //*******************

   //2019.07.25 ����w���������ύX_S
   int P_FL; //�΍s���׸�(0:��`��,1:�΍s��)
   //2019.07.25 ����w���������ύX_E

//Add_Str 2019/02/22 Ri
   //*******************
   //�m�Y���I���G���e�[�V����
   //*******************
   AnsiString s_S1NO_JP;         // S1�t���a��
   AnsiString s_S2NO_JP;         // S2�t���a��
   AnsiString s_S3NO_JP;         // S3�t���a��
   AnsiString s_S4NO_JP;         // S4�t���a��

   AnsiString s_S1NO_EN;         // S1�t���p��
   AnsiString s_S2NO_EN;         // S2�t���p��
   AnsiString s_S3NO_EN;         // S3�t���p��
   AnsiString s_S4NO_EN;         // S4�t���p��
   //*******************
//Add_End 2019/02/22 Ri

   // 2012.01.20 �����ގ�
   AnsiString s_ChinaM;
   // *******************

   // 2020.06.19 ���a���t���Ēǉ�_S
   AnsiString sBltMat;           // ���t�{���g�ގ�
   // 2020.06.19 ���a���t���Ēǉ�_E

   // 2012.11.27 �G�|�L�V�h�����L
   int i_1365Flg;                // �ڰюd�㒍�L�׸�
   // ***************************

   int i_RecNo;                  // HEADS�ް� ں���No
   int i_ErrFlg;
   int i_Value_1;
   int i_Value_2;
   int i;
   int i_Siage;

   //2018.02.09 �W���t�����W�ގ��ǉ�_S
   int i_CODE;
   //2018.02.09 �W���t�����W�ގ��ǉ�_E

   //double d_wkValue;

   bool bRet;
   AnsiString s_HEADS_DATA[1500];
   AnsiString s_SpecCode;    // �d�l����("":���d�l/"A":�װA�d�l/"B":�װB�d�l)
   bool       bFind;
   bool       bFlag;

   // 2020.10.30 ���x�v�ی�ǏC��_S
   AnsiString s_S1OndoCode;      // S1���x�v���
   AnsiString s_S2OndoCode;      // S2���x�v���
   AnsiString s_S3OndoCode;      // S3���x�v���
   AnsiString s_S4OndoCode;      // S4���x�v���
   // 2020.10.30 ���x�v�ی�ǏC��_E

   //2020.11.24 �K�X�P�b�g���L�ǉ�_S
   bool bGas_A;           // A����Ķ޽��ėL��
   bool bGas_B;           // B����Ķ޽��ėL��
   double d_Koukei;       // ���a
   AnsiString s_GasMatA;  // A����Ķ޽��čގ�
   AnsiString s_GasMatB;  // B����Ķ޽��čގ�
   AnsiString s_GasName;  // �K�X�P�b�g���́@�@
   //2020.11.24 �K�X�P�b�g���L�ǉ�_E

   if(pcHeadsData->GetMirrorFlag()){
        // �װ�d�l�̏ꍇ�AA�d�l�AB�d�l�ȊO�͏����s�v(����I��)
        if(pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_NONE) return(true);
   }

   // AnsiString�^HEADS�ް����擾
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);

   AnsiString P_HEADS_DATA[1500];
   // HEADS÷���ް���AnsiString�z��Ɏ擾
   bRet = pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
   if (!bRet) return (false);

   // �d�l���ߕ�������擾(̧�ٖ��쐬�Ŏg�p)
   s_SpecCode = AnsiString(pcHeadsData->GetSpecCode());

   s_Tmplate_Pass = G_Template_Dir + "����w����A3.xls";                 // ����ڰ�̧��
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "MAN" + G_SAKUBAN + s_SpecCode + ".xls";
   s_MakeExcel_Pass = G_xls_Output_Dir + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
/*   s_MakeExcel_Pass = G_xls_Output_Dir + "MAN";
   s_MakeExcel_Pass = s_MakeExcel_Pass + G_SAKUBAN;
   s_MakeExcel_Pass = s_MakeExcel_Pass + s_SpecCode;
   s_MakeExcel_Pass = s_MakeExcel_Pass + ".xls";           // ��߰��̧��
*/
   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w����w�����x�쐬�J�n    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "�쐬�t�@�C�����F    �w" + s_MakeExcel_Pass + "�x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �e���v���[�g�@�R�s�[
   //******************************
   if (!Template_Copy(s_Tmplate_Pass,s_MakeExcel_Pass)){
      // �װ۸ޒǉ�
      G_ErrLog_Text = "�t�@�C���R�s�[�����s���܂����B�w" + s_Tmplate_Pass +
                      "�x���w" + s_MakeExcel_Pass + "�x";
      Write_Error_Log(G_ErrLog_Text);

      return false;
   } else {
      G_Log_Text = "�t�@�C���R�s�[�����B�w" + s_Tmplate_Pass +
                      "�x���w" + s_MakeExcel_Pass + "�x";
      Write_Log(G_Log_Text);
   }

   //******************************
   //***  Excel Book Open
   //******************************
   // ̧�ٵ����
   try {
      //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
      //Windows 2008Server �ڐA�Ή�
      exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );

   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ��ĺڸ��ݎ擾
   try {
      //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
      //Windows 2008Server �ڐA�Ή�
      exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");

   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //***************************************************************************
   //***                                                                     ***
   //***                       ����w�����@�f�[�^�Z�b�g                      ***
   //***                                                                     ***
   //***************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_SHEETNAME);
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)EXCEL_SHEETNAME);

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          �`���@�@�f�[�^�Z�b�g                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //************************
   //***  HEADS VER
   //************************
   // �wA001�x
   i_RecNo = 302;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "S";
   // 2017.10.06 �Z�[���Ή�_S
   } else if (s_HEADS_DATA[i_RecNo].TrimRight() == "2") {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "T";
   // 2017.10.06 �Z�[���Ή�_E
   } else {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   //s_Text = "HEADS VER." + s_Text;
   Excel_Cell_DataSet(1, 21, s_Text);
   G_Log_Text = "HEADS VER            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �}�ʒ���
   //***************************
   // �wA002�x
   i_RecNo = 319;
   s_Text = "Rev." + s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(3, 21, s_Text);
   G_Log_Text = "�}�ʒ���              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �쐬��
   //***************************
   // �wA003�x
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(4, 21, s_Text);
   G_Log_Text = "�쐬��                RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �Ǘ����ϰ�
   //***************************
   // �wA004�x
   i_RecNo = 417;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "0"){
      s_Text = "";
      Excel_Cell_DataSet(1, 27, "");
   } else {
      s_Text = "K";
      Excel_Cell_DataSet(1, 27, "K");
   }
   G_Log_Text = "�Ǘ����ϰ�          �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �c�ƒS����
   //***************************
   // �wA005�x
   i_RecNo = 3;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(4, 6, s_Text);
   G_Log_Text = "�c�ƒS����            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   // �wA006�x
   //-------------------------------
   // 2007.10.15 �d�l���ߕ�����ǉ�
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E   
   s_Text = s_Text + pcHeadsData->GetSpecCode();
   // 2007.10.15
   //-------------------------------

   Excel_Cell_DataSet(5, 5, s_Text);
   G_Log_Text = "�����ԍ�             �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ڰ��ް�ޮݒ��L  03.06.27�ǉ�
   //***************************
   // �wA007�x
   s_Text = s_HEADS_DATA[1380].TrimRight();
   Excel_Cell_DataSet(5, 10, s_Text);
   G_Log_Text = "�ڰ��ް�ޮݒ��L             �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  �^�@��
   //*********************
   // �wA008�x
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(6, 5, s_Text);
   G_Log_Text = "�^��                  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �[�@��
   //***************************
   // �wA009�x
   //i_RecNo = 12;
   i_RecNo = 411;
   s_Text = s_HEADS_DATA[i_RecNo];
   s_Text = s_Text.SubString(1,4) + "�N" +
            s_Text.SubString(6,2) + "��" +
            s_Text.SubString(9,2) + "��";
   Excel_Cell_DataSet(5, 23, s_Text);
   G_Log_Text = "�쐬��                RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //***  ��  ��
   //*******************
   // �wA010�x
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(6, 23, s_Text);
      G_Log_Text = "�䐔                  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�䐔      RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �� �� ��
   //***************************
   // �wA011�x
   i_RecNo = 1276;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(7, 5, s_Text);
   G_Log_Text = "������                RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �o�[�R�[�h
   //***************************
   // �wA012�x
   //2019.07.25 ����w���������ύX_S
   ////"'*" + A006��(1�`2������) + A006��(4�`7������) + "%MD" + A003�̕��� + "*"
   ////���쐬A012��A013(�o�[�R�[�h)�ɏo�͂��܂�
   //s_Text = "'*" + G_SAKUBAN.SubString(1,2) + G_SAKUBAN.SubString(3,4) + "%MD" + s_HEADS_DATA[1087] + "*";
   //Excel_Cell_DataSet(1, 19, s_Text);
   //// �wA013�x
   //Excel_Cell_DataSet(2, 25, s_Text);
   //2019.07.25 ����w���������ύX_E
   // 2023.05.30 �o�[�R�[�h�ύX_S
   s_Text = G_SAKUBAN;
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�o�[�R�[�h"));
   Excel_Cell_DataSet(4, 5, s_Text);
   G_Log_Text = "�o�[�R�[�h���         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("����w����"));
   // 2023.05.30 �o�[�R�[�h�ύX_E

   //****************************
   //***  �h���F
   //****************************
   // �wA014�x
   i_RecNo = 31;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) == 1){
      // RecNo:539 �h���F���
      i_RecNo = 32;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   } else if (s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 5) {
      // �h���F����ð��ٓ��a���� + RecNo:539 �h���F���
      //      s_Text = Search_TOSOU_CODE(s_Text, "J");

      i_RecNo = 32;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   } else {
      // �h���F����ð��ٓ��a���̂��
      s_Text = Search_TOSOU_CODE(s_Text, "J");
   }

   i_RecNo = 1365;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   if( s_Text2.Length() != 0 ) {
      i_1365Flg = 1;
   }

   i_RecNo = 31;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text2.ToIntDef(0) == 6 ) {  // �w���[���C��
      i_1365Flg = 0;
   } else if ( s_Text2.ToIntDef(0) == 7 ) {  // �r�[�Y
      i_1365Flg = 0;
   }
   // *******************************

   if ( i_1365Flg == 1 ) {                              // �ڰюd�㒍�L�L
      i_RecNo = 1365;                                   // ���L(�t���[���d��)
      s_Text = s_Text + "  " + s_HEADS_DATA[i_RecNo].TrimRight();
   }
   // ***************************

   //2019.12.27 ����w���������ύX_S
   s_Text = HanToZen(s_Text);
   //2019.12.27 ����w���������ύX_E

   Excel_Cell_DataSet(8, 6, s_Text);
   G_Log_Text = "�h���F               �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  MIX�L���P�C�Q �擾
   //***************************
   AnsiString s_MIX1,s_MIX2;
   // MIX�L���P
   if (s_HEADS_DATA[1062].TrimRight() == "0"){
      s_MIX1 = "";
   } else if (s_HEADS_DATA[1062].TrimRight() == "1"){
      s_MIX1 = "H";
   } else if (s_HEADS_DATA[1062].TrimRight() == "2"){
      s_MIX1 = "M";
   } else if (s_HEADS_DATA[1062].TrimRight() == "3"){
      s_MIX1 = "M";
   } else {
      s_MIX1 = "";
   }

   // MIX�L���Q
   if (s_HEADS_DATA[1062].TrimRight() == "0"){
      s_MIX2 = "";
   } else if (s_HEADS_DATA[1062].TrimRight() == "1"){
      s_MIX2 = "M";
   } else if (s_HEADS_DATA[1062].TrimRight() == "2"){
      s_MIX2 = "M";
   } else if (s_HEADS_DATA[1062].TrimRight() == "3"){
      s_MIX2 = "L";
   } else {
      s_MIX2 = "";
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          �a���@�@�f�[�^�Z�b�g                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //**************************
   //***  �X�^�[�g�v���[�g
   //**************************
   Get_Start_PlateA3(s_Side_B, s_Side_A, pcHeadsData);
   // �wB001�x
   Excel_Cell_DataSet(9, 6, s_Side_B);
   // �wB002�x
   Excel_Cell_DataSet(9, 14, s_Side_A);

   //***************************
   //***  B����ڰĕҐ� 1
   //***  B����ڰĕҐ� 2
   //***************************
   // �wB003�x B����ڰĕҐ� 1
   s_Text = s_HEADS_DATA[837].TrimRight() + "�~" + s_HEADS_DATA[838].TrimRight();
   s_Text = s_Text + s_MIX1;
   G_Log_Text = "B����ڰĕҐ� 1       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // �wB003�x B����ڰĕҐ� 2
   i_Value_1 = s_HEADS_DATA[839].ToIntDef(0);   // B�Ґ�21
   i_Value_2 = s_HEADS_DATA[840].ToIntDef(0);   // B�Ґ�22

   if (i_Value_1 * i_Value_2 == 0){
      s_Text2 = "";
   } else {
      s_Text2 = AnsiString(i_Value_1) + "�~" + AnsiString(i_Value_2) + s_MIX2;
      s_Text2 = " �^ " + s_Text2;
   }
   G_Log_Text = "B����ڰĕҐ� 2       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   Excel_Cell_DataSet(10, 7, s_Text + s_Text2);

   //***************************
   //***  A����ڰĕҐ� 1
   //***  A����ڰĕҐ� 2
   //***************************
   // �wB004�x A����ڰĕҐ� 1
   s_Text = s_HEADS_DATA[841].TrimRight() + "�~" + s_HEADS_DATA[842].TrimRight();
   s_Text = s_Text + s_MIX1;
   G_Log_Text = "A����ڰĕҐ� 1       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // �wB004�x A����ڰĕҐ� 2
   i_Value_1 = s_HEADS_DATA[843].ToIntDef(0);   // B�Ґ�21
   i_Value_2 = s_HEADS_DATA[844].ToIntDef(0);   // B�Ґ�22

   if (i_Value_1 * i_Value_2 == 0){
      s_Text2 = "";
   } else {
      s_Text2 = AnsiString(i_Value_1) + "�~" + AnsiString(i_Value_2) + s_MIX2;
      s_Text2 = " �^ " + s_Text2;
   }
   G_Log_Text = "A����ڰĕҐ� 2       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   Excel_Cell_DataSet(10, 15, s_Text + s_Text2);

   //***************************
   //***  ��ڰčގ�
   //***************************
   // �wB005�x
   i_RecNo = 43;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "��ڰĂ̍ޗ�           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���";
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   //2009.04.27 �ټ�Ē��L
   i_RecNo = 433;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      s_Text = s_Text + " ��";
   }
   //********************

   Excel_Cell_DataSet(11, 7, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2019.07.25 ����w���������ύX_S
   //***************************
   //***  �΍s���t���O
   //***************************
   // 2022.04.21 CX-01D�ǉ�_S
    if (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 1) {
       //CX-01D
       P_FL = 1; // �΍s��
    }
    //if (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
    else if (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
       // 2022.04.21 CX-01D�ǉ�_S
       //CX-10D
       P_FL = 1; // �΍s��
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 21 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 90) {
       //LX-90D
       P_FL = 1; // �΍s��
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 111 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 80) {
       //YX-80
       P_FL = 1; // �΍s��
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 111 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 83) {
       //YX-83
       P_FL = 1; // �΍s��
    }
    else {
       P_FL = 0; // ��`��
    }
   //2019.07.25 ����w���������ύX_E

   //***************************
   //***  B���޽��čގ�
   //***************************
   // �wB006�x
   i_RecNo = 835;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "B���޽��čގ�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���";

   s_Text = Get_Zaisitu_NameA3(s_Text);

   //2018.02.09 ���_�p�r���L�ǉ�_S
   i_RecNo = 43;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text2 = Type_MAT(s_Text2);
   if( s_Text2 == "HAS" ) {              // �v���[�g�ގ��n�X�e���C
      i_RecNo = 835;
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text2 = Type_MAT(s_Text2);
      if( s_Text2 == "FPM" ) {           // �a���K�X�P�b�g�ގ��o�C�g��
         s_Text = s_Text + "�^���ʃe�[�v";
      }
   }
   //2018.02.09 ���_�p�r���L�ǉ�_E

   Excel_Cell_DataSet(12, 7, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���޽��čގ�
   //***************************
   // �wB007�x
   i_RecNo = 836;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "A���޽��čގ�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���";

   s_Text = Get_Zaisitu_NameA3(s_Text);

   //2018.02.09 ���_�p�r���L�ǉ�_S
   i_RecNo = 43;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text2 = Type_MAT(s_Text2);
   if( s_Text2 == "HAS" ) {              // �v���[�g�ގ��n�X�e���C
      i_RecNo = 836;
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text2 = Type_MAT(s_Text2);
      if( s_Text2 == "FPM" ) {           // �`���K�X�P�b�g�ގ��o�C�g��
         s_Text = s_Text + "�^���ʃe�[�v";
      }
   }
   //2018.02.09 ���_�p�r���L�ǉ�_E

   Excel_Cell_DataSet(12, 15, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B��ɽ�ٍގ�
   //***************************
   // �wB008�x
   i_RecNo = 894;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // 2013.04.03 SUS316L�������ݼ�
   if ( s_HEADS_DATA[37].ToIntDef(0) > 300 && s_Text == "1003" ) {
       s_Text = "1004";
   }
   // ****************************

   G_Log_Text = "B��ɽ�ٍގ�           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���";

   s_Text = Get_Zaisitu_NameA3(s_Text);

   //2009.04.27 �ټ�Ē��L
   if (s_HEADS_DATA[436].TrimRight() == "0" && s_HEADS_DATA[437].TrimRight() == "0"){
   } else {
       // 2019.12.23 �ټ�ďC��_S
       //i_RecNo = 894;
       //i_Value_1 = s_HEADS_DATA[i_RecNo].ToIntDef(0);
       //if ( i_Value_1 > 1000 && i_Value_1 < 2000 ){
       //    s_Text = s_Text + " ��";
       //}
       s_Text = s_Text + " ��";
       // 2019.12.23 �ټ�ďC��_E
   }
   //2019.07.25 ����w���������ύX_S
   ////********************
   ////2010.06.07 CX-10D�ǉ�
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10D" ) {
   //    s_Text = s_Text + " (S1,S4)";
   //}
   ////*********************
   if ( P_FL == 1 ) {
       // �΍s��
       s_Text = s_Text + " (S1,S4)";
   }
   //2019.07.25 ����w���������ύX_E

   Excel_Cell_DataSet(13, 7, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B��ɽ�ِF
   //***************************
   // �wB009�x
   i_RecNo = 894;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "B��ɽ�ٍގ�           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���A";
   // 2015.01.16 �m�Y���F�ύX
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   // ***********************
   //if (s_Text == "2019"){
   //   s_Text = "��";
   //} else if ( s_Text == "2026"){
   //   s_Text = "��";
   //} else if ( s_Text == "2030"){
   //   s_Text = "��";
   //} else {
   //   s_Text = "";
   //}
   s_Text2 = "";
   for (i = 0; i < G_BOOTCL_RecCnt; i++) {
      if (G_BOOTCL[i][0] == s_Text) {
         if (G_BOOTCL[i][1] == P_Model) {
            s_Text2 = G_BOOTCL[i][2];
         }
      }
   }
   s_Text = s_Text2;
// ***********************

   Excel_Cell_DataSet(13, 12, s_Text);
   G_Log_Text = G_Log_Text + "B��ɽ�ِF �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A��ɽ�ٍގ�
   //***************************
   // �wB010�x
   i_RecNo = 909;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // 2013.04.03 SUS316L�������ݼ�
   if ( s_HEADS_DATA[37].ToIntDef(0) > 300 && s_Text == "1003" ) {
       s_Text = "1004";
   }
   // ****************************

   G_Log_Text = "A��ɽ�ٍގ�           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���";

   s_Text = Get_Zaisitu_NameA3(s_Text);

   //2009.04.27 �ټ�Ē��L
   if (s_HEADS_DATA[436].TrimRight() == "0" && s_HEADS_DATA[437].TrimRight() == "0"){
   } else {
       // 2019.12.23 �ټ�ďC��_S
       //i_RecNo = 909;
       //i_Value_1 = s_HEADS_DATA[i_RecNo].ToIntDef(0);
       //if ( i_Value_1 > 1000 && i_Value_1 < 2000 ){
       //    s_Text = s_Text + " ��";
       //}
       s_Text = s_Text + " ��";
       // 2019.12.23 �ټ�ďC��_E
   }
   //********************

   //2019.07.25 ����w���������ύX_S
   ////2010.06.07 CX-10D�ǉ�
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10D" ) {
   //    s_Text = s_Text + " (S2,S3)";
   //}
   ////*********************
   if ( P_FL == 1 ) {
       // �΍s��
       s_Text = s_Text + " (S2,S3)";
   }
   //2019.07.25 ����w���������ύX_E

   Excel_Cell_DataSet(13, 15, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A��ɽ�ِF
   //***************************
   // �wB011�x
   i_RecNo = 909;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "A��ɽ�ٍގ�           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���A";
   // 2015.01.16 �m�Y���F�ύX
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   // ***********************
   //if (s_Text == "2019"){
   //   s_Text = "��";
   //} else if ( s_Text == "2026"){
   //   s_Text = "��";
   //} else if ( s_Text == "2030"){
   //   s_Text = "��";
   //} else {
   //   s_Text = "";
   //}
   s_Text2 = "";
   for (i = 0; i < G_BOOTCL_RecCnt; i++) {
      if (G_BOOTCL[i][0] == s_Text) {
         if (G_BOOTCL[i][1] == P_Model) {
            s_Text2 = G_BOOTCL[i][2];
         }
      }
   }
   s_Text = s_Text2;
// ***********************

   Excel_Cell_DataSet(13, 25, s_Text);
   G_Log_Text = G_Log_Text + "A��ɽ�ِF �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  S1�p��E���a
   //***************************
   // �wB012�x
   //2009.07.31 ASMEɽ�ٖ��̕ύX
   //s_S1TankanNM = "";   // ���x�v���̍ގ��Ɏg��
   //
   //i_RecNo = 1136;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   ////2008.07.24 CX-10�ǉ�
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4)); //B���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //       s_Text = "�����O�[�ǂP�@" + s_HEADS_DATA[1152].TrimRight();
   //   } else  {
   //       s_Text = "�����O�[�ǂQ�@" + s_HEADS_DATA[1152].TrimRight();
   //   }
   //
   //} else if (s_Text == "1" || s_Text == "3"){
   ////if (s_Text == "1" || s_Text == "3"){
   ////*******************
   //   s_S1TankanNM = Get_Tankan_Data("S1", pcHeadsData);  // S1�[�ǎ��
   //   s_Text = s_S1TankanNM + "�@" + s_HEADS_DATA[1184].TrimRight();
   //
   //} else if (s_Text == "2"){
   //   s_S1TankanNM = Get_Tankan_Data("S1", pcHeadsData);  // S1�[�ǎ��
   //   s_Text = s_S1TankanNM + "�@" + s_HEADS_DATA[1184].TrimRight() + "�v�e";
   //
   //} else {
   //   if (s_HEADS_DATA[893].SubString(2,1) == "4") {  // �t�����W�t���m�Y��
   //      s_Text = s_HEADS_DATA[1152].TrimRight() + "�@" + Get_Flange_Data(s_HEADS_DATA[893].TrimRight());
   //      s_S1TankanNM = s_HEADS_DATA[894].TrimRight();  // �m�Y���ގ����g��
   //   } else {
   //      s_Text = s_HEADS_DATA[1152].TrimRight();
   //      s_S1TankanNM = s_HEADS_DATA[43].TrimRight(); // �[�ǂ��t���Ȃ��u�[�c�m�Y���̏ꍇ�A�v���[�g�ގ����g��
   //   }
   //}

   s_S1TankanNM = "";  // S1�[�����
   s_S1TugiteNM = "";  // S1�p�薼��

   i_RecNo = 1136;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   if ( s_Text == "1" || s_Text == "2" || s_Text == "3" ) {
      //2019.07.25 ����w���������ύX_S
      //s_S1TugiteNM = s_HEADS_DATA[1184].TrimRight();  // S1�[���p�薼��
      s_S1TugiteNM = s_HEADS_DATA[1152].TrimRight();  // S1ɽ�ٌp�薼��
      //2019.07.25 ����w���������ύX_E
   } else {
      s_S1TugiteNM = s_HEADS_DATA[1152].TrimRight();  // S1ɽ�ٌp�薼��
   }
   if ( s_S1TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�P�T�O�|�m�o�r" ) {
      s_S1TugiteNM = "�`�r�l�d �b�k�r�P�T�O�|" + s_S1TugiteNM.Delete(1,34);
   } else if ( s_S1TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�R�O�O�|�m�o�r" ) {
      s_S1TugiteNM = "�`�r�l�d �b�k�r�R�O�O�|" + s_S1TugiteNM.Delete(1,34);
   }
   // 2013.04.03 ɽ�ًK�i�ǉ�
   if ( s_S1TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�S�O�O�|�m�o�r" ) {
      s_S1TugiteNM = "�`�r�l�d �b�k�r�S�O�O�|" + s_S1TugiteNM.Delete(1,34);
   }
   // ***********************

   //2019.07.25 ����w���������ύX_S
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if ( P_Model == "CX-10" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //      s_Text = "�����O�[�ǂP " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "�����O�[�ǂQ " + s_S1TugiteNM;
   //   }
   //} else if ( P_Model == "CX-10D" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //      s_Text = "�����O�[�ǂP " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "�����O�[�ǂQ " + s_S1TugiteNM;
   //   }
   //// 2016.04.22 CX-00�ǉ�
   //} else if ( P_Model == "CX-00" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //      s_Text = "�����O�[�ǂP " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "�����O�[�ǂQ " + s_S1TugiteNM;
   //   }
   //} else if ( P_Model == "CXW-00" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //      s_Text = "�����O�[�ǂP " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "�����O�[�ǂQ " + s_S1TugiteNM;
   //   }
   //// ********************
   //// 2013.04.03 SX-30�ǉ�
   //} else if ( P_Model == "SX-30" && s_S1TugiteNM.SubString(1,12) == "�i�h�r�R�O�j" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //      s_Text = "�����O�[�� " + s_S1TugiteNM;
   //// ********************
   //// 2016.07.22 SX-30S�ǉ�
   //} else if ( P_Model == "SX-30S" && s_S1TugiteNM.SubString(1,12) == "�i�h�r�R�O�j" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //      s_Text = "�����O�[�� " + s_S1TugiteNM;
   //// *********************
   //} else if ( s_Text == "1" || s_Text == "3" ) {
   //   s_S1TankanNM = Get_Tankan_DataA3("S1", pcHeadsData);  // S1�[�ǎ��
   //   s_Text = s_S1TankanNM + " " + s_S1TugiteNM;
   //
   //} else if ( s_Text == "2" ) {
   //   s_S1TankanNM = Get_Tankan_DataA3("S1", pcHeadsData);  // S1�[�ǎ��
   //   s_Text = s_S1TankanNM + " " + s_S1TugiteNM + "�v�e";
   //
   //} else {
   //   // 2011.06.20 ɽ�ًK�i�ύX
   //   //if (s_HEADS_DATA[893].SubString(2,1) == "4") {  // �t�����W�t���m�Y��
   //   if (s_HEADS_DATA[893].SubString(3,1) == "4") {  // �t�����W�t���m�Y��
   //   // ***********************
   //      s_Text = s_S1TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[893].TrimRight());
   //   } else {
   //      s_Text = s_S1TugiteNM;
   //   }
   //
   //}
   ////***************************
   if ( s_Text == "1" || s_Text == "3" ) {
      s_S1TankanNM = Get_Tankan_DataA3("S1", pcHeadsData);  // S1�[�ǎ��
      s_Text = s_S1TugiteNM + " " + s_S1TankanNM;

   } else if ( s_Text == "2" ) {
      s_S1TankanNM = Get_Tankan_DataA3("S1", pcHeadsData);  // S1�[�ǎ��
      s_Text = s_S1TugiteNM + " " + s_S1TankanNM + " �v�e";

   } else {
      if (s_HEADS_DATA[893].SubString(3,1) == "4") {  // �t�����W�t���m�Y��
         s_Text = s_S1TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[893].TrimRight());
      } else {
         s_Text = s_S1TugiteNM;
      }

   }
   //2019.07.25 ����w���������ύX_E

   //2014.05.28 �p��K�i�\�L�ύX
   if ( s_S1TugiteNM.SubString(1,6) == "�i�h�r" ) {
   } else {
       s_Text = "��" + s_Text;
   }
   //***************************

   Excel_Cell_DataSet(14, 8, s_Text);
   G_Log_Text = "S1�p��E���a         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  S2�p��E���a
   //***************************
   // �wB013�x
   //2009.07.31 ASMEɽ�ٖ��̕ύX
   //s_S2TankanNM = "";   // ���x�v���̍ގ��Ɏg��
   //
   //i_RecNo = 1137;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text == "1" || s_Text == "3"){
   //   s_S2TankanNM = Get_Tankan_Data("S2", pcHeadsData);
   //   s_Text = s_S2TankanNM + "�@" + s_HEADS_DATA[1188].TrimRight();
   //
   //} else if (s_Text == "2"){
   //   s_S2TankanNM = Get_Tankan_Data("S2", pcHeadsData);
   //   s_Text = s_S2TankanNM + "�@" + s_HEADS_DATA[1188].TrimRight() + "�v�e";
   //
   //} else {
   //   if (s_HEADS_DATA[908].SubString(2,1) == "4") {    // �t�����W�t���m�Y��
   //      s_Text = s_HEADS_DATA[1156].TrimRight() + "�@" + Get_Flange_Data(s_HEADS_DATA[908].TrimRight());
   //      s_S1TankanNM = s_HEADS_DATA[909].TrimRight(); // �m�Y���ގ����g��
   //   } else {
   //      s_Text = s_HEADS_DATA[1156].TrimRight();
   //      s_S1TankanNM = s_HEADS_DATA[43].TrimRight(); // �[�ǂ��t���Ȃ��u�[�c�m�Y���̏ꍇ�A�v���[�g�ގ����g��
   //   }
   //}

   s_S2TankanNM = "";  // S2�[�����
   s_S2TugiteNM = "";  // S2�p�薼��

   i_RecNo = 1137;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   if ( s_Text == "1" || s_Text == "2" || s_Text == "3" ){
      //2019.07.25 ����w���������ύX_S
      //s_S2TugiteNM = s_HEADS_DATA[1188].TrimRight();  // S2�[���p�薼��
      s_S2TugiteNM = s_HEADS_DATA[1156].TrimRight();  // S2ɽ�ٌp�薼��
      //2019.07.25 ����w���������ύX_E
   } else {
      s_S2TugiteNM = s_HEADS_DATA[1156].TrimRight();  // S2ɽ�ٌp�薼��
   }
   if ( s_S2TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�P�T�O�|�m�o�r" ) {
      s_S2TugiteNM = "�`�r�l�d �b�k�r�P�T�O�|" + s_S2TugiteNM.Delete(1,34);
   } else if ( s_S2TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�R�O�O�|�m�o�r" ) {
      s_S2TugiteNM = "�`�r�l�d �b�k�r�R�O�O�|" + s_S2TugiteNM.Delete(1,34);
   }
   // 2013.04.03 ɽ�ًK�i�ǉ�
   if ( s_S2TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�S�O�O�|�m�o�r" ) {
      s_S2TugiteNM = "�`�r�l�d �b�k�r�S�O�O�|" + s_S2TugiteNM.Delete(1,34);
   }
   // ***********************

   //2019.07.25 ����w���������ύX_S
   //if ( s_Text == "1" || s_Text == "3" ) {
   //   s_S2TankanNM = Get_Tankan_DataA3("S2", pcHeadsData);  // S2�[��
   //   s_Text = s_S2TankanNM + " " + s_S2TugiteNM;
   //
   //} else if ( s_Text == "2" ) {
   //   s_S2TankanNM = Get_Tankan_DataA3("S2", pcHeadsData);  // S2�[��
   //   s_Text = s_S2TankanNM + " " + s_S2TugiteNM + "�v�e";
   //
   //} else {
   //   // 2011.06.20 ɽ�ًK�i�ύX
   //   //if ( s_HEADS_DATA[908].SubString(2,1) == "4" ) {  // �t�����W�t���m�Y��
   //   if ( s_HEADS_DATA[908].SubString(3,1) == "4" ) {  // �t�����W�t���m�Y��
   //   // ***********************
   //      s_Text = s_S2TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[908].TrimRight());
   //   } else {
   //      s_Text = s_S2TugiteNM;
   //   }
   //
   //}
   if ( s_Text == "1" || s_Text == "3" ) {
      s_S2TankanNM = Get_Tankan_DataA3("S2", pcHeadsData);  // S2�[��
      s_Text = s_S2TugiteNM + " " + s_S2TankanNM;

   } else if ( s_Text == "2" ) {
      s_S2TankanNM = Get_Tankan_DataA3("S2", pcHeadsData);  // S2�[��
      s_Text = s_S2TugiteNM + " " + s_S2TankanNM + " �v�e";

   } else {
      if ( s_HEADS_DATA[908].SubString(3,1) == "4" ) {  // �t�����W�t���m�Y��
         s_Text = s_S2TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[908].TrimRight());
      } else {
         s_Text = s_S2TugiteNM;
      }

   }
   //2019.07.25 ����w���������ύX_E

   //2014.05.28 �p��K�i�\�L�ύX
   if ( s_S2TugiteNM.SubString(1,6) == "�i�h�r" ) {
   } else {
       s_Text = "��" + s_Text;
   }
   //***************************

   Excel_Cell_DataSet(14, 16, s_Text);
   G_Log_Text = "S2�p��E���a         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  S1���x�v
   //***  B��S1���ы敪
   //***************************
   // �wB014�x,�wB015�x
   i_RecNo = 898;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S1���x�v���

   //2019.07.25 ����w���������ύX_S
   //if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 11) {
      if ( s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2 ||
           s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 4 ||
           s_Text.ToIntDef(0) == 5 || s_Text.ToIntDef(0) == 8 ||
           s_Text.ToIntDef(0) == 9 || s_Text.ToIntDef(0) == 10  ) {
         s_Text2 = " I�^";
      } else if ( s_Text.ToIntDef(0) == 6 || s_Text.ToIntDef(0) == 11 ) {
         s_Text2 = " L�^";
      } else {
         s_Text2 = "";
      }
   //2019.07.25 ����w���������ύX_E
      // �wB014�x
      s_Text = AnsiString(s_HEADS_DATA[899].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[900].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1357].TrimRight()));

      //2019.07.25 ����w���������ύX_S
      s_Text = s_Text + s_Text2;
      //2019.07.25 ����w���������ύX_E

      Excel_Cell_DataSet(15, 7, s_Text);
      G_Log_Text = "S1���x�v             �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB015�x
      if (s_HEADS_DATA[846].TrimRight() == "1"){
         s_Text = "���їp";
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(15, 11, s_Text);
      //2019.07.25 ����w���������ύX_S
      //G_Log_Text = "B��S1���x�v���ގ�    �w" + s_Text + "�x���Z�b�g�B";
      G_Log_Text = "S1���ы敪           �w" + s_Text + "�x���Z�b�g�B";
      //2019.07.25 ����w���������ύX_E
      Write_Log(G_Log_Text);

   } else {
      // �wB014�x
      s_Text = "";
      Excel_Cell_DataSet(15, 7, s_Text);
      G_Log_Text = "S1���x�v             �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB015�x
      s_Text = "";
      Excel_Cell_DataSet(15, 11, s_Text);
      //2019.07.25 ����w���������ύX_S
      //G_Log_Text = "B��S1���x�v���ގ�    �w" + s_Text + "�x���Z�b�g�B";
      G_Log_Text = "S1���ы敪           �w" + s_Text + "�x���Z�b�g�B";
      //2019.07.25 ����w���������ύX_E

      Write_Log(G_Log_Text);
   }

   //***************************
   //***  S2���x�v
   //***  A��S2���ы敪
   //***************************
   // �wB016�x,�wB017�x
   i_RecNo = 913;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S2���x�v���

   //2019.07.25 ����w���������ύX_S
   //if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 11) {
      if ( s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2 ||
           s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 4 ||
           s_Text.ToIntDef(0) == 5 || s_Text.ToIntDef(0) == 8 ||
           s_Text.ToIntDef(0) == 9 || s_Text.ToIntDef(0) == 10  ) {
         s_Text2 = " I�^";
      } else if ( s_Text.ToIntDef(0) == 6 || s_Text.ToIntDef(0) == 11 ) {
         s_Text2 = " L�^";
      } else {
         s_Text2 = "";
      }
   //2019.07.25 ����w���������ύX_E
      // �wB016�x
      s_Text = AnsiString(s_HEADS_DATA[914].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[915].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1358].TrimRight()));

      //2019.07.25 ����w���������ύX_S
      s_Text = s_Text + s_Text2;
      //2019.07.25 ����w���������ύX_E

      Excel_Cell_DataSet(15, 15, s_Text);
      G_Log_Text = "S2���x�v             �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB017�x
      if (s_HEADS_DATA[847].TrimRight() == "1"){
         s_Text = "���їp";
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(15, 24, s_Text);
      //2019.07.25 ����w���������ύX_S
      //G_Log_Text = "A��S2���x�v���ގ�    �w" + s_Text + "�x���Z�b�g�B";
      G_Log_Text = "S2���ы敪           �w" + s_Text + "�x���Z�b�g�B";
      //2019.07.25 ����w���������ύX_E
      Write_Log(G_Log_Text);

   } else {
      // �wB016�x
      s_Text = "";
      Excel_Cell_DataSet(15, 15, s_Text);
      G_Log_Text = "S2���x�v             �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB017�x
      s_Text = "";
      Excel_Cell_DataSet(15, 24, s_Text);
      //2019.07.25 ����w���������ύX_S
      //G_Log_Text = "A��S2���x�v���ގ�    �w" + s_Text + "�x���Z�b�g�B";
      G_Log_Text = "S2���ы敪           �w" + s_Text + "�x���Z�b�g�B";
      //2019.07.25 ����w���������ύX_E
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  S1���͌v
   //***************************
   // �wB018�x
   s_Text = s_HEADS_DATA[901].TrimRight();   // S1���͌v���

   // 2020.07.17 ���͌v�ύX_S
   ////2019.07.25 ����w���������ύX_S
   //////2004.05.19 ���͌v�����ǉ�
   //////if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   ////if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //////*************************
   //if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 20) {
   //2019.07.25 ����w���������ύX_E
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 26) {
   // 2020.07.17 ���͌v�ύX_E
      // S1���͌v�ڐ� 1
      i_RecNo = 902;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S1���͌v�ڐ� 2
         i_RecNo = 903;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S1���͌v
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[901].TrimRight()) + " ";

            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S1���͌v�ڐ�2   RecNo:" + AnsiString(i_RecNo)
                            + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

            s_Text = "";
         }

      //2018.04.12 �ڐ��ǉ�_S
      } else if (s_Text == "-0.10") {
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S1���͌v�ڐ� 2
         i_RecNo = 903;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S1���͌v
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[901].TrimRight()) + " ";

            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S1���͌v�ڐ�2   RecNo:" + AnsiString(i_RecNo)
                            + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

            s_Text = "";
         }
      //2018.04.12 �ڐ��ǉ�_E

      } else {
         G_ErrLog_Text = "S1���͌v�ڐ�1   RecNo:" + AnsiString(i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = "";
      }

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(16, 7, s_Text);
   G_Log_Text = "S1���͌v             �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  S2���͌v
   //***************************
   // �wB019�x
   s_Text = s_HEADS_DATA[916].TrimRight();   // S2���͌v���

   // 2020.07.17 ���͌v�ύX_S
   ////2019.07.25 ����w���������ύX_S
   //////2004.05.19 ���͌v�����ǉ�
   //////if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   ////if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //////*************************
   //if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 20) {
   ////2019.07.25 ����w���������ύX_E
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 26) {
   // 2020.07.17 ���͌v�ύX_E
      // S2���͌v�ڐ� 1
      i_RecNo = 917;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S2���͌v�ڐ� 2
         i_RecNo = 918;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S2���͌v
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[916].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S2���͌v�ڐ�2   RecNo:" + AnsiString(i_RecNo)
                            + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

            s_Text = "";
         }


      //2018.04.12 �ڐ��ǉ�_S
      } else if (s_Text == "-0.10") {
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S2���͌v�ڐ� 2
         i_RecNo = 918;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S2���͌v
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[916].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S2���͌v�ڐ�2   RecNo:" + AnsiString(i_RecNo)
                            + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

            s_Text = "";
         }

      //2018.04.12 �ڐ��ǉ�_E

      } else {
         G_ErrLog_Text = "S2���͌v�ڐ�1   RecNo:" + AnsiString(i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = "";
      }

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(16, 15, s_Text);
   G_Log_Text = "S2���͌v             �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  S1�����p�薼�a
   //***************************
   // �wB020�x
   s_Text = ZenToHan(s_HEADS_DATA[1216].TrimRight()) + ZenToHan(s_HEADS_DATA[1217].TrimRight());
   Excel_Cell_DataSet(17, 7, s_Text);
   G_Log_Text = "S1�����p�薼�a       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  S2�����p�薼�a
   //***************************
   // �wB021�x
   s_Text = ZenToHan(s_HEADS_DATA[1220].TrimRight()) + ZenToHan(s_HEADS_DATA[1221].TrimRight());
   Excel_Cell_DataSet(17, 15, s_Text);
   G_Log_Text = "S2�����p�薼�a       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  S3�p��E���a
   //***************************
   // �wB022�x
   //2009.07.31 ASMEɽ�ٖ��̕ύX
   //s_S3TankanNM = "";   // S3�[�ǎ�ށwB024�x,�wB025�x�Ŏg�p
   //
   //i_RecNo = 1138;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text == "1" || s_Text == "3"){
   //   s_S3TankanNM = Get_Tankan_Data("S3", pcHeadsData);
   //   s_Text = s_S3TankanNM + "�@" + s_HEADS_DATA[1192].TrimRight();
   //
   //} else if (s_Text == "2"){
   //   s_S3TankanNM = Get_Tankan_Data("S3", pcHeadsData);
   //   s_Text = s_S3TankanNM + "�@" + s_HEADS_DATA[1192].TrimRight() + "�v�e";
   //
   //} else {
   //   if (s_HEADS_DATA[923].SubString(2,1) == "4") {
   //      s_Text = s_HEADS_DATA[1160].TrimRight() + "�@" + Get_Flange_Data(s_HEADS_DATA[923].TrimRight());
   //   } else {
   //      s_Text = s_HEADS_DATA[1160].TrimRight();
   //   }
   //}
   //
   //Excel_Cell_DataSet(18, 8, s_Text);

   s_S3TankanNM = "";  // S3�[�����
   s_S3TugiteNM = "";  // S3�p�薼��

   i_RecNo = 1138;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   if ( s_Text == "1" || s_Text == "2" || s_Text == "3" ) {
      //2019.07.25 ����w���������ύX_S
      //s_S3TugiteNM = s_HEADS_DATA[1192].TrimRight();  // S3�[���p�薼��
      s_S3TugiteNM = s_HEADS_DATA[1160].TrimRight();  // S3ɽ�ٌp�薼��
      //2019.07.25 ����w���������ύX_E
   } else {
      s_S3TugiteNM = s_HEADS_DATA[1160].TrimRight();  // S3ɽ�ٌp�薼��
   }
   if ( s_S3TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�P�T�O�|�m�o�r" ) {
      s_S3TugiteNM = "�`�r�l�d �b�k�r�P�T�O�|" + s_S3TugiteNM.Delete(1,34);
   } else if ( s_S3TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�R�O�O�|�m�o�r" ) {
      s_S3TugiteNM = "�`�r�l�d �b�k�r�R�O�O�|" + s_S3TugiteNM.Delete(1,34);
   }
   // 2013.04.03 ɽ�ًK�i�ǉ�
   if ( s_S3TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�S�O�O�|�m�o�r" ) {
      s_S3TugiteNM = "�`�r�l�d �b�k�r�S�O�O�|" + s_S3TugiteNM.Delete(1,34);
   }
   // ***********************

   //2019.07.25 ����w���������ύX_S
   //// 2016.04.22 CX-00�ǉ�
   //if ( P_Model == "CX-00" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //      s_Text = "�����O�[�ǂP " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "�����O�[�ǂQ " + s_S1TugiteNM;
   //   }
   //} else if ( P_Model == "CXW-00" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //      s_Text = "�����O�[�ǂP " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "�����O�[�ǂQ " + s_S1TugiteNM;
   //   }
   //// 2013.04.03 SX-30�ǉ�
   ////if ( P_Model == "SX-30" && s_S3TugiteNM.SubString(1,12) == "�i�h�r�R�O�j" ) {
   //} else if ( P_Model == "SX-30" && s_S3TugiteNM.SubString(1,12) == "�i�h�r�R�O�j" ) {
   //// ********************
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //   s_Text = "�����O�[�� " + s_S3TugiteNM;
   //// 2016.07.22 SX-30S�ǉ�
   //} else if ( P_Model == "SX-30S" && s_S3TugiteNM.SubString(1,12) == "�i�h�r�R�O�j" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //   s_Text = "�����O�[�� " + s_S3TugiteNM;
   // // ********************
   ////if ( s_Text == "1" || s_Text == "3" ) {
   //} else if ( s_Text == "1" || s_Text == "3" ) {
   //// ********************
   //   s_S3TankanNM = Get_Tankan_DataA3("S3", pcHeadsData);  // S3�[�ǎ��
   //   s_Text = s_S3TankanNM + " " + s_S3TugiteNM;
   //
   //} else if ( s_Text == "2" ) {
   //   s_S3TankanNM = Get_Tankan_DataA3("S3", pcHeadsData);  // S3�[�ǎ��
   //   s_Text = s_S3TankanNM + " " + s_S3TugiteNM + "�v�e";
   //
   //} else {
   //   // 2011.06.20 ɽ�ًK�i�ύX
   //   //if ( s_HEADS_DATA[923].SubString(2,1) == "4" ) {  // �t�����W�t���m�Y��
   //   if ( s_HEADS_DATA[923].SubString(3,1) == "4" ) {  // �t�����W�t���m�Y��
   //   // ***********************
   //      s_Text = s_S3TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[923].TrimRight());
   //   } else {
   //      s_Text = s_S3TugiteNM;
   //   }
   //
   //}
   if ( s_Text == "1" || s_Text == "3" ) {
      s_S3TankanNM = Get_Tankan_DataA3("S3", pcHeadsData);  // S3�[�ǎ��
      s_Text = s_S3TugiteNM + " " + s_S3TankanNM;

   } else if ( s_Text == "2" ) {
      s_S3TankanNM = Get_Tankan_DataA3("S3", pcHeadsData);  // S3�[�ǎ��
      s_Text = s_S3TugiteNM + " " + s_S3TankanNM + " �v�e";

   } else {
      if ( s_HEADS_DATA[923].SubString(3,1) == "4" ) {  // �t�����W�t���m�Y��
         s_Text = s_S3TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[923].TrimRight());
      } else {
         s_Text = s_S3TugiteNM;
      }

   }
   //2019.07.25 ����w���������ύX_E

   //2014.05.28 �p��K�i�\�L�ύX
   if ( s_S3TugiteNM.SubString(1,6) == "�i�h�r" ) {
   } else {
       s_Text = "��" + s_Text;
   }
   //***************************

   Excel_Cell_DataSet(18, 8, s_Text);
   G_Log_Text = "S3�p��E���a         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //***************************

   //***************************
   //***  S4�p��E���a
   //***************************
   // �wB023�x
   //2009.07.31 ASMEɽ�ٖ��̕ύX
   //s_S4TankanNM = "";   // S4�[�ǎ�ށwB026�x,�wB027�x�Ŏg�p
   //
   //i_RecNo = 1139;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   ////2008.07.24 CX-10�ǉ�
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4)); //A���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //       s_Text = "�����O�[�ǂP�@" + s_HEADS_DATA[1152].TrimRight();
   //   } else  {
   //       s_Text = "�����O�[�ǂQ�@" + s_HEADS_DATA[1152].TrimRight();
   //   }
   //} else if (s_Text == "1" || s_Text == "3"){
   ////if (s_Text == "1" || s_Text == "3"){
   ////*******************
   //   s_S4TankanNM = Get_Tankan_Data("S4", pcHeadsData);
   //   s_Text = s_S4TankanNM + "�@" + s_HEADS_DATA[1196].TrimRight();
   //
   //} else if (s_Text == "2"){
   //   s_S4TankanNM = Get_Tankan_Data("S4", pcHeadsData);
   //   s_Text = s_S4TankanNM + "�@" + s_HEADS_DATA[1196].TrimRight() + "�v�e";
   //
   //} else {
   //   if (s_HEADS_DATA[938].SubString(2,1) == "4") {
   //      s_Text = s_HEADS_DATA[1164].TrimRight() + "�@" + Get_Flange_Data(s_HEADS_DATA[938].TrimRight());
   //   } else {
   //      s_Text = s_HEADS_DATA[1164].TrimRight();
   //   }
   //}
   //
   //Excel_Cell_DataSet(18, 16, s_Text);

   s_S4TankanNM = "";  // S4�[�����
   s_S4TugiteNM = "";  // S4�p�薼��

   i_RecNo = 1139;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   if ( s_Text == "1" || s_Text == "2" || s_Text == "3" ){
      //2019.07.25 ����w���������ύX_S
      //s_S4TugiteNM = s_HEADS_DATA[1196].TrimRight();  // S4�[���p�薼��
      s_S4TugiteNM = s_HEADS_DATA[1164].TrimRight();  // S4ɽ�ٌp�薼��
      //2019.07.25 ����w���������ύX_E
   } else {
      s_S4TugiteNM = s_HEADS_DATA[1164].TrimRight();  // S4ɽ�ٌp�薼��
   }
   if ( s_S4TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�P�T�O�|�m�o�r" ) {
      s_S4TugiteNM = "�`�r�l�d �b�k�r�P�T�O�|" + s_S4TugiteNM.Delete(1,34);
   } else if ( s_S4TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�R�O�O�|�m�o�r" ) {
      s_S4TugiteNM = "�`�r�l�d �b�k�r�R�O�O�|" + s_S4TugiteNM.Delete(1,34);
   }
   // 2013.04.03 ɽ�ًK�i�ǉ�
   if ( s_S4TugiteNM.SubString(1,34) == "�`�r�l�d�@�b�k�`�r�r�S�O�O�|�m�o�r" ) {
      s_S4TugiteNM = "�`�r�l�d �b�k�r�S�O�O�|" + s_S4TugiteNM.Delete(1,34);
   }
   // ***********************

   //2019.07.25 ����w���������ύX_S
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));  // A���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //      s_Text = "�����O�[�ǂP " + s_S4TugiteNM;
   //   } else  {
   //      s_Text = "�����O�[�ǂQ " + s_S4TugiteNM;
   //   }
   //
   //} else if ( P_Model == "CX-10D" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B���[�Ǎގ�
   //   if ( s_Text2.TrimRight() == "�b�`�b�V�O�R" || s_Text2.TrimRight() == "�r�s�o�f" ) {
   //      s_Text = "�����O�[�ǂP " + s_S4TugiteNM;
   //   } else  {
   //      s_Text = "�����O�[�ǂQ " + s_S4TugiteNM;
   //   }
   //
   //} else if ( s_Text == "1" || s_Text == "3" ) {
   //   s_S4TankanNM = Get_Tankan_DataA3("S4", pcHeadsData);  // S4�[�ǎ��
   //   s_Text = s_S4TankanNM + " " + s_S4TugiteNM;
   //
   //} else if ( s_Text == "2" ) {
   //   s_S4TankanNM = Get_Tankan_DataA3("S4", pcHeadsData);  // S4�[�ǎ��
   //   s_Text = s_S4TankanNM + " " + s_S4TugiteNM + "�v�e";
   //
   //} else {
   //   // 2011.06.20 ɽ�ًK�i�ύX
   //   //if ( s_HEADS_DATA[938].SubString(2,1) == "4" ) {  // �t�����W�t���m�Y��
   //   if ( s_HEADS_DATA[938].SubString(3,1) == "4" ) {  // �t�����W�t���m�Y��
   //   // ***********************
   //      s_Text = s_S4TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[938].TrimRight());
   //   } else {
   //      s_Text = s_S4TugiteNM;
   //   }
   //
   //}
   if ( s_Text == "1" || s_Text == "3" ) {
      s_S4TankanNM = Get_Tankan_DataA3("S4", pcHeadsData);  // S4�[�ǎ��
      s_Text = s_S4TugiteNM + " " + s_S4TankanNM;

   } else if ( s_Text == "2" ) {
      s_S4TankanNM = Get_Tankan_DataA3("S4", pcHeadsData);  // S4�[�ǎ��
      s_Text = s_S4TugiteNM + " " + s_S4TankanNM + " �v�e";

   } else {
      if ( s_HEADS_DATA[938].SubString(3,1) == "4" ) {  // �t�����W�t���m�Y��
         s_Text = s_S4TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[938].TrimRight());
      } else {
         s_Text = s_S4TugiteNM;
      }

   }
   //2019.07.25 ����w���������ύX_E

   //2014.05.28 �p��K�i�\�L�ύX
   if ( s_S4TugiteNM.SubString(1,6) == "�i�h�r" ) {
   } else {
       s_Text = "��" + s_Text;
   }
   //***************************

   Excel_Cell_DataSet(18, 16, s_Text);
   G_Log_Text = "S4�p��E���a         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //***************************

   //***************************
   //***  S3���x�v
   //***  S3���ы敪
   //***************************
   // �wB024�x �wB025�x
   s_Text = s_HEADS_DATA[928].TrimRight();      // S3���x�v���

   //2019.07.25 ����w���������ύX_S
   //if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 11) {
      if ( s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2 ||
           s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 4 ||
           s_Text.ToIntDef(0) == 5 || s_Text.ToIntDef(0) == 8 ||
           s_Text.ToIntDef(0) == 9 || s_Text.ToIntDef(0) == 10  ) {
         s_Text2 = " I�^";
      } else if ( s_Text.ToIntDef(0) == 6 || s_Text.ToIntDef(0) == 11 ) {
         s_Text2 = " L�^";
      } else {
         s_Text2 = "";
      }
   //2019.07.25 ����w���������ύX_E
      // �wB024�x
      s_Text = AnsiString(s_HEADS_DATA[929].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[930].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1357].TrimRight()));

      //2019.07.25 ����w���������ύX_S
      s_Text = s_Text + s_Text2;
      //2019.07.25 ����w���������ύX_E

      Excel_Cell_DataSet(19, 7, s_Text);
      G_Log_Text = "S3���x�v             �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB025�x
      //2019.07.25 ����w���������ύX_S
      //if (s_HEADS_DATA[846].TrimRight() == "1"){    // B���ы敪
      //   s_Text = "���їp";
      //2020.02.04 P_FL�C��_S
      //if ( P_FL = 0 && s_HEADS_DATA[846].TrimRight() == "1" ) {
      if ( P_FL == 0 && s_HEADS_DATA[846].TrimRight() == "1" ) {
      //2020.02.04 P_FL�C��_E
         // �΍s�� B���ы敪
         s_Text = "���їp";
      //2020.02.04 P_FL�C��_S
      //} else if ( P_FL = 1 && s_HEADS_DATA[847].TrimRight() == "1" ) {
      } else if ( P_FL == 1 && s_HEADS_DATA[847].TrimRight() == "1" ) {
      //2020.02.04 P_FL�C��_E
         // �΍s�� A���ы敪
         s_Text = "���їp";
      //2019.07.25 ����w���������ύX_E
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(19, 11, s_Text);
      G_Log_Text = "S3���ы敪           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   } else {
      s_Text = "";
      // �wB024�x
      Excel_Cell_DataSet(19, 7, s_Text);
      G_Log_Text = "S3���x�v             �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB025�x
      Excel_Cell_DataSet(19, 11, s_Text);
      G_Log_Text = "S3���ы敪           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  S4���x�v
   //***  S4���ы敪
   //***************************
   // �wB026�x�wB027�x
   s_Text = s_HEADS_DATA[943].TrimRight();      // S4���x�v���

   //2019.07.25 ����w���������ύX_S
   //if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 11) {
      if ( s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2 ||
           s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 4 ||
           s_Text.ToIntDef(0) == 5 || s_Text.ToIntDef(0) == 8 ||
           s_Text.ToIntDef(0) == 9 || s_Text.ToIntDef(0) == 10  ) {
         s_Text2 = " I�^";
      } else if ( s_Text.ToIntDef(0) == 6 || s_Text.ToIntDef(0) == 11 ) {
         s_Text2 = " L�^";
      } else {
         s_Text2 = "";
      }
   //2019.07.25 ����w���������ύX_E
      // �wB026�x
      s_Text = AnsiString(s_HEADS_DATA[944].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[945].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1358].TrimRight()));

      //2019.07.25 ����w���������ύX_S
      s_Text = s_Text + s_Text2;
      //2019.07.25 ����w���������ύX_E

      Excel_Cell_DataSet(19, 15, s_Text);
      G_Log_Text = "S4���x�v             �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB027�x
      //2019.07.25 ����w���������ύX_S
      //if (s_HEADS_DATA[847].TrimRight() == "1"){    // A���ы敪
      //   s_Text = "���їp";
      //2020.02.04 P_FL�C��_S
      //if ( P_FL = 0 && s_HEADS_DATA[847].TrimRight() == "1" ) {
      if ( P_FL == 0 && s_HEADS_DATA[847].TrimRight() == "1" ) {
      //2020.02.04 P_FL�C��_E
         // ��`�� A���ы敪
         s_Text = "���їp";
      //2020.02.04 P_FL�C��_S
      //} else if ( P_FL = 1 && s_HEADS_DATA[846].TrimRight() == "1" ) {
      } else if ( P_FL == 1 && s_HEADS_DATA[846].TrimRight() == "1" ) {
      //2020.02.04 P_FL�C��_E
         // �΍s�� B���ы敪
         s_Text = "���їp";
      //2019.07.25 ����w���������ύX_E
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(19, 24, s_Text);
      G_Log_Text = "S4���ы敪           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   } else {
      s_Text = "";
      // �wB026�x
      Excel_Cell_DataSet(19, 15, s_Text);
      G_Log_Text = "S4���x�v             �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB027�x
      Excel_Cell_DataSet(19, 24, s_Text);
      G_Log_Text = "S4���ы敪           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //*********************************
   //***  S3���͌v 2002.10.19 S.Y
   //*********************************
   // �wB028�x
   s_Text = s_HEADS_DATA[931].TrimRight();   // S3���͌v���

   // 2020.07.17 ���͌v�ύX_S
   ////2019.07.25 ����w���������ύX_S
   //////2004.05.19 ���͌v�����ǉ�
   //////if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   ////if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //////*************************
   //if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 20) {
   ////2019.07.25 ����w���������ύX_E
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 26) {
   // 2020.07.17 ���͌v�ύX_E
      // S3���͌v�ڐ� 1
      i_RecNo = 932;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S3���͌v�ڐ� 2
         i_RecNo = 933;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S3���͌v
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[931].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S3���͌v�ڐ�2   RecNo:" + AnsiString(i_RecNo)
                            + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

            s_Text = "";
         }

      //2018.04.12 �ڐ��ǉ�_S
      } else if (s_Text == "-0.10") {
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S3���͌v�ڐ� 2
         i_RecNo = 933;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S3���͌v
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[931].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S3���͌v�ڐ�2   RecNo:" + AnsiString(i_RecNo)
                            + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

            s_Text = "";
         }
      //2018.04.12 �ڐ��ǉ�_E

      } else {
         G_ErrLog_Text = "S3���͌v�ڐ�1   RecNo:" + AnsiString(i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = "";
      }

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(20, 7, s_Text);
   G_Log_Text = "S3���͌v             �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //***************************
   //***  S4���͌v
   //***************************
   // �wB029�x
   // S4���͌v�ڐ� 1
   // i_RecNo = 947;
   // s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // if (AllFloatChk(s_Text.c_str(),0) == 0){
   //   s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

   // } else {
   //    G_ErrLog_Text = "S4���͌v�ڐ�1   RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
   //    Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
   //    Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   //
   //    i_ErrFlg = 1;
   // }

   // S4���͌v�ڐ� 2
   // i_RecNo = 948;
   // s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // if (AllFloatChk(s_Text.c_str(),0) == 0){
   //   s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
   // } else {
   //    G_ErrLog_Text = "S4���͌v�ڐ�2   RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
   //    Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
   //    Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

   //    i_ErrFlg = 1;
   //  }

   // s_Text = s_HEADS_DATA[946].TrimRight();
   // if (s_Text == "01" || s_Text == "02" || s_Text == "03" || s_Text == "04" || s_Text == "09"){
   //    s_Text = Get_AturyokuKei(s_HEADS_DATA[946].TrimRight()) + " ";
   //    s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
   // } else {
   //    s_Text = "";
   // }

   // if (i_ErrFlg == 0){
   //    Excel_Cell_DataSet(20, 15, s_Text);
   //    G_Log_Text = "S4���͌v             �w" + s_Text + "�x���Z�b�g�B";
   //    Write_Log(G_Log_Text);
   // } else {
   //    s_Text = "";
   //    Excel_Cell_DataSet(20, 15, s_Text);
   // }

   //***************************
   //***  S4���͌v 2002.10.19 S.Y
   //***************************
   // �wB029�x
   s_Text = s_HEADS_DATA[946].TrimRight();   // S4���͌v���

   // 2020.07.17 ���͌v�ύX_S
   ////2019.07.25 ����w���������ύX_S
   //////2004.05.19 ���͌v�����ǉ�
   //////if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   ////if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //////*************************
   //if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 20) {
   ////2019.07.25 ����w���������ύX_E
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 26) {
   // 2020.07.17 ���͌v�ύX_E
      // S4���͌v�ڐ� 1
      i_RecNo = 947;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S4���͌v�ڐ� 2
         i_RecNo = 948;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S4���͌v
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[946].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S4���͌v�ڐ�2   RecNo:" + AnsiString(i_RecNo)
                            + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

            s_Text = "";
         }

      //2018.04.12 �ڐ��ǉ�_S
      } else if (s_Text == "-0.10") {
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S4���͌v�ڐ� 2
         i_RecNo = 948;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S4���͌v
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[946].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S4���͌v�ڐ�2   RecNo:" + AnsiString(i_RecNo)
                            + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

            s_Text = "";
         }
      //2018.04.12 �ڐ��ǉ�_E

      } else {
         G_ErrLog_Text = "S4���͌v�ڐ�1   RecNo:" + AnsiString(i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = "";
      }

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(20, 15, s_Text);
   G_Log_Text = "S4���͌v             �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  S3�t�̌p�薼�a
   //***************************
   // �wB030�x
   s_Text = ZenToHan(s_HEADS_DATA[1224].TrimRight()) + ZenToHan(s_HEADS_DATA[1225].TrimRight());
   Excel_Cell_DataSet(21, 7, s_Text);
   G_Log_Text = "S3�t�̌p�薼�a       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  S4�t�̌p�薼�a
   //***************************
   // �wB031�x
   s_Text = ZenToHan(s_HEADS_DATA[1228].TrimRight()) + ZenToHan(s_HEADS_DATA[1229].TrimRight());
   Excel_Cell_DataSet(21, 15, s_Text);
   G_Log_Text = "S4�t�̌p�薼�a       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���[�Ǎގ�
   //***************************
   // �wB032�x
   s_Text = s_HEADS_DATA[1136].TrimRight();  // S1�[��

   switch (s_Text.ToIntDef(0)) {
      case 1: case 2: case 3:
         // �a�����̎擾
         s_Text = Get_Zaisitu_NameA3(s_HEADS_DATA[1060].TrimRight());
         break;
      default:
         s_Text = "";
         break;
   }

   //2009.04.27 �ټ�Ē��L
   if (s_HEADS_DATA[436].TrimRight() == "0" && s_HEADS_DATA[437].TrimRight() == "0"){
   } else {
      if (s_Text != ""){
         s_Text = s_Text + " ��";
      } else if (s_HEADS_DATA[1331].TrimRight() == "�����}�[�������O�[�ǁ@E333263"){
         s_Text = "��";
      }
   }
   //********************
   //2010.06.07 CX-10D�ǉ�
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //*********************
   //2019.07.25 ����w���������ύX_S
   // CX-10D�ł�S1,S4(B��)���ݸގ�t
   else if ( P_FL == 1 ) {
      // �΍s���ł�S1,S4(B��)�ɒ[�ǎ�t
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //2019.07.25 ����w���������ύX_E
   Excel_Cell_DataSet(22, 7, s_Text);
   G_Log_Text = "B���[�Ǎގ�          �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���[�Ǎގ�
   //***************************
   // �wB033�x
   s_Text = s_HEADS_DATA[1137].TrimRight();  // S2�[��

   switch (s_Text.ToIntDef(0)) {
      case 1: case 2: case 3:
         // �a�����̎擾
         s_Text = Get_Zaisitu_NameA3(s_HEADS_DATA[1061].TrimRight());
         break;
      default:
         s_Text = "";
         break;
   }

   //2008.07.24 CX-10�ǉ�
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10" ) {
      s_Text = Get_Zaisitu_NameA3(s_HEADS_DATA[1061].TrimRight());
   }
   //*******************
   //2009.04.27 �ټ�Ē��L
   if (s_HEADS_DATA[436].TrimRight() == "0" && s_HEADS_DATA[437].TrimRight() == "0"){
   } else {
      if (s_Text != ""){
         s_Text = s_Text + " ��";
      } else if (s_HEADS_DATA[1331].TrimRight() == "�����}�[�������O�[�ǁ@E333263"){
         s_Text = "��";
      }
   }
   //********************
   //2010.06.07 CX-10D�ǉ�
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
      //2019.07.25 ����w���������ύX_S
      //if (s_Text != ""){
      //   s_Text = s_Text + " (S2,S3)";
      //}
      // CX-10D�ł�S2,S3(A��)���ݸނ͕t���Ȃ�
      s_Text = "";
      //2019.07.25 ����w���������ύX_E
   }
   //*********************
   //2019.07.25 ����w���������ύX_S
   else if ( P_FL == 1 ) {
      // �΍s���ł�S2,S3(A��)�ɒ[�ǎ�t
      if (s_Text != ""){
         s_Text = s_Text + " (S2,S3)";
      }
   }
   //2019.07.25 ����w���������ύX_E

   Excel_Cell_DataSet(22, 15, s_Text);
   G_Log_Text = "A���[�Ǎގ�          �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���޽��čގ�
   //***************************
   // �wB034�x
   i_RecNo = 1136;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();     // S1�[��
   // 2020.12.21 �-Ķ޽��ĕύX_S
   i_RecNo = 1138;
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();    // S3�[��
   i_RecNo = 1139;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();    // S4�[��
   // 2020.12.21 �-Ķ޽��ĕύX_E
   if (s_Text == "1" || s_Text == "2" || s_Text == "3"){
      i_RecNo = 893;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S1ɽ�ٌp��
      // 2011.06.20 ɽ�ًK�i�ύX
      //if (s_Text.SubString(2,1) == "2" || s_Text.SubString(2,1) == "3"){
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
      // ***********************
         //2017.10.06 ��Ķ޽��čގ��ύX_S
         //// B�޽���CODE�Ō���
         //i_RecNo = 835;
         //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         //s_Text = Chg_Zaisitu_Code(s_Text,"1");
         //s_Text = Get_Zaisitu_Name(s_Text);
         s_Text = Get_SheetGasketMatA3("B", pcHeadsData);  // B����Ķ޽��čގ�
         //2017.10.06 ��Ķ޽��čގ��ύX_E

      } else {
         s_Text = "";
      }
   // 2020.12.21 �-Ķ޽��ĕύX_S
   } else if ( P_FL == 1 && (s_Text2 == "1" || s_Text2 == "2" || s_Text2 == "3") ) {
      // �΍s��(S4�[��)
      i_RecNo = 938;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S4ɽ�ٌp��
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
         s_Text = Get_SheetGasketMatA3("B", pcHeadsData);  // B����Ķ޽��čގ�
      } else {
         s_Text = "";
      }
   } else if ( P_FL != 1 && (s_Text1 == "1" || s_Text1 == "2" || s_Text1 == "3") ) {
      // �΍s���ȊO(S3�[��)
      i_RecNo = 923;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S3ɽ�ٌp��
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
         s_Text = Get_SheetGasketMatA3("B", pcHeadsData);  // B����Ķ޽��čގ�
      } else {
         s_Text = "";
      }
   // 2020.12.21 �-Ķ޽��ĕύX_E
   } else {
      s_Text = "";
   }
   //2010.06.07 CX-10D�ǉ�
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //*********************
   // 2022.04.21 CX-01D�ǉ�_S
   // 2023.02.16 �����O�[�Ǐ����ύX_S
   //else if ( P_Model == "CX-01D" ) {
   else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
   // 2023.02.16 �����O�[�Ǐ����ύX_E
      if (s_Text != ""){
         s_Text = s_Text + " (S1)";
      }
   }
   // 2022.04.21 CX-01D�ǉ�_E
   //2019.07.25 ����w���������ύX_S
   // CX-10D�ł�S1,S4(B��)���ݸގ�t
   else if ( P_FL == 1 ) {
      // �΍s���ł�S1,S4(B��)�ɒ[�ǎ�t
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //2019.07.25 ����w���������ύX_E
   Excel_Cell_DataSet(23, 7, s_Text);
   G_Log_Text = "B���޽��čގ�        �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���޽��čގ�
   //***************************
   // �wB035�x
   i_RecNo = 1137;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();     // S2�[��
   // 2020.12.21 �-Ķ޽��ĕύX_S
   i_RecNo = 1138;
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();    // S3�[��
   i_RecNo = 1139;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();    // S4�[��
   // 2020.12.21 �-Ķ޽��ĕύX_E
   if (s_Text == "1" || s_Text == "2" || s_Text == "3"){
      i_RecNo = 908;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S2ɽ�ٌp��
      // 2011.06.20 ɽ�ًK�i�ύX
      //if (s_Text.SubString(2,1) == "2" || s_Text.SubString(2,1) == "3"){
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
      // ***********************
         //2017.10.06 ��Ķ޽��čގ��ύX_S
         // A�޽���CODE�Ō���
         //i_RecNo = 836;
         //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         //s_Text = Chg_Zaisitu_Code(s_Text,"1");
         //s_Text = Get_Zaisitu_Name(s_Text);
         s_Text = Get_SheetGasketMatA3("A", pcHeadsData);  // A����Ķ޽��čގ�
         //2017.10.06 ��Ķ޽��čގ��ύX_E

      } else {
         s_Text = "";
      }
   // 2020.12.21 �-Ķ޽��ĕύX_S
   } else if ( P_FL == 1 && (s_Text1 == "1" || s_Text1 == "2" || s_Text1 == "3") ) {
      // �΍s��(S3�[��)
      i_RecNo = 923;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S3ɽ�ٌp��
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
         s_Text = Get_SheetGasketMatA3("A", pcHeadsData);  // A����Ķ޽��čގ�
      } else {
         s_Text = "";
      }
   } else if ( P_FL != 1 && (s_Text2 == "1" || s_Text2 == "2" || s_Text2 == "3") ) {
      // �΍s���ȊO(S4�[��)
      i_RecNo = 938;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S4ɽ�ٌp��
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
         s_Text = Get_SheetGasketMatA3("A", pcHeadsData);  // A����Ķ޽��čގ�
      } else {
         s_Text = "";
      }
   // 2020.12.21 �-Ķ޽��ĕύX_E
   } else {
      s_Text = "";
   }

   // 2020.12.21 �-Ķ޽��ĕύX_S
   ////2008.07.24 CX-10�ǉ�
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10" ) {
   //   i_RecNo = 938;
   //   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S4ɽ�ٌp��
   //   if (s_Text.SubString(2,1) == "2" || s_Text.SubString(2,1) == "3"){
   //      // A�޽���CODE�Ō���
   //      i_RecNo = 836;
   //      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //      s_Text = Chg_Zaisitu_Code(s_Text,"1");
   //      s_Text = Get_Zaisitu_NameA3(s_Text);
   //
   //   } else {
   //      s_Text = "";
   //   }
   ////2008.08.07 CX-10�ȊO�ł��o�͂���
   ////} else {
   ////   s_Text = "";
   ////********************************
   //}
   ////*******************
   // 2020.12.21 �-Ķ޽��ĕύX_E
   //2010.06.07 CX-10D�ǉ�
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
      //2019.07.25 ����w���������ύX_S
      //if (s_Text != ""){
      //   s_Text = s_Text + " (S1,S4)";
      //}
      // CX-10D�ł�S2,S3(A��)���ݸނ͕t���Ȃ�
      s_Text = "";
      //2019.07.25 ����w���������ύX_E
   }
   //*********************
   // 2022.04.21 CX-01D�ǉ�_S
   // 2023.02.16 �����O�[�Ǐ����ύX_S
   //else if ( P_Model == "CX-01D" ) {
   else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
   // 2023.02.16 �����O�[�Ǐ����ύX_E
      if (s_Text != ""){
         s_Text = s_Text + " (S3)";
      }
   }
   // 2022.04.21 CX-01D�ǉ�_E
   //2019.07.25 ����w���������ύX_S
   else if ( P_FL == 1 ) {
      // �΍s���ł�S2,S3(A��)�ɒ[�ǎ�t
      if (s_Text != ""){
         s_Text = s_Text + " (S2,S3)";
      }
   }
   //2019.07.25 ����w���������ύX_E
   Excel_Cell_DataSet(23, 15, s_Text);
   G_Log_Text = "A���޽��čގ�        �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B����ݼލގ�
   //***************************
   // �wB036�x
   i_RecNo = 1050;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2018.02.09 �W���t�����W�ގ��ǉ�_S
   i_RecNo = 37;
   i_CODE = s_HEADS_DATA[i_RecNo].ToIntDef(0); //�t���[���R�[�h
   //2018.02.09 �W���t�����W�ގ��ǉ�_E
   if (s_Text != "0000"){
      // SPFLMB�Ō���
      s_Text = Get_Zaisitu_NameA3(s_Text);

   } else {
      i_RecNo = 1136;   // S1�[��
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      //2008.07.24 CX-10�ǉ�
      P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
      if (P_Model == "CX-10" ) {
         s_Text = "";
      //2010.06.07 CX-10D�ǉ�
      } else if ( P_Model == "CX-10D" ) {
         s_Text = "";
      //*********************
      // 2023.02.16 �����O�[�Ǐ����ύX_S
      //// 2021.01.06 CX(W)-03�ǉ�(CX(W)-00��CX(W)-01)_S
      //////2016.04.22 CX-00�ǉ�
      ////} else if ( P_Model == "CX-00" ) {
      ////   s_Text = "";
      ////} else if ( P_Model == "CXW-00" ) {
      ////   s_Text = "";
      //////********************
      //} else if ( P_Model == "CX-01" ) {
      //   s_Text = "";
      //} else if ( P_Model == "CXW-01" ) {
      //   s_Text = "";
      //// 2021.01.06 CX(W)-03�ǉ�(CX(W)-00��CX(W)-01)_E
      //// 2022.04.21 CX-01D�ǉ�_S
      //} else if ( P_Model == "CX-01D" ) {
      //   s_Text = "";
      //// 2022.04.21 CX-01D�ǉ�_E
      } else if ( P_Model == "CX-01" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
         s_Text = "";
      } else if ( P_Model == "CXW-01" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
         s_Text = "";
      } else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
         s_Text = "";
      // 2023.02.16 �����O�[�Ǐ����ύX_E
      // 2013.04.03 SX-30�ǉ�
      } else if ( P_Model == "SX-30" && s_S1TugiteNM.SubString(1,12) == "�i�h�r�R�O�j" ) {
         s_Text = "";
      // ********************
      // 2016.07.22 SX-30S�ǉ�
      } else if ( P_Model == "SX-30S" && s_S1TugiteNM.SubString(1,12) == "�i�h�r�R�O�j" ) {
         s_Text = "";
      // *********************
      } else if (s_Text == "3"){
      //if (s_Text == "3"){
      //*******************
         // 2020.09.01 ���p�ݸޕύX_S
         ////2019.07.25 ����w���������ύX_S
         ////s_Text = "����ݼ�  SS40  W.F";
         //s_Text = "����ݼ�  SS400  W.F";
         ////2019.07.25 ����w���������ύX_E
         if (P_Model == "UX-10" && s_HEADS_DATA[1060].TrimRight() == "1060" ) {
             s_Text = "SS400  W.F";
         } else if ( P_Model == "LX-00" && s_HEADS_DATA[1060].TrimRight() == "1060" ) {
             s_Text = "SS400  W.F";
         }
         // 2020.09.01 ���p�ݸޕύX_E
         // 2020.10.30 ���p�ݸޕύX_S
         else {
             s_Text = "";
         }
         // 2020.10.30 ���p�ݸޕύX_E
      //2018.02.09 �W���t�����W�ގ��ǉ�_S
      } else if (s_Text == "1"){
         // �t���[�t�����W�[��
         s_Text = "C.S.";
      } else if ( i_CODE == 11 || i_CODE == 12 || i_CODE == 13 || i_CODE == 61 ||
                  i_CODE == 21 || i_CODE == 22 || i_CODE == 26 || i_CODE == 62 ||
                  i_CODE == 16 || i_CODE == 14 || i_CODE == 15 || i_CODE == 63 ||
                  i_CODE == 29 || i_CODE == 27 || i_CODE == 28 || i_CODE == 64 ) {
         // �p�C�v�m�Y��
         s_Text = "C.S.";
      //2018.02.09 �W���t�����W�ގ��ǉ�_S
      } else {
         s_Text = "";
      }
   }

   //2009.04.27 �ټ�Ē��L
   i_RecNo = 437;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      if (s_Text != ""){
         s_Text = s_Text + " ��";
      } else if (s_HEADS_DATA[1136].ToIntDef(0) == 1 ||
                 s_HEADS_DATA[1136].ToIntDef(0) == 2 ) {  // �[�ǃt�����W�t
         s_Text = "C.S. ��";
      // 2011.06.20 ɽ�ًK�i�ύX
      //} else if (s_HEADS_DATA[893].SubString(2,1) == "4") {  // �t�����W�t���m�Y��
      } else if (s_HEADS_DATA[893].SubString(3,1) == "4") {  // �t�����W�t���m�Y��
      // ***********************
         s_Text = "C.S. ��";
      }
   }
   //********************

   //2019.07.25 ����w���������ύX_S
   if ( P_FL == 1 ) {
      // �΍s���ł�S1,S4(B��)�Ƀt�����W��t
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //2019.07.25 ����w���������ύX_E

   Excel_Cell_DataSet(24, 7, s_Text);
   G_Log_Text = "B����ݼލގ�         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //***************************
   //***  A����ݼލގ�
   //***************************
   // �wB037�x
   i_RecNo = 1051;   // SPFLMA
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2018.02.09 �W���t�����W�ގ��ǉ�_S
   i_RecNo = 37;
   i_CODE = s_HEADS_DATA[i_RecNo].ToIntDef(0); //�t���[���R�[�h
   //2018.02.09 �W���t�����W�ގ��ǉ�_E
   if (s_Text != "0000"){
      // SPFLMB�Ō���
      s_Text = Get_Zaisitu_NameA3(s_Text);

   } else {
      i_RecNo = 1137;   // S2�[��
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      //2008.07.24 CX-10�ǉ�
      P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
      if (P_Model == "CX-10" ) {
         s_Text = "";
      //2010.06.07 CX-10D�ǉ�
      } else if ( P_Model == "CX-10D" ) {
         s_Text = "";
      //*********************
      // 2022.04.21 CX-01D�ǉ�_S
      // 2023.02.16 �����O�[�Ǐ����ύX_S
      //} else if ( P_Model == "CX-01D" ) {
      } else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
      // 2023.02.16 �����O�[�Ǐ����ύX_E
         s_Text = "";
      // 2022.04.21 CX-01D�ǉ�_E
      } else if (s_Text == "3"){
      //if (s_Text == "3"){
      //*******************
         // 2020.09.01 ���p�ݸޕύX_S
         ////2019.07.25 ����w���������ύX_S
         ////s_Text = "����ݼ�  SS40  W.F";
         //s_Text = "����ݼ�  SS400  W.F";
         ////2019.07.25 ����w���������ύX_E
         if (P_Model == "UX-10" && s_HEADS_DATA[1061].TrimRight() == "1060" ) {
             s_Text = "SS400  W.F";
         } else if ( P_Model == "LX-00" && s_HEADS_DATA[1061].TrimRight() == "1060" ) {
             s_Text = "SS400  W.F";
         }
         // 2020.09.01 ���p�ݸޕύX_E
         // 2020.10.30 ���p�ݸޕύX_S
         else {
             s_Text = "";
         }
         // 2020.10.30 ���p�ݸޕύX_E
      //2018.02.09 �W���t�����W�ގ��ǉ�_S
      } else if (s_Text == "1"){
         // �t���[�t�����W�[��
         s_Text = "C.S.";
      } else if ( i_CODE == 11 || i_CODE == 12 || i_CODE == 13 || i_CODE == 61 ||
                  i_CODE == 21 || i_CODE == 22 || i_CODE == 26 || i_CODE == 62 ||
                  i_CODE == 16 || i_CODE == 14 || i_CODE == 15 || i_CODE == 63 ||
                  i_CODE == 29 || i_CODE == 27 || i_CODE == 28 || i_CODE == 64 ) {
         // �p�C�v�m�Y��
         s_Text = "C.S.";
      //2018.02.09 �W���t�����W�ގ��ǉ�_S
      } else {
         s_Text = "";
      }
   }

   //2009.04.27 �ټ�Ē��L
   i_RecNo = 437;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      if (s_Text != ""){
         s_Text = s_Text + " ��";
      } else if (s_HEADS_DATA[1137].ToIntDef(0) == 1 ||
                 s_HEADS_DATA[1137].ToIntDef(0) == 2 ) {  // �[�ǃt�����W�t
         s_Text = "C.S. ��";
      // 2011.06.20 ɽ�ًK�i�ύX
      //} else if (s_HEADS_DATA[908].SubString(2,1) == "4") {  // �t�����W�t���m�Y��
      } else if (s_HEADS_DATA[908].SubString(3,1) == "4") {  // �t�����W�t���m�Y��
      // ***********************
         s_Text = "C.S. ��";
      }
   }
   //********************

   //2019.07.25 ����w���������ύX_S
   if ( P_FL == 1 ) {
      // �΍s���ł�S2,S3(A��)�Ƀt�����W��t
      if (s_Text != ""){
         s_Text = s_Text + " (S2,S3)";
      }
   }
   //2019.07.25 ����w���������ύX_E

   Excel_Cell_DataSet(24, 15, s_Text);
   G_Log_Text = "A����ݼލގ�         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �d��^�C�g��
   //***  B���d�㖼��
   //***************************
   // �wB038�x,�wB039�x
   i_RecNo = 1136;   // S1�[��
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   //2008.07.24 CX-10�ǉ�
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10" ) {
      // �wB038�x
      s_Text = "�[�ǎd��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB039�x
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // �`�^���̏ꍇ�͎d��T
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // �b�`�b�V�O�R
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      ////2010.06.07 CX-10�ύX
      //} else if ( s_Text.TrimRight() == "" ) {              // �Ȃ�
      //  s_Text = "�\�\�\�\�\�\";
      ////********************
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // �r�s�o�f
      //  s_Text = "�O�ʂ̂��ڰтƓ���F";
      //} else {
      //  s_Text = "���O  �_��";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else {
        s_Text = "���O  �_��";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   //2010.06.07 CX-10D�ǉ�
   } else if ( P_Model == "CX-10D" ) {
      // �wB038�x
      s_Text = "�[�ǎd��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB039�x
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // �b�`�b�V�O�R
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "" ) {              // �Ȃ�
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // �r�s�o�f
      //  s_Text = "�O�ʂ̂��ڰтƓ���F (S1,S4)";
      //} else {
      //  s_Text = "���O  �_�� (S1,S4)";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F (S1,S4)";
      } else {
        s_Text = "���O  �_�� (S1,S4)";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   //*********************

   // 2023.02.16 �����O�[�Ǐ����ύX_S
   ////2016.05.19 CX-00�ǉ�
   //// 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_S
   ////} else if ( P_Model == "CX-00" ) {
   //} else if ( P_Model == "CX-01" ) {
   //// 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_E
   } else if ( P_Model == "CX-01" && s_HEADS_DATA[36].Pos("Y") == 0  ) {
   // 2023.02.16 �����O�[�Ǐ����ύX_E
      // �wB038�x
      s_Text = "�[�ǎd��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB039�x
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // �b�`�b�V�O�R
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "" ) {              // �Ȃ�
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // �r�s�o�f
      //  s_Text = "�O�ʂ̂��ڰтƓ���F";
      //} else {
      //  s_Text = "���O  �_��";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      // ***********************
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else {
        s_Text = "���O  �_��";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   // 2023.02.16 �����O�[�Ǐ����ύX_S
   ////2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_S
   ////} else if ( P_Model == "CXW-00" ) {
   //} else if ( P_Model == "CXW-01" ) {
   ////2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_E
   } else if ( P_Model == "CXW-01" && s_HEADS_DATA[36].Pos("Y") == 0  ) {
   // 2023.02.16 �����O�[�Ǐ����ύX_E
      // �wB038�x
      s_Text = "�[�ǎd��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB039�x
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // �b�`�b�V�O�R
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "" ) {              // �Ȃ�
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // �r�s�o�f
      //  s_Text = "�O�ʂ̂��ڰтƓ���F";
      //} else {
      //  s_Text = "���O  �_��";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else {
        s_Text = "���O  �_��";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   //********************

   // 2023.02.16 �����O�[�Ǐ����ύX_S
   // 2022.04.21 CX-01D�ǉ�_S
   //} else if ( P_Model == "CX-01D" ) {
   } else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0  ) {
   // 2023.02.16 �����O�[�Ǐ����ύX_E
      // �wB038�x
      s_Text = "�[�ǎd��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F (S1)";
      } else {
        s_Text = "���O  �_�� (S1)";
      }
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   // 2022.04.21 CX-01D�ǉ�_E

   // 2013.04.03 SX-30�ǉ�
   } else if ( P_Model == "SX-30" && s_S1TugiteNM.SubString(1,12) == "�i�h�r�R�O�j" ) {
      // �wB038�x
      s_Text = "�[�ǎd��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB039�x
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // �b�`�b�V�O�R
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "" ) {              // �Ȃ�
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "STKM13A" ) {       // �r�s�j�l�P�R�`
      //  s_Text = "�O�ʂ̂��ڰтƓ���F";
      //} else {
      //  s_Text = "���O  �_��";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else {
        s_Text = "���O  �_��";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   // ********************

    // 2016.07.22 SX-30S��
   } else if ( P_Model == "SX-30S" && s_S1TugiteNM.SubString(1,12) == "�i�h�r�R�O�j" ) {
      // �wB038�x
      s_Text = "�[�ǎd��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB039�x
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // �b�`�b�V�O�R
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "" ) {              // �Ȃ�
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "STKM13A" ) {       // �r�s�j�l�P�R�`
      //  s_Text = "�O�ʂ̂��ڰтƓ���F";
      //} else {
      //  s_Text = "���O  �_��";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else {
        s_Text = "���O  �_��";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   // ********************

   } else if (s_Text == 1 || s_Text == 2){
   //if (s_Text == 1 || s_Text == 2){
   //********************
      // �wB038�x
      s_Text = "�[�ǎd��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB039�x
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if(  s_Text.SubString(1,2) == "TP") {     // �`�^���̏ꍇ�͎d��T
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      ////2004.01.28 ASME
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      ////***************
      //} else {
      //  s_Text = "���O  �_��";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.23 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.23 Ư�ْǉ�_E
      } else {
        s_Text = "���O  �_��";
        //2019.07.25 ����w���������ύX_S
        if ( P_FL == 1 ) {
           // �΍s���ł�S1,S4(B��)�ɒ[�ǎ�t
           s_Text = s_Text + " (S1,S4)";
        }
        //2019.07.25 ����w���������ύX_E
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   } else if (s_Text == 3){
      // �wB038�x
      s_Text = "�[�ǎd��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB039�x
      //2019.10.10 �����O�[�ǎd��ύX_S
      //s_Text = "�O�ʂ̂��ڰтƓ���F";
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // ALBC�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "STPG" ) { // STPG�̏ꍇ�͓h��
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else if ( s_Text == "SGP" ) {  // SGP�̏ꍇ�͓h��
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else if ( s_Text == "CS" ) {   // CS�̏ꍇ�͓h��
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else {                         // ���̑��̏ꍇ�͎d�グ�P
        s_Text = "���O  �_��";
      }
      //2019.10.10 �����O�[�ǎd��ύX_E
      //2019.07.25 ����w���������ύX_S
      if ( P_FL == 1 ) {
         // �΍s���ł�S1,S4(B��)�ɒ[�ǎ�t
         s_Text = s_Text + " (S1,S4)";
      }
      //2019.07.25 ����w���������ύX_E
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   } else {
      // �wB038�x
      s_Text = "�m�Y���d��";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "�d��^�C�g��         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      // �wB039�x
      i_RecNo = 904;   // S1ɽ�َd��
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text = Get_SiageMeisyouA3(s_Text);
      //2019.07.25 ����w���������ύX_S
      if ( P_FL == 1 ) {
         // �΍s���ł�S1,S4(B��)�Ƀm�Y����t
         if (s_Text != "" && s_Text != "�\�\�\�\�\�\"){
            s_Text = s_Text + " (S1,S4)";
         }
      }
      //2019.07.25 ����w���������ύX_E
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  A���d�㖼��
   //***************************
   // �wB040�x
   i_RecNo = 1137;   // S2�[��
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   //2008.07.24 CX-10�ǉ�
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10" ) {
      // �wB040�x
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // �`�^���̏ꍇ�͎d��T
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // �b�`�b�V�O�R
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      ////2010.06.07 CX-10�ύX
      //} else if ( s_Text.TrimRight() == "" ) {              // �Ȃ�
      //  s_Text = "�\�\�\�\�\�\";
      ////********************
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // �r�s�o�f
      //  s_Text = "�O�ʂ̂��ڰтƓ���F";
      //} else {
      //  s_Text = "���O  �_��";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else {
        s_Text = "���O  �_��";
      }
      // ***********************
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   //2010.06.07 CX-10D�ǉ�
   } else if ( P_Model == "CX-10D" ) {
      // �wB040�x
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // �b�`�b�V�O�R
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "" ) {              // �Ȃ�
      //  s_Text = "�\�\�\�\�\�\";
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // �r�s�o�f
      //  s_Text = "�O�ʂ̂��ڰтƓ���F (S2,S3)";
      //} else {
      //  s_Text = "���O  �_�� (S2,S3)";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F (S2,S3)";
      } else {
        s_Text = "���O  �_�� (S2,S3)";
      }
      // ***********************
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   //*********************

   // 2022.04.21 CX-01D�ǉ�_S
   // 2023.02.16 �����O�[�Ǐ����ύX_S
   //} else if ( P_Model == "CX-01D" ) {
   } else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0  ) {
   // 2023.02.16 �����O�[�Ǐ����ύX_E
      // �wB040�x
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F (S3)";
      } else {
        s_Text = "���O  �_�� (S3)";
      }
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   // 2022.04.21 CX-01D�ǉ�_E

   } else if (s_Text == 1 || s_Text == 2){
   //if (s_Text == 1 || s_Text == 2){
   //********************
      // 2016.08.26 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if(  s_Text.SubString(1,2) == "TP") {     // �`�^���̏ꍇ�͎d��T
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      ////2004.01.28 ASME
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // �`�^���̏ꍇ�͎d��T
      //  // 2010.06.07 ����ύX
      //  //s_Text = "�|�|�|�|�|";
      //  s_Text = "�\�\�\�\�\�\";
      //  //********************
      ////***************
      //} else {
      //  // �wB040�x
      //  s_Text = "���O  �_��";
      //}
      // 2017.05.12 �ގ����ޕύX
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.23 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.23 Ư�ْǉ�_E
      } else {
        s_Text = "���O  �_��";
        //2019.07.25 ����w���������ύX_S
        if ( P_FL == 1 ) {
           // �΍s���ł�S2,S3(A��)�ɒ[�ǎ�t
           s_Text = s_Text + " (S2,S3)";
        }
        //2019.07.25 ����w���������ύX_E
      }
      // ***********************
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   } else if (s_Text == 3){
      // �wB040�x
      //2019.10.28 �����O�[�ǎd��ύX_S
      //s_Text = "�O�ʂ̂��ڰтƓ���F";
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "ALBC" ) { // �`�k�a�b�̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_S
      } else if ( s_Text == "NI" ) {   // �j�b�P���̏ꍇ�͎d��T
        s_Text = "�\�\�\�\�\�\";
      // 2021.03.10 Ư�ْǉ�_E
      } else if ( s_Text == "-" ) {    // �Ȃ�
        s_Text = "�\�\�\�\�\�\";
      } else if ( s_Text == "STPG" ) { // �r�s�o�f
        s_Text = "�O�ʂ̂��ڰтƓ���F";
      } else {
        s_Text = "���O  �_��";
      }
      //2019.10.10 �����O�[�ǎd��ύX_E
      //2019.07.25 ����w���������ύX_S
      if ( P_FL == 1 ) {
         // �΍s���ł�S2,S3(A��)�ɒ[�ǎ�t
         s_Text = s_Text + " (S2,S3)";
      }
      //2019.07.25 ����w���������ύX_E
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   } else {
      // �wB040�x
      i_RecNo = 919;   // S2ɽ�َd��
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text = Get_SiageMeisyouA3(s_Text);
      //2019.07.25 ����w���������ύX_S
      if ( P_FL == 1 ) {
         // �΍s���ł�S2,S3(A��)�Ƀm�Y����t
         if (s_Text != "" && s_Text != "�\�\�\�\�\�\"){
            s_Text = s_Text + " (S2,S3)";
         }
      }
      //2019.07.25 ����w���������ύX_E
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A���d�㖼��          �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  A���ی슯�ގ�
   //***************************
   // �wB041�x
   //2019.07.25 ����w���������ύX_S
   //i_RecNo = 1357;   // A���ی슯�ގ�
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //s_Text = Get_Zaisitu_NameA3(s_Text);
   //Excel_Cell_DataSet(25, 7, s_Text);
   //G_Log_Text = "A���ی슯�ގ�          �w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   i_RecNo = 141;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // A���x�v����

   // 2020.10.30 ���x�v�ی�ǏC��_S
   s_S1OndoCode = s_HEADS_DATA[898].TrimRight();  // S1���x�v���
   s_S2OndoCode = s_HEADS_DATA[913].TrimRight();  // S2���x�v���
   s_S3OndoCode = s_HEADS_DATA[928].TrimRight();  // S3���x�v���
   s_S4OndoCode = s_HEADS_DATA[943].TrimRight();  // S4���x�v���

   if ( P_FL == 1 ) {
      // �΍s���ł�S2,S3��A��
      if (s_S2OndoCode.ToIntDef(0) == 0 && s_S3OndoCode.ToIntDef(0) == 0){
         s_Text = "0";  // A���ɉ��x�v�͕t���Ȃ�
      }
   } else {
      // �ʏ�� S2,S4��A��
      if (s_S2OndoCode.ToIntDef(0) == 0 && s_S4OndoCode.ToIntDef(0) == 0){
         s_Text = "0";  // A���ɉ��x�v�͕t���Ȃ�
      }
   }
   // 2020.10.30 ���x�v�ی�ǏC��_E

   if (s_Text.ToIntDef(0) != 0){
      if (Type_MAT(s_HEADS_DATA[1358].TrimRight()) != "-") {
         s_Text = s_HEADS_DATA[1358].SubString(1,4);
         s_Text = Get_Zaisitu_NameA3(s_Text);
         //2019.10.10 ���x�v�ی�ǐ��@�ǉ�_S
         //if ( P_FL == 1 ) {
         //   // �΍s���ł�S2,S3(A��)�ɕی�ǎ�t
         //   s_Text = s_Text + " (S2,S3)";
         //}
         //2019.10.10 ���x�v�ی�ǐ��@�ǉ�_E
      } else {
         s_Text = "";
      }

      //2019.10.10 ���x�v�ی�ǐ��@�ǉ�_S
      s_Text2 = s_HEADS_DATA[913].TrimRight();  // S2���x�v���
      s_Text3 = s_HEADS_DATA[908].TrimRight();  // S2�m�Y���K�i�R�[�h
      s_Text4 = s_HEADS_DATA[907].TrimRight();  // S2�m�Y�����a
      s_Text2 = Get_Well_Temp(s_Text2, s_Text3, s_Text4);

      if (s_Text2 == "") {
         if ( P_FL == 1 ) {
            // �΍s��
            s_Text2 = s_HEADS_DATA[928].TrimRight();  // S3���x�v���
            s_Text3 = s_HEADS_DATA[923].TrimRight();  // S3�m�Y���K�i�R�[�h
            s_Text4 = s_HEADS_DATA[922].TrimRight();  // S3�m�Y�����a
            s_Text2 = Get_Well_Temp(s_Text2, s_Text3, s_Text4);
         } else {
            s_Text2 = s_HEADS_DATA[943].TrimRight();  // S4���x�v���
            s_Text3 = s_HEADS_DATA[938].TrimRight();  // S4�m�Y���K�i�R�[�h
            s_Text4 = s_HEADS_DATA[937].TrimRight();  // S4�m�Y�����a
            s_Text2 = Get_Well_Temp(s_Text, s_Text3, s_Text4);
         }
      }

      s_Text = s_Text + s_Text2;

      if ( P_FL == 1 ) {
         // �΍s���ł�S2,S3(A��)�ɕی�ǎ�t
         s_Text = s_Text + " (S2,S3)";
      }
      //2019.10.10 ���x�v�ی�ǐ��@�ǉ�_E

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(25, 15, s_Text);
   G_Log_Text = "A�����x�v�ی�Ǎގ�  �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //2019.07.25 ����w���������ύX_E

   //***************************
   //***  B���ی슯�ގ�
   //***************************
   // �wB042�x
   //2019.07.25 ����w���������ύX_S
   //i_RecNo = 1358;   // B���ی슯�ގ�
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //s_Text = Get_Zaisitu_NameA3(s_Text);
   //Excel_Cell_DataSet(25, 15, s_Text);
   //G_Log_Text = "B���ی슯�ގ�          �w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   i_RecNo = 140;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // B���x�v����

   // 2020.10.30 ���x�v�ی�ǏC��_S
   if ( P_FL == 1 ) {
      // �΍s���ł�S1,S4��B��
      if (s_S1OndoCode.ToIntDef(0) == 0 && s_S4OndoCode.ToIntDef(0) == 0){
         s_Text = "0";  // B���ɉ��x�v�͕t���Ȃ�
      }
   } else {
      // �ʏ�� S1,S3��B��
      if (s_S1OndoCode.ToIntDef(0) == 0 && s_S3OndoCode.ToIntDef(0) == 0){
         s_Text = "0";  // B���ɉ��x�v�͕t���Ȃ�
      }
   }
   // 2020.10.30 ���x�v�ی�ǏC��_E

   if (s_Text.ToIntDef(0) != 0){
      if (Type_MAT(s_HEADS_DATA[1357].TrimRight()) != "-") {
         s_Text = s_HEADS_DATA[1357].SubString(1,4);
         s_Text = Get_Zaisitu_NameA3(s_Text);
         //2019.10.10 ���x�v�ی�ǐ��@�ǉ�_S
         //if ( P_FL == 1 ) {
         //   // �΍s���ł�S1,S4(B��)�Ƀt�����W��t
         //   s_Text = s_Text + " (S1,S4)";
         //}
         //2019.10.10 ���x�v�ی�ǐ��@�ǉ�_E
      } else {
         s_Text = "";
      }

      //2019.10.10 ���x�v�ی�ǐ��@�ǉ�_S
      // B���ی슯���@
      s_Text2 = s_HEADS_DATA[898].TrimRight();  // S1���x�v���
      s_Text3 = s_HEADS_DATA[893].TrimRight();  // S1�m�Y���K�i�R�[�h
      s_Text4 = s_HEADS_DATA[892].TrimRight();  // S1�m�Y�����a
      s_Text2 = Get_Well_Temp(s_Text2, s_Text3, s_Text4);
      if (s_Text2 == "") {
         if ( P_FL == 1 ) {
            // �΍s��
            s_Text2 = s_HEADS_DATA[943].TrimRight();      // S4���x�v���
            s_Text3 = s_HEADS_DATA[938].TrimRight();  // S4�m�Y���K�i�R�[�h
            s_Text4 = s_HEADS_DATA[937].TrimRight();  // S4�m�Y�����a
            s_Text2 = Get_Well_Temp(s_Text, s_Text3, s_Text4);
         } else {
            // ��`��
            s_Text2 = s_HEADS_DATA[928].TrimRight();      // S3���x�v���
            s_Text3 = s_HEADS_DATA[923].TrimRight();  // S3�m�Y���K�i�R�[�h
            s_Text4 = s_HEADS_DATA[922].TrimRight();  // S3�m�Y�����a
            s_Text2 = Get_Well_Temp(s_Text2, s_Text3, s_Text4);
         }
      }

      s_Text = s_Text + s_Text2;

      if ( P_FL == 1 ) {
         // �΍s���ł�S1,S4(B��)�Ƀt�����W��t
         s_Text = s_Text + " (S1,S4)";
      }
      //2019.10.10 ���x�v�ی�ǐ��@�ǉ�_E

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(25, 7, s_Text);
   G_Log_Text = "B�����x�v�ی�Ǎގ�  �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //2019.07.25 ����w���������ύX_E

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          �b���@�@�f�[�^�Z�b�g                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  ���t���čގ�
   //***************************
   // �wC001�x
   i_RecNo = 1041;   // �ގ��w��
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   // 2020.06.19 ���a���t���Ēǉ�_S
   s_Text1 = G_KS_Syurui + "�|" + G_KS_Model;
   // 2020.06.19 ���a���t���Ēǉ�_E

   // 2017.06.09 �C�O�����t����
   if ( s_Text == "1186" ) {
      // SNB7������
      i_RecNo = 1019;   // BDI
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2 == "2.000" || s_Text2 == "36.000" || s_Text2 == "52.000"){
         // ���Ča2�h�AM36�AM52
         i_RecNo = 438;   // �ƊE����
         s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
         if (s_Text2 == "0019"){
            // �ƊE����19
            s_Text = "SA-193-GrB7/194�C�O��";
            i_RecNo = 29;   // �K�i����
            if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 6 ) {
                // �K�i���ނ�06:�`�r�l�d
                s_Text = "SA-193-GrB7/194";
            }
            if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 31 ) {
                // �K�i���ނ�31:���q��
                s_Text = "SA-193-GrB7/194";
            }
         } else {
            s_Text  = Search_HD_MATERIAL(s_Text);
         }
      } else {
         s_Text  = Search_HD_MATERIAL(s_Text);
      }
   } else if ( s_Text == "32C2" ) {
      // SA-193-GrB7
      i_RecNo = 1019;   // BDI
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2 == "2.000" || s_Text2 == "36.000" || s_Text2 == "52.000"){
         // ���Ča2�h�AM36�AM52
         i_RecNo = 438;   // �ƊE����
         s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
         if (s_Text2 == "0059" || s_Text2 == "0060"){
            // �ƊE����59,60
            s_Text = "SA-193-GrB7/194�C�O��";
            i_RecNo = 29;   // �K�i����
            if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 6 ) {
                // �K�i���ނ�06:�`�r�l�d
                s_Text = "SA-193-GrB7/194";
            }
            if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 31 ) {
                // �K�i���ނ�31:���q��
                s_Text = "SA-193-GrB7/194";
            }
         } else if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 13 ) {
             // �K�i���ނ�13:�`�a�r
             s_Text = "SA-193-GrB7/194�C�O��";
         } else {
            s_Text  = Search_HD_MATERIAL(s_Text);
         }
      } else {
         s_Text  = Search_HD_MATERIAL(s_Text);
      }
      // *************************
      // 2020.06.19 ���a���t���Ēǉ�_S
   } else if (( s_Text == "A157" ) && ( s_Text1 == "�k�w�|�R�O")) {
      // Q235B2: S/L Q235B ���a 45/35 CODE A157
         s_Text = "Q235B";                         // ���t�{���g�ގ�
   } else if (( s_Text == "A163" ) && ( s_Text1 == "�k�w�|�T�O")) {
      // 45_2  : S/L 45/35 ���a Q235B CODE A163
         s_Text = "45/35";                            // ���t�{���g�ގ�
   // 2020.06.19 ���a���t���Ēǉ�_E
   //if (( s_Text != "" ) && ( s_Text != "0000" ) ){
   } else if (( s_Text != "" ) && ( s_Text != "0000" ) ){
      s_Text  = Search_HD_MATERIAL(s_Text);
   } else {
      i_RecNo = 1021;   // BOL22
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }

   // 2020.06.19 ���a���t���Ēǉ�_S
   if (( s_Text == "Q235B2" ) && ( s_Text1 == "�k�w�|�R�O")) {
      // Q235B2: S/L Q235B ���a 45/35 CODE A157
      s_Text = "Q235B";                            // ���t�{���g�ގ�
   } else if (( s_Text == "45_2" ) && ( s_Text1 == "�k�w�|�T�O")) {
      // 45_2  : S/L 45/35 ���a Q235B CODE A163
      s_Text = "45/35";                               // ���t�{���g�ގ�
   }
   // 2020.06.19 ���a���t���Ēǉ�_E

   // 2012.01.20 �����ގ�
   i_RecNo = 1107;
   s_ChinaM = s_HEADS_DATA[i_RecNo].TrimRight();

   s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
   if ( s_Text2 == "�t�w�|�P�O" ) {                            // �����ގ�
   } else if ( s_Text2 == "�k�w�|�O�O" ) {                     // �����ގ�
   } else if ( s_Text2 == "�k�w�|�P�O" ) {                     // �����ގ�
   } else if ( s_Text2 == "�k�w�|�R�O" ) {                     // �����ގ�
   } else if ( s_Text2 == "�k�w�|�T�O" ) {                     // �����ގ�
   } else if ( s_Text2 == "�r�w�|�Q�O" ) {                     // �����ގ�
   } else if ( s_Text2 == "�r�w�|�S�P" ) {                     // �����ގ�
   } else if ( s_Text2 == "�r�w�|�S�R" ) {                     // �����ގ�
   } else if ( s_Text2 == "�q�w�|�P�P" ) {                     // �����ގ�
   } else if ( s_Text2 == "�q�w�|�P�R" ) {                     // �����ގ�
   } else if ( s_Text2 == "�q�w�|�R�O" ) {                     // �����ގ�
   } else if ( s_Text2 == "�q�w�|�T�O" ) {                     // �����ގ�
   } else if ( s_Text2 == "�q�w�|�V�O" ) {                     // �����ގ�
   } else {
       s_ChinaM = "";
   }

   if ( s_ChinaM == "CHINA" ) {                                // �����ގ�
       if ( s_Text == "SS400" ) {
           s_Text = "Q235B";
       }
   }
   // *******************

   // 2020.06.19 ���a���t���Ēǉ�_S
   sBltMat = s_Text; // ���t�{���g�ގ�
   // 2020.06.19 ���a���t���Ēǉ�_E

   //2009.04.27 �ټ�Ē��L
   i_RecNo = 435;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      s_Text = s_Text + " ��";
   }
   //********************

   Excel_Cell_DataSet(27, 8, s_Text);
   G_Log_Text = "���t���� �ގ�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���t���� �a
   //***************************
   // ���Ė{����0�Ȃ�o�͂��Ȃ�
   // �wC002�x
   i_RecNo = 1286;   // ���Ė{��L
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) != 0){
      i_RecNo = 1019;   // BDI
      s_Text = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
      if (s_Text == "42"){
         //2004.01.22
         //s_Text = "1 5/8-8UN";
         s_Text = "1 5/8""-8UN";
         //**********
      } else if (s_Text == "52"){
         //2004.01.22
         //s_Text = "2-8UN";
         s_Text = "2""-8UN";
         //**********
      } else {
         s_Text = "M" + s_Text;
      }

      // 2017.05.29 ���t���Ča�����C��
      i_RecNo = 1019;   // BDI
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2 == "1.630" ){
         // 1 5/8-8UN
         s_Text = "1 5/8""-8UN";
      } else if (s_Text2 == "2.000" ){
         // 2-8UN
         s_Text = "2""-8UN";
      }
      // *****************************

      s_Text = s_Text + "�~" + s_HEADS_DATA[1376].TrimRight() + "L";   //�Q�ƃf�[�^�ύX 02/10/24 Miya

      //2019.07.25 ����w���������ύX_S
      if (P_HEADS_DATA[36].Pos("M") != 0){
         s_Text =  s_Text + "  �v�i�b�g";
      }
      //2019.07.25 ����w���������ύX_E

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(27, 12, s_Text);
   G_Log_Text = "���t���� �a          �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���t���� �{��L
   //***************************
   // ���Ė{����0�Ȃ�o�͂��Ȃ�
   // �wC003�x
   i_RecNo = 1286;   // ���Ė{��L
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) == 0){
      s_Text = "";
   }
   Excel_Cell_DataSet(27, 24, s_Text);
   G_Log_Text = "���t���� �{���k      �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���t���� ���Ďd��
   //***************************
   // �wC004�x
   i_RecNo = 1042;   // SPBLF
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "01") {
      s_Text = "�L�F��Ұ�";
   } else if (s_Text == "02"){
      s_Text = "MFZn-�U";
   } else if (s_Text == "03"){
      s_Text = "�԰ײ�";
   //2009.06.03 �O����Ұ�ү��ǉ�
   } else if (s_Text == "04"){
      s_Text = "�O����Ұ�";
   //***************************
   //2013.11.08 ����Ēǉ�
   } else if (s_Text == "05"){
      s_Text = "�^�P�R�[�g";
   //********************
   //2018.08.20 C3604 ���ү�ůĒǉ�_S
   } else if (s_Text == "06"){
      s_Text = "C3604 ���ү�ů�";
   //2018.08.20 C3604 ���ү�ůĒǉ�_E
   //2020.10.02 ů�,ܯ��ү��d��ǉ�_S
   } else if (s_Text == "07"){
      i_RecNo = 263;   // ���Ķ�ް�L��
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text == "1" || s_Text == "1.00") {
         s_Text = "��ؽ����(ů�,ܯ��ү��d��)";
      } else {
         s_Text = "���t���Ă�ů�,ܯ��ү��d��";
      }
   } else if (s_Text == "08"){
      i_RecNo = 263;   // ���Ķ�ް�L��
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text == "1" || s_Text == "1.00") {
         s_Text = "��ؽ����(ů�ү��d��)";
      } else {
         s_Text = "���t���Ă�ů�ү��d��";
      }
   //2020.10.02 ů�,ܯ��ү��d��ǉ�_E
   } else {
      s_Text = "";
      //2020.10.02 ů�,ܯ��ү��d��ǉ�_S
      i_RecNo = 263;   // ���Ķ�ް�L��
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text == "1" || s_Text == "1.00") {
         s_Text = "��ؽ����";
         //2022.11.15 SUS�{���g�C��_S
         i_RecNo = 1041;   // �ގ��w��
         s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
         if (s_Text2 == "31C9" || s_Text2 == "3101" || s_Text2 == "1101" ) {
            s_Text = "";
         }
         //2022.11.15 SUS�{���g�C��_E
      }
      //2020.10.02 ů�,ܯ��ү��d��ǉ�_E
      //2020.10.16 ů�,ܯ��ү��d��C��_S
      else {
         s_Text = "";
      }
      //2020.10.16 ů�,ܯ��ү��d��C��_E
   }

   Excel_Cell_DataSet(28, 7, s_Text);
   G_Log_Text = "���t���� ���Ďd��     RecNo:" + FormatFloat("0000",i_RecNo) + "���w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���t���� BLS
   //***************************
   // ���Ė{����0�Ȃ�o�͂��Ȃ�
   // �wC005�x
   i_RecNo = 1287;   // ���Ė{��S
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) != 0){
      i_RecNo = 1019;   // BDI
      s_Text = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
      if (s_Text == "42"){
         //2004.01.24
         //s_Text = "1 5/8-8UN";
         s_Text = "1 5/8""-8UN";
         //**********
      } else if (s_Text == "52"){
         //2004.01.24
         //s_Text = "2-8UN";
         s_Text = "2""-8UN";
         //**********
      } else {
         s_Text = "M" + s_Text;
      }

      // 2017.05.29 ���t���Ča�����C��
      i_RecNo = 1019;   // BDI
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2 == "1.630" ){
         // 1 5/8-8UN
         s_Text = "1 5/8""-8UN";
      } else if (s_Text2 == "2.000" ){
         // 2-8UN
         s_Text = "2""-8UN";
      }
      // *****************************

      s_Text = s_Text + "�~" + s_HEADS_DATA[1377].TrimRight() + "L";   //�Q�ƃf�[�^�ύX 02/10/24 Miya

      //2019.07.25 ����w���������ύX_S
      if (P_HEADS_DATA[36].Pos("M") != 0){
         s_Text =  s_Text + "  �v�i�b�g";
      }
      //2019.07.25 ����w���������ύX_E

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(28, 12, s_Text);
   G_Log_Text = "���t���� ���Ďd��    �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���t���� �{��S
   //***************************
   // �wC006�x
   i_RecNo = 1287;   // S���Ė{��
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) == 0){
      s_Text = "";
   }
   Excel_Cell_DataSet(28, 24, s_Text);
   G_Log_Text = "���t���� �{���r       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2014.05.28 �p��K�i�\�L�ύX
   //***************************
   //***  S�ڰэE��
   //***************************

   AnsiString s_S1Nzl_Syurui;                         // S1ɽ�َ��
   AnsiString s_S1Nzl_Tugite;                         // S1ɽ�ٌp��
   AnsiString s_S2Nzl_Syurui;                         // S2ɽ�َ��
   AnsiString s_S2Nzl_Tugite;                         // S2ɽ�ٌp��
   AnsiString s_S3Nzl_Syurui;                         // S3ɽ�َ��
   AnsiString s_S3Nzl_Tugite;                         // S3ɽ�ٌp��
   AnsiString s_S4Nzl_Syurui;                         // S4ɽ�َ��
   AnsiString s_S4Nzl_Tugite;                         // S4ɽ�ٌp��

   AnsiString s_S[5];
   int i_OgataFlg;               // 0:���^���^,1:��^,2:E���

   s_S1Nzl_Syurui = s_HEADS_DATA[891].TrimRight();    // S1ɽ�َ��
   s_S1Nzl_Tugite = s_HEADS_DATA[893].TrimRight();    // S1ɽ�ٌp��
   s_S2Nzl_Syurui = s_HEADS_DATA[906].TrimRight();    // S2ɽ�َ��
   s_S2Nzl_Tugite = s_HEADS_DATA[908].TrimRight();    // S2ɽ�ٌp��
   s_S3Nzl_Syurui = s_HEADS_DATA[921].TrimRight();    // S3ɽ�َ��
   s_S3Nzl_Tugite = s_HEADS_DATA[923].TrimRight();    // S3ɽ�ٌp��
   s_S4Nzl_Syurui = s_HEADS_DATA[936].TrimRight();    // S4ɽ�َ��
   s_S4Nzl_Tugite = s_HEADS_DATA[938].TrimRight();    // S4ɽ�ٌp��

   s_Text = G_KS_Syurui + "�|" + G_KS_Model;

   if (s_Text == "�t�w�|�W�O" || s_Text == "�t�w�|�X�O" || s_Text == "�t�w�|�P�O�O" ||
       s_Text == "�t�w�|�P�O�O�q" || s_Text == "�t�w�|�P�R�O" || s_Text == "�r�w�|�V�O" ||
       s_Text == "�r�w�|�W�O�r" || s_Text == "�r�w�|�X�O" || s_Text == "�r�w�|�X�O�l" ||
       s_Text == "�r�w�|�X�O�k" || s_Text == "�q�w�|�V�O" || s_Text == "�q�w�|�X�O" ||
       s_Text == "�q�w�|�P�O�O" || s_Text == "�v�w�|�X�O" || s_Text == "�b�w�|�P�O" ||
       s_Text == "�b�w�|�P�O�c" || s_Text == "�m�w�|�X�O"){
           i_OgataFlg = 1;
   //2019.07.25 ����w���������ύX_S
   } else if (s_Text == "�k�w�|�T�O" || s_Text == "�k�w�|�T�O�r"){
           i_OgataFlg = 1;
   //2019.07.25 ����w���������ύX_E
   } else if (s_Text == "�t�w�|�P�O" || s_Text == "�t�w�|�Q�O" ||
       s_Text == "�t�w�|�R�O" || s_Text == "�k�w�|�Q�O" ||
       s_Text == "�r�w�|�S�O" || s_Text == "�r�w�|�S�P" ||
       s_Text == "�r�w�|�S�R" || s_Text == "�d�w�|�P�T"){
           i_OgataFlg = 2;
   } else {
           i_OgataFlg = 0;
   }

   //*************************
   //***  S1�@����
   //*************************
   if (s_S1Nzl_Syurui == "0"){
       s_S[1] = "0";
   } else {
       s_Text = s_S1Nzl_Tugite.SubString(3,1);

       if (s_Text == "1"){
           if (i_OgataFlg == 1){
               s_S[1] = "N";
           } else {
               s_S[1] = "K";
           }
       } else if (s_Text == "2"){
           s_S[1] = "T";
       } else if (s_Text == "3"){
           s_S[1] = "K";
       } else if (s_Text == "4"){
           s_S[1] = "1";
       } else {
           G_Log_Text = "S1ɽ�ٌp��w" + s_S1Nzl_Tugite + "�x�ɂ��AS1�E���ް����ُ�ł��B";
           Write_Error_Log(G_Log_Text);
           s_S[1] = "*";
       }
   }

   //*************************
   //***  S2�@����
   //*************************
   if (s_S2Nzl_Syurui == "0"){
       s_S[2] = "0";
   } else {
       s_Text = s_S2Nzl_Tugite.SubString(3,1);

       if (s_Text == "1"){
           if (i_OgataFlg == 1){
               s_S[2] = "N";
           } else {
               s_S[2] = "K";
           }
       } else if (s_Text == "2"){
           s_S[2] = "T";
       } else if (s_Text == "3"){
           s_S[2] = "K";
       } else if (s_Text == "4"){
           s_S[2] = "2";
       } else {
           G_Log_Text = "S2ɽ�ٌp��w" + s_S2Nzl_Tugite + "�x�ɂ��AS2�E���ް����ُ�ł��B";
           Write_Error_Log(G_Log_Text);
           s_S[2] = "*";
       }
   }

   //*************************
   //***  S3�@����
   //*************************
   if (s_S3Nzl_Syurui == "0"){
       s_S[3] = "0";
   } else {
       s_Text = s_S3Nzl_Tugite.SubString(3,1);

       if (s_Text == "1"){
           if (i_OgataFlg == 1){
               s_S[3] = "N";
           } else {
               s_S[3] = "K";
           }
       } else if (s_Text == "2"){
           s_S[3] = "T";
       } else if (s_Text == "3"){
           s_S[3] = "K";
       } else if (s_Text == "4"){
           s_S[3] = "3";
       } else {
           G_Log_Text = "S3ɽ�ٌp��w" + s_S3Nzl_Tugite + "�x�ɂ��AS3�E���ް����ُ�ł��B";
           Write_Error_Log(G_Log_Text);
           s_S[3] = "*";
       }
   }

   //*************************
   //***  S4�@����
   //*************************
   if (s_S4Nzl_Syurui == "0"){
       s_S[4] = "0";
   } else {
       s_Text = s_S4Nzl_Tugite.SubString(3,1);

       if (s_Text == "1"){
           if (i_OgataFlg == 1){
               s_S[4] = "N";
           } else {
               s_S[4] = "K";
           }
       } else if (s_Text == "2"){
           s_S[4] = "T";
       } else if (s_Text == "3"){
           s_S[4] = "K";
       } else if (s_Text == "4"){
           s_S[4] = "4";
       } else {
           G_Log_Text = "S4ɽ�ٌp��w" + s_S4Nzl_Tugite + "�x�ɂ��AS4�E���ް����ُ�ł��B";
           Write_Error_Log(G_Log_Text);
           s_S[4] = "*";
       }
   }

   //***************************
   //***  S�ڰэE��
   //***************************
   // �wC029�x
   // �e�擾���ڌ���
   s_Text = "S" + s_S[1] + s_S[2] + s_S[3] + s_S[4];

   Excel_Cell_DataSet(33, 1, s_Text);
   G_Log_Text = "S�ڰэE��             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //***************************

   //***************************
   //***  �ڰэގ� �ގ�
   //***************************
   // �wC007�x 02.09.20�ύX N.UCHIDA     Get_Zaisitu_Name

   switch (s_HEADS_DATA[37].ToIntDef(0)) {
      case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
         s_Text = s_HEADS_DATA[34].TrimRight();
         //2008.07.24 RX-10B�ǉ�
         //if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX"){
         if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX" || s_Text.SubString(1,2) == "RX"){
         //*********************
            s_Text = "�r�t�r�R�O�S";

         } else {
            s_Text = "�r�r�S�O�O";
         }

         break;
      default :
         i_RecNo = 1044;   // SPFRM
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

         if (s_Text == "0000" || s_Text == "1040"){
            s_Text = "�r�r�S�O�O";
         } else {
            i_RecNo = 1044;   // SPFRM
            s_Text = Get_Zaisitu_NameA3(s_HEADS_DATA[i_RecNo].TrimRight());
         }
         break;
   }

   // 2012.01.20 �����ގ�
   if ( s_ChinaM == "CHINA" ) {                                // �����ގ�
       if ( s_Text == "�r�r�S�O�O" ) {
           s_Text = "�p�Q�R�T�a";
       }
   }
   // *******************

   //2009.04.27 �ټ�Ē��L
   i_RecNo = 434;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      s_Text = s_Text + " ��";
   }
   //********************

   Excel_Cell_DataSet(32, 8, s_Text);
   G_Log_Text = "�ڰэގ� �ގ�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ڰэގ� ����
   //***************************
   // �wC008�x
   i_RecNo = 1284;   // �ڰь�
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(32, 15, s_Text);
   G_Log_Text = "�ڰэގ� ����         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ڰэގ� ��
   //***************************
   // �wC009�x
   i_RecNo = 1285;   // �ڰѕ�
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(32, 22, s_Text);
   G_Log_Text = "�ڰэގ� ��           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2013/07/30 GB�\�L����
   ////***************************
   ////***  �㕔LU1
   ////***************************
   //// �wC010�x
   //i_RecNo = 1022;   // RLU1
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(30, 10, s_Text);
   //G_Log_Text = "�㕔�k�t�P            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   //
   ////***************************
   ////***  �㕔LU2
   ////***************************
   //// �wC011�x
   //i_RecNo = 1023;   // RLU2
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(30, 12, s_Text);
   //G_Log_Text = "�㕔�k�t�Q            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   //
   ////***************************
   ////***  �㕔LU5
   ////***************************
   //// �wC011�x
   //i_RecNo = 1378;   // RLU5
   //if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) != 0 ) {
   //  s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //  Excel_Cell_DataSet(30, 12, s_Text);
   //  Excel_Cell_DataSet(30, 11, "LU3 =");
   //  G_Log_Text = "�㕔�k�t�R            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���㏑�Z�b�g�B";
   //  Write_Log(G_Log_Text);
   //}

   //***************************
   //***  �㕔LU
   //***************************
   // �wC010�x
   i_RecNo = 1022;   // RLU1
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   i_RecNo = 1378;   // RLU5
   if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) != 0 ) {
     i_RecNo = 1378;   // RLU5
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(34, 10, s_Text);
   // �wC013�x
     Excel_Cell_DataSet(34, 7, "LU1/3 =");
     G_Log_Text = "�㕔�k�t              RecNo:1022/1378�w" + s_Text + "�x���㏑�Z�b�g�B";
     Write_Log(G_Log_Text);
   } else {
     i_RecNo = 1023;   // RLU2
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(34, 10, s_Text);
     G_Log_Text = "�㕔�k�t              RecNo:1022/1023�w" + s_Text + "�x���㏑�Z�b�g�B";
     Write_Log(G_Log_Text);
   }

   //***************************
   //***  ����LU
   //***************************
   // �wC011�x
   i_RecNo = 1024;   // RLL1
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   i_RecNo = 1379;   // RLL5
   if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) != 0 ) {
     i_RecNo = 1379;   // RLL5
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(34, 12, s_Text);
   // �wC014�x
     Excel_Cell_DataSet(34, 11, "LL1/3 =");
     G_Log_Text = "�����k�k              RecNo:1024/1379�w" + s_Text + "�x���㏑�Z�b�g�B";
     Write_Log(G_Log_Text);
   } else {
     i_RecNo = 1025;   // RLL2
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(34, 12, s_Text);
     G_Log_Text = "�����k�t              RecNo:1024/1025�w" + s_Text + "�x���㏑�Z�b�g�B";
     Write_Log(G_Log_Text);
   }
   // *********************

   //***************************
   //***  �޲���ް���
   //***************************
   // �wC012�x
   i_RecNo = 1290;   // �޲���ް���
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(34, 16, s_Text);
   G_Log_Text = "�޲���ް���          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �b
   //***************************
   // �wC015�x
   i_RecNo = 1026;   // CMAX
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 18, s_Text);
   G_Log_Text = "�b                    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �o
   //***************************
   // �wC016�x
   i_RecNo = 1028;   // P
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 24, s_Text);
   G_Log_Text = "�o                    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���[���Z���^�[
   //***************************
   // �wC017�x
   s_Text2 = G_KS_Syurui + "�|" + G_KS_Model;
   if ( s_Text2 == "�k�w�|�R�O" ) {                            // �k�w�|�R�O
       s_Text = "�Q�����ȓ�";
   } else {                                                    // ���̑��^��
       s_Text = "�R�����ȓ�";
   }
   Excel_Cell_DataSet(35, 7, s_Text);
   G_Log_Text = "���[���Z���^�[        �w" + s_Text + "�x���㏑�Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �{���g�J�o�[
   //***************************
   // �wC018�x  2003/06/04 ������HEADS�e�L�X�g�̓��e�𖳏����ŏo��
   i_RecNo = 1367;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(29, 7, s_Text);
   G_Log_Text = "�{���g�J�o�[         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //**********************************************************
   //***  ���Ė{��L,S�@���l����
   //***    �wC019�x,�wC020�x,�wC021�x�ׂ̈̏����O����
   //**********************************************************
   i_ErrFlg = 0;

   // ���Ė{��L
   i_RecNo = 1286;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      i_Value_1 = s_Text.ToIntDef(0);   // ���Ė{��L
   } else {
      G_ErrLog_Text = "���Ė{���k      RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      i_ErrFlg = 1;
   }

   // ���Ė{��S
   i_RecNo = 1287;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      i_Value_2 = s_Text.ToIntDef(0);   // ���Ė{��S
   } else {
      G_ErrLog_Text = "���Ė{���r      RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      i_ErrFlg = 1;
   }

   //***************************
   //***  BLCP1
   //***************************
   // �wC019�x,�wC020�x
   if (i_ErrFlg == 0){
      i_RecNo = 1092;      // BLCP1
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         // BLCP1 �wC019�x
         s_Text = s_Text + "L ";
         Excel_Cell_DataSet(29, 10, s_Text);
         G_Log_Text = "BLCP1                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

         // BLCP1�{�� �wC020�x
         s_Text = AnsiString(i_Value_1 + i_Value_2);
         Excel_Cell_DataSet(29, 24, s_Text);
         G_Log_Text = "BLCP1 �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

      } else {
         s_Text = "";
         Excel_Cell_DataSet(29, 10, s_Text);
         Excel_Cell_DataSet(29, 24, s_Text);
         G_Log_Text = "BLCP1                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
         G_Log_Text = "BLCP1 �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      }
   } else {
      s_Text = "";
      Excel_Cell_DataSet(29, 10, s_Text);
      Excel_Cell_DataSet(29, 24, s_Text);
      G_Log_Text = "BLCP1                �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
      G_Log_Text = "BLCP1 �{��           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  BLCEL
   //***************************
   // �wC021�x,�wC022�x
   if (i_ErrFlg == 0){
      i_RecNo = 1096;      // BLCEL
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         // BLCEL �wC021�x
         s_Text = s_Text + "L+C";
         Excel_Cell_DataSet(30, 10, s_Text);
         G_Log_Text = "BLCEL                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

         // BLCEL�{�� �wC022�x
         s_Text = AnsiString(i_Value_1);
         Excel_Cell_DataSet(30, 24, s_Text);
         G_Log_Text = "BLCEL �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

      } else {
         s_Text = "";
         Excel_Cell_DataSet(30, 10, s_Text);
         Excel_Cell_DataSet(30, 24, s_Text);
         G_Log_Text = "BLCEL                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
         G_Log_Text = "BLCEL �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      }
   } else {
      s_Text = "";
      Excel_Cell_DataSet(30, 10, s_Text);
      Excel_Cell_DataSet(30, 24, s_Text);
      G_Log_Text = "BLCEL                �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
      G_Log_Text = "BLCEL �{��           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  BLCES
   //***************************
   // �wC023�x,�wC024�x
   if (i_ErrFlg == 0){
      i_RecNo = 1097;      // BLCES
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      // 2016.08.30 �{���g�J�o�[�o�͏����C��
      //if (s_Text.ToIntDef(0) != 0 ){
      if (s_Text.ToIntDef(0) != 0 && i_Value_2 != 0){
      // ***********************************
         // BLCEL �wC023�x
         s_Text = s_Text + "L+C";
         Excel_Cell_DataSet(31, 10, s_Text);
         G_Log_Text = "BLCES                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

         // BLCEL�{�� �wC024�x
         s_Text = AnsiString(i_Value_2);
         Excel_Cell_DataSet(31, 24, s_Text);
         G_Log_Text = "BLCES �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

      } else {
         s_Text = "";
         Excel_Cell_DataSet(31, 10, s_Text);
         Excel_Cell_DataSet(31, 24, s_Text);
         G_Log_Text = "BLCES                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
         G_Log_Text = "BLCES �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      }
   } else {
      s_Text = "";
      Excel_Cell_DataSet(31, 10, s_Text);
      Excel_Cell_DataSet(31, 24, s_Text);
      G_Log_Text = "BLCES                �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
      G_Log_Text = "BLCES �{��           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  �ݶ����ā@���
   //***************************
   // �wC025�x 02.09.20�ύX N.UCHIDA
   i_RecNo = 1045;   // SPANC
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // 2014.09.22 �ݶ����ޕύX
   //if (s_Text == "00" || s_Text == "01" || s_Text == "06" || s_Text == "07"){
   if (s_Text == "00" || s_Text == "01"){
   // ***********************
      s_Text = "�A���J�[�{���g�E�i�b�g";
   } else if (s_Text == "02") {
      s_Text = "�z�[���C���A���J�[";
   } else if (s_Text == "03") {
      s_Text = "�P�~�J���E�A���J�[";
   } else if (s_Text == "04") {
      s_Text = "�t�V�t���{���g�E�i�b�g";
   } else if (s_Text == "05") {
      s_Text = "�Z�b�g�{���g�E�i�b�g";
   // 2014.09.22 �ݶ����ޕύX
   } else if (s_Text == "06") {
      s_Text = "�A���J�[�{���g�E�i�b�g�i�i�`�^�j";
   } else if (s_Text == "07") {
      s_Text = "�A���J�[�{���g�E�i�b�g�i�i�^�j";
   // ***********************
   } else if (s_Text == "09") {
      s_Text = "�A���J�[�{���g�s�v";
   // 2021.04.07 �_�u���i�b�g�ǉ�_S
   } else if (s_Text == "11") {
      s_Text = "�A���J�[�{���g�E�_�u���i�b�g�i�P��~�P�A�R��~�P�j";
   } else if (s_Text == "13") {
      s_Text = "�P�~�J���E�A���J�[�E�_�u���i�b�g�i�P��~�P�A�R��~�P�j";
   } else if (s_Text == "15") {
      s_Text = "�Z�b�g�{���g�E�_�u���i�b�g�i�P��~�P�A�R��~�P�j";
   // 2021.04.07 �_�u���i�b�g�ǉ�_E
   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(36, 8, s_Text);
   G_Log_Text = "�ݶ����ĺ���          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   if ( s_Text == "�A���J�[�{���g�s�v" || s_Text == "" ) {
      Excel_Cell_DataSet(37, 8, "");
      Excel_Cell_DataSet(37, 15, "");
      Excel_Cell_DataSet(37, 23, "");
   } else {
      //***************************
      //***  �ݶ����ā@����
      //***************************
      // �wC026�x
      // ��ʺ��ނ̎擾
      s_Text = s_HEADS_DATA[1045].TrimRight();  // SPANC
      if (s_Text == "00" || s_Text == "01" || s_Text == "06" || s_Text == "07"){
         s_Text = s_HEADS_DATA[264].TrimRight();  // ������ݗL��
         if (s_Text == "1.00"){
            s_Syubetu = "1";
         } else {
            s_Syubetu = "0";
         }
      //2003.12.24 ΰٲ݂�SUS�ɂ��敪�̍폜
      //} else if (s_Text == "02") {
      //   s_Text = s_HEADS_DATA[37].TrimRight();   // �ڰь^������
      //   switch (s_Text.ToIntDef(0)){
      //      case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
      //         s_Syubetu = "1";
      //         break;
      //      default:
      //         s_Text = s_HEADS_DATA[1048].TrimRight();  // SPANM
      //         if (s_Text == "1001") {
      //            s_Syubetu = "1";
      //         } else {
      //            s_Syubetu = "0";
      //         }
      //   }
      //*************************************
      } else {
         s_Syubetu = "0";
      }

      // �ݶ����Ē�Ͻ� ����
      G_Log_Text = "";
      s_SQL = "";
      s_SQL = s_SQL + "SELECT * FROM HD_ANCHOR_SIZE";
      // 2012.06.08 �ݶ����Č��������ύX(RX-10-EP)
      //s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "'";
      switch (s_HEADS_DATA[37].ToIntDef(0)) {      // �t���[���R�[�h
         case 71: case 72: case 81: case 91: case 381: case 391:  // �d�o�V���[�Y
            s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
            s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "E'";
            break;
         //2018.08.08 �A���J�[�ύX_S
         case 34: case 35: case 36: case 37: case 38:
         case 44: case 45: case 46: case 47: case 48:
         case 334: case 335: case 336: case 337: case 338:
         case 344: case 345: case 346: case 347: case 348:  // �y�t���[��
              s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
              s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "Z'";
              break;
         //2018.08.08 �A���J�[�ύX_E
         default :
            s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
            s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "'";
            break;
      }
      // *****************************************
      s_SQL = s_SQL + "    AND ANCHOR_STAT = " + s_Syubetu + "";
      s_SQL = s_SQL + "    AND ABCHOR_TYPE = " + s_HEADS_DATA[1045].TrimRight() + ";";

      wkQuery->Close();
      // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
      //wkQuery->DatabaseName = ODBC_DSN_DNO;
      wkQuery->ConnectionName = ODBC_DSN_DNO;
      // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
      wkQuery->SQL->Clear();
      wkQuery->SQL->Add(s_SQL);
      wkQuery->Open();
      wkQuery->First();

      if(wkQuery->Eof){
         s_Text = "";
      } else {
         s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                + "�~" + wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight() + "�k "
                + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
      }

      //2003.12.08 �����w��ύX
      //if (s_HEADS_DATA[1045].TrimRight() != "02" && s_HEADS_DATA[1045].TrimRight() != "03" &&
      //    s_HEADS_DATA[1047].TrimRight() != "0" && s_HEADS_DATA[1047].TrimRight() != "" ) {  // SPANM
      if (s_HEADS_DATA[1047].TrimRight() != "0" && s_HEADS_DATA[1047].TrimRight() != "" ) {  // SPANM
      //***********************
         s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
      }

      //2003.12.24 ΰٲݒ����w��ύX
      if (s_HEADS_DATA[1045].TrimRight() == "02" ) {
         //2004.03.15 ΰٲݒ����ǉ�
         if (s_HEADS_DATA[1047].ToIntDef(0) == 60 ) {   // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "60";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 70 ) {   // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "70";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 80 ) {   // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "80";
         //2004.04.14 ΰٲݒ����ǉ�
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 90 ) {   // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "90";
         //************************
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 100 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "10";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 120 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "12";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 130 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "13";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 150 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "15";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 190 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "19";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 230 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "23";
         } else if (s_HEADS_DATA[1047].TrimRight() != "0" && s_HEADS_DATA[1047].TrimRight() != "" ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "�~" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "�k "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
         } else {
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight();
            if ( s_Text == "M12" ) {
                s_Text = s_Text + "�~" + wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight()
                  + "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() + "80";
            } else {
                s_Text = s_Text + "�~" + wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight()
                  + "�k " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() + "13";
            }
         }
         //************************
      }
      //***********************

      //2015.11.19 �P�~�J���A���J�[�S�ʕύX
      ////2006.02.06 �жٱݶ���BOLT_TYPE�̒����ɂ��ύX�̒ǉ�
      //if ( s_HEADS_DATA[1045].TrimRight() == "03" ) {
      //   if ( wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() == "AP-20" ) {
      //      if ( s_HEADS_DATA[1047].ToIntDef(0) == 0 ) {   // SPANM
      //         s_Text = "AP-20";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 180 ) {   // SPANM
      //         s_Text = "AP-20S";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 250 ) {   // SPANM
      //         s_Text = "AP-20";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 300 ) {   // SPANM
      //         s_Text = "AP-20L";
      //      } else {
      //         s_Text = "";
      //      }
      //   } else if ( wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() == "AP-24" ) {
      //      if ( s_HEADS_DATA[1047].ToIntDef(0) == 0 ) {   // SPANM
      //         s_Text = "AP-24";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 270 ) {   // SPANM
      //         s_Text = "AP-24S";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 375 ) {   // SPANM
      //         s_Text = "AP-24";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 420 ) {   // SPANM
      //         s_Text = "AP-24L";
      //      } else {
      //         s_Text = "";
      //      }
      //   } else if ( wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() == "AP-30" ) {
      //      if ( s_HEADS_DATA[1047].ToIntDef(0) == 0 ) {   // SPANM
      //         s_Text = "AP-30";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 310 ) {   // SPANM
      //         s_Text = "AP-30S";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 430 ) {   // SPANM
      //         s_Text = "AP-30";
      //      } else {
      //         s_Text = "";
      //      }
      //   // 2012.07.03 AP-12�ǉ�
      //   } else if ( wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() == "AP-12" ) {
      //      if ( s_HEADS_DATA[1047].ToIntDef(0) == 0 ) {   // SPANM
      //         s_Text = "AP-12";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 90 ) {    // SPANM
      //         s_Text = "AP-12S";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 130 ) {   // SPANM
      //         s_Text = "AP-12";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 250 ) {   // SPANM
      //         s_Text = "AP-12L";
      //      } else {
      //         s_Text = "";
      //      }
      //   // ********************
      //   } else {
      //      s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
      //   }
      //   if ( s_HEADS_DATA[1047].TrimRight() != "0" && s_HEADS_DATA[1047].TrimRight() != "" ) {
      //      if ( s_Text == "AP-20L"
      //        && s_HEADS_DATA[1047].ToIntDef(0) > 290
      //        && s_HEADS_DATA[1047].ToIntDef(0) <= 295 ) {
      //           s_Text2 = "295";
      //      } else if ( s_Text == "AP-24"
      //        && s_HEADS_DATA[1047].ToIntDef(0) > 370
      //        && s_HEADS_DATA[1047].ToIntDef(0) <= 375 ) {
      //           s_Text2 = "375";
      //      } else {
      //           s_Text2 = FormatFloat("0",ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
      //      }
      //      s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
      //             + "�~" + s_Text2 + "�k "
      //             + s_Text;
      //   } else {
      //      s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
      //             + "�~" + wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight() + "�k "
      //             + s_Text;
      //   }
      //}
      ////****************************************************
      if ( s_HEADS_DATA[1045].TrimRight() == "03" ) {
         s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
         if (s_Text == "HP-12") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 71 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 250) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         } else if (s_Text == "HP-20") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 141 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 350) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         } else if (s_Text == "HP-24") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 371 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 375) {
               s_Text2 = "375";
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 450) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         } else if (s_Text == "HP-30") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 500) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         }

         s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                + "�~" + s_Text2 + "�k "
                + s_Text;
      }
      // **********************************

      // 2021.04.07 �_�u���i�b�g�ǉ�_S
      if ( s_HEADS_DATA[1045].TrimRight() == "13" ) {
         // �P�~�J���A���J�[�E�_�u���i�b�g
         s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
         if (s_Text == "HP-12") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 71 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 250) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         } else if (s_Text == "HP-20") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 141 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 350) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         } else if (s_Text == "HP-24") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 450) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         } else if (s_Text == "HP-30") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 500) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         }

         s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                + "�~" + s_Text2 + "�k "
                + s_Text;
      }
      // 2021.04.07 �_�u���i�b�g�ǉ�_E

      //2018.08.08 �A���J�[�ύX_S
      s_Text2 = wkQuery->FieldByName("NUM")->AsString.TrimRight();
      s_Text = s_Text + " �^ " + s_Text2 + "�{";
      //2018.08.08 �A���J�[�ύX_E

      //2018.08.08 �A���J�[�ύX_S
      //Excel_Cell_DataSet(34, 8, s_Text);
      Excel_Cell_DataSet(37, 8, s_Text);
      //2018.08.08 �A���J�[�ύX_E
      G_Log_Text = "�ݶ����Ē���          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      //***************************
      //***  �ݶ����ā@�ގ�
      //***************************
      // �wC027�x 02.09.20�ύX N.UCHIDA

      s_Text = s_HEADS_DATA[1048].TrimRight();  // SPANM
      if ( s_Text != "" && s_Text != "0000") {
        s_Text = Search_HD_MATERIAL(s_Text);
      } else {
        i_RecNo = 1045;   // SPANC
        s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

        if (s_Text == "00" || s_Text == "01" || s_Text == "03" || s_Text == "05" || s_Text == "06" || s_Text == "07"){
           switch (s_HEADS_DATA[37].ToIntDef(0)) {
              case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
                 s_Text = "�r�t�r�R�O�S";
                 break;
              default :
                 if (s_HEADS_DATA[1048].TrimRight() == "1001"){
                    s_Text = "�r�t�r�R�O�S";
                 // 2016/09/14 �ގ��R�[�h�ύX
                 } else if (s_HEADS_DATA[1048].TrimRight() == "30C9"){
                    s_Text = "�r�r�R�O�S";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "31C9"){
                    s_Text = "�r�t�r�R�O�S";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "32C9"){
                    s_Text = "�r�`�|�P�X�R�|�f���D�a�W";
                 // *************************
                 } else {
                    s_Text = "�r�r�S�O�O";
                 }
                 break;
           }
        } else if (s_Text == "02") {
           switch (s_HEADS_DATA[37].ToIntDef(0)) {
              case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
                 s_Text = "�r�t�r�R�O�S";
                 break;
              default :
                 if (s_HEADS_DATA[1048].TrimRight() == "1001"){
                    s_Text = "�r�t�r�R�O�S";
                 } else {
                    //2004.01.07 �ގ��ύX
                    //s_Text = "�b�D�r�D";
                    s_Text = "�r�r�S�O�O";
                    //*******************
                 }
                 break;
           }
        } else if (s_Text == "04") {
           //2004.01.06 �ގ��ύX
           //s_Text = "�r�c�Q�S�^�r�r�S�O�O";
           //2004.01.19 �ގ��ύX
           //s_Text = "�r�c�Q�S";
           //2004.02.02 �ގ��ύX
           //s_Text = "�r�q�Q�R�T";
           s_Text = "�r�c�R�S�T";
           //*******************
           //*******************
           //*******************
        } else if (s_Text == "09") {
           s_Text = "";
        // 2022.06.10 �_�u���i�b�g�ǉ�_S
        } else if (s_Text == "11" || s_Text == "13" || s_Text == "15"){
            switch (s_HEADS_DATA[37].ToIntDef(0)) {
              case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
                 s_Text = "�r�t�r�R�O�S";
                 break;
              default :
                 if (s_HEADS_DATA[1048].TrimRight() == "1001"){
                    s_Text = "�r�t�r�R�O�S";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "30C9"){
                    s_Text = "�r�r�R�O�S";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "31C9"){
                    s_Text = "�r�t�r�R�O�S";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "32C9"){
                    s_Text = "�r�`�|�P�X�R�|�f���D�a�W";
                 } else {
                    s_Text = "�r�r�S�O�O";
                 }
                 break;
           }
        // 2022.06.10 �_�u���i�b�g�ǉ�_E
        } else {
           s_Text = "";
        }
      }

      // 2012.01.20 �����ގ�
      if ( s_ChinaM == "CHINA" ) {                                // �����ގ�
          if ( s_Text == "�r�r�S�O�O" ) {
              s_Text = "�p�Q�R�T�a";
          }
      }
      // *******************

      Excel_Cell_DataSet(37, 15, s_Text);
      G_Log_Text = "�ݶ����čގ�          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      /*
      i_RecNo = 168;   // �ݶ����čގ�
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(34, 15, s_Text);
      G_Log_Text = "�ݶ����čގ�          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
      */

      //***************************
      //***  �ݶ����ā@�d��
      //***************************
      // �ݶ����čގ�����r�t�r�R�O�S��̏ꍇ�o�͂��Ȃ�
      // �wC028�x
      if (s_Text != "�r�t�r�R�O�S"){
         i_RecNo = 1049;   // �ݶ����Ďd��
         i_Siage  =s_HEADS_DATA[i_RecNo].ToIntDef(0);
         if (i_Siage == 0 ) {
           //2004.01.07 �d��ύX
           //s_Text = "�W��";
           i_RecNo = 1045;   // SPANC
           s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
           if (s_Text == "02" || s_Text == "03"){
             s_Text = "��Ұ�";
           } else {
             s_Text = "�W��";
           }
           //*******************
         } else if ( i_Siage == 1 ) {
           s_Text = "Znү�";
         } else if ( i_Siage == 2 ) {
           s_Text = "�Ƹ�";
         } else {
           s_Text = "";
         }
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(37, 23, s_Text);
      G_Log_Text = "�ݶ����Ďd��          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   }

   //2011.03.11 �H���ʐ^�v���L�ǉ�
   //*******************
   //*** ���L(�H���ʐ^�v)
   //*******************
   // �wC030�x
   i_RecNo = 1102;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(40, 18, s_Text);
   G_Log_Text = "���L(�H���ʐ^�v)     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*****************************

   //*************************

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          �c���@�@�f�[�^�Z�b�g                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  ���u�ԍ�
   //***************************
   // �wD001�x
   // 2020.10.30 �D�Ԓǉ�_S
   //i_RecNo = 28;    // �H���ԍ�
   if(s_HEADS_DATA[36].Pos("M") != 0)  {
      // ���p�̏ꍇ
      i_RecNo = 270;   // �D��
   } else {
      // ���p�łȂ��ꍇ
      i_RecNo = 28;    // �H���ԍ�
     Write_Log("�H���ԍ�");
   }
   // 2020.10.30 �D�Ԓǉ�_E
   s_Text = ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   Excel_Cell_DataSet(42, 9, s_Text);
   G_Log_Text = "�H���ԍ�              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �@��ԍ�
   //***************************
   // �wD002�x
   i_RecNo = 27;  // �@��ԍ�
   //2004.10.04
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   int w_count = s_Text.AnsiPos("�`");

   if (w_count){
      s_Text = ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "�`" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
   }else{
      s_Text = ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   }
   //**********

   // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_S
   if (Tokki_Code("39", s_HEADS_DATA, 1500) == true) {
      // �@��ԍ����L���ڗL
      // �@��ԍ����O���L�ɋL�q
      s_Text = "�����Q��";
   }
   // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_E

   Excel_Cell_DataSet(42, 18, s_Text);
   G_Log_Text = "�@��ԍ�              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���݌v����
   //***************************
   // �wD003�x
   i_RecNo = 825;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2017.06.09 �����ǉ�
   //if (AllFloatChk(s_Text.c_str(),0) == 0){
   if (AllFloatChk(s_Text.c_str(),1) == 0){
   //*******************
      if (s_Text.ToDouble() < 900){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "��C�J��";
      }
   } else {
      G_ErrLog_Text = "B���݌v����     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(44, 6, s_Text);
   G_Log_Text = "B���݌v����           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���݌v���x
   //***************************
   // �wD004�x
   i_RecNo = 829;    // B���݌v���x
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "B���݌v���x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }
   //2004.07.15 �����Ȗ��ǉ�
   if (s_HEADS_DATA[268].TrimRight() == "1" && s_HEADS_DATA[269].TrimRight() == "11") { // �����Ȗ��g�P�P
      i_RecNo = 854;    // B���o�����x
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         s_Text = FormatFloat("##0.00",s_Text2.ToDouble()) + "/" + s_Text;
      } else {
         G_ErrLog_Text = "B���o�����x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text2 + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = "***.**/" + s_Text;
      }
      i_RecNo = 852;    // B���������x
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         s_Text = FormatFloat("##0.00",s_Text2.ToDouble()) + "/" + s_Text;
      } else {
         G_ErrLog_Text = "B���������x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text2 + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = "***.**/" + s_Text;
      }
   }
   //*************************

   Excel_Cell_DataSet(45, 6, s_Text);
   G_Log_Text = "B���݌v���x           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���������� ��
   //***************************
   // �wD005�x
   i_RecNo = 826;    // B����������
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() != 0){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "-----";
      }
   } else {
      G_ErrLog_Text = "B���������� ��   RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(46, 6, s_Text);
   G_Log_Text = "B�������� ��        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���������� ����
   //***************************
   // �wD006�x
   i_RecNo = 827;    // B����������
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() != 0){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "-----";
      }
   } else {
      G_ErrLog_Text = "B���������� ����   RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(47, 6, s_Text);
   G_Log_Text = "B�������� ����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2004.07.15 �����Ȗ��ǉ�
   //***************************
   //***  B������
   //***************************
   if (s_HEADS_DATA[268].TrimRight() == "1") { // �����Ȗ���
      s_Text = "����";
   // �wD018�x
      Excel_Cell_DataSet(48, 4, s_Text);

      i_RecNo = 107;    // �����M��
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("########0",s_Text.ToDouble()) + " kW/";
      } else {
         G_ErrLog_Text = "�����M��        RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = "*********kW/";
      }
      i_RecNo = 856;    // B������
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         //2004.08.16 �؂�グ
         if (s_HEADS_DATA[846].TrimRight() == "1") { // �a���X�`�[��
            s_Text = s_Text + FormatFloat("#####0.00",ceil(s_Text2.ToDouble() / 60 * 100) / 100);
         } else {                                    // �a���t
            s_Text = s_Text + FormatFloat("#####0.00",ceil(s_Text2.ToDouble() * 1000 / 60 * 100) / 100);
         }
         //if (s_HEADS_DATA[846].TrimRight() == "1") { // �a���X�`�[��
         //   s_Text = s_Text + FormatFloat("#####0.00",s_Text2.ToDouble() / 60);
         //} else {                                    // �a���t
         //   s_Text = s_Text + FormatFloat("#####0.00",s_Text2.ToDouble() * 1000 / 60);
         //}
         //*******************
      } else {
         G_ErrLog_Text = "B������         RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text2 + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = s_Text + "******.**";
      }
   // �wD019�x
   Excel_Cell_DataSet(48, 6, s_Text);
   G_Log_Text = "B������               RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[846].TrimRight() == "1") { // �a���X�`�[��
      s_Text = "kg/min";
   } else {                                    // �a���t
      s_Text = "lit/min";
   }
   // �wD020�x
   Excel_Cell_DataSet(48, 11, s_Text);
   }
   //*************************

   //***************************
   //***  B�����e��
   //***************************
   // �wD007�x
   i_RecNo = 1282;    // Bΰ��ޗ�
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "B�����e��       RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(49, 6, s_Text);
   G_Log_Text = "B�����e��             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �`�M�ʐ�
   //***************************
   // �wD008�x
   i_RecNo = 1067;    // �`�M�ʐ�
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
//      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      s_Text = s_Text.TrimRight();
   } else {
      G_ErrLog_Text = "�`�M�ʐ�        RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(50, 6, s_Text);
   G_Log_Text = "�`�M�ʐ�              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���݌v����
   //***************************
   // �wD009�x
   i_RecNo = 830;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2017.06.09 �����ǉ�
   //if (AllFloatChk(s_Text.c_str(),0) == 0){
   if (AllFloatChk(s_Text.c_str(),1) == 0){
   //*******************
      if (s_Text.ToDouble() < 900){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "��C�J��";
      }
   } else {
      G_ErrLog_Text = "A���݌v����     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(44, 14, s_Text);
   G_Log_Text = "A���݌v����           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���݌v���x
   //***************************
   // �wD010�x
   i_RecNo = 834;    // A���݌v���x
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "A���݌v���x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }
   //2004.07.15 �����Ȗ��ǉ�
   if (s_HEADS_DATA[268].TrimRight() == "1" && s_HEADS_DATA[269].TrimRight() == "11") { // �����Ȗ��g�P�P
      i_RecNo = 855;    // A���o�����x
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         s_Text = FormatFloat("##0.00",s_Text2.ToDouble()) + "/" + s_Text;
      } else {
         G_ErrLog_Text = "A���o�����x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text2 + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = "***.**/" + s_Text;
      }
      i_RecNo = 853;    // A���������x
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         s_Text = FormatFloat("##0.00",s_Text2.ToDouble()) + "/" + s_Text;
      } else {
         G_ErrLog_Text = "A���������x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text2 + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = "***.**/" + s_Text;
      }
   }
   //*************************

   Excel_Cell_DataSet(45, 14, s_Text);
   G_Log_Text = "A���݌v���x           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���������� ��
   //***************************
   // �wD011�x
   i_RecNo = 831;    // A����������
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() != 0){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "-----";
      }
   } else {
      G_ErrLog_Text = "A���������� ��   RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(46, 14, s_Text);
   G_Log_Text = "A�������� ��        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���������� ����
   //***************************
   // �wD012�x
   i_RecNo = 832;    // A����������
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() != 0){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "-----";
      }
   } else {
      G_ErrLog_Text = "A���������� ����   RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(47, 14, s_Text);
   G_Log_Text = "A�������� ����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2004.07.15 �����Ȗ��ǉ�
   //***************************
   //***  A������
   //***************************
   if (s_HEADS_DATA[268].TrimRight() == "1") { // �����Ȗ���
      i_RecNo = 857;    // A������
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         //2004.08.16 �؂�グ
         if (s_HEADS_DATA[847].TrimRight() == "1") { // �`���X�`�[��
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() / 60 * 100) / 100);
         } else {                                    // �`���t
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() * 1000 / 60 * 100) / 100);
         }
         //if (s_HEADS_DATA[847].TrimRight() == "1") { // �`���X�`�[��
         //   s_Text = FormatFloat("#####0.00",s_Text.ToDouble() / 60);
         //} else {                                    // �`���t
         //   s_Text = FormatFloat("#####0.00",s_Text.ToDouble() * 1000 / 60);
         //}
         //*******************
      } else {
         G_ErrLog_Text = "A������         RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text2 + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

         s_Text = s_Text + "******.**";
      }
   // �wD021�x
   Excel_Cell_DataSet(48, 14, s_Text);
   G_Log_Text = "A������               RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[847].TrimRight() == "1") { // �`���X�`�[��
      s_Text = "kg/min";
   } else {                                    // �`���t
      s_Text = "lit/min";
   }
   // �wD022�x
   Excel_Cell_DataSet(48, 24, s_Text);
   }
   //*************************

   //***************************
   //***  A�����e��
   //***************************
   // �wD013�x
   i_RecNo = 1283;    // Aΰ��ޗ�
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "A�����e��       RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(49, 14, s_Text);
   G_Log_Text = "A�����e��             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �@�펿��
   //***************************
   // �wD014�x
   i_RecNo = 1029;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(50, 17, s_Text);
   G_Log_Text = "�@�펿��              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  �K�p�K�i
   //************************
   // �wD015�x
   i_RecNo = 29;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //G_Log_Text = "�K�p�K�i              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x�ɂ��A";
   //2013.11.08 �K�p�K�i(����)�\�L����
   //s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),true,true, s_HEADS_DATA[1].TrimRight());
   if (s_HEADS_DATA[1].TrimRight()==0) {
       // �a��
       s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),true,false, s_HEADS_DATA[1].TrimRight());
   } else {
       // �p��
       s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),false,false, s_HEADS_DATA[1].TrimRight());
   }
   //*********************************
   // 2011.05.30 ��n����
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   //2013.11.08 �K�p�K�i(����)�\�L����
   //if ( ( s_Text2.ToIntDef(0) == 0 ) && ( s_HEADS_DATA[1].TrimRight() == "0" ) ) {
   if ( s_Text2.ToIntDef(0) == 0 ) {
   //*********************************
       // 2012.01.20 ��n�t���O
       //if ( s_HEADS_DATA[5].TrimRight() == "42842" ) {
       //    //��n����(42842)(�a��)����
       //    s_Text = "��n����(�a��)";
       //} else if ( s_HEADS_DATA[5].TrimRight() == "43935" ) {
       //    //��n����(43935)(�a��)����
       //    s_Text = "��n����(�a��)";
       //} else if ( s_HEADS_DATA[401].TrimRight() == "42842" ) {
       //    //��n����(42842)(�a��)����
       //    s_Text = "��n����(�a��)";
       //} else if ( s_HEADS_DATA[401].TrimRight() == "43935" ) {
       //    //��n����(43935)(�a��)����
       //    s_Text = "��n����(�a��)";
       //}
       if ( s_HEADS_DATA[1107].TrimRight() == "CHINA" ) {
          //��n����(����)����
          s_Text = "��n����(����)";
       }
       // *********************
   }
   // *******************
   //2004.07.15 �����Ȗ��Œǉ�
   i_RecNo = 1372;
   s_Text = s_Text + "  " + s_HEADS_DATA[i_RecNo].TrimRight();
   //*************************
   Excel_Cell_DataSet(51, 6, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���t���@�@Max
   //***************************
   // �wD016�x
//   i_RecNo = 1014;    // RMAX
   i_RecNo = 1089;    // BMAX �ɕύX�@2002.11.07 Miya
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "RMAX            RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(52, 9, s_Text);
   G_Log_Text = "���t���@ Max          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���t���@�@Min
   //***************************
   // �wD017�x
   i_RecNo = 1015;    // RMAX
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "RMIN            RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      s_Text = "";
   }

   Excel_Cell_DataSet(52, 15, s_Text);
   G_Log_Text = "���t���@ Min          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2016/09/14 �X�e�b�J�[�ǉ�
   //***************************
   //***  �X�e�b�J�[
   //***************************
   // �wD018�x
   i_RecNo = 1122;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "�����R�[�h            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���A";
   s_Text2 = "";

   if( s_Text.Length() == 5 ) {

      for (i = 0; i < G_STACKER_RecCnt; i++) {
         if (G_STACKER[i][0] == s_Text) {
            s_Text2 = G_STACKER[i][1];
         }
      }

      if( s_Text2 == "" ) {
         s_Text2 = "�p��";
      }

   } else {
      s_Text2 = "�p��";
   }

   s_Text = s_Text2;

   if (s_HEADS_DATA[1].TrimRight()==0) {
   // �wD023�x
      // �a��
       // 2020.10.30 �X�e�b�J�[�ύX_S
      //Excel_Cell_DataSet(53, 18, "");
      //G_Log_Text = "�X�e�b�J�[            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x�������B";
      //Write_Log(G_Log_Text);
      if ( s_HEADS_DATA[36].Pos("M") != 0 ) {
         // ���p�̏ꍇ�X�e�b�J�[�͕K���t��
         Excel_Cell_DataSet(53, 24, "�a��");
         G_Log_Text = "�X�e�b�J�[            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         Excel_Cell_DataSet(53, 18, "");
         G_Log_Text = "�X�e�b�J�[            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x�������B";
         Write_Log(G_Log_Text);
      }
      // 2020.10.30 �X�e�b�J�[�ύX_E
   } else {
   // �wD024�x
      // �p��
      // 2020.10.30 �X�e�b�J�[�ύX_S
      if ( s_HEADS_DATA[36].Pos("M") != 0 ) {
         // ���p�̏ꍇ�X�e�b�J�[�͕K���t��
         s_Text = "�p��";
      }
      // 2020.10.30 �X�e�b�J�[�ύX_E
      Excel_Cell_DataSet(53, 24, s_Text);
      G_Log_Text = "�X�e�b�J�[            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  �@�B����
   //***************************
   // �wD025�x
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   //2019.12.27 ����w���������ύX_S
   //      ���p�łȂ��ꍇ
   if(s_HEADS_DATA[36].Pos("M") == 0)  {
       // �o�͂��Ȃ�
       s_Text = "";
   }
   //2019.12.27 ����w���������ύX_E

   if(s_Text != "")
   {
      Excel_Cell_DataSet(41, 9, s_Text);
   }

   // **************************

   //*************************

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          E���@�@�f�[�^�Z�b�g                        ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  �A�C�v���[�g
   //***************************
   // �wE001�x
   i = 0;

   i_Value_1 = s_HEADS_DATA[823].ToIntDef(0);
   i_Value_2 = s_HEADS_DATA[1011].ToIntDef(0);

   //�@���p�łȂ��ꍇ(�A�C�v���[�g�Ȃ�)
   if(P_HEADS_DATA[36].Pos("M")==0){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   //�A�A�C�v���[�g���t���Ȃ��^���̏ꍇ
   // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_S
   ////CX-00�̏ꍇ
   //CX-01�̏ꍇ
   // 2021.01.06 CX-03�ǉ�(CX-00��CX-01)_E
   else if (i_Value_1 == 14 && i_Value_2 == 0){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_S
   ////CXW-00�̏ꍇ
   //CXW-01�̏ꍇ
   // 2021.01.06 CXW-03�ǉ�(CXW-00��CXW-01)_S
   else if (i_Value_1 == 20 && i_Value_2 == 0){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   // 2021.01.06 CX-03�ǉ�_S
   //CX-03�̏ꍇ
   else if (i_Value_1 == 14 && i_Value_2 == 3){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
   }
   //CXW-03�̏ꍇ
   else if (i_Value_1 == 20 && i_Value_2 == 3){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
   }
   // 2021.01.06 CX-03�ǉ�_E
   //CX-10�̏ꍇ
   else if (i_Value_1 == 14 && i_Value_2 == 10){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   //CX-10D�̏ꍇ
   else if (i_Value_1 == 15 && i_Value_2 == 10){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   //LX-00�̏ꍇ
   else if (i_Value_1 == 3 && i_Value_2 == 0){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   //RX-00�̏ꍇ
   else if (i_Value_1 == 8 && i_Value_2 == 0){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   //UX-01�̏ꍇ
   else if (i_Value_1 == 2 && i_Value_2 == 0){
      //""���o�͈ʒuE001�ɏo�͂���
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   else{
//�B����ȊO
    // 2020.07.37 �A�C�v���[�g���L����_S
    // A���@

    if (s_HEADS_DATA[823].ToIntDef(0) == 2 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
       //UX-10
       s_Text = "  A=330";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 3 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
       //LX-10
       s_Text = "  A=380";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 3 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 30) {
       //LX-30
       s_Text = "  A=500";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 3 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 50) {
       //LX-50
       s_Text = "  A=640";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 4 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 40) {
       //SX-40
       s_Text = "  A=700";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
       //RX-10
       s_Text = "  A=340";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 30) {
       //RX-30
       if ((s_HEADS_DATA[170].ToIntDef(0) >= 3) &&
           (s_HEADS_DATA[170].ToIntDef(0) <= 280)) {
          s_Text = "  A=500";
       }
       else if ((s_HEADS_DATA[170].ToIntDef(0) >= 281) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 400)) {
          s_Text = "  A=300";
       }
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 50) {
       //RX-50
       s_Text = "  A=700";
    }
    else {
       s_Text = "";
    }
    // 2020.07.37 �A�C�v���[�g���L����_E
    //  �iA)P�^�̏ꍇ
        if(s_HEADS_DATA[36].Pos("P") != 0) {
            //  "S�t���[����GB�T�|�[�g�A�C�v���[�g�t"���o�͈ʒuE001�ɏo�͂���
            // 2020.07.17 �A�C�v���[�g���L����_S
            //Excel_Cell_DataSet(2 + i, 36, "S�t���[����GB�T�|�[�g�A�C�v���[�g�t");
            s_Text = "S�t���[����GB�T�|�[�g�A�C�v���[�g�t" + s_Text;
            Excel_Cell_DataSet(2 + i, 36, s_Text);
            // 2020.07.17 �A�C�v���[�g���L����_E
            //  �܂�F001�Ɂh���h���o�͂�E001��F001��L001�̉��Ɍr��������
            Excel_Cell_DataSet(2 + i, 35, "��");
            /*
            Excel_Cell_SetUnderline(2 + i, 36);    //E001
            Excel_Cell_SetUnderline(2 + i, 35);    //F001
            Excel_Cell_SetUnderline(2 + i, 56);    //L001
            */
            Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
            //2019.07.25 ����w���������ύX_S
            //i++;
            //2019.07.25 ����w���������ύX_E
        }
    //  �iB)J�^�̏ꍇ
        else if(s_HEADS_DATA[36].Pos("J") != 0) {
            //  "S�t���[���A�C�v���[�g�t"���o�͈ʒuE001�ɏo�͂���
            // 2020.07.17 �A�C�v���[�g���L����_S
            //Excel_Cell_DataSet(2 + i, 36, "S�t���[���A�C�v���[�g�t");
            s_Text = "S�t���[���A�C�v���[�g�t" + s_Text;
            Excel_Cell_DataSet(2 + i, 36, s_Text);
            // 2020.07.17 �A�C�v���[�g���L����_E
            //  �܂�F001�Ɂh���h���o�͂�E001��F001��L001�̉��Ɍr��������
            Excel_Cell_DataSet(2 + i, 35, "��");
            /*
            Excel_Cell_SetUnderline(2 + i, 36);    //E001
            Excel_Cell_SetUnderline(2 + i, 35);    //F001
            Excel_Cell_SetUnderline(2 + i, 56);    //L001
            */
            Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
            //2019.07.25 ����w���������ύX_S
            //i++;
            //2019.07.25 ����w���������ύX_E
        }
   }

   //***************************
   //***  ���̖����t
   //***************************
   // �wE002�x
   //2019.07.25 ����w���������ύX_S
   i = 1;
   //2019.07.25 ����w���������ύX_E
   //�@ �p�łȂ��ꍇ(�A�C�v���[�g�Ȃ�)
   if(P_HEADS_DATA[36].Pos("M")==0)  {
      //""���o��
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   //�A ��ȊO
   }else{
      //"���̖����t"���o��
      Excel_Cell_DataSet(2 + i, 36, "���̖����t");
      //�܂�F002�Ɂh���h���o�͂�E002��F002��F002�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E002
      Excel_Cell_SetUnderline(2 + i, 35);    //F002
      Excel_Cell_SetUnderline(2 + i, 56);    //L002
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***  ���L(�֖��P)
   //***************************
   // �wE003�x
   //2019.07.25 ����w���������ύX_S
   i = 2;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1302;   // ���L(�֖��P)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F003�Ɂh���h���o�͂�E003��F003��L003�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E003
      Excel_Cell_SetUnderline(2 + i, 35);    //F003
      Excel_Cell_SetUnderline(2 + i, 56);    //L003
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***  ���L(ɽ�قP)
   //***************************
   // �wE004�x
   //2019.07.25 ����w���������ύX_S
   i = 3;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1325;   // ���L(ɽ�قP)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
        //�܂�F004�Ɂh���h���o�͂�E004��F004��L004�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E004
      Excel_Cell_SetUnderline(2 + i, 35);    //F004
      Excel_Cell_SetUnderline(2 + i, 56);    //L004
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***  ���L(ɽ�قQ)
   //***************************
   // �wE005�x
   //2019.07.25 ����w���������ύX_S
   i = 4;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1326;   // ���L(ɽ�قQ)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F005�Ɂh���h���o�͂�E005��F005��L005�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E005
      Excel_Cell_SetUnderline(2 + i, 35);    //F005
      Excel_Cell_SetUnderline(2 + i, 56);    //L005
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***  ���L(�q��d�l)
   //***************************
   // �wE006�x
   //2019.07.25 ����w���������ύX_S
   i = 5;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1331;   // ���L(�q��d�l)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F006�Ɂh���h���o�͂�E006��F006��L006�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E006
      Excel_Cell_SetUnderline(2 + i, 35);    //F006
      Excel_Cell_SetUnderline(2 + i, 56);    //L006
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***  ���L(�q��d�l�Q)
   //***************************
   // �wE007�x
   //2019.07.25 ����w���������ύX_S
   i = 6;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1055;   // ���L(�q��d�l�Q)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F007�Ɂh���h���o�͂�E007��F007��L007�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E007
      Excel_Cell_SetUnderline(2 + i, 35);    //F007
      Excel_Cell_SetUnderline(2 + i, 56);    //L007
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***  ���L(�K�X�P�b�g)
   //***************************
   // �wE008�x
   //2019.07.25 ����w���������ύX_S
   i = 7;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1054;   // ���L(�K�X�P�b�g)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F008�Ɂh���h���o�͂�E008��F008��L008�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E008
      Excel_Cell_SetUnderline(2 + i, 35);    //F008
      Excel_Cell_SetUnderline(2 + i, 56);    //L008
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***  ���L(���Ķ޲���ް�����P)
   //***************************
   // �wE009�x
   //2019.07.25 ����w���������ύX_S
   i = 8;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1312;   // ���L(���Ķ޲���ް�����P)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F009�Ɂh���h���o�͂�E009��F009��L009�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E009
      Excel_Cell_SetUnderline(2 + i, 35);    //F009
      Excel_Cell_SetUnderline(2 + i, 56);    //L009
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***  ���L(���p�H��)
   //***************************
   // �wE010�x
   //2019.07.25 ����w���������ύX_S
   i = 9;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1318;   // ���L(���p�H��)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F010�Ɂh���h���o�͂�E010��F010��L010�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E010
      Excel_Cell_SetUnderline(2 + i, 35);    //F010
      Excel_Cell_SetUnderline(2 + i, 56);    //L010
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   //2019.07.25 ����w���������ύX_S
   }
   //2019.07.25 ����w���������ύX_E

   //***************************
   //***   ���L(�������)
   //***************************
   // �wE011�x
   //2019.07.25 ����w���������ύX_S
   i = 10;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1330;   // ���L(�������)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F011�Ɂh���h���o�͂�E011��F011��L011�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E011
      Excel_Cell_SetUnderline(2 + i, 35);    //F011
      Excel_Cell_SetUnderline(2 + i, 56);    //L011
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***   ���L(���`�F�b�g�X�p�i)
   //***************************
   // �wE012�x
   //2019.07.25 ����w���������ύX_S
   i = 11;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1335;   // ���L(���`�F�b�g�X�p�i)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      //if (i >= 8) {
      //     Excel_Cell_DataSet(31 + i, 13, s_Text);
      //} else {
      //     Excel_Cell_DataSet(35 + i, 6, s_Text);
      //}
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F012�Ɂh���h���o�͂�E012��F012��L012�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E012
      Excel_Cell_SetUnderline(2 + i, 35);    //F012
      Excel_Cell_SetUnderline(2 + i, 56);    //L012
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   //2019.07.25 ����w���������ύX_S
   //}
   //2019.07.25 ����w���������ύX_E

   //***************************
   //***    ���L(�d�������P)
   //***************************
   // �wE013�x
   //2019.07.25 ����w���������ύX_S
   i = 12;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1304;   // ���L(�d�������P)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      // �܂�F013�Ɂh���h���o�͂�E013��F013��L013�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E013
      Excel_Cell_SetUnderline(2 + i, 35);    //F013
      Excel_Cell_SetUnderline(2 + i, 56);    //L013
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***    ���L(��ڰĖ����P)
   //***************************
   // �wE014�x
   //2019.07.25 ����w���������ύX_S
   i = 13;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1306;   // ���L(��ڰĖ����P)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F014�Ɂh���h���o�͂�E014��F014��L014�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E014
      Excel_Cell_SetUnderline(2 + i, 35);    //F014
      Excel_Cell_SetUnderline(2 + i, 56);    //L014
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   //2019.07.25 ����w���������ύX_S
   }
   //2019.07.25 ����w���������ύX_E

   //***************************
   //***    ���L(����ݼ�)
   //***************************
   // �wE015�x
   //2019.07.25 ����w���������ύX_S
   i = 14;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1329;   // ���L(����ݼ�)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2020.11.24 �K�X�P�b�g���L�ǉ�_S
   s_GasMatA = "";                                     // A����Ķ޽��čގ�
   s_GasMatB = "";                                     // B����Ķ޽��čގ�
   s_GasMatA = Get_SheetGasketMatA3("A", pcHeadsData); // A����Ķ޽��čގ�
   s_GasMatB = Get_SheetGasketMatA3("B", pcHeadsData); // B����Ķ޽��čގ�
   bGas_A = true;                                      // A����Ķ޽��ėL�@
   bGas_B = true;                                      // B����Ķ޽��ėL
   if (s_Text.Trim() == ""){
      // ����ݼޒ��L����
      bGas_A = false;                                  // A����Ķ޽��Ė��@
      bGas_B = false;                                  // B����Ķ޽��Ė�
   }
   if (s_HEADS_DATA[893].SubString(3,1)== "1") {
      // B���ް�ɽ��
      bGas_B = false;                                  // B����Ķ޽��Ė�
   }
   if (s_HEADS_DATA[906].SubString(3,1)== "1") {
      // A���ް�ɽ��
      bGas_A = false;                                  // A����Ķ޽��Ė��@
   }

   if (bGas_A == true || bGas_B == true ) {  // �K�X�P�b�g�v
      s_Text2 = s_HEADS_DATA[892].TrimRight(); // �r�P�m�Y�����a
      d_Koukei = s_Text2.ToDouble();
      s_Text3 = s_HEADS_DATA[893].TrimRight(); // �r�P�m�Y���p��
      if ( d_Koukei >= 250 ) {
         s_GasName = "�@�S�ʍ���ݼ޶޽���";
      } else if (s_Text3.SubString(1,2) == "04" ||
                 s_Text3.SubString(1,2) == "05" ||
                 s_Text3.SubString(1,2) == "44" ||
                 s_Text3.SubString(1,2) == "08" ||
                 s_Text3.SubString(1,2) == "09" ||
                 s_Text3.SubString(1,2) == "48") {
         if ( d_Koukei >= 10 ) {
            s_GasName = "�@�S�ʍ���ݼ޶޽���";
         } else {
            s_GasName = "�@�ݸ޶޽���";
         }
         s_GasName = "�@�ݸ޶޽���";
      } else {
         s_GasName = "�@�ݸ޶޽���";
      }
   } else {
      s_GasName = "";
   }

   if (bGas_A == false && bGas_B == true ) {
      // B����Ķ޽��ėL
      s_Text4 = s_GasName + "(" + s_GasMatB + ")";
   } else if (bGas_A == true && bGas_B == false ) {
      // A����Ķ޽��ėL
      s_Text4 == s_GasName + "(" + s_GasMatA + ")";
   } else if (bGas_A == true && bGas_B == true ) {
      // ������Ķ޽��ėL
      if (s_GasMatA != s_GasMatB) {
         // AB��Ķ޽��ĈقȂ�
         s_Text4 = s_GasName + "(B:" + s_GasMatB + "/A:" + s_GasMatA + ")";
      } else {
         s_Text4 = s_GasName + "(" + s_GasMatB + ")";
      }
   } else {
      // ��Ķ޽��Ė�
      s_Text4 = "";
   }

   s_Text = s_Text + s_Text4;
   //2020.11.24 �K�X�P�b�g���L�ǉ�_E
   if (s_Text.Trim() != ""){
      //if (i >= 8) {
      //     Excel_Cell_DataSet(31 + i, 13, s_Text);
      //} else {
      //     Excel_Cell_DataSet(35 + i, 6, s_Text);
      //}
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F015�Ɂh���h���o�͂�E015��F015��L015�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E015
      Excel_Cell_SetUnderline(2 + i, 35);    //F015
      Excel_Cell_SetUnderline(2 + i, 56);    //L015
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(2 + i, 30);
      Excel_Cell_SetUnderline(2 + i, 32);
      Excel_Cell_SetUnderline(2 + i, 56);
      Excel_Cell_SetUnderline(2 + i, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }

   //***************************
   //***    ���L(�\���i)
   //***************************
   // �wE016�x
   //2019.07.25 ����w���������ύX_S
   i = 15;
   //2019.07.25 ����w���������ύX_E
   i_RecNo = 1336;   // ���L(�\���i)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      //if (i >= 8) {
      //     Excel_Cell_DataSet(31 + i, 13, s_Text);
      //} else {
      //     Excel_Cell_DataSet(35 + i, 6, s_Text);
      //}
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F016�Ɂh���h���o�͂�E016��F016��L016�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E016
      Excel_Cell_SetUnderline(2 + i, 35);    //F016
      Excel_Cell_SetUnderline(2 + i, 56);    //L016
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 ����w���������ύX_S
      //i++;
      //2019.07.25 ����w���������ύX_E
   }
   //2019.07.25 ����w���������ύX_S
   //}
   //2019.07.25 ����w���������ύX_E

   //*************************

   // 2020.05.28 �����������L�ǉ�_S
   //***************************
   //***    ���L(��������)
   //***************************
   // �wE017�x
   i = 16;
   s_Text = "";
   s_Text2 = "";
   s_Text3 = "";
   s_Text4 = "";

   s_Text = Get_HotAirDrying();

   G_Log_Text = "���������擾          �w" + s_Text + "�x";
   Write_Log(G_Log_Text);

   G_Log_Text = "s_SpecCode            �w" + s_SpecCode + "�x";
   Write_Log(G_Log_Text);

   i_Length = s_Text.Length();
   if (i_Length < 21 ) {
       s_Text = "";
   }

   if(s_SpecCode == "B"){

      if ( s_Text != "" ) {
         //�O����
         s_Text2 = s_Text.SubString(1,21);
         //B������
         if ( s_Text.Pos("B(1)") > 0 ) {
            s_Text3 = "1";
         } else if ( s_Text.Pos("B(2)") > 0 ) {
            s_Text3 = "2";
         } else if ( s_Text.Pos("B(3)") > 0 ) {
            s_Text3 = "3";
         } else if ( s_Text.Pos("B(4)") > 0 ) {
            s_Text3 = "4";
         } else if ( s_Text.Pos("B(5)") > 0 ) {
            s_Text3 = "5";
         } else if ( s_Text.Pos("B(6)") > 0 ) {
            s_Text3 = "6";
         } else if ( s_Text.Pos("B(7)") > 0 ) {
            s_Text3 = "7";
         } else if ( s_Text.Pos("B(8)") > 0 ) {
            s_Text3 = "8";
         } else if ( s_Text.Pos("B(9)") > 0 ) {
            s_Text3 = "9";
         } else if ( s_Text.Pos("B(10)") > 0 ) {
            s_Text3 = "10";
         } else if ( s_Text.Pos("B(11)") > 0 ) {
            s_Text3 = "11";
         } else if ( s_Text.Pos("B(12)") > 0 ) {
            s_Text3 = "12";
         } else if ( s_Text.Pos("B(13)") > 0 ) {
            s_Text3 = "13";
         } else if ( s_Text.Pos("B(14)") > 0 ) {
            s_Text3 = "14";
         } else if ( s_Text.Pos("B(15)") > 0 ) {
            s_Text3 = "15";
         } else if ( s_Text.Pos("B(16)") > 0 ) {
            s_Text3 = "16";
         } else if ( s_Text.Pos("B(17)") > 0 ) {
            s_Text3 = "17";
         } else if ( s_Text.Pos("B(18)") > 0 ) {
            s_Text3 = "18";
         } else if ( s_Text.Pos("B(19)") > 0 ) {
            s_Text3 = "19";
         } else if ( s_Text.Pos("B(20)") > 0 ) {
            s_Text3 = "20";
         } else {
            s_Text3 = "0";
         }
         //A������
         if ( s_Text.Pos("A(1)") > 0) {
            s_Text4 = "1";
         } else if ( s_Text.Pos("A(2)") > 0 ) {
            s_Text4 = "2";
         } else if ( s_Text.Pos("A(3)") > 0 ) {
            s_Text4 = "3";
         } else if ( s_Text.Pos("A(4)") > 0 ) {
            s_Text4 = "4";
         } else if ( s_Text.Pos("A(5)") > 0 ) {
            s_Text4 = "5";
         } else if ( s_Text.Pos("A(6)") > 0 ) {
            s_Text4 = "6";
         } else if ( s_Text.Pos("A(7)") > 0 ) {
            s_Text4 = "7";
         } else if ( s_Text.Pos("A(8)") > 0 ) {
            s_Text4 = "8";
         } else if ( s_Text.Pos("A(9)") > 0 ) {
            s_Text4 = "9";
         } else if ( s_Text.Pos("A(10)") > 0 ) {
            s_Text4 = "10";
         } else if ( s_Text.Pos("A(11)") > 0 ) {
            s_Text4 = "11";
         } else if ( s_Text.Pos("A(12)") > 0 ) {
            s_Text4 = "12";
         } else if ( s_Text.Pos("A(13)") > 0 ) {
            s_Text4 = "13";
         } else if ( s_Text.Pos("A(14)") > 0 ) {
            s_Text4 = "14";
         } else if ( s_Text.Pos("A(15)") > 0 ) {
            s_Text4 = "15";
         } else if ( s_Text.Pos("A(16)") > 0 ) {
            s_Text4 = "16";
         } else if ( s_Text.Pos("A(17)") > 0 ) {
            s_Text4 = "17";
         } else if ( s_Text.Pos("A(18)") > 0 ) {
            s_Text4 = "18";
         } else if ( s_Text.Pos("A(19)") > 0 ) {
            s_Text4 = "19";
         } else if ( s_Text.Pos("A(20)") > 0 ) {
            s_Text4 = "20";
         } else {
            s_Text4 = "0";
         }
         // B���@���L�F�O���� + A���� + ")/A(" + B���� + ")��"
         s_Text = s_Text2 + s_Text4 + ")/A(" + s_Text3 + ")��";

      }

   }

   if (s_Text.Trim() != ""){
      s_Text = s_Text + "  ���`�F�b�N�V�[�g�䐔������";
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F017�Ɂh���h���o�͂�E017��F017��L017�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }

   // 2020.06.19 ���a���t���Ēǉ�_S
   //************************
   //***  ���L(���a���t����)
   //************************
   // �wE018�x
   i = 17;
   s_Text = "";
   s_Text1 = "";
   s_Text2 = "";
   s_Text3 = "";
   s_Text4 = "";

   i_RecNo = 1113;                              // ���a���Ė{��
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();

   if (s_Text1.ToIntDef(0) != 0){
      // ���a���ėL��
      s_Text1 = "";
      i_RecNo = 1108;   // BDIS(�a)
      s_Text1 = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
      if (s_Text1 == "42"){
         // 1 5/8-8UN
         s_Text1 = "1 5/8""-8UN";
      } else if (s_Text1 == "52"){
         // 2-8UN
         s_Text1 = "2""-8UN";
      } else if (s_Text1 == "1.630" ){
         // 1 5/8-8UN
         s_Text1 = "1 5/8""-8UN";
      } else if (s_Text1 == "2.000" ){
         // 2-8UN
         s_Text1 = "2""-8UN";
      } else {
         s_Text1 = "M" + s_Text1;
      }

      i_RecNo = 1377;                              // ���a���đS��
      s_Text = "���a���t�{���g�F" + s_Text1 + "�~" + s_HEADS_DATA[i_RecNo].TrimRight() + "L";

      if (P_HEADS_DATA[36].Pos("M") != 0){
         s_Text =  s_Text + "  �v�i�b�g";          // ���p�v�i�b�g
      }

      // �ގ����胍�W�b�N
      s_Text1 = "";
      i_RecNo = 1041;                              // �ގ��w��
      s_Text2 = P_HEADS_DATA[i_RecNo].TrimRight();
      i_RecNo = 1021;                              // BOL22
      s_Text3 = P_HEADS_DATA[i_RecNo].TrimRight();
      s_Text4 = G_KS_Syurui + "�|" + G_KS_Model;

      if (( s_Text2 == "A157" ) && ( s_Text4 == "�k�w�|�R�O")) {
         // Q235B2: S/L Q235B ���a 45/35 CODE A157
         s_Text1 = "45/35";                        // ���t�{���g�ގ�
      } else if (( s_Text2 == "A163" ) && ( s_Text4 == "�k�w�|�T�O")) {
         // 45_2  : S/L 45/35 ���a Q235B CODE A163
         s_Text1 = "Q235B";                        // ���t�{���g�ގ�
      } else if (( s_Text2 != "" ) && ( s_Text2 != "0000" ) ){
         s_Text1 = sBltMat;                        // ���t�{���g�ގ�
      } else {
         if (( s_Text3 == "Q235B2" ) && ( s_Text4 == "�k�w�|�R�O")) {
            // Q235B2: S/L Q235B ���a 45/35 CODE A157
            s_Text1 = "45/35";                        // ���t�{���g�ގ�
         } else if (( s_Text3 == "45_2" ) && ( s_Text4 == "�k�w�|�T�O")) {
            // 45_2  : S/L 45/35 ���a Q235B CODE A163
            s_Text1 = "Q235B";                     // ���t�{���g�ގ�
         } else {
            s_Text1 = sBltMat;                     // ���t�{���g�ގ�
         }
      }

      i_RecNo = 435;                               // �ټ��
      if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      } else {
         s_Text1 = s_Text1 + " ��";
      }

      s_Text = s_Text + "  " + s_Text1;

      s_Text1 = "";
      i_RecNo = 1113;                              // ���a���Ė{��
      s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();

      if (s_Text1.ToIntDef(0) == 0){
         s_Text = "";
      } else {
         s_Text = s_Text + "  " + s_Text1 + "�{";
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F018�Ɂh���h���o�͂�E018��F018��L018�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }

   //************************
   //***  ���L(���a���Ķ�ްBLCP1S)
   //************************
   // �wE019�x
   i = 18;
   s_Text = "";
   s_Text1 = "";
   s_Text2 = "";

   i_RecNo = 1113;                                 // ���a���Ė{��
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();

   if (s_Text1.ToIntDef(0) != 0){
      i_RecNo = 1111;      // BLCP1S
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2.ToIntDef(0) != 0 ){
         // ���a���ėL��
         i_RecNo = 1108;   // BDIS(�a)
         s_Text3 = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
         if (s_Text3 == "42"){
            // 1 5/8-8UN
            s_Text3 = "1 5/8""-8UN�p";
         } else if (s_Text3 == "52"){
            // 2-8UN
            s_Text3 = "2""-8UN�p";
         } else if (s_Text3 == "1.630" ){
            // 1 5/8-8UN
            s_Text3 = "1 5/8""-8UN�p";
         } else if (s_Text3 == "2.000" ){
            // 2-8UN
            s_Text3 = "2""-8UN�p";
         } else {
            s_Text3 = "M" + s_Text3+ "�p";
         }

         s_Text = "���a���t�{���g�J�o�[�F" + s_Text3 + "�~" + s_Text2 + "L";
         s_Text = s_Text + "  " + s_Text1 + "�{";
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F019�Ɂh���h���o�͂�E019��F019��L019�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }

   //************************
   //***  ���L(���a���Ķ�ްBLCESS)
   //************************
   // �wE020�x
   i = 19;
   s_Text = "";
   s_Text1 = "";
   s_Text2 = "";

   i_RecNo = 1113;                                 // ���a���Ė{��
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();

   if (s_Text1.ToIntDef(0) != 0){
      i_RecNo = 1112;      // BLCESS
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2.ToIntDef(0) != 0 ){
         // ���a���ėL��
         i_RecNo = 1108;   // BDIS(�a)
         s_Text3 = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
         if (s_Text3 == "42"){
            // 1 5/8-8UN
            s_Text3 = "1 5/8""-8UN�p";
         } else if (s_Text3 == "52"){
            // 2-8UN
            s_Text3 = "2""-8UN�p";
         } else if (s_Text3 == "1.630" ){
            // 1 5/8-8UN
            s_Text3 = "1 5/8""-8UN�p";
         } else if (s_Text3 == "2.000" ){
            // 2-8UN
            s_Text3 = "2""-8UN�p";
         } else {
            s_Text3 = "M" + s_Text3+ "�p";
         }

         s_Text = "���a���t�{���g�J�o�[�F" + s_Text3 + "�~" + s_Text2 + "L+C";
         s_Text = s_Text + "  " + s_Text1 + "�{";
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F020�Ɂh���h���o�͂�E020��F020��L020�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2020.06.19 ���a���t���Ēǉ�_E

   // 2020.10.30 ���͗e��UNS�K�i�x���ǉ�_S
   //************************
   //***  ���L(���͗e��UNS�K�i�x��)
   //************************
   // �wE021�x
   i = 20;
   s_Text = "";

   if ( s_HEADS_DATA[29].ToIntDef(0) == 1 ||
        s_HEADS_DATA[29].ToIntDef(0) == 2 ||
        s_HEADS_DATA[29].ToIntDef(0) == 3 ){
      // ���͗e��i�K�i���ށj
      if ( s_HEADS_DATA[43].TrimRight() == "3472" ) {
         // �t�m�r�@�m�P�O�Q�V�U
         s_Text = "�i�h�r�K�i�ގ����L�v";
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F021�Ɂh���h���o�͂�E021��F021��L021�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2020.10.30 ���͗e��UNS�K�i�x���ǉ�_E

   // 2021.10.19 ���L�ǉ�_S
   //************************
   //***  �_�C�g�N���L
   //************************
   // �wE022�x
   i = 21;
   s_Text = "";

   //--- �䒍����R�[�h
   i_RecNo = 1064;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "04633"){
       // �_�C�g�N�����h����h��w��
       s_Text = "���j�h���͎�h��̂��ƁBTS2021-041�ɂ��";
       Excel_Cell_DataSet(11, 16, s_Text);
       G_Log_Text = "�_�C�g�N�����h����h��w��     �w" + s_Text + "�x���Z�b�g�B";
       Write_Log(G_Log_Text);
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F022�Ɂh���h���o�͂�E022��F022��L022�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2021.10.19 ���L�ǉ�_E

   // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_S
   //************************
   //***  �@��ԍ�
   //************************
   // �wE023�x

   i = 21;
   s_Text = "";

   if (Tokki_Code("39", s_HEADS_DATA, 1500) == true) {
      // �@��ԍ����L���ڗL
      // �@��ԍ����O���L�ɋL�q

      i_RecNo = 27;  // �@��ԍ�
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

      int w_count = s_Text.AnsiPos("�`");

      if (w_count) {
         s_Text = "���@��ԍ�  " + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "�`" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
      } else {
         s_Text = "���@��ԍ�  " + ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F023�Ɂh���h���o�͂�E023��F023��L023�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_E

   // 2022.04.11 �A�q�D�����L�ǉ�_S
   //************************
   //***  �A�q�D��
   //************************
   // �wE024�x

   i = 22;
   s_Text = "";

   if (Tokki_Code("29", s_HEADS_DATA, 1500) == true) {
      // SHIP NAME���L�L���ڗL
      // �A�q�D�����L�q
      s_Text = Tokki_Zen("29", s_HEADS_DATA, 1500);
      s_Text = s_Text.SubString(10, s_Text.Length() - 9);
      s_Text = "�A�q�D��:" + s_Text;
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //�܂�F024�Ɂh���h���o�͂�E024��F024��L024�̉��Ɍr��������
      Excel_Cell_DataSet(2 + i, 35, "��");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2022.04.11 �A�q�D�����L�ǉ�_E


   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          G���@�@�f�[�^�Z�b�g                        ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //2019.10.31 �X�p�i�ύX_S
   //***************************
   //***    �H��^�C�g��
   //***************************
   // �wG000�x

   //2019.12.27 ����w���������ύX_S
   ////      ���p�łȂ��ꍇ
   // if(s_HEADS_DATA[36].Pos("M") == 0)  {
   //     // "�H��"���o��
   //     Excel_Cell_DataSet(43, 30, "�H��");
   // }
   //2019.12.27 ����w���������ύX_E
   //2019.10.31 �X�p�i�ύX_E

   //***************************
   //***    �X�p�i����
   //***************************
   // �wG001�x
    //2019.10.31 �X�p�i�ύX_S
    //�@    ���p�łȂ��ꍇ
    if(s_HEADS_DATA[36].Pos("M") == 0)  {
    //2019.12.02 �X�p�i�ύX_S
        //// "----"���o��
        //Excel_Cell_DataSet(44, 44, "----");
        //�`    ���`�F�b�g�Ȃ��̏ꍇ
        if (s_HEADS_DATA[262].TrimRight() != "0" && s_HEADS_DATA[262].TrimRight() != "2"){
            //"----"���o��
            Excel_Cell_DataSet(44, 44, "----");
        }
        //�a    ����ȊO
        else{
            //2020/03/31 ���ʕύX_S
            ////"1"���o��
            //Excel_Cell_DataSet(44, 44, "1");
            //"1/��"���o��
            Excel_Cell_DataSet(44, 44, "1/��");
            //2020/03/31 ���ʕύX_E
        }
        Excel_Cell_DataSet(44, 36, "���`�F�b�g�X�p�i");
    //2019.12.02 �X�p�i�ύX_E
    }
    //�A    RX-00�̏ꍇ
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"���o��
        Excel_Cell_DataSet(44, 44, "----");
    }
    //�B    ���`�F�b�g�Ȃ��̏ꍇ
    else if (s_HEADS_DATA[262].TrimRight() != "0" && s_HEADS_DATA[262].TrimRight() != "2"){
        //"----"���o��
        Excel_Cell_DataSet(44, 44, "----");
    }
    //�C    ����ȊO
    else{
        //2020/03/31 ���ʕύX_S
        ////"1"���o��
        //Excel_Cell_DataSet(44, 44, "1");
        //"1/��"���o��
        Excel_Cell_DataSet(44, 44, "1/��");
        //2020/03/31 ���ʕύX_E
    }

   //***************************
   //***    ���p�u���V����
   //***************************
   // �wG002�x

    //�@    ���p�łȂ��ꍇ
    if(P_HEADS_DATA[36].Pos("M")==0)  {
        // "----"���o��
        Excel_Cell_DataSet(45, 44, "----");
    }
    //�A    RX-00�̏ꍇ
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"���o��
        Excel_Cell_DataSet(45, 44, "----");
    }
    //2019.07.25 ����w���������ύX_S
    ////�B    ���`�F�b�g�Ȃ��̏ꍇ
    //else if (s_HEADS_DATA[262].TrimRight() != "1" && s_HEADS_DATA[262].TrimRight() != "2"){
    //�B    ���p�H��Ȃ��̏ꍇ
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
    //2019.07.25 ����w���������ύX_E
        //"----"���o��
        Excel_Cell_DataSet(45, 44, "----");
    }
    //�C    ����ȊO
    else{
        //2020/03/31 ���ʕύX_S
        ////"1"���o��
        //Excel_Cell_DataSet(45, 44, "1");
        //"1/��"���o��
        Excel_Cell_DataSet(45, 44, "1/��");
        //2020/03/31 ���ʕύX_E
    }

   //***************************
   //***    �Q�[�W�v���[�g�@L
   //***************************
   // �wG003�x

    //2019.07.25 ����w���������ύX_S
    ////�@    ���p�łȂ��ꍇ
    //if(P_HEADS_DATA[36].Pos("M") == 0)  {
    //    // "----"���o��
    //    Excel_Cell_DataSet(46, 44, "----");
    //}
    ////�A    RX-00�̏ꍇ
    //else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
    //    //"----"���o��
    //    Excel_Cell_DataSet(46, 44, "----");
    //}
    ////�B    ���`�F�b�g�Ȃ��̏ꍇ
    //else if (s_HEADS_DATA[262].TrimRight() != "1" && s_HEADS_DATA[262].TrimRight() != "2"){
    //    //"----"���o��
    //    Excel_Cell_DataSet(46, 44, "----");
    //}
    ////�C    ����ȊO
    //else{
    //    s_Text = HanToZen(P_HEADS_DATA[1126].Trim());
    //    Excel_Cell_DataSet(46, 44, s_Text);
    //}
    //�@    ���p�łȂ��ꍇ
    if(P_HEADS_DATA[36].Pos("M") == 0)  {
        // "----"���o��
        s_Text = "----";
    }
    //�A    RX-00�̏ꍇ
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"���o��
        s_Text = "----";
    }
    //�B    ���p�H��Ȃ��̏ꍇ
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
        //"----"���o��
        s_Text = "----";
    }
    //�C    ����ȊO
    else{
        s_Text = HanToZen(P_HEADS_DATA[1126].Trim());
    }
    Excel_Cell_DataSet(46, 40, s_Text);
    //2019.07.25 ����w���������ύX_E

   //***************************
   //***    �Q�[�W�v���[�g�@Max
   //***************************
   // �wG004�x

    //2019.07.25 ����w���������ύX_S
    ////�@    ���p�łȂ��ꍇ
    //if(P_HEADS_DATA[36].Pos("M")==0)  {
    ////"----"���o��
    //    Excel_Cell_DataSet(47, 44, "----");
    //}
    ////�A    RX-00�̏ꍇ
    //if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
    ////"----"���o��
    //    Excel_Cell_DataSet(47, 44, "----");
    //}
    ////�B    ���p�H��Ȃ��̏ꍇ
    //if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
    ////"----"���o��
    //    Excel_Cell_DataSet(47, 44, "----");
    //}
    ////�C    ����ȊO
    //else{
    //    s_Text = HanToZen(P_HEADS_DATA[1089].Trim());
    //    Excel_Cell_DataSet(47, 44, s_Text);
    //}
    //�@    ���p�łȂ��ꍇ
    if(P_HEADS_DATA[36].Pos("M") == 0)  {
        // "----"���o��
        s_Text = "----";
    }
    //�A    RX-00�̏ꍇ
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"���o��
        s_Text = "----";
    }
    //�B    ���p�H��Ȃ��̏ꍇ
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
        //"----"���o��
        s_Text = "----";
    }
    //�C    ����ȊO
    else{
        s_Text = HanToZen(P_HEADS_DATA[1089].Trim());
    }
    Excel_Cell_DataSet(47, 40, s_Text);
    //2019.07.25 ����w���������ύX_E

   //***************************
   //***    �Q�[�W�v���[�g�@Min
   //***************************
   // �wG005�x

    //2019.07.25 ����w���������ύX_S
    ////�@    ���p�łȂ��ꍇ
    //if(P_HEADS_DATA[36].Pos("M")==0)  {
    ////"----"���o��
    //    Excel_Cell_DataSet(48, 44, "----");
    //}
    ////�A    RX-00�̏ꍇ
    //if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
    ////"----"���o��
    //    Excel_Cell_DataSet(48, 44, "----");
    //}
    ////�B    ���p�H��Ȃ��̏ꍇ
    //if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
    ////"----"���o��
    //    Excel_Cell_DataSet(48, 44, "----");
    //}
    ////�C    ����ȊO
    //else{
    //    s_Text = HanToZen(P_HEADS_DATA[1015].Trim());
    //    Excel_Cell_DataSet(48, 44, s_Text);
    //}
    if(P_HEADS_DATA[36].Pos("M") == 0)  {
        // "----"���o��
        s_Text = "----";
    }
    //�A    RX-00�̏ꍇ
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"���o��
        s_Text = "----";
    }
    //�B    ���p�H��Ȃ��̏ꍇ
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
        //"----"���o��
        s_Text = "----";
    }
    //�C    ����ȊO
    else{
        s_Text = HanToZen(P_HEADS_DATA[1015].Trim());
    }
    Excel_Cell_DataSet(48, 40, s_Text);
    //2019.07.25 ����w���������ύX_E

   //2019.07.25 ����w���������ύX_S
   //***************************
   //***    �Q�[�W�v���[�g
   //***************************

    if(P_HEADS_DATA[36].Pos("M") == 0)  {
        // "----"���o��
        s_Text = "----";
    }
    //�A    RX-00�̏ꍇ
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"���o��
        s_Text = "----";
    }
    //�B    ���p�H��Ȃ��̏ꍇ
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
        //"----"���o��
        s_Text = "----";
    }
    //�C    ����ȊO
    else{
        //2020/03/31 ���ʕύX_S
        //s_Text = "1";
        s_Text = "1/��";
        //2020/03/31 ���ʕύX_E
    }
    Excel_Cell_DataSet(47, 44, s_Text);
   //2019.07.25 ����w���������ύX_E

   //***************************
   //***    �ړ��~��
   //***************************
   // �wG006�x

    //�@    ���p�łȂ��ꍇ
    if(P_HEADS_DATA[36].Pos("M")==0)  {
    //"----"���o��
        Excel_Cell_DataSet(49, 44, "----");
    }
    //�A    ���p�H��Ȃ��̏ꍇ
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
    //"----"���o��
        Excel_Cell_DataSet(49, 44, "----");
    }
    //�B    J�^�̏ꍇ�ړ��~�ߕs�v
    else if(s_HEADS_DATA[36].Pos("J") != 0) {
    //"----"���o��
        Excel_Cell_DataSet(49, 44, "----");
    }//�C	����ȊO
    else{
        //2020/03/31 ���ʕύX_S
        ////"1"���o��
        //Excel_Cell_DataSet(49, 44, "1");
        //"1/��"���o��
        Excel_Cell_DataSet(49, 44, "1/��");
        //2020/03/31 ���ʕύX_E
    }

   //***************************
   //***    �m�Y���I���G���e�[�V����
   //***************************
   // �wS1�x�wS2�x�wS3�x�wS4�x
   s_S1NO_JP = "";           // S1�t���a��
   s_S2NO_JP = "";           // S2�t���a��
   s_S3NO_JP = "";           // S3�t���a��
   s_S4NO_JP = "";           // S4�t���a��

   s_S1NO_EN = "";           // S1�t���p��
   s_S2NO_EN = "";           // S2�t���p��
   s_S3NO_EN = "";           // S3�t���p��
   s_S4NO_EN = "";           // S4�t���p��

    //�@    �΍s���̏ꍇ
    //2019.07.25 ����w���������ύX_S
    ////CX-10D
    //if ( (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) ||
    ////LX-90D
    //     (s_HEADS_DATA[823].ToIntDef(0) == 21 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 90) ||
    ////YX-80
    //     (s_HEADS_DATA[823].ToIntDef(0) == 111&& s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 80)){
    if ( P_FL == 1 ) {
    //2019.07.25 ����w���������ύX_E
        //(A)   AB���]�̏ꍇ
        if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // AB�t
            //S1�t���a���@�@P_HEADS_DATA[76].TrimRight()
            s_S1NO_JP = P_HEADS_DATA[76].TrimRight();
            //S2�t���a���@�@P_HEADS_DATA[45].TrimRight()
            s_S2NO_JP = P_HEADS_DATA[45].TrimRight();
            //S3�t���a���@�@P_HEADS_DATA[45].TrimRight()
            s_S3NO_JP = P_HEADS_DATA[45].TrimRight();
            //S4�t���a���@�@P_HEADS_DATA[76].TrimRight()
            s_S4NO_JP = P_HEADS_DATA[76].TrimRight();
            //S1�t���p���@�@P_HEADS_DATA[135].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[135].TrimRight();
            //S2�t���p���@�@P_HEADS_DATA[134].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[134].TrimRight();
            //S3�t���p���@�@P_HEADS_DATA[134].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[134].TrimRight();
            //S4�t���p���@�@P_HEADS_DATA[135].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[135].TrimRight();
        }
        //(B)   ����ȊO
        else{
            //S1�t���a���@�@P_HEADS_DATA[45].TrimRight()
            s_S1NO_JP = P_HEADS_DATA[45].TrimRight();
            //S2�t���a���@�@P_HEADS_DATA[76].TrimRight()
            s_S2NO_JP = P_HEADS_DATA[76].TrimRight();
            //S3�t���a���@�@P_HEADS_DATA[76].TrimRight()
            s_S3NO_JP = P_HEADS_DATA[76].TrimRight();
            //S4�t���a���@�@P_HEADS_DATA[45].TrimRight()
            s_S4NO_JP = P_HEADS_DATA[45].TrimRight();
            //S1�t���p���@�@P_HEADS_DATA[134].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[134].TrimRight();
            //S2�t���p���@�@P_HEADS_DATA[135].TrimRight()
            s_S2NO_EN = P_HEADS_DATA[135].TrimRight();
            //S3�t���p���@�@P_HEADS_DATA[135].TrimRight()
            s_S3NO_EN = P_HEADS_DATA[135].TrimRight();
            //S4�t���p���@�@P_HEADS_DATA[134].TrimRight()
            s_S4NO_EN = P_HEADS_DATA[134].TrimRight();
        }
    }
    //�A    ����ȊO
    else{
        //(A)   AB���]�̏ꍇ
        if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // AB�t
            //S1�t���a���@�@P_HEADS_DATA[76].TrimRight()
            s_S1NO_JP = P_HEADS_DATA[76].TrimRight();
            //S2�t���a���@�@P_HEADS_DATA[45].TrimRight()
            s_S2NO_JP = P_HEADS_DATA[45].TrimRight();
            //S3�t���a���@�@P_HEADS_DATA[76].TrimRight()
            s_S3NO_JP = P_HEADS_DATA[76].TrimRight();
            //S4�t���a���@�@P_HEADS_DATA[45].TrimRight()
            s_S4NO_JP = P_HEADS_DATA[45].TrimRight();
            //S1�t���p���@�@P_HEADS_DATA[135].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[135].TrimRight();
            //S2�t���p���@�@P_HEADS_DATA[134].TrimRight()
            s_S2NO_EN = P_HEADS_DATA[134].TrimRight();
            //S3�t���p���@�@P_HEADS_DATA[135].TrimRight()
            s_S3NO_EN = P_HEADS_DATA[135].TrimRight();
            //S4�t���p���@�@P_HEADS_DATA[134].TrimRight()
            s_S4NO_EN = P_HEADS_DATA[134].TrimRight();
        }
        //(B)   ����ȊO
        else{
            //S1�t���a���@�@P_HEADS_DATA[45].TrimRight()
            s_S1NO_JP = P_HEADS_DATA[45].TrimRight();
            //S2�t���a���@�@P_HEADS_DATA[76].TrimRight()
            s_S2NO_JP = P_HEADS_DATA[76].TrimRight();
            //S3�t���a���@�@P_HEADS_DATA[45].TrimRight()
            s_S3NO_JP = P_HEADS_DATA[45].TrimRight();
            //S4�t���a���@�@P_HEADS_DATA[76].TrimRight()
            s_S4NO_JP = P_HEADS_DATA[76].TrimRight();
            //S1�t���p���@�@P_HEADS_DATA[134].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[134].TrimRight();
            //S2�t���p���@�@P_HEADS_DATA[135].TrimRight()
            s_S2NO_EN = P_HEADS_DATA[135].TrimRight();
            //S3�t���p���@�@P_HEADS_DATA[134].TrimRight()
            s_S3NO_EN = P_HEADS_DATA[134].TrimRight();
            //S4�t���p���@�@P_HEADS_DATA[135].TrimRight()
            s_S4NO_EN = P_HEADS_DATA[135].TrimRight();
        }
    }

   //***************************
   //***    S1�m�Y���I���G���e�[�V����
   //***************************
   // �wG007�x

    //�@    S1�m�Y�������̏ꍇ
    //2019.07.25 ����w���������ύX_S
    //if(s_HEADS_DATA[1136].TrimRight() == "1"){
    if( s_HEADS_DATA[891].TrimRight() == "1" ) {
    //2019.07.25 ����w���������ύX_E
        //�a���̏ꍇ
        if(P_HEADS_DATA[1].TrimRight() == "0") { // �a��
            //S1�t���a�� + "�p ����"���o�͈ʒuG007�ɏo�͂���
            // 2020.10.30 �I���G���e�[�V�����ύX_S
            //s_Text = s_S1NO_JP + "�p ����";
            s_Text = s_S1NO_JP + " ����";
            // 2020.10.30 �I���G���e�[�V�����ύX_E
        }
        //���̑��̏ꍇ
        else{
            //S1�t���p�� + " IN"���o�͈ʒuG007�ɏo�͂���
            s_Text = s_S1NO_EN + " IN";
        }
    }
    //�A    S1�m�Y���o���̏ꍇ
    //2019.07.25 ����w���������ύX_S
    //else if( s_HEADS_DATA[1136].TrimRight() == "2" ) {
    else if( s_HEADS_DATA[891].TrimRight() == "2" ) {
    //2019.07.25 ����w���������ύX_E
        //�a���̏ꍇ
        if (P_HEADS_DATA[1].TrimRight() == "0") { // �a��
            //S1�t���a�� + "�p �o��"���o�͈ʒuG007�ɏo�͂���
            // 2020.10.30 �I���G���e�[�V�����ύX_S
            //s_Text = s_S1NO_JP + "�p �o��";
            s_Text = s_S1NO_JP + " �o��";
            // 2020.10.30 �I���G���e�[�V�����ύX_E
        }
        //���̑��̏ꍇ
        else{
            //S1�t���p�� + " OUT"���o�͈ʒuG007�ɏo�͂���
            s_Text = s_S1NO_EN + " OUT";
        }
    }
    //�B    ����ȊO
    else{
        //""���o�͈ʒuG007�ɏo�͂���
        s_Text = "";
    }
    Excel_Cell_DataSet(51, 37, s_Text);

   //***************************
   //***    S2�m�Y���I���G���e�[�V����
   //***************************
   // �wG008�x

    //�@    S2�m�Y�������̏ꍇ
    //2019.07.25 ����w���������ύX_S
    //if ( s_HEADS_DATA[1137].TrimRight() == "1" ) {
    if( s_HEADS_DATA[906].TrimRight() == "1" ) {
    //2019.07.25 ����w���������ύX_E
    //�a���̏ꍇ
        if (P_HEADS_DATA[1].TrimRight() == "0") { // �a��
            //S2�t���a�� + "�p ����"���o�͈ʒuG008�ɏo�͂���
            // 2020.10.30 �I���G���e�[�V�����ύX_S
            //s_Text = s_S2NO_JP + "�p ����";
            s_Text = s_S2NO_JP + " ����";
            // 2020.10.30 �I���G���e�[�V�����ύX_E

        }
        //���̑��̏ꍇ
        else{
            //S2�t���p�� + " IN"���o�͈ʒuG008�ɏo�͂���
            s_Text = s_S2NO_EN + " IN";
        }
    }
    //�A    S2�m�Y���o���̏ꍇ
    //2019.07.25 ����w���������ύX_S
    //else if ( s_HEADS_DATA[1137].TrimRight() == "2" ) {
    else if( s_HEADS_DATA[906].TrimRight() == "2" ) {
    //2019.07.25 ����w���������ύX_E
    //�a���̏ꍇ
        if (P_HEADS_DATA[1].TrimRight() == "0") { // �a��
            //S2�t���a�� + "�p �o��"���o�͈ʒuG008�ɏo�͂���
            // 2020.10.30 �I���G���e�[�V�����ύX_S
            //s_Text = s_S2NO_JP + "�p �o��";
            s_Text = s_S2NO_JP + " �o��";
            // 2020.10.30 �I���G���e�[�V�����ύX_E
        }
        //���̑��̏ꍇ
        else{
            //S2�t���p�� + " OUT"���o�͈ʒuG008�ɏo�͂���
            s_Text = s_S2NO_EN + " OUT";
        }
    }
    //�B    ����ȊO
    else{
        //""���o�͈ʒuG008�ɏo�͂���
        s_Text = "";
    }
    Excel_Cell_DataSet(51, 45, s_Text);


   //***************************
   //***    S3�m�Y���I���G���e�[�V����
   //***************************
   // �wG009�x

    //�@    S3�m�Y�������̏ꍇ
    //2019.07.25 ����w���������ύX_S
    //if ( s_HEADS_DATA[1138].TrimRight() == "1" ) {
    if( s_HEADS_DATA[921].TrimRight() == "1" ) {
    //2019.07.25 ����w���������ύX_E
    //�a���̏ꍇ
        if (P_HEADS_DATA[1].TrimRight() == "0") { // �a��
            //S3�t���a�� + "�p ����"���o�͈ʒuG009�ɏo�͂���
            // 2020.10.30 �I���G���e�[�V�����ύX_S
            //s_Text = s_S3NO_JP + "�p ����";
            s_Text = s_S3NO_JP + " ����";
            // 2020.10.30 �I���G���e�[�V�����ύX_E
        }
        //���̑��̏ꍇ
        else{
            //S3�t���p�� + " IN"���o�͈ʒuG009�ɏo�͂���
            s_Text = s_S3NO_EN + " IN";
        }
    }
    //�A    S3�m�Y���o���̏ꍇ
    //2019.07.25 ����w���������ύX_S
    //else if ( s_HEADS_DATA[1138].TrimRight() == "2" ) {
    else if( s_HEADS_DATA[921].TrimRight() == "2" ) {
    //2019.07.25 ����w���������ύX_E
    //�a���̏ꍇ
        if (P_HEADS_DATA[1].TrimRight() == "0") { // �a��
            //S3�t���a�� + "�p �o��"���o�͈ʒuG009�ɏo�͂���
            // 2020.10.30 �I���G���e�[�V�����ύX_S
            //s_Text = s_S3NO_JP + "�p �o��";
            s_Text = s_S3NO_JP + " �o��";
            // 2020.10.30 �I���G���e�[�V�����ύX_E
        }
        //���̑��̏ꍇ
        else{
            //S3�t���p�� + " OUT"���o�͈ʒuG009�ɏo�͂���
            s_Text = s_S3NO_EN + " OUT";
        }
    }
    //�B    ����ȊO
    else{
        //""���o�͈ʒuG009�ɏo�͂���
        s_Text = "";
    }
    Excel_Cell_DataSet(52, 37, s_Text);


   //***************************
   //***    S4�m�Y���I���G���e�[�V����
   //***************************
   // �wG010�x

    //�@    S4�m�Y�������̏ꍇ
    //2019.07.25 ����w���������ύX_S
    //if ( s_HEADS_DATA[1139].TrimRight() == "1" ) {
    if( s_HEADS_DATA[936].TrimRight() == "1" ) {
    //2019.07.25 ����w���������ύX_E
    //�a���̏ꍇ
        if (P_HEADS_DATA[1].TrimRight() == "0") { // �a��
            //S4�t���a�� + "�p ����"���o�͈ʒuG010�ɏo�͂���
            // 2020.10.30 �I���G���e�[�V�����ύX_S
            //s_Text = s_S4NO_JP + "�p ����";
            s_Text = s_S4NO_JP + " ����";
            // 2020.10.30 �I���G���e�[�V�����ύX_E
        }
        //���̑��̏ꍇ
        else{
            //S4�t���p�� + " IN"���o�͈ʒuG010�ɏo�͂���
            s_Text = s_S4NO_EN + " IN";
        }
    }
    //�A    S4�m�Y���o���̏ꍇ
    //2019.07.25 ����w���������ύX_S
    //else if ( s_HEADS_DATA[1139].TrimRight() == "2" ) {
    else if( s_HEADS_DATA[936].TrimRight() == "2" ) {
    //2019.07.25 ����w���������ύX_E
    //�a���̏ꍇ
        if (P_HEADS_DATA[1].TrimRight() == "0") { // �a��
            //S4�t���a�� + "�p �o��"���o�͈ʒuG010�ɏo�͂���
            // 2020.10.30 �I���G���e�[�V�����ύX_S
            //s_Text = s_S4NO_JP + "�p �o��";
            s_Text = s_S4NO_JP + " �o��";
            // 2020.10.30 �I���G���e�[�V�����ύX_E
        }
        //���̑��̏ꍇ
        else{
            //S4�t���p�� + " OUT"���o�͈ʒuG010�ɏo�͂���
            s_Text = s_S4NO_EN + " OUT";
        }
    }
    //�B    ����ȊO
    else{
        //""���o�͈ʒuG010�ɏo�͂���
        s_Text = "";
    }
    Excel_Cell_DataSet(52, 45, s_Text);

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                        �� ����w�����@��r����                      ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //2004.12.15 REV00�̏ꍇ�̒�����ύX
   ////2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   ////// ���߂̐���w�������쐬���Ă���A��޼ޮݔԍ��̎擾
   ////d_RevNo = Sheet_Exist_Check(G_xls_Output_Dir);
   ////if (d_RevNo.Trim() != ""){
   ////   // �O��̧���߽
   ////   s_Text = G_xls_Output_Dir + d_RevNo + "\\MAN" + G_SAKUBAN + ".xls";
   ////   Sheet_Compare_Syori(s_Text);
   ////}
   //// ���߂̐���w�������쐬���Ă���A��޼ޮݔԍ��̎擾
   //d_RevNo=Sheet_Exist_Check(G_xls_Output_Dir_old.SubString(1,G_xls_Output_Dir_old.LastDelimiter("\\")));
   //if ((d_RevNo.Trim() != "-01") && (d_RevNo.Trim() != "")){
   //   // �O��̧���߽
   //   s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + ".xls";
   //   Write_Log("!?!" +  s_Text);
   //   Sheet_Compare_Syori(s_Text);
   //}
   //else if (d_RevNo.Trim() == "-01"){
   //   s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + ".xls";
   //   Sheet_Compare_Syori(s_Text);
   //}
   ////*************************************
   //2023.02.08 ��Ԍ����ǉ�_S
   AnsiString OLD_SAKUBAN;         // �U��������
   AnsiString  Work,Rev;
   AnsiString xls_Output_Dir_old;  // �U������o�̓f�B���N�g��
   int i_Sakuban;
   //2023.02.08 ��Ԍ����ǉ�_E

   if (s_HEADS_DATA[319].TrimRight() == "0" || s_HEADS_DATA[319].TrimRight() == "00") {
   } else {
       d_RevNo=Sheet_Exist_CheckA3(G_xls_Output_Dir_old.SubString(1,G_xls_Output_Dir_old.LastDelimiter("\\")), pcHeadsData);
       if ((d_RevNo.Trim() != "-01") && (d_RevNo.Trim() != "")){
          // �O��̧���߽
		  //2023/4/21 �����ԍ������ǉ��Ή� Mod
		  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
		  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
          if (!FileExists(s_Text.c_str()))
          {
			  //2023/4/21 �����ԍ������ǉ��Ή� Mod
			  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + ".xls";
			  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
          }
          Write_Log("!?!" +  s_Text);
          //2023.02.08 ��Ԍ����ǉ�_S
          bFind = true;
          //2023.02.08 ��Ԍ����ǉ�_E
          Sheet_Compare_SyoriA3(s_Text);
       }
       else if (d_RevNo.Trim() == "-01"){
          d_RevNo = FormatFloat("00", G_HEADS_DATA_Rev - 1);
		  //2023/4/21 �����ԍ������ǉ��Ή� Mod
		  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
		  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
          bFlag = false;
          if (!FileExists(s_Text.c_str())){
			  //2023/4/21 �����ԍ������ǉ��Ή� Mod
			  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + ".xls";
			  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
              if (FileExists(s_Text.c_str())){
                  bFlag = true;
              }
          } else {
              bFlag = true;
          }

          bFind = false;
          if (!bFlag){
              while(d_RevNo.ToIntDef(0) >= 0 && bFind == false) {
                  // ̧�ّ�������
				  //2023/4/21 �����ԍ������ǉ��Ή� Mod
				  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_�o�^\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
				  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_�o�^\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
                  if (!FileExists(s_Text.c_str())){
					  //2023/4/21 �����ԍ������ǉ��Ή� Mod
					  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_�o�^\\MAN" + G_SAKUBAN + s_SpecCode + ".xls";
					  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_�o�^\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
                      if (FileExists(s_Text.c_str())){
                          bFlag = true;
                      }
                  } else {
                      bFlag = true;
                  }

                  if (bFlag){
                      G_Log_Text = "s_Text?�w " + s_Text + " �x";
                      Write_Log(G_Log_Text);
                      bFind = true;
                  } else {
                      d_RevNo = FormatFloat("00",d_RevNo.ToIntDef(0) - 1);
                  }
              }
          } else {
              G_Log_Text = "s_Text?�w " + s_Text + " �x";
              Write_Log(G_Log_Text);
              bFind = true;
          }
          if (bFind) {
              Sheet_Compare_SyoriA3(s_Text);
          }
       }

       //2023.02.08 ��Ԍ����ǉ�_S
       if( (!bFind) && (G_SAKUBAN.Length() > 7) && (G_SAKUBAN.SubString(1, 2) == "10") ) {
          //�O�񕔕i�W�J�ް��Ǎ��Ɏ��s���W���œ��͂��ꂽ�U������O�񕔕i�W�J�ް��Ǎ�

          //********************************
          //**  �U������o�̓f�B���N�g��
          //********************************
          OLD_SAKUBAN = G_SAKUBAN.SubString(3, 6);

          Work = OLD_SAKUBAN.SubString(1, 4);
          i_Sakuban = Work.ToIntDef(0);
          if( i_Sakuban % 2 == 1 ){
             i_Sakuban --;
          }
          Work.sprintf("%04d",i_Sakuban);

          if (G_HEADS_DATA_Rev - 1 <= 0 ) {
             Rev.sprintf("%02d", 0);
          }else{
             Rev.sprintf("%02d",G_HEADS_DATA_Rev - 1);
          }

          xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + OLD_SAKUBAN + "\\";

          d_RevNo=Sheet_Exist_CheckA3(xls_Output_Dir_old, pcHeadsData);
          if ((d_RevNo.Trim() != "-01") && (d_RevNo.Trim() != "")){
             // �O��̧���߽
			 //2023/4/21 �����ԍ������ǉ��Ή� Mod
			 //s_Text = xls_Output_Dir_old + d_RevNo + "\\MAN" + OLD_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
			 s_Text = xls_Output_Dir_old + d_RevNo + "\\MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
             if (!FileExists(s_Text.c_str()))
             {
				//2023/4/21 �����ԍ������ǉ��Ή� Mod
				//s_Text = xls_Output_Dir_old + d_RevNo + "\\MAN" + OLD_SAKUBAN + s_SpecCode + ".xls";
				s_Text = xls_Output_Dir_old + d_RevNo + "\\MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + ".xls";
             }
             Write_Log("!?!" +  s_Text);
             bFind = true;
             Sheet_Compare_SyoriA3(s_Text);
          }
          else if (d_RevNo.Trim() == "-01"){
             d_RevNo = FormatFloat("00", G_HEADS_DATA_Rev - 1);
			 //2023/4/21 �����ԍ������ǉ��Ή� Mod
			 //s_Text = xls_Output_Dir_old + "MAN" + OLD_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
			 s_Text = xls_Output_Dir_old + "MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
             bFlag = false;
             if (!FileExists(s_Text.c_str())){
				//2023/4/21 �����ԍ������ǉ��Ή� Mod
				//s_Text = xls_Output_Dir_old + "MAN" + OLD_SAKUBAN + s_SpecCode + ".xls";
				s_Text = xls_Output_Dir_old + "MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + ".xls";
                if (FileExists(s_Text.c_str())){
                   bFlag = true;
                }
             } else {
                bFlag = true;
             }

             bFind = false;
             if (!bFlag){
                while(d_RevNo.ToIntDef(0) >= 0 && bFind == false) {
                   // ̧�ّ�������
				   //2023/4/21 �����ԍ������ǉ��Ή� Mod
				   //s_Text = xls_Output_Dir_old + "\\000_�o�^\\MAN" + OLD_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
				   s_Text = xls_Output_Dir_old + "\\000_�o�^\\MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
                   if (!FileExists(s_Text.c_str())){
					   //2023/4/21 �����ԍ������ǉ��Ή� Mod
					   //s_Text = xls_Output_Dir_old + "\\000_�o�^\\MAN" + OLD_SAKUBAN + s_SpecCode + ".xls";
					   s_Text = xls_Output_Dir_old + "\\000_�o�^\\MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + ".xls";
                      if (FileExists(s_Text.c_str())){
                          bFlag = true;
                      }
				   } else {
                      bFlag = true;
				   }

                   if (bFlag){
                      G_Log_Text = "s_Text?�w " + s_Text + " �x";
                      Write_Log(G_Log_Text);
                      bFind = true;
                   } else {
                      d_RevNo = FormatFloat("00",d_RevNo.ToIntDef(0) - 1);
                   }
                }
             } else {
                G_Log_Text = "s_Text?�w " + s_Text + " �x";
                Write_Log(G_Log_Text);
                bFind = true;
             }
             if (bFind) {
                Sheet_Compare_SyoriA3(s_Text);
             }

          }

       }
       //2023.02.08 ��Ԍ����ǉ�_E
   }
   //**********************************

   //Windows 2008Server �ڐA�Ή�
   wkQuery->Close();
   delete wkQuery;
   //******************************
   //***  Excel Book Close
   //******************************
   //PDF�t�@�C���o�͎��
   //exApplication.Exec(Function("Run") << "Gsub_PDFOut");

   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   //Windows 2008Server �ڐA�Ή�
   exApplication.OlePropertySet("DisplayAlerts", false);
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w����w�����x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   //Windows 2008Server �ڐA�Ή�
   //delete wkQuery;


   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F �ŐV����w�����A���ߐ���w������r����
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F
// ��  �l      �F
//---------------------------------------------------------------------------
void __fastcall Sheet_Compare_SyoriA3(AnsiString Excel_OldPass){
   AnsiString s_MakeExcel_Pass;

   s_MakeExcel_Pass = G_xls_Output_Dir + "HIKAKU.xls"; // ��߰��̧��

   //******************************
   //***  �e���v���[�g�@�R�s�[
   //******************************
   if (!Template_Copy(Excel_OldPass,s_MakeExcel_Pass)){
      // �װ۸ޒǉ�
      G_ErrLog_Text = "�t�@�C���R�s�[�����s���܂����B�w" + Excel_OldPass +
                      "�x���w" + s_MakeExcel_Pass + "�x";
      Write_Error_Log(G_ErrLog_Text);

      return;
   } else {
      G_Log_Text = "�t�@�C���R�s�[�����B �w" + Excel_OldPass +
                      "�x���w" + s_MakeExcel_Pass + "�x";
      Write_Log(G_Log_Text);
   }

   // ̧�ٵ����
   //exWorkbook_old  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
   //Windows 2008Server �ڐA�Ή�
   exWorkbook_old  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
   // ��ĺڸ��ݎ擾
   //exWorksheets_old = exWorkbook_old.Exec(PropertyGet("WorkSheets"));
   //Windows 2008Server �ڐA�Ή�
   exWorksheets_old = exWorkbook_old.OlePropertyGet("WorkSheets");
   // ��Ă̎w��
   //exWorksheet_old = exWorksheets_old.Exec(PropertyGet("Item") << EXCEL_SHEETNAME);
   //Windows 2008Server �ڐA�Ή�
   exWorksheet_old = exWorksheets_old.OlePropertyGet("item",OleVariant(EXCEL_SHEETNAME));
   //***************************************************************************
   //***                                                                     ***
   //***                       ����w�����@��r����                          ***
   //***                                                                     ***
   //***************************************************************************

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          �`���@�@�@��r                             ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //*********************
   //***  �^�@��
   //*********************
   // �wA008�x
   if(!Sheet_Cell_CompareA3(6,5)){
      Excel_Cell_DataSet(6, 19, "*");
      // 2020.05.28 �����������L�ǉ�_S
      if ( Excel_Cell_DataGet (18, 36) == "" ) {
      } else if ( Excel_Cell_DataGet (18, 36) == "'" ) {
      } else {
         Excel_Cell_DataSet(18, 55, "*");
      }
      // 2020.05.28 �����������L�ǉ�_E
   }

   //***************************
   //***  �[�@��
   //***************************
   // �wA009�x
   if(!Sheet_Cell_CompareA3(5,23)){
      Excel_Cell_DataSet(5, 28, "*");
   }

   //*******************
   //***  ��  ��
   //*******************
   // �wA010�x
   if(!Sheet_Cell_CompareA3(6, 23)){
      Excel_Cell_DataSet(6, 28, "*");
      // 2020.05.28 �����������L�ǉ�_S
      if ( Excel_Cell_DataGet (18, 36) == "" ) {
      } else if ( Excel_Cell_DataGet (18, 36) == "'" ) {
      } else {
         Excel_Cell_DataSet(18, 55, "*");
      }
      // 2020.05.28 �����������L�ǉ�_E
   }

   //***************************
   //***  �� �� ��
   //***************************
   // �wA011�x
   if(!Sheet_Cell_CompareA3(7, 5)){
      Excel_Cell_DataSet(7, 26, "*");
   }

   //***************************
   //***  �h �� �F
   //***************************
   // �wA014�x
   if(!Sheet_Cell_CompareA3(8, 6)){
      Excel_Cell_DataSet(8, 26, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          �a���@�@��r                               ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //*******************************************
   //***  B����ڰĕҐ� 1 �� B����ڰĕҐ� 2
   //*******************************************
   // �wB003�x
   if(!Sheet_Cell_CompareA3(10, 7)){
      Excel_Cell_DataSet(10, 13, "*");
   }

   //*******************************************
   //***  A����ڰĕҐ� 1 �� A����ڰĕҐ� 2
   //*******************************************
   // �wB004�x
   if(!Sheet_Cell_CompareA3(10, 15)){
      Excel_Cell_DataSet(10, 26, "*");
   }

   //***************************
   //***  ��ڰčގ�
   //***************************
   // �wB005�x
   if(!Sheet_Cell_CompareA3(11, 7)){
      Excel_Cell_DataSet(11, 26, "*");
   }

   //***************************
   //***  B���޽��čގ�
   //***************************
   // �wB006�x
   if(!Sheet_Cell_CompareA3(12, 7)){
      Excel_Cell_DataSet(12, 13, "*");
   }

   //***************************
   //***  A���޽��čގ�
   //***************************
   // �wB007�x
   if(!Sheet_Cell_CompareA3(12, 15)){
      Excel_Cell_DataSet(12, 26, "*");
   }

   ////2011.03.11 �H���ʐ^�v���L�ǉ�
   ////***************************
   ////***  �H���ʐ^�v
   ////***************************
   //if(!Sheet_Cell_Compare(12, 18)){
   //   Excel_Cell_DataSet(12, 26, "*");
   //}
   //*****************************

   //***********************************
   //***  B��ɽ�ٍގ� �� B��ɽ�ِF
   //***********************************
   // �wB008�x,�wB009�x
   if(!Sheet_Cell_CompareA3(13, 7) || !Sheet_Cell_CompareA3(13, 11)){
      Excel_Cell_DataSet(13, 13, "*");
   }

   //***********************************
   //***  A��ɽ�ٍގ� �� A��ɽ�ِF
   //***********************************
   // �wB010�x,�wB011�x
   if(!Sheet_Cell_CompareA3(13, 15) || !Sheet_Cell_CompareA3(13, 24)){
      Excel_Cell_DataSet(13, 26, "*");
   }

   //***************************
   //***  S1�p��E���a
   //***************************
   // �wB012�x
   if(!Sheet_Cell_CompareA3(14, 8)){
      Excel_Cell_DataSet(14, 13, "*");
   }

   //***************************
   //***  S2�p��E���a
   //***************************
   // �wB013�x
   if(!Sheet_Cell_CompareA3(14, 16)){
      Excel_Cell_DataSet(14, 26, "*");
   }

   //***************************
   //***  S1���x�v
   //***  B��S1���x�v���ގ�
   //***************************
   // �wB014�x,�wB015�x
   if(!Sheet_Cell_CompareA3(15, 7) || !Sheet_Cell_CompareA3(15, 11)){
      Excel_Cell_DataSet(15, 13, "*");
   }

   //***************************
   //***  S2���x�v
   //***  A��S2���x�v���ގ�
   //***************************
   // �wB016�x,�wB017�x
   if(!Sheet_Cell_CompareA3(15, 15) || !Sheet_Cell_CompareA3(15, 24)){
      Excel_Cell_DataSet(15, 26, "*");
   }

   //***************************
   //***  S1���͌v
   //***************************
   // �wB018�x
   if(!Sheet_Cell_CompareA3(16, 7)){
      Excel_Cell_DataSet(16, 13, "*");
   }

   //***************************
   //***  S2���͌v
   //***************************
   // �wB019�x
   if(!Sheet_Cell_CompareA3(16, 15)){
      Excel_Cell_DataSet(16, 26, "*");
   }

   //***************************
   //***  S1�����p�薼�a
   //***************************
   // �wB020�x
   if(!Sheet_Cell_CompareA3(17, 7)){
      Excel_Cell_DataSet(17, 13, "*");
   }

   //***************************
   //***  S2�����p�薼�a
   //***************************
   // �wB021�x
   if(!Sheet_Cell_CompareA3(17, 15)){
      Excel_Cell_DataSet(17, 26, "*");
   }

   //***************************
   //***  S3�p��E���a
   //***************************
   // �wB022�x
   if(!Sheet_Cell_CompareA3(18, 8)){
      Excel_Cell_DataSet(18, 13, "*");
   }

   //***************************
   //***  S4�p��E���a
   //***************************
   // �wB023�x
   if(!Sheet_Cell_CompareA3(18, 16)){
      Excel_Cell_DataSet(18, 26, "*");
   }

   //***************************
   //***  S3���x�v
   //***  S3���ы敪
   //***************************
   // �wB024�x�wB025�x
   if(!Sheet_Cell_CompareA3(19, 7) || !Sheet_Cell_CompareA3(19, 11)){
      Excel_Cell_DataSet(19, 13, "*");
   }

   //***************************
   //***  S4���x�v
   //***  S4���ы敪
   //***************************
   // �wB026�x�wB027�x
   if(!Sheet_Cell_CompareA3(19, 15) || !Sheet_Cell_CompareA3(19, 24)){
      Excel_Cell_DataSet(19, 26, "*");
   }

   //***************************
   //***  S3���͌v
   //***************************
   // �wB028�x
   if(!Sheet_Cell_CompareA3(20, 7)){
      Excel_Cell_DataSet(20, 13, "*");
   }

   //***************************
   //***  S4���͌v
   //***************************
   // �wB029�x
   if(!Sheet_Cell_CompareA3(20, 15)){
      Excel_Cell_DataSet(20, 26, "*");
   }

   //***************************
   //***  S3�t�̌p�薼�a
   //***************************
   // �wB030�x
   if(!Sheet_Cell_CompareA3(21, 7)){
      Excel_Cell_DataSet(21, 13, "*");
   }

   //***************************
   //***  S4�t�̌p�薼�a
   //***************************
   // �wB031�x
   if(!Sheet_Cell_CompareA3(21, 15)){
      Excel_Cell_DataSet(21, 26, "*");
   }

   //***************************
   //***  B���[�Ǎގ�
   //***************************
   // �wB032�x
   if(!Sheet_Cell_CompareA3(22, 7)){
      Excel_Cell_DataSet(22, 13, "*");
   }

   //***************************
   //***  A���[�Ǎގ�
   //***************************
   // �wB033�x
   if(!Sheet_Cell_CompareA3(22, 15)){
      Excel_Cell_DataSet(22, 26, "*");
   }

   //***************************
   //***  B���޽��čގ�
   //***************************
   // �wB034�x
   if(!Sheet_Cell_CompareA3(23, 7)){
      Excel_Cell_DataSet(23, 13, "*");
   }

   //***************************
   //***  A���޽��čގ�
   //***************************
   // �wB035�x
   if(!Sheet_Cell_CompareA3(23, 15)){
      Excel_Cell_DataSet(23, 26, "*");
   }

   //***************************
   //***  B����ݼލގ�
   //***************************
   // �wB036�x
   if(!Sheet_Cell_CompareA3(24, 7)){
      Excel_Cell_DataSet(24, 13, "*");
   }

   //***************************
   //***  A����ݼލގ�
   //***************************
   // �wB037�x
   if(!Sheet_Cell_CompareA3(24, 15)){
      Excel_Cell_DataSet(24, 26, "*");
   }

   //***************************
   //***  B���d�㖼��
   //***************************
   // �wB039�x
   if(!Sheet_Cell_CompareA3(26, 7)){
      Excel_Cell_DataSet(26, 13, "*");
   }

   //***************************
   //***  A���d�㖼��
   //***************************
   // �wB040�x
   if(!Sheet_Cell_CompareA3(26, 15)){
      Excel_Cell_DataSet(26, 26, "*");
   }

   //***************************
   //***  B���ی�Ǎގ�
   //***************************
   // �wB041�x
   if(!Sheet_Cell_CompareA3(25, 7)){
      Excel_Cell_DataSet(25, 13, "*");
   }

   //***************************
   //***  A���ی�Ǎގ�
   //***************************
   // �wB042�x
   if(!Sheet_Cell_CompareA3(25, 15)){
      Excel_Cell_DataSet(25, 26, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          �b���@�@��r                               ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //**********************************************************************
   //***  ���t�{���g�ގ�
   //**********************************************************************
   // �wC001�x,�wC002�x,�wC003�x,
   if(!Sheet_Cell_CompareA3(27, 8) || !Sheet_Cell_CompareA3(27, 12) || !Sheet_Cell_CompareA3(27, 24)){
      Excel_Cell_DataSet(27, 26, "*");
   }

   //**********************************************************************
   //***  ���t�{���g�d�グ
   //**********************************************************************
   // �wC004�x,�wC005�x,�wC006�x,
   if(!Sheet_Cell_CompareA3(28, 7) || !Sheet_Cell_CompareA3(28, 12) || !Sheet_Cell_CompareA3(28, 24)){
      Excel_Cell_DataSet(28, 26, "*");
   }

   //*******************************
   //***  �ڰэގ� �ގ�,����,��
   //*******************************
   // �wC007�x,�wC008�x,�wC009�x
   if(!Sheet_Cell_CompareA3(32, 8) || !Sheet_Cell_CompareA3(32, 15) || !Sheet_Cell_CompareA3(32, 22)){
      Excel_Cell_DataSet(32, 26, "*");
   }

   //*******************************
   //***  �K�C�h�o�[
   //*******************************
   // �wC010�x,�wC011�x,�wC012�x,�wC013�x,�wC014�x
   if(!Sheet_Cell_CompareA3(34, 7) || !Sheet_Cell_CompareA3(34, 10) || !Sheet_Cell_CompareA3(34, 11) ||
      !Sheet_Cell_CompareA3(34, 12)|| !Sheet_Cell_CompareA3(34, 16)){
      Excel_Cell_DataSet(34, 26, "*");
   }

   //*******************************
   //***  �K�C�h�o�[
   //*******************************
   // �wC015�x,�wC016�x,�wC017�x
   if(!Sheet_Cell_CompareA3(35, 7) || !Sheet_Cell_CompareA3(35, 18) || !Sheet_Cell_CompareA3(35, 24)){
      Excel_Cell_DataSet(35, 26, "*");
   }

   //*******************************
   //***  �{���g�J�o�[BLCP1
   //*******************************
   // �wC018�x,�wC019�x,�wC020�x
   if(!Sheet_Cell_CompareA3(29, 7) || !Sheet_Cell_CompareA3(29, 10) || !Sheet_Cell_CompareA3(29, 24)){
      Excel_Cell_DataSet(29, 26, "*");
   }

   //*******************************
   //***  �{���g�J�o�[BLCEL
   //*******************************
   // �wC021�x,�wC022�x
   if(!Sheet_Cell_CompareA3(30, 10) || !Sheet_Cell_CompareA3(30, 24)){
      Excel_Cell_DataSet(30, 26, "*");
   }

   //*******************************
   //***  �{���g�J�o�[BLCES
   //*******************************
   // �wC023�x,�wC024�x
   if(!Sheet_Cell_CompareA3(31, 10) || !Sheet_Cell_CompareA3(31, 24)){
      Excel_Cell_DataSet(31, 26, "*");
   }

   //*******************************
   //***  �A���J�[�{���g���
   //*******************************
   // �wC025�x
   if(!Sheet_Cell_CompareA3(36, 8)){
      Excel_Cell_DataSet(36, 26, "*");
   }

   //*******************************
   //***  �A���J�[�{���g����,����,�ގ�,�d��
   //*******************************
   // �wC026�x,�wC027�x,�wC028�x
   if(!Sheet_Cell_CompareA3(37, 8) || !Sheet_Cell_CompareA3(37, 15) || !Sheet_Cell_CompareA3(37, 23)){
      Excel_Cell_DataSet(37, 26, "*");
   }

   //*******************************
   //***  S�ڰэE��
   //*******************************
   // �wC029�x
   if(!Sheet_Cell_CompareA3(33, 1)){
      Excel_Cell_DataSet(33, 26, "*");
   }

   //*******************************
   //***  �H���ʐ^�v
   //*******************************
   // �wC030�x
   if(!Sheet_Cell_CompareA3(40, 18)){
      Excel_Cell_DataSet(40, 26, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          �c���@�@��r                               ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  ���u�ԍ��^�D��
   //***************************
   // �wD001�x
   if(!Sheet_Cell_CompareA3(42, 9)){
      Excel_Cell_DataSet(42, 13, "*");
   }

   //***************************
   //***  �@��ԍ�
   //***************************
   // �wD002�x
   if(!Sheet_Cell_CompareA3(42, 18)){
      Excel_Cell_DataSet(42, 26, "*");
   }

   //***************************
   //***  B���݌v����
   //***************************
   // �wD003�x
   if(!Sheet_Cell_CompareA3(44, 6)){
      Excel_Cell_DataSet(44, 13, "*");
   }

   //***************************
   //***  B���݌v���x
   //***************************
   // �wD004�x
   if(!Sheet_Cell_CompareA3(45, 6)){
      Excel_Cell_DataSet(45, 13, "*");
   }

   //***************************
   //***  B���������� ��
   //***************************
   // �wD005�x
   if(!Sheet_Cell_CompareA3(46, 6)){
      Excel_Cell_DataSet(46, 13, "*");
   }

   //***************************
   //***  B���������� ����
   //***************************
   // �wD006�x
   if(!Sheet_Cell_CompareA3(47, 6)){
      Excel_Cell_DataSet(47, 13, "*");
   }

   //***************************
   //***  B�����e��
   //***************************
   // �wD007�x
   if(!Sheet_Cell_CompareA3(49, 6)){
      Excel_Cell_DataSet(49, 13, "*");
   }

   //***************************
   //***  �`�M�ʐ�
   //***************************
   // �wD008�x
   if(!Sheet_Cell_CompareA3(50, 6)){
      Excel_Cell_DataSet(50, 13, "*");
   }

   //***************************
   //***  A���݌v����
   //***************************
   // �wD009�x
   if(!Sheet_Cell_CompareA3(44, 14)){
      Excel_Cell_DataSet(44, 26, "*");
   }

   //***************************
   //***  A���݌v���x
   //***************************
   // �wD010�x
   if(!Sheet_Cell_CompareA3(45, 14)){
      Excel_Cell_DataSet(45, 26, "*");
   }

   //***************************
   //***  A���������� ��
   //***************************
   // �wD011�x
   if(!Sheet_Cell_CompareA3(46, 14)){
      Excel_Cell_DataSet(46, 26, "*");
   }

   //***************************
   //***  A���������� ����
   //***************************
   // �wD012�x
   if(!Sheet_Cell_CompareA3(47, 14)){
      Excel_Cell_DataSet(47, 26, "*");
   }

   //***************************
   //***  A�����e��
   //***************************
   // �wD013�x
   if(!Sheet_Cell_CompareA3(49, 14)){
      Excel_Cell_DataSet(49, 26, "*");
   }

   //***************************
   //***  �@�펿��
   //***************************
   // �wD014�x
   if(!Sheet_Cell_CompareA3(50, 17)){
      Excel_Cell_DataSet(50, 26, "*");
   }

   //************************
   //***  �K�p�K�i
   //************************
   // �wD015�x
   if(!Sheet_Cell_CompareA3(51, 6)){
      Excel_Cell_DataSet(51, 26, "*");
   }

   //***************************
   //***  ���t���@�@Max,Min
   //***************************
   // �wD016�x,�wD017�x
   if(!Sheet_Cell_CompareA3(52, 9) || !Sheet_Cell_CompareA3(52, 15)){
      Excel_Cell_DataSet(52, 26, "*");
   }

   //***************************
   //***  B������
   //***************************
   // �wD018�x,�wD019�x,�wD020�x
   if(!Sheet_Cell_CompareA3(48, 4) || !Sheet_Cell_CompareA3(48, 6) || !Sheet_Cell_CompareA3(48, 11)){
      Excel_Cell_DataSet(48, 13, "*");
   }

   //***************************
   //***  A������
   //***************************
   // �wD021�x,�wD022�x
   if(!Sheet_Cell_CompareA3(48, 14) || !Sheet_Cell_CompareA3(48, 24)){
      Excel_Cell_DataSet(48, 26, "*");
   }

   //***************************
   //***  �X�e�b�J�[
   //***************************
   // �wD023�x,�wD024�x
   if(!Sheet_Cell_CompareA3(53, 18) || !Sheet_Cell_CompareA3(53, 24)){
      Excel_Cell_DataSet(53, 26, "*");
   }

   //************************
   //***  �@�햼��
   //************************
   // �wD025�x
   if(!Sheet_Cell_CompareA3(41, 9)){
      Excel_Cell_DataSet(41, 26, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          E���@�@��r                                ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //************************
   //***  S�t���[���A�C�v���[�g�t
   //************************
   // �wE001�x
   if(!Sheet_Cell_CompareA3(2, 36)){
      Excel_Cell_DataSet(2, 55, "*");
      /*
      Excel_Cell_SetUnderline(2, 36);
      Excel_Cell_SetUnderline(2, 35);
      Excel_Cell_SetUnderline(2, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(2, 30);
      Excel_Cell_SetUnderline(2, 32);
      Excel_Cell_SetUnderline(2, 56);
      Excel_Cell_SetUnderline(2, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(2, 30, 2, 57);
   }

   //************************
   //***  ���̖����t
   //************************
   // �wE002�x
   if(!Sheet_Cell_CompareA3(3, 36)){
      Excel_Cell_DataSet(3, 55, "*");
      /*
      Excel_Cell_SetUnderline(3, 36);
      Excel_Cell_SetUnderline(3, 35);
      Excel_Cell_SetUnderline(3, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(3, 30);
      Excel_Cell_SetUnderline(3, 32);
      Excel_Cell_SetUnderline(3, 56);
      Excel_Cell_SetUnderline(3, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(3, 30, 3, 57);
   }

   //************************
   //***  ���L(�֖��P)
   //************************
   // �wE003�x
   if(!Sheet_Cell_CompareA3(4, 36)){
      Excel_Cell_DataSet(4, 55, "*");
      /*Excel_Cell_SetUnderline(4, 36);
      Excel_Cell_SetUnderline(4, 35);
      Excel_Cell_SetUnderline(4, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(4, 30);
      Excel_Cell_SetUnderline(4, 32);
      Excel_Cell_SetUnderline(4, 56);
      Excel_Cell_SetUnderline(4, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(4, 30, 4, 57);
   }

   //************************
   //***  ���L(ɽ�قP)
   //************************
   // �wE004�x
   if(!Sheet_Cell_CompareA3(5, 36)){
      Excel_Cell_DataSet(5, 55, "*");
      /*Excel_Cell_SetUnderline(5, 36);
      Excel_Cell_SetUnderline(5, 35);
      Excel_Cell_SetUnderline(5, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(5, 30);
      Excel_Cell_SetUnderline(5, 32);
      Excel_Cell_SetUnderline(5, 56);
      Excel_Cell_SetUnderline(5, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(5, 30, 5, 57);
   }

   //************************
   //***  ���L(ɽ�قQ)
   //************************
   // �wE005�x
   if(!Sheet_Cell_CompareA3(6, 36)){
      Excel_Cell_DataSet(6, 55, "*");
      /*
      Excel_Cell_SetUnderline(6, 36);
      Excel_Cell_SetUnderline(6, 35);
      Excel_Cell_SetUnderline(6, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(6, 30);
      Excel_Cell_SetUnderline(6, 32);
      Excel_Cell_SetUnderline(6, 56);
      Excel_Cell_SetUnderline(6, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(6, 30, 6, 57);
   }

   //************************
   //***  ���L(�q��d�l�P)
   //************************
   // �wE006�x
   if(!Sheet_Cell_CompareA3(7, 36)){
      Excel_Cell_DataSet(7, 55, "*");
      /*
      Excel_Cell_SetUnderline(7, 36);
      Excel_Cell_SetUnderline(7, 35);
      Excel_Cell_SetUnderline(7, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(7, 30);
      Excel_Cell_SetUnderline(7, 32);
      Excel_Cell_SetUnderline(7, 56);
      Excel_Cell_SetUnderline(7, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(7, 30, 7, 57);
   }

   //************************
   //***  ���L(�q��d�l�Q)
   //************************
   // �wE007�x
   if(!Sheet_Cell_CompareA3(8, 36)){
      Excel_Cell_DataSet(8, 55, "*");
      /*
      Excel_Cell_SetUnderline(8, 36);
      Excel_Cell_SetUnderline(8, 35);
      Excel_Cell_SetUnderline(8, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(8, 30);
      Excel_Cell_SetUnderline(8, 32);
      Excel_Cell_SetUnderline(8, 56);
      Excel_Cell_SetUnderline(8, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(8, 30, 8, 57);
   }

   //************************
   //***  ���L(�K�X�P�b�g)
   //************************
   // �wE008�x
   if(!Sheet_Cell_CompareA3(9, 36)){
      Excel_Cell_DataSet(9, 55, "*");
      /*
      Excel_Cell_SetUnderline(9, 36);
      Excel_Cell_SetUnderline(9, 35);
      Excel_Cell_SetUnderline(9, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(9, 30);
      Excel_Cell_SetUnderline(9, 32);
      Excel_Cell_SetUnderline(9, 56);
      Excel_Cell_SetUnderline(9, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(9, 30, 9, 57);
   }

   //************************
   //***  ���L(���Ė����P)
   //************************
   // �wE009�x
   if(!Sheet_Cell_CompareA3(10, 36)){
      Excel_Cell_DataSet(10, 55, "*");
      /*
      Excel_Cell_SetUnderline(10, 36);
      Excel_Cell_SetUnderline(10, 35);
      Excel_Cell_SetUnderline(10, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(10, 30);
      Excel_Cell_SetUnderline(10, 32);
      Excel_Cell_SetUnderline(10, 56);
      Excel_Cell_SetUnderline(10, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(10, 30, 10, 57);
   }

   //************************
   //***  ���L(���p�H��)
   //************************
   // �wE010�x
   if(!Sheet_Cell_CompareA3(11, 36)){
      Excel_Cell_DataSet(11, 55, "*");
      /*
      Excel_Cell_SetUnderline(11, 36);
      Excel_Cell_SetUnderline(11, 35);
      Excel_Cell_SetUnderline(11, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(11, 30);
      Excel_Cell_SetUnderline(11, 32);
      Excel_Cell_SetUnderline(11, 56);
      Excel_Cell_SetUnderline(11, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(11, 30, 11, 57);
   }

   //************************
   //***  ���L(�������)
   //************************
   // �wE011�x
   if(!Sheet_Cell_CompareA3(12, 36)){
      Excel_Cell_DataSet(12, 55, "*");
      /*Excel_Cell_SetUnderline(12, 36);
      Excel_Cell_SetUnderline(12, 35);
      Excel_Cell_SetUnderline(12, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(12, 30);
      Excel_Cell_SetUnderline(12, 32);
      Excel_Cell_SetUnderline(12, 56);
      Excel_Cell_SetUnderline(12, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(12, 30, 12, 57);
   }

   //************************
   //***  ���L(���`�F�b�g�X�p�i)
   //************************
   // �wE012�x
   if(!Sheet_Cell_CompareA3(13, 36)){
      Excel_Cell_DataSet(13, 55, "*");
      /*
      Excel_Cell_SetUnderline(13, 36);
      Excel_Cell_SetUnderline(13, 35);
      Excel_Cell_SetUnderline(13, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(13, 30);
      Excel_Cell_SetUnderline(13, 32);
      Excel_Cell_SetUnderline(13, 56);
      Excel_Cell_SetUnderline(13, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(13, 30, 13, 57);
   }

   //************************
   //***  ���L(�d�������P)
   //************************
   // �wE013�x
   if(!Sheet_Cell_CompareA3(14, 36)){
      Excel_Cell_DataSet(14, 55, "*");
      /*
      Excel_Cell_SetUnderline(14, 36);
      Excel_Cell_SetUnderline(14, 35);
      Excel_Cell_SetUnderline(14, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(14, 30);
      Excel_Cell_SetUnderline(14, 32);
      Excel_Cell_SetUnderline(14, 56);
      Excel_Cell_SetUnderline(14, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(14, 30, 14, 57);
   }

   //************************
   //***  ���L(��ڰĖ����P)
   //************************
   // �wE014�x
   if(!Sheet_Cell_CompareA3(15, 36)){
      Excel_Cell_DataSet(15, 55, "*");
      /*
      Excel_Cell_SetUnderline(15, 36);
      Excel_Cell_SetUnderline(15, 35);
      Excel_Cell_SetUnderline(15, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(15, 30);
      Excel_Cell_SetUnderline(15, 32);
      Excel_Cell_SetUnderline(15, 56);
      Excel_Cell_SetUnderline(15, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(15, 30, 15, 57);
   }

   //************************
   //***  ���L(����ݼ�)
   //************************
   // �wE015�x
   if(!Sheet_Cell_CompareA3(16, 36)){
      Excel_Cell_DataSet(16, 55, "*");
      /*
      Excel_Cell_SetUnderline(16, 36);
      Excel_Cell_SetUnderline(16, 35);
      Excel_Cell_SetUnderline(16, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(16, 30);
      Excel_Cell_SetUnderline(16, 32);
      Excel_Cell_SetUnderline(16, 56);
      Excel_Cell_SetUnderline(16, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(16, 30, 16, 57);
   }

   //************************
   //***  ���L(�\���i)
   //************************
   // �wE016�x
   if(!Sheet_Cell_CompareA3(17, 36)){
      Excel_Cell_DataSet(17, 55, "*");
      /*
      Excel_Cell_SetUnderline(17, 36);
      Excel_Cell_SetUnderline(17, 35);
      Excel_Cell_SetUnderline(17, 55);
      //2019.07.25 ����w���������ύX_S
      Excel_Cell_SetUnderline(17, 30);
      Excel_Cell_SetUnderline(17, 32);
      Excel_Cell_SetUnderline(17, 56);
      Excel_Cell_SetUnderline(17, 58);
      //2019.07.25 ����w���������ύX_E
      */
      Excel_Cell_SetUnderline(17, 30, 17, 57);
   }

   // 2020.06.19 �����������L�ǉ�_S
   //***************************
   //***    ���L(��������)
   //***************************
   // �wE017�x
   if(!Sheet_Cell_CompareA3(18, 36)){
      Excel_Cell_DataSet(18, 55, "*");
      Excel_Cell_SetUnderline(18, 30, 18, 57);
   }

   //2020.06.19 ���a���t���Ēǉ�_S
   //************************
   //***  ���L(���a���t����)
   //************************
   // �wE018�x
   if(!Sheet_Cell_CompareA3(19, 36)){
      Excel_Cell_DataSet(19, 55, "*");
      Excel_Cell_SetUnderline(19, 30, 19, 57);
   }

   //************************
   //***  ���L(���a���Ķ�ްBLCP1S)
   //************************
   // �wE019�x
   if(!Sheet_Cell_CompareA3(20, 36)){
      Excel_Cell_DataSet(20, 55, "*");
      Excel_Cell_SetUnderline(20, 30, 20, 57);
   }

   //************************
   //***  ���L(���a���Ķ�ްBLCESS)
   //************************
   // �wE020�x
   if(!Sheet_Cell_CompareA3(21, 36)){
      Excel_Cell_DataSet(21, 55, "*");
      Excel_Cell_SetUnderline(21, 30, 21, 57);
   }
   // 2020.06.19 �����������L�ǉ�_E

   // 2020.10.30 ���͗e��UNS�K�i�x���ǉ�_S
   //************************
   //***  ���L(���͗e��UNS�K�i�x��)
   //************************
   // �wE021�x
   if(!Sheet_Cell_CompareA3(22, 36)){
      Excel_Cell_DataSet(22, 55, "*");
      Excel_Cell_SetUnderline(22, 30, 22, 57);
   }
   // 2020.10.30 ���͗e��UNS�K�i�x���ǉ�_E

   // 2021.10.19 ���L�ǉ�_S
   //************************
   //***  �_�C�g�N���L
   //************************
   // �wE022�x
   if(!Sheet_Cell_CompareA3(23, 36)){
      Excel_Cell_DataSet(23, 55, "*");
      Excel_Cell_SetUnderline(23, 30, 23, 57);
   }
   // 2021.10.19 ���L�ǉ�_E

   // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_S
   //************************
   //***  �@��ԍ�
   //************************
   // �wE023�x
   if(!Sheet_Cell_CompareA3(24, 36)){
      Excel_Cell_DataSet(24, 55, "*");
      Excel_Cell_SetUnderline(24, 30, 24, 57);
   }
   // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_E

   // 2022.04.05 �A�q�D�����L�ǉ�_S
   //************************
   //***  �@��ԍ�
   //************************
   // �wE024�x
   if(!Sheet_Cell_CompareA3(25, 36)){
      Excel_Cell_DataSet(25, 55, "*");
      Excel_Cell_SetUnderline(25, 30, 25, 57);
   }
   // 2022.04.05 �A�q�D�����L�ǉ�_E


   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          G���@�@��r                                ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //************************
   //***  �X�p�i����
   //************************
   // �wG001�x
   if(!Sheet_Cell_CompareA3(44, 44)){
      Excel_Cell_DataSet(44, 55, "*");
   }

   //************************
   //***  ���p�u���V����
   //************************
   // �wG002�x
   if(!Sheet_Cell_CompareA3(45, 44)){
      Excel_Cell_DataSet(45, 55, "*");
   }

   //************************
   //***  �Q�[�W�v���[�g�@L
   //************************
   // �wG003�x
   //2019.07.25 ����w���������ύX_S
   //if(!Sheet_Cell_CompareA3(46, 44)){
   if(!Sheet_Cell_CompareA3(46, 40)){
   //2019.07.25 ����w���������ύX_E
      Excel_Cell_DataSet(46, 55, "*");
   }

   //************************
   //***  �Q�[�W�v���[�g�@Max
   //************************
   // �wG004�x
   //2019.07.25 ����w���������ύX_S
   //if(!Sheet_Cell_CompareA3(47, 44)){
   if(!Sheet_Cell_CompareA3(47, 40) || !Sheet_Cell_CompareA3(47, 44)){
   //2019.07.25 ����w���������ύX_E
      Excel_Cell_DataSet(47, 55, "*");
   }

   //************************
   //***  �Q�[�W�v���[�g�@Min
   //************************
   // �wG005�x
   //2019.07.25 ����w���������ύX_S
   //if(!Sheet_Cell_CompareA3(48, 44)){
   if(!Sheet_Cell_CompareA3(48, 40)){
   //2019.07.25 ����w���������ύX_E
      Excel_Cell_DataSet(48, 55, "*");
   }

   //************************
   //***  �ړ��~��
   //************************
   // �wG006�x
   if(!Sheet_Cell_CompareA3(49, 44)){
      Excel_Cell_DataSet(49, 55, "*");
   }

   //************************
   //***  S1�m�Y���I���G���e�[�V����
   //************************
   // �wG007�x
   if(!Sheet_Cell_CompareA3(51, 37)){
      Excel_Cell_DataSet(51, 42, "*");
   }

   //************************
   //***  S2�m�Y���I���G���e�[�V����
   //************************
   // �wG008�x
   if(!Sheet_Cell_CompareA3(51, 45)){
      Excel_Cell_DataSet(51, 55, "*");
   }

   //************************
   //***  S3�m�Y���I���G���e�[�V����
   //************************
   // �wG009�x
   if(!Sheet_Cell_CompareA3(52, 37)){
      Excel_Cell_DataSet(52, 42, "*");
   }

   //************************
   //***  S4�m�Y���I���G���e�[�V����
   //************************
   // �wG010�x
   if(!Sheet_Cell_CompareA3(52, 45)){
      Excel_Cell_DataSet(52, 55, "*");
   }

   //******************************
   //***  Excel Book Close
   //******************************
   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   //Windows 2008Server �ڐA�Ή�
   exApplication.OlePropertySet("DisplayAlerts", false);
   exWorkbook_old.OleFunction("Close");

   exWorkCell_old.Clear();
   exWorkbook_old.Clear();
   exWorksheet_old.Clear();
   exWorksheets_old.Clear();

   DeleteFile(s_MakeExcel_Pass);
}

//---------------------------------------------------------------------------
// ���{��֐����F
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F
// ��  �l      �F
//---------------------------------------------------------------------------
bool __fastcall Sheet_Cell_CompareA3(int Row, int Col){
   AnsiString Newsht_Value;
   AnsiString Oldsht_Value;
   int        OldLayout;

   OldLayout = 0;   // �I�t�Z�b�g�l��������
   //***************************
   //***  �ŐV�̒l�@�擾
   //***************************
   //exWorkCell   = exWorksheet.Exec(PropertyGet("Cells") << Row << Col);
   //Newsht_Value = exWorkCell.Exec(PropertyGet("Value"));
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells",Row,Col);
   Newsht_Value = exWorkCell.OlePropertyGet("Value");

//   // �����ύX�������߁A�O�񒠕[���������̏ꍇ�A��r�ʒu�����炷
//   if ( Row >= 9 && Row <= 11 ) {        // �v���[�g�y�уK�X�P�b�g�d�l�����i�����ύX�ɂ��e���͈́j
//    //exWorkCell_old = exWorksheet_old.Exec(PropertyGet("Cells") << 8 << 13);
//    //Oldsht_Value = exWorkCell_old.Exec(PropertyGet("Value"));
//    //Windows 2008Server �ڐA�Ή�
//    exWorkCell_old = exWorksheet_old.OlePropertyGet("Cells",8,13);
//    Oldsht_Value = exWorkCell_old.OlePropertyGet("Value");
//    if ( Oldsht_Value != "" ) {   //�W�s�ڂ̂P�R���ڂɁh�i���̓`�F�b�N���ڂł��j�h�������� ���@������
//       OldLayout = 1;         // �I�t�Z�b�g�l��ݒ�
//    }
//   }

   //***************************
   //***  �O��̒l�@�擾
   //***************************
   //exWorkCell_old = exWorksheet_old.Exec(PropertyGet("Cells") << (Row + OldLayout) << Col);
   //Oldsht_Value = exWorkCell_old.Exec(PropertyGet("Value"));
   //Windows 2008Server �ڐA�Ή�
   exWorkCell_old = exWorksheet_old.OlePropertyGet("Cells",(Row + OldLayout),Col);
   Oldsht_Value = exWorkCell_old.OlePropertyGet("Value");
   // ���O�쐬
   G_Log_Text = "�Z���ԍ�  [" + FormatFloat("00",Row) + "," + FormatFloat("00",Col)
              + "]  �O��l�w" + Oldsht_Value + "�x ����l�w" + Newsht_Value + "�x";


   Write_Log(G_Log_Text);

   if (Newsht_Value.TrimRight() != Oldsht_Value.TrimRight()){
      return false;
   }

   return true;
}

//-------------------------------------------------------------------------------------
// ���{��֐����F �ߋ�Excel��ā@��������
// �T  �v      �F Rev*���ިڸ�ؖ����������A�w�����̑��݂���ő����޼ޮݔԍ����擾
// ��  ��      �F
// �߂�l      �F �w�����̑��݂��钼�߂���޼ޮݔԍ�
// ��  �l      �F
//-------------------------------------------------------------------------------------
AnsiString __fastcall Sheet_Exist_CheckA3(AnsiString path, HeadsData* pcHeadsData){
   AnsiString RevNo;
   AnsiString NowRev;
   AnsiString SheetPass;
   bool       bFind;
   bool       bFlag;
   AnsiString sSpecCode;

   // 2007.10.10 �װ�d�l�Ή� �d�l���ߺ���
   sSpecCode = AnsiString(pcHeadsData->GetSpecCode());

   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   //NowRev = s_HEADS_DATA[319].TrimRight();
   NowRev = IntToStr(G_HEADS_DATA_Rev);
   //*************************************
   if ( NowRev.ToIntDef(0) == 0 ) {
      RevNo = "";
      return RevNo;
   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   //}
   //RevNo = FormatFloat("00",NowRev.ToIntDef(0) - 1);
   } else {
      RevNo = FormatFloat("00",NowRev.ToIntDef(0) - 1);
   }
   //*************************************
   bFind = false;
   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   //while(RevNo.ToIntDef(0) >= 0 && bFind == false) {
   //   // ̧�ّ�������
   //   SheetPass = path + RevNo + "\\MAN" + G_SAKUBAN + ".xls";
   //   if (FileExists(SheetPass.c_str()) == true){
   //     bFind = true;
   //   } else {
   //     RevNo = FormatFloat("00",RevNo.ToIntDef(0) - 1);
   //   }
   //}
   while(RevNo.ToIntDef(0) >= 0 && bFind == false) {
      // ̧�ّ�������
      bFlag = false;
	  //2023/4/21 �����ԍ������ǉ��Ή� Mod
	  //SheetPass = path + RevNo + "\\MAN" + G_SAKUBAN + sSpecCode + "R" + RevNo + ".xls";
	  SheetPass = path + RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + sSpecCode + "R" + RevNo + ".xls";
      G_Log_Text = "SheetPass?�w " + SheetPass + " �x";
      Write_Log(G_Log_Text);
      if (FileExists(SheetPass.c_str()))
      {
          bFlag = true;
      }
      if (!bFlag) {
		  //2023/4/21 �����ԍ������ǉ��Ή� Mod
		  //SheetPass = path + RevNo + "\\MAN" + G_SAKUBAN + sSpecCode + ".xls";
		  SheetPass = path + RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + sSpecCode + ".xls";
		  if (FileExists(SheetPass.c_str()))
          {
              bFlag = true;
          }
	  }

      if (bFlag){
        bFind = true;
        G_Log_Text = "true?�w true �x";
        Write_Log(G_Log_Text);
      } else {
        RevNo = FormatFloat("00",RevNo.ToIntDef(0) - 1);
      }
   }
   //*************************************
   return RevNo;
}
////-------------------------------------------------------------------------------------
//// ���{��֐����F �ߋ�Excel��ā@��������
//// �T  �v      �F Rev*���ިڸ�ؖ����������A�w�����̑��݂���ő����޼ޮݔԍ����擾
//// ��  ��      �F
//// �߂�l      �F �w�����̑��݂��钼�߂���޼ޮݔԍ�
//// ��  �l      �F
////-------------------------------------------------------------------------------------
//AnsiString __fastcall Sheet_Exist_Check(AnsiString path){
//   WIN32_FIND_DATA fd;
//   AnsiString RevNo;
//   AnsiString filenm;
//   AnsiString NowRev;
//   AnsiString SheetPass;
//
//   HANDLE hFind = FindFirstFile((path + "*").c_str(), &fd);
//   NowRev = s_HEADS_DATA[319].TrimRight();
//   if ( NowRev.ToIntDef(0) == 0 ) {
//      RevNo = "";
//      return RevNo;
//   }
//
//   if(hFind != INVALID_HANDLE_VALUE) {
//      do {
//         filenm = (AnsiString)fd.cFileName;
//         if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
//            // ̧�ّ�������
//            SheetPass = path + filenm + "\\MAN" + G_SAKUBAN + ".xls";
//            if (FileExists(SheetPass.c_str())){
//               if ( filenm.ToIntDef(0) != 0 ) {
//                 // ��޼ޮݔԍ����r����B
//                 if ( (RevNo < filenm) && (NowRev > filenm) ){
//                    RevNo = filenm;
//                 }
//               }
//            }
//         }
//      } while(FindNextFile(hFind, &fd));
//
//      FindClose(hFind);
//   }
//
//   return RevNo;
//}

//---------------------------------------------------------------------------
// ���{��֐����F �d�㖼�́@�擾
// �T  �v      �F �����Ƃ��ēn���ꂽ���ނŎ擾����B
// ��  ��      �F Code�FS1ɽ�َd��(RECORD NO�F904) or S2ɽ�َd��(RECORD NO�F919)
// �߂�l      �F �ϊ�����
// ��  �l      �F
//---------------------------------------------------------------------------
AnsiString __fastcall Get_SiageMeisyouA3(AnsiString Code){
   AnsiString s_Name;

   if (Code == "1"){
      s_Name = "���O  �_��";

   } else if (Code == "2"){
      s_Name = "�����#320�O��";

   } else if (Code == "3"){
      s_Name = "�����#250�O��";

   } else if (Code == "4"){
      s_Name = "�����#180�O��";

   } else if (Code == "5"){
      s_Name = "�\�\�\�\�\�\";

   } else if (Code == "6"){
      s_Name = "���_��O��";

   } else {
      s_Name = "";
   }

   return s_Name;
}

//---------------------------------------------------------------------------
// ���{��֐����F �ގ�����Ͻ��@�a�����̎擾
// �T  �v      �F �����Ƃ��ēn���ꂽ���ނōގ�����Ͻ�����������B
// ��  ��      �F Code�F�����p����
// �߂�l      �F �ϊ�����
// ��  �l      �F
//---------------------------------------------------------------------------
AnsiString __fastcall Get_Zaisitu_NameA3(AnsiString Code){
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //TQuery *wkQuery;
   //wkQuery = new TQuery(DataModule1);
   TFDQuery *wkQuery;
   wkQuery = new TFDQuery(DataModule1);
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
   AnsiString s_Name;
   AnsiString s_SQL;

   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + Code + "';";

   wkQuery->Close();
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Name = "";
   } else {
      s_Name = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   delete wkQuery;

   return s_Name;
}

//---------------------------------------------------------------------------
// ���{��֐����F ���͌v��ށ@�擾
// �T  �v      �F �����Ƃ��ēn���ꂽ���͌v��ނ���������B
// ��  ��      �F Code�F���͌v���
// �߂�l      �F �ϊ�����
// ��  �l      �F
//---------------------------------------------------------------------------
AnsiString __fastcall Get_AturyokuKeiA3(AnsiString s_Code){
   AnsiString s_Aturyokukei;   // ���͌v���

   if (s_Code.ToIntDef(0) == 1){
      s_Aturyokukei = "BC������";

   } else if (s_Code.ToIntDef(0) == 2){
      s_Aturyokukei = "SUS������";

   } else if (s_Code.ToIntDef(0) == 3){
      s_Aturyokukei = "BC������";

   } else if (s_Code.ToIntDef(0) == 4){
      s_Aturyokukei = "SUS������";

   } else if (s_Code.ToIntDef(0) == 5){
      s_Aturyokukei = "BC������";

   } else if (s_Code.ToIntDef(0) == 6){
      // 2020.07.17 ���͌v�ύX_S
      //s_Aturyokukei = "SS/TF��ϸ";
      s_Aturyokukei = "C.S./TF��ϸ";
      // 2020.07.17 ���͌v�ύX_E

   } else if (s_Code.ToIntDef(0) == 7){
      s_Aturyokukei = "SUS��ϸ";

   } else if (s_Code.ToIntDef(0) == 8){
      // 2020.07.17 ���͌v�ύX_S
      //s_Aturyokukei = "SS/TF��ϸ";
      s_Aturyokukei = "C.S./TF��ϸ";
      // 2020.07.17 ���͌v�ύX_E

   } else if (s_Code.ToIntDef(0) == 9){
      s_Aturyokukei = "SUS��ϸ";

   } else if (s_Code.ToIntDef(0) == 10){
      // 2020.07.17 ���͌v�ύX_S
      //s_Aturyokukei = "SS/TF��ϸ";
      s_Aturyokukei = "C.S./TF��ϸ";
      // 2020.07.17 ���͌v�ύX_E

   } else if (s_Code.ToIntDef(0) == 11){
      s_Aturyokukei = "SUS��ϸ";

   } else if (s_Code.ToIntDef(0) == 12){
      // 2020.07.17 ���͌v�ύX_S
      //s_Aturyokukei = "SS/TF��ϸ";
      s_Aturyokukei = "C.S./TF��ϸ";
      // 2020.07.17 ���͌v�ύX_E

   } else if (s_Code.ToIntDef(0) == 13){
      s_Aturyokukei = "SUS��ϸ";

   //2004.05.19 ���͌v�����ǉ�
   } else if (s_Code.ToIntDef(0) == 18){
      s_Aturyokukei = "SUS������";

   } else if (s_Code.ToIntDef(0) == 19){
      s_Aturyokukei = "BC������";

   } else if (s_Code.ToIntDef(0) == 20){
      s_Aturyokukei = "SUS������";
   //*************************

   // 2020.07.17 ���͌v�ύX_S
   } else if (s_Code.ToIntDef(0) == 21){
      s_Aturyokukei = "BC������";

   } else if (s_Code.ToIntDef(0) == 22){
      s_Aturyokukei = "SUS������";

   } else if (s_Code.ToIntDef(0) == 23){
      s_Aturyokukei = "SUS�EC.S.��ϸ";

   } else if (s_Code.ToIntDef(0) == 24){
      s_Aturyokukei = "SUS�EC.S.��ϸ";

   } else if (s_Code.ToIntDef(0) == 25){
      s_Aturyokukei = "SUS�EC.S.��ϸ";

   } else if (s_Code.ToIntDef(0) == 26){
      s_Aturyokukei = "SUS�EC.S.��ϸ";
   // 2020.07.17 ���͌v�ύX_E

   } else {
      s_Aturyokukei = "";
   }

   return s_Aturyokukei;
}

//---------------------------------------------------------------------------
// ���{��֐����F �[���ް��쐬
// �T  �v      �F
// ��  ��      �F AnsiString Syurui�F�[�ǎ��(S1�`E4)
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
AnsiString __fastcall Get_Tankan_DataA3(AnsiString Syurui, HeadsData* pcHeadsData)
{
   AnsiString s_Tkn_Syurui;   // �[�ǎ��
   AnsiString s_HEADS_DATA[1500];

   // AnsiString�^��HEADS�ް����擾
   pcHeadsData->SetAnsiStringData(s_HEADS_DATA);

// 6��ڂ��e�ގ�����G�A�����A�t�����p��ɕύX  2002.10.20 S.Y
   int HEADS[8][9] = {{ 1136,  901,  898,  895,  892,  905, 1144, 1216, 1217  },
                      { 1137,  916,  913,  910,  907,  920, 1145, 1220, 1221  },
                      { 1138,  931,  928,  925,  922,  935, 1146, 1224, 1225  },
                      { 1139,  946,  943,  940,  937,  950, 1147, 1228, 1229  },
                      { 1140,  961,  958,  955,  952,  965, 1148, 1232, 1233  },
                      { 1141,  976,  973,  970,  967,  980, 1149, 1236, 1237  },
                      { 1142,  991,  988,  985,  982,  995, 1150, 1240, 1241  },
                      { 1143, 1006, 1003, 1000,  997, 1010, 1151, 1244, 1245  }};


   int s_Prs_Syurui;          // ���͌v���
   int s_Tmp_Syurui;          // ���x�v���
   int iWFflg;                // WF�[�ǁH
   double s_Ank_Kokei;        // ���������a
   double s_Nzl_Kokei;        // ɽ�ٌ��a
   AnsiString s_Nzl_Tugite;   // ɽ�ٌp��
   AnsiString s_Nzl_Syurui;   // ɽ�َ��
   AnsiString s_PGtyp;        // ���͌v��ށi�����p�j
   AnsiString s_TPtyp;        // ���x�v��ށi�����p�j
   AnsiString s_DRtyp;        // �t������ށi�����p�j
   AnsiString s_ARtyp;        // �G�A������ށi�����p�j
   AnsiString P_Model;
   AnsiString P_Katasiki;

   int i_HeadsNo;

   //2019.07.25 ����w���������ύX_S
   int HEADSM[8] = { 893, 908, 923, 938, 953, 968, 983, 998 };

   AnsiString s_NzCd;         // ɽ�ٌp�躰��
   AnsiString s_KeyMat;       // �ݸލގ���ށi�ݸޒ[�ǂ�STPG�j
   AnsiString s_MatCD;        // �ް��׸�
   AnsiString s_ULdiv;        // ��ݼ޺���

   AnsiString s_Frame;        // �^�����
   AnsiString s_Ver;          // �ް�ޮ�No.
   int i_Length;              // ������
     AnsiString s_Text;            // �ėp������
//   AnsiString s_RText;         // �ݸޒ��L�@
//   AnsiString s_Mat;          // �[�Ǎގ�
//   AnsiString s_BootFlg;      // �ް��׸�
//   AnsiString s_FCode;        // ��ݼ޺���


   int i_FL; //�΍s���׸�(0:��`��,1:�΍s��)

   //2019.07.05 ����w���������ύX_E

   s_Nzl_Syurui = "";
   if( Syurui == "S1" ) {
      i_HeadsNo = 0;
      s_Nzl_Syurui = s_HEADS_DATA[891].TrimRight();
   } else if( Syurui == "S2" ) {
      i_HeadsNo = 1;
      s_Nzl_Syurui = s_HEADS_DATA[906].TrimRight();
   } else if( Syurui == "S3" ) {
      i_HeadsNo = 2;
      s_Nzl_Syurui = s_HEADS_DATA[921].TrimRight();
   } else if( Syurui == "S4" ) {
      i_HeadsNo = 3;
      s_Nzl_Syurui = s_HEADS_DATA[936].TrimRight();
   } else if( Syurui == "E1" ) {
      i_HeadsNo = 4;
      s_Nzl_Syurui = s_HEADS_DATA[951].TrimRight();
   } else if( Syurui == "E2" ) {
      i_HeadsNo = 5;
      s_Nzl_Syurui = s_HEADS_DATA[966].TrimRight();
   } else if( Syurui == "E3" ) {
      i_HeadsNo = 6;
      s_Nzl_Syurui = s_HEADS_DATA[981].TrimRight();
   } else if( Syurui == "E4" ) {
      i_HeadsNo = 7;
      s_Nzl_Syurui = s_HEADS_DATA[996].TrimRight();
   }

   //2019.07.25 ����w���������ύX_S
   if (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
      //CX-10D
      i_FL = 1; // �΍s��
   }
   else if (s_HEADS_DATA[823].ToIntDef(0) == 21 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 90) {
      //LX-90D
      i_FL = 1; // �΍s��
   }
   else if (s_HEADS_DATA[823].ToIntDef(0) == 111 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 80) {
      //YX-80
      i_FL = 1; // �΍s��
   }
   else if (s_HEADS_DATA[823].ToIntDef(0) == 111 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 83) {
      //YX-83
      i_FL = 1; // �΍s��
   }
   else {
      i_FL = 0; // ��`��
   }

   if( Syurui == "S1" ) {
      s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      s_ULdiv = "U";
   } else if( Syurui == "S2" ) {
      s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      s_ULdiv = "U";
   } else if( Syurui == "S3" ) {
      if ( i_FL == 1 ) {
         // �΍s��
         s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      } else {
         // ��`��
         s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      }
      s_ULdiv = "L";
   } else if( Syurui == "S4" ) {
      if ( i_FL == 1 ) {
         // �΍s��
         s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      } else {
         // ��`��
         s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      }
      s_ULdiv = "L";
   } else if( Syurui == "E1" ) {
      s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      s_ULdiv = "U";
   } else if( Syurui == "E2" ) {
      s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      s_ULdiv = "U";
   } else if( Syurui == "E3" ) {
      if ( i_FL == 1 ) {
         // �΍s��
         s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      } else {
         // ��`��
         s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      }
      s_ULdiv = "L";
   } else if( Syurui == "E4" ) {
      if ( i_FL == 1 ) {
         // �΍s��
         s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      } else {
         // ��`��
         s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      }
      s_ULdiv = "L";
   }

   // �����O�ގ����
   s_KeyMat = Type_MAT( s_MatCD );

   // �m�Y���p��R�[�h
   s_NzCd =   s_HEADS_DATA[HEADSM[i_HeadsNo]].SubString(1, 2);
   //2019.07.25 ����w���������ύX_E

   // ���͌v���
   s_Prs_Syurui  = s_HEADS_DATA[HEADS[i_HeadsNo][1]].TrimRight().ToIntDef(0);
   // ���x�v���
   s_Tmp_Syurui  = s_HEADS_DATA[HEADS[i_HeadsNo][2]].TrimRight().ToIntDef(0);
   // ���������a
   s_Ank_Kokei   = StrToDblDef(s_HEADS_DATA[HEADS[i_HeadsNo][3]].TrimRight(),0);
   // ɽ�ٌ��a
   s_Nzl_Kokei   = StrToDblDef(s_HEADS_DATA[HEADS[i_HeadsNo][4]].TrimRight(),0);
   // 2011.06.20 ɽ�ًK�i�ύX
   //// ɽ�ٌp��(�擪�ꌅ)
   // ɽ�ٌp��(�擪��)
   //s_Nzl_Tugite  = s_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(1,1);
   s_Nzl_Tugite  = s_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(1,2);
   // ***********************

   //***********************************
   //***  �[���ް��@�쐬
   //***********************************
   s_Tkn_Syurui = s_HEADS_DATA[HEADS[i_HeadsNo][0]].TrimRight();  // �[�ǎ��
   // 2011.06.20 ɽ�ًK�i�ύX
   //if ( ( s_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(3,1) == "1" ) ||
   //     ( s_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(3,1) == "2" ) ) {
   if ( ( s_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(4,1) == "1" ) ||
        ( s_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(4,1) == "2" ) ) {
   // ***********************
     iWFflg=1;
   } else {
     iWFflg=0;
   }
   // (1)

   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //��2002/10/24
   if(s_Tkn_Syurui == "0"){
      return "";
   }

   // 02.10.16 �V�K�ǉ�����
   if (Syurui == "S3" || Syurui == "S4" || Syurui == "E3" || Syurui == "E4"){
      // ɽ�َ�� = "3" ���� ɽ�ٌp��̐擪�񌅖� = "4"
      // 2011.06.20 ɽ�ًK�i�ύX
      //if (s_Nzl_Syurui == "3" && s_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(2,1) != "4" && s_Nzl_Kokei != 0 ){
      //   if (s_Nzl_Tugite == "7"){
      if ( s_Nzl_Syurui == "3"
        && s_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(3,1) != "4"
        && s_Nzl_Kokei != 0 ) {
         if (s_Nzl_Tugite == "07"){
      // ***********************
            return "�[�ǂw";
         } else {
            P_Katasiki = "�[�ǂx";
            return "�[�ǂx";
         }
      }
   }

   // ���͌v�����L�[�쐬
   if (s_Prs_Syurui == 0 ) {
      s_PGtyp = "-";
   } else if ( (s_Prs_Syurui > 5 ) && (s_Prs_Syurui < 16) ){
      s_PGtyp = "K";
   } else if ( s_Prs_Syurui == 5 ){
      s_PGtyp = "S";
   //2004.05.19 ���͌v�����ǉ�
   } else if ( s_Prs_Syurui == 18 || s_Prs_Syurui == 19 || s_Prs_Syurui == 20 ){
      s_PGtyp = "S";
   //*************************
   } else {
      s_PGtyp = "B";
   }

   // ���x�v�����L�[�쐬
   if (s_Tmp_Syurui == 0 ) {
      s_TPtyp = "-";
   } else {
      s_TPtyp = "T";
   }
   if (Syurui == "S1" || Syurui == "S2" || Syurui == "E1" || Syurui == "E2"){
      // �G�A�����L��
      s_DRtyp = "-";
      if ( s_Ank_Kokei > 0 ) {
         s_ARtyp = "A";
      } else {
         s_ARtyp = "-";
      }
   } else {
      // �t�����T�C�Y
      s_ARtyp = "-";
      if ( s_Ank_Kokei > 0 ) {
         // 2011.06.20 ɽ�ًK�i�ύX
         //if ( s_Nzl_Tugite != "7" ) {
         if ( s_Nzl_Tugite != "07" ) {
         // ***********************
           s_DRtyp = "L";
         }  else {
           s_DRtyp = "S";
         }
      } else {
         s_DRtyp = "-";
      }
   }

   //2019.07.25 ����w���������ύX_S
   // �c�a����
   // 2023.02.16 �����O�[�Ǐ����ύX_S
   // 2021.01.06 CX(W)-03�ǉ�(CX(W)-00��CX(W)-01)_S
   //if ( P_Model == "CX-00" || P_Model == "CXW-00" ) {
   //   //CX-00 �ݸޒ[��
   //if ( P_Model == "CX-01" || P_Model == "CXW-01" ) {
   if (( P_Model == "CX-01" || P_Model == "CX-01D" || P_Model == "CXW-01" ) && s_HEADS_DATA[36].Pos("Y")==0 ){
   // 2023.02.16 �����O�[�Ǐ����ύX_E
      //CX-00 �ݸޒ[��
   // 2021.01.06 CX(W)-03�ǉ�(CX(W)-00��CX(W)-01)_E
      if ( s_KeyMat == "ALBC" ) {
          // �`�k�a�b
      } else if ( s_KeyMat == "STPG" ) {
          // �r�s�o�f
      } else {
          //���̑�
          // 2020.05.22.�ݸލގ��C��_S
          //s_KeyMat == "-";
          s_KeyMat = "-";
          // 2020.05.22.�ݸލގ��C��_E
      }
      s_ULdiv = "-"; // �ʒu�Œ�
      s_PGtyp = "-"; // �R�b�N�Ȃ�
      s_TPtyp = "-"; // ���x�v���͌v���Ȃ�
      s_DRtyp = "-"; // �t���Ȃ�
      s_ARtyp = "-"; // �G�A���Ȃ�

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);

   } else if  ( P_Model == "CX-10" || P_Model == "CX-10D" ) {
      //CX-10 �ݸޒ[��
      if ( s_KeyMat == "ALBC" ) {
          // �`�k�a�b
      } else if ( s_KeyMat == "STPG" ) {
          // �r�s�o�f
      } else {
          //���̑�
          // 2020.05.22.�ݸލގ��C��_S
          //s_KeyMat == "-";
          s_KeyMat = "-";
          // 2020.05.22.�ݸލގ��C��_E
      }
      s_ULdiv = "-"; // �ʒu�Œ�
      s_PGtyp = "-"; // �R�b�N�Ȃ�
      s_TPtyp = "-"; // ���x�v���͌v���Ȃ�
      s_DRtyp = "-"; // �t���Ȃ�
      s_ARtyp = "-"; // �G�A���Ȃ�

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);

   } else if ( ( P_Model == "SX-30" || P_Model == "SX-30S" ) && s_NzCd == "41" ) {
      //SX-30,30S JIS30K�ݸޒ[��
      if ( s_KeyMat == "ALBC" ) {
          // �`�k�a�b
      } else if ( s_KeyMat == "CS" ) {
          // �b�r�i�r�s�j�l�P�R�`�j
      } else {
          //���̑�
          // 2020.05.22.�ݸލގ��C��_S
          //s_KeyMat == "-";
          s_KeyMat = "-";
          // 2020.05.22.�ݸލގ��C��_E
      }
      s_ULdiv = "-"; // �ʒu�Œ�
      s_PGtyp = "-"; // �R�b�N�Ȃ�
      s_TPtyp = "-"; // ���x�v���͌v���Ȃ�
      s_DRtyp = "-"; // �t���Ȃ�
      s_ARtyp = "-"; // �G�A���Ȃ�

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);

   } else if ( ( s_HEADS_DATA[36].Pos("M") != 0 ) &&
               ( Syurui == "S1" || Syurui == "S2" || Syurui == "S3" || Syurui == "S4" ) ) {
      //���p�ݸޒ[��
      if ( s_KeyMat == "ALBC" ) {
         // �`�k�a�b
      } else if ( s_KeyMat == "STPG" ) {
         // �r�s�o�f
      } else if ( s_KeyMat == "CS" ) {
          // �b�r�i�r�s�j�l�P�R�`�j
      } else if ( s_KeyMat == "SGP" ) {
         // �r�f�o
      } else {
         //���̑�
         // 2020.05.22.�ݸލގ��C��_S
         //s_KeyMat == "-";
         s_KeyMat = "-";
         // 2020.05.22.�ݸލގ��C��_E
      }
      //s_ULdiv(�ݒ��)
      s_PGtyp = "T"; // �ʌ^��
      // 2020.09.01 ���p�ݸޕύX_S
      //s_TPtyp = "-"; // ���x�v���͌v���Ȃ�
      if (s_Prs_Syurui == 0) {
          s_TPtyp = "-"; // ���͌v���Ȃ��@
      }
      else {
          s_TPtyp = "P"; // ���͌v������
      }
      // 2020.09.01 ���p�ݸޕύX_E
      s_DRtyp = "-"; // �t���Ȃ�
      s_ARtyp = "-"; // �G�A���Ȃ�

      if ( ( P_Model == "RX-70" ) &&
           ( s_KeyMat == "STPG" || s_KeyMat == "-" ) ) {
         //RX-70 ���ގ� �ݸޒ[��
         s_Text = s_HEADS_DATA[36].TrimRight();
         i_Length = s_Text.Length();                    // ������
         s_Text = s_Text.SubString( i_Length, 1 );
         if ( s_Text == "R" ) {
            s_Text = s_HEADS_DATA[36].TrimRight();
            s_Frame = s_Text.SetLength( i_Length - 1 ); // �^�����
         } else {
            s_Frame = s_HEADS_DATA[36].TrimRight();     // �^�����
         }
         s_Ver = s_HEADS_DATA[1267].TrimRight();

         if ( s_Frame == "NPM" ||  s_Frame == "KNPM" || s_Frame == "TNPM" ) {
            if ( s_Ver == "200207" ) {
               s_KeyMat = "HP";
            } else {
               s_KeyMat = "P";
            }
         } else if ( s_Frame == "NHPM" ||  s_Frame == "KNHPM" || s_Frame == "TNHPM" ) {
            s_KeyMat = "HP";
         } else {
            s_KeyMat = "-";
         }
      }

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);

   } else {

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_Nzl_Kokei, Syurui.SubString(1,1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp,false);

   }
   //2019.07.25 ����w���������ύX_E

   //2019.07.25 ����w���������ύX_S
   //P_Katasiki = Search_HD_FRNG_MST(P_Model, s_Nzl_Kokei, Syurui.SubString(1,1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp,false);
   //2019.07.25 ����w���������ύX_E
   if ( P_Katasiki == "" ) {
//     P_Katasiki = Search_HD_FRNG_MST("-", s_Nzl_Kokei, Syurui.SubString(1,1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp);
     P_Katasiki = "�Y���[�ǂȂ�";
   }
   //2009.07.31 �[�����̕ύX
   if ( P_Katasiki == "�[�ǂP�R�i���j" ) {
       P_Katasiki = "�[�ǂP�R";
   }
   //***********************
   if ( P_Katasiki != "" ) {
     return P_Katasiki;
   }
   return "";

}

//---------------------------------------------------------------------------
// ���{��֐����F �t�����W��ގ擾
// �T  �v      �F
// ��  ��      �F AnsiString Syurui�F�[�ǎ��(S1�`E4)
// �߂�l      �F �t�����W���]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
AnsiString __fastcall Get_Flange_DataA3(AnsiString Tugite)
{
   AnsiString s_Flange_Syurui;   // ��ݼގ��
   AnsiString s_Text;

   // 2011.06.20 ɽ�ًK�i�ύX
   //s_Text = Tugite.SubString(3,1);
   s_Text = Tugite.SubString(4,1);
   // ***********************

   if (s_Text == "0"){
      s_Flange_Syurui = "�t���[�e";

   } else if (s_Text == "1"){
      s_Flange_Syurui = "�q�e�D�v�e";

   } else if (s_Text == "2"){
      s_Flange_Syurui = "�v�e";

   } else if (s_Text == "3"){
      s_Flange_Syurui = "�k�i";

   } else if (s_Text == "4"){
      s_Flange_Syurui = "�k�i�@�e";

   } else if (s_Text == "6"){
      s_Flange_Syurui = "�k�i";

   } else if (s_Text == "7"){
      s_Flange_Syurui = "�T�j�^���l�W";

   } else if (s_Text == "8"){
      s_Flange_Syurui = "�T�j�^���w���[��";

   } else if (s_Text == "9"){
      s_Flange_Syurui = "�l�W�e";

   } else {
      s_Flange_Syurui = "";
   }
   return s_Flange_Syurui;
}

//---------------------------------------------------------------------------
// ���{��֐����F ���x�v���ގ��擾
// �T  �v      �F
// ��  ��      �F TanSyurui:�[�ǎ��(S1�`S4)
//             �F TanName  :�[�ǖ���
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F ���i�W�J�ް�,HEARTS�p�ް��ɍE���ް���������
//---------------------------------------------------------------------------
AnsiString __fastcall Get_T_ZaZaisituA3(AnsiString TanSyurui, AnsiString TanName, HeadsData* pcHeadsData)
{
   AnsiString s_T_ZaZaisitu;  // ���x�v���̍ގ�����

   AnsiString s_NT_Zaisitu;   // ɽ�فE�[�Ǎގ�����
   AnsiString s_Code;         // ���x�v���̍ގ�����
   AnsiString s_HEADS_DATA[1500];

   pcHeadsData->SetAnsiStringData(s_HEADS_DATA);

   s_T_ZaZaisitu = "";

   //**********************************
   //***  ���x�v���̍ގ����ނ̌���
   //**********************************
   if (TanSyurui == "S1"){
      if (s_HEADS_DATA[898].ToIntDef(0) == 2) {   // ���x�v��ނ��O�Q�F�X�`�[���p�̏ꍇ�́A�r�t�r�S�P�O�Ƃ���
        return "�r�t�r�S�P�O";
      } else {
         if (TanName.Trim() != "") {
            s_NT_Zaisitu = s_HEADS_DATA[1060].SubString(1,4);   // B�[�Ǎގ�
         } else {
            s_NT_Zaisitu = s_HEADS_DATA[894].TrimRight();   // S1ɽ�ٍގ�
         }
      }
   } else if (TanSyurui == "S2"){
      if (s_HEADS_DATA[913].ToIntDef(0) == 2) {   // ���x�v��ނ��O�Q�F�X�`�[���p�̏ꍇ�́A�r�t�r�S�P�O�Ƃ���
        return "�r�t�r�S�P�O";
      } else {
         if (TanName.Trim() != "") {
            s_NT_Zaisitu = s_HEADS_DATA[1061].SubString(1,4);   // A�[�Ǎގ�
         } else {
            s_NT_Zaisitu = s_HEADS_DATA[909].TrimRight();   // S2ɽ�ٍގ�
         }
      }
   } else if (TanSyurui == "S3"){
      if (s_HEADS_DATA[928].ToIntDef(0) == 2) {   // ���x�v��ނ��O�Q�F�X�`�[���p�̏ꍇ�́A�r�t�r�S�P�O�Ƃ���
        return "�r�t�r�S�P�O";
      } else {
         if (TanName.Trim() != "") {
            s_NT_Zaisitu = s_HEADS_DATA[1060].SubString(1,4);   // B�[�Ǎގ�
         } else {
            s_NT_Zaisitu = s_HEADS_DATA[924].TrimRight();   // S3ɽ�ٍގ�
         }
      }
   } else if (TanSyurui == "S4"){
      if (s_HEADS_DATA[943].ToIntDef(0) == 2) {   // ���x�v��ނ��O�Q�F�X�`�[���p�̏ꍇ�́A�r�t�r�S�P�O�Ƃ���
        return "�r�t�r�S�P�O";
      } else {
         if (TanName.Trim() != "") {
            s_NT_Zaisitu = s_HEADS_DATA[1061].SubString(1,4);   // A�[�Ǎގ�
         } else {
            s_NT_Zaisitu = s_HEADS_DATA[939].TrimRight();   // S4ɽ�ٍގ�
         }
      }
   }

//   //**********************************
//   //***  �ގ����ނ̕ϊ�
//   //**********************************
//   switch (s_NT_Zaisitu.ToIntDef(0)){
//      case 1001:
//         s_Code = "1003";
//         break;
//      case 1002:
//         s_Code = "1004";
//         break;
//      case 1003:
//         s_Code = "1003";
//         break;
//      case 1004:
//         s_Code = "1004";
//         break;
//      case 1005:
//         s_Code = "1005";
//         break;
//      case 1006:
//         s_Code = "1006";
//         break;
//      case 1012:
//         s_Code = "1045";
//         break;
//      case 1013:
//         s_Code = "1045";
//         break;
//      case 1014:
//         s_Code = "1032";
//         break;
//      case 1015:
//         s_Code = "1033";
//         break;
//      case 1016:
//         s_Code = "1045";
//         break;
//      case 1017:
//         s_Code = "1045";
//         break;
//      case 1018:
//         s_Code = "1045";
//         break;
//      case 1031:
//         s_Code = "1032";
//         break;
//      case 1045:
//         s_Code = "1045";
//         break;
//      case 1056:
//         s_Code = "1056";
//         break;
//      case 1064:
//         s_Code = "1045";
//         break;
//      default:
//         s_Code = "";
//
//         //return s_T_ZaZaisitu;
//         break;
//   }

   //**********************************
   //***  �ގ�����Ͻ��̌���
   //**********************************
   s_Code = Chg_Zaisitu_Code( s_NT_Zaisitu, 1);
   s_T_ZaZaisitu = Get_Zaisitu_NameA3(s_Code);

   return s_T_ZaZaisitu;
}

//---------------------------------------------------------------------------
// ���{��֐����F �X�^�[�g�v���[�g�擾
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F ���i�W�J�ް�,HEARTS�p�ް��ɍE���ް���������
//---------------------------------------------------------------------------
bool __fastcall Get_Start_PlateA3(AnsiString &Side_B, AnsiString &Side_A, HeadsData* pcHeadsData){

   FILE *wkfp;

   // CSV�Ǎ��p
   char  buf[1024];                            /* ������o�b�t�@   */
   char  *tok;                                 /* �g�[�N���|�C���^ */
   int   i;                                    /* ���[�v�J�E���^   */
   char  token[100][100];                      /* �g�[�N���o�b�t�@ */

   // ��ڰčE���ް��p
   AnsiString s_Ana_Plate;    //
   AnsiString s_Ana_Kata;     // �^��
   AnsiString s_Ana_Kbn;      // �敪
   AnsiString s_Ana_ABKbn;    // AB�敪
   AnsiString s_Ana_Anaake;   // �E��
   AnsiString s_Ana_Suryo;    // 1�䕪����
   AnsiString s_Ana_Zaisitu;  // ��ڰčގ�
   AnsiString s_Ana_Siyou;    // �d�l

   AnsiString s_Text;

   AnsiString wk_FilePass;
   AnsiString hs_FilePass;

   // 2007.10.10 �װ�d�l�Ή� �d�l���ߎ擾
   AnsiString s_SpecCode;
   s_SpecCode = AnsiString(pcHeadsData->GetSpecCode());

   // �����f�[�^�Z�b�g
   Side_B = "�a��";
   Side_A = "�`��";

   // ��ڰčE���ް��߽
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN + s_SpecCode + ".csv";
   wk_FilePass = G_AnaFilePass + "HOL" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".csv";

   if ((wkfp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "��ڰčE���ް� �w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);

   } else {
      // �Ǎ��݉\�̊�
      memset(buf,0x00,sizeof(buf));
      while(fgets(buf,sizeof(buf),wkfp)!=NULL){
         i = 0;                                 // �g�[�N���J�E���^������
         memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
         tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
         while(tok){                            // �g�[�N�������݂����
            sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
            tok = GetToken(NULL,",");           // ���̃g�[�N��������

            if (i>100) {
               break;
            }
         }

         s_Ana_Plate  = token[0];               // ��ڰčE���ް��� �^��
         s_Ana_Kbn    = token[1];               // ��ڰčE���ް��� �敪
         s_Ana_ABKbn  = token[2];               // ��ڰčE���ް��� AB�敪
         s_Ana_Anaake = token[3];               // ��ڰčE���ް��� �E��
         s_Ana_Suryo  = token[4];               // ��ڰčE���ް��� 1�䕪����
         s_Ana_Zaisitu= token[5];               // ��ڰčE���ް��� ��ڰČ^��

         // �L��ں�������
//         if (s_Ana_Plate.SubString(1,2) == G_K_Syurui && s_Ana_Plate.SubString(4,2) == G_K_Model1 && s_Ana_Kbn == "D"){
         if ( s_Ana_Kbn == "D"){
            if (s_Ana_ABKbn == "B"){
               Side_B = "�a���i�X�^�[�g�j";
               Side_A = "�`��";
            } else {
               Side_B = "�a��";
               Side_A = "�`���i�X�^�[�g�j";
            }
         }
      }
   }

   fclose(wkfp);  // ��ڰčE���ް� ����

   //delete wkfp;

   return true;
}


//---------------------------------------------------------------------------
// ���{��֐����F �V�[�g�K�X�P�b�g�ގ��擾
// �T  �v      �F
// ��  ��      �F s_Side�F���̎��(A:A��,B:B��)
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2017.10.06�ǉ�
//---------------------------------------------------------------------------
AnsiString __fastcall Get_SheetGasketMatA3(AnsiString s_Side, HeadsData* pcHeadsData)
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

   AnsiString s_HEADS_DATA[1500];

   // AnsiString�^��HEADS�ް����擾
   pcHeadsData->SetAnsiStringData(s_HEADS_DATA);

   //***************************
   //***  �^       ��
   //***************************
   i_RecNo = 33;
   s_Katasiki = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  ��       ��
   //***************************
   i_RecNo = 34;
   s_Syurui = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  �a�X�`�[���敪(CASE1)
   //***************************
   i_RecNo = 846;
   s_Flow1 = s_HEADS_DATA[i_RecNo].TrimRight();

    //***************************
   //***  B�X�`�[���敪(CASE1)
   //***************************
   i_RecNo = 847;
   s_Flow2 = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  �a�X�`�[���敪(CASE2)
   //***************************
   i_RecNo = 869;
   s_Flow3 = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  B�X�`�[���敪(CASE2)
   //***************************
   i_RecNo = 870;
   s_Flow4 = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  �a���݌v���x
   //***************************
   i_RecNo = 829;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   d_Bdt = s_Text.ToDouble();

   //***************************
   //***  A���݌v���x
   //***************************
   i_RecNo = 834;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
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

   //***************************
   //***  �� �� �I ��
   //***************************
   s_GasMat="";
   s_Text="";
   if (i_Flow == 1) {
      // ���C�P�i�d�o�c�l�j
      s_GasMat ="�d�o�c�l";
   } else if (i_Flow == 2) {
      // ���C�Q�i�o���J�[�m���D�f�e�R�O�O�j
      s_GasMat ="�u�^���f�e�R�O�O";
   } else {
      if(s_Side=="B") {
         //�a���v���[�g�K�X�P�b�g
         i_RecNo = 835;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      } else if (s_Side=="A") {
         //�`���v���[�g�K�X�P�b�g
         i_RecNo = 836;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      }
      s_Text = Chg_Zaisitu_Code(s_Text,"2");
      s_GasMat = Get_Zaisitu_NameA3(s_Text);
   }

   return s_GasMat;

}


//---------------------------------------------------------------------------
// ���{��֐����F ���x�v�ی�ǐ��@�擾
// �T  �v      �F
// ��  ��      �F Key       �F���x�v���
//                s_Kikaku  �F�m�Y���K�i�R�[�h
//                s_FrngSize�F�m�Y�����a
// �߂�l      �F �ی�ǐ��@
// ��  �l      �F 2019.10.18�ǉ�
//---------------------------------------------------------------------------
AnsiString __fastcall Get_Well_Temp(AnsiString Key, AnsiString s_Kikaku,AnsiString s_FrngSize)
{
   int i_Kikaku;              // �K�i���
   AnsiString s_hogokan;      // �ی��


   //***************************
   //***  �K       �i
   //***************************
   if ( s_Kikaku.SubString(1,2)=="04" || s_Kikaku.SubString(1,2)=="05" ||
        s_Kikaku.SubString(1,2)=="08" || s_Kikaku.SubString(1,2)=="09" ||
        s_Kikaku.SubString(1,2)=="44" || s_Kikaku.SubString(1,2)=="48" ) {
      // ANSI,JPI
      i_Kikaku = 1;
   }
   else if ( s_Kikaku.SubString(1,2)=="06") {
      // �T���^�j
      i_Kikaku = 2;
   }
   else {
      i_Kikaku = 0;
   }

   //***************************
   //***  ��   ��   ��
   //***************************
   if ( Key == "01" || Key == "05" || Key == "06" ) {
      // �h�^�A�h�^���p�A�k�^���p
      if( i_Kikaku == 1 ) { // ANSI,JPI
         if ( s_FrngSize.ToDouble() <= 1 ) {
            s_hogokan = " / 56L";
         }
         else if ( s_FrngSize.ToDouble() <= 2 ) {
            s_hogokan = " / 71L";
         }
         else if ( s_FrngSize.ToDouble() <= 4 ) {
            s_hogokan = " / 101L";
         }
         else if ( s_FrngSize.ToDouble() <= 14 ) {
            s_hogokan = " / 131L";
         }
         else {
            s_hogokan = "";
         }
      }
      else {
         if ( s_FrngSize.ToDouble() <= 25 ) {
            s_hogokan = " / 56L";
         }
         else if ( s_FrngSize.ToDouble() <= 50 ) {
            s_hogokan = " / 71L";
         }
         else if ( s_FrngSize.ToDouble() <= 100 ) {
            s_hogokan = " / 101L";
         }
         else if ( s_FrngSize.ToDouble() <= 350 ) {
            s_hogokan = " / 131L";
         }
         else {
            s_hogokan = "";
         }
      }
   }
   else if ( Key == "02" || Key == "10" || Key == "11" ) {
      // �h�^�A�h�^���p�A�k�^���p�i�X�`�[���j
      s_hogokan = " / 56L";
   }
   else if ( Key == "03" ) {
      // �T�j�^��
      if ( s_FrngSize.ToDouble() <= 2.5 ) {
         s_hogokan = " / 65L";
      }
      else if ( s_FrngSize.ToDouble() <= 4 ) {
         s_hogokan = " / 105L";
      }
      else {
         s_hogokan = "";
      }
   }
   else if ( Key == "04" ) {
      // �T�j�^���i�X�`�[���j
      s_hogokan = " / 65L";
   }
   else {
      s_hogokan = "";
   }

   return s_hogokan;

}


//---------------------------------------------------------------------------
// ���{��֐����F ���L�R�[�h�L���m�F
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F ���L����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2022.04.04�ǉ�
//---------------------------------------------------------------------------
bool __fastcall Tokki_Code(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu)
{

   bool bSearch;

   bSearch = false;

   if ( s_HEADS_DATA[451].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[454].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[457].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[460].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[463].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[466].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[469].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[472].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[475].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[478].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[481].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[484].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[487].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[490].TrimRight() == Key ) {
      bSearch = true;
   }

   if( bSearch ){
      return(true);
   } else {
      return(false);
   }

}

//---------------------------------------------------------------------------
// ���{��֐����F ���L���ځi�S�p�j�擾
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F ���L����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2022.04.05�ǉ�
//---------------------------------------------------------------------------
AnsiString __fastcall Tokki_Zen(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu)
{

   AnsiString s_RetText;

   s_RetText = "";

   if ( s_HEADS_DATA[451].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[453].TrimRight();
   }
   if ( s_HEADS_DATA[454].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[456].TrimRight();
   }
   if ( s_HEADS_DATA[457].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[459].TrimRight();
   }
   if ( s_HEADS_DATA[460].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[462].TrimRight();
   }
   if ( s_HEADS_DATA[463].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[465].TrimRight();
   }
   if ( s_HEADS_DATA[466].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[468].TrimRight();
   }
   if ( s_HEADS_DATA[469].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[471].TrimRight();
   }
   if ( s_HEADS_DATA[472].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[474].TrimRight();
   }
   if ( s_HEADS_DATA[475].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[477].TrimRight();
   }
   if ( s_HEADS_DATA[478].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[480].TrimRight();
   }
   if ( s_HEADS_DATA[481].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[483].TrimRight();
   }
   if ( s_HEADS_DATA[484].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[486].TrimRight();
   }
   if ( s_HEADS_DATA[487].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[489].TrimRight();
   }
   if ( s_HEADS_DATA[490].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[492].TrimRight();
   }

   return(s_RetText);

}







