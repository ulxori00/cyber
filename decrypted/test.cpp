#include <iostream>

using namespace std;

struct tree_node_t {
	tree_node_t* parent;
	tree_node_t* brother;

	tree_node_t* child;		// Albo child albo value
	int value;				// Jesli value != NULL to jest lisciem
};

tree_node_t* addNode(tree_node_t* parent, int value);
void removeNode(tree_node_t* root, tree_node_t* node);
void insertDigitToTree(tree_node_t* parent, tree_node_t** current, int digit);
tree_node_t* initTree();
void printTree(tree_node_t root);
int countChildren(tree_node_t* root);
int countNodes(tree_node_t* root);
void minimTree(tree_node_t* root);
bool checkEqialTree(tree_node_t* rootA, tree_node_t* rootB);
tree_node_t* copyTree(tree_node_t* copy);
tree_node_t* findNode(tree_node_t* root, int number);
int sizeTree(tree_node_t* root);
void deleteTree(tree_node_t** root, int size);
int bruteForce(tree_node_t* rootA, tree_node_t* rootB);

int signUnsignedNodes(tree_node_t* rootB, int number);
int tabSum(int** tab, int* taken, int sizeA, int sizeB, int currentA, int currentB);
int maxTabSum(int** tab, int sizeA, int sizeB);
int tableSollution(tree_node_t* rootA, tree_node_t* rootB, int size);

tree_node_t* addNode(tree_node_t* parent, int value)			// Nie zajmuje sie korzeniem
{
	tree_node_t* new_node = (tree_node_t*)malloc(sizeof(tree_node_t));

	if (new_node != NULL)
	{
		// Ustawienie wartosci nowego wezla
		new_node->parent = parent;
		new_node->value = value;

		new_node->brother = NULL;
		new_node->child = NULL;

		// Ustawienie wartosci rodzica/starszego brata

		if (parent != NULL)
		{
			tree_node_t* p;

			if (parent->child != NULL)
			{
				p = parent->child;
				while (p->brother != NULL) p = p->brother;
				p->brother = new_node;
			}
			else parent->child = new_node;
		}
		return new_node;
	}
	else return NULL;
}

void removeNode(tree_node_t* root, tree_node_t* node)
{
	if (node == NULL) {}
	else if (node->value > 0)
	{
		if (node->parent != NULL)
		{
			if (node->parent->child == node) node->parent->child = node->brother;
			else
			{
				tree_node_t* p = node->parent->child;
				while (p->brother != node) p = p->brother;
				p->brother = node->brother;
			}
			free(node);
		}
		minimTree(root);
	}
	else {
		tree_node_t* child = node->child;
		node->value = node->child->value;
		if (node->child->value == NULL || node->child->value <= 0)
		{
			tree_node_t* p = node->child->child;
			while (p != NULL)
			{
				p->parent = node;
				p = p->brother;
			}
		}
		node->child = node->child->child;
		free(child);
	}
}

void insertDigitToTree(tree_node_t* parent, tree_node_t** current, int digit)		// Uzupelnienie funkcji initTree
{
	if (*current == NULL) *current = addNode(parent, digit); 
	else (*current)->value = ((*current)->value * 10) + digit;
}

tree_node_t* initTree()
{
	tree_node_t* root = NULL;
	tree_node_t* parent = NULL;
	tree_node_t* current = NULL;
	
	char z;
	int number = 0;
	do
	{
		z = cin.get();

		switch (z)
		{
		case '0': insertDigitToTree(parent, &current, 0);	break;
		case '1': insertDigitToTree(parent, &current, 1);	break;
		case '2': insertDigitToTree(parent, &current, 2);	break;
		case '3': insertDigitToTree(parent, &current, 3);	break;
		case '4': insertDigitToTree(parent, &current, 4);	break;
		case '5': insertDigitToTree(parent, &current, 5);	break;
		case '6': insertDigitToTree(parent, &current, 6);	break;
		case '7': insertDigitToTree(parent, &current, 7);	break;
		case '8': insertDigitToTree(parent, &current, 8);	break;
		case '9': insertDigitToTree(parent, &current, 9);	break;
		case ',': current = NULL;							break;	// Opuszcza flage
		case '(': parent = addNode(parent, NULL);			break;	// Tworzy wezel pod 'parent' i wezel staje sie nowym 'parent' 
		case ')': parent = parent->parent;					break;	// Konczy wprowadzaniw dziecka, parent idzie o jedno wyzej 
		}

		if (root == NULL)
		{
			if (parent != NULL) root = parent;
			else root = current;
		}

	} while (z != ';');


	return root;
}

