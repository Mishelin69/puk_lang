#include "pukser.hpp"
#include "Puxer.h"
#include "pukser_helper_func.h"
#include "puxer_helper_defs.h"

#include <memory>

void Pukser::Pukser::parse(const char* fn) {

    Puxer::Puxer pux;
    auto res = pux.open_file(fn);

    if (res != 0) {
        std::cout << "Opening file: " << fn << " failed!";
    }

    Puxer::PuxerTokenResponse token;
    token = pux.get_token();

    while (token.token != Puxer::t_eof) {

        //std::cout << "TOKEN(" << token.token << "): " << token.ident.i_name << std::endl;

        switch (token.token) {

        case Puxer::t_number:
            handle_number(token);
            break;

        case Puxer::t_fndef:
            parse_definition(pux, token);
            break;

        default:
            parse_toplevel(pux, token);
            break;
        }

        //token = pux.get_token();
    }

}

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

void Pukser::Pukser::handle_toplevel(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    if (parse_toplevel(pux, res)) {
        fprintf(stderr, "Parsed top level definition");
    }

}

void Pukser::Pukser::handle_functiondef(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    if (parse_definition(pux, res)) {
        fprintf(stderr, "Parsed top level definition");
    }

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

    case Puxer::PuxerNumber:
        result = std::make_unique<UknownExprAST>(res.ident.value);
        result.get()->type = Puxer::PuxerNumber;
        break;

    default:
        std::cout << "Num val => " << res.ident.value << std::endl;
        result = std::make_unique<ExprAST>();
        result.get()->type = Puxer::PuxerBadNumber;
        break;
    }

    return std::move(result);
}

std::unique_ptr<Pukser::PrototypeAST> Pukser::Pukser::parse_prototype(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

	if (res.token != Puxer::t_identifier) {
		fprintf(stderr, "Error: Expected funciton name in function definition");
		return nullptr;
	}

	std::string fn_name = res.ident.i_name;
	res = pux.get_token();

	if (res.token != '(') {
		fprintf(stderr, "Error: Expected '(' in prototype");
		return nullptr;
	}

	std::vector<std::string> arg_names;

	while ((res = pux.get_token()).token == Puxer::t_identifier) {
		arg_names.push_back(res.ident.i_name);
	}

	if (res.token != ')') {
		fprintf(stderr, "Error: Expected ')' in prototype");
		return nullptr;
	}

    Puxer::PuxerTokenResponse p;
    p = pux.get_token();

    if (p.token != '=') {
        std::cout << "Error: expected '=' found " << (char)p.token << " instead!" << std::endl;
        return nullptr;
    }

    p = pux.get_token();

    if (p.token != '>') {
        std::cout << "Error: expected '>' found " << (char)p.token << " instead!" << std::endl;
        return nullptr;
    }

    //type
    p = pux.get_token();

    if (p.token != Puxer::t_identifier) {
        std::cout << "expected return type, found: " << p.token << std::endl;;
        return nullptr;
    }

    std::shared_ptr<Puxer::PuxerCustomType> type;
    
    if (p.ident.var_type.type == Puxer::PuxerCustom) {

        if (types_map.find(p.ident.var_type.name) == types_map.end()) {
            std::cout << "Warning: Couldnt find the type definition! Will look more into the file " << (char)p.token << " instead!" << std::endl;
            
            types_map.insert({ p.ident.var_type.name, std::make_shared<Puxer::PuxerCustomType>(
                std::string("NOT FOUND"), Puxer::PuxerCustom, 0)});
        }
        else {
            type = types_map[p.ident.var_type.name];
        }
    }

    auto ret = std::make_unique<PrototypeAST>(fn_name, std::move(arg_names), type);

    return std::move(ret);
}

std::unique_ptr<Pukser::FunctionAST> Pukser::Pukser::parse_definition(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    auto prototype = parse_prototype(pux, res = pux.get_token());

    if (!prototype)
        return nullptr;

    res = pux.get_token();

    if (res.token != '{') {
        fprintf(stderr, "Error: Expected '{' after funciton definition");
        return nullptr;
    }

    std::vector<std::unique_ptr<ExprAST>> body;
    parse_curly(pux, res, body);

    if (!body.empty())
        return std::make_unique<FunctionAST>(std::move(prototype), std::move(body));

    return nullptr;
}

std::unique_ptr<Pukser::FunctionAST> Pukser::Pukser::parse_toplevel(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    if (auto E = parse_expression(pux, res)) {

        std::vector<std::unique_ptr<ExprAST>> body;
        body.push_back(std::move(E));

        auto prototype = std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(prototype), std::move(body));
    }

    return nullptr;
}


std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_binary_ops_rhs(
        Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res, int prec, std::unique_ptr<ExprAST> LHS) {

    while (1) {

        int tok_prec = handle_token_precedence(res);

        //if tok_prec is smaller than 0 => not a binary operator found in the ref map
        if (tok_prec < prec)
            return LHS;

        //save the operator and get nex token
        int binary_operator = res.token;
        res = pux.get_token();

        //get right hand side of the pair
        auto RHS = parse_primary(pux, res);

        if (!RHS)
            return nullptr;

        //look ahead to see if there's another token with higher precedence than the current one 
        //example a + b could be a + b * c
        int next_prec = handle_token_precedence(res);

        if (tok_prec < next_prec) {
            std::cout << "OKAY DIDNT EXPECT THIS TO HAPPEN FRFR :(" << std::endl;
            RHS = parse_binary_ops_rhs(pux, res, tok_prec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        LHS = std::make_unique<BinaryExprAST>((char) binary_operator, std::move(LHS), std::move(RHS));
    }

    return nullptr;
}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_expression(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    auto LHS = parse_primary(pux, res);

    if (!LHS) 
        return nullptr;

    return parse_binary_ops_rhs(pux, res, 0, std::move(LHS));
}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_primary(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {
    
    switch (res.token) {

        case Puxer::t_number:
            return handle_number(res);
            break;

        case Puxer::t_identifier:
            return parse_identifier(pux, res);
            break;

        case Puxer::t_valdef:
            break;

        case '(':
            return parse_paren(pux, res = pux.get_token());
            break;

        default:
            std::cout << "pukova hlava: " << (char)res.token << " " << res.token << std::endl;
            return log_error("Unknown token, expecting expression\n");
            break;
    }
}

void Pukser::Pukser::parse_curly(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res, std::vector<std::unique_ptr<ExprAST>>& body) {

    if (res.token == '}')
        nullptr;

    while (res.token != '}') {

        auto expr = parse_expression(pux, res = pux.get_token());
        std::cout << "Parsing Expression" << std::endl;

        if (!expr) {
            return ;
        }

        body.push_back(std::move(expr));

    }

    std::cout << "End of function !! " << std::endl;
}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_paren(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    auto v = parse_expression(pux, res);

    if (!v)
        return nullptr;

    if (res.token != ')')
        return log_error("expected ')'");

    return v;
}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_identifier(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    std::string name_ref = res.ident.i_name;

    Puxer::PuxerTokenResponse tok = pux.get_token();

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
