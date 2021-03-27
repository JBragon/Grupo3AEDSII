#include <stdio.h>
#include <stdlib.h>

struct item
{
    int cod
};

typedef struct item Item;

struct node {
    Item item;
    struct node *left;
    struct node *right;
};

typedef struct node Node;

Node *init(){
    return NULL;
}

Item itemCreate(int cod)
{
    Item item;
    item.cod = cod;
    return item;
}

Node *insert(Node *root, Item x){

    if(root == NULL)
    {

        Node *aux = (Node *)malloc(sizeof(Node));
        aux->item = x;
        aux->left = NULL;
        aux->right = NULL;

        return aux;
    }
    else
    {
        if(x.cod > root->item.cod)
        {
            root->right = insert(root->right, x);
        }
        else
        {
            root->left = insert(root->left, x);
        }
    }

    return root;

}

void print(Node *root)
{
    if(root != NULL)
    {
        printf("%d \n", root->item.cod);
        print(root->left);
        print(root->right);
    }
}

void getWinner(Node vetor[])
{
    Node *aux = (Node *)malloc(sizeof(Node));

    for(int i = 0; i < sizeof(vetor); i++)
    {
        if(i%2 == 0)
        {
            *aux->right = vetor[i];
        }
        else
        {
            *aux->left = vetor[i];
        }
    }

    if(aux->left > aux->right)
    {
        aux->item = aux->right->item;
    }
    else
    {
        aux->item = aux->left->item;
    }

    printf("Vencedor %d \n", &aux->item.cod);
}

int main()
{
    //Node *root = init();

    int numeros[] = {10, 4};

    Node vetor[2];

    for(int i = 0; i < sizeof(numeros); i++)
    {
        Node *aux = (Node *)malloc(sizeof(Node));
        aux->item = itemCreate(numeros[i]);
        aux->left = NULL;
        aux->right = NULL;

        vetor[i] = *aux;
    }

    getWinner(vetor);

    // root = insert(root, itemCreate(9));
    // root = insert(root, itemCreate(25));
    // root = insert(root, itemCreate(666));
    // root = insert(root, itemCreate(333));
    // root = insert(root, itemCreate(45));
    // root = insert(root, itemCreate(5));

    // print(root);

    return 0;
}