void printTree(tree_node_t root)
{
	if (root.value == NULL || root.value <= 0) cout << '[';
	else cout << root.value;

	tree_node_t* p = root.child;
	while (p != NULL)
	{
		printTree(*p);

		p = p->brother;
		if (p != NULL) cout << '.';
	}

	if (root.value == NULL || root.value <= 0) cout << ']';
}

int countChildren(tree_node_t* root)
{
	if (root->value > 0) return 0;
	else
	{
		int count = 0;
		tree_node_t* p = root->child;
		while (p != NULL)
		{
			count++;
			p = p->brother;
		}
		return count;
	}
}

int countNodes(tree_node_t* root)
{
	int size = 1;

	tree_node_t* p = root->child;
	while (p != NULL)
	{
		size += countNodes(p);
		p = p->brother;
	}
	return size;
}

void minimTree(tree_node_t* root)
{
	while ((root->value == NULL || root->value <= 0) && countChildren(root) <= 1) removeNode(NULL, root);
	
	tree_node_t* p = root->child;
	while (p != NULL)
	{
		minimTree(p);
		p = p->brother;
	}
}

bool checkEqialTree(tree_node_t* rootA, tree_node_t* rootB)
{
	bool eqial = 1;

	minimTree(rootA);
	minimTree(rootB);

	if (rootA->value != rootB->value) eqial = 0;
	else
	{
		tree_node_t* p = rootA->child;
		while (p != NULL)
		{
			bool eq = 0;
			tree_node_t* q = rootB->child;
			while (q != NULL)
			{
				if (checkEqialTree(p, q))
				{
					eq = 1;
					break;
				}

				q = q->brother;
			}

			if (eq != 1) eqial = 0;

			p = p->brother;
		}
	}

	return eqial;
}

tree_node_t* copyTree(tree_node_t* copy)
{
	tree_node_t* root = addNode(NULL, copy->value);

	tree_node_t* p = copy->child;
	tree_node_t* q = NULL;
	while (p != NULL)
	{
		tree_node_t* tmp = copyTree(p);

		tmp->parent = root;
		tmp->brother = NULL;

		if (root->child == NULL) root->child = tmp;
		else q->brother = tmp;
		q = tmp;

		p = p->brother;
	}

	return root;
}

tree_node_t* findNode(tree_node_t* root, int number)
{
	tree_node_t* result = NULL;
	if (root->value == number) return root;
	
	tree_node_t* p = root->child;
	while (p != NULL)
	{
		result = findNode(p, number);
		if (result != NULL) return result;
		p = p->brother;
	}
	return result;
}

int sizeTree(tree_node_t* root)
{
	int size = 0;

	tree_node_t* p = root->child;
	while (p != NULL)
	{
		if (p->value > 0) size++;
		else size += sizeTree(p);
		p = p->brother;
	}
	return size;
}

void deleteTree(tree_node_t** root, int size)
{
	for(int i = 0; i < size; i++) removeNode(*root, findNode(*root, i + 1));
	free((*root)->child);
	free(*root);
}

