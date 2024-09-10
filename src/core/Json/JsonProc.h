#pragma once
#pragma once
#include <cassert>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
// used for json format transformation
namespace Json
{
/*
The type of each Value
*/
enum class ValueType
{
    NONE,
    OBJECT,
    INTEGER,
    DOUBLE,
    BOOLEAN,
    STRING,
    ARRAY
};
class Value;
class ObjectValue;
class IntValue;
class DoubleValue;
class BoolValue;
class StringValue;
class ArrayValue;
using ValuePtr = std::shared_ptr<Value>;

using Map = std::map<std::string, Value *>;
/*! \brief Generate spaces to formatize json output
    \param[in] indent the number of spaces to append
    \return generated spaces
*/
static std::string make_indent(size_t indent)
{
    static bool is_init = false;
    const int bufsize = 10;
    static std::string buf[bufsize];

    if (!is_init)
    {
        for (int i = 0; i < bufsize; i++)
        {
            for (int j = 0; j < i; j++)
            {
                buf[i] += '\t';
            }
        }
        is_init = true;
    }
    if (indent < bufsize)
        return buf[indent];
    else
    {
        std::string str;
        while (indent-- > 0)
        {
            str += '\t';
        }
        return str;
    }
}

/*! \brief Value

The abstract base class for Json unit, which contains the unit value, including basic value, such as integer,float, and
collection value, such as array, object.

The value and its child values will collectively form a Json tree, which can be translated into string format.

All constructors of the values are made private, and they can only be created through make_value or new_value functions.
*/
class Value
{
  public:
    /*! \brief get the value type of the value node
    \return the type of this value
    */
    virtual ValueType get_type() const
    {
        return ValueType::NONE;
    }

    /*! \brief Append child attribute into an object value node
    \param[in] key the name of the attribute
    \param[in] v the child value
    */
    virtual void append(const std::string &key, Value &v)
    {
        assert(false && "not supported");
    }

    /*! \brief Append list element into an array value node
    \param[in] v the list element to be appended
    */
    virtual void append(Value &v)
    {
        assert(false && "not supported");
    }

  public:
    /*! \brief translate the json value tree into string format
    \param[in] indent the number of spaces to be appended at the beginning
    \param[in] cont_line true if the indent should be appended
    */
    virtual std::string to_string(size_t indent = 0, bool cont_line = true) const = 0;

    virtual ~Value()
    {
    }

    /*! \brief release memory on heap occupied by this json tree.*/
    virtual void release()
    {
    }
};

/*! \brief ObjectValue

The Object in Json, with child attributes.
*/
class ObjectValue : public Value
{
  private:
    Map attributes;
    std::vector<std::pair<std::string, Value *>> attributes_vec;

    ObjectValue() = default;

  public:
    friend ObjectValue make_object();
    friend ObjectValue &new_object();
    ObjectValue(ObjectValue &v) = default;
    ObjectValue(ObjectValue &&v) = default;

    /*!
    \sa Value::get_type
    */
    ValueType get_type() const override
    {
        return ValueType::OBJECT;
    }

    /*! brief get the attribute value coresponding to the attribute name
    \param[in] key the attribute name
    \return the corresponding attribute value
    */
    Value &get_value(const std::string &key) const
    {
        auto it = attributes.find(key);
        if (it == attributes.end())
            assert(false && "key not found!");
        return *it->second;
    }

    /*! \breif check whether the attribute exists
    \param[in] key the attribute name
    \return true of the attribute exists in this object
    */
    bool contains_key(const std::string &key) const
    {
        return (attributes.find(key) != attributes.end());
    }

    /*
    \sa Value::append
    */
    void append(const std::string &key, Value &v) override
    {
        attributes.insert_or_assign(key, &v);
        attributes_vec.push_back(std::make_pair(key, &v));
    }

    /*! \sa Value::to_string
     */
    std::string to_string(size_t indent = 0, bool cont_line = true) const override
    {
        std::string str;
        if (cont_line)
            str += "\n";
        str += make_indent(indent) + "{\n";
        for (int i = 0; i < attributes_vec.size(); i++)
        {
            const std::pair<std::string, Value *> &p = attributes_vec[i];
            const std::string name = p.first;
            const Value *v = p.second;

            str += make_indent(indent + 1) + "\"" + name + "\"" + " : " + v->to_string(indent + 2, true);

            if (i != attributes_vec.size() - 1)
                str += ",";
            str += "\n";
        }
        str += make_indent(indent) + "}";
        return str;
    }

