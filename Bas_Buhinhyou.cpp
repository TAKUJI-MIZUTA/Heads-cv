//---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�W�����i�\(Buhinhyou)  �쐬���W���[��
//�@ �� �� �ҁFT.Kawai
//�@ �� �� ���F2002.08.07
//�@ �X �V ���F2002.09.27
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <dir.h>
#include <io.h>
#include <direct.h>

#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_Buhinhyou.h"

#include "HeadsData.h"

#define  EXCEL_SHEETNAME    "�W�����i�\"          // EXCEL�V�[�g��(�W�����i�\)

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ��ڰĕ��i�W�J���� Class
//---------------------------------------------------------------------------
class TBuhinhyou
{
private:
   FILE *wkfp;
//   AnsiString P_PREV_PARTS[500][16];
//   AnsiString P_WK_PARTS[500][15];
//   AnsiString P_WK_PARTS3[500][15];
   AnsiString P_PREV_PARTS[1000][18];
   AnsiString P_WK_PARTS[1000][17];
   AnsiString P_WK_PARTS3[1000][17];

   int P_PrevRec;   // �O���ް���

   int P_compflg;   // ��r�׸�(�O��̧�ّ��݂�1)
   int P_WkRec;     // �W�v�pܰ��ް��i�[ں��ސ�
   int P_WkRec3;    // �W�v�pܰ��ް��i�[ں��ސ�
   int P_Row;       // �ް���čs��
   int P_PageNum;   // �y�[�W���i�V�`�R�����C�A�E�g�p�j
   int iBLTCV;
   //2003.11.14 �ϐ��ǉ�
   int P_SHIP_Num;  // ���p���i�s��
   //*******************
   //2019.10.23 �t���i�ް��o�͍s���ύX_S
   int P_ACCS_Num;  // �t���i���i�s��
   //2019.10.23 �t���i�ް��o�͍s���ύX_E

   HeadsData* m_pcHeadsData;
   AnsiString P_HEADS_DATA[1500];
   AnsiString P_HEADS_DATA_A[1500];
   AnsiString P_HEADS_DATA_B[1500];

   bool __fastcall Buhinhyou_Data_Sakusei(void);
   bool __fastcall Buhinhyou(void);              //
   //2003.11.14 �֐��ǉ�
   bool __fastcall Read_Buhin_Data(void);        // ���p���i�s���Ǎ�
   //*******************

   //2019.10.23 �t���i�ް��o�͍s���ύX_S
   bool __fastcall Read_ACCS_Data(void);         // �t���i�s���Ǎ�
   //2019.10.23 �t���i�ް��o�͍s���ύX_E

   bool __fastcall Write_Buhin_Detail(void);     //
   bool __fastcall Compare_Data( AnsiString* );

   bool __fastcall Prev_Data_Get(void);
   //2023.02.08 �����ԍ������ǉ�_S
   bool __fastcall Prev_Old_Data_Get(void);
   //2023.02.08 �����ԍ������ǉ�_E
   void __fastcall Output_WK_PARTS(bool);
   void __fastcall Output_WK_PARTS3(void);
   void __fastcall Buhin_Syukei( AnsiString* );
   void __fastcall Buhin_Syukei2( AnsiString* );
   void __fastcall Buhin_Syukei3( AnsiString* );
   //2003.11.14 �֐��ǉ�
   void __fastcall Buhin_Syukei4( AnsiString* ); // �m�Y���p
   //*******************
   void __fastcall Cyuki_Out( void );
   void __fastcall BLT_Cover_Out( void );
   void __fastcall BLT_Cover_Out_Small( void );
   AnsiString __fastcall Csv_Exist_Check(AnsiString);
   bool __fastcall Compare_PrevData( AnsiString *P_PARTS );
   bool __fastcall Compare_PrevData2( AnsiString *P_PARTS );
   void __fastcall Write_BorderLine(void);

   // 2020.07.17 �ی�J�o�[���L�ǉ�_S
   bool __fastcall Tokki_Code(AnsiString);
   // 2020.07.17 �ی�J�o�[���L�ǉ�_E

   //--------------------------------
   // 2007.10.12 �װ�d�l�Ή� �֐��ǉ�
   bool __fastcall Read_Buhin_Data_Sorted(AnsiString filepass, AnsiString s_DATA[][17]);
   bool __fastcall Sort_HEADS_Data(AnsiString s_DATA[][17], AnsiString s_SortDATA[][17]);
   // 2007.11.08
   bool __fastcall TBuhinhyou::Sort_HEADS_PlData(AnsiString s_wkTmpPlData[][18], AnsiString s_wkTmpHlData[][18], AnsiString s_SortPlDATA[][17]);
   // 2007.11.08
   // 2007.10.12
   //--------------------------------

   //--------------------------------
   // 2008.03.19 �װ�d�l�Ή� �֐��ǉ�
   AnsiString __fastcall TBuhinhyou::ConvertRyutaiName(AnsiString s_SideCd, bool bMirror);
   // 2008.03.19
   //--------------------------------

public:
   bool __fastcall Excute(HeadsData* pcHeadsData, HeadsData* apcHeadsDataAll);    //  ���͌v���i�W�J���� ���s

};
//---------------------------------------------------------------------------
// ���{��֐����F ���͌v���i�W�J����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F
//---------------------------------------------------------------------------
bool __fastcall Buhinhyou(HeadsData* pcHeadsData, HeadsData* apcHeadsDataAll){
   bool        ret;

   TBuhinhyou  buhinhyou;    // ���͌v�ް� �쐬Class��������,�֐��I����j��

   ret = buhinhyou.Excute(pcHeadsData, apcHeadsDataAll); // ���͌v�ް� �쐬���s

   return ret;
}