int bruteForce(tree_node_t* rootA, tree_node_t* rootB, int size)
{
	bool* remove_nodes = (bool*)malloc(size * sizeof(bool));
	for (int i = 0; i < size; i++) *(remove_nodes + i) = 0;

	int max = 0;

	int stop = 0;
	while (stop == 0)
	{
		
		tree_node_t* A = copyTree(rootA);
		tree_node_t* B = copyTree(rootB);
		for (int i = 0; i < size; i++)
		{
			if (*(remove_nodes + i) == 1)
			{
				removeNode(A, findNode(A, i + 1));
				removeNode(B, findNode(B, i + 1));
			}
		}

		if (checkEqialTree(A, B))
		{
			int tmp_size = sizeTree(A);
			if (tmp_size > max) max = tmp_size;
		}

		stop = 1;
		for (int i = 0; i < size; i++)
		{
			if (*(remove_nodes + i) == 1) *(remove_nodes + i) = 0;
			else 
			{ 
				stop = 0;
				*(remove_nodes + i) = 1;
				i = size;
			}
		}
		deleteTree(&A, size);
		deleteTree(&B, size);
	}

	return max;
}

int signUnsignedNodes(tree_node_t* root, int number)
{
	int n = number;

	if (root->value == NULL)
	{
		root->value = number;
		n--;
	}
	
	tree_node_t* p = root->child;
	while (p != NULL)
	{
		if (p->value == NULL) n = signUnsignedNodes(p, n);
		p = p->brother;
	}
	return n;
}

int tabSum(int** tab, int* taken, int sizeA, int sizeB, int currentA, int currentB)
{
	int sum = 0;
	while (currentA != sizeA)
	{
		while (*(taken + currentA) == 1)
		{
			currentA++;
			if (currentA == sizeA) break;
		} 
		int tmp;
		if (currentB == sizeB - 1) tmp = *(*(tab + currentB) + currentA);
		else
		{
			*(taken + currentA) = 1;
			tmp = tabSum(tab, taken, sizeA, sizeB, 0, currentB + 1) + *(*(tab + currentB) + currentA);
			*(taken + currentA) = 0;
		}

		if (tmp > sum) sum = tmp;
		do {
			currentA++;
			if (currentA == sizeA) break;
		} while (*(taken + currentA) == 1);
	}
	return sum;
}

int maxTabSum(int** tab, int sizeA, int sizeB)
{
	int result = 0;
	if (sizeA >= sizeB)
	{
		int* taken = (int*)malloc(sizeA * sizeof(int));

		result = tabSum(tab, taken, sizeA, sizeB, 0, 0);

		free(taken);
	}
	else
	{
		int* taken = (int*)malloc(sizeB * sizeof(int));
		int** tab2 = (int**)malloc(sizeA * sizeof(int*));
		for (int i = 0; i < sizeA; i++)
		{
			*(tab2 + i) = (int*)malloc(sizeB * sizeof(int));
		}
		for (int i = 0; i < sizeA; i++)
		{
			for (int j = 0; j < sizeB; j++)
			{
				*(*(tab2 + i) + j) = *(*(tab + j) + i);
			}
		}

		result = tabSum(tab2, taken, sizeB, sizeA, 0, 0);

		for (int i = 0; i < sizeA; i++) free(*(tab2 + i));
		free(tab2);
		free(taken);
	}

	return result;
}

