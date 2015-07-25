/* Victor Paredes and William Drake
This program is an lexical Analzyer for the programming language Little C.
It's job is to take all the characters in the file given to it and then it puts them into lexemes which are given token codes which describe what they are.
Token codes include the int_keyword, RIGHTCURLYBRACE, and SEMICOLON. 

*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* define the longest allowable lexeme length */
#define MAX_LEXEME_LENGTH 100

/* Many programs that do low-level work (like this lexical analyzer)
   use global variables used to avoid passing around lots of
   parameters - think of them like the instance variables of a class.
   We could encapsulate them in a struct or at least declare them as
   static to restrict them to file scope to better restrict
   access.  */

int charClass;  // will hold one of the character class constants defined below
char lexeme[MAX_LEXEME_LENGTH];  // last lexeme processed
char nextChar;  // most recently read char from input
int lexLen;     // how long is current lexeme?
int nextToken;  // most recently processed token
FILE *in_fp;    // FILE pointer to input file
int level;

/* Function prototypes -- very simple since they communicate through globals */
void addChar();
void getChar();
void getNonBlank();
int lex();
void error(char *);
void entryMsg(char *);
void exitMsg(char *);
void match(char *);

/*start the Grammar functions*/

void add_op();
void additive_expression();
void assignment_expression();
void compound_statement();
void conditional_expression();
void conditional_statement();
void constant();
void declaration();
void declaration_list();
void equality_op();
void equality_expression();
void expression_statement();
void floating_type_specifier();
void for_statement();
void for_expressions();
void initialized_declarator_list();
void integer_type_specifier();
void iterative_statement();
void logical_and_expression();
void logical_or_expression();
void multiplicative_expression();
void mult_op();
void null_statement();
void parenthesized_expression();
void primary_expression();
void program();
void relational_expression();
void relational_op();
void statement();
void statement_list();
void type_specifier();
void while_statement();




/* Character classes */
#define LETTER 0
#define DIGIT 1
#define DECIMALPT 2
#define UNDERSCORE 3
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define FLOAT_LIT 12
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define SEMICOLON 27
#define OR_OP 28
#define AND_OP 29
#define EQUAL_OP 30
#define NOTEQUAL_OP 31
#define LESS_OP 32
#define GREATER_OP 33
#define LESSEQUAL_OP 34
#define GREATEREQUAL_OP 35
#define MODULUS_OP 41
#define LEFTCURLYBRACE 45
#define RIGHTCURLYBRACE 46
/*Keyword token codes*/
#define IF_KEYWORD 36
#define WHILE_KEYWORD 37 
#define FOR_KEYWORD 38
#define INT_KEYWORD 39 
#define FLOAT_KEYWORD 40
#define MAIN_KEYWORD 42
#define VOID_KEYWORD 43
#define RETURN_KEYWORD 44
/* main driver */
int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    exit(1);
  }

  /* open the input data file and process contents */
  if ((in_fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "%s: cannot open input file %s for reading\n", 
	    argv[0], argv[1]);
    exit(1);
  }

  /* we have our input file open, we now proceed with lexical analysis */
  /* initialize things by getting the first char */
  getChar();
  do {
    lex(); /* process (and in this case, print info about) next lexeme */
    
  } while (nextToken != EOF);

  fclose(in_fp);
  return 0;
}

/* function to return a printable representation of the defined
   token codes above, given the numeric code */
