static TreeNode* getAll_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// Cycle started                     
	currentNode = createSemicolonNode(); 
	returnNode = currentNode;            
	while (*pos < tokensStk->size)       
	{                                    
	unsigned int oldPos = *pos;          
#define return break                   
#define nullptr                      

	// getGlobalChunk()                           
	errStr = getGlobalChunk_errStr;
	tempNode = getGlobalChunk(fe, tree, tokensStk, pos);
	if (!tempNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	currentNode-> leftBranch = tempNode;             
	currentNode->rightBranch = createSemicolonNode();
	currentNode = currentNode->rightBranch;          
#undef return                                      
#undef nullptr                                     
	}                                              

	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getAll(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getAll_0(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getGlobalChunk_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getFunction()                           
	errStr = getFunction_errStr;
	returnNode = getFunction(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getGlobalChunk_1(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getDeclarationStatement()                           
	errStr = getDeclarationStatement_errStr;
	returnNode = getDeclarationStatement(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getGlobalChunk(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getGlobalChunk_0(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getGlobalChunk_1(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getExpressionUnit_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getIdentifier()                           
	errStr = getIdentifier_errStr;
	returnNode = getIdentifier(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getExpressionUnit_1(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getIdentifier()                           
	errStr = getIdentifier_errStr;
	returnNode = getIdentifier(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// '('                        
	errStr = "expected a \'(\'";
	if (!isOperation(OPER_TYPE_LBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	isErrorReal = !isErrorReal;     

	// ')'                        
	errStr = "expected a \')\'";
	if (!isOperation(OPER_TYPE_RBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getExpressionUnit_2(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getNumber()                           
	errStr = getNumber_errStr;
	returnNode = getNumber(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getExpressionUnit_3(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// '('                        
	errStr = "expected a \'(\'";
	if (!isOperation(OPER_TYPE_LBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	// getExpression()                           
	errStr = getExpression_errStr;
	returnNode = getExpression(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	isErrorReal = !isErrorReal;     

	// ')'                        
	errStr = "expected a \')\'";
	if (!isOperation(OPER_TYPE_RBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getExpressionUnit(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getExpressionUnit_0(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getExpressionUnit_1(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getExpressionUnit_2(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getExpressionUnit_3(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getMultiplicativeExpression_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getExpressionUnit()                           
	errStr = getExpressionUnit_errStr;
	leftNode = getExpressionUnit(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// '*'                        
	errStr = "expected a \'*\'";
	if (!isOperation(OPER_TYPE_MUL))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	isErrorReal = !isErrorReal;     

	// getMultiplicativeExpression()                           
	errStr = getMultiplicativeExpression_errStr;
	rightNode = getMultiplicativeExpression(fe, tree, tokensStk, pos);
	if (!rightNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getMultiplicativeExpression_1(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getExpressionUnit()                           
	errStr = getExpressionUnit_errStr;
	leftNode = getExpressionUnit(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// '/'                        
	errStr = "expected a \'/\'";
	if (!isOperation(OPER_TYPE_DIV))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	isErrorReal = !isErrorReal;     

	// getMultiplicativeExpression()                           
	errStr = getMultiplicativeExpression_errStr;
	rightNode = getMultiplicativeExpression(fe, tree, tokensStk, pos);
	if (!rightNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getMultiplicativeExpression_2(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getExpressionUnit()                           
	errStr = getExpressionUnit_errStr;
	leftNode = getExpressionUnit(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// '%'                        
	errStr = "expected a \'%\'";
	if (!isOperation(OPER_TYPE_RMD))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	isErrorReal = !isErrorReal;     

	// getMultiplicativeExpression()                           
	errStr = getMultiplicativeExpression_errStr;
	rightNode = getMultiplicativeExpression(fe, tree, tokensStk, pos);
	if (!rightNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getMultiplicativeExpression_3(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getExpressionUnit()                           
	errStr = getExpressionUnit_errStr;
	returnNode = getExpressionUnit(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getMultiplicativeExpression(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getMultiplicativeExpression_0(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getMultiplicativeExpression_1(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getMultiplicativeExpression_2(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getMultiplicativeExpression_3(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getAdditiveExpression_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getMultiplicativeExpression()                           
	errStr = getMultiplicativeExpression_errStr;
	leftNode = getMultiplicativeExpression(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// '+'                        
	errStr = "expected a \'+\'";
	if (!isOperation(OPER_TYPE_ADD))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	isErrorReal = !isErrorReal;     

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// getAdditiveExpression()                           
	errStr = getAdditiveExpression_errStr;
	rightNode = getAdditiveExpression(fe, tree, tokensStk, pos);
	if (!rightNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getAdditiveExpression_1(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getMultiplicativeExpression()                           
	errStr = getMultiplicativeExpression_errStr;
	leftNode = getMultiplicativeExpression(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// '-'                        
	errStr = "expected a \'-\'";
	if (!isOperation(OPER_TYPE_SUB))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	isErrorReal = !isErrorReal;     

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// getAdditiveExpression()                           
	errStr = getAdditiveExpression_errStr;
	rightNode = getAdditiveExpression(fe, tree, tokensStk, pos);
	if (!rightNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getAdditiveExpression_2(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getMultiplicativeExpression()                           
	errStr = getMultiplicativeExpression_errStr;
	returnNode = getMultiplicativeExpression(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getAdditiveExpression(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getAdditiveExpression_0(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getAdditiveExpression_1(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getAdditiveExpression_2(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getExpression_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getAdditiveExpression()                           
	errStr = getAdditiveExpression_errStr;
	returnNode = getAdditiveExpression(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getExpression(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getExpression_0(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getFunction_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// getSpecifier()                           
	errStr = getSpecifier_errStr;
	returnNode = getSpecifier(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// getIdentifier()                           
	errStr = getIdentifier_errStr;
	leftNode = getIdentifier(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// '('                        
	errStr = "expected a \'(\'";
	if (!isOperation(OPER_TYPE_LBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	isErrorReal = !isErrorReal;     

	// ')'                        
	errStr = "expected a \')\'";
	if (!isOperation(OPER_TYPE_RBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// getStatementList()                           
	errStr = getStatementList_errStr;
	rightNode = getStatementList(fe, tree, tokensStk, pos);
	if (!rightNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getFunction(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getFunction_0(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getFunctionCall_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getIdentifier()                           
	errStr = getIdentifier_errStr;
	returnNode = getIdentifier(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// '('                        
	errStr = "expected a \'(\'";
	if (!isOperation(OPER_TYPE_LBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	isErrorReal = !isErrorReal;     

	// ')'                        
	errStr = "expected a \')\'";
	if (!isOperation(OPER_TYPE_RBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getFunctionCall(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getFunctionCall_0(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getAssign_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getIdentifier()                           
	errStr = getIdentifier_errStr;
	rightNode = getIdentifier(fe, tree, tokensStk, pos);
	if (!rightNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// '='                        
	errStr = "expected a \'=\'";
	if (!isOperation(OPER_TYPE_ASGN))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	isErrorReal = !isErrorReal;     

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// getExpression()                           
	errStr = getExpression_errStr;
	leftNode = getExpression(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getAssign(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getAssign_0(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getDeclaration_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getSpecifier()                           
	errStr = getSpecifier_errStr;
	returnNode = getSpecifier(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// getAssign()                           
	errStr = getAssign_errStr;
	leftNode = getAssign(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getDeclaration(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getDeclaration_0(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getStatement_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getDeclarationStatement()                           
	errStr = getDeclarationStatement_errStr;
	returnNode = getDeclarationStatement(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getStatement_1(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getAssignStatement()                           
	errStr = getAssignStatement_errStr;
	returnNode = getAssignStatement(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getStatement_2(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getConditionStatement()                           
	errStr = getConditionStatement_errStr;
	returnNode = getConditionStatement(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getStatement_3(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getReturnStatement()                           
	errStr = getReturnStatement_errStr;
	returnNode = getReturnStatement(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getStatement(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getStatement_0(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getStatement_1(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getStatement_2(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getStatement_3(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getStatementList_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// '{'                          
	errStr = "expected a \'{\'";
	if (!isPunctuation(PUNC_TYPE_FIGURE_L))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                      

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// Cycle started                     
	currentNode = createSemicolonNode(); 
	returnNode = currentNode;            
	while (*pos < tokensStk->size)       
	{                                    
	unsigned int oldPos = *pos;          
#define return break                   
#define nullptr                      

	// getStatement()                           
	errStr = getStatement_errStr;
	tempNode = getStatement(fe, tree, tokensStk, pos);
	if (!tempNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	currentNode-> leftBranch = tempNode;             
	currentNode->rightBranch = createSemicolonNode();
	currentNode = currentNode->rightBranch;          
#undef return                                      
#undef nullptr                                     
	}                                              

	isErrorReal = !isErrorReal;     

	// '}'                          
	errStr = "expected a \'}\'";
	if (!isPunctuation(PUNC_TYPE_FIGURE_R))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                      

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getStatementList_1(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	isErrorReal = !isErrorReal;     

	// getStatement()                           
	errStr = getStatement_errStr;
	returnNode = getStatement(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getStatementList(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getStatementList_0(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getStatementList_1(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getConditionStatement_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// <IF>                         
	errStr = "expected a \'IF\'";
	if (!isKeyword(KEY_WORD_IF))    
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	createNode(returnNode);         
	currentNode = returnNode;       
	(*pos)++;                     

	isErrorReal = !isErrorReal;     

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// '('                        
	errStr = "expected a \'(\'";
	if (!isOperation(OPER_TYPE_LBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	// getExpression()                           
	errStr = getExpression_errStr;
	leftNode = getExpression(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// ')'                        
	errStr = "expected a \')\'";
	if (!isOperation(OPER_TYPE_RBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// getStatementList()                           
	errStr = getStatementList_errStr;
	rightNode = getStatementList(fe, tree, tokensStk, pos);
	if (!rightNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getConditionStatement_1(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// <WHILE>                         
	errStr = "expected a \'WHILE\'";
	if (!isKeyword(KEY_WORD_WHILE))    
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	createNode(returnNode);         
	currentNode = returnNode;       
	(*pos)++;                     

	isErrorReal = !isErrorReal;     

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// '('                        
	errStr = "expected a \'(\'";
	if (!isOperation(OPER_TYPE_LBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	// getExpression()                           
	errStr = getExpression_errStr;
	leftNode = getExpression(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// ')'                        
	errStr = "expected a \')\'";
	if (!isOperation(OPER_TYPE_RBR))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                    

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// getStatementList()                           
	errStr = getStatementList_errStr;
	rightNode = getStatementList(fe, tree, tokensStk, pos);
	if (!rightNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getConditionStatement(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getConditionStatement_0(fe, tree, tokensStk, pos);  
	}                                              

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getConditionStatement_1(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getAssignStatement_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getAssign()                           
	errStr = getAssign_errStr;
	returnNode = getAssign(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	isErrorReal = !isErrorReal;     

	// ';'                          
	errStr = "expected a \';\'";
	if (!isPunctuation(PUNC_TYPE_SEMICOLON))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                      

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getAssignStatement(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getAssignStatement_0(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getDeclarationStatement_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	// getDeclaration()                           
	errStr = getDeclaration_errStr;
	returnNode = getDeclaration(fe, tree, tokensStk, pos);
	if (!returnNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	isErrorReal = !isErrorReal;     

	// ';'                          
	errStr = "expected a \';\'";
	if (!isPunctuation(PUNC_TYPE_SEMICOLON))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                      

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getDeclarationStatement(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getDeclarationStatement_0(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

static TreeNode* getReturnStatement_0(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                   
	ASSERT_ALL();                     
	bool isErrorReal = false;       

	const char* errStr = "";      

	TreeNode*  returnNode = nullptr;  
	TreeNode* currentNode = nullptr;  
	TreeNode*    leftNode = nullptr;  
	TreeNode*   rightNode = nullptr;  
	TreeNode*    tempNode = nullptr;

	returnNode = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
	// <RETURN>                         
	errStr = "expected a \'RETURN\'";
	if (!isKeyword(KEY_WORD_RETURN))    
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	createNode(returnNode);         
	currentNode = returnNode;       
	(*pos)++;                     

	isErrorReal = !isErrorReal;     

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	// getExpression()                           
	errStr = getExpression_errStr;
	leftNode = getExpression(fe, tree, tokensStk, pos);
	if (!leftNode)                          
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                    
	// ';'                          
	errStr = "expected a \';\'";
	if (!isPunctuation(PUNC_TYPE_SEMICOLON))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
	(*pos)++;                      

	// _                                         
	errStr = "missing space";                  
	if (!getWhiteSpace(fe, tree, tokensStk, pos))
	{                                  
		if (isErrorReal)                 
		{                                
			pushSyntaxError(*pos, errStr); 
		}                                
		return nullptr;                  
	}                                  
                                               
	if (!returnNode->rightBranch)
		returnNode->rightBranch = rightNode;
	if (!returnNode->leftBranch)
		returnNode-> leftBranch =  leftNode;
	return returnNode;
}                 

static TreeNode* getReturnStatement(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{                                  
	ASSERT_ALL();                    
	unsigned int oldPos = *pos;      
	TreeNode* returnNode = nullptr;

	if (!returnNode)                                 
	{                                                
		*pos = oldPos;                                 
		returnNode = getReturnStatement_0(fe, tree, tokensStk, pos);  
	}                                              

	return returnNode;
}                 

