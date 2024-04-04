//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�\�������ʊ֐�
//�@ �� �� �ҁFS.Ogita
//�@ �� �� ���F2007.03
//�@ �X �V ���F
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <math.h>

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "SINSEICom.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ���{��֐����F �����ԍ��擾("-"����)
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �����ԍ�("-"����)
// ��  �l      �F �d�l�� �y���L ���P�z
//---------------------------------------------------------------------------
AnsiString __fastcall GetSerialNoWithHyphen(void)
{
   AnsiString strRetSerialNo = "";  // �߂�l
   AnsiString strSerial = "";       // �����ԍ�
   AnsiString strCount = "";        // �䐔
   int nRecNo;

   // HEADS�f�[�^���琻���ԍ��̎擾
   nRecNo = 316;
   strSerial = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "�����ԍ� -�t�� (1/3)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strSerial + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   
   // �Q�����ڂƂR�����ڂ̊ԂɁh-�h������
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //strSerial.Insert("-",3);
   strSerial = GetSeizouBangou(strSerial);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E   

   // HEADS�f�[�^����䐔�̎擾
   nRecNo = 39;
   strCount = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "�����ԍ� -�t�� (2/3)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strCount + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   switch( strCount.ToIntDef(-1) )
   {
      case 2:
         strRetSerialNo = strSerial + "A/B";
         break;
      case 3:
         strRetSerialNo = strSerial + "A/B/C";
         break;
      case 4:
         strRetSerialNo = strSerial + "A/B/C/D";
         break;
      case 5:
         strRetSerialNo = strSerial + "A/B/C/D/E";
         break;
      default:
         strRetSerialNo = strSerial;
         break;
   }

   G_Log_Text = "�����ԍ� -�t�� (3/3)        �w" + strRetSerialNo + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   
   return strRetSerialNo;

}


//---------------------------------------------------------------------------
// ���{��֐����F �����ԍ��擾("-"�Ȃ�)
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �����ԍ�("-"�Ȃ�)
// ��  �l      �F �d�l�� �y���L ���R�z
//---------------------------------------------------------------------------
AnsiString __fastcall GetSerialNo(void)
{
   AnsiString strRetSerialNo = "";  // �߂�l
   AnsiString strSerial = "";       // �����ԍ�
   AnsiString strCount = "";        // �䐔
   int nRecNo;

   // HEADS�f�[�^���琻���ԍ��̎擾
   nRecNo = 316;
   strSerial = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "�����ԍ� -�Ȃ� (1/3)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strSerial + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // HEADS�f�[�^����䐔�̎擾
   nRecNo = 39;
   strCount = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "�����ԍ� -�Ȃ� (2/3)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strCount + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   switch( strCount.ToIntDef(-1) )
   {
      case 2:
         strRetSerialNo = strSerial + "A/B";
         break;
      case 3:
         strRetSerialNo = strSerial + "A/B/C";
         break;
      case 4:
         strRetSerialNo = strSerial + "A/B/C/D";
         break;
      case 5:
         strRetSerialNo = strSerial + "A/B/C/D/E";
         break;
      default:
         strRetSerialNo = strSerial;
         break;
   }

   G_Log_Text = "�����ԍ� -�Ȃ� (3/3)        �w" + strRetSerialNo + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   return strRetSerialNo;

}


