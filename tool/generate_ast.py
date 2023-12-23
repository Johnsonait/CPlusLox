import argparse
from typing import List

def main(dir :str):
    defineAst(
        outputDir=dir,
        baseName= "Expr",
        types = [
            "Binary   : std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right",
            "Grouping : std::shared_ptr<Expr> expression",
            "Literal  : std::variant<double,std::string,std::monostate> value",
            "Unary    : Token op, std::shared_ptr<Expr> right"
        ]
    )


def defineAst(outputDir : str, baseName : str, types : List[str]) -> None:
    path = outputDir + "/" +  baseName.lower()

    hpp_write(path,baseName,types)
    cpp_write(path,baseName,types)


def defineType(file,baseName,className,fieldList):
    """
    Writes in the definitions for concrete types
    """
    file.write(f"class {className} : public {baseName} {'{'}\n")
    file.write(f"public:\n")

    file.write(f"\t{className}({fieldList}) {'{'}\n")
    fields = fieldList.split(", ")
    for field in fields:
        name = field.split(" ")[1]
        file.write(f"\t\t this->{name} = {name}; \n")
    file.write(f"\t{'}'}\n")

    # Desctructor override
    file.write(f"\tvirtual ~{className}() override = default;\n\n")

    # Visitor patter
    templ = "T"
    file.write(f"\ttemplate<typename {templ}>\n")
    file.write(f"\t{templ} accept(std::shared_ptr<Visitor<{templ}>> visitor) {'{'}\n")
    file.write(f"\t\treturn visitor.visit{className}{baseName}(this);\n")
    file.write(f"\t{'}'}\n")

    # Field setup
    for field in fields:
        file.write(f"\t{field};\n")


    file.write(f"{'}'};\n")

def defineVisitor(file,baseName,types):
    templ = "T"
    file.write(f"template<typename {templ}>\n")
    file.write(f"class Visitor {'{'}\n")
    
    for type in types:
        typeName = type.split(":")[0].strip()
        file.write(f"\tvirtual {templ} visit{typeName}{baseName}({typeName} {baseName.lower()}) = 0;\n")



    file.write(f"{'}'};\n\n")


def hpp_write(path : str, baseName : str, types : List[str]) -> None:
    hpp_path = path + ".hpp"

    with open(hpp_path,'w') as f_hpp:
        f_hpp.write(f"#ifndef {baseName.upper()}_HPP\n")
        f_hpp.write(f"#define {baseName.upper()}_HPP\n\n")

        f_hpp.write("#include \"token.hpp\"\n\n")

        f_hpp.write("#include <list>\n")
        f_hpp.write("#include <string>\n")
        f_hpp.write("#include <variant>\n")
        f_hpp.write("#include <memory>\n")
        f_hpp.write("\n")

        f_hpp.write("namespace Lox {\n\n")
        
        # Set up base class
        f_hpp.write(f"class {baseName} {'{'}\n\n")
        f_hpp.write("public:\n")
        f_hpp.write(f"\t{baseName}() = default;\n")
        f_hpp.write(f"\tvirtual ~{baseName}() = 0;\n\n")
        
        templ = "T"
        f_hpp.write(f"\ttemplate<typename {templ}>\n")
        f_hpp.write(f"\t{templ} accept(std::shared_ptr<Visitor<{templ}>> visitor) {'{'}\n")
        f_hpp.write(f"\t{'}'}\n")

        f_hpp.write("};\n\n")

        # Generate each subclass of the Expr class
        for type in types:
            className = type.split(" : ")[0].strip()
            fields = type.split(" : ")[1].strip()
            defineType(f_hpp, baseName, className, fields)
            f_hpp.write("\n")

        # Define visitor(s)
        defineVisitor(f_hpp,baseName,types)

        f_hpp.write("} // namespace Lox\n\n")

        f_hpp.write("#endif")

def cpp_write(path : str, baseName : str, types : List[str]) -> None:
    cpp_path = path + ".cpp"


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--dir",type=str,default="generated",help="Directory to store generated files")
    args = parser.parse_args()

    main(dir = args.dir)