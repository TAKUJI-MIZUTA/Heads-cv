//---------------------------------------------------------------------------

#ifndef Bas_SijisyoHA3
#define Bas_SijisyoHA3
//---------------------------------------------------------------------------
#endif

extern bool __fastcall SijisyoA3(HeadsData* pcHeadsData);          // ����w����

AnsiString __fastcall Sheet_Exist_CheckA3(AnsiString path, HeadsData* pcHeadsData); // �ߋ�Excel���(����w����)��������
AnsiString __fastcall Get_Zaisitu_NameA3(AnsiString Code);          // �ގ�����Ͻ�����
AnsiString __fastcall Chg_Zaisitu_CodeA3(AnsiString Code,AnsiString Typ ); //�ގ��R�[�h�ϊ��}�X�^����
AnsiString __fastcall Get_AturyokuKeiA3(AnsiString s_Code);         // ���͌v��ގ擾

AnsiString __fastcall Get_SiageMeisyouA3(AnsiString Code);          // �d�㖼�́@�擾
AnsiString __fastcall Get_Tankan_DataA3(AnsiString Syurui, HeadsData* pcHeadsData);   // �[�ǎ�ށ@�擾
AnsiString __fastcall Get_Flange_DataA3(AnsiString Tugite);         // ��ݼގ�ގ擾
bool       __fastcall Get_Start_PlateA3(AnsiString &Side_B, AnsiString &Side_A, HeadsData* pcHeadsData);
AnsiString __fastcall Get_T_ZaZaisituA3(AnsiString TanSyurui, AnsiString TanName, HeadsData* pcHeadsData);

void       __fastcall Sheet_Compare_SyoriA3(AnsiString Excel_OldPass);    //
bool       __fastcall Sheet_Cell_CompareA3(int Row, int Col);             //
//2017.10.04 ��Ķ޽��čގ��ύX_S
AnsiString __fastcall Get_SheetGasketMatA3(AnsiString s_Side, HeadsData* pcHeadsData);  // �V�[�g�K�X�P�b�g�ގ��擾
//2017.10.04 ��Ķ޽��čގ��ύX_E
//2019.10.10 ���x�v�ی�ǒǉ�_S
AnsiString __fastcall Get_Well_Temp(AnsiString Key, AnsiString s_Kikaku,AnsiString s_FrngSize);  // ���x�v�ی�ǐ��@�擾
//2019.10.10 ���x�v�ی�ǒǉ�_E
// 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_S
extern bool __fastcall Tokki_Code(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2022.04.04 �@��ԍ��@�햼�̓��L�ǉ�_E
// 2002.04.11 �A�q�D���ǉ�_S
extern AnsiString __fastcall Tokki_Zen(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2022.04.11 �A�q�D���ǉ�_E


