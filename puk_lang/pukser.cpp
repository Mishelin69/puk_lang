#include "pukser.hpp"
#include "Puxer.h"
#include "pukser_helper_func.h"
#include "puxer_helper_defs.h"

#include <memory>

#define PUKSER_UNQ_SCE(c) \
    std::move(std::make_unique<SingleCharExpr>(c));

void Pukser::Pukser::parse(const char* fn) {

    Puxer::Puxer pux;
    auto res = pux.open_file(fn);

    if (res != 0) {
        std::cout << "Opening file: " << fn << " failed!";
    }

    Puxer::PuxerTokenResponse token;
    token = pux.get_token();

    while (token.token != Puxer::t_eof) {

        switch (token.token) {

        case Puxer::t_number:
            last_expr = handle_number(token);
            break;

        case Puxer::t_fndef:
            parse_definition(pux, token);
            break;

        case '=':
            last_expr = parse_binary_ops_rhs(pux, token, handle_token_precedence(token), std::move(last_expr));
            break;

        default:
            parse_toplevel(pux, token);
            break;
        }

        //token = pux.get_token();
    }
    //std::cout << std::endl << "Am of nodes: " << dynamic_cast<ExpressionBodyAST*>((abs_tree[0].get()))->body.size() << std::endl;
}

void Pukser::Pukser::create_scope() {
    PukserScope _scope;
    scope.push(_scope);
}