//---------------------------------------------------------------------------
// ���{��֐����F �Z���t���̎擾
// �T  �v      �F
// ��  ��      �F �����t��
// �߂�l      �F �����t������ϊ������Z���t��
// ��  �l      �F �d�l�� �y���L ���S�z
//---------------------------------------------------------------------------
AnsiString __fastcall GetLiquidShortName(AnsiString strLiquidLongName)
{
   AnsiString strLiquidShortName = "";

   G_Log_Text = "�Z���t�� (�ϊ��O)        �w" + strLiquidLongName + "�x�����͂��ꂽ�B";
   Write_Log(G_Log_Text);

   if( strLiquidLongName.Trim() == "SEA WATER" )
   {
      strLiquidShortName = "S.W.";
   }
   else if( strLiquidLongName.Trim() == "S.W." )
   {
      strLiquidShortName = "S.W.";
   }
   else if( ( strLiquidLongName.Trim() == "fresh water" ) ||
            ( strLiquidLongName.Trim() == "FRESH WATER" ) )
   {
      strLiquidShortName = "F.W.";   
   }
   else if( strLiquidLongName.Trim() == "F.W." )
   {
      strLiquidShortName = "F.W.";
   }
   else if( strLiquidLongName.Trim() == "Lubricatng Oil" )
   {
      strLiquidShortName = "L.O.";
   }
   else if( strLiquidLongName.Trim() == "LO" )
   {
	  strLiquidShortName = "L.O.";
   }
   // 2013.04.03 ���ޏ����ύX
   else if( strLiquidLongName.Trim() == "LUB.OIL" )
   {
	  strLiquidShortName = "L.O.";
   }
   // ***********************
   else
   {
      strLiquidShortName = strLiquidLongName;
   }

   G_Log_Text = "�Z���t�� (�ϊ���)        �w" + strLiquidShortName + "�x���o�͂��ꂽ�B";
   Write_Log(G_Log_Text);

   return strLiquidShortName;

}

//---------------------------------------------------------------------------
// ���{��֐����F ���������������̎擾
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F ��������������
// ��  �l      �F �d�l�� �y���L ���T�z
//---------------------------------------------------------------------------
AnsiString __fastcall HigherTempPressure(void)
{
   AnsiString strRet = "";          // �߂�l
   AnsiString strABReversal = "";   // AB�t�]������
   int nRecNo = 0;                  // HEADS�f�[�^�̔z��ԍ�

   // AB�t�]������̎擾
   nRecNo = 137;
   strABReversal = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "��������������(1/2)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strABReversal + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   // AB�t�]������̔��ʁi���������������̔z��ԍ��ݒ�j
   if( strABReversal.ToIntDef(-1) == 0 )
   {
      nRecNo = 827;
   }
   else
   {
      nRecNo = 832;
   }
   // ���������������̎擾
   strRet = G_HEADS_DATA[nRecNo].Trim();
   strRet = FormatFloat("##0.00",StrToCurr(strRet));

   G_Log_Text = "��������������(2/2)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strRet + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   return strRet;
}

//---------------------------------------------------------------------------
// ���{��֐����F AB�̐����̍������̐��l���擾����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F AB�̐����̍������̐��l
// ��  �l      �F 
//---------------------------------------------------------------------------
AnsiString __fastcall GetHighPressureValueAorB(void)
{
   AnsiString strRet = "";          // �߂�l
   AnsiString strAVal,strBVal;
   int nRecNo = 0;                  // HEADS�f�[�^�̔z��ԍ�

   nRecNo = 832;
   strAVal = G_HEADS_DATA[nRecNo].Trim();

   nRecNo = 827;
   strBVal = G_HEADS_DATA[nRecNo].Trim();

   G_Log_Text = "AB���������̍�����:AVal(1/3)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strAVal + "�x���擾�B";
   Write_Log(G_Log_Text);
   G_Log_Text = "AB���������̍�����:BVal(2/3)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strBVal + "�x���擾�B";
   Write_Log(G_Log_Text);   

   double dwAVal,dwBVal;
   try{ dwAVal = strAVal.ToDouble();
   }catch( EConvertError &e ){
      dwAVal = 0.0;
   }
   try{ dwBVal = strBVal.ToDouble();
   }catch( EConvertError &e ){
      dwBVal = 0.0;
   }


   if( dwAVal > dwBVal )
   {
      strRet = strAVal;
   }
   else
   {
      strRet = strBVal;
   }

   strRet = FormatFloat("##0.00",StrToCurr(strRet));

   G_Log_Text = "AB���������̍�����:Result(3/3)        �w" + strRet + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   return strRet;
}