int tableSollution(tree_node_t* rootA, tree_node_t* rootB, int size)
{
	int nodesA = countNodes(rootA);
	int nodesB = countNodes(rootB);
	signUnsignedNodes(rootA, 0);
	signUnsignedNodes(rootB, 0);
	int result = 0;

	int** table = (int**)malloc(nodesB * sizeof(int*));		//Aby dostac sie do i-tego wezla drzewa A i j-tego drzewa B:	 *(*(table + j - 1) + i - 1)

	for (int i = 0; i < nodesB; i++)
	{
		*(table + i) = (int*)malloc(nodesA * sizeof(int));
	}


	// Wypelnianie tabeli - 1

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j) *(*(table + i) + j) = 1;
			else *(*(table + i) + j) = 0;
		}
	}

	// Wypelnianie tabeli - 2

	for (int i = nodesA - 1; i >= size; i--)
	{
		for (int j = 0; j < size; j++)
		{
			int set = 0;

			tree_node_t* p = findNode(rootA, size - i)->child;
			while (p != NULL)
			{
				if (p->value == (j + 1)) set = 1;
				else if (p->value < 1)  if (*(*(table + j) + size - p->value) == 1) set = 1;
				p = p->brother;
			}

			*(*(table + j) + i) = set;
		}
	}

	for (int j = nodesB - 1; j >= size; j--)
	{
		for (int i = 0; i < size; i++)
		{
			int set = 0;

			tree_node_t* p = findNode(rootB, size - j)->child;
			while (p != NULL)
			{
				if (p->value == (i + 1)) set = 1;
				else if (p->value < 1) if (*(*(table + size - p->value) + i) == 1) set = 1;
				p = p->brother;
			}

			*(*(table + j) + i) = set;
		}
	}

	// Wypelnienie tabeli - 3

	for (int i = nodesA - 1; i >= size; i--)
	{
		for (int j = nodesB - 1; j >= size; j--)
		{
			tree_node_t* A = findNode(rootA, size - i);
			tree_node_t* B = findNode(rootB, size - j);
			int sizeA = countChildren(A);
			int sizeB = countChildren(B);

			int** tab = (int**)malloc(sizeB * sizeof(int*));		

			for (int n = 0; n < sizeB; n++)
			{
				*(tab + n) = (int*)malloc(sizeA * sizeof(int));
			}

			// Pobranie wartoĂ„Ä…Ă˘â‚¬Ĺźci dla tabeli 
			
			tree_node_t* p = A->child;
			for (int m = 0; m < sizeA; m++)
			{
				tree_node_t* q = B->child;
				for (int n = 0; n < sizeB; n++)
				{
					int a, b;
					if (p->value > 0) a = p->value - 1;
					else a = size - p->value;

					if (q->value > 0) b = q->value - 1;
					else b = size - q->value;

					*(*(tab + n) + m) = *(*(table + b) + a);
					
					q = q->brother;
				}
				p = p->brother;
			}
			
			int set = maxTabSum(tab, sizeA, sizeB);

			p = B->child;
			for (int n = 0; n < sizeB; n++)
			{
				int node;
				if (p->value > 0) node = p->value - 1;
				else node = size - p->value;

				int tmp = *(*(table + node) + size - A->value);
				if (tmp > set) set = tmp;

				p = p->brother;
			}

			p = A->child;
			for (int n = 0; n < sizeA; n++)
			{
				int node;
				if (p->value > 0) node = p->value - 1;
				else node = size - p->value;

				int tmp = *(*(table + size - B->value) + node);
				if (tmp > set) set = tmp;

				p = p->brother;
			}

			*(*(table + j) + i) = set;

			if (result < set) result = set;


			// Zwalnianie pamieci
			for (int n = 0; n < sizeB; n++)
			{
				free(*(tab + n));
			}

			free(tab);

		}
	}

	

	/*for (int j = 0; j < nodesB; j++)
	{
		if (j == size) cout << endl;
		for (int i = 0; i < nodesA; i++)
		{
			if (i == size) cout << ' ';
			cout << *(*(table + j) + i) << ' ';
		}
		cout << endl;
	}*/

	//Zwalnianie pamieci

	for (int i = 0; i < nodesB; i++)
	{
		free(*(table + i));
	}

	free(table);

	return result;
}

int main()
{
	int n;
	cin >> n;
	cin.get();

	tree_node_t* forest = (tree_node_t*)malloc(n * sizeof(tree_node_t));

	for (int i = 0; i < n; i++)
	{
		*(forest + i) = *initTree();
		tree_node_t* p = (forest + i)->child;
		while (p != NULL)
		{
			p->parent = (forest + i);
			p = p->brother;
		}
	}
	int size = sizeTree(forest);

	/*for (int i = 0; i < (n - 1); i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			cout << size - bruteForce(forest + i, forest + j, size) << endl;
		}
	}*/

	for (int i = 0; i < (n - 1); i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			cout << size - tableSollution(forest + i, forest + j, size) << endl;
		}
	}
	
	return 0;
}