#include "lex1.h"
#include <iostream>
#include <string>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <set>
#include <bits/stdc++.h>

using namespace std;

enum NonTerminal
{
    Program,
    MainClassDecl,
    MultiStatement,
    SingleStatement,
    VarDecl,
    AssignExpr,
    MethodCall,
    BranchStatement,
    RemBranch,
    WhileLoop,
    ForLoop,
    Block,
    Type,
    Factor,
    ObjConstruct,
    Name,
    BinOp,
    FrOp,
    BkOp,
    Expr,
    Condition,
    CompOp,
    ForInit,
    ForUpdate,
    Args,
    RemVarDecl,
    RemObj,
    Did,
    RemExpr,
    Empty,
    BkExpr,
    Entity
};

struct TreeNode
{
    NonTerminal nonTerminal;
    TokenType terminal;
    string lexeme;
    vector<TreeNode *> children;

    TreeNode(NonTerminal nt, const string &lex) : nonTerminal(nt), lexeme(lex) {}
    TreeNode(TokenType tt, const string &lex) : terminal(tt), lexeme(lex) {}
};

class Parser
{
private:
    // Lexer lexer;
    Token currentToken;
    int currTokenNo;
    int noOfToken;

public:
    vector<Token> TokenList;

    Parser()
    {
        currTokenNo = 0;
    }

    void setToken()
    {
        noOfToken = TokenList.size();
        currentToken = TokenList[currTokenNo];
    }

    Token nextToken()
    {
        currTokenNo++;
        if (currTokenNo < noOfToken)
        {
            // cout<<TokenList[currTokenNo].lexeme<<endl;
            return TokenList[currTokenNo];
        }
        exit(1);
    }

    TreeNode *parse()
    {
        return Program1();
    }

    TreeNode *Program1()
    {
        TreeNode *programNode = new TreeNode(Program, "Program");
        programNode->children.push_back(MainClassDecl1());
        return programNode;
    }

    TreeNode *MainClassDecl1()
    {
        TreeNode *mainClassDeclNode = new TreeNode(MainClassDecl, "MainClassDecl");

        consume(PUBLIC);
        consume(CLASS);
        // mainClassDeclNode->children.push_back(consume(ID)); // Main class name
        consume(ID);
        consume(LBRACE);
        consume(PUBLIC);
        consume(STATIC);
        consume(VOID);
        consume(ID);
        consume(LPAREN);
        consume(STRINGTYPE);
        consume(LBRACK);
        consume(RBRACK);
        // mainClassDeclNode->children.push_back(consume(ID)); // args
        consume(ID);
        consume(RPAREN);
        consume(LBRACE);
        mainClassDeclNode->children.push_back(MultiStatement1());
        consume(RBRACE);
        consume(RBRACE);

        return mainClassDeclNode;
    }

    TreeNode *MultiStatement1()
    {
        TreeNode *multiStatementNode = new TreeNode(MultiStatement, "MultiStatement");

        while (true)
        {
            TreeNode *singleStatementNode = SingleStatement1();
            if (singleStatementNode == nullptr)
            {
                break;
            }
            multiStatementNode->children.push_back(singleStatementNode);
        }

        return multiStatementNode;
    }

    TreeNode *SingleStatement1()
    {
        TreeNode *singleStatementNode = new TreeNode(SingleStatement, "SingleStatement");

        Token nextToken = currentToken;
        Token tp = peepnextToken();
        switch (nextToken.type)
        {
        case LBRACE:
            singleStatementNode->children.push_back(Block1());
            break;
        case INT:
        case BOOL:
        case STRINGTYPE:
        case CLASS:
            singleStatementNode->children.push_back(VarDecl1());
            addNextToken(SEMI, singleStatementNode);
            // consume(SEMI);
            break;
        case ID:
            if (tp.type == ASSIGN)
            {
                singleStatementNode->children.push_back(AssignExpr1());
            }
            else if (tp.type == LPAREN || tp.type == DOT)
            {
                singleStatementNode->children.push_back(MethodCall1());
            }
            else if(tp.type == ID){
                singleStatementNode->children.push_back(VarDecl1());
            }
            else if(tp.type == INC||tp.type==DEC){
                
                singleStatementNode->children.push_back(BkExpr1());
            }
            // cout<<tp.type<<endl;
            addNextToken(SEMI, singleStatementNode);
            // consume(SEMI);
            break;
        case IF:
            singleStatementNode->children.push_back(BranchStatement1());
            break;
        case WHILE:
            singleStatementNode->children.push_back(WhileLoop1());
            break;
        case FOR:
            singleStatementNode->children.push_back(ForLoop1());
            break;
        case RETURN:
            // singleStatementNode->children.push_back(consume(RETURN));
            addNextToken(RETURN, singleStatementNode);
            singleStatementNode->children.push_back(Expr1());
            addNextToken(SEMI, singleStatementNode);
            // consume(SEMI);
            break;
        case RBRACE:
            singleStatementNode = nullptr;
            break;
        default:
            cerr << "Syntax error: Unexpected token  " << currentToken.lexeme << "at Line no " << currentToken.lineNo << endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }

        return singleStatementNode;
    }

