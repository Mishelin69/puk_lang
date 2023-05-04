#pragma once
#include "puxer_helper_defs.h"

namespace Puxer {

	struct PuxerTokenResponse {

		int token;
		PuxerIdentifier ident;

        PuxerTokenResponse() = default;
        PuxerTokenResponse(PuxerTokenResponse& other) = default;
        ~PuxerTokenResponse() = default;

	};

}
