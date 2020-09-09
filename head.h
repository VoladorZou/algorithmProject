#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
//数据结构
#define LENGTH 100
struct Node;//用前声明
  //孩子单链表的结构体
struct ListNode
{
	struct Node *pointToTree;//指向树结构体
	struct ListNode *next;
};
//树结点结构体定义
struct Node
{
	char id[20];//商品编号
	char name[20];//商品或分类的名称
	double price;//商品价格
	struct Node *Child[LENGTH];//树结构体指针数组存放子分类或子商品
	struct Node *parent;//记录父结点
	struct ListNode *childhead;//用单链表记录当前结点的所有不为空的子结点
}Goods[LENGTH];//数组Goods用于文件保存
//全局变量
Node *root, *current;
ListNode *Lroot;
int num = 0;//用于每次添加商品或分类时控制添加数目
int i = 0;//用作循环变量
int k = 0;//用于记录Add函数添加子结点的情况
int t = 0;
int select = 1;//功能选择变量
int subselect = 1;//子功能选择变量
FILE *fp;
char str[20];//用于存储查询时用户输入的中文名称
//函数声明
void LInitiate(ListNode **Lroot);
void Initiate(Node **root);
void Load();
void Visit(Node *ptr);
void Browse(void Visit(Node *ptr));
void Change_directory();
void Random_Change_directory();
void ChangeToRoot();
void Add();
void loadAdd();
void subSearchVisit(Node *ptr);
void SearchVisit(Node *ptr, void subSearchVisit(Node *ptr));
void Destroy(Node **root);
void Delete(Node *ptr);
void Modify();
void subModify(Node *ptr);
void Seek();
void subSaveVisit(Node *ptr);
void subSave(Node *root, void subSaveVisit(Node *ptr));
void subFileSaveVisit();
void subFileSave(Node *root, void subFileSaveVisit());
//菜单
void Menu()
{
	//系统要求
	  //1.至少3级目录，第一级目录不少于3个分类
	  //2.设计合适的交互界面
	  //3.管理系统数据的永久性保存
	while (select)
	{
		printf("————  简单商品分类管理系统  ————\n");
		printf("——1. 浏览全部          2. 切换目录——\n");
		printf("——3. 添加功能          4. 删除功能——\n");
		printf("——5. 修改功能          6. 查找功能——\n");
		printf("——0. 退出              7. 回到根目录—\n");
		printf("请输入你的选择：\n");
		scanf("%d", &select);
		switch (select)
		{
		case 1:
			Browse(Visit);
			printf("\n");
			break;
		case 2:
			Change_directory();
			break;
		case 3:
			printf("请输入您打算添加的商品或分类的数目：\n");
			scanf("%d", &num);
			for (int j = 0; j < num; j++)
			{
				Add();
			}
			break;
		case 4:
			printf("请输入要删除的当前目录下某个子分类或某个商品信息：\n");
			scanf("%s", str);
			ListNode *pL, *s;
			Node *p;
			pL = current->childhead;
			while (pL->next != NULL)
			{
				s = pL;
				pL = pL->next;
				p = pL->pointToTree;
				if (strcmp(str,p->name)==0)
				{
					Delete(p);
					s->next = pL->next;
					printf("删除成功！\n");
					break;
				}
			}
			break;
		case 5:
			Modify();
			break;
		case 6:
			Seek();
			break;
		case 7:
			ChangeToRoot();
			break;
		case 0: break;
		default: printf("输入错误！请重新选择。\n");
		}
	}
}
//孩子单链表的初始化
void LInitiate(ListNode **Lroot)
{
	*Lroot = (ListNode *)malloc(sizeof(ListNode));
	(*Lroot)->pointToTree = NULL;
	(*Lroot)->next = NULL;
}
//树链表的初始化
void Initiate(Node **root)
{
	*root = (Node *)malloc(sizeof(Node));
	strcpy((*root)->id, "");
	strcpy((*root)->name, "商品根目录");
	(*root)->price = 0.0;
	for (i = 0; i < LENGTH; i++)
	{
		(*root)->Child[i] = NULL;
	}
	(*root)->parent = NULL;
	LInitiate(&(*root)->childhead);
}
//从文件中加载已有数据到链表中
void Load()
{
	if ((fp = fopen("Goods.txt", "wb+")) == NULL)
	{
		printf("打开文件出错！\n");
		exit(1);
	}
	//未知文件长度，使用feof(FILE *Stream)判断文件结尾
	//feof(fp)通过fread/fscanf是否读写出错判断，应该在读写函数之后进行判断，否则多进行一轮循环
	while (!feof(fp))
	{
		if (fread(&Goods[t], sizeof(struct Node), 1, fp) != 0)
		{
			if (Goods[t].parent != NULL)
			{
				printf("%d\n", t);
				//上一次操作完成，程序关闭之后，期间所占用的内存被系统回收；
				  //那么parent指针所指向的内容就没有被保留了，所以会出错。
				current = Goods[t].parent;
				printf("%s\n", current->name);
				loadAdd();
			}
			t++;
		}
	}
	fclose(fp);
}
//查看功能函数
  //浏览当前商品分类目录的所有内容（子分类和当前目录下的商品）
