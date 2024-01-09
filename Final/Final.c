/*プログラムの流れ
1. dataファイルからN人の解答をトレース
2. rangeファイルからli,siを一つずつ読み込み
3. di=samelist(li,si)とし、samelistはbit列のマスクを使用し照合
Aさんと一致する場合、その人数を配列diに記録
プログラム上はdi=d,li=x,si=yである
ここで一般に、不明なsamelist(li,si),既知のsamelist(lk,sk)に対し、
li=lk,si>skの場合、samelist(li,si)はsamelist(lk,sk)の部分集合
li+si=lk+sk,si<skの場合も同様なので、例えば
samelist(7,15)を知りたくてsamelist(16,6)がわかっているなら
samelist(16,6)*samelist(7,9)で出せる
samelist(7,4)が既知なら
samelist(7,15)=samelist(16,6)*samelist(7,4)*samelist(11,5)
というようにどんどん絞り込むことができる
d3=d1*d2とは、d3[i]=d1[i]*d2[i]となる演算とする(diの各要素は0か1)
探索のため、この既知のsamelistの二分木と配列を２つ作成する。１つはliの値の二分木にsiの値の配列、
もう1つはli+siの値の二分木にsiの値の配列
4. answerとして出力するのはsize(di)である
*/

#include <stdio.h>
#include <stdlib.h>
#include "Index.h"

Item NULLitem = {-1, "NULL"};

static link head1x, head2x, z;//zは葉、head1,head2は二分木の根
link STNEW(Item x, Item y, link l1x, link r1x, link l1y, link r1y,int* d){ //二分木に新たな接点を挿入
    link l = malloc(sizeof *l); 
    //printf("link made\n");//for debug
    l->x = x; l->y = y; l->l1x = l1x; l->r1x = r1x; l->l1y = l1y; l->r1y = r1y; l->d=d;
    return l;
}
//リンクを作成
void STinit(){ //二分木の初期化
    z = STNEW(NULLitem ,NULLitem, NULL, NULL, NULL, NULL,NULL);
    head1x = STNEW(NULLitem,NULLitem,z,z,z,z,NULL); 
    head2x = STNEW(NULLitem,NULLitem,z,z,z,z,NULL);
    //printf("ST1,2初期化完了\n");//for debug
}

link search1(link h, Key x, Key y, link hMax){
    //printf("hのデータ:(%d,%d,%d),z=%d\n",&h,key(h->x),key(h->y),&z);//for debug
    if(h==head1x){
        //printf("headから移動しました\n");//for debug
        return search1(h->r1x,x,y,hMax);
    }
    if(h==z){
        //printf("探索不成功\n");//for debug
        return NULL;
    }
    Key t=key(h->x);
    if eq(x, t){
        Key u=key(h->y);
        if eq(y,u) {
            return h;
        }
        if (h->l1y==z&&h->r1y==z){
            //printf("左右がどちらもz\n");//for debug
            if(key(h->y)<=u){
                //printf("探索結果：(%d,%d)\n",key(h->x),key(h->y));//for debug
                return h;
            }
            else {
                //printf("探索結果：(%d,%d)\n",key(hMax->x),key(hMax->y));//for debug
                return hMax;
            }
        }
        if less(y,u) {
            //printf("y(%d)<u(%d)\n",y,u);//for debug
            return search1(h->l1y,x,y,hMax);
        }
        else {
            //printf("y(%d)>u(%d)\n",y,u);//for debug
            return search1(h->r1y,x,y,h);
        }
    }
    if less(x, t) {
        //printf("x(%d)<t(%d)\n",x,t);//for debug
        return search1(h->l1x, x, y, NULL);
    }
    else {
        //printf("x(%d)>t(%d)\n",x,t);//for debug
        return search1(h->r1x, x, y ,NULL);
    }
}

