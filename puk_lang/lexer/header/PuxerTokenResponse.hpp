#pragma once
#include "../../helper/header/PuxerHelperDef.hpp"

namespace Puxer {

	struct PuxerTokenResponse {

		int token;
		PuxerIdentifier ident;

        PuxerTokenResponse() = default;
        PuxerTokenResponse(PuxerTokenResponse& other) = default;
        ~PuxerTokenResponse() = default;

	};

}
