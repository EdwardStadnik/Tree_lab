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
    public:
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
	    node *new_node(int value);	        // выделение новой вершины
	    void erase_node(node*);           	// удаление данной вершины
		void clear(node *);					// рекурсиный снос дерева через подкорни 
	    static  node *rotate_left(node*);  	// левое вращение в данной вершине
				node *rotate_right(node*); 	// правое вращение в данной вершине
		void show(node*);			    // рекурсивный вывод на экран дерева
	    
		node* find(node*, int);				// поиск элемента с таким значением в поддереве с таким корнем
		bool insert(int, node**);     		// вставка элемента из поддерева с таким корнем
	    bool erase(int, node**);         	// удаление элемента из поддерева с таким корнем
	    bool getmin(node**,node**);   		// нахождение и удаление максимального узла поддерева
		
		void balance_insert(node**);      	// баланисровка после вставки
	    bool balance_erase_left(node**);    // балансировка левого вращения
	    bool balance_erase_right(node**);   // балансировка правого вращения
	
	public:
	    RB();				// создание пустого древа
	    ~RB(); 				// снос дерева		
	    node* find(int);    // поиск вершины с данным значением - выдает указатель на нее
	    void insert(int);  	// вставка данного значения в дерево - ничего не выдает
	    void erase(int);    // удаление вершины с данным значением - ничего не выдает
	    void clear();		// полный снос дерева
	    void show();      	// демонстрация древа как массива
};

// -----------------------------------------------------------------------------------------------
// внутренний функционал: операции с деревом
// -----------------------------------------------------------------------------------------------

// выделение новой вершины
RB::node *RB::new_node(int value) { 
    nodes_count++;
    node* node = new RB::node;
    node->value = value;
    node->left = 0;
	node->right = 0;
    node->color = true;
    return node;
}

//удаление данной вершины
void RB::erase_node(node *node) { //удаление данной вершины
    nodes_count--;
    delete node;
}

//рекурсиный снос дерева через подкорни
void RB::clear(node *node) { 
    if (!node) {
		return;
	};	 // если вдруг добрались до листьев - ничего не делай
    clear(node->left);
    clear(node->right);
    erase_node(node);
}

// левое вращение в данной вершине
RB::node *RB::rotate_left(node* node) {
    RB::node *right = node -> right;
    RB::node *local = right -> left;
    right -> left = node;
    node -> right = local;
    return right;
}

// правое вращение в данной вершине
RB::node *RB::rotate_right(node *node) {
    RB::node *left = node -> left;
    RB::node *local = left -> right;
    left -> right = node;
    node -> left = local;
    return left;
}

// рекурсивный вывод на экран дерева
void RB::show(node* node)	{
	if (!node) {
		return;
	}
	bool color = node->color;
	if (color){
		std::cout << " - " << node->value << " red " << " - ";
	}
	if (!color){
		std::cout << " - " << node->value << " black " << " - ";
	}
	show(node->left);
	show(node->right);
}

// поиск элемента с таким значением в поддереве с таким корнем
node* RB::find(node* node, int value) {
    while (node) {
        if (node->value == value) {
			return *node;
        	node = node->value>value?node->left:node->right;
    	}
    }
}

// вставка элемента из поддерева с таким корнем
bool RB::insert(int value, node **root) {
    node *node = *root;
    if (!node) {
		*root = new_node(value);
	} else {
        if (value == node->value) {
			return true;
		}
        if (insert(value, value<node->value? &node->left: &node->right)){ //!!!
			return true;
		}
        balance_insert(root);
    }
    return false;
}

// удаление элемента из поддерева с таким корнем
bool RB::erase(int value, node **root) {
    node *t,*node=*root;
    if (!node) {
		return false;
	}
    if (node->value<value) {
        if (erase(value, &node->right)) {
			return balance_erase_right(root);
		}
    } else if(node->value>value) {
        if (erase(value, &node->left)){
			return balance_erase_left(root);
		}
    } else {
        bool res;
        if(!node->right) {
            *root = node->left;
            res =! node->color;
        } else {
            res = getmin(&node->right,root);
            t = *root;
            t->color = node->color;
            t->left = node->left;
            t->right = node->right;
            if (res) {
				res = balance_erase_right(root);
			}
        }
        erase_node(node);
        return res;
    }
    return 0;
}

bool RB::getmin(node **root, node **res) {
    node *node = *root;
    if (node->left) {
        if (getmin(&node->left,res)) {
			return balance_erase_left(root);
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

void RB::balance_insert(node **root) { // балансировка дерева после ввода
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
        	left = node->left = rotate_left(left);        	
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
            *root = rotate_right(node);
            return;
        }
    }
    // случай правой ветви
    if (right and right->color) {
        left_color = right->left;
		if (left_color and left_color->color) {
			right = node->right = rotate_right(right);
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
            *root = rotate_left(node);
            return;
        }
    }
}

bool RB::balance_erase_left(node **root) {
    RB::node *node = *root;
    RB::node *left = node->left;
    RB::node *right = node->right;
    if (left and left->color) {
        left->color = false;
		return false;
    }
    if (right and right->color) { // слцчай 1
        node->color=true;
        right->color=false;
        node = *root = rotate_left(node);
        if (balance_erase_left(&node->left)) {
			node->left->color = false;
		}
        return false;
    }
    unsigned int mask = 0;
    RB::node *rl = right->left;
    RB::node *rr = right->right;
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
            rr->color = node->color = false;
            *root = rotate_right(node);	
        case 3:     // случай 4 - if (p21 and p21->red)
            right->color = true;
            rl->color = false;
            right = node->right = rotate_left(right);
            rl = right->right;
    }
    return false;
}

bool RB::balance_erase_right(node **root) {
    RB::node *node = *root;
    RB::node *left = node->left;
    RB::node *right = node->right;
    if (right and right->color) {
		right->color=false;
		return false;
	}
    if (left and left->color) { // слцчай 1
        node->color = true;
        left->color = false;
        node = *root = rotate_right(node);
        if (balance_erase_right(&node->right)){
			node->right->color = false;
		}
        return false;
    }
    unsigned int mask=0;
    RB::node *ll = left->left;
    RB::node *lr = left->right;
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
            *root = rotate_right(node);
    	case 2:
        	
        case 3:     // слцчай 3 - if(lr and lr->color)
            left->color = true;
            lr->color = false;
            left = node->left = rotate_left(left);
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
    clear(root); 
}

// поиск вершины с данным значением - выдает указатель на нее
RB::node find(int value) {
    return find(root, value);
}

// вставка данного значения в дерево - ничего не выдает
void RB::insert(int value) {
    insert(value, &root);
    if (root) {
		root->color=false;
	}
}

// удаление вершины с данным значением - ничего не выдает
void RB::erase(int value) {
    erase(value, &root);
}

// снос дерева
void RB::clear() {
    clear(root);
    root = 0;
}


// демонстрация древа как массива
void RB::show() {
    show(root);
}
