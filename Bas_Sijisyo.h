//---------------------------------------------------------------------------

#ifndef Bas_SijisyoH
#define Bas_SijisyoH
//---------------------------------------------------------------------------
#endif

extern bool __fastcall Sijisyo(HeadsData* pcHeadsData);          // ����w����

AnsiString __fastcall Sheet_Exist_Check(AnsiString path, HeadsData* pcHeadsData); // �ߋ�Excel���(����w����)��������
AnsiString __fastcall Get_Zaisitu_Name(AnsiString Code);          // �ގ�����Ͻ�����
AnsiString __fastcall Chg_Zaisitu_Code(AnsiString Code,AnsiString Typ ); //�ގ��R�[�h�ϊ��}�X�^����
AnsiString __fastcall Get_AturyokuKei(AnsiString s_Code);         // ���͌v��ގ擾

AnsiString __fastcall Get_SiageMeisyou(AnsiString Code);          // �d�㖼�́@�擾
AnsiString __fastcall Get_Tankan_Data(AnsiString Syurui, HeadsData* pcHeadsData);   // �[�ǎ�ށ@�擾
AnsiString __fastcall Get_Flange_Data(AnsiString Tugite);         // ��ݼގ�ގ擾
bool       __fastcall Get_Start_Plate(AnsiString &Side_B, AnsiString &Side_A, HeadsData* pcHeadsData);
AnsiString __fastcall Get_T_ZaZaisitu(AnsiString TanSyurui, AnsiString TanName, HeadsData* pcHeadsData);

void       __fastcall Sheet_Compare_Syori(AnsiString Excel_OldPass);    //
bool       __fastcall Sheet_Cell_Compare(int Row, int Col);             //
//2017.10.04 ��Ķ޽��čގ��ύX_S
AnsiString __fastcall Get_SheetGasketMat(AnsiString s_Side, HeadsData* pcHeadsData);  // �V�[�g�K�X�P�b�g�ގ��擾
//2017.10.04 ��Ķ޽��čގ��ύX_E


