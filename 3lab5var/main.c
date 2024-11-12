#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#define MAX_LINE_LENGTH 256

typedef enum {
    NODE_OPERATOR_IF,
    NODE_OPERATOR_ELIF,
    NODE_OPERATOR_ELSE,
    NODE_OPERATOR_FOR,
    NODE_OPERATOR_WHILE,
    NODE_OPERATOR_ASSIGN,     // Присваивание "="
    NODE_OPERATOR_IN,         // Оператор "in"
    NODE_OPERATOR_COLON,      // Двоеточие ":"
    NODE_OPERATOR_INDENT,     // Вложенность (отступ)
    NODE_OPERATOR_DEDENT,     // Уменьшение вложенности
    NODE_OPERATOR_INT,        // Для чисел и выражений
    NODE_OPERATOR_ERROR
} NodeType;
typedef struct Node
{
    NodeType type;
    struct Node *left;
    struct Node *right;
    char *value;
} Node;

Node* astRoot = NULL;  // Глобальный корневой узел всего дерева
Node* parseOperator();
Node* parseOperatorIf();
Node* parseOperatorFor();
Node* parseOperatorWhile();
void printParseTree(const Node* root, int depth);
void freeParseTree(Node* root);
int matchToken(const char* token);
void nextToken();
void parseError(const char* expected);

int currentTokenIndex = 0;
char** construction;
int tokenCount;
int isError = 0;

Node* createNode(const NodeType type, Node* left, Node* right, const char* value) {
    Node* node = malloc(sizeof(Node));
    node->type = type;
    node->left = left;
    node->right = right;
    node->value = value ? strdup(value) : NULL;
    return node;
}

const char* getCurrentToken()
{
    if(currentTokenIndex < tokenCount)
    {
        return construction[currentTokenIndex];
    }
    return NULL;
}

void nextToken()
{
    currentTokenIndex++;
}

void prevToken()
{
    if(currentTokenIndex > 0)
    {
        currentTokenIndex--;
    }
}

int matchToken(const char* token)
{
    int result = 0;
    const char* currentToken = getCurrentToken();
    if(currentToken && strcmp(currentToken, token) == 0)
    {
        nextToken();
        result = 1;
    }
    return result;
}

void parseError(const char* expected) {
    printf("Error: Expected '%s' but found '%s'\n", expected, getCurrentToken());
}

Node* appendToRight(Node* root, Node* newNode) {
    Node* current = root;
    while (current->right != NULL) {
        current = current->right;
    }
    current->right = newNode;
    return root;
}

Node* getDeepestChild(Node* node)
{
    Node* temp = node;
    while(temp -> right != NULL)
    {
        temp = temp -> right;
    }
    return temp;
}

Node* parseOperator()
{
    Node* result = NULL;
    isError = 0;
    if(matchToken("IF"))
    {
        result = parseOperatorIf();
    }
    else if(matchToken("WHILE"))
    {
        result = parseOperatorWhile();
    }
    else if(matchToken("FOR"))
    {
        result = parseOperatorFor();
    }
    else if(matchToken("EXPRESSION"))
    {
        puts("Valid EXPRESSION-construction\n");
        result = createNode(NODE_OPERATOR_INT, NULL, NULL, "EXPRESSION");
    }
    isError = result == NULL ? 1 : 0;
    return result;
}

Node* expressionHandler(Node* root,Node* current)
{
    if(matchToken("EXPRESSION"))
    {
        if(current->left == NULL)
        {
            current->left = createNode(NODE_OPERATOR_INT, NULL, NULL, "EXPRESSION");
        }
        else
        {
            current->right = createNode(NODE_OPERATOR_INT, NULL, NULL, "EXPRESSION");
            current = current->right;
        }
    }
    else
    {
        current->right = parseOperator();
        if(current->right == NULL)
        {
            if(!isError)
            {
                freeParseTree(root);
                return NULL;
            }
            current->right = createNode(NODE_OPERATOR_ERROR, NULL, NULL, "ERROR");
            nextToken();
        }
        current = getDeepestChild(current);
    }
    return current;
}



