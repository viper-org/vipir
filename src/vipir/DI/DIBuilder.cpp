// Copyright 2025 solar-mist


#include "vipir/DI/DIBuilder.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/Debug/QueryAddress.h"
#include "vipir/Module.h"

#include <vasm/codegen/builder/OpcodeBuilder.h>

#include <cstring>

enum DwarfLanguage {
    DW_LANG_C11 = 0x001d,
};

enum DwarfChildren {
    DW_CHILDREN_no = 0,
    DW_CHILDREN_yes = 1
};

enum DwarfForm {
    DW_FORM_addr = 0x01,
    DW_FORM_block2 = 0x03,
    DW_FORM_block4 = 0x04,
    DW_FORM_data2 = 0x05,
    DW_FORM_data4 = 0x06,
    DW_FORM_data8 = 0x07,
    DW_FORM_string = 0x08,
    DW_FORM_block = 0x09,
    DW_FORM_block1 = 0x0a,
    DW_FORM_data1 = 0x0b,
    DW_FORM_flag = 0x0c,
    DW_FORM_sdata = 0x0d,
    DW_FORM_strp = 0x0e,
    DW_FORM_udata = 0x0f,
    DW_FORM_ref_addr = 0x10,
    DW_FORM_ref1 = 0x11,
    DW_FORM_ref2 = 0x12,
    DW_FORM_ref4 = 0x13,
    DW_FORM_ref8 = 0x14,
    DW_FORM_ref_udata = 0x15,
    DW_FORM_indirect = 0x16,
    DW_FORM_sec_offset = 0x17,
    DW_FORM_exprloc = 0x18,
    DW_FORM_flag_present = 0x19,
    DW_FORM_line_strp = 0x1f,
};

enum DwarfAttribute {
    DW_AT_sibling = 0x01,
    DW_AT_location = 0x02,
    DW_AT_name = 0x03,
    DW_AT_ordering = 0x09,
    DW_AT_byte_size = 0x0b,
    DW_AT_bit_offset = 0x0c,
    DW_AT_bit_size = 0x0d,
    DW_AT_stmt_list = 0x10,
    DW_AT_low_pc = 0x11,
    DW_AT_high_pc = 0x12,
    DW_AT_language = 0x13,
    DW_AT_discr = 0x15,
    DW_AT_discr_value = 0x16,
    DW_AT_visibility = 0x17,
    DW_AT_import = 0x18,
    DW_AT_string_length = 0x19,
    DW_AT_common_reference = 0x1a,
    DW_AT_comp_dir = 0x1b,
    DW_AT_const_value = 0x1c,
    DW_AT_containing_type = 0x1d,
    DW_AT_default_value = 0x1e,
    DW_AT_inline = 0x20,
    DW_AT_is_optional = 0x21,
    DW_AT_lower_bound = 0x22,
    DW_AT_producer = 0x25,
    DW_AT_prototyped = 0x27,
    DW_AT_return_addr = 0x2a,
    DW_AT_start_scope = 0x2c,
    DW_AT_bit_stride = 0x2e,
    DW_AT_upper_bound = 0x2f,
    DW_AT_abstract_origin = 0x31,
    DW_AT_accessibility = 0x32,
    DW_AT_address_class = 0x33,
    DW_AT_artificial = 0x34,
    DW_AT_base_types = 0x35,
    DW_AT_calling_convention = 0x36,
    DW_AT_count = 0x37,
    DW_AT_data_member_location = 0x38,
    DW_AT_decl_column = 0x39,
    DW_AT_decl_file = 0x3a,
    DW_AT_decl_line = 0x3b,
    DW_AT_declaration = 0x3c,
    DW_AT_discr_list = 0x3d,
    DW_AT_encoding = 0x3e,
    DW_AT_external = 0x3f,
    DW_AT_frame_base = 0x40,
    DW_AT_friend = 0x41,
    DW_AT_identifier_case = 0x42,
    DW_AT_macro_info = 0x43,
    DW_AT_namelist_item = 0x44,
    DW_AT_priority = 0x45,
    DW_AT_segment = 0x46,
    DW_AT_specification = 0x47,
    DW_AT_static_link = 0x48,
    DW_AT_type = 0x49,
    DW_AT_use_location = 0x4a,
    DW_AT_variable_parameter = 0x4b,
    DW_AT_virtuality = 0x4c,
    DW_AT_vtable_elem_location = 0x4d,
    DW_AT_allocated = 0x4e,
    DW_AT_associated = 0x4f,
    DW_AT_data_location = 0x50,
    DW_AT_byte_stride = 0x51,
    DW_AT_entry_pc = 0x52,
    DW_AT_use_UTF8 = 0x53,
    DW_AT_extension = 0x54,
    DW_AT_ranges = 0x55,
    DW_AT_trampoline = 0x56,
    DW_AT_call_column = 0x57,
    DW_AT_call_file = 0x58,
    DW_AT_call_line = 0x59,
    DW_AT_description = 0x5a,
    DW_AT_binary_scale = 0x5b,
    DW_AT_decimal_scale = 0x5c,
    DW_AT_small = 0x5d,
    DW_AT_decimal_sign = 0x5e,
    DW_AT_digit_count = 0x5f,
    DW_AT_picture_string = 0x60,
    DW_AT_mutable = 0x61,
    DW_AT_threads_scaled = 0x62,
    DW_AT_explicit = 0x63,
    DW_AT_object_pointer = 0x64,
    DW_AT_endianity = 0x65,
    DW_AT_elemental = 0x66,
    DW_AT_pure = 0x67,
    DW_AT_recursive = 0x68,
    DW_AT_signature = 0x69,
    DW_AT_main_subprogram = 0x6a,
    DW_AT_data_bit_offset = 0x6b,
    DW_AT_const_expr = 0x6c,
    DW_AT_enum_class = 0x6d,
    DW_AT_linkage_name = 0x6e,
    DW_AT_string_length_bit_size = 0x6f,
    DW_AT_string_length_byte_size = 0x70,
    DW_AT_rank = 0x71,
    DW_AT_str_offsets_base = 0x72,
    DW_AT_addr_base = 0x73,
    DW_AT_rnglists_base = 0x74,
    DW_AT_dwo_name = 0x76,
    DW_AT_reference = 0x77,
    DW_AT_rvalue_reference = 0x78,
    DW_AT_macros = 0x79,
    DW_AT_call_all_calls = 0x7a,
    DW_AT_call_all_source_calls = 0x7b,
    DW_AT_call_all_tail_calls = 0x7c,
    DW_AT_call_return_pc = 0x7d,
    DW_AT_call_value = 0x7e,
    DW_AT_call_origin = 0x7f,
    DW_AT_call_parameter = 0x80,
    DW_AT_call_pc = 0x81,
    DW_AT_call_tail_call = 0x82,
    DW_AT_call_target = 0x83,
    DW_AT_call_target_clobbered = 0x84,
    DW_AT_call_data_location = 0x85,
    DW_AT_call_data_value = 0x86,
    DW_AT_noreturn = 0x87,
    DW_AT_alignment = 0x88,
    DW_AT_export_symbols = 0x89,
    DW_AT_deleted = 0x8a,
    DW_AT_defaulted = 0x8b,
    DW_AT_loclists_base = 0x8c,
};

