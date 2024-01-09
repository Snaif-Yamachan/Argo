typedef int Key;//keyはint型
typedef struct { Key key; char information[10]; } Item; //Item型はkeyとinfoの組
typedef struct Node* link;
struct Node{
    Item x,y;//x=li,y=si
    link l1x,r1x;//二分木のxの位置
    link l1y,r1y;//二分木のyの配列
    int* d;//data
};

#define key(A) (A.key)

#define less(A, B) (A < B)
#define exch(A, B) { Item t = A; A = B; B = t; } 
#define compexch(A, B) if (less(B, A)) exch(A, B)
#define eq(A, B) (A == B)

int ITEMscan(Key *);
void ITEMshow(Item);

void ST1init();
void ST2init();
int ST1count(void);
int ST2count(void);
void STinsert(Key,Key,int*,int);
link ST1search(Key,Key);
link ST2search(Key,Key);
Item STselect(int);