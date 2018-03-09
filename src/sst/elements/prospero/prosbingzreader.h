// Copyright 2009-2017 Sandia Corporation. Under the terms
// of Contract DE-NA0003525 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2017, Sandia Corporation
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#ifndef _H_SST_PROSPERO_GZ_BINARY_READER
#define _H_SST_PROSPERO_GZ_BINARY_READER

#include "prosreader.h"
#include "zlib.h"

namespace SST {
namespace Prospero {

class ProsperoCompressedBinaryTraceReader : public ProsperoTraceReader {

public:
        ProsperoCompressedBinaryTraceReader( Component* owner, Params& params );
        ~ProsperoCompressedBinaryTraceReader();
        ProsperoTraceEntry* readNextEntry();

	SST_ELI_REGISTER_SUBCOMPONENT(
               	ProsperoCompressedBinaryTraceReader,
               	"prospero",
               	"ProsperoBCompressedinaryTraceReader",
               	SST_ELI_ELEMENT_VERSION(1,0,0),
               	"Compressed Binary Trace Reader",
	       	"SST::Prospero::ProsperoTraceReader"
	)

       	SST_ELI_DOCUMENT_PARAMS(
               	{ "file", "Sets the file for the trace reader to use", "" }
       	)

private:
	void copy(char* target, const char* source, const size_t buffOffset, const size_t len);
	gzFile traceInput;
	char* buffer;
	uint32_t recordLength;

};

}
}

#endif
