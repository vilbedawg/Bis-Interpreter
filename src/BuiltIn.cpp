#include "../include/BuiltIn.hpp"

// Native clock
size_t ClockCallable::getArity() const
{
    return 0u;
}

std::any ClockCallable::call(Interpreter& interpreter, const std::vector<std::any>& args) const
{
    static_assert(std::is_integral_v<std::chrono::system_clock::rep>,
                  "Representation of ticks isn't an integral value.");

    // Returns Unix time in seconds.
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(now).count());
}

std::string ClockCallable::toString() const
{
    return "<native fn>";
}

// Native print
size_t PrintCallable::getArity() const
{
    return arity;
}

std::any PrintCallable::call(Interpreter& interpreter, const std::vector<std::any>& args) const
{
    std::stringstream stream;
    for (const auto& arg : args)
    {
        stream << stringify(arg, stream) << ' ';
    }
    std::cout << stream.str() << '\n';
    return {};
}

std::string PrintCallable::toString() const
{
    return "native print";
}

std::string stringify(const std::any& item, std::stringstream& stream)
{
    if (item.type() == typeid(shared_ptr_any)) {
        return stringify(*(std::any_cast<shared_ptr_any>(item)), stream);
    }

    if (item.type() == typeid(bool))
        return std::any_cast<bool>(item) ? "true" : "false";

    if (item.type() == typeid(char))
        return std::to_string(std::any_cast<char>(item));

    if (item.type() == typeid(FunctionType))
        return std::any_cast<FunctionType>(item).toString();

    if (item.type() == typeid(std::string))
    {
        auto str = std::any_cast<std::string>(item);
        if (str == "\\n")
            return "\n";
        else if (str == "\\t")
            return "\t";
        else
            return str;
    }

    if (item.type() == typeid(double))
    {
        auto num_as_string = std::to_string(std::any_cast<double>(item));
        num_as_string.erase(num_as_string.find_last_not_of('0') + 1, std::string::npos);
        num_as_string.erase(num_as_string.find_last_not_of('.') + 1, std::string::npos);
        return num_as_string;
    }

    if (item.type() == typeid(int))
    {
        return std::to_string(std::any_cast<int>(item));
    }

    if (item.type() == typeid(List))
    {
        auto items = std::any_cast<List>(item);
        stream << "[";
        auto len = items.length();
        for (size_t i = 0u; i < len; ++i)
        {
            stream << ' ';
            stream << stringify(items.at(static_cast<int>(i)), stream);
            stream << ",";
        }
        stream.seekp(-1, std::ios_base::end);
        stream << " ]";
        return {};
    }

    return "nil";
}