Node* tokenCheck(int* errorFlag,const char* tokenName, const NodeType validType)
{
    Node* result = NULL;
    if(!matchToken(tokenName))
    {
        parseError(tokenName);
        *errorFlag = 1;
        result = createNode(NODE_OPERATOR_ERROR, NULL, NULL, "EXPRESSION");
    }
    else
    {
        result = createNode(validType, NULL, NULL, tokenName);
    }
    return result;
}

Node* parseBlockBody(Node* current) {
    while (getCurrentToken() && !matchToken("DEDENT")) {
        if (matchToken("ELIF") || matchToken("ELSE")|| matchToken("WHILE")|| matchToken("FOR")) {
            prevToken();  // Возвращаемся назад, чтобы обработать эти токены в `parseOperatorIf`
            break;
        }
        
        Node* bodyNode = parseOperator();  // Разбираем выражение внутри блока
        if (!bodyNode) {
            parseError("Expected valid statement");
            return NULL;
        }
        current->right = bodyNode;
        current = getDeepestChild(current);
    }
    return current;
}

Node* parseOperatorIf() {
    Node* ifNode = createNode(NODE_OPERATOR_IF, NULL, NULL, "IF");
    Node* current = ifNode;
    
    int errorFlag = 0;
    current->left = tokenCheck(&errorFlag, "EXPRESSION", NODE_OPERATOR_INT);  // Условие IF

    if (!matchToken("COLON") || !matchToken("INDENT")) { // Проверяем наличие ":" и отступа
        parseError("Expected COLON and INDENT after IF");
        errorFlag = 1;
    }

    if (!errorFlag) {
        current = parseBlockBody(current); // Разбор тела if-блока
    }

    // Разбор elif и else
    while (getCurrentToken() && !errorFlag) {
        if (matchToken("ELIF")) {
            Node* elifNode = createNode(NODE_OPERATOR_ELIF, NULL, NULL, "ELIF");
            current->right = elifNode;
            current = elifNode;
            
            current->left = tokenCheck(&errorFlag, "EXPRESSION", NODE_OPERATOR_INT);
            if (!matchToken("COLON") || !matchToken("INDENT")) {
                parseError("Expected COLON and INDENT after ELIF");
                errorFlag = 1;
            }

            if (!errorFlag) {
                current = parseBlockBody(current);
            }

        } else if (matchToken("ELSE")) {
            Node* elseNode = createNode(NODE_OPERATOR_ELSE, NULL, NULL, "ELSE");
            current->right = elseNode;
            current = elseNode;

            if (!matchToken("COLON") || !matchToken("INDENT")) {
                parseError("Expected COLON and INDENT after ELSE");
                errorFlag = 1;
            }

            if (!errorFlag) {
                current = parseBlockBody(current);
            }
            break;
        } else {
            break;
        }
    }

    if (errorFlag) {
        puts("Invalid IF construction\n");
    } else {
        puts("Valid IF construction\n");
    }
    return ifNode;
}

Node* parseOperatorFor() {
    Node* forNode = createNode(NODE_OPERATOR_FOR, NULL, NULL, "FOR");
    Node* current = forNode;
    int errorFlag = 0;

    // Parse the loop variable expression
    current->left = tokenCheck(&errorFlag, "EXPRESSION", NODE_OPERATOR_INT);

    // Check for the "IN" keyword and parse the iterable
    if (!matchToken("IN")) {
        parseError("Expected 'IN' after FOR");
        errorFlag = 1;
    } else {
        current->right = tokenCheck(&errorFlag, "EXPRESSION", NODE_OPERATOR_INT);
    }

    // Expect a colon and indentation for the for block
    if (!matchToken("COLON") || !matchToken("INDENT")) {
        parseError("Expected COLON and INDENT after FOR");
        errorFlag = 1;
    }

    // Parse the body of the for loop
    if (!errorFlag) {
        current = parseBlockBody(current);
    }

    if (errorFlag) {
        puts("Invalid FOR construction\n");
    } else {
        puts("Valid FOR construction\n");
    }

    return forNode;
}


