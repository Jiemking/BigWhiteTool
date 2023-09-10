#include "Encrypt.h"
#include "cJSON.h"
#include "http.h"
#include <iostream>
using namespace std;
char *host = "49.234.45.244";//官网
char *regaddr = "api/1000/bigwhite/reg";
char *logaddr = "api/1000/bigwhite/logon";
char *APPKey = "a277c9e28840d6f444df6ec6517634b4";
char *rc4key = "BIGWHITE2323971665ABCDEFGHJJKLMN";
char *imeipath = "/storage/emulated/0/imei";
char *kmpath = "/storage/emulated/0/flkm.txt";
char imei[50];
char *ztm;

char *calculateMD5(char *str) {
    char *aaa = str;
    unsigned char *bbb = (unsigned char *)aaa;
    MD5_CTX md5c;
    MD5Init(&md5c);
    int i;
    unsigned char decrypt[16];
    MD5Update(&md5c, bbb, strlen((char *)bbb));
    MD5Final(&md5c, decrypt);
    char *md5String = (char *)malloc(33); // Allocate memory for MD5 hash string (32 characters + null terminator)
    if (md5String != NULL) {
        for (i = 0; i < 16; i++) {
            sprintf(&md5String[i * 2], "%02x", decrypt[i]);
        }
        md5String[32] = '\0'; // Null-terminate the string
    }
    return md5String; // Return the MD5 hash string (remember to free this memory after use)
}


bool isValidTime(int clientTime) {
    time_t currentTime = time(NULL);
    int serverTime = static_cast<int>(currentTime);

    // Allow for a time difference of up to 30 seconds
    return abs(serverTime - clientTime) <= 30;
}

void printErrorAndExit(const char* message) {
    cout << "Error: " << message << endl;
    exit(1);
}


int Register()
{
    char username[50];
    char password[50];
    cout << endl << "请输入注册账号";
    cin >> username;
    cout << endl << "请输入注册账号:";
    cin >> password;
    char imei[50];
    cin >> imei;
    // 获取时间
    time_t t;
    t = time(NULL);
    int timedata = time(&t);

    // 合并微验提交数据
    char value[256];


    char post[256];
    sprintf(post, "account=%s&password=%s&udid=%s&time=%d", username, password, imei, timedata);

    char md5[256];
    sprintf(md5, "%s%s", post, APPKey);

    char sign[256];
    sprintf(sign, "&sign=%s", calculateMD5(md5));

    char data[256];
    sprintf(data, "data=%s", Encrypt(post, rc4key));

    char postdata[256];
    sprintf(postdata, "%s%s", data,sign);
    ztm = httppost(host, regaddr, postdata);

    cJSON *cjson = cJSON_Parse(ztm);
    int code = cJSON_GetObjectItem(cjson, "code")->valueint;
    int time = cJSON_GetObjectItem(cjson, "time")->valueint;
    char *msg = cJSON_GetObjectItem(cjson, "msg")->valuestring;
    cout << msg << endl;
    exit(0);
}
int Login()
{
    char username[50];
    char password[50];
    char defaultUsername[] = "123456";
    char defaultPassword[] = "123456";

    cout << "请输入账号 (直接回车使用默认值): ";
    cin.getline(username, sizeof(username));
    if (strlen(username) == 0) {
        strcpy(username, defaultUsername); // 使用默认值
    }
    cout << "请输入密码 (直接回车使用默认值): ";
    cin.getline(password, sizeof(password));
    if (strlen(password) == 0) {
        strcpy(password, defaultPassword); // 使用默认值
    }

    //自动获取设备码
    FILE * fpp;
    fpp=popen("getprop ro.serialno","r");
    fgets(imei,sizeof(imei),fpp);

    cout << "正在验证卡密中......." << endl;

    // 获取时间
    time_t currentTime;
    int clientTime;
    time(&currentTime);
    clientTime = static_cast<int>(currentTime);

    char post[256];
    snprintf(post, sizeof(post), "account=%s&password=%s&udid=%s&time=%d", username, password, imei, clientTime);

    char md5[256];
    snprintf(md5, sizeof(md5), "%s%s", post, APPKey);

    char sign[256];
    snprintf(sign, sizeof(sign), "&sign=%s", calculateMD5(md5));


    char data[256];
    snprintf(data, sizeof(data), "data=%s", Encrypt(post, rc4key));

    char postdata[512]; // Allowing some extra space
    snprintf(postdata, sizeof(postdata), "%s%s", data, sign);

    ztm = httppost(host, logaddr, postdata);


    cJSON* cjson = cJSON_Parse(ztm);
    if (!cjson) {
        printErrorAndExit("JSON解析失败");
    }

    int code = cJSON_GetObjectItem(cjson, "code")->valueint;
    int serverTime = cJSON_GetObjectItem(cjson, "time")->valueint;
    const char* msg = cJSON_GetObjectItem(cjson, "msg")->valuestring;

    if (code == 200) {
        if (!isValidTime(serverTime)) {
            printErrorAndExit("密码错了");
        }
        if (serverTime - clientTime > 30||serverTime - clientTime < -30)
        {
            printErrorAndExit("Decrypted data JSON parsing failed");
        }

        const char* encryptedData = cJSON_GetObjectItem(cjson, "data")->valuestring;
        char* decryptedData = Decrypt(encryptedData, rc4key); // Implement Decrypt function


        cJSON* datajson = cJSON_Parse(decryptedData);
        if (!datajson) {
            printErrorAndExit("Decrypted data JSON parsing failed");
        }

        const char* tokenState = cJSON_GetObjectItem(datajson, "tokenState")->valuestring;
        if (strcmp(tokenState, "y") == 0) {
            cJSON* infoObject = cJSON_GetObjectItem(datajson, "info");
            const char* uid = cJSON_GetObjectItem(infoObject, "uid")->valuestring;
            const char* vipExpDate = cJSON_GetObjectItem(infoObject, "vipExpDate")->valuestring;
            cout << "到期时间: " << vipExpDate << endl;


            std::tm tm;
            std::istringstream ss(vipExpDate);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            std::time_t vipExpTime = std::mktime(&tm);


            auto currentTime = std::chrono::system_clock::now();
            std::time_t currentTimestamp = std::chrono::system_clock::to_time_t(currentTime);

            if (vipExpTime < currentTimestamp) {
                std::cout << "会员已到期" << std::endl;
                return false; // Return false to indicate expired membership
            } else {
                showmenu=0x1000;
                std::cout << "登录成功" << std::endl;
                return true;
            }
        } else {
            cout << "登录设备过多，请解绑其他设备后登录" << endl;
        }
    } else {
        cout << msg << endl;
    }

    printErrorAndExit("Login failed");

}
