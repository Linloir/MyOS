/*** 
 * Author       : Linloir
 * Date         : 2022-08-05 14:51:04
 * LastEditTime : 2022-08-07 22:51:54
 * Description  : Minimum implementation of ELF structure
 */

#include "types.h"

#ifndef _ELF_H_
#define _ELF_H_

typedef uint32 Elf32_Addr;  //Unsigned program address
typedef uint16 Elf32_Half;  //Unsigned medium integer
typedef uint32 Elf32_Off;   //Unsigned file offset
typedef int32 Elf32_Sword;  //Signed large integer
typedef uint32 Elf32_Word;  //Unsigned large integer

class Elf32Header;
class Elf32SectionHeader;
class Elf32ProgramHeader;

#define EI_NIDENT   16

//Identification Indexes (e_ident field)===============================

//ELF provides an object file frame work to support multiple process-
//ors, multiple data encodings, and multiple classes of machines.

//To support this object file family, the initial bytes of the file
//specify how to interpret the file, independent of the processor on
//which the inquiry is made and independent of the file's remaining
//contents

#define EI_MAG0     0       //File identification, should be 0x7F
#define EI_MAG1     1       //File identification, should be 'E'
#define EI_MAG2     2       //File identification, should be 'L'
#define EI_MAG3     3       //File identification, should be 'F'
#define EI_CLASS    4       //File class, identifies the file's class
#define EI_DATA     5       //Data encoding, specifies the data encoding of the processor-specific data in the object file
#define EI_VERSION  6       //File version, must be EV_CURRENT
#define EI_PAD      7       //Start of padding bytes, marks the beginning of the unused bytes in e_ident
#define EI_NIDENT   16      //Size of e_ident[]

//The file format is designed to be portable among machines of various
//sizes, without imposing the sizes of the largest machine on the
//smallest.

//Class ELFCLASS32 supports machines with files and virtual address
//spaces up to 4 gigabytes; It uses the basic types defined above

//Class ELFCLASS64 is reserved for 64-bit architectures. Its appearance
//here shows how the object file may change, but the 64-bit format is
//otherwise unspecified

#define ELFCLASSNONE    0   //Invalid class
#define ELFCLASS32      1   //32-bit objects
#define ELFCLASS64      2   //64-bit objects

//Byte EI_DATA specifies the data encoding of the processor-specific
//data in the object file, the following encodings are currently defined

#define ELFDATANONE     0   //Invalid data encoding
#define ELFDATA2LSB     1   //Least significant byte occupying the lowest address, which is little-endian mode
#define ELFDATA2MSB     2   //Most significant byte occupying the lowest address, which is big-endian mode

//=====================================================================

//Object File Types (e_type field)=====================================

//Although the core file contents are unspecified, type ET_CORE is
//reserved to mark the file.

//Values from ET_LOPROC through ET_HIPROC (inclusive) are reserved
//for processor-specific semantics.

//Other values are reserved and will be assigned to new object file
//types as neccessary

#define ET_NONE     0       //No file type
#define ET_REL      1       //Relocatable file
#define ET_EXEC     2       //Executable file
#define ET_DYN      3       //Shared object file
#define ET_CORE     4       //Core file
#define ET_LOPROC   0xFF00  //Processor-specific
#define ET_HIPROC   0xFFFF  //Processor-specific

//=====================================================================

//Machine Architecture Types (e_machine field)=========================

//Other values are reserved and will be assigned to new machines as
//neccessary.

//Processor-specific ELF names use the machine name to distinguish them

//In this minimum implementation, only Intel 80386 architecture is
//considered valid

#define EM_NONE     0       //No machine
#define EM_M32      1       //AT&T WE 32100
#define EM_SPARC    2       //SPARC
#define EM_386      3       //Intel 80386 | IA-32
#define EM_68K      4       //Motorola 68000
#define EM_88K      5       //Motorola 88000
#define EM_860      7       //Intel 80860
#define EM_MIPS     8       //MIPS RS3000

//====================================================================

//Object File Version (e_version field)===============================

//The value 1 signifies the original file format; extensions will
//create new versions with higher numbers

//The value of EV_CURRENT, though given as 1 below, will change as
//necessary to reflect the current version number

#define EV_NONE     0       //Invalid version
#define EV_CURRENT  1       //Current version

//====================================================================

