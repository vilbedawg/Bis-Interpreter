#include "../include/Interpreter.hpp"
#include "../include/Lexer.hpp"
#include "../include/Logger.hpp"
#include "../include/Parser.hpp"
#include "../include/Resolver.hpp"

#include <fstream>

std::string readFile(std::string_view filename)
{
    std::ifstream file{filename.data(), std::ios::ate};
    if (!file)
    {
        std::cerr << "Failed to open file " << filename.data() << '\n';
        std::exit(74); // I/O error
    }
    std::string file_contents;
    file_contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(file_contents.data(), file_contents.size());
    file.close();

    return file_contents;
}

void run(const std::string& source)
{
    Lexer lexer{source};
    auto tokens = lexer.scanTokens();

    Parser parser{std::move(tokens)};
    const auto statements = parser.parse();

    // Stop if there were any syntax errors.
    if (Error::hadError)
    {
        Error::report();
        return;
    }

    Interpreter interpreter;

    Resolver resolver{interpreter};
    resolver.resolve(statements);

    // Stop if there were any resolution errors.
    if (Error::hadError)
    {
        Error::report();
        return;
    }

    interpreter.interpret(statements);

    // Report all runtime errors, if any.
    if (Error::hadRuntimeError)
    {
        Error::report();
    }
}

void initFile(const std::string& filename)
{
    std::string file_contents = readFile(filename);
    run(file_contents);
    if (Error::hadError)
    {
        std::exit(65);
    }
    if (Error::hadRuntimeError)
    {
        std::exit(70);
    }
}

void runPrompt()
{
    while (true)
    {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line))
        {
            return;
        }

        run(line);
        if (Error::hadError)
        {
            std::exit(65);
        }
        if (Error::hadRuntimeError)
        {
            std::exit(70);
        }
        std::cout << "\n";
    }
}

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        std::exit(64);
    }
    else if (argc == 2)
    {
        initFile(argv[1]);
    }
    else
    {
        runPrompt();
    }

    return 0;
}