    /*! \sa Value::release
     */
    void release() override
    {
        for (std::pair<std::string, Value *> p : attributes_vec)
        {
            p.second->release();
            delete p.second;
        }
    }
};

/* \brief the basic value containing an integer
 */
class IntValue : public Value
{
  private:
    /*! \brief the actual value
     */
    const int value;

    using Value::append;

    IntValue() = default;

  public:
    friend IntValue make_value(int);
    friend IntValue &new_value(int);
    IntValue(int v) : value(v)
    {
    }
    IntValue(IntValue &v) = default;
    IntValue(IntValue &&v) = default;
    /*! \sa Value::get_type
     */
    ValueType get_type() const override
    {
        return ValueType::INTEGER;
    }
    /*! \brief get the integer value
    \return the integer value
    */
    int get_value()
    {
        return value;
    }

    /*! \sa Value::to_string
     */
    std::string to_string(size_t indent = 0, bool cont_line = true) const override
    {
        return (cont_line ? "" : make_indent(indent)) + std::to_string(value);
    }
};
/*! \brief the basic value containing a double
 */
class DoubleValue : public Value
{
  private:
    const double value;

    using Value::append;

    DoubleValue() = default;

  public:
    friend DoubleValue make_value(double);
    friend DoubleValue &new_value(double);

    DoubleValue(double v) : value(v)
    {
    }

    DoubleValue(DoubleValue &v) = default;

    DoubleValue(DoubleValue &&v) = default;

    /*! \sa Value::get_type
     */
    ValueType get_type() const override
    {
        return ValueType::DOUBLE;
    }
    /*! \brief get the double value
    \return the double value
    */
    double get_value()
    {
        return value;
    }
    /*! \sa Value::to_string*/
    std::string to_string(size_t indent = 0, bool cont_line = true) const override
    {
        return (cont_line ? "" : make_indent(indent)) + std::to_string(value);
    }
};
/*! \brief the basic value containing a boolean
 */
class BoolValue : public Value
{
  private:
    const bool value;

    using Value::append;

    BoolValue() = default;

  public:
    friend BoolValue make_value(bool);
    friend BoolValue &new_value(bool);

    BoolValue(bool v) : value(v)
    {
    }

    BoolValue(BoolValue &v) = default;

    BoolValue(BoolValue &&v) = default;
    /*! \sa Value::get_type
     */
    ValueType get_type() const override
    {
        return ValueType::BOOLEAN;
    }
    /*! \brief get the boolean value
    \return the contained boolean value
    */
    bool get_value()
    {
        return value;
    }
    /*! \sa Value::to_string
     */
    std::string to_string(size_t indent = 0, bool cont_line = true) const override
    {
        std::string s = value ? "true" : "false";
        return (cont_line ? "" : make_indent(indent)) + s;
    }
};
/*! \brief the basic value for a string
 */
class StringValue : public Value
{
  private:
    const std::string value;

    using Value::append;

    StringValue() = default;

  public:
    friend StringValue make_value(const char *);
    friend StringValue make_value(const std::string &);
    friend StringValue make_value(std::string &&);
    friend StringValue &new_value(const char *);
    friend StringValue &new_value(const std::string &);
    friend StringValue &new_value(std::string &&);

    StringValue(const std::string &v) : value(v)
    {
    }

    StringValue(std::string &&v) : value(std::move(v))
    {
    }
    /*! \sa Value::get_type
     */
    ValueType get_type() const override
    {
        return ValueType::STRING;
    }
    /*! \brief get the contained string value
    \return the contained string value
    */
    const std::string &get_value()
    {
        return value;
    }
    /*! \sa Value::to_string
     */
    std::string to_string(size_t indent = 0, bool cont_line = true) const override
    {
        return (cont_line ? "" : make_indent(indent)) + "\"" + value + "\"";
    }
};

/*! \brief the value containing a lits of child values

    used to store '[]' values in Json
*/
class ArrayValue : public Value
{
  private:
    std::vector<Value *> arr;

    ArrayValue() = default;