class Elf32Header {
    public:
        //The initial bytes mark the file as an object file and provide
        //machine-independent data with which to decode and interpret
        //the file's contents.
        uint8           e_ident[EI_NIDENT];
        //This member identifies the object file type
        Elf32_Half      e_type;
        //This member's value specifies the required architecture for an
        //individual file
        //In this implement, CPU architecture is constraint to IA-32, which
        //should only be Intel 80386 architecture
        Elf32_Half      e_machine;
        //This member identifies the object file version
        Elf32_Word      e_version;
        //This member gives the virtual address to which the system first
        //transfers control, thus starting the process
        //If the file has no associated entry point, this member holds zero
        Elf32_Addr      e_entry;
        //This member holds the program header table's file offset in bytes.
        //If the file has no program header table, this member holds zero
        Elf32_Off       e_phoff;
        //This member holds the section header table's file offset in bytes.
        //If the file has no section header table, this member holds zero
        Elf32_Off       e_shoff;
        //This member holds processor-specific flags associated with the
        //file.
        //Flag names take the form EF_machine_flag.
        Elf32_Word      e_flags;
        //This member holds the ELF header's size in bytes
        Elf32_Half      e_ehsize;
        //This member holds the size in bytes of on entry in the file's
        //program header table
        //All entries in the table are the same size
        Elf32_Half      e_phentsize;
        //This member holds the number of entries in the program header
        //table.
        //Thus the product of e_phentsize and e_phnum gives the table's
        //size in bytes.
        //If a file has no program header table, e_phnum holds the value
        //zero.
        Elf32_Half      e_phnum;
        //This member holds a section header's size in bytes.
        //A section header is one entry in the section header table, all
        //entries are the same size
        Elf32_Half      e_shentsize;
        //This member holds the number of entries in the section header
        //table.
        //Thus the product of e_shentsize and e_shnum gives the section
        //header table's size in bytes.
        //If a file has no section header table, e_shnum holds the value
        //zero
        Elf32_Half      e_shnum;
        //This member holds the section header table index of the entry
        //associated with the section name string table.
        //If the file has no section name string table, this member holds
        //the value SHN_UNDEF
        Elf32_Half      e_shstrndx;
    public:
        //Convert an 32-bit address to a pointer of Elf32Header instance
        static Elf32Header* from(uint32 addr);
        //Judge whether the elf file is valid
        bool isValid();
        uint32 segmentCnt();
        //Parse the nth segment descriptor (program header) of the elf file
        Elf32ProgramHeader* segmentAt(uint32 idx);
        uint32 entry();
};

//Special Section Header Table Indexes================================

//Some section header table indexes are reserved, an object file will
//not have sections for these special indexes

//This value marks an undefined, missing, irrelevant, or otherwise
//meaningless section reference.
//For example, a symbol "defined" relative to section number SHN_UNDEF
//is an undefined symbol
#define SHN_UNDEF       0
//This value specifies the lower bound of the range of reserved indexes
#define SHN_LORESERVE   0xFF00
//Values from SHN_LOPROC to SHN_HIPROC (inclusive) are reserved for
//processor-specific semantics
#define SHN_LOPROC      0xFF00
#define SHN_HIPROC      0xFF1F
//This value specifies absolute values for the corresponding reference.
//For example, symbols defined relative to section number SHN_ABS have
//absolute values and are not affected by relocation
#define SHN_ABS         0xFFF1
//Symbols defined relative to this section are common symbols, such as
//FORTRAN COMMON or unallocated C external variables
#define SHN_COMMON      0xFFF2
//This value specifies the upper bound of the range of reserved indexes.
//The system reserves indexes between SHN_LORESERVE and SHN_HIRESERVE,
//inclusive. The values DO NOT reference the section header table.
//That is, the section header table DOES NOT CONTAIN entries for the
//reserved indexes
#define SHN_HIRESERVE   0xFFFF

//====================================================================

//Section Types (sh_type field)=======================================

