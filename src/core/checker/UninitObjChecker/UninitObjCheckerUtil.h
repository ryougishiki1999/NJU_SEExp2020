#ifndef CHECKER_UNINITOBJCHECKER_UTIL_H
#define CHECKER_UNINITOBJCHECKER_UTIL_H
#include <clang/StaticAnalyzer/Core/PathSensitive/MemRegion.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/ProgramState.h>
#include <map>

class FieldNode;
class RegularFieldNode;
class BaseClassNode;
class FieldListMgr;
class FindUninitializedFields;

/// Wrapped FieldNode for reference and pointee, and struct for dereference....
class LocFieldNode;
class NeedsCastLocFieldNode;
class CyclicLocFieldNode;

struct DereferenceInfo
{
    const clang::ento::TypedValueRegion *_region;
    const bool _needs_cast_back;
    const bool _is_cyclic;

    DereferenceInfo(const clang::ento::TypedValueRegion *r, bool ncb, bool ic)
        : _region(r), _needs_cast_back(ncb), _is_cyclic(ic)
    {
    }
};

/// FieldList should essentially be wrapped around an ImmutableList
/// which should be modified only by its Factory. e.g. add() and replaceHead()
using FieldList = llvm::ImmutableList<const FieldNode &>;

using UninitFieldMap = std::map<const clang::ento::FieldRegion *, llvm::SmallString<50>>;

//===----------------------------------------------------------------------------
//                    General Uitility Methods Decl
//===----------------------------------------------------------------------------

/// Primitive Objects only need to be ananlyed whether their value is undefined.
bool is_primitive_type(const clang::QualType &type);

bool is_reference_type(const clang::QualType &type);

void print_fieldlist_tail(llvm::raw_ostream &out, const FieldList &list);

std::string get_variable_name(const clang::FieldDecl *field);

//===----------------------------------------------------------------------------
//                      Reference Uitility Methods Decl
//===----------------------------------------------------------------------------
/// Dereferences \p FR and returns with the pointee's region, and whether it
/// needs to be casted back to it's location type.
static llvm::Optional<DereferenceInfo> dereference(clang::ento::ProgramStateRef state,
                                                   const clang::ento::FieldRegion *fr);
/// Returns whether \p T can be (transitively) dereferenced to a void pointer
/// type (void*, void**, ...).
static bool is_void_pointer(clang::QualType type);

//===----------------------------------------------------------------------------
//                      Class FieldNode Decl
//===----------------------------------------------------------------------------

// We will use this interface to wrap FieldRegion* and store addinitional
// information about fields. FieldNodes can print note messages about
// themselves.
class FieldNode
{
  public:
    // We only allow the specific construct method to construct FieldNode
    FieldNode(const clang::ento::FieldRegion *field_region) : _fr(field_region)
    {
    }
    FieldNode() = delete;
    FieldNode(const FieldNode &) = delete;
    FieldNode(FieldNode &&) = delete;

    FieldNode &operator=(const FieldNode &) = delete;
    FieldNode &operator=(FieldNode &&) = delete;

    void Profile(llvm::FoldingSetNodeID &ID) const
    {
        ID.AddPointer(this);
    }

    bool is_same_region(const clang::ento::FieldRegion *another) const
    {
        // According to document in CSA, special FieldNode may wrap nullpointers.
        // We would not agree to unique and equal these objects.
        if (_fr == nullptr)
        {
            return false;
        }

        return _fr == another;
    }

    const clang::ento::FieldRegion *get_region() const
    {
        return _fr;
    }
    const clang::FieldDecl *get_decl() const
    {
        assert(_fr);
        return _fr->getDecl();
    }

    /// The note message should state Message Like "uninitialized field....".
    virtual void print_note_message(llvm::raw_ostream &out) const = 0;
    /// Print any prefixes before the fieldchain. Could contain casts, etc.
    virtual void print_prefix(llvm::raw_ostream &out) const = 0;
    /// Print the node. Should contain the name of the field stored in FR.
    virtual void print_node(llvm::raw_ostream &out) const = 0;
    /// Print the separator. For example, fields may be separated with '.' or
    /// "->".
    virtual void print_separator(llvm::raw_ostream &out) const = 0;

    virtual bool is_base() const
    {
        return false;
    }

  protected:
    const clang::ento::FieldRegion *_fr;
};

//===----------------------------------------------------------------------------
//            Class RegularFieldNode(public FieldNode) Decl
//===----------------------------------------------------------------------------

