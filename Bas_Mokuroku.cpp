//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�}�ʖژ^  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.05.31
//�@ �X �V ���F2002.09.10
//             2007.10.09 �װ�d�l�Ή�
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_Mokuroku.h"

#include <math.h>       //2004.08.16 �؂�グ
#include <StrUtils.hpp> //2007.11.16 �@�햼�̒��o

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F �}�ʖژ^�@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Mokuroku(HeadsData* pcHeadsData)
{
   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�
   AnsiString s_Text;            // �ėp������
   //2004.07.15 �ϐ��ǉ�
   AnsiString s_Text2;           // �ėp������Q
   int        i_Length;          // ������
   //*******************
   int        i_RecNo;           // HEADS�ް� ں���No
   //2019.10.31 NK�ʎY�O���L_S
   AnsiString s_Text3;           // �ėp������R
   //2019.10.31 NK�ʎY�O���L_E
   // 2020.04.20 �����������L�ǉ�_S
   AnsiString s_Text4;           // �ėp������S
   AnsiString s_Text5;           // �ėp������T
   // 2020.04.20 �����������L�ǉ�_E

   AnsiString s_UtiKbn;
   int        i_Pos;

   // 2007.11.15 �ϐ��ǉ�
   AnsiString s_TextA;  // �@�햼��(�~���[A�d�l)
   AnsiString s_TextB;  // �@�햼��(�~���[B�d�l)
   //*******************

   // 2022.01.06 ��߰����L�ǉ�_E
   AnsiString s_S1Nzl_Syurui; // S1ɽ�َ��
   AnsiString s_S2Nzl_Syurui; // S2ɽ�َ��
   AnsiString s_S3Nzl_Syurui; // S3ɽ�َ��
   AnsiString s_S4Nzl_Syurui; // S4ɽ�َ��
   AnsiString s_E1Nzl_Syurui; // E1ɽ�َ��
   AnsiString s_E2Nzl_Syurui; // E2ɽ�َ��
   AnsiString s_E3Nzl_Syurui; // E3ɽ�َ��
   AnsiString s_E4Nzl_Syurui; // E4ɽ�َ��
   // 2022.01.06 ��߰����L�ǉ�_E

   // 2022.04.11 ���E����C��
   bool bTok_S;               // S�����E�׸�
   bool bTok_E;               // E�����E�׸�
   // 2022.04.11 ���E����C��

   bool bRet;
   AnsiString s_HEADS_DATA[1500];

   // ���d�l�łȂ��ꍇ�͏����s�v(����I��)
   if(!pcHeadsData->GetSpecType()==DEF_SPEC_TYPE_NONE) return(true);

   // HEADS�ް���۰�ٕϐ��ɾ��
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);

   s_Tmplate_Pass = G_Template_Dir + "�}�ʖژ^.xls";                 // ����ڰ�̧��
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "DLT" + G_SAKUBAN + ".xls"; // ��߰��̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "DLT" + GetFileSeizouBangou(G_SAKUBAN) + ".xls"; // ��߰��̧��

   // ۸ލ쐬
   G_Log_Text = "------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w�}�ʖژ^�x�쐬�J�n    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "------------------------------------------------";
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
   //***                         �}�ʖژ^�@�f�[�^�Z�b�g                      ***
   //***                                                                     ***
   //***************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�}�ʖژ^");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�}�ʖژ^"));

   // �ł��o���敪
   s_UtiKbn = s_HEADS_DATA[1259].SubString(1,1);

   // 2017.06.28 �z�t��ǉ�_S
   //*******************
   //***  �z  �t  ��
   //*******************
   i_RecNo = 438;   // �ƊE����
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "0009"){
       // �ƊE����009(�܂邲�ƃp�b�N�A�o���H��)
       s_Text = "���@�������5���(�H���T�� �E���޽�@�E��ڰā@�E�g���@�E�����@�E(ɽ�ف��g��orɽ�ف��O�H�����g��or�O�H�����g��))�@�i���ۏ؎��1���";
   } else {
       s_Text = "���@�������5���(�H���T�� �E��ڰā@�E�g���@�E�����@�E(ɽ�ف��g��orɽ�ف��O�H�����g��or�O�H�����g��))�@�i���ۏ؎��1���";
   }
   Excel_Cell_DataSet(1, 1, s_Text);
   G_Log_Text = "�z�t��               RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   // 2017.06.28 �z�t��ǉ�_E

   //*******************
   //***  ��i���t
   //*******************
   // �wA001�x
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(3, 14, s_Text);
   G_Log_Text = "��i���t             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2011.05.30 ��n�����ԍ��ǉ�
   //*******************
   //*** ��n�����ԍ�
   //*******************
   i_RecNo = 1103;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 1, s_Text);
   G_Log_Text = "��n�����ԍ�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //****************************

   //*******************
   //***  �����ԍ�
   //*******************
   // �wA002�x
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E   

   //----------------------------------------------
   // 2007.10.09 �װ�d�l�̏ꍇ�A������"A/B"��ǉ�
   if(pcHeadsData->GetMirrorFlag()){
      s_Text = s_Text + "A/B";
   }
   // 2007.10.09
   //----------------------------------------------

   Excel_Cell_DataSet(7, 1, s_Text);
   G_Log_Text = "�����ԍ�            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //***  �ژ^���
   //*******************
   // �wA003�x
//   s_Text = "H4�W����";
//   Excel_Cell_DataSet(6, 3, s_Text);
//   G_Log_Text = "�ژ^���            �w" + s_Text + "�x���Z�b�g�B";
//   Write_Log(G_Log_Text);

   //*******************
   //***  �^  ��
   //*******************
   // �wA004�x
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(7, 3, s_Text);
   G_Log_Text = "�^��                 RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //***  ��  ��
   //*******************
   // �wA005�x
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(7, 7, s_Text);
      G_Log_Text = "�䐔                 RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�䐔          RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //*******************
   //***  �[����
   //*******************
   // �wA006�x
   i_RecNo = 1065;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(7, 9, s_Text);
   G_Log_Text = "�[����               RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //********************************
   //***  �a���@�o�͐}�ʃZ�b�g
   //********************************
   i_Pos = s_HEADS_DATA[36].Pos("M");
   if (i_Pos != 0){
      SyuturyokuZumen_Set_S(s_UtiKbn, true, pcHeadsData);     // ���p
   } else {
      SyuturyokuZumen_Set_S(s_UtiKbn, false, pcHeadsData);    // �W��
   }


