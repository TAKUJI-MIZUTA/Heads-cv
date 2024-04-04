//---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���ё����ʌv�Z��(Steam)  �쐬���W���[��
//�@ �� �� �ҁFT.Kawai
//�@ �� �� ���F2002.06.10
//�@ �X �V ���F2002.09.20
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_Steam.h"

#define MdevideOnF(a,b)  ( ( (a) != 0.0f && (b) != 0.0f )? ( (a)/(b) ) : (0.0f) )
#define EXCEL_SCP_SHEETNAME    "��-є�d"          // EXCEL�V�[�g��(���є�d)
#define EXCEL_SCP_NAIKEI       "���a�ϊ�"          // EXCEL�V�[�g��(���a�ϊ�)
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F ���ё����ʌv�Z��(Steam)�@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Steam(void)
{
   AnsiString s_Text;            // �ėp������
   AnsiString s_SheetName;       // ��Ė���
   AnsiString s_S1Nossle;        // S1ɽ�َ��
   AnsiString s_S2Nossle;        // S2ɽ�َ��
   AnsiString s_S3Nossle;        // S3ɽ�َ��
   AnsiString s_S4Nossle;        // S4ɽ�َ��
   AnsiString s_E1Nossle;        // E1ɽ�َ��
   AnsiString s_E2Nossle;        // E2ɽ�َ��
   AnsiString s_E3Nossle;        // E3ɽ�َ��
   AnsiString s_E4Nossle;        // E4ɽ�َ��

   int        i_RecNo;           // HEADS�ް� ں���No
   int        i_Errflg;          // �װ�׸�
   float      f_Hijyu;           // ��d��
   float      f_Temp;            // �g�p���x
   float      f_Naikei;          // �z�ǂ̓��a
   float      f_Sounyu;          // ���ё�����
   int        i_Kikaku;          // �����m�Y���̋K�i�@0:JIS 1:ANSI 2:�T�j�^��

   f_Hijyu = f_Naikei = f_Temp = f_Sounyu = 0;

   //*******************
   //***  ��Ă̎w��
   //*******************
   s_SheetName = "�X�`�[��";
   // ��Ă̎w��
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_SheetName);

   //********************************************
   //***  ���ё����ʌv�Z���@�f�[�^�Z�b�g
   //********************************************
   G_Log_Text = "***************  " + s_SheetName + "�쐬�J�n  ***************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   i_Errflg = 0;

   //***************************
   //***  ���є�d��
   //***************************
   i_RecNo = 1101;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();

   if (AllFloatChk(s_Text.c_str(),0) == 0){
      f_Temp = StrToFloat( s_Text );
      f_Temp = FourDownFiveUp( f_Temp, 1 );
      s_Text = FloatToStr( f_Temp );

      if( HijyuGet(&s_Text) == 0 )
      {
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            f_Hijyu = StrToFloat( s_Text );
            if (f_Hijyu == 0){
               s_Text = "   -";
            } else {
               s_Text = FormatFloat("#,###,##0.0000",StrToFloat(s_Text));
            }
            Excel_Cell_DataSet(13, 21, s_Text);
            //2004.07.16 ���a�P�ʕύX
            //Excel_Cell_DataSet(20, 15, s_Text);
            Excel_Cell_DataSet(20, 16, s_Text);
            //***********************
            G_Log_Text = "���є�d��         �w" + s_Text + "�x���Z�b�g�B";
            Write_Log(G_Log_Text);
         } else {
            G_ErrLog_Text = "���є�d��            �s���Ȓl�w" + s_Text + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
            i_Errflg =1;
         }
      } else {
         G_ErrLog_Text = "���є�d��       ���x�̒l�ɑΉ������d�ʂ�����܂���B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
         i_Errflg =1;
      }
   } else {
      G_ErrLog_Text = "���є�d��       ���x�̒l�ɑΉ������d�ʂ�����܂���B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      i_Errflg =1;
   }

   //***************************
   //***  �g�p���x
   //***************************
   i_RecNo = 1101;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      f_Temp = StrToFloat( s_Text );
      f_Temp = FourDownFiveUp( f_Temp, 1 );
      if (f_Temp == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0",f_Temp);
      }
      Excel_Cell_DataSet(13, 39, s_Text);
      G_Log_Text = "�g�p���x            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�g�p���x              �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �z�ǂ̓��a
   //***************************
   if (StrToDblDef(G_HEADS_DATA[852].TrimRight(),0) >= StrToDblDef(G_HEADS_DATA[853].TrimRight(),0)) {

      //******************************************
      //***  (B�������x) >= (A�������x)�̂Ƃ�
      //******************************************
      s_S1Nossle = G_HEADS_DATA[891].TrimRight();      // S1ɽ�َ��
      s_S3Nossle = G_HEADS_DATA[921].TrimRight();      // S3ɽ�َ��
      s_E1Nossle = G_HEADS_DATA[951].TrimRight();      // E1ɽ�َ��
      s_E3Nossle = G_HEADS_DATA[981].TrimRight();      // E3ɽ�َ��

      if (s_S1Nossle == "1"){
         s_Text = G_HEADS_DATA[892].TrimRight();       // S1ɽ�ٌ��a
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[893] );

      } else if (s_S1Nossle != "1" && s_S3Nossle == "1"){
         s_Text = G_HEADS_DATA[922].TrimRight();       // S3ɽ�ٌ��a
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[923] );

      } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle == "1"){
         s_Text = G_HEADS_DATA[952].TrimRight();       // E1ɽ�ٌ��a
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[953] );

      } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle == "1"){
         s_Text = G_HEADS_DATA[982].TrimRight();       // E3ɽ�ٌ��a
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[983] );

      } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle != "1"){
         s_Text = "";
      }

   } else {
      //******************************************
      //***  (B�������x) < (A�������x)�̂Ƃ�
      //******************************************
      s_S2Nossle = G_HEADS_DATA[906].TrimRight();      // S2ɽ�َ��
      s_S4Nossle = G_HEADS_DATA[936].TrimRight();      // S4ɽ�َ��
      s_E2Nossle = G_HEADS_DATA[966].TrimRight();      // E2ɽ�َ��
      s_E4Nossle = G_HEADS_DATA[996].TrimRight();      // E4ɽ�َ��

      if (s_S2Nossle == "1"){
         s_Text = G_HEADS_DATA[907].TrimRight();       // S2ɽ�ٌ��a
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[908] );

      } else if (s_S2Nossle != "1" && s_S4Nossle == "1"){
         s_Text = G_HEADS_DATA[937].TrimRight();       // S4ɽ�ٌ��a
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[938] );

      } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle == "1"){
         s_Text = G_HEADS_DATA[967].TrimRight();       // E2ɽ�ٌ��a
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[968] );

      } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E3Nossle == "1"){
         s_Text = G_HEADS_DATA[997].TrimRight();       // E4ɽ�ٌ��a
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[998] );

      } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle != "1"){
         s_Text = "";
      }

   }

   if (AllFloatChk(s_Text.c_str(),0) == 0){
      f_Naikei = StrToFloat( s_Text );
      if (f_Naikei == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("0", f_Naikei);
         NaikeiGet( &s_Text, i_Kikaku );
         if (s_Text != "" ) {
            //2004.07.16 ���a�P�ʕύX
            //s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
            s_Text = FormatFloat("##,###,##0.0",StrToFloat(s_Text)*10);
            //***********************
            f_Naikei = StrToFloat( s_Text );
         } else {
            G_ErrLog_Text = "�z�ǂ̓��a            �ϊ��ł��܂���ł����w" + FormatFloat("0", f_Naikei) + "�x �ł��B";
            Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
            Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
            i_Errflg =1;
         }
      }
      Excel_Cell_DataSet(15, 36, s_Text);
      Excel_Cell_DataSet(20, 26, s_Text);
      G_Log_Text = "�z�ǂ̓��a         �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�z�ǂ̓��a            �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      i_Errflg =1;
   }

   //***************************
   //***  ���ё�����
   //***************************
   if( i_Errflg == 0 )
   {
      //2004.07.16 ���a�P�ʕύX
      //f_Sounyu = 0.28 * 30 * f_Hijyu * f_Naikei * f_Naikei;
      f_Sounyu = 0.0028 * 20 * f_Hijyu * f_Naikei * f_Naikei;
      //***********************
      f_Sounyu = FourDownFiveUp( f_Sounyu, 1 );
      if (AllFloatChk( FloatToStr(f_Sounyu).c_str(), 0) == 0){
         s_Text = FormatFloat("#,###,##0",f_Sounyu);

         Excel_Cell_DataSet(22, 9, s_Text);

         G_Log_Text = "���ё�����         �w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "���ё�����              �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }
   }

   //***************************
   //***  �����ԍ�
   //***************************
   // �wB006�x
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E   
   Excel_Cell_DataSet(55, 41, s_Text);
   G_Log_Text = "�����ԍ�           �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  HEADS VER
   //***************************
   // �wA005�x
   i_RecNo = 302;
   if (G_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "S";
   } else {
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(55, 12, s_Text);
   G_Log_Text = "HEADS VER          �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************************
   //***
   //***  �o�c�e�o�͗p�@�ݒ���e��������
   //***
   //********************************************
   AnsiString s_Old_SheetName;
   AnsiString s_CellText;

   int i_ConfRow;

   // ���݂̃V�[�g���̂�ۑ�
   s_Old_SheetName = exWorksheet.OlePropertyGet("Name");
   // ��Ă̎w��
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�o�c�e�ݒ�");

   // �ŏI�s�ɃZ�b�g
   s_CellText = "Def";
   i_ConfRow = 1;
   while (s_CellText != ""){
      exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << i_ConfRow << 2);
      s_CellText = exWorkCell.Exec(PropertyGet("Value"));

      i_ConfRow++;
   }

   // �o�c�e�o�͑Ώۼ�ĂƂ��Đݒ�
   exWorkCell.Exec(PropertySet("Value") << s_SheetName);
   // �ۑ����Ă������V�[�g���Ďw��
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

   G_Log_Text = "***************  " + s_SheetName + "�쐬�I��  ***************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F ���є�d�ʎ擾����
