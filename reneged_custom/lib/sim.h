typedef struct Node {
int type;
double clock;
struct Node* next;
}Node;
Node* createNode(int, double);
Node* insertNode(Node*, Node*);
Node* removeNode(Node*, Node**);
void printLists(Node*);
double arrival(double);
double departure(double, double);

Node* createNode(int type, double clock)
{
	Node* NodePtr = (Node*)malloc(sizeof(Node));
	NodePtr->type=type;
	NodePtr->clock=clock;
	NodePtr->next=NULL;
	// printf("creating node...");
	return NodePtr;
}

Node* insertNode(Node* head, Node* newNodePtr)
{
	if(head == NULL)
	{
		return newNodePtr;
	}
	else if (head->clock > newNodePtr->clock )
	{
		newNodePtr->next = head;
		return newNodePtr;
	}
	else
	{
		Node* prev = NULL;
		Node* curr = head;
		
		while((curr!=NULL)&&(curr->clock <= newNodePtr->clock))
		{
			prev = curr;
			curr = curr->next;
		}
		prev->next = newNodePtr;
		newNodePtr->next = curr;

		return head;
	}
}

Node* removeNode(Node* head, Node** nextRef)
{
	*nextRef = head;
	
	if(head!=NULL)
	{
		head = head->next;
	}
	
    (*nextRef)->next = NULL;
    
	return head;
}

void printLists(Node* head)
{
    Node* printNode = head;

    // �̧ǦL�X�`�I���e
    while (printNode != NULL)
    {
        // printf("type: %d clock:%2f -->", printNode->type, printNode->clock );
        printf("|");
        printNode = printNode->next;
    }
    printf("\n");

}

int getCount(Node * head)
{
	int count = 0;  // Initialize count
	Node * current = head;  // Initialize current
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return count;
}
