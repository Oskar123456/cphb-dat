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
- syntax tree --> **sematic analyser**
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

'<token-name, attribute-value>'

token-name is an abstract symbol, used during syntax analysis. 
attribute-value points to an entry in the symbol table.