// �T  �v      �F ���є�d��ܰ���Ă��Q�Ƃ��ATPVH(���x)�ɑΉ������d�ʂ��擾
// ��  ��      �F AnsiString inText  // TPVH(���x)
// �߂�l      �F AnsiString inText  // ��d��
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
int __fastcall HijyuGet(AnsiString *inText)
{
   AnsiString s_Old_SheetName;
   AnsiString s_CellText;

   int i;

   // ���݂̃V�[�g���̂�ۑ�
   s_Old_SheetName = exWorksheet.OlePropertyGet("Name");

   // ��Ă̎w��(���є�d)
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_SCP_SHEETNAME);

   // �ŏI�s�ɃZ�b�g
   s_CellText = "Def";
   i = 4;

   while (s_CellText != ""){
      exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << i << 1);
      s_CellText = exWorkCell.Exec(PropertyGet("Value"));

      if( s_CellText == *inText ){
         *inText = exWorksheet.Exec(PropertyGet("Cells") << i << 2);

         // �ۑ����Ă������V�[�g���Ďw��
         exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

         return 0;
      }

      i++;
   }

   *inText = "";

   // �ۑ����Ă������V�[�g���Ďw��
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

   return 1;
}

//---------------------------------------------------------------------------
// ���{��֐����F �K�X�Ǔ��a�擾����
// �T  �v      �F �K�X�Ǔ��a��ܰ���Ă��Q�Ƃ��A�m�Y�����a�ɑΉ�������a���擾
// ��  ��      �F AnsiString inText  // �m�Y�����a
// �߂�l      �F AnsiString inText  // ���a
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
int __fastcall NaikeiGet(AnsiString *inText, int i_Kikaku)
{
   AnsiString s_Old_SheetName;
   AnsiString s_CellText;

   int i;

   // ���݂̃V�[�g���̂�ۑ�
   s_Old_SheetName = exWorksheet.OlePropertyGet("Name");

   // ��Ă̎w��(���a�ϊ�)
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_SCP_NAIKEI);

   // �ŏI�s�ɃZ�b�g
   s_CellText = "Def";
   if (i_Kikaku == 0 ) {                // JIS
      i = 4;
   } else if ( i_Kikaku == 1 ) {        // ANSI
      i = 19;
   } else {                             // �T�j�^��
      i = 34;
   }

   while (s_CellText != ""){
      exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << i << 1);
      s_CellText = exWorkCell.Exec(PropertyGet("Value"));

      if( s_CellText == *inText ){
         *inText = exWorksheet.Exec(PropertyGet("Cells") << i << 2);

         // �ۑ����Ă������V�[�g���Ďw��
         exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

         return 0;
      }

      i++;
   }

   *inText = "";

   // �ۑ����Ă������V�[�g���Ďw��
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

   return 1;
}

