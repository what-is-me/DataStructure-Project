#include "BTree.h"

pair *lower_bound(pair *f, pair *l, int key) {//寻找大于等于key的最小记录位置
    for (; f != l; f++) {
        if (f->key >= key) break;
    }
    return f;
}

BTNode *BTNodeNew(BTNode *p) {//新建B树节点
    BTNode *ret = (BTNode *) malloc(sizeof(BTNode));
    memset(ret->child, 0, sizeof(ret->child));
    ret->p = p;
    ret->keyNum = 0;
    ret->isLeaf = 1;
    return ret;
}

void shiftBy(void *ptr, int length, int shift, int sz) {//ptr右移length位
    memmove(ptr + shift * sz, ptr, length * sz);
}

void split(BTNode *x, int pos) {//把pos孩子分成两个节点，都作为x的孩子
    BTNode *z = BTNodeNew(x);
    BTNode *y = x->child[pos];
    z->isLeaf = y->isLeaf;
    memmove(z->val, y->val + M, (M - 1) * sizeof(pair));
    if (!y->isLeaf) {
        memmove(z->child, y->child + M, M * sizeof(BTNode *));
        for (int i = 0; i < M; ++i)
            z->child[i]->p = z;
    }
    z->keyNum = y->keyNum = M - 1;
    shiftBy(x->child + pos + 1, x->keyNum - pos, 1, sizeof(BTNode *));  //注意child本身keyNum多一个
    x->child[pos + 1] = z;
    shiftBy(x->val + pos, x->keyNum - pos, 1, sizeof(pair));
    x->val[pos] = y->val[M - 1];
    ++x->keyNum;
}

int insertEmpty(BTNode *x, int key, Record val) {//在有空间容纳的B-树叶子节点插入记录
    while (TRUE) {
        int i = lower_bound(x->val, x->val + x->keyNum, key) - x->val;
        if (i != x->keyNum && key >= x->val[i].key) {
            return 0;
        }
        if (x->isLeaf) {
            shiftBy(x->val + i, x->keyNum - i, 1, sizeof(pair));
            x->val[i] = (pair) {.key = key, .rcd = val};
            ++x->keyNum;
            return 1;
        }
        if (x->child[i]->keyNum == 2 * M - 1) {
            split(x, i);
            if (x->val[i].key < key)
                ++i;
            else if (key >= x->val[i].key) {
                return 0;
            }
        }
        x = x->child[i];
    }
}

void merge(BTNode *x, int i) {//将x的i孩子与i+1个孩子合并，用x的i键作为分隔，这两个孩子都只有M-1个孩子，合并后有2*M-1个
    BTNode *y = x->child[i], *z = x->child[i + 1];
    y->keyNum = 2 * M - 1;
    y->val[M - 1] = x->val[i];
    memmove(y->val + M, z->val, (M - 1) * sizeof(pair));
    if (!y->isLeaf) {
        memmove(y->child + M, z->child, M * sizeof(BTNode *));
        for (int j = M; j <= 2 * M - 1; ++j)
            y->child[j]->p = y;
    }
    shiftBy(x->val + i + 1, x->keyNum - i - 1, -1, sizeof(pair));
    shiftBy(x->child + i + 2, x->keyNum - i - 1, -1, sizeof(BTNode *));
    --x->keyNum;
}