link search2(link h, Key x, Key y, link hMax){
    //printf("hのデータ:(%d,%d,%d),z=%d\n",&h,key(h->x),key(h->y),&z);//for debug
    if(h==head2x){
        //printf("headから移動しました\n");//for debug
        return search2(h->r1y,x,y,hMax);
    }
    if(h==z){
        //printf("探索不成功\n");//for debug
        return NULL;
    }
    Key t=key(h->x)+key(h->y);
    if eq(x+y, t){
        Key u=key(h->y);
        if eq(y,u) return h;
        if (h->l1y==z&&h->r1y==z){
            //printf("左右がどちらもz\n");//for debug
            if(key(h->y)<=u){
                //printf("探索結果：(%d,%d)\n",h->x,h->y);//for debug
                return h;
            }
            else {
                //printf("探索結果：(%d,%d)\n",hMax->x,hMax->y);//for debug
                return hMax;
            }
        }
        if less(y,u) return search2(h->l1y,x,y,hMax);
        else return search2(h->r1y,x,y,h);
    }
    if less(x, t) return search2(h->l1x, x, y, NULL);
    else return search2(h->r1x, x, y ,NULL);
}


link ST1search(Key x,Key y){
    //printf("Search1開始\n");//for debug
    return search1(head1x, x, y, head1x);
}
link ST2search(Key x,Key y){
    //printf("Search2開始\n");//for debug
    return search2(head2x, x, y, head2x);
}
link insert(link h,Item itemx, Item itemy,int* d){
    if(h==head1x){
        //printf("head1から移動しました\n");//for debug
        h->r1x=insert(h->r1x,itemx,itemy,d);
    }
    if(h==head2x){
        //printf("head2から移動しました\n");//for debug
        h->r1x=insert(h->r1x,itemx,itemy,d);
    }
    if(h==z){
        return STNEW(itemx,itemy,z,z,z,z,d);
    }
    Key x=key(itemx),y=key(itemy);
    Key nowx=key(h->x),nowy=key(h->y);
    if(x==nowx){
        if less(y,nowy){
            h->l1y=insert(h->l1y,itemx,itemy,d);
        }
        else{
            h->r1y=insert(h->r1y,itemx,itemy,d);
        }
    }
    else if less(x,nowx){
        h->l1x=insert(h->l1x,itemx,itemy,d);
    }
    else{
        h->r1x=insert(h->r1x,itemx,itemy,d);
    }
    return h;
}
void STinsert(Key x,Key y,int* d,int mode){
    if(mode==1){
        Item X={x,""},Y={y,""};
        head1x=insert(head1x,X,Y,d);
        //printf("ST1に挿入完了\n");//for debug
    }
    else{
        Item X={x+y,""},Y={y,""};
        head2x=insert(head2x,X,Y,d);
        //printf("ST2に挿入完了\n");//for debug
    }
    return;
}

int ITEMscan(Key *x) 
{ return scanf("%d", x); }  

void ITEMshow(Item x) 
{ printf("%3d ", key(x)); }  

int* VectorMul(int* x,int N,int* y,int M){
    if (N!=M){
        printf("Error: VectorMulに渡された引数の2配列の要素数が等しくありません");
        return NULL;
    }
    int* ans;
    ans=(int*)malloc(sizeof(int)*N);
    for(int i=0;i<N;i++){
        ans[i]=x[i]*y[i];
    }
    return ans;
}

