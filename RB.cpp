#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<memory.h>
#include<iostream>
#include<string>

// -----------------------------------------------------------------------------------------------
// объяление объекта
// -----------------------------------------------------------------------------------------------

class RB{
    struct node{
		node *left;
		node *right;
		node *parent;  
		int value;
		bool color; // true - красная, false - черная 
		int height;
	};
	node *root;    		// корень
	int nodes_count; 	// число всех вершин 
	
	private:
	    node *New_Node(int value);	        // выделение новой вершины
	    void Erase_Node(node*);           	// удаление данной вершины
		void Clear(node *)					//рекурсиный снос дерева через подкорни 
	    static  node *Rotate_left(node*);  	// левое вращение в данной вершине
				node *Rotate_right(node*); 	// правое вращение в данной вершине 
	    
		bool Insert(int, node**);     		// вставка элемента из поддерева с таким корнем
	    bool Erase(int, node**);         	// удаление элемента из поддерева с таким корнем
	    bool GetMin(node**,node**);   		// нахождение и удаление максимального узла поддерева
		
		void Balance_Insert(node**);      	// баланисровка после вставки
	    bool Balance_Erase_left(node**);    // балансировка левого вращения
	    bool Balance_Erase_right(node**);   // балансировка правого вращения
	
	public:
	    RB();				// создание пустого древа
	    ~RB(); 				// снос дерева		
	    node Find(int);    	// поиск вершины с данным значением - выдает указатель на нее
	    void Insert(int);  	// вставка данного значения в дерево - ничего не выдает
	    void Erase(int);    // удаление вершины с данным значением - ничего не выдает
	    //void Show();      // демонстрация древа как массива
};

// -----------------------------------------------------------------------------------------------
// внутренний функционал: операции с деревом
// -----------------------------------------------------------------------------------------------

// выделение новой вершины
RB::node *RB::New_Node(int value) { 
    nodes_count++;
    node *node = new node;
    node->value = value;
    node->left = 0;
	node->right = 0;
    node->color = true;
    return node;
}

//удаление данной вершины
void RB::Erase_Node(node *node) { //удаление данной вершины
    nodes_count--;
    delete node;
}

//рекурсиный снос дерева через подкорни
void RB::Clear(node *node) { 
    if (!node) {
		return;
	};	 // если вдруг добрались до листьев - ничего не делай
    Clear(node->left);
    Clear(node->right);
    Erase_Node(node);
}

// левое вращение в данной вершине
RB::node *RB::Rotate_left(node *node) {
    node *left = node -> left;
    node *local = left -> right;
    right -> left = node;
    node -> right = local;
    return right;
}

// правое вращение в данной вершине
RB::node *RB::Rotate_right(node *node) {
    node *right = node -> right;
    node *local = right -> left;
    left -> right = node;
    node -> = local;
    return right;
}

// вставка элемента из поддерева с таким корнем
bool RB::Insert(int value, node **root) {
    node *node = *root;
    if (!node) {
		*root = NewNode(value);
	} else {
        if (value == node->value) {
			return true;
		}
        if (Insert(value, height, value<node->value?&node->p1:&node->p2)){ //!!!
			return true;
		}
        Balance_Insert(root);
    }
    return false;
}

// удаление элемента из поддерева с таким корнем
bool RB::Erase(int value, node **root) {
    node *t,*node=*root;
    if (!node) {
		return false;
	}
    if (node->value<value) {
        if (Erase(value, &node->right)) {
			return Balance_Erase_right(root);
		}
    } else if(node->value>value) {
        if (Erase(value, &node->left)){
			return Balance_Erase_left(root);
		}
    } else {
        bool res;
        if(!node->right) {
            *root = node->left;
            res =! node->color;
        } else {
            res = GetMin(&node->right,root);
            t = *root;
            t->color = node->color;
            t->left = node->left;
            t->right = node->right;
            if (res) {
				res = Balance_Erase_right(root);
			}
        }
        Erase_Node(node);
        return res;
    }
    return 0;
}

bool RB::GetMin(node **root, node **res) {
    node_st *node=*root;
    if (node->left) {
        if (GetMin(&node->left,res)) {
			return BalanceRemove_left(root);
		}
    } else {
        *root = node->right;
        *res = node;
        return !node->color;
    }
    return false;
}

// -----------------------------------------------------------------------------------------------
// внутренний функционал: отлабка и стабилизация дерева
// -----------------------------------------------------------------------------------------------

