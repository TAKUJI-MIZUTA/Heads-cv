//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�\����FAX�p��(NK�ȊO)
//�@ �� �� �ҁFS.Ogita
//�@ �� �� ���F2007.02
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "SINSEICom.h"  // �\�������ʊ֐���`�p

#include "DataModule.h"

#include "Bas_ShinseiFaxNonNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F �\����FAX�p��(NK�ȊO)  �쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall ShinseiFaxNonNK(void)
{
   AnsiString strSheetName;   // �g�p�V�[�g��
   AnsiString strGetText;     // �擾HEADS�f�[�^
   AnsiString strWriteText;   // �`��f�[�^
   int nRecNo;                // HEADS�f�[�^�z��ԍ�

   strSheetName = "��-FAX";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "��-FAX");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("Item",OleVariant("��-FAX"));

   //******************************
   //***  �쐬�J�n۸ނ̏��o��
   //******************************
   G_Log_Text = "-----------    �w��-FAX�x�쐬�J�n2    -----------";
   Write_Log(G_Log_Text);

   // �K�i�R�[�h�̎擾
   nRecNo = 29;
   strGetText = G_HEADS_DATA[nRecNo].Trim();

   //***************************
   //***  FAX�ԍ�
   //***************************
   switch( strGetText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         strWriteText = "078-291-1309";
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strWriteText = "078-221-5744";
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strWriteText = "078-261-0780";
         break;
      case SEISEKISYO_STANDARDNO_NK:
         strWriteText = "0798-36-6861";
         break;
      case SEISEKISYO_STANDARDNO_GL:
         //2014.01.07 FAX�ԍ��ύX
         //strWriteText = "078-322-0700";
         strWriteText = "078-291-8500";
         //*******************
         break;
      case SEISEKISYO_STANDARDNO_KR:
         //2014.01.07 FAX�ԍ��ύX
         //strWriteText = "078-232-1385";
         strWriteText = "078-325-3441";
         //*******************
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strWriteText = "078-331-3395";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strWriteText = "06-6534-1436";
         break;
      default :
         strWriteText = "078-221-5744";
         break;
   }   
   Excel_Cell_DataSet(1, 3, strWriteText);
   G_Log_Text = "��-FAX�FFAX�ԍ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";   
   Write_Log(G_Log_Text);

   //***************************
   //***  �K�i��
   //***************************
   nRecNo = 29;
   strWriteText = Search_KIKAKU_CODE(G_HEADS_DATA[nRecNo].TrimRight(),true,true, G_HEADS_DATA[1].TrimRight());
   Excel_Cell_DataSet(3, 3, strWriteText);
   G_Log_Text = "��-FAX�F�K�i��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(21, 4, strWriteText);
   G_Log_Text = "��-FAX�F�����ԍ�        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �䐔
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].TrimRight();
   Excel_Cell_DataSet(21, 10, strWriteText);
   G_Log_Text = "��-FAX�F�䐔        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����s��
   //***************************
   strWriteText = GetCarvedSeal1st();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(48, 3, strWriteText);
      G_Log_Text = "��-FAX�F�����s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  �����s��
   //***************************
   strWriteText = GetCarvedSeal2nd();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(49, 3, strWriteText);
      G_Log_Text = "��-FAX�F�����s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  ����O�s��
   //***************************
   strWriteText = GetCarvedSeal3rd();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(50, 3, strWriteText);
      G_Log_Text = "��-FAX�F����O�s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  ����l�s��
   //***************************
   strWriteText = GetCarvedSeal4th();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(51, 3, strWriteText);
      G_Log_Text = "��-FAX�F����l�s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  ����܍s��
   //***************************
   strWriteText = GetCarvedSeal5th();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(52, 3, strWriteText);
      G_Log_Text = "��-FAX�F����܍s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

return true;
}