//---------------------------------------------------------------------------
// ���{��֐����F �W�����i�\�W�J���� �쐬���s
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Excute(HeadsData* pcHeadsData, HeadsData* apcHeadsDataAll)
{
   // 2007.10.11 �װ�d�l�Ή�
   // ���d�l�ȊO�͏����s�v(����I��)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);

   // HeadsData�׽�ϐ�
   m_pcHeadsData = pcHeadsData;

   // HEADS�ް���ϐ��ɾ�āA�װ�d�l�̏ꍇ�A�װ���ް��ྯ�
   if(!apcHeadsDataAll[0].SetAnsiStringData(P_HEADS_DATA)) return(false);
   if(apcHeadsDataAll[0].GetMirrorFlag()){
      // �װA�d�l�A�װB�d�l���ް�
      if(!apcHeadsDataAll[1].SetAnsiStringData(P_HEADS_DATA_A)) return(false);
      if(!apcHeadsDataAll[2].SetAnsiStringData(P_HEADS_DATA_B)) return(false);
   }

   // ۸ލ쐬
   Write_Log("--------------------------------------------------------");
   Write_Log("----------      �w�W�����i�\�쐬�����x�J�n      ----------");
   Write_Log("--------------------------------------------------------");
   Write_Error_Log("--------------------------------------------------------");
   Write_Error_Log("----------      �w�W�����i�\�쐬�����x�J�n      ----------");
   Write_Error_Log("--------------------------------------------------------");

   //***********************************
   //***  ���͌v�ް��쐬
   //***********************************
   Buhinhyou_Data_Sakusei();

   // ۸ލ쐬
   Write_Log("--------------------------------------------------------");
   Write_Log("----------      �w�W�����i�\�쐬�����x�I��      ----------");
   Write_Log("--------------------------------------------------------");
   Write_Error_Log("--------------------------------------------------------");
   Write_Error_Log("----------      �w�W�����i�\�쐬�����x�I��      ----------");
   Write_Error_Log("--------------------------------------------------------");

   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F �W�����i�\(Buhinhyou)�@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Buhinhyou_Data_Sakusei(void)
{
   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�
   AnsiString s_SQL;             // SQL��
   AnsiString s_Text;            // �ėp������
   AnsiString s_Bunbo;
   AnsiString s_Bunsi;
   AnsiString d_RevNo;           // ����w�����̂��钼�߂�RevNo

   int i_RecNo;           // HEADS�ް� ں���No
   int i_RevNo;           // HEADS�ް� ں���No
   //int i_ErrFlg;
   //int i_Value_1;
   //int i_Value_2;

   //double d_wkValue;

//   s_Tmplate_Pass = G_Template_Dir + "�W�����i�\.xls";             // ����ڰ�̧��
   s_Tmplate_Pass = G_Template_Dir + "�W�����i�\A3L.xls";             // ����ڰ�̧��

   //2019/05/17 ���p�Z�p�ǉ�
   bHasOldDataFlag = false;
   if (bIsNotKatashiki) {
       AnsiString ToFolder, ToFolder1, Rev;
       AnsiString Work, TooFolder, TooFolder1;
       int Sakuban, Number;
       AnsiString sRev, sRev1;
       AnsiString sFirstChar;
       int i_PosIdx;
       AnsiString sNumberList;
       AnsiString sFileName;

       // �ړ���t�H���_�̍쐬
      //2022/12/15 �����ǉ��Ή� Mod_S
      //Work = G_SAKUBAN.SubString(1, 4);
      if (G_SAKUBAN.Length() > 7)
      {
    	  Work = G_SAKUBAN.SubString(1, 6);
      }
      else
      {
    	  Work = G_SAKUBAN.SubString(1, 4);
      }
      //2022/12/15 �����ǉ��Ή� Mod_E       
       if( gP_Sakuban_Mode == "S" )
       {
          // ��̔��f
          Sakuban = Work.ToIntDef(0);
		  if( Sakuban % 2 == 1 ){
             Sakuban --;
          }
       	  //2022/12/15 �����ǉ��Ή� Mod_S
    	  //sprintf(Work.c_str(),"%04d",Sakuban);
	     if (Sakuban > 9999)
         {
			 Work.sprintf("%06d",Sakuban);
		 }
		 else
		 {
			 Work.sprintf("%04d",Sakuban);
	     }
	     //2022/12/15 �����ǉ��Ή� Mod_E
          ToFolder1 = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
       } else {
           ToFolder1 = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
       }
       bool bHasData = false;
       AnsiString sAB = "A";
       if (bMirrorFlag) {
           sAB = "B";
           bMirrorFlag = false;
       }
       sRev.sprintf("R%02d", 0 );
       sRev1.sprintf("%02d", 0 );
       sFirstChar = G_SAKUBAN.SubString(1, 1);
       sNumberList = "0123456789";
       i_PosIdx = sNumberList.Pos(sFirstChar);
       if (i_PosIdx > 0) {
           if (G_HEADS_DATA_Rev > 0) {
               sRev.sprintf("R%02d", G_HEADS_DATA_Rev);
               sRev1.sprintf("%02d", G_HEADS_DATA_Rev);
           }
       }

	   ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
             + IncludeTrailingBackslash(ToFolder1)
             + IncludeTrailingBackslash(Work)
             + IncludeTrailingBackslash(G_SAKUBAN);

	   //2023/4/21 �����ԍ������ǉ��Ή� Mod
	   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sAB + sRev + ".xls";
	   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sAB + sRev + ".xls";
       if (FileExists(sFileName)) {
           s_Tmplate_Pass = sFileName;
           bHasData = true;
           bHasOldDataFlag = true;
       } else {
		   //2023/4/21 �����ԍ������ǉ��Ή� Mod
		   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sRev + ".xls";
		   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sRev + ".xls";
           if (FileExists(sFileName)) {
			   s_Tmplate_Pass = sFileName;
               bHasData = true;
               bHasOldDataFlag = true;
           }
       }

       if (!bHasData) {
           ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
                 + IncludeTrailingBackslash(ToFolder1)
                 + IncludeTrailingBackslash(Work)
                 + IncludeTrailingBackslash(G_SAKUBAN)
                 + IncludeTrailingBackslash(sRev1);

		   //2023/4/21 �����ԍ������ǉ��Ή� Mod
		   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sAB + sRev + ".xls";
		   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sAB + sRev + ".xls";
           if (FileExists(sFileName)) {
               s_Tmplate_Pass = sFileName;
               bHasData = true;
               bHasOldDataFlag = true;
           } else {
			   //2023/4/21 �����ԍ������ǉ��Ή� Mod
			   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sRev + ".xls";
			   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sRev + ".xls";
               if (FileExists(sFileName)) {
                   s_Tmplate_Pass = sFileName;
                   bHasData = true;
                   bHasOldDataFlag = true;
               }
           }
       }

       if (!bHasData) {
           sRev.sprintf("R%02d", 0 );
           sRev1.sprintf("%02d", 0 );
           sFirstChar = G_SAKUBAN.SubString(1, 1);
           sNumberList = "0123456789";
           i_PosIdx = sNumberList.Pos(sFirstChar);
           if (i_PosIdx > 0) {
               if (G_HEADS_DATA_Rev > 1) {
                   sRev.sprintf("R%02d", G_HEADS_DATA_Rev-1);
                   sRev1.sprintf("%02d", G_HEADS_DATA_Rev-1);
               }
           }

           ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
                 + IncludeTrailingBackslash(ToFolder1)
                 + IncludeTrailingBackslash(Work)
                 + IncludeTrailingBackslash(G_SAKUBAN)
                 + IncludeTrailingBackslash(sRev1);

		   //2023/4/21 �����ԍ������ǉ��Ή� Mod
		   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sAB + sRev + ".xls";
		   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sAB + sRev + ".xls";
           if (FileExists(sFileName)) {
               s_Tmplate_Pass = sFileName;
               bHasOldDataFlag = true;
           } else {
			   //2023/4/21 �����ԍ������ǉ��Ή� Mod
			   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sRev + ".xls";
			   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sRev + ".xls";
               if (FileExists(sFileName)) {
                   s_Tmplate_Pass = sFileName;
                   bHasOldDataFlag = true;
               }
           }
       }
   }

   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "C" + G_SAKUBAN + ".xls"; // ��߰��̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "C" + GetFileSeizouBangou(G_SAKUBAN) + ".xls"; // ��߰��̧��

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w�W�����i�\�x�쐬�J�n    ----------";
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
   //***                       �W�����i�\�@�f�[�^�Z�b�g                      ***
   //***                                                                     ***
   //***************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_SHEETNAME);
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)EXCEL_SHEETNAME);

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                    �w�b�_�[���@�@�f�[�^�Z�b�g                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   // 2014.04.07 ͯ�ް�폜����
   //// 2012.11.07 ����ͯ�ް�폜
   //////----------------------------
   ////// 2007.11.15 �~���[�d�l�Ή�
   //////if ( P_HEADS_DATA[36].Pos("M") != 0 ) Excel_HeaderSet();
   ////if ( P_HEADS_DATA[36].Pos("M") != 0 ) Excel_HeaderSet(m_pcHeadsData->GetMirrorFlag());
   //if (P_HEADS_DATA[1107].TrimRight() == "CHINA"){
   //   // ����
   //// 2013.04.03 �����ǉ�
   //   // ��������
   //} else if (P_HEADS_DATA[1107].TrimRight() == "KYOGEN"){
   //// *******************�@�@
   //} else {
   //   // �����ȊO
   //   if ( P_HEADS_DATA[36].Pos("M") != 0 ) Excel_HeaderSet(m_pcHeadsData->GetMirrorFlag());                                                 // ���{��
   //}
   ////// 2007.11.15
   //////----------------------------
   //// ************************
   if ( P_HEADS_DATA[36].Pos("M") != 0 ) Excel_HeaderSet(m_pcHeadsData->GetMirrorFlag());
   // ************************

   //*********************
   //***  PHE�^��
   //*********************
   // �wA001�x
   i_RecNo = 33;
   if (P_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
   } else {
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(2, 2, s_Text);
   G_Log_Text = "�^��                  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //*** �S����
   //*********************
   // �wA002�x
   i_RecNo = 1079;
   s_Text = P_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(3, 23, s_Text);
   G_Log_Text = "�S����                RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ��}��
   //***************************
   // �wA003�x
   i_RecNo = 1087;
   s_Text = "'"+ HanToZen( P_HEADS_DATA[i_RecNo] );
   Excel_Cell_DataSet(3, 31, s_Text);
   G_Log_Text = "��}��                RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �}��
   //***************************
   // �wA004�x
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_Text = HanToZen( "C" + G_SAKUBAN );
   s_Text = HanToZen( "C" + GetFileSeizouBangou(G_SAKUBAN) );
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   Excel_Cell_DataSet(2, 61, s_Text);
   //2003.09.08 �}�ԏo�͒ǉ�
   Excel_Cell_DataSet(135, 61, s_Text);
   Excel_Cell_DataSet(204, 61, s_Text);
   Excel_Cell_DataSet(273, 61, s_Text);
   Excel_Cell_DataSet(342, 61, s_Text);
   //***********************
   G_Log_Text = "�}��                 �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  REV
   //***************************
   // �wA005�x
   i_RecNo = 319;
   i_RevNo = P_HEADS_DATA[i_RecNo].ToIntDef(0);
   d_RevNo = FormatFloat("0",i_RevNo);
   s_Text = "�q�d�u." + HanToZen( d_RevNo );
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   Excel_Cell_DataSet(2, 73, s_Text);
   //2003.09.08 �q�d�u�o�͒ǉ�
   //2003.09.09 �q�d�u�o�͒���
   //Excel_Cell_DataSet(135, 73, s_Text);
   //Excel_Cell_DataSet(204, 73, s_Text);
   //Excel_Cell_DataSet(273, 73, s_Text);
   //Excel_Cell_DataSet(342, 73, s_Text);
   Excel_Cell_DataSet(135, 68, s_Text);
   Excel_Cell_DataSet(204, 68, s_Text);
   Excel_Cell_DataSet(273, 68, s_Text);
   Excel_Cell_DataSet(342, 68, s_Text);
   //*************************
   G_Log_Text = "�q�d�u               �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2003.09.09 �u�d�q�o�͒ǉ�
   //***************************
   //***  VER
   //***************************
   i_RecNo = 1267;
   s_Text = "�u�d�q." + P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(66, 75, s_Text);
   Excel_Cell_DataSet(135, 75, s_Text);
   Excel_Cell_DataSet(204, 75, s_Text);
   Excel_Cell_DataSet(273, 75, s_Text);
   Excel_Cell_DataSet(342, 75, s_Text);
   G_Log_Text = "�u�d�q               �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*************************

   //***************************
   //***  �����ԍ�
   //***************************
   // �wA006�x
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //s_Text = HanToZen( G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4) );
   s_Text = HanToZen( GetSeizouBangou(G_SAKUBAN) );
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E   

   //------------------------------------
   // 2007.10.11 �װ�d�l�Ή�
   if(m_pcHeadsData->GetMirrorFlag()){
      s_Text = s_Text + HanToZen( "A/B" );
   }
   // 2007.10.11
   //------------------------------------

   Excel_Cell_DataSet(6, 3, s_Text);
   G_Log_Text = "�����ԍ�             �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �[����
   //***************************
   // �wA007�x
   i_RecNo = 1065;
   s_Text = P_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(6, 14, s_Text);
   G_Log_Text = "�[����               �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //***  ��  ��
   //*******************
   // �wA008�x
   i_RecNo = 39;
   s_Text = P_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = "'"+ HanToZen(s_Text);
      Excel_Cell_DataSet(6, 36, s_Text);
      G_Log_Text = "�䐔                  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�䐔      RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      //Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //************************
   //***  �K�p�K�i
   //************************
   // �wA009�x
   i_RecNo = 29;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "�K�p�K�i              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x�ɂ��A";

   ZeroSupress( s_Text, s_Text, 2 );

   s_Text = Search_KIKAKU_CODE(s_Text,true,true, P_HEADS_DATA[1].TrimRight());
   Excel_Cell_DataSet(6, 38, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //2019/05/17 ���p�Z�p�ǉ�
   if (!bIsNotKatashiki) {

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                        ���ו��@�@�f�[�^�Z�b�g                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------
   // 2007.10.11 �W�v�p�ϐ�������
   P_WkRec = 0;
   P_WkRec3 = 0;

   //2003.11.14 �֐��ǉ�
   Read_Buhin_Data();        // ���p���i�s���Ǎ�
   //*******************

   //2019.10.23 �t���i�ް��o�͍s���ύX_S
   Read_ACCS_Data();         // �t���i�s���Ǎ�
   //2019.10.23 �t���i�ް��o�͍s���ύX_E

   Write_Buhin_Detail();

   Cyuki_Out();

   // �y�[�W����ǉ��������A�f�[�^���P�s���ǉ����Ă��Ȃ��ꍇ�͂P�y�[�W���Z����B
   if(P_Row <= ( DefExcelPageLine + (P_PageNum-1) * DefExcelPageLine ) + 3) {
        P_PageNum--;
   }

   // �V�E�W�����i�\�p�@�s�v�y�[�W�폜����
   Excel_Cell_Delete(P_PageNum+1);

   //2019/05/17 ���p�Z�p�ǉ�
   } else {
       if (!bHasOldDataFlag) {
           // �V�E�W�����i�\�p�@�s�v�y�[�W�폜����
           Excel_Cell_Delete(1);
       }
   }

   //******************************
   //***  Excel Book Close
   //******************************
   // 2014/08/07 ���p�Z�p�ǉ�
   // ���ꎯ�ʕ�����EN(�p��)�̏ꍇ�A���p�ϊ��}�N�������s
   if(G_Language == "EN") {
        exApplication.OleFunction("Run" , OleVariant("TranslateJPtoEN"));
   }

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
   G_Log_Text = "----------    �w�W�����i�\�x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

//2003.11.14 ���p���i�s��****************************************************
//---------------------------------------------------------------------------
// ���{��֐����F ���p���i�s���Ǎ�
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Read_Buhin_Data(void)
{
   char  buf[1024];                            /* ������o�b�t�@   */
   char  *tok;                                 /* �g�[�N���|�C���^ */
   int   i;                                    /* ���[�v�J�E���^   */
   char  token[100][100];                      /* �g�[�N���o�b�t�@ */

//   AnsiString P_PARTS[16]; // ���i�f�[�^
   AnsiString P_PARTS[17]; // ���i�f�[�^
   AnsiString wk_FilePass;
   FILE *fp;

   // 2007.10.11 �װ�d�l�Ή� �ǉ�
   AnsiString wk_PARTS[1000][17];
   int i_wkNum;
   int j;

   iBLTCV = 0;
   P_SHIP_Num = 0;
   i_wkNum = 0;

   //***************************
   //**  ���i�W�J�ް��Ǎ�
   //***************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0)
   {
      //�}���`�T�[�o�Ή� 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN)
      //            + "HD_PARTS.csv";
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
      //�}���`�T�[�o�Ή� 2009/1/13 MOD END
   } else {
      //�}���`�T�[�o�Ή� 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN)
      //            + "HD_PARTS.csv";
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
      //�}���`�T�[�o�Ή� 2009/1/13 MOD END
   }

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      fclose(fp);    // ����
      return false;
   }

   //*****************
   // �Ǎ��݉\�̊�
   //*****************
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
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

      //*****************************************
      //***  ���i�f�[�^(HD_PARTS.csv)�̎擾
      //***  (���i����)
      //*****************************************
      P_PARTS[0] = token[0];     // ���i����
      P_PARTS[1] = token[1];     // ���ʎq
      P_PARTS[2] = token[2];     // ���i�^��
      P_PARTS[3] = token[3];     // ���i����
      P_PARTS[4] = token[4];     // �ގ��^��
      P_PARTS[5] = token[5];     // �}��
      P_PARTS[6] = token[6];     // ��}�p�}��
      P_PARTS[7] = token[7];     // �P�䕪����
      P_PARTS[8] = token[8];     // �\����
      P_PARTS[9] = token[9];     // ���v
      P_PARTS[10] = token[10];   // �d�l�P
      P_PARTS[11] = token[11];   // �d�l�Q
      P_PARTS[12] = token[12];   // �d�l�R
      P_PARTS[13] = token[13];   // �d�l�S
      P_PARTS[14] = token[14];   // �d�l�T
      P_PARTS[15] = token[15];   // �ގ�����
      P_PARTS[16] = token[16];   // �d�l����(""/"A"/"B")

      //---------------------------------------------------
      //  ���p���i�s��
      //---------------------------------------------------
      if( P_PARTS[0] == "SHIP" )
      {
        //---------------------------------
        // 2007.10.11 �װ�d�l�Ή�
        // A�d�l��B�d�l�œ��ް������݂���ꍇ������̂ŁAϰ�ނ���

         if( i_wkNum > 0 ){
            for( j = 0; j < i_wkNum; j++){
                if( (wk_PARTS[j][1] == P_PARTS[1])
                 && (wk_PARTS[j][3] == P_PARTS[3])
                 && (wk_PARTS[j][4] == P_PARTS[4])
                 && (wk_PARTS[j][5] == P_PARTS[5])
                 && (wk_PARTS[j][10] == P_PARTS[10])
                 && (wk_PARTS[j][11] == P_PARTS[11])
                 && (wk_PARTS[j][12] == P_PARTS[12])
                 && (wk_PARTS[j][13] == P_PARTS[13]) )
                {
                    break;
                }
            }
         }

         if(( i_wkNum == 0 ) || ( j == i_wkNum )){
            // �܂��擾���Ă��Ȃ���΁A��Ɣz��Ɋi�[
            wk_PARTS[i_wkNum][0] = P_PARTS[0];     // ���i����
            wk_PARTS[i_wkNum][1] = P_PARTS[1];     // ���ʎq
            wk_PARTS[i_wkNum][2] = P_PARTS[2];     // ���i�^��
            wk_PARTS[i_wkNum][3] = P_PARTS[3];     // ���i����
            wk_PARTS[i_wkNum][4] = P_PARTS[4];     // �ގ��^��
            wk_PARTS[i_wkNum][5] = P_PARTS[5];     // �}��
            wk_PARTS[i_wkNum][6] = P_PARTS[6];     // ��}�p�}��
            wk_PARTS[i_wkNum][7] = P_PARTS[7];     // �P�䕪����
            wk_PARTS[i_wkNum][8] = P_PARTS[8];     // �\����
            wk_PARTS[i_wkNum][9] = P_PARTS[9];     // ���v
            wk_PARTS[i_wkNum][10] = P_PARTS[10];   // �d�l�P
            wk_PARTS[i_wkNum][11] = P_PARTS[11];   // �d�l�Q
            wk_PARTS[i_wkNum][12] = P_PARTS[12];   // �d�l�R
            wk_PARTS[i_wkNum][13] = P_PARTS[13];   // �d�l�S
            wk_PARTS[i_wkNum][14] = P_PARTS[14];   // �d�l�T
            wk_PARTS[i_wkNum][15] = P_PARTS[15];   // �ގ�����
            wk_PARTS[i_wkNum][16] = P_PARTS[16];   // �d�l����(""/"A"/"B")

            // SHIP�s����
            i_wkNum++;

            P_SHIP_Num++;
         }

        // 2007.10.11 �װ�d�l�Ή�
        //---------------------------------

      //2019.10.10 �t���i�ް��o�͍s���ύX_S
      }

   }

   fclose(fp);    // ����
   return true;
}
//***************************************************************************

//2019.10.23 �t���i���i�s��_S************************************************
//---------------------------------------------------------------------------
// ���{��֐����F �t���i���i�s���Ǎ�
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Read_ACCS_Data(void)
{
   char  buf[1024];                            /* ������o�b�t�@   */
   char  *tok;                                 /* �g�[�N���|�C���^ */
   int   i;                                    /* ���[�v�J�E���^   */
   char  token[100][100];                      /* �g�[�N���o�b�t�@ */

   AnsiString P_PARTS[17]; // ���i�f�[�^
   AnsiString wk_FilePass;
   FILE *fp;

   AnsiString wk_PARTS[1000][17];
   int i_wkNum;
   int j;

   iBLTCV = 0;
   P_ACCS_Num = 0;  // �t���i���i�s��
   i_wkNum = 0;

   //***************************
   //**  ���i�W�J�ް��Ǎ�
   //***************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0)
   {
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
   } else {
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
   }

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      fclose(fp);    // ����
      return false;
   }

   //*****************
   // �Ǎ��݉\�̊�
   //*****************
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
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

      //*****************************************
      //***  ���i�f�[�^(HD_PARTS.csv)�̎擾
      //***  (���i����)
      //*****************************************
      P_PARTS[0] = token[0];     // ���i����
      P_PARTS[1] = token[1];     // ���ʎq
      P_PARTS[2] = token[2];     // ���i�^��
      P_PARTS[3] = token[3];     // ���i����
      P_PARTS[4] = token[4];     // �ގ��^��
      P_PARTS[5] = token[5];     // �}��
      P_PARTS[6] = token[6];     // ��}�p�}��
      P_PARTS[7] = token[7];     // �P�䕪����
      P_PARTS[8] = token[8];     // �\����
      P_PARTS[9] = token[9];     // ���v
      P_PARTS[10] = token[10];   // �d�l�P
      P_PARTS[11] = token[11];   // �d�l�Q
      P_PARTS[12] = token[12];   // �d�l�R
      P_PARTS[13] = token[13];   // �d�l�S
      P_PARTS[14] = token[14];   // �d�l�T
      P_PARTS[15] = token[15];   // �ގ�����
      P_PARTS[16] = token[16];   // �d�l����(""/"A"/"B")

      //---------------------------------------------------
      //  �t���i���i�s��
      //---------------------------------------------------
      if( P_PARTS[0] == "ACCS" )
      {
        //---------------------------------
        // 2007.10.11 �װ�d�l�Ή�
        // A�d�l��B�d�l�œ��ް������݂���ꍇ������̂ŁAϰ�ނ���

         if( i_wkNum > 0 ){
            for( j = 0; j < i_wkNum; j++){
                if( (wk_PARTS[j][1] == P_PARTS[1])
                 && (wk_PARTS[j][3] == P_PARTS[3])
                 && (wk_PARTS[j][4] == P_PARTS[4])
                 && (wk_PARTS[j][5] == P_PARTS[5])
                 && (wk_PARTS[j][10] == P_PARTS[10])
                 && (wk_PARTS[j][11] == P_PARTS[11])
                 && (wk_PARTS[j][12] == P_PARTS[12])
                 && (wk_PARTS[j][13] == P_PARTS[13]) )
                {
                    break;
                }
            }
         }

         if(( i_wkNum == 0 ) || ( j == i_wkNum )){
            // �܂��擾���Ă��Ȃ���΁A��Ɣz��Ɋi�[
            wk_PARTS[i_wkNum][0] = P_PARTS[0];     // ���i����
            wk_PARTS[i_wkNum][1] = P_PARTS[1];     // ���ʎq
            wk_PARTS[i_wkNum][2] = P_PARTS[2];     // ���i�^��
            wk_PARTS[i_wkNum][3] = P_PARTS[3];     // ���i����
            wk_PARTS[i_wkNum][4] = P_PARTS[4];     // �ގ��^��
            wk_PARTS[i_wkNum][5] = P_PARTS[5];     // �}��
            wk_PARTS[i_wkNum][6] = P_PARTS[6];     // ��}�p�}��
            wk_PARTS[i_wkNum][7] = P_PARTS[7];     // �P�䕪����
            wk_PARTS[i_wkNum][8] = P_PARTS[8];     // �\����
            wk_PARTS[i_wkNum][9] = P_PARTS[9];     // ���v
            wk_PARTS[i_wkNum][10] = P_PARTS[10];   // �d�l�P
            wk_PARTS[i_wkNum][11] = P_PARTS[11];   // �d�l�Q
            wk_PARTS[i_wkNum][12] = P_PARTS[12];   // �d�l�R
            wk_PARTS[i_wkNum][13] = P_PARTS[13];   // �d�l�S
            wk_PARTS[i_wkNum][14] = P_PARTS[14];   // �d�l�T
            wk_PARTS[i_wkNum][15] = P_PARTS[15];   // �ގ�����
            wk_PARTS[i_wkNum][16] = P_PARTS[16];   // �d�l����(""/"A"/"B")

            // SHIP�s����
            i_wkNum++;

            P_ACCS_Num++;
         }


      }

   }

   fclose(fp);    // ����
   return true;
}
//2019.10.23 �t���i���i�s��_E************************************************

//---------------------------------------------------------------------------
// ���{��֐����F �W�����i�\(Buhinhyou)�@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Write_Buhin_Detail(void)
{
   char  buf[1024];                            /* ������o�b�t�@   */
   char  *tok;                                 /* �g�[�N���|�C���^ */
   int   i;                                    /* ���[�v�J�E���^   */
   char  token[100][100];                      /* �g�[�N���o�b�t�@ */

   int   RecNo;

//@@@@
   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�

   int   i_plateflg = 0;   // �v���[�g���i���ٕ\���׸�
   int   i_frameflg = 0;   // �t���[�����i���ٕ\���׸�
   int   i_nozzlflg = 0;   // �m�Y�����i���ٕ\���׸�
   int   i_Atempflg = 0;   // A�����x�v���i���ٕ\���׸�
   int   i_Btempflg = 0;   // B�����x�v���i���ٕ\���׸�
   int   i_Apresflg = 0;   // A�����͌v���i���ٕ\���׸�
   int   i_Bpresflg = 0;   // B�����͌v���i���ٕ\���׸�
   int   i_accsflg  = 0;   // �t���i���ٕ\���׸�
   int   i_shipflg  = 0;   // ���p���i���ٕ\���׸�
   int   i_errflg   = 0;   // �G���[���i���ٕ\���׸�

   int   i_anaakeflg = 0;  // �E�����ٕ\���׸�
   int   i_tyukiflg = 0;
   int   i_RecNo;

   bool  b_YobiSumMode;

   AnsiString s_Text;

//   AnsiString P_PARTS[15]; // ���i�f�[�^
   AnsiString P_PARTS[17]; // ���i�f�[�^

   int   i_ErrItemCnt;          // "ERROR-"�ŏo�͂��ꂽ���ڂ̃J�E���^
   int   i_Base_Y;              // "ERROR-"�̍��ڂ̏����o���ʒu(Y��)

   bool bRet;
   AnsiString s_wkData[1000][17];
   int j;

//@@@@

   AnsiString wk_FilePass;
   FILE *fp;

   //2005-10-07
   i_ErrItemCnt = 0;
   i_Base_Y = 112;

   iBLTCV = 0;
   //***************************
   //**  ���i�W�J�ް��Ǎ�
   //***************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0)
   {
      //�}���`�T�[�o�Ή� 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN)
      //            + "HD_PARTS.csv";
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
      //�}���`�T�[�o�Ή� 2009/1/13 MOD END

   } else {
      //�}���`�T�[�o�Ή� 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN)
      //            + "HD_PARTS.csv";
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
      //�}���`�T�[�o�Ή� 2009/1/13 MOD END
   }
   //***************************
   //**  �O�񕔕i�W�J�ް��Ǎ�
   //***************************
   if( Prev_Data_Get() )
      P_compflg = 1;
   else
      P_compflg = 0;

   //2023.02.08 �����ԍ������ǉ�_S
   if( (P_compflg == 0) && (G_SAKUBAN.Length() > 7) && (G_SAKUBAN.SubString(1, 2) == "10") ) {
      //�O�񕔕i�W�J�ް��Ǎ��Ɏ��s���W���œ��͂��ꂽ�U������O�񕔕i�W�J�ް��Ǎ�
      if( Prev_Old_Data_Get() ) {
         P_compflg = 1;
      } else {
         P_compflg = 0;
      }
   }
   //2023.02.08 �����ԍ������ǉ�_E

   //2004.12.15 REV00�̏ꍇ�̒�����ύX
   if (P_HEADS_DATA[319].TrimRight() == "0" || P_HEADS_DATA[319].TrimRight() == "00") {
      P_compflg = 0;
   }
   //**********************************

    //-----------------------------------------------------------------------------
    // 2007.10.12 �װ�d�l�Ή�
    // ���i�W�J�ް��̓Ǎ��݂͕ʊ֐��ōs���悤�ύX
    // �ʊ֐��ŁA�װ�d�l���ް���ϰ��,��Ă���Ĕz��Ɋi�[�����B
    // ϰ��,��Ă��ꂽ�z���ް�����������B

/*   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   //*****************
   // �Ǎ��݉\�̊�
   //*****************
   P_Row = 8;        // �����o�͍s�ݒ�
   RecNo = 0;
   P_PageNum = 0;
   b_YobiSumMode = false;

   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
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

      //*****************************************
      //***  ���i�f�[�^(HD_PARTS.csv)�̎擾
      //***  (�^���A���ʎq�ȊO�S�p)
      //*****************************************
      P_PARTS[0] = token[0];     // ���i����
      P_PARTS[1] = token[1];     // ���ʎq
      P_PARTS[2] = token[2];     // ���i�^��
      P_PARTS[3] = token[3];     // ���i����
      P_PARTS[4] = token[4];     // �ގ��^��
      P_PARTS[5] = token[5];     // �}��
      P_PARTS[6] = token[6];     // ��}�p�}��
      P_PARTS[7] = token[7];     // �P�䕪����
      P_PARTS[8] = token[8];     // �\����
      P_PARTS[9] = token[9];     // ���v
      P_PARTS[10] = token[10];   // �d�l�P
      P_PARTS[11] = token[11];   // �d�l�Q
      P_PARTS[12] = token[12];   // �d�l�R
      P_PARTS[13] = token[13];   // �d�l�S
      P_PARTS[14] = token[14];   // �d�l�T
*/

   //*********************************************
   // ���i�ް���z��ɓǂݍ���(ϰ��,��ċ@�\�t)
   //*********************************************
   bRet = Read_Buhin_Data_Sorted(wk_FilePass, s_wkData);
   if(!bRet) return(false);

   //*****************
   // �Ǎ��݉\�̊�
   //*****************
   P_Row = 8;        // �����o�͍s�ݒ�
   RecNo = 0;
   P_PageNum = 0;
   b_YobiSumMode = false;

   for(j=0;j<1000;j++){
     if( s_wkData[j][0].Length() == 0 ) break;

      //*****************************************
      //***  ���i�f�[�^(HD_PARTS.csv)�̎擾
      //***  (�^���A���ʎq�ȊO�S�p)
      //*****************************************
      P_PARTS[0] = s_wkData[j][0];     // ���i����
      P_PARTS[1] = s_wkData[j][1];     // ���ʎq
      P_PARTS[2] = s_wkData[j][2];     // ���i�^��
      P_PARTS[3] = s_wkData[j][3];     // ���i����
      P_PARTS[4] = s_wkData[j][4];     // �ގ��^��
      P_PARTS[5] = s_wkData[j][5];     // �}��
      P_PARTS[6] = s_wkData[j][6];     // ��}�p�}��
      P_PARTS[7] = s_wkData[j][7];     // �P�䕪����
      P_PARTS[8] = s_wkData[j][8];     // �\����
      P_PARTS[9] = s_wkData[j][9];     // ���v
      P_PARTS[10] = s_wkData[j][10];   // �d�l�P
      P_PARTS[11] = s_wkData[j][11];   // �d�l�Q
      P_PARTS[12] = s_wkData[j][12];   // �d�l�R
      P_PARTS[13] = s_wkData[j][13];   // �d�l�S
      P_PARTS[14] = s_wkData[j][14];   // �d�l�T
      P_PARTS[15] = s_wkData[j][15];   // �ގ�����
      P_PARTS[16] = s_wkData[j][16];   // �װ�d�l(""/"A"/"B")

    // 2007.10.12 �װ�d�l�Ή�
    // ���i�W�J�ް��̓Ǎ��݂͕ʊ֐��ōs���悤�ύX
    //-----------------------------------------------------------------------------

      // ����0�̏ꍇ�̓u�����N�Ƃ���
      //2009/04/01 �����ԍ�22-7621�̕W�����i�\�ŃG���[�Ή� MOD_S
      //if (P_PARTS[7] == "0" ) {
      if ((P_PARTS[7] == "0" ) || (P_PARTS[7] == "�O" )) {
         P_PARTS[7] = "";
      }
      //if (P_PARTS[8] == "0" ) {
      if ((P_PARTS[8] == "0" ) || (P_PARTS[8] == "�O" )) {
         P_PARTS[8] = "";
      }
      //2009/04/01 �����ԍ�22-7621�̕W�����i�\�ŃG���[�Ή� MOD_E

      //----------------------------
      // 2007.10.12 �װ�d�l�Ή�
      //for( i=0; i<15; i++ ) {
      for( i=0; i<17; i++ ) {
      // 2007.10.12
      //----------------------------
         if( i > 1 ){
//            if ( P_PARTS[i].Pos("�t���m�Y��") != 0 ) {
//               P_PARTS[i] = HanToZen(P_PARTS[i].SubString(1,11)) + P_PARTS[i].SubString(12,P_PARTS[i].Length());
//            } else {
               P_PARTS[i] = HanToZen(P_PARTS[i]);
//            }
         }
      }

      //****************************************************************************
      //***                                                                      ***
      //***                         �v���[�g���i�o��                             ***
      //***                                                                      ***
      //****************************************************************************
      if( P_PARTS[0] == "PLATE" ){
         //*****************************
         //***  �^�C�g���@�Z�b�g
         //*****************************
         //��2002/10/24 �W�v������ǉ�
         if( i_plateflg == 0 )
         {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            // ���s
            P_Row++;

            Excel_Cell_DataSet(P_Row, 3, "�v���[�g���i");
            P_Row++;
            i_plateflg++;
            P_WkRec = 0;
         }
         //��2002/10/24

         //��2014/07/17 ���v��0�ł���ꍇ�A�o�͂��Ȃ��悤�ɂ���B
         if ((P_PARTS[9] == "0" ) || (P_PARTS[9] == "�O" )) {
             continue;
         }
         //��2014/07/17

         if ( P_PARTS[1] == "DPLT--" ) {
            i_anaakeflg = 0;
            i_tyukiflg = 0;

            //*******************************
            //***  �v���[�g�{�̃f�[�^�o��
            //*******************************
            // ���i�f�[�^
            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]); // ���i�P(���i�^��)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]); // ���i�Q(���i����)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]); // �ގ�����
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]); // �}��
            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]); // 1�䕪����
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]); // �\����
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]); // ���v
            if ( P_compflg == 1 ) {
               if( !Compare_Data( P_PARTS )){
                  Excel_Cell_DataSet(P_Row, 80, "*");  // ��r
               }
            }
            P_Row++;

         } else if( P_PARTS[1] == "PLHL--" ) {
            //*********************************************
            //*** �v���[�g�E���f�[�^�o�� ���ʎq�PLHL--�
            //*********************************************
            if( i_anaakeflg == 0 ){
               //******************
               //*** ���L(�֖�1)
               //******************
               i_RecNo = 1302;
               s_Text = P_HEADS_DATA[i_RecNo];
               Excel_Cell_DataSet(P_Row, 5, s_Text);
               Excel_Cell_DataSet(P_Row, 47, "�E���d�l");
               P_Row++;

               i_anaakeflg = 1;
            }

            if( i_tyukiflg == 0 ){
               //*****************
               // ���L(�d������1)
               //*****************
               i_RecNo = 1304;
               s_Text = P_HEADS_DATA[i_RecNo];
               Excel_Cell_DataSet(P_Row, 5, s_Text);
               i_tyukiflg = 1;
            }

            //****************
            // �E�����i�f�[�^
            //****************
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]); // �d�l�P
            Excel_Cell_DataSet(P_Row, 62, P_PARTS[11]); // �d�l�Q
            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);  // 1�䕪����
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);  // �\����
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);  // ���v
            Excel_Cell_DataSet(P_Row, 79, P_PARTS[13]); // �P�ʁi���j
            if( P_compflg == 1 ){
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");   // ��r
            }

            //----------------------------------------
            // 2007.10.15 �װ�d�l�Ή� ���@���̏o��
            if ( P_PARTS[16] != "" ) {
                Excel_Cell_DataSet(P_Row, 66, P_PARTS[16] + "���@");  // ���@����
            }
            // 2007.10.15
            //----------------------------------------

            P_Row++;

         } else {
            //**********************************************
            // �v���[�g���i�f�[�^ (����=0�̏ꍇ�ǂݔ�΂�)
            //**********************************************

            //---------------------------------------------------
            // 2007.10.25 �װ�d�l�Ή�
            // �ȉ��̕��i�W�v���@��ύX(�W�v����������������)
            //  [D��ڰĶ޽���A][D��ڰĶ޽���B]
            //  [D��ڰĶ޽���DA][D��ڰĶ޽���DB]
            //  [E��ڰĶ޽���DA][E��ڰĶ޽���DB]
            // 2007.11.02 �ǉ�
            //  [E��ڰė����Ĕ�]

            //Buhin_Syukei2( P_PARTS );

            if( ( P_PARTS[1] == "DPLG-A" )
             || ( P_PARTS[1] == "DPLG-B" )
             || ( P_PARTS[1] == "DPLGDA" )
             || ( P_PARTS[1] == "DPLGDB" )
             || ( P_PARTS[1] == "EPLGDA" )
             || ( P_PARTS[1] == "EPLGDB" )
             || ( P_PARTS[1] == "EPGS-R" ) ){
                // ���i�W�v
                Buhin_Syukei( P_PARTS );
            } else {
                // ���i�W�v
                Buhin_Syukei2( P_PARTS );
            }

            // 2007.10.25
            //---------------------------------------------------

            b_YobiSumMode = true;
            // ���s
//            if( ZenToHan( P_PARTS[7] )!= "0" ) {
//               Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]); // ���i�P(���i�^��)
//               Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]); // ���i�Q(���i����)
//               Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]); // �ގ�����
//               Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]); // �}��
//               Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]); // 1�䕪����
//               Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]); // �\����
//               Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]); // ���v
//               if( P_compflg == 1 ) {
//                  if( !Compare_Data( P_PARTS ) )
//                     Excel_Cell_DataSet(P_Row, 80, "*");  // ��r
//               }
//               P_Row++;
//            }
         }
      }


      //****************************************************************************
      //***                                                                      ***
      //***                         �t���[�����i�o��                             ***
      //***                                                                      ***
      //****************************************************************************
      else if( P_PARTS[0] == "MOATE" ){
         if( P_PARTS[1] == "EPBP-A" || P_PARTS[1] == "EPBP-B" )
         {
            Buhin_Syukei3( P_PARTS );
            b_YobiSumMode = true;
         }
      }
      else if( P_PARTS[0] == "FRAME" ){
         if( i_frameflg == 0 ) {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // ���s
               P_Row++;
               Write_BorderLine();
            }
            Excel_Cell_DataSet(P_Row, 3, "�t���[�����i");
            P_Row++;
            i_frameflg++;
         }

         // 2022.05.23 �t���[���K�X�؂�ǉ�_S
         //*****************************
         // �t���[��S�K�X�؂�f�[�^�o��
         //*****************************
         if( P_PARTS[1] == "FRAMSG" )
         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);   // ���i�P(���i�^��)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);   // ���i�Q(���i����)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);   // �ގ�����
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);   // �}��
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]);  // �d�l�P

            // ��ʏ�d�l 28�޲Ă𒴂��邩
            // ��װ�d�l  22�޲Ă𒴂��邩
            if( P_PARTS[11].Length() > 28 ) {
               Excel_Cell_DataSet(P_Row, 55, P_PARTS[11]);  // �d�l�Q
            } else {
               Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]);  // �d�l�Q
            }
            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);   // 1�䕪����
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);   // �\����
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);   // ���v
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // ��r
            }

            P_Row++;

         }
         // 2022.05.23 �t���[���K�X�؂�ǉ�_E

         //*****************************
         //*** �t���[��S�{�̃f�[�^�o��
         //*****************************
         // 2022.05.23 �t���[���K�X�؂�ǉ�_S
         //if( P_PARTS[1] == "FRAM-S" )
         else if( P_PARTS[1] == "FRAM-S" )
         // 2022.05.23 �t���[���K�X�؂�ǉ�_E
         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);   // ���i�P(���i�^��)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);   // ���i�Q(���i����)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);   // �ގ�����
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);   // �}��
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]);  // �d�l�P
//            Excel_Cell_DataSet(P_Row, 62, P_PARTS[11]);  // �d�l�Q
            if( P_PARTS[11].Length() > 28 ) {
               Excel_Cell_DataSet(P_Row, 55, P_PARTS[11]);  // �d�l�Q
            } else {
               Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]);  // �d�l�Q
            }
            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);   // 1�䕪����
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);   // �\����
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);   // ���v
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // ��r
            }
            P_Row++;

            //*************
            // ���L(ɽ��1)
            //*************
            i_RecNo = 1325;
            s_Text = P_HEADS_DATA[i_RecNo];
            Excel_Cell_DataSet(P_Row, 47, s_Text);

            P_Row++;

            //*************
            // ���L(ɽ��2)
            //*************
            i_RecNo = 1326;
            s_Text = P_HEADS_DATA[i_RecNo];
            Excel_Cell_DataSet(P_Row, 47, s_Text);

            P_Row++;
         }

         // 2022.05.23 �t���[���K�X�؂�ǉ�_S
         //*****************************
         // �t���[��E�K�X�؂�f�[�^�o��
         //*****************************
         else if( P_PARTS[1] == "FRAMEG" )


         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);  // ���i�P(���i�^��)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);  // ���i�Q(���i����)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);  // �ގ�����
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);  // �}��
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]); // �d�l�P

            // ��ʏ�d�l 28�޲Ă𒴂��邩
            // ��װ�d�l  22�޲Ă𒴂��邩
            if( P_PARTS[11].Length() > (( m_pcHeadsData->GetMirrorFlag())? 22: 28 ) ){
               Excel_Cell_DataSet(P_Row, 55, P_PARTS[11]); // �d�l�Q
            } else {
               Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // �d�l�Q
            }

            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);  // 1�䕪����
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);  // �\����
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);  // ���v
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // ��r
            }

            if (P_PARTS[16] != "" ) {
                Excel_Cell_DataSet(P_Row, 66, P_PARTS[16] + "���@");  // ���@����
            }

            P_Row++;

         }
         // 2022.05.23 �t���[���K�X�؂�ǉ�_E

         //*****************************
         // �t���[��E�{�̃f�[�^�o��
         //*****************************
         else if( P_PARTS[1] == "FRAM-E" )
         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);  // ���i�P(���i�^��)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);  // ���i�Q(���i����)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);  // �ގ�����
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);  // �}��
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]); // �d�l�P
//            Excel_Cell_DataSet(P_Row, 62, P_PARTS[11]); // �d�l�Q

            //-------------------------------------
            // 2007.11.14 �װ�d�l�Ή�
            // ��ʏ�d�l 28�޲Ă𒴂��邩
            // ��װ�d�l  22�޲Ă𒴂��邩

            //if( P_PARTS[11].Length() > 28 ) {
            if( P_PARTS[11].Length() > (( m_pcHeadsData->GetMirrorFlag())? 22: 28 ) ){
               Excel_Cell_DataSet(P_Row, 55, P_PARTS[11]); // �d�l�Q
            } else {
               Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // �d�l�Q
            }
            // 2007.11.14
            //-------------------------------------

            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);  // 1�䕪����
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);  // �\����
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);  // ���v
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // ��r
            }

            //---------------------------------------
            // 2007.10.12 �װ�d�l�Ή� ���@���̒ǉ�
            if (P_PARTS[16] != "" ) {
                Excel_Cell_DataSet(P_Row, 66, P_PARTS[16] + "���@");  // ���@����
            }
            // 2007.10.12
            //---------------------------------------

            P_Row++;

            //---------------------------------------
            // 2007.10.17 �װ�d�l�Ή�
            // �װA�d�l�̒���ɂ͏o�͂��Ȃ�(if���ǉ�)

            // ���d�l/�װB�d�l�̏ꍇ
            if ( ZenToHan( P_PARTS[16] ) != "A"){
                //*************
                // ���L(ɽ��3)
                //*************
                i_RecNo = 1327;
                s_Text = P_HEADS_DATA[i_RecNo];
                Excel_Cell_DataSet(P_Row, 47, s_Text);

                P_Row++;

                //*************
                // ���L(ɽ��4)
                //*************
                i_RecNo = 1328;
                s_Text = P_HEADS_DATA[i_RecNo];
                Excel_Cell_DataSet(P_Row, 47, s_Text);

                P_Row++;

                // ܰ��i�[�ް�����?
                if( P_WkRec3 > 0 )
                    Output_WK_PARTS3();

                // ���s
                P_Row++;
            }

            // 2007.10.17
            //---------------------------------------
         }

         //********************************
         //*** �㕔�K�C�h�o�[�f�[�^�o��
         //********************************
         else if( P_PARTS[1] == "GDBR-U" )
         {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(true);

            // ���s
            P_Row++;

            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]); // ���i�P(���i�^��)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]); // ���i�Q(���i����)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]); // �ގ�����
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]); // �}��

            i_RecNo = 1022;
            s_Text = "�k�t�P��" + HanToZen( P_HEADS_DATA[i_RecNo] );
            Excel_Cell_DataSet(P_Row, 47, s_Text);     // �d�l(�k�t�P)
            Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // �d�l�Q

            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]); // 1�䕪����
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]); // �\����
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]); // ���v
            Excel_Cell_DataSet(P_Row, 79, P_PARTS[13]); // �P�ʁi���j
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // ��r
            }
            P_Row++;

            // �d�l(2�s��)
            i_RecNo = 1023;
            s_Text = "�k�t�Q��" + HanToZen( P_HEADS_DATA[i_RecNo] );
            Excel_Cell_DataSet(P_Row, 47, s_Text);     // �d�l(�k�t�Q)

            i_RecNo = 1288;
            if( P_HEADS_DATA[i_RecNo].ToIntDef(0) !=  0 )
            {
               s_Text = "�k�t�R��" + HanToZen( P_HEADS_DATA[i_RecNo] );
               Excel_Cell_DataSet(P_Row, 58, s_Text);  // �d�l(�k�t�R)
            }
            P_Row++;
         }

         //*********************************
         //***  �����K�C�h�o�[�f�[�^�o��
         //*********************************
         else if( P_PARTS[1] == "GDBR-L" )
         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]); // ���i�P(���i�^��)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]); // ���i�Q(���i����)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]); // �ގ�����
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]); // �}��

            i_RecNo = 1024;
            s_Text = "�k�k�P��" + HanToZen( P_HEADS_DATA[i_RecNo] );
            Excel_Cell_DataSet(P_Row, 47, s_Text);     // �d�l(�k�k�P)
            Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // �d�l�Q

            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]); // 1�䕪����
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]); // �\����
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]); // ���v
            Excel_Cell_DataSet(P_Row, 79, P_PARTS[13]); // �P�ʁi���j
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // ��r
            }
            P_Row++;

            // �d�l(2�s��)
            i_RecNo = 1025;
            s_Text = "�k�k�Q��" + HanToZen( P_HEADS_DATA[i_RecNo] );
            Excel_Cell_DataSet(P_Row, 47, s_Text);     // �d�l(�k�k�Q)

            i_RecNo = 1289;
            if( P_HEADS_DATA[i_RecNo].ToIntDef(0) !=  0 )
            {
               s_Text = "�k�k�R��" + HanToZen( P_HEADS_DATA[i_RecNo] );
               Excel_Cell_DataSet(P_Row, 58, s_Text);  // �d�l(�k�k�R)
            }
            P_Row++;
         }

         else
         {
            //**********************************************
            // �t���[�����i�f�[�^ (����=0�̏ꍇ�ǂݔ�΂�)
            //**********************************************
            if( ( ZenToHan( P_PARTS[7] )!= "" )&&
                ( P_PARTS[4].SubString(1,5) != "GDBR-" ) )
            {
               if( ( P_PARTS[1] != "BNWR--" )&&( P_PARTS[1] != "BOLTNR" ) ) {
                 if ( iBLTCV == 1 ) {
                   iBLTCV=2;
//                   BLT_Cover_Out();
                 }
               }
               Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);  // ���i�P(���i�^��)
               Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);  // ���i�Q(���i����)
               if( ( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) ) && (P_HEADS_DATA[36].Pos("M")!=0) ) {
                  Excel_Cell_DataSet(P_Row, 13, "�i�b�g�͂v�i�b�g�̂���");  // ���p�̏ꍇ�A�{���g�i�b�g�ɒ��L��ǉ�
               }
               Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);  // �ގ�����
               //2014.12.11 ���čގ��ύX
               if( ( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) )
                && ( P_HEADS_DATA[36].Pos("B")!=0) ) {
                   if( P_PARTS[4] == "�r�t�r�R�O�S�^�b�R�U�O�S" ) {
                       //B�ڰ�SUS304/C3604�̏ꍇCrү��ǉ�
                       Excel_Cell_DataSet(P_Row, 24, "�r�t�r�R�O�S�^�b�R�U�O�S Crү�");
                   } else if( P_PARTS[4] == "�r�t�r�R�P�U�^�b�R�U�O�S" ) {
                       //B�ڰ�SUS316/C3604�̏ꍇCrү��ǉ�
                       Excel_Cell_DataSet(P_Row, 24, "�r�t�r�R�P�U�^�b�R�U�O�S Crү�");
                   }
               }
               //***********************
               Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);  // �}��

               Excel_Cell_DataSet(P_Row, 47, P_PARTS[10]); // �d�l1
               if ( P_PARTS[11].Length() > 0 ) {
                 Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // �d�l2
               }
               //2014.12.11 ���čގ��ύX
               if( ( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) )
                  && ( P_PARTS[11] == "�w���[���C��" ) ) {
                  //�԰ײ݂̏ꍇ�d�l2�ύX
                  Excel_Cell_DataSet(P_Row, 58, "���t�a�����̓w���[���C��");
               }
               //***********************
               Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);  // 1�䕪����
               Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);  // �\����
               Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);  // ���v