void Pukser::Pukser::drop_cur_scope() {

    if (scope.empty()) {
        log_error("Error: cannot drop scope, when none are present");
        return ;
    }

    scope.pop();
}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::declare_var(VariableExprAST var) {

    if (scope.empty()) {
        
        if (var.type == Puxer::PuxerUnknown || var.type == Puxer::PuxerNumber) {
            return log_error("Variable without declared types are not allowed in global scope!");
        }

        globals.insert({var.name, var});
        stack_size += var.type_info->bytes_size;

        return std::make_unique<VariableExprAST>(globals[var.name]);
    }

    auto cur_scope = scope.top();
    cur_scope.vars.insert({var.name, var});

    return std::make_unique<VariableExprAST>(cur_scope.vars[var.name]);
}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::fetch_var(std::string& name) {

    if (auto var = globals.find(name); var != globals.end()) {
        return std::make_unique<VariableExprAST>(var->second);
    }
 
    PukserScope& cur_scope = scope.top();

    if (auto var = cur_scope.vars.find(name); var != cur_scope.vars.end()) {
        return std::make_unique<VariableExprAST>(var->second);
    }   

    return nullptr;
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

std::shared_ptr<Puxer::PuxerCustomType> Pukser::Pukser::handle_type(Puxer::PuxerTokenResponse& res) {

    if (res.token != Puxer::t_identifier) {
        return nullptr;
    }

    if (res.ident.var_type.type == Puxer::PuxerCustom) {

        if (types_map.find(res.ident.var_type.name) == types_map.end()) {

            if (res.token != Puxer::t_identifier) {
                std::cout << "Warning: expected identifier, found: " << res.token << std::endl;
            }

            std::cout << "Warning: Couldnt find the type definition! Will look more into the file " << res.ident.i_name << " instead! (" << res.token << ")" << std::endl;

            types_map.insert({ res.ident.var_type.name, std::make_shared<Puxer::PuxerCustomType>(
                res.ident.i_name, Puxer::PuxerUnknown, 0) });
        }
        else {
            return types_map[res.ident.var_type.name];
        }
    }

    else {
        return builtin_types[res.ident.var_type.name];
    }

}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::handle_number(Puxer::PuxerTokenResponse& res) {

    std::unique_ptr<ExprAST> result;
    std::cout << "Number found!" << std::endl;

    switch (res.ident.var_type.type) {

    case Puxer::PuxerI32:
        std::cout << "Type: i32" << std::endl;
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

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::parse_valdef(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    res = pux.get_token();

    if (res.token != Puxer::t_identifier) {
        std::cout << "Error: Expected identifier, found token: " << res.token << std::endl;
        return nullptr;
    }

    std::string ident_name = res.ident.i_name; 
    res = pux.get_token();


    if (res.token == ';') {
        return declare_var({ident_name, Puxer::PuxerUnknown, nullptr});
    }

    if (res.token == ':') {

        res = pux.get_token();
        auto type = handle_type(res);

        if (!type) {
            std::cout << "Error: expected identifier, found token: " << res.token << std::endl;
            return nullptr;
        }

        return declare_var({ident_name, type.get()->type, type});
    }

    return declare_var({ident_name, Puxer::PuxerUnknown, nullptr});
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

    res = pux.get_token();

    if (res.token != '=') {
        std::cout << "Error: expected '=' found " << (char)res.token << " instead!" << std::endl;
        return nullptr;
    }

    res = pux.get_token();

    if (res.token != '>') {
        std::cout << "Error: expected '>' found " << (char)res.token << " instead!" << std::endl;
        return nullptr;
    }

    //type
    res = pux.get_token();

    if (res.token != Puxer::t_identifier) {
        std::cout << "expected return type, found: " << res.token << std::endl;;
        return nullptr;
    }

    std::shared_ptr<Puxer::PuxerCustomType> type = handle_type(res);

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

    auto body = std::make_unique<ExpressionBodyAST>();
    parse_curly(pux, res, body->body);

    if (!body->body.empty()) {
        abs_tree.push_back(std::move(body));
        return std::make_unique<FunctionAST>(std::move(prototype), std::move(body));
    }

    return nullptr;
}

std::unique_ptr<Pukser::FunctionAST> Pukser::Pukser::parse_toplevel(
    Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res) {

    if (auto E = parse_expression(pux, res)) {

        auto body = std::make_unique<ExpressionBodyAST>();
        body->body.push_back(std::move(E));

        auto prototype = std::make_unique<PrototypeAST>("", std::vector<std::string>(), handle_type(res));
        return std::move(std::make_unique<FunctionAST>(std::move(prototype), std::move(body)));
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
        std::cout << "Binarty OP: " << (char)res.token << std::endl;
        res = pux.get_token();

        //get right hand side of the pair
        auto RHS = parse_primary(pux, res);

        if (!RHS)
            return nullptr;

        //look ahead to see if there's another token with higher precedence than the current one 
        //example a + b could be a + b * c
        int next_prec = handle_token_precedence(res);

        if (tok_prec < next_prec) {
            std::cout << "OKAY DIDNT EXPECT THIS TO HAPPEN FRFR :(" << (char)res.token << std::endl;
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

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::handle_SCE(char c) {
    std::unique_ptr<ExprAST> ret = std::make_unique<SingleCharExpr>(c);
    return std::move(ret);
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
            return parse_valdef(pux, res);
            break;

        case '(':
            return parse_paren(pux, res = pux.get_token());
            break;

        case '-':
            return handle_SCE('-');
            break;

        case ';':
            res = pux.get_token();
            return nullptr;
            break;

        case '*':
            return nullptr;
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
        return;

    create_scope();

    while (res.token != '}') {

        std::cout << "Parsing Expression" << std::endl;
        auto expr = parse_expression(pux, res = pux.get_token());

        if (expr) {
            body.push_back(std::move(expr));
            std::cout << "Hm goofy " << res.ident.i_name << " " << res.token << std::endl;
        }
    }

    drop_cur_scope();
    std::cout << "End of function !! " << std::endl;
    res = pux.get_token();
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

    res = pux.get_token();

    //variable ref otherwise a func call
    if (res.token != '(') {

        if (res.token == '.') {
            //IMPLEMENT SOON AS A FEATURE (KINDA BASIC ONE)
        }

        if (res.token == '{') {
            //Initializer list, to be implemented ...
        }
        //otherwise some binary operation
        int valid = handle_token_precedence(res);
        return parse_binary_ops_rhs(pux, res, valid, std::move(fetch_var(name_ref))); 
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

            res = pux.get_token();

            if (res.token == ')')
                break;

            if (res.token != ',')
                return log_error("Expected ')' or ',' in argument list");
        }
    }

    return std::make_unique<CallExprAST>(name_ref, std::move(args));
}