Node* parseOperatorWhile() {
    Node* whileNode = createNode(NODE_OPERATOR_WHILE, NULL, NULL, "WHILE");
    Node* current = whileNode;
    int errorFlag = 0;

    // Parse the condition expression
    current->left = tokenCheck(&errorFlag, "EXPRESSION", NODE_OPERATOR_INT);
    // Expect indentation for the while block
    if (!matchToken("COLON")) {
        parseError("COLON");
        errorFlag = 1;
    }
    // Expect indentation for the while block
    if (!matchToken("INDENT")) {
        parseError("INDENT");
        errorFlag = 1;
    }

    // Parse the body of the `while` loop
    while (!errorFlag && !matchToken("DEDENT")) {
        current = expressionHandler(whileNode, current);
        if (current == NULL || isError) {
            errorFlag = 1;
            break;
        }
    }

    if (errorFlag) {
        puts("Invalid WHILE construction\n");
    } else {
        puts("Valid WHILE construction\n");
    }

    return whileNode;
}

void printParseTree(const Node* root, const int depth) {
    if (root == NULL) {
        return;
    }

    // Отступ для текущего уровня узла
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // Выводим тип узла и значение, если оно присутствует
    switch (root->type) {
        case NODE_OPERATOR_INT:
            printf("INT: %s\n", root->value);
            break;
        case NODE_OPERATOR_ERROR:
            printf("ERROR: %s\n", root->value);
            break;
        case NODE_OPERATOR_IF:
            printf("IF\n");
            break;
        case NODE_OPERATOR_FOR:
            printf("FOR\n");
            break;
        case NODE_OPERATOR_WHILE:
            printf("WHILE\n");
            break;
        case NODE_OPERATOR_ELSE:
            printf("ELSE\n");
            break;
        case NODE_OPERATOR_ELIF:
            printf("ELIF\n");
            break;
        case NODE_OPERATOR_INDENT:
            printf("INDENT\n");
            break;
        case NODE_OPERATOR_DEDENT:
            printf("DEDENT\n");
            break;
        default:
            break;
    }

    // Рекурсивно выводим дерево с увеличением отступа только для левого узла (вложенные блоки)
    printParseTree(root->left, depth + 1);  // Левый узел — на новом уровне вложенности
    printParseTree(root->right, depth);     // Правый узел — на том же уровне
}
void freeParseTree(Node* root)
{
    if (root == NULL) return;

    if (root->left)
    {
        freeParseTree(root->left);
    }
    if (root->right)
    {
        freeParseTree(root->right);
    }
    if (root->value)
    {
        free(root->value);
    }
    free(root);
}
char** tokenize(const char* input, int* tokenCount) {
    char *inputCopy = strdup(input);
    const char *token = strtok(inputCopy, " \n");
    char **tokens = NULL;
    char **temp = NULL;
    int count = 0;

    while (token != NULL) {
        temp = realloc(tokens, sizeof(char*) * (count + 1));
        if(temp != NULL)
        {
            tokens = temp;
            tokens[count++] = strdup(token);
            token = strtok(NULL, " \n");
        }
    }

    *tokenCount = count;
    free(inputCopy);
    return tokens;
}


void parseConstruction() {
    Node* root = NULL;
    Node* parsedNode = NULL;
    
    while (getCurrentToken() && (parsedNode = parseOperator()) != NULL) { 
        if (root == NULL) {
            root = parsedNode;  // Initialize root on the first valid node
        } else {
            root = appendToRight(root, parsedNode);  // Append subsequent nodes to the right
        }
    }

    if (root != NULL) {
        puts("Full Parse Tree:\n");
        printParseTree(root, 0);
        freeParseTree(root);  // Free the entire tree after printing
    }
}

void test(FILE* outputFile) {
    char line[MAX_LINE_LENGTH];
    puts("\n");
    outputFile = fopen("tokens.txt", "r");
    while (fgets(line, sizeof(line), outputFile))
    {
        char** tokens = tokenize(line, &tokenCount);
        construction = tokens;
        currentTokenIndex = 0;
        if(tokens != NULL)
        {
            parseConstruction();
            for (int i = 0; i < tokenCount; i++)
            {
                free(tokens[i]);
            }
            free(tokens);
        }
    }
    fclose(outputFile);
}