//               if( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) )
                  Excel_Cell_DataSet(P_Row, 79, P_PARTS[13]); // �P�ʁi���j

               if( P_compflg == 1 )
               {
                  if( !Compare_Data( P_PARTS ) )
                     Excel_Cell_DataSet(P_Row, 80, "*");      // ��r
               }
               P_Row++;
               if( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) ) {
                   iBLTCV=1;
               }
            }

         }
      }

      //****************************************************************************
      //***                                                                      ***
      //***                           �m�Y�����i�o��                             ***
      //***                                                                      ***
      //****************************************************************************
      else if( P_PARTS[0] == "NOZZLE" )
      {
         if ( iBLTCV == 1 ) {
            iBLTCV=2;
            BLT_Cover_Out();
            // 2014/08/25 ���p�Z�p�ҏW
            if(P_HEADS_DATA[36].Pos("Z") > 0) {
                BLT_Cover_Out_Small();
            }
         }
         if( i_nozzlflg == 0 )
         {
            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // ���s
               P_Row++;
               Write_BorderLine();
            }
            Excel_Cell_DataSet(P_Row, 3, "�m�Y�����i");

            P_Row++;
            i_nozzlflg++;
            P_WkRec = 0;
         }

         //2003.12.22
         //Buhin_Syukei2( P_PARTS );
         Buhin_Syukei4( P_PARTS );
         //**********
         b_YobiSumMode = true;
      }

      //---------------------------------------------------
      //  B�����x�v�ް����o��
      //---------------------------------------------------
      else if( P_PARTS[0] == "B_TEMP" )
      {
         if( i_Btempflg == 0 )
         {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // ���s
               P_Row++;
               Write_BorderLine();
            }

            //-----------------------------------------------
            // 2008.03.19 �~���[�d�l�̏ꍇ�AB���t�����o��
            //Excel_Cell_DataSet(P_Row, 3, "�a�����x�v");
            s_Text = ConvertRyutaiName("B", m_pcHeadsData->GetMirrorFlag());
            Excel_Cell_DataSet(P_Row, 3, s_Text + "�����x�v");
            //-----------------------------------------------

            P_Row++;
            i_Btempflg++;
            P_WkRec = 0;
         }

         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }

      //---------------------------------------------------
      //  A�����x�v�ް����o��
      //---------------------------------------------------
      else if( P_PARTS[0] == "A_TEMP" )
      {
         if( i_Atempflg == 0 )
         {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // ���s
               P_Row++;
               Write_BorderLine();
            }

            //-----------------------------------------------
            // 2008.03.19 �~���[�d�l�̏ꍇ�AA���t�����o��
            //Excel_Cell_DataSet(P_Row, 3, "�`�����x�v");
            s_Text = ConvertRyutaiName("A", m_pcHeadsData->GetMirrorFlag());
            Excel_Cell_DataSet(P_Row, 3, s_Text + "�����x�v");
            //-----------------------------------------------

            P_Row++;
            i_Atempflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;

      }

      //---------------------------------------------------
      //  B�����͌v�ް����o��
      //---------------------------------------------------
      else if( P_PARTS[0] == "B_PRSG" )
      {
         if( i_Bpresflg == 0 )
         {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // ���s
               P_Row++;
               Write_BorderLine();
            }

            //-----------------------------------------------
            // 2008.03.19 �~���[�d�l�̏ꍇ�AB���t�����o��
            //Excel_Cell_DataSet(P_Row, 3, "�a�����͌v");
            s_Text = ConvertRyutaiName("B", m_pcHeadsData->GetMirrorFlag());
            Excel_Cell_DataSet(P_Row, 3, s_Text + "�����͌v");
            //-----------------------------------------------

            P_Row++;
            i_Bpresflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }

      //---------------------------------------------------
      //  A�����͌v�ް����o��
      //---------------------------------------------------
      else if( P_PARTS[0] == "A_PRSG" )
      {
         if( i_Apresflg == 0 )
         {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // ���s
               P_Row++;
               Write_BorderLine();
            }

            //-----------------------------------------------
            // 2008.03.19 �~���[�d�l�̏ꍇ�AA���t�����o��
            //Excel_Cell_DataSet(P_Row, 3, "�`�����͌v");
            s_Text = ConvertRyutaiName("A", m_pcHeadsData->GetMirrorFlag());
            Excel_Cell_DataSet(P_Row, 3, s_Text + "�����͌v");
            //-----------------------------------------------

            P_Row++;
            i_Apresflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }

      //---------------------------------------------------
      //  �t���i�ް����o��
      //---------------------------------------------------
      else if( P_PARTS[0] == "ACCS" )
      {
         if( i_accsflg == 0 )
         {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            //2019.10.10 �t���i�ް��o�͍s���ύX_S
            //if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
            //   // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - ( P_ACCS_Num + 3 ) ) ) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s
               // ( P_ACCS_Num + 3 ):�\�z�����o�͍s��
            //2019.10.10 �t���i�ް��o�͍s���ύX_E
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // ���s
               P_Row++;
               Write_BorderLine();
            }
            Excel_Cell_DataSet(P_Row, 3, "�t���i");
            //G_Log_Text = "�^��                  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
            //Write_Log(G_Log_Text);
            P_Row++;
            i_accsflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }

      //---------------------------------------------------
      //  ���p���i�ް����o��
      //---------------------------------------------------
      else if( P_PARTS[0] == "SHIP" )
      {
         if( i_shipflg == 0 )
         {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            //2003.11.13 ���y�[�W�����ύX
            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - ( P_SHIP_Num + 3 ) ) ) {
              // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@
              // ( P_SHIP_Num + 3 ):�\�z�����o�͍s��
            //if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {  // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
            //***************************
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // ���s
               P_Row++;
               Write_BorderLine();
            }
            Excel_Cell_DataSet(P_Row, 3, "���p���i");
            P_Row++;
            i_shipflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }
      //---------------------------------------------------
      //  �G���[�o��
      //---------------------------------------------------
      else if( P_PARTS[0] == "ERROR" )
      {
        //----------------------------------
        // 2007.11.14 �R�����g�A�E�g
        /*
         //2005-10-07 �W�J�ł��Ȃ������\���i�����o�͂ł���悤�ɕύX
         //Excel_Cell_DataSet(112, 2,  P_PARTS[2]);  // ���i�P(���i�^��)
         int iTmp;
         iTmp = i_Base_Y + i_ErrItemCnt;
         Excel_Cell_DataSet(iTmp, 2,  P_PARTS[2]);  // ���i�P(���i�^��)
         Excel_Cell_DataSet(iTmp, 74,  P_PARTS[8]);  // �\���i��
         i_ErrItemCnt++;
        */
        // 2007.11.14 �R�����g�A�E�g
        //----------------------------------

        //----------------------------------
        // 2007.11.14 �ǉ�
         if( i_errflg == 0 )
         {
            // ܰ��i�[�ް�����?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {  // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // ���s
               P_Row++;
               Write_BorderLine();
            }

            i_errflg++;
            P_WkRec = 0;
         }

         Excel_Cell_DataSet(P_Row, 2,  P_PARTS[2]);  // ���i�P(���i�^��)
         Excel_Cell_DataSet(P_Row, 74,  P_PARTS[8]);  // �\���i��
         P_Row++;
        // 2007.11.14 �ǉ�
        //----------------------------------

      }

      RecNo++;
   }

   // ܰ��i�[�ް�����?
   if ( P_WkRec > 0 ){
      Output_WK_PARTS(b_YobiSumMode);
   }

   fclose(fp);    // ����
   return true;
}
//---------------------------------------------------------------------------
// ���{��֐����F �O�񕔕i�ް��擾����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �O�񕔕i�W�J�ް����m�ۂ���
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Prev_Data_Get( void )
{
   char  buf[1024];                            /* ������o�b�t�@   */
   char  *tok;                                 /* �g�[�N���|�C���^ */
   int   i, j;                                 /* ���[�v�J�E���^   */
   char  token[100][100];                      /* �g�[�N���o�b�t�@ */

   AnsiString wk_FilePass;  //
   AnsiString wk_RevPass;   // �O�񕔕i�W�J�ް��߽
   //AnsiString P_PREV_PARTS[15]; // �O�񕔕i�ް�

//   struct ffblk ffblk;
//   int done;
   int i_rev = -1;
   AnsiString sRevNo;

   //2023.03.02 �����ԍ��ǉ�_S
   AnsiString  Work,Rev;
   AnsiString xls_Output_Dir_old;  // �o�̓f�B���N�g��
   int i_Sakuban;
   //2023.03.02 �����ԍ��ǉ�_E

   //------------------------------
   // 2007.10.15
   bool bRet;
   AnsiString s_wkPARTS[1000][17];
   // 2007.10.15
   //------------------------------

   //***************************
   //**  �O�񕔕i�W�J�ް��Ǎ�
   //***************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0)
   {
      //�}���`�T�[�o�Ή� 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN);
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN);
      //�}���`�T�[�o�Ή� 2009/1/13 MOD END
   } else {
      //�}���`�T�[�o�Ή� 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN);
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN);
      //�}���`�T�[�o�Ή� 2009/1/13 MOD END
   }
   //2023.03.02 �����ԍ��ǉ�_S
   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   ////sRevNo = Csv_Exist_Check(wk_FilePass);
   //sRevNo = Csv_Exist_Check(G_xls_Output_Dir_old);
   ////*************************************

    //********************************
    //**  �o�̓f�B���N�g��
    //********************************
    if (G_SAKUBAN.Length() > 7) {
        Work = G_SAKUBAN.SubString(1, 6);
    } else {
        Work = G_SAKUBAN.SubString(1, 4);
    }
    i_Sakuban = Work.ToIntDef(0);
    if( i_Sakuban % 2 == 1 ){
        i_Sakuban --;
    }
    if (i_Sakuban > 9999) {
        Work.sprintf("%06d",i_Sakuban);
    } else {
        Work.sprintf("%04d",i_Sakuban);
    }

    if (G_HEADS_DATA_Rev - 1 <= 0 ) {
        Rev.sprintf("%02d", 0);
    }else{
        Rev.sprintf("%02d",G_HEADS_DATA_Rev - 1);
    }

	//2023/4/21 �����ԍ������ǉ��Ή� Mod
	//xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_DATA_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\";
	xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_DATA_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + GetFileSeizouBangou(G_SAKUBAN) + "\\";

    sRevNo = Csv_Exist_Check(xls_Output_Dir_old);
   //2023.03.02 �����ԍ��ǉ�_E
