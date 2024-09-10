#include "UninitObjCheckerUtil.h"
#include <clang/Basic/SourceManager.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/ProgramStateTrait.h>
#include <llvm/ADT/SmallSet.h>

using namespace clang;
using namespace clang::ento;

/// We'll mark fields (and pointee of fields) that are confirmed to be
/// uninitialized as already analyzed.

REGISTER_SET_WITH_PROGRAMSTATE(AnalyzedRegions, const MemRegion *)

//===----------------------------------------------------------
//				Methods for FieldListMgr
//===----------------------------------------------------------

bool FieldListMgr::contains(const FieldRegion *fr) const
{

    if (is_empty())
        return false;

    for (const FieldNode &node : _field_list)
    {
        if (node.is_same_region(fr))
        {
            return true;
        }
    }
    return false;
}
void FieldListMgr::print_note_message(llvm::raw_ostream &out) const
{
    if (is_empty())
    {
        return;
    }

    const FieldNode &last_field = get_head();

    last_field.print_note_message(out);
    out << '\'';

    for (const FieldNode &node : _field_list)
    {
        node.print_prefix(out);
    }

    out << "this->";
    print_fieldlist_tail(out, _field_list.getTail());
    last_field.print_node(out);
    out << '\'';
}

template <class FieldNodeT> inline FieldListMgr FieldListMgr::add(const FieldNodeT &field_node)
{
    assert(!contains(field_node.get_region()));

    FieldListMgr new_mgr = *this;
    new_mgr._field_list = _list_factory.add(field_node, _field_list);
    return new_mgr;
}

template <class FieldNodeT> inline FieldListMgr FieldListMgr::replace_head(const FieldNodeT &field_node)
{
    FieldListMgr new_mgr(_list_factory, _field_list.getTail());
    return new_mgr.add(field_node);
}

//===----------------------------------------------------------
//				Methods for FindUninitializedFields
//===----------------------------------------------------------

FindUninitializedFields::FindUninitializedFields(ProgramStateRef state, const TypedValueRegion *const typed_region)
    : _state(state), _obj_region(typed_region)
{

    is_non_union_obj_uninit(_obj_region, FieldListMgr(_list_factory));
}

bool FindUninitializedFields::is_union_obj_uninit(const TypedValueRegion *t_region)
{
    assert(t_region->getValueType()->isUnionType() && "This Method only checks union objects");
    // TODO: Implement support for union fields.
    return false;
}

bool FindUninitializedFields::is_non_union_obj_uninit(const TypedValueRegion *t_region, FieldListMgr local_list_mgr)
{

    assert(t_region->getValueType()->isRecordType() && !t_region->getValueType()->isUnionType() &&
           "This Method only checks non-union record object");

    const RecordDecl *rd = t_region->getValueType()->getAsRecordDecl()->getDefinition();

    if (!rd)
    {
        _is_any_field_init = true;
        return true;
    }

    // TODO: about CheckerOptions IgnoreRecordsWithFieldPattern

    bool contains_uninit_field = false;

    // Are all of this non-union's fields initialized?
    for (const FieldDecl *fd : rd->fields())
    {

        std::string field_name = std::string(fd->getName()); // for debugging efficiently...

        const auto field_val = _state->getLValue(fd, loc::MemRegionVal(t_region)).castAs<loc::MemRegionVal>();
        const auto *field_region = field_val.getRegionAs<FieldRegion>();

        QualType q_type = fd->getType();

        // If local_list_mgr already contains field_region, then we encountered a
        // cyclic reference. In this case, region field_region is already under
        // checking at an earlier node in the directed tree.
        if (local_list_mgr.contains(field_region))
        {
            return false;
        }

        if (q_type->isStructureOrClassType())
        {
            if (is_non_union_obj_uninit(field_region, local_list_mgr.add(RegularFieldNode(field_region))))
            {
                contains_uninit_field = true;
            }
            continue;
        }

        if (q_type->isUnionType())
        {
            if (is_union_obj_uninit(field_region))
            {
                // TODO: is_union_obj_uninit(TypedValueRegion*) always returns false,
                // for it is not implemented.

                /*if (add_fields_to_uninits(
                        local_list_mgr.add(RegularFieldNode(field_region)))) {
                  contains_uninit_field = true;
                }*/
                ;
            }
            else
            {
                _is_any_field_init = true;
            }
            continue;
        }

        if (q_type->isArrayType())
        {
            _is_any_field_init = true;
            continue;
        }

        SVal v = _state->getSVal(field_val);

        if (is_reference_type(q_type) || v.getAs<nonloc::LocAsInteger>())
        {
            if (is_reference_obj_uninit(field_region, local_list_mgr))
            {
                contains_uninit_field = true;
            }
            continue;
        }

        if (is_primitive_type(q_type))
        {
            if (is_primitive_obj_uninit(v))
            {
                if (add_fields_to_uninits(local_list_mgr.add(RegularFieldNode(field_region))))
                {
                    contains_uninit_field = true;
                }
            }
            continue;
        }

        llvm_unreachable("All cases are handled !");
    }

    // Checking bases. The checker will regard inherited data members as direct
    // fields.
    const auto *cxx_rd = dyn_cast<CXXRecordDecl>(rd);

    if (!cxx_rd)
    {
        return contains_uninit_field;
    }

    for (const CXXBaseSpecifier &base_spec : cxx_rd->bases())
    {
        const auto *base_region =
            _state->getLValue(base_spec, t_region).castAs<loc::MemRegionVal>().getRegionAs<TypedValueRegion>();
        // If the head of the list is also a BaseClass, we'll overwrite it to avoid
        // note messages like 'this->A::B::x'.

        if (!local_list_mgr.is_empty() && local_list_mgr.get_head().is_base())
        {
            if (is_non_union_obj_uninit(base_region, local_list_mgr.replace_head(BaseClassNode(base_spec.getType()))))
            {
                contains_uninit_field = true;
            }
        }
        else
        {
            if (is_non_union_obj_uninit(base_region, local_list_mgr.add(BaseClassNode(base_spec.getType()))))
            {
                contains_uninit_field = true;
            }
        }
    }
    return contains_uninit_field;
}

