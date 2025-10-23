
import os
import json
from math import trunc

os.makedirs('../generated', exist_ok=True)

function_counter = 0

def generate_swizzle_four():
    components = ['x', 'y', 'z', 'w']
    swizzles = []
    
    for x in components:
        swizzles.append(x)
        for y in components:
            swizzles.append(x+y)
            for z in components:
                swizzles.append(x+y+z)
                for w in components:
                    swizzles.append(x+y+z+w)
    components = ['r', 'g', 'b', 'a']
    for x in components:
        swizzles.append(x)
        for y in components:
            swizzles.append(x+y)
            for z in components:
                swizzles.append(x+y+z)
                for w in components:
                    swizzles.append(x+y+z+w)
    return swizzles

def generate_swizzle_three():
    components = ['x', 'y', 'z']

    swizzles = []
    for x in components:
        swizzles.append(x)
        for y in components:
            swizzles.append(x+y)
            for z in components:
                swizzles.append(x+y+z)
                for w in components:
                    swizzles.append(x+y+z+w)
    components = ['r', 'g', 'b']
    for x in components:
        swizzles.append(x)
        for y in components:
            swizzles.append(x+y)
            for z in components:
                swizzles.append(x+y+z)
                for w in components:
                    swizzles.append(x+y+z+w)
    return swizzles

def generate_swizzle_two():
    components = ['x', 'y']
    swizzles = []
    for x in components:
        swizzles.append(x)
        for y in components:
            swizzles.append(x+y)
            for z in components:
                swizzles.append(x+y+z)
                for w in components:
                    swizzles.append(x+y+z+w)
    components = ['r', 'g']
    for x in components:
        swizzles.append(x)
        for y in components:
            swizzles.append(x+y)
            for z in components:
                swizzles.append(x+y+z)
                for w in components:
                    swizzles.append(x+y+z+w)
    return swizzles