//---------------------------------------------------------------------------
// ���{��֐����F ���󍀖ڈ�s�ڎ擾
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F ���󍀖ڈ�s��
// ��  �l      �F �d�l�� �y���L ���Q  �P�^�T�z
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal1st(void)
{
   AnsiString strRet = "";    // �߂�l
   AnsiString strText = "";   // ��ƕϐ�
   int nRecNo = 0;              // HEADS�f�[�^�̔z��ԍ�

   // �K�i�R�[�h�̎擾
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = "ABS�}�[�NA    B";      
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = "LR TEST KOB�ؖ����ԍ�";
         break;
      case SEISEKISYO_STANDARDNO_NK:
         strRet = "NK   K";
         break;
      case SEISEKISYO_STANDARDNO_GL:
         strRet = "�ؖ����ԍ�KOB";
         break;
      case SEISEKISYO_STANDARDNO_KR:
         strRet = "KR�}�[�N KOBEM-�ؖ����ԍ�";
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = "BV�}�[�N�ؖ����ԍ�";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "DBP�ؖ����ԍ�";
         break;
      default:
         strRet = "ABS�}�[�NA    B";
         break;
   }

   return strRet;
}

//---------------------------------------------------------------------------
// ���{��֐����F ���󍀖ړ�s�ڎ擾
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F ���󍀖ړ�s��
// ��  �l      �F �d�l�� �y���L ���Q  �Q�^�T�z
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal2nd(void)
{
   AnsiString strRet = "";    // �߂�l
   AnsiString strText = "";   // ��ƕϐ�
   int nRecNo = 0;            // HEADS�f�[�^�̔z��ԍ�

   // �K�i�R�[�h�̎擾
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         strRet = "MFG " + GetSerialNoWithHyphen() ;
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = "KO�ؖ����ԍ�";
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_NK:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_GL:
         {
            AnsiString strTmp;
            int nRecNoTypeB = 827;
            int nRecNoTypeA = 832;
            strRet  = "W.T.P ";
            strTmp  = G_HEADS_DATA[nRecNoTypeB].Trim();
            strRet += FormatFloat("##0.00",StrToCurr(strTmp));
            strRet += "/";
            strTmp  = G_HEADS_DATA[nRecNoTypeA].Trim();
            strRet += FormatFloat("##0.00",StrToCurr(strTmp));
            strRet += " Mpa :B��/A�� T.P."; 
         }
         break;
      case SEISEKISYO_STANDARDNO_KR:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = "W.T.P. "+GetHighPressureValueAorB()+"MPa";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "";
         break;
      default:
         strRet = "KO�ؖ����ԍ�";
         break;
   }

   return strRet;
}


//---------------------------------------------------------------------------
// ���{��֐����F ���󍀖ڎO�s�ڎ擾
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F ���󍀖ڎO�s��
// ��  �l      �F �d�l�� �y���L ���Q  �R�^�T�z
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal3rd(void)
{
   AnsiString strRet = "";    // �߂�l
   AnsiString strText = "";   // ��ƕϐ�
   int nRecNo = 0;            // HEADS�f�[�^�̔z��ԍ�

   // �K�i�R�[�h�̎擾
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1071;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 827;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = "W.T.P. "+strLiquidName+" "+strPressure+"MPa";
            
            G_Log_Text = "���󍀖ڎO�s�ڋK�i"+strText+" �t��[1071] �F["+G_HEADS_DATA[1071]+"]["+strLiquidName+"] ����[827]"+strPressure;
            Write_Log(G_Log_Text);            
         }
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = "W.T.P. "+GetHighPressureValueAorB()+"MPa";
         break;
	  case SEISEKISYO_STANDARDNO_NK:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_GL:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_KR:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1071;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 827;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = "W.T.P. "+strLiquidName+" "+strPressure+"MPa";

            G_Log_Text = "���󍀖ڎO�s�ڋK�i"+strText+" �t��[1071] �F["+G_HEADS_DATA[1071]+"]["+strLiquidName+"] ����[827]"+strPressure;
            Write_Log(G_Log_Text);
         }
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "";
         break;
      default:
         strRet = GetSerialNoWithHyphen();
         break;
   }

   return strRet;
}