//   wk_RevPass = wk_FilePass + "*";

//   //****************************
//   //*** �O�񕔕i�W�J�f�[�^����
//   //****************************
//   done = findfirst( wk_RevPass.c_str(), &ffblk, FA_DIREC);
//   while (!done)
//   {
//      AnsiString wkFileName;
//
//      wkFileName = ffblk.ff_name;
//      if( wkFileName.SubString( 4, 2 ).ToIntDef(0) > i_rev )
//         i_rev = wkFileName.SubString( 4, 2 ).ToIntDef(0);
//
//      done = findnext(&ffblk);
//   }


   //------------------------------------
   // 2007.10.15 �װ�d�l�Ή� ���ʊ֐���

/*   //****************************
   //*** �O�񕔕i�W�J�f�[�^�Ǎ�
   //****************************
//   if( i_rev != -1 )
   if ( sRevNo != "" )
   {
//      wk_FilePass = wk_FilePass + FormatFloat( "0#", i_rev )
      wk_FilePass = wk_FilePass + sRevNo + "\\HD_PARTS.csv";

      if ((wkfp = fopen(wk_FilePass.c_str(),"r")) == NULL){
         // �װ۸ލ쐬
         G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
         Write_Error_Log(G_ErrLog_Text);
         fclose(wkfp);    // ����
         return false;
      }

      memset(buf,0x00,sizeof(buf));
      P_PrevRec = 0;
      while(fgets(buf,sizeof(buf),wkfp)!=NULL)
      {
         i = 0;                                 // �g�[�N���J�E���^������
         memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
         tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
         while(tok)
         {                            // �g�[�N�������݂����
            sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
            tok = GetToken(NULL,",");           // ���̃g�[�N��������

            if (i>100) {
               break;
            }
         }

         //*****************************
         // �O�񕔕i�f�[�^�̎擾
         // (�^���A���ʎq�ȊO�S�p)
         //*****************************
         for( j=0; j<16; j++ )
         {
            P_PREV_PARTS[P_PrevRec][j] = token[j];
            if( j > 1 )
               P_PREV_PARTS[P_PrevRec][j] = HanToZen(P_PREV_PARTS[P_PrevRec][j]);

            //******************************************
            // �z��̍Ō�ɓǍ����׸ޗp�ް����Z�b�g����
            //******************************************
            if( j == 15 )
               P_PREV_PARTS[P_PrevRec][j] = "0";
         }
         P_PrevRec++;
      }
   }
   else
   {
      return false;
   }

   return true;
*/

    if ( sRevNo != "" ){

        wk_FilePass = wk_FilePass + sRevNo + "\\HD_PARTS.csv";

        //****************************
        //*** �O�񕔕i�W�J�f�[�^�Ǎ�
        //****************************
        bRet = Read_Buhin_Data_Sorted(wk_FilePass, s_wkPARTS);
        if(!bRet) return(false);

        P_PrevRec = 0;

        for( i = 0; i < 1000; i++ ){
            if( s_wkPARTS[i][0] != "" ){
                for( j = 0; j < 17; j++ ){
                    // ���ʎq�ȊO�͑S�p�ϊ�
                    if( j > 1 ){
                        P_PREV_PARTS[i][j] = HanToZen(s_wkPARTS[i][j]);
                    }else{
                        P_PREV_PARTS[i][j] = s_wkPARTS[i][j];
                    }
                }

                //******************************************
                // �z��̍Ō�ɓǍ����׸ޗp�ް����Z�b�g����
                //******************************************
                P_PREV_PARTS[i][17] = "0";

                // �ް��s����
                P_PrevRec++;
            }else{
                break;
            }
        }
    }else{
        return(false);
    }

    return(true);

   // 2007.10.15 �װ�d�l�Ή�
   //------------------------------------
}

//---------------------------------------------------------------------------
// ���{��֐����F ��(�U��)�O�񕔕i�ް��擾����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �W���œ��͂��ꂽ�U������̍�Ԃɂ���
//                �U����Ԃ̑O�񕔕i�W�J�ް����m�ۂ���
//                2023.02.08 ��Ԍ����ǉ��Ή� (�V�K�쐬)
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Prev_Old_Data_Get( void )
{
    char  buf[1024];                            /* ������o�b�t�@   */
    char  *tok;                                 /* �g�[�N���|�C���^ */
    int   i, j;                                 /* ���[�v�J�E���^   */
    char  token[100][100];                      /* �g�[�N���o�b�t�@ */

    AnsiString wk_FilePass;  //
    AnsiString wk_RevPass;   // �O�񕔕i�W�J�ް��߽

    int i_rev = -1;
    AnsiString sRevNo;

    bool bRet;
    AnsiString s_wkPARTS[1000][17];

    AnsiString OLD_SAKUBAN;  // �U��������
    AnsiString  Work,Rev;
    AnsiString xls_Output_Dir_old;  // �U������o�̓f�B���N�g��
    int i_Sakuban;

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

    xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_DATA_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + OLD_SAKUBAN + "\\";

    //***************************
    //**  �O�񕔕i�W�J�ް��Ǎ�
    //***************************
    wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                + IncludeTrailingBackslash(G_SAKUBAN);

    sRevNo = Csv_Exist_Check(xls_Output_Dir_old);

    if ( sRevNo != "" ){

        wk_FilePass = wk_FilePass + sRevNo + "\\HD_PARTS.csv";

        //****************************
        //*** �O�񕔕i�W�J�f�[�^�Ǎ�
        //****************************
        bRet = Read_Buhin_Data_Sorted(wk_FilePass, s_wkPARTS);
        if(!bRet) return(false);

        P_PrevRec = 0;

        for( i = 0; i < 1000; i++ ){
            if( s_wkPARTS[i][0] != "" ){
                for( j = 0; j < 17; j++ ){
                    // ���ʎq�ȊO�͑S�p�ϊ�
                    if( j > 1 ){
                        P_PREV_PARTS[i][j] = HanToZen(s_wkPARTS[i][j]);
                    }else{
                        P_PREV_PARTS[i][j] = s_wkPARTS[i][j];
                    }
                }

                //******************************************
                // �z��̍Ō�ɓǍ����׸ޗp�ް����Z�b�g����
                //******************************************
                P_PREV_PARTS[i][17] = "0";

                // �ް��s����
                P_PrevRec++;
            }else{
                break;
            }
        }
    }else{
        return(false);
    }

    return(true);


}