//This value marks the section header as inactive, it does not have an
//associated section. Other members of the section header have undefined
//values.
#define SHT_NULL        0
//The section holds information defined by the program, whose format and
//meaning are determined solely by the program
#define SHT_PROGBITS    1
//The section holds a symbol table.
//SHT_SYMTAB provides symbols for link editing, though it may also be
//used for dynamic linking.
//As a complete symbol table, it may contain many symbols unnecessary
//for dynamic linking. Consequently, an object file may also contain
//a SHT_DYNSYM section.
#define SHT_SYMTAB      2
//The section holds a string table.
//An object file may have multiple string table sections.
#define SHT_STRTAB      3
//The section holds relocation entries with explicit addends, such as
//type Elf32_Rela for the 32-bit class of object files.
//An object file may have multiple relocation sections.
#define SHT_RELA        4
//The section holds a symbol hash table.
//All objects participating in dynamic linking must contain a symbol
//hash table.
//Currently, an object file may have only one hash table.
#define SHT_HASH        5
//The section holds information for dynamic linking.
//Currently, an object file may have only one dynamic section.
#define SHT_DYNAMIC     6
//The section holds information that marks the file in some way.
#define SHT_NOTE        7
//A section of this types occupies no space in the file but otherwise
//resembles SHT_PROGBITS.
//Although this section contains no bytes, the sh_offset member
//contains the conceptual file offset
#define SHT_NOBITS      8
//The section holds relocation entries without explicit addends, such
//as type Elf32_Rel for the 32-bit class of object files.
//An object file may have multiple relocation sections.
#define SHT_REL         9
//This section type is reserved but has unspecified semantics.
//Programs that contain a section of this type do not conform to the
//ABI.
#define SHT_SHLIB       10
//The section holds a symbol table.
//SHT_DYNSYM holds a minimal set of dynamic linking symbols
#define SHT_DYNSYM      11
//Values from SHT_LOPROC to SHT_HIPROC (inclusive) are reserved for
//processor-specific semantics
#define SHT_LOPROC      0x70000000
//Values from SHT_LOPROC to SHT_HIPROC (inclusive) are reserved for
//processor-specific semantics
#define SHT_HIPROC      0x7FFFFFFF
//This value specifies the lower bound of the range of indexes
//reserved for application programs.
//Section types between SHT_LOSER and SHT_HIUSER may be used by
//the application, without conflicting with current or future
//system-defined section types.
#define SHT_LOUSER      0x80000000
//This value specifies the upper bound of the range of indexes
//reserved for application programs.
//Section types between SHT_LOSER and SHT_HIUSER may be used by
//the application, without conflicting with current or future
//system-defined section types.
#define SHT_HIUSER      0xFFFFFFFF

//====================================================================

//Section Attribute Flags (sh_flags field)============================

//The section contains data that should be writable during process
//execution
#define SHF_WRITE       0x1
//The section occupies memory during process execution.
//Some control sections do not reside in the memory image of an object
//file, this attribute is off for those sections.
#define SHF_ALLOC       0x2
//The section contains executable machine instructions.
#define SHF_EXECINSTR   0x4
//All bits included in this mask are reserved for processor-specific
//semantics.
#define SHF_MASKPROC    0xF0000000

//====================================================================

class Elf32SectionHeader {
    public:
        //This member specifies the name of the section.
        //Its value is an index into the section header string table
        //section, giving the location of a null-terminated string.
        Elf32_Word      sh_name;
        //This member categorizes the section's contents and semantics.
        Elf32_Word      sh_type;
        //Miscellaneous attributes description.
        Elf32_Word      sh_flags;
        //If the section will appear in the memory image of a process,
        //this member gives the address at which the section's first
        //byte should reside.
        //Otherwise, the member contains 0.
        Elf32_Addr      sh_addr;
        //This member's value gives the byte offset from the beginning
        //of the file to the first byte in the section.
        //One section type, SHT_NOBITS described below, occupies no
        //space in the file, and its sh_offset member locates the conceptual
        //placement in the file.
        Elf32_Off       sh_offset;
        //This member gives the section's size in bytes.
        //Unless the section type is SHT_NOBITS, the section occupies
        //sh_size bytes in the file. A section of type SHT_NOBITS may
        //have a non-zero size, but it occupies NO space in the file.
        Elf32_Word      sh_size;
        //This member holds a section header table index link, whose
        //interpretation depends on the section type
        //- SHT_DYNAMIC: The section header index of the string table
        //used by entries in the section
        //- SHT_HASH: The section header index of the symbol table to
        //which the hash table applies
        //- SHT_REL: The section header index of the associated symbol
        //table
        //- SHT_RELA: The section header index of the associated symbol
        //table
        //- SHT_SYMTAB: The section header index of the associated
        //string table
        //- SHT_DYNSYM: The section header index of the associated
        //string table
        //- other: SHN_UNDEF
        Elf32_Word      sh_link;
        //This member holds extra information, whose interpretation
        //depends on the section type.
        //- SHT_DYNAMIC: 0
        //- SHT_HASH: 0
        //- SHT_REL: The section header index of the section to which
        //the relocation applies.
        //- SHT_RELA: The section header index of the section to which
        //the relocation applies.
        //- SHT_SYMTAB: One greater than the symbol table index of the
        //last local symbol (binding STB_LOCAL)
        //- SHT_DYNSYM: One greater than the symbol table index of the
        //last local symbol (binding STB_LOCAL)
        //- other: 0
        Elf32_Word      sh_info;
        //Some sections have address alignment constraints.
        //For example, if a section holds a doubleword, the system must
        //ensure doubleword alignment for the entire section.
        //That is, the value of sh_addr must by congruent to 0, modulo the
        //value of sh_addralign (sh_addr ≡ 0, mod(sh_addralign))
        //Currently, only 0 and positive integral powers of two are allowed.
        //Values 0 and 1 mean the section has no alignment constraints.
        Elf32_Word      sh_addralign;
        //Some sections hold a table of fixed-size entries, such as a
        //symbo table.
        //For such a section, this member gives the size in bytes of
        //each entry.
        //The member contains 0 if the section does not hold a table
        //of fixed-size entries.
        Elf32_Word      sh_entsize;
};

