#include "JsonProc.h"
#include <stack>
void JsonUseCase()
{

    // generate json and transform to string
    {
        auto object_value = Json::make_object();
        auto int_value = Json::make_value(1);
        auto double_value = Json::make_value(1.1);
        auto bool_value = Json::make_value(true);
        auto bool2_value = Json::new_value(false);
        auto string_value = Json::make_value("str");
        auto array_value = Json::make_array();
        std::string s2 = "str2";
        auto string2_value = Json::make_value(s2);
        std::string s3 = "str3";
        auto string3_value = Json::make_value(std::move(s3));

        array_value.append(int_value);
        array_value.append(double_value);
        array_value.append(bool_value);
        array_value.append(bool2_value);
        array_value.append(string_value);
        array_value.append(string2_value);
        array_value.append(string3_value);

        object_value.append("array", array_value);
        // expected json text: {"array":[1,1.100000,true,"str"]}

        // you can stringify any type of value
        std::string json_txt = Json::stringify(object_value);
        //std::cout << json_txt << std::endl;

        auto &root = Json::parse(json_txt);
        std::string txt = Json::stringify(root);
        //std::cout << txt << std::endl;
        root.release();
    }

    // generate json tree on heap
    {
        auto &object_value = Json::new_object();
        auto &array_value = Json::new_array();
        auto &int_value = Json::new_value(1);
        auto &double_value = Json::new_value(1.1);
        auto &bool_value = Json::new_value(true);
        auto &bool2_value = Json::new_value(false);
        auto &string_value = Json::new_value("str");
        std::string str2 = "str2";
        auto &string2_value = Json::new_value(str2);
        std::string str3 = "str3";
        auto &string3_value = Json::new_value(std::move(str3));

        object_value.append("array", array_value);
        array_value.append(int_value);
        array_value.append(double_value);
        array_value.append(bool_value);
        array_value.append(bool2_value);
        array_value.append(string_value);
        array_value.append(string2_value);
        array_value.append(string3_value);

        std::string txt = Json::stringify(object_value);
        auto &parsed_result = Json::parse(txt);
        //std::cout << txt << "\n";

        // use release to free memory on json tree
        object_value.release();
    }
}
namespace Json
{
IntValue make_value(int v)
{
    return IntValue(v);
}

DoubleValue make_value(double v)
{
    return DoubleValue(v);
}

BoolValue make_value(bool v)
{
    return BoolValue(v);
}

StringValue make_value(const char *v)
{
    return StringValue(std::string(v));
}

StringValue make_value(const std::string &v)
{
    return StringValue(v);
}

StringValue make_value(std::string &&v)
{
    return StringValue(std::move(v));
}

ArrayValue make_array()
{
    return ArrayValue();
}

ObjectValue make_object()
{
    return ObjectValue();
}

IntValue &new_value(int v)
{
    return *new IntValue(v);
}

DoubleValue &new_value(double v)
{
    return *new DoubleValue(v);
}

BoolValue &new_value(bool v)
{
    return *new BoolValue(v);
}

StringValue &new_value(const char *v)
{
    return *new StringValue(std::string(v));
}

StringValue &new_value(const std::string &v)
{
    return *new StringValue(v);
}

StringValue &new_value(std::string &&v)
{
    return *new StringValue(std::move(v));
}

ObjectValue &new_object()
{
    return *new ObjectValue();
}

ArrayValue &new_array()
{
    return *new ArrayValue();
}

// translate json into string
std::string stringify(Value &root)
{
    return root.to_string(0, true);
}

static bool is_space(char t)
{
    return t == ' ' || t == '\t' || t == '\b' || t == '\r' || t == '\n';
}

class Stream
{
private:
    const char*str;

    std::stack<char> put_back_chars;

    int index = 0;

    int length;

public:
    Stream(std::string &_str) : str(_str.c_str()), length(_str.length())
    {
    }

    char fetch_char()
    {
        if (!put_back_chars.empty())
        {
            char c = put_back_chars.top();
            put_back_chars.pop();
            return c;
        }
        else
        {
            if (index == length)
                return 0;
            return str[index++];
        }
    }

    void put_back(char c)
    {
        put_back_chars.push(c);
    }
};
Stream &operator>>(Stream &in, char& t)
{
    t = in.fetch_char();
    return in;
}

static char get_next_char(Stream &stream)
{
    char t = 0;
    while (true)
    {
        stream >> t;
        if (t == 0 || !is_space(t))
            break;
    }
    return t;
}

static Value *__parse2(Stream &stream)
{
    char temp;
    temp = get_next_char(stream);

    Value *value = nullptr;
    if (temp == '{')
    {
        value = &new_object();
        while (true)
        {
            Value *name = __parse2(stream);
            if (name == nullptr)
                break;
            assert(name->get_type() == ValueType::STRING);

            Value *v = __parse2(stream);
            value->append(static_cast<StringValue *>(name)->get_value(), *v);
        }
    }
    else if (temp == '[')
    {
        value = &new_array();
        while (true)
        {
            Value *v = __parse2(stream);
            if (v == nullptr)
                break;
            value->append(*v);
        }
    }
    else if (temp == ']' || temp == '}')
    {
    }
    else if (temp == ',' || temp == ':')
    {
        value = __parse2(stream);
    }
    else if (temp == '\"')
    {
        bool flag = false;
        std::string tmpstr;
        while (true)
        {
            stream >> temp;
            if (temp == 0)
                break;
            if (temp == '\"' && !flag)
                break;

            if (temp == '\\')
                flag = true;
            else
                flag = false;

            tmpstr += temp;
        }
        assert(temp == '\"');

        value = &new_value(tmpstr);
        assert(value->get_type() == ValueType::STRING);
    }
    else if (temp == 't')
    {
        std::string tmpstr;
        tmpstr += temp;
        while (true)
        {
            temp = get_next_char(stream);
            if (temp == 0)
                break;
            if (!isalpha(temp))
            {
                stream.put_back(temp);
                break;
            }
            tmpstr += temp;
        }
        assert(tmpstr == "true");
        value = &new_value(true);
        assert(value->get_type() == ValueType::BOOLEAN);
    }
    else if (temp == 'f')
    {
        std::string tmpstr;
        tmpstr += temp;
        while (true)
        {
            temp = get_next_char(stream);
            if (temp == 0)
                break;
            if (!isalpha(temp))
            {
                stream.put_back(temp);
                break;
            }
            tmpstr += temp;
        }
        assert(tmpstr == "false");
        value = &new_value(false);
        assert(value->get_type() == ValueType::BOOLEAN);
    }
    else if (isdigit(temp))
    {
        std::string tmpstr;
        tmpstr += temp;

        bool is_float = false;
        while ((temp = get_next_char(stream)) != 0)
        {
            if (!(isdigit(temp) || temp == '.'))
            {
                stream.put_back(temp);
                break;
            }
            if (temp == '.')
            {
                is_float = true;
            }
            tmpstr += temp;
        }

        if (is_float)
        {
            double d;
            d = atof(tmpstr.c_str());
            value = &new_value(d);
            assert(value->get_type() == ValueType::DOUBLE);
        }
        else
        {
            int i;
            i = atoi(tmpstr.c_str());
            value = &new_value(i);
            assert(value->get_type() == ValueType::INTEGER);
        }
    }
    return value;
}


// parse str into json
// todo: text error detection
Value &parse(std::string &str)
{
    std::string source = str;
    // append space around each unit

    Stream stream(source);
    return *__parse2(stream);
}
} // namespace Json