//---------------------------------------------------------------------------
// ���{��֐����F ���i��r����
// �T  �v      �F
// ��  ��      �F AnsiString P_PARTS �F ��r�Ώۃf�[�^
// �߂�l      �F ��r���� [true:����Ȃ� false:���Ⴀ��]
// ��  �l      �F �O�񕔕i�W�J�f�[�^�ƍ���̃f�[�^���r����
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Compare_Data( AnsiString *P_PARTS )
{
   int i;   // ٰ�߶���
   bool bFound;

   bFound = false;
   for( i=0; i<P_PrevRec; i++ )
   {
      //*****************************************************************
      // �O�񕔕i�ް��ƍ��񕔕i�ް��̕��i����[1���]+���ʎq[2���]��v?
      //*****************************************************************
      //-----------------------------------------
      // 2007.10.12 �װ�d�l�Ή�
      //  �װ�d�l����[17���]����r�Ώۂɒǉ�

//      if( ( P_PARTS[0] == P_PREV_PARTS[i][0] )&&( P_PARTS[1] == P_PREV_PARTS[i][1] )
//        &&( P_PREV_PARTS[i][15] == "0" ) )
      if( ( P_PARTS[0] == P_PREV_PARTS[i][0] )&&( P_PARTS[1] == P_PREV_PARTS[i][1] )
        &&( P_PREV_PARTS[i][17] == "0" )&&( P_PARTS[16] == P_PREV_PARTS[i][16] ) )

      // 2007.10.12
      //-----------------------------------------
      {
         bFound = true;
         //**************************
         // �Y��ں��ނ�Ǎ��ς݂ɾ��
         //**************************
         //----------------------------
         // 2007.10.15
         //P_PREV_PARTS[i][15] = "1" ;
         P_PREV_PARTS[i][17] = "1" ;
         // 2007.10.15
         //----------------------------

         if ( P_PREV_PARTS[i][7] == "�O" ) P_PREV_PARTS[i][7] = "";
         if( P_PARTS[2]  != P_PREV_PARTS[i][2] ) return false;        // ���i�^��
         else if( P_PARTS[3]  != P_PREV_PARTS[i][3] ) return false;   // ���i����
         else if( P_PARTS[4]  != P_PREV_PARTS[i][4] ) return false;   // �ގ�����
         else if( P_PARTS[5]  != P_PREV_PARTS[i][5] ) return false;   // �}��
         else if( P_PARTS[6]  != P_PREV_PARTS[i][6] ) return false;   // ��}�p�}��
         else if( P_PARTS[7]  != P_PREV_PARTS[i][7] ) return false;   // 1�䕪����
         else if( P_PARTS[10] != P_PREV_PARTS[i][10] ) return false;  // �d�l1
         else if( P_PARTS[11] != P_PREV_PARTS[i][11] ) return false;  // �d�l2
         else return true;
      }
   }
   if ( bFound == false ) return false;

   return true;
}


//---------------------------------------------------------------------------
// ���{��֐����F ܰ����i�ް��o�͏���
// �T  �v      �F ܰ����i�z��Ɋi�[���ꂽ�ް����o�͂���
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F ɽ�ٕ��i�ް��ȍ~�Ŏg�p
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Output_WK_PARTS(bool bDataTyp)
{
   int i, j;
   AnsiString a_Bunrui, a_Zaisitu;
   AnsiString s_Text;
   bool b_GGPL;

   for( i=0; i<P_WkRec; i++)
   {
       b_GGPL = false;
      if( P_WK_PARTS[i][0] != "\0" )
      {
         a_Bunrui = P_WK_PARTS[i][0];
         a_Zaisitu = P_WK_PARTS[i][1];

         Excel_Cell_DataSet(P_Row, 4,  P_WK_PARTS[i][2]);  // ���i�P(���i�^��)
         if ( P_WK_PARTS[i][3].TrimRight() != "" )
           Excel_Cell_DataSet(P_Row, 13, P_WK_PARTS[i][3]);  // ���i�Q(���i����)
         Excel_Cell_DataSet(P_Row, 24, P_WK_PARTS[i][4]);  // �ގ�����
         Excel_Cell_DataSet(P_Row, 36, P_WK_PARTS[i][5]);  // �}��

         Excel_Cell_DataSet(P_Row, 47, P_WK_PARTS[i][10]); // �d�l1
         if ( P_WK_PARTS[i][11].Length() > 0 ) {
           if( P_WK_PARTS[i][1] == "GGPL--" ) {
             b_GGPL = true;
           } else {
             Excel_Cell_DataSet(P_Row, 68, P_WK_PARTS[i][11]); // �d�l2
           }
         }

         if ( P_WK_PARTS[i][7] == "0" || P_WK_PARTS[i][7] == "�O" ) {
            Excel_Cell_DataSet(P_Row, 71, "");  // 1�䕪����
         } else {
            Excel_Cell_DataSet(P_Row, 71, P_WK_PARTS[i][7]);  // 1�䕪����
         }
         if ( P_WK_PARTS[i][8] == "0" || P_WK_PARTS[i][8] == "�O" ) {
            Excel_Cell_DataSet(P_Row, 74, "");  // �\����
         } else {
            Excel_Cell_DataSet(P_Row, 74, P_WK_PARTS[i][8]);  // �\����
         }
         Excel_Cell_DataSet(P_Row, 78, P_WK_PARTS[i][9]);  // ���v
         Excel_Cell_DataSet(P_Row, 79, P_WK_PARTS[i][13]); // �P�ʁi���j
         if( P_compflg == 1 )
         {
            if( bDataTyp == false ) {
               if( !Compare_PrevData( P_WK_PARTS[i] ) )
                  Excel_Cell_DataSet(P_Row, 80, "*"); // �O���ް���r

            } else {
               if( !Compare_PrevData2( P_WK_PARTS[i] ) )
                  Excel_Cell_DataSet(P_Row, 80, "*"); // �O���ް���r
            }
         }

         //----------------------------------------
         // 2007.10.12 �װ�d�l�Ή� ���@���̏o��
         // 2007.11.02 �ǉ��d�l "EPGS-R"(E��ڰė����Ĕ�)��ǉ�
         if ( P_WK_PARTS[i][16] != "" ) {
            if( ( P_WK_PARTS[i][1] == "PLHL--" )
             || ( P_WK_PARTS[i][1] == "DPLG-A" )
             || ( P_WK_PARTS[i][1] == "DPLG-B" )
             || ( P_WK_PARTS[i][1] == "DPLGDA" )
             || ( P_WK_PARTS[i][1] == "DPLGDB" )
             || ( P_WK_PARTS[i][1] == "EPLGDA" )
             || ( P_WK_PARTS[i][1] == "EPLGDB" )
             || ( P_WK_PARTS[i][1] == "EPGS-R" )
             || ( P_WK_PARTS[i][1] == "FRAM-E" ) ){
                Excel_Cell_DataSet(P_Row, 66, P_WK_PARTS[i][16] + "���@");  // ���@����
            }
         }
         // 2007.10.12
         //----------------------------------------

         P_Row++;
         if ( b_GGPL == true ) {  // �Q�[�W�v���[�g�u�e�X�g��ō��v�����i�Ɉ�
           Excel_Cell_DataSet(P_Row, 68, P_WK_PARTS[i][11]); // �d�l2
           P_Row++;
         }
      }

      //*************************
      // �i�[��ܰ��ް���ر����
      //*************************
      //----------------------------------------
      // 2007.10.12 �װ�d�l�Ή�
      //for ( j=0; j<15; j++ )
      for ( j=0; j<17; j++ )
      // 2007.10.12
      //----------------------------------------
         P_WK_PARTS[i][j]="";
   }

   //if( ( a_Bunrui == "NOZZLE" )&&( a_Zaisitu == "TANKAN" ) )   02.09.27�ύX
   if( ( a_Bunrui == "NOZZLE" )&&( a_Zaisitu == "FRNG-" ) )
   {
      // ���L(�[�ǈʒu)
      s_Text = HanToZen( P_HEADS_DATA[1335] );
      Excel_Cell_DataSet(P_Row, 47, s_Text);
      P_Row++;
   }

   P_WkRec = 0;
}

//---------------------------------------------------------------------------
// ���{��֐����F ܰ����i�ް��o�͏���
// �T  �v      �F ܰ����i�z��Ɋi�[���ꂽ�ް����o�͂���
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F ɽ�ٕ��i�ް��ȍ~�Ŏg�p
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Output_WK_PARTS3(void)
{
   int i, j;
   AnsiString a_Bunrui, a_Zaisitu;
   AnsiString s_Text;

   for( i=0; i<P_WkRec3; i++)
   {
      if( P_WK_PARTS3[i][0] != "\0" )
      {
         a_Bunrui = P_WK_PARTS3[i][0];
         a_Zaisitu = P_WK_PARTS3[i][1];

         Excel_Cell_DataSet(P_Row, 4,  P_WK_PARTS3[i][2]);  // ���i�P(���i�^��)
         if ( P_WK_PARTS3[i][3].TrimRight() != "" )
           Excel_Cell_DataSet(P_Row, 13, P_WK_PARTS3[i][3]);  // ���i�Q(���i����)
         Excel_Cell_DataSet(P_Row, 24, P_WK_PARTS3[i][4]);  // �ގ�����
         Excel_Cell_DataSet(P_Row, 36, P_WK_PARTS3[i][5]);  // �}��

         Excel_Cell_DataSet(P_Row, 47, P_WK_PARTS3[i][10]); // �d�l1
         if ( P_WK_PARTS3[i][11].Length() > 0 ) {
           Excel_Cell_DataSet(P_Row, 68, P_WK_PARTS3[i][11]); // �d�l2
         }

         if (P_WK_PARTS3[i][7] == "0" || P_WK_PARTS3[i][7] == "�O" ) {
            Excel_Cell_DataSet(P_Row, 71, "");  // 1�䕪����
         } else {
            Excel_Cell_DataSet(P_Row, 71, P_WK_PARTS3[i][7]);  // 1�䕪����
         }
         if (P_WK_PARTS3[i][8] == "0" || P_WK_PARTS3[i][8] == "�O" ) {
            Excel_Cell_DataSet(P_Row, 74, "");  // �\����
         } else {
            Excel_Cell_DataSet(P_Row, 74, P_WK_PARTS3[i][8]);  // �\����
         }
         Excel_Cell_DataSet(P_Row, 78, P_WK_PARTS3[i][9]);  // ���v
         Excel_Cell_DataSet(P_Row, 79, P_WK_PARTS3[i][13]); // �P�ʁi���j
         if( P_compflg == 1 )
         {
           if( !Compare_PrevData2( P_WK_PARTS3[i] ) )
              Excel_Cell_DataSet(P_Row, 80, "*"); // �O���ް���r
         }

         //------------------------------------------
         // 2007.10.12 �װ�d�l�Ή� ���@���̒ǉ�
         // 2007.11.02 �ǉ��d�l "EPGS-R"(E��ڰė����Ĕ�)�ǉ�
         if (P_WK_PARTS3[i][16] != "" ) {
            if( ( P_WK_PARTS3[i][1] == "PLHL--" )
             || ( P_WK_PARTS3[i][1] == "DPLG-A" )
             || ( P_WK_PARTS3[i][1] == "DPLG-B" )
             || ( P_WK_PARTS3[i][1] == "DPLGDA" )
             || ( P_WK_PARTS3[i][1] == "DPLGDB" )
             || ( P_WK_PARTS3[i][1] == "EPLGDA" )
             || ( P_WK_PARTS3[i][1] == "EPLGDB" )
             || ( P_WK_PARTS3[i][1] == "EPGS-R" )
             || ( P_WK_PARTS3[i][1] == "FRAM-E" ) ){
                Excel_Cell_DataSet(P_Row, 66, P_WK_PARTS3[i][16] + "���@");  // ���@����
            }
         }
         // 2007.10.12
         //------------------------------------------

         P_Row++;
      }

      //*************************
      // �i�[��ܰ��ް���ر����
      //*************************
      //--------------------------
      // 2007.10.12 �װ�d�l�Ή�
      //for ( j=0; j<15; j++ )
      for ( j=0; j<17; j++ )
      // 2007.10.12
      //--------------------------
         P_WK_PARTS3[i][j]="";
   }

   P_WkRec3 = 0;
}

//---------------------------------------------------------------------------
// ���{��֐����F ���i�ް��W�v����
// �T  �v      �F ���ꕔ�i�̐��ʁA�\�����A���v���W�v����
// ��  ��      �F AnsiString *P_PARTS �F�W�v�Ώ�ں���
// �߂�l      �F �Ȃ�
// ��  �l      �F ɽ�ٕ��i�ް��ȍ~�Ŏg�p
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Buhin_Syukei( AnsiString *P_PARTS )
{
   int i, j;
   int i_matchflg =0;

   //****************************************
   // ���Ɋi�[����Ă��镔�i�ް��Ɣ�r
   //****************************************
   for( i=0; i<P_WkRec; i++ )
   {
      //03.12.22 ��r�����ύX
      //if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[3] == P_WK_PARTS[i][3] )
      //  &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
      //  &&( P_PARTS[12] == P_WK_PARTS[i][12] )&&( P_PARTS[13] == P_WK_PARTS[i][13] ) )
      //{
      //---------------------------------------
      // 2007.10.12 �װ�d�l�Ή� �װ�d�l����[16]����r
      //if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[3] == P_WK_PARTS[i][3] )
      //  &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
      //  &&( P_PARTS[10] == P_WK_PARTS[i][10] )&&( P_PARTS[11] == P_WK_PARTS[i][11] )
      //  &&( P_PARTS[12] == P_WK_PARTS[i][12] )&&( P_PARTS[13] == P_WK_PARTS[i][13] ) )
      //{
      if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[3] == P_WK_PARTS[i][3] )
        &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
        &&( P_PARTS[10] == P_WK_PARTS[i][10] )&&( P_PARTS[11] == P_WK_PARTS[i][11] )
        &&( P_PARTS[12] == P_WK_PARTS[i][12] )&&( P_PARTS[13] == P_WK_PARTS[i][13] )
        &&( P_PARTS[16] == P_WK_PARTS[i][16] ) )
      {
      // 2007.10.12
      //---------------------------------------

      //*********************
         // �P�䕪���� �W�v
         P_WK_PARTS[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][7]).ToIntDef(0)
                                              + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
         // �\���� �W�v
         P_WK_PARTS[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][8]).ToIntDef(0)
                                              + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
         // ���v
         P_WK_PARTS[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][9]).ToIntDef(0)
                                              + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
         i_matchflg = 1;

      }
   }

   //***************************************
   // �Y���������ʂȂ��̏ꍇܰ����i�ް��Ɋi�[
   //***************************************
   if( i_matchflg == 0 )
   {
      //----------------------------------------------
      // 2007.10.12 �װ�d�l�Ή� �װ�d�l����[16]���i�[
      //for( j=0; j<15; j++ )
      for( j=0; j<17; j++ )
      // 2007.10.12
      //----------------------------------------------
         P_WK_PARTS[P_WkRec][j] = P_PARTS[j];
   }

   //i_matchflg = 0;
   P_WkRec++;
}

//��2002/10/24
//---------------------------------------------------------------------------
// ���{��֐����F ���i�ް��W�v����2
// �T  �v      �F ���ꕔ�i�̐��ʁA�\�����A���v���W�v����
// ��  ��      �F AnsiString *P_PARTS �F�W�v�Ώ�ں���
// �߂�l      �F �Ȃ�
// ��  �l      �F ɽ�ٕ��i�ް��ȍ~�Ŏg�p
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Buhin_Syukei2( AnsiString *P_PARTS )
{
   int i, j;
   int i_matchflg =0;

   if( ZenToHan( P_PARTS[7] )!= "" ) {
     //****************************************
     // ���Ɋi�[����Ă��镔�i�ް��Ɣ�r
     //****************************************
     for( i=0; i<P_WkRec; i++ )
     {
     //-------------------------------------------------
     // 2007.10.12 �װ�d�l�Ή� �װ�d�l����[16]����r
       //if( ( P_PARTS[1] == P_WK_PARTS[i][1] ) // &&( P_PARTS[2] == P_WK_PARTS[i][2] )
       //  &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] ) )
       if( ( P_PARTS[1] == P_WK_PARTS[i][1] ) // &&( P_PARTS[2] == P_WK_PARTS[i][2] )
         &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
         &&( P_PARTS[16] == P_WK_PARTS[i][16] ))
     // 2007.10.12
     //-------------------------------------------------
       {
          // �P�䕪���� �W�v
          P_WK_PARTS[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][7]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
          // �\���� �W�v
          P_WK_PARTS[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][8]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
          // ���v
          P_WK_PARTS[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][9]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
          P_WK_PARTS[i][13] = P_PARTS[13];
          i_matchflg = 1;

       }
     }
     //***************************************
     // �Y���������ʂȂ��̏ꍇܰ����i�ް��Ɋi�[
     //***************************************
     if( i_matchflg == 0 )
     {
        //----------------------------------------------
        // 2007.10.12 �װ�d�l�Ή� �װ�d�l����[16]���i�[
        //for( j=0; j<15; j++ )
        for( j=0; j<17; j++ )
        // 2007.10.12
        //----------------------------------------------
           P_WK_PARTS[P_WkRec][j] = P_PARTS[j];
     }

     //i_matchflg = 0;
     P_WkRec++;
   }
}

