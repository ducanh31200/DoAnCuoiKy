#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<time.h>

#define nmax 3
int n;
int choise;
int X[4] = { 0,0,-1,1 };
int Y[4] = { -1,1,0,0 };

void gotoxy(int x, int y)
{
	HANDLE	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(handle, coord);
}

struct Node
{
	int action;
	int depth;
	int cost;
	int state[nmax][nmax];
	int td_x_0, td_y_0;
	Node* nextNode;
	Node* undoNode;

	Node* parent;
	Node* child;
};
struct Node_List
{
	int length;
	Node* head;
	Node* tail;
};

Node* newNode()
{
	Node* temp = new Node;
	temp->action = -1;
	temp->cost = temp->depth = 0;
	temp->nextNode = temp->parent = NULL;
	return temp;
}
Node* copyNode(Node* node)
{
	Node* temp = newNode();
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			temp->state[i][j] = node->state[i][j];
	temp->nextNode = temp->undoNode = NULL;
	temp->td_x_0 = node->td_x_0;
	temp->td_y_0 = node->td_y_0;
	temp->cost = 0;
	temp->depth = 0;
	return temp;
}
Node* childNode(Node* node, int action)
{
	Node* temp = copyNode(node);
	temp->parent = node;
	int x = temp->td_x_0 + X[action];
	int y = temp->td_y_0 + Y[action];
	temp->state[temp->td_x_0][temp->td_y_0] = temp->state[x][y];
	temp->state[x][y] = 0;
	temp->td_x_0 = x;
	temp->td_y_0 = y;
	temp->depth = node->depth + 1;
	temp->action = action;
	temp->cost = node->cost;
	return temp;
}

bool isOposite(int action1, int action2)
{
	if (action1 == -1 || action2 == -1 || action1 == action2)
		return false;
	if ((action1 < 2 && action2 < 2) || (action1 > 1 && action2 > 1))
		return true;
	return false;
}
bool isEqual(int stateA[][nmax], int stateB[][nmax])
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			if (stateA[i][j] != stateB[i][j])
				return false;
		}
	return true;
}
Node_List* newNodeList()
{
	Node_List* temp = new Node_List;
	temp->length = 0;
	
	temp->head = NULL;
	temp->tail = NULL;
	return temp;
}
void addNodeList(Node* node, Node_List* NodeList)
{
	NodeList->length++;
	if (NodeList->head == NULL)
	{
		NodeList->head = node;
	}
	else if (NodeList->tail == NULL)
	{
		NodeList->tail = node;
		NodeList->tail->undoNode = NodeList->head;
		NodeList->head->nextNode = NodeList->tail;
	}
	else
	{
		NodeList->tail->nextNode = node;
		node->undoNode = NodeList->tail;
		NodeList->tail = node;
	}
}

void insertNodeList(Node* node, Node_List* NodeList)
{
	NodeList->length++;
	if (NodeList->head == NULL)
	{
		NodeList->head = node;
	}
	else if (NodeList->tail == NULL)
	{
		if (NodeList->head->cost > node->cost)
		{
			NodeList->tail = NodeList->head;
			NodeList->head = node;
		}
		else NodeList->tail = node;
		NodeList->head->nextNode = NodeList->tail;
		NodeList->tail->undoNode = NodeList->head;
	}
	else if (NodeList->head->cost >= node->cost)
	{
		node->nextNode = NodeList->head;
		NodeList->head->undoNode = node;
		NodeList->head = node;
	}
	else if (NodeList->tail->cost <= node->cost)
	{
		node->undoNode = NodeList->tail;
		NodeList->tail->nextNode = node;
		NodeList->tail = node;
	}
	else
	{
		Node* root = NodeList->head;
		while (root->nextNode != NULL && root->cost < node->cost)
			root = root->nextNode;
		node->nextNode = root;
		node->undoNode = root->undoNode;
		root->undoNode = node;
	}
}