/// A basic field type rather than a pointer or reference.
class RegularFieldNode final : public FieldNode
{
  public:
    RegularFieldNode(const clang::ento::FieldRegion *fr) : FieldNode(fr)
    {
    }

    virtual void print_note_message(llvm::raw_ostream &out) const override
    {
        out << "uninitialized Field : RegularFieldNode";
    }

    virtual void print_prefix(llvm::raw_ostream &out) const override
    {
    }

    virtual void print_node(llvm::raw_ostream &out) const override
    {
        out << get_variable_name(get_decl());
    }

    virtual void print_separator(llvm::raw_ostream &out) const override
    {
        out << ".";
    }
};

//===----------------------------------------------------------------------------
//              Class BaseClassNode(public FieldNode) Decl
//===----------------------------------------------------------------------------

/// Represents that the FieldNode that comes after this is declared in a base
/// of the previous FieldNode. As such, this descendant doesn't wrap a
/// FieldRegion, and is purely a tool to describe a relation between two other
/// FieldRegion wrapping descendants.
class BaseClassNode final : public FieldNode
{
  public:
    BaseClassNode(const clang::QualType &type) : FieldNode(nullptr), _base_class_type(type)
    {
        assert(!type.isNull());
        assert(type->getAsCXXRecordDecl());
    }

    virtual void print_note_message(llvm::raw_ostream &out) const override
    {
        // TODO: This function can never be reached.
        /*llvm_unreachable("This node can never be the final node in the fieldlist :
           " "BaseClassNode");*/
    }

    virtual void print_prefix(llvm::raw_ostream &out) const override
    {
    }

    virtual void print_node(llvm::raw_ostream &out) const override
    {
        out << _base_class_type->getAsCXXRecordDecl()->getName() << "::";
    }

    virtual void print_separator(llvm::raw_ostream &out) const override
    {
    }

    virtual bool is_base() const override
    {
        return true;
    }

  private:
    const clang::QualType _base_class_type;
};

//===----------------------------------------------------------------------------
//              Class LocFieldNode(public FieldNode) Decl for Reference
//===----------------------------------------------------------------------------
/// Represents a pointer or a reference field.
class LocFieldNode final : public FieldNode
{
  public:
    LocFieldNode(const clang::ento::FieldRegion *fr, const bool is_dereferenced = true)
        : FieldNode(fr), _is_dereferenced(is_dereferenced)
    {
    }

    virtual void print_note_message(llvm::raw_ostream &out) const override
    {
        if (_is_dereferenced)
        {
            out << "uninitialized pointee: ";
        }
        else
        {
            out << "uninitialized pointer: ";
        }
    }

    virtual void print_prefix(llvm::raw_ostream &out) const override
    {
    }

    virtual void print_node(llvm::raw_ostream &out) const override
    {
        out << get_variable_name(get_decl());
    }

    virtual void print_separator(llvm::raw_ostream &out) const override
    {
        if (get_decl()->getType()->isPointerType())
        {
            out << "->";
        }
        else
        {
            out << ".";
        }
    }

  private:
    const bool _is_dereferenced;
};

//===----------------------------------------------------------------------------
//        Class NeedsCastLocFieldNode(public FieldNode) Decl for Reference
//===----------------------------------------------------------------------------
/// Represents a nonloc::LocAsInteger or void* field, that point to objects, but
/// needs to be casted back to its dynamic type for a correct note message.
class NeedsCastLocFieldNode final : public FieldNode
{
  public:
    NeedsCastLocFieldNode(const clang::ento::FieldRegion *fr, const clang::QualType &type)
        : FieldNode(fr), _cast_back_type(type)
    {
    }

    virtual void print_note_message(llvm::raw_ostream &out) const override
    {
        out << "uninitialized pointee: ";
    }

    virtual void print_prefix(llvm::raw_ostream &out) const override
    {
        // if this object is a nonloc::LocAsInteger.
        if (get_decl()->getType()->isIntegerType())
        {
            out << "reinterpret_cast";
        }
        // if this pointer's dynamic type is different then it's static type
        else
        {
            out << "static_cast";
        }
        out << "<" << _cast_back_type.getAsString() << ">(";
    }

    virtual void print_node(llvm::raw_ostream &out) const override
    {
        out << get_variable_name(get_decl()) << ")";
    }

    virtual void print_separator(llvm::raw_ostream &out) const override
    {
        out << "->";
    }

  private:
    clang::QualType _cast_back_type;
};