    TreeNode *VarDecl1()
    {

        TreeNode *varDeclNode = new TreeNode(VarDecl, "VarDecl");
        varDeclNode->children.push_back(Type1());
        addNextToken(ID, varDeclNode);
        varDeclNode->children.push_back(RemVarDecl1());
        return varDeclNode;
    }

    TreeNode *RemVarDecl1()
    {
        // RemVarDecl ::= Empty | EQU Factor

        if (currentToken.type == ASSIGN)
        {
            TreeNode *remVarDeclNode = new TreeNode(RemVarDecl, "RemVarDecl");
            addNextToken(ASSIGN, remVarDeclNode);
            remVarDeclNode->children.push_back(Factor1());
            return remVarDeclNode;
        }
        else 
        {
            return new TreeNode(RemVarDecl, "Empty");
        }
    }

    TreeNode *AssignExpr1()
    {
        // AssignExpr ::= ID ASSIGN Expr
        TreeNode *assignExprNode = new TreeNode(AssignExpr, "AssignExpr");
        addNextToken(ID, assignExprNode);
        addNextToken(ASSIGN, assignExprNode);
        assignExprNode->children.push_back(Factor1());
        return assignExprNode;
    }

    TreeNode *MethodCall1()
    {
        // MethodCall ::= Name LPAREN Args RPAREN
        TreeNode *methodCallNode = new TreeNode(MethodCall, "MethodCall");
        methodCallNode->children.push_back(Name1());
        addNextToken(LPAREN, methodCallNode);
        // consume(LPAREN);
        methodCallNode->children.push_back(Args1());
        addNextToken(RPAREN, methodCallNode);
        // consume(RPAREN);
        return methodCallNode;
    }

    TreeNode *BranchStatement1()
    {
        // BranchStatement ::= IF LPAREN Condition RPAREN Block RemBranch
        TreeNode *branchStatementNode = new TreeNode(BranchStatement, "BranchStatement");

        addNextToken(IF, branchStatementNode);
        addNextToken(LPAREN, branchStatementNode);
        // consume(LPAREN);
        branchStatementNode->children.push_back(Condition1());
        addNextToken(RPAREN, branchStatementNode);
        // consume(RPAREN);
        branchStatementNode->children.push_back(Block1());
        branchStatementNode->children.push_back(RemBranch1());
        return branchStatementNode;
    }

    TreeNode *RemBranch1()
    {
        if (currentToken.type == ELSE)
        {
            TreeNode *remBranchNode = new TreeNode(RemBranch, "RemBranch");
            addNextToken(ELSE, remBranchNode);
            remBranchNode->children.push_back(Block1());
            return remBranchNode;
        }
        else
        {
            return new TreeNode(RemBranch, "Empty");
        }
    }

    TreeNode *WhileLoop1()
    {

        TreeNode *whileLoopNode = new TreeNode(WhileLoop, "WhileLoop");
        addNextToken(WHILE, whileLoopNode);
        addNextToken(LPAREN, whileLoopNode);
        // consume(LPAREN);
        whileLoopNode->children.push_back(Condition1());
        addNextToken(RPAREN, whileLoopNode);
        // consume(RPAREN);
        whileLoopNode->children.push_back(Block1());
        return whileLoopNode;
    }

