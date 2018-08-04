/// <summary>
/// Author： Huangl
/// 转换托管字符串至非托管字符串
/// 转换到ANSI编码
/// </summary>
/// <param name="strObj"></param>
/// <returns></returns>
inline std::string CLIStrToStdStr(System::String^ strObj)
{
    using namespace System::Runtime::InteropServices;
    auto ptr = Marshal::StringToHGlobalAnsi(strObj);
    std::string str(static_cast<char*>(ptr.ToPointer()));
    Marshal::FreeHGlobal(ptr);
    return str;
}