Node* popNodeList(Node_List* NodeList)
{
	
	NodeList->length--;
	Node* temp = NodeList->head;
	NodeList->head = NodeList->head->nextNode;
	if (NodeList->head != NULL)
	{
		NodeList->head->undoNode = NULL;
		if (NodeList->head == NodeList->tail)
			NodeList->tail = NULL;
	}

	temp->nextNode = temp->undoNode = NULL;
	return temp;
	
	
}
bool isExistInNodeList(Node* node, Node_List* NodeList)
{
	Node* root = NodeList->head;
	while (root != NULL)
	{
		if (isEqual(root->state, node->state))
			return true;
		root = root->nextNode;
	}
	return false;
}


void findZero(Node* node)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			if (node->state[i][j] == 0)
			{
				node->td_x_0 = i;
				node->td_y_0 = j;
				return;
			}
		}
}

bool isLegalMove(Node* node, int action)
{
	if (node->td_x_0 + X[action] < 0 || node->td_x_0 + X[action] >= n)
		return false;
	if (node->td_y_0 + Y[action] < 0 || node->td_y_0 + Y[action] >= n)
		return false;
	return true;
}
void inputTable(int state[][nmax])
{

	std::cin.ignore();
	bool okay;
	do
	{
		okay = true;
		bool isUse[25] = { false };
		for (int j = 0; j < n; j++)
			for (int i = 0; i < n; i++)
			{
				std::cin >> state[i][j];
				if (isUse[state[i][j]] == false)
					isUse[state[i][j]] = true;
				else
				{
					i = j = n;
					okay = false;
					std::cout << "Nhap khong hop le !!!	\n";
					std::cout << "Moi nhap lai !!!	\n";
				}
			}
	} while (okay == false);
}

void randomTable(int state[][nmax])
{
	int temp = n * n;
	bool isUse[25] = { false };
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int number;
			do
			{
				number = rand() % temp;
			} while (isUse[number] == true);
			isUse[number] = true;
			state[i][j] = number;
		}
	}
}

void createGoal(Node* start, Node*& goal)
{
	int step = rand() % 15 + 25;
	goal = start;
	Node* temp;
	while (step > 0)
	{
		int newAction;
		do
		{
			newAction = rand() % 4;
		} while (newAction == goal->action || isLegalMove(goal, newAction)  == false);
		if (goal != start)
		{
			temp = goal;
			goal = childNode(goal, newAction);
			delete temp;
		}
		else goal = childNode(goal, newAction);
		step--;
	}
}

