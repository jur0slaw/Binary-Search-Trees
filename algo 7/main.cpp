#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int wielkosci[5]={25000,75000,125000,250000,500000};

int zliczanie=0;

struct node
{
  int key_value;
  node *left;
  node *right;
  node * parent;
};

class btree
{
    public:
        btree();
        ~btree();

        void insert(int key);
        node *search(int key);
        void delete_node(int key);
        void destroy_tree();

    private:
        void destroy_tree(node *leaf);
        void insert(int key, node *leaf);
        node *search(int key, node *leaf);
        void delete_node(node * leaf);
        node *succesor(node *leaf);
        node *root;
};

btree::btree()
{
  root=NULL;
}

btree::~btree()
{
  destroy_tree();
}

void btree::destroy_tree(node *leaf)
{
  if(leaf!=NULL)
  {
    if(leaf==root){
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        root=NULL;
    }else{
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        delete leaf;
    }
  }
}

void btree::insert(int key, node *leaf)
{

  if(key< leaf->key_value)
  {
    if(leaf->left!=NULL){
     zliczanie++;
     insert(key, leaf->left);
    }
    else
    {
      leaf->left=new node;
      leaf->left->key_value=key;
      leaf->left->left=NULL;    //Sets the left child of the child node to null
      leaf->left->right=NULL;   //Sets the right child of the child node to null
      leaf->left->parent=leaf;
    }
  }
  else if(key>=leaf->key_value)
  {
    if(leaf->right!=NULL){
      insert(key, leaf->right);
      zliczanie++;
    }
    else
    {
      leaf->right=new node;
      leaf->right->key_value=key;
      leaf->right->left=NULL;  //Sets the left child of the child node to null
      leaf->right->right=NULL; //Sets the right child of the child node to null
      leaf->right->parent=leaf;
    }
  }
}

node *btree::search(int key, node *leaf)
{

  if(leaf!=NULL)
  {
    if(key==leaf->key_value)
      return leaf;
    if(key<leaf->key_value){
        zliczanie++;
        return search(key, leaf->left);
    }
    else{
        zliczanie++;
        return search(key, leaf->right);
    }
  }
  else{ cout << "Nie znaleziono" << endl;
        return NULL;
  }
}

void btree::insert(int key)
{
  if(root!=NULL)
    insert(key, root);
  else
  {
    root=new node;
    root->key_value=key;
    root->left=NULL;
    root->right=NULL;
    root->parent=NULL;
  }
}

node *btree::succesor(node * leaf){
    node * temp = leaf;
    while(temp->left!=NULL){
        temp = temp->left;
    }
    return  temp;
}

void btree::delete_node(node * leaf){


    if(leaf->left==NULL && leaf->right==NULL){
            if(leaf->parent->left==leaf){
                leaf->parent->left=NULL;
            }else{
                leaf->parent->right=NULL;
            }
         delete leaf;
    }else {if(leaf->right==NULL){
            leaf->key_value=leaf->left->key_value;
            delete_node(leaf->left);
            zliczanie++;
        }else if(leaf->left==NULL){
            leaf->key_value=leaf->right->key_value;
            delete_node(leaf->right);
            zliczanie++;
        }else{
            node * temp = succesor(leaf->right);
            leaf->key_value=temp->key_value;
            delete_node(temp);
        }
    }
}

void btree::delete_node(int key){
    if(search(key)!=NULL){
        delete_node(search(key));
    }
}

node *btree::search(int key)
{
  return search(key, root);
}

void btree::destroy_tree()
{
  destroy_tree(root);
}

int main()
{
    btree drzewko;
    srand(time(NULL));
    int zakres = 500000;
    int lista[zakres];
    int temp;
    int temp2;
    fstream f;
    f.open("algo_7_dane.txt",ios::out);


    for(int i = 0;i<5;i++){
            f << "Dla rozmiaru: " << wielkosci[i] << endl << endl;
        for(int j = 0;j<20;j++){
            f << "Proba #" << j+1 << endl << endl;
            for(int k=0;k<wielkosci[i];k++){
                lista[k]=rand();

            }
            for(int k = 0; k<wielkosci[i];k++){
                drzewko.insert(lista[k]);
            }
            zliczanie=0;
            do{
                    temp2 = rand()%wielkosci[i];
                    temp = lista[temp2]+1;
            }while(temp==lista[temp2+1]);

            drzewko.insert(temp);

            f << "Liczba wywolan insert: " << zliczanie << endl;

            zliczanie = 0;

            drzewko.search(lista[temp2]);

            f << "Liczba wywolan search: " << zliczanie << endl;

            zliczanie = 0;

            drzewko.delete_node(lista[temp2]);

            f << "Liczba wywolan delete: " << zliczanie << endl;

            drzewko.destroy_tree();

        }
    }


    //cout << drzewko.search(lista[zakres/2]) << endl;



    f.close();
    return 0;
}
