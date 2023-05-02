#include "pukser.hpp"
#include "Puxer.h"
#include "pukser_helper_func.h"
#include "puxer_helper_defs.h"
#include <memory>

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::log_error(const char* s) {
	fprintf(stderr, "Error: %s", s);
	return nullptr;
}

int Pukser::Pukser::handle_token_precedence(Puxer::PuxerTokenResponse& res) {

    if (!isascii(res.token)) {
        return -1;
    }

    int prec = BinopPrecedence[res.token];
    if (prec <= 0) return -1;
    return prec;
}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_expression(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    auto LHS = parse_primary(pux, res);

    if (!LHS) 
        return nullptr;
    
    return parse_binary_ops_rhs(res, 0, std::move(LHS));
}

void Pukser::Pukser::parse(const char* fn) {

	Puxer::Puxer pux;
	auto res = pux.open_file(fn);

	if (res != 0) {
		std::cout << "Opening file: " << fn << " failed!";
	}

	auto token = pux.get_token();

	while (token.token != Puxer::t_eof) {
        
        std::cout << "TOKEN(" << token.token << "): " << token.ident.i_name << std::endl;

		switch (token.token) {

		case Puxer::t_number:
			handle_number(token);
			break;

		default:
			break;
		}

		token = pux.get_token();
	}

}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_primary(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    switch (res.token) {

        case Puxer::t_number:
            return handle_number(res);
            break;

        case Puxer::t_identifier:
            return parse_identifier(pux, res);
            break;

        case '(':
            return parse_paren(pux, res = pux.get_token());
            break;

        default:
            return log_error("Unknown token, expecting expression");
            break;
    }
}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_paren(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

	auto v = parse_expression(pux, res);

	if (!v)
		return nullptr;

	if (res.token != ')')
		return log_error("expected ')'");

	return v;
}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_identifier(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

	std::string name_ref = res.ident.i_name;

	auto tok = pux.get_token();

	//variable ref otherwise a func call
	if (res.token != '(') {
		return std::make_unique<VariableExprAST>(name_ref);
	}

	res = pux.get_token();
	std::vector<std::unique_ptr<ExprAST>> args;

	if (res.token != ')') {

		while (1) {

			if (auto arg = parse_expression(pux, res)) {
				args.push_back(std::move(arg));
			}
			else {
				return nullptr;
			}
			
			tok = pux.get_token();

			if (tok.token == ')')
				break;

			if (tok.token != ',')
				return log_error("Expected ')' or ',' in argument list");
		}
	}

	return std::make_unique<CallExprAST>(name_ref, std::move(args));
}


std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::handle_number(Puxer::PuxerTokenResponse& res) {

	std::unique_ptr<ExprAST> result;

	switch (res.ident.var_type.type) {

	case Puxer::PuxerI32:
		result = std::make_unique<I32ExprAST>(str_toi32(res.ident.value));
		result.get()->type = Puxer::PuxerI32;
		break;

	case Puxer::PuxerI64:
		result = std::make_unique<I64ExprAST>(str_toi64(res.ident.value));
		result.get()->type = Puxer::PuxerI64;
		break;

	case Puxer::PuxerF32:
		result = std::make_unique<F32ExprAST>(str_tof32(res.ident.value));
		result.get()->type = Puxer::PuxerF32;
		break;

	case Puxer::PuxerF64:
		result = std::make_unique<F64ExprAST>(str_tof64(res.ident.value));
		result.get()->type = Puxer::PuxerF64;
		break;

	case Puxer::PuxerU32:
		result = std::make_unique<U32ExprAST>(str_tou32(res.ident.value));
		result.get()->type = Puxer::PuxerU32;
		break;

	case Puxer::PuxerU64:
		result = std::make_unique<U64ExprAST>(str_tou64(res.ident.value));
		result.get()->type = Puxer::PuxerU64;
		break;

    case Puxer::PuxerUnknown:
        result = std::make_unique<UknownExprAST>(res.ident.value);
        result.get()->type = Puxer::PuxerUnknown;
        break;

    default:
        result.get()->type = Puxer::PuxerBadNumber;
        break;
	}

	return std::move(result);
}
