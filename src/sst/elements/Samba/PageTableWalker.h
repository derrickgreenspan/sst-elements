// Copyright 2009-2018 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2018, NTESS
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.
//

/* Author: Amro Awad
 * E-mail: aawad@sandia.gov
 */


#ifndef _H_SST_PTW
#define _H_SST_PTW

#include <sst_config.h>
#include <sst/core/component.h>
#include <sst/core/timeConverter.h>
#include <sst/elements/memHierarchy/memEvent.h>
#include <sst/core/interfaces/simpleMem.h>
#include <sst/core/link.h>
#include <sst/core/event.h>
#include<map>
#include<vector>
#include <sst/core/sst_types.h>
// This file defines the page table walker and 

typedef std::pair<uint64_t, int> id_type;

using namespace SST::Interfaces;
using namespace SST;

namespace SST { namespace SambaComponent{

	class PageTableWalker
	{


		int fault_level; // indicates the step where the page fault handler is at

		int sizes; // This indicates the number of sizes supported

		long long int * page_size; // By default, lets assume 4KB pages

		int * assoc; // This represents the associativiety

		long long int *** tags; // This will hold the tags

		int *** lru; // This will hold the lru positions

		SST::Link * to_mem; // This links the Page table walker to the memory hierarchy


		SST::Link * to_opal; // This links the Page table walker to the opal memory manager

		int * hold; // This is used to tell the TLB hierarchy to stall, to emulate overhead of page fault handler or TLB shootdown


		// ------------- Note that we assume that for each Samba componenet instance, all units run the same VMA, thus all share the same page table
		// ------------- Our assumption is based on the fact that Ariel instances (mapped one-to-one to Samba instances) can only run one application

		// Holds CR3 value of current context
		long long int *CR3;

		// Holds the PGD physical pointers, the key is the 9 bits 39-47, i.e., VA/(4096*512*512*512)
		std::map<long long int, long long int> * PGD;

		// Holds the PUD physical pointers, the key is the 9 bits 30-38, i.e., VA/(4096*512*512)
		std::map<long long int, long long int> * PUD;

		// Holds the PMD physical pointers, the key is the 9 bits 21-29, i.e., VA/(4096*512)
		std::map<long long int, long long int> * PMD;

		// Holds the PTE physical pointers, the key is the 9 bits 12-20, i.e., VA/(4096)
		std::map<long long int, long long int> * PTE; // This should give you the exact physical address of the page


		// The structures below are used to quickly check if the page is mapped or not
		std::map<long long int,int> * MAPPED_PAGE_SIZE4KB;
		std::map<long long int,int> * MAPPED_PAGE_SIZE2MB;
		std::map<long long int,int> * MAPPED_PAGE_SIZE1GB;




		// This link is used to send internal events within the page table walker
		SST::Link * s_EventChan;



		bool stall; // This indicates the page table walker is stalling due to a fault

		int  * sets; //stores the number of sets

		int* size; // Number of entries

		int hits; // number of hits

		int misses; // number of misses

		int os_page_size; // This is a hack for the size of the frames returned by the OS, by default

		int latency; // indicates the latency in cycles

		int emulate_faults; // if set, the page faults will be communicated to Opal

		int upper_link_latency; // This indicates the upper link latency

		int max_outstanding; // indicates the number of maximum outstanding misses

		int max_width; // indicates the number of maximum accesses on the same cycle

		int parallel_mode; // very specific case for L1 PageTableWalker in case of overlapping with accessing the cache

		std::vector<SST::Event *> * service_back; // This is used to pass ready requests back to the previous level

		std::map<SST::Event *, long long int> * service_back_size; // This is used to pass the size of the  requests back to the previous level

		std::map<SST::Event *, SST::Cycle_t > ready_by; // this one is used to keep track of requests that are delayed inside this structure, compensating for latency

		std::map<SST::Event *, long long int> ready_by_size; // this one is used to keep track of requests' sizes inside this structure

		std::vector<SST::Event *> pushed_back; // This is what we got returned from other structures

		std::map<SST::Event *, long long int> pushed_back_size; // This is the sizes of the translations we got returned from other structures

		std::vector<SST::Event *> pending_misses; // This the number of pending misses, only erased when pushed back from next level

		std::vector<SST::Event *> not_serviced; // This holds those accesses not serviced yet


		int self_connected; // his parameter indidicates if the PTW is self-connected or actually connected to the memory hierarchy

		int page_walk_latency; // this is really nothing than the page walk latency in case of having no walkers

		SST::Cycle_t currTime;

		SST::Component * Owner;

		uint64_t line_size; // For setting base address of MemEvents

		public: 

		PageTableWalker() {} // For serialization
		PageTableWalker(int page_size, int assoc, PageTableWalker * next_level, int size);
		PageTableWalker(int tlb_id, PageTableWalker * Next_level,int level, SST::Component * owner, SST::Params& params);

		void setPageTablePointers( long long int * cr3, std::map<long long int, long long int> * pgd,  std::map<long long int, long long int> * pud,  std::map<long long int, long long int> * pmd,  std::map<long long int, long long int> * pte,  std::map<long long int,int> * gb,  std::map<long long int,int> * mb,  std::map<long long int,int> * kb)
		{
			CR3 = cr3;
			PGD = pgd;
			PUD = pud;
			PMD = pmd;
			PTE = pte;
			MAPPED_PAGE_SIZE4KB = kb;
			MAPPED_PAGE_SIZE2MB = mb;
			MAPPED_PAGE_SIZE1GB = gb;
		}

		// Does the translation and updating the statistics of miss/hit
		long long int translate(long long int vadd);


		void setEventChannel(SST::Link * x) { s_EventChan = x; }

		void finish(){}

		void set_ToMem(SST::Link * l) { to_mem = l;} 

		void set_ToOpal(SST::Link * l) { to_opal = l;} 

		void setLineSize(uint64_t size) { line_size = size; }

		// Find if it exists
		bool check_hit(long long int vadd, int struct_id);

		// To insert the translaiton
		int find_victim_way(long long int vadd, int struct_id);

		void setServiceBack( std::vector<SST::Event *> * x) { service_back = x;}

		void setHold(int * tmp) { hold = tmp; }

		void recvResp(SST::Event* event);

		void recvOpal(SST::Event* event);

		void setServiceBackSize( std::map<SST::Event *, long long int> * x) { service_back_size = x;}

		std::vector<SST::Event *> * getPushedBack(){return & pushed_back;}

		std::map<SST::Event *, long long int> * getPushedBackSize(){return & pushed_back_size;}

		std::map<long long int, int> WSR_COUNT;
		std::map<long long int, bool> WSR_READY;

		std::map<long long int, long long int> WID_Add;

		std::map<long long int, SST::Event *> WID_EV;
		std::map<id_type, long long int> MEM_REQ;

		void update_lru(long long int vaddr, int struct_id);


		Statistic<uint64_t>* statPageTableWalkerHits;

		Statistic<uint64_t>* statPageTableWalkerMisses;

		void handleEvent(SST::Event* event);

		int getHits(){return hits;}
		int getMisses(){return misses;}

		void insert_way(long long int vaddr, int way, int struct_id);

		// This one is to push a request to this structure
		void push_request(SST::Event * x) {not_serviced.push_back(x);}

		bool tick(SST::Cycle_t x);


	};
}}

#endif