void Browse(void Visit(Node *ptr))
{
	Visit(current);
	for (i = 0; i < LENGTH; i++)
	{
		if (current->Child[i] != NULL)
		{
			Visit(current->Child[i]);
		}
	}
}
//访问结点函数
void Visit(Node *ptr)
{
	if (ptr != NULL)
	{
		if (strcmp(ptr->id, "") != 0)
		{
			printf("%s ", ptr->id);
			printf("%8s  ", ptr->name);
		}
		if (strcmp(ptr->id, "") == 0)
		{
			printf("%s: ", ptr->name);
		}
		if (strcmp(ptr->name, "商品根目录") == 0)
		{
			printf("\n");
		}
		if (ptr->price != 0)
		{
			printf("%6.2lf\n", ptr->price);
		}
		if (ptr->price == 0)
		{
			printf("\n");
		}
	}
}
//遍历函数
  //查找的访问
void SearchVisit(Node *root, void subSearchVisit(Node *ptr))
{
	if (root != NULL)
	{
		subSearchVisit(root);
		/*for (i = 0; i < LENGTH; i++)
		{
			if (root->Child[i] != NULL)
			{
				printf("  %s", root->Child[i]->name);
				SearchVisit(root->Child[i], subSearchVisit);
			}
		}*/
		ListNode *pL;
		Node *p;
		pL = root->childhead;
		while (pL->next!=NULL)
		{
			pL = pL->next;
			p = pL->pointToTree;
			SearchVisit(p,subSearchVisit);
		}
	}
}
void subSearchVisit(Node *ptr)
{
	if (strcmp(str, ptr->name) == 0)
	{
		current = ptr;
	}
	if (strcmp(str, ptr->id) == 0)
	{
		current = ptr;
	}
}
//切换目录函数
  //切换当前分类目录到上一级分类目录或下一级分类目录（扩展：切换到任何一个目录）或直接切换到根目录
