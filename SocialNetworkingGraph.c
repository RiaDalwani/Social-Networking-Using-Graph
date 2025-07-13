#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MXUSRS 100

typedef struct Frndnode {
    struct Usr *frnd;
    struct Frndnode *nxt;
} Frndnode;

typedef struct Usr {
    char usrname[50];
    Frndnode *frnds;
    int vstd;
    struct Usr *prnt;
} Usr;

typedef struct Ntwrk {
    Usr *usrs[MXUSRS];
    int usr_count;
} Ntwrk;

Ntwrk* crt_ntwrk() {
    Ntwrk *ntwrk = (Ntwrk *)malloc(sizeof(Ntwrk));
    ntwrk->usr_count = 0;
    return ntwrk;
}

Usr* fnd_usr(Ntwrk *ntwrk, const char *usrname) {
    for (int i = 0; i < ntwrk->usr_count; i++) {
        if (strcmp(ntwrk->usrs[i]->usrname, usrname) == 0) {
            return ntwrk->usrs[i];
        }
    }
    return NULL;
}

void add_usr(Ntwrk *ntwrk, const char *usrname) {
    if (ntwrk->usr_count >= MXUSRS) {
        printf("Max user limit reached.\n");
        return;
    }
    if (fnd_usr(ntwrk, usrname)) {
        printf("User %s already exists.\n", usrname);
        return;
    }
    Usr *new_usr = (Usr *)malloc(sizeof(Usr));
    strcpy(new_usr->usrname, usrname);
    new_usr->frnds = NULL;
    new_usr->vstd = 0;
    ntwrk->usrs[ntwrk->usr_count++] = new_usr;
    printf("User %s added successfully.\n", usrname);
}

int are_frnds(Usr *u1, Usr *u2) {
    Frndnode *current = u1->frnds;
    while (current) {
        if (current->frnd == u2) {
            return 1;
        }
        current = current->nxt;
    }
    return 0;
}

void add_frnd(Ntwrk *ntwrk, const char *usr1, const char *usr2) {
    Usr *u1 = fnd_usr(ntwrk, usr1);
    Usr *u2 = fnd_usr(ntwrk, usr2);
    if (!u1 || !u2) {
        printf("One or both users not found.\n");
        return;
    }
    if (u1 == u2) {
        printf("A user cannot be friends with themselves.\n");
        return;
    }
    if (are_frnds(u1, u2)) {
        printf("These users are already friends.\n");
        return;
    }
    Frndnode *f1 = (Frndnode *)malloc(sizeof(Frndnode));
    f1->frnd = u2;
    f1->nxt = u1->frnds;
    u1->frnds = f1;
    Frndnode *f2 = (Frndnode *)malloc(sizeof(Frndnode));
    f2->frnd = u1;
    f2->nxt = u2->frnds;
    u2->frnds = f2;
    printf("Friendship established between %s and %s.\n", usr1, usr2);
}

