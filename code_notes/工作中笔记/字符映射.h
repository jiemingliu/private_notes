#pragma once
#include "stdafx.h"
class KWMapper{
public:
	KWMapper(){
		_mapper.insert(pair<CString,CString>(_T("OILSAT"),_T("SOIL")));
		_mapper.insert(pair<CString,CString>(_T("GASSAT"),_T("SGAS")));
		_mapper.insert(pair<CString,CString>(_T("RATSOL"),_T("RS")));
		_mapper.insert(pair<CString,CString>(_T("WATSAT"),_T("SWAT")));
		_mapper.insert(pair<CString,CString>(_T("SOIL"),_T("OILSAT")));
		_mapper.insert(pair<CString,CString>(_T("SGAS"),_T("GASSAT")));
		_mapper.insert(pair<CString,CString>(_T("RS"),_T("RATSOL")));
		_mapper.insert(pair<CString,CString>(_T("SWAT"),_T("WATSAT")));
	}
	~KWMapper(){}
public:
	KWMapper(const KWMapper& mapper){_mapper = mapper._mapper;}
	KWMapper& operator=(const KWMapper& mapper){
		_mapper = mapper._mapper;
		return *this;
	}
	int setItem(CString hisim , CString ecl){
		if(_mapper.find(hisim) == _mapper.end()){
			return 1;
		}
		_mapper[hisim]=ecl;
		return 0;
	}
	CString& operator[](const CString& hisim ){
		return _mapper[hisim];
	}
	const CString& getItem(const CString& hisim)const{
		if(_mapper.find(hisim) != _mapper.end()){
			return _mapper.find(hisim)->second;
		}
		return hisim;
	}
	const CString& operator()(const CString& hisim){
		return getItem(hisim);
	}

private:
	std::map<CString,CString> _mapper;
};