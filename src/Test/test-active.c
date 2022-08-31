#include "Active.h"

int main() {
    FILE *f = fopen("test/active/in.txt", "r");
    Active *head = ActiveLoad(f);
    head = ActiveInsert(head, "sample99", 1);
    head = ActiveErase(head, "sample1");
    ActiveTraversal(head, stdout);
    ActiveUnload(head, "test/active/out.txt");
    fclose(f);
    ActiveDel(head);
}