//   if (s_UtiKbn == "0" || s_UtiKbn == "9"){
//      //***************  �ł��o���敪�@�O�C�X  **********************
//
//      SyuturyokuZumen_Set_S('A', "J");     // �o�̓p�^�[���`
//
//   } else if (s_UtiKbn == "1"){
//      //***************  �ł��o���敪�@�P  **********************
//      SyuturyokuZumen_Set_S('B', "J");     // �o�̓p�^�[���a
//
//   } else if (s_UtiKbn == "2"){
//      //***************  �ł��o���敪�@�Q  **********************
//
//      SyuturyokuZumen_Set_S('C', "J");     // �o�̓p�^�[���b
//
//   } else if (s_UtiKbn == "3"){
//      //***************  �ł��o���敪�@�R  **********************
//
//      if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//         // ����
//         SyuturyokuZumen_Set_S('E', "J");  // �o�̓p�^�[���d
//      } else {
//         // �t
//         SyuturyokuZumen_Set_S('D', "J");  // �o�̓p�^�[���c
//      }
//
//   } else if (s_UtiKbn == "4"){
//      //***************  �ł��o���敪�@�S  **********************
//      s_Text = s_HEADS_DATA[29].TrimRight();   // ���͗e��i�K�i���ށj
//
//      if (s_HEADS_DATA[327].TrimRight() == ""){
//         // ��������
//         i_Pos = s_HEADS_DATA[36].Pos("M");
//         if (i_Pos != 0){
//            // ���p
//            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
//               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//                  // ����
//                  SyuturyokuZumen_Set_S('G', "J");  // �o�̓p�^�[���f
//               } else {
//                  // �t
//                  SyuturyokuZumen_Set_S('F', "J");  // �o�̓p�^�[���e
//               }
//            } else {
//               SyuturyokuZumen_Set_S('C', "J");     // �o�̓p�^�[���b
//            }
//         } else {
//            // �񔕗p
//            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
//               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//                  // ����
//                  SyuturyokuZumen_Set_S('I', "J");  // �o�̓p�^�[���h
//               } else {
//                  // �t
//                  SyuturyokuZumen_Set_S('H', "J");  // �o�̓p�^�[���g
//               }
//            } else {
//               SyuturyokuZumen_Set_S('J', "J");     // �o�̓p�^�[���i
//            }
//         }
//
//      } else {
//         // �����L��
//         i_Pos = s_HEADS_DATA[36].Pos("M");
//         if (i_Pos != 0){
//            // ���p
//            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
//               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//                  // ����
//                  SyuturyokuZumen_Set_S('L', "J");  // �o�̓p�^�[���k
//               } else {
//                  // �t
//                  SyuturyokuZumen_Set_S('K', "J");  // �o�̓p�^�[���j
//               }
//            } else {
//               SyuturyokuZumen_Set_S('M', "J");     // �o�̓p�^�[���l
//            }
//         } else {
//            // �񔕗p
//            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
//               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//                  // ����
//                  SyuturyokuZumen_Set_S('O', "J");  // �o�̓p�^�[���n
//               } else {
//                  // �t
//                  SyuturyokuZumen_Set_S('N', "J");  // �o�̓p�^�[���m
//               }
//            } else {
//               SyuturyokuZumen_Set_S('P', "J");     // �o�̓p�^�[���o
//            }
//         }
//
//      }
//
//   } else if (s_UtiKbn == "5"){
//      //***************  �ł��o���敪�@�T  **********************
//
//      SyuturyokuZumen_Set_S('J', "J");
//
//   } else if (s_UtiKbn == "6"){
//      //***************  �ł��o���敪�@�U  **********************
//
//      if (s_HEADS_DATA[327].TrimRight() == ""){
//         // ��������
//         if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//            // ����
//            SyuturyokuZumen_Set_S('R', "J");  // �o�̓p�^�[���q
//         } else {
//            // �t
//            SyuturyokuZumen_Set_S('Q', "J");  // �o�̓p�^�[���p
//         }
//      } else {
//         // �����L��
//         if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//            // ����
//            SyuturyokuZumen_Set_S('T', "J");  // �o�̓p�^�[���s
//         } else {
//            // �t
//            SyuturyokuZumen_Set_S('S', "J");  // �o�̓p�^�[���r
//         }
//      }
//
//   } else if (s_UtiKbn == "7"){
//      //***************  �ł��o���敪�@�V  **********************
//
//      SyuturyokuZumen_Set_S('U', "J");        // �o�̓p�^�[���t
//
//   } else if (s_UtiKbn == "8"){
//      //***************  �ł��o���敪�@�W  **********************
//
//      SyuturyokuZumen_Set_S('V', "J");        // �o�̓p�^�[���u
//
//   }

   //---------------------------------------
   // 2007.10.10 �װ�d�l�Ή�
   // ��B�ɏo�͂��钍�L�ް���2�s���ɂ��炷

   //2019/05/17 ���p�Z�p�ǉ�
   if (!bIsNotKatashiki) {
   //*******************
   //*** ���L(�ڰѴװ)
   //*******************
   // �wB014�x
   i_RecNo = 1346;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(28, 2, s_Text);
   Excel_Cell_DataSet(30, 2, s_Text);
   G_Log_Text = "���L(�ڰѴװ)        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(���͌v�װ)
   //*******************
   // �wB015�x
   i_RecNo = 1345;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(29, 2, s_Text);
   Excel_Cell_DataSet(31, 2, s_Text);
   G_Log_Text = "���L(���͌n�װ)      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(���x�v�װ)
   //*******************
   // �wB016�x
   i_RecNo = 1344;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(30, 2, s_Text);
   Excel_Cell_DataSet(32, 2, s_Text);
   G_Log_Text = "���L(���x�v�װ)      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�[�Ǵװ)
   //*******************
   // �wB017�x
   i_RecNo = 1343;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(31, 2, s_Text);
   Excel_Cell_DataSet(33, 2, s_Text);
   G_Log_Text = "���L(�[�Ǵװ)        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(ɽ�ٴװ)
   //*******************
   // �wB018�x
   i_RecNo = 1342;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(32, 2, s_Text);
   Excel_Cell_DataSet(34, 2, s_Text);
   G_Log_Text = "���L(ɽ�ٴװ)        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(IERPV�װ)
   //*******************
   // �wB019�x
   i_RecNo = 1340;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(33, 2, s_Text);
   Excel_Cell_DataSet(35, 2, s_Text);
   G_Log_Text = "���L(IERPV�װ)       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(����)
   //*******************
   // �wB021�x
   i_RecNo = 1334;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(35, 2, s_Text);
   Excel_Cell_DataSet(37, 2, s_Text);
   G_Log_Text = "���L(����)           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2011.02.09 IERR�װ�ǉ�
   //*******************
   //*** ���L(IERR�װ)
   //*******************
   // �wB022�x
   i_RecNo = 1341;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(38, 2, s_Text);
   G_Log_Text = "���L(IERR�װ)        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //***********************

   //*******************
   //*** ���L(���߽)
   //*******************
   // �wB023�x
   i_RecNo = 1333;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(37, 2, s_Text);
   Excel_Cell_DataSet(39, 2, s_Text);
   G_Log_Text = "���L(���߽)          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2007.10.10 �װ�d�l�Ή�
   //---------------------------------------

   //*******************
   //*** �p��K�i
   //*******************
   // �wB150�x
   //2009.06.23 ASMEɽ�ٖ��̕ύX
   //s_Text = "�a���p�� " + s_HEADS_DATA[1154].TrimRight() + "  "+ "�`���p�� " + s_HEADS_DATA[1158].TrimRight();
   s_Text2 = s_HEADS_DATA[1154].TrimRight();
   if ( s_Text2.SubString(1,17) == "ASME CLASS150-NPS" ) {
       s_Text2 = "ASME CLS150-" + s_Text2.Delete(1,17);
   } else if ( s_Text2.SubString(1,17) == "ASME CLASS300-NPS" ) {
       s_Text2 = "ASME CLS300-" + s_Text2.Delete(1,17);
   // 2013.04.03 ɽ�ًK�i�ǉ�
   } else if ( s_Text2.SubString(1,17) == "ASME CLASS400-NPS" ) {
       s_Text2 = "ASME CLS400-" + s_Text2.Delete(1,17);
   // ***********************
   // 2011.11.29 ɽ�ًK�i�ύX
   //// 2011.06.20 ɽ�ًK�i�ύX
   //} else if ( s_Text2.SubString(1,14) == "GB PN1.0MPa-DN" ) {
   //    s_Text2 = "GB PN1.0-DN" + s_Text2.Delete(1,14);
   //} else if ( s_Text2.SubString(1,14) == "GB PN1.6MPa-DN" ) {
   //    s_Text2 = "GB PN1.6-DN" + s_Text2.Delete(1,14);
   //} else if ( s_Text2.SubString(1,14) == "GB PN2.5MPa-DN" ) {
   //    s_Text2 = "GB PN2.5-DN" + s_Text2.Delete(1,14);
   //// ***********************
   // ***********************
   }
   //2014.05.28 �p��K�i�\�L�ύX
   //s_Text = "�a���p�� " + s_Text2;
   if ( s_Text2.SubString(1,3) == "JIS" ) {
       s_Text = "�a���p�� " + s_Text2;
   } else {
       s_Text = "�a���p�� ��" + s_Text2;
   }
   //***************************
   s_Text2 = s_HEADS_DATA[1158].TrimRight();
   if ( s_Text2.SubString(1,17) == "ASME CLASS150-NPS" ) {
       s_Text2 = "ASME CLS150-" + s_Text2.Delete(1,17);
   } else if ( s_Text2.SubString(1,17) == "ASME CLASS300-NPS" ) {
       s_Text2 = "ASME CLS300-" + s_Text2.Delete(1,17);
   // 2013.04.03 ɽ�ًK�i�ǉ�
   } else if ( s_Text2.SubString(1,17) == "ASME CLASS400-NPS" ) {
       s_Text2 = "ASME CLS400-" + s_Text2.Delete(1,17);
   // ***********************
   // 2011.11.29 ɽ�ًK�i�ύX
   //// 2011.06.20 ɽ�ًK�i�ύX
   //} else if ( s_Text2.SubString(1,14) == "GB PN1.0MPa-DN" ) {
   //    s_Text2 = "GB PN1.0-DN" + s_Text2.Delete(1,14);
   //} else if ( s_Text2.SubString(1,14) == "GB PN1.6MPa-DN" ) {
   //    s_Text2 = "GB PN1.6-DN" + s_Text2.Delete(1,14);
   //} else if ( s_Text2.SubString(1,14) == "GB PN2.5MPa-DN" ) {
   //    s_Text2 = "GB PN2.5-DN" + s_Text2.Delete(1,14);
   //// ***********************
   // ***********************
   }
   //2014.05.28 �p��K�i�\�L�ύX
   //s_Text = s_Text + "  �`���p�� " + s_Text2;
   if ( s_Text2.SubString(1,3) == "JIS" ) {
       s_Text = s_Text + "  �`���p�� " + s_Text2;
   } else {
       s_Text = s_Text + "  �`���p�� ��" + s_Text2;
   }
   //***************************
   //2004.04.23 YX-80�p��K�i�Ȃ�
   if ( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ){
      s_Text = "";
   }
   //****************************
   Excel_Cell_DataSet(10, 8, s_Text);
   G_Log_Text = "�p��        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2004.07.15 �����Ȗ��ǉ�
   //*******************
   //*** �����Ȗ���
   //*******************
   // �wB151�x
   if (s_HEADS_DATA[268].TrimRight() == "1") { // �����Ȗ���
      s_Text = "�����Ȏd�l����                 B���@�@�@             �@A��";
      Excel_Cell_DataSet(11, 8, s_Text);
      G_Log_Text = "�����Ȗ���           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //*******************
   //*** �����M��
   //*******************
   // �wB152�x
   if (s_HEADS_DATA[268].TrimRight() == "1") { // �����Ȗ���
      i_RecNo = 107;    // �����M��
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("########0",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 1) {
            s_Text ="        " + s_Text;
         } else if (i_Length == 2) {
            s_Text ="       " + s_Text;
         } else if (i_Length == 3) {
            s_Text ="      " + s_Text;
         } else if (i_Length == 4) {
            s_Text ="     " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="    " + s_Text;
         } else if (i_Length == 6) {
            s_Text ="   " + s_Text;
         } else if (i_Length == 7) {
            s_Text ="  " + s_Text;
         } else if (i_Length == 8) {
            s_Text =" " + s_Text;
         }
         s_Text = "�����M��(kW)                           " + s_Text;
      } else {
         G_ErrLog_Text = "�����M��        RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
         s_Text = "�����M��(kW)                           *********";
      }
      Excel_Cell_DataSet(12, 8, s_Text);
      G_Log_Text = "�����M��             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //*******************
   //*** �����o�����x
   //*******************
   // �wB153�x
   if (s_HEADS_DATA[268].TrimRight() == "1" && s_HEADS_DATA[269].TrimRight() == "11") { // �����Ȗ��g�P�P
      i_RecNo = 852;    // B���������x
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("###0.00",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 4) {
            s_Text ="   " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="  " + s_Text;
         } else if (i_Length == 6) {
            s_Text =" " + s_Text;
         }
         s_Text2 = "����/�o�����x(��)�@�@" + s_Text;
      } else {
         G_ErrLog_Text = "B���������x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
         s_Text2 = "����/�o�����x(��)�@�@****.**";
      }
      i_RecNo = 854;    // B���o�����x
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("###0.00",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 4) {
            s_Text ="/   " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="/  " + s_Text;
         } else if (i_Length == 6) {
            s_Text ="/ " + s_Text;
         } else {
            s_Text ="/" + s_Text;
         }
         s_Text2 = s_Text2 + s_Text;
      } else {
         G_ErrLog_Text = "B���o�����x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
         s_Text2 = s_Text2 + "/****.**";
      }
      i_RecNo = 853;    // A���������x
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("###0.00",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 4) {
            s_Text ="         " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="        " + s_Text;
         } else if (i_Length == 6) {
            s_Text ="       " + s_Text;
         } else {
            s_Text ="      " + s_Text;
         }
         s_Text2 = s_Text2 + s_Text;
      } else {
         G_ErrLog_Text = "A���������x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
         s_Text2 = s_Text2 + "      ****.**";
      }
      i_RecNo = 855;    // A���o�����x
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("###0.00",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 4) {
            s_Text ="/   " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="/  " + s_Text;
         } else if (i_Length == 6) {
            s_Text ="/ " + s_Text;
         } else {
            s_Text ="/" + s_Text;
         }
         s_Text2 = s_Text2 + s_Text;
      } else {
         G_ErrLog_Text = "A���o�����x     RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
         s_Text2 = s_Text2 + "/****.**";
      }
      s_Text = s_Text2;
      Excel_Cell_DataSet(13, 8, s_Text);
      G_Log_Text = "�����o�����x         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //*******************
   //*** ����
   //*******************
   // �wB154�x
   if (s_HEADS_DATA[268].TrimRight() == "1") { // �����Ȗ���
      i_RecNo = 856;    // B������
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_HEADS_DATA[846].TrimRight() == "1") { // �a���X�`�[��
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 �؂�グ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="     " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="    " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="   " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="  " + s_Text;
            } else if (i_Length == 8) {
               s_Text =" " + s_Text;
            }
            s_Text2 = "����(kg/min:lit/min)          " + s_Text;
         } else {
            G_ErrLog_Text = "B������         RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
            s_Text2 = "����(kg/min:lit/min)          ******.**";
         }
      } else if (s_HEADS_DATA[847].TrimRight() == "1") { // �`���X�`�[��
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 �؂�グ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() * 1000 / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() * 1000 / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="     " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="    " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="   " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="  " + s_Text;
            } else if (i_Length == 8) {
               s_Text =" " + s_Text;
            }
            s_Text2 = "����(lit/min:kg/min)          " + s_Text;
         } else {
            G_ErrLog_Text = "B������         RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
            s_Text2 = "����(lit/min:kg/min)          ******.**";
         }
      } else {                                    // �����t
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 �؂�グ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() * 1000 / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() * 1000 / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="     " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="    " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="   " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="  " + s_Text;
            } else if (i_Length == 8) {
               s_Text =" " + s_Text;
            }
            s_Text2 = "����(lit/min)                  " + s_Text;
         } else {
            G_ErrLog_Text = "B������         RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
            s_Text2 = "����(lit/min)                   ******.**";
         }
      }
      i_RecNo = 857;    // A������
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_HEADS_DATA[847].TrimRight() == "1") { // �`���X�`�[��
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 �؂�グ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="                   " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="                  " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="                 " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="                " + s_Text;
            } else if (i_Length == 8) {
               s_Text ="               " + s_Text;
            } else {
               s_Text ="              " + s_Text;
            }
            s_Text2 = s_Text2 + s_Text;
         } else {
            G_ErrLog_Text = "A������         RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
            s_Text2 = s_Text2 + "              ******.**";
         }
      } else {                                    // �`���t
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 �؂�グ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() * 1000 / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() * 1000 / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="                   " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="                  " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="                 " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="                " + s_Text;
            } else if (i_Length == 8) {
               s_Text ="               " + s_Text;
            } else {
               s_Text ="              " + s_Text;
            }
            s_Text2 = s_Text2 + s_Text;
         } else {
            G_ErrLog_Text = "A������         RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
            s_Text2 = s_Text2 + "              ******.**";
         }
      }
      s_Text = s_Text2;
      if (s_HEADS_DATA[269].TrimRight() == "11") { // �����Ȗ��g�P�P
         Excel_Cell_DataSet(14, 8, s_Text);
      } else {
         Excel_Cell_DataSet(13, 8, s_Text);
      }
      G_Log_Text = "����                 RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   ////2003.12.05 �~���[�d�l���L�ǉ�
   //// �wB154�x
   //i_RecNo = 1363;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(14, 8, s_Text);
   //G_Log_Text = "���L(�װ�d�l)        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   //*****************************
   //*************************

   //*******************
   //*** ���L(����ݼ�)
   //*******************
   // �wB155�x
   i_RecNo = 1329;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(15, 8, s_Text);
   G_Log_Text = "���L(����ݼ�)        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2003.11.19 ���`�F�b�g���L�ǉ�
   // �wB156�x
   i_RecNo = 1335;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 8, s_Text);
   G_Log_Text = "���L(�����)          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*****************************

   //*******************
   //*** ���L(�`�ʂP)
   //*******************
   // �wB157�x
   i_RecNo = 1298;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(17, 8, s_Text);
   G_Log_Text = "���L(�`�ʂP)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�^���P)
   //*******************
   // �wB158�x
   i_RecNo = 1300;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 8, s_Text);
   G_Log_Text = "���L(�^���P)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�d�������P)
   //*******************
   // �wB159�x
   i_RecNo = 1304;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(19, 8, s_Text);
   G_Log_Text = "���L(�d�������P)     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(��ڰĖ����P)
   //*******************
   // �wB160�x
   i_RecNo = 1306;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(20, 8, s_Text);
   G_Log_Text = "���L(��ڰĖ����P)    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(��ڰčގ��P)
   //*******************
   // �wB161�x
   i_RecNo = 1308;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(21, 8, s_Text);
   G_Log_Text = "���L(��ڰčގ��P)    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�޽��čގ��P)
   //*******************
   // �wB162�x
   i_RecNo = 1310;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(22, 8, s_Text);
   G_Log_Text = "���L(�޽��čގ��P)   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(ɽ�قP)
   //*******************
   // �wB163�x
   i_RecNo = 1325;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(23, 8, s_Text);
   G_Log_Text = "���L(ɽ�قP)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(ɽ�قQ)
   //*******************
   // �wB164�x
   i_RecNo = 1326;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(24, 8, s_Text);
   G_Log_Text = "���L(ɽ�قQ)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�ڰэގ�)
   //*******************
   // �wB165�x
   i_RecNo = 1320;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(25, 8, s_Text);
   G_Log_Text = "���L(�ڰэގ�)       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(���čގ�)
   //*******************
   // �wB166�x
   i_RecNo = 1314;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(26, 8, s_Text);
   G_Log_Text = "���L(���čގ�)       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(���Ďd��P)
   //*******************
   // �wB167�x
   i_RecNo = 1315;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(27, 8, s_Text);
   G_Log_Text = "���L(���Ďd��P)     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�ݶ��ގ�)
   //*******************
   // �wB168�x
   i_RecNo = 1322;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(28, 8, s_Text);
   G_Log_Text = "���L(�ݶ��ގ�)       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�ݶ���)
   //*******************
   // �wB169�x
   i_RecNo = 1321;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(29, 8, s_Text);
   G_Log_Text = "���L(�ݶ���)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

