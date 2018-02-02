//  Created by qtfreet00 on 2017/10/10.
//  Copyright © 2017年 qtfreet00. All rights reserved.
//

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
using std::string;

static void StringReplace(string &strBase, string strSrc, string strDes);

static void StringReplace(string &strBase, string strSrc, string strDes)
{
    string::size_type pos = 0;
    string::size_type srcLen = strSrc.size();
    string::size_type desLen = strDes.size();
    pos=strBase.find(strSrc, pos);
    while ((pos != string::npos))
    {
        strBase.replace(pos, srcLen, strDes);
        pos=strBase.find(strSrc, (pos+desLen));
    }
}

int main(int argc, const char * argv[]) {

    // insert code here...
    printf("010 Editor v8.0.1 MacOS x64 Patcher\n");
    printf("请输入010 editor v8.0.1 x64可执行文件路径(末尾不能有空格):\n");
    char path_s[256]={0};
    fgets(path_s,(sizeof path_s/sizeof path_s[0]),stdin);
    int len =strlen(path_s);
    if(path_s&&len>6){
        if(path_s[len-1]=='\n'){
            path_s[len-1]='\0';
        }
    }
    string path_t(path_s);
    StringReplace(path_t, "\\ ", " ");
    const char * path = path_t.c_str();

    printf("%s\n",path);
    FILE *fp= fopen(path, "r+");
    if(!fp){
        printf("读取文件错误!\n");
        return -1;
    }
    fseek(fp, 0,SEEK_END);
    unsigned int long fsize;
    fsize = ftell(fp);
    // printf("%ld\n",fsize);
    if(fsize!=7463520){
        printf("请拖入正确的文件，v8.0.1 x64版本\n");
        return -1;
    }
    rewind(fp);
    unsigned char *tmp = new unsigned char [fsize*sizeof(char)];
    memset(tmp,0,fsize);
    fread(tmp, sizeof(char), fsize, fp);
    fclose(fp);
    //d818e 13 01 --> db 00    
    //d8196 0F 85 B2 00 00 00 --> e9 b3 00 00 00 90
    int a = *(tmp+0xd818e);
    int b = *(tmp+0xd818f);
    int c = *(tmp+0xd8196);
    int d = *(tmp+0xd8197);
    int e = *(tmp+0xd8198);
    int f = *(tmp+0xd819b);
    if(a==0x13&&b==0x01&&c==0x0f&&d==0x85&&e==0xb2&&f==0){
        *(tmp+0xd818e)=0xdb;
        *(tmp+0xd818f)=0;
        *(tmp+0xd8196)=0xe9;
        *(tmp+0xd8197)=0xb3;
        *(tmp+0xd8198)=0;
        *(tmp+0xd819b)=0x90;
        FILE *out = fopen(path, "w");
        fwrite(tmp, sizeof(char), fsize, out);
        fflush(out);
        fclose(out);
        printf("patch文件成功\n");
        getchar();
        return 0;    }
    printf("文件可能已经被修改过，patch失败\n");
    getchar();
    return 0;
}