void listMove(Node* start, Node* temp, Node*& goal)
{
	delete goal;
	goal = temp;
	Node* root = goal;
	while (root != start)
	{
		root->parent->child = root;
		if (root->undoNode != NULL)
			root->undoNode->nextNode = root->nextNode;
		if (root->nextNode != NULL)
			root->nextNode->undoNode = root->undoNode;
		root->nextNode = root->undoNode = NULL;
		root = root->parent;
	}
}
int Heuristic(int S[][nmax], int G[][nmax])
{
	int xS[9] = { 0 };
	int yS[9] = { 0 };
	int xG[9] = { 0 };
	int yG[9] = { 0 };
	int point = 0;
	
	
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			xS[S[i][j]] = i; yS[S[i][j]] = j;
			xG[G[i][j]] = i; yG[G[i][j]] = j;
		}
	for (int i = 0; i < 9; i++)
	{
		int Xpoint = (xS[i] > xG[i]) ? (xS[i] - xG[i]) : ( xG[i] -xS[i] );
		int Ypoint = (yS[i] > yG[i]) ? (yS[i] - yG[i]) : ( yG[i] -yS[i] );
		point +=  Xpoint + Ypoint;
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (S[i][j] != G[i][j])
			{
				point++;
			}
		}
	return point;
}
bool BFS(Node* start, Node*& goal)
{
	Node_List* listPoint = newNodeList();
	Node_List* checkExist = newNodeList();
	addNodeList(start, listPoint);
	Node* root;
	do
	{
		root = popNodeList(listPoint);
		for (int i = 0; i < 4; i++)
		{
			if (isLegalMove(root, i) && isOposite(root->action, i) == false)
			{
				Node* temp = childNode(root, i);
				
				if (isEqual(temp->state, goal->state))
				{
					listMove(start, temp, goal);
					return true;
				}
				addNodeList(temp, listPoint);
			}
		}
		addNodeList(root, checkExist);
	} while (root->depth < 30);

	return false;
}
bool AStar(Node* start, Node*& goal)
{
	Node_List* listPoint = newNodeList();
	Node_List* checkExist = newNodeList();

	start->cost = Heuristic(start->state, goal->state);

	addNodeList(start, listPoint);
	int step = 0;
	Node* root;
	while (true)
	{
		root = popNodeList(listPoint);
		for (int i = 0; i < 4; i++)
		{
			if (isLegalMove(root, i) && isOposite(root->action, i) == false)
			{
				Node* temp = childNode(root, i);
				if (isExistInNodeList(temp, checkExist) == false)
				{
					if (isEqual(temp->state, goal->state))
					{
						listMove(start, temp, goal);
						return true;
					}
					temp->cost = step + Heuristic(temp->state, goal->state);
					insertNodeList(temp, listPoint);
				}
				else delete temp;
			}
		}
		addNodeList(root, checkExist);
		step++;
	} 
	return false;
}
void setUpGame(Node*& start, Node*& goal)
{
	system("cls");
	std::cout << "*** ----------------- ***\n";
	std::cout << "	1. Random table		   \n";
	std::cout << "	2. Tu tao table		   \n";
	std::cout << "	Lua chon cua ban:	 ";

	n = 3; choise = 0;
	
	std::cin >> choise;

	if (choise == 1)
	{
		do
		{
			randomTable(start->state);	
			findZero(start);
			createGoal(start, goal);  
			//randomTable(goal->state);
			findZero(goal);
		} while (BFS(start, goal) == false);
	}
	else if (choise == 2)
	{
		std::cout << "Nhap trang thai bat dau: ";
		inputTable(start->state); 	
		findZero(start);
		std::cout << "Nhap trang thai dich: ";
		inputTable(goal->state);	
		findZero(goal);
		while ((BFS(start, goal) == false))
		{
			printf("Khong tim duoc loi giai, moi nhap lai !\n");
			std::cout << "Nhap trang thai bat dau: ";
			inputTable(start->state); 	
			findZero(start);
			std::cout << "Nhap trang thai dich: ";
			inputTable(goal->state);	
			findZero(goal);
		}
	}
	
	Node* temp = copyNode(goal);
	AStar(copyNode(start), temp);	
	system("cls");
}

void paintCell(int x, int y, int number,bool move)
{
	if (!move)
	{
		gotoxy(x, y); printf("%c%c%c%c%c%c", 201, 205, 205, 205, 205, 187);
		gotoxy(x, y + 1); printf("%c %2d %c", 186, number, 186);
		gotoxy(x, y + 2); printf("%c%c%c%c%c%c", 200, 205, 205, 205, 205, 188);
		
	}
	else
	{
		gotoxy(x, y); printf("%c%c%c%c%c%c", 220, 220, 220, 220, 220, 220);
		gotoxy(x, y + 1); printf("%c%2d  %c", 221, number, 222);
		gotoxy(x, y + 2); printf("%c%c%c%c%c%c", 223, 223, 223, 223, 223, 223);
	}

}
void earseCell(int x, int y)
{
	gotoxy(x, y); printf("%c%c%c%c%c%c", 201, 205, 205, 205, 205, 187);
	gotoxy(x, y + 1); printf("%c    %c", 186, 186);
	gotoxy(x, y + 2); printf("%c%c%c%c%c%c", 200, 205, 205, 205, 205, 188);
}