//2004.01.07 ���L�폜
//   //*******************
//   //*** ���L(�ݶ��d��)
//   //*******************
//   // �wB170�x
//   i_RecNo = 1323;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   Excel_Cell_DataSet(30, 8, s_Text);
//   G_Log_Text = "���L(�ݶ��d��)       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
//   Write_Log(G_Log_Text);
//*******************
   //2011.03.11 �H���ʐ^�v���L�ǉ�
   //*******************
   //*** ���L(�H���ʐ^�v)
   //*******************
   // �wB170�x
   i_RecNo = 1102;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(30, 8, s_Text);
   G_Log_Text = "���L(�H���ʐ^�v)     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*****************************

   //*******************
   //*** ���L(�޲���ް�����P)
   //*******************
   // �wB171�x
   i_RecNo = 1318;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(31, 8, s_Text);
   G_Log_Text = "���L(�޲���ް�����P) RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(���Ė����P)
   //*******************
   // �wB172�x
   i_RecNo = 1312;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(32, 8, s_Text);
   G_Log_Text = "���L(���Ė����P)     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�֖��P)
   //*******************
   // �wB173�x
   i_RecNo = 1302;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(33, 8, s_Text);
   G_Log_Text = "���L(�֖��P)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�ڒ���)
   //*******************
   // �wB174�x
   i_RecNo = 1337;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(34, 8, s_Text);
   G_Log_Text = "���L(�ڒ���)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�t���[���d��)
   //*******************
   // �wB175�x
   i_RecNo = 1365;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 8, s_Text);
   G_Log_Text = "���L(�t���[���d��)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�D��)
   //*******************
   // �wB176�x
   i_RecNo = 1364;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   //2019.10.31 NK�ʎY�O���L_S
   G_Log_Text = "���L(�D��)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���擾�B";
   Write_Log(G_Log_Text);

   //NK�ʎY�O���L
   i_RecNo = 536;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();

   G_Log_Text = "NK�K�i             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text2 + "�x���擾�B";
   Write_Log(G_Log_Text);

   if (s_Text2 == "15") {
      //�m�j�Q��
      s_Text3 = "  �t���[����NK�����ō����鎖";
   } else if (s_Text2 == "29") {
      //�m�j�R��
      s_Text3 = "  �t���[����NK�����ō����鎖";
   } else {
      s_Text3 =  "";
   }

   G_Log_Text = "NK���L             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text3 + "�x���擾�B";
   Write_Log(G_Log_Text);

   s_Text = s_Text + s_Text3;
   //2019.10.31 NK�ʎY�O���L_E

   Excel_Cell_DataSet(36, 8, s_Text);
   G_Log_Text = "���L(�D��)         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(���͗e��)
   //*******************
   // �wB177�x
   i_RecNo = 1332;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(37, 8, s_Text);
   G_Log_Text = "���L(���͗e��)       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2017.10.06 �����������L�ǉ�_S
   //*******************
   //*** ���L(����������)
   //*******************
   // �wB179�x
   i_RecNo = 1057;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      s_Text = "���͗��ʗ��ʃe�[�v�i�Г���t���͕s�v�j�A���x�b�g�t";
      Excel_Cell_DataSet(38, 8, s_Text);
      G_Log_Text = "���L(����������)     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   //2017.10.06 �����������L�ǉ�_E

   //*******************
   //*** ���L(�֖�(��))
   //*******************
   // �wB179�x
   i_RecNo = 1338;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2017.10.06 �����������L�ǉ�_S
   //Excel_Cell_DataSet(38, 8, s_Text);
   Excel_Cell_DataSet(38, 5, s_Text);
   //2017.10.06 �����������L�ǉ�_E
   G_Log_Text = "���L(�֖�(��))       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(���͗e��)
   //*******************
   // �wB181�x
   i_RecNo = 1339;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2017.10.06 �����������L�ǉ�_S
   //Excel_Cell_DataSet(40, 8, s_Text);
   Excel_Cell_DataSet(40, 5, s_Text);
   //2017.10.06 �����������L�ǉ�_E
   G_Log_Text = "���L(�Ǘ����(��))   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�m�Y���R)
   //*******************
   // �wB183�x
   i_RecNo = 1327;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(42, 8, s_Text);
   G_Log_Text = "���L(�Ǘ����(��))   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //*** ���L(�m�Y���S)
   //*******************
   // �wB184�x
   i_RecNo = 1328;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(43, 8, s_Text);
   G_Log_Text = "���L(�Ǘ����(��))   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //-----------------------------
   // 2007.10.10 �װ�d�l�Ή�

   if(!pcHeadsData->GetMirrorFlag()){
        // �װ�d�l�łȂ��ꍇ
        //2006.11.22 �@�햼�̒��L�ǉ�
        i_Pos = s_HEADS_DATA[36].Pos("M");
        if (i_Pos != 0){
            // ���p

            //*******************
            //*** �@�햼�̂P
            //*******************
            // �wB185�x
            i_RecNo = 26;
            s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
            s_Text = "�@�햼�́F" + s_Text;
            Excel_Cell_DataSet(44, 8, s_Text);
            G_Log_Text = "�@�햼�̂P           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            //*******************
            //*** �@�햼�̂Q
            //*******************
            // �wB186�x

        }
   //***************************
   }else{
        //2008.01.08 �װ�d�l(���p�ȊO)�C��
        //// �@�햼��(���d�l)
        //i_RecNo = 26;
        //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
        //
        //// �~���[A�d�l�A�~���[B�d�l�̋@�햼�̂��쐬
        //s_TextA = GetEquipmentName(s_Text, "A");
        //s_TextB = GetEquipmentName(s_Text, "B");
        //
        ////*******************
        ////*** �@�햼�̂P
        ////*******************
        //Excel_Cell_DataSet(44, 2, s_TextA);
        //G_Log_Text = "�@�햼�̂P           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_TextA + "�x���Z�b�g�B";
        //Write_Log(G_Log_Text);
        //
        //s_Text = "��A���@�@�햼��";
        //Excel_Cell_DataSet(44, 5, s_Text);
        //G_Log_Text = "�@�햼�̂P�i�}�ԁj   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
        //Write_Log(G_Log_Text);
        //
        //s_Text = "�E���L�@�햼�̂���֍���̎�";
        //Excel_Cell_DataSet(44, 8, s_Text);
        //G_Log_Text = "�@�햼�̂P�i���l�j   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
        //Write_Log(G_Log_Text);
        //
        ////*******************
        ////*** �@�햼�̂Q
        ////*******************
        //Excel_Cell_DataSet(45, 2, s_TextB);
        //G_Log_Text = "�@�햼�̂Q           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_TextB + "�x���Z�b�g�B";
        //Write_Log(G_Log_Text);
        //
        //s_Text = "��B���@�@�햼��";
        //Excel_Cell_DataSet(45, 5, s_Text);
        //G_Log_Text = "�@�햼�̂Q�i�}�ԁj   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
        //Write_Log(G_Log_Text);
        //
        //s_Text = "�EB���@�̖����󍀖ڂ�A���@�̍��E����Ⴂ�̂���";
        //Excel_Cell_DataSet(45, 8, s_Text);
        //G_Log_Text = "�@�햼�̂Q�i���l�j   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
        //Write_Log(G_Log_Text);
        i_Pos = s_HEADS_DATA[36].Pos("M");
        if (i_Pos != 0){
            // ���p
            // �@�햼��(���d�l)
            i_RecNo = 26;
            s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

            // �~���[A�d�l�A�~���[B�d�l�̋@�햼�̂��쐬
            s_TextA = GetEquipmentName(s_Text, "A");
            s_TextB = GetEquipmentName(s_Text, "B");

            //*******************
            //*** �@�햼�̂P
            //*******************
            Excel_Cell_DataSet(44, 2, s_TextA);
            G_Log_Text = "�@�햼�̂P           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_TextA + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            s_Text = "��A���@�@�햼��";
            Excel_Cell_DataSet(44, 5, s_Text);
            G_Log_Text = "�@�햼�̂P�i�}�ԁj   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            s_Text = "�E���L�@�햼�̂���֍���̎�";
            Excel_Cell_DataSet(44, 8, s_Text);
            G_Log_Text = "�@�햼�̂P�i���l�j   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            //*******************
            //*** �@�햼�̂Q
            //*******************
            Excel_Cell_DataSet(45, 2, s_TextB);
            G_Log_Text = "�@�햼�̂Q           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_TextB + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            s_Text = "��B���@�@�햼��";
            Excel_Cell_DataSet(45, 5, s_Text);
            G_Log_Text = "�@�햼�̂Q�i�}�ԁj   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);

            s_Text = "�EB���@�̖����󍀖ڂ�A���@�̍��E����Ⴂ�̂���";
            Excel_Cell_DataSet(45, 8, s_Text);
            G_Log_Text = "�@�햼�̂Q�i���l�j   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);
        } else {
            // ���p�ȊO

            //*******************
            //*** ���󍀖ڒ��L
            //*******************

            s_Text = "�EB���@�̖����󍀖ڂ�A���@�̍��E����Ⴂ�̂���";
            Excel_Cell_DataSet(45, 8, s_Text);
            G_Log_Text = "���󍀖ڒ��L         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);
        }
        //********************************
   }

   // 2007.10.10
   //-----------------------------

   //*******************
   //***  �����
   //*******************
   // �wC001�x
   i_RecNo = 431;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "0") {
      s_Text = "��";
   } else {
      s_Text = "�L" ;
   }
   Excel_Cell_DataSet(48, 3, s_Text);
   G_Log_Text = "�����            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //***  �����񍐏�
   //*******************
   // �wC002�x
   i_RecNo = 441;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "0") {
      s_Text = "�s�v";
   } else {
      s_Text = "�v" ;
   }
   Excel_Cell_DataSet(49, 3, s_Text);
   G_Log_Text = "�����񍐏�          �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //***  �ټ��
   //*******************
   // �wC003�x
   if (s_HEADS_DATA[433] != "0" || s_HEADS_DATA[434] != "0" || s_HEADS_DATA[435] != "0"
          || s_HEADS_DATA[436] != "0" || s_HEADS_DATA[437] != "0"){

      s_Text = "�v";
   } else {
      s_Text = "�s�v";
   }
   Excel_Cell_DataSet(50, 3, s_Text);
   G_Log_Text = "�ټ��               �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //***  ��ڰ�
   //*******************
   // �wC004�x
   i_RecNo = 433;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "��ڰ�" ;
   }
   Excel_Cell_DataSet(51, 3, s_Text);
   G_Log_Text = "��ڰ�               �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //***  �ڰ�
   //*******************
   // �wC005�x
   i_RecNo = 434;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "�ڰ�" ;
   }
   Excel_Cell_DataSet(52, 3, s_Text);
   G_Log_Text = "�ڰ�                �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  ���t����ů�
   //*********************
   // �wC006�x
   i_RecNo = 435;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "���t����ů�" ;
   }
   Excel_Cell_DataSet(53, 3, s_Text);
   G_Log_Text = "���t����ů�         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  ɽ��(�߲��)
   //*********************
   // �wC007�x
   i_RecNo = 436;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "ɽ��(�߲��)" ;
   }
   Excel_Cell_DataSet(51, 5, s_Text);
   G_Log_Text = "ɽ��(�߲��)         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  ɽ�وꎮ
   //*********************
   // �wC008�x
   i_RecNo = 437;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "ɽ�وꎮ" ;
   }
   Excel_Cell_DataSet(52, 5, s_Text);
   G_Log_Text = "ɽ�وꎮ            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  �\���i
   //*********************
   // �wC009�x
   i_RecNo = 1265;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = "��";
   } else {
      s_Text = "�L" ;
   }
   Excel_Cell_DataSet(54, 3, s_Text);
   G_Log_Text = "�\���i              �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   }

   //*********************
   //***  �[  ��
   //*********************
   // �wC010�x
   //i_RecNo = 12;
   i_RecNo = 411;
   s_Text = s_HEADS_DATA[i_RecNo];
   if(s_Text.Trim() != ""){
      s_Text = s_Text.SubString(1,4) + "�N" +
               s_Text.SubString(6,2) + "��" +
               s_Text.SubString(9,2) + "��";
   } else {
      s_Text = "";
   }
   Excel_Cell_DataSet(46, 9, s_Text);
   G_Log_Text = "�[��                 RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //--------------------------
   //2007.10.09 �װ�d�l�Ή�

   //2019/05/17 ���p�Z�p�ǉ�
   if (!bIsNotKatashiki) {
   if(pcHeadsData->GetMirrorFlag()){
        //*********************
        //***  �����󍀖�
        //*********************
        s_Text = "���@�@�@�@�@�@���@�@�@��@�@�@���@�@�@��(A���@)";
        Excel_Cell_DataSet(49, 7, s_Text);
        G_Log_Text = "�[�����󍀖�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
   }
   //2007.10.09
   //--------------------------

   //*********************
   //***  ���u�ԍ�
   //*********************
   // �wC011�x
   i_RecNo = 28;
   s_Text = "���u�ԍ�  " + ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   Excel_Cell_DataSet(50, 7, s_Text);
   G_Log_Text = "���u�ԍ�             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  �@��ԍ�
   //*********************
   // �wC012�x
   i_RecNo = 27;
   //2004.10.01
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   int w_count = s_Text.AnsiPos("�`");

   if (w_count){
      s_Text = "�@��ԍ�  " + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "�`" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
   }else{
      s_Text = "�@��ԍ�  " + ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   }

   // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_S
   if (Tokki_Code("39", s_HEADS_DATA, 1500) == true) {
      // �@��ԍ����L���ڗL
      // �@��ԍ����O���L�ɋL�q
      s_Text = "�����Q��";
   }
   // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_E

   Excel_Cell_DataSet(50, 11, s_Text);
   G_Log_Text = "�@��ԍ�             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

//   //*********************
//   //***  ���u�ԍ�/�@��ԍ�
//   //*********************
//   // �wC012�x
//    s_Text = "���u�ԍ� " + s_HEADS_DATA[28].TrimRight() + spc(20-s_HEADS_DATA[28].TrimRight().Length())
//           + "�@�@��ԍ� " + s_HEADS_DATA[27].TrimRight();
//   Excel_Cell_DataSet(50, 7, s_Text);
//   G_Log_Text = "�@��ԍ�             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
//   Write_Log(G_Log_Text);

   //*********************
   //***  B���݌v����
   //*********************
   // �wC013�x
   i_RecNo = 825;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   //2017.06.09 �����ǉ�
   //if (AllFloatChk(s_Text.c_str(),0) == 0){
   if (AllFloatChk(s_Text.c_str(),1) == 0){
   //*******************
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "��C�J��";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
      Excel_Cell_DataSet(52, 9, s_Text);
      G_Log_Text = "B���݌v����          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "B���݌v����            �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //*********************
   //***  B���݌v���x
   //*********************
   // �wC014�x
   i_RecNo = 829;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(53, 9, s_Text);
      G_Log_Text = "B���݌v���x      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "B���݌v���x        �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //************************
   //***  B��ýĈ��� ����
   //************************
   // �wC015�x
   i_RecNo = 827;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(54, 9, s_Text);
      G_Log_Text = "B��ýĈ��� ����      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "B��ýĈ��� ����        �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //************************
   //***  B��ýĈ��� ��
   //************************
   // �wC016�x
   i_RecNo = 826;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(55, 9, s_Text);
      G_Log_Text = "B��ýĈ��� ��      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "B��ýĈ��� ��        �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //************************
   //***  A���݌v����
   //************************
   // �wC017�x
   i_RecNo = 830;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   //2017.06.09 �����ǉ�
   //if (AllFloatChk(s_Text.c_str(),0) == 0){
   if (AllFloatChk(s_Text.c_str(),1) == 0){
   //*******************
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "��C�J��";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
      Excel_Cell_DataSet(52, 12, s_Text);
      G_Log_Text = "A���݌v����          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "A���݌v����            �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //************************
   //***  A���݌v���x
   //************************
   // �wC018�x
   i_RecNo = 834;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(53, 12, s_Text);
      G_Log_Text = "A���݌v���x      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "A���݌v���x        �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //************************
   //***  A��ýĈ��� ����
   //************************
   // �wC019�x
   i_RecNo = 832;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(54, 12, s_Text);
      G_Log_Text = "A��ýĈ��� ����      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "A��ýĈ��� ����        �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //************************
   //***  A��ýĈ��� ��
   //************************
   // �wC020�x
   i_RecNo = 831;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(55, 12, s_Text);
      G_Log_Text = "A��ýĈ��� ��      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "A��ýĈ��� ��        �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //************************
   //***  B�����e��
   //************************
   // �wC021�x
   i_RecNo = 1282;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(56, 9, s_Text);
   G_Log_Text = "B�����e��            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  �`�M�ʐ�
   //************************
   // �wC022�x
   i_RecNo = 1067;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(57, 9, s_Text);
   G_Log_Text = "�`�M�ʐ�             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  A�����e��
   //************************
   // �wC023�x
   i_RecNo = 1283;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(56, 12, s_Text);
   G_Log_Text = "A�����e��            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  �@�펿��
   //************************
   // �wC024�x
   i_RecNo = 1029;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(57, 12, s_Text);
   G_Log_Text = "�@�펿��             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  �K�p�K�i
   //************************
   // �wC025�x
   i_RecNo = 29;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "�K�p�K�i             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x�ɂ��A";
   if (s_HEADS_DATA[1].TrimRight() == "0") {    //�a��
      s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),true,false, s_HEADS_DATA[1].TrimRight());
      // 2011.05.30 ��n����
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if ( s_Text2.ToIntDef(0) == 0 ) {
         // 2012.01.20 ��n�t���O
         //if ( s_HEADS_DATA[5].TrimRight() == "42842" ) {
         //   //��n����(42842)(�a��)����
         //   s_Text = "��n����(�a��)";
         //} else if ( s_HEADS_DATA[5].TrimRight() == "43935" ) {
         //   //��n����(43935)(�a��)����
         //   s_Text = "��n����(�a��)";
         //} else if ( s_HEADS_DATA[401].TrimRight() == "42842" ) {
         //   //��n����(42842)(�a��)����
         //   s_Text = "��n����(�a��)";
         //} else if ( s_HEADS_DATA[401].TrimRight() == "43935" ) {
         //   //��n����(43935)(�a��)����
         //   s_Text = "��n����(�a��)";
         //}
         if ( s_HEADS_DATA[1107].TrimRight() == "CHINA" ) {
            //��n����(����)����
            s_Text = "��n����(����)";
         }
         // *********************
      }
      // *******************
   } else {
      s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),false,false, s_HEADS_DATA[1].TrimRight());
      // 2012.01.20 ��n�t���O
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if ( s_Text2.ToIntDef(0) == 0 ) {
         if ( s_HEADS_DATA[1107].TrimRight() == "CHINA" ) {
            //��n����(����)����
            s_Text = "��n����(����)";
         }
      }
      // *******************
   }
   i_RecNo = 1372;
   s_Text = s_Text + "  " + s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(58, 9, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  ���t���@ MAX
   //************************
   // �wC026�x
   i_RecNo = 1089;
   s_Text = "MAX " + s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(60, 9, s_Text);
   G_Log_Text = "���t���@ MAX         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  ���t���@ MIN
   //************************
   // �wC027�x
   i_RecNo = 1015;
   s_Text = "MIN " + s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(60, 12, s_Text);
   G_Log_Text = "���t���@ MIN         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  �������
   //************************
   i_RecNo = 319;
   if (s_HEADS_DATA[i_RecNo].TrimRight().ToIntDef(0) != 0){
      //************************
      //***  ��  ��
      //************************
      // �wC050�x
      i_RecNo = 319;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(59, 1, s_Text);
      G_Log_Text = "����                 RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      //************************
      //***  �ݒ�ύX
      //************************
      // �wC051�x
      s_Text = "�݌v�ύX";
      Excel_Cell_DataSet(59, 2, s_Text);
      G_Log_Text = "�ݒ�ύX            �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      //************************
      //***  �N����
      //************************
      // �wC052�x
      i_RecNo = 1087;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(59, 4, s_Text);
      G_Log_Text = "�N����               RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      //************************
      //***  �S����
      //************************
      // �wC053�x
      i_RecNo = 1079;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(59, 6, s_Text);
      G_Log_Text = "�S����               RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   }
    //************************
    //***  �c�ƎЈ��ԍ�
    //************************
    // �wC054�x
    i_RecNo = 2;
    s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
    Excel_Cell_DataSet(61, 1, s_Text);
    G_Log_Text = "�c�ƎЈ��ԍ�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);

    //************************
    //***  �c�ƎЈ���
    //************************
    // �wC055�x
    i_RecNo = 3;
    s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
    Excel_Cell_DataSet(61, 3, s_Text);
    G_Log_Text = "�c�ƎЈ���           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);

    //***************************
    //***  HEADS VER
    //***************************
    // �wC056�x
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
    Excel_Cell_DataSet(61, 14, s_Text);
    G_Log_Text = "HEADS VER           �w" + s_Text + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);

    //2019/05/17 ���p�Z�p�ǉ�
    if (!bIsNotKatashiki) {
    //--------------------------------
    // 2007.10.09 �װ�d�l�Ή�
    // 2007.10.10 �s=36�ɕύX
    //***************************
    //***  �װ�d�l
    //***************************
    if(pcHeadsData->GetMirrorFlag()){
        i_RecNo = 1363;
        s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
        //Excel_Cell_DataSet(34, 2, s_Text);  // "�~���[�d�l"
        Excel_Cell_DataSet(36, 2, s_Text);  // "�~���[�d�l"
        G_Log_Text = "�װ�d�l             �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2007.10.09
    // 2007.10.10
    //--------------------------------
    }

    // 2020.05.28 �����������L�ǉ�_S
    //*************************************
    //***  ���������w���P(���O(1.16))
    //*************************************
    s_Text = "";
    s_Text5 = "";

    s_Text = Get_HotAirDrying();
    i_Length = s_Text.Length();
    if (i_Length < 21 ) {
        s_Text = "";
    }

    if ( s_Text != "" ) {
        if(pcHeadsData->GetMirrorFlag()){
           s_Text = "A���@:" + s_Text;
        }
        s_Text5 = s_Text + "  ���`�F�b�N�V�[�g�䐔������";
        Excel_Cell_DataSet(1, 16, s_Text5);
        G_Log_Text = "���������w���P      �w" + s_Text5 + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }

    //*************************************
    //***  ���������w���Q(���O(2.16))
    //*************************************
    if(pcHeadsData->GetMirrorFlag()){
        s_Text = "";
        s_Text2 = "";
        s_Text3 = "";
        s_Text4 = "";
        s_Text5 = "";

        s_Text = Get_HotAirDrying();
        i_Length = s_Text.Length();
        if (i_Length < 21 ) {
            s_Text = "";
        }

        if ( s_Text != "" ) {
            //�O����
            s_Text2 = "B���@:" + s_Text.SubString(1,21);
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
            if ( s_Text.Pos("A(1)") > 0 ) {
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
            // B���@���L�F �O���� + A���� + ")/A(" + B���� + ")��"
            s_Text = s_Text2 + s_Text4 + ")/A(" + s_Text3 + ")��";
            s_Text5 = s_Text + "  ���`�F�b�N�V�[�g�䐔������";

            Excel_Cell_DataSet(2, 16, s_Text5);
        G_Log_Text = "���������w���Q      �w" + s_Text5 + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
        }
    }
    // 2020.05.28 �����������L�ǉ�_E

    // 2020.07.31 ���L�폜_S
    //// 2020.05.28 SX-41���p���L�ǉ�_S
    ////*************************************
    ////***  SX-41���p���L(���O(3.16))
    ////*************************************
    //if ( G_KS_Syurui == "�r�w" && G_KS_Model == "�S�P" ){
    //    //SX-41
    //    if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 96 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 97 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 98 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 99 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 396 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 397 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 398 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 399 ) {
    //        // NPM,NHPM,NUPM,NSPM
    //        s_Text = "���čގ���SNB7/S45C����t���@��(2.4+t)xN�̂���TS2019-146";
    //        Excel_Cell_DataSet(3, 16, s_Text);
    //        G_Log_Text = "SX-41���p���L       �w" + s_Text + "�x���Z�b�g�B";
    //        Write_Log(G_Log_Text);
    //    }
    //}
    //// 2020.05.28 SX-41���p���L�ǉ�_E
    // 2020.07.31 ���L�폜_E
    // 2020.07.31 �ی�J�o�[���L�ǉ�_S
    //*************************************
    //***  �ی�J�o�[���p���L(���O(3.16))
    //*************************************
    if (Tokki_Code("1E", s_HEADS_DATA, 1500) == true) {
        if(s_HEADS_DATA[36].Pos("M") == 0)  {
             // ���p�łȂ��ꍇ
             s_Text = "�ی�J�o�[�v";
        } else {
             // ���p�̏ꍇ
             s_Text = "���ݗp�ی�J�o�[�v";
        }
        Excel_Cell_DataSet(3, 16, s_Text);
        G_Log_Text = "�ی�J�o�[���L  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2020.07.31 �ی�J�o�[���L�ǉ�_E

    // 2020.05.12 E�m�Y���t���I�[�_�[�̌x���ǉ�_S
    //*************************************
    //***  Eɽ�ٕt�����ނ̌x��(���O(4.16))
    //*************************************
    // RX10NP/NHP�F99�`106���A143�`150���A210��
    // RX10NUP/NSP�F97�`104���A141�`148���A208�`210��
    // LX10�F56�`62���A87�`92���A133�`140��
    // 2022.01.26 ���L�C��_S
                     s_Text = "";
    // 2022.01.26 ���L�C��_E
    if ( G_KS_Syurui == "�q�w" && G_KS_Model == "�P�P" ){
            //RX-11
        if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 92 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 93 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 96 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 97 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 392 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 393 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 396 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 397 ) {
            // NP,NHP,NPM,NHPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 99) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 106)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 143) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 150)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if (s_HEADS_DATA[170].ToIntDef(0) == 210) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else {
                s_Text = "";
            }
        } else if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 94 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 95 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 98 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 99 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 394 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 395 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 398 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 399 ) {
            // NUP,NSP,NUPM,NSPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 97) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 104)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 141) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 148)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 208) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 210)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else {
                s_Text = "";
            }
        }
    } else if ( G_KS_Syurui == "�q�w" && G_KS_Model == "�P�R" ){
        //RX-13
        if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 92 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 93 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 96 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 97 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 392 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 393 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 396 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 397 ) {
            // NP,NHP,NPM,NHPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 99) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 106)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 143) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 150)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if (s_HEADS_DATA[170].ToIntDef(0) == 210) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else {
                s_Text = "";
            }
        } else if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 94 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 95 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 98 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 99 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 394 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 395 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 398 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 399 ) {
            // NUP,NSP,NUPM,NSPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 97) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 104)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 141) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 148)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 208) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 210)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else {
                 s_Text = "";
            }
        }
    } else if ( G_KS_Syurui == "�k�w" && G_KS_Model == "�P�O" ){
        //LX-10
        if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 92 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 93 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 94 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 96 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 97 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 98 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 392 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 393 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 394 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 936 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 937 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 398 ) {
            // NP,NHP,NUP,NPM,NHPM,NUPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 56) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 62)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 87) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 92)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 133) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 140)) {
                s_Text = "�V���K�C�h�o�[�����قȂ邽��CP���@�m�F�̎��i�݌v�֘A���̎��j";
            } else {
               s_Text = "";
            }
        }
    } else {
        s_Text = "";
    }

    // �d�m�Y�����Ȃ��ꍇ���L�͏o���Ȃ�
    if ( s_HEADS_DATA[951].ToIntDef(0) == 0 &&
         s_HEADS_DATA[966].ToIntDef(0) == 0 &&
         s_HEADS_DATA[981].ToIntDef(0) == 0 &&
         s_HEADS_DATA[996].ToIntDef(0) == 0 ){
        s_Text = "";
    }

    if ( s_Text != "" ) {
        Excel_Cell_DataSet(4, 16, s_Text);
        G_Log_Text = "Eɽ�ٕt�����ް�x��  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2020.05.12 E�m�Y���t���I�[�_�[�̌x���ǉ�_E

    // 2020.10.30 ���͗e��UNS�K�i�x���ǉ�_S
    if ( s_HEADS_DATA[29].ToIntDef(0) == 1 ||
         s_HEADS_DATA[29].ToIntDef(0) == 2 ||
         s_HEADS_DATA[29].ToIntDef(0) == 3 ){
        // ���͗e��i�K�i���ށj
        if ( s_HEADS_DATA[43].TrimRight() == "3472" ) {
            // �t�m�r�@�m�P�O�Q�V�U
            s_Text = "�i�h�r�K�i�ގ����L�v";
        } else {
            s_Text = "";
        }
    }  else {
        s_Text = "";
    }

    if ( s_Text != "" ) {
        Excel_Cell_DataSet(5, 16, s_Text);
        G_Log_Text = "���͗e��UNS�K�i�x��  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2020.10.30 ���͗e��UNS�K�i�x���ǉ�_E

    // 2021.04.15 ���L�ǉ�_S
    //*************************************
    //***  ���S���𔭑�(���O(5.16))
    //*************************************
    s_Text = "";

    i_RecNo = 1275;
    if (s_HEADS_DATA[i_RecNo].TrimRight() == "3"){
        // ���S���𔭑�
        s_Text = "���S���𔭑�";
        Excel_Cell_DataSet(5, 16, s_Text);
        G_Log_Text = "���S���𔭑�    �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }

    //***********************************************
    //***  SILVER(HEAT RESISTACE PAINT�@(���O(6.16))
    //***********************************************
    s_Text = "";

    i_RecNo = 31;
    if (s_HEADS_DATA[i_RecNo].TrimRight() == "B"){
        // SILVER(HEAT RESISTACE PAINT
        s_Text = "�ăV���b�g�u���X�g�v";
        Excel_Cell_DataSet(6, 16, s_Text);
        G_Log_Text = "SILVER(HEAT RESISTACE PAINT�@  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }

    //***********************************************
    //***  SILVER(HEAT RESISTACE PAINT�A(���O(7.17))
    //***********************************************
    s_Text = "";

    i_RecNo = 31;
    if (s_HEADS_DATA[i_RecNo].TrimRight() == "B"){
        // SILVER(HEAT RESISTACE PAINT
        s_Text = "���j�h���͑ϔM�h���̎��A�h���F�̓V���o�[";
        Excel_Cell_DataSet(7, 16, s_Text);
        G_Log_Text = "SILVER(HEAT RESISTACE PAINT�@  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }

    //*************************************
    //***  �W���O�h��(���O(8.16))
    //*************************************
    s_Text = "";

    // 2021.05.19 ���L�ǉ�_S
    //if (Tokki_Code("1T", s_HEADS_DATA, 1500) == true) {
    if (Tokki_Code("1X", s_HEADS_DATA, 1500) == true) {
    // 2021.05.19 ���L�ǉ�_E
        // �W���O�h��
        s_Text = "�W���O�h���g�p�^�H�����ӂ̂���";
        Excel_Cell_DataSet(8, 16, s_Text);
        G_Log_Text = "�W���O�h�����L  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2021.04.15 ���L�ǉ�_E

    // 2021.05.17 ���L�ǉ�_S
    //*************************************
    //***  �v�w���L(���O(9.16)�A(10.16))
    //*************************************
    s_Text = "";
    s_Text2 = "";
    s_Text3 = "";
    s_Text4 = "";
    s_Text5 = "";

    if ( G_KS_Syurui == "�v�w" ) {

        i_RecNo = 835;   // B���޽��čގ�
        s_Text3 = s_HEADS_DATA[i_RecNo].TrimRight();
        s_Text3 = Type_MAT(s_Text3);

        i_RecNo = 836;   // A���޽��čގ�
        s_Text4 = s_HEADS_DATA[i_RecNo].TrimRight();
        s_Text4 = Type_MAT(s_Text4);

        if ( s_Text3 == "TCG" || s_Text4 == "TCG" ) {
            s_Text5 = "TCG";
        }

        if ( G_KS_Model == "�T�R" && s_Text5 == "TCG" ) {
            //WX-53 TCG
            s_Text = "�n�ڃv���[�g��ƍH���t���[EX2005-046(�ŐV��)�o�}�v";
            s_Text2 = "�n�ڌ����v�̏�EX2020-056(�ŐV��)��Y�t�̂���";
        } else if (G_KS_Model == "�X�O" && s_Text5 == "TCG" ) {
            //WX-90 TCG
            s_Text = "�n�ڃv���[�g��ƍH���t���[EX2005-046(�ŐV��)�o�}�v";
            s_Text2 = "�n�ڌ����v�̏�EX2020-056(�ŐV��)��Y�t�̂���";
        } else {
            //WX
            s_Text = "�n�ڃv���[�g��ƍH���t���[EX2005-046(�ŐV��)�o�}�v";
            s_Text2 = "";
        }

        Excel_Cell_DataSet(9, 16, s_Text);
        G_Log_Text = "�v�w���L�P  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);

        Excel_Cell_DataSet(10, 16, s_Text2);
        G_Log_Text = "�v�w���L�Q  �w" + s_Text2 + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);

    }
    // 2021.05.17 ���L�ǉ�_E

    // 2021.10.19 ���L�ǉ�_S
    //*************************************
    //***  �_�C�g�N���L(���O(11.16))
    //*************************************
    //--- �䒍����R�[�h
    i_RecNo = 1064;
    if (s_HEADS_DATA[i_RecNo].TrimRight() == "04633"){
        // �_�C�g�N�����h����h��w��
        s_Text = "���j�h���͎�h��̂��ƁBTS2021-041�ɂ��";
        Excel_Cell_DataSet(11, 16, s_Text);
        G_Log_Text = "�_�C�g�N�����h����h��w��     �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2021.10.19 ���L�ǉ�_E

    // 2022.01.26 ��߰����L�ǉ�_E
    //****************************************************
    //***  �X�y�[�T���L(���O(12.16)�A(13.16) �A(14.16))
    //****************************************************
    s_Text = "";
    s_Text2 = "";
    s_Text3 = "";
    s_Text4 = "";
    s_Text5 = "";

    i_RecNo = 835;   // B���޽��čގ�
    s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
    i_RecNo = 836;   // A���޽��čގ�
    s_Text3 = s_HEADS_DATA[i_RecNo].TrimRight();
    s_S1Nzl_Syurui = s_HEADS_DATA[891].TrimRight(); // S1ɽ�َ��
    s_S2Nzl_Syurui = s_HEADS_DATA[906].TrimRight(); // S2ɽ�َ��
    s_S3Nzl_Syurui = s_HEADS_DATA[921].TrimRight(); // S3ɽ�َ��
    s_S4Nzl_Syurui = s_HEADS_DATA[936].TrimRight(); // S4ɽ�َ��
    s_E1Nzl_Syurui = s_HEADS_DATA[951].TrimRight(); // E1ɽ�َ��
    s_E2Nzl_Syurui = s_HEADS_DATA[966].TrimRight(); // E2ɽ�َ��
    s_E3Nzl_Syurui = s_HEADS_DATA[981].TrimRight(); // E3ɽ�َ��
    s_E4Nzl_Syurui = s_HEADS_DATA[996].TrimRight(); // E4ɽ�َ��

    if ( G_KS_Syurui == "�t�w" && G_KS_Model == "�X�O" ){
        // UX-90
        if ( s_Text2 == "2067" || s_Text3 == "2067" ) {
            // GZ-NBR
            s_Text4 = "�f�r�j�ގ������Y���m�a�q";
        }
       s_Text5 = "�ʐ}�o�}(E366044-1)�v�B�g���}(���i�A�d�ʁA���t���@)�͑Ή���";
    }
    else if ( G_KS_Syurui == "�k�w" && G_KS_Model == "�P�O" ){
         // LX-10
         if ( s_Text2 == "2063" || s_Text3 == "2063" ||
              s_Text2 == "2064" || s_Text3 == "2064" ) {
            // TCG
            s_Text4 = "�f�r�j�ގ����s�b�f";
        }
        s_Text5 = "�ʐ}�o�}(E348550-1)�v�B�g���}(���i�A�d�ʁA���t���@)�͑Ή���";
    }
    else if ( G_KS_Syurui == "�v�g" && G_KS_Model == "�R�O" ){
        // WH-30
        s_Text4 = "�^�����v�g�|�R�O";
        s_Text5 = "�ʐ}�o�}(E383984-2)�v�B�g���}(���i�A�d�ʁA���t���@)�͑Ή���";
    }

    if(s_Text4 != ""){

        s_Text = s_Text4 + "�̂��߁A�E�J���̃t���[���ƃv���[�g�̊ԂɃX�y�[�T�v�B";

        Excel_Cell_DataSet(12, 16, s_Text);
        G_Log_Text = "�X�y�[�T���L�P  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);

        Excel_Cell_DataSet(13, 16, s_Text5);
        G_Log_Text = "�X�y�[�T���L�Q  �w" + s_Text5 + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);

        // 2022.04.12 ���E����C��_S
        //if( s_S1Nzl_Syurui == "0" || s_S2Nzl_Syurui == "0" || s_S3Nzl_Syurui == "0" || s_S4Nzl_Syurui == "0" ) {
        //    // S�����E
        //    s_Text = s_Text4 + "�����E�d�l�̂��߁A�X�y�[�T���E�Ή��v�B";
        //}
        //else if ( s_E1Nzl_Syurui == "0" && s_E2Nzl_Syurui == "0" && s_E3Nzl_Syurui == "0" && s_E4Nzl_Syurui == "0" ) {
        //    // E���E��
        //    s_Text = "";
        //}
        //else if ( s_E1Nzl_Syurui != "0" && s_E2Nzl_Syurui != "0" && s_E3Nzl_Syurui != "0" && s_E4Nzl_Syurui != "0" ) {
        //    // E���l�E
        //    s_Text = "";
        //}
        //else {
        //    // E�����E
        //    s_Text = s_Text4 + "�����E�d�l�̂��߁A�X�y�[�T���E�Ή��v�B";
        //}

        bTok_S = true;
        bTok_E = true;
        // S��
        if( s_S1Nzl_Syurui != "0" && s_S2Nzl_Syurui != "0" && s_S3Nzl_Syurui != "0" && s_S4Nzl_Syurui != "0" ) {
            // S���l�E
            bTok_S = false;
        }
        else if ( s_S1Nzl_Syurui == "0" && s_S2Nzl_Syurui == "0" && s_S3Nzl_Syurui == "0" && s_S4Nzl_Syurui == "0" ) {
            // S���E��
            bTok_S = false;
        }
        else {
            // S�����E
            bTok_S = true;
        }

        // E��
        if ( s_E1Nzl_Syurui != "0" && s_E2Nzl_Syurui != "0" && s_E3Nzl_Syurui != "0" && s_E4Nzl_Syurui != "0" ) {
            // E���l�E
            bTok_E = false;
        }
         else if ( s_E1Nzl_Syurui == "0" && s_E2Nzl_Syurui == "0" && s_E3Nzl_Syurui == "0" && s_E4Nzl_Syurui == "0" ) {
            // E���E��
            bTok_E = false;
        }
        else {
            // E�����E
            bTok_E = true;
        }

        if ( bTok_S == true || bTok_E == true) {
            // ���E�L
            s_Text = s_Text4 + "�����E�d�l�̂��߁A�X�y�[�T���E�Ή��v�B";
        }
        else {
            // ���E��
            s_Text = "";
        }
        // 2022.04.12 ���E����C��_E

        if(s_Text != ""){
            Excel_Cell_DataSet(14, 16, s_Text);
            G_Log_Text = "�X�y�[�T���L�R  �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);
        }

    }
    // 2022.01.26 ��߰����L�ǉ�_E

    // 2022.01.26 ���L�ǉ�_S
    //*********************************
    //***  ���V���ƒ��L(���O(15.16))
    //*********************************
    //--- �䒍����R�[�h
    i_RecNo = 1064;
    if ( s_HEADS_DATA[i_RecNo].TrimRight() == "06346" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "06448" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "07194" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "10374" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "10383" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "11261" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "12063" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "12403" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "12404" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "18033" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "18253" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "18254" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "18255" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "19823" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "20676" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "24120" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "27410" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "28934" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "43573" ) {
        // ���V���ƒ���
        s_Text = "TS2019-146�Ɠ��d�l�i�������A�ቷ���̉^�]���͂������j�łȂ����Ƃ��m�F���邱�ƁB���d�l�̏ꍇ�A�������ߑΉ��̂��ƁB";
        Excel_Cell_DataSet(15, 16, s_Text);
        G_Log_Text = "���V���ƒ��L  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }

    //*********************************
    //***  F3�ڒ��ܒ��L(���O(16.16))
    //*********************************
    s_Text = "";
    s_Text2 = "";
    s_Text3 = "";

    i_RecNo = 835;   // B���޽��čގ�
    s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
    i_RecNo = 836;   // A���޽��čގ�
    s_Text3 = s_HEADS_DATA[i_RecNo].TrimRight();

    if ( s_Text2 == "2003" || s_Text3 == "2003" ||
         s_Text2 == "2005" || s_Text3 == "2005" ||
         s_Text2 == "2011" || s_Text3 == "2011" ||
         s_Text2 == "2021" || s_Text3 == "2021" ||
         s_Text2 == "2038" || s_Text3 == "2038" ||
         s_Text2 == "2041" || s_Text3 == "2041" ||
         s_Text2 == "2045" || s_Text3 == "2045" ||
         s_Text2 == "2079" || s_Text3 == "2079" ||
         s_Text2 == "2083" || s_Text3 == "2083" ||
         s_Text2 == "2098" || s_Text3 == "2098" ) {
        // �H�i�K�X�P�b�g�@�@�@
        s_Text = "���ӁF�e�R�K�X�P�b�g�̓A���R�[���֗n�����܂��B";
        Excel_Cell_DataSet(16, 16, s_Text);
        G_Log_Text = "F3�ڒ��ܒ��L  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2022.01.26 ���L�ǉ�_E

    // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_S
    //*************************************
    //***  �@��ԍ����O���L(���O(17.16))
    //*************************************
    s_Text = "";
    if (Tokki_Code("39", s_HEADS_DATA, 1500) == true) {
        // �@��ԍ����L���ڗL
        i_RecNo = 27;
        int w_count = s_Text.AnsiPos("�`");

        if (w_count){
            s_Text = "���@��ԍ�  " + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "�`" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
        } else {
            s_Text = "���@��ԍ�  " + ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
        }

        Excel_Cell_DataSet(17, 16, s_Text);
        G_Log_Text = "�@��ԍ����O���L  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_E

    // 2022.04.11 �A�q�D���ǉ�_S
    //*************************************
    //***  �A�q�D�����O���L(���O(18.16))
    //*************************************
    s_Text = "";
    if (Tokki_Code("29", s_HEADS_DATA, 1500) == true) {
        // SHIP NAME���L�L���ڗL
        s_Text = Tokki_Zen("29", s_HEADS_DATA, 1500);
        s_Text = s_Text.SubString(10, s_Text.Length() - 9);
        s_Text = "�A�q�D��:" + s_Text;

        Excel_Cell_DataSet(18, 16, s_Text);
        G_Log_Text = "�A�q�D�����O���L  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2022.04.11 �A�q�D���ǉ��ǉ�_E

    // 2022.04.11 �ψ��ێ����Ԏw��ǉ�_S
    //**************************************
    //***  �ψ��ێ����Ԓ��L(���O(19.16))
    //**************************************
    s_Text = "";
    if (Tokki_Code("2C", s_HEADS_DATA, 1500) == true) {
        // �ψ��ێ����ԕW��
        s_Text = "�ψ��ێ����ԕW��";
    }
    else if (Tokki_Code("2D", s_HEADS_DATA, 1500) == true) {
        // �ψ��ێ�����30��
        s_Text = "�ψ��ێ�����30��";
    }
    else if (Tokki_Code("2E", s_HEADS_DATA, 1500) == true) {
        // �ψ��ێ����Ԏw�肠��
        s_Text = "�ψ��ێ����Ԏw�肠��";
    }

    if ( s_Text != "" ) {
        Excel_Cell_DataSet(19, 16, s_Text);
        G_Log_Text = "�ψ��ێ����Ԓ��L  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2022.04.11 �ψ��ێ����Ԏw��ǉ�_E

    // 2023.09.26 RX-30�^Eɽ�ٕt�x��_S
    //**************************************
    //***  RX-30�^Eɽ�ٕt�x��(���O(20.16))
    //**************************************
    s_Text = "";

    if ( G_KS_Syurui == "�q�w" && G_KS_Model == "�R�O" ){
        // RX-30
        if ( s_E1Nzl_Syurui != "0" || s_E2Nzl_Syurui != "0" || s_E3Nzl_Syurui != "0" || s_E4Nzl_Syurui != "0" ) {
            // E��ɽ�ٕt
            s_Text = "�W���d�t���[���g�p�s��";
        }
    }

    if ( s_Text != "" ) {
        Excel_Cell_DataSet(20, 16, s_Text);
        G_Log_Text = "RX-30�^Eɽ�ٕt�x��  �w" + s_Text + "�x���Z�b�g�B";
        Write_Log(G_Log_Text);
    }
    // 2023.09.26 RX-30�^Eɽ�ٕt�x��_E

    // 2018 09.18 �o�[�R�[�h�폜_S
    //// 2017.12.01 �o�[�R�[�h�ǉ�_S
    ////***************************
    ////***  �o�[�R�[�h
    ////***************************
    //s_Text = G_SAKUBAN;
    //s_Text = "*" + s_Text + "*";
    //Excel_Cell_DataSet(62, 10, s_Text);
    //G_Log_Text = "�o�[�R�[�h           �w" + s_Text + "�x���Z�b�g�B";
    //Write_Log(G_Log_Text);
    //// 2017.12.01 �o�[�R�[�h�ǉ�_E
    //// 2018 09.18 �o�[�R�[�h�폜_E
    // 2023.05.30 �o�[�R�[�h�ύX_S
    s_Text = G_SAKUBAN;
    exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�o�[�R�[�h"));
    Excel_Cell_DataSet(4, 5, s_Text);
    G_Log_Text = "�o�[�R�[�h���         �w" + s_Text + "�x���Z�b�g�B";
    Write_Log(G_Log_Text);
    exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�}�ʖژ^"));
    // 2023.05.30 �o�[�R�[�h�ύX_E

   //******************************
   //***  Excel Book Close
   //******************************
   // �}�N�����s
   //PDF�t�@�C���o�͎��
   //exApplication.Exec(Function("Run") << "Gsub_PDFOut");

   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   //Windows 2008Server �ڐA�Ή�
   exApplication.OlePropertySet("DisplayAlerts", false);

   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ۸ލ쐬
   G_Log_Text = "------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w�}�ʖژ^�x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F �o�͐}�ʁ@�Z�b�g
// �T  �v      �F
// ��  ��      �F Pattern [�o�̓p�^�[��]
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
void __fastcall SyuturyokuZumen_Set_S(AnsiString s_UtiKbn, bool bMarine, HeadsData* pcHeadsData)
{
   AnsiString s_Text;
   int        i_RowCnt;

   bool bRet;
   AnsiString s_HEADS_DATA[1500];
   int i;
   int iLoop;
   AnsiString sMirrorCode;

   // HEADS�ް���۰�ٕϐ��ɾ��
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return;

   //*******************
   //***  �}�ʗ�������
   //*******************
   for (i_RowCnt = 0; i_RowCnt < 32; i_RowCnt++){
      s_Text = "";
      Excel_Cell_DataSet(10 + i_RowCnt, 2, s_Text);
      Excel_Cell_DataSet(10 + i_RowCnt, 5, s_Text);
      Excel_Cell_DataSet(10 + i_RowCnt, 7, s_Text);
      Excel_Cell_DataSet(10 + i_RowCnt, 8, s_Text);
   }

   i_RowCnt = 0;

   if(!pcHeadsData->GetMirrorFlag()){
        iLoop = 1;  // ���d�l
   }else{
        iLoop = 2;  // �װA�A�װB
   }

   for(i=0; i<iLoop; i++){

        if(pcHeadsData->GetMirrorFlag()){
            // �װ�d�l�ł���
            if(i==0){
                sMirrorCode = "A";
            }else{
                sMirrorCode = "B";
            }
        }else{
            // �װ�d�l�łȂ�
            sMirrorCode = "";
        }

        //2019/05/17 ���p�Z�p�ǉ�
        if (!bIsNotKatashiki) {
            //***************************
            //***  �G�������g�\���}
            //***************************
            s_Text = "�G�������g�\���}";
            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

			//2023/4/21 �����ԍ������ǉ��Ή� Mod
			//s_Text = "A" + G_SAKUBAN + sMirrorCode;
			s_Text = "A" + GetFileSeizouBangou(G_SAKUBAN) + sMirrorCode;
            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

            i_RowCnt = i_RowCnt + 2;

			//*******************
            //***  �g���}
            //*******************
            s_Text = "�g���}";
            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

			//2023/4/21 �����ԍ������ǉ��Ή� Mod
			//s_Text = "B" + G_SAKUBAN + sMirrorCode;
			s_Text = "B" + GetFileSeizouBangou(G_SAKUBAN) + sMirrorCode;
            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

            i_RowCnt = i_RowCnt + 2;
        }
   }

   //*******************
   //***  �W�����i�\
   //*******************
   s_Text = "�W�����i�\";
   Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_Text = "C" + G_SAKUBAN;
   s_Text = "C" + GetFileSeizouBangou(G_SAKUBAN);
   Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

   i_RowCnt = i_RowCnt + 2;

   //*******************
   //***  �v�ڕ\
   //*******************
   if( bMarine == true ) {
      //2019/05/17 ���p�Z�p�ǉ�
      if (!bIsNotKatashiki) {
          s_Text = "�v�ڕ\";
          Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

		  //2023/4/21 �����ԍ������ǉ��Ή� Mod
		  //s_Text = "M" + G_SAKUBAN;
		  s_Text = "M" + GetFileSeizouBangou(G_SAKUBAN);
          Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

          i_RowCnt = i_RowCnt + 2;
      }
   }

   if (s_HEADS_DATA[327].TrimRight() != ""){        // �����O���

        for(i=0; i<iLoop; i++){

            if(pcHeadsData->GetMirrorFlag()){
                // �װ�d�l�ł���
                if(i==0){
                    sMirrorCode = "A";
                }else{
                    sMirrorCode = "B";
                }
            }else{
                // �װ�d�l�łȂ�
                sMirrorCode = "";
            }

            //2019/05/17 ���p�Z�p�ǉ�
            if (!bIsNotKatashiki) {
                //***************************
                //***  ���G�������g�\���}
                //***************************
                s_Text = "���G�������g�\���}";
                Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

                s_Text = "A" + s_HEADS_DATA[327].TrimRight() + sMirrorCode;
                Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

                i_RowCnt = i_RowCnt + 2;

                //*******************
                //***  ���g���}
                //*******************
                s_Text = "���g���}";
                Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

                s_Text = "B" + s_HEADS_DATA[327].TrimRight() + sMirrorCode;
                Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

                i_RowCnt = i_RowCnt + 2;
            }
        }
    }

}

//---------------------------------------------------------------------------
// ���{��֐����F ���p��
// �T  �v      �F
// ��  ��      �F ����
// �߂�l      �F ���p�󔒕�����
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
AnsiString __fastcall spc(int iNum)
{
   AnsiString retSpc;
   int ilp;

   retSpc="";
   for(ilp=0; ilp<iNum; ilp++) retSpc += "�@";

   return retSpc;

}

//---------------------------------------------------------------------------
// ���{��֐����F �~���[A�d�l�A�~���[B�d�l�̋@�햼�̂��擾����
// �T  �v      �F
// ��  ��      �F s_NameOrg AnsiString(I)   ���d�l�̋@�햼��
//                s_NameA   AnsiString(O)   �~���[A�d�l�̋@�햼��
//                s_NameB   AnsiString(O)   �~���[B�d�l�̋@�햼��
// �߂�l      �F �Ȃ�
//
// ��  �l      �F P_CV00.exe�̕���������쐬�����Ɠ�������邱��
//                (Module1.bas ��Fnc_Write_SIYO �֐�)
//
// ���@��      �F2007.11.15 �V�K�쐬
//---------------------------------------------------------------------------
AnsiString __fastcall GetEquipmentName(AnsiString s_NameOrg, AnsiString sSpecCode)
{
   AnsiString s_Text6L; // 1�`6������
   AnsiString s_Text6R; // 6�����ڈȍ~
   AnsiString s_Text9L; // 1�`9������
   AnsiString s_Text9R; // 9�����ڈȍ~
   AnsiString s_NoHan;     // No
   AnsiString s_NoZen;     // No
   AnsiString s_RetText;

   // ���d�l�̋@�햼�̂̕���������
   s_Text6L = LeftStr(s_NameOrg, 6);
   s_Text6R = s_NameOrg.SubString(s_Text6L.Length() + 1, s_NameOrg.Length() - s_Text6L.Length());
   s_Text9L = LeftStr(s_NameOrg, 9);
   s_Text9R = s_NameOrg.SubString(s_Text9L.Length() + 1, s_NameOrg.Length() - s_Text9L.Length());

   if( sSpecCode == "A"){
      s_NoHan = "1";
      s_NoZen = "�P";
   }else if( sSpecCode == "B" ){
      s_NoHan = "2";
      s_NoZen = "�Q";
   }

   // ���d�l�̋@�햼�̂���A�~���[A�d�l�A�~���[B�d�l�̋@�햼�̂��쐬
   if( s_NameOrg.Trim().Length() == 0 ){    // ��
      s_RetText = "";

   } else if( s_Text6L == "No.1/2" ){
      s_RetText = "No." + s_NoHan;
      s_RetText = s_RetText + s_Text6R;

   }else if( s_Text6L == "NO.1/2" ){
      s_RetText = "NO." + s_NoHan + s_Text6R;

   }else if( s_Text9L == "No.1/No.2" ){
      s_RetText = "No." + s_NoHan + s_Text9R;

   }else if( s_Text9L == "NO.1/NO.2" ){
      s_RetText = "NO." + s_NoHan + s_Text9R;

   }else if( s_Text9L == "NO.1/No.2" ){
      s_RetText = "NO." + s_NoHan + s_Text9R;

   }else if( s_Text9L == "No.1/NO.2" ){
      s_RetText = "No." + s_NoHan + s_Text9R;

   }else if( s_Text6L == "�m���D�P�^�Q" ){
      s_RetText = "�m���D" + s_NoZen + s_Text6R;

   }else if( s_Text6L == "�m�n�D�P�^�Q" ){
      s_RetText = "�m�n�D" + s_NoZen + s_Text6R;

   }else if( s_Text9L == "�m���D�P�^�m���D�Q" ){
      s_RetText = "�m���D" + s_NoZen + s_Text9R;

   }else if( s_Text9L == "�m�n�D�P�^�m�n�D�Q" ){
      s_RetText = "�m�n�D" + s_NoZen + s_Text9R;

   }else if( s_Text9L == "�m�n�D�P�^�m���D�Q" ){
      s_RetText = "�m�n�D" + s_NoZen + s_Text9R;

   }else if( s_Text9L == "�m���D�P�^�m�n�D�Q" ){
      s_RetText = "�m���D" + s_NoZen + s_Text9R;

   }else{
      s_RetText = s_NameOrg;
   }

   return(s_RetText);
}

//---------------------------------------------------------------------------
// ���{��֐����F �o�͐}�ʁ@�Z�b�g
// �T  �v      �F
// ��  ��      �F Pattern [�o�̓p�^�[��]
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
//void __fastcall SyuturyokuZumen_Set_S_OLD(char Pattern, AnsiString Lang)
//{
//   AnsiString s_Text;
//   int        i_RowCnt;
//
//   //*******************
//   //***  �}�ʗ�������
//   //*******************
//   for (i_RowCnt = 0; i_RowCnt < 32; i_RowCnt++){
//      s_Text = "";
//      Excel_Cell_DataSet(10 + i_RowCnt, 2, s_Text);
//      Excel_Cell_DataSet(10 + i_RowCnt, 5, s_Text);
//      Excel_Cell_DataSet(10 + i_RowCnt, 7, s_Text);
//      Excel_Cell_DataSet(10 + i_RowCnt, 8, s_Text);
//   }
//
//   i_RowCnt = 0;
//
//   //*******************
//   //***  �v�ڕ\
//   //*******************
//   switch (Pattern) {
//      case 'C': case 'F': case 'G': case 'K': case 'L': case 'M':
//         if (Lang == "J"){
//            s_Text = "�v�ڕ\";
//         } else {
//            s_Text = "PRINCIPAL ITEMS";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "M" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  �G�������g�\���}
//   //***************************
//   switch (Pattern) {
//      case 'H':  case 'I': case 'J': case 'N': case 'O': case 'P':
//      case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
//         if (Lang == "J"){
//            s_Text = "�G�������g�\���}";
//         } else {
//            s_Text = "DWG. OF ELEMENT ARRANGEMENT";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "A" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //*******************
//   //***  �g���}
//   //*******************
//   switch (Pattern) {
//      case 'A': case 'U':
//         break;
//
//      default :
//         if (Lang == "J"){
//            s_Text = "�g���}";
//         } else {
//            s_Text = "ASSEMBLY DRAWING";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "B" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//   }
//
//   //*******************
//   //***  �W�����i�\
//   //*******************
//   switch (Pattern) {
//      case 'A': case 'U':
//         break;
//
//      default :
//         if (Lang == "J"){
//            s_Text = "�W�����i�\";
//         } else {
//            s_Text = "ASSEMBLY DRAWING";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "B" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//   }
//
//   //***************************
//   //***  ���X�g
//   //***************************
//   switch (Pattern) {
//      case 'C': case 'F': case 'G': case 'K': case 'L': case 'M': case 'Q': case 'R': case 'S': case 'T':
//         if (Lang == "J"){
//            s_Text = "�p�[�c���X�g";
//         } else {
//            s_Text = "PARTS LIST";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "PAR" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//
//         if (Lang == "J"){
//            s_Text = "�t���i���X�g";
//         } else {
//            s_Text = "ACCESSORIES LIST";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "ACT" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//
//         if (Lang == "J"){
//            s_Text = "�H��X�g";
//         } else {
//            s_Text = "TOOL LIST";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "TOL" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  ���͗e��i�t�j
//   //***************************
//   switch (Pattern) {
//      case 'D': case 'F': case 'H': case 'K': case 'N': case 'Q': case 'S':
//         if (Lang == "J"){
//            s_Text = "���x�v�Z��";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            s_Text = "PVC" + G_SAKUBAN;
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//
//            s_Text = "�t���[�V�[�g";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            s_Text = "PVF" + G_SAKUBAN;
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//         }
//
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  ���͗e��i�X�`�[���j
//   //***************************
//   switch (Pattern) {
//      case 'E': case 'G': case 'I': case 'L': case 'O': case 'R': case 'T':
//         if (Lang == "J"){
//            s_Text = "���x�v�Z��";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            s_Text = "PVC" + G_SAKUBAN;
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//
//            s_Text = "�t���[�V�[�g";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            s_Text = "PVF" + G_SAKUBAN;
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//
//            s_Text = "�X�`�[�������ʌv�Z��";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            //s_Text = "PVC" + G_SAKUBAN;
//            s_Text = "";
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//         }
//
//         break;
//
//      default :
//         break;
//   }
//
//   //*******************
//   //***  ���v�ڕ\
//   //*******************
//   switch (Pattern) {
//      case 'K': case 'L': case 'M':
//         if (Lang == "J"){
//            s_Text = "���v�ڕ\";
//         } else {
//            s_Text = "OLD PRINCIPAL ITEMS";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "M" + s_HEADS_DATA[327].TrimRight();
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //*******************
//   //***  ���g���}
//   //*******************
//   switch (Pattern) {
//      case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'S': case 'T':
//         if (Lang == "J"){
//            s_Text = "���g���}";
//         } else {
//            s_Text = "OLD ASSEMBLY DRAWING";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "B" + s_HEADS_DATA[327].TrimRight();
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  ���G�������g�\���}
//   //***************************
//   switch (Pattern) {
//      case 'N': case 'O': case 'P': case 'S': case 'T':
//         if (Lang == "J"){
//            s_Text = "���G�������g�\���}";
//         } else {
//            s_Text = "OLD DWG. OF ELEMENT ARRANGEMENT";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "A" + s_HEADS_DATA[327].TrimRight();
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  �����v�̏�
//   //***************************
//   switch (Pattern) {
//      case 'V':
//         if (Lang == "J"){
//            s_Text = "�����v�̏�";
//         } else {
//            s_Text = "INSPECTION PROCEDURE";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "D" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         //i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//}

//---------------------------------------------------------------------------
// ���{��֐����F ���L�R�[�h�L���m�F
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F ���L����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2020.07.17�ǉ�
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
