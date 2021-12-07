# Code Stye

Greaper is written using a particular code style, the main focus of this style is have clean interfaces that should be understandable by just doing a quick read, but also, maybe, not as clean source files but easy to navigate ones.
This will be enforced to use if you want to collaborate to Greaper project but recomended if you are just making a module.
This style is very influenced by my background as C with classes style and a lot of Win32 programming, but with long names (if needed to understand their purpose) that with things like intellisense or other auto-complete tools make them easy to write.


## Naming

Class or struct names must follow this style:
        class ClassName
if it's an interface
		class IClassName

Almost in no circumstance a class or struct names can start with a lower letter.

- variables:
	- private: m_VariableName
	- protected: m_VariableName
	- public: VariableName (unless its name collides with another name, which in that case lower letter or an underscore may be used)
	- global or static variable: gVariableName or VARIABLE_NAME or VariableName
	- public but should not be used by the public: _VariableName (use this as less as possible)
	- in function declaration (const ClassName& className, StructName* structName)
	- inside a function: VariableType variableName
	
- functions:
	- private/protected/public/static/global: FunctionName(...) (if the name collides functionName)
	- public but should not be used by the public: _FunctionName(...)
	
- templates parameters: template<typename T, ClassName TClassName>

- macros: TEST_MACRO_FUNCTION(var0, var1, ...)

## Files

Files must start with a header comment where you can state who belongs that file or by which copyright owner is covered. You can add more information to that comment.

- source files: Almost nothing else must be done, you can write your code letting at least a blank line between the header and the code. The includes must be on top of the file after the header, unless very few certain conditions.

- header files: The same rules as source files applies, but, after the header you must add #pragma once (we assume all compilers support it), after that add the include guards unless certain conditions (its a header implementation file .inl, or a file that may trigger reinclude on purpose).
The header guards must follow this rule:
if the file holds a class
		#ifndef PROJECTNAME_CLASS_NAME_H
		#define PROJECTNAME_CLASS_NAME_H 1
		
		...
		
		#endif /* PROJECTNAME_CLASS_NAME_H */
if it does not hold a class, instead of CLASS_NAME, some representative name following the same naming scheme.
After the header guards and before a blank line must follow.
Then you can start your code, its recomended to start with a namespace, you must not use the 'using namespace ...' in a header unless certain conditions (for example, using namespace std::string_literals is permited but only in one file and must have header guards).

## Code

The code must need as less comments as possible, therefore, just by reading the function name you should understand what does it do and so on. This also includes, variable names, class/struct names, macros, template arguments (you can use T or T1 and T2 if reading what are making a template of gives the necesary context ex: template<class T> Vec2)

A comentary on top of a class that its scope can be big or not clear it will be appreciated.

You must let at least an empty line between function declarations so a comment may be added after and also to follow with the style

## Examples
- Header example
		/***********************************************************************************
		*   Copyright 2021 Marcos Sánchez Torrent.                                         *
		*   All Rights Reserved.                                                           *
		***********************************************************************************/
		
		#ifndef CS_EXAMPLE_H
		#define CS_EXAMPLE_H 1
		
		namespace greaper::cs
		{
		#define CS_NAMING(cs, name) #cs "_" #name
		
			INLINE String CreateNamingScheme(const String& codeStyleName, const String& text) noexcept
			{
				return CS_NAMING(codeStyleName, text) "," + codeStyleName + "," + text;
			}
			
			class CodeStyle : public ICodeStyle
			{
				static constexpr int32 CodeStyleVersion = 15;
				
				Vector<String> m_Verification;
			protected:
				void OnVerification(CSContext* context, const String& text)override;
				
			public:
				bool TestVerification(const String& text)const noexcept override;
				
				inline static constexpr StringView StyleName = "TestStyle"sv;
			};
		}
		
		#endif /* CS_EXAMPLE_H */
- Source file example
		/***********************************************************************************
		*   Copyright 2021 Marcos Sánchez Torrent.                                         *
		*   All Rights Reserved.                                                           *
		***********************************************************************************/
		
		#include "CSExample.h"
		
		using namespace greaper;
		using namespace greaper::cs;
		
		bool CodeStyle::TestVerification(const String& text)const noexcept
		{
			if(text.empty())
				return true;
			
			CSContext* globalContext = GetGlobalContext();
			
			OnVerification(globalContext, text);
			
			return globalContext->VerificationResult.IsOK();
		}
