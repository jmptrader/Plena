// IndicatorLinearRegressionForecast.h: interface for the CIndicatorLinearRegressionForecast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORLinearRegressionForecast_H__52A2F0F5_8BE9_4204_974B_1C3D63A4D013__INCLUDED_)
#define AFX_INDICATORLinearRegressionForecast_H__52A2F0F5_8BE9_4204_974B_1C3D63A4D013__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Indicator.h"

class CIndicatorLinearRegressionForecast : public CIndicator  
{
public:
	void SetParamInfo();
	CIndicatorLinearRegressionForecast(LPCTSTR name, int type, int members, CChartPanel* owner);
	virtual ~CIndicatorLinearRegressionForecast();
	BOOL Calculate();

};

#endif // !defined(AFX_INDICATORLinearRegressionForecast_H__52A2F0F5_8BE9_4204_974B_1C3D63A4D013__INCLUDED_)

