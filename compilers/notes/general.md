# COMPILERS
---

## General

- source program --> 
- **prepocessor** --> 
- mod. src. prog. --> 
- **compiler** --> 
- target ass. prog. --> 
- **assembler** --> 
- relocatable machine code  --> 
- **linker/loader** (lib files, relocatable object files) -->
- target machine code

Break compilation up into *analysis* and *synthesis*.   
- *analysis*: 
break program into constituent pieces, and 
impose grammatical structure. Ensure sound
syntax & semantics.
Produce "intermediate representation" of 
program.
Collect data in **symbol table**.
**front end**. 
- *synthesis*:  
**back end**

#### phases of a compiler

- character stream --> **lexical analyser**
- token stream --> **syntax analyser**
- syntax tree --> **semantic analyser**
- syntax tree --> **intermediate code generation**
- intermediate representation --> **machine-independent code optimiser** 
- intermediate representation --> **code generator**
- target-machine code --> **machine-dependent code optimiser** 
- target-machine code

"The **symbol table** is used by all stages of the compiler."

##### lexical analysis

aka *scanning*.

groups characters into meaningful sequences called *lexemes*. 
For each lexeme, it outputs a *token* of the form
'''
<token-name, attribute-value>
'''
token-name is an abstract symbol, used during syntax analysis. 
attribute-value points to an entry in the symbol table.

##### syntax analysis

aka *parsing*

parses first components of the token (ID) to create 
tree-like intermediate representation depicting grammatical 
structure of token stream.

##### semantic analysis

Ensures semantic consistency with language specification.

Gathers type information and stores it in either 
symbol table or syntax tree.

Performs **type checking** (for example type-coercion 
eg. int + float -> int).

##### intermediate code generation

During compilation, source program will be 
translated into one or more intermediate languages 
(syntax tree is one such language).

The intermediate code produced in this step should be 
- easy to produce
- easy to translate

Example of intermediate form is *three-address code*, 
which consists of assembly-like instructions with 
three operands.

##### code optimisation

Optimise the intermediate code. Usually for performance, 
but could be code-size and powerconsumption.

##### code generation

Maps intermediate code/representation to target language.

Eg. for machine code it would select registers 
& memory locations.

##### passes

one or more phases in compilation may be 
grouped into one pass. Passes can interface 
with one or more other passes that do not 
have overlapping phases. Eg. compile source 
to known intermediate, for which there exist 
several backend passes, that can produce 
machine code for different target systems.

#### tools

1. **parser generators**: automatically produce syntax 
analyser, based on grammatical description of language.
2. **scanner generators**: produce lexical analysers 
from regular expression description of tokens of language.
3. **syntax-directed translation engines**: produce 
collections of routines for walking a parse tree and 
generating intermediate code.
4. **code-generator generators**: prudce code generator 
from collection of rules for translating each operation 
of intermediate language into machine language for a 
target machine.
5. **data-flow analysis engines**: facilitate gathering 
of information about how values are transmitted from 
one part of a program to each other. *key* path to 
code optimisation.
6. **compiler-construction toolkits**: provide 
integrated set of routines for constructing 
various phases of a compiler.

#### misc

**Scope Rules**. The scope of a declaration of x is 
the context in which uses of x refer to this declaration. 
A language uses static scope or lexical scope
if it is possible to determine the scope of a 
declaration by looking only at the program. 
Otherwise, the language uses dynamic scope.

**Environments**. The association of names with locations in memory and
then with values can be described in terms of environments, which map
names to locations in store, and states, which map locations to their
values.

**Scope**.
- Static/Lexical : compiler searches outwards in lexical order 
for the nearest definition.
- Dynamic : instead of lexical order, it is is chronological 
order; latest declaration.

## simple syntax-directed translator

#### Grammar

calculator:

left-associative: + -
left-associative: * / (higher presedence)

factor -> digit | (expr)

term   -> term * factor
       -> term / factor
       -> factor

expr   -> expr + term
       -> expr - term
       -> term

Hence:

expr   -> expr + term | expr - term | term
term   -> term * factor | term / factor | factor
factor -> digit | (expr)