int main(int argc, char *argv[]){
    if(argc<4){//コマンドライン引数は４つ
        printf("ファイル名の指定数が不足しています。データファイル、範囲ファイル、解答ファイルの順に指定してください。");
        return -1;
    }
    char *dataFileName=argv[1];
    char *rangeFileName=argv[2];
    char *answerFileName=argv[3];

    int N=1000;
    int M=10000;

    char dummy;//邪魔な文字を読み込みずらすため
    Key x,y;

    int* Asan;
    int** data;//dataメモリの動的確保
    Asan=(int*)malloc(sizeof(int)*M);
    data=(int**)malloc(sizeof(int*)*N);
    for(int i=0;i<N;i++){
        data[i]=(int*)malloc(sizeof(int)*M);
    }

    FILE *dataFile=fopen(dataFileName,"r");//dataファイルopen
    if(dataFile==NULL){
        printf("データファイルを開けませんでした。");
        return -1;
    }
    Asan=(int*)malloc(sizeof(int)*M);
    for(int i=0;i<M;i++){
        Asan[i]=(int)fgetc(dataFile)-48;
        //printf("%d",Asan[i]); //for debug
    }
    //printf("\n");//for debug
    for(int i=0;i<N;i++){//配列に値を入力
        dummy=(int)fgetc(dataFile);
        for(int j=0;j<M;j++){
            data[i][j]=(int)fgetc(dataFile)-48;
            //if(i==0) printf("(%d,",data[i][j]);    //for debug
            if(data[i][j]==Asan[j]){//質問がAさんと一致なら１，そうでなければ０
            data[i][j]=1;
            }
            else data[i][j]=0;
            //if(i==0) printf("%d)",data[i][j]);    //for debug
        }
        //printf("\n"); //for debug
    }
    fclose(dataFile);

    char Model[7];
    Key a,b;
    link h1=malloc(sizeof *h1),h2=malloc(sizeof *h2);//探索用
    int dcount;
    link L1=malloc(sizeof *L1),L2=malloc(sizeof *L2);//最後の挿入時に使用
    STinit();

    FILE *rangeFile=fopen(rangeFileName,"r");//rangeファイルopen
    if(rangeFile==NULL){
        printf("範囲ファイルを開けませんでした。");
        return -1;
    }
    FILE *answerFile=fopen(answerFileName,"w");//answer file open
    if(answerFile==NULL){
        printf("解答ファイルを開けませんでした。");
        return -1;
    }
    fgets(Model,7,rangeFile);
    dummy=fgetc(rangeFile);
    while(fscanf(rangeFile,"%d",&x)!=EOF){//rangeを取得し、その結果を返す
        dummy=fgetc(rangeFile);
        fscanf(rangeFile,"%d",&y);
        //printf("今回のrange:(%d,%d)\n",x,y); //for debug
        int* d1=malloc(sizeof(int)*N);//ansのdata
        int* d2=malloc(sizeof(int)*N);//作らなければならないdata
        for(int i=0;i<N;i++) {
            d1[i]=1;
            d2[i]=1;
            //printf("%d",d1[i]);//for debug
        }
        a=x;b=y;
        dcount=0;
        while(b>0){//いざ、探索
            //printf("探索を開始します\n");//for debug
            h1=ST1search(a,b);//printf("Search1完了\n");//for debug
            h2=ST2search(a,b);//printf("Search2完了\n");//for debug
            if(h1==NULL&&h2==NULL)break;//両方hitなしなら一から自作なのでbreak
            if(h2==NULL){
                d1=VectorMul(d1,N,h1->d,N);
                a=a+key(h1->y);
                b=b-key(h1->y);
                //printf("ST1採択");//for debug
            }
            else if(h1==NULL){
                d1=VectorMul(d1,N,h2->d,N);
                b=b-key(h2->y);
                //printf("ST2採択");//for debug
            }
            else if(key(h1->y)>=key(h2->y)){//ST1採択
                d1=VectorMul(d1,N,h1->d,N);
                a=a+key(h1->y);
                b=b-key(h1->y);
                //printf("ST1採択");//for debug
            }
            else{//ST2採択
                d1=VectorMul(d1,N,h2->d,N);
                b=b-key(h2->y);
                //printf("ST2採択");//for debug
            }
        }
        if(b<0){
            printf("Error:search missed");
            return -1;
        }
        if(b>0){//探索だけでansのdataを作り上げられなかった場合、自作
            for(int i=0;i<N;i++){
                for(int j=0;j<b;j++){
                    d2[i]=d2[i]*=data[i][a+j];
                }
            }
            //printf("自作した部分のdataをinsertします\n");//for debug
            STinsert(a,b,d2,1);//作成した残りのdataをinsert
            STinsert(a,b,d2,2);
            //printf("insert completed\n");//for debug
            d1=VectorMul(d1,N,d2,N);
        }
        for(int i=0;i<N;i++){
            dcount+=d1[i];
        }
        //printf("dcount=%d\n",dcount);//for debug
        if(a!=x||b!=y){
            //printf("もとのdataをinsertします\n");//for debug
            STinsert(x,y,d1,1);//ansのdataをinsert
            STinsert(x,y,d1,2);
            //printf("insert completed\n");//for debug
        }
        fprintf(answerFile,"%d, %d, %d\n",x,y,dcount);
    
            //printf("ここまでOK\n");//for debug
    }
    printf("正常終了\n");
    fclose(rangeFile);
    fclose(answerFile);
    free(head1x);
    free(head2x);
    return 0;
}