//---------------------------------------------------------------------------
// ���{��֐����F ���󍀖ڎl�s�ڎ擾
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F ���󍀖ڎl�s��
// ��  �l      �F �d�l�� �y���L ���Q  �S�^�T�z
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal4th(void)
{
   AnsiString strRet = "";    // �߂�l
   AnsiString strText = "";   // ��ƕϐ�
   int nRecNo = 0;            // HEADS�f�[�^�̔z��ԍ�

   // �K�i�R�[�h�̎擾
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1073;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 832;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = strLiquidName+" "+strPressure+"MPa";

            G_Log_Text = "���󍀖ڎl�s�ڋK�i"+strText+" �t��[1073] �F["+G_HEADS_DATA[1073]+"]["+strLiquidName+"] ����[832]"+strPressure;
            Write_Log(G_Log_Text);
         }
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = "W.T.P. "+HigherTempPressure()+"MPa";      
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_NK:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1071;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 827;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = "W.T.P. "+strLiquidName+" "+strPressure+"MPa";

            G_Log_Text = "���󍀖ڎl�s�ڋK�i[1071]"+strText+" �t�� �F["+G_HEADS_DATA[1071]+"]["+strLiquidName+"] ����[827]"+strPressure;
            Write_Log(G_Log_Text);            
         }
         break;
      case SEISEKISYO_STANDARDNO_GL:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_KR:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1073;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 832;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = strLiquidName+" "+strPressure+"MPa";

            G_Log_Text = "���󍀖ڎl�s�ڋK�i"+strText+" �t��[1073] �F["+G_HEADS_DATA[1073]+"]["+strLiquidName+"] ����[832]"+strPressure;
            Write_Log(G_Log_Text);            
         }
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "";
         break;
      default:
         strRet = "W.T.P. "+HigherTempPressure()+"MPa";
         break;
   }


   return strRet;

}


//---------------------------------------------------------------------------
// ���{��֐����F ���󍀖ڌ܍s�ڎ擾
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F ���󍀖ڌ܍s��
// ��  �l      �F �d�l�� �y���L ���Q  �T�^�T�z
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal5th(void)
{
   AnsiString strRet = "";    // �߂�l
   AnsiString strText = "";   // ��ƕϐ�
   int nRecNo = 0;            // HEADS�f�[�^�̔z��ԍ�

   // �K�i�R�[�h�̎擾
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_NK:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1073;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 832;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = strLiquidName+" "+strPressure+"MPa";
            
            G_Log_Text = "���󍀖ڌ܍s�ڋK�i"+strText+" �t��[1073] �F["+G_HEADS_DATA[1073]+"]["+strLiquidName+"] ����[832]"+strPressure;
            Write_Log(G_Log_Text);
         }
         break;
      case SEISEKISYO_STANDARDNO_GL:
         strRet = "";
         break;
	  case SEISEKISYO_STANDARDNO_KR:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "";
         break;
      default:
         strRet = "";
         break;
   }

   return strRet;
}