/* for output purposes only, not part of the lexer */
char *tokenCode(int code) {
  //define string for output
  switch (code) {

  case INT_LIT:
    return "INT_LIT";
  case FLOAT_LIT:
    return "FLOAT_LIT";

  case IDENT:
    return "IDENT";

  case ASSIGN_OP:
    return "ASSIGN_OP";

  case ADD_OP:
    return "ADD_OP";

  case SUB_OP:
    return "SUB_OP";

  case MULT_OP:
    return "MULT_OP";

  case DIV_OP:
    return "DIV_OP";

  case LEFT_PAREN:
    return "LEFT_PAREN";

  case RIGHT_PAREN:
    return "RIGHT_PAREN";

  case OR_OP:
    return "OR_OP";

  case AND_OP:
    return "AND_OP";

  case EQUAL_OP:
    return "EQUAL_OP";

  case NOTEQUAL_OP:
    return "NOTEQUAL_OP";

  case LESS_OP:
    return "LESS_OP";

  case GREATER_OP:
    return "GREATER_OP";

  case LESSEQUAL_OP:
    return "LESSEQUAL_OP";

  case GREATEREQUAL_OP:
    return "GREATEREQUAL_OP";

  case IF_KEYWORD:
    return "IF_KEYWORD";

  case WHILE_KEYWORD:
    return "WHILE_KEYWORD";

  case FOR_KEYWORD:
    return "FOR_KEYWORD";

  case INT_KEYWORD:
    return "INT_KEYWORD";

  case FLOAT_KEYWORD:
    return "FLOAT_KEYWORD";

  case MODULUS_OP:
    return "MODULUS_OP";

  case MAIN_KEYWORD:
    return "MAIN_KEYWORD";
   
  case VOID_KEYWORD:
    return "VOID_KEYWORD";
    
  case RETURN_KEYWORD:
    return "RETURN_KEYWORD";

  case LEFTCURLYBRACE:
    return "LEFTCURLYBRACE";
   
  case RIGHTCURLYBRACE:
    return "RIGHTCURLYBRACE";

  case SEMICOLON:
    return "SEMICOLON";

  default:
    return "UNKNOWN"; 
  } 
} 

/* lookup - a function to look up operators and parentheses -
   note that this both returns one of the token constants defined
   above and sets the nextToken global variable.
*/
int lookup(char ch) { 

  switch (ch) { 
  case '(': 
    addChar(); 
    nextToken = LEFT_PAREN;
    getChar(); 
    break;
  case ')':
    addChar();
    nextToken = RIGHT_PAREN;
    getChar(); 
    break;
  case '+':
    addChar();
    nextToken = ADD_OP;
    getChar();
    break;
  case '-':
    addChar();
    nextToken = SUB_OP;
    getChar();
    break;
  case '*':
    addChar();
    nextToken = MULT_OP;
    getChar();
    break;
  case '/':
    addChar();
    nextToken = DIV_OP;
    getChar();
    break;
    //checks for equals and assign operators
  case '=':
    addChar();
    getChar();
    //checks if second char after first char is also a =
    if(nextChar == '=')
      {
	nextToken = EQUAL_OP;
	getChar();
      }
    else
      {
	nextToken = ASSIGN_OP;
      }
    break;
    //checks if there is a not equals to operator
  case '!':
    addChar();
    getChar();
    if(nextChar == '=')
      {
	nextToken = NOTEQUAL_OP;
	getChar();
      }
    else 
      {
	nextToken = UNKNOWN;
      }
    //checks if there is a AND operator
  case '&':
    addChar();
    getChar();
    if(nextChar == '&')
      {
	nextToken = AND_OP;
      }
    else
      {
	nextToken = UNKNOWN;
      }
    break;
    //check if there is a OR operator
  case '|':
    addChar();
    getChar();
    if(nextChar == '|')
      {
	nextToken = OR_OP;
	getChar();
      }
    else
      {
	nextToken = UNKNOWN;
      }
    //checks if ther is a semicolon
    case ';':
    addChar();
    nextToken = SEMICOLON;
    getChar();
    break;
  case '<':
    addChar();
    getChar();
    if(nextChar == '=')
      {
	nextToken = LESSEQUAL_OP;
        getChar();
      }
    else
      {
	nextToken = LESS_OP;
      }
    break;
  case '{':
    addChar();
    nextToken = LEFTCURLYBRACE;
    getChar();
    break;
  case '}':
    addChar();
    nextToken = RIGHTCURLYBRACE;
    getChar();
    break;
    //checks if it is a greater operator and a greater or equals than operator
  case '>':
    addChar();
    getChar();
    if(nextChar == '=')
      {
	nextToken = GREATEREQUAL_OP;
	getChar();
      }
    else
      {
	nextToken = GREATER_OP;
      }
    break;
  case '%':
    addChar();
    nextToken = MODULUS_OP;
    getChar();
    break;
  default:
    addChar();
    nextToken = EOF;
    break;
  }
  return nextToken;
}

/* print a message that we are exiting a particular function
   and decrement level to restore indentation */
void exitMsg(char *function) {

  level--;

  //printf("<%s> exit\n", function);
}

/* print a message that a token was matched */
void match(char *rule) {

  indent();

  printf("Match: %s Token: %2d (%s) in <%s>\n",
	 lexeme, nextToken, tokenCode(nextToken), rule);

}