void remove_frnd(Ntwrk *ntwrk, const char *usr1, const char *usr2) {
    Usr *u1 = fnd_usr(ntwrk, usr1);
    Usr *u2 = fnd_usr(ntwrk, usr2);
    if (!u1 || !u2) {
        printf("One or both users not found.\n");
        return;
    }

    Frndnode* remove_frnd_from_list(Frndnode *head, Usr *usr) {
        Frndnode *prev = NULL, *curr = head;
        while (curr != NULL) {
            if (curr->frnd == usr) {
                if (prev) {
                    prev->nxt = curr->nxt;
                } else {
                    head = curr->nxt;
                }
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->nxt;
        }
        return head;
    }

    u1->frnds = remove_frnd_from_list(u1->frnds, u2);
    u2->frnds = remove_frnd_from_list(u2->frnds, u1);
    printf("Friendship removed between %s and %s.\n", usr1, usr2);
}

void get_frnds(Ntwrk *ntwrk, const char *usrname) {
    Usr *usr = fnd_usr(ntwrk, usrname);
    if (!usr) {
        printf("User not found.\n");
        return;
    }
    printf("Friends of %s:\n", usrname);
    Frndnode *current = usr->frnds;
    while (current) {
        printf("%s ", current->frnd->usrname);
        current = current->nxt;
    }
    printf("\n");
}

void get_mutual_frnds(Ntwrk *ntwrk, const char *usr1, const char *usr2) {
    Usr *u1 = fnd_usr(ntwrk, usr1);
    Usr *u2 = fnd_usr(ntwrk, usr2);
    if (!u1 || !u2) {
        printf("One or both users not found.\n");
        return;
    }
    printf("Mutual friends between %s and %s:\n", usr1, usr2);
    Frndnode *current1 = u1->frnds;
    while (current1) {
        Frndnode *current2 = u2->frnds;
        while (current2) {
            if (current1->frnd == current2->frnd) {
                printf("%s ", current1->frnd->usrname);
            }
            current2 = current2->nxt;
        }
        current1 = current1->nxt;
    }
    printf("\n");
}

void reset_vstd(Ntwrk *ntwrk) {
    for (int i = 0; i < ntwrk->usr_count; i++) {
        ntwrk->usrs[i]->vstd = 0;
        ntwrk->usrs[i]->prnt = NULL;
    }
}

void fnd_shortest_path(Ntwrk *ntwrk, const char *usr1, const char *usr2) {
    Usr *start = fnd_usr(ntwrk, usr1);
    Usr *end = fnd_usr(ntwrk, usr2);
    if (!start || !end) {
        printf("One or both users not found.\n");
        return;
    }

    reset_vstd(ntwrk);
    start->vstd = 1;
    Usr *queue[MXUSRS];
    int front = 0, rear = 0;
    queue[rear++] = start;

    while (front < rear) {
        Usr *current = queue[front++];
        if (current == end) break;

        Frndnode *frndnode = current->frnds;
        while (frndnode) {
            Usr *neighbor = frndnode->frnd;
            if (!neighbor->vstd) {
                neighbor->vstd = 1;
                neighbor->prnt = current;
                queue[rear++] = neighbor;
            }
            frndnode = frndnode->nxt;
        }
    }

    if (!end->vstd) {
        printf("No path found.\n");
        return;
    }

    Usr *path[MXUSRS];
    int path_length = 0;
    Usr *temp = end;
    while (temp) {
        path[path_length++] = temp;
        temp = temp->prnt;
    }

    printf("Shortest path between %s and %s:\n", usr1, usr2);
    for (int i = path_length - 1; i >= 0; i--) {
        printf("%s ", path[i]->usrname);
    }
    printf("\n");
}

void dsply_ntwrk(Ntwrk *ntwrk) {
    printf("\nSocial Network:\n");
    for (int i = 0; i < ntwrk->usr_count; i++) {
        Usr *usr = ntwrk->usrs[i];
        printf("%s: ", usr->usrname);
        Frndnode *current = usr->frnds;
        while (current) {
            printf("%s ", current->frnd->usrname);
            current = current->nxt;
        }
        printf("\n");
    }
}

int main() {
    printf("Welcome to FRIENDS: where people connect\n");
    Ntwrk *ntwrk = crt_ntwrk();
    int choice;
    char usrname1[50], usrname2[50];
    while (1) {
        printf("\n1. Add User\n2. Add Friend\n3. Remove Friend\n4. List Friends\n5. Mutual Friends\n6. Shortest Path\n7. Display Network\n8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter username to add: ");
                scanf("%s", usrname1);
                add_usr(ntwrk, usrname1);
                break;
            case 2:
                printf("Enter first username: ");
                scanf("%s", usrname1);
                printf("Enter second username: ");
                scanf("%s", usrname2);
                add_frnd(ntwrk, usrname1, usrname2);
                break;
            case 3:
                printf("Enter first username: ");
                scanf("%s", usrname1);
                printf("Enter second username: ");
                scanf("%s", usrname2);
                remove_frnd(ntwrk, usrname1, usrname2);
                break;
            case 4:
                printf("Enter username to list friends: ");
                scanf("%s", usrname1);
                get_frnds(ntwrk, usrname1);
                break;
            case 5:
                printf("Enter first username: ");
                scanf("%s", usrname1);
                printf("Enter second username: ");
                scanf("%s", usrname2);
                get_mutual_frnds(ntwrk, usrname1, usrname2);
                break;
            case 6:
                printf("Enter start username: ");
                scanf("%s", usrname1);
                printf("Enter target username: ");
                scanf("%s", usrname2);
                fnd_shortest_path(ntwrk, usrname1, usrname2);
                break;
            case 7:
                dsply_ntwrk(ntwrk);
                break;
            case 8:
                printf("Thank You, Hope you had a great experience at FRIENDS.\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
