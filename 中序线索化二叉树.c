#include <stdio.h>
#include <stdlib.h>

// 1. 定义数据类型和结构体
typedef int ElemType;

typedef struct ThreadNode 
{
    ElemType data;
    struct ThreadNode* lchild, * rchild;
    int ltag, rtag; // 标志位：0代表孩子指针，1代表线索指针
} ThreadNode, * ThreadTree;
// 全局变量 pre，指向当前访问结点的前驱
ThreadNode* pre = NULL;

// 2. visit 函数：建立线索的核心逻辑
void visit(ThreadNode* q)
{
    if (q->lchild == NULL) 
    { // 左子树为空，建立前驱线索
        q->lchild = pre;
        q->ltag = 1;
    }
    if (pre != NULL && pre->rchild == NULL) {
        pre->rchild = q;     // 建立前驱结点的后继线索
        pre->rtag = 1;
    }
    pre = q;
}

// 3. 中序遍历并线索化（对应图片右上角）
void InThread(ThreadTree T) 
{
    if (T != NULL) 
    {
        InThread(T->lchild); // 中序遍历左子树
        visit(T);            // 访问根节点（线索化）
        InThread(T->rchild); // 中序遍历右子树
    }
}

// 4. 主调用函数：创建中序线索化二叉树
void CreateInThread(ThreadTree T) 
{
    pre = NULL;              // pre 初始化为 NULL
    if (T != NULL)
    {         // 非空二叉树才能线索化
        InThread(T);         // 中序线索化二叉树
        if (pre->rchild == NULL)
        {
            pre->rtag = 1;   // 处理遍历的最后一个结点
        }
    }
}
//----辅助函数 ---

// 辅助：简单创建一个二叉树用于测试
// 结构：    1
//        /   \
//       2     3
ThreadTree CreateTestTree() 
{
    ThreadNode* n1 = (ThreadNode*)malloc(sizeof(ThreadNode));
    ThreadNode* n2 = (ThreadNode*)malloc(sizeof(ThreadNode));
    ThreadNode* n3 = (ThreadNode*)malloc(sizeof(ThreadNode));

    n1->data = 1; n1->ltag = 0; n1->rtag = 0;
    n2->data = 2; n2->ltag = 0; n2->rtag = 0;
    n3->data = 3; n3->ltag = 0; n3->rtag = 0;

    n1->lchild = n2; n1->rchild = n3;
    n2->lchild = NULL; n2->rchild = NULL;
    n3->lchild = NULL; n3->rchild = NULL;

    return n1;
}

// 辅助：利用线索化后的结果进行中序遍历（验证是否成功）
void InOrderTraverse(ThreadTree T)
{
    ThreadNode* p = T;
    while (p != NULL) 
    {
        // 找到中序开始的第一个节点（最左下）
        while (p->ltag == 0) p = p->lchild;

        printf("%d ", p->data);

        // 如果右标志是线索，则直接找后继
        while (p != NULL && p->rtag == 1) 
        {
            p = p->rchild;
            if (p) printf("%d ", p->data);
        }
        // 否则转到右子树
        if (p) p = p->rchild;
    }
}

int main() 
{
    ThreadTree T = CreateTestTree();

    printf("正在进行中序线索化...\n");
    CreateInThread(T);
    printf("线索化完成。\n");

    printf("通过线索中序遍历结果为: ");
    InOrderTraverse(T); // 应该输出 2 1 3
    printf("\n");

    //system("pause"); // 防止 VS 窗口一闪而过
    return 0;
}