/* if an error is encountered, print a message and exit */
void error(char *msg) {

  printf("PARSE ERROR! %s\n", msg);
  exit(1);
}


/* indentation */
void indent() {

  int i;
  for (i=0; i<level-1; i++) {
    printf("| ");
  }
  if (level > 0) printf("+-");

}


/* print a message that we have entered a particular function
   and increment level so any functions called before we exit
   will be indented */
void entryMsg(char *function) {

  indent();

  printf("<%s> entry\n", function);

  level++;
}



/* addChar - a function to add next char to lexeme being processed */
void addChar(){

  if (lexLen <= (MAX_LEXEME_LENGTH-2)) {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = '\0';
  } 
  else {
    fprintf(stderr, "Error - lexeme too long: %s \n", lexeme);
  }
}

/* getChar - a function get the next char of input and determine
   its character class 

   sets global variables nextChar and charClass.
*/
void getChar(){

  if ((nextChar = getc(in_fp)) != EOF) {
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else if (nextChar == '.') 
      charClass = DECIMALPT;
    else if (nextChar == '_')
      charClass = UNDERSCORE;
    else
      charClass = UNKNOWN;
  } 
  else {
    charClass = EOF;
  }
}

/* getNonBlank - a function to call getChar until it returns a
   non-whitespace character

   sets global variables nextChar and charClass via its calls to getChar.
*/
void getNonBlank(){

  while (isspace(nextChar))
    getChar();
}

/* lex - a simple lexical analyzer for arithmetic expressions.

   One call parses one lexeme.  And here, prints out some info about it.
*/
int lex(){

  lexLen = 0;
  getNonBlank();
  switch (charClass) {
  case LETTER:
  case UNDERSCORE:
    /* parse identifiers (would also match keywords if we had any) */
    /* identifiers here start with a letter then can be followed by
       any number of letters and numbers */
    addChar();  // build lexeme
    getChar();  // move to next char of input, see if still part of an IDENT
    while (charClass == LETTER || charClass == DIGIT || charClass == UNDERSCORE){
      addChar(); // continue to build lexeme
      getChar(); // and on to next char
    }
    if(strcmp(lexeme,"int")== 0)
      {
	nextToken = INT_KEYWORD;
      }
    else if(strcmp(lexeme,"float")== 0)
      {
	nextToken = FLOAT_KEYWORD;
      }
    else if(strcmp(lexeme,"if")== 0)
      {
	nextToken = IF_KEYWORD;
      }
    else if(strcmp(lexeme,"for")== 0)
      {
	nextToken = FOR_KEYWORD;
      }
    else if(strcmp(lexeme,"while")==0)
      {
	nextToken = WHILE_KEYWORD;
      }
    else if(strcmp(lexeme, "main")== 0)
      {
	nextToken = MAIN_KEYWORD;
      }
    //sees if it is a return keyword
    else if(strcmp(lexeme, "return")== 0)
      {
	nextToken = RETURN_KEYWORD;
      }
    // sees if it is a void keyword
    else if(strcmp(lexeme, "void")== 0)
      {
	nextToken = VOID_KEYWORD;
      }
    else
      {
	nextToken = IDENT;
      }
    break;
    
  case DIGIT:
    /* parse integer literals - starts with a digit, continue until we
       encounter something else */
    addChar();
    getChar();
    while (charClass == DIGIT){
      addChar();
      getChar();
    }
    if (charClass == DECIMALPT) { // we might have a float
      addChar();
      getChar();
      while (charClass == DIGIT){
	addChar();
	getChar();
      }
      nextToken = FLOAT_LIT;
    }
    else {
      // it was just an int
      nextToken = INT_LIT;
    }
    break;

  case UNKNOWN:
    /* Parentheses and operators -- all are single character in our
       simple language.   So they become tokens in their own right, if
       recognized by lookup, become EOF otherwise.
    */
    lookup(nextChar);
    // getChar();
    break;

  case EOF:
    /* EOF -- return a special EOF token */
    nextToken = EOF;
    lexeme[0] = 'E';
    lexeme[1] = 'O';
    lexeme[2] = 'F';
    lexeme[3] = '\0';
    break;

  } /* end of switch */

  printf("Next token is: %2d (%12s), next lexeme is %s\n",
	 nextToken, tokenCode(nextToken), lexeme);
  return nextToken;
} /* end lex */