void showTable(int x, int y, int state[][nmax])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			if (state[i][j] != 0)
				paintCell((x + i) * 7, (y + j) * 3, state[i][j],false);
			else earseCell((x + i) * 7, (y + j) * 3);
	}
}

int playerMove(Node* node)
{
	int action = -1;
	while (1)
	{
		gotoxy(0, n * 4);
		std::cout << "Your Move: " << ((action == -1) ? "     " : (action == 0) ? " UP   " : (action == 1) ? " DOWN " : (action == 2) ? " LEFT " : " RIGHT");
		char c = _getch();
		switch (c)
		{
		case 72:
			if (node->td_y_0 >= 1)
				action = 0;
			else action = -1;
			break;
		case 80:
			if (node->td_y_0 < n - 1)
				action = 1;
			else action = -1;

			break;
		case 75:
			if (node->td_x_0 >= 1)
				action = 2;
			else action = -1;

			break;
		case 77:
			if (node->td_x_0 < n - 1)
				action = 3;
			else action = -1;

			break;
		case 13:
			if (action != -1)
			{
				return action;
			}
			break;
		}
	}
}


bool shakeCoin()
{
	int coin = rand() % 20;
	for (int i = 0; i < coin; i++)
	{
		gotoxy(0, n * 4 + 3); std::cout << (i % 2 == 0 ? "YES" : "NO ");
		Sleep(50);
	}
	gotoxy(0, n * 4 + 3); std::cout << (coin % 2 == 1 ? "YES" : "NO ");
	return (coin % 2 == 1 ?  true :  false);
}



int computerTurn(Node* start)
{
	gotoxy(0, n * 4);
	return start->child->action;
}

void paintMove(Node*& start, int action)
{

	if (action == start->child->action)
	{
		Node* child = start->child;
		showTable(0, 0, start->state);
		paintCell(start->td_x_0 * 7, start->td_y_0 * 3, child->state[start->td_x_0][start->td_y_0],true);
		earseCell(child->td_x_0 * 7, child->td_y_0 * 3);
		delete start;
		start = child;
		start->parent = NULL;
	}
	else
	{
		Node* parent = childNode(start, action);
		showTable(0, 0, start->state);
		paintCell(start->td_x_0 * 7, start->td_y_0 * 3, parent->state[start->td_x_0][start->td_y_0],true);
		earseCell(parent->td_x_0 * 7, parent->td_y_0 * 3);
		parent->child = start;
		start->parent = parent;
		start = parent;
		start->parent = NULL;
	}
}


void PlayGame(Node* start, Node* goal)
{
	int step = 0;
	while (1)
	{
		gotoxy(0, n * 4 + 1);
		std::cout << (step % 2 == 0 ? "Player  " : "Computer") << " An mot phim bat ki de tung dong xu ";
		fflush(stdin);
		if (step % 2 == 1)
			Sleep(1000);
		else
			_getch();
		if (shakeCoin() == true)
		{
			gotoxy(0, n * 4 + 1);

			std::cout << (step % 2 == 0 ? "Player  " : "Computer") << "     Su dung mui ten va nhan ENTER  ";
			int newAction = (step % 2 == 0) ? playerMove(start) : computerTurn(start);
			paintMove(start, newAction);
			if (isEqual(start->state, goal->state) == true)
			{
				//system("cls");
				gotoxy(0, n * 4 + 6);
				std::cout << (step % 2 == 0 ? "Player  " : "Computer") << " Win";
				_getch();
				break;
			}
		}
		else step += 1;
	}
}
int main()
{
	srand((int)time(NULL));
	while (1)
	{
		Node* start = newNode();
		Node* goal = newNode();
		setUpGame(start, goal);
		showTable(0, 0, start->state);
		gotoxy(0, n * 3); std::cout << "Start";
		showTable(n + 1, 0, goal->state);
		gotoxy((n + 1) * 7, n * 3); std::cout << "Goal";
		PlayGame(start, goal);
	}
	return 1;
}