//---------------------------------------------------------------------------
// ���{��֐����F ���i�ް��W�v����3
// �T  �v      �F ���ꕔ�i�̐��ʁA�\�����A���v���W�v����
// ��  ��      �F AnsiString *P_PARTS �F�W�v�Ώ�ں���
// �߂�l      �F �Ȃ�
// ��  �l      �F ɽ�ٕ��i�ް��ȍ~�Ŏg�p
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Buhin_Syukei3( AnsiString *P_PARTS )
{
   int i, j;
   int i_matchflg =0;

   if( ZenToHan( P_PARTS[7] )!= "" ) {
     //****************************************
     // ���Ɋi�[����Ă��镔�i�ް��Ɣ�r
     //****************************************
     for( i=0; i<P_WkRec3; i++ )
     {
       if( ( P_PARTS[1] == P_WK_PARTS3[i][1] ) // &&( P_PARTS[2] == P_WK_PARTS[i][2] )
         &&( P_PARTS[4] == P_WK_PARTS3[i][4] )&&( P_PARTS[5] == P_WK_PARTS3[i][5] ) )
       {
          // �P�䕪���� �W�v
          P_WK_PARTS3[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS3[i][7]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
          // �\���� �W�v
          P_WK_PARTS3[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS3[i][8]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
          // ���v
          P_WK_PARTS3[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS3[i][9]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
          P_WK_PARTS3[i][13] = P_PARTS[13];
          i_matchflg = 1;

       }
     }
     //***************************************
     // �Y���������ʂȂ��̏ꍇܰ����i�ް��Ɋi�[
     //***************************************
     if( i_matchflg == 0 )
     {
        //---------------------------
        // 2007.10.15 �װ�d�l�Ή�
        //for( j=0; j<15; j++ )
        for( j=0; j<17; j++ )
        // 2007.10.15
        //---------------------------
           P_WK_PARTS3[P_WkRec3][j] = P_PARTS[j];
     }

     //i_matchflg = 0;
     P_WkRec3++;
   }
}

//2003.12.22 �֐��ǉ�
//---------------------------------------------------------------------------
// ���{��֐����F ���i�ް��W�v����4
// �T  �v      �F ���ꕔ�i�̐��ʁA�\�����A���v���W�v����
// ��  ��      �F AnsiString *P_PARTS �F�W�v�Ώ�ں���
// �߂�l      �F �Ȃ�
// ��  �l      �F ɽ�ٕ��i�p
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Buhin_Syukei4( AnsiString *P_PARTS )
{
   int i, j;
   int i_matchflg =0;

   if( ZenToHan( P_PARTS[7] )!= "" ) {
     //****************************************
     // ���Ɋi�[����Ă��镔�i�ް��Ɣ�r
     //****************************************
     for( i=0; i<P_WkRec; i++ )
     {
       // 2017.01.12 �[�ǏW�v�ύX_S
       //if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[4] == P_WK_PARTS[i][4] )
       //  &&( P_PARTS[5] == P_WK_PARTS[i][5] )&&( P_PARTS[10] == P_WK_PARTS[i][10] )
       //  &&( P_PARTS[11] == P_WK_PARTS[i][11] ) )
       //{
       //   // �P�䕪���� �W�v
       //   P_WK_PARTS[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][7]).ToIntDef(0)
       //                                        + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
       //   // �\���� �W�v
       //   P_WK_PARTS[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][8]).ToIntDef(0)
       //                                        + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
       //   // ���v
       //   P_WK_PARTS[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][9]).ToIntDef(0)
       //                                        + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
       //   P_WK_PARTS[i][13] = P_PARTS[13];
       //   i_matchflg = 1;
       //
       //}
       if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[2] == P_WK_PARTS[i][2] )
         &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
         &&( P_PARTS[10] == P_WK_PARTS[i][10] )&&( P_PARTS[11] == P_WK_PARTS[i][11] ) )
       {
          // �P�䕪���� �W�v
          P_WK_PARTS[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][7]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
          // �\���� �W�v
          P_WK_PARTS[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][8]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
          // ���v
          P_WK_PARTS[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][9]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
          P_WK_PARTS[i][13] = P_PARTS[13];
          i_matchflg = 1;

       }
       // 2017.01.12 �[�ǏW�v�ύX_E
     }
     //***************************************
     // �Y���������ʂȂ��̏ꍇܰ����i�ް��Ɋi�[
     //***************************************
     if( i_matchflg == 0 )
     {
        //---------------------------
        // 2007.10.15 �װ�d�l�Ή�
        //for( j=0; j<15; j++ )
        for( j=0; j<17; j++ )
        // 2007.10.15
        //---------------------------
           P_WK_PARTS[P_WkRec][j] = P_PARTS[j];
     }

     //i_matchflg = 0;
     P_WkRec++;
   }
}
//*******************

//---------------------------------------------------------------------------
// ���{��֐����F ���i��r����
// �T  �v      �F
// ��  ��      �F AnsiString P_PARTS �F ��r�Ώۃf�[�^
// �߂�l      �F ��r���� [true:����Ȃ� false:���Ⴀ��]
// ��  �l      �F �O�񕔕i�W�J�f�[�^�ƍ���̃f�[�^���r����  �W�v�^�C�v�f�[�^�p
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Compare_PrevData( AnsiString *P_PARTS )
{
   int i, j;
   //AnsiString P_WKPre[16];
   AnsiString P_WKPre[18];

   //****************************************
   // ���Ɋi�[����Ă��镔�i�ް��Ɣ�r
   //****************************************
   for( i=0; i<P_PrevRec; i++ )
   {
      //-----------------------------------------
      // 2007.10.12 �װ�d�l�Ή�
      //  �װ�d�l����[17���]����r�Ώۂɒǉ�

      //if( ( P_PARTS[0]  == P_PREV_PARTS[i][0]  )&&( P_PARTS[1]  == P_PREV_PARTS[i][1]  )&&( P_PARTS[3]  == P_PREV_PARTS[i][3]  )
      //  &&( P_PARTS[4]  == P_PREV_PARTS[i][4]  )&&( P_PARTS[5]  == P_PREV_PARTS[i][5]  )
      //  &&( P_PARTS[12] == P_PREV_PARTS[i][12] )&&( P_PARTS[13] == P_PREV_PARTS[i][13] ) )

      if( ( P_PARTS[0]  == P_PREV_PARTS[i][0]  )&&( P_PARTS[1]  == P_PREV_PARTS[i][1]  )&&( P_PARTS[3]  == P_PREV_PARTS[i][3]  )
        &&( P_PARTS[4]  == P_PREV_PARTS[i][4]  )&&( P_PARTS[5]  == P_PREV_PARTS[i][5]  )
        &&( P_PARTS[12] == P_PREV_PARTS[i][12] )&&( P_PARTS[13] == P_PREV_PARTS[i][13] )&&( P_PARTS[16] == P_PREV_PARTS[i][16] ))
      // 2007.10.12
      //-----------------------------------------
      {
         //--------------------------
         // 2007.10.15
         //for( j=0; j<15; j++ ) {
         for( j=0; j<18; j++ ) {
         // 2007.10.15
         //--------------------------

           if ( j < 7 || j > 9 )
             P_WKPre[j] = P_PREV_PARTS[i][j];
         }
         // �P�䕪���� �W�v
         P_WKPre[7] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][7]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[7]).ToIntDef(0) ) );
         // �\���� �W�v
         P_WKPre[8] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][8]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[8]).ToIntDef(0) ) );
         // ���v
         P_WKPre[9] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][9]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[9]).ToIntDef(0) ) );
      }
   }
   if (P_WKPre[7] == "�O" ) P_WKPre[7] = "";
   if (P_WKPre[8] == "�O" ) P_WKPre[8] = "";
   if (P_PARTS[7] == "�O" ) P_PARTS[7] = "";
   if (P_PARTS[8] == "�O" ) P_PARTS[8] = "";

   //*****************************************************************
   // �O�񕔕i�ް��ƍ��񕔕i�ް��̕��i����[1���]+���ʎq[2���]��v?
   //*****************************************************************
   if(      P_PARTS[2]  != P_WKPre[2]  ) return false;  // ���i�^��
   else if( P_PARTS[3]  != P_WKPre[3]  ) return false;  // ���i����
   else if( P_PARTS[4]  != P_WKPre[4]  ) return false;  // �ގ�����
   else if( P_PARTS[5]  != P_WKPre[5]  ) return false;  // �}��
   else if( P_PARTS[6]  != P_WKPre[6]  ) return false;  // ��}�p�}��
   else if( P_PARTS[7]  != P_WKPre[7]  ) return false;  // 1�䕪����
   else if( P_PARTS[8]  != P_WKPre[8]  ) return false;  //
   else if( P_PARTS[9]  != P_WKPre[9]  ) return false;  //
   else if( P_PARTS[10]  != P_WKPre[10]  ) return false;  //
   else if( P_PARTS[11]  != P_WKPre[11]  ) return false;  //
   else return true;

}
//---------------------------------------------------------------------------
// ���{��֐����F ���i��r����
// �T  �v      �F
// ��  ��      �F AnsiString P_PARTS �F ��r�Ώۃf�[�^
// �߂�l      �F ��r���� [true:����Ȃ� false:���Ⴀ��]
// ��  �l      �F �O�񕔕i�W�J�f�[�^�ƍ���̃f�[�^���r����  �W�v�^�C�v�f�[�^�p
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Compare_PrevData2( AnsiString *P_PARTS )
{
   int i, j;
   //AnsiString P_WKPre[16];
   AnsiString P_WKPre[18];

   //****************************************
   // ���Ɋi�[����Ă��镔�i�ް��Ɣ�r
   //****************************************
   for( i=0; i<P_PrevRec; i++ )
   {
      //-----------------------------------------
      // 2007.10.12 �װ�d�l�Ή�
      //  �װ�d�l����[17���]����r�Ώۂɒǉ�
      //if( ( P_PARTS[0]  == P_PREV_PARTS[i][0]  )&&( P_PARTS[1]  == P_PREV_PARTS[i][1]  )
      //  &&( P_PARTS[4]  == P_PREV_PARTS[i][4]  )&&( P_PARTS[5]  == P_PREV_PARTS[i][5]  ) )
      if( ( P_PARTS[0]  == P_PREV_PARTS[i][0]  )&&( P_PARTS[1]  == P_PREV_PARTS[i][1]  )
        &&( P_PARTS[4]  == P_PREV_PARTS[i][4]  )&&( P_PARTS[5]  == P_PREV_PARTS[i][5]  )
        &&( P_PARTS[16]  == P_PREV_PARTS[i][16]  ))
      // 2007.10.12
      //-----------------------------------------
      {
         //---------------------------
         // 2007.10.15
         //for( j=0; j<15; j++ ) {
         for( j=0; j<18; j++ ) {
         // 2007.10.15
         //---------------------------

           if ( j < 7 || j > 9 )
             P_WKPre[j] = P_PREV_PARTS[i][j];
         }
         // �P�䕪���� �W�v
         P_WKPre[7] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][7]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[7]).ToIntDef(0) ) );
         // �\���� �W�v
         P_WKPre[8] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][8]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[8]).ToIntDef(0) ) );
         // ���v
         P_WKPre[9] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][9]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[9]).ToIntDef(0) ) );
      }
   }
   if (P_WKPre[7] == "�O" ) P_WKPre[7] = "";
   if (P_WKPre[8] == "�O" ) P_WKPre[8] = "";
   if (P_PARTS[7] == "�O" ) P_PARTS[7] = "";
   if (P_PARTS[8] == "�O" ) P_PARTS[8] = "";

   //*****************************************************************
   // �O�񕔕i�ް��ƍ��񕔕i�ް��̕��i����[1���]+���ʎq[2���]��v?
   //*****************************************************************
   if(      P_PARTS[2]  != P_WKPre[2]  ) return false;  // ���i�^��
   else if( P_PARTS[3]  != P_WKPre[3]  ) return false;  // ���i����
   else if( P_PARTS[4]  != P_WKPre[4]  ) return false;  // �ގ�����
   else if( P_PARTS[5]  != P_WKPre[5]  ) return false;  // �}��
   else if( P_PARTS[6]  != P_WKPre[6]  ) return false;  // ��}�p�}��
   else if( P_PARTS[7]  != P_WKPre[7]  ) return false;  // 1�䕪����
   else if( P_PARTS[8]  != P_WKPre[8]  ) return false;  //
   else if( P_PARTS[9]  != P_WKPre[9]  ) return false;  //
   else if( P_PARTS[10]  != P_WKPre[10]  ) return false;  //
   else if( P_PARTS[11]  != P_WKPre[11]  ) return false;  //
   else return true;

}

//��2002/10/24

//---------------------------------------------------------------------------
// ���{��֐����F ���L�o��
// �T  �v      �F �Ō�ɒ��L���o�͂���
// ��  ��      �F AnsiString *P_PARTS �F�W�v�Ώ�ں���
// �߂�l      �F �Ȃ�
// ��  �l      �F ɽ�ٕ��i�ް��ȍ~�Ŏg�p
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Cyuki_Out( void )
{
   int  iOutCnt;
   // 2020.07.17 �ی�J�o�[���L�ǉ�_S
   AnsiString s_Text;            // �ėp������
   // 2020.07.17 �ی�J�o�[���L�ǉ�_E
   iOutCnt=0;

   if ( P_HEADS_DATA[1335].TrimRight() != "" ) iOutCnt++;  //�@�[�ǈʒu
   if ( P_HEADS_DATA[1302].TrimRight() != "" ) iOutCnt++;  //�@�֖��P
   if ( P_HEADS_DATA[1320].TrimRight() != "" ) iOutCnt++;  //�@�t���[���ގ�
   if ( P_HEADS_DATA[1365].TrimRight() != "" ) iOutCnt++;  //�@�t���[���d��
   if ( P_HEADS_DATA[1304].TrimRight() != "" ) iOutCnt++;  //�@�d������
   if ( P_HEADS_DATA[1325].TrimRight() != "" ) iOutCnt++;  //�@�m�Y���P
   if ( P_HEADS_DATA[1326].TrimRight() != "" ) iOutCnt++;  //�@�m�Y���Q
   if ( P_HEADS_DATA[1314].TrimRight() != "" ) iOutCnt++;  //�@�{���g�ގ�
   if ( P_HEADS_DATA[1315].TrimRight() != "" ) iOutCnt++;  //�@�{���g�d��P
   //2004.01.07 ���L�폜
   //if ( P_HEADS_DATA[1322].TrimRight() != "" ) iOutCnt++;  //�@�A���J�[�ގ�
   //*******************
   if ( P_HEADS_DATA[1323].TrimRight() != "" ) iOutCnt++;  //�@�A���J�[�d��
   //2004.03.15 ���L�ǉ�
   if ( P_HEADS_DATA[1018].TrimRight() != "" ) iOutCnt++;  //�@�{���g�����~��
   //*******************
   // 2020.07.17 �ی�J�o�[���L�ǉ�_S
   if (Tokki_Code("1E") == true)                 iOutCnt++;  //�@�ی�J�o�[
   // 2020.07.17 �ی�J�o�[���L�ǉ�_E

   //2003.11.14 ���y�[�W�����ύX
   if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - ( iOutCnt + 3 ) )) {
     // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s
     // ( iOutCnt + 3 ):�\�z�����o�͍s��
   //if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {  // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):�y�[�W�̍ŏI�s�@8:�\�z�����o�͍s��
      P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
      P_PageNum++;
   } else {
      P_Row++;
      P_Row++;
      P_Row++;
   //   P_Row++;
   //***************************
   }
   if ( P_HEADS_DATA[1335].TrimRight() != "" ) {  //�@�[�ǈʒu
      Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1335].TrimRight());
      P_Row++;
   }
   if ( P_HEADS_DATA[1302].TrimRight() != "" ) {  //�@�֖��P
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1302].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1320].TrimRight() != "" ) {  //�@�t���[���ގ�
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1320].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1365].TrimRight() != "" ) {  //�@�t���[���d��
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1365].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1304].TrimRight() != "" ) {  //�@�d������
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1304].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1325].TrimRight() != "" ) {  //�@�m�Y���P
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1325].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1326].TrimRight() != "" ) {  //�@�m�Y���Q
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1326].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1314].TrimRight() != "" ) {  //�@�{���g�ގ�
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1314].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1315].TrimRight() != "" ) {  //�@�{���g�d��P
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1315].TrimRight());
     P_Row++;
   }
   //2004.01.07 ���L�폜
   //if ( P_HEADS_DATA[1322].TrimRight() != "" ) {  //�@�A���J�[�ގ�
   //  Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1322].TrimRight());
   //  P_Row++;
   //}
   //*******************
   if ( P_HEADS_DATA[1323].TrimRight() != "" ) {  //�@�A���J�[�d��
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1323].TrimRight());
     P_Row++;
   }
   //2004.03.15 ���L�ǉ�
   if ( P_HEADS_DATA[1018].TrimRight() != "" ) {  //�@�{���g�����~��
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1018].TrimRight());
     P_Row++;
   }
   //*******************

   // 2020.07.31 �ی�J�o�[���L�ǉ�_S
   if (Tokki_Code("1E") == true) {
      if(P_HEADS_DATA[36].Pos("M") == 0)  {
         // ���p�łȂ��ꍇ
         s_Text = "�ی�J�o�[�v";
      } else {
         // ���p�̏ꍇ
         s_Text = "���ݗp�ی�J�o�[�v";
      }
      Excel_Cell_DataSet(P_Row, 3, s_Text );
      P_Row++;
   }
   // 2020.07.31 �ی�J�o�[���L�ǉ�_E

}


