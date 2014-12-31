#include <r3x_dispatcher.h>
#include <nt_malloc.h>
r3x_global_domain_t* r3x_init_domain(void) {
	r3x_global_domain_t* Domain = NULL; 
	Domain = nt_malloc(sizeof(r3x_global_domain_t));
	Domain->Jobs = nt_malloc(sizeof(r3x_job_t*)*16);
	Domain->CurrentJobID = 0;
	Domain->NumberOfActiveJobs = 0;
	Domain->TotalNumberOfJobs = 16;
	return Domain;
}
unsigned int r3x_dispatch_job(int InstructionPointer, int CycleUpdate, r3x_global_domain_t* Domain, bool ismain) {
	if(Domain->NumberOfActiveJobs >= Domain->TotalNumberOfJobs-1) { 
		Domain->Jobs = nt_realloc(Domain->Jobs, sizeof(r3x_job_t*) * (Domain->TotalNumberOfJobs + 16));
		Domain->TotalNumberOfJobs += 16;
	}
	for(unsigned int i = 0; i < Domain->TotalNumberOfJobs; i++) {
		if(Domain->Jobs[i] == NULL) {	
			Domain->Jobs[i] = nt_malloc(sizeof(r3x_job_t));
			Domain->Jobs[i]->Stack = Stack.Create();
			Domain->Jobs[i]->CallStack = Stack.Create();
			Stack.Push(Domain->Jobs[i]->Stack, 0);
			Domain->Jobs[i]->EqualFlag = false;
			Domain->Jobs[i]->GreaterFlag = false;
			Domain->Jobs[i]->LesserFlag = false;
			Domain->Jobs[i]->ZeroFlag = false;
			Domain->Jobs[i]->CycleUpdate = CycleUpdate;
			Domain->Jobs[i]->InstructionPointer = InstructionPointer;
			Domain->Jobs[i]->ismain = ismain;		
			Domain->Jobs[i]->JobID = i;
			Domain->Jobs[i]->paused = false;
			Domain->NumberOfActiveJobs++;
			return i;
		}	
	}
	printf("DISPATCHER: Unable to find free pointer. Please report this bug!\n");
	exit(0);
}
int r3x_load_job_state(r3x_cpu_t* CPU, r3x_global_domain_t* Domain, unsigned int JobID) {
	if(Domain->NumberOfActiveJobs == 0) { 
		return -1;
	}
	if((unsigned int)JobID >= Domain->TotalNumberOfJobs) {
		return -1;
	}
	if(Domain->Jobs[JobID] != NULL) { 
		// Load Job State
		CPU->InstructionPointer = Domain->Jobs[JobID]->InstructionPointer;
		CPU->Stack = Domain->Jobs[JobID]->Stack;
		CPU->CallStack = Domain->Jobs[JobID]->CallStack;
		CPU->EqualFlag = Domain->Jobs[JobID]->EqualFlag;
		CPU->ZeroFlag = Domain->Jobs[JobID]->ZeroFlag;
		CPU->LesserFlag = Domain->Jobs[JobID]->LesserFlag;
		CPU->GreaterFlag = Domain->Jobs[JobID]->GreaterFlag;
		for(unsigned int i = 0; i <= MAX_NUMBER_OF_REGISTERS; i++) {
			CPU->Regs[i] = Domain->Jobs[JobID]->Regs[i];
		}
		return 0;
	}
	return -1;
}
void r3x_save_job_state(r3x_cpu_t* CPU, r3x_global_domain_t* Domain, unsigned int JobID) { 
	if((unsigned int)JobID >= Domain->TotalNumberOfJobs) { 
		return;	
	}
	if(Domain->Jobs[JobID] != NULL) {
		Domain->Jobs[JobID]->InstructionPointer = CPU->InstructionPointer;
		Domain->Jobs[JobID]->Stack = CPU->Stack;
		Domain->Jobs[JobID]->CallStack = CPU->CallStack;
		Domain->Jobs[JobID]->EqualFlag = CPU->EqualFlag;
		Domain->Jobs[JobID]->ZeroFlag = CPU->ZeroFlag;
		Domain->Jobs[JobID]->LesserFlag = CPU->LesserFlag;
		CPU->GreaterFlag = Domain->Jobs[JobID]->GreaterFlag;
		for(int i = 0; i <= MAX_NUMBER_OF_REGISTERS; i++) {
			Domain->Jobs[JobID]->Regs[i] = CPU->Regs[i];		
		}
	}
}
void r3x_exit_job(r3x_global_domain_t* Domain, int JobID) {
	if((unsigned int)JobID > Domain->TotalNumberOfJobs){ 
		return;
	}
	if(Domain->Jobs[JobID] != NULL) { 
		Stack.DestroyStack(Domain->Jobs[JobID]->Stack);
		Stack.DestroyStack(Domain->Jobs[JobID]->CallStack); 
		nt_free(Domain->Jobs[JobID]);
		Domain->Jobs[JobID] = NULL;
		Domain->NumberOfActiveJobs--;
	}
}
