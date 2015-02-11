// Copyright 2009-2014 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2014, Sandia Corporation
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#ifndef _H_EMBER_TRAFFIC_GEN
#define _H_EMBER_TRAFFIC_GEN

#include "mpi/embermpigen.h"

namespace SST {
namespace Ember {

class EmberTrafficGenGenerator : public EmberMessagePassingGenerator {

public:
	EmberTrafficGenGenerator(SST::Component* owner, Params& params);
    bool generate( std::queue<EmberEvent*>& evQ);
    bool primary( ) {
        return false;
    }

private:
    MessageResponse m_resp;
    void*    m_sendBuf;
    void*    m_recvBuf;

	uint32_t m_messageSize;
    uint32_t m_computeTime;
};

}
}

#endif