//<add-op> ::= + | -
void add_op()
{	
entryMsg("add_op");
if(nextToken == ADD_OP || nextToken == SUB_OP)
{
	match("add_op");
	lex();
}
else
{
	error("Expected terminal + or Expected terminal - was not found!");
}
exitMsg("app_op");
}
//<additive-expression> ::= <multiplicative-expression> [ <add-op> <additive-expression> ]
void additive_expression()
{
	entryMsg("additive_expression");
	multiplicative_expression();
	if(nextToken == ADD_OP || nextToken == SUB_OP)
	{
		add_op();
		additive_expression();
	}
	exitMsg("additive_expression");
}
void assignment_expression()
{
	entryMsg("assignment_expression");
	if(nextToken == IDENT)
	{
		match("assignment_expression");
		lex();
		if(nextToken == ASSIGN_OP)
		{
			match("assignment_expression");
			lex();
			conditional_expression();
		}
		else
		{
			error("Expected token = was not found");
		}
	}
	else
	{
		error("Expected token IDENT was not found");
	}
	exitMsg("assignment_expression");
}
void compound_statement()
{
  entryMsg("compound_statement");

  if (nextToken == LEFTCURLYBRACE)
  {
    match("compound_statement");
    lex();

    if (nextToken == FLOAT_KEYWORD || nextToken == INT_KEYWORD)
    {
      match("compound_statement");
      lex();

      declaration_list();
    }

    if (nextToken == IDENT || nextToken == LEFTCURLYBRACE || nextToken == IF_KEYWORD || nextToken == WHILE_KEYWORD || nextToken == SEMICOLON)
    {
      match("compound_statement");
      lex();

      statement_list();
    }

    if (nextToken == RIGHTCURLYBRACE)
    {
      match("compound_statement");
      lex();
    }
    else
    {
      error("Expected token } was not found.");
    }
  }
  else
  {
    error("Expected token { was not found.");
  }

  exitMsg("compound_statement");
}
void conditional_expression()
{

}
void conditional_statement()
{

}
void constant()
{

}
void declaration()
{

}
void declaration_list()
{

}
void equality_op()
{

}
void equality_expression()
{

}
void expression_statement()
{

}
void floating_type_specifier()
{

}
void for_statement()
{

}
void for_expressions()
{

}
void initialized_declarator_list()
{
  
}
void integer_type_specifier()
{
  entryMsg("integer_type_specifier");

  if (nextToken == INT_KEYWORD)
  {
    match("integer_type_specifier");
    lex();
  }
  else
  {
    error("Expected token int was not found.");
  }

  exitMsg("integer_type_specifier");
}
void iterative_statement()
{
  entryMsg("iterative_statement");

  if (nextToken == WHILE_KEYWORD)
  {
    match("iterative_statement");
    lex();

    while_statement();
  }
  else if (nextToken == FOR_KEYWORD)
  {
    match("iterative_statement");
    lex();

    for_statement();
  }
  else
  {
    error("Expected token was not found");
  }

  exitMsg("iterative_statement");
}
void logical_and_expression()
{
  entryMsg("logical_and_expression");

  equality_expression();

  if (nextToken == AND_OP)
  {
    match("logical_and_expression");
    lex();

    logical_and_expression();
  }

  exitMsg("logical_and_expression");
}
void logical_or_expression()
{
  entryMsg("logical_or_expression");

  logical_and_expression();

  if (nextToken == OR_OP)
  {
    match("logical_or_expression");
    lex();

    logical_or_expression();
  }

  exitMsg("logical_or_expression");
}
void multiplicative_expression()
{
  entryMsg("multiplicative_expression");

  primary_expression();

  if (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MODULUS_OP)
  {
    match("multiplicative_expression");
    lex();

    mult_op();

    multiplicative_expression();
  }

  exitMsg("multiplicative_expression");
}
void mult_op()
{
  entryMsg("mult_op");

  if (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MODULUS_OP)
  {
    match("mult_op");
    lex();
  }
  else
  {
    error("Expected multiplication operator was not found");
  }

  exitMsg("mult_op");
}
void null_statement()
{
  entryMsg("null_statement");

  if (nextToken == SEMICOLON)
  {
    match("null_statement");
    lex();
  }
  else
  {
    error("Expected ; was not found.");
  }

  exitMsg("null_statement");
}
void parenthesized_expression()
{
  entryMsg("parenthesized_expression");

  if (nextToken == LEFT_PAREN)
  {
    match("parenthesized_expression");
    lex();

    conditional_expression();

    if (nextToken == RIGHT_PAREN)
    {
      match("parenthesized_expression");
      lex();
    }
    else
    {
      error("Expected token ) was not found.");
    }
  }
  else
  {
    error("Expected token ( was not found.");
  }

  exitMsg("parenthesized_expression");
}
void primary_expression()
{
  entryMsg("primary_expression");

  if (nextToken == IDENT)
  {
    match("primary_expression");
    lex();
  }
  else if (nextToken == INT_LIT || nextToken == FLOAT_LIT)
  {
    match("primary_expression");
    lex();

    constant();
  }
  else if (nextToken == LEFT_PAREN)
  {
    match("primary_expression");
    lex();

    parenthesized_expression();
  }
  else
  {
    error("Expected token was not found.");
  }

  exitMsg("primary_expression");
}
void program()
{
  entryMsg("program");

  if (nextToken == VOID_KEYWORD)
  {
    match("program");
    lex();

    if (nextToken == MAIN_KEYWORD)
    {
      match("program");
      lex();

      if (nextToken == LEFT_PAREN)
      {
        match("program");
        lex();

        if (nextToken == RIGHT_PAREN)
        {
          match("program");
          lex();

          compound_statement();
        }
        else
        {
          error("Expected token ) was not found.");
        }
      }
      else
      {
        error("Expected token ( was not found.");
      }
    }
    else
    {
      error("Expected token main was not found.");
    }
  }
  else
  {
    error("Expected token void was not found.");
  }

  exitMsg("program");
}
void relational_expression()
{
  entryMsg("relational_expression");

  additive_expression();

  if (nextToken == LESS_OP || nextToken == LESSEQUAL_OP || nextToken == GREATER_OP || nextToken == GREATEREQUAL_OP)
  {
    relational_op();

    relational_expression();
  }

  exitMsg("relational_expression");
}
void relational_op()
{
  entryMsg("relational_op");

  if (nextToken == LESS_OP || nextToken == LESSEQUAL_OP || nextToken == GREATER_OP || nextToken == GREATEREQUAL_OP)
  {
    match("relational_op");
    lex();
  }
  else
  {
    error("Expected relational operator not found.");
  }

  exitMsg("relational_op");
}
void statement()
{
  entryMsg("statement");

  if (nextToken == IDENT)
  {
    match("statement");
    lex();

    expression_statement();
  }
  else if (nextToken == LEFTCURLYBRACE)
  {
    match("statement");
    lex();

    compound_statement();
  }
  else if (nextToken == IF_KEYWORD)
  {
    match("statement");
    lex();

    conditional_statement();
  }
  else if (nextToken == WHILE_KEYWORD)
  {
    match("statement");
    lex();

    iterative_statement();
  }
  else if (nextToken == SEMICOLON)
  {
    match("statement");
    lex();

    null_statement();
  }
  else
  {
    error("Expected token was not found.");
  }

  exitMsg("statement");
}
void statement_list()
{
  entryMsg("statement_list");

  statement();

  if (nextToken == IDENT || nextToken == LEFTCURLYBRACE || nextToken == IF_KEYWORD || nextToken == WHILE_KEYWORD || nextToken == SEMICOLON)
  {
    match("statement_list");
    lex();

    statement_list();
  }

  exitMsg("statement_list");
}
void type_specifier()
{
  entryMsg("type_specifier");

  if (nextToken == FLOAT_KEYWORD)
  {
    match("type_specifier");
    lex();

    floating_type_specifier();
  }
  else if (nextToken == INT_KEYWORD)
  {
    match("type_specifier");
    lex();

    integer_type_specifier();
  }
  else
  {
    error("Expected token 'float' | 'int' was not found.");
  }

  exitMsg("type_specifier");
}
void while_statement()
{
  entryMsg("while_statement");

  if (nextToken == WHILE_KEYWORD)
  {
    match("while_statement");
    lex();

    if (nextToken == LEFT_PAREN)
    {
      match("while_statement");
      lex();

      conditional_expression();

      if (nextToken == RIGHT_PAREN)
      {
        match("while_statement");
        lex();

        statement();
      }
      else
      {
        error("Expected token ) was not found.");
      }
    }
    else
    {
      error("Expected token ( was not found.");
    }
  }
  else
  {
    eroor("Expected token while was not found.");
  }

  exitMsg("while_statement");

}