enum DwarfStandardOps {
    DW_LNS_copy = 1,
    DW_LNS_advance_pc = 2,
    DW_LNS_advance_line = 3,
    DW_LNS_set_file = 4,
    DW_LNS_set_column = 5,
    DW_LNS_negate_stmt = 6,
    DW_LNS_set_basic_block = 7,
    DW_LNS_const_add_pc = 8,
    DW_LNS_fixed_advance_pc = 9,
    DW_LNS_set_prologue_end = 10,
    DW_LNS_set_epilogue_begin = 11,
    DW_LNS_set_isa = 12
};

enum DwarfExtendedOps {
    DW_LNE_end_sequence = 1,
    DW_LNE_set_address = 2,
    DW_LNE_define_file = 3,
    DW_LNE_set_discriminator = 4,

    DW_LNE_lo_user = 128,

    DW_LNE_hi_user = 255
};

enum DwarfLineContents {
    DW_LNCT_path = 0x1,
    DW_LNCT_directory_index = 0x2,
    DW_LNCT_timestamp = 0x3,
    DW_LNCT_size = 0x4,
    DW_LNCT_MD5 = 0x5,
    DW_LNCT_lo_user = 0x2000,
    DW_LNCT_hi_user = 0x3fff
};
enum DwarfTag {
    DW_TAG_array_type = 0x01,
    DW_TAG_class_type = 0x02,
    DW_TAG_entry_point = 0x03,
    DW_TAG_enumeration_type = 0x04,
    DW_TAG_formal_parameter = 0x05,
    DW_TAG_imported_declaration = 0x08,
    DW_TAG_label = 0x0a,
    DW_TAG_lexical_block = 0x0b,
    DW_TAG_member = 0x0d,
    DW_TAG_pointer_type = 0x0f,
    DW_TAG_reference_type = 0x10,
    DW_TAG_compile_unit = 0x11,
    DW_TAG_string_type = 0x12,
    DW_TAG_structure_type = 0x13,
    DW_TAG_subroutine_type = 0x15,
    DW_TAG_typedef = 0x16,
    DW_TAG_union_type = 0x17,
    DW_TAG_unspecified_parameters = 0x18,
    DW_TAG_variant = 0x19,
    DW_TAG_common_block = 0x1a,
    DW_TAG_common_inclusion = 0x1b,
    DW_TAG_inheritance = 0x1c,
    DW_TAG_inlined_subroutine = 0x1d,
    DW_TAG_module = 0x1e,
    DW_TAG_ptr_to_member_type = 0x1f,
    DW_TAG_set_type = 0x20,
    DW_TAG_subrange_type = 0x21,
    DW_TAG_with_stmt = 0x22,
    DW_TAG_access_declaration = 0x23,
    DW_TAG_base_type = 0x24,
    DW_TAG_catch_block = 0x25,
    DW_TAG_const_type = 0x26,
    DW_TAG_constant = 0x27,
    DW_TAG_enumerator = 0x28,
    DW_TAG_file_type = 0x29,
    DW_TAG_friend = 0x2a,
    DW_TAG_namelist = 0x2b,
    DW_TAG_namelist_item = 0x2c,
    DW_TAG_packed_type = 0x2d,
    DW_TAG_subprogram = 0x2e,
    DW_TAG_template_type_parameter = 0x2f,
    DW_TAG_template_value_parameter = 0x30,
    DW_TAG_thrown_type = 0x31,
    DW_TAG_try_block = 0x32,
    DW_TAG_variant_part = 0x33,
    DW_TAG_variable = 0x34,
    DW_TAG_volatile_type = 0x35,
    DW_TAG_dwarf_procedure = 0x36,
    DW_TAG_restrict_type = 0x37,
    DW_TAG_interface_type = 0x38,
    DW_TAG_namespace = 0x39,
    DW_TAG_imported_module = 0x3a,
    DW_TAG_unspecified_type = 0x3b,
    DW_TAG_partial_unit = 0x3c,
    DW_TAG_imported_unit = 0x3d,
    DW_TAG_condition = 0x3f,
    DW_TAG_shared_type = 0x40,
    DW_TAG_type_unit = 0x41,
    DW_TAG_rvalue_reference_type = 0x42,
    DW_TAG_template_alias = 0x43,
    DW_TAG_coarray_type = 0x44,
    DW_TAG_generic_subrange = 0x45,
    DW_TAG_dynamic_type = 0x46,
    DW_TAG_atomic_type = 0x47,
    DW_TAG_call_site = 0x48,
    DW_TAG_call_site_parameter = 0x49,
    DW_TAG_skeleton_unit = 0x4a,
    DW_TAG_immutable_type = 0x4b,

    DW_TAG_lo_user = 0x4080,

    DW_TAG_MIPS_loop = 0x4081,
    DW_TAG_format_label = 0x4101,
    DW_TAG_function_template = 0x4102,
    DW_TAG_class_template = 0x4103,

    DW_TAG_GNU_BINCL = 0x4104,
    DW_TAG_GNU_EINCL = 0x4105,

    DW_TAG_GNU_template_template_param = 0x4106,
    DW_TAG_GNU_template_parameter_pack = 0x4107,
    DW_TAG_GNU_formal_parameter_pack = 0x4108,
    DW_TAG_GNU_call_site = 0x4109,
    DW_TAG_GNU_call_site_parameter = 0x410a,

    DW_TAG_hi_user = 0xffff
};