void Change_directory()
{
	subselect = 1;
	while (subselect)
	{
		printf("————  切换分类目录的子菜单  ————\n");
		printf("——1. 到上一层          2. 到下一层——\n");
		printf("——3. 到根目录          4. 随意切换——\n");
		printf("——0. 退出菜单\n");
		printf("请输入你的选择：\n");
		scanf("%d", &subselect);
		switch (subselect)
		{
		case 1:
			current = current->parent;
			printf("已跳转到指定目录:\n");
			Browse(Visit);
			break;
		case 2:
			printf("请输入打算切换到的下一层目录名称：\n");
			scanf("%s", str);
			ListNode *p;
			p = current->childhead;
			p = p->next;
			while (p != NULL)
			{
				if (strcmp(str, p->pointToTree->name) == 0)
				{
					current = p->pointToTree;
					break;
				}
				p = p->next;
			}
			if (strcmp(str, p->pointToTree->name) == 0)
			{
				printf("已跳转到指定目录:\n");
				Browse(Visit);
			}
			/*
			for (i = 0; i < LENGTH; i++)
			{
				if (current->Child[i] != NULL)
				{
					if (strcmp(str, current->Child[i]->name) == 0)
					{
						current = current->Child[i];
						printf("已跳转到指定目录:\n");
						Browse(Visit);
					}
				}
			}*/
			break;
		case 3:
			ChangeToRoot();
			subselect = 0;
			break;
		case 4:
			Random_Change_directory();
			subselect = 0;
			break;
		case 0: break;
		default: printf("输入错误！请重新选择。\n");
		}
	}
}
void Random_Change_directory()
{
	printf("请输入打算切换到的分类名称：\n");
	scanf("%s", str);
	SearchVisit(root, subSearchVisit);
	if (strcmp(str, current->name) == 0)
	{
		printf("已跳转到指定目录:\n");
		Browse(Visit);
	}
}
void ChangeToRoot()
{
	current = root;
	if (strcmp(current->name, "商品根目录") == 0)
	{
		printf("已跳转到指定目录:\n");
		Browse(Visit);
	}
}
//添加新商品信息函数
  //在当前分类目录下添加新的商品目录，或者添加新商品信息
void loadAdd()
{
	Node *q;//用于申请新的树结点
	ListNode *p, *qL;
	q = (Node *)malloc(sizeof(Node));
	qL = (ListNode *)malloc(sizeof(ListNode));
	strcpy(q->id, Goods[t].id);
	strcpy(q->name, Goods[t].name);
	q->price = Goods[t].price;
	LInitiate(&q->childhead);
	p = current->childhead;
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = qL;
	qL->pointToTree = q;
	qL->next = NULL;
	q->parent = current;//指向父结点
	current->Child[k] = q;
	for (i = 0; i < LENGTH; i++)
	{
		q->Child[i] = NULL;
	}
	k++;
}
void Add()
{
	subselect = 1;
	Node *q;//用于申请新的树结点
	ListNode *p, *qL;
	q = (Node *)malloc(sizeof(Node));
	qL = (ListNode *)malloc(sizeof(ListNode));
	printf("输入1代表添加商品,输入2代表添加目录：\n");
	scanf("%d", &subselect);
	if (subselect == 1)
	{
		printf("请输入商品编号：\n");
		scanf("%s", q->id);
	}
	if (subselect == 2)
	{
		strcpy(q->id, "");
	}
	printf("请输入新的商品或分类名称：\n");
	scanf("%s", q->name);
	if (subselect == 1)
	{
		printf("请输入商品价格：\n");
		scanf("%lf", &q->price);
	}
	if (subselect == 2)
	{
		q->price = 0.0;
	}
	LInitiate(&q->childhead);
	p = current->childhead;
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = qL;
	qL->pointToTree = q;
	qL->next = NULL;
	q->parent = current;//指向父结点
	current->Child[k] = q;
	for (i = 0; i < LENGTH; i++)
	{
		q->Child[i] = NULL;
	}
	k++;
	printf("添加成功！\n");
	subselect = 1;
}
//删除商品信息函数
  //在当前目录下删除某个子商品分类或某个商品信息
void Delete(Node *ptr)
{
	Destroy(&ptr);
	for ( i = 0; i < LENGTH; i++)
	{
		if (current->Child[i]!=NULL)
		{
			if (strcmp(ptr->name, current->Child[i]->name) == 0)
			{
				current->Child[i] = NULL;
			}
		}
	}
}
void Destroy(Node **root)
{
	for (i = 0; i < LENGTH; i++)
	{
		if ((*root) != NULL && (*root)->Child[i] != NULL)
		{
			Destroy(&(*root)->Child[i]);
		}
	}
	free(*root);
}
//修改商品信息函数
  //在当前目录下修改某个商品或者分类目录的信息
