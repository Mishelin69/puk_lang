#pragma once
#include <string>

namespace Puxer {

	enum PuxerType {

		PuxerChar = 0,

		PuxerI32,
		PuxerI64,

		PuxerU8,
		PuxerU32,
		PuxerU64,

		PuxerF32,
		PuxerF64,

		PuxerPointer,
		PuxerFunction,

		PuxerCustom,
		PuxerUnknown,
		PuxerNumber,
		PuxerBadNumber,
		PuxerConstString,
		PuxerFunctionCall,
		PuxerVariable,
        PuxerNone,
	};

	enum PuxerToken {

		t_eof = -1,

		t_structdef = -2,
		t_valdef = -3,

		t_identifier = -4,
		t_number = -5,

		t_ptr = -6,
		t_const_str = -7,

		t_fndef = -8,

		t_for = -9,
		t_while = -10,
		t_foreach = -11,
		t_forlin = -12,
		t_crackdef = -13,

		t_invalid_number = -500,
	};

	struct PuxerCustomType {

		std::string name;
		PuxerType type;
		size_t bytes_size = 0;

	public:
        PuxerCustomType() = default;
		PuxerCustomType(const PuxerCustomType& other) = default;
        ~PuxerCustomType() = default;

        PuxerCustomType(std::string name, PuxerType type, size_t bytes_size):
            name(name), type(type), bytes_size(bytes_size) {}

	};

	struct PuxerFnRetPair {

		std::string* name;
		PuxerCustomType type;

	};

	struct PuxerIdentifier {

		std::string i_name;
		PuxerCustomType var_type;

		std::string value;
		size_t val_len;

	};

}
