#ifndef UTILS_H
#define UTILS_H

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1 // 启用弱命名空间

#include <string>
#include <unordered_map>
#include <cryptopp/md5.h> // Crypto++ MD5 头文件
#include <cryptopp/hex.h> // Crypto++ 十六进制编码头文件

using namespace std;

namespace utils
{

    string stringToMD5(const string &input)
    {
        CryptoPP::Weak1::MD5 hash;
        CryptoPP::byte digest[CryptoPP::Weak1::MD5::DIGESTSIZE];                           // 修改这里，使用 CryptoPP::byte
        hash.CalculateDigest(digest, (const CryptoPP::byte *)input.c_str(), input.size()); // 修改这里，确保类型一致

        // 将哈希值转换为十六进制字符串
        string output;
        CryptoPP::HexEncoder encoder;
        encoder.Attach(new CryptoPP::StringSink(output));
        encoder.Put(digest, sizeof(digest));
        encoder.MessageEnd();

        return output;
    }

    unordered_map<string, string> initIndexMap(string title, string url, string id, string md5_id, string fetch_url)
    {
        unordered_map<string, string> index_map;
        index_map["fetch_url"] = fetch_url;
        index_map["md5_id"] = md5_id;
        index_map["id"] = id;
        index_map["url"] = url;
        index_map["title"] = title;
        return index_map;
    }

} // namespace utils

#endif // UTILS_H