void erase(BTree *tr, BTNode *x, int key) {//从B树tr的节点x里删除key
    int i = lower_bound(x->val, x->val + x->keyNum, key) - x->val;
    if (i != x->keyNum && key >= x->val[i].key) {//找到key了
        if (x->isLeaf) {// x是叶节点，直接删除
            shiftBy(x->val + i + 1, --x->keyNum - i, -1, sizeof(pair));  //需要移动的内存是x->keyNum-i-1
        } else {
            if (x->child[i]->keyNum >= M) {//前驱所在孩子有足够的孩子(以应对它的孩子的需求)
                BTNode *y = x->child[i];
                while (!y->isLeaf)
                    y = y->child[y->keyNum];  //找前驱
                x->val[i] = y->val[y->keyNum - 1];
                erase(tr, x->child[i], x->val[i].key);
            } else if (x->child[i + 1]->keyNum >= M) {//后继所在孩子有足够的孩子
                BTNode *y = x->child[i + 1];
                while (!y->isLeaf)
                    y = y->child[0];  //找后继
                x->val[i] = y->val[0];
                erase(tr, x->child[i + 1], x->val[i].key);
            } else {//都没有,那么把这两个节点都合并到y中，并且挪动x的孩子和键
                merge(x, i);
                if (tr->root->keyNum == 0)  // keyNum==0只是没有键了，但是还可能有一个孩子，这时根变成这个孩子
                    tr->root = x->child[i], tr->root->p = NULL;
                erase(tr, x->child[i], key);
            }
        }
    } else if (!x->isLeaf)  //没有找到key,只要保证x->child[i]->keyNum足够多即可无脑递归，然而很难保证
    {
        if (x->child[i]->keyNum == M - 1) {
            BTNode *y = x->child[i];
            if (i >= 1 && x->child[i - 1]->keyNum >= M) {//左兄弟，取走它的最大孩子
                //找相邻的兄弟借节点，类似旋转操作,把x的一个键移入要删的key所在孩子，把它的兄弟的一个key和孩子移入x
                //但是从左还是右借并不完全一样，所以不能一概处理
                BTNode *z = x->child[i - 1];
                shiftBy(y->val, y->keyNum, 1, sizeof(pair));
                y->val[0] = x->val[i - 1];
                x->val[i - 1] = z->val[z->keyNum - 1];
                if (!y->isLeaf) {
                    shiftBy(y->child, y->keyNum + 1, 1, sizeof(BTNode *));
                    y->child[0] = z->child[z->keyNum], y->child[0]->p = y;
                }
                --z->keyNum, ++y->keyNum;
                erase(tr, y, key);
            } else if (i < x->keyNum && x->child[i + 1]->keyNum >= M) {//右兄弟,取走它的最小孩子
                BTNode *z = x->child[i + 1];
                y->val[y->keyNum] = x->val[i];
                x->val[i] = z->val[0];
                if (!y->isLeaf) {
                    y->child[y->keyNum + 1] = z->child[0], y->child[y->keyNum + 1]->p = y;
                    shiftBy(z->child + 1, z->keyNum, -1, sizeof(BTNode *));
                }
                shiftBy(z->val + 1, z->keyNum - 1, -1, sizeof(pair));
                --z->keyNum, ++y->keyNum;
                erase(tr, y, key);
            } else {//两个兄弟都没有节点借,那么将它与随便左右哪个兄弟合并，然而还是要特判一下
                if (i != 0)--i;
                y = x->child[i];
                merge(x, i);
                if (tr->root->keyNum == 0) {
                    free(tr->root);
                    tr->root = y, tr->root->p = NULL;
                }
                erase(tr, y, key);
            }
        } else
            erase(tr, x->child[i], key);
    }
}

BTree *BTreeNew() {//新建B-树
    BTree *ret = (BTree *) malloc(sizeof(BTree));
    ret->root = BTNodeNew(0);
    return ret;
}

int Insert(BTree *tr, int key, Record val) {//插入记录{key,val}
    if (tr->root->keyNum == 2 * M - 1) {
        BTNode *x = BTNodeNew(0);
        x->isLeaf = FALSE, x->child[0] = tr->root;
        tr->root->p = x, tr->root = x;
        split(x, 0);
    }
    return insertEmpty(tr->root, key, val);
}

Iterator Find(BTree *tr, int key) {//在B树tr中找到key所在节点以及位置
    BTNode *x = tr->root;
    int ret;
    while (x) {
        int i = lower_bound(x->val, x->val + x->keyNum, key) - x->val;
        if (x->val[i].key == key)
            return (Iterator) {i, 1, x};
        if (i != x->keyNum)
            ret = x->val[i].key;
        x = x->child[i];
    }
    return (Iterator) {0, 0, 0};
}


void Erase(BTree *tr, int key) {
    Iterator it = Find(tr, key);
    LessonDel(it.data->val[it.i].rcd.lessons);
    ActiveDel(it.data->val[it.i].rcd.activities);
    erase(tr, tr->root, key);
}

__attribute__((unused)) int StudentUpdate(BTree *tr, int key, Record rcd) {
    Iterator it = Find(tr, key);
    if (it.tag == 0)return 0;
    rcd.activities = it.data->val[it.i].rcd.activities;
    rcd.lessons = it.data->val[it.i].rcd.lessons;
    it.data->val[it.i].rcd = rcd;
    return 1;
}

void RecordPrint(Iterator it, FILE *file, int simple) {
    if (it.data == 0)return;
    int key = it.data->val[it.i].key;
    Record rcd = it.data->val[it.i].rcd;
    fprintf(file, "{\n");
    fprintf(file, "  \"key\": \"%06d\",\n", key);
    fprintf(file, "  \"name\": \"%s\",\n", isEmpty(rcd.name));
    fprintf(file, "  \"gender\": \"%s\",\n", isEmpty(rcd.gender));
    fprintf(file, "  \"profession\": \"%s\",\n", isEmpty(rcd.profession));
    fprintf(file, "  \"birth_year\": \"%d\",\n", rcd.birth_year);
    fprintf(file, "  \"enrollment_year\": \"%d\"", rcd.year);
    if (simple)fprintf(file, "\n");
    else {
        fprintf(file, ",\n");
        LessonTraversal(rcd.lessons, file);
        ActiveTraversal(rcd.activities, file);
    }
    fprintf(file, "}");
}