bool FindUninitializedFields::is_reference_obj_uninit(const FieldRegion *f_region, FieldListMgr local_list_mgr)
{

    SVal v = _state->getSVal(f_region);

    assert((is_reference_type(f_region->getDecl()->getType()) || v.getAs<nonloc::LocAsInteger>()) &&
           "this method only checkes reference objets");

    if (v.isUnknown() || v.getAs<loc::ConcreteInt>())
    {
        _is_any_field_init = true;
        return false;
    }

    if (v.isUndef())
    {
        return add_fields_to_uninits(local_list_mgr.add(LocFieldNode(f_region, /*_is_dereferencde*/ false)), f_region);
    }

    // TODO: checker option about CheckPointeeInitialization
    // if this option set to false, stop by this step and return false

    llvm::Optional<DereferenceInfo> deref_info = dereference(_state, f_region);

    if (!deref_info)
    {
        _is_any_field_init = true;
        return false;
    }

    if (deref_info->_is_cyclic)
    {
        return add_fields_to_uninits(local_list_mgr.add(CyclicLocFieldNode(f_region)), f_region);
    }

    const TypedValueRegion *region = deref_info->_region;
    const bool needs_cast_back = deref_info->_needs_cast_back;

    QualType dyn_type = region->getLocationType();
    QualType pointee_type = dyn_type->getPointeeType();

    if (pointee_type->isStructureOrClassType())
    {
        if (needs_cast_back)
        {
            return is_non_union_obj_uninit(region, local_list_mgr.add(NeedsCastLocFieldNode(f_region, dyn_type)));
        }
        return is_non_union_obj_uninit(region, local_list_mgr.add(LocFieldNode(f_region)));
    }

    if (pointee_type->isUnionType())
    {
        if (is_union_obj_uninit(region))
        {
            // TODO: is_union_obj_union(TypedValueRegion*) always returns false, for
            // it is not implemented.

            /*if (needs_cast_back) {
              return add_fields_to_uninits(
                  local_list_mgr.add(NeedsCastLocFieldNode(f_region, dyn_type)),
                  region);
            }
            return add_fields_to_uninits(local_list_mgr.add(LocFieldNode(f_region)),
                                         region);*/
            ;
        }
        else
        {
            _is_any_field_init = true;
            return false;
        }
    }

    // std::string pointee_name = std::string(pointee_type->getTypeClassName()); // for debugging efficiently...

    if (pointee_type->isArrayType())
    {
        _is_any_field_init = true;
        return false;
    }

    assert(is_primitive_type(pointee_type) || is_reference_type(pointee_type) &&
                                                  "At this point, f_region must have a primitive dynamic type, or "
                                                  "it must be a null, undefined, unknown or concrete pointer ");

    SVal pointee_v = _state->getSVal(region);

    if (is_primitive_obj_uninit(pointee_v))
    {
        if (needs_cast_back)
        {
            return add_fields_to_uninits(local_list_mgr.add(NeedsCastLocFieldNode(f_region, dyn_type)), region);
        }
        return add_fields_to_uninits(local_list_mgr.add(LocFieldNode(f_region)), region);
    }

    _is_any_field_init = true;
    return false;
}

bool FindUninitializedFields::is_primitive_obj_uninit(const SVal &value)
{
    if (value.isUndef())
    {
        return true;
    }

    _is_any_field_init = true;
    return false;
}

