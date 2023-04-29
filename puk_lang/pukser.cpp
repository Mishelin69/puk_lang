#include "pukser.hpp"
#include "Puxer.h"
#include "pukser_helper_func.h"

void Pukser::Pukser::log_error(const char* s) {
	fprintf(stderr, "Error: %s", s);
}

void Pukser::Pukser::parse(const char* fn) {

	Puxer::Puxer pux;

	auto res = pux.open_file(fn);

	if (res != 0) {
		std::cout << "Opening file: " << fn << " failed!";
	}

	auto token = pux.get_token();

	while (token.token != Puxer::t_eof) {

		token = pux.get_token();

		switch (token.token) {

		case Puxer::t_number:
			handle_number(token);
			break;

		default:
			break;
		}
	}

}

std::unique_ptr<Pukser::ExprAST> Pukser::Pukser::handle_number(Puxer::PuxerTokenResponse& res) {

	std::unique_ptr<ExprAST> result;

	switch (res.ident.var_type.type) {

	case Puxer::PuxerI32:
		result = std::make_unique<I32ExprAST>(str_toi32(res.ident.value));
		break;

	case Puxer::PuxerI64:
		result = std::make_unique<I64ExprAST>(str_toi64(res.ident.value));
		break;

	}


	return std::move(result);
}