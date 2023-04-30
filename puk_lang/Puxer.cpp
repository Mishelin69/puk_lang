#include "Puxer.h"
#include <cstdio>
#include <cstring>

#if defined __GNUC__
    #define open_file_portable(f_ptr, name, mode, code_addr) \
        *f_ptr = fopen(name, mode);\
        *code_addr = 0;
#else
    #define open_file_portable(f_ptr, name, mode, code_addr)\
        *code_addr = fopen_s(&this->fs, filename, "r");
#endif

static const char* VALID_INTEGERS[6] = { "i32", "i64", "u32", "u64", "f32", "f64 "};

Puxer::Puxer::Puxer() {

	this->fs = nullptr;

}

Puxer::Puxer::~Puxer() {

	if (this->fs) {
		fclose(fs);
	}
}

int Puxer::Puxer::open_file(const char* filename) {

	error_t code = 0;
    open_file_portable(&this->fs, filename, "r", &code);

    if (!this->fs) {
        std::cout << "Error opening file: " << filename << std::endl;
        exit(-1);
    }
	
	return code;
}

Puxer::PuxerToken Puxer::Puxer::PuxerHandleIndentifier(std::string& s, PuxerIdentifier& i) {

	if (s == "fn") {
		return PuxerToken::t_fndef;
	}
	else if (s == "let") {
		return PuxerToken::t_valdef;
	}
	else if (s == "struct") {
		return PuxerToken::t_structdef;
	}
	else if (s == "for") {
		return PuxerToken::t_for;
	}
	else if (s == "while") {
		return PuxerToken::t_while;
	}
	else if (s == "crack") {
		return PuxerToken::t_crackdef;
	}

	i.i_name = s;
	i.var_type = PuxerCustomType{ "", PuxerType::PuxerCustom, sizeof(i.i_name)};

	return PuxerToken::t_identifier;
}

bool Puxer::Puxer::PuxerHandleNumber(std::string& s, PuxerIdentifier& i) {


	uint8_t num_len = 0;
	bool floating = false;
	PuxerType type;
	size_t j = 0;

    for (; j < s.length() && s[j] != '_'; ++j) {

        if (isalpha(s[j])) {
            return true;
        }

        else if (s[j] == '.' && floating) {
            return true;

        }

        else if (s[j] == '.') {
            floating = true;
            num_len += 1;
        }

        else
            num_len += 1;
    }

    i.value = s.substr(0, num_len);

    //check for bigger numbers than 64 bit can hold
    if (num_len > 20) {
        return true;
    }

    if (j >= s.length()) 
        return false;

    //offset number part from the type part
    const char* type_string = s.c_str() + num_len;

    if (strcmp(type_string, "i32")) {

        if (floating) {

            std::cout << "Mismatched types";
            return true;
        }

        //auto convert to bigger number
        if (num_len > 10) {
            i.var_type.bytes_size = 64;
            i.var_type.type = PuxerType::PuxerI64;
        }
        else {
            i.var_type.bytes_size = 32;
            i.var_type.type = PuxerType::PuxerI32;
        }

        return false;
    }

    if (strcmp(type_string, "u32")) {

        if (floating) {

            std::cout << "Mismatched types";
            return true;
        }

        //auto convert to bigger number
        if (num_len > 10) {
            i.var_type.bytes_size = 64;
            i.var_type.type = PuxerType::PuxerU64;
        }
        else {
            i.var_type.bytes_size = 32;
            i.var_type.type = PuxerType::PuxerU32;
        }

        return false;
    }

    if (strcmp(type_string, "f32")) {

        //auto convert to bigger number
        if (num_len > 10) {
            i.var_type.bytes_size = 64;
            i.var_type.type = PuxerType::PuxerF64;
        }
        else {
            i.var_type.bytes_size = 32;
            i.var_type.type = PuxerType::PuxerF32;
        }

        return false;
    }

    if (strcmp(type_string, "f64")) {

        i.var_type.bytes_size = 64;
        i.var_type.type = PuxerType::PuxerF64;

        return false;
    }

    if (strcmp(type_string, "i64")) {

        if (floating) {

            std::cout << "Mismatched types";
            return true;
        }

        i.var_type.bytes_size = 64;
        i.var_type.type = PuxerType::PuxerF64;

        return false;
    }

    if (strcmp(type_string, "u64")) {

        if (floating) {
            std::cout << "Mismatched types";
            return true;
        }

        i.var_type.bytes_size = 64;
        i.var_type.type = PuxerType::PuxerF64;

        return false;
    }

    return true;
}