enum DwarfLocationOp {
    DW_OP_addr = 0x03,
    DW_OP_deref = 0x06,
    DW_OP_const1u = 0x08,
    DW_OP_const1s = 0x09,
    DW_OP_const2u = 0x0a,
    DW_OP_const2s = 0x0b,
    DW_OP_const4u = 0x0c,
    DW_OP_const4s = 0x0d,
    DW_OP_const8u = 0x0e,
    DW_OP_const8s = 0x0f,
    DW_OP_constu = 0x10,
    DW_OP_consts = 0x11,
    DW_OP_dup = 0x12,
    DW_OP_drop = 0x13,
    DW_OP_over = 0x14,
    DW_OP_pick = 0x15,
    DW_OP_swap = 0x16,
    DW_OP_rot = 0x17,
    DW_OP_xderef = 0x18,
    DW_OP_abs = 0x19,
    DW_OP_and = 0x1a,
    DW_OP_div = 0x1b,
    DW_OP_minus = 0x1c,
    DW_OP_mod = 0x1d,
    DW_OP_mul = 0x1e,
    DW_OP_neg = 0x1f,
    DW_OP_not = 0x20,
    DW_OP_or = 0x21,
    DW_OP_plus = 0x22,
    DW_OP_plus_uconst = 0x23,
    DW_OP_shl = 0x24,
    DW_OP_shr = 0x25,
    DW_OP_shra = 0x26,
    DW_OP_xor = 0x27,
    DW_OP_bra = 0x28,
    DW_OP_eq = 0x29,
    DW_OP_ge = 0x2a,
    DW_OP_gt = 0x2b,
    DW_OP_le = 0x2c,
    DW_OP_lt = 0x2d,
    DW_OP_ne = 0x2e,
    DW_OP_skip = 0x2f,
    DW_OP_lit0 = 0x30,
    DW_OP_lit1 = 0x31,
    DW_OP_lit2 = 0x32,
    DW_OP_lit3 = 0x33,
    DW_OP_lit4 = 0x34,
    DW_OP_lit5 = 0x35,
    DW_OP_lit6 = 0x36,
    DW_OP_lit7 = 0x37,
    DW_OP_lit8 = 0x38,
    DW_OP_lit9 = 0x39,
    DW_OP_lit10 = 0x3a,
    DW_OP_lit11 = 0x3b,
    DW_OP_lit12 = 0x3c,
    DW_OP_lit13 = 0x3d,
    DW_OP_lit14 = 0x3e,
    DW_OP_lit15 = 0x3f,
    DW_OP_lit16 = 0x40,
    DW_OP_lit17 = 0x41,
    DW_OP_lit18 = 0x42,
    DW_OP_lit19 = 0x43,
    DW_OP_lit20 = 0x44,
    DW_OP_lit21 = 0x45,
    DW_OP_lit22 = 0x46,
    DW_OP_lit23 = 0x47,
    DW_OP_lit24 = 0x48,
    DW_OP_lit25 = 0x49,
    DW_OP_lit26 = 0x4a,
    DW_OP_lit27 = 0x4b,
    DW_OP_lit28 = 0x4c,
    DW_OP_lit29 = 0x4d,
    DW_OP_lit30 = 0x4e,
    DW_OP_lit31 = 0x4f,
    DW_OP_reg0 = 0x50,
    DW_OP_reg1 = 0x51,
    DW_OP_reg2 = 0x52,
    DW_OP_reg3 = 0x53,
    DW_OP_reg4 = 0x54,
    DW_OP_reg5 = 0x55,
    DW_OP_reg6 = 0x56,
    DW_OP_reg7 = 0x57,
    DW_OP_reg8 = 0x58,
    DW_OP_reg9 = 0x59,
    DW_OP_reg10 = 0x5a,
    DW_OP_reg11 = 0x5b,
    DW_OP_reg12 = 0x5c,
    DW_OP_reg13 = 0x5d,
    DW_OP_reg14 = 0x5e,
    DW_OP_reg15 = 0x5f,
    DW_OP_reg16 = 0x60,
    DW_OP_reg17 = 0x61,
    DW_OP_reg18 = 0x62,
    DW_OP_reg19 = 0x63,
    DW_OP_reg20 = 0x64,
    DW_OP_reg21 = 0x65,
    DW_OP_reg22 = 0x66,
    DW_OP_reg23 = 0x67,
    DW_OP_reg24 = 0x68,
    DW_OP_reg25 = 0x69,
    DW_OP_reg26 = 0x6a,
    DW_OP_reg27 = 0x6b,
    DW_OP_reg28 = 0x6c,
    DW_OP_reg29 = 0x6d,
    DW_OP_reg30 = 0x6e,
    DW_OP_reg31 = 0x6f,
    DW_OP_breg0 = 0x70,
    DW_OP_breg1 = 0x71,
    DW_OP_breg2 = 0x72,
    DW_OP_breg3 = 0x73,
    DW_OP_breg4 = 0x74,
    DW_OP_breg5 = 0x75,
    DW_OP_breg6 = 0x76,
    DW_OP_breg7 = 0x77,
    DW_OP_breg8 = 0x78,
    DW_OP_breg9 = 0x79,
    DW_OP_breg10 = 0x7a,
    DW_OP_breg11 = 0x7b,
    DW_OP_breg12 = 0x7c,
    DW_OP_breg13 = 0x7d,
    DW_OP_breg14 = 0x7e,
    DW_OP_breg15 = 0x7f,
    DW_OP_breg16 = 0x80,
    DW_OP_breg17 = 0x81,
    DW_OP_breg18 = 0x82,
    DW_OP_breg19 = 0x83,
    DW_OP_breg20 = 0x84,
    DW_OP_breg21 = 0x85,
    DW_OP_breg22 = 0x86,
    DW_OP_breg23 = 0x87,
    DW_OP_breg24 = 0x88,
    DW_OP_breg25 = 0x89,
    DW_OP_breg26 = 0x8a,
    DW_OP_breg27 = 0x8b,
    DW_OP_breg28 = 0x8c,
    DW_OP_breg29 = 0x8d,
    DW_OP_breg30 = 0x8e,
    DW_OP_breg31 = 0x8f,
    DW_OP_regx = 0x90,
    DW_OP_fbreg = 0x91,
    DW_OP_bregx = 0x92,
    DW_OP_piece = 0x93,
    DW_OP_deref_size = 0x94,
    DW_OP_xderef_size = 0x95,
    DW_OP_nop = 0x96,
    DW_OP_push_object_address = 0x97,
    DW_OP_call2 = 0x98,
    DW_OP_call4 = 0x99,
    DW_OP_call_ref = 0x9a,
    DW_OP_form_tls_address = 0x9b,
    DW_OP_call_frame_cfa = 0x9c,
    DW_OP_bit_piece = 0x9d,
    DW_OP_implicit_value = 0x9e,
    DW_OP_stack_value = 0x9f,

