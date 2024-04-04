//---------------------------------------------------------------------------

#ifndef SINSEIComH
#define SINSEIComH
//---------------------------------------------------------------------------
#endif

extern AnsiString __fastcall GetSerialNoWithHyphen(void);
extern AnsiString __fastcall GetSerialNo(void);
extern AnsiString __fastcall GetLiquidShortName(AnsiString strLiquidLongName);
extern AnsiString __fastcall GetHighPressureValueAorB(void);
extern AnsiString __fastcall HigherTempPressure(void);
extern AnsiString __fastcall GetCarvedSeal1st(void);
extern AnsiString __fastcall GetCarvedSeal2nd(void);
extern AnsiString __fastcall GetCarvedSeal3rd(void);
extern AnsiString __fastcall GetCarvedSeal4th(void);
extern AnsiString __fastcall GetCarvedSeal5th(void);
extern AnsiString __fastcall ConvertMaterialJtoE(AnsiString strMaterialCode);
extern AnsiString __fastcall NumeralJtoEAllCapital( AnsiString strNumeral);
extern AnsiString __fastcall NumeralJtoEHeadCapital( AnsiString strNumeral);
extern bool __fastcall ExcelSheetCopy( AnsiString strToFilePass, AnsiString strToSheetName, AnsiString strFromFilePath, AnsiString strFromSheetName);