    TreeNode *ForLoop1()
    {

        TreeNode *forLoopNode = new TreeNode(ForLoop, "ForLoop");
        addNextToken(FOR, forLoopNode);
        addNextToken(LPAREN, forLoopNode);
        // consume(LPAREN);
        forLoopNode->children.push_back(ForInit1());
        addNextToken(SEMI, forLoopNode);
        // consume(SEMI);
        forLoopNode->children.push_back(Condition1());
        addNextToken(SEMI, forLoopNode);
        // consume(SEMI);
        forLoopNode->children.push_back(ForUpdate1());
        // cout<<"-------inside forr  "<<currentToken.lexeme<<endl;
        addNextToken(RPAREN, forLoopNode);
        // consume(RPAREN);
        forLoopNode->children.push_back(Block1());
        return forLoopNode;
    }

    TreeNode *Block1()
    {
        // Block ::= LBRACE MultiStatement RBRACE
        TreeNode *blockNode = new TreeNode(Block, "Block");
        addNextToken(LBRACE, blockNode);
        // consume(LBRACE);
        blockNode->children.push_back(MultiStatement1());

        addNextToken(RBRACE, blockNode);
        // consume(RBRACE);
        return blockNode;
    }

    TreeNode *Type1()
    {

        TreeNode *typeNode = new TreeNode(Type, "Type");
        switch (currentToken.type)
        {
        case INT:
            addNextToken(INT, typeNode);
            break;
        case BOOL:
            addNextToken(BOOL, typeNode);
            break;
        case STRINGTYPE:
            addNextToken(STRINGTYPE, typeNode);
            break;
        case ID:
            addNextToken(ID, typeNode);
            break;
        default:
            // Handle syntax error
            cerr << "Syntax error: Unexpected token " << currentToken.lexeme << "at Line no " << currentToken.lineNo << endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }
        return typeNode;
    }

    TreeNode *Factor1()
    {
        // Factor ::= NUM | TRUE | FALSE | STRING | THIS | NULL | LPAREN Expr RPAREN | Name | ObjConstruct | NOT Factor
        TreeNode *factorNode = new TreeNode(Factor, "Factor");
        switch (currentToken.type)
        {
        case NUM:
        case TRUE:
        case FALSE:
        case ID:
        case LPAREN:
            factorNode->children.push_back(Expr1());
            break;
        case STRING:
            addNextToken(STRING, factorNode);
            break;
        case THIS:
            addNextToken(THIS, factorNode);
            factorNode->children.push_back(Did1());
            break;
        case NULL_:
            addNextToken(NULL_, factorNode);
            break;

        case NEW:
            factorNode->children.push_back(ObjConstruct1());
            break;
        case NOT:
            addNextToken(NOT, factorNode);
            factorNode->children.push_back(Expr1());
            break;

        default:
            cerr << "Syntax error: Unexpected token " << currentToken.lexeme << endl;
            exit(1);
        }
        return factorNode;
    }

    TreeNode *ObjConstruct1()
    {
        // ObjConstruct := NEW RemObj
        TreeNode *objConstructNode = new TreeNode(ObjConstruct, "ObjConstruct");
        addNextToken(NEW, objConstructNode);
        objConstructNode->children.push_back(RemObj1());
        return objConstructNode;
    }

    TreeNode *RemObj1()
    {
        // RemObj := ID LPAREN Expr RPAREN | Type LPAREN Expr RPAREN
        TreeNode *remObjNode = new TreeNode(RemObj, "RemObj");
        // cout<<"here"<<endl;
        if (currentToken.type == ID)
        {
            addNextToken(ID, remObjNode);
        }
        else if (currentToken.type == INT || currentToken.type == BOOL || currentToken.type == STRINGTYPE)
        {
            remObjNode->children.push_back(Type1());
        }
        else
        {
            // Handle syntax error
            cerr << "Syntax error: Unexpected token " << currentToken.lexeme << "at Line no " << currentToken.lineNo << endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }
        addNextToken(LPAREN, remObjNode);
        
        addNextToken(RPAREN, remObjNode);
        // consume(RPAREN);
        return remObjNode;
    }

    TreeNode *Name1()
    {
        // Name ::= ID Did
        TreeNode *nameNode = new TreeNode(Name, "Name");
        addNextToken(ID, nameNode);
        nameNode->children.push_back(Did1());
        return nameNode;
    }