//-----------------------------------------------------------------------------
// ���{��֐����F �l�̌ܓ�����Ӽޭ��
// �T  �v      �F
// �p�����[�^  �F �Ȃ�
// �߂�l      �F 0:���퓮�� 1:�ُ퓮��
// �@�\����    �F
// ��  �l      �F
//-----------------------------------------------------------------------------
double  __fastcall  FourDownFiveUp( double  data,   // In.  �ϊ��O�ް�
                                    int     col     // In.  �Ώۏ�������(1�`)
                                  )
{
    // FourDownFiveUp�ϐ���`
            int     lop;        // ٰ�߶���
            long    lwork;      // ܰ��ް�
            double  ret;        // �����ް�
            double  dwork;      // ܰ��ް�
            double  mod;        // ܰ��ް�
            double  fixed;      // �萔

    // FourDownFiveUp�ϐ�������
    fixed = 1.0f;

    // ���オ��萔���
    for ( lop = 1; lop < col; lop++ )
    {
        fixed *= 10.0f;
    }

    // ����2���܂Ő�����
    dwork = data * fixed;
    lwork = (long)dwork;

    // �l�̌ܓ�
    mod = (double)( dwork - (double)lwork );
    if ( (long)(mod*10.0f) >= 5L )
    {
        lwork += 1L;
    }

    // �ϊ��l���
    ret = MdevideOnF( (double)lwork, fixed );

    // �����ް�[�ϊ����l�ް�(double�^)]
    return  ret;
}



