#include "model_readdisc.h"

/*
 *用文件模拟磁盘，文件的组织方式：
 *因为实验模拟的是有128个块，每个块有64个字节的磁盘，最小单位也就是字节
 *因此我把文件分成128行，每一行用64个8bit整数表示，每个字符用空格隔开。
 *
 *
 *int readByte(int blockId,int byteId);
 *从磁盘读取第blockId个块的第byteId个字节并返回  失败返回WRONG_RETURN = 0xFFFF
 *
 *int writeByte(int blockId,int byteId,int byte);
 * 把byte写进buf2
 *
 *int writeBuf2Disc();
 * 把buf2的内容写进disc
 *
 *
 *
 *
 *
 */

Model_ReadDisc::Model_ReadDisc(const char *s): buf1_block(RESET),discPath(s)
{
    openFile(READ_MODE,s);
    loadBuf2();               //打开文件并把disc.txt文件读入缓冲区buf2
}

int Model_ReadDisc::readByte(int blockId,int byteId) {
    if(blockId < buf2_LEN && byteId < buf1_LEN) {
        loadBuf1(blockId);
        return buf1[byteId];
    }
    else {
        printf("out of index\n");
        return WRONG_RETURN;
    }
}

int Model_ReadDisc::writeByte(int blockId,int byteId,int byte) {
    if(buf1_block == blockId)
       buf1_block = RESET;             //如果读指针指向当前缓冲区  RESET buf1_block
    buf2[blockId][byteId] = byte;

    return TRUE_RETURN;
}


int Model_ReadDisc::loadBuf2() {  //disc.txt文件全部读入buf2
    int ch;
    for(int j = 0;j < buf2_LEN;++j) {
        int num = 0,i = 0;
        while((ch = fgetc(fp)) != '\n' && ch != EOF) {      //ch != EOF加一下比较好
            if(ch != ' ') {
                num = num*10 + ch - '0';
            }
            else {
                buf2[j][i++] = num;
                num = 0;
           }
        }
        buf2[j][i++] = num;
        if(i != buf1_LEN) return WRONG_RETURN;
    }
    return TRUE_RETURN;
}


int Model_ReadDisc::writeBuf2Disc() {
    /*
     *关闭文件  重置打开方式(w+模式打开)
     *再把buf2全部写入rootPath
     *
     */

    fclose(fp);
    openFile(WRITE_MODE,discPath);
    for(int j = 0;j < buf2_LEN;++j) {
        for(int i = 0;i < buf1_LEN;++i) {
            int num = buf2[j][i];
            int z = 0;
            if(num == 0) {
                buf1[z++] = 0;
            }
            else {
                while(num != 0) {
                    buf1[z++] = num % 10;
                    num /= 10;
                }
            }
            while(z--) {
                if(fputc(buf1[z] + '0',fp) == EOF) {
                    printf_s("\nFunction(writeBuf2Disc) has something wrong  P1 %c\n",buf2[j] + '0');
                    return WRONG_RETURN;
                }
            }
            if(i != buf1_LEN - 1) {
                int c = fputc(' ',fp);
                if(c == EOF) {
                    printf_s("\nFunction(writeBuf2Disc) has something wrong  P2 \n");
                    return WRONG_RETURN;
                }
            }
        }
        fputc('\n',fp);
    }
    return TRUE_RETURN;
}





int * Model_ReadDisc::loadBuf1(int blockId) {
    if(blockId == buf1_block)            //检查要读的块和当前缓冲区的块相不相等
        return buf1;
    buf1_block = blockId;
    for(int i = 0;i < buf1_LEN;++i)
        buf1[i] = buf2[blockId][i];
    return buf1;
}

int * Model_ReadDisc::getPoniter(int blockId,int byteId) {
    return &buf2[blockId][byteId];
}
/*
 *
 *
 *
 */
void Model_ReadDisc::showBuf(int blockId) {
    printf("BLOCK_NUM %d:",blockId);
    for(int j = 0;j < buf1_LEN;++j) {
        printf("%d ",buf2[blockId][j]);
    }
    printf("\n");
}

int Model_ReadDisc::openFile(int mode,const char * s) {
    if(mode == READ_MODE) {
        fp = fopen(s,"r");
    }
    else  if ( WRITE_MODE){
        fp = fopen(s,"w+");
    }
    if(fp != nullptr) {
        printf("Disc(%s) is successfully open (in mode %d)!\n",s,mode);
        return 1;
    }
    else {
        printf("Disc(%s) has something wrong! (in mode %d) ! \n",s,mode);
        return 0;
    }
}

void Model_ReadDisc::close() {
    writeBuf2Disc();
    fclose(fp);
}




/*
 *
 *
 * 没用到的函数
void Model_ReadDisc::toBlock(int blockId) {//把读写指针指到指定的行
    rewind(fp);
    while(blockId > 0) {
        while(fgetc(fp) != '\n');
        --blockId;
    }
}

void Model_ReadDisc::loadBuf1(int a[]) {
    for(int i = 0;i < buf1_LEN; ++i) {
        buf1[i] = a[i];
    }
    buf1_block = -1;                              //缓存buf1变了  重置一下保存的缓存块的id
}


*/