    TreeNode *Did1()
    {

        if (currentToken.type == DOT)
        {
            TreeNode *didNode = new TreeNode(Did, "Did");
            addNextToken(DOT, didNode);
            addNextToken(ID, didNode);
            didNode->children.push_back(Did1());
            return didNode;
        }
        else
        {
            return new TreeNode(Did, "Empty");
        }
    }

    TreeNode *BinOp1()
    {
        // BinOp ::= PLUS | MINUS | MULTIPLY | DIVIDE | MOD | AND | OR
        TreeNode *binOpNode = new TreeNode(BinOp, "BinOp");
        switch (currentToken.type)
        {
        case PLUS:
            addNextToken(PLUS, binOpNode);
            break;
        case MINUS:
            addNextToken(MINUS, binOpNode);
            break;
        case MULTIPLY:
            addNextToken(MULTIPLY, binOpNode);
            break;
        case DIVIDE:
            addNextToken(DIVIDE, binOpNode);
            break;
        case MOD:
            addNextToken(MOD, binOpNode);
            break;
        case AND:
            addNextToken(AND, binOpNode);
            break;
        case OR:
            // binOpNode->children.push_back(consume(currentToken.type).lexeme);
            addNextToken(OR, binOpNode);
            break;
        default:
            // Handle syntax error
            cerr << "Syntax error: Unexpected token " << currentToken.lexeme << "at Line no " << currentToken.lineNo << endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }
        return binOpNode;
    }

    TreeNode *FrOp1()
    {
        // FrOp ::= PLUS | MINUS | NOT
        TreeNode *frOpNode = new TreeNode(FrOp, "FrOp");
        switch (currentToken.type)
        {
        case PLUS:
            addNextToken(PLUS, frOpNode);
            break;
        case MINUS:
            addNextToken(MINUS, frOpNode);
            break;
        case NOT:
            // frOpNode->children.push_back(consume(currentToken.type).lexeme);
            addNextToken(NOT, frOpNode);
            break;
        default:
            // Handle syntax error
            cerr << "Syntax error: Unexpected token " << currentToken.lexeme << "at Line no " << currentToken.lineNo << endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }
        return frOpNode;
    }

    TreeNode *BkOp1()
    {
        // BkOp ::= INC | DEC
        TreeNode *bkOpNode = new TreeNode(BkOp, "BkOp");
        switch (currentToken.type)
        {
        case INC:
            addNextToken(INC, bkOpNode);
            break;
        case DEC:
            // bkOpNode->children.push_back(consume(currentToken.type).lexeme);
            addNextToken(DEC, bkOpNode);
            break;
        default:
            // Handle syntax error
            cerr << "Syntax error: Unexpected token " << currentToken.lexeme << "at Line no " << currentToken.lineNo << endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }
        return bkOpNode;
    }

    TreeNode *Expr1()
    {
        // Expr ::= Entity  | LPAREN Expr RPAREN | Expr Binop Expr
        TreeNode *exprNode = new TreeNode(Expr, "Expr");
        if (currentToken.type == LPAREN)
        {
            addNextToken(LPAREN, exprNode);
            exprNode->children.push_back(Expr1());
            addNextToken(RPAREN, exprNode);
        }
        else
        {
            exprNode->children.push_back(Entity1());
        }

        if (isBinaryOperator(currentToken.type))
        {

            exprNode->children.push_back(BinOp1());
            exprNode->children.push_back(Expr1());
        }
        return exprNode;
    }

    // TreeNode* RemExpr1() {
    //     // RemExpr ::= BinOp Expr | Empty
    //     TreeNode* remExprNode = new TreeNode(RemExpr, "RemExpr");
    //     if (isBinaryOperator(currentToken.type)) {
    //         remExprNode->children.push_back(BinOp1());
    //         remExprNode->children.push_back(Expr1());
    //     } else {
    //         remExprNode->children.push_back(new TreeNode(Empty, "Empty"));
    //     }
    //     return remExprNode;
    // }

    TreeNode *BkExpr1()
    {
        // BkExpr ::= ID BkOp
        TreeNode *bkExprNode = new TreeNode(BkExpr, "BkExpr");
        // bkExprNode->children.push_back(consume(ID).lexeme);
        addNextToken(ID, bkExprNode);
        bkExprNode->children.push_back(BkOp1());
        return bkExprNode;
    }