void RB::Balance_Insert(node **root) { // балансировка дерева после ввода
    node *left, *right, *left_color,*right_color;
    node *node = *root;
    if (node->color) {
    	return;	
	};
    left = node->left;
    right = node->right;
    
    // проверка на родитель-ребенок: красный-красный; и исправление   
    // случай левой ветви
    if (left and left->color) {
        right_color = left->right;
        if (right_color and right_color->color) {
        	left = node->left = Rotate_left(left);        	
		}
		right_color = left->left;
        if (left_color and left_color->color) {
            node->color = true;
            left->color = false;
            if (right and right->color) {
				// перекраска вершин 
                left_color->color = true;
                right->color = false;
                return;
            }
            *root = Rotate_right(node);
            return;
        }
    }
    // случай правой ветви
    if (right and right->color) {
        left_color = right->left;
		if (left_color and left_color->color) {
			right = node->right = Rotate_right(right);
		}
        right_color = right->right;
        if (right_color and right_color->color) {
            node->color = true;
            right->color = false;
            if(left and left->color) {
				// перекраска вершин 
                right_color->color = true;
                left->color = false;
                return;
            }
            *root = Rotate_left(node);
            return;
        }
    }
}

bool RB::Balance_Erase_left(node **root) {
    node *node = *root;
    node *left = node->left;
    node *right = node->right;
    if (left and left->color) {
        left->color = false;
		return false;
    }
    if (right and right->color) { // слцчай 1
        node->color=true;
        right->color=false;
        node = *root = Rotate_left(node);
        if (Balance_Erase_left(&node->left)) {
			node->left->color = false;
		}
        return false;
    }
    unsigned int mask = 0;
    node *rl=->p1;
    node *rr=p2->p2;
    if (rl and rl->color) {
		mask|=1;
	}
    if (rr and rr->color) {
    	mask|=2;
    }
    switch(mask) {
        case 0:     // случай 2 - if ((!rl || !rl->color) and (!rr || !rr->color))
            right->color = true;
            return true;
        case 1:
        
		case 2:     // случай 3 - if (p22 and p22->red)
            right->color = node->color;
            p22->color = node->color = false;
            *root = Rotate_right(node);	
        case 3:     // случай 4 - if (p21 and p21->red)
            right->color = true;
            rl->color = false;
            right = node->right = Rotate_left(right);
            rl = right->right;
    }
    return false;
}

bool RB::Balance_Erase_right(node **root) {
    node *node = *root;
    node *left = node->left;
    node *right = node->right;
    if (right and right->color) {
		right->color=false;
		return false;
	}
    if (left and left->color) { // слцчай 1
        node->color = true;
        left->color = false;
        node = *root = Rotate_right(node);
        if (BalanceRemove2(&node->right)){
			node->right->color = false;
		}
        return false;
    }
    unsigned int mask=0;
    node *ll = left->left;
    node *lr = left->right;
    if (ll and ll->color){
		mask|=1;
	}
    if (lr and lr->color){
		mask|=2;
	}
    switch(mask) {
        case 0:     // слцчай 2 - if((!lr || !lr->color) and (!ll || !ll->color))
            left->color=true;
            return true;
        case 1:     // слцчай 4 - if(ll and ll->color)
            left->color = node->color;
            ll->color = node->color = false;
            *root = Rotate_right(node);
    	case 2:
        	
        case 3:     // слцчай 3 - if(lr and lr->color)
            left->color = true;
            lr->color = false;
            left = node->left = Rotate_left(p1);
            ll = left->left;
    }
    return false;
}

// -----------------------------------------------------------------------------------------------
// пользовательские возможности
// -----------------------------------------------------------------------------------------------

// создание пустого древа
RB::RB() { 
    root = 0;
    nodes_count = 0;
}

// снос дерева
RB::~RB() {
    Clear(root); 
}

// поиск вершины с данным значением - выдает указатель на нее
int RB::Find(int value) {
    node *node = root;
    while(node) {
        if (node->value == value)
		return true;
        node = node->value>value?node->left:node->right;
    }
    return *node;
}

// вставка данного значения в дерево - ничего не выдает
void RB::Insert(int value) {
    Insert(value, &root);
    if (root) {
		root->color=false;
	}
}

// удаление вершины с данным значением - ничего не выдает
void RB::Erase(int value) {
    Erase(&root,value);
}

// снос дерева
void RB::Clear() {
    Clear(root);
    root = 0;
}

