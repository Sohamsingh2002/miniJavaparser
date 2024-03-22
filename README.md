
Our Java Lexer and Parser system effectively tokenizes and parses Java source code, implementing key functionalities to ensure accurate analysis and processing. The Lexer component employs regular expressions to recognize tokens such as keywords (PUBLIC, CLASS, IF, ELSE, WHILE, FOR), identifiers (ID), literals (NUM, TRUE, FALSE, STRING), and punctuation symbols (LPAREN, RPAREN, LBRACE, RBRACE, SEMI). Robust error handling mechanisms are integrated to detect and report lexical errors, ensuring the reliability of tokenization.

The Parser utilizes a recursive descent parsing strategy, adhering to the grammar rules of the Java language to construct a parse tree. Each non-terminal symbol in the grammar corresponds to a parsing method, facilitating the recognition of language constructs such as classes (MainClassDecl), methods (MethodCall), statements (SingleStatement), and expressions (Expr). The grammar rules guide the parsing process, enabling the identification and construction of parse tree nodes representing various language elements.

Error handling is seamlessly integrated into the Parser, providing informative messages for syntax errors encountered during parsing. The grammar specification serves as the foundation for parsing, ensuring adherence to language rules and structures. The Parser handles complex language constructs such as loops (WhileLoop, ForLoop), conditional statements (BranchStatement), variable declarations (VarDecl), and expressions (Expr) with precision and efficiency.

Performance optimizations, including caching and lookahead optimization, enhance parsing speed and efficiency, making the Lexer and Parser suitable for handling large codebases. The system offers APIs and command-line interfaces for seamless integration into software development workflows. Continuous improvement and maintenance are prioritized to address bug fixes, incorporate new language features, and enhance overall performance based on user feedback and community contributions.

In summary, our Java Lexer and Parser system provides a reliable solution for developers seeking to analyze and process Java source code effectively, enabling robust software development practices and tooling support.

Additionally, our system includes functionality to generate a parse tree in  DOT file format, allowing users to visualize the hierarchical structure of the parsed Java source code. This parse tree representation captures the relationships between language constructs, displaying parent-child relationships between nodes corresponding to non-terminal symbols, terminals, and other language elements. By leveraging tools such as Graphviz simulator, users can visualize the parse tree graphically, gaining insights into the syntactic structure of the code. This visualization aids in understanding the parsing process, identifying potential errors or inconsistencies, and facilitating code comprehension and analysis.

Here is parse tree for our input java code:
![graphviz](https://github.com/Sohamsingh2002/miniJavaparser/assets/99115955/da225e42-905a-4e9e-9638-c7b1256d7d28)

