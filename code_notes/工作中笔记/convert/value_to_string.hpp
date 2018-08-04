/********************************************************************
	created:	2017/01/10
	created:	10:1:2017   14:24
	file base:	data_convert
	file ext:	hpp
	author:		Caimq
	
	purpose:	数值转换为字符串
*********************************************************************/
#pragma once
#include <string>
#include <sstream>
#include <assert.h>

namespace TGS {
namespace Convert {

    inline static std::string toString(int value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    inline static std::string& trimZero( std::string& buff )
    {
        std::string::reverse_iterator rit = buff.rbegin();
        size_t i = 0;
        while ( *rit == '0')
        {
            ++i;
            ++rit;
        }
        if(*rit == '.')
        {
            ++i;
        }

        assert(i < buff.length());
        buff = buff.substr(0, buff.length() - i);

        return buff;
    }

    inline static std::string toString(float val, int nPrecision = 2, bool bIgnore = true)
    {
        std::stringstream ss;
        if (nPrecision == 0 && bIgnore)
        {	
            // 浮点转整型需四舍五入 [10/17/2014 caimq]
            int tmp = 0;
            if(val > 0)
            {
                tmp = (int)(val + 0.5f);
            }
            else
            {
                tmp = (int)(val - 0.5f);
            }
            ss << tmp;
            return ss.str();
        }
        ss.precision(nPrecision); 
        // 设置精度后，如果不使用_IOSfixed标志，一定会是科学计数方式效果
        // 因此，去尾0效果需额外处理
        ss.setf(_IOSdec|_IOSfixed);

        ss << val;
        std::string stmp = ss.str();
        if (bIgnore)
        {
            stmp = trimZero(stmp);
        }
        return stmp;
    }

} // Convert
} // TGS