//===----------------------------------------------------------------------------
//         Class CyclicLocFieldNode(public FieldNode) Decl for Reference
//===----------------------------------------------------------------------------
/// Represents a Loc field that points to itself.
class CyclicLocFieldNode final : public FieldNode
{
  public:
    CyclicLocFieldNode(const clang::ento::FieldRegion *fr) : FieldNode(fr)
    {
    }

    virtual void print_note_message(llvm::raw_ostream &out) const override
    {
        out << "object references itself: ";
    }

    virtual void print_prefix(llvm::raw_ostream &out) const override
    {
    }

    virtual void print_node(llvm::raw_ostream &out) const override
    {
        out << get_variable_name(get_decl());
    }

    virtual void print_separator(llvm::raw_ostream &out) const override
    {
        // TODO: this function can never be reached.
        /*llvm_unreachable(
            "CyclicLocField objects must be the last node of FieldList");*/
    }
};

//===----------------------------------------------------------------------------
//                      Class FieldListMgr Decl
//===----------------------------------------------------------------------------

/// Lists of FieldNode will be constructed where the head element of List is the
/// object under checking, and every other element is a field, and the element
/// that precedes it is the object that contains it.
class FieldListMgr
{
  public:
    FieldListMgr() = delete;
    FieldListMgr(const FieldListMgr &) = default;
    FieldListMgr(FieldList::Factory &list_factory) : _list_factory(list_factory)
    {
    }

    template <class FieldNodeT> FieldListMgr add(const FieldNodeT &field_node);

    template <class FieldNodeT> FieldListMgr replace_head(const FieldNodeT &field_node);

    bool contains(const clang::ento::FieldRegion *fr) const;

    bool is_empty() const
    {
        return _field_list.isEmpty();
    }

    const FieldNode &get_head() const
    {
        return _field_list.getHead();
    }

    const clang::ento::FieldRegion *get_uninit_region() const
    {
        return get_head().get_region();
    }
    // const FieldList &get_field_list() const { return _field_list; }

    void print_note_message(llvm::raw_ostream &out) const;

  private:
    FieldList::Factory &_list_factory;
    FieldList _field_list;

    FieldListMgr(FieldList::Factory &list_factory, FieldList new_field_list) : _list_factory(list_factory)
    {
        _field_list = new_field_list;
    }
};

//===----------------------------------------------------------------------------
//                      Class FindUninitializedFiedls Decl
//===----------------------------------------------------------------------------

/// Searches for and stores uninitialized fields in a non-union object:
/// FindUninitializedFields
class FindUninitializedFields
{
  public:
    FindUninitializedFields(clang::ento::ProgramStateRef state, const clang::ento::TypedValueRegion *typed_region);
    FindUninitializedFields() = delete;
    FindUninitializedFields(const FindUninitializedFields &) = delete;
    FindUninitializedFields(FindUninitializedFields &&) = delete;

    FindUninitializedFields &operator=(const FindUninitializedFields &) = delete;
    FindUninitializedFields &operator=(FindUninitializedFields &&) = delete;

    std::pair<clang::ento::ProgramStateRef, const UninitFieldMap &> get_results() const
    {
        return {_state, _uninit_fieldmap};
    }

    bool is_any_field_initialized()
    {
        return _is_any_field_init;
    }

  private:
    clang::ento::ProgramStateRef _state;
    const clang::ento::TypedValueRegion *const _obj_region;
    bool _is_any_field_init = false;

    /// A factory for creating or modifying FieldList
    FieldList::Factory _list_factory;
    /// A map for assigning uninitialized regions to note messages.
    UninitFieldMap _uninit_fieldmap;

    // We'll traverse each node of the above graph with the appropriate one of
    // these methods:
    bool is_union_obj_uninit(const clang::ento::TypedValueRegion *t_region);
    /// Checks non-union object. Return True if the region is found with
    /// uninitializaiton.
    bool is_non_union_obj_uninit(const clang::ento::TypedValueRegion *t_region, FieldListMgr local_list_mgr);
    /// Checkes a pointer or reference object. Return True if object itself is
    /// uninitialized.
    bool is_reference_obj_uninit(const clang::ento::FieldRegion *f_region, FieldListMgr local_list_mgr);
    /// Checkes the value of a primitive object.
    bool is_primitive_obj_uninit(const clang::ento::SVal &value);

    // Add qualified Fields to _uninit_fieldmap
    bool add_fields_to_uninits(FieldListMgr local_list, const clang::ento::MemRegion *pointee_r = nullptr);
};

#endif