int btree_dfs_out_stu(BTNode *rt, FILE *file, int have_print_before, int simple) {//遍历B-树
    if (!rt) return have_print_before;
    for (int i = 0; i < rt->keyNum; ++i) {
        have_print_before |= btree_dfs_out_stu(rt->child[i], file, have_print_before, simple);
        if (have_print_before)fprintf(file, ",\n");
        RecordPrint((Iterator) {.i=i, .tag=1, .data=rt}, file, simple);
        have_print_before = 1;
    }
    return btree_dfs_out_stu(rt->child[rt->keyNum], file, have_print_before, simple);
}

void btree_dfs_ul(BTNode *rt, const char *filename) {
    if (!rt) return;
    for (int i = 0; i < rt->keyNum; ++i) {
        FILE *file = fopen(filename, "a+");
        fprintf(file, "%d %s %s %s %d %d\n",
                rt->val[i].key,
                isEmpty(rt->val[i].rcd.name),
                isEmpty(rt->val[i].rcd.profession),
                isEmpty(rt->val[i].rcd.gender),
                rt->val[i].rcd.birth_year,
                rt->val[i].rcd.year
        );
        fclose(file);
        LessonUnload(rt->val[i].rcd.lessons, filename);
        ActiveUnload(rt->val[i].rcd.activities, filename);

    }
    for (int i = 0; i <= rt->keyNum; ++i)btree_dfs_ul(rt->child[i], filename);
}

void BTreeUnload(BTree *tr, const char *filename) {
    fclose(fopen(filename, "w"));
    btree_dfs_ul(tr->root, filename);
}

BTree *BTreeLoad(const char *filename) {
    BTree *ret = BTreeNew();
    FILE *file = fopen(filename, "r");
    int key;
    Record rcd;
    while (~fscanf(file, "%d%s%s%s%d%d", &key,
                   rcd.name,
                   rcd.profession,
                   rcd.gender,
                   &rcd.birth_year,
                   &rcd.year)) {
        rcd.lessons = LessonLoad(file);
        rcd.activities = ActiveLoad(file);
        Insert(ret, key, rcd);
    }
    fclose(file);
    return ret;
}

Record RecordCreate() {
    return (Record) {
            .year=0,
            .birth_year=0,
            .activities=NULL,
            .lessons=LessonNew()
    };
}

void BTreeOutputStu(BTree *tr, FILE *file, int simple) {
    fprintf(file, "[");
    btree_dfs_out_stu(tr->root, file, 0, simple);
    fprintf(file, "]\n");
}

void BTNodeDel(BTNode *cur) {
    for (int i = 0; i < cur->keyNum; i++) {
        LessonDel(cur->val[i].rcd.lessons);
        ActiveDel(cur->val[i].rcd.activities);
    }
    if (!cur->isLeaf)
        for (int i = 0; i <= cur->keyNum; i++) {
            BTNodeDel(cur->child[i]);
        }
    free(cur);
}

void BTreeDel(BTree *tr) {
    BTNodeDel(tr->root);
    free(tr);
}

int BTreeQueryActivity(BTree *tr, char *name) {
    return BTree_dfs_activities(tr->root, name);
}

int BTree_dfs_activities(BTNode *rt, char *name) {
    int ret = 0;
    for (int i = 0; i < rt->keyNum; i++) {
        ret += (ActiveFind(rt->val[i].rcd.activities, name) != NULL);
    }
    if (!rt->isLeaf)
        for (int i = 0; i <= rt->keyNum; i++) {
            ret += BTree_dfs_activities(rt->child[i], name);
        }
    return ret;
}

LessonQueryData BTreeQueryLessons(BTree *tr, char *name) {
    LessonQueryData ret = BTree_dfs_lessons(tr->root, name);
    if(ret.tmp.sum==0)return ret;
    ret.ave = ret.tmp.sum / ret.tmp.num;
    ret.good = ret.tmp.good * 100.0 / ret.tmp.num;
    ret.passed = ret.tmp.passed * 100.0 / ret.tmp.num;
    return ret;
}

LessonQueryData BTree_dfs_lessons(BTNode *rt, char *name) {
    LessonQueryData ret = {.max=0, .min=100, .tmp={.sum=0, .num=0, .good=0, .passed=0}};
    for (int i = 0; i < rt->keyNum; i++) {
        double grade = LessonFind(rt->val[i].rcd.lessons, name).grade;
        if (grade >= 0) {
            ret.max = Max(ret.max, grade);
            ret.min = Min(ret.min, grade);
            ret.tmp.sum += grade;
            ret.tmp.num++;
            if (grade >= 60)ret.tmp.passed++;
            if (grade >= 90)ret.tmp.good++;
        }
    }
    if (!rt->isLeaf)
        for (int i = 0; i <= rt->keyNum; i++) {
            LessonQueryData res = BTree_dfs_lessons(rt->child[i], name);
            ret.max = Max(ret.max, res.max);
            ret.min = Min(ret.min, res.min);
            ret.tmp.sum += res.tmp.sum;
            ret.tmp.num += res.tmp.num;
            ret.tmp.passed += res.tmp.passed;
            ret.tmp.good += res.tmp.good;
        }
    return ret;
}
