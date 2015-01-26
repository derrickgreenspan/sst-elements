
#include <sst_config.h>
#include "membackend/simpleMemBackend.h"

using namespace SST;
using namespace SST::MemHierarchy;

/*------------------------------- Simple Backend ------------------------------- */
SimpleMemory::SimpleMemory(Component *comp, Params &params) : MemBackend(comp, params){
    std::string access_time = params.find_string("access_time", "100 ns");
    self_link = ctrl->configureSelfLink("Self", access_time,
            new Event::Handler<SimpleMemory>(this, &SimpleMemory::handleSelfEvent));
}

void SimpleMemory::handleSelfEvent(SST::Event *event){
    MemCtrlEvent *ev = static_cast<MemCtrlEvent*>(event);
    MemController::DRAMReq *req = ev->req;
    ctrl->handleMemResponse(req);
    delete event;
}

bool SimpleMemory::issueRequest(MemController::DRAMReq *req){
    uint64_t addr = req->baseAddr_ + req->amtInProcess_;
    ctrl->dbg.debug(_L10_, "Issued transaction for address %" PRIx64 "\n", (Addr)addr);
    self_link->send(1, new MemCtrlEvent(req));
    return true;
}