def generate_types():
    types = ["Float32", "UInt32", "Int32", "Bool8", "Float16", "UInt16", "Int16"]


    bit_width_mapping = {
        "Float32": 32,
        "UInt32": 32,
        "Int32": 32,
        "Bool8": 8,
        "Float16": 16,
        "UInt16": 16,
        "Int16": 16,
    }

    data_type_mapping = {
        "Float32": "f32",
        "UInt32": "u32",
        "Int32": "i32",
        "Bool8": "b8",
        "Float16": "f16",
        "UInt16": "u16",
        "Int16": "i16",
        "Float32x2": "f32x2",
        "Float32x3": "f32x3",
        "Float32x4": "f32x4",
        "UInt32x2": "u32x2",
        "UInt32x3": "u32x3",
        "UInt32x4": "u32x4",
        "Int32x2": "i32x2",
        "Int32x3": "i32x3",
        "Int32x4": "i32x4",
        "Bool8x2": "b8x2",
        "Bool8x3": "b8x3",
        "Bool8x4": "b8x4",
        "Float16x2": "f16x2",
        "Float16x3": "f16x3",
        "Float16x4": "f16x4",
        "UInt16x2": "u16x2",
        "UInt16x3": "u16x3",
        "UInt16x4": "u16x4",
        "Int16x2": "i16x2",
        "Int16x3": "i16x3",
        "Int16x4": "i16x4",
        "Float32x2x2": "f32x2x2",
        "Float32x2x3": "f32x2x3",
        "Float32x2x4": "f32x2x4",
        "Float32x3x2": "f32x3x2",
        "Float32x3x3": "f32x3x3",
        "Float32x3x4": "f32x3x4",
        "Float32x4x2": "f32x4x2",
        "Float32x4x3": "f32x4x3",
        "Float32x4x4": "f32x4x4",
        "Float16x2x2": "f16x2x2",
        "Float16x2x3": "f16x2x3",
        "Float16x2x4": "f16x2x4",
        "Float16x3x2": "f16x3x2",
        "Float16x3x3": "f16x3x3",
        "Float16x3x4": "f16x3x4",
        "Float16x4x2": "f16x4x2",
        "Float16x4x3": "f16x4x3",
        "Float16x4x4": "f16x4x4",
        "GeometryTriangle": "GeometryTriangle",
        "GeometryLine": "GeometryLine",
        "GeometryPoint": "GeometryPoint"
    }

    
    base_type_mapping = {
        'Float32': 'Float32',
        'Float32x2': 'Float32',
        'Float32x3': 'Float32',
        'Float32x4': 'Float32',
        'Float16': 'Float16',
        'Float16x2': 'Float16',
        'Float16x3': 'Float16',
        'Float16x4': 'Float16',
        'Int32': 'Int32',
        'Int32x2': 'Int32',
        'Int32x3': 'Int32',
        'Int32x4': 'Int32',
        'Int16': 'Int16',
        'Int16x2': 'Int16',
        'Int16x3': 'Int16',
        'Int16x4': 'Int16',
        'UInt32': 'UInt32',
        'UInt32x2': 'UInt32',
        'UInt32x3': 'UInt32',
        'UInt32x4': 'UInt32',
        'UInt16': 'UInt16',
        'UInt16x2': 'UInt16',
        'UInt16x3': 'UInt16',
        'UInt16x4': 'UInt16',
        'Bool8': 'Bool8',
        'Bool8x2': 'Bool8',
        'Bool8x3': 'Bool8',
        'Bool8x4': 'Bool8'
    }

    
    vector_size_mapping = {
        'Float32': 1, 'Float32x2': 2, 'Float32x3': 3, 'Float32x4': 4,
        'Float16': 1, 'Float16x2': 2, 'Float16x3': 3, 'Float16x4': 4,
        'Int32': 1, 'Int32x2': 2, 'Int32x3': 3, 'Int32x4': 4,
        'Int16': 1, 'Int16x2': 2, 'Int16x3': 3, 'Int16x4': 4,
        'UInt32': 1, 'UInt32x2': 2, 'UInt32x3': 3, 'UInt32x4': 4,
        'UInt16': 1, 'UInt16x2': 2, 'UInt16x3': 3, 'UInt16x4': 4,
        'Bool8': 1, 'Bool8x2': 2, 'Bool8x3': 3, 'Bool8x4': 4
    }



    bit_operator_names = ['or', 'and', 'xor', 'lsh', 'rsh']
    bit_operators = ['|', '&', '^', '<<', '>>']

    scalar_operator_names = ['add', 'sub', 'mul', 'div', 'mod']
    scalar_operators = ['+', '-', '*', '/', '%']

    comparison_operator_names = ['lt', 'lte', 'gt', 'gte', 'eq', 'neq']
    comparison_operators = ['<', '<=', '>', '>=', '==', '!=']

    bool_operator_names = ['oror', 'andand', 'eq', 'neq']
    bool_operators = ['||', '&&', '==', '!=']

    assignment_operator_names = ['addasg', 'subasg', 'mulasg', 'divasg']
    assignment_operators = ['+=', '-=', '*=', '/=']

    bit_assignment_operator_names = ['orasg', 'andasg', 'xorasg', 'lshasg', 'rhsasg']
    bit_assignment_operators = ['|=', '&=', '^=', '<<=', '>>=']

    scale_operator_names = ['scale', 'scale', 'scale', 'scale', 'scale', 'scale']
    scale_operators = ['*', '*', '*', '*', '*', '*']
    scale_operator_types = ['Float32', 'Float16', 'UInt32', 'UInt16', 'Int32', 'Int16']

    index_operator_names = ['index_Int32', 'index_UInt32', 'index_Int16', 'index_UInt16']
    index_operators = ['[]', '[]', '[]', '[]']
    index_types = ['Int32', 'UInt32', 'Int16', 'UInt16']

    vector_matrix_operator_names = ['transform']
    vector_matrix_operators = ['*']

    header_file = open("../generated/types.h", 'w')
    source_file = open("../generated/types.cc", 'w')

    ### Type conversion enum and SPIRV conversion methods ###
    spirv_intrinsics = open("../generated/spirv.h", 'w')

    class NamerEntry:
        def __init__(self, name, type):
            self.name = name
            self.type = type

    class Namer:
        def __init__(self):
            self.names = []

        def definition(self):
            namer_struct = ''
            namer_struct += 'struct Namer\n'
            namer_struct += '{\n'
            namer_struct += '    Namer()\n'
            namer_struct += '    {\n'
            for name in self.names:
                namer_struct += f'        {name.name}Type.name = "{name.type}"_c;\n'
            namer_struct += '    }\n'
            namer_struct += '};\n'
            namer_struct += 'Namer namer;\n\n'
            return namer_struct

    namer = Namer()
    namer_line = ''

    conversion_table_enum = "enum TypeConversionTable\n"
    conversion_table_enum += '{\n'

    class TypeConverter:
        def __init__(self, enu, target, source_data_type, spirv_conversion_function, spirv_conversion_prep, spirv_conversion_arguments):
            self.enum = enu
            self.target = target
            self.source_data_type = source_data_type    
            self.spirv_conversion_function = spirv_conversion_function
            self.spirv_conversion_prep = spirv_conversion_prep
            self.spirv_conversion_arguments = spirv_conversion_arguments

    type_conversions = []
    for type1 in types:
        for type2 in types:
            if type1 != type2:
                conversion_table_enum += f'    {type1}To{type2},\n'
                spirv_conversion_function = ''
                spirv_conversion_prep = ''
                width1 = bit_width_mapping[type1]
                width2 = bit_width_mapping[type2]

                if type1.startswith('Bool'):
                    #if type2.startswith('Int') or type2.startswith('UInt'):
                    spirv_conversion_prep = f'            SPIRVResult trueValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::{type2}(1));\n'
                    spirv_conversion_prep += f'            SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::{type2}(0));\n'
                    type_conversions.append(TypeConverter(f'{type1}To{type2}', type2, data_type_mapping[type1], 'OpSelect', spirv_conversion_prep, 'trueValue, falseValue'))
                else:    
                    if not type2.startswith('Bool'):
                        spirv_conversion_prep = ''
                        if type1.startswith('Float'):                    
                            if width1 != width2:
                                spirv_conversion_function = 'OpFConvert'
                            if type2.startswith('UInt'):
                                spirv_conversion_function = 'OpConvertFToU'
                            elif type2.startswith('Int'):
                                spirv_conversion_function = 'OpConvertFToS'
                        elif type1.startswith('UInt'):
                            if width1 != width2:
                                spirv_conversion_function = 'OpUConvert'
                            if type2.startswith('Int'):
                                spirv_conversion_function = 'OpBitcast'
                            elif type2.startswith('Float'):
                                spirv_conversion_function = 'OpConvertUToF'
                        elif type1.startswith('Int'):
                            if width1 != width2:
                                spirv_conversion_function = 'OpSConvert'
                            if type2.startswith('UInt'):
                                spirv_conversion_function = 'OpBitcast'
                            elif type2.startswith('Float'):
                                spirv_conversion_function = 'OpConvertSToF'
                        type_conversions.append(TypeConverter(f'{type1}To{type2}', type2, data_type_mapping[type1], spirv_conversion_function, spirv_conversion_prep, ''))
                    else:
                        if type1.startswith('UInt') or type1.startswith('Int'):
                            spirv_conversion_prep += f'            SPIRVResult falseValue = GenerateConstantSPIRV(c, g, ConstantCreationInfo::{type1}(0));\n'
                            type_conversions.append(TypeConverter(f'{type1}To{type2}', type2, data_type_mapping[type1], 'OpINotEqual', spirv_conversion_prep, 'falseValue'))


    conversion_table_enum = conversion_table_enum[0:-2]
    conversion_table_enum += '\n};\n\n'

    spirv_type_construction = ''

    spirv_intrinsics.write("#pragma once\n")
    spirv_intrinsics.write("//-------------------------------------------------\n")
    spirv_intrinsics.write("// *** Generated by type_gen.py. ***\n")
    spirv_intrinsics.write("//       DO NOT MODIFY!!!\n")
    spirv_intrinsics.write("//-------------------------------------------------\n")
    spirv_intrinsics.write("static auto CreateSampledImageSPIRV = [](const Compiler* c, SPIRVGenerator* g, SPIRVResult img, SPIRVResult samp) -> SPIRVResult\n")
    spirv_intrinsics.write("{\n")
    spirv_intrinsics.write("        assert(img.parentTypes.size() > 0);\n")
    spirv_intrinsics.write("        SPIRVResult image = LoadValueSPIRV(c, g, img, true);\n")
    spirv_intrinsics.write("        SPIRVResult sampler = LoadValueSPIRV(c, g, samp, true);\n")
    spirv_intrinsics.write("        uint32_t typeSymbol = AddType(g, TStr::Compact(\"sampledImage_\", img.parentTypes[0]), OpTypeSampledImage, SPVArg{img.parentTypes[0]});\n")
    spirv_intrinsics.write("        uint32_t sampledImage = g->writer->MappedInstruction(OpSampledImage, SPVWriter::Section::LocalFunction, typeSymbol, image, sampler);\n")
    spirv_intrinsics.write("        return SPIRVResult(sampledImage, typeSymbol, true);\n")
    spirv_intrinsics.write("};\n\n")

    spirv_type_converter_list = []
    spirv_type_construction = ''
    for converter in type_conversions:
        spirv_type_construction += f'SPIRVResult SPIRV_{converter.enum}(const Compiler* c, SPIRVGenerator* g, uint32_t vectorSize, SPIRVResult value) \n{{\n'
        spirv_type_construction += f'    if (value.isLiteral)\n'
        spirv_type_construction += f'    {{\n'
        spirv_type_construction += f'        assert(vectorSize == 1);\n'
        spirv_type_construction += f'        return GenerateConstantSPIRV(c, g, ConstantCreationInfo::{converter.target}(value.literalValue.{converter.source_data_type}));\n'
        spirv_type_construction += f'    }}\n'
        spirv_type_construction += f'    else\n'
        spirv_type_construction += f'    {{\n'
        spirv_type_construction += f'        uint32_t type;\n'
        spirv_type_construction += f'        if (vectorSize > 1)\n'
        spirv_type_construction += f'            type = GeneratePODTypeSPIRV(c, g, TypeCode::{converter.target}, vectorSize);\n'
        spirv_type_construction += f'        else\n'
        spirv_type_construction += f'            type = GeneratePODTypeSPIRV(c, g, TypeCode::{converter.target});\n'
        if not converter.spirv_conversion_arguments:
            spirv_type_construction += f'        value = LoadValueSPIRV(c, g, value);\n'
        if converter.spirv_conversion_prep:
            spirv_type_construction += converter.spirv_conversion_prep
        if not converter.spirv_conversion_arguments:
            spirv_type_construction += f'        uint32_t res = g->writer->MappedInstruction({converter.spirv_conversion_function}, SPVWriter::Section::LocalFunction, type, value);\n'
        else:
            spirv_type_construction += f'        uint32_t res = g->writer->MappedInstruction({converter.spirv_conversion_function}, SPVWriter::Section::LocalFunction, type, value, {converter.spirv_conversion_arguments});\n'
        spirv_type_construction += f'        return SPIRVResult(res, type, true);\n'
        spirv_type_construction += f'    }}\n'
        spirv_type_construction += f'}}\n\n'
        spirv_type_converter_list.append(f'    std::pair{{ TypeConversionTable::{converter.enum}, &SPIRV_{converter.enum} }}')

    
    spirv_intrinsics.write(spirv_type_construction[0:-2])
    spirv_intrinsics.write('\n\n')
    spirv_intrinsics.write('using SPIRVConversionFunction = SPIRVResult (*)(const Compiler*, SPIRVGenerator*, uint32_t, SPIRVResult);\n')
    spirv_intrinsics.write(f'constexpr StaticMap<TypeConversionTable, SPIRVConversionFunction, {len(spirv_type_converter_list)}> ConverterTable = {{\n')
    spirv_intrinsics.write(",\n".join(spirv_type_converter_list))
    spirv_intrinsics.write('\n};\n\n')

    #spirv_intrinsics.write('constexpr StaticMap default_intrinsics = std::array{\n')
    

    class IntrinsicPair:
        def __init__(self, decl_name, api_name):
            self.decl_name = decl_name
            self.api_name = api_name

    def fnv1a_64(s: str) -> int:
        hash = 0xcbf29ce484222325  # 64-bit offset basis
        fnv_prime = 0x100000001b3  # 64-bit FNV prime

        for c in s:
            hash ^= ord(c)
            hash = (hash * fnv_prime) % (1 << 64)  # simulate 64-bit unsigned wraparound

        return hash

    def IntrinsicSortKey(item):
        return fnv1a_64(item.api_name)
    
    def MemberSortKey(member):
        return fnv1a_64(member.decl_name) if member.api_name == None else fnv1a_64(member.api_name)
    intrinsic_list = []

    ### Built-in data types (Float, Int, UInt, Bool and their vector/matrix variants)
    header_file.write("#pragma once\n")
    header_file.write("//-------------------------------------------------\n")
    header_file.write("// *** Generated by type_gen.py. ***\n")
    header_file.write("//       DO NOT MODIFY!!!\n")
    header_file.write("//-------------------------------------------------\n")
    header_file.write('#include "ast/types/type.h"\n')
    #header_file.write('#include "ast/variable.h"\n')
    #header_file.write('#include "ast/function.h"\n')
    header_file.write('#include "ast/enumeration.h"\n')
    header_file.write('#include "ast/structure.h"\n')
    header_file.write('#include "ast/expressions/intexpression.h"\n')
    header_file.write('#include "ast/expressions/enumexpression.h"\n')
    header_file.write('namespace GPULang\n')
    header_file.write('{\n')
    
    header_file.write(conversion_table_enum)

    source_file.write("//-------------------------------------------------\n")
    source_file.write("// *** Generated by type_gen.py. ***\n")
    source_file.write("//       DO NOT MODIFY!!!\n")
    source_file.write("//-------------------------------------------------\n")
    source_file.write('#include "types.h"\n')
    source_file.write('namespace GPULang\n')
    source_file.write('{\n')

    spirv_intrinsic_list = []
    def spirv_intrinsic(fun, arg):
        spirv_intrinsic_code = f'SPIRVResult SPIRV_{fun}(const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args)\n'
        spirv_intrinsic_code += '{\n'
        spirv_intrinsic_code += f'{arg}'
        spirv_intrinsic_code += '}\n\n'
        #spirv_intrinsic_list.append(f'    &SPIRV_{fun} //')
        return spirv_intrinsic_code

    class Variable():
        def __init__(self, decl_name, api_name, type_name, pointer=False, uniform=False, mutable=False, literal=False, strict=False):
            self.decl_name = decl_name
            self.api_name = api_name
            self.type_name = type_name
            self.pointer = pointer
            self.uniform = uniform
            self.mutable = mutable
            self.literal = literal
            self.strict = strict

    class Function():
        def __init__(self, decl_name, api_name, return_type, parameters, documentation=None, compile_time=False, is_constructor=False, is_member=False):
            self.decl_name = decl_name
            self.api_name = api_name
            self.return_type = return_type
            self.parameters = parameters
            self.documentation = documentation
            self.compile_time = compile_time
            self.is_constructor = is_constructor
            self.is_member = is_member
            global function_counter
            self.index = function_counter
            function_counter += 1

            self.spirv = ''

        def declaration(self):
            if len(self.parameters) > 0:
                return_string = f'/// {self.api_name} with {", ".join([param.type_name for param in self.parameters])}\n'
            else:
                return_string = f'/// {self.api_name}\n'
            for param in self.parameters:
                return_string += f'extern Variable {param.decl_name};\n'
            return_string += f'extern Function {self.decl_name};\n'
            return_string += f'inline constexpr auto {self.decl_name}_name = "{self.api_name}"_c;\n'
            return_string += f'inline constexpr auto {self.decl_name}_backend_index = {self.index}u;\n'
            return_string += '\n'
            return return_string

        def definition(self):
            if len(self.parameters) > 0:
                return_string = f'/// {self.api_name} with {", ".join([param.type_name for param in self.parameters])}\n'
            else:
                return_string = f'/// {self.api_name}\n'
            for param in self.parameters:
                return_string += f'Variable {param.decl_name};\n'
                if param.pointer:
                    return_string += f'inline constexpr std::array {param.decl_name}_modifiers = {{Type::FullType::Modifier::Pointer}};\n'
                    return_string += f'inline constexpr std::array {param.decl_name}_modifierValues = {{(Expression*)nullptr}};\n'

            return_string += f'Function {self.decl_name};\n'
            if len(self.parameters) > 0:
                return_string += f'inline constexpr std::array {self.decl_name}_args = {{ {", ".join([f"&{param.decl_name}" for param in self.parameters])} }};\n'
            return_string += '\n'
            return return_string

        def setup(self):
            if len(self.parameters) > 0:
                return_string = f'    /// {self.api_name} with {", ".join([param.type_name for param in self.parameters])}\n'
            else:
                return_string = f'    /// {self.api_name}\n'
            for param in self.parameters:
                return_string += f'    {param.decl_name}.name = "{param.api_name}"_c;\n'
                return_string += f'    {param.decl_name}.type = Type::FullType{{ {param.type_name}Type.name }};\n'
                if param.literal:
                    return_string += f'    {param.decl_name}.type.literal = true;\n'
                if param.strict:
                    return_string += f'    {param.decl_name}.type.strict = true;\n'
                if param.mutable: 
                    return_string += f'    {param.decl_name}.type.mut = true;\n'
                if param.pointer:
                    return_string += f'    {param.decl_name}.type.modifiers = {param.decl_name}_modifiers;\n'
                    return_string += f'    {param.decl_name}.type.modifierValues = {param.decl_name}_modifierValues;\n'
            if self.documentation:
                return_string += f'    {self.decl_name}.documentation = "{self.documentation}"_c;\n'
            return_string += f'    {self.decl_name}.name = {self.decl_name}_name;\n'
            
            if self.compile_time:
                return_string += f'    {self.decl_name}.compileTime = true;\n'
            return_string += f'    {self.decl_name}.backendIndex = {self.index};\n'
            return_string += f'    {self.decl_name}.returnType = Type::FullType {{ {self.return_type}Type.name }};\n'
            if len(self.parameters) > 0:
                return_string += f'    {self.decl_name}.parameters = {self.decl_name}_args;\n'
            for param in self.parameters:
                return_string += f'    Symbol::Resolved(&{param.decl_name})->typeSymbol = &{param.type_name}Type;\n'
                if param.uniform:
                    return_string += f'    Symbol::Resolved(&{param.decl_name})->storage = Storage::Uniform;\n'

            return_type = self.return_type;
            if return_type in data_type_mapping:
                return_type = data_type_mapping[return_type]
            args = []
            argsWithNames = []
            for param in self.parameters:
                if param.type_name in data_type_mapping:
                    typeString = ''
                    if param.uniform:
                        typeString += 'uniform '
                    if param.pointer:
                        typeString += '*'
                    if param.literal:
                        typeString += 'literal '
                    if param.mutable:
                        typeString += 'mutable '
                    typeString += data_type_mapping[param.type_name]
                    args.append(typeString)
                    argsWithNames.append(f'{param.api_name} : {typeString}')
                else:
                    typeString = ''
                    if param.uniform:
                        typeString += 'uniform '
                    if param.pointer:
                        typeString += '*'
                    if param.literal:
                        typeString += 'literal '
                    if param.mutable:
                        typeString += 'mutable '
                    typeString += param.type_name
                    args.append(typeString)
                    argsWithNames.append(f'{param.api_name} : {typeString}')
            return_string += f'    Symbol::Resolved(&{self.decl_name})->signature = "{self.api_name}({",".join(args)}) {return_type}"_c;\n'
            return_string += f'    Symbol::Resolved(&{self.decl_name})->name = "{self.api_name}({",".join(args)})"_c;\n'
            return_string += f'    Symbol::Resolved(&{self.decl_name})->nameWithVarNames = "{self.api_name}({", ".join(argsWithNames)})"_c;\n'
            return_string += f'    Symbol::Resolved(&{self.decl_name})->returnTypeSymbol = &{self.return_type}Type;\n'
            return_string += '\n'
            return return_string

        def pair(self):
            return IntrinsicPair(
                decl_name=self.decl_name,
                api_name=self.api_name
            )
        
        def typed_pair(self):
            params = []
            for param in self.parameters:
                param_string = ''
                if param.uniform:
                    param_string += 'uniform '
                if param.pointer:
                    param_string += '*'
                if param.literal:
                    param_string += 'literal '
                if param.mutable:
                    param_string += 'mutable '
                
                if param.type_name in data_type_mapping:
                    param_string += data_type_mapping[param.type_name]
                else:
                    param_string += param.type_name

                params.append(param_string)
            return IntrinsicPair(
                decl_name=self.decl_name,
                api_name=f'{self.api_name}({",".join(params)})'
            )
        
        def spirv_code(self):
            spirv_intrinsic_code = f'SPIRVResult SPIRV_{self.decl_name}(const Compiler* c, SPIRVGenerator* g, uint32_t returnType, const std::vector<SPIRVResult>& args)\n'
            spirv_intrinsic_code += '{\n'
            spirv_intrinsic_code += f'{self.spirv}'
            spirv_intrinsic_code += '}\n'
            spirv_intrinsic_list.append({"code": f'    &SPIRV_{self.decl_name}', "idx": self.index})
            return spirv_intrinsic_code
        
    class ScalarType:
        def __init__(self, name, base_type, column_size, row_size):
            self.name = name
            self.base_type = base_type
            self.columnSize = column_size
            self.rowSize = row_size
            self.swizzles = []
            if self.columnSize == 2 and self.rowSize == 1:
                self.swizzles = generate_swizzle_two()
            elif self.columnSize == 3 and self.rowSize == 1:
                self.swizzles = generate_swizzle_three()
            elif self.columnSize == 4 and self.rowSize == 1:
                self.swizzles = generate_swizzle_four()

        def declaration(self):
            type_declaration = ''
            type_declaration += f'struct {self.name} : public Type\n'
            type_declaration += '{\n'
            type_declaration += f'    {self.name}();\n'
            type_declaration += '};\n'
            type_declaration += f'extern {self.name} {self.name}Type;\n\n'
            return type_declaration

        def definition(self, setup_string, lookup_list):
            for swizzle in self.swizzles:
                if len(swizzle) > 1:
                    swizzle_type = f'{self.base_type}x{len(swizzle)}'
                else:
                    swizzle_type = self.base_type
                lookup_list.append(IntrinsicPair(decl_name=f'{swizzle_type}Type', api_name=f'{swizzle}'))
            lookup_list.sort(key=IntrinsicSortKey)
            type_definition = ''
            type_definition = f'static constexpr StaticMap<HashString, Symbol*, {len(lookup_list)}> {self.name}_symbolLookup = {{\n'
            type_definition += ',\n'.join(f'    std::pair{{ "{item.api_name}"_h, &{item.decl_name} }}' for item in lookup_list)
            type_definition += f'\n    }};\n\n'
            type_definition += f'{self.name}::{self.name}()\n'
            type_definition += '{\n'
            type_definition += f'    this->name = "{data_type_mapping[self.name]}"_c;\n'
            type_definition += f'    this->columnSize = {self.columnSize};\n'
            type_definition += f'    this->rowSize = {self.rowSize};\n'
            type_definition += f'    this->byteSize = {trunc((bit_width_mapping[self.base_type] * self.columnSize * self.rowSize) / 8)};\n'
            type_definition += f'    this->category = Type::ScalarCategory;\n'
            type_definition += f'    this->baseType = TypeCode::{self.base_type};\n'
            type_definition += f'    this->builtin = true;\n'
            type_definition += f'\n'
            type_definition += f'{setup_string}'
            type_definition += f'    this->scope.symbolLookup = {self.name}_symbolLookup;\n'
            type_definition += '}\n'
            type_definition += f'{type_name} {type_name}Type;\n\n'

            return type_definition

    declaration_string = ""
    definition_string = ""
    functions = []
    type_list = []
    web_types = {
        "keywords": ["const", "var", "uniform", "mutable", "literal", "in", "out", "return", "break", "discard", "ray_ignore", "for", "while", "if", "else", "switch", "case", "struct", "render_state", "sampler_state", "program", "entry_point", "binding", "group", "enum", "generate", "#include", "#pragma", "declared", "workgroup", "alias", "true", "false"],
        "types": [],
        "builtin_functions": [],
    }
    for size in range(1, 5):
        for type in types:
            if size == 1:
                data_type_name = data_type_mapping[type]
                type_name = type
            else:
                type_name = f'{type}x{size}'
                data_type_name = f'{data_type_mapping[type]}x{size}'

            spirv_type_construction = ''
            builtin_type = ScalarType(name=type_name, base_type=type, column_size=size, row_size=1)
            declaration_string += builtin_type.declaration()
            web_types['types'].append(data_type_name)

            intrinsic_list.append(IntrinsicPair(decl_name=f'{type_name}Type', api_name=data_type_name))
            namer.names.append(NamerEntry(type_name, data_type_name))
            setup_string = ""
            member_functions = []

            # Conversions
            for type2 in types:

                if size == 1 and type2 == type:
                    continue

                # Don't allow conversion from bool to float types
                if type == 'Bool8' and type2.startswith('Float'):
                    continue

                if size == 1:
                    data_type_name2 = data_type_mapping[type2]
                    type_name2 = type2
                else:
                    type_name2 = f'{type2}x{size}'
                    data_type_name2 = f'{data_type_mapping[type2]}x{size}'

                function_name = f'{type_name}_from_{type_name2}'
                arg_name = f'{type_name}_from_{type_name2}_arg'

                fun = Function(
                    decl_name=function_name,
                    api_name=data_type_name,
                    return_type=type_name,
                    compile_time=True,
                    parameters=[Variable(decl_name=arg_name, api_name='val', type_name=type_name2)],
                    documentation=f'Convert from {data_type_name2} to {data_type_name}',
                    is_constructor=True
                )   
                if type == type2:
                    spirv_type_construction += spirv_intrinsic(function_name, '    return args[0];\n')
                    fun.spirv = '    return args[0];\n'
                else:
                    spirv_type_construction += spirv_intrinsic(function_name, f'    return ConverterTable[TypeConversionTable::{type2}To{type}](c, g, {size}, args[0]);\n')
                    fun.spirv = f'    return ConverterTable[TypeConversionTable::{type2}To{type}](c, g, {size}, args[0]);\n'
                functions.append(fun)

                if size > 1:
                    function_name = f'{type_name}_splat_{type2}'
                    arg_name = f'{type_name}_splat_{type2}_arg'

                    bit_width1 = bit_width_mapping[type]
                    bit_width2 = bit_width_mapping[type2]
                    #if type2 != 'Bool8' and bit_width1 != bit_width2:
                    #    continue

                    fun = Function(
                        decl_name=function_name,
                        api_name=data_type_name,
                        return_type=type_name,
                        parameters=[Variable(decl_name=arg_name, api_name='val', type_name=type2)],
                        compile_time=True,
                        documentation=f'Splat {data_type_mapping[type2]} to {data_type_name}',
                        is_constructor=True
                    )                    

                    spirv_function = '    SPIRVResult val = args[0];\n'
                    if type != type2: # if the types are different, convert since this is a splat conversion constructor
                        spirv_function += f'    if (val.isLiteral)\n'
                        spirv_function += f'        val = val.ConvertTo(TypeCode::{base_type_mapping[type_name]});\n'
                        spirv_function += f'    else\n'
                        spirv_function += f'        val = ConverterTable[TypeConversionTable::{type2}To{type}](c, g, 1, val);\n'
                    spirv_function += f'    return GenerateSplatCompositeSPIRV(c, g, returnType, {size}, val);\n'
                    spirv_type_construction += spirv_intrinsic(function_name, spirv_function)
                    fun.spirv = spirv_function
                    functions.append(fun)


            four_component_combinations = [[1, 1, 1, 1], [2, 1, 1], [1, 2, 1], [1, 1, 2], [2,2], [3, 1], [1,3]]
            three_component_combiations = [[1,1,1], [2, 1], [1, 2]]
            two_component_combinations = [[1, 1]]
            if size == 1:
                combinations = [[1]]
            elif size == 2:
                combinations = two_component_combinations
            elif size == 3:
                combinations = three_component_combiations
            elif size == 4:
                combinations = four_component_combinations
            for ctor_idx, comb in enumerate(combinations):
                function_name = f'{type_name}_ctor{ctor_idx}'

                list_entry_key = []
                args = []
                spirv_function =  "        SPIRVResult loadedArg = LoadValueSPIRV(c, g, args[0]);\n"
                spirv_function += "        if (returnType == loadedArg.typeName) {\n"
                spirv_function += "            return loadedArg;\n"
                spirv_function += "        }\n"
                if type != type2:
                    spirv_function += f'    loadedArg = GenerateConversionSPIRV(c, g, TypeConversionTable::{type2}To{type}, {size}, loadedArg);\n'
                spirv_function += '    return loadedArg;\n'

                fun = Function(
                    decl_name =function_name,
                    api_name = f'{data_type_mapping[type_name]}',
                    return_type = type_name,
                    compile_time = True,
                    parameters = [],
                    is_constructor=True
                )

                for arg_idx, s in enumerate(comb):
                    if s == 1:
                        arg_type_name = type
                    else:
                        arg_type_name = f'{type}x{s}'
                    arg_name = f'{function_name}_arg{arg_idx}_{arg_type_name}'
                    fun.parameters.append(Variable(decl_name=arg_name, api_name=f'arg{arg_idx}', type_name=arg_type_name))
                    args.append(arg_name)
                    if s == 1:
                        list_entry_key.append(f'{data_type_mapping[type]}')
                    else:
                        list_entry_key.append(f'{data_type_mapping[type]}x{s}')

                spirv_function = f'    SPIRVResult convertedArgs[{len(comb)}];\n'
                if size > 1:
                    spirv_function += f'    return GenerateCompositeSPIRV(c, g, returnType, args);\n'
                else:
                    spirv_function += f'    return args[0];\n'
                spirv_type_construction += spirv_intrinsic(function_name, spirv_function)
                fun.spirv = spirv_function
                functions.append(fun)

            #spirv_intrinsics.write(spirv_type_construction)
            
            for name, op, idx_type in zip(index_operator_names, index_operators, index_types):
                function_name = f'{type_name}_operator_{name}'
                arg_name = f'{type_name}_operator_{name}_arg'
                fun = Function(
                    decl_name=function_name,
                    api_name=f'operator{op}',
                    return_type=type,
                    parameters=[Variable(decl_name=arg_name, api_name='arg', type_name=idx_type)],
                    is_member=True
                )

                spirv_function = f'    g->typeState.storage = args[0].scope;\n'
                spirv_function += f'    SPIRVResult returnTypePtr = GeneratePointerTypeSPIRV(c, g, {function_name}.returnType, &{type}Type);\n'
                spirv_function += f'    g->typeState.storage = SPIRVResult::Storage::Function;\n'
                spirv_function += '    SPIRVResult index = LoadValueSPIRV(c, g, args[1]);\n'
                spirv_function += '    SPIRVResult ret = args[0];\n'
                spirv_function += '    ret.AddAccessChainLink({index});\n'
                spirv_function += '    ret.typeName = returnTypePtr.typeName;\n'
                spirv_function += '    ret.parentTypes = returnTypePtr.parentTypes;\n'
                spirv_function += '    ret.scope = args[0].scope;\n'
                spirv_function += '    ret.isValue = false;\n'
                spirv_function += '    return ret;\n'
                fun.spirv = spirv_function
                member_functions.append(fun)
                functions.append(fun)

            if type == 'Bool8':
                for name, op in zip(bool_operator_names, bool_operators):
                    function_name = f'{type_name}_operator_{name}_{type_name}'
                    arg_name = f'{function_name}_arg'
                    fun = Function(
                        decl_name=function_name,
                        api_name=f'operator{op}',
                        return_type=type_name,
                        parameters=[Variable(decl_name=arg_name, api_name='arg', type_name=type_name)],
                        is_member=True
                    )

                    spirv_function =  ''
                    spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                    spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'

                    if op == '||':
                        spirv_op = 'OpLogicalOr'
                    elif op == '&&':
                        spirv_op = 'OpLogicalAnd'
                    elif op == '==':
                        spirv_op = 'OpLogicalEqual'
                    elif op == '!=':
                        spirv_op = 'OpLogicalNotEqual'
                    spirv_function += f'    uint32_t ret = g->writer->MappedInstruction({spirv_op}, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                    spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                    fun.spirv = spirv_function
                    member_functions.append(fun)
                    functions.append(fun)

            else:
                operator_sets = [
                    zip(scalar_operator_names, scalar_operators), 
                    zip(assignment_operator_names, assignment_operators),
                ]
                for operator_set in operator_sets:
                    return_type = type_name
                    for name, op in operator_set:
                        function_name = f'{type_name}_operator_{name}_{type_name}'
                        arg_name = f'{function_name}_arg'
                        fun = Function(
                            decl_name=function_name,
                            api_name=f'operator{op}',
                            return_type=type_name,
                            parameters=[Variable(decl_name=arg_name, api_name='arg', type_name=type_name)],
                            is_member=True
                        )

                        spirv_function = ''
                        spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                        spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'

                        spirv_op_type = ''
                        if type.startswith('Float'):
                            spirv_op_type = 'F'
                        elif type.startswith('Int'):
                            if op == '/' or op == '/=' or op == '%' or op == '%=':
                                spirv_op_type = 'S'
                            else:
                                spirv_op_type = 'I'
                        elif type.startswith('UInt'):
                            if op == '/' or op == '/=' or op == '%' or op == '%=':
                                spirv_op_type = 'S'
                            else:
                                spirv_op_type = 'I'

                        if op == '+' or op == '+=':
                            spirv_op = f'Op{spirv_op_type}Add'
                        elif op == '-' or op == '-=':
                            spirv_op = f'Op{spirv_op_type}Sub'
                        elif op == '*' or op == '*=':
                            spirv_op = f'Op{spirv_op_type}Mul'
                        elif op == '/' or op == '/=':
                            spirv_op = f'Op{spirv_op_type}Div'
                        elif op == '%' or op == '%=':
                            spirv_op = f'Op{spirv_op_type}Mod'
                        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction({spirv_op}, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                        fun.spirv = spirv_function
                        member_functions.append(fun)
                        functions.append(fun)

                for name, op in zip(comparison_operator_names, comparison_operators):
                    function_name = f'{type_name}_operator_{name}_{type_name}'
                    arg_name = f'{function_name}_arg'
                    return_type = f'Bool8'
                    if size > 1:
                        return_type = f'Bool8x{size}'
                    fun = Function(
                        decl_name=function_name,
                        api_name=f'operator{op}',
                        return_type=return_type,
                        parameters=[Variable(decl_name=arg_name, api_name='arg', type_name=type_name)],
                        is_member=True
                    )

                    spirv_function = ''
                    spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                    spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'

                    spirv_op_type = ''
                    if type.startswith('Float'):
                        spirv_op_type = 'F'
                        if op == '<' or op == '>' or op == '<=' or op == '>=' or op == '==' or op == '!=':
                            spirv_op_type += 'Ord'
                    elif type.startswith('Int'):
                        if op == '<' or op == '>' or op == '<=' or op == '>=':
                            spirv_op_type = 'S'
                        else:
                            spirv_op_type = 'I'
                    elif type.startswith('UInt'):
                        if op == '<' or op == '>' or op == '<=' or op == '>=':
                            spirv_op_type = 'S'
                        else:
                            spirv_op_type = 'I'

                    if op == '<':
                        spirv_op = f'Op{spirv_op_type}LessThan'
                    elif op == '>':
                        spirv_op = f'Op{spirv_op_type}GreaterThan'
                    elif op == '<=':
                        spirv_op = f'Op{spirv_op_type}LessThanEqual'
                    elif op == '>=':
                        spirv_op = f'Op{spirv_op_type}GreaterThanEqual'
                    elif op == '==':
                        spirv_op = f'Op{spirv_op_type}Equal'
                    elif op == '!=':
                        spirv_op = f'Op{spirv_op_type}NotEqual'
                    spirv_function += f'    uint32_t ret = g->writer->MappedInstruction({spirv_op}, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                    spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                    fun.spirv = spirv_function
                    member_functions.append(fun)
                    functions.append(fun)
                
                if size > 1:
                    for name, op, scale_type in zip(scale_operator_names, scale_operators, scale_operator_types):
                        function_name = f'{type_name}_operator_{name}_{scale_type}'
                        arg_name = f'{function_name}_arg'

                        if bit_width_mapping[type] != bit_width_mapping[scale_type]:
                            continue

                        # If scale type is float, then we must convert the vector to float always
                        if scale_type.startswith('Float'):
                            scalar_return_type = scale_type
                            vector_result_type = f'{scale_type}x{size}'
                        else:
                            scalar_return_type = base_type_mapping[type_name]
                            vector_result_type = type_name
                        fun = Function(
                            decl_name=function_name,
                            api_name=f'operator{op}',
                            return_type=vector_result_type,
                            parameters=[Variable(decl_name=arg_name, api_name='arg', type_name=scale_type)],
                            is_member=True
                        )

                        spirv_function = ''

                        # If we scale to float, convert whatever is on the left to float
                        if scalar_return_type.startswith('Float'):
                            if type != scalar_return_type:
                                spirv_function += f'    SPIRVResult lhs = ConverterTable[TypeConversionTable::{type}To{scalar_return_type}](c, g, {size}, args[0]);\n'
                                spirv_function += f'    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'
                            elif scale_type != scalar_return_type:
                                spirv_function += f'    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                                spirv_function += f'    SPIRVResult rhs = ConverterTable[TypeConversionTable::{scale_type}To{scalar_return_type}](c, g, 1, args[1]);\n'
                            else:
                                spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                                spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'
                        else:
                            if type != scalar_return_type:
                                spirv_function += f'    SPIRVResult lhs = ConverterTable[TypeConversionTable::{type}To{scalar_return_type}](c, g, {size}, args[0]);\n'
                                spirv_function += f'    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'
                            elif scale_type != scalar_return_type:
                                spirv_function += f'    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                                spirv_function += f'    SPIRVResult rhs = ConverterTable[TypeConversionTable::{scale_type}To{scalar_return_type}](c, g, 1, args[1]);\n'
                            else:
                                spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                                spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'
                            spirv_function += f'    rhs = GenerateSplatCompositeSPIRV(c, g, returnType, {size}, rhs);\n'
                        if scalar_return_type.startswith('Float'):
                            spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpVectorTimesScalar, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                        else:
                            spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpIMul, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                        fun.spirv = spirv_function
                        member_functions.append(fun)
                        functions.append(fun)

            # Matrix transform
            if type.startswith('Float') and size > 1:
                for cols in range(2, 5):
                    compatible_matrix_type = f'Float32x{size}x{cols}'
                    return_type = f'{type}x{cols}'

                    
                    for name, op in zip(vector_matrix_operator_names, vector_matrix_operators):
                        function_name = f'{type_name}_operator_{name}_{compatible_matrix_type}'
                        arg_name = f'{function_name}_arg'

                        fun = Function(
                            decl_name=function_name,
                            api_name=f'operator{op}',
                            return_type=return_type,
                            parameters=[Variable(decl_name=arg_name, api_name='arg', type_name=compatible_matrix_type)],
                            is_member=True
                        )

                        spirv_function = ''
                        spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                        spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'
                        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpVectorTimesMatrix, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                        fun.spirv = spirv_function
                        member_functions.append(fun)
                        functions.append(fun)

            if type.startswith("UInt") or type.startswith("Int"):
                operator_sets = [
                    zip(bit_operator_names, bit_operators)
                    , zip(bit_assignment_operator_names, bit_assignment_operators)
                    ]
                for operator_set in operator_sets:
                    for name, op in operator_set:
                        function_name = f'{type_name}_operator_{name}_{type_name}'
                        arg_name = f'{function_name}_arg'
                        fun = Function(
                            decl_name=function_name,
                            api_name=f'operator{op}',
                            return_type=type_name,
                            parameters=[Variable(decl_name=arg_name, api_name='arg', type_name=type_name)],
                            is_member=True
                        )

                        if op == '&':
                            spirv_op = 'OpBitwiseAnd'
                        elif op == '|':
                            spirv_op = 'OpBitwiseOr'
                        elif op == '^':
                            spirv_op = 'OpBitwiseXor'
                        elif op == '<<':
                            spirv_op = 'OpShiftLeftLogical'
                        elif op == '>>':
                            spirv_op = 'OpShiftRightLogical'
                        spirv_function = ''
                        spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                        spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'
                        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction({spirv_op}, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                        fun.spirv = spirv_function
                        member_functions.append(fun)
                        functions.append(fun)


            pair_list = []
            for fun in member_functions:
                pair_list.append(fun.pair())
                pair_list.append(fun.typed_pair())
                declaration_string += fun.declaration()
                definition_string += fun.definition()
                setup_string += fun.setup()
            definition_string += builtin_type.definition(setup_string, pair_list)

    # Matrix types
    matrix_types = ['Float32', 'Float16']

    scalar_operator_names = ['add', 'sub', 'mul']
    scalar_operators = ['+', '-', '*']

    assignment_operator_names = ['addasg', 'subasg', 'mulasg']
    assignment_operators = ['+=', '-=', '*=']

    scale_operator_names = ['scale', 'scale']
    scale_operators = ['*', '*']
    scale_operator_types = ['Float32', 'Float16']

    transform_operator_names = ['transform']
    transform_operators = ['*']

    for type in matrix_types:
        for row_size in range(2, 5):
            for column_size in range(2, 5):

                member_functions = []
                type_name = f'{type}x{column_size}x{row_size}'
                data_type_name = f'{data_type_mapping[type]}x{column_size}x{row_size}'
                intrinsic_list.append(IntrinsicPair(decl_name=f'{type_name}Type', api_name=data_type_name))
                namer.names.append(NamerEntry(type_name, data_type_name))
                web_types['types'].append(data_type_name)
                setup_string = ""
                
                builtin_type = ScalarType(name=type_name, base_type=type, column_size=column_size, row_size=row_size)
                declaration_string += builtin_type.declaration()

                vector_ctor_name = f'{type_name}_{type}_{row_size}_ctor'

                fun = Function(
                    decl_name=vector_ctor_name,
                    api_name=f'{data_type_name}',
                    return_type=type_name,
                    compile_time = True,
                    parameters=[],
                    is_constructor=True
                )

                for arg_index in range(0, column_size):
                    fun.parameters.append(Variable(
                        decl_name=f'{vector_ctor_name}_arg{arg_index}',
                        api_name=f'arg{arg_index}',
                        type_name=f'{type}x{row_size}'
                    ))
                fun.spirv = f'    return GenerateCompositeSPIRV(c, g, returnType, args);\n'
                functions.append(fun)

                if column_size == row_size:
                    fun = Function(
                        decl_name=f'{type_name}_identity',
                        api_name=f'{data_type_name}',
                        return_type=type_name,
                        compile_time=True,
                        parameters=[],
                        is_constructor=True
                    )
                    spirv_function = ''
                    spirv_function += '    SPIRVResult zero = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(0));\n'
                    spirv_function += '    SPIRVResult one = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(1));\n'
                    spirv_function += f'    uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, {column_size});\n'
                    cols = []
                    for col in range(0, column_size):
                        if column_size == 2:
                            elements = ['zero', 'zero']
                        elif column_size == 3:
                            elements = ['zero', 'zero', 'zero']
                        elif column_size == 4:
                            elements = ['zero', 'zero', 'zero', 'zero']
                        elements[col] = 'one'
                        spirv_function += f'    SPIRVResult col{col} = GenerateCompositeSPIRV(c, g, vectorType, {{{", ".join(elements)}}});\n'
                        cols.append(f'col{col}')
                    spirv_function += f'    return GenerateCompositeSPIRV(c, g, returnType, {{{", ".join(cols)}}});\n'
                    fun.spirv = spirv_function
                    functions.append(fun)

                fun = Function(
                    decl_name=f'{type_name}_raw_list',
                    api_name=f'{data_type_name}',
                    return_type=type_name,
                    compile_time=True,
                    parameters=[],
                    is_constructor=True
                )
                for arg_index in range(0, column_size * row_size):
                    fun.parameters.append(Variable(
                        decl_name=f'{type_name}_raw_list_arg{arg_index}',
                        api_name=f'arg{arg_index}',
                        type_name=type
                    ))
                spirv_function = ''
                spirv_function += f'    uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, {column_size});\n'
                for col in range(0, column_size):
                    args = []
                    for row in range(0, row_size):
                        args.append(f'args[{row * column_size + col}]')
                    spirv_function += f'    SPIRVResult col{col} = GenerateCompositeSPIRV(c, g, vectorType, {{{", ".join(args)}}});\n'
                spirv_function += f'    return GenerateCompositeSPIRV(c, g, returnType, {{{", ".join([f"col{col}" for col in range(0, column_size)])}}});\n'
                fun.spirv = spirv_function
                functions.append(fun)


                vec_type = f'{type}x{column_size}'
                for name, op, idx_type in zip(index_operator_names, index_operators, index_types):
                    function_name = f'{type_name}_operator_{name}'
                    arg_name = f'{type_name}_operator_{name}_arg'
                    fun = Function(
                        decl_name=function_name,
                        api_name=f'operator{op}',
                        return_type=vec_type,
                        parameters=[Variable(decl_name=arg_name, api_name='idx', type_name=idx_type)],
                        is_member=True
                    )

                    spirv_function = f'    g->typeState.storage = args[0].scope;\n'
                    spirv_function += f'    SPIRVResult returnTypePtr = GeneratePointerTypeSPIRV(c, g, {function_name}.returnType, &{vec_type}Type);\n'
                    spirv_function += f'    g->typeState.storage = SPIRVResult::Storage::Function;\n'
                    spirv_function += '    SPIRVResult index = LoadValueSPIRV(c, g, args[1]);\n'
                    spirv_function += '    SPIRVResult ret = args[0];\n'
                    spirv_function += '    ret.AddAccessChainLink({index});\n'
                    spirv_function += '    ret.typeName = returnTypePtr.typeName;\n'
                    spirv_function += '    ret.parentTypes = returnTypePtr.parentTypes;\n'
                    spirv_function += '    ret.scope = args[0].scope;\n'
                    spirv_function += '    ret.isValue = false;\n'
                    spirv_function += '    return ret;\n'
                    fun.spirv = spirv_function
                    member_functions.append(fun)
                    functions.append(fun)

                for name, op in zip(transform_operator_names, transform_operators):
                    function_name = f'{type_name}_{name}_{type}x{column_size}'
                    arg_name = f'{function_name}_vec'

                    fun = Function(
                        decl_name=function_name,
                        api_name=f'operator{op}',
                        return_type=f'{type}x{column_size}',
                        parameters=[Variable(decl_name=arg_name, api_name='vec', type_name=f'{type}x{row_size}')],
                        is_member=True
                    )

                    spirv_function = ''
                    spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                    spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'
                    spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpMatrixTimesVector, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                    spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                    fun.spirv = spirv_function
                    member_functions.append(fun)
                    functions.append(fun)

                operator_sets = [
                    zip(scalar_operator_names, scalar_operators), 
                    zip(assignment_operator_names, assignment_operators)
                ]
                for operator_set in operator_sets:
                    for name, op in operator_set:
                        function_name = f'{type_name}_operator_{name}_{type_name}'
                        arg_name = f'{function_name}_arg'

                        fun = Function(
                            decl_name=function_name,
                            api_name=f'operator{op}',
                            return_type=type_name,
                            parameters=[Variable(decl_name=arg_name, api_name='arg', type_name=type_name)],
                            is_member=True
                        )

                        spirv_function = ''
                        spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                        spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'

                        if op == '+' or op == '+=':
                            spirv_function += f'    uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, {column_size});\n'
                            spirv_function += f'    TransientArray<SPVArg> intermediateArgs({column_size});\n'
                            spirv_function += f'    for (uint32_t i = 0; i < {column_size}; i++)\n'
                            spirv_function += '    {\n'
                            spirv_function += '        uint32_t lhsVec = g->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, vectorType, lhs, i);\n'
                            spirv_function += '        uint32_t rhsVec = g->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, vectorType, rhs, i);\n'
                            spirv_function += '        uint32_t res = g->writer->MappedInstruction(OpFAdd, SPVWriter::Section::LocalFunction, vectorType, SPVArg(lhsVec), SPVArg(rhsVec));\n'
                            spirv_function += '        intermediateArgs.Append(SPVArg(res));\n'
                            spirv_function += '    }\n'
                            spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpCompositeConstruct, SPVWriter::Section::LocalFunction, returnType, SPVArgList(intermediateArgs));\n'
                        elif op == '-' or op == '-=':
                            spirv_function += f'    uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float, {column_size});\n'
                            spirv_function += f'    TransientArray<SPVArg> intermediateArgs({column_size});\n'
                            spirv_function += f'    for (uint32_t i = 0; i < {column_size}; i++)\n'
                            spirv_function += '    {\n'
                            spirv_function += '        uint32_t lhsVec = g->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, vectorType, lhs, i);\n'
                            spirv_function += '        uint32_t rhsVec = g->writer->MappedInstruction(OpCompositeExtract, SPVWriter::Section::LocalFunction, vectorType, rhs, i);\n'
                            spirv_function += '        uint32_t res = g->writer->MappedInstruction(OpFSub, SPVWriter::Section::LocalFunction, vectorType, SPVArg(lhsVec), SPVArg(rhsVec));\n'
                            spirv_function += '        intermediateArgs.Append(SPVArg(res));\n'
                            spirv_function += '    }\n'
                            spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpCompositeConstruct, SPVWriter::Section::LocalFunction, returnType, SPVArgList(intermediateArgs));\n'
                        elif op == '*' or op == '*=':
                            spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpMatrixTimesMatrix, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                        fun.spirv = spirv_function
                        member_functions.append(fun)
                        functions.append(fun)

                op = '*'
                function_name = f'{type_name}_operator_scale_{type}'
                arg_name = f'{function_name}_arg'
                fun = Function(
                    decl_name=function_name,
                    api_name=f'operator{op}',
                    return_type=type_name,
                    parameters=[Variable(decl_name=arg_name, api_name='arg', type_name=type)],
                    is_member=True
                )

                spirv_function = ''
                spirv_function += '    SPIRVResult lhs = LoadValueSPIRV(c, g, args[0]);\n'
                spirv_function += '    SPIRVResult rhs = LoadValueSPIRV(c, g, args[1]);\n'
                spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpMatrixTimesScalar, SPVWriter::Section::LocalFunction, returnType, lhs, rhs);\n'
                spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                fun.spirv = spirv_function
                member_functions.append(fun)
                functions.append(fun)

                pair_list = []
                for fun in member_functions:
                    pair_list.append(fun.pair())
                    pair_list.append(fun.typed_pair())
                    declaration_string += fun.declaration()
                    definition_string += fun.definition()
                    setup_string += fun.setup()
                definition_string += builtin_type.definition(setup_string, pair_list)

    class Type:
        def __init__(self, name, category=None, base_type = None, api_name=None):
            self.name = name
            self.category = category
            self.baseType = base_type
            self.api_name = api_name

        def declaration(self):
            class_decl = ''
            class_decl += f'struct {self.name} : public Type\n'
            class_decl += '{\n'
            class_decl += f'    {self.name}();\n'
            class_decl += '};\n'
            class_decl += f'extern {self.name} {self.name}Type;\n\n'
            return class_decl

        def definition(self):
            class_def = ''
            class_def += f'{self.name}::{self.name}()\n'
            class_def += '{\n'
            name = self.name[0].lower() + self.name[1:] if self.api_name is None else self.api_name
            namer.names.append(NamerEntry(self.name, name))
            class_def += f'    this->name = "{name}"_c;\n'
            #class_def += f'    this->symbolTyope = "Symbol::SymbolType::TypeType"_c;\n'
            if self.category:
                class_def += f'    this->category = Type::{self.category};\n'
            if self.baseType:
                class_def += f'    this->baseType = TypeCode::{self.baseType};\n'
            class_def += '    this->builtin = true;\n'
            class_def += '};\n'
            class_def += f'{self.name} {self.name}Type;\n\n'
            return class_def

        def pair(self):
            return IntrinsicPair(decl_name=f'{self.name}Type', api_name=self.name[0].lower() + self.name[1:] if self.api_name is None else self.api_name)
        
        def web_type(self):
            return self.name

    # Texture types
    texture_dimensions = ['1D', '2D', '3D', 'Cube']
    texture_multisampling = ['', 'MS', '', '']
    texture_array = ['Array', 'Array', '', 'Array']
    combinedSampler = ['', 'Sampled'];
    for sampled in combinedSampler:
        for dim, ms, array in zip(texture_dimensions, texture_multisampling, texture_array):

            type = Type(f'{sampled}Texture{dim}', f'{sampled}TextureCategory', f'{sampled}Texture{dim}', f'texture{sampled}{dim}')
            data_type_mapping[type.name] = type.api_name
            type_list.append(type)

            if ms : 
                type = Type(f'{sampled}Texture{dim}MS', f'{sampled}TextureCategory', f'{sampled}Texture{dim}', f'texture{sampled}{dim}MS')
                data_type_mapping[type.name] = type.api_name
                type_list.append(type)

                if array:
                    type = Type(f'{sampled}Texture{dim}MSArray', f'{sampled}TextureCategory', f'{sampled}Texture{dim}', f'texture{sampled}{dim}MSArray')
                    data_type_mapping[type.name] = type.api_name
                    type_list.append(type)

            if array:
                type = Type(f'{sampled}Texture{dim}Array', f'{sampled}TextureCategory', f'{sampled}Texture{dim}', f'texture{sampled}{dim}Array')
                data_type_mapping[type.name] = type.api_name
                type_list.append(type)

    # Pixel cache types
    type = Type('PixelCache', 'PixelCacheCategory', 'PixelCache', 'pixelCache')
    data_type_mapping[type.name] = type.api_name
    type_list.append(type)

    type = Type('PixelCacheMS', 'PixelCacheCategory', 'PixelCacheMS', 'pixelCacheMS')
    data_type_mapping[type.name] = type.api_name
    type_list.append(type)

    type = Type('Sampler', 'SamplerCategory', 'Sampler', 'sampler')
    data_type_mapping[type.name] = type.api_name
    type_list.append(type)

    type = Type('FunctionPtr', api_name='Function')
    type_list.append(type)

    type = Type('AccelerationStructure', 'AccelerationStructureCategory', 'AccelerationStructure', 'accelerationStructure')
    data_type_mapping[type.name] = type.api_name
    type_list.append(type)

    type = Type('Void', 'VoidCategory', 'Void', 'void')
    data_type_mapping[type.name] = type.api_name
    type_list.append(type)

    class EnumMember:
        def __init__(self, decl_name, api_name=None, value=None):
            self.decl_name = decl_name
            self.api_name = api_name
            self.value = value

    class Enumeration:
        def __init__(self, name, type_name, members=[]):
            self.name = name
            self.type_name = type_name
            self.members = members

        def declaration(self):
            decl = f'struct {self.name} : public Enumeration\n'
            decl += '{\n'
            decl += f'    {self.name}();\n'
            decl += '};\n'
            decl += f'extern {self.name} {self.name}Type;\n\n'

            value = 0
            for member in self.members:
                value = member.value if member.value is not None else value 
                decl += f'inline constexpr auto {self.name}{member.decl_name}_value = {hex(value)};\n'
                decl += f'extern EnumExpression {self.name}{member.decl_name};\n'
                value = value + 1
            return decl
        
        def definition(self):
            defn = ''
            for i, member in enumerate(self.members):
                defn += f'EnumExpression {self.name}{member.decl_name};\n'
            defn += f'{self.name}::{self.name}()\n'
            defn += '{\n'
            defn += f'    this->name = "{self.name}"_c;\n'
            defn += '    this->category = Type::EnumCategory;\n'
            defn += f'    this->type = Type::FullType{{ {self.type_name}Type.name }};\n'
            defn += f'    this->thisResolved->typeSymbol = &{self.type_name}Type;\n'
            defn += '    this->baseType = TypeCode::UInt;\n'
            defn += '    this->type.literal = true;\n'
            defn += '    this->builtin = true;\n'


            labels = []
            for i, member in enumerate(self.members):
                defn += f'    {self.name}{member.decl_name}.value = {self.name}{member.decl_name}_value;\n'
                defn += f'    {self.name}{member.decl_name}.type = Type::FullType{{ {self.name}Type.name, true }};\n'
                defn += f'    {self.name}{member.decl_name}.underlyingType = Type::FullType{{ {self.type_name}Type.name }};\n'
                defn += f'    {self.name}{member.decl_name}.thisResolved->type = this;\n'
                labels.append( f'"{member.decl_name if member.api_name is None else member.api_name}"_c' )

            defn += f'    this->labels = std::array{{ {", ".join(labels)} }};\n'


            defn += '    this->fromUnderlyingType.name = this->name;\n'
            defn += '    this->fromUnderlyingType.returnType = Type::FullType{{ this->name }};\n'
            defn += '    this->fromUnderlyingType.compileTime = true;\n'
            defn += '    this->fromUnderlyingType.constructorType = this;\n'
            defn += '    this->fromUnderlyingType.parameters = { &this->fromUnderlyingTypeArg };\n'
            defn += '    this->fromUnderlyingTypeArg.name = "arg"_c;\n'
            defn += '    this->fromUnderlyingTypeArg.type = this->type;\n'
            defn += '    Symbol::Resolved(&this->fromUnderlyingTypeArg)->typeSymbol = this->thisResolved->typeSymbol;\n'
            defn += '    Symbol::Resolved(&this->fromUnderlyingType)->returnTypeSymbol = this;\n'

            defn += '    this->toUnderlyingType.name = this->type.name;\n'
            defn += '    this->toUnderlyingType.returnType = this->type;\n'
            defn += '    this->toUnderlyingType.compileTime = true;\n'
            defn += '    this->toUnderlyingType.constructorType = this;\n'
            defn += '    this->toUnderlyingType.parameters = { &this->toUnderlyingTypeArg };\n'
            defn += '    this->toUnderlyingTypeArg.name = "arg"_c;\n'
            defn += '    this->toUnderlyingTypeArg.type = Type::FullType{{ this->name }};\n'
            defn += '    Symbol::Resolved(&this->toUnderlyingTypeArg)->typeSymbol = this;\n'
            defn += '    Symbol::Resolved(&this->toUnderlyingType)->returnTypeSymbol = this->thisResolved->typeSymbol;\n'

            defn += '    this->eqOp.name = "operator=="_c;\n'
            defn += '    this->eqOp.returnType = Type::FullType{{ "Bool8"_c }};\n'
            defn += '    this->eqOpArg.name = "rhs"_c;\n'
            defn += '    this->eqOpArg.type = Type::FullType{{ this->name }};\n'
            defn += '    Symbol::Resolved(&this->eqOpArg)->typeSymbol = this;\n'
            defn += '    Symbol::Resolved(&this->eqOp)->returnTypeSymbol = &Bool8Type;\n'

            defn += '    this->neqOp.name = "operator!="_c;\n'
            defn += '    this->neqOp.returnType = Type::FullType{{ "Bool8"_c }};\n'
            defn += '    this->neqOpArg.name = "rhs"_c;\n'
            defn += '    this->neqOpArg.type = Type::FullType{{ this->name }};\n'
            defn += '    Symbol::Resolved(&this->neqOpArg)->typeSymbol = this;\n'
            defn += '    Symbol::Resolved(&this->neqOp)->returnTypeSymbol = &Bool8Type;\n'

            self.members.append(EnumMember(decl_name='eqOp', api_name='operator==', value=None))
            self.members.append(EnumMember(decl_name='neqOp', api_name='operator!=', value=None))

            
            self.members.sort(key=MemberSortKey)
            defn += f'    this->scope.symbolLookup = StaticMap<HashString, Symbol*, {len(self.members)}> {{\n'        
            defn += ',\n'.join(f'        std::pair{{ "{member.decl_name if member.api_name == None else member.api_name}"_h, &{(self.name + member.decl_name) if member.api_name == None else self.name + 'Type.' + member.decl_name} }}' for member in self.members)
            defn += '\n    };\n'
            defn += '};\n'
            defn += f'{self.name} {self.name}Type;\n\n'
            return defn

        def pairs(self):
            return [
                IntrinsicPair(decl_name=f'{self.name}Type', api_name=self.name),
                IntrinsicPair(decl_name=f'{self.name}Type.toUnderlyingType', api_name=f'{self.name}({data_type_mapping[self.type_name]})'),
                IntrinsicPair(decl_name=f'{self.name}Type.fromUnderlyingType', api_name=f'{data_type_mapping[self.type_name]}({self.name})')
                # We could add the underlying type name pair here without argument so it shows up in overload suggestions, like below
                # IntrinsicPair(decl_name=f'{self.name}Type.fromUnderlyingType', api_name={data_type_mapping[self.type_name]})
            ]
        
        def web_type(self):
            return self.name;

    enums = []

    enum = Enumeration(
        name = "CompareMode",
        type_name = "UInt32",
        members=[
            EnumMember("Less"),
            EnumMember("LessEqual"),
            EnumMember("Greater"),
            EnumMember("GreaterEqual"),
            EnumMember("Equal"),
            EnumMember("NotEqual"),
            EnumMember("Always"),
            EnumMember("Never")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = "StencilOp",
        type_name = "UInt32",
        members=[
            EnumMember("Keep"),
            EnumMember("Zero"),
            EnumMember("Replace"),
            EnumMember("Increment"),
            EnumMember("Decrement"),
            EnumMember("Invert"),
            EnumMember("IncrementWrap"),
            EnumMember("DecrementWrap")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'ExecutionScope',
        type_name = 'UInt32',
        members=[
            EnumMember("Global"),
            EnumMember("Device"),
            EnumMember("Workgroup"),
            EnumMember("Subgroup"),
            EnumMember("Invocation"),
            EnumMember("Queue")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'MemorySemantics',
        type_name = 'UInt32',
        members=[
            EnumMember(decl_name="Acquire", value=0x1),
            EnumMember(decl_name="Release", value=0x2),
            EnumMember(decl_name="AcquireRelease", value=0x4),
            EnumMember(decl_name="SequentiallyConsistent", value=0x8),
            EnumMember(decl_name="Relaxed", value=0x10)
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'RasterizationMode',
        type_name = 'UInt32',
        members=[
            EnumMember("Fill"),
            EnumMember("Line"),
            EnumMember("Point")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'CullFace',
        type_name = 'UInt32',
        members=[
            EnumMember("None"),
            EnumMember("Front"),
            EnumMember("Back"),
            EnumMember("FrontAndBack")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'WindingOrder',
        type_name = 'UInt32',
        members=[
            EnumMember("Clockwise"),
            EnumMember("CounterClockwise")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'LogicOp',
        type_name = 'UInt32',
        members=[
            EnumMember("And"),
            EnumMember("AndInverted"),
            EnumMember("AndReverse"),
            EnumMember("Clear"),
            EnumMember("Copy"),
            EnumMember("CopyInverted"),
            EnumMember("Equivalence"),
            EnumMember("Invert"),
            EnumMember("Nand"),
            EnumMember("No"),
            EnumMember("Nor"),
            EnumMember("Or"),
            EnumMember("OrInverted"),
            EnumMember("OrReverse"),
            EnumMember("Set"),
            EnumMember("Xor")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'BlendFactor',
        type_name = 'UInt32',
        members=[
            EnumMember("Zero"),
            EnumMember("One"),
            EnumMember("SourceColor"),
            EnumMember("OneMinusSourceColor"),
            EnumMember("SourceAlpha"),
            EnumMember("OneMinusSourceAlpha"),
            EnumMember("DestinationColor"),
            EnumMember("OneMinusDestinationColor"),
            EnumMember("DestinationAlpha"),
            EnumMember("OneMinusDestinationAlpha"),
            EnumMember("ConstantColor"),
            EnumMember("OneMinusConstantColor"),
            EnumMember("ConstantAlpha"),
            EnumMember("OneMinusConstantAlpha")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'BlendOperation',
        type_name = 'UInt32',
        members=[
            EnumMember("Add"),
            EnumMember("Subtract"),
            EnumMember("ReverseSubtract"),
            EnumMember("Min"),
            EnumMember("Max")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'BlendColorMask',
        type_name = 'UInt32',
        members=[
            EnumMember("None"),
            EnumMember("R"),
            EnumMember("RG"),
            EnumMember("RGB"),
            EnumMember("RGBA")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'FilterMode',
        type_name = 'UInt32',
        members=[
            EnumMember("Point", value = 0),
            EnumMember("Nearest", value = 0),
            EnumMember("Linear", value = 1),
            EnumMember("MinMagMipLinear", value = 0x1 | (0x1 << 1) | (0x1) << 2),
            EnumMember("MinMagLinearMipPoint", value = 0x1 | (0x1 << 1) | (0x0) << 2),
            EnumMember("MinLinearMagMipPoint", value = 0x1 | (0x0 << 1) | (0x0) << 2),
            EnumMember("MinMagMipPoint", value = 0x0 | (0x0 << 1) | (0x0) << 2),
            EnumMember("MinMagPointMipLinear", value = 0x0 | (0x0 << 1) | (0x1) << 2),
            EnumMember("MinPointMagMipLinear", value = 0x0 | (0x1 << 1) | (0x1) << 2),
            EnumMember("MinLinearMagPointMipLinear", value = 0x1 | (0x0 << 1) | (0x1) << 2),
            EnumMember("MinPointMagLinearMipPoint", value = 0x0 | (0x1 << 1) | (0x0) << 2)
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'AddressMode',
        type_name = 'UInt32',
        members=[
            EnumMember("Repeat"),
            EnumMember("Mirror"),
            EnumMember("Clamp"),
            EnumMember("Border")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'Color',
        type_name = 'UInt32',
        members=[
            EnumMember("Transparent"),
            EnumMember("Black"),
            EnumMember("White")
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'InputTopology',
        type_name = 'UInt32',
        members=[
            EnumMember("Invalid"),
            EnumMember("Points"),
            EnumMember("Lines"),
            EnumMember("LinesAdjacency"),
            EnumMember("Triangles"),
            EnumMember("TrianglesAdjacency"),
            EnumMember("Quads"),
            EnumMember("Isolines"),
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'OutputTopology',
        type_name = 'UInt32',
        members=[
            EnumMember("Invalid"),
            EnumMember("Points"),
            EnumMember("Lines"),
            EnumMember("Triangles"),
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'Patch',
        type_name = 'UInt32',
        members=[
            EnumMember("Invalid"),
            EnumMember("Isolines"),
            EnumMember("Triangles"),
            EnumMember("Quads"),
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'Winding',
        type_name = 'UInt32',
        members=[
            EnumMember("Invalid"),
            EnumMember("Clockwise"),
            EnumMember("CounterClockwise"),
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'Partition',
        type_name = 'UInt32',
        members=[
            EnumMember("Invalid"),
            EnumMember("IntegerSteps"),
            EnumMember("FractionalEven"),
            EnumMember("FractionalOdd"),
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'PixelOrigin',
        type_name = 'UInt32',
        members=[
            EnumMember("Invalid"),
            EnumMember("LowerLeft"),
            EnumMember("UpperLeft"),
            EnumMember("Center"),
        ]
    )
    enums.append(enum)

    enum = Enumeration(
        name = 'ComputeDerivatives',
        type_name = 'UInt32',
        members=[
            EnumMember("Invalid"),
            EnumMember("IndexLinear"),
            EnumMember("IndexQuad"),
        ]
    )
    enums.append(enum)

    class StructMember:
        def __init__(self, decl_name, api_name, type, array_size=1):
            self.decl_name = decl_name
            self.api_name = api_name
            self.type = type
            self.array_size = array_size

    class Struct:
        def __init__(self, name, members=[]):
            self.name = name
            self.members = members

        def declaration(self):
            decl = f'struct {self.name} : public Structure\n'
            decl += '{\n'
            decl += f'    {self.name}();\n'
            decl += '};\n'
            decl += f'extern {self.name} {self.name}Type;\n\n'

            for member in self.members:
                decl += f'extern Variable {self.name}{member.decl_name};\n'
                if member.array_size > 1:
                    decl += f'extern IntExpression {self.name}{member.decl_name}ArraySize;\n'
            return decl
        
            return decl
        
        def definition(self):
            defn = ''
            for i, member in enumerate(self.members):
                defn += f'Variable {self.name}{member.decl_name};\n'
                if member.array_size > 1:
                    defn += f'IntExpression {self.name}{member.decl_name}ArraySize({member.array_size});\n'

            byte_size = 0
            offsets = []
            for member in self.members:
                base_type = base_type_mapping[member.type]
                row_count = vector_size_mapping[member.type]
                offsets.append(byte_size)
                byte_size += bit_width_mapping[base_type] * row_count * member.array_size / 8
            defn += f'{self.name}::{self.name}()\n'
            defn += '{\n'
            defn += f'    this->name = "{self.name}"_c;\n'
            defn += f'    this->resolved = Alloc<Structure::__Resolved>();\n'
            defn += '    this->category = Type::StructureCategory;\n'
            defn += '    this->arraySizeExpression = nullptr;\n'
            defn += '    this->isArray = false;\n'
            defn += '    this->scope.owningSymbol = this;\n'
            defn += '    this->scope.type = Scope::ScopeType::Type;\n'
            defn += '    this->baseType = TypeCode::InvalidType;\n'
            defn += '    this->builtin = true;\n'
            defn += '    Structure::__Resolved* typeResolved = static_cast<Structure::__Resolved*>(this->resolved);\n'
            defn += f'    typeResolved->usageFlags.bits = 0x0;\n'
            defn += f'    typeResolved->accessBits.bits = 0x0;\n'
            defn += f'    typeResolved->byteSize = {trunc(byte_size)};\n'
            
            defn += f'    typeResolved->baseAlignment = 0;\n'
            defn += f'    typeResolved->packMembers = false;\n'

            size = 0
            for i, member in enumerate(self.members):
                defn += f'    {self.name}{member.decl_name}.name = "{member.api_name}"_c;\n'
                if member.array_size > 1:
                    defn += f'    {self.name}{member.decl_name}.type = Type::FullType{{ {member.type}Type.name, {{Type::FullType::Modifier::Array}}, {{&{self.name}{member.decl_name}ArraySize}} }};\n'
                else:
                    defn += f'    {self.name}{member.decl_name}.type = Type::FullType{{ {member.type}Type.name }};\n'
                defn += f'    {self.name}{member.decl_name}.thisResolved->typeSymbol = &{member.type}Type;\n'
                defn += f'    {self.name}{member.decl_name}.thisResolved->structureOffset = {trunc(offsets[i])};\n'

            defn += f'    this->symbols = TransientArray<Symbol*>({{ {", ".join(f"&{self.name}{member.decl_name}" for member in self.members)} }});\n'
            self.members.sort(key=MemberSortKey)
            defn += f'    this->scope.symbolLookup = StaticMap<HashString, Symbol*, {len(self.members)}> {{\n'        
            defn += ',\n'.join(f'        std::pair{{ "{member.api_name}"_h, &{self.name + member.decl_name} }}' for member in self.members)
            defn += '\n    };\n'
            defn += '};\n'
            defn += f'{self.name} {self.name}Type;\n\n'
            return defn

        def pairs(self):
            return [
                IntrinsicPair(decl_name=f'{self.name}Type', api_name=self.name),
            ]
        
        def web_type(self):
            return self.name;

    structs = []

    def geometry_polygon(num):
        return[ StructMember('Position', 'position', 'Float32x4', num),
            StructMember('PointSize', 'pointSize', 'Float32', num),
            StructMember('CullDistance', 'cullDistance', 'Float32', num),
            StructMember('ClipDistance', 'clipDistance', 'Float32', num)
        ]


    struct = Struct(
        name = 'GeometryPoint',
        members= geometry_polygon(1)
    )
    structs.append(struct)

    struct = Struct(
        name = 'GeometryLine',
        members=geometry_polygon(2)
    )
    structs.append(struct)

    struct = Struct(
        name = 'GeometryLineAdjacency',
        members=geometry_polygon(4)
    )
    structs.append(struct)

    struct = Struct(
        name = 'GeometryTriangle',
        members=geometry_polygon(3)
    )
    structs.append(struct)

    struct = Struct(
        name = 'GeometryTriangleAdjacency',
        members=geometry_polygon(6)
    )
    structs.append(struct)

    class StateMember:
        def __init__(self, name, data_type, array_size=1):
            self.name = name
            self.data_type = data_type
            self.array_size = array_size

    class State:
        def __init__(self, name, members):
            self.name = name
            self.members = members

        def declaration(self):
            decl = ''
            for member in self.members:
                decl += f'extern Variable {self.name}{member.name};\n'
                if member.array_size > 1:
                    decl += f'extern IntExpression {self.name}{member.name}ArraySize;\n'
            decl += f'struct {self.name} : public Type\n'
            decl += '{\n'
            decl += f'    {self.name}();\n'
            decl += '};\n'
            decl += f'extern {self.name} {self.name}Type;\n\n'
            return decl

        def definition(self):
            defn = ''
            for member in self.members:
                defn += f'Variable {self.name}{member.name};\n'
                if member.array_size > 1:
                    defn += f'IntExpression {self.name}{member.name}ArraySize({member.array_size});\n'

            defn += f'{self.name}::{self.name}()\n'
            defn += '{\n'
            defn += f'    this->name = "{self.name}"_c;\n'
            defn += '    this->builtin = true;\n'
            for member in self.members:
                defn += f'    {self.name}{member.name}.name = "{member.name}"_c;\n'
                if member.array_size > 1:
                    defn += f'    {self.name}{member.name}.type = Type::FullType{{ {member.data_type}Type.name, {{Type::FullType::Modifier::Array}}, {{&{self.name}{member.name}ArraySize}} }};\n'
                else:
                    defn += f'    {self.name}{member.name}.type = Type::FullType{{ {member.data_type}Type.name }};\n'
                defn += f'    Symbol::Resolved(&{self.name}{member.name})->typeSymbol = &{member.data_type}Type;\n\n'

            def MemberSortKey(member):
                return fnv1a_64(member.name )
            self.members.sort(key=MemberSortKey)
            defn += f'    this->scope.symbolLookup = StaticMap<HashString, Symbol*, {len(self.members)}>{{\n'
            defn += ',\n'.join(f'        std::pair{{ "{member.name}"_h, &{self.name}{member.name} }}' for member in self.members)
            defn += '\n    };\n'
            defn += '};\n'
            defn += f'{self.name} {self.name}Type;\n\n'

            return defn
        
        def web_type(self):
            return self.name

    states = []
    state = State(
        name = 'StencilState',
        members=[
            StateMember('Fail', 'StencilOp'),
            StateMember('Pass', 'StencilOp'),
            StateMember('DepthFail', 'StencilOp'),
            StateMember('CompareFunction', 'CompareMode'),
            StateMember('CompareMask', 'UInt32'),
            StateMember('WriteMask', 'UInt32'),
            StateMember('Reference', 'UInt32')
        ]
    )
    states.append(state)

    state = State(
        name = 'RenderState',
        members=[
            StateMember('DepthClampEnabled', 'Bool8'),
            StateMember('DepthTestEnabled', 'Bool8'),
            StateMember('DepthWriteEnabled', 'Bool8'),
            StateMember('DepthBiasEnabled', 'Bool8'),
            StateMember('DepthBiasFactor', 'Float32'),
            StateMember('DepthBiasClamp', 'Float32'),
            StateMember('DepthBiasSlopeFactor', 'Float32'),
            StateMember('DepthTestFunction', 'CompareMode'),
            StateMember('DepthBoundsTestEnabled', 'Bool8'),
            StateMember('DepthBoundsMin', 'Float32'),
            StateMember('DepthBoundsMax', 'Float32'),
            StateMember('NoRasterization', 'Bool8'),
            StateMember('Rasterize', 'RasterizationMode'),
            StateMember('Cull', 'CullFace'),
            StateMember('WindingOrder', 'WindingOrder'),
            StateMember('ScissorEnabled', 'Bool8'),
            StateMember('StencilEnabled', 'Bool8'),
            StateMember('StencilFront', 'StencilState'),
            StateMember('StencilBack', 'StencilState'),
            StateMember('LogicOpEnabled', 'Bool8'),
            StateMember('LogicOp', 'LogicOp'),
            StateMember('BlendEnabled', 'Bool8', 8),
            StateMember('SourceBlend', 'BlendFactor', 8),
            StateMember('DestinationBlend', 'BlendFactor', 8),
            StateMember('SourceAlphaBlend', 'BlendFactor', 8),
            StateMember('DestinationAlphaBlend', 'BlendFactor', 8),
            StateMember('ColorBlendOp', 'BlendOperation', 8),
            StateMember('AlphaBlendOp', 'BlendOperation', 8),
            StateMember('Mask', 'BlendColorMask', 8)
        ]
    )
    states.append(state)

    state = State(
        name = 'SamplerState',
        members=[
            StateMember('Address', 'AddressMode'),
            StateMember('AddressU', 'AddressMode'),
            StateMember('AddressV', 'AddressMode'),
            StateMember('AddressW', 'AddressMode'),
            StateMember('Filter', 'FilterMode'),
            StateMember('MinFilter', 'FilterMode'),
            StateMember('MagFilter', 'FilterMode'),
            StateMember('MipFilter', 'FilterMode'),
            StateMember('MipLodBias', 'Float32'),
            StateMember('AnisotropyEnabled', 'Bool8'),
            StateMember('MaxAnisotropy', 'UInt32'),
            StateMember('CompareEnabled', 'Bool8'),
            StateMember('CompareFunction', 'CompareMode'),
            StateMember('MinLod', 'Float32'),
            StateMember('MaxLod', 'Float32'),
            StateMember('Border', 'Color'),
            StateMember('UnnormalizedSamplingEnabled', 'Bool8')
        ]
    )
    states.append(state)

    state = State(
        name = 'Program',
        members=[
            StateMember('VertexShader', 'FunctionPtr'),
            StateMember('HullShader', 'FunctionPtr'),
            StateMember('DomainShader', 'FunctionPtr'),
            StateMember('GeometryShader', 'FunctionPtr'),
            StateMember('PixelShader', 'FunctionPtr'),
            StateMember('ComputeShader', 'FunctionPtr'),
            StateMember('TaskShader', 'FunctionPtr'),
            StateMember('MeshShader', 'FunctionPtr'),
            StateMember('RayGenerationShader', 'FunctionPtr'),
            StateMember('RayAnyHitShader', 'FunctionPtr'),
            StateMember('RayClosestHitShader', 'FunctionPtr'),
            StateMember('RayMissShader', 'FunctionPtr'),
            StateMember('RayIntersectionShader', 'FunctionPtr'),
            StateMember('RayCallableShader', 'FunctionPtr'),
            StateMember('RenderState', 'RenderState')
        ]
    )
    states.append(state)

    # This sucks, it should get the memory semantics from the memory semantics enum
    spirv_atomic_tables = ""
    spirv_atomic_tables += "static const uint32_t MemorySemanticsToSPIRV(const uint32_t sem)\n"
    spirv_atomic_tables += "{\n"
    spirv_atomic_tables += "    uint32_t result = 0;\n"
    spirv_atomic_tables += "    uint32_t mask = sem;\n"
    spirv_atomic_tables += "    if (mask == 0x0)\n"
    spirv_atomic_tables += "        return 0x0; // Relaxed\n"
    spirv_atomic_tables += "    if ((mask & 0x1) == 0x1)\n"
    spirv_atomic_tables += "        result |= 0x2; // Acquire\n"
    spirv_atomic_tables += "    if ((mask & 0x2) == 0x2)\n"
    spirv_atomic_tables += "        result |= 0x4; // Release\n"
    spirv_atomic_tables += "    if ((mask & 0x4) == 0x4)\n"
    spirv_atomic_tables += "        result |= 0x8; // AcquireRelease\n"
    spirv_atomic_tables += "    if ((mask & 0x8) == 0x8)\n"
    spirv_atomic_tables += "        result |= 0x10; // SequentiallyConsistent\n"
    spirv_atomic_tables += "    return result;\n"
    spirv_atomic_tables += "}\n"
    
    spirv_atomic_tables += "static const uint32_t SemanticsTable[] =\n"
    spirv_atomic_tables += "{\n"
    spirv_atomic_tables += "    0x0,\n"
    spirv_atomic_tables += "    0x1,\n"
    spirv_atomic_tables += "    0x2,\n"
    spirv_atomic_tables += "    0x4,\n"
    spirv_atomic_tables += "    0x8,\n"
    spirv_atomic_tables += "    0x10,\n"
    spirv_atomic_tables += "    0x20,\n"
    spirv_atomic_tables += "    0x40,\n"
    spirv_atomic_tables += "    0x80,\n"
    spirv_atomic_tables += "    0x100\n"
    spirv_atomic_tables += "};\n"
    spirv_atomic_tables += "\n"
    spirv_atomic_tables += "static auto ScopeToAtomicScope = [](SPIRVResult::Storage scope) -> uint32_t\n"
    spirv_atomic_tables += "{\n"
    spirv_atomic_tables += "    switch (scope)\n"
    spirv_atomic_tables += "    {\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Function:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Input:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Output:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::PushConstant:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Private:\n"
    spirv_atomic_tables += "            return 4;\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::WorkGroup:\n"
    spirv_atomic_tables += "            return 2;\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Device:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Uniform:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::UniformConstant:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::StorageBuffer:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::MutableImage:\n"
    spirv_atomic_tables += "            return 1;\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Sampler:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Image:\n"
    spirv_atomic_tables += "            assert(false);\n"
    spirv_atomic_tables += "    }\n"
    spirv_atomic_tables += "    return 0;\n"
    spirv_atomic_tables += "};\n"
    spirv_atomic_tables += "\n"
    spirv_atomic_tables += "static auto ScopeToMemorySemantics = [](SPIRVResult::Storage scope) -> uint32_t\n"
    spirv_atomic_tables += "{\n"
    spirv_atomic_tables += "    switch (scope)\n"
    spirv_atomic_tables += "    {\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::WorkGroup:\n"
    spirv_atomic_tables += "            return 0x100; // WorkgroupMemory\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Uniform:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::UniformConstant:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::StorageBuffer:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Sampler:\n"
    spirv_atomic_tables += "            return 0x40; // UniformMemory\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::Image:\n"
    spirv_atomic_tables += "        case SPIRVResult::Storage::MutableImage:\n"
    spirv_atomic_tables += "            return 0x800; // ImageMemory\n"
    spirv_atomic_tables += "    }\n"
    spirv_atomic_tables += "    return 0x0;\n"
    spirv_atomic_tables += "};\n\n"
    spirv_intrinsics.write(spirv_atomic_tables)

    source_file.write(namer.definition())
    source_file.write('} // namespace GPULang\n\n')

    source_file.write('#include "compiler.h"\n')
    source_file.write('namespace GPULang \n{\n')

    for type in type_list:
        declaration_string += type.declaration()
        definition_string += type.definition()
        intrinsic_list.append(type.pair())
        web_types['types'].append(type.api_name if type.api_name is not None else type.name)

    for enum in enums:
        declaration_string += enum.declaration()
        definition_string += enum.definition()
        intrinsic_list.extend(enum.pairs())
        web_types['types'].append(enum.name)

    for struct in structs:
        declaration_string += struct.declaration()
        definition_string += struct.definition()
        intrinsic_list.extend(struct.pairs())
        web_types['types'].append(struct.name)

    for state in states:
        declaration_string += state.declaration()
        definition_string += state.definition()
        #web_types['types'].append(state.name)
        
    header_file.write(declaration_string[0:-1] + '\n')
    header_file.write("\n")
    source_file.write(definition_string[0:-1] + '\n')
    source_file.write("\n")

    header_file.write('} // namespace GPULang\n\n')
    source_file.write('} // namespace GPULang\n\n')

    ### Intrinsics
    intrinsics_header = open("../generated/intrinsics.h", "w")
    intrinsics_source = open("../generated/intrinsics.cc", "w")

    float_types = ['Float32', 'Float32x2', 'Float32x3', 'Float32x4', 'Float16', 'Float16x2', 'Float16x3', 'Float16x4']
    integer_types = ['Int32', 'Int32x2', 'Int32x3', 'Int32x4', 'Int16', 'Int16x2', 'Int16x3', 'Int16x4', 'UInt32', 'UInt32x2', 'UInt32x3', 'UInt32x4', 'UInt16', 'UInt16x2', 'UInt16x3', 'UInt16x4']
    scalar_types = float_types + integer_types
    float_vec_types = ['Float32x2', 'Float32x3', 'Float32x4', 'Float16x2', 'Float16x3', 'Float16x4']
    signed_types = ['Int32', 'Int32x2', 'Int32x3', 'Int32x4', 'Int16', 'Int16x2', 'Int16x3', 'Int16x4', 'Float32', 'Float32x2', 'Float32x3', 'Float32x4', 'Float16', 'Float16x2', 'Float16x3', 'Float16x4']
    bool_types = ['Bool8', 'Bool8x2', 'Bool8x3', 'Bool8x4']


    intrinsics_header.write("//-------------------------------------------------\n")
    intrinsics_header.write("// *** Generated by type_gen.py. ***\n")
    intrinsics_header.write("//       DO NOT MODIFY!!!\n")
    intrinsics_header.write("//-------------------------------------------------\n")
    intrinsics_header.write('namespace GPULang\n')
    intrinsics_header.write('{\n')

    intrinsics_source_start = ''
    intrinsics_source_start += "//-------------------------------------------------\n"
    intrinsics_source_start += "// *** Generated by type_gen.py. ***\n"
    intrinsics_source_start += "//       DO NOT MODIFY!!!\n"
    intrinsics_source_start += "//-------------------------------------------------\n"
    intrinsics_source_start += '#include "ast/function.h"\n'
    intrinsics_source_start += '#include "ast/variable.h"\n'
    intrinsics_source_start += '#include "types.h"\n'
    intrinsics_source_start += '#include "intrinsics.h"\n'
    intrinsics_source_start += '#include "compiler.h"\n'

    intrinsics_source_start += 'namespace GPULang\n'
    intrinsics_source_start += '{\n'

    intrinsics_source.write(intrinsics_source_start)

    float_only_single_argument_intrinsics = [
        'acos', 'acosh', 'asin', 'asinh', 'atan', 'atanh', 'cos', 'cosh', 'exp',
        'exp2', 'invSqrt', 'log', 'log2', 'sin', 'sinh', 'sqrt', 'tan', 'tanh'
    ]

    float_only_single_argument_spirv_intrinsics = [
        'Acos', 'Acosh', 'Asin', 'Asinh', 'Atan', 'Atanh', 'Cos', 'Cosh', 'Exp',
        'Exp2', 'InverseSqrt', 'Log', 'Log2', 'Sin', 'Sinh', 'Sqrt', 'Tan', 'Tanh'
    ]

    docs = [
        'Returns the arc cosine of a value.',
        'Returns the hyperbolic arc cosine of a value.',
        'Returns the arc sine of a value.',
        'Returns the hyperbolic arc sine of a value.',
        'Returns the arc tangent of a value.',
        'Returns the hyperbolic arc tangent of a value.',
        'Returns the cosine of a value.',
        'Returns the hyperbolic cosine of a value.',
        'Returns the exponential of a value.',
        'Returns the base 2 exponential of a value.',
        'Returns the inverse square root of a value.',
        'Returns the natural logarithm of a value.',
        'Returns the base 2 logarithm of a value.',
        'Returns the sine of a value.',
        'Returns the hyperbolic sine of a value.',
        'Returns the square root of a value.',
        'Returns the tangent of a value.',
        'Returns the hyperbolic tangent of a value.'
    ]

    for intrinsic, spirv_op, doc in zip(float_only_single_argument_intrinsics, float_only_single_argument_spirv_intrinsics, docs):
        for type in float_types:
            function_name = f'{intrinsic}_{type}'
            argument_name = f'{function_name}_arg'
            fun = Function(
                decl_name = function_name,
                api_name = intrinsic,
                return_type = type,
                documentation = doc,
                parameters = [Variable(decl_name = argument_name, api_name = "val", type_name=type)],    
            )

            spirv_function = ''
            spirv_function += '    SPIRVResult arg = LoadValueSPIRV(c, g, args[0]);\n'
            spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), {spirv_op}, arg);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            fun.spirv = spirv_function
            functions.append(fun)

    # Atan2
    for type in float_types:
        function_name = f'atan2_{type}'
        y_name = f'atan2_{type}_y'
        x_name = f'atan2_{type}_x'
        fun = Function(
            decl_name = function_name,
            api_name = 'atan2',
            return_type = type,
            documentation = 'Returns the angle whose tangent is the quotient of the two specified numbers.',
            parameters = [
                Variable(decl_name = y_name, api_name = "y", type_name=type),
                Variable(decl_name = x_name, api_name = "x", type_name=type)
            ]
        )
        

        spirv_function = ''
        spirv_function += '    SPIRVResult y = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult x = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Atan2, y, x);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        fun.spirv = spirv_function
        functions.append(fun)
        
    # Pow
    for type in float_types:
        intrinsic = 'pow'
        function_name = f'{intrinsic}_{type}'
        argument_name = f'{function_name}_arg'
        exponent_name = f'{function_name}_exp'
        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns the result of raising a value to the power of an exponent.',
            parameters = [
                Variable(decl_name = argument_name, api_name = "val", type_name=type),
                Variable(decl_name = exponent_name, api_name = "exponent", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult exponent = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Pow, val, exponent);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        fun.spirv = spirv_function
        functions.append(fun)

    # Mad
    for type in scalar_types:
        intrinsic = 'mad'
        function_name = f'{intrinsic}_{type}'
        argument_name = f'{function_name}_arg'
        multiplier_name = f'{function_name}_mul'
        addend_name = f'{function_name}_add'

        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns the result of multiplying a value by a multiplier and adding an addend.',
            parameters = [
                Variable(decl_name = argument_name, api_name = "val", type_name=type),
                Variable(decl_name = multiplier_name, api_name = "multiplier", type_name=type),
                Variable(decl_name = addend_name, api_name = "addend", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult multiplicant = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    SPIRVResult addend = LoadValueSPIRV(c, g, args[2]);\n'
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Fma, val, multiplicant, addend);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        fun.spirv = spirv_function
        functions.append(fun)

    # Dot
    for type in float_vec_types:
        intrinsic = 'dot'
        function_name = f'{intrinsic}_{type}'
        x_name = f'{function_name}_x'
        y_name = f'{function_name}_y'

        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = base_type_mapping[type],
            documentation = 'Returns the dot product of two vectors.',
            parameters = [
                Variable(decl_name = x_name, api_name = "x", type_name=type),
                Variable(decl_name = y_name, api_name = "y", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult x = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult y = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpDot, SPVWriter::Section::LocalFunction, returnType, x, y);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        fun.spirv = spirv_function
        functions.append(fun)

    # Reflect
    for type in float_vec_types:
        intrinsic = 'reflect'
        function_name = f'{intrinsic}_{type}'
        incident_name = f'{function_name}_incident'
        normal_name = f'{function_name}_normal'

        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns the reflection of a vector through a surface using an incident vector and normal.',
            parameters = [
                Variable(decl_name = incident_name, api_name = "incident", type_name=type),
                Variable(decl_name = normal_name, api_name = "normal", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult incident = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult normal = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Reflect, incident, normal);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'

        fun.spirv = spirv_function
        functions.append(fun)

    # Refract
    for type in float_vec_types:
        intrinsic = 'refract'
        function_name = f'{intrinsic}_{type}'
        incident_name = f'{function_name}_incident'
        normal_name = f'{function_name}_normal'
        ior_name = f'{function_name}_ior'
        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns the refraction of a vector through a surface using an incident vector, normal and an index of refraction.',
            parameters = [
                Variable(decl_name = incident_name, api_name = "incident", type_name=type),
                Variable(decl_name = normal_name, api_name = "normal", type_name=type),
                Variable(decl_name = ior_name, api_name = "ior", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult incident = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult normal = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    SPIRVResult ior = LoadValueSPIRV(c, g, args[2]);\n'
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Refract, incident, normal, ior);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'

        fun.spirv = spirv_function
        functions.append(fun)

    # Cross
    cross_product_types = ['Float32x3', 'Float16x3']
    intrinsic = 'cross'
    for type in cross_product_types:
        function_name = f'{intrinsic}_{type}'
        v0_name = f'{function_name}_v0'
        v1_name = f'{function_name}_v1'
        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns the cross product of two vectors.',
            parameters = [
                Variable(decl_name = v0_name, api_name = "v0", type_name=type),
                Variable(decl_name = v1_name, api_name = "v1", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult x = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult y = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Cross, x, y);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'

        fun.spirv = spirv_function
        functions.append(fun)

    # Length & Normalize
    ops = ['length', 'normalize']
    docs = ['Returns the length of the vector.', 'Returns the normalized vector.']
    spirv_ops = ['Length', 'Normalize']
    for intrinsic, spirv_op, doc in zip(ops, spirv_ops, docs):
        for type in float_vec_types:
            function_name = f'{intrinsic}_{type}'
            argument_name = f'{function_name}_arg'

            if intrinsic == 'length':
                return_type = 'Float32'
            else:
                return_type = type
            fun = Function(
                decl_name = function_name,
                api_name = intrinsic,
                return_type = return_type,
                documentation = doc,
                parameters = [
                    Variable(decl_name = argument_name, api_name = "val", type_name=type)
                ]
            )

            spirv_function = ''
            spirv_function += '    SPIRVResult vec = LoadValueSPIRV(c, g, args[0]);\n'    
            spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), {spirv_op}, vec);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'

            fun.spirv = spirv_function
            functions.append(fun)

    # Distance
    for type in float_vec_types:
        intrinsic = 'distance'
        function_name = f'{intrinsic}_{type}'
        p0_name = f'{function_name}_p0'
        p1_name = f'{function_name}_p1'
        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns the distance between two points.',
            parameters = [
                Variable(decl_name = p0_name, api_name = "p0", type_name=type),
                Variable(decl_name = p1_name, api_name = "p1", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult v0 = LoadValueSPIRV(c, g, args[0]);\n'    
        spirv_function += '    SPIRVResult v1 = LoadValueSPIRV(c, g, args[1]);\n'    
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Distance, v0, v1);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'

        fun.spirv = spirv_function
        functions.append(fun)

    # Min & Max
    ops = ['min', 'max']
    spirv_ops = ['Min', 'Max']
    for intrinsic, spirv_op in zip(ops, spirv_ops):
        for type in scalar_types:
            function_name = f'{intrinsic}_{type}'
            x_name = f'{function_name}_x'
            y_name = f'{function_name}_y'
            fun = Function(
                decl_name = function_name,
                api_name = intrinsic,
                return_type = type,
                documentation = f'Returns the {intrinsic}imum of x and y.',
                parameters = [
                    Variable(decl_name = x_name, api_name = "x", type_name=type),
                    Variable(decl_name = y_name, api_name = "y", type_name=type)
                ]
            )

            if  type.startswith('Float'):
                spirv_op_type = 'F'
            elif type.startswith('Int'):
                spirv_op_type = 'S'
            elif type.startswith('UInt'):
                spirv_op_type = 'U'
            spirv_function = ''
            spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n'    
            spirv_function += '    SPIRVResult limit = LoadValueSPIRV(c, g, args[1]);\n'    
            spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), {spirv_op_type}{spirv_op}, val, limit);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'

            fun.spirv = spirv_function
            functions.append(fun)

    # Clamp
    for type in scalar_types:
        intrinsic = 'clamp'
        function_name = f'{intrinsic}_{type}'
        value_name = f'{function_name}_val'
        min_name = f'{function_name}_min'
        max_name = f'{function_name}_max'
        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns the value clamped between min and max.',
            parameters = [
                Variable(decl_name = value_name, api_name = "val", type_name=type),
                Variable(decl_name = min_name, api_name = "min", type_name=type),
                Variable(decl_name = max_name, api_name = "max", type_name=type)
            ]
        )
   
        if  type.startswith('Float'):
            spirv_op_type = 'F'
        elif type.startswith('Int'):
            spirv_op_type = 'S'
        elif type.startswith('UInt'):
            spirv_op_type = 'U'
        spirv_function = ''
        spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n'    
        spirv_function += '    SPIRVResult min = LoadValueSPIRV(c, g, args[1]);\n'    
        spirv_function += '    SPIRVResult max = LoadValueSPIRV(c, g, args[2]);\n'    
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), {spirv_op_type}Clamp, val, min, max);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'

        fun.spirv = spirv_function
        functions.append(fun)
    
    # Lerp
    for type in float_types:
        intrinsic = 'lerp'
        function_name = f'{intrinsic}_{type}'
        a_name = f'{function_name}_a'
        b_name = f'{function_name}_b'
        t_name = f'{function_name}_t'

        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns the linear interpolation between a and b by t.',
            parameters = [
                Variable(decl_name = a_name, api_name = "a", type_name=type),
                Variable(decl_name = b_name, api_name = "b", type_name=type),
                Variable(decl_name = t_name, api_name = "t", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult a = LoadValueSPIRV(c, g, args[0]);\n'    
        spirv_function += '    SPIRVResult b = LoadValueSPIRV(c, g, args[1]);\n'    
        spirv_function += '    SPIRVResult t = LoadValueSPIRV(c, g, args[2]);\n'    
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), FMix, a, b, t);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    # Step
    for type in float_types:
        intrinsic = 'step'
        function_name = f'{intrinsic}_{type}'
        edge_name = f'{function_name}_edge'
        x_name = f'{function_name}_x'
        
        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns 0.0 if x < edge, otherwise returns 1.0.',
            parameters = [
                Variable(decl_name = edge_name, api_name = "edge", type_name=type),
                Variable(decl_name = x_name, api_name = "x", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult edge = LoadValueSPIRV(c, g, args[0]);\n'    
        spirv_function += '    SPIRVResult x = LoadValueSPIRV(c, g, args[1]);\n'    
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), Step, edge, x);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)
    
    # Smoothstep
    for type in float_types:
        intrinsic = 'smoothstep'
        function_name = f'{intrinsic}_{type}'
        edge0_name = f'{function_name}_edge0'
        edge1_name = f'{function_name}_edge1'
        x_name = f'{function_name}_x'
        fun = Function(
            decl_name = function_name,
            api_name = intrinsic,
            return_type = type,
            documentation = 'Returns the smoothstep interpolation of the input.',
            parameters = [
                Variable(decl_name = edge0_name, api_name = "edge0", type_name=type),
                Variable(decl_name = edge1_name, api_name = "edge1", type_name=type),
                Variable(decl_name = x_name, api_name = "x", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult edge0 = LoadValueSPIRV(c, g, args[0]);\n'    
        spirv_function += '    SPIRVResult edge1 = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    SPIRVResult x = LoadValueSPIRV(c, g, args[2]);\n'    
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), SmoothStep, edge0, edge1, x);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    # Ceil, Floor, Fract, Saturate, Trunc, Ddx, Ddy, Fwidth
    ops = ['ceil', 'floor', 'fract', 'saturate', 'trunc', 'ddx', 'ddy', 'fwidth']
    docs = [
        'Returns the smallest integer value that is greater than or equal to the input.',
        'Returns the largest integer value that is less than or equal to the input.',
        'Returns the fractional part of the input.',
        'Returns the input clamped to the range [0, 1].',
        'Returns the integer part of the input, removing any fractional part.',
        'Returns the derivative of the input with respect to the screen x coordinate.',
        'Returns the derivative of the input with respect to the screen y coordinate.',
        'Returns the width of the input, which is the maximum of the absolute values of the derivatives in the screen x and y coordinates.'
    ]
    spirv_ops = ['Ceil', 'Floor', 'Fract', 'Saturate', 'Trunc', 'DPdx', 'DPdy', 'Fwidth']
    for intrinsic, spirv_op, doc in zip(ops, spirv_ops, docs):
        for type in float_types:
            function_name = f'{intrinsic}_{type}'
            argument_name = f'{function_name}_arg'
            fun = Function(
                decl_name = function_name,
                api_name = intrinsic,
                return_type = type,
                documentation = doc,
                parameters = [
                    Variable(decl_name = argument_name, api_name = "val", type_name=type)
                ]
            )

            spirv_function = ''
            spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n' 
            if intrinsic == 'ddx' or intrinsic == 'ddy' or intrinsic == 'fwidth':
                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(Op{spirv_op}, SPVWriter::Section::LocalFunction, returnType, val);\n'
            elif intrinsic == 'saturate':
                size = vector_size_mapping[type]
                spirv_function += f'    SPIRVResult min = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(0));\n'
                spirv_function += f'    SPIRVResult max = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float(1));\n'
                if size > 1:
                    spirv_function += f'    min = GenerateSplatCompositeSPIRV(c, g, returnType, {size}, min);\n'
                    spirv_function += f'    max = GenerateSplatCompositeSPIRV(c, g, returnType, {size}, max);\n'
                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), FClamp, val, min, max);\n'
            else:
                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), {spirv_op}, val);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    # Sign and Abs
    ops = ['sign', 'abs']
    docs = [
        'Returns the sign of the value.',
        'Returns the absolute value of the input.'
    ]
    spirv_ops = ['Sign', 'Abs']
    for intrinsic, spirv_op, doc in zip(ops, spirv_ops, docs):
        for type in signed_types:
            function_name = f'{intrinsic}_{type}'
            argument_name = f'{function_name}_arg'
            fun = Function( 
                decl_name = function_name,
                api_name = intrinsic,
                return_type = type,
                documentation = doc,
                parameters = [
                    Variable(decl_name = argument_name, api_name = "val", type_name=type)
                ]
            )
            if  type.startswith('Float'):
                spirv_op_type = 'F'
            elif type.startswith('Int'):
                spirv_op_type = 'S'
            spirv_function = ''
            spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n'    
            spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), {spirv_op_type}{spirv_op}, val);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    cast_types = ["Float16", "UInt16", "Int16"]

    for type1 in cast_types:
        for type2 in cast_types:
            if type1 != type2:
                intrinsic = f'castTo{data_type_mapping[type1].title()}'
                function_name = f'{intrinsic}_{type2}'
                argument_name = f'{function_name}_arg'
                fun = Function( 
                    decl_name = function_name,
                    api_name = intrinsic,
                    return_type = type1,
                    documentation = f'Casts to type {type1} without conversion.',
                    parameters = [
                        Variable(decl_name = argument_name, api_name = "val", type_name=type2)
                    ]
                )
                spirv_function = ''
                spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n'    
                spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpBitcast, SPVWriter::Section::LocalFunction, returnType, val);\n'
                spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                

                fun.spirv = spirv_function
                functions.append(fun)

    cast_types = ["Float32", "UInt32", "Int32"]

    for type1 in cast_types:
        for type2 in cast_types:
            if type1 != type2:
                intrinsic = f'castTo{data_type_mapping[type1].title()}'
                function_name = f'{intrinsic}_{type2}'
                argument_name = f'{function_name}_arg'
                fun = Function( 
                    decl_name = function_name,
                    api_name = intrinsic,
                    return_type = type1,
                    documentation = f'Casts to type {type1} without conversion.',
                    parameters = [
                        Variable(decl_name = argument_name, api_name = "val", type_name=type2)
                    ]
                )

                spirv_function = ''
                spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n'    
                spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpBitcast, SPVWriter::Section::LocalFunction, returnType, val);\n'
                spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                

                fun.spirv = spirv_function
                functions.append(fun)

    # Any and all
    ops = ['any', 'all']
    docs = [
        'Returns true if any component of the vector is non-zero.',
        'Returns true if all components of the vector are non-zero.'
    ]
    spirv_op = ['Any', 'All']
    for intrinsic, spirv_op, doc in zip(ops, spirv_op, docs):
        for type in bool_types:
            function_name = f'{intrinsic}_{type}'
            argument_name = f'{function_name}_arg'

            fun = Function( 
                decl_name = function_name,
                api_name = intrinsic,
                return_type = f'Bool8',
                documentation = doc,
                parameters = [
                    Variable(decl_name = argument_name, api_name = "val", type_name=type)
                ]
            )

            spirv_function = ''
            spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n'    
            spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(Op{spirv_op}, SPVWriter::Section::LocalFunction, returnType, val);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    ops = ['transpose', 'inverse']
    docs = [
        'Returns the transposed matrix.',
        'Returns the inverse of the matrix.'
    ]
    matrix_types = []
    for i in range(2, 5):
        for j in range(2, 5):
            matrix_types.append(f'Float32x{i}x{j}')
            matrix_types.append(f'Float16x{i}x{j}')

    for intrinsic, doc in zip(ops, docs):
        for type in matrix_types:
            function_name = f'{intrinsic}_{type}'
            argument_name = f'{function_name}_arg'

            fun = Function( 
                decl_name = function_name,
                api_name = intrinsic,
                return_type = type,
                documentation = doc,
                parameters = [
                    Variable(decl_name = argument_name, api_name = "val", type_name=type)
                ]
            )
            
            spirv_function = ''
            spirv_function += '    SPIRVResult val = LoadValueSPIRV(c, g, args[0]);\n'    
            if intrinsic == 'transpose':
                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpTranspose, SPVWriter::Section::LocalFunction, returnType, val);\n'
            else:
                spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpExtInst, SPVWriter::Section::LocalFunction, returnType, SPVArg(g->writer->Import(GLSL)), MatrixInverse, val);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)


    # Builtin value getters
    intrinsics = ['GetOutputLayer', 'GetOutputViewport', 'GetIndex', 'GetInstanceIndex', 'GetBaseIndex', 'GetBaseInstanceIndex', 'GetDrawIndex']
    docs = [
        'Returns the output layer for the current vertex.',
        'Returns the output viewport for the current vertex.',
        'Returns the index of the current vertex.',
        'Returns the instance index of the current vertex.',
        'Returns the base index of the current vertex.',
        'Returns the base instance index of the current vertex.',
        'Returns the draw index of the current vertex.'
    ]
    vertex_value_builtins_spirv = ['Layer', 'ViewportIndex', 'VertexIndex', 'InstanceIndex', 'BaseVertex', 'BaseInstance', 'DrawIndex']
    for builtin, spirv_builtin, doc in zip(intrinsics, vertex_value_builtins_spirv, docs):
        intrinsic = builtin
        function_name = f'Vertex{intrinsic}'

        fun = Function( 
            decl_name = function_name,
            api_name = f'vertex{intrinsic}',
            return_type = 'UInt32',
            documentation = doc,
            parameters = [
            ],
            
        )

        spirv_function = ''
        if builtin == 'GetOutputLayer':
            spirv_function += '    g->writer->Capability(Capabilities::ShaderViewportIndexLayerEXT);\n'
        elif builtin == 'GetOutputViewport':
            spirv_function += '    g->writer->Capability(Capabilities::ShaderViewportIndexLayerEXT);\n'
        elif builtin == 'GetIndex' or builtin == 'GetInstanceIndex' or builtin == 'GetBaseIndex' or builtin == 'GetBaseInstanceIndex' or builtin == 'GetDrawIndex':
            spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
        spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 1);\n'
        spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
        spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{builtin}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
        spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
        spirv_function += '    g->interfaceVariables.Insert(ret);\n'
        spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
        spirv_function += '    res.parentTypes.push_back(baseType);\n'
        spirv_function += '    return res;\n'
        
        
        fun.spirv = spirv_function
        functions.append(fun)

    unsigned_types = ['UInt16', 'UInt32']
    intrinsics = ['SetOutputLayer', 'SetOutputViewport']
    docs = [
        'Sets the output layer for the current vertex.',
        'Sets the output viewport for the current vertex.'
    ]
    vertex_value_builtins_spirv = ['Layer', 'ViewportIndex']
    for builtin, spirv_builtin, doc in zip(intrinsics, vertex_value_builtins_spirv, docs):
        for type in unsigned_types:
            intrinsic = builtin
            function_name = f'Vertex{intrinsic}_{type}'
            argument_name = f'{function_name}_arg'

            fun = Function( 
                decl_name = function_name,
                api_name = f'vertex{intrinsic}',
                return_type = 'Void',
                documentation = doc,
                parameters = [
                    Variable(decl_name = argument_name, api_name = "val", type_name=type)
                ]
            )

            spirv_function = '' 
            if builtin == 'SetOutputLayer':
                spirv_function += '    g->writer->Capability(Capabilities::ShaderViewportIndexLayerEXT);\n'
            elif builtin == 'SetOutputViewport':
                spirv_function += '    g->writer->Capability(Capabilities::ShaderViewportIndexLayerEXT);\n'
            spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 1);\n'
            spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));\n'
            spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{builtin}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);\n'
            spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
            spirv_function += '    g->interfaceVariables.Insert(ret);\n'
            spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
            spirv_function += '    g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ret}, loaded);\n'
            spirv_function += '    return SPIRVResult::Invalid();\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    # Export vertex coordinates
    four_component_float_vec_types = ['Float32x4', 'Float16x4']
    vertex_value_builtins_spirv = ['Position', 'Position']

    for type, spirv_builtin in zip(four_component_float_vec_types, vertex_value_builtins_spirv):
        intrinsic = 'ExportCoordinates'
        function_name = f'Vertex{intrinsic}_{type}'
        argument_name = f'{function_name}_arg'
        fun = Function( 
            decl_name = function_name,
            api_name = f'vertex{intrinsic}',
            return_type = 'Void',
            documentation = 'Exports the value as the vertex position to the rasterizer. This function must be called at least once in a vertex shader. This is the same as the `SV_POSITION` in HLSL or `gl_Position` in GLSL.',
            parameters = [
                Variable(decl_name = argument_name, api_name = "val", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
        spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 4);\n'
        spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32x4_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));\n'
        spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gplVertexCoordinates"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);\n'
        spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
        spirv_function += '    g->interfaceVariables.Insert(ret);\n'
        spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ret}, loaded);\n'
        spirv_function += '    return SPIRVResult::Invalid();\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'ExportPrimitiveIndex'
    docs = 'Exports the primitive index of the current primitive being processed. This can be read in the pixel shader with pixelGetPrimitiveIndex().'

    function_name = f'Geometry{intrinsic}'
    argument_name = f'{function_name}_arg'
    fun = Function( 
        decl_name = function_name,
        api_name = f'geometry{intrinsic}',
        return_type = 'UInt32',
        documentation=doc,
        parameters = [
            Variable(decl_name = argument_name, api_name = "index", type_name="UInt32")

        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    g->writer->Capability(Capabilities::Geometry);\n'
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 1);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gplGeometryExportPrimitiveIndex"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::PrimitiveId);\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
    spirv_function += '    g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ret}, loaded);\n'
    spirv_function += '    return SPIRVResult::Invalid();\n'

    fun.spirv = spirv_function
    functions.append(fun)

    primitive_get_id_stages = ['Hull', 'Domain', 'Pixel']
    intrinsic = "GetPrimitiveIndex"
    doc = "Gets the primitive index being processed. If the previous stage was a GeometryShader, it must call ExportPrimitiveIndex to set the value."
    for stage in primitive_get_id_stages:
        function_name = f'{stage}{intrinsic}'
        argument_name = f'{function_name}_arg'

        fun = Function( 
            decl_name = function_name,
            api_name = f'{stage.lower()}{intrinsic}',
            return_type = 'UInt32',
            documentation=doc,
            parameters = [
                Variable(decl_name = argument_name, api_name = "index", type_name="UInt32")

            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
        if stage == 'Hull' or stage == 'Domain':
            spirv_function += '    g->writer->Capability(Capabilities::Tessellation);\n'
        spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 1);\n'
        spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));\n'
        spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{stage}ExportPrimitiveIndex"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
        spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::PrimitiveId);\n'
        spirv_function += '    g->interfaceVariables.Insert(ret);\n'
        spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ret}, loaded);\n'
        spirv_function += '    return SPIRVResult::Invalid();\n'

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'GetInvocationIndex'
    docs = 'Returns the invocation index of the vertex within the primitive being processed.'

    function_name = f'Geometry{intrinsic}'
    fun = Function( 
        decl_name = function_name,
        api_name = f'geometry{intrinsic}',
        return_type = 'UInt32',
        documentation=doc,
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    g->writer->Capability(Capabilities::Geometry);\n'
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 1);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gplGeometryGetInvocationIndex"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::InvocationId);\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
    spirv_function += '    res.parentTypes.push_back(baseType);\n'
    spirv_function += '    return res;\n'

    fun.spirv = spirv_function
    functions.append(fun)

    for type, spirv_builtin in zip(four_component_float_vec_types, vertex_value_builtins_spirv):
        intrinsic = 'ExportVertex'
        function_name = f'Geometry{intrinsic}_{type}'
        argument_name = f'{function_name}_arg'
        fun = Function( 
            decl_name = function_name,
            api_name = f'geometry{intrinsic}',
            return_type = 'Void',
            documentation = 'Exports the state of all *out* values as vertex data, and sets the argument *val* as the rasterization position. This function must be called for each point in the topology given by output_primitive. This is the same as writing `gl_Position` followed by calling EmitVertex() in GLSL.',
            parameters = [
                Variable(decl_name = argument_name, api_name = "val", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
        spirv_function += '    g->writer->Capability(Capabilities::Geometry);\n'
        spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 4);\n'
        spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32x4_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));\n'
        spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gplGeometryCoordinates"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);\n'
        spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
        spirv_function += '    g->interfaceVariables.Insert(ret);\n'
        spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ret}, loaded);\n'
        spirv_function += f'    g->writer->Instruction(OpEmitVertex, SPVWriter::Section::LocalFunction);\n'
        spirv_function += '    return SPIRVResult::Invalid();\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'ExportPrimitive'
    docs = 'Exports the current primitive to the output stream. This function must be called after all vertices of a primitive have been exported.'

    function_name = f'Geometry{intrinsic}'
    fun = Function( 
        decl_name = function_name,
        api_name = f'geometry{intrinsic}',
        return_type = 'Void',
        documentation=doc,
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Geometry);\n'
    spirv_function += f'    g->writer->Instruction(OpEndPrimitive, SPVWriter::Section::LocalFunction);\n'
    spirv_function += '    return SPIRVResult::Invalid();\n'

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'ExportOuterTessellationLevels'
    docs = 'Sets the outer tessellation levels for the current patch. This function must be called once per patch.'

    function_name = f'Hull{intrinsic}'
    fun = Function( 
        decl_name = function_name,
        api_name = f'hull{intrinsic}',
        return_type = 'Void',
        documentation=docs,
        parameters = [
            Variable(decl_name = f'{function_name}_arg', api_name = "levels", type_name='Float32x4')
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    g->writer->Capability(Capabilities::Tessellation);\n'
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 4);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32x4_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gplHullOuterTessellationLevels"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::TessLevelOuter);\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
    spirv_function += '    g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ret}, loaded);\n'
    spirv_function += '    return SPIRVResult::Invalid();\n'
    
    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'ExportInnerTessellationLevels'
    docs = 'Sets the inner tessellation levels for the current patch. This function must be called once per patch.'

    function_name = f'Hull{intrinsic}'
    fun = Function( 
        decl_name = function_name,
        api_name = f'hull{intrinsic}',
        return_type = 'Void',
        documentation=docs,
        parameters = [
            Variable(decl_name = f'{function_name}_arg', api_name = "levels", type_name='Float32x2')
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    g->writer->Capability(Capabilities::Tessellation);\n'
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 2);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32x2_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gplInnerHullTessellationLevels"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::TessLevelInner);\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
    spirv_function += '    g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ret}, loaded);\n'
    spirv_function += '    return SPIRVResult::Invalid();\n'
    
    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'GetTessellationCoordinates'
    docs = 'Returns the tessellation coordinates of the point being processed. For quads, the z coordinate is meaningless and will always be 0, and x,y are bilinear weights. For triangles, x,y,z form barycentric coordinates.'

    function_name = f'Domain{intrinsic}'
    fun = Function( 
        decl_name = function_name,
        api_name = f'domain{intrinsic}',
        return_type = 'Float32x3',
        documentation=docs,
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    g->writer->Capability(Capabilities::Tessellation);\n'
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 23);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32x3_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gplDomainTessellationCoordinates"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::TessCoord);\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
    spirv_function += '    res.parentTypes.push_back(baseType);\n'
    spirv_function += '    return res;\n'
    
    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'GetInvocationIndex'
    docs = 'Returns the invocation index of the vertex within the patch being processed.'

    function_name = f'Domain{intrinsic}'
    fun = Function( 
        decl_name = function_name,
        api_name = f'domain{intrinsic}',
        return_type = 'UInt32',
        documentation=doc,
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    g->writer->Capability(Capabilities::Tessellation);\n'
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 1);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gplDomainGetInvocationIndex"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::InvocationId);\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
    spirv_function += '    res.parentTypes.push_back(baseType);\n'
    spirv_function += '    return res;\n'

    fun.spirv = spirv_function
    functions.append(fun)

    def geometry_read_function(size, ret):
        return_function = ''
        # Declare the inputs as the 4 variables Position, PointSize, CullDistance and ClipDistance
        return_function += f'    SPIRVResult arraySizeConstant = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int({size}));\n'
        return_function += '    uint32_t float32Type = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 1);\n'
        return_function += f'    uint32_t float32ArrayType = AddType(g, "[{size}]_f32", OpTypeArray, SPVArg(float32Type), arraySizeConstant);\n'
        return_function += f'    uint32_t float32ArrayInputTypePtr = GPULang::AddType(g, TStr("ptr_[{size}]_f32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(float32ArrayType));\n'
        return_function += f'    uint32_t float32ArrayFunctionTypePtr = GPULang::AddType(g, TStr("ptr_[{size}]_f32_Function"), OpTypePointer, VariableStorage::Function, SPVArg(float32ArrayType));\n'
        return_function += '    uint32_t float32x4Type = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 4);\n'
        return_function += f'    uint32_t float32x4ArrayType = AddType(g, "[{size}]_f32x4", OpTypeArray, SPVArg(float32x4Type), arraySizeConstant);\n'
        return_function += f'    uint32_t float32x4ArrayInputTypePtr = GPULang::AddType(g, TStr("ptr_[{size}]_f32x4_Input"), OpTypePointer, VariableStorage::Input, SPVArg(float32x4ArrayType));\n'
        return_function += f'    uint32_t float32x4ArrayFunctionTypePtr = GPULang::AddType(g, TStr("ptr_[{size}]_f32x4_Function"), OpTypePointer, VariableStorage::Function, SPVArg(float32x4ArrayType));\n'
        return_function += f'    uint32_t positions = GPULang::AddSymbol(g, TStr("gplVertexPosition[{size}]"), SPVWriter::Section::Declarations, OpVariable, float32x4ArrayInputTypePtr, VariableStorage::Input);\n'
        return_function += f'    uint32_t pointSizes = GPULang::AddSymbol(g, TStr("gplPointSize[{size}]"), SPVWriter::Section::Declarations, OpVariable, float32ArrayInputTypePtr, VariableStorage::Input);\n'
        return_function += f'    uint32_t cullDistances = GPULang::AddSymbol(g, TStr("gplCullDistance[{size}]"), SPVWriter::Section::Declarations, OpVariable, float32ArrayInputTypePtr, VariableStorage::Input);\n'
        return_function += f'    uint32_t clipDistances = GPULang::AddSymbol(g, TStr("gplClipDistance[{size}]"), SPVWriter::Section::Declarations, OpVariable, float32ArrayInputTypePtr, VariableStorage::Input);\n'
        return_function += f'    g->writer->Decorate(SPVArg(positions), Decorations::BuiltIn, Builtins::Position);\n'
        return_function += f'    g->writer->Decorate(SPVArg(pointSizes), Decorations::BuiltIn, Builtins::PointSize);\n'
        return_function += f'    g->writer->Decorate(SPVArg(cullDistances), Decorations::BuiltIn, Builtins::CullDistance);\n'
        return_function += f'    g->writer->Decorate(SPVArg(clipDistances), Decorations::BuiltIn, Builtins::ClipDistance);\n'

        # Get access chains to the members of the struct
        return_function += '    SPIRVResult const0 = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(0));\n'
        return_function += '    SPIRVResult const1 = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(1));\n'
        return_function += '    SPIRVResult const2 = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(2));\n'
        return_function += '    SPIRVResult const3 = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int(3));\n'
        return_function += f'    uint32_t ptrToPositions = g->writer->MappedInstruction(OpAccessChain, SPVWriter::Section::LocalFunction, float32x4ArrayFunctionTypePtr, SPVArg({ret}), const0);\n'
        return_function += f'    uint32_t ptrToPointSizes = g->writer->MappedInstruction(OpAccessChain, SPVWriter::Section::LocalFunction, float32ArrayFunctionTypePtr, SPVArg({ret}), const1);\n'
        return_function += f'    uint32_t ptrToCullDistances = g->writer->MappedInstruction(OpAccessChain, SPVWriter::Section::LocalFunction, float32ArrayFunctionTypePtr, SPVArg({ret}), const2);\n'
        return_function += f'    uint32_t ptrToClipDistances = g->writer->MappedInstruction(OpAccessChain, SPVWriter::Section::LocalFunction, float32ArrayFunctionTypePtr, SPVArg({ret}), const3);\n'

        # Copy
        return_function += '    g->writer->Instruction(OpCopyMemory, SPVWriter::Section::LocalFunction, SPVArg(ptrToPositions), SPVArg(positions));\n'
        return_function += '    g->writer->Instruction(OpCopyMemory, SPVWriter::Section::LocalFunction, SPVArg(ptrToPointSizes), SPVArg(pointSizes));\n'
        return_function += '    g->writer->Instruction(OpCopyMemory, SPVWriter::Section::LocalFunction, SPVArg(ptrToCullDistances), SPVArg(cullDistances));\n'
        return_function += '    g->writer->Instruction(OpCopyMemory, SPVWriter::Section::LocalFunction, SPVArg(ptrToClipDistances), SPVArg(clipDistances));\n'
        return_function += '    g->interfaceVariables.Insert(positions);\n'
        return_function += '    g->interfaceVariables.Insert(pointSizes);\n'
        return_function += '    g->interfaceVariables.Insert(cullDistances);\n'
        return_function += '    g->interfaceVariables.Insert(clipDistances);\n'
        return return_function

    intrinsic = "GetPoint"
    function_name = f'Geometry{intrinsic}'
    fun = Function(
        decl_name = function_name,
        api_name = f'geometry{intrinsic}',
        return_type = 'GeometryPoint',
        documentation = 'Returns a struct of the current point.',
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_GeometryPoint_Function"), OpTypePointer, VariableStorage::Function, SPVArg(returnType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{intrinsic}"), SPVWriter::Section::LocalFunction, OpVariable, typePtr, VariableStorage::Function);\n'
    spirv_function += geometry_read_function(1, "ret")
    spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
    spirv_function += '    res.parentTypes.push_back(returnType);\n'
    spirv_function += '    return res;\n'

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = "GetLine"
    function_name = f'Geometry{intrinsic}'
    fun = Function(
        decl_name = function_name,
        api_name = f'geometry{intrinsic}',
        return_type = 'GeometryLine',
        documentation = 'Returns a struct of the current line.',
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_GeometryLine_Function"), OpTypePointer, VariableStorage::Function, SPVArg(returnType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{intrinsic}"), SPVWriter::Section::LocalFunction, OpVariable, typePtr, VariableStorage::Function);\n'
    spirv_function += geometry_read_function(2, "ret")
    spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
    spirv_function += '    res.parentTypes.push_back(returnType);\n'
    spirv_function += '    return res;\n'

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = "GetTriangle"
    function_name = f'Geometry{intrinsic}'
    fun = Function(
        decl_name = function_name,
        api_name = f'geometry{intrinsic}',
        return_type = 'GeometryTriangle',
        documentation = 'Returns a struct of the current triangle.',
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    
    # Declare return struct
    spirv_function += f'    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_GeometryTriangle_Function"), OpTypePointer, VariableStorage::Function, SPVArg(returnType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{intrinsic}"), SPVWriter::Section::LocalFunction, OpVariable, typePtr, VariableStorage::Function);\n'

    spirv_function += geometry_read_function(3, "ret")

    spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
    spirv_function += '    res.parentTypes.push_back(returnType);\n'
    spirv_function += '    return res;\n'

    fun.spirv = spirv_function
    functions.append(fun)

    for type in four_component_float_vec_types:
        intrinsic = 'GetCoordinates'
        spirv_builtin = 'FragCoord'
        function_name = f'Pixel{intrinsic}_{type}'

        fun = Function( 
            decl_name = function_name,
            api_name = f'pixel{intrinsic}',
            return_type = type,
            documentation = 'Returns the post transformation coordinates of the current pixel. xy is the pixel position in non-normalized coordinates, z is the depth in the range [0, 1] or [-1, 1] based on the graphics API.',
            parameters = [
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
        spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 4);\n'
        spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32x4_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
        spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{function_name}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
        spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
        spirv_function += '    g->interfaceVariables.Insert(ret);\n'
        spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
        spirv_function += '    res.parentTypes.push_back(baseType);\n'
        spirv_function += '    return res;\n'
        
        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'GetFrontFacing'
    spirv_builtin = 'FrontFacing'
    function_name = f'Pixel{intrinsic}'
    fun = Function(
        decl_name = function_name,
        api_name = f'pixel{intrinsic}',
        return_type = 'Bool8',
        documentation = 'Returns the facing direction of the current pixel',
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Bool8, 1);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_b8_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{function_name}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
    spirv_function += '    res.parentTypes.push_back(baseType);\n'
    spirv_function += '    return res;\n'
    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'GetDepth'
    spirv_builtin = 'FragDepth'
    function_name = f'Pixel{intrinsic}'
    fun = Function( 
        decl_name = function_name,
        api_name = f'pixel{intrinsic}',
        return_type = 'Float32',
        documentation = 'Returns the depth value for the current pixel',
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 1);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{function_name}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
    spirv_function += '    res.parentTypes.push_back(baseType);\n'
    spirv_function += '    return res;\n'
    

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'SetDepth'
    function_name = f'Pixel{intrinsic}'
    fun = Function( 
        decl_name = function_name,
        api_name = f'pixel{intrinsic}',
        return_type = 'Void',
        documentation = 'Sets the depth value for the current pixel',
        parameters = [
            Variable(decl_name = f'{function_name}_val', api_name = "val", type_name='Float32')
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, 1);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_f32_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{function_name}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
    spirv_function += '    g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ret}, loaded);\n'
    spirv_function += '    return SPIRVResult::Invalid();\n' 
    

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'ExportColor'
    for type in scalar_types:
        for idx in index_types:
            function_name = f'Pixel{intrinsic}_{type}_{idx}'
            color_argument_name = f'{function_name}_color'
            index_argument_name = f'{function_name}_index'

            fun = Function( 
                decl_name = function_name,
                api_name = f'pixel{intrinsic}',
                return_type = 'Void',
                documentation = f'Exports a color value to the framebuffer output at index. This is the same as writing to `SV_TARGET<index>` in HLSL or to a GLSL `layout(location = <index>)` out variable.',
                parameters = [
                    Variable(decl_name = color_argument_name, api_name = "color", type_name=type),
                    Variable(decl_name = index_argument_name, api_name = "index", type_name=idx, literal=True)
                ]
            )

            spirv_function = ''
            spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
            spirv_function += f'    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::{base_type_mapping[type]}, {vector_size_mapping[type]});\n'
            spirv_function += f'    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_{data_type_mapping[type]}_Output"), OpTypePointer, VariableStorage::Output, SPVArg(baseType));\n'
            spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{function_name}", "_", args[1].literalValue.i), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Output);\n'
            spirv_function += '    g->writer->Decorate(SPVArg{ret}, Decorations::Index, args[1].literalValue.i);\n'
            spirv_function += '    g->writer->Decorate(SPVArg{ret}, Decorations::Location, args[1].literalValue.i);\n'
            spirv_function += '    g->interfaceVariables.Insert(ret);\n'
            spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
            spirv_function += '    g->writer->Instruction(OpStore, SPVWriter::Section::LocalFunction, SPVArg{ret}, loaded);\n'
            spirv_function += '    return SPIRVResult::Invalid();\n' 
            

            fun.spirv = spirv_function
            functions.append(fun)

    # TODO: Hmm maybe these should all be UInt16 considering dispatch sizes can't exceed 65535?????
    intrinsics = ['GetLocalThreadIndices', 'GetGlobalThreadIndices', 'GetWorkgroupIndices', 'GetWorkGroupDimensions']
    docs = [
        'Returns the local thread indices within the workgroup',
        'Returns the global thread indices in the dispatch',
        'Returns the workgroup indices in the dispatch',
        'Returns the dimensions of the workgroup in the dispatch'
    ]
    compute_builtin_getters_spirv = ['LocalInvocationId', 'GlobalInvocationId', 'WorkgroupId', 'WorkgroupSize']
    for intrinsic, spirv_builtin, doc in zip(intrinsics, compute_builtin_getters_spirv, docs):
        function_name = f'Compute{intrinsic}'

        if intrinsic == 'GetLocalThreadIndices':
            doc = 'Returns the local thread indices within the workgroup'
        elif intrinsic == 'GetGlobalThreadIndices':
            doc = 'Returns the global thread indices in the dispatch'
        elif intrinsic == 'GetWorkgroupIndices':
            doc = 'Returns the workgroup indices in the dispatch'
        elif intrinsic == 'GetWorkGroupDimensions':
            doc = 'Returns the dimensions of the workgroup in the dispatch'
        fun = Function( 
            decl_name = function_name,
            api_name = f'compute{intrinsic}',
            return_type = 'UInt32x3',
            documentation = doc,
            parameters = [
            ]
        )

        spirv_function = ''
        spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 3);\n'
        spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32x3_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
        spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{function_name}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
        spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
        spirv_function += '    g->interfaceVariables.Insert(ret);\n'
        spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
        spirv_function += '    res.parentTypes.push_back(baseType);\n'
        spirv_function += '    return res;\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'GetIndexInWorkgroup'
    spirv_builtin = 'LocalInvocationIndex'
    function_name = f'Compute{intrinsic}'

    fun = Function( 
        decl_name = function_name,
        api_name = f'compute{intrinsic}',
        return_type = 'UInt32',
        documentation = 'Returns flattened index of the current thread in the workgroup using (x * workgroupSize.y + y) * workgroupSize.x + z',
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 1);\n'
    spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
    spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{function_name}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
    spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
    spirv_function += '    g->interfaceVariables.Insert(ret);\n'
    spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
    spirv_function += '    res.parentTypes.push_back(baseType);\n'
    spirv_function += '    return res;\n'
    

    fun.spirv = spirv_function
    functions.append(fun)

    subgroup_builtin_getters = ['GetId', 'GetSize', 'GetNum']
    subgroup_builtin_getters_spirv = ['SubgroupId', 'SubgroupSize', 'NumSubgroups']
    subgroup_builtin_getters_docs = [
        'Returns the subgroup ID of the current thread',
        'Returns the size of the subgroup',
        'Returns the number of subgroups in the workgroup'
    ]
    for intrinsic, spirv_builtin, doc in zip(subgroup_builtin_getters, subgroup_builtin_getters_spirv, subgroup_builtin_getters_docs):
        function_name = f'Subgroup{intrinsic}'

        fun = Function( 
            decl_name = function_name,
            api_name = f'subgroup{intrinsic}',
            return_type = 'UInt32x3',
            documentation = doc,
            parameters = [
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniform);\n'
        spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 3);\n'
        spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32x3_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
        spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{function_name}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
        spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
        spirv_function += '    g->interfaceVariables.Insert(ret);\n'
        spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
        spirv_function += '    res.parentTypes.push_back(baseType);\n'
        spirv_function += '    return res;\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    subgroup_builtin_masks = ['GetThreadMask', 'GetThreadAndLowerMask', 'GetLowerMask', 'GetThreadAndGreaterMask', 'GetGreaterMask']
    subgroup_builtin_masks_spirv = ['SubgroupEqMask', 'SubgroupLeMask', 'SubgroupLtMask', 'SubgroupGeMask', 'SubgroupGtMask']
    subgroup_builtin_docs = [
        'Returns a 128 bit subgroup mask where the current thread is active',
        'Returns a 128 bit subgroup mask where the current thread and all lower threads are active',
        'Returns a 128 bit subgroup mask where all lower threads are active',
        'Returns a 128 bit subgroup mask where the current thread and all greater threads are active',
        'Returns a 128 bit subgroup mask where all greater threads are active'
    ]
    for intrinsic, spirv_builtin, doc in zip(subgroup_builtin_masks, subgroup_builtin_masks_spirv, subgroup_builtin_docs):
        function_name = f'Subgroup{intrinsic}'

        fun = Function( 
            decl_name = function_name,
            api_name = f'subgroup{intrinsic}',
            return_type = 'UInt32x4',
            documentation = doc,
            parameters = [
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniform);\n'
        spirv_function += '    uint32_t baseType = GeneratePODTypeSPIRV(c, g, TypeCode::UInt32, 4);\n'
        spirv_function += '    uint32_t typePtr = GPULang::AddType(g, TStr("ptr_u32x4_Input"), OpTypePointer, VariableStorage::Input, SPVArg(baseType));\n'
        spirv_function += f'    uint32_t ret = GPULang::AddSymbol(g, TStr("gpl{function_name}"), SPVWriter::Section::Declarations, OpVariable, typePtr, VariableStorage::Input);\n'
        spirv_function += f'    g->writer->Decorate(SPVArg{{ret}}, Decorations::BuiltIn, Builtins::{spirv_builtin});\n'
        spirv_function += '    g->interfaceVariables.Insert(ret);\n'
        spirv_function += '    SPIRVResult res(ret, typePtr, false, false, SPIRVResult::Storage::Input);\n'
        spirv_function += '    res.parentTypes.push_back(baseType);\n'
        spirv_function += '    return res;\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    
    intrinsic = 'GetFirstActiveThread'
    function_name = f'Subgroup{intrinsic}'

    fun = Function( 
        decl_name = function_name,
        api_name = f'subgroup{intrinsic}',
        return_type = 'UInt32',
        documentation= 'Returns the ID of the first active thread in the subgroup. If no threads are active, returns 0.',
        parameters = [
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniform);\n'
    spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformElect, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup);\n'
    spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
    

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'BroadcastFirstActiveThread'
    for type in scalar_types:
        function_name = f'Subgroup{intrinsic}_{type}'
        value_argument_name = f'{function_name}_value'
        fun = Function( 
            decl_name = function_name,
            api_name = f'subgroup{intrinsic}',
            return_type = type,
            documentation = 'Returns the value of the provided argument for the first active thread in the subgroup',
            parameters = [
                Variable(decl_name = value_argument_name, api_name = "value", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniformBallot);\n'
        spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBroadcastFirst, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, loaded);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsics = ['Ballot', 'InverseBallot']
    subgroup_ballot_ops_spirv = ['GroupNonUniformBallot', 'GroupNonUniformInverseBallot']
    docs = [
        'Constructs a 128 bit subgroup thread mask to the value of the predicate argument for each active thread',
        'Constructs a 128 bit subgroup thread mask to the inverse of the value of the predicate argument for each active thread'
    ]
    for intrinsic, doc, spirv in zip(intrinsics, docs, subgroup_ballot_ops_spirv):
        function_name = f'Subgroup{intrinsic}'
        predicate_argument_name = f'{function_name}_predicate'

        if intrinsic == 'Ballot':
            doc = 'Constructs a 128 bit subgroup thread mask to the value of the predicate argument for each active thread'
        if intrinsic == 'InverseBallot':
            doc = 'Constructs a 128 bit subgroup thread mask to the inverse of the value of the predicate argument for each active thread'
        fun = Function( 
            decl_name = function_name,
            api_name = f'subgroup{intrinsic}',
            return_type = 'UInt32x4',
            documentation = doc,
            parameters = [
                Variable(decl_name = predicate_argument_name, api_name = "predicate", type_name='Bool8')
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniformBallot);\n'
        spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(Op{spirv}, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, loaded);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'BallotBitCount'
    function_name = f'Subgroup{intrinsic}'
    mask_name = f'{function_name}_mask'
    fun = Function( 
        decl_name = function_name,
        api_name = f'subgroup{intrinsic}',
        return_type = 'UInt32',
        documentation = 'Returns the number of bits set to 1 in a 128 bit subgroup thread mask',
        parameters = [
            Variable(decl_name = mask_name, api_name = "value", type_name='UInt32x4')
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniformBallot);\n'
    spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
    spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBallotBitCount, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, 0, loaded);\n'
    spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
    

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'BallotFirstOne'
    function_name = f'Subgroup{intrinsic}'
    mask_name = f'{function_name}_mask'
    fun = Function( 
        decl_name = function_name,
        api_name = f'subgroup{intrinsic}',
        return_type = 'UInt32',
        documentation = 'Returns the first one (ctz) in a 128 bit subgroup thread mask',
        parameters = [
            Variable(decl_name = mask_name, api_name = "value", type_name='UInt32x4')
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniformBallot);\n'
    spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
    spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBallotFindLSB, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, loaded);\n'
    spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
    

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'BallotLastOne'
    function_name = f'Subgroup{intrinsic}'
    mask_name = f'{function_name}_mask'
    fun = Function( 
        decl_name = function_name,
        api_name = f'subgroup{intrinsic}',
        return_type = 'UInt32',
        documentation = 'Returns the last one (clz) in a 128 bit subgroup thread mask',
        parameters = [
            Variable(decl_name = mask_name, api_name = "value", type_name='UInt32x4')
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniformBallot);\n'
    spirv_function += '    SPIRVResult loaded = LoadValueSPIRV(c, g, args[0]);\n'
    spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBallotFindMSB, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, loaded);\n'
    spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
    

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsic = 'BallotBit'
    function_name = f'Subgroup{intrinsic}'
    mask_name = f'{function_name}_mask'
    index_name = f'{function_name}_index'
    fun = Function( 
        decl_name = function_name,
        api_name = f'subgroup{intrinsic}',
        return_type = 'UInt32',
        documentation = 'Extracts a specific bit from a 128 bit subgroup thread mask',
        parameters = [
            Variable(decl_name = mask_name, api_name = "mask", type_name='UInt32x4'),
            Variable(decl_name = index_name, api_name = "index", type_name='UInt32', literal=True)
        ]
    )

    spirv_function = ''
    spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniformBallot);\n'
    spirv_function += '    SPIRVResult mask = LoadValueSPIRV(c, g, args[0]);\n'
    spirv_function += '    SPIRVResult bit = LoadValueSPIRV(c, g, args[1]);\n'
    spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformBallotBitExtract, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, mask, bit);\n'
    spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
    

    fun.spirv = spirv_function
    functions.append(fun)

    intrinsics = ['SwapDiagonal', 'SwapVertical', 'SwapHorizontal']
    docs = [
        'Swaps the value at the current thread with the value at the diagonal thread in the subgroup',
        'Swaps the value at the current thread with the value at the vertical thread in the subgroup',
        'Swaps the value at the current thread with the value at the horizontal thread in the subgroup'
    ]
    subgroup_swap_op_directions = [2, 1, 0]
    for intrinsic, direction, doc in zip(intrinsics, subgroup_swap_op_directions, docs):
        for type in scalar_types:
            function_name = f'Subgroup{intrinsic}_{type}'
            value_argument_name = f'{function_name}_value'


            fun = Function( 
                decl_name = function_name,
                api_name = f'subgroup{intrinsic}',
                return_type = type,
                documentation = doc,
                parameters = [
                    Variable(decl_name = value_argument_name, api_name = "value", type_name=type)
                ]
            )

            spirv_function = ''
            spirv_function += '    g->writer->Capability(Capabilities::GroupNonUniformQuad);\n'
            spirv_function += '    SPIRVResult mask = LoadValueSPIRV(c, g, args[0]);\n'
            spirv_function += f'    SPIRVResult direction = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int({direction}));\n'
            spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpGroupNonUniformQuadSwap, SPVWriter::Section::LocalFunction, returnType, ExecutionScopes::Subgroup, mask, direction);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    # Atomics
    atomic_types = ['UInt32', 'Int32', 'UInt16', 'Int16', 'Float32', 'Float16']
    intrinsics = ['Load', 'Increment', 'Decrement']
    atomic_builtins_spirv = ['OpAtomicLoad', 'OpAtomicIIncrement', 'OpAtomicIDecrement']
    docs = [
        'Loads the value at the pointer location with the specified memory semantics',
        'Increments the value at the pointer location with the specified memory semantics, returns the old value',
        'Decrements the value at the pointer location with the specified memory semantics, returns the old value'
    ]

    for type in atomic_types:
        for intrinsic, spirv_builtin, doc in zip(intrinsics, atomic_builtins_spirv, docs):
            function_name = f'Atomic{intrinsic}_{type}'
            ptr_argument_name = f'{function_name}_ptr'
            semantics_argument_name = f'{function_name}_semantics'

            if type.startswith('Float') and intrinsic not in ['Load']:
                continue

            if intrinsic == 'Load':
                doc = 'Loads the value at the pointer location with the specified memory semantics'
            elif intrinsic == 'Increment':
                doc = 'Increments the value at the pointer location with the specified memory semantics, returns the old value'
            elif intrinsic == 'Decrement':
                doc = 'Decrements the value at the pointer location with the specified memory semantics, returns the old value'
            fun = Function( 
                decl_name = function_name,
                api_name = f'atomic{intrinsic}',
                return_type = type,
                documentation = doc,
                parameters = [
                    Variable(decl_name = ptr_argument_name, api_name = "ptr", type_name=type, pointer=True),
                    Variable(decl_name = semantics_argument_name, api_name = "semantics", type_name='MemorySemantics', literal=True)
                ]
            )

            spirv_function = ''
            spirv_function += '    uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);\n'
            spirv_function += '    uint32_t scope = ScopeToAtomicScope(args[0].scope);\n'
            spirv_function += '    uint32_t semantics = MemorySemanticsToSPIRV(args[1].literalValue.ui);\n'
            spirv_function += '    semantics |= ScopeToMemorySemantics(args[0].scope);\n'
            spirv_function += '    SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));\n'
            spirv_function += '    SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));\n'
            spirv_function += f'    uint32_t ret = g->writer->MappedInstruction({spirv_builtin}, SPVWriter::Section::LocalFunction, returnType, ptr, scopeId, semanticsId);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    intrinsics = ['Store', 'Exchange', 'Add', 'Subtract', 'And', 'Or', 'Xor']
    atomic_builtins_spirv = ['Store', 'Exchange', 'Add', 'Sub', 'And', 'Or', 'Xor']
    docs = [
        'Stores the value at the pointer location with the specified memory semantics, returns the old value',
        'Exchanges the value at the pointer location with the specified value and memory semantics, returns the old value',
        'Adds the value to the pointer location with the specified memory semantics, returns the old value',
        'Subtracts the value from the pointer location with the specified memory semantics, returns the old value',
        'Performs a bitwise AND operation with the value at the pointer location with the specified memory semantics, returns the old value',
        'Performs a bitwise OR operation with the value at the pointer location with the specified memory semantics, returns the old value',
        'Performs a bitwise XOR operation with the value at the pointer location with the specified memory semantics, returns the old value'
    ]
    for type in atomic_types:
        for intrinsic, doc, spirv_intrinsic in zip(intrinsics, docs, atomic_builtins_spirv):
            function_name = f'Atomic{intrinsic}_{type}'
            ptr_argument_name = f'{function_name}_ptr'
            value_argument_name = f'{function_name}_value'
            semantics_argument_name = f'{function_name}_semantics'
            spirv_builtin = f'OpAtomic{spirv_intrinsic}'

            # Skip  float atomics for unspported operations
            if intrinsic not in ['Store', 'Exchange'] and type.startswith('Float'):
                continue
                
            if intrinsic in ['Add', 'Subtract']:
                spirv_builtin = f'OpAtomicI{spirv_intrinsic}'

            fun = Function( 
                decl_name = function_name,
                api_name = f'atomic{intrinsic}',
                return_type = type if intrinsic != 'Store' else 'Void',
                documentation = doc,
                parameters = [
                    Variable(decl_name = ptr_argument_name, api_name = "ptr", type_name=type, pointer=True),
                    Variable(decl_name = value_argument_name, api_name = "value", type_name=type),
                    Variable(decl_name = semantics_argument_name, api_name = "semantics", type_name='MemorySemantics', literal=True)
                ]
            )

            spirv_function = ''
            spirv_function += '    uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);\n'
            spirv_function += '    uint32_t scope = ScopeToAtomicScope(args[0].scope);\n'
            spirv_function += '    uint32_t semantics = MemorySemanticsToSPIRV(args[2].literalValue.ui);\n'
            spirv_function += '    semantics |= ScopeToMemorySemantics(args[0].scope);\n'
            spirv_function += '    SPIRVResult valueLoaded = LoadValueSPIRV(c, g, args[1]);\n'
            spirv_function += '    SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));\n'
            spirv_function += '    SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));\n'
            if intrinsic == 'Store':
                spirv_function += f'    g->writer->Instruction({spirv_builtin}, SPVWriter::Section::LocalFunction, ptr, scopeId, semanticsId, valueLoaded);\n'
                spirv_function += '    return SPIRVResult::Invalid();\n'
            else:
                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction({spirv_builtin}, SPVWriter::Section::LocalFunction, returnType, SPVArg(ptr), scopeId, semanticsId, valueLoaded);\n'
                spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    intrinsics = ['Min', 'Max']
    atomic_builtins_spirv = ['Min', 'Max']
    docs = [
        'Compares the value at the pointer with the comparand, and stores the smallest value in the pointer, returns old value',
        'Compares the value at the pointer with the comparand, and stores the largest value in the pointer, returns old value'
    ]
    for atomic_type in atomic_types:

        # Skip min/max for float types
        if atomic_type.startswith('Float'):
            continue

        for intrinsic, doc, spirv_intrinsic in zip(intrinsics, docs, atomic_builtins_spirv):

            if type.startswith('Int'):
                spirv_builtin = f'OpAtomicS{spirv_intrinsic}'
            else:
                spirv_builtin = f'OpAtomicU{spirv_intrinsic}'

            function_name = f'Atomic{intrinsic}_{atomic_type}'
            ptr_argument_name = f'{function_name}_ptr'
            compare_argument_name = f'{function_name}_compare'
            semantics_argument_name = f'{function_name}_semantics'
            fun = Function( 
                decl_name = function_name,
                api_name = f'atomic{intrinsic}',
                return_type = type,
                documentation = doc,
                parameters = [
                    Variable(decl_name = ptr_argument_name, api_name = "ptr", type_name=type, pointer=True),
                    Variable(decl_name = compare_argument_name, api_name = "compare", type_name=type),
                    Variable(decl_name = semantics_argument_name, api_name = "semantics", type_name='MemorySemantics', literal=True)
                ]
            )

            spirv_function = ''
            spirv_function += '    uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);\n'
            spirv_function += '    uint32_t scope = ScopeToAtomicScope(args[0].scope);\n'
            spirv_function += '    uint32_t semantics = MemorySemanticsToSPIRV(args[2].literalValue.ui);\n'
            spirv_function += '    semantics |= ScopeToMemorySemantics(args[0].scope);\n'
            spirv_function += '    SPIRVResult compare = LoadValueSPIRV(c, g, args[1]);\n'
            spirv_function += '    SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));\n'
            spirv_function += '    SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));\n'
            spirv_function += f'    uint32_t ret = g->writer->MappedInstruction({spirv_builtin}, SPVWriter::Section::LocalFunction, returnType, SPVArg(ptr), scopeId, semanticsId, compare);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    for atomic_type in atomic_types:

        # Skip compare exchange for float types
        if atomic_type.startswith('Float'):
            continue

        intrinsic = 'CompareExchange'
        function_name = f'Atomic{intrinsic}_{atomic_type}'
        ptr_argument_name = f'{function_name}_ptr'
        value_argument_name = f'{function_name}_value'
        compare_argument_name = f'{function_name}_compare'
        semantics_argument_name = f'{function_name}_semantics'
        fun = Function( 
            decl_name = function_name,
            api_name = f'atomic{intrinsic}',
            return_type = type,
            documentation = 'Compares the value at the pointer location with the comparison, and exchanges it with value if they are identical using the specified memory semantics, always returns the old value',
            parameters = [
                Variable(decl_name = ptr_argument_name, api_name = "ptr", type_name=type, pointer=True),
                Variable(decl_name = value_argument_name, api_name = "value", type_name=type),
                Variable(decl_name = compare_argument_name, api_name = "compare", type_name=type),
                Variable(decl_name = semantics_argument_name, api_name = "semantics", type_name='MemorySemantics', literal=True)
            ]
        )

        spirv_function = ''
        spirv_function += '    uint32_t ptr = AccessChainSPIRV(c, g, args[0].name, args[0].typeName, args[0].accessChain);\n'
        spirv_function += '    uint32_t scope = ScopeToAtomicScope(args[0].scope);\n'
        spirv_function += '    uint32_t semantics = MemorySemanticsToSPIRV(args[3].literalValue.ui);\n'
        spirv_function += '    semantics |= ScopeToMemorySemantics(args[0].scope);\n'
        spirv_function += '    SPIRVResult value = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    SPIRVResult compare = LoadValueSPIRV(c, g, args[2]);\n'
        spirv_function += '    SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(scope));\n'
        spirv_function += '    SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt(semantics));\n'
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction({spirv_builtin}, SPVWriter::Section::LocalFunction, returnType, SPVArg(ptr), scopeId, semanticsId, semanticsId, value, compare);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'Insert'
    for type in unsigned_types:
        function_name = f'Bit{intrinsic}_{type}'
        base_argument_name = f'{function_name}_base'
        value_argument_name = f'{function_name}_value'
        offset_argument_name = f'{function_name}_offset'
        count_argument_name = f'{function_name}_count'

        fun = Function( 
            decl_name = function_name,
            api_name = f'bit{intrinsic}',
            return_type = type,
            documentation = 'Insert bit into bitmask',
            parameters = [
                Variable(decl_name = base_argument_name, api_name = "base", type_name=type),
                Variable(decl_name = value_argument_name, api_name = "value", type_name=type),
                Variable(decl_name = offset_argument_name, api_name = "offset", type_name=type),
                Variable(decl_name = count_argument_name, api_name = "count", type_name=type)
            ]
        )

        spirv_function = ''
        spirv_function += '    SPIRVResult base = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult value = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    SPIRVResult offset = LoadValueSPIRV(c, g, args[2]);\n'
        spirv_function += '    SPIRVResult count = LoadValueSPIRV(c, g, args[3]);\n'
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpBitFieldInsert, SPVWriter::Section::LocalFunction, returnType, base, value, offset, count);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'Extract'
    for type in types:
        function_name = f'Bit{intrinsic}_{type}'
        base_argument_name = f'{function_name}_base'
        offset_argument_name = f'{function_name}_offset'
        count_argument_name = f'{function_name}_count'

        if not type.startswith('UInt') and not type.startswith('Int'):
            continue

        fun = Function( 
            decl_name = function_name,
            api_name = f'bit{intrinsic}',
            return_type = type,
            documentation = 'Extract a specific bit from a bitmask',
            parameters = [
                Variable(decl_name = base_argument_name, api_name = "base", type_name=type),
                Variable(decl_name = offset_argument_name, api_name = "offset", type_name=type),
                Variable(decl_name = count_argument_name, api_name = "count", type_name=type)
            ]
        )

        if type.startswith('UInt'):
            spirv_op = "OpBitFieldUExtract"
        elif type.startswith('Int'):
            spirv_op = "OpBitFieldSExtract"
        spirv_function = ''
        spirv_function += '    SPIRVResult base = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult offset = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += '    SPIRVResult count = LoadValueSPIRV(c, g, args[2]);\n'
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction({spirv_op}, SPVWriter::Section::LocalFunction, returnType, base, offset, count);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsics = ['Reverse', 'Count']
    docs = [
        'Reverses the bits in a bitmask',
        'Counts the number of bits set to 1 in a bitmask'
    ]
    for intrinsic, doc in zip(intrinsics, docs):
        for type in types:
            function_name = f'Bit{intrinsic}_{type}'
            base_argument_name = f'{function_name}_base'

            if not type.startswith('UInt') and not type.startswith('Int'):
                continue

            fun = Function( 
                decl_name = function_name,
                api_name = f'bit{intrinsic}',
                return_type = type,
                documentation = doc,
                parameters = [
                    Variable(decl_name = base_argument_name, api_name = "base", type_name=type)
                ]
            )

            spirv_function = ''
            spirv_function += '    SPIRVResult base = LoadValueSPIRV(c, g, args[0]);\n'
            if intrinsic == 'Count':
                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpBitCount, SPVWriter::Section::LocalFunction, returnType, base);\n'
            elif intrinsic == 'Reverse':
                spirv_function += '    g->writer->Capability(Capabilities::BitInstructions);\n'
                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpBitReverse, SPVWriter::Section::LocalFunction, returnType, base);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    intrinsics = ['ExecutionBarrier', 'ExecutionBarrierSubgroup', 'ExecutionBarrierWorkgroup']
    docs = [
        'Execution barrier to ensure all threads have reached this point before proceeding',
        'Execution barrier to ensure all threads in the subgroup have reached this point before proceeding',
        'Execution barrier to ensure all threads in the workgroup have reached this point before proceeding'
    ]
    for intrinsic, doc in zip(intrinsics, docs):
        function_name = f'{intrinsic}'

        fun = Function( 
            decl_name = function_name,
            api_name = f'{intrinsic[0].lower() + intrinsic[1:]}',
            return_type = 'Void',
            documentation = doc,
            parameters = [
            ]
        )

        if intrinsic.endswith('Subgroup'):
            scope = '3'
            semantics = '0x2 | 0x80'
        elif intrinsic.endswith('Workgroup'):
            scope = '2'
            semantics = '0x2 | 0x100'
        else:
            scope = '2'
            semantics = '0x2 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800'
        spirv_function = ''
        spirv_function += f'    SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt({scope}));\n'
        spirv_function += f'    SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt({semantics}));\n'
        spirv_function += f'    g->writer->Instruction(OpControlBarrier, SPVWriter::Section::LocalFunction, scopeId, scopeId, semanticsId);\n'
        spirv_function += '    return SPIRVResult(0xFFFFFFFF, returnType);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    memory_barrier_intrinsics = ['MemoryBarrier', 'MemoryBarrierBuffer', 'MemoryBarrierTexture', 'MemoryBarrierAtomic', 'MemoryBarrierSubgroup', 'MemoryBarrierWorkgroup']
    for intrinsic in memory_barrier_intrinsics:
        function_name = f'{intrinsic}'

        doc = 'Memory barrier to ensure memory operations are completed before proceeding'
        if intrinsic.endswith('Buffer'):
            doc = 'Memory barrier to ensure buffer memory operations are completed before proceeding'
        elif intrinsic.endswith('Texture'):
            doc = 'Memory barrier to ensure texture memory operations are completed before proceeding'
        elif intrinsic.endswith('Atomic'):
            doc = 'Memory barrier to ensure atomic memory operations are completed before proceeding'
        elif intrinsic.endswith('Subgroup'):
            doc = 'Memory barrier to ensure subgroup memory operations are completed before proceeding'
        elif intrinsic.endswith('Workgroup'):
            doc = 'Memory barrier to ensure workgroup memory operations are completed before proceeding'
        fun = Function( 
            decl_name = function_name,
            api_name = f'{intrinsic[0].lower() + intrinsic[1:]}',
            return_type = 'Void',
            documentation = doc,
            parameters = [
            ]
        )

        if intrinsic.endswith('Buffer'):
            scope = '2'
            semantics = '0x2 | 0x40'
        elif intrinsic.endswith('Texture'):
            scope = '2'
            semantics = '0x2 | 0x800'
        elif intrinsic.endswith('Atomic'):
            scope = '2'
            semantics = '0x2 | 0x400'
        elif intrinsic.endswith('Subgroup'):
            scope = '3'
            semantics = '0x2 | 0x80'
        elif intrinsic.endswith('Workgroup'):
            scope = '2'
            semantics = '0x2 | 0x100'
        else:
            semantics = '0x2 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800'
        spirv_function = ''
        spirv_function += f'    SPIRVResult scopeId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt({scope}));\n'
        spirv_function += f'    SPIRVResult semanticsId = GenerateConstantSPIRV(c, g, ConstantCreationInfo::UInt({semantics}));\n'
        spirv_function += f'    g->writer->Instruction(OpMemoryBarrier, SPVWriter::Section::LocalFunction, scopeId, semanticsId);\n'
        spirv_function += '    return SPIRVResult(0xFFFFFFFF, returnType);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    texture_types_no_ms = ['Texture1D', 'Texture2D', 'Texture3D', 'TextureCube', 'Texture1DArray', 'Texture2DArray', 'TextureCubeArray']
    texture_types_ms = ['Texture2DMS', 'Texture2DMSArray']

    texture_types = texture_types_no_ms + texture_types_ms

    type = 'UInt32'
    texture_size_types = {
        'Texture1D': f"{type}",
        'Texture2D': f"{type}x2",
        'Texture3D': f"{type}x3",
        'TextureCube': f"{type}x3",
        'Texture1DArray': f"{type}x2",
        'Texture2DArray': f"{type}x3",
        'TextureCubeArray': f"{type}x4",
        'Texture2DMS': f"{type}x2",
        'Texture2DMSArray': f"{type}x3"
    }

    type = 'Int32'
    texture_denormalized_index_types = {
        'Texture1D': f"{type}",
        'Texture2D': f"{type}x2",
        'Texture3D': f"{type}x3",
        'TextureCube': f"{type}x3",
        'Texture1DArray': f"{type}x2",
        'Texture2DArray': f"{type}x3",
        'TextureCubeArray': f"{type}x4",
        'Texture2DMS': f"{type}x2",
        'Texture2DMSArray': f"{type}x3"
    }

    type = 'Float32'
    texture_float_index_types = {
        'Texture1D': f"{type}",
        'Texture2D': f"{type}x2",
        'Texture3D': f"{type}x3",
        'TextureCube': f"{type}x3",
        'Texture1DArray': f"{type}x2",
        'Texture2DArray': f"{type}x3",
        'TextureCubeArray': f"{type}x4",
        'Texture2DMS': f"{type}x2",
        'Texture2DMSArray': f"{type}x3"
    }

    texture_float_index_proj_sizes = {
        'Texture1D': 2,
        'Texture2D': 3,
        'Texture3D': 4,
        'TextureCube': 4,
        'Texture1DArray': 3,
        'Texture2DArray': 4,
        'Texture2DMS': 3,
        'Texture2DMSArray': 4
    }

    intrinsic = 'GetSize'
    for type in texture_types_no_ms:
        return_type = texture_size_types[type]
        function_name = f'Texture{intrinsic}_{type}'
        texture_argument_name = f'{function_name}_texture'

        fun = Function( 
            decl_name = function_name,
            api_name = f'texture{intrinsic}',
            return_type = return_type,
            documentation = 'Get the size of a texture',
            parameters = [
                Variable(decl_name = texture_argument_name, api_name = "texture", type_name=type, pointer=True, uniform=True, mutable=True, strict=True)
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::ImageQuery);\n'
        spirv_function += '    SPIRVResult texture = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageQuerySize, SPVWriter::Section::LocalFunction, returnType, texture);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'GetSizeMip'
    for type in texture_types_no_ms:
        return_type = texture_size_types[type]
        function_name = f'Texture{intrinsic}_{type}'
        texture_argument_name = f'{function_name}_texture'
        mip_argument_name = f'{function_name}_mip'

        fun = Function( 
            decl_name = function_name,
            api_name = f'texture{intrinsic}',
            return_type = return_type,
            documentation = 'Get the size of a texture at a specific mip level',
            parameters = [
                Variable(decl_name = texture_argument_name, api_name = "texture", type_name=type, pointer=True, uniform=True, strict=True),
                Variable(decl_name = mip_argument_name, api_name = "mip", type_name='UInt32')
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::ImageQuery);\n'
        spirv_function += '    SPIRVResult texture = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += '    SPIRVResult mip = LoadValueSPIRV(c, g, args[1]);\n'
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageQuerySizeLod, SPVWriter::Section::LocalFunction, returnType, texture, mip);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'GetMips'
    for type in texture_types_no_ms:
        function_name = f'Texture{intrinsic}_{type}'
        texture_argument_name = f'{function_name}_texture'

        fun = Function( 
            decl_name = function_name,
            api_name = f'texture{intrinsic}',
            return_type = type,
            documentation = 'Get the number of mips in a texture',
            parameters = [
                Variable(decl_name = texture_argument_name, api_name = "texture", type_name=type, pointer=True, uniform=True),
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::ImageQuery);\n'
        spirv_function += '    SPIRVResult texture = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageQueryLevels, SPVWriter::Section::LocalFunction, returnType, texture);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    intrinsic = 'GetSamples'
    for type in texture_types_ms:
        function_name = f'Texture{intrinsic}_{type}'
        texture_argument_name = f'{function_name}_texture'

        fun = Function( 
            decl_name = function_name,
            api_name = f'texture{intrinsic}',
            return_type = type,
            documentation = 'Get the number of samples in a multisampled texture',
            parameters = [
                Variable(decl_name = texture_argument_name, api_name = "texture", type_name=type, pointer=True, uniform=True),
            ]
        )

        spirv_function = ''
        spirv_function += '    g->writer->Capability(Capabilities::ImageQuery);\n'
        spirv_function += '    SPIRVResult texture = LoadValueSPIRV(c, g, args[0]);\n'
        spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageQuerySamples, SPVWriter::Section::LocalFunction, returnType, texture);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        

        fun.spirv = spirv_function
        functions.append(fun)

    # Helper function to generate a version of the texture sampling method both for combined texture-samplers and for textures with samplers provided separately.
    def generate_texture_intrinsic_base(intrinsic, type):

        sampled_args = [
            Variable(decl_name = f'Sampled{intrinsic}_texture', api_name = "texture", type_name=f'Sampled{type}', pointer=True, uniform=True)
        ]
        base_args = [
            Variable(decl_name = f'{intrinsic}_texture', api_name = "texture", type_name=type, pointer=True, uniform=True),
            Variable(decl_name = f'{intrinsic}_sampler', api_name = "sampler", type_name='Sampler', pointer=True, uniform=True)
        ]
        
        return [(base_args, ''), (sampled_args, 'Sampled')]

    intrinsic = 'GetSampledMip'
    for type in texture_types_no_ms:
        coordinate_type = texture_float_index_types[type]
        base_function_name = f'Texture{intrinsic}_{type}'

        for defs in generate_texture_intrinsic_base(base_function_name, type):
            args, prefix = defs
            function_name = f'{prefix}{base_function_name}'
            coordinate_argument_name = f'{function_name}_coordinate'

            fun = Function( 
                decl_name = function_name,
                api_name = f'texture{intrinsic}',
                return_type = 'Float32x2',
                documentation = 'Get the mip level of a texture at a specific coordinate. The return value is a vector where the first value is the mip level to sample, and the second is the offset relative to the base mip for which the sample would occur',
                parameters = args + [
                    Variable(decl_name = coordinate_argument_name, api_name = "coordinate", type_name=coordinate_type)
                ]
            )

            spirv_function = ''
            if prefix: # Prefix here is merely 'Sampled'
                spirv_function += '    SPIRVResult sampledImage = LoadValueSPIRV(c, g, args[0]);\n'
                spirv_function += '    SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);\n'
            else:
                spirv_function += '    SPIRVResult sampledImage = CreateSampledImageSPIRV(c, g, args[0], args[1]);\n'
                spirv_function += '    SPIRVResult coord = LoadValueSPIRV(c, g, args[2]);\n'
            spirv_function += '    g->writer->Capability(Capabilities::ImageQuery);\n'
            spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpImageQueryLod, SPVWriter::Section::LocalFunction, returnType, sampledImage, coord);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    texture_load_store_intrinsics = ['Load', 'LoadMip', 'Store', 'StoreMip']
    for type in texture_types:
        coordinate_type = texture_denormalized_index_types[type]
        for intrinsic in texture_load_store_intrinsics:
            hasMip = intrinsic in ['LoadMip', 'StoreMip']
            hasStore = intrinsic in ['Store', 'StoreMip']
            function_name = f'Texture{intrinsic}_{type}'
            texture_argument_name = f'{function_name}_texture'
            coordinate_argument_name = f'{function_name}_coordinate'

            if hasStore:
                doc = f'Store a single texel without using a sampler value at an absolute non-normalized coordinate'
            else:
                doc = f'Load a single texel without using a sampler value at an absolute non-normalized coordinate'
            if hasMip:
                doc += ' at a specific mip level'

            fun = Function( 
                decl_name = function_name,
                api_name = f'texture{intrinsic}',
                return_type = 'Float32x4' if not hasStore else 'Void',
                documentation = doc,
                parameters = [
                    Variable(decl_name = texture_argument_name, api_name = "texture", type_name=type, pointer=True, uniform=True, mutable=True),
                    Variable(decl_name = coordinate_argument_name, api_name = "coordinate", type_name=coordinate_type)
                ]
            )

            if hasMip:
                fun.parameters.append(
                    Variable(decl_name = f'{function_name}_mip', api_name = "mip", type_name='Int32')
                )
            if hasStore:
                fun.parameters.append(
                    Variable(decl_name = f'{function_name}_value', api_name = "value", type_name='Float32x4')
                )

            spirv_function = ''
            spirv_function += '    SPIRVResult texture = LoadValueSPIRV(c, g, args[0]);\n'
            spirv_function += '    SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);\n'
            if hasStore:
                spirv_function += '    SPIRVResult value = LoadValueSPIRV(c, g, args[2]);\n'
                if hasMip:
                    spirv_function += '    SPIRVResult mip = LoadValueSPIRV(c, g, args[3]);\n'
            else:
                if hasMip:
                    spirv_function += '    SPIRVResult mip = LoadValueSPIRV(c, g, args[2]);\n'

            if hasStore:
                spirv_function += '    uint32_t ret = 0xFFFFFFFF;\n'
                if hasMip:
                    spirv_function += f'    g->writer->Instruction(OpImageWrite, SPVWriter::Section::LocalFunction, texture, coord, value, ImageOperands::Lod, mip);\n'
                else:
                    spirv_function += f'    g->writer->Instruction(OpImageWrite, SPVWriter::Section::LocalFunction, texture, coord, value);\n'
            else:
                if hasMip:
                    spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageRead, SPVWriter::Section::LocalFunction, returnType, texture, coord, ImageOperands::Lod, mip);\n'
                else:
                    spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageRead, SPVWriter::Section::LocalFunction, returnType, texture, coord);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    texture_fetch_intrinsics = ['Fetch', 'FetchSample']
    for type in texture_types:

        if type.startswith('TextureCube'):
            continue

        for intrinsic in texture_fetch_intrinsics:
            function_name = f'Texture{intrinsic}_{type}'
            texture_argument_name = f'{function_name}_texture'
            coordinate_argument_name = f'{function_name}_coordinate'
            lod_argument_name = f'{function_name}_lod'

            doc = f'Fetch a single texel without using a sampler value at a coordinate'
            if intrinsic == 'FetchSample':
                doc += ' at a specific sample index'
            fun = Function( 
                decl_name = function_name,
                api_name = f'texture{intrinsic}',
                return_type = 'Float32x4',
                documentation = doc,
                parameters = [
                    Variable(decl_name = texture_argument_name, api_name = "texture", type_name=type, pointer=True, uniform=True),
                    Variable(decl_name = coordinate_argument_name, api_name = "coordinate", type_name=texture_denormalized_index_types[type]),
                    Variable(decl_name = lod_argument_name, api_name = "lod", type_name='UInt32')
                ]
            )
            if intrinsic == 'FetchSample':
                fun.parameters.append(
                    Variable(decl_name = f'{function_name}_sample', api_name = "sample", type_name='UInt32')
                )

            spirv_function = ''
            spirv_function += '    SPIRVResult texture = LoadValueSPIRV(c, g, args[0]);\n'
            spirv_function += '    SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);\n'
            spirv_function += '    SPIRVResult mip = LoadValueSPIRV(c, g, args[2]);\n'
            if intrinsic == 'FetchSample':
                spirv_function += '    SPIRVResult sample = LoadValueSPIRV(c, g, args[3]);\n'
                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageFetch, SPVWriter::Section::LocalFunction, returnType, texture, coord, ImageOperands::Lod, mip, ImageOperands::Sample, sample);\n'
            else:
                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageFetch, SPVWriter::Section::LocalFunction, returnType, texture, coord, ImageOperands::Lod, mip);\n'
            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
            

            fun.spirv = spirv_function
            functions.append(fun)

    texture_gather_intrinsics = ['Gather', 'GatherOffset']
    for type in texture_types_no_ms:

        if type.startswith('Texture1D') or type.startswith('Texture3D'):
            continue

        for intrinsic in texture_gather_intrinsics:
            base_function_name = f'Texture{intrinsic}_{type}'
            for defs in generate_texture_intrinsic_base(base_function_name, type):
                args, prefix = defs
                function_name = f'{prefix}{base_function_name}'
                coordinate_argument_name = f'{function_name}_coordinate'
                component_argument_name = f'{function_name}_component'

                doc = f'Gather 4 values of a quad at a coordinate where the component is the channel to read'
                if intrinsic == 'GatherOffset':
                    doc += ' with an offset applied to the coordinate'
                fun = Function( 
                    decl_name = function_name,
                    api_name = f'texture{intrinsic}',
                    return_type = 'Float32x4',
                    documentation = doc,
                    parameters = args + [
                        Variable(decl_name = coordinate_argument_name, api_name = "coordinate", type_name=texture_denormalized_index_types[type]),
                        Variable(decl_name = component_argument_name, api_name = "component", type_name='Int32')
                    ]
                )
                if intrinsic == 'GatherOffset':
                    fun.parameters.append(
                        Variable(decl_name = f'{function_name}_offset', api_name = "offset", type_name='UInt32')
                    )

                spirv_function = ''
                spirv_function += '    SPIRVResult texture = LoadValueSPIRV(c, g, args[0]);\n'
                spirv_function += '    SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);\n'
                spirv_function += '    SPIRVResult component = LoadValueSPIRV(c, g, args[2]);\n'
                if intrinsic == 'GatherOffset':
                    spirv_function += '    SPIRVResult offset = LoadValueSPIRV(c, g, args[3]);\n'
                    spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageFetch, SPVWriter::Section::LocalFunction, returnType, texture, coord, component, ImageOperands::Offset, offset);\n'
                else:
                    spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageFetch, SPVWriter::Section::LocalFunction, returnType, texture, coord, component);\n'
                spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                

                fun.spirv = spirv_function
                functions.append(fun)

    intrinsic = 'PixelCacheLoad'
    for type in ['PixelCache', 'PixelCacheMS']:
        function_name = f'Texture{intrinsic}_{type}'
        texture_argument_name = f'{function_name}_texture'
        coordinate_argument_name = f'{function_name}_coordinate'

        fun = Function( 
            decl_name = function_name,
            api_name = f'texture{intrinsic}',
            return_type = type,
            documentation = 'Load a pixel value from a previous thread',
            parameters = [
                Variable(decl_name = texture_argument_name, api_name = "texture", type_name=type, pointer=True, uniform=True)
            ]
        )

        if type.endswith('MS'):
            fun.parameters.append(
                Variable(decl_name = f'{function_name}_sample', api_name = "sample", type_name='UInt32')
            )

        spirv_function = ''
        spirv_function += '    SPIRVResult texture = LoadValueSPIRV(c, g, args[0]);\n'
        if type.endswith('MS'):
            spirv_function += '    SPIRVResult sample = LoadValueSPIRV(c, g, args[1]);\n'
            spirv_function += '    SPIRVResult coord = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float32(0), 2);\n'
            spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageRead, SPVWriter::Section::LocalFunction, returnType, texture, coord, ImageOperands::Sample, sample);\n'
        else:
            spirv_function += '    SPIRVResult coord = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Float32(0), 2);\n'
            spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageRead, SPVWriter::Section::LocalFunction, returnType, texture, coord);\n'
        spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
        fun.spirv = spirv_function
        functions.append(fun)
        

    intrinsic = 'Sample'
    lod_modifiers = ['', 'Lod', 'Grad', 'Bias']
    compare_modifiers = ['', 'Compare']
    projection_modifiers = ['', 'Proj']
    offset_modifiers = ['', 'Offset']
    for lod in lod_modifiers:
        for comp in compare_modifiers:
            for proj in projection_modifiers:
                for offset in offset_modifiers:
                    for type in texture_types_no_ms:
                        base_function_name = f'Texture{intrinsic}{lod}{proj}{comp}{offset}_{type}'

                        for defs in generate_texture_intrinsic_base(base_function_name, type):
                            args, prefix = defs


                            if comp == 'Compare' and type.startswith('TextureCube'):
                                continue

                            if proj == 'Proj' and type.endswith('Array'):
                                continue

                            if proj == 'Proj' and type.startswith('TextureCube'):
                                continue

                            if offset == 'Offset' and type.startswith('TextureCube'):
                                continue

                            function_name = f'{prefix}{base_function_name}'
                            coordinate_type = texture_float_index_types[type]
                            coordinate_argument_name = f'{function_name}_coordinate'

                            doc = 'Sample a texture at coordinate'
                            if lod == 'Lod':
                                doc += ' with explicit level of detail'
                            elif lod == 'Grad':
                                doc += ' with explicit gradient'
                            elif lod == 'Bias':
                                doc += ' with a mip bias'
                            if proj == 'Proj':
                                doc += ' with projection'
                            if comp == 'Compare':
                                doc += ' with depth comparison'
                            if offset == 'Offset':
                                doc += ' with an offset applied to the coordinate'

                            fun = Function( 
                                decl_name = function_name,
                                api_name = f'texture{intrinsic}{lod}{proj}{comp}{offset}',
                                return_type = 'Float32x4' if not comp else 'Float32',
                                documentation = doc,
                                parameters = args + [
                                    Variable(decl_name = coordinate_argument_name, api_name = "coordinate", type_name=coordinate_type),
                                ]
                            )
                            if lod == 'Lod':
                                fun.parameters.append(
                                    Variable(decl_name = f'{function_name}_lod', api_name = "lod", type_name='Float32')
                                )
                            elif lod == 'Grad':
                                fun.parameters += [
                                    Variable(decl_name = f'{function_name}_grad_x', api_name = "grad_x", type_name=coordinate_type),
                                    Variable(decl_name = f'{function_name}_grad_y', api_name = "grad_y", type_name=coordinate_type)
                                ]
                            elif lod == 'Bias':
                                fun.parameters.append(
                                    Variable(decl_name = f'{function_name}_bias', api_name = "bias", type_name='Float32')
                                )
                            if proj == 'Proj':
                                fun.parameters.append(
                                    Variable(decl_name = f'{function_name}_proj', api_name = "proj", type_name='Float32')
                                )
                            if comp == 'Compare':
                                fun.parameters.append(
                                    Variable(decl_name = f'{function_name}_compare', api_name = "compare", type_name='Float32')
                                )
                            if offset == 'Offset':
                                fun.parameters.append(
                                    Variable(decl_name = f'{function_name}_offset', api_name = "offset", type_name=coordinate_type)
                                )
                         
                            spirv_function = ''
                            spirv_function += '    g->writer->Capability(Capabilities::Shader);\n'
                            if prefix: # Prefix here is merely 'Sampled'
                                spirv_function += '    SPIRVResult sampledImage = LoadValueSPIRV(c, g, args[0]);\n'
                                spirv_function += '    SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);\n'
                            else:
                                spirv_function += '    SPIRVResult sampledImage = CreateSampledImageSPIRV(c, g, args[0], args[1]);\n'
                                spirv_function += '    SPIRVResult coord = LoadValueSPIRV(c, g, args[2]);\n'
                            spirv_args = []
                            if prefix:
                                spirv_arg_counter = 2 # Sampled image + coordinate
                            else:
                                spirv_arg_counter = 3 # Texture + sampler + coordinate

                            spirv_proj = '' 
                            if proj:
                                spirv_proj = 'Proj'
                                spirv_function += f'    SPIRVResult proj = LoadValueSPIRV(c, g, args[{spirv_arg_counter}]);\n'
                                spirv_arg_counter += 1
                                spirv_function += f'    uint32_t vectorType = GeneratePODTypeSPIRV(c, g, TypeCode::Float32, {texture_float_index_proj_sizes[type]});\n'
                                spirv_function += f'    coord = GenerateCompositeSPIRV(c, g, vectorType, {{ coord, proj }});\n'
                            spirv_compare = ''
                            if comp:
                                spirv_compare = 'Dref'
                                spirv_function += f'    SPIRVResult compare = LoadValueSPIRV(c, g, args[{spirv_arg_counter}]);\n'
                                spirv_arg_counter += 1
                                spirv_args.append('compare')
                            spirv_lod = 'ImplicitLod'
                            if lod == 'Lod' or lod == 'Grad':
                                spirv_lod = 'ExplicitLod'
                                if lod == 'Lod':
                                    spirv_function += f'    SPIRVResult lod = LoadValueSPIRV(c, g, args[{spirv_arg_counter}]);\n'
                                    spirv_arg_counter += 1
                                    spirv_args.append('ImageOperands::Lod, lod')
                                elif lod == 'Grad':
                                    spirv_function += f'    SPIRVResult gradX = LoadValueSPIRV(c, g, args[{spirv_arg_counter}]);\n'
                                    spirv_arg_counter += 1
                                    spirv_function += f'    SPIRVResult gradY = LoadValueSPIRV(c, g, args[{spirv_arg_counter}]);\n'
                                    spirv_arg_counter += 1
                                    spirv_args.append('ImageOperands::Grad, gradX, gradY')
                            elif lod == 'Bias':
                                spirv_function += f'    SPIRVResult bias = LoadValueSPIRV(c, g, args[{spirv_arg_counter}]);\n'
                                spirv_arg_counter += 1
                                spirv_args.append('ImageOperands::Bias, bias')
                            if offset:
                                spirv_function += '    g->writer->Capability(Capabilities::ImageQuery);\n'
                                spirv_function += f'    SPIRVResult offset = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int32(0), 2);\n'
                                spirv_args.append('ImageOperands::Offset, offset')

                            if spirv_args:
                                spirv_arg_list = ', '.join(spirv_args)
                                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageSample{spirv_proj}{spirv_compare}{spirv_lod}, SPVWriter::Section::LocalFunction, returnType, sampledImage, coord, {spirv_arg_list});\n'
                            else:
                                spirv_function += f'    uint32_t ret = g->writer->MappedInstruction(OpImageSample{spirv_proj}{spirv_compare}{spirv_lod}, SPVWriter::Section::LocalFunction, returnType, sampledImage, coord);\n'
                            spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                            

                            fun.spirv = spirv_function
                            functions.append(fun)

    # TODO: Implement correctly with different types of textures and value combinations
    intrinsics = ['AtomicLoad', 'AtomicStore', 'AtomicExchange', 'AtomicCompareExchange', 'AtomicAdd', 'AtomicSubtract', 'AtomicMin', 'AtomicMax', 'AtomicAnd', 'AtomicOr', 'AtomicXor']
    for intrinsic in intrinsics:
        for tex_type in texture_types_no_ms:
            for scalar_type in types:

                # Skip atomic operations on textures with float components      
                if scalar_type.startswith('Float'):
                    if intrinsic not in ['AtomicLoad', 'AtomicStore', 'AtomicExchange']:
                        continue
                elif scalar_type.startswith('Bool'): 
                    continue
                    
                function_name = f'Texture{intrinsic}_{tex_type}_{scalar_type}'
                texture_argument_name = f'{function_name}_texture'
                coordinate_argument_name = f'{function_name}_coordinate'
                value_argument_name = f'{function_name}_value'

                fun = Function( 
                    decl_name = function_name,
                    api_name = f'texture{intrinsic}',
                    return_type = scalar_type,
                    documentation = 'Get a pointer to a texel. This enables it to be used for atomic operations.',
                    parameters = [
                        Variable(decl_name = texture_argument_name, api_name = "texture", type_name=tex_type, pointer=True, uniform=True),
                        Variable(decl_name = coordinate_argument_name, api_name = "coordinate", type_name=texture_denormalized_index_types[tex_type]),
                        Variable(decl_name = value_argument_name, api_name = "value", type_name=scalar_type)
                    ]
                )

                if intrinsic == 'AtomicLoad':
                    spirv_op = 'OpAtomicLoad'
                elif intrinsic == 'AtomicStore':
                    spirv_op = 'OpAtomicStore'
                elif intrinsic == 'AtomicExchange':
                    spirv_op = 'OpAtomicExchange'
                elif intrinsic == 'AtomicCompareExchange':
                    spirv_op = 'OpAtomicCompareExchange'
                elif intrinsic == 'AtomicAdd':
                    spirv_op = 'OpAtomicIAdd'
                elif intrinsic == 'AtomicSubtract':
                    spirv_op = 'OpAtomicISub'
                elif intrinsic == 'AtomicMin':
                    if scalar_type.startswith('Int'):
                        spirv_op = 'OpAtomicSMin'
                    else:
                        spirv_op = 'OpAtomicUMin'
                elif intrinsic == 'AtomicMax':
                    if scalar_type.startswith('Int'):
                        spirv_op = 'OpAtomicSMax'
                    else:
                        spirv_op = 'OpAtomicUMax'
                elif intrinsic == 'AtomicAnd':
                    spirv_op = 'OpAtomicAnd'
                elif intrinsic == 'AtomicOr':
                    spirv_op = 'OpAtomicOr'
                elif intrinsic == 'AtomicXor':
                    spirv_op = 'OpAtomicXor'
    
                spirv_function = ''
                spirv_function += '    SPIRVResult texture = LoadValueSPIRV(c, g, args[0]);\n'
                spirv_function += '    SPIRVResult coord = LoadValueSPIRV(c, g, args[1]);\n'
                spirv_function += '    SPIRVResult value = LoadValueSPIRV(c, g, args[2]);\n'
                spirv_function += '    SPIRVResult sample = GenerateConstantSPIRV(c, g, ConstantCreationInfo::Int32(0));\n'
                spirv_function += f'    g->typeState.storage = SPIRVResult::Storage::Image;\n';
                spirv_function += f'    SPIRVResult type = GenerateTypeSPIRV(c, g, {texture_argument_name}.type, Symbol::Resolved(&{texture_argument_name})->typeSymbol);\n'
                spirv_function += f'    g->typeState.storage = SPIRVResult::Storage::Function;\n';
                spirv_function += '    uint32_t ret = g->writer->MappedInstruction(OpImageTexelPointer, SPVWriter::Section::LocalFunction, type.typeName, texture, coord, sample);\n'
                spirv_function += f'    ret = g->writer->MappedInstruction({spirv_op}, SPVWriter::Section::LocalFunction, returnType, SPVArg(ret), value);\n'
                spirv_function += '    return SPIRVResult(ret, returnType, true);\n'
                
                fun.spirv = spirv_function
                functions.append(fun)


    spirv_code = ''
    for fun in functions:
        spirv_code += fun.spirv_code() + '\n'
    spirv_intrinsics.write(spirv_code[0:-2])
    spirv_intrinsics.write('\n\n')
    spirv_intrinsics.write('using SPIRVIntrinsic = SPIRVResult(*)(const Compiler*, SPIRVGenerator*, uint32_t, const std::vector<SPIRVResult>&);\n')
    spirv_intrinsics.write(f'constexpr std::array<SPIRVIntrinsic, {len(spirv_intrinsic_list)}> SPIRVDefaultIntrinsics = {{\n')
    spirv_intrinsics.write(f",\n".join(f'    {str["code"]} /* {i} -> {str["idx"]} */' for i, str in enumerate(spirv_intrinsic_list)))
    #spirv_intrinsics.write(",\n".join(spirv_intrinsic_list))
    spirv_intrinsics.write('\n};\n')
    #spirv_intrinsics.write('} // namespace GPULang\n\n')
    
    intrinsic_decls = ''
    intrinsic_defs = ''
    intrinsic_def_fragments = []
    intrinsic_def_fragment = ''
    intrinsic_setup_fragments = []
    intrinsic_setup_fragment = ''
    intrinsic_setup = ''
    class WebIntrinsic:
        def __init__(self, name, initial_function):
            self.name = name
            self.overload = [initial_function]

    web_intrinsic_list = []
    web_intrinsic_set = dict()
    counter = 0
    for fun in functions:
        if not fun.is_member:
            intrinsic_decls += fun.declaration()
            intrinsic_defs += fun.definition()
            intrinsic_setup += fun.setup()
            intrinsic_def_fragment += fun.definition()
            intrinsic_setup_fragment += fun.setup()
            if counter >= 128:
                intrinsic_def_fragments.append(intrinsic_def_fragment)
                intrinsic_def_fragment = ''
                intrinsic_setup_fragments.append(intrinsic_setup_fragment)
                intrinsic_setup_fragment = ''
                counter = 0
            counter += 1
            intrinsic_list.append(fun.typed_pair())
            intrinsic_list.append(fun.pair())
            if not fun.is_constructor:
                web_intrinsic = web_intrinsic_set.get(fun.api_name)
                if web_intrinsic is not None:
                    web_intrinsic.overload.append(fun)
                else:
                    web_intrinsic_set[fun.api_name] = WebIntrinsic(fun.api_name, fun)

    # add remaining
    intrinsic_def_fragments.append(intrinsic_def_fragment)
    intrinsic_setup_fragments.append(intrinsic_setup_fragment)
    web_intrinsic_set = dict(sorted(web_intrinsic_set.items(), key=lambda item: item[1].name))

    for key, value in web_intrinsic_set.items():
        web_types['builtin_functions'].append(key)
        overloads = []
        for overload in value.overload:
            params = []
            for param in overload.parameters:
                param_string = ''
                if param.uniform:
                    param_string += 'uniform '
                param_string += param.api_name + ': '
                if param.pointer:
                    param_string += '*'
                if param.literal:
                    param_string += 'literal '
                if param.mutable:
                    param_string += 'mutable '
                
                if param.type_name in data_type_mapping:
                    param_string += data_type_mapping[param.type_name]
                else:
                    param_string += param.type_name
                params.append(param_string)
            overloads.append({ "signature": f'{overload.api_name}({", ".join(params)}) {data_type_mapping[overload.return_type]}' })
        web_intrinsic_list.append({
            "name": key,
            "documentation": value.overload[0].documentation,
            "target_support": [1,1,1,1],
            "overloads": overloads
        })
    intrinsics_header.write(intrinsic_decls)
    intrinsics_header.write('void SetupIntrinsics();\n\n')
    for i, fragment in enumerate(intrinsic_setup_fragments):
        intrinsics_header.write(f'void SetupIntrinsics{i}();\n\n')

    intrinsic_list.sort(key=IntrinsicSortKey)
    intrinsics_header.write(f'inline constexpr StaticMap<HashString, Symbol*, {len(intrinsic_list)}> DefaultIntrinsics = {{\n')
    intrinsics_header.write(',\n'.join(f'    std::pair{{ "{i.api_name}"_h, &{i.decl_name} }} /* {n} */' for n, i in enumerate(intrinsic_list)))
    intrinsics_header.write('\n};\n\n')
    intrinsics_header.write('} // namespace GPULang\n\n')

    for i, [defs, fragment] in enumerate(zip(intrinsic_def_fragments, intrinsic_setup_fragments)):
        intrinsics_source_fragment = open(f'../generated/intrinsics{i}_fragment.cc', "w")
        intrinsics_source_fragment.write(intrinsics_source_start)
        intrinsics_source_fragment.write(defs)
        intrinsics_source_fragment.write(f'void SetupIntrinsics{i}()\n')
        intrinsics_source_fragment.write('{\n')
        intrinsics_source_fragment.write(fragment)
        intrinsics_source_fragment.write('}\n')
        intrinsics_source_fragment.write(f'}} // namespace GPULang\n')

    intrinsics_source.write('void SetupIntrinsics()\n')
    intrinsics_source.write('{\n')
    intrinsics_source.write('    static bool IsSetup = false;\n')
    intrinsics_source.write('    if (IsSetup)\n')
    intrinsics_source.write('        return;\n')
    intrinsics_source.write('    IsSetup = true;\n\n')
    for i, fragment in enumerate(intrinsic_setup_fragments):
        intrinsics_source.write(f'    SetupIntrinsics{i}();\n')
    #intrinsics_source.write(intrinsic_setup)
    intrinsics_source.write('}\n')


    intrinsics_source.write('} // namespace GPULang\n')

    web_syntax = open('../../../docs/syntax.json', 'w')
    web_syntax.write(json.dumps(web_types, indent=4))
    web_syntax.close()

    web_intrinsics = open('../../../docs/intrinsics.json', 'w')
    web_intrinsics.write(json.dumps(web_intrinsic_list, indent=4))
    web_intrinsics.close()


generate_types()

print("Types generated!")