//---------------------------------------------------------------------------
// ���{��֐����F ���L�o��
// �T  �v      �F �Ō�ɒ��L���o�͂���
// ��  ��      �F AnsiString *P_PARTS �F�W�v�Ώ�ں���
// �߂�l      �F �Ȃ�
// ��  �l      �F ɽ�ٕ��i�ް��ȍ~�Ŏg�p
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::BLT_Cover_Out( void )
{
   int          i_RecNo;
   int          i_ErrFlg;
   int          i_Value_1;
   int          i_Value_2;
   int          i_Daisuu;
   AnsiString   s_Text;
   //***************************
   //***  �{���g�J�o�[
   //***************************
   // �wC018�x
   i_RecNo = 1092;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text != "0"){
      i_RecNo = 1096;
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text != "0"){
         s_Text = "���t�{���g�J�o�[";
         Excel_Cell_DataSet(P_Row, 5, "���t�{���g�J�o�[");
      } else {
        return;
      }
   } else {
        return;
   }

   G_Log_Text = "�{���g�J�o�[         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //**********************************************************
   //***  ���Ė{��L,S�@���l����
   //***    �wC019�x,�wC020�x,�wC021�x�ׂ̈̏����O����
   //**********************************************************
   i_ErrFlg = 0;
   s_Text = P_HEADS_DATA[39];
   i_Daisuu = s_Text.ToIntDef(0);

   // ���Ė{��L
   i_RecNo = 1286;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
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
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
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
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "���t�{���g�J�o�[");
         // BLCP1 �wC019�x
         s_Text = s_Text + "L";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text)); // �d�l�P
         G_Log_Text = "BLCP1                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

         // 2014/08/25 ���p�Z�p�ҏW
         // BLCP1 �d�l�Q
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCP1�{�� �wC020�x
         s_Text = AnsiString(i_Value_1 + i_Value_2);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // �\����
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // ���v
         G_Log_Text = "BLCP1 �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

      }
   }

   P_Row++;
   //***************************
   //***  BLCEL
   //***************************
   // �wC021�x,�wC022�x
   if (i_ErrFlg == 0){
      i_RecNo = 1096;      // BLCEL
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "���t�{���g�J�o�[");
         // BLCEL �wC021�x
         s_Text = s_Text + "L �L���b�v�t";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text));
         G_Log_Text = "BLCEL                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

         // 2014/08/25 ���p�Z�p�ҏW
         // BLCEL �d�l�Q
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCEL�{�� �wC022�x
         s_Text = AnsiString(i_Value_1);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // �\����
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // ���v
         G_Log_Text = "BLCEL �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

      }
   }
   P_Row++;

   //***************************
   //***  BLCES
   //***************************
   // �wC023�x,�wC024�x
   if (i_ErrFlg == 0){
      i_RecNo = 1097;      // BLCES
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "���t�{���g�J�o�[");
         // BLCES �wC023�x
         s_Text = s_Text + "L �L���b�v�t";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text));
         G_Log_Text = "BLCES                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

         // 2014/08/25 ���p�Z�p�ҏW
         // BLCES �d�l�Q
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCES�{�� �wC024�x
         s_Text = AnsiString(i_Value_2);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // �\����
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // ���v
         G_Log_Text = "BLCES �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

      }
   }

   P_Row++;
}
//---------------------------------------------------------------------------
// ���{��֐����F ���L�o��(���a�{���g�J�o�[)
// �T  �v      �F BLCP1S��BLCESS���o�͂���B
// ��  ��      �F
// �߂�l      �F �Ȃ�
// ��  �l      �F BLT_Cover_Out(void)�𓥏P���č쐬�B
//---------------------------------------------------------------------------
// 2014/08/25 ���p�Z�p�ҏW
void __fastcall TBuhinhyou::BLT_Cover_Out_Small( void )
{
   int          i_RecNo;
   int          i_ErrFlg;
   int          i_Value;
   int          i_Daisuu;
   AnsiString   s_Text;

   //***************************
   //***  �{���g�J�o�[
   //***************************
   i_RecNo = 1111;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text != "0"){
      i_RecNo = 1112;
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text != "0"){
         s_Text = "���t�{���g�J�o�[";
         Excel_Cell_DataSet(P_Row, 5, "���t�{���g�J�o�[");
      } else {
        return;
      }
   } else {
        return;
   }

   G_Log_Text = "���a�{���g�J�o�[         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //**********************************************************
   //***  ���Ė{�����l����
   //**********************************************************
   i_ErrFlg = 0;
   s_Text = P_HEADS_DATA[39];
   i_Daisuu = s_Text.ToIntDef(0);

   // ���Ė{��S
   i_RecNo = 1113;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      i_Value = s_Text.ToIntDef(0);   // ���Ė{��S
   } else {
      G_ErrLog_Text = "���a���Ė{���r      RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�

      i_ErrFlg = 1;
   }

   //***************************
   //***  BLCP1S
   //***************************
   if (i_ErrFlg == 0){
      i_RecNo = 1111;      // BLCP1S
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "���t�{���g�J�o�[");
         // BLCP1S
         s_Text = s_Text + "L";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text)); // �d�l�P
         G_Log_Text = "BLCP1S                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

         // BLCP1S �d�l�Q
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCP1S�{��
         s_Text = AnsiString(i_Value);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // �\����
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // ���v
         G_Log_Text = "BLCP1S �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      }
   }

   P_Row++;
   //***************************
   //***  BLCESS
   //***************************
   if (i_ErrFlg == 0){
      i_RecNo = 1112;      // BLCESS
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "���t�{���g�J�o�[");
         // BLCESS
         s_Text = s_Text + "L �L���b�v�t";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text));
         G_Log_Text = "BLCESS                �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);

         // BLCESS �d�l�Q
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCESS�{��
         s_Text = AnsiString(i_Value);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // �\����
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // ���v
         G_Log_Text = "BLCESS �{��           �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      }
   }

   P_Row++;
}
//-------------------------------------------------------------------------------------
// ���{��֐����F �ߋ�Excel��ā@��������
// �T  �v      �F Rev*���ިڸ�ؖ����������A�w�����̑��݂���ő����޼ޮݔԍ����擾
// ��  ��      �F
// �߂�l      �F �w�����̑��݂��钼�߂���޼ޮݔԍ�
// ��  �l      �F
//-------------------------------------------------------------------------------------
AnsiString __fastcall TBuhinhyou::Csv_Exist_Check(AnsiString path){
   AnsiString RevNo;
   AnsiString NowRev;
   AnsiString SheetPass;
   bool       bFind;

   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   //NowRev = P_HEADS_DATA[319].TrimRight();
   NowRev = G_HEADS_DATA_Rev;
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
   while(RevNo.ToIntDef(0) >= 0 && bFind == false) {
      // ̧�ّ�������
      SheetPass = path + RevNo + "\\HD_PARTS.csv";
      // 2023.02.08 ���O�ǉ�_S
      G_Log_Text = "SheetPass?�w " + SheetPass + " �x";
      Write_Log(G_Log_Text);
      // 2023.02.08 ���O�ǉ�_E
      if (FileExists(SheetPass.c_str()) == true){
        bFind = true;
      } else {
        RevNo = FormatFloat("00",RevNo.ToIntDef(0) - 1);
      }
   }
   if ( bFind == false ) {
     RevNo = "";
   }
   return RevNo;
}
//��2002/10/24

//��2003/01/21
//-------------------------------------------------------------------------------------
// ���{��֐����F �u���b�N���Ƀn�C�t���ŋ[���r�����o��
// �T  �v      �F
// ��  ��      �F
// �߂�l      �F
// ��  �l      �F
//-------------------------------------------------------------------------------------
void __fastcall TBuhinhyou::Write_BorderLine(void){
//   Excel_Cell_DataSet(P_Row,  3, "-------------------------------------------------");
//   Excel_Cell_DataSet(P_Row, 24, "----------------------------");
//   Excel_Cell_DataSet(P_Row, 36, "--------------------------");
//   Excel_Cell_DataSet(P_Row, 47, "----------------------------------------------------");
//   Excel_Cell_DataSet(P_Row, 71, "------");
//   Excel_Cell_DataSet(P_Row, 74, "------");
//   Excel_Cell_DataSet(P_Row, 78, "----------");
   Excel_Cell_DataSet(P_Row,  3, "-------------------------------------------------------------------------");
   Excel_Cell_DataSet(P_Row, 24, "--------------------------------------------");
   Excel_Cell_DataSet(P_Row, 36, "------------------------------------------");
   Excel_Cell_DataSet(P_Row, 47, "--------------------------------------------------------------------------");
   Excel_Cell_DataSet(P_Row, 71, "---------");
   Excel_Cell_DataSet(P_Row, 74, "----------");
   Excel_Cell_DataSet(P_Row, 78, "--------------");
   P_Row++;
}
//��2003/01/21



