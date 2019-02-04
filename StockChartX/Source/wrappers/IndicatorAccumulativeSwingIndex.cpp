// IndicatorAccumulativeSwingIndex.cpp: implementation of the CIndicatorAccumulativeSwingIndex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorAccumulativeSwingIndex.h"
#include "tasdk.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndicatorAccumulativeSwingIndex::CIndicatorAccumulativeSwingIndex(LPCTSTR name, int type, int members, CChartPanel* owner)
{
	szName = name;
	ownerPanel = owner;
	pCtrl = owner->pCtrl;
	seriesType = type;
	memberCount = members;
	Initialize();
	nSpace = 0;


	// Resize param arrays for this indicator.
	// NOTE! Set all array sizes to max number of parameters.
	// ALL three arrays must be resized.
	paramCount = 2;
	paramStr.resize(paramCount);
	paramDbl.resize(paramCount);
	paramInt.resize(paramCount);

	indicatorType = indAccumulativeSwingIndex;

}

CIndicatorAccumulativeSwingIndex::~CIndicatorAccumulativeSwingIndex()
{
	CIndicator::OnDestroy();
}

void CIndicatorAccumulativeSwingIndex::SetParamInfo(){


	/*  Required inputs for this indicator:
	
	  1. paramStr[0] = Symbol (eg "msft")
	  2. paramDbl[1] = Limit Move Value (eg 12)

	*/

	SetParam(0, ptSymbol, "");
	SetParam(1, ptLimitMoveValue, "12");

}

BOOL CIndicatorAccumulativeSwingIndex::Calculate()
{
	/*
		1. Validate the indicator parameters (if any)
		2. Validate available inputs
		3. Gather the inputs into a TA-SDK recordset
		4. Calculate the indicator
		5. If there is only one output, store the data
		   in the data_master array of this series. 
		   If there are two or more outputs, create new 
		   CSeriesStandard for each additional ouput
	*/


	// Get input from user
	if(!GetUserInput()) return FALSE;


	// Validate
	long size = pCtrl->RecordCount();
	if(size == 0) return FALSE;
//	Revision 6/10/2004 made by Katchei
//	Addition of type cast (int)
	if(paramStr.size() < (unsigned int)paramCount)
//	End Of Revision
		return FALSE;
	

	if(paramDbl[1] <= 0){
		//ProcessError("Invalid Limit Move Value for indicator " + szName);
		return FALSE;
	}
	
	// Get the data
	CField* pOpen = SeriesToField("Open", paramStr[0] + ".open", size);
	if(!EnsureField(pOpen, paramStr[0] + ".open")) return FALSE;
	CField* pHigh = SeriesToField("High", paramStr[0] + ".high", size);
	if(!EnsureField(pHigh, paramStr[0] + ".high")) return FALSE;
	CField* pLow = SeriesToField("Low", paramStr[0] + ".low", size);
	if(!EnsureField(pLow, paramStr[0] + ".low")) return FALSE;
	CField* pClose = SeriesToField("Close", paramStr[0] + ".close", size);	
	if(!EnsureField(pClose, paramStr[0] + ".close")) return FALSE;
	
 	CNavigator* pNav = new CNavigator();
	CRecordset* pRS = new CRecordset();
	CRecordset* pInd = NULL;

	pRS->addField(pOpen);
	pRS->addField(pHigh);
	pRS->addField(pLow);
	pRS->addField(pClose);

	pNav->setRecordset(pRS);


	// Calculate the indicator
	CIndex ta;
	pInd = ta.AccumulativeSwingIndex(pNav, pRS, paramDbl[1], szName);

	if(pCtrl->m_Language==1){
		szTitle = "IBA" + CString("("+paramDbl[1]+")");
	}
	else{
		szTitle = "ASI" + CString("("+paramDbl[1]+")");
	}
	
	// Output the indicator values
	Clear();
	CSeries* series = GetSeries(paramStr[0] + ".close");
	double value = 0, jdate = 0;
	for(int n = 0; n < size; ++n){
		value = pInd->getValue(szName, n + 1);
		jdate = series->data_master[n].jdate;
		AppendValue(jdate, value);
	}
 
	// Clean up
	delete pRS;
	delete pInd;	
	delete pNav;


	return CIndicator::Calculate();
}