void Modify()
{
	subselect = 1;
	while (subselect == 1)
	{
		printf("请输入在当前目录下要修改的某个商品或者分类目录的名称：\n");
		scanf("%s", str);
		if (strcmp(str, current->name) == 0)
		{
			subModify(current);
			printf("修改成功！\n");
			subselect = 2;
		}
		for (i = 0; i < LENGTH; i++)
		{
			if (current->Child[i] != NULL)
			{
				if (strcmp(str, current->Child[i]->name) == 0)
				{
					subModify(current->Child[i]);
					printf("修改成功！\n");
					subselect = 2;
				}
			}
		}
		if (subselect==2)
		{
			break;
		}
		printf("当前目录下没有找到你要修改的信息，请重新输入。\n");
	}
	subselect = 1;
}
void subModify(Node *ptr)
{
	printf("请输入新的商品或分类名称：\n");
	scanf("%s", ptr->name);
	if (strcmp("", ptr->id) != 0)
	{
		printf("请输入编号：\n");
		scanf("%s", ptr->id);
		printf("请输入商品价格：\n");
		scanf("%lf", &ptr->price);
	}
}
//查找函数
  //根据某个商品编号或名称在整个系统中查找某个商品并显示它的全部信息
void Seek()
{
	current = root;
	printf("请输入要查找的商品编号或名称：【1——根据编号查询，2——根据名称查询】\n");
	scanf("%d", &subselect);
	if (subselect == 1)
	{
		printf("请输入要查找的商品的编号：\n");
		scanf("%s", str);
		SearchVisit(current, subSearchVisit);
		printf("查找到的数据内容是：\n");
		Visit(current);
	}
	else
		if (subselect == 2)
		{
			printf("请输入要查找的商品的名称：\n");
			scanf("%s", str);
			SearchVisit(current, subSearchVisit);
			printf("查找到的数据内容是：\n");
			Visit(current);
		}
		else printf("输入错误！\n");
	subselect = 1;
}
//将树结构的结点信息存入数组中
void subSaveVisit(Node *ptr)
{
	strcpy(Goods[i].id, ptr->id);
	strcpy(Goods[i].name, ptr->name);
	Goods[i].price = ptr->price;
	//Goods->Child[i] = ptr->Child[i];
	//Goods[i].childhead = ptr->childhead;
	Goods[i].parent = ptr->parent;
	i++;
}
void subSave(Node *root, void subSaveVisit(Node *ptr))
{
	if (root != NULL)
	{
		subSaveVisit(root);
		ListNode *pL;
		Node *p;
		pL = root->childhead;
		while (pL->next != NULL)
		{
			pL = pL->next;
			p = pL->pointToTree;
			subSave(p, subSaveVisit);
		}
	}
}
//保存链表数据到文件
void Save()
{
	//每次保存都应把树结构体数组Goods初始回“0”的状态；
	  //以免因为商品或分类减少时，没有对以前存放数据的地方归零。
	for ( i = 0; i < LENGTH; i++)
	{
		strcpy(Goods[i].id, "");
		strcpy(Goods[i].name, "");
		Goods->price = 0.0;
		for ( int j = 0; j < LENGTH; j++)
		{
			Goods[i].Child[j] = NULL;
		}
		Goods[i].childhead = NULL;
		Goods[i].parent = NULL;
	}
	  //第一步先将树结构的结点信息存入数组中
	current = root;
	i = 0;
	subSave(current, subSaveVisit);
	if ((fp = fopen("Goods.txt", "wb+")) == NULL)
	{
		printf("打开文件出错！\n");
		exit(1);
	}
	  //第二步将数组信息存入文件，永久保存
	current = root;
	i = 0;
	subFileSave(current, subFileSaveVisit);
	fclose(fp);
	printf("成功保存数据到文件\n");
}
//将数组信息存入文件中
void subFileSaveVisit()
{
	if (fwrite(&Goods[i], sizeof(struct Node), 1, fp) != 1)
	{
		printf("写入文件出错！\n");
	}
	i++;
}
void subFileSave(Node *root, void subFileSaveVisit())
{
	if (root != NULL)
	{
		subFileSaveVisit();
		ListNode *pL;
		Node *p;
		pL = root->childhead;
		while (pL->next != NULL)
		{
			pL = pL->next;
			p = pL->pointToTree;
			subFileSave(p, subFileSaveVisit);
		}
	}
}