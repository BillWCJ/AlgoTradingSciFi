#ifndef AT_Header_Curl_H_INCLUDED
#define AT_Header_Curl_H_INCLUDED

//----------------------------------------Definitions - Curl-----------------------------
//--------------------Debug Data
struct data {
    char trace_ascii;// 1 or 0
};

//--------------------Write Function Storage
struct MemoryStruct {
    char *memory;
    size_t size;
};
//---------------------------------------------------------------------------------------

#endif // AT_Header_Curl_H_INCLUDED