    TreeNode *Entity1()
    {
        // Entity ::= NUM | TRUE | FALSE | STRING | ID | LPAREN Expr RPAREN | Name | ObjConstruct | NOT Factor
        TreeNode *entityNode = new TreeNode(Entity, "Entity");
        switch (currentToken.type)
        {
        case NUM:
            addNextToken(NUM, entityNode);
            break;
        case TRUE:
            addNextToken(TRUE, entityNode);
            break;
        case FALSE:
            addNextToken(FALSE, entityNode);
            break;
        case STRING:
            addNextToken(STRING, entityNode);
            break;
        case ID:
            // entityNode->children.push_back(consume(currentToken.type).lexeme);
            addNextToken(ID, entityNode);
            break;
        default:
            // Handle syntax error
            // entityNode->children.push_back( new TreeNode(Empty, "Empty"));
            // cerr << "Syntax error: Unexpected token " << currentToken.lexeme <<" at : line no "<<currentToken.lineNo<< endl;
            cerr << "Syntax error: Unexpected token " << currentToken.lexeme << "at Line no " << currentToken.lineNo << endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }
        return entityNode;
    }

    TreeNode *Condition1()
    {
        // Condition ::= Expr CompOp Expr
        TreeNode *conditionNode = new TreeNode(Condition, "Condition");
        conditionNode->children.push_back(Expr1());
        conditionNode->children.push_back(CompOp1());
        conditionNode->children.push_back(Expr1());
        return conditionNode;
    }

    TreeNode *CompOp1()
    {
        // CompOp ::= EQ | NEQ | LT | LEQ | GT | GEQ
        TreeNode *compOpNode = new TreeNode(CompOp, "CompOp");
        switch (currentToken.type)
        {
        case EQ:
            addNextToken(EQ, compOpNode);
            break;
        case NEQ:
            addNextToken(NEQ, compOpNode);
            break;
        case LT:
            addNextToken(LT, compOpNode);
            break;
        case LEQ:
            addNextToken(LEQ, compOpNode);
            break;
        case GT:
            addNextToken(GT, compOpNode);
            break;
        case GEQ:
            addNextToken(GEQ, compOpNode);
            break;

        default:
            // Handle syntax error
            cerr << "Syntax error: Unexpected token " << currentToken.lexeme << "at Line no " << currentToken.lineNo << endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }
        return compOpNode;
    }

    TreeNode *ForInit1()
    {
        // ForInit ::= VarDecl | AssignExpr
        TreeNode *forInitNode = new TreeNode(ForInit, "ForInit");
        // forInitNode->children.push_back(VarDecl1());

        if (currentToken.type == ID)
        {
            forInitNode->children.push_back(AssignExpr1());
        }
        else if (currentToken.type == INT || currentToken.type == BOOL || currentToken.type == STRINGTYPE)
        {
            forInitNode->children.push_back(VarDecl1());
        }
        return forInitNode;
    }

    TreeNode *ForUpdate1()
    {
        // ForUpdate ::= BkExpr
        TreeNode *forUpdateNode = new TreeNode(ForUpdate, "ForUpdate");
        forUpdateNode->children.push_back(BkExpr1());
        return forUpdateNode;
    }

    TreeNode *Args1()
    {
        // Args ::= Expr | Empty
        TreeNode *argsNode = new TreeNode(Args, "Args");
        if (currentToken.type == RPAREN)
        {
            argsNode->children.push_back(new TreeNode(Empty, "Empty"));
        }
        else
        {
            argsNode->children.push_back(Expr1());
        }
        return argsNode;
    }

    Token peepnextToken()
    {
        if (currTokenNo < noOfToken - 1)
        {
            return TokenList[currTokenNo + 1];
        }
        exit(1);
    }