bool FindUninitializedFields::add_fields_to_uninits(FieldListMgr local_list_mgr,
                                                    const clang::ento::MemRegion *pointee_r)
{

    const FieldRegion *fr = local_list_mgr.get_uninit_region();

    // TODO: assert pointee region as a parameter for dereferenceable fields
    assert((pointee_r || !is_reference_type(fr->getDecl()->getType())) &&
           "One must also pass the pointee region as a parameter for "
           "dereferenceable fields!");

    if (_state->getStateManager().getContext().getSourceManager().isInSystemHeader(fr->getDecl()->getLocation()))
    {
        return false;
    }

    // TODO: about checker options about IgnoreGuardedFields

    if (_state->contains<AnalyzedRegions>(fr))
    {
        return false;
    }

    if (pointee_r)
    {
        if (_state->contains<AnalyzedRegions>(pointee_r))
        {
            return false;
        }
        _state = _state->add<AnalyzedRegions>(pointee_r);
    }

    _state = _state->add<AnalyzedRegions>(fr);

    UninitFieldMap::mapped_type note_message_buf;
    llvm::raw_svector_ostream os(note_message_buf);

    local_list_mgr.print_note_message(os);

    return _uninit_fieldmap.insert({fr, std::move(note_message_buf)}).second;
}

//===----------------------------------------------------------
//				Methods for General Utility functions
//===----------------------------------------------------------

bool is_primitive_type(const QualType &type)
{
    return type->isBuiltinType() || type->isEnumeralType() || type->isFunctionType() || type->isAtomicType() ||
           type->isVectorType() || type->isScalarType();
}

bool is_reference_type(const QualType &type)
{
    return type->isAnyPointerType() || type->isReferenceType();
}

void print_fieldlist_tail(llvm::raw_ostream &out, const FieldList &list)
{
    if (list.isEmpty())
    {
        return;
    }

    print_fieldlist_tail(out, list.getTail());

    // TODO: the format of printing node
    list.getHead().print_node(out);
    list.getHead().print_separator(out);
}

/// if Field is captured lambda variable, we aquire it's name from the
/// lambda's captures
std::string get_variable_name(const clang::FieldDecl *field)
{
    // TODO: figure out how to reach this branch that parent is lambda

    // CXXRecordDecl represents a C++ struct/union/class
    // const auto *cxx_parent = dyn_cast<CXXRecordDecl>(field->getParent());
    // std::string pname = std::string(field->getParent()->getName()); // supplying for debugging efficiently...

    // if (cxx_parent && cxx_parent->isLambda()) {
    //   assert(cxx_parent->captures_begin());

    //   // using clang::CXXRecordDecl::capture_const_iterator = const
    //   // LambdaCapture*
    //   auto iter = cxx_parent->captures_begin() + field->getFieldIndex();

    //   if (iter->capturesVariable()) {
    //     return llvm::Twine("/*captured varaiable*/" +
    //                        iter->getCapturedVar()->getName())
    //         .str();
    //   }

    //   if (iter->capturesThis()) {
    //     return "/*'this' capture*/";
    //   }
    // }

    std::string name = std::string(field->getName()); // supplying for debugging
    return std::string(field->getName());
}

//===----------------------------------------------------------
//		Methods for Reference(Pointee) Utility functions
//===----------------------------------------------------------
static llvm::Optional<DereferenceInfo> dereference(clang::ento::ProgramStateRef state,
                                                   const clang::ento::FieldRegion *fr)
{

    llvm::SmallSet<const TypedValueRegion *, 5> visited_regions;

    SVal v = state->getSVal(fr);
    assert(v.getAsRegion() && "v must have an underlying region!");

    // If the static type of the field is a void pointer, or it is a
    // nonloc::LocAsInteger, we need to cast it back to the dynamic type before
    // dereferencing.

    bool needs_cast_back = is_void_pointer(fr->getDecl()->getType()) || v.getAs<nonloc::LocAsInteger>();

    const auto *region = v.getAsRegion()->getAs<TypedValueRegion>();

    if (!region)
    {
        return None;
    }

    visited_regions.insert(region);
    // acquire the dynamic type of R,
    QualType dynamic_type = region->getLocationType();

    while (const MemRegion *tmp = state->getSVal(region, dynamic_type).getAsRegion())
    {

        region = tmp->getAs<TypedValueRegion>();

        if (!region)
        {
            return None;
        }
        // Found a cyclic pointer, like int *ptr = (int *)&ptr.
        if (!visited_regions.insert(region).second)
        {
            DereferenceInfo info(region, needs_cast_back, true);
            return info;
        }

        dynamic_type = region->getLocationType();

        if (is_reference_type(dynamic_type->getPointeeType()))
        {
            break;
        }
    }

    // std::string region_name =
    //    region->getString(); // for debuggingefficiently...
    // std::string super_region_name =
    //    region->getSuperRegion()->getString(); // for debugging efficiently...

    while (isa<CXXBaseObjectRegion>(region))
    {

        needs_cast_back = true;

        const auto *super_region = dyn_cast<TypedValueRegion>(region->getSuperRegion());

        if (!super_region)
        {
            break;
        }
        region = super_region;

        // std::string region_name_in =
        //    region->getString(); /* for debugging efficiently...*/
        // std::string super_region_name_in =
        //    region->getSuperRegion()->getString(); /* for debugging
        //    efficiently...*/
    }

    DereferenceInfo info(region, needs_cast_back, false);
    return info;
}

static bool is_void_pointer(clang::QualType type)
{
    while (!type.isNull())
    {
        if (type->isVoidPointerType())
        {
            return true;
        }
        type = type->getPointeeType();
    }

    return false;
}