Puxer::PuxerTokenResponse Puxer::Puxer::get_token() {

    if (!this->fs) {
        std::cout << "Invalid file"; 
        exit(-1);
    }

    if (!this->identifier_queue.empty()) {
        PuxerIdentifier i;

        int last_char = this->identifier_queue.front();
        this->identifier_queue.pop();

        i.i_name = last_char;
        i.var_type = PuxerCustomType{ "", PuxerType::PuxerUnknown, sizeof(last_char) };

        return PuxerTokenResponse{ PuxerToken::t_identifier, i };
    }

    char last_char = ' ';

    //skip whitespaces
    while (isspace(last_char)) {
        last_char = fgetc(this->fs);
    }

    //build the identifier
    if (isalpha(last_char)) {

        std::string identifier;
        identifier += last_char;

        while (isalnum(last_char = fgetc(this->fs))) {
            identifier += last_char;
        }

        if (!isspace(last_char) && last_char != '\n' && last_char != '\r')
            this->identifier_queue.push(last_char);

        //figure out what kind of puk token this is and assign value to it
        auto i = PuxerIdentifier{};
        auto token = PuxerHandleIndentifier(identifier, i);

        switch (token) {

            case PuxerToken::t_structdef:
                return PuxerTokenResponse{ PuxerToken::t_structdef, i };
                break;

            case PuxerToken::t_valdef:
                return PuxerTokenResponse{ PuxerToken::t_valdef, i };
                break;

            case PuxerToken::t_identifier:
                return PuxerTokenResponse{ PuxerToken::t_identifier, i };
                break;

            case PuxerToken::t_fndef:
                return PuxerTokenResponse{ PuxerToken::t_fndef, i };
                break;

            case PuxerToken::t_for:
                return PuxerTokenResponse{ PuxerToken::t_for, i };
                break;

            case PuxerToken::t_while:
                return PuxerTokenResponse{ PuxerToken::t_while, i };
                break;

            case PuxerToken::t_foreach:
                return PuxerTokenResponse{ PuxerToken::t_foreach, i };
                break;

            case PuxerToken::t_crackdef:
                return PuxerTokenResponse{ PuxerToken::t_crackdef, i };
                break;

            default:
                break;
        }
    }

    if (isdigit(last_char) || last_char == '.') {

        PuxerIdentifier i;
        std::string num_string;

        i.var_type.type = PuxerType::PuxerNumber;

        bool explicit_type = false;
        bool invalid_num = false;

        //allow this king of notation 100_usize, gives type
        while (isdigit(last_char) || last_char == '.' || last_char == '_' || (explicit_type && isalpha(last_char))) {

            if (last_char == '_' && !explicit_type) {
                explicit_type = true;
            }
            //not valid syntax ... 1_usize_usize
            else if (last_char == '_' && explicit_type) {
                break;
            }

            num_string += last_char;
            last_char = fgetc(this->fs);
        }

        if (invalid_num) {

            //read it the rest until space or EOF is found 
            while (!isspace(last_char) && last_char != EOF) {

                num_string += last_char;
                last_char = fgetc(this->fs);
            }

            goto number_error;
        }

        if (!isspace(last_char) && last_char != '\n' && last_char != '\r')
            this->identifier_queue.push(last_char);

        invalid_num = PuxerHandleNumber(num_string, i);

        if (invalid_num) {

            //read it the rest until space or EOF is found 
            while (!isspace(last_char) && last_char != EOF) {

                num_string += last_char;
                last_char = fgetc(this->fs);
            }

            goto number_error;
        }

        return PuxerTokenResponse{ PuxerToken::t_number, i };

        //===============================
number_error:

        PuxerIdentifier num_i;
        num_i.value = num_string;

        return PuxerTokenResponse{ PuxerToken::t_invalid_number, num_i };
    }

    //const strings
    if (last_char == '"') {

        PuxerIdentifier i;
        std::string s;

        last_char = fgetc(this->fs);

        while (last_char != '"' && last_char != EOF) {
            s += last_char;
            last_char = fgetc(this->fs);
        }

        i.var_type.type = PuxerConstString;
        i.value = s;

        return PuxerTokenResponse { PuxerToken::t_const_str, i};
    }

    //comments
    if (last_char == '/') {

        auto next_char = fgetc(this->fs);
        PuxerIdentifier i;

        //found comment
        if (next_char == '/') {
            while (last_char != EOF && last_char != '\n' && last_char != '\r')
                last_char = fgetc(this->fs);
        }

        if (last_char != EOF) {
            return this->get_token();
        }
    }

    PuxerIdentifier i;

    if (last_char == EOF) {
        return PuxerTokenResponse{ PuxerToken::t_eof, i };
    }

    std::string s;

    i.i_name = last_char;
    i.var_type = PuxerCustomType{ "", PuxerType::PuxerUnknown, sizeof(last_char) };

    return PuxerTokenResponse{ PuxerToken::t_identifier, i };
}