    void addNextToken(TokenType expectedType, TreeNode *currNode)
    {
        if (currentToken.type == expectedType)
        {
            Token token = currentToken;
            currNode->children.push_back(new TreeNode(currentToken.type, currentToken.lexeme));
            currentToken = nextToken();
            // return token;
        }
        else
        {
            // Handle syntax error
            cerr << "Syntax error: Expected - " << TokenTypeStrings[expectedType] << " but found " << currentToken.lexeme << "in Line no  " << currentToken.lineNo << endl;
            // cerr << "Syntax error: Unexpected token " << currentToken.lexeme << "at Line no "<<currentToken.lineNo<< endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }
    }

    void consume(TokenType expectedType, TreeNode *currNode = NULL)
    {
        if (currentToken.type == expectedType)
        {
            Token token = currentToken;
            currentToken = nextToken();
            // return token;
        }
        else
        {
            // Handle syntax error
            cerr << "Syntax error: Expected - " << TokenTypeStrings[expectedType] << " but found " << currentToken.lexeme << "in Line no : " << currentToken.lineNo << endl;
            // cerr << "Syntax error: Unexpected token " << currentToken.lexeme << "at Line no "<<currentToken.lineNo<< endl;
            cerr << "Parsing unsuccessful: Stopped due to syntax error " << endl;
            exit(1);
        }
    }

    bool isBinaryOperator(TokenType t)
    {
        if (t == PLUS || t == MINUS || t == MULTIPLY || t == DIVIDE || t == MOD || t == AND || t == OR)
        {
            return true;
        }
        return false;
    }
};

void printTree(TreeNode *root, int depth = 0)
{
    cout << string(depth * 2, ' ') << root->lexeme << endl;

    // Recursively print each child node
    for (TreeNode *child : root->children)
    {
        printTree(child, depth + 1);
    }
}

// Function to write parse tree to DOT file with labels
void writeParseTreeToDot(TreeNode *root, const string &fileName)
{
    ofstream dotFile(fileName);
    if (!dotFile)
    {
        cerr << "Error: Could not open DOT file for writing." << endl;
        return;
    }

    dotFile << "digraph ParseTree {" << endl;
    // Queue for level order traversal
    queue<TreeNode *> q;
    unordered_map<TreeNode *, int> nodeIds;
    int nodeId = 0;

    // Level order traversal to assign IDs to nodes
    q.push(root);
    while (!q.empty())
    {
        TreeNode *currentNode = q.front();
        q.pop();
        string nodeLabel = currentNode->lexeme;
        dotFile << "  " << nodeId << " [label=\"" << nodeLabel << "\"];" << endl;
        nodeIds[currentNode] = nodeId++;
        for (TreeNode *child : currentNode->children)
        {
            q.push(child);
        }
    }

    // Write edges to DOT file
    q.push(root);
    while (!q.empty())
    {
        TreeNode *currentNode = q.front();
        q.pop();
        int parentId = nodeIds[currentNode];
        for (TreeNode *child : currentNode->children)
        {
            int childId = nodeIds[child];
            dotFile << "  " << parentId << " -> " << childId << ";" << endl;
            q.push(child);
        }
    }

    dotFile << "}" << endl;
    dotFile.close();
}

void printToken(vector<Token> &TokenList,string tokenFile )
{

    string filename=tokenFile;
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }

    for (const Token& item : TokenList) {
        outputFile << item.lexeme <<","<<TokenTypeStrings[item.type]<<","<<item.lineNo<< endl;
    }

    outputFile.close();
    cout << "Data successfully written to " << filename << endl;
    // for (int i = 0; i < TokenList.size(); i++)
    // {
    //     // printf("%s -- %s -- %d \n",TokenList[i].lexeme,TokenList[i].type,TokenList[i].lineNo);
    //     cout << TokenList[i].lexeme << "--" << TokenTypeStrings[TokenList[i].type] << "--" << TokenList[i].lineNo << endl;
    // }
}

void exitHandler()
{
    cout << endl
         << endl;
    cerr << "Parsing stopped due to syntax error" << std::endl;
    // Perform any cleanup operations here
}


int main()
{

    // atexit(exitHandler);

    string treeFile,filename,tokenFileName;
    cout<<"Enter input file name : ";
    cin>>filename;
    treeFile="tree.dot";
    tokenFileName="token.txt";


    Parser parser;
    pair<vector<Token>, bool> Tk;
    cout << "Phase 1 - Lexical Analyser (Started)" << endl;
    Tk = getTokenList(filename);
    cout << "Phase 1 - Lexical Analyser (Completed)" << endl;
    if (Tk.second == false)
    {
        cout << "Error : Unknown Token" << endl;
        return 0;
    }
    parser.TokenList = Tk.first;

    printToken(parser.TokenList,tokenFileName);

    parser.setToken();
    cout << "Phase 2 - Parsing (Started)" << endl;
    TreeNode *root = parser.parse();

    cout << "Phase 2- Parsing (Successfully completed)" << endl;
    writeParseTreeToDot(root, treeFile);

    return 0;
}