  public:
    friend ArrayValue make_array();
    friend ArrayValue &new_array();

    ArrayValue(ArrayValue &v) = default;

    ArrayValue(ArrayValue &&v) = default;
    /*! \sa Value::get_type
     */
    ValueType get_type() const override
    {
        return ValueType::ARRAY;
    }
    /*! \brief get the child child at the 'index' position
    \param[in] index the index number for the childlist
    \return the value in the list at the index
    */
    Value &get_value(int index) const
    {
        return *arr[index];
    }
    /*! sa Value::append
     */
    void append(Value &v) override
    {
        arr.push_back(&v);
    }
    /*! \brief the number of child values
    \return the number of child values
    */
    size_t size() const
    {
        return arr.size();
    }
    /*! \sa Value::to_string
     */
    std::string to_string(size_t indent = 0, bool cont_line = true) const override
    {
        std::string str;
        if (cont_line)
            str += "\n";
        str += make_indent(indent);
        str += "[\n";
        for (int i = 0; i < arr.size(); i++)
        {
            Value *v = arr[i];
            str += v->to_string(indent + 1, false);
            if (i != arr.size() - 1)
                str += ",";
            str += "\n";
        }
        str += make_indent(indent) + "]";
        return str;
    }
    /*! \sa Value::release
     */
    void release() override
    {
        for (Value *v : arr)
        {
            v->release();
            delete v;
        }
    }
};
/*! \brief create an IntValue on stack
\param[in] v the contained value
\return the created Value
*/
IntValue make_value(int v);
/*! \brief create a DoubleValue on stack
\param[in] v the contained value
\return the created Value
*/
DoubleValue make_value(double v);
/*! \brief create a BoolValue on stack
\param[in] v the contained value
\return the created Value
 */
BoolValue make_value(bool v);
/*! \brief create a StringValue on stack
\param[in] v the contained value
\return the created Value
 */
StringValue make_value(const char *v);
/*! \brief create a StringValue on stack
\param[in] v the contained value
\return the created Value
 */
StringValue make_value(const std::string &v);
/*! \brief create a StringValue on stack
\param[in] v the contained value
\return the created Value
 */
StringValue make_value(std::string &&v);
/*! \brief create an ArrayValue on stack
\return the created Value
 */
ArrayValue make_array();
/*! \brief create an ObjectValue on stack
\return the created Value
 */
ObjectValue make_object();
/*! \brief create an IntValue on heap
\param[in] v the contained value
\return the created Value
 */
IntValue &new_value(int v);
/*! \brief create a DoublegValue on heap
\param[in] v the contained value
\return the created Value
 */
DoubleValue &new_value(double v);
/*! \brief create a BoolValue on heap
\param[in] v the contained value
\return the created Value
 */
BoolValue &new_value(bool v);
/*! \brief create a StringValue on heap
\param[in] v the contained value
\return the created Value
 */
StringValue &new_value(const char *v);
/*! \brief create a StringValue on heap
\param[in] v the contained value
\return the created Value
 */
StringValue &new_value(const std::string &v);
/*! \brief create a StringValue on heap
\param[in] v the contained value
\return the created Value
 */
StringValue &new_value(std::string &&v);
/*! \brief create an ObjectValue on heap
\return the created Value
 */
ObjectValue &new_object();
/*! \brief create an ArrayValue on heap
\return the created Value
 */
ArrayValue &new_array();

/*! \brief translate Json value into string format
\param[in] root the root value node of the json tree
\return the string format for the json tree
*/
std::string stringify(Value &root);

/*
class DestroyableValue : public Value {
private:
  Value *value;

public:
  DestroyableValue(Value *v) : value(v) {}

  ValueType get_type() const override { return value->get_type(); }

  void append(const std::string &key, Value &v) override {
    value->append(key, v);
  }

  void append(Value &v) override { value->append(v); }

  std::string to_string() const override { return value->to_string(); }

  ~DestroyableValue() override { value->release(); }
};*/

/*! \brief parse the string into json tree
\param[in] str the source string
\return root value of the parsed json tree
*/
Value &parse(std::string &str);

}; // namespace Json

#include <fstream>
#include <iostream>
#include <sstream>
/*! \brief usecase

display the way of using Json functions, including generating, parsing and stringify json data.
*/
void JsonUseCase();