//Segment Type (p_type field)=========================================

//The array element is unused, other members' values are undefined.
//This type lets the program header table have ignored entries.
#define PT_NULL         0
//The array element specifies a loadable segment.
//This is what we actually care about while loading statically linked
//programs.
//The bytes from the file are mapped to the beginning of the memory
//segment. If the segment's memory size is larger than the file size,
//the "extra" bytes are defined to hold the value 0 and follow the
//segment's initialized area.
//The file size may not be larger than the memory size.
//Loadable segment entries in the program header table appear in
//ascending order, sorted on the p_vaddr member.
#define PT_LAOD         1
//The array element specifies dynamic linking information.
#define PT_DYNAMIC      2
//The array element specifies the location and size of a null-terminated
//path name to invoke as an interpreter.
//This segment type is meaningful only for executable files and may not
//occur more than once in a file. If it's present, it must precede any
//loadable segment entry.
#define PT_INTERP       3
//The array element specifies the location and size of auxiliary
//information.
#define PT_NOTE         4
//This segment type is reserved but has unspecified semantics.
//Programs that contain an array element of this type do not conform
//to the ABI
#define PT_SHLIB        5
//The array element, if present, specifies the location and size of
//the program header table itself, both in the file and in the memory
//image of the program.
//This segment type may not occur more than once in a file.
//Moreover, it may occur only if the program header table is part of
//the memory image of the program.
//If it is present, it must precede any loadable segment entry.
#define PT_PHDR         6
//Values from PT_LOPROC to PT_HIPROC (inclusive) are reserved for
//processor-specific semantics.
#define PT_LOPROC       0x70000000
//Values from PT_LOPROC to PT_HIPROC (inclusive) are reserved for
//processor-specific semantics.
#define PT_HIPROC       0x7FFFFFFF

//====================================================================

class Elf32ProgramHeader {
    public:
        //This member tells what kind of segment this array element
        //describes or how to interpret the array element's information.
        Elf32_Word      p_type;
        //This member gives the offset from the beginning of the object
        //file at which the first byte of the segment resides.
        Elf32_Off       p_offset;
        //This member gives the virtual address at which the first byte
        //of the segment resides in memory
        Elf32_Addr      p_vaddr;
        //On systems for which physical addressing is relevant, this
        //member is reserved for the segment's physical address.
        //Because System V ignores physical addressing for application
        //programs, this member has unspecified contents for executable
        //files and shared objects.
        Elf32_Addr      p_paddr;
        //This member gives the number of bytes in file image of the
        //segment, it may be zero.
        Elf32_Word      p_filesz;
        //This member gives the number of bytes in the memory image of
        //the segment, it may be zero.
        Elf32_Word      p_memsz;
        //This member gives flags relevant to the segment
        Elf32_Word      p_flags;
        //Loadable process segments must have congruent values for
        //p_addr and p_offset, modulo the page size.
        //This member gives the values to which the segments are aligned
        //in memory and in the file.
        //Values 0 and 1 mean no alignment is required.
        //Otherwise, p_align should be a positive, integral power of 2,
        //and p_vaddr should equal p_offset, modulo p_align
        Elf32_Word      p_align;
    public:
        //Judge whether the segment needs to be loaded into memory
        bool shouldLoad();
};

#endif