//---------------------------------------------------------------------------
// ���{��֐����F ���i�W�J�ް��Ǎ�(���,ϰ�ޏ����t)
// �T  �v      �F �����œn���ꂽ���i�W�J�ް�̧��(HD_PARTS.csv)��ǂݍ���
//                ��āAϰ�ތ�ɁA������z��ɃZ�b�g����B
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �װA�d�l���װB�d�l�œ��ް�������ꍇ��ϰ��,��Ă��s��
//                ��̧�ق̑O�����
//                  (1)�ް��́A�ȉ��̂����ꂩ�ō\������邱��
//                  �@����d�l�݂̂��ް�
//                  �@��װA�d�l���װB�d�l���ް�
//                  (2)�װA�d�l���ް�(�S�s)�̌���װB�d�l���ް�(�S�s)���o�͂���Ă��邱��
//
//                2007.10.12 �װ�d�l�Ή� (�V�K�쐬)
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Read_Buhin_Data_Sorted(AnsiString filepass, AnsiString s_DATA[][17])
{
    char  buf[1024];                            /* ������o�b�t�@   */
    char  *tok;                                 /* �g�[�N���|�C���^ */
    int   i, j, k;                                 /* ���[�v�J�E���^   */
    char  token[100][100];                      /* �g�[�N���o�b�t�@ */
    int   iDataNum;
    int   iDataIdx;
    AnsiString s_wkData[1000][17];
    bool bSearch;
    AnsiString s_LineData[17];

    AnsiString wk_FilePass;
    FILE *fp;

    AnsiString s_sortDATA[1000][17];
    int iRow;

    // 2007.11.08 ��ڰ��ް��A�E���ް��p
    AnsiString s_wkPlData[50][17];
    AnsiString s_wkTmpPlData[50][18];
    AnsiString s_wkTmpHlData[50][18];
    int iTmpPlDataNum;
    int iTmpHlDataNum;

    iDataNum = 0;
    iTmpPlDataNum = 0;
    iTmpHlDataNum = 0;

    memset(s_wkData,0x00,sizeof(s_wkData));
    memset(s_wkPlData,0x00,sizeof(s_wkPlData));
    memset(s_wkTmpPlData,0x00,sizeof(s_wkTmpPlData));
    memset(s_wkTmpHlData,0x00,sizeof(s_wkTmpHlData));

    //*******************************
    //***  HEADS�f�[�^�@�Ǎ�
    //*******************************
    wk_FilePass = filepass;
    if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
        fclose(fp);    // ����
        return false;
    }

    //*****************
    // �Ǎ��݉\�̊�
    //*****************
    memset(buf,0x00,sizeof(buf));
    while(fgets(buf,sizeof(buf),fp)!=NULL){
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

        // 1�s�ް��z��Ɋi�[
        for( j = 0; j < 17 ; j++ ){
            s_LineData[j] = token[j];
        }

        // �z��ǉ��׸ޏ�����
        bSearch = false;

        //------------------------------------------------------
        // 2008.03.19 �~���[B�d�l�̏ꍇ�A
        // ���x�v�A�y�сA���͌v�f�[�^���AA����B���œ���ւ���
        // ���ʎq"A_TEMP"/"B_TEMP"/"A_PRSG"/"B_PRSG"
        //------------------------------------------------------
        if( s_LineData[16] == "B"){
            if( s_LineData[0] == "A_TEMP" ){
                s_LineData[0] = "B_TEMP";
            }else if( s_LineData[0] == "B_TEMP" ){
                s_LineData[0] = "A_TEMP";
            }else if( s_LineData[0] == "A_PRSG" ){
                s_LineData[0] = "B_PRSG";
            }else if( s_LineData[0] == "B_PRSG" ){
                s_LineData[0] = "A_PRSG";
            }
        }

        //2008.08.21 �[�ǉ��~�ߏC��
        //�~���[B�d�l�̏ꍇ�A
        //�[�ǉ��~�߂̃f�[�^���AA����B���œ���ւ���
        //���ʎq"RSTP-A"/"RSTP-B"
        if( s_LineData[16] == "B"){
            if( s_LineData[1] == "RSTP-A" ){
                s_LineData[1] = "RSTP-B";
            }else if( s_LineData[1] == "RSTP-B" ){
                s_LineData[1] = "RSTP-A";
            }
        }
        //***************************

        //============================
        // �װ�d�l���ް��s�̏ꍇ
        //============================
        if( s_LineData[16] == "A" || s_LineData[16] == "B" ){
            if(iDataNum > 0){
                for( j = 0; j < iDataNum ; j++ ){

                    //------------------------------------------------
                    // �װA�d�l���װB�d�l�ŁA�������i��1�ɂ܂Ƃ߂�
                    // [�W�v1]=[�W�v5]>[�W�v4]>[�W�v2]=[�W�v3]�̏��ɏW�v������������
                    //
                    // ���װ�d�l�̕��i�W�v�d�l�y�сA�ȉ��̊����֐������Ƃɍ쐬
                    //   Buhin_Syukei,Buhin_Syukei2,Buhin_Syukei3,Buhin_Syukei4
                    //------------------------------------------------

                    //------------------------------------------------
                    // ���W�v1
                    // ���ڰ�/�ڰт̂����A[1]���ʎq��
                    //  "DPLG-A"/"DPLG-B"/"DPLGDA"/
                    //  "DPLGDB"/"EPLGDA"/"EPLGDB"/"EPGS-R"/"FRAM-E"
                    // �[1]���ʎq,[3]���i����,[4]�ގ�����,[5]�}��,
                    //  [10]�d�l1 �` [13]�d�l4���S�Ĉ�v�����ް�
                    // ��1�ɂ܂Ƃ߂�
                    //------------------------------------------------
                    if( ( s_LineData[0] == "PLATE" )
                     || ( s_LineData[0] == "FRAME" )){

                         if( ( s_LineData[1] == "DPLG-A" )
                          || ( s_LineData[1] == "DPLG-B" )
                          || ( s_LineData[1] == "DPLGDA" )
                          || ( s_LineData[1] == "DPLGDB" )
                          || ( s_LineData[1] == "EPLGDA" )
                          || ( s_LineData[1] == "EPLGDB" )
                          || ( s_LineData[1] == "EPGS-R" )
                          || ( s_LineData[1] == "FRAM-E" ) ){

                            if( ( s_wkData[j][1] == s_LineData[1] )
                             && ( s_wkData[j][3] == s_LineData[3] )
                             && ( s_wkData[j][4] == s_LineData[4] )
                             && ( s_wkData[j][5] == s_LineData[5] )
                             && ( s_wkData[j][10] == s_LineData[10] )
                             && ( s_wkData[j][11] == s_LineData[11] )
                             && ( s_wkData[j][12] == s_LineData[12] )
                             && ( s_wkData[j][13] == s_LineData[13] ) ){

                                bSearch = true;
                                break;
                            }
                        }
                    }

                    //------------------------------------------------
                    // ���W�v2
                    // ���ڰĂ̂����A[�W�v1]�ȊO�̎��ʎq�������ް�
                    // �[1]���ʎq,[4]�ގ�����,[5]�}�Ԃ�
                    //  �S�Ĉ�v�����ް���1�ɂ܂Ƃ߂�
                    // ���������A"PLHL--","DPLT--"�͏������Ȃ�
                    //------------------------------------------------
                    if( ( s_LineData[0] == "PLATE" ) ){

                        if( ( s_LineData[1] != "PLHL--" )
                         && ( s_LineData[1] != "DPLG-A" )
                         && ( s_LineData[1] != "DPLG-B" )
                         && ( s_LineData[1] != "DPLGDA" )
                         && ( s_LineData[1] != "DPLGDB" )
                         && ( s_LineData[1] != "EPLGDA" )
                         && ( s_LineData[1] != "EPLGDB" )
                         && ( s_LineData[1] != "EPGS-R" )
                         && ( s_LineData[1] != "DPLT--" ) ){

                            if( ( s_wkData[j][1] == s_LineData[1] )
                            && ( s_wkData[j][4] == s_LineData[4] )
                            && ( s_wkData[j][5] == s_LineData[5] ) ){

                                bSearch = true;
                                break;
                            }
                        }
                    }

                    //------------------------------------------------
                    // ���W�v3
                    // ����i����=MOATE
                    // �[1]���ʎq,[4]�ގ�����,[5]�}�Ԃ�
                    //  �S�Ĉ�v�����ް���1�ɂ܂Ƃ߂�
                    //------------------------------------------------
                    if( ( s_LineData[0] == "MOATE" ) ){

                        if( ( s_LineData[1] == "EPBP-A" )
                         || ( s_LineData[1] == "EPBP-B" ) ){

                            if( ( s_wkData[j][1] == s_LineData[1] )
                             && ( s_wkData[j][4] == s_LineData[4] )
                             && ( s_wkData[j][5] == s_LineData[5] ) ){

                                bSearch = true;
                                break;
                            }
                        }
                    }

                    //------------------------------------------------
                    // ���W�v4
                    // ����i����=ɽ��
                    // �[1]���ʎq,[4]�ގ�����,[5]�}��,[10]�d�l1,[11]�d�l2��
                    //  �S�Ĉ�v�����ް���1�ɂ܂Ƃ߂�
                    //------------------------------------------------
                    if( ( s_LineData[0] == "NOZZLE" ) ){

                        if( ( s_wkData[j][1] == s_LineData[1] )
                         && ( s_wkData[j][4] == s_LineData[4] )
                         && ( s_wkData[j][5] == s_LineData[5] )
                         && ( s_wkData[j][10] == s_LineData[10] )
                         && ( s_wkData[j][11] == s_LineData[11] ) ){

                            bSearch = true;
                            break;
                        }
                    }

                    //------------------------------------------------
                    // ���W�v5
                    // ��W�v1�`�W�v4�𖞂����Ȃ��S�Ă̕��i
                    // �[0]���i����,[1]���ʎq,[3]���i����,[4]�ގ�����,[5]�}��,
                    //  [10]�d�l1 �` [13]�d�l4���S�Ĉ�v�����ް�
                    //  ��1�ɂ܂Ƃ߂�
                    // ��ERROR�s�͂܂Ƃ߂Ȃ�
                    // ��"PLHL--","DPLT--"�͏������Ȃ�
                    // ���W�v���@�́A[�W�v1]�Ɠ���
                    //------------------------------------------------
                    if( ( ! ( s_LineData[0] == "ERROR" ) )
                     && ( ! ( ( s_LineData[0] == "PLATE" ) && ( s_LineData[1] == "PLHL--" ) ) )
                     && ( ! ( ( s_LineData[0] == "PLATE" ) && ( s_LineData[1] == "DPLT--" ) ) ) ){

                        if( ( s_wkData[j][0] == s_LineData[0] )
                         && ( s_wkData[j][1] == s_LineData[1] )
                         && ( s_wkData[j][3] == s_LineData[3] )
                         && ( s_wkData[j][4] == s_LineData[4] )
                         && ( s_wkData[j][5] == s_LineData[5] )
                         && ( s_wkData[j][10] == s_LineData[10] )
                         && ( s_wkData[j][11] == s_LineData[11] )
                         && ( s_wkData[j][12] == s_LineData[12] )
                         && ( s_wkData[j][13] == s_LineData[13] ) ){

                            bSearch = true;
                            break;
                        }
                    }

                }
            }
        }

        if( bSearch ){
            // �u�����ް����װA�d�l�v�܂��́u�����ް����װB�d�l�ő������ް����װB�d�l�v�̏ꍇ
            // ���u�����ް����װA�d�l�A���A�������ް����װB�d�l�v�̹���͑��݂��Ȃ�
            if( ( ( s_LineData[16] == "A" ) )
             || ( ( s_LineData[16] == "B" ) && ( s_wkData[j][16] == "B") ) ){

                // 1�䕪���ʂɒl�𑫂�
                s_wkData[j][7] = HanToZen( IntToStr( ZenToHan(s_wkData[j][7]).ToIntDef(0)
                                                   + ZenToHan(s_LineData[7]).ToIntDef(0) ) );

                // �\�����ɒl�𑫂�
                s_wkData[j][8] = HanToZen( IntToStr( ZenToHan(s_wkData[j][8]).ToIntDef(0)
                                                   + ZenToHan(s_LineData[8]).ToIntDef(0) ) );
            }

            // ���v���װA�d�l/�װB�d�l�̑��v�𑫂�
            s_wkData[j][9] = HanToZen( IntToStr( ZenToHan(s_wkData[j][9]).ToIntDef(0)
                                               + ZenToHan(s_LineData[9]).ToIntDef(0) ) );

            // �����ް����װB�d�l�ŁA�������ް����װA�d�l�̏ꍇ
            if( ( s_LineData[16] == "B" ) && ( s_wkData[j][16] == "A" ) ){
                // �װ�d�l�����������("A"��"")
                s_wkData[j][16] = "";
            }

        }else{
            // "PLATE"����ڰ��ް��s("DPLT--")
            if( ( s_LineData[0] == "PLATE" ) && ( s_LineData[1] == "DPLT--" ) ){
                for( j = 0; j < 17 ; j++ ){
                    s_wkTmpPlData[iTmpPlDataNum][j] = s_LineData[j];
                }
                s_wkTmpPlData[iTmpPlDataNum][17] = iTmpPlDataNum;
                iTmpPlDataNum++;

            // "PLATE"�̍E���ް��s("PLHL--")
            }else if( ( s_LineData[0] == "PLATE" ) && ( s_LineData[1] == "PLHL--" ) ){
                for( j = 0; j < 17 ; j++ ){
                    s_wkTmpHlData[iTmpHlDataNum][j] = s_LineData[j];
                }
                s_wkTmpHlData[iTmpHlDataNum][17] = iTmpPlDataNum - 1;
                iTmpHlDataNum++;

            // ��L�ȊO�̕��i�ŁA��v�ް����Ȃ��ꍇ
            }else{

                //*******************************
                // ��v�ް����Ȃ���Δz��ɒǉ�
                //*******************************
                for( j = 0; j < 17 ; j++ ){
                    s_wkData[iDataNum][j] = s_LineData[j];

                }
                // �z��Index�ݸ����
                iDataNum++;
            }
        }
    }

    fclose(fp);    // ����
    //delete fp;

    //********************************************
    // ��ڰ�-�E���ް���ϰ�ށA��Ă��āA�z��Ɋi�[
    //********************************************
    Sort_HEADS_PlData(s_wkTmpPlData, s_wkTmpHlData, s_wkPlData);

    //********************************************
    // �ް��𕔕i���ށA���ʎq�ſ�Ă��āA�z��Ɋi�[
    //  "PLATE"��"MOATE"��"FRAME"��"NOZZLE"��
    //  "B_TEMP"��"A_TEMP"��"B_PRSG"��"A_PRSG"��
    //  "ACCS"��"SHIP"��"ERROR"
    //********************************************
    Sort_HEADS_Data(s_wkData, s_sortDATA);

    //********************************************
    // ��ڰ�-�E���ް��Ƃ���ȊO���ް��𓝍�
    //********************************************
    iRow = 0;

    // ��ڰā|�E���ް����i�[
    for( i = 0; i < 1000; i++){
        if( s_wkPlData[i][0] != "" ){
            for( j = 0; j < 17; j++){
                s_DATA[iRow][j] = s_wkPlData[i][j];
            }
            iRow++;
        }else{
            break;
        }
    }

    // ����ȊO���ް�(��č�)���i�[
    for( i = 0; i< 1000; i++){
        if( s_sortDATA[i][0] != "" ){
            for( j = 0; j < 17; j++){
                s_DATA[iRow][j] = s_sortDATA[i][j];
            }
            iRow++;
        }else{
            break;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F ���i�W�J�ް��z���ް���Ă���
// �T  �v      �F �����œn���ꂽ���i�W�J�ް��z���Ă���
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �װA�d�l���װB�d�l�ŁA���ʎq�������ް����ɕ��בւ���
//
//                2007.10.15 �װ�d�l�Ή� (�V�K�쐬)
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Sort_HEADS_Data(AnsiString s_DATA[][17], AnsiString s_SortDATA[][17])
{
    // ���i���ޔz��
    AnsiString sPartsType[11] = {"PLATE", "MOATE", "FRAME", "NOZZLE",
                                 "B_TEMP", "A_TEMP", "B_PRSG", "A_PRSG",
                                 "ACCS", "SHIP", "ERROR"};
    // ��đO�ް��s��
    int iDataNum;

    // �����ώ��ʎq�z��
    AnsiString sTmp[100];
    int iTmpCnt;

    // ���݂̔�r��
    int iIdRow;
    AnsiString sIdName;
    int iMatchRow;

    int i, j, k;

    // ��Č�z��p
    int iSortDataNum;
    bool bDone;     // ��Ĕz��Ɏ擾�ς��ǂ���

    //-----------------------------------
    // ��Č�z�񏉊���
    //-----------------------------------
    memset(s_SortDATA, 0x00, sizeof(s_SortDATA));

    //-----------------------------------
    // �ް�������
    //-----------------------------------
    iDataNum = 0;
    i = 0;
    // NULL�ɂȂ�܂Ŷ���
    while( s_DATA[i][0] != "" ){
        iDataNum++;
        i++;
    }

    // �ް�0���Ȃ�I��
    if( iDataNum==0 ) return(true);

    // ����ް��i�[����������
    iSortDataNum = 0;
    iTmpCnt = 0;

    // ���i���ނ��Ƃɏ���
    for( i = 0; i < 11; i++ ){

        //--------------------------------------------
        // �����ώ��ʎq�ر
        //--------------------------------------------
        if( iTmpCnt > 0 ){
            for( j = 0; j < iTmpCnt; j++ ){
                sTmp[j] = "";
            }
        }
        iTmpCnt = 0;

        //--------------------------------------------
        // �����l
        //--------------------------------------------
        iMatchRow = 0;  // ��r���s�T��
        iIdRow  = 0;    // ��r���s
        sIdName = "";   // ��r�����ʎq

        while( iMatchRow < iDataNum ){

            //--------------------------------------------
            // ��r�����ʎq�ݒ�(�����l)
            //--------------------------------------------
            bDone = false;
            if( (s_DATA[iMatchRow][0] != sIdName) && (s_DATA[iMatchRow][0] == sPartsType[i])){
                for( j = 0; j < iTmpCnt; j++){
                    if( s_DATA[iMatchRow][1] == sTmp[j] ){
                        // ���ɏ����ς̎��ʎq
                        bDone = true;
                        break;
                    }
                }

                if(!bDone){
                    //--------------------------------------------
                    // �擾���鎯�ʎq��ݒ�
                    //--------------------------------------------
                    iIdRow = iMatchRow;
                    sIdName = s_DATA[iIdRow][1];

                    sTmp[iTmpCnt] = sIdName;
                    iTmpCnt++;

                    //--------------------------------------------
                    // ��r�����ʎq�ƈ�v�����ް���Ĕz��Ɋi�[
                    //--------------------------------------------
                    for( j = iMatchRow; j < iDataNum; j++ ){

                        if( (s_DATA[j][0] == sPartsType[i]) && (s_DATA[j][1] == sIdName) ){
                            for( k = 0; k < 17; k++){
                                s_SortDATA[iSortDataNum][k] = s_DATA[j][k];
                            }
                            iSortDataNum++;
                        }
                    }
                }
            }
            iMatchRow++;
        }
    }

    return(true);
}

//---------------------------------------------------------------------------
// ���{��֐����F ���i�W�J�ް�����ڰ��ް��A�E���ް���ϰ�ނ��Ŀ�Ă���
// �T  �v      �F �����œn���ꂽ��ڰ��ް��z��A�E���ް���ϰ�ނ��Ŀ�Ă���
// ��  ��      �F s_PlData�@��ڰ��ް��z��
//                s_HlData  �E���ް��z��
//                s_SortPlData ϰ�ށA��Č����ڰ��ް��A�E���ް��z��
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �װA�d�l���װB�d�l�ŁA�����ް��͓������A�Ō�ɿ�Ă���
//
//                2007.11.08 �װ�d�l�Ή� (�V�K�쐬)
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Sort_HEADS_PlData(AnsiString s_wkTmpPlData[][18], AnsiString s_wkTmpHlData[][18], AnsiString s_SortPlDATA[][17])
{

    int i, j, k;
    int iPlDataNum;     // ϰ�ށA��Č����ڰāA�E���ް��s��
    int iTmpPlDataNum;  // ��ڰ��ް��s��
    int iTmpHlDataNum;  // �E���ް��s��

    bool bMatch;
    int bMatchRow;

    //----------------------------------------------------
    // ������
    //----------------------------------------------------
    // �z�񏉊���
    memset(s_SortPlDATA,0x00,sizeof(s_SortPlDATA));

    // �z����������
    iPlDataNum = 0;
    iTmpPlDataNum = 0;
    iTmpHlDataNum = 0;

    //----------------------------------------------------
    // �ް�������
    //----------------------------------------------------
    i = 0;
    // ��ڰ��ް���(NULL�ɂȂ�܂Ŷ���)
    while( s_wkTmpPlData[i][0] != "" ){
        iTmpPlDataNum++;
        i++;
    }

    i = 0;
    // �E���ް���(NULL�ɂȂ�܂Ŷ���)
    while( s_wkTmpHlData[i][0] != "" ){
        iTmpHlDataNum++;
        i++;
    }

        AnsiString iAplNo;
        AnsiString iBplNo;

    //----------------------------------------------------
    // �װB�d�l����ڰ��ް��ŁA�װA�d�l�ɓ����ް�������΁A
    // ��װA�d�l����ڰ��ް��̎d�l���ߺ��ނ�"A"��""�ɍX�V
    // ��װB�d�l����ڰ��ް��𖳌��ɂ���
    // ������ɂ�����ڰ��ް�����ڰ�No�Ɠ����l�����E���ް�������΁A
    //  �E���ް�(�װB�d�l)����ڰ�No���װA�d�l�̒l�ōX�V
    //----------------------------------------------------
    for( i = 0; i < iTmpPlDataNum ; i++ ){

        bMatch = false;

        // �װB�d�l����ڰ��ް��̏ꍇ
        if( s_wkTmpPlData[i][16] == "B"){

            // �װB�d�l����ڰ�No
            iBplNo = s_wkTmpPlData[i][17].ToIntDef(-1);

            // A�d�l�ɓ�����ڰĂ��Ȃ����T��
            for( j = 0; j < i ; j++ ){
                if( s_wkTmpPlData[j][16] == "A" ){
                    for( k = 0; k < 16; k++){
                        //2010/05/14 HEADS�V�X�e����Q�A���\-0004 �� �d�l�P�̏������O��(k != 10)
                        if( (k != 7) && (k != 8) && (k != 9) && (k != 10) ){   // ���ʂ͔�r���Ȃ�
                            if( s_wkTmpPlData[i][k] != s_wkTmpPlData[j][k] ){
                                break;
                            }
                        }
                        // A�d�l�ɓ�����ڰĂ���
                        if( k == 15 ){
                            bMatch = true;
                            bMatchRow = j;
                            // �װA�d�l����ڰ�No
                            iAplNo = s_wkTmpPlData[bMatchRow][17].ToIntDef(-1);
                        }
                    }
                }
                if( bMatch ) break;
            }
        }

        if( bMatch ){

            // ��ڰ�(A�d�l)�̎d�l���ߺ��ނ�����("A"��"")
            s_wkTmpPlData[bMatchRow][16] = "";

            // ��ڰ�(A�d�l)�̑�������ڰ�(B�d�l)�̑����𑫂�
            s_wkTmpPlData[bMatchRow][9] = HanToZen( IntToStr( ZenToHan(s_wkTmpPlData[bMatchRow][9]).ToIntDef(0)
                                        + ZenToHan(s_wkTmpPlData[i][9]).ToIntDef(0) ) );

            // ��ڰ�(B�d�l)����ڰ�No�𖳌��ɂ���
            s_wkTmpPlData[i][17] = -1;

            // �E��(B�d�l)����ڰ�No�ŁA��ڰ�(B�d�l)����ڰ�No�Ɠ����ł���΁A��ڰ�(A�d�l)��ڰ�No�ōX�V
            for( j = 0; j < iTmpHlDataNum; j++){
                if( s_wkTmpHlData[j][17].ToIntDef(-1) == iBplNo ){

                    s_wkTmpHlData[j][17] = iAplNo;
                }
            }
        }
    }

    //----------------------------------------------------
    // �װB�d�l�̍E���ް��ŁA�װA�d�l�ɓ����ް�������΁A
    // �װA�d�l�̍E���ް��̎d�l���ߺ��ނ�"A"��""�ɍX�V���A
    // �װB�d�l�̍E���ް��𖳌��ɂ���
    //----------------------------------------------------
    for( i = 0; i < iTmpHlDataNum ; i++ ){

        bMatch = false;

        // �װB�d�l�̍E���ް��̏ꍇ
        if( s_wkTmpHlData[i][16] == "B"){

            // A�d�l�ɓ����E���ް����Ȃ����T��
            for( j = 0; j < i ; j++ ){
                if( s_wkTmpHlData[j][16] == "A" ){
                    for( k = 0; k < 16; k++){
                        if( (k != 7) && (k != 8) && (k != 9) ){
                            if( s_wkTmpHlData[i][k] != s_wkTmpHlData[j][k] ){
                                break;
                            }
                            // A�d�l�ɓ����E���ް�����
                            if( k == 15 ){
                                bMatch = true;
                                bMatchRow = j;
                            }
                        }
                    }
                }
                if( bMatch ) break;
            }
        }

        if( bMatch ){
            // �E��(A�d�l)�̎d�l���ߺ��ނ�����("A"��"")
            s_wkTmpHlData[bMatchRow][16] = "";

            // �E��(A�d�l)�̑��v�ɍE��(B�d�l)�̑��v�𑫂�
            s_wkTmpHlData[bMatchRow][9] = HanToZen( IntToStr( ZenToHan(s_wkTmpHlData[bMatchRow][9]).ToIntDef(0)
                                        + ZenToHan(s_wkTmpHlData[i][9]).ToIntDef(0) ) );

            // �E��(B�d�l)����ڰ�No�𖳌��ɂ���
            s_wkTmpHlData[i][17] = -1;
        }
    }

    //----------------------------------------------------
    // ��ڰ��ް��A�E���ް����A�܂Ƃ߂Ĕz��Ɋi�[
    //----------------------------------------------------
    for( i = 0; i < iTmpPlDataNum; i++ ){
        // �z��Ɋi�[���K�v����ڰ��ް��s�ł��邩
        if( s_wkTmpPlData[i][17].ToIntDef(-1) != -1 ){

            // ��ڰ��ް��s��z��Ɋi�[
            for( j = 0; j < 17; j++ ){
                s_SortPlDATA[iPlDataNum][j] = s_wkTmpPlData[i][j];
            }
            iPlDataNum++;

            // ��ڰ��ް��s�ɑΉ�����E���s��T��
            for( j = 0; j < iTmpHlDataNum; j++ ){
                // ��ڰ��ް��s�ɑΉ�����E���ް��s�ł��邩
                if(s_wkTmpPlData[i][17] == s_wkTmpHlData[j][17]){

                    // �E���ް��s��z��Ɋi�[
                    for( k = 0; k < 17; k++ ){
                        s_SortPlDATA[iPlDataNum][k] = s_wkTmpHlData[j][k];
                    }
                    iPlDataNum++;
                }
            }
        }
    }

    return(true);
}

//---------------------------------------------------------------------------
// ���{��֐����F ���̖��@�擾����
// �T  �v      �F ���x�v/���͌v�ɏo�͂���[XX]����XX�̕������擾����
// ��  ��      �F s_SideCd  "B":B��/"A":A��
//                bMirror�@True:�~���[�d�l/False:�ʏ�d�l
//
// �߂�l      �F �擾������
// ��  �l      �F
//                2008.03.19 �װ�d�l�Ή� (�V�K�쐬)
//---------------------------------------------------------------------------
AnsiString __fastcall TBuhinhyou::ConvertRyutaiName(AnsiString s_SideCd, bool bMirror)
{

    AnsiString s_RyutaiName;
    AnsiString s_Name;

    if( ! bMirror ){
        //===================================
        // �ʏ�g�p�̏ꍇ
        //===================================
        // �w�葤�̃R�[�h("�`"/"�a")
        s_RyutaiName = HanToZen( s_SideCd );

    }else{
        //===================================
        // �~���[�d�l�̏ꍇ
        //===================================
        //-----------------------------------
        // HEADS�e�L�X�g�f�[�^���痬�̖����擾
        //-----------------------------------
        if( s_SideCd == "B" ){
            // B�����̖�
            s_RyutaiName = P_HEADS_DATA[1071].Trim();
            if( s_RyutaiName == "" ) s_RyutaiName = "����";
        }else{
            // A�����̖�
            s_RyutaiName = P_HEADS_DATA[1073].Trim();
            if( s_RyutaiName == "" ) s_RyutaiName = "�ቷ";
        }

        //--------------------------------------
        // ���̖����ȗ��ł���ꍇ�͏ȗ��`�ɕϊ�
        //--------------------------------------
        s_Name = ZenToHan( s_RyutaiName ).UpperCase();
        if( s_Name == "SEA WATER")          s_RyutaiName = "S.W.";
        if( s_Name == "FRESH WATER")        s_RyutaiName = "F.W.";
        if( s_Name == "COOLING WATER")      s_RyutaiName = "C.W.";
        if( s_Name == "LUBRICATING OIL")    s_RyutaiName = "L.O.";

        //--------------------------------------
        // ���̖���S�p�ɕϊ�
        //--------------------------------------
        s_RyutaiName = HanToZen( s_RyutaiName );
    }

    //---------------------------------------------------
    // �ϊ���̗��̖��������Ԃ�
    //---------------------------------------------------
    return( s_RyutaiName );

}

//---------------------------------------------------------------------------
// ���{��֐����F ���L�R�[�h�L���m�F
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F ���L����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2020.07.17�ǉ�
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Tokki_Code(AnsiString Key)
{

   bool bSearch;

   bSearch = false;

   if ( P_HEADS_DATA[451].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[454].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[457].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[460].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[463].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[466].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[469].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[472].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[475].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[478].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[481].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[484].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[487].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[490].TrimRight() == Key ) {
      bSearch = true;
   }

   if( bSearch ){
      return(true);
   } else {
      return(false);
   }

}