    DW_OP_implicit_pointer = 0xa0,
    DW_OP_addrx = 0xa1,
    DW_OP_constx = 0xa2,
    DW_OP_entry_value = 0xa3,
    DW_OP_const_type = 0xa4,
    DW_OP_regval_type = 0xa5,
    DW_OP_deref_type = 0xa6,
    DW_OP_xderef_type = 0xa7,
    DW_OP_convert = 0xa8,
    DW_OP_reinterpret = 0xa9,

    DW_OP_lo_user = 0xe0,
    DW_OP_hi_user = 0xff
};

enum DwarfLocationListEntry {
    DW_LLE_end_of_list = 0x0,
    DW_LLE_base_addressx = 0x1,
    DW_LLE_startx_endx = 0x2,
    DW_LLE_startx_length = 0x3,
    DW_LLE_offset_pair = 0x4,
    DW_LLE_default_location = 0x5,
    DW_LLE_base_address = 0x6,
    DW_LLE_start_end = 0x7,
    DW_LLE_start_length = 0x8
};

namespace vipir
{
    DIBuilder::DIBuilder(Module& module)
        : mModule(module)
        , mAbbrevCount(1)
    {
    }

    void DIBuilder::setProducer(std::string_view producer)
    {
        mProducer = std::string(producer);
    }

    void DIBuilder::setFilename(std::string_view filename)
    {
        mFilename = std::string(filename);
    }

    void DIBuilder::setDirectory(std::string_view directory)
    {
        mDirectory = std::string(directory);
    }

    void DIBuilder::setSourceInfo(Value* value, int line, int col)
    {
        value->mLine = line;
        value->mCol = col;
    }

    void DIBuilder::setSourceInfo(Function* func, int line, int col, int endLine, int endCol)
    {
        setSourceInfo(func, line, col);
        func->mEndLine = endLine;
        func->mEndCol = endCol;
    }

    void DIBuilder::setDebugType(Value* value, DIType* type)
    {
        value->mDebugType = type;
    }

    DIType* DIBuilder::createBasicType(std::string name, Type* type, uint8_t encoding)
    {
        auto ptr = new DIBasicType(std::move(name), type, encoding);

        mDebugTypes.push_back(std::unique_ptr<DIType>(ptr));

        return ptr;
    }

    DIType* DIBuilder::createPointerType(DIType* base)
    {
        auto ptr = new DIPointerType(base);

        mDebugTypes.push_back(std::unique_ptr<DIType>(ptr));

        return ptr;
    }

    DIType* DIBuilder::createStructureType(std::string name, StructType* structType, int line, int col)
    {
        auto ptr = new DIStructureType(std::move(name), structType, line, col);

        mDebugTypes.push_back(std::unique_ptr<DIType>(ptr));

        return ptr;
    }

    DIVariable* DIBuilder::createLocalVariable(std::string name, Function* parent, DIType* type, int line, int col)
    {
        auto ptr = new DIVariable(std::move(name), parent, type, line, col);

        parent->mDebugVariables.emplace_back(ptr);

        return ptr;
    }

    DIVariable* DIBuilder::createParameterVariable(std::string name, Function* parent, DIType* type)
    {
        auto ptr = new DIVariable(std::move(name), parent, type);

        parent->mDebugVariables.emplace_back(ptr);

        return ptr;
    }


    static inline int EncodeULEB128(uint64_t value, char(&out)[16])
    {
        std::memset(out, 0, 16);
        int idx = 0;
        do {
            auto byte = value & 0x7f;
            value >>= 7;
            if (value) byte |= 0x80;
            out[idx++] = byte;
        } while (value);
        return idx;
    }
    static inline int EncodeSLEB128(int64_t value, char(&out)[16])
    {
        std::memset(out, 0, 16);

        bool negative = value < 0;
        int idx = 0;
        while (true)
        {
            auto byte = value & 0x7f;
            value >>= 7;
            if (negative) value |= (~0ull << 57);
            if ((value == 0 && !(byte & 0x40)) || (value == -1 && byte & 0x40))
            {
                out[idx++] = byte;
                return idx;
            }
            out[idx++] = byte | 0x80;
        }
    }

    static inline int PhysicalToDwarfRegister(int id)
    {
        switch (id)
        {
            case 0: return 0; // rax
            case 1: return 2; // rcx
            case 2: return 1; // rdx
            case 3: return 3; // rbx
            case 4: return 7; // rsp
            case 5: return 6; // rbp
            case 6: return 4; // rsi
            case 7: return 5; // rdi

            // r8-r15
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
                return id;
        }
        return -1;
    }


    std::uint32_t DIBuilder::getStringPosition(std::string str)
    {
        if (mStrings.find(str) != mStrings.end())
        {
            return mStrings[str];
        }

        auto position = mOpcodeBuilder->getPosition(".debug_str");
        mOpcodeBuilder->createInstruction(".debug_str")
            .string(str, true)
            .emit();
        
        mStrings[str] = position;
        return position;
    }

    std::uint32_t DIBuilder::getLineStringPosition(std::string str)
    {
        if (mLineStrings.find(str) != mLineStrings.end())
        {
            return mLineStrings[str];
        }

        auto position = mOpcodeBuilder->getPosition(".debug_line_str");
        mOpcodeBuilder->createInstruction(".debug_line_str")
            .string(str, true)
            .emit();
        
        mLineStrings[str] = position;
        return position;
    }

    void DIBuilder::initializeStringTable()
    {
    }

    int DIBuilder::getNextAbbrevId()
    {
        return mAbbrevCount++;
    }

    void DIBuilder::writeULEB128(uint64_t value, std::string section)
    {
        char temp[16];
        auto len = EncodeULEB128(value, temp);
        
        mOpcodeBuilder->createInstruction(section)
            .string(std::string_view(temp, temp + len))
            .emit();
    }

    void DIBuilder::writeSLEB128(int64_t value, std::string section)
    {
        char temp[16];
        auto len = EncodeSLEB128(value, temp);
        
        mOpcodeBuilder->createInstruction(section)
            .string(std::string_view(temp, temp + len))
            .emit();
    }

    void DIBuilder::writeTypeOffset(DIType* type, std::string section)
    {
        if (type->mOffset == -1)
        {
            type->mWriteTo.push_back({mOpcodeBuilder->getPosition(section), section});
        }
        mOpcodeBuilder->createInstruction(section)
            .immediate((uint32_t)type->mOffset)
            .emit();
    }