//---------------------------------------------------------------------------
// ���{��֐����F �ގ��R�[�h�̓��{��ϊ�
// �T  �v      �F
// ��  ��      �F �ގ��R�[�h
// �߂�l      �F �ގ��̓��{�ꖼ
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
AnsiString __fastcall ConvertMaterialJtoE(AnsiString strMaterialCode)
{
   AnsiString strMaterialName = "";
   // 2016.11.08 �ގ����ޕύX
   //AnsiString strCode = "";
   //int nCode;
   AnsiString s_Text;
   // ***********************

   // �K�i�R�[�h�̎擾
   // 2016.11.08 �ގ����ޕύX
   //int nRecNo = 29;
   //strCode = G_HEADS_DATA[nRecNo].Trim();
   //nCode = strCode.ToIntDef(-1);
   //
   //switch( strMaterialCode.ToIntDef(-1) )
   //{
   //   case 1014:
   //   case 1015:
   //   case 1032:
   //   case 1033:
   //   case 1057:
   //   case 1058:
   //   //2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2�ǉ�
   //   case 1075:
   //   case 1089:
   //   case 1090:
   //   //*************************************************
   //   //2010.12.02 SB-265-Gr.11�ǉ�
   //   case 1076:
   //   //***************************
   //   case 1085:
   //   case 1098:
   //      strMaterialName = "Titanium";
   //      break;
   //   default:
   //      /*
   //      if( nCode == 14 )
   //      {
   //         strMaterialName = "Stainless steel";
   //      }
   //      else
   //      {
   //         strMaterialName = "Stainless";
   //      }
   //      */
   //      // 20070510 ABS BV��Stainless �� Stainless steel�ɕύX�̂���
   //      // ��Lif�����R�����g�A�E�g(ABS BV LR)�̂݌��󗘗p
   //      strMaterialName = "Stainless steel";
   //      break;
   //}
   s_Text = Type_MAT(strMaterialCode);
   if( s_Text == "Ti" ) {           // �`�^���̏ꍇ�͎d��T
      strMaterialName = "Titanium";
   } else {
      strMaterialName = "Stainless steel";
   }
   // ***********************


   return strMaterialName;

}