    void DIBuilder::createDwarfHeader()
    {
        // unit length
        mOpcodeBuilder->createInstruction(".debug_info")
            .immediate((uint32_t)0)
            .emit();

        // format
        mOpcodeBuilder->createInstruction(".debug_info")
            .immediate((uint16_t)0x5)
            .emit();

        // unit type
        mOpcodeBuilder->createInstruction(".debug_info")
            .immediate((uint8_t)0x1)
            .emit();
        

        // address size
        mOpcodeBuilder->createInstruction(".debug_info")
            .immediate((uint8_t)0x8)
            .emit();
        
        // offset into .debug_abbrev
        mOpcodeBuilder->relocLabel(".debug_abbrev", "dwarf32", ".debug_info");
        mOpcodeBuilder->createInstruction(".debug_info")
            .immediate((uint32_t)0)
            .emit();
    }

    void DIBuilder::createAbbrevEntry(DebugAbbrevEntry entry)
    {
        char temp[16];

        writeULEB128(entry.id, ".debug_abbrev");
        writeULEB128(entry.type, ".debug_abbrev");
        
        mOpcodeBuilder->createInstruction(".debug_abbrev")
            .immediate((uint8_t)(entry.hasChildren ? DW_CHILDREN_yes : DW_CHILDREN_no))
            .emit();

        for (auto& info : entry.info)
        {
            writeULEB128(info.attribute, ".debug_abbrev");
            writeULEB128(info.type, ".debug_abbrev");
        }

        mOpcodeBuilder->createInstruction(".debug_abbrev")
            .immediate((uint16_t)0)
            .emit();
    }

    // Helper types from cppreference.com
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    void DIBuilder::createInfoEntry(DebugInfoEntry entry)
    {
        char temp[16];

        writeULEB128(entry.abbrev->id, ".debug_info");

        for (auto info : entry.info)
        {
            std::visit(overloaded{
                [this, &temp](ULEB128 arg) {
                    writeULEB128(arg.mValue, ".debug_info");
                },
                [this, &temp](SLEB128 arg) {
                    writeSLEB128(arg.mValue, ".debug_info");
                },
                [this](Relocation arg) {
                    mOpcodeBuilder->relocLabel(arg.symbol, arg.location, arg.section, arg.offset, arg.addend);
                    if (arg.size == 32)
                        mOpcodeBuilder->createInstruction(".debug_info")
                            .immediate((uint32_t)mOpcodeBuilder->getLabel(arg.symbol).first)
                            .emit();
                    else if (arg.size == 64)
                        mOpcodeBuilder->createInstruction(".debug_info")
                            .immediate(mOpcodeBuilder->getLabel(arg.symbol).first)
                            .emit();
                },
                [this](TypeOffset arg) {
                    writeTypeOffset(arg.mType, ".debug_info");
                },
                [this](auto arg) { 
                    mOpcodeBuilder->createInstruction(".debug_info")
                        .immediate(arg)
                        .emit();
                }
            }, info);
        }
    }

    uint8_t EncodeSpecialOpcode(int lineInc, int opAdv)
    {
        uint32_t ret = (lineInc + 5) + (14 * opAdv) + 13;
        if (ret > 255) return 0;
        return ret;
    }

    void DIBuilder::writeSpecialOpcode(int lineInc, int addrInc)
    {
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate(EncodeSpecialOpcode(lineInc, addrInc))
            .emit();
    }

    void DIBuilder::writeStandardOpcode(uint8_t opcode, std::vector<std::variant<ULEB128, SLEB128> > operands)
    {
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate(opcode)
            .emit();
        for (auto& operand : operands)
        {
            std::visit(overloaded {
                [this](ULEB128 arg) { writeULEB128(arg.mValue, ".debug_line"); },
                [this](SLEB128 arg) { writeSLEB128(arg.mValue, ".debug_line"); },
            }, operand);
        }
    }

    void DIBuilder::incrementSourceLocation(int lineChange, int addressChange)
    {
        auto specialOpcode = EncodeSpecialOpcode(lineChange, addressChange);
        if (!specialOpcode)
        {
            writeStandardOpcode(DW_LNS_advance_line, {SLEB128(lineChange)});
            writeStandardOpcode(DW_LNS_advance_pc, {ULEB128(addressChange)});
            writeStandardOpcode(DW_LNS_copy, {});
        }
        else
        {
            writeSpecialOpcode(lineChange, addressChange);
        }
    }