//---------------------------------------------------------------------------
// ���{��֐����F Excel�V�[�g�̃R�s�[�֐�
// �T  �v      �F
// ��  ��      �F �R�s�[��t�@�C����,�R�s�[��V�[�g(Before),�R�s�[���t�@�C����,�R�s�[���V�[�g��
// �߂�l      �F �ގ��̓��{�ꖼ
// ��  �l      �F Before�̈Ӗ��E�E�E�w�肵���V�[�g�̑O�ɃV�[�g���R�s�[����B
//---------------------------------------------------------------------------
bool __fastcall ExcelSheetCopy( AnsiString strToFilePass, AnsiString strToSheetName, AnsiString strFromFilePath, AnsiString strFromSheetName)
{
   // �R�s�[��p�ϐ�
   Variant exWorkbookTo;
   Variant exWorksheetsTo;
   Variant exWorksheetTo;
   // �R�s�[���p�ϐ�
   Variant exWorkbookFrom;
   Variant exWorksheetsFrom;
   Variant exWorksheetFrom;

   //******************************
   //***  �R�s�[��Excel Book Open
   //******************************
   // ̧�ٵ����
   try {
	  //exWorkbookTo  = exWorkbooks.Exec(Function("Open") << strToFilePass.c_str());
	  //Windows 2008Server �ڐA�Ή�
	  exWorkbookTo  = exWorkbooks.OleFunction("Open", (OleVariant)strToFilePass );
   }
   catch (...) {
      G_ErrLog_Text = "�w" + strToFilePass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ��ĺڸ��ݎ擾
   try {
	  //exWorksheetsTo = exWorkbookTo.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheetsTo = exWorkbookTo.OlePropertyGet("WorkSheets");
   }
   catch (...) {
      G_ErrLog_Text = "�w" + strToFilePass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ��Ď擾
   try {
	  //exWorksheetTo = exWorksheetsTo.Exec(PropertyGet("Item") << strToSheetName.c_str());
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheetTo = exWorksheetsTo.OlePropertyGet("item",(OleVariant)strToSheetName );
   }
   catch (...) {
	  G_ErrLog_Text = "�w" + strToSheetName + "�x�̼�Ă̎擾�Ɏ��s���܂����I�I";
	  Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //******************************
   //***  �R�s�[��Excel Book Open
   //******************************
   // ̧�ٵ����
   try {
	  //exWorkbookFrom  = exWorkbooks.Exec(Function("Open") << strFromFilePath.c_str());
	  //Windows 2008Server �ڐA�Ή�
	  exWorkbookFrom  = exWorkbooks.OleFunction("Open", (OleVariant)strFromFilePath );
   }
   catch (...) {
      G_ErrLog_Text = "�w" + strFromFilePath + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ��ĺڸ��ݎ擾
   try {
	  //exWorksheetsFrom = exWorkbookFrom.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheetsFrom = exWorkbookFrom.OlePropertyGet("WorkSheets");
   }
   catch (...) {
      G_ErrLog_Text = "�w" + strFromFilePath + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ��Ď擾
   try {
	  //exWorksheetFrom = exWorksheetsFrom.Exec(PropertyGet("Item") << strFromSheetName.c_str());
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheetFrom = exWorksheetsFrom.OlePropertyGet("Item",(OleVariant)strFromSheetName);
   }
   catch (...) {
	  G_ErrLog_Text = "�w" + strFromSheetName + "�x�̼�Ă̎擾�Ɏ��s���܂����I�I";
	  Write_Error_Log(G_ErrLog_Text);
	  return false;
   }

   //******************************
   //***  �R�s�[�̎��s
   //******************************
   // �R�s�[�̎��s
   try {
	  //exWorksheetFrom.Exec(Function("Copy") << Variant(exWorksheetTo));
	  exWorksheetFrom.OleFunction("Copy", (OleVariant)exWorksheetTo);
	  //exWorksheetTo.OleFunction("Copy", (OleVariant)exWorksheetFrom);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + strFromSheetName + "�x�̼�ĺ�߰�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //******************************
   //***  �ҏWExcelbook�̕ۑ�
   //******************************
   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   //Windows 2008Server �ڐA�Ή�
   exApplication.OlePropertySet("DisplayAlerts",false);
   //exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << strToFilePass.c_str());
   exWorkbookTo.OleFunction("SaveAs", (OleVariant)strToFilePass);
   //******************************
   //***  Excel�I�u�W�F�N�g�̉��
   //******************************
   exWorkbookFrom.OleFunction("Close");
   exWorkbookTo.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   return true;


}


//---------------------------------------------------------------------------
// ���{��֐����F ���l�̉p��ϊ��i�S�đ啶���j
// �T  �v      �F
// ��  ��      �F ���l
// �߂�l      �F �p�ꉻ�������l
// ��  �l      �F
//---------------------------------------------------------------------------
AnsiString __fastcall NumeralJtoEAllCapital( AnsiString strNumeral)
{
   AnsiString strRet = "";
   int nInput,nTen,nOne;

   nInput = strNumeral.ToIntDef(-1);

   if( nInput > -1 )
   {
      // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	  nTen = floor((double)(nInput / 10));
	  // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
      switch (nTen)
	   {
	      case 0:
            if( nInput == 0 )
            {
               strRet = "ZERO";
            }
	         break;
	      case 1:
            nOne = nInput % 10;
            switch(nOne)
            {
               case 0:
                  strRet = "TEN";
                  break;
               case 1:
                  strRet = "ELEVEN";
                  break;
               case 2:
                  strRet = "TWELVE";
                  break;
               case 3:
                  strRet = "THIRTEEN";
                  break;
               case 4:
                  strRet = "FOURTEEN";
                  break;
               case 5:
                  strRet = "FIFTEEN";
                  break;
               case 6:
                  strRet = "SIXTEEN";
                  break;
               case 7:
                  strRet = "SEVENTEEN";
                  break;
               case 8:
                  strRet = "EIGHTEEN";
                  break;
               case 9:
                  strRet = "NINETEEN";
                  break;
               default:
                  strRet = "";
                  break;
            }
	         break;
	      case 2:
            strRet = "TWENTY";
	         break;
	      case 3:
            strRet = "THIRTY";
	         break;
	      case 4:
            strRet = "FORTY";
	         break;
	      case 5:
            strRet = "FIFTY";
	         break;
	      case 6:
            strRet = "SIXTY";
	         break;
	      case 7:
            strRet = "SEVENTY";
	         break;
	      case 8:
            strRet = "EIGHTY";
	         break;
	      case 9:
            strRet = "NINETY";        
	         break;
         default:
            break;
	   }

      if( (nInput >= 20) || ((nInput >= 1) && (nInput <= 9)) )
      {
   		nOne = nInput % 10;
	      switch (nOne)
		   {
		      case 0:
		         break;
		      case 1:
	               strRet += " ONE";
		         break;
		      case 2:
	            strRet += " TWO";
		         break;
		      case 3:
	            strRet += " THREE";
		         break;
		      case 4:
	            strRet += " FOUR";
		         break;
		      case 5:
	            strRet += " FIVE";
		         break;
		      case 6:
	            strRet += " SIX";
		         break;
		      case 7:
	            strRet += " SEVEN";
		         break;
		      case 8:
	            strRet += " EIGHT";
		         break;
		      case 9:
	            strRet += " NINE";
		         break;
	         default:
               strRet += "";
	            break;
		   }
      }

   }

   return strRet;
}


//---------------------------------------------------------------------------
// ���{��֐����F ���l�̉p��ϊ��i�擪�����啶���j
// �T  �v      �F
// ��  ��      �F ���l
// �߂�l      �F �p�ꉻ�������l
// ��  �l      �F
//---------------------------------------------------------------------------
AnsiString __fastcall NumeralJtoEHeadCapital( AnsiString strNumeral)
{
   AnsiString strRet = "";
   int nInput,nTen,nOne;

   nInput = strNumeral.ToIntDef(-1);

   if( nInput > -1 )
   {
	  // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	  nTen = floor((double)(nInput / 10));
	  // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
      switch (nTen)
	   {
	      case 0:
            nOne = nInput % 10;
            switch(nOne)
            {
               case 0:
                  strRet = "Zero";
                  break;
               case 1:
                  strRet = "One";
                  break;
               case 2:
                  strRet = "Two";
                  break;
               case 3:
                  strRet = "Three";
                  break;
               case 4:
                  strRet = "Four";
                  break;
               case 5:
                  strRet = "Five";
                  break;
               case 6:
                  strRet = "Six";
                  break;
               case 7:
                  strRet = "Seven";
                  break;
               case 8:
                  strRet = "Eight";
                  break;
               case 9:
                  strRet = "Nine";
                  break;
               default:
                  strRet = "";
                  break;
            }
	         break;
	      case 1:
            nOne = nInput % 10;
            switch(nOne)
            {
               case 0:
                  strRet = "Ten";
                  break;
               case 1:
                  strRet = "Eleven";
                  break;
               case 2:
                  strRet = "Twelve";
                  break;
               case 3:
                  strRet = "Thirteen";
                  break;
               case 4:
                  strRet = "Fourteen";
                  break;
               case 5:
                  strRet = "Fifteen";
                  break;
               case 6:
                  strRet = "Sixteen";
                  break;
               case 7:
                  strRet = "Seventeen";
                  break;
               case 8:
                  strRet = "Eighteen";
                  break;
               case 9:
                  strRet = "Nineteen";
                  break;
               default:
                  strRet = "";
                  break;
            }
	         break;
	      case 2:
            strRet = "Twenty";
	         break;
	      case 3:
            strRet = "Thirty";
	         break;
	      case 4:
            strRet = "Forty";
	         break;
	      case 5:
            strRet = "Fifty";
	         break;
	      case 6:
            strRet = "Sixty";
	         break;
	      case 7:
            strRet = "Seventy";
	         break;
	      case 8:
            strRet = "Eighty";
	         break;
	      case 9:
            strRet = "Ninety";        
	         break;
         default:
            break;
	   }

      if( nInput >= 20 )
      {
   		nOne = nInput % 10;
	      switch (nOne)
		   {
		      case 0:
		         break;
		      case 1:
	               strRet += " one";
		         break;
		      case 2:
	            strRet += " two";
		         break;
		      case 3:
	            strRet += " three";
		         break;
		      case 4:
	            strRet += " four";
		         break;
		      case 5:
	            strRet += " five";
		         break;
		      case 6:
	            strRet += " six";
		         break;
		      case 7:
	            strRet += " seven";
		         break;
		      case 8:
	            strRet += " eight";
		         break;
		      case 9:
	            strRet += " nine";
		         break;
	         default:
               strRet = "";
	            break;
		   }
      }

   }

   return strRet;


}