    void DIBuilder::createDebugLine(std::vector<MC::EmitSourceInfo*>& sourceInfo)
    {
        mOpcodeBuilder->setSection(".debug_line");
        // unit length(will be overwritten)
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint32_t)0)
            .emit();

        // version
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint16_t)0x5)
            .emit();

        // address size
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)0x8)
            .emit();

        // segment selector size
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)0)
            .emit();

        auto headerLengthPos = mOpcodeBuilder->getPosition(".debug_line");

        // min inst length
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();

        // max ops per inst
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();

        // default is stmt
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();

        // Maybe generate these depending on the line information?
        // line base
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)(-5))
            .emit();

        // line range
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)14)
            .emit();

        // opcode base
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)13)
            .emit();

        uint8_t standardOpcodeLengths[] = { 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 };
        for (auto length : standardOpcodeLengths)
        {
            mOpcodeBuilder->createInstruction(".debug_line")
                .immediate(length)
                .emit();
        }

        // directory entry format count
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();
        
        // directory entry format
        writeULEB128(DW_LNCT_path, ".debug_line");
        writeULEB128(DW_FORM_line_strp, ".debug_line"); // Should debug line str be used here?

        // TODO: Iterate over provided directories
        // number of directories
        writeULEB128(1, ".debug_line");
        // compilation directory
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate(getLineStringPosition(mDirectory))
            .emit();

        // filename entry format count
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();
        
        // filename entry format
        writeULEB128(DW_LNCT_path, ".debug_line");
        writeULEB128(DW_FORM_line_strp, ".debug_line"); // Should debug line str be used here?

        // TODO: Iterate over provided filenames?
        // number of filenames
        writeULEB128(1, ".debug_line");
        // source filename
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate(getLineStringPosition(mFilename))
            .emit();
        
        // header length
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint32_t)mOpcodeBuilder->getPosition(".debug_line")-8)
            .emit(headerLengthPos);

        // TODO: Generate line number program
        
        int lastLine = 1;
        int lastCol = 1;
        int lastAddress = 0;
        writeStandardOpcode(DW_LNS_set_file, {0_uleb});
        writeStandardOpcode(DW_LNS_set_column, {1_uleb});

        auto pos = mOpcodeBuilder->getPosition(".debug_line");
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)0)
            .emit();
        writeULEB128(9, ".debug_line");
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)DW_LNE_set_address)
            .emit();
        mOpcodeBuilder->relocLabel(".text", "dwarf", ".debug_line");
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint64_t)mOpcodeBuilder->getLabel(".text").first)
            .emit();

        for (auto sourceLoc : sourceInfo)
        {
            auto lineDiff = sourceLoc->mLine - lastLine;
            auto colDiff = sourceLoc->mCol - lastCol;
            auto addrDiff = sourceLoc->mAddress - lastAddress;

            if (colDiff)
                writeStandardOpcode(DW_LNS_set_column, {ULEB128(sourceLoc->mCol)});
            incrementSourceLocation(lineDiff, addrDiff);

            lastLine = sourceLoc->mLine;
            lastCol = sourceLoc->mCol;
            lastAddress = sourceLoc->mAddress;
        }

        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)0)
            .emit();
        writeULEB128(1, ".debug_line");
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)DW_LNE_end_sequence)
            .emit();

        // unit length
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint32_t)mOpcodeBuilder->getPosition(".debug_line")-4)
            .emit(0, true);
    }

    void DIBuilder::createDebugArranges()
    {
        // version
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint16_t)2)
            .emit();

        // debug info offset
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint32_t)0)
            .emit();

        // address size
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint8_t)0x8)
            .emit();
        
        // segment selector size
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint8_t)0)
            .emit();
        
        // .text
        mOpcodeBuilder->relocLabel(".text", "dwarf", ".debug_arranges");
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint64_t)0)
            .emit();
        
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint64_t)mOpcodeBuilder->getPosition(".text"))
            .emit();
        
        // null entry (end)
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint64_t)0)
            .emit();
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint64_t)0)
            .emit();
        
        // unit length
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint32_t)mOpcodeBuilder->getPosition(".debug_arranges"))
            .emit(0);
    }
    
    void DIBuilder::createDebugAddr()
    {
        mOpcodeBuilder->createInstruction(".debug_addr")
            .immediate((uint32_t)0)
            .emit();

        mOpcodeBuilder->createInstruction(".debug_addr")
            .immediate((uint16_t)5)
            .emit();

        mOpcodeBuilder->createInstruction(".debug_addr")
            .immediate((uint8_t)0x8)
            .emit();
        
        mOpcodeBuilder->createInstruction(".debug_addr")
            .immediate((uint8_t)0x0)
            .emit();


        mOpcodeBuilder->relocLabel(".text", "dwarf", ".debug_addr");
        mOpcodeBuilder->createInstruction(".debug_addr")
            .immediate((uint64_t)0x0)
            .emit();
        

        mOpcodeBuilder->createInstruction(".debug_addr")
        .immediate((uint32_t)mOpcodeBuilder->getPosition(".debug_addr")-4)
        .emit(0, true);
    }

    void DIBuilder::createDebugLoclists()
    {
        // unit length(will be overwritten)
        mOpcodeBuilder->createInstruction(".debug_loclists")
            .immediate((uint32_t)0x0)
            .emit();

        mOpcodeBuilder->createInstruction(".debug_loclists")
            .immediate((uint16_t)5)
            .emit();

        mOpcodeBuilder->createInstruction(".debug_loclists")
            .immediate((uint8_t)0x8)
            .emit();
        
        mOpcodeBuilder->createInstruction(".debug_loclists")
            .immediate((uint8_t)0x0)
            .emit();
        
        mOpcodeBuilder->createInstruction(".debug_loclists")
            .immediate((uint32_t)0x0)
            .emit();
    }

    constexpr std::size_t AlignUp(std::size_t value, std::size_t align)
    {
        return (value + align - 1) & ~(align - 1);
    }

    void DIBuilder::generateDwarf(MC::Builder& mcBuilder, codegen::OpcodeBuilder& opcodeBuilder)
    {
        mOpcodeBuilder = &opcodeBuilder;
        mOpcodeBuilder->addLabel(".LEtext", ".text", false);

        initializeStringTable();
        
        uint32_t filenamePosition = getStringPosition(mFilename);
        uint32_t directoryPosition = getStringPosition(mDirectory);
        uint32_t producerPosition = getStringPosition(mProducer);

        createDwarfHeader();
        createDebugLoclists();
        
        /*
         * Compile Unit
         */
        DebugAbbrevEntry compileUnit = { getNextAbbrevId(), DW_TAG_compile_unit, true, {
            { DW_AT_producer, DW_FORM_strp },
            { DW_AT_language, DW_FORM_data1 },
            { DW_AT_name, DW_FORM_strp },
            { DW_AT_comp_dir, DW_FORM_strp },
            { DW_AT_low_pc, DW_FORM_addr },
            { DW_AT_high_pc, DW_FORM_data8 },
            { DW_AT_stmt_list, DW_FORM_sec_offset },
        }};
        createAbbrevEntry(compileUnit);
        
        DebugInfoEntry compileUnitInfo = { &compileUnit, {
            producerPosition,
            (uint8_t)DW_LANG_C11,
            filenamePosition,
            directoryPosition,
            Relocation(".text", "dwarf", ".debug_info", 0, 0, 64),
            opcodeBuilder.getPosition(".text"),
            Relocation(".debug_line", "dwarf32", ".debug_info", 0, 0, 32),
        }};
        createInfoEntry(compileUnitInfo);

        DebugAbbrevEntry basicTypeAbbrev = { getNextAbbrevId(), DW_TAG_base_type, false, {
            { DW_AT_byte_size, DW_FORM_data1 },
            { DW_AT_encoding, DW_FORM_data1 },
            { DW_AT_name, DW_FORM_strp },
        }};
        createAbbrevEntry(basicTypeAbbrev);

        DebugAbbrevEntry pointerTypeAbbrev = { getNextAbbrevId(), DW_TAG_pointer_type, false, {
            { DW_AT_byte_size, DW_FORM_data1 },
            { DW_AT_type, DW_FORM_ref4 }
        }};
        createAbbrevEntry(pointerTypeAbbrev);

        DebugAbbrevEntry structTypeAbbrev = { getNextAbbrevId(), DW_TAG_structure_type, true, {
            { DW_AT_name, DW_FORM_strp },
            { DW_AT_byte_size, DW_FORM_data2 },
            { DW_AT_decl_file, DW_FORM_strp },
            { DW_AT_decl_line, DW_FORM_data1 },
            { DW_AT_decl_column, DW_FORM_data1 },
        }};
        createAbbrevEntry(structTypeAbbrev);
        DebugAbbrevEntry memberAbbrev = { getNextAbbrevId(), DW_TAG_member, false, {
            { DW_AT_name, DW_FORM_strp },
            { DW_AT_byte_size, DW_FORM_data2 },
            { DW_AT_decl_file, DW_FORM_strp },
            { DW_AT_decl_line, DW_FORM_data1 },
            { DW_AT_decl_column, DW_FORM_data1 },
            { DW_AT_type, DW_FORM_ref4 },
            { DW_AT_data_member_location, DW_FORM_data2 },
        }};
        createAbbrevEntry(memberAbbrev);

        for (auto& type : mDebugTypes)
        {
            type->mOffset = opcodeBuilder.getPosition(".debug_info");
            if (auto basicType = dynamic_cast<DIBasicType*>(type.get()))
            {
                DebugInfoEntry typeInfo = { &basicTypeAbbrev, {
                    (uint8_t)(basicType->mType->getSizeInBits() / 8),
                    (uint8_t)basicType->mEncoding,
                    getStringPosition(basicType->mName)
                }};
                createInfoEntry(typeInfo);
            }
            else if (auto pointerType = dynamic_cast<DIPointerType*>(type.get()))
            {
                DebugInfoEntry typeInfo = { &pointerTypeAbbrev, {
                    (uint8_t)0x8,
                    TypeOffset(pointerType->mBaseType)
                }};
                createInfoEntry(typeInfo);
            }
            else if (auto structureType = dynamic_cast<DIStructureType*>(type.get()))
            {
                DebugInfoEntry typeInfo = { &structTypeAbbrev, {
                    getStringPosition(structureType->mName),
                    (uint16_t)(structureType->mStructType->getSizeInBits() / 8),
                    getStringPosition(mFilename),
                    (uint8_t)structureType->mLine,
                    (uint8_t)structureType->mCol,
                }};
                createInfoEntry(typeInfo);
                int idx = 0;
                int offset = 0;
                for (auto& member : structureType->mMembers)
                {
                    DebugInfoEntry memberInfo = { &memberAbbrev, {
                        getStringPosition(member.name),
                        (uint16_t)(structureType->mStructType->getField(idx)->getSizeInBits()/8),
                        getStringPosition(mFilename),
                        (uint8_t)structureType->mLine,
                        (uint8_t)structureType->mCol,
                        TypeOffset(member.type),
                        (uint16_t)offset
                    }};
                    offset += AlignUp(structureType->mStructType->getField(idx)->getSizeInBits()/8, structureType->mStructType->getAlignment()/8);
                    createInfoEntry(memberInfo);
                    ++idx;
                }
                opcodeBuilder.createInstruction(".debug_info")
                    .immediate((uint8_t)0)
                    .emit();
            }
        }

        for (auto& type : mDebugTypes)
        {
            for (auto writeTo : type->mWriteTo)
            {
                opcodeBuilder.createInstruction(writeTo.section)
                    .immediate((uint32_t)type->mOffset)
                    .emit(writeTo.offset, true);
            }
        }

        DebugAbbrevEntry singleValueVariableAbbrev = { getNextAbbrevId(), DW_TAG_variable, false, {
            { DW_AT_name, DW_FORM_strp },
            { DW_AT_decl_file, DW_FORM_strp },
            { DW_AT_decl_line, DW_FORM_data2 },
            { DW_AT_decl_column, DW_FORM_data2 },
            { DW_AT_type, DW_FORM_ref4 },
            { DW_AT_location, DW_FORM_exprloc },
        }};
        DebugAbbrevEntry singleValueParameterAbbrev = singleValueVariableAbbrev;
        singleValueParameterAbbrev.type = DW_TAG_formal_parameter;
        singleValueParameterAbbrev.id = getNextAbbrevId();

        createAbbrevEntry(singleValueVariableAbbrev);
        createAbbrevEntry(singleValueParameterAbbrev);


        DebugAbbrevEntry multiValueVariableAbbrev = { getNextAbbrevId(), DW_TAG_variable, false, {
            { DW_AT_name, DW_FORM_strp },
            { DW_AT_decl_file, DW_FORM_strp },
            { DW_AT_decl_line, DW_FORM_data2 },
            { DW_AT_decl_column, DW_FORM_data2 },
            { DW_AT_type, DW_FORM_ref4 },
            { DW_AT_location, DW_FORM_sec_offset },
        }};
        DebugAbbrevEntry multiValueParameterAbbrev = multiValueVariableAbbrev;
        multiValueParameterAbbrev.type = DW_TAG_formal_parameter;
        multiValueParameterAbbrev.id = getNextAbbrevId();

        createAbbrevEntry(multiValueVariableAbbrev);
        createAbbrevEntry(multiValueParameterAbbrev);
        
        int idx = 2;
        for (auto& global : mModule.getGlobals())
        {
            if (auto func = dynamic_cast<Function*>(global.get()))
            {
                auto emittedValue = global->getEmittedValue();
                auto label = static_cast<lir::Lbl*>(emittedValue.get());
                
                std::string endSym = opcodeBuilder.getLabelAfter(label->getName());
                if (endSym.empty()) endSym = ".LEtext";

                uint64_t start = opcodeBuilder.getLabel(label->getName()).first;
                uint64_t end = opcodeBuilder.getLabel(endSym).first;
                if (end == 0) end = opcodeBuilder.getPosition(".text");

                auto type = global->mDebugType;
                uint32_t typeOffset = type->mOffset;

                bool hasChildren = false;
                if (!func->mDebugVariables.empty()) hasChildren = true;

                DebugAbbrevEntry subprogramAbbrev = { getNextAbbrevId(), DW_TAG_subprogram, hasChildren, {
                    { DW_AT_external, DW_FORM_flag_present },
                    { DW_AT_name, DW_FORM_strp },
                    { DW_AT_decl_file, DW_FORM_data1 },
                    { DW_AT_decl_line, DW_FORM_data1 },
                    { DW_AT_decl_column, DW_FORM_data1 },
                    { DW_AT_type, DW_FORM_ref4 },
                    { DW_AT_low_pc, DW_FORM_addr },
                    { DW_AT_high_pc, DW_FORM_data8 },
                    { DW_AT_frame_base, DW_FORM_exprloc },
                    { DW_AT_call_all_calls, DW_FORM_flag_present },
                }};
                createAbbrevEntry(subprogramAbbrev);
                
                DebugInfoEntry subprogramInfoStart = { &subprogramAbbrev, {
                    getStringPosition(label->getName()),
                    (uint8_t)0,
                    (uint8_t)global->mLine,
                    (uint8_t)global->mCol,
                    typeOffset,
                    Relocation(".text", "dwarf", ".debug_info", 0, start, 64),
                    end - start,
                    ULEB128(1),
                    (uint8_t)DW_OP_call_frame_cfa
                }};
                createInfoEntry(subprogramInfoStart);

                auto encodeValue = [this](lir::OperandPtr& value, std::string section){
                    if (auto imm = dynamic_cast<lir::Immediate*>(value.get()))
                    {
                        writeULEB128(10, section);
                        mOpcodeBuilder->createInstruction(section)
                            .immediate((uint8_t)DW_OP_implicit_value)
                            .emit();
                        writeULEB128(8, section);
                        mOpcodeBuilder->createInstruction(section)
                            .immediate((uint64_t)imm->value())
                            .emit();
                    }
                    if (auto vreg = dynamic_cast<lir::VirtualReg*>(value.get()))
                    {
                        if (vreg->mVreg->onStack())
                        {
                            writeULEB128(2, section);
                            mOpcodeBuilder->createInstruction(section)
                                .immediate((uint8_t)DW_OP_fbreg)
                                .emit();
                            writeSLEB128(-vreg->mVreg->getStackOffset() - 16, section);
                        }
                        else
                        {
                            writeULEB128(2, section);
                            mOpcodeBuilder->createInstruction(section)
                                .immediate((uint8_t)DW_OP_regx)
                                .emit();
                            writeSLEB128(PhysicalToDwarfRegister(vreg->mVreg->getPhysicalRegister()), section);
                        }
                    }
                };

                for (auto& debugVariable : func->mDebugVariables)
                {
                    debugVariable->mOffset = opcodeBuilder.getPosition(".debug_info");

                    auto values = debugVariable->mValues;
                    if (values.size() == 1)
                    {
                        DebugAbbrevEntry* abbrev = debugVariable->mParameter ? &singleValueParameterAbbrev : &singleValueVariableAbbrev;

                        DebugInfoEntry variableInfo = { abbrev, {
                            getStringPosition(debugVariable->mName),
                            getStringPosition(mFilename),
                            (uint16_t)debugVariable->mLine,
                            (uint16_t)debugVariable->mCol,
                            (uint32_t)debugVariable->mType->mOffset,
                        }};
                        createInfoEntry(variableInfo);
                        if (values[0].pointee)
                        {
                            writeULEB128(6, ".debug_info");
                            opcodeBuilder.createInstruction(".debug_info")
                                .immediate((uint8_t)DW_OP_implicit_pointer)
                                .emit();
                            
                            opcodeBuilder.createInstruction(".debug_info")
                                .immediate((uint32_t)values[0].pointee->mOffset)
                                .emit();
                                
                            writeSLEB128(0, ".debug_info");
                        }
                        else
                        {
                            auto value = values[0].value->getEmittedValue();
                            encodeValue(value, ".debug_info");
                        }
                    }
                    else
                    {
                        DebugAbbrevEntry* abbrev = debugVariable->mParameter ? &multiValueParameterAbbrev : &multiValueVariableAbbrev;

                        DebugInfoEntry variableInfo = { abbrev, {
                            getStringPosition(debugVariable->mName),
                            getStringPosition(mFilename),
                            (uint16_t)debugVariable->mLine,
                            (uint16_t)debugVariable->mCol,
                            (uint32_t)debugVariable->mType->mOffset,
                            (uint32_t)mOpcodeBuilder->getPosition(".debug_loclists")
                        }};
                        createInfoEntry(variableInfo);

                        for (auto& value : debugVariable->mValues)
                        {
                            uint64_t endAddress = 0;
                            if (value.endAddress)
                                endAddress = value.endAddress->getAddress();
                            else
                                endAddress = mOpcodeBuilder->getPosition(".text");

                            opcodeBuilder.createInstruction(".debug_loclists")
                                .immediate((uint8_t)DW_LLE_offset_pair)
                                .emit();
                            writeULEB128(value.startAddress->getAddress(), ".debug_loclists");
                            writeULEB128(endAddress, ".debug_loclists");
                            if (values[0].pointee)
                            {
                                writeULEB128(6, ".debug_loclists");
                                opcodeBuilder.createInstruction(".debug_loclists")
                                    .immediate((uint8_t)DW_OP_implicit_pointer)
                                    .emit();
                                
                                opcodeBuilder.createInstruction(".debug_loclists")
                                    .immediate((uint32_t)values[0].pointee->mOffset)
                                    .emit();
                                
                                writeSLEB128(0, ".debug_loclists");
                            }
                            else
                            {
                                auto emittedValue = value.value->getEmittedValue();
                                encodeValue(emittedValue, ".debug_loclists");
                            }
                        }
                        opcodeBuilder.createInstruction(".debug_loclists")
                            .immediate((uint8_t)DW_LLE_end_of_list)
                            .emit();
                    }
                }

                // Emit a null byte to signify this is the end of this entry's children
                if (hasChildren)
                {
                    opcodeBuilder.createInstruction(".debug_info")
                        .immediate((uint8_t)0)
                        .emit();
                }
            }
        }

        opcodeBuilder.createInstruction(".debug_info")
            .immediate((uint8_t)0)
            .emit();
        opcodeBuilder.createInstruction(".debug_info")
            .immediate((uint8_t)0)
            .emit();
        opcodeBuilder.createInstruction(".debug_abbrev")
            .immediate((uint8_t)0)
            .emit();

        createDebugLine(mcBuilder.getSourceInfo());
        createDebugAddr();
        //createDebugArranges();
        
        // unit lengths
        opcodeBuilder.createInstruction(".debug_info")
            .immediate((uint32_t)opcodeBuilder.getPosition(".debug_info")-4)
            .emit(0, true);
        opcodeBuilder.createInstruction(".debug_loclists")
            .immediate((uint32_t)opcodeBuilder.getPosition(".debug_loclists")-4)
            .